/**********************************************************************
 *  Copyright (c) 2008-2016, Alliance for Sustainable Energy.
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

#include "RefrigerationDefrostCycleParameters.hpp"
#include "RefrigerationDefrostCycleParameters_Impl.hpp"

#include <utilities/idd/OS_Refrigeration_DefrostCycleParameters_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include "../utilities/time/Time.hpp"

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  RefrigerationDefrostCycleParameters_Impl::RefrigerationDefrostCycleParameters_Impl(const IdfObject& idfObject,
                                                                                     Model_Impl* model,
                                                                                     bool keepHandle)
    : ModelObject_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == RefrigerationDefrostCycleParameters::iddObjectType());
  }

  RefrigerationDefrostCycleParameters_Impl::RefrigerationDefrostCycleParameters_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                                     Model_Impl* model,
                                                                                     bool keepHandle)
    : ModelObject_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == RefrigerationDefrostCycleParameters::iddObjectType());
  }

  RefrigerationDefrostCycleParameters_Impl::RefrigerationDefrostCycleParameters_Impl(const RefrigerationDefrostCycleParameters_Impl& other,
                                                                                     Model_Impl* model,
                                                                                     bool keepHandle)
    : ModelObject_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& RefrigerationDefrostCycleParameters_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType RefrigerationDefrostCycleParameters_Impl::iddObjectType() const {
    return RefrigerationDefrostCycleParameters::iddObjectType();
  }

  boost::optional<int> RefrigerationDefrostCycleParameters_Impl::durationofDefrostCycle() const {
    return getInt(OS_Refrigeration_DefrostCycleParametersFields::DurationofDefrostCycle,true);
  }

  boost::optional<int> RefrigerationDefrostCycleParameters_Impl::dripDownTime() const {
    return getInt(OS_Refrigeration_DefrostCycleParametersFields::DripDownTime,true);
  }

  // bool RefrigerationDefrostCycleParameters_Impl::checkStartTimeOverlap(const boost::optional<openstudio::Time>& defrostStartTime1, const boost::optional<openstudio::Time>& defrostStartTime2) const {
  //   boost::optional<int>
  //   if ( defrostStartTime1 && defrostStartTime2 ) {
  //     defrost1StartTime.totalMinutes()
  //     pass
  //   }
  //   boost::optional<int> hour = getInt(OS_Refrigeration_DefrostCycleParametersFields::Defrost1HourStartTime,true);
  //   boost::optional<int> minute = getInt(OS_Refrigeration_DefrostCycleParametersFields::Defrost1MinuteStartTime,true);
  //   if ( hour && minute ) {
  //     return Time(0, *hour, *minute);
  //   } 
  //   return boost::none;
  // }

  boost::optional<openstudio::Time> RefrigerationDefrostCycleParameters_Impl::defrost1StartTime() const {
    boost::optional<int> hour = getInt(OS_Refrigeration_DefrostCycleParametersFields::Defrost1HourStartTime,true);
    boost::optional<int> minute = getInt(OS_Refrigeration_DefrostCycleParametersFields::Defrost1MinuteStartTime,true);
    if ( hour && minute ) {
      return Time(0, *hour, *minute);
    } 
    return boost::none;
  }

  boost::optional<openstudio::Time> RefrigerationDefrostCycleParameters_Impl::defrost2StartTime() const {
    boost::optional<int> hour = getInt(OS_Refrigeration_DefrostCycleParametersFields::Defrost2HourStartTime,true);
    boost::optional<int> minute = getInt(OS_Refrigeration_DefrostCycleParametersFields::Defrost2MinuteStartTime,true);
    if ( hour && minute ) {
      return Time(0, *hour, *minute);
    } 
    return boost::none;
  }

  boost::optional<openstudio::Time> RefrigerationDefrostCycleParameters_Impl::defrost3StartTime() const {
    boost::optional<int> hour = getInt(OS_Refrigeration_DefrostCycleParametersFields::Defrost3HourStartTime,true);
    boost::optional<int> minute = getInt(OS_Refrigeration_DefrostCycleParametersFields::Defrost3MinuteStartTime,true);
    if ( hour && minute ) {
      return Time(0, *hour, *minute);
    } 
    return boost::none;
  }

  boost::optional<openstudio::Time> RefrigerationDefrostCycleParameters_Impl::defrost4StartTime() const {
    boost::optional<int> hour = getInt(OS_Refrigeration_DefrostCycleParametersFields::Defrost4HourStartTime,true);
    boost::optional<int> minute = getInt(OS_Refrigeration_DefrostCycleParametersFields::Defrost4MinuteStartTime,true);
    if ( hour && minute ) {
      return Time(0, *hour, *minute);
    } 
    return boost::none;
  }

  boost::optional<openstudio::Time> RefrigerationDefrostCycleParameters_Impl::defrost5StartTime() const {
    boost::optional<int> hour = getInt(OS_Refrigeration_DefrostCycleParametersFields::Defrost5HourStartTime,true);
    boost::optional<int> minute = getInt(OS_Refrigeration_DefrostCycleParametersFields::Defrost5MinuteStartTime,true);
    if ( hour && minute ) {
      return Time(0, *hour, *minute);
    } 
    return boost::none;
  }

  boost::optional<openstudio::Time> RefrigerationDefrostCycleParameters_Impl::defrost6StartTime() const {
    boost::optional<int> hour = getInt(OS_Refrigeration_DefrostCycleParametersFields::Defrost6HourStartTime,true);
    boost::optional<int> minute = getInt(OS_Refrigeration_DefrostCycleParametersFields::Defrost6MinuteStartTime,true);
    if ( hour && minute ) {
      return Time(0, *hour, *minute);
    } 
    return boost::none;
  }

  boost::optional<openstudio::Time> RefrigerationDefrostCycleParameters_Impl::defrost7StartTime() const {
    boost::optional<int> hour = getInt(OS_Refrigeration_DefrostCycleParametersFields::Defrost7HourStartTime,true);
    boost::optional<int> minute = getInt(OS_Refrigeration_DefrostCycleParametersFields::Defrost7MinuteStartTime,true);
    if ( hour && minute ) {
      return Time(0, *hour, *minute);
    } 
    return boost::none;
  }

  boost::optional<openstudio::Time> RefrigerationDefrostCycleParameters_Impl::defrost8StartTime() const {
    boost::optional<int> hour = getInt(OS_Refrigeration_DefrostCycleParametersFields::Defrost8HourStartTime,true);
    boost::optional<int> minute = getInt(OS_Refrigeration_DefrostCycleParametersFields::Defrost8MinuteStartTime,true);
    if ( hour && minute ) {
      return Time(0, *hour, *minute);
    } 
    return boost::none;
  }

  bool RefrigerationDefrostCycleParameters_Impl::setDurationofDefrostCycle(boost::optional<int> durationofDefrostCycle) {
    bool result(false);
    if (durationofDefrostCycle) {
      result = setInt(OS_Refrigeration_DefrostCycleParametersFields::DurationofDefrostCycle, durationofDefrostCycle.get());
    }
    else {
      resetDurationofDefrostCycle();
      result = true;
    }
    return result;
  }

  void RefrigerationDefrostCycleParameters_Impl::resetDurationofDefrostCycle() {
    bool result = setString(OS_Refrigeration_DefrostCycleParametersFields::DurationofDefrostCycle, "");
    OS_ASSERT(result);
  }

  bool RefrigerationDefrostCycleParameters_Impl::setDripDownTime(boost::optional<int> dripDownTime) {
    bool result(false);
    if (dripDownTime) {
      result = setInt(OS_Refrigeration_DefrostCycleParametersFields::DripDownTime, dripDownTime.get());
    }
    else {
      resetDripDownTime();
      result = true;
    }
    return result;
  }

  void RefrigerationDefrostCycleParameters_Impl::resetDripDownTime() {
    bool result = setString(OS_Refrigeration_DefrostCycleParametersFields::DripDownTime, "");
    OS_ASSERT(result);
  }

  bool RefrigerationDefrostCycleParameters_Impl::setDefrost1StartTime(const openstudio::Time& defrost1StartTime) {
    bool hours = setInt(OS_Refrigeration_DefrostCycleParametersFields::Defrost1HourStartTime, defrost1StartTime.hours());
    bool minutes = setInt(OS_Refrigeration_DefrostCycleParametersFields::Defrost1MinuteStartTime, defrost1StartTime.minutes());
    if ( !hours || !minutes ) {
      resetDefrost1StartTime();
    }
    return hours && minutes;
  }

  void RefrigerationDefrostCycleParameters_Impl::resetDefrost1StartTime() {
    bool result = setString(OS_Refrigeration_DefrostCycleParametersFields::Defrost1HourStartTime, "");
    OS_ASSERT(result);
    result = setString(OS_Refrigeration_DefrostCycleParametersFields::Defrost1MinuteStartTime, "");
    OS_ASSERT(result);
  }

  bool RefrigerationDefrostCycleParameters_Impl::setDefrost2StartTime(const openstudio::Time& defrost2StartTime) {
    bool hours = setInt(OS_Refrigeration_DefrostCycleParametersFields::Defrost2HourStartTime, defrost2StartTime.hours());
    bool minutes = setInt(OS_Refrigeration_DefrostCycleParametersFields::Defrost2MinuteStartTime, defrost2StartTime.minutes());
    if ( !hours || !minutes ) {
      resetDefrost2StartTime();
    }
    return hours && minutes;
  }

  void RefrigerationDefrostCycleParameters_Impl::resetDefrost2StartTime() {
    bool result = setString(OS_Refrigeration_DefrostCycleParametersFields::Defrost2HourStartTime, "");
    OS_ASSERT(result);
    result = setString(OS_Refrigeration_DefrostCycleParametersFields::Defrost2MinuteStartTime, "");
    OS_ASSERT(result);
  }

  bool RefrigerationDefrostCycleParameters_Impl::setDefrost3StartTime(const openstudio::Time& defrost3StartTime) {
    bool hours = setInt(OS_Refrigeration_DefrostCycleParametersFields::Defrost3HourStartTime, defrost3StartTime.hours());
    bool minutes = setInt(OS_Refrigeration_DefrostCycleParametersFields::Defrost3MinuteStartTime, defrost3StartTime.minutes());
    if ( !hours || !minutes ) {
      resetDefrost3StartTime();
    }
    return hours && minutes;
  }

  void RefrigerationDefrostCycleParameters_Impl::resetDefrost3StartTime() {
    bool result = setString(OS_Refrigeration_DefrostCycleParametersFields::Defrost3HourStartTime, "");
    OS_ASSERT(result);
    result = setString(OS_Refrigeration_DefrostCycleParametersFields::Defrost3MinuteStartTime, "");
    OS_ASSERT(result);
  }

  bool RefrigerationDefrostCycleParameters_Impl::setDefrost4StartTime(const openstudio::Time& defrost4StartTime) {
    bool hours = setInt(OS_Refrigeration_DefrostCycleParametersFields::Defrost4HourStartTime, defrost4StartTime.hours());
    bool minutes = setInt(OS_Refrigeration_DefrostCycleParametersFields::Defrost4MinuteStartTime, defrost4StartTime.minutes());
    if ( !hours || !minutes ) {
      resetDefrost4StartTime();
    }
    return hours && minutes;
  }

  void RefrigerationDefrostCycleParameters_Impl::resetDefrost4StartTime() {
    bool result = setString(OS_Refrigeration_DefrostCycleParametersFields::Defrost4HourStartTime, "");
    OS_ASSERT(result);
    result = setString(OS_Refrigeration_DefrostCycleParametersFields::Defrost4MinuteStartTime, "");
    OS_ASSERT(result);
  }

  bool RefrigerationDefrostCycleParameters_Impl::setDefrost5StartTime(const openstudio::Time& defrost5StartTime) {
    bool hours = setInt(OS_Refrigeration_DefrostCycleParametersFields::Defrost5HourStartTime, defrost5StartTime.hours());
    bool minutes = setInt(OS_Refrigeration_DefrostCycleParametersFields::Defrost5MinuteStartTime, defrost5StartTime.minutes());
    if ( !hours || !minutes ) {
      resetDefrost5StartTime();
    }
    return hours && minutes;
  }

  void RefrigerationDefrostCycleParameters_Impl::resetDefrost5StartTime() {
    bool result = setString(OS_Refrigeration_DefrostCycleParametersFields::Defrost5HourStartTime, "");
    OS_ASSERT(result);
    result = setString(OS_Refrigeration_DefrostCycleParametersFields::Defrost5MinuteStartTime, "");
    OS_ASSERT(result);
  }

  bool RefrigerationDefrostCycleParameters_Impl::setDefrost6StartTime(const openstudio::Time& defrost6StartTime) {
    bool hours = setInt(OS_Refrigeration_DefrostCycleParametersFields::Defrost6HourStartTime, defrost6StartTime.hours());
    bool minutes = setInt(OS_Refrigeration_DefrostCycleParametersFields::Defrost6MinuteStartTime, defrost6StartTime.minutes());
    if ( !hours || !minutes ) {
      resetDefrost6StartTime();
    }
    return hours && minutes;
  }

  void RefrigerationDefrostCycleParameters_Impl::resetDefrost6StartTime() {
    bool result = setString(OS_Refrigeration_DefrostCycleParametersFields::Defrost6HourStartTime, "");
    OS_ASSERT(result);
    result = setString(OS_Refrigeration_DefrostCycleParametersFields::Defrost6MinuteStartTime, "");
    OS_ASSERT(result);
  }

  bool RefrigerationDefrostCycleParameters_Impl::setDefrost7StartTime(const openstudio::Time& defrost7StartTime) {
    bool hours = setInt(OS_Refrigeration_DefrostCycleParametersFields::Defrost7HourStartTime, defrost7StartTime.hours());
    bool minutes = setInt(OS_Refrigeration_DefrostCycleParametersFields::Defrost7MinuteStartTime, defrost7StartTime.minutes());
    if ( !hours || !minutes ) {
      resetDefrost7StartTime();
    }
    return hours && minutes;
  }

  void RefrigerationDefrostCycleParameters_Impl::resetDefrost7StartTime() {
    bool result = setString(OS_Refrigeration_DefrostCycleParametersFields::Defrost7HourStartTime, "");
    OS_ASSERT(result);
    result = setString(OS_Refrigeration_DefrostCycleParametersFields::Defrost7MinuteStartTime, "");
    OS_ASSERT(result);
  }

  bool RefrigerationDefrostCycleParameters_Impl::setDefrost8StartTime(const openstudio::Time& defrost8StartTime) {
    bool hours = setInt(OS_Refrigeration_DefrostCycleParametersFields::Defrost8HourStartTime, defrost8StartTime.hours());
    bool minutes = setInt(OS_Refrigeration_DefrostCycleParametersFields::Defrost8MinuteStartTime, defrost8StartTime.minutes());
    if ( !hours || !minutes ) {
      resetDefrost8StartTime();
    }
    return hours && minutes;
  }

  void RefrigerationDefrostCycleParameters_Impl::resetDefrost8StartTime() {
    bool result = setString(OS_Refrigeration_DefrostCycleParametersFields::Defrost8HourStartTime, "");
    OS_ASSERT(result);
    result = setString(OS_Refrigeration_DefrostCycleParametersFields::Defrost8MinuteStartTime, "");
    OS_ASSERT(result);
  }

} // detail

RefrigerationDefrostCycleParameters::RefrigerationDefrostCycleParameters(const Model& model)
  : ModelObject(RefrigerationDefrostCycleParameters::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::RefrigerationDefrostCycleParameters_Impl>());
}

IddObjectType RefrigerationDefrostCycleParameters::iddObjectType() {
  return IddObjectType(IddObjectType::OS_Refrigeration_DefrostCycleParameters);
}

/// @cond
RefrigerationDefrostCycleParameters::RefrigerationDefrostCycleParameters(std::shared_ptr<detail::RefrigerationDefrostCycleParameters_Impl> impl)
  : ModelObject(impl)
{}
/// @endcond

} // model
} // openstudio

