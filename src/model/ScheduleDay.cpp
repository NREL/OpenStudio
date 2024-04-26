/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "ScheduleDay.hpp"
#include "ScheduleDay_Impl.hpp"

#include "Model.hpp"
#include "ScheduleTypeRegistry.hpp"
#include "ScheduleTypeLimits.hpp"
#include "ScheduleTypeLimits_Impl.hpp"
#include "Schedule.hpp"
#include "Schedule_Impl.hpp"
#include "ScheduleRuleset.hpp"
#include "ScheduleRuleset_Impl.hpp"
#include "ScheduleRule.hpp"
#include "ScheduleRule_Impl.hpp"
#include "Timestep.hpp"
#include "Timestep_Impl.hpp"

#include "../utilities/idf/IdfExtensibleGroup.hpp"
#include <utilities/idd/OS_Schedule_Day_FieldEnums.hxx>
#include <utilities/idd/OS_Schedule_Rule_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include "../utilities/core/Assert.hpp"

#include "../utilities/time/Time.hpp"
#include "../utilities/data/TimeSeries.hpp"
#include "../utilities/data/Vector.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    ScheduleDay_Impl::ScheduleDay_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
      : ScheduleBase_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == ScheduleDay::iddObjectType());

      // connect signals
      this->ScheduleDay_Impl::onChange.connect<ScheduleDay_Impl, &ScheduleDay_Impl::clearCachedVariables>(this);
    }

    ScheduleDay_Impl::ScheduleDay_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle)
      : ScheduleBase_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == ScheduleDay::iddObjectType());

      // connect signals
      this->ScheduleDay_Impl::onChange.connect<ScheduleDay_Impl, &ScheduleDay_Impl::clearCachedVariables>(this);
    }

    ScheduleDay_Impl::ScheduleDay_Impl(const ScheduleDay_Impl& other, Model_Impl* model, bool keepHandle)
      : ScheduleBase_Impl(other, model, keepHandle) {
      // connect signals
      this->ScheduleDay_Impl::onChange.connect<ScheduleDay_Impl, &ScheduleDay_Impl::clearCachedVariables>(this);
    }

    std::vector<IdfObject> ScheduleDay_Impl::remove() {
      if (OptionalParentObject parent = this->parent()) {
        LOG(Info, "Cannot remove " << briefDescription() << ", because that would invalidate " << parent->briefDescription() << ".");
        return {};
      }
      return ParentObject_Impl::remove();
    }

    boost::optional<ParentObject> ScheduleDay_Impl::parent() const {
      OptionalParentObject result;
      // parented by ScheduleRuleset or ScheduleRule
      ModelObjectVector users = getObject<ScheduleDay>().getModelObjectSources<ModelObject>();
      if (users.size() == 1u) {
        if (OptionalScheduleRuleset scheduleRuleset = users[0].optionalCast<ScheduleRuleset>()) {
          result = *scheduleRuleset;
        }
        if (OptionalScheduleRule scheduleRule = users[0].optionalCast<ScheduleRule>()) {
          result = *scheduleRule;
        }
      }
      return result;
    }

    bool ScheduleDay_Impl::setParent(ParentObject& newParent) {
      if (OptionalScheduleRule scheduleRule = newParent.optionalCast<ScheduleRule>()) {
        if (getObject<ScheduleDay>().getModelObjectSources<ModelObject>().empty()) {
          bool result = newParent.setPointer(OS_Schedule_RuleFields::DayScheduleName, handle());
          OS_ASSERT(result);
          return true;
        }
      }
      return false;
    }

    const std::vector<std::string>& ScheduleDay_Impl::outputVariableNames() const {
      static const std::vector<std::string> result;
      return result;
    }

    IddObjectType ScheduleDay_Impl::iddObjectType() const {
      return ScheduleDay::iddObjectType();
    }

    boost::optional<ScheduleTypeLimits> ScheduleDay_Impl::scheduleTypeLimits() const {
      OptionalScheduleTypeLimits result =
        getObject<ModelObject>().getModelObjectTarget<ScheduleTypeLimits>(OS_Schedule_DayFields::ScheduleTypeLimitsName);
      if (!result) {
        for (const Schedule& schedule : getObject<ScheduleDay>().getModelObjectSources<Schedule>()) {
          result = schedule.scheduleTypeLimits();
          if (result) {
            return result;
          }
        }
      }
      return result;
    }

    bool ScheduleDay_Impl::isScheduleTypeLimitsDefaulted() const {
      return !getObject<ScheduleDay>().getModelObjectTarget<ScheduleTypeLimits>(OS_Schedule_DayFields::ScheduleTypeLimitsName);
    }

    std::string ScheduleDay_Impl::interpolatetoTimestep() const {
      boost::optional<std::string> value = getString(OS_Schedule_DayFields::InterpolatetoTimestep, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool ScheduleDay_Impl::isInterpolatetoTimestepDefaulted() const {
      return isEmpty(OS_Schedule_DayFields::InterpolatetoTimestep);
    }

    std::vector<openstudio::Time> ScheduleDay_Impl::times() const {
      if (!m_cachedTimes) {

        std::vector<openstudio::Time> result;

        for (const IdfExtensibleGroup& group : extensibleGroups()) {
          OptionalInt hour = group.getInt(OS_Schedule_DayExtensibleFields::Hour, true);
          OptionalInt minute = group.getInt(OS_Schedule_DayExtensibleFields::Minute, true);

          if (hour && minute) {
            openstudio::Time time(0, *hour, *minute);
            if (time.totalMinutes() <= 0.5 || time.totalDays() > 1.0) {
              LOG(Error, "Time " << time << " in " << briefDescription() << " is out of range.");
            } else {
              result.push_back(time);
            }
          } else {
            LOG(Error, "Could not read time " << group.groupIndex() << " in " << briefDescription() << ".");
          }
        }

        m_cachedTimes = result;
      }

      return m_cachedTimes.get();
    }

    std::vector<double> ScheduleDay_Impl::values() const {
      if (!m_cachedValues) {

        std::vector<double> result;

        for (const IdfExtensibleGroup& group : extensibleGroups()) {
          OptionalDouble value = group.getDouble(OS_Schedule_DayExtensibleFields::ValueUntilTime, true);

          if (value) {
            result.push_back(*value);
          } else {
            LOG(Error, "Could not read value " << group.groupIndex() << " in " << briefDescription() << ".");
          }
        }

        m_cachedValues = result;
      }

      return m_cachedValues.get();
    }

    double ScheduleDay_Impl::getValue(const openstudio::Time& time) const {
      if (time.totalMinutes() < 0.0 || time.totalDays() > 1.0) {
        return 0.0;
      }

      // We'll calculate the entire day when we request a single value but that's on purpose:
      // * We're talking about max 60 timesteps * 24 hours = 1440 points,
      //   but realistically more often than not you'll have a timestep of 6 (our default) or 4 so 96 to 144 points. So not a lot of points
      // * We cache the timeSeries, and,
      // * More often than not, the use case is to do it for the entire day anyways (eg: openstudio-standards to determine occupancy schedules)
      TimeSeries ts = this->timeSeries();

      DateTimeVector dateTimes = ts.dateTimes();
      Vector values = ts.values();

      const unsigned N = dateTimes.size();
      OS_ASSERT(values.size() == N);

      if (N == 0) {
        return 0.0;
      }

      Vector x(N + 2);
      Vector y(N + 2);

      x[0] = 0.0;
      std::string interpolatetoTimestep = this->interpolatetoTimestep();
      if (istringEqual("No", interpolatetoTimestep)) {
        y[0] = values[0];
      } else if (istringEqual("Average", interpolatetoTimestep)) {
        y[0] = values[0];
      } else if (istringEqual("Linear", interpolatetoTimestep)) {
        y[0] = 0.0;
      }

      for (unsigned i = 0; i < N; ++i) {
        openstudio::Time t = dateTimes[i].time();
        if (t.totalDays() == 0.0) {  // this is 00:00:00 from the next day
          t = openstudio::Time(0, 24, 0);
        }

        x[i + 1] = t.totalDays();
        y[i + 1] = values[i];
      }

      x[N + 1] = 1.0;
      y[N + 1] = values[N - 1];

      double result = interp(x, y, time.totalDays(), HoldNextInterp, NoneExtrap);

      return result;
    }

    openstudio::TimeSeries ScheduleDay_Impl::timeSeries() const {
      if (!m_cachedTimeSeries) {

        int numberOfTimestepsPerHour;
        if (boost::optional<Timestep> timestep = this->model().getOptionalUniqueModelObject<Timestep>()) {
          numberOfTimestepsPerHour = timestep->numberOfTimestepsPerHour();
        } else {
          numberOfTimestepsPerHour = 6;
        }

        Date startDate(Date(MonthOfYear(MonthOfYear::Jan), 1));  // this is arbitrary
        int minutes = 60 / numberOfTimestepsPerHour;
        DateTime startDateTime(startDate, Time(0, 0, 0));

        DateTimeVector tsDateTimes;
        for (size_t hour = 0; hour < 24; ++hour) {
          for (size_t minute = minutes; minute <= 60; minute += minutes) {
            if (minute == 60) {
              openstudio::Time t(0, hour + 1, 0);
              tsDateTimes.push_back(startDateTime + t);
            } else {
              openstudio::Time t(0, hour, minute);
              tsDateTimes.push_back(startDateTime + t);
            }
          }
        }

        std::vector<double> values = this->values();          // these are already sorted
        std::vector<openstudio::Time> times = this->times();  // these are already sorted

        const unsigned N = times.size();
        OS_ASSERT(values.size() == N);

        TimeSeries result;
        if (N == 0) {
          return result;
        }

        Vector x(N + 2);
        Vector y(N + 2);

        x[0] = -0.000001;
        y[0] = 0.0;

        for (unsigned i = 0; i < N; ++i) {
          x[i + 1] = times[i].totalSeconds();
          y[i + 1] = values[i];
        }

        x[N + 1] = 86400.000001;
        y[N + 1] = 0.0;

        std::string interpolatetoTimestep = this->interpolatetoTimestep();
        Vector tsValues(tsDateTimes.size());
        for (unsigned j = 0; j < tsDateTimes.size(); ++j) {
          openstudio::Time t = tsDateTimes[j].time();
          if (t.totalDays() == 0.0) {  // this is 00:00:00 from the next day
            t = openstudio::Time(0, 24, 0);
          }

          if (istringEqual("No", interpolatetoTimestep)) {
            tsValues[j] = interp(x, y, t.totalSeconds(), HoldNextInterp, NoneExtrap);
          } else if (istringEqual("Average", interpolatetoTimestep)) {
            double minutes = 60.0 / numberOfTimestepsPerHour;
            double ti = minutes * 60.0;  // total seconds of the timestep interval
            tsValues[j] = interp(x, y, t.totalSeconds(), AverageInterp, NoneExtrap, ti);
          } else if (istringEqual("Linear", interpolatetoTimestep)) {
            tsValues[j] = interp(x, y, t.totalSeconds(), LinearInterp, NoneExtrap);
          }
        }

        result = TimeSeries(tsDateTimes, tsValues, "");
        m_cachedTimeSeries = result;
      }

      return m_cachedTimeSeries.get();
    }

    bool ScheduleDay_Impl::setScheduleTypeLimits(const ScheduleTypeLimits& scheduleTypeLimits) {
      if (scheduleTypeLimits.model() != model()) {
        return false;
      }
      if (!candidateIsCompatibleWithCurrentUse(scheduleTypeLimits)) {
        return false;
      }
      return setPointer(OS_Schedule_DayFields::ScheduleTypeLimitsName, scheduleTypeLimits.handle());
    }

    bool ScheduleDay_Impl::resetScheduleTypeLimits() {
      if (okToResetScheduleTypeLimits()) {
        return setString(OS_Schedule_DayFields::ScheduleTypeLimitsName, "");
      }
      return false;
    }

    bool ScheduleDay_Impl::setInterpolatetoTimestep(const std::string& interpolatetoTimestep) {
      return setString(OS_Schedule_DayFields::InterpolatetoTimestep, interpolatetoTimestep);
    }

    void ScheduleDay_Impl::resetInterpolatetoTimestep() {
      bool result = setString(OS_Schedule_DayFields::InterpolatetoTimestep, "");
      OS_ASSERT(result);
    }

    bool ScheduleDay_Impl::addValue(const openstudio::Time& untilTime, double value) {
      if (untilTime.totalMinutes() <= 0.5 || untilTime.totalDays() > 1.0) {
        return false;
      }

      // Check validity, cannot be NaN, Inf, etc
      if (std::isinf(value)) {
        LOG(Warn, "Cannot setDouble to Infinity for " << this->briefDescription());
        return false;
      } else if (std::isnan(value)) {
        LOG(Warn, "Cannot setDouble to a NaN for " << this->briefDescription());
        return false;
      }

      int untilHours = untilTime.hours() + 24 * untilTime.days();
      int untilMinutes = untilTime.minutes() + (int)floor((untilTime.seconds() / 60.0) + 0.5);

      if (untilMinutes >= 60) {
        untilHours += 1;
        untilMinutes += -60;
      }

      // use set to determine whether to overwrite or insert, and where
      std::set<openstudio::Time> times;
      std::pair<std::set<openstudio::Time>::const_iterator, bool> insertResult;
      for (const openstudio::Time& time : this->times()) {
        insertResult = times.insert(time);
        OS_ASSERT(insertResult.second);
      }

      insertResult = times.insert(untilTime);
      auto index = (unsigned)std::distance<std::set<openstudio::Time>::const_iterator>(times.begin(), insertResult.first);
      OS_ASSERT(index <= numExtensibleGroups());
      bool result(true);
      if (insertResult.second) {
        // new time--insert an extensible group
        std::vector<std::string> groupValues;
        groupValues.push_back(boost::lexical_cast<std::string>(untilHours));
        groupValues.push_back(boost::lexical_cast<std::string>(untilMinutes));
        groupValues.push_back(toString(value));

        IdfExtensibleGroup group = insertExtensibleGroup(index, groupValues);
        OS_ASSERT(!group.empty());
      } else {
        // time already exists, overwrite value
        IdfExtensibleGroup group = getExtensibleGroup(index);
        result = group.setDouble(OS_Schedule_DayExtensibleFields::ValueUntilTime, value);
      }

      return result;
    }

    boost::optional<double> ScheduleDay_Impl::removeValue(const openstudio::Time& time) {

      boost::optional<unsigned> timeIndex;

      std::vector<openstudio::Time> times = this->times();
      for (unsigned i = 0; i < times.size(); ++i) {
        if (times[i] == time) {
          timeIndex = i;
          break;
        }
      }

      if (!timeIndex) {
        return boost::none;
      }

      boost::optional<double> result;

      std::vector<double> values = this->values();
      OS_ASSERT(values.size() == times.size());

      clearValues();
      for (unsigned i = 0; i < times.size(); ++i) {
        if (i == *timeIndex) {
          result = values[i];
        } else {
          addValue(times[i], values[i]);
        }
      }

      return result;
    }

    void ScheduleDay_Impl::clearValues() {
      this->clearExtensibleGroups();

      addValue(Time(1, 0), 0.0);
    }

    void ScheduleDay_Impl::ensureNoLeapDays() {
      // nothing to do
    }

    bool ScheduleDay_Impl::candidateIsCompatibleWithCurrentUse(const ScheduleTypeLimits& candidate) const {
      // currently only check ScheduleDay against Schedules
      ScheduleVector users = getObject<ScheduleDay>().getModelObjectSources<Schedule>();
      for (const Schedule& user : users) {
        if (OptionalScheduleTypeLimits userLimits = user.scheduleTypeLimits()) {
          if (!isCompatible(*userLimits, candidate)) {
            return false;
          }
        }
      }
      return true;
    }

    bool ScheduleDay_Impl::okToResetScheduleTypeLimits() const {
      for (const ModelObject& user : getObject<ScheduleDay>().getModelObjectSources<ModelObject>()) {
        // schedules using this ScheduleDay need to also have no limits
        if (user.optionalCast<Schedule>() && user.cast<Schedule>().scheduleTypeLimits()) {
          return false;
        }
        if (user.optionalCast<ScheduleRule>()) {
          // test indirect use through ScheduleRule
          if (user.cast<ScheduleRule>().scheduleRuleset().scheduleTypeLimits()) {
            return false;
          }
        } else if (!user.optionalCast<ScheduleBase>()) {
          // other non-schedule users impose limits
          return false;
        }
      }
      return true;
    }

    void ScheduleDay_Impl::clearCachedVariables() {
      m_cachedTimes.reset();
      m_cachedValues.reset();
      m_cachedTimeSeries.reset();
    }

    void ScheduleDay_Impl::clearCachedTimeSeries() {
      m_cachedTimeSeries.reset();
    }

  }  // namespace detail

  ScheduleDay::ScheduleDay(const Model& model) : ScheduleBase(ScheduleDay::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::ScheduleDay_Impl>());

    addValue(Time(1, 0), 0.0);
  }

  ScheduleDay::ScheduleDay(const Model& model, double value) : ScheduleBase(ScheduleDay::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::ScheduleDay_Impl>());

    addValue(Time(1, 0), value);
  }

  IddObjectType ScheduleDay::iddObjectType() {
    IddObjectType result(IddObjectType::OS_Schedule_Day);
    return result;
  }

  bool ScheduleDay::isScheduleTypeLimitsDefaulted() const {
    return getImpl<detail::ScheduleDay_Impl>()->isScheduleTypeLimitsDefaulted();
  }

  std::string ScheduleDay::interpolatetoTimestep() const {
    return getImpl<detail::ScheduleDay_Impl>()->interpolatetoTimestep();
  }

  bool ScheduleDay::isInterpolatetoTimestepDefaulted() const {
    return getImpl<detail::ScheduleDay_Impl>()->isInterpolatetoTimestepDefaulted();
  }

  std::vector<openstudio::Time> ScheduleDay::times() const {
    return getImpl<detail::ScheduleDay_Impl>()->times();
  }

  std::vector<double> ScheduleDay::values() const {
    return getImpl<detail::ScheduleDay_Impl>()->values();
  }

  double ScheduleDay::getValue(const openstudio::Time& time) const {
    return getImpl<detail::ScheduleDay_Impl>()->getValue(time);
  }

  openstudio::TimeSeries ScheduleDay::timeSeries() const {
    return getImpl<detail::ScheduleDay_Impl>()->timeSeries();
  }

  bool ScheduleDay::setInterpolatetoTimestep(const std::string& interpolatetoTimestep) {
    return getImpl<detail::ScheduleDay_Impl>()->setInterpolatetoTimestep(interpolatetoTimestep);
  }

  void ScheduleDay::resetInterpolatetoTimestep() {
    getImpl<detail::ScheduleDay_Impl>()->resetInterpolatetoTimestep();
  }

  bool ScheduleDay::addValue(const openstudio::Time& untilTime, double value) {
    return getImpl<detail::ScheduleDay_Impl>()->addValue(untilTime, value);
  }

  boost::optional<double> ScheduleDay::removeValue(const openstudio::Time& time) {
    return getImpl<detail::ScheduleDay_Impl>()->removeValue(time);
  }

  void ScheduleDay::clearValues() {
    getImpl<detail::ScheduleDay_Impl>()->clearValues();
  }

  /// @cond
  ScheduleDay::ScheduleDay(std::shared_ptr<detail::ScheduleDay_Impl> impl) : ScheduleBase(std::move(impl)) {}
  /// @endcond

}  // namespace model
}  // namespace openstudio
