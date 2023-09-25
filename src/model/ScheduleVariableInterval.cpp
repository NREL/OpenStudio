/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "ScheduleVariableInterval.hpp"
#include "ScheduleVariableInterval_Impl.hpp"
#include "Model.hpp"
#include "ModelExtensibleGroup.hpp"

#include "ScheduleTypeLimits.hpp"
#include "ScheduleTypeLimits_Impl.hpp"

#include <utilities/idd/OS_Schedule_VariableInterval_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include "../utilities/time/DateTime.hpp"
#include "../utilities/data/TimeSeries.hpp"
#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    ScheduleVariableInterval_Impl::ScheduleVariableInterval_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
      : ScheduleInterval_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == ScheduleVariableInterval::iddObjectType());
    }

    ScheduleVariableInterval_Impl::ScheduleVariableInterval_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model,
                                                                 bool keepHandle)
      : ScheduleInterval_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == ScheduleVariableInterval::iddObjectType());
    }

    ScheduleVariableInterval_Impl::ScheduleVariableInterval_Impl(const ScheduleVariableInterval_Impl& other, Model_Impl* model, bool keepHandle)
      : ScheduleInterval_Impl(other, model, keepHandle) {}

    IddObjectType ScheduleVariableInterval_Impl::iddObjectType() const {
      return ScheduleVariableInterval::iddObjectType();
    }

    boost::optional<ScheduleTypeLimits> ScheduleVariableInterval_Impl::scheduleTypeLimits() const {
      return getObject<ModelObject>().getModelObjectTarget<ScheduleTypeLimits>(OS_Schedule_VariableIntervalFields::ScheduleTypeLimitsName);
    }

    bool ScheduleVariableInterval_Impl::setScheduleTypeLimits(const ScheduleTypeLimits& scheduleTypeLimits) {
      if (scheduleTypeLimits.model() != model()) {
        return false;
      }
      return setPointer(OS_Schedule_VariableIntervalFields::ScheduleTypeLimitsName, scheduleTypeLimits.handle());
    }

    bool ScheduleVariableInterval_Impl::resetScheduleTypeLimits() {
      // can only reset if not used
      if (okToResetScheduleTypeLimits()) {
        return setString(OS_Schedule_VariableIntervalFields::ScheduleTypeLimitsName, "");
      }
      return false;
    }

    bool ScheduleVariableInterval_Impl::interpolatetoTimestep() const {
      boost::optional<std::string> value = getString(OS_Schedule_VariableIntervalFields::InterpolatetoTimestep, true);
      OS_ASSERT(value);
      return openstudio::istringEqual(value.get(), "Yes");
    }

    bool ScheduleVariableInterval_Impl::isInterpolatetoTimestepDefaulted() const {
      return isEmpty(OS_Schedule_VariableIntervalFields::InterpolatetoTimestep);
    }

    int ScheduleVariableInterval_Impl::startMonth() const {
      boost::optional<int> value = getInt(OS_Schedule_VariableIntervalFields::StartMonth, true);
      OS_ASSERT(value);
      return value.get();
    }

    int ScheduleVariableInterval_Impl::startDay() const {
      boost::optional<int> value = getInt(OS_Schedule_VariableIntervalFields::StartDay, true);
      OS_ASSERT(value);
      return value.get();
    }

    double ScheduleVariableInterval_Impl::outOfRangeValue() const {
      boost::optional<double> value = getDouble(OS_Schedule_VariableIntervalFields::OutOfRangeValue, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool ScheduleVariableInterval_Impl::isOutOfRangeValueDefaulted() const {
      return isEmpty(OS_Schedule_VariableIntervalFields::OutOfRangeValue);
    }

    bool ScheduleVariableInterval_Impl::setInterpolatetoTimestep(bool interpolatetoTimestep, bool driverMethod) {
      bool result = false;
      if (interpolatetoTimestep) {
        result = setString(OS_Schedule_VariableIntervalFields::InterpolatetoTimestep, "Yes", driverMethod);
      } else {
        result = setString(OS_Schedule_VariableIntervalFields::InterpolatetoTimestep, "No", driverMethod);
      }
      OS_ASSERT(result);
      return result;
    }

    void ScheduleVariableInterval_Impl::resetInterpolatetoTimestep(bool driverMethod) {
      bool result = setString(OS_Schedule_VariableIntervalFields::InterpolatetoTimestep, "", driverMethod);
      OS_ASSERT(result);
    }

    bool ScheduleVariableInterval_Impl::setStartMonth(int startMonth, bool driverMethod) {
      bool result = setInt(OS_Schedule_VariableIntervalFields::StartMonth, startMonth, driverMethod);
      return result;
    }

    bool ScheduleVariableInterval_Impl::setStartDay(int startDay, bool driverMethod) {
      bool result = setInt(OS_Schedule_VariableIntervalFields::StartDay, startDay, driverMethod);
      return result;
    }

    bool ScheduleVariableInterval_Impl::setOutOfRangeValue(double outOfRangeValue, bool driverMethod) {
      bool result = setDouble(OS_Schedule_VariableIntervalFields::OutOfRangeValue, outOfRangeValue, driverMethod);
      OS_ASSERT(result);
      return result;
    }

    void ScheduleVariableInterval_Impl::resetOutOfRangeValue(bool driverMethod) {
      bool result = setString(OS_Schedule_VariableIntervalFields::OutOfRangeValue, "", driverMethod);
      OS_ASSERT(result);
    }

    void ScheduleVariableInterval_Impl::ensureNoLeapDays() {
      boost::optional<int> month;
      boost::optional<int> day;

      month = getInt(OS_Schedule_VariableIntervalFields::StartMonth);
      if (month && (month.get() == 2)) {
        day = this->getInt(OS_Schedule_VariableIntervalFields::StartDay);
        if (day && (day.get() == 29)) {
          this->setInt(OS_Schedule_VariableIntervalFields::StartDay, 28);
        }
      }

      for (const IdfExtensibleGroup& group : this->extensibleGroups()) {
        month = group.getInt(OS_Schedule_VariableIntervalExtensibleFields::Month);
        if (month && (month.get() == 2)) {
          day = group.getInt(OS_Schedule_VariableIntervalExtensibleFields::Day);
          if (day && (day.get() == 29)) {
            this->setInt(OS_Schedule_VariableIntervalExtensibleFields::Day, 28);
          }
        }
      }
    }

    openstudio::TimeSeries ScheduleVariableInterval_Impl::timeSeries() const {
      unsigned numExtensibleGroups = this->numExtensibleGroups();
      if (numExtensibleGroups == 0) {
        return TimeSeries(Date(MonthOfYear::Jan, 1), 0, Vector(), "");
      }

      OptionalInt startMonth = this->startMonth();
      OptionalInt startDay = this->startMonth();
      // Grotesquely assume that the schedule starts at the start of the first day
      OptionalInt startHour = 0;
      OptionalInt startMinute = 0;
      OS_ASSERT(startMonth);
      OS_ASSERT(startDay);
      OS_ASSERT(startHour);
      OS_ASSERT(startMinute);

      DateTimeVector dateTimes;
      dateTimes.push_back(DateTime(Date(MonthOfYear(*startMonth), *startDay), Time(0, *startHour, *startMinute)));
      Vector values(numExtensibleGroups);
      unsigned i = 0;
      for (const ModelExtensibleGroup& group : castVector<ModelExtensibleGroup>(extensibleGroups())) {
        OptionalInt month = group.getInt(0);
        OptionalInt day = group.getInt(1);
        OptionalInt hour = group.getInt(2);
        OptionalInt minute = group.getInt(3);
        OptionalDouble x = group.getDouble(4);
        OS_ASSERT(month);
        OS_ASSERT(day);
        OS_ASSERT(hour);
        OS_ASSERT(minute);
        OS_ASSERT(x);
        dateTimes.push_back(DateTime(Date(MonthOfYear(*month), *day), Time(0, *hour, *minute)));
        values[i] = *x;
        ++i;
      }

      TimeSeries result(dateTimes, values, "");
      result.setOutOfRangeValue(this->outOfRangeValue());

      return result;
    }

    bool ScheduleVariableInterval_Impl::setTimeSeries(const openstudio::TimeSeries& timeSeries) {
      // check the values
      openstudio::Vector values = timeSeries.values();
      for (const auto& value : values) {
        // Get the position
        int pos = (int)(&value - &values[0]);
        // Check validity, cannot be NaN, Inf, etc
        if (std::isinf(value)) {
          LOG(Warn, "There is Infinity on position " << pos << " in the timeSeries provided for " << this->briefDescription());
          return false;
        } else if (std::isnan(value)) {
          LOG(Warn, "There is a NaN on position " << pos << " in the timeSeries provided for " << this->briefDescription());
          return false;
        }
      }

      clearExtensibleGroups(false);

      DateTime firstReportDateTime = timeSeries.firstReportDateTime();
      Date startDate = firstReportDateTime.date();

      // set the start date
      this->setStartMonth(startDate.monthOfYear().value(), false);
      this->setStartDay(startDate.dayOfMonth(), false);

      // set the out of range value
      double outOfRangeValue = timeSeries.outOfRangeValue();
      this->setOutOfRangeValue(outOfRangeValue);

      // set the values
      std::vector<long> secondsFromFirstReport = timeSeries.secondsFromFirstReport();
      for (unsigned i = 0; i < values.size(); ++i) {
        DateTime dateTime = firstReportDateTime + Time(0, 0, 0, secondsFromFirstReport[i]);
        Date date = dateTime.date();
        Time time = dateTime.time();

        std::vector<std::string> temp;
        temp.push_back(boost::lexical_cast<std::string>(date.monthOfYear().value()));
        temp.push_back(boost::lexical_cast<std::string>(date.dayOfMonth()));
        temp.push_back(boost::lexical_cast<std::string>(time.hours()));
        temp.push_back(boost::lexical_cast<std::string>(time.minutes()));
        temp.push_back(toString(values[i]));

        auto group = pushExtensibleGroup(temp, false).cast<ModelExtensibleGroup>();
        OS_ASSERT(!group.empty());
      }

      this->emitChangeSignals();

      return true;
    }

  }  // namespace detail

  ScheduleVariableInterval::ScheduleVariableInterval(const Model& model) : ScheduleInterval(ScheduleVariableInterval::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::ScheduleVariableInterval_Impl>());
  }

  IddObjectType ScheduleVariableInterval::iddObjectType() {
    IddObjectType result(IddObjectType::OS_Schedule_VariableInterval);
    return result;
  }

  bool ScheduleVariableInterval::interpolatetoTimestep() const {
    return getImpl<detail::ScheduleVariableInterval_Impl>()->interpolatetoTimestep();
  }

  bool ScheduleVariableInterval::isInterpolatetoTimestepDefaulted() const {
    return getImpl<detail::ScheduleVariableInterval_Impl>()->isInterpolatetoTimestepDefaulted();
  }

  int ScheduleVariableInterval::startMonth() const {
    return getImpl<detail::ScheduleVariableInterval_Impl>()->startMonth();
  }

  int ScheduleVariableInterval::startDay() const {
    return getImpl<detail::ScheduleVariableInterval_Impl>()->startDay();
  }

  double ScheduleVariableInterval::outOfRangeValue() const {
    return getImpl<detail::ScheduleVariableInterval_Impl>()->outOfRangeValue();
  }

  bool ScheduleVariableInterval::isOutOfRangeValueDefaulted() const {
    return getImpl<detail::ScheduleVariableInterval_Impl>()->isOutOfRangeValueDefaulted();
  }

  bool ScheduleVariableInterval::setInterpolatetoTimestep(bool interpolatetoTimestep) {
    return getImpl<detail::ScheduleVariableInterval_Impl>()->setInterpolatetoTimestep(interpolatetoTimestep);
  }

  void ScheduleVariableInterval::resetInterpolatetoTimestep() {
    getImpl<detail::ScheduleVariableInterval_Impl>()->resetInterpolatetoTimestep();
  }

  bool ScheduleVariableInterval::setStartMonth(int startMonth) {
    return getImpl<detail::ScheduleVariableInterval_Impl>()->setStartMonth(startMonth);
  }

  bool ScheduleVariableInterval::setStartDay(int startDay) {
    return getImpl<detail::ScheduleVariableInterval_Impl>()->setStartDay(startDay);
  }

  bool ScheduleVariableInterval::setOutOfRangeValue(double outOfRangeValue) {
    return getImpl<detail::ScheduleVariableInterval_Impl>()->setOutOfRangeValue(outOfRangeValue);
  }

  void ScheduleVariableInterval::resetOutOfRangeValue() {
    getImpl<detail::ScheduleVariableInterval_Impl>()->resetOutOfRangeValue();
  }

  /// @cond
  ScheduleVariableInterval::ScheduleVariableInterval(std::shared_ptr<detail::ScheduleVariableInterval_Impl> impl)
    : ScheduleInterval(std::move(impl)) {}
  /// @endcond

}  // namespace model
}  // namespace openstudio
