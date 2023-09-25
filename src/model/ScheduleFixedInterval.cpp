/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "ScheduleFixedInterval.hpp"
#include "ScheduleFixedInterval_Impl.hpp"
#include "Model.hpp"
#include "ModelExtensibleGroup.hpp"

#include "ScheduleTypeLimits.hpp"
#include "ScheduleTypeLimits_Impl.hpp"
#include "YearDescription.hpp"
#include "YearDescription_Impl.hpp"

#include <utilities/idd/OS_Schedule_FixedInterval_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include "../utilities/time/DateTime.hpp"
#include "../utilities/data/TimeSeries.hpp"
#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    ScheduleFixedInterval_Impl::ScheduleFixedInterval_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
      : ScheduleInterval_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == ScheduleFixedInterval::iddObjectType());
    }

    ScheduleFixedInterval_Impl::ScheduleFixedInterval_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle)
      : ScheduleInterval_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == ScheduleFixedInterval::iddObjectType());
    }

    ScheduleFixedInterval_Impl::ScheduleFixedInterval_Impl(const ScheduleFixedInterval_Impl& other, Model_Impl* model, bool keepHandle)
      : ScheduleInterval_Impl(other, model, keepHandle) {}

    IddObjectType ScheduleFixedInterval_Impl::iddObjectType() const {
      return ScheduleFixedInterval::iddObjectType();
    }

    boost::optional<ScheduleTypeLimits> ScheduleFixedInterval_Impl::scheduleTypeLimits() const {
      return getObject<ModelObject>().getModelObjectTarget<ScheduleTypeLimits>(OS_Schedule_FixedIntervalFields::ScheduleTypeLimitsName);
    }

    bool ScheduleFixedInterval_Impl::setScheduleTypeLimits(const ScheduleTypeLimits& scheduleTypeLimits) {
      if (scheduleTypeLimits.model() != model()) {
        return false;
      }
      if (!candidateIsCompatibleWithCurrentUse(scheduleTypeLimits)) {
        return false;
      }
      return setPointer(OS_Schedule_FixedIntervalFields::ScheduleTypeLimitsName, scheduleTypeLimits.handle());
    }

    bool ScheduleFixedInterval_Impl::resetScheduleTypeLimits() {
      // can only reset if not used
      if (okToResetScheduleTypeLimits()) {
        return setString(OS_Schedule_FixedIntervalFields::ScheduleTypeLimitsName, "");
      }
      return false;
    }

    openstudio::TimeSeries ScheduleFixedInterval_Impl::timeSeries() const {
      int year =
        2009;  // we need to pass assumed year in here, otherwise it uses YearDescription().assumedYear from Date.cpp which doesn't return the same assumed year (?)
      if (boost::optional<YearDescription> yd = this->model().getOptionalUniqueModelObject<YearDescription>()) {
        year = yd->assumedYear();
      }

      const Date startDate(openstudio::MonthOfYear(this->startMonth()), this->startDay(), year);
      const Time intervalLength(0, 0, this->intervalLength());

      Vector values(this->numExtensibleGroups());
      unsigned i = 0;
      for (const ModelExtensibleGroup& group : castVector<ModelExtensibleGroup>(extensibleGroups())) {
        OptionalDouble x = group.getDouble(0);
        OS_ASSERT(x);
        values[i] = *x;
        ++i;
      }

      TimeSeries result(startDate, intervalLength, values, "");
      result.setOutOfRangeValue(this->outOfRangeValue());

      return result;
    }

    bool ScheduleFixedInterval_Impl::setTimeSeries(const openstudio::TimeSeries& timeSeries) {
      // check the interval
      boost::optional<openstudio::Time> intervalTime = timeSeries.intervalLength();
      if (!intervalTime) {
        return false;
      }

      auto intervalLengthAsInteger = [](const double value) -> int {
        double integralPart = 0.0;
        if (std::modf(value, &integralPart) == 0.0) {
          // The intervalLength is actually an int, not a double
          return static_cast<int>(integralPart);
        }
        return -1;
      };

      // check the interval
      const double intervalLengthDouble = intervalTime->totalMinutes();
      const int intervalLength = intervalLengthAsInteger(intervalLengthDouble);
      if (intervalLength < 0) {
        return false;
      }

      // check that first report is whole number of intervals from start date
      const DateTime firstReportDateTime = timeSeries.firstReportDateTime();
      const DateTime startDateTime = timeSeries.startDateTime();
      const Date startDate = startDateTime.date();

      const Time firstReportTime = firstReportDateTime.time();

      const double numIntervalsToFirstReportDouble = std::max(1.0, firstReportTime.totalMinutes() / intervalLengthDouble);
      const int numIntervalsToFirstReport = intervalLengthAsInteger(numIntervalsToFirstReportDouble);
      if (numIntervalsToFirstReport < 0) {
        return false;
      }

      // check the values
      const openstudio::Vector values = timeSeries.values();
      for (size_t pos = 0; const auto& value : values) {
        // Check validity, cannot be NaN, Inf, etc
        if (std::isinf(value)) {
          LOG(Warn, "There is Infinity on position " << pos << " in the timeSeries provided for " << this->briefDescription());
          return false;
        } else if (std::isnan(value)) {
          LOG(Warn, "There is a NaN on position " << pos << " in the timeSeries provided for " << this->briefDescription());
          return false;
        }
        ++pos;
      }

      // at this point we are going to change the object
      clearExtensibleGroups(false);

      // set the interval
      this->setIntervalLength(intervalLength, false);

      // set the start date
      this->setStartMonth(startDate.monthOfYear().value(), false);
      this->setStartDay(startDate.dayOfMonth(), false);

      // set the out of range value
      const double outOfRangeValue = timeSeries.outOfRangeValue();

      // add in numIntervalsToFirstReport-1 outOfRangeValues to pad the timeseries
      for (int i = 0; i < numIntervalsToFirstReport - 1; ++i) {
        std::vector<std::string> temp;
        temp.push_back(toString(outOfRangeValue));

        const auto group = pushExtensibleGroup(temp, false).cast<ModelExtensibleGroup>();
        OS_ASSERT(!group.empty());
      }

      // set the values
      for (const double value : values) {
        std::vector<std::string> temp;
        temp.push_back(toString(value));

        const auto group = pushExtensibleGroup(temp, false).cast<ModelExtensibleGroup>();
        OS_ASSERT(!group.empty());
      }

      this->emitChangeSignals();

      return true;
    }

    bool ScheduleFixedInterval_Impl::interpolatetoTimestep() const {
      boost::optional<std::string> value = getString(OS_Schedule_FixedIntervalFields::InterpolatetoTimestep, true);
      OS_ASSERT(value);
      return openstudio::istringEqual(value.get(), "Yes");
    }

    bool ScheduleFixedInterval_Impl::isInterpolatetoTimestepDefaulted() const {
      return isEmpty(OS_Schedule_FixedIntervalFields::InterpolatetoTimestep);
    }

    bool ScheduleFixedInterval_Impl::translatetoScheduleFile() const {
      boost::optional<std::string> value = getString(OS_Schedule_FixedIntervalFields::TranslatetoScheduleFile, true);
      OS_ASSERT(value);
      return openstudio::istringEqual(value.get(), "Yes");
    }

    bool ScheduleFixedInterval_Impl::isTranslatetoScheduleFileDefaulted() const {
      return isEmpty(OS_Schedule_FixedIntervalFields::TranslatetoScheduleFile);
    }

    int ScheduleFixedInterval_Impl::startMonth() const {
      boost::optional<int> value = getInt(OS_Schedule_FixedIntervalFields::StartMonth, true);
      OS_ASSERT(value);
      return value.get();
    }

    int ScheduleFixedInterval_Impl::startDay() const {
      boost::optional<int> value = getInt(OS_Schedule_FixedIntervalFields::StartDay, true);
      OS_ASSERT(value);
      return value.get();
    }

    int ScheduleFixedInterval_Impl::intervalLength() const {
      boost::optional<int> value = getInt(OS_Schedule_FixedIntervalFields::IntervalLength, true);
      if (value) {
        return value.get();
      }
      return 0;
    }

    double ScheduleFixedInterval_Impl::outOfRangeValue() const {
      boost::optional<double> value = getDouble(OS_Schedule_FixedIntervalFields::OutOfRangeValue, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool ScheduleFixedInterval_Impl::isOutOfRangeValueDefaulted() const {
      return isEmpty(OS_Schedule_FixedIntervalFields::OutOfRangeValue);
    }

    bool ScheduleFixedInterval_Impl::setInterpolatetoTimestep(bool interpolatetoTimestep, bool driverMethod) {
      bool result = false;
      if (interpolatetoTimestep) {
        result = setString(OS_Schedule_FixedIntervalFields::InterpolatetoTimestep, "Yes", driverMethod);
      } else {
        result = setString(OS_Schedule_FixedIntervalFields::InterpolatetoTimestep, "No", driverMethod);
      }
      OS_ASSERT(result);
      return result;
    }

    void ScheduleFixedInterval_Impl::resetInterpolatetoTimestep(bool driverMethod) {
      const bool result = setString(OS_Schedule_FixedIntervalFields::InterpolatetoTimestep, "", driverMethod);
      OS_ASSERT(result);
    }

    bool ScheduleFixedInterval_Impl::setTranslatetoScheduleFile(bool translatetoScheduleFile, bool driverMethod) {
      bool result = false;
      if (translatetoScheduleFile) {
        result = setString(OS_Schedule_FixedIntervalFields::TranslatetoScheduleFile, "Yes", driverMethod);
      } else {
        result = setString(OS_Schedule_FixedIntervalFields::TranslatetoScheduleFile, "No", driverMethod);
      }
      OS_ASSERT(result);
      return result;
    }

    void ScheduleFixedInterval_Impl::resetTranslatetoScheduleFile(bool driverMethod) {
      const bool result = setString(OS_Schedule_FixedIntervalFields::TranslatetoScheduleFile, "", driverMethod);
      OS_ASSERT(result);
    }

    bool ScheduleFixedInterval_Impl::setStartMonth(int startMonth, bool driverMethod) {
      const bool result = setInt(OS_Schedule_FixedIntervalFields::StartMonth, startMonth, driverMethod);
      return result;
    }

    bool ScheduleFixedInterval_Impl::setStartDay(int startDay, bool driverMethod) {
      const bool result = setInt(OS_Schedule_FixedIntervalFields::StartDay, startDay, driverMethod);
      return result;
    }

    bool ScheduleFixedInterval_Impl::setIntervalLength(int intervalLength, bool driverMethod) {
      const bool result = setDouble(OS_Schedule_FixedIntervalFields::IntervalLength, intervalLength, driverMethod);
      return result;
    }

    bool ScheduleFixedInterval_Impl::setOutOfRangeValue(double outOfRangeValue, bool driverMethod) {
      const bool result = setDouble(OS_Schedule_FixedIntervalFields::OutOfRangeValue, outOfRangeValue, driverMethod);
      OS_ASSERT(result);
      return result;
    }

    void ScheduleFixedInterval_Impl::resetOutOfRangeValue(bool driverMethod) {
      const bool result = setString(OS_Schedule_FixedIntervalFields::OutOfRangeValue, "", driverMethod);
      OS_ASSERT(result);
    }

    void ScheduleFixedInterval_Impl::ensureNoLeapDays() {
      boost::optional<int> month;
      boost::optional<int> day;

      month = getInt(OS_Schedule_FixedIntervalFields::StartMonth);
      if (month && (month.get() == 2)) {
        day = this->getInt(OS_Schedule_FixedIntervalFields::StartDay);
        if (day && (day.get() == 29)) {
          this->setInt(OS_Schedule_FixedIntervalFields::StartDay, 28);
        }
      }
    }

  }  // namespace detail

  ScheduleFixedInterval::ScheduleFixedInterval(const Model& model) : ScheduleInterval(ScheduleFixedInterval::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::ScheduleFixedInterval_Impl>());
  }

  IddObjectType ScheduleFixedInterval::iddObjectType() {
    IddObjectType result(IddObjectType::OS_Schedule_FixedInterval);
    return result;
  }

  bool ScheduleFixedInterval::interpolatetoTimestep() const {
    return getImpl<detail::ScheduleFixedInterval_Impl>()->interpolatetoTimestep();
  }

  bool ScheduleFixedInterval::isInterpolatetoTimestepDefaulted() const {
    return getImpl<detail::ScheduleFixedInterval_Impl>()->isInterpolatetoTimestepDefaulted();
  }

  bool ScheduleFixedInterval::translatetoScheduleFile() const {
    return getImpl<detail::ScheduleFixedInterval_Impl>()->translatetoScheduleFile();
  }

  bool ScheduleFixedInterval::isTranslatetoScheduleFileDefaulted() const {
    return getImpl<detail::ScheduleFixedInterval_Impl>()->isTranslatetoScheduleFileDefaulted();
  }

  int ScheduleFixedInterval::startMonth() const {
    return getImpl<detail::ScheduleFixedInterval_Impl>()->startMonth();
  }

  int ScheduleFixedInterval::startDay() const {
    return getImpl<detail::ScheduleFixedInterval_Impl>()->startDay();
  }

  int ScheduleFixedInterval::intervalLength() const {
    return getImpl<detail::ScheduleFixedInterval_Impl>()->intervalLength();
  }

  double ScheduleFixedInterval::outOfRangeValue() const {
    return getImpl<detail::ScheduleFixedInterval_Impl>()->outOfRangeValue();
  }

  bool ScheduleFixedInterval::isOutOfRangeValueDefaulted() const {
    return getImpl<detail::ScheduleFixedInterval_Impl>()->isOutOfRangeValueDefaulted();
  }

  bool ScheduleFixedInterval::setInterpolatetoTimestep(bool interpolatetoTimestep) {
    return getImpl<detail::ScheduleFixedInterval_Impl>()->setInterpolatetoTimestep(interpolatetoTimestep);
  }

  void ScheduleFixedInterval::resetInterpolatetoTimestep() {
    getImpl<detail::ScheduleFixedInterval_Impl>()->resetInterpolatetoTimestep();
  }

  bool ScheduleFixedInterval::setTranslatetoScheduleFile(bool translatetoScheduleFile) {
    return getImpl<detail::ScheduleFixedInterval_Impl>()->setTranslatetoScheduleFile(translatetoScheduleFile);
  }

  void ScheduleFixedInterval::resetTranslatetoScheduleFile() {
    getImpl<detail::ScheduleFixedInterval_Impl>()->resetTranslatetoScheduleFile();
  }

  bool ScheduleFixedInterval::setStartMonth(int startMonth) {
    return getImpl<detail::ScheduleFixedInterval_Impl>()->setStartMonth(startMonth);
  }

  bool ScheduleFixedInterval::setStartDay(int startDay) {
    return getImpl<detail::ScheduleFixedInterval_Impl>()->setStartDay(startDay);
  }

  bool ScheduleFixedInterval::setIntervalLength(int intervalLength) {
    return getImpl<detail::ScheduleFixedInterval_Impl>()->setIntervalLength(intervalLength);
  }

  bool ScheduleFixedInterval::setOutOfRangeValue(double outOfRangeValue) {
    return getImpl<detail::ScheduleFixedInterval_Impl>()->setOutOfRangeValue(outOfRangeValue);
  }

  void ScheduleFixedInterval::resetOutOfRangeValue() {
    getImpl<detail::ScheduleFixedInterval_Impl>()->resetOutOfRangeValue();
  }

  /// @cond
  ScheduleFixedInterval::ScheduleFixedInterval(std::shared_ptr<detail::ScheduleFixedInterval_Impl> impl) : ScheduleInterval(std::move(impl)) {}
  /// @endcond

}  // namespace model
}  // namespace openstudio
