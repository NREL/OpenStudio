/**********************************************************************
 *  Copyright (c) 2008-2013, Alliance for Sustainable Energy.
 *  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 **********************************************************************/

#include <model/ScheduleDay.hpp>
#include <model/ScheduleDay_Impl.hpp>

#include <model/Model.hpp>
#include <model/ScheduleTypeRegistry.hpp>
#include <model/ScheduleTypeLimits.hpp>
#include <model/ScheduleTypeLimits_Impl.hpp>
#include <model/Schedule.hpp>
#include <model/Schedule_Impl.hpp>
#include <model/ScheduleRuleset.hpp>
#include <model/ScheduleRuleset_Impl.hpp>
#include <model/ScheduleRule.hpp>
#include <model/ScheduleRule_Impl.hpp>

#include <utilities/idf/IdfExtensibleGroup.hpp>
#include <utilities/idd/OS_Schedule_Day_FieldEnums.hxx>
#include <utilities/idd/OS_Schedule_Rule_FieldEnums.hxx>

#include <utilities/units/OSQuantityVector.hpp>

#include <utilities/core/Assert.hpp>

#include <utilities/time/Time.hpp>
#include <utilities/data/Vector.hpp>

namespace openstudio {
namespace model {

namespace detail {

  ScheduleDay_Impl::ScheduleDay_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
    : ScheduleBase_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == ScheduleDay::iddObjectType());
  }

  ScheduleDay_Impl::ScheduleDay_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                     Model_Impl* model,
                                     bool keepHandle)
    : ScheduleBase_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == ScheduleDay::iddObjectType());
  }

  ScheduleDay_Impl::ScheduleDay_Impl(const ScheduleDay_Impl& other,
                                     Model_Impl* model,
                                     bool keepHandle)
    : ScheduleBase_Impl(other,model,keepHandle)
  {}

  std::vector<IdfObject> ScheduleDay_Impl::remove() {
    if (OptionalParentObject parent = this->parent()) {
      LOG(Info,"Cannot remove " << briefDescription() << ", because that would invalidate "
          << parent->briefDescription() << ".");
      return IdfObjectVector();
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

  const std::vector<std::string>& ScheduleDay_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType ScheduleDay_Impl::iddObjectType() const
  {
    return ScheduleDay::iddObjectType();
  }

  boost::optional<ScheduleTypeLimits> ScheduleDay_Impl::scheduleTypeLimits() const {
    OptionalScheduleTypeLimits result = getObject<ModelObject>().getModelObjectTarget<ScheduleTypeLimits>(OS_Schedule_DayFields::ScheduleTypeLimitsName);
    if (!result) {
      BOOST_FOREACH(const Schedule& schedule,getObject<ScheduleDay>().getModelObjectSources<Schedule>()) {
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

  bool ScheduleDay_Impl::interpolatetoTimestep() const 
  {
    return getBooleanFieldValue(OS_Schedule_DayFields::InterpolatetoTimestep);
  }

  bool ScheduleDay_Impl::isInterpolatetoTimestepDefaulted() const 
  {
    return isEmpty(OS_Schedule_DayFields::InterpolatetoTimestep);
  }

  std::vector<openstudio::Time> ScheduleDay_Impl::times() const {
    std::vector<openstudio::Time> result;

    BOOST_FOREACH(const IdfExtensibleGroup& group, extensibleGroups()) {
      OptionalInt hour = group.getInt(OS_Schedule_DayExtensibleFields::Hour, true);
      OptionalInt minute = group.getInt(OS_Schedule_DayExtensibleFields::Minute, true);

      if (hour && minute){
        openstudio::Time time(0, *hour, *minute);
        if (time.totalDays() <= 0.0 || time.totalDays() > 1.0){
          LOG(Error, "Time " << time << " in " << briefDescription() << " is out of range." );
        }else{
          result.push_back(time);
        }
      }else{
        LOG(Error, "Could not read time " << group.groupIndex() << " in " << briefDescription() << "." );
      }
    }

    return result;
  }

  std::vector<double> ScheduleDay_Impl::values() const {
    std::vector<double> result;

    BOOST_FOREACH(const IdfExtensibleGroup& group, extensibleGroups()) {
      OptionalDouble value = group.getDouble(OS_Schedule_DayExtensibleFields::ValueUntilTime, true);

      if (value){
        result.push_back(*value);
      }else{
        LOG(Error, "Could not read value " << group.groupIndex() << " in " << briefDescription() << "." );
      }
    }

    return result;
  }

  double ScheduleDay_Impl::getValue(const openstudio::Time& time) const
  {
    if (time.totalDays() < 0.0 || time.totalDays() > 1.0){
      return 0.0;
    }

    std::vector<double> values = this->values(); // these are already sorted
    std::vector<openstudio::Time> times = this->times(); // these are already sorted

    unsigned N = times.size();
    OS_ASSERT(values.size() == N);

    if (N == 0){
      return 0.0;
    }

    openstudio::Vector x(N + 2);
    openstudio::Vector y(N + 2);

    x[0] = -0.000001;
    y[0] = 0.0;

    for (unsigned i = 0; i < N; ++i){
      x[i + 1] = times[i].totalDays();
      y[i + 1] = values[i];
    }

    x[N + 1] = 1.000001;
    y[N + 1] = 0.0;

    InterpMethod interpMethod;
    if (this->interpolatetoTimestep()){
      interpMethod = LinearInterp;
    }else{
      interpMethod = HoldNextInterp;
    }

    double result = interp(x, y, time.totalDays(), interpMethod, NoneExtrap);

    return result;
  }

  boost::optional<Quantity> ScheduleDay_Impl::getValueAsQuantity(const openstudio::Time& time, bool returnIP) const {
    return toQuantity(getValue(time),returnIP);
  }

  bool ScheduleDay_Impl::setScheduleTypeLimits(const ScheduleTypeLimits& scheduleTypeLimits) {
    if (scheduleTypeLimits.model() != model()) {
      return false;
    }
    if (!candidateIsCompatibleWithCurrentUse(scheduleTypeLimits)) {
      return false;
    }
    return setPointer(OS_Schedule_DayFields::ScheduleTypeLimitsName,scheduleTypeLimits.handle());
  }

  bool ScheduleDay_Impl::resetScheduleTypeLimits() {
    if (okToResetScheduleTypeLimits()) {
      return setString(OS_Schedule_DayFields::ScheduleTypeLimitsName,"");
    }
    return false;
  }

  void ScheduleDay_Impl::setInterpolatetoTimestep(bool interpolatetoTimestep) 
  {
    setBooleanFieldValue(OS_Schedule_DayFields::InterpolatetoTimestep,interpolatetoTimestep);
  }

  void ScheduleDay_Impl::resetInterpolatetoTimestep() 
  {
    bool result = setString(OS_Schedule_DayFields::InterpolatetoTimestep, "");
    OS_ASSERT(result);
  }

  bool ScheduleDay_Impl::addValue(const openstudio::Time& untilTime, double value) {
    if (untilTime.totalDays() <= 0.0 || untilTime.totalDays() > 1.0) {
      return false;
    }

    int untilHours = untilTime.hours() + 24*untilTime.days();
    int untilMinutes = untilTime.minutes() + floor((untilTime.seconds()/60.0) + 0.5);

    if (untilMinutes >= 60){
      untilHours += 1;
      untilMinutes += -60;
    }

    // use set to determine whether to overwrite or insert, and where
    std::set<openstudio::Time> times;
    std::pair<std::set<openstudio::Time>::const_iterator,bool> insertResult;
    BOOST_FOREACH(const openstudio::Time& time,this->times()) {
      insertResult = times.insert(time);
      OS_ASSERT(insertResult.second);
    }

    insertResult = times.insert(untilTime);
    unsigned index = std::distance<std::set<openstudio::Time>::const_iterator>(times.begin(),insertResult.first);
    OS_ASSERT(index >= 0);
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
    }
    else {
      // time already exists, overwrite value
      IdfExtensibleGroup group = getExtensibleGroup(index);
      result = group.setDouble(OS_Schedule_DayExtensibleFields::ValueUntilTime,value);
    }

    return result;
  }

  bool ScheduleDay_Impl::addValue(const openstudio::Time& untilTime, const Quantity& value) {
    OptionalDouble dval = toDouble(value);
    if (dval) {
      return addValue(untilTime,*dval);
    }
    return false;
  }


  boost::optional<double> ScheduleDay_Impl::removeValue(const openstudio::Time& time){
    
    boost::optional<unsigned> timeIndex;

    std::vector<openstudio::Time> times = this->times();
    for (unsigned i = 0; i < times.size(); ++i){
      if (times[i] == time){
        timeIndex = i;
        break;
      }
    }

    if (!timeIndex){
      return boost::none; 
    }

    boost::optional<double> result;

    std::vector<double> values = this->values();
    OS_ASSERT(values.size() == times.size());

    clearValues();
    for (unsigned i = 0; i < times.size(); ++i){
      if (i == *timeIndex){
        result = values[i];
      }else{
        addValue(times[i], values[i]);
      }
    }

    return result;
  }


  void ScheduleDay_Impl::clearValues()
  {
    this->clearExtensibleGroups();

    addValue(Time(1,0),0.0);
  }

  bool ScheduleDay_Impl::candidateIsCompatibleWithCurrentUse(const ScheduleTypeLimits& candidate) const {
    // currently only check ScheduleDay against Schedules
    ScheduleVector users = getObject<ScheduleDay>().getModelObjectSources<Schedule>();
    BOOST_FOREACH(const Schedule& user,users) {
      if (OptionalScheduleTypeLimits userLimits = user.scheduleTypeLimits()) {
        if (!isCompatible(*userLimits,candidate)) {
          return false;
        }
      }
    }
    return true;
  }

  bool ScheduleDay_Impl::okToResetScheduleTypeLimits() const {
    BOOST_FOREACH(const ModelObject& user, getObject<ScheduleDay>().getModelObjectSources<ModelObject>()) {
      // schedules using this ScheduleDay need to also have no limits
      if (user.optionalCast<Schedule>() && user.cast<Schedule>().scheduleTypeLimits()) {
        return false;
      }
      if (user.optionalCast<ScheduleRule>()) {
        // test indirect use through ScheduleRule
        if (user.cast<ScheduleRule>().scheduleRuleset().scheduleTypeLimits()) {
          return false;
        }
      }
      else if (!user.optionalCast<ScheduleBase>()) {
        // other non-schedule users impose limits
        return false;
      }
    }
    return true;
  }

} // detail

ScheduleDay::ScheduleDay(const Model& model)
  : ScheduleBase(ScheduleDay::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::ScheduleDay_Impl>());

  addValue(Time(1,0),0.0);
}

IddObjectType ScheduleDay::iddObjectType() {
  IddObjectType result(IddObjectType::OS_Schedule_Day);
  return result;
}

bool ScheduleDay::isScheduleTypeLimitsDefaulted() const {
  return getImpl<detail::ScheduleDay_Impl>()->isScheduleTypeLimitsDefaulted();
}

bool ScheduleDay::interpolatetoTimestep() const {
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

OSQuantityVector ScheduleDay::getValues(bool returnIP) const {
  return getImpl<detail::ScheduleDay_Impl>()->getValues(returnIP);
}

double ScheduleDay::getValue(const openstudio::Time& time) const {
  return getImpl<detail::ScheduleDay_Impl>()->getValue(time);
}

boost::optional<Quantity> ScheduleDay::getValueAsQuantity(const openstudio::Time& time,
                                                          bool returnIP) const
{
  return getImpl<detail::ScheduleDay_Impl>()->getValueAsQuantity(time,returnIP);
}

void ScheduleDay::setInterpolatetoTimestep(bool interpolatetoTimestep) {
  getImpl<detail::ScheduleDay_Impl>()->setInterpolatetoTimestep(interpolatetoTimestep);
}

void ScheduleDay::resetInterpolatetoTimestep() {
  getImpl<detail::ScheduleDay_Impl>()->resetInterpolatetoTimestep();
}

bool ScheduleDay::addValue(const openstudio::Time& untilTime, double value) {
  return getImpl<detail::ScheduleDay_Impl>()->addValue(untilTime, value);
}

bool ScheduleDay::addValue(const openstudio::Time& untilTime, const Quantity& value) {
  return getImpl<detail::ScheduleDay_Impl>()->addValue(untilTime,value);
}

boost::optional<double> ScheduleDay::removeValue(const openstudio::Time& time){
  return getImpl<detail::ScheduleDay_Impl>()->removeValue(time);
}

void ScheduleDay::clearValues()
{
  getImpl<detail::ScheduleDay_Impl>()->clearValues();
}

/// @cond
ScheduleDay::ScheduleDay(boost::shared_ptr<detail::ScheduleDay_Impl> impl)
  : ScheduleBase(impl)
{}
/// @endcond


} // model
} // openstudio

