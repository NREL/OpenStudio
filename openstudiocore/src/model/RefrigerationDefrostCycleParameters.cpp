/**********************************************************************
 *  Copyright (c) 2008-2014, Alliance for Sustainable Energy.
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

#include <model/RefrigerationDefrostCycleParameters.hpp>
#include <model/RefrigerationDefrostCycleParameters_Impl.hpp>

#include <utilities/idd/OS_Refrigeration_DefrostCycleParameters_FieldEnums.hxx>

#include <utilities/time/Time.hpp>

#include <utilities/core/Assert.hpp>

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

  // boost::optional<int> RefrigerationDefrostCycleParameters_Impl::defrost1HourStartTime() const {
  //   return getInt(OS_Refrigeration_DefrostCycleParametersFields::Defrost1HourStartTime,true);
  // }

  // boost::optional<int> RefrigerationDefrostCycleParameters_Impl::defrost1MinuteStartTime() const {
  //   return getInt(OS_Refrigeration_DefrostCycleParametersFields::Defrost1MinuteStartTime,true);
  // }

  // boost::optional<int> RefrigerationDefrostCycleParameters_Impl::defrost2HourStartTime() const {
  //   return getInt(OS_Refrigeration_DefrostCycleParametersFields::Defrost2HourStartTime,true);
  // }

  // boost::optional<int> RefrigerationDefrostCycleParameters_Impl::defrost2MinuteStartTime() const {
  //   return getInt(OS_Refrigeration_DefrostCycleParametersFields::Defrost2MinuteStartTime,true);
  // }

  // boost::optional<int> RefrigerationDefrostCycleParameters_Impl::defrost3HourStartTime() const {
  //   return getInt(OS_Refrigeration_DefrostCycleParametersFields::Defrost3HourStartTime,true);
  // }

  // boost::optional<int> RefrigerationDefrostCycleParameters_Impl::defrost3MinuteStartTime() const {
  //   return getInt(OS_Refrigeration_DefrostCycleParametersFields::Defrost3MinuteStartTime,true);
  // }

  // boost::optional<int> RefrigerationDefrostCycleParameters_Impl::defrost4HourStartTime() const {
  //   return getInt(OS_Refrigeration_DefrostCycleParametersFields::Defrost4HourStartTime,true);
  // }

  // boost::optional<int> RefrigerationDefrostCycleParameters_Impl::defrost4MinuteStartTime() const {
  //   return getInt(OS_Refrigeration_DefrostCycleParametersFields::Defrost4MinuteStartTime,true);
  // }

  // boost::optional<int> RefrigerationDefrostCycleParameters_Impl::defrost5HourStartTime() const {
  //   return getInt(OS_Refrigeration_DefrostCycleParametersFields::Defrost5HourStartTime,true);
  // }

  // boost::optional<int> RefrigerationDefrostCycleParameters_Impl::defrost5MinuteStartTime() const {
  //   return getInt(OS_Refrigeration_DefrostCycleParametersFields::Defrost5MinuteStartTime,true);
  // }

  // boost::optional<int> RefrigerationDefrostCycleParameters_Impl::defrost6HourStartTime() const {
  //   return getInt(OS_Refrigeration_DefrostCycleParametersFields::Defrost6HourStartTime,true);
  // }

  // boost::optional<int> RefrigerationDefrostCycleParameters_Impl::defrost6MinuteStartTime() const {
  //   return getInt(OS_Refrigeration_DefrostCycleParametersFields::Defrost6MinuteStartTime,true);
  // }

  // boost::optional<int> RefrigerationDefrostCycleParameters_Impl::defrost7HourStartTime() const {
  //   return getInt(OS_Refrigeration_DefrostCycleParametersFields::Defrost7HourStartTime,true);
  // }

  // boost::optional<int> RefrigerationDefrostCycleParameters_Impl::defrost7MinuteStartTime() const {
  //   return getInt(OS_Refrigeration_DefrostCycleParametersFields::Defrost7MinuteStartTime,true);
  // }

  // boost::optional<int> RefrigerationDefrostCycleParameters_Impl::defrost8HourStartTime() const {
  //   return getInt(OS_Refrigeration_DefrostCycleParametersFields::Defrost8HourStartTime,true);
  // }

  // boost::optional<int> RefrigerationDefrostCycleParameters_Impl::defrost8MinuteStartTime() const {
  //   return getInt(OS_Refrigeration_DefrostCycleParametersFields::Defrost8MinuteStartTime,true);
  // }

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

  // void RefrigerationDefrostCycleParameters_Impl::resetDefrost1StartTime() {
  //   bool result = setString(OS_Refrigeration_DefrostCycleParametersFields::Defrost1StartTime, "");
  //   OS_ASSERT(result);
  // }

  // bool RefrigerationDefrostCycleParameters_Impl::setDefrost1HourStartTime(boost::optional<int> defrost1HourStartTime) {
  //   bool result(false);
  //   if (defrost1HourStartTime) {
  //     result = setInt(OS_Refrigeration_DefrostCycleParametersFields::Defrost1HourStartTime, defrost1HourStartTime.get());
  //   }
  //   else {
  //     resetDefrost1HourStartTime();
  //     result = true;
  //   }
  //   return result;
  // }

  // void RefrigerationDefrostCycleParameters_Impl::resetDefrost1HourStartTime() {
  //   bool result = setString(OS_Refrigeration_DefrostCycleParametersFields::Defrost1HourStartTime, "");
  //   OS_ASSERT(result);
  // }

  // bool RefrigerationDefrostCycleParameters_Impl::setDefrost1MinuteStartTime(boost::optional<int> defrost1MinuteStartTime) {
  //   bool result(false);
  //   if (defrost1MinuteStartTime) {
  //     result = setInt(OS_Refrigeration_DefrostCycleParametersFields::Defrost1MinuteStartTime, defrost1MinuteStartTime.get());
  //   }
  //   else {
  //     resetDefrost1MinuteStartTime();
  //     result = true;
  //   }
  //   return result;
  // }

  // void RefrigerationDefrostCycleParameters_Impl::resetDefrost1MinuteStartTime() {
  //   bool result = setString(OS_Refrigeration_DefrostCycleParametersFields::Defrost1MinuteStartTime, "");
  //   OS_ASSERT(result);
  // }

  // bool RefrigerationDefrostCycleParameters_Impl::setDefrost2HourStartTime(boost::optional<int> defrost2HourStartTime) {
  //   bool result(false);
  //   if (defrost2HourStartTime) {
  //     result = setInt(OS_Refrigeration_DefrostCycleParametersFields::Defrost2HourStartTime, defrost2HourStartTime.get());
  //   }
  //   else {
  //     resetDefrost2HourStartTime();
  //     result = true;
  //   }
  //   return result;
  // }

  // void RefrigerationDefrostCycleParameters_Impl::resetDefrost2HourStartTime() {
  //   bool result = setString(OS_Refrigeration_DefrostCycleParametersFields::Defrost2HourStartTime, "");
  //   OS_ASSERT(result);
  // }

  // bool RefrigerationDefrostCycleParameters_Impl::setDefrost2MinuteStartTime(boost::optional<int> defrost2MinuteStartTime) {
  //   bool result(false);
  //   if (defrost2MinuteStartTime) {
  //     result = setInt(OS_Refrigeration_DefrostCycleParametersFields::Defrost2MinuteStartTime, defrost2MinuteStartTime.get());
  //   }
  //   else {
  //     resetDefrost2MinuteStartTime();
  //     result = true;
  //   }
  //   return result;
  // }

  // void RefrigerationDefrostCycleParameters_Impl::resetDefrost2MinuteStartTime() {
  //   bool result = setString(OS_Refrigeration_DefrostCycleParametersFields::Defrost2MinuteStartTime, "");
  //   OS_ASSERT(result);
  // }

  // bool RefrigerationDefrostCycleParameters_Impl::setDefrost3HourStartTime(boost::optional<int> defrost3HourStartTime) {
  //   bool result(false);
  //   if (defrost3HourStartTime) {
  //     result = setInt(OS_Refrigeration_DefrostCycleParametersFields::Defrost3HourStartTime, defrost3HourStartTime.get());
  //   }
  //   else {
  //     resetDefrost3HourStartTime();
  //     result = true;
  //   }
  //   return result;
  // }

  // void RefrigerationDefrostCycleParameters_Impl::resetDefrost3HourStartTime() {
  //   bool result = setString(OS_Refrigeration_DefrostCycleParametersFields::Defrost3HourStartTime, "");
  //   OS_ASSERT(result);
  // }

  // bool RefrigerationDefrostCycleParameters_Impl::setDefrost3MinuteStartTime(boost::optional<int> defrost3MinuteStartTime) {
  //   bool result(false);
  //   if (defrost3MinuteStartTime) {
  //     result = setInt(OS_Refrigeration_DefrostCycleParametersFields::Defrost3MinuteStartTime, defrost3MinuteStartTime.get());
  //   }
  //   else {
  //     resetDefrost3MinuteStartTime();
  //     result = true;
  //   }
  //   return result;
  // }

  // void RefrigerationDefrostCycleParameters_Impl::resetDefrost3MinuteStartTime() {
  //   bool result = setString(OS_Refrigeration_DefrostCycleParametersFields::Defrost3MinuteStartTime, "");
  //   OS_ASSERT(result);
  // }

  // bool RefrigerationDefrostCycleParameters_Impl::setDefrost4HourStartTime(boost::optional<int> defrost4HourStartTime) {
  //   bool result(false);
  //   if (defrost4HourStartTime) {
  //     result = setInt(OS_Refrigeration_DefrostCycleParametersFields::Defrost4HourStartTime, defrost4HourStartTime.get());
  //   }
  //   else {
  //     resetDefrost4HourStartTime();
  //     result = true;
  //   }
  //   return result;
  // }

  // void RefrigerationDefrostCycleParameters_Impl::resetDefrost4HourStartTime() {
  //   bool result = setString(OS_Refrigeration_DefrostCycleParametersFields::Defrost4HourStartTime, "");
  //   OS_ASSERT(result);
  // }

  // bool RefrigerationDefrostCycleParameters_Impl::setDefrost4MinuteStartTime(boost::optional<int> defrost4MinuteStartTime) {
  //   bool result(false);
  //   if (defrost4MinuteStartTime) {
  //     result = setInt(OS_Refrigeration_DefrostCycleParametersFields::Defrost4MinuteStartTime, defrost4MinuteStartTime.get());
  //   }
  //   else {
  //     resetDefrost4MinuteStartTime();
  //     result = true;
  //   }
  //   return result;
  // }

  // void RefrigerationDefrostCycleParameters_Impl::resetDefrost4MinuteStartTime() {
  //   bool result = setString(OS_Refrigeration_DefrostCycleParametersFields::Defrost4MinuteStartTime, "");
  //   OS_ASSERT(result);
  // }

  // bool RefrigerationDefrostCycleParameters_Impl::setDefrost5HourStartTime(boost::optional<int> defrost5HourStartTime) {
  //   bool result(false);
  //   if (defrost5HourStartTime) {
  //     result = setInt(OS_Refrigeration_DefrostCycleParametersFields::Defrost5HourStartTime, defrost5HourStartTime.get());
  //   }
  //   else {
  //     resetDefrost5HourStartTime();
  //     result = true;
  //   }
  //   return result;
  // }

  // void RefrigerationDefrostCycleParameters_Impl::resetDefrost5HourStartTime() {
  //   bool result = setString(OS_Refrigeration_DefrostCycleParametersFields::Defrost5HourStartTime, "");
  //   OS_ASSERT(result);
  // }

  // bool RefrigerationDefrostCycleParameters_Impl::setDefrost5MinuteStartTime(boost::optional<int> defrost5MinuteStartTime) {
  //   bool result(false);
  //   if (defrost5MinuteStartTime) {
  //     result = setInt(OS_Refrigeration_DefrostCycleParametersFields::Defrost5MinuteStartTime, defrost5MinuteStartTime.get());
  //   }
  //   else {
  //     resetDefrost5MinuteStartTime();
  //     result = true;
  //   }
  //   return result;
  // }

  // void RefrigerationDefrostCycleParameters_Impl::resetDefrost5MinuteStartTime() {
  //   bool result = setString(OS_Refrigeration_DefrostCycleParametersFields::Defrost5MinuteStartTime, "");
  //   OS_ASSERT(result);
  // }

  // bool RefrigerationDefrostCycleParameters_Impl::setDefrost6HourStartTime(boost::optional<int> defrost6HourStartTime) {
  //   bool result(false);
  //   if (defrost6HourStartTime) {
  //     result = setInt(OS_Refrigeration_DefrostCycleParametersFields::Defrost6HourStartTime, defrost6HourStartTime.get());
  //   }
  //   else {
  //     resetDefrost6HourStartTime();
  //     result = true;
  //   }
  //   return result;
  // }

  // void RefrigerationDefrostCycleParameters_Impl::resetDefrost6HourStartTime() {
  //   bool result = setString(OS_Refrigeration_DefrostCycleParametersFields::Defrost6HourStartTime, "");
  //   OS_ASSERT(result);
  // }

  // bool RefrigerationDefrostCycleParameters_Impl::setDefrost6MinuteStartTime(boost::optional<int> defrost6MinuteStartTime) {
  //   bool result(false);
  //   if (defrost6MinuteStartTime) {
  //     result = setInt(OS_Refrigeration_DefrostCycleParametersFields::Defrost6MinuteStartTime, defrost6MinuteStartTime.get());
  //   }
  //   else {
  //     resetDefrost6MinuteStartTime();
  //     result = true;
  //   }
  //   return result;
  // }

  // void RefrigerationDefrostCycleParameters_Impl::resetDefrost6MinuteStartTime() {
  //   bool result = setString(OS_Refrigeration_DefrostCycleParametersFields::Defrost6MinuteStartTime, "");
  //   OS_ASSERT(result);
  // }

  // bool RefrigerationDefrostCycleParameters_Impl::setDefrost7HourStartTime(boost::optional<int> defrost7HourStartTime) {
  //   bool result(false);
  //   if (defrost7HourStartTime) {
  //     result = setInt(OS_Refrigeration_DefrostCycleParametersFields::Defrost7HourStartTime, defrost7HourStartTime.get());
  //   }
  //   else {
  //     resetDefrost7HourStartTime();
  //     result = true;
  //   }
  //   return result;
  // }

  // void RefrigerationDefrostCycleParameters_Impl::resetDefrost7HourStartTime() {
  //   bool result = setString(OS_Refrigeration_DefrostCycleParametersFields::Defrost7HourStartTime, "");
  //   OS_ASSERT(result);
  // }

  // bool RefrigerationDefrostCycleParameters_Impl::setDefrost7MinuteStartTime(boost::optional<int> defrost7MinuteStartTime) {
  //   bool result(false);
  //   if (defrost7MinuteStartTime) {
  //     result = setInt(OS_Refrigeration_DefrostCycleParametersFields::Defrost7MinuteStartTime, defrost7MinuteStartTime.get());
  //   }
  //   else {
  //     resetDefrost7MinuteStartTime();
  //     result = true;
  //   }
  //   return result;
  // }

  // void RefrigerationDefrostCycleParameters_Impl::resetDefrost7MinuteStartTime() {
  //   bool result = setString(OS_Refrigeration_DefrostCycleParametersFields::Defrost7MinuteStartTime, "");
  //   OS_ASSERT(result);
  // }

  // bool RefrigerationDefrostCycleParameters_Impl::setDefrost8HourStartTime(boost::optional<int> defrost8HourStartTime) {
  //   bool result(false);
  //   if (defrost8HourStartTime) {
  //     result = setInt(OS_Refrigeration_DefrostCycleParametersFields::Defrost8HourStartTime, defrost8HourStartTime.get());
  //   }
  //   else {
  //     resetDefrost8HourStartTime();
  //     result = true;
  //   }
  //   return result;
  // }

  // void RefrigerationDefrostCycleParameters_Impl::resetDefrost8HourStartTime() {
  //   bool result = setString(OS_Refrigeration_DefrostCycleParametersFields::Defrost8HourStartTime, "");
  //   OS_ASSERT(result);
  // }

  // bool RefrigerationDefrostCycleParameters_Impl::setDefrost8MinuteStartTime(boost::optional<int> defrost8MinuteStartTime) {
  //   bool result(false);
  //   if (defrost8MinuteStartTime) {
  //     result = setInt(OS_Refrigeration_DefrostCycleParametersFields::Defrost8MinuteStartTime, defrost8MinuteStartTime.get());
  //   }
  //   else {
  //     resetDefrost8MinuteStartTime();
  //     result = true;
  //   }
  //   return result;
  // }

  // void RefrigerationDefrostCycleParameters_Impl::resetDefrost8MinuteStartTime() {
  //   bool result = setString(OS_Refrigeration_DefrostCycleParametersFields::Defrost8MinuteStartTime, "");
  //   OS_ASSERT(result);
  // }

} // detail

RefrigerationDefrostCycleParameters::RefrigerationDefrostCycleParameters(const Model& model)
  : ModelObject(RefrigerationDefrostCycleParameters::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::RefrigerationDefrostCycleParameters_Impl>());

  // TODO: Appropriately handle the following required object-list fields.
}

IddObjectType RefrigerationDefrostCycleParameters::iddObjectType() {
  return IddObjectType(IddObjectType::OS_Refrigeration_DefrostCycleParameters);
}

boost::optional<int> RefrigerationDefrostCycleParameters::durationofDefrostCycle() const {
  return getImpl<detail::RefrigerationDefrostCycleParameters_Impl>()->durationofDefrostCycle();
}

boost::optional<int> RefrigerationDefrostCycleParameters::dripDownTime() const {
  return getImpl<detail::RefrigerationDefrostCycleParameters_Impl>()->dripDownTime();
}

boost::optional<openstudio::Time> RefrigerationDefrostCycleParameters::defrost1StartTime() const {
  return getImpl<detail::RefrigerationDefrostCycleParameters_Impl>()->defrost1StartTime();
}

boost::optional<openstudio::Time> RefrigerationDefrostCycleParameters::defrost2StartTime() const {
  return getImpl<detail::RefrigerationDefrostCycleParameters_Impl>()->defrost2StartTime();
}

boost::optional<openstudio::Time> RefrigerationDefrostCycleParameters::defrost3StartTime() const {
  return getImpl<detail::RefrigerationDefrostCycleParameters_Impl>()->defrost3StartTime();
}

boost::optional<openstudio::Time> RefrigerationDefrostCycleParameters::defrost4StartTime() const {
  return getImpl<detail::RefrigerationDefrostCycleParameters_Impl>()->defrost4StartTime();
}

boost::optional<openstudio::Time> RefrigerationDefrostCycleParameters::defrost5StartTime() const {
  return getImpl<detail::RefrigerationDefrostCycleParameters_Impl>()->defrost5StartTime();
}

boost::optional<openstudio::Time> RefrigerationDefrostCycleParameters::defrost6StartTime() const {
  return getImpl<detail::RefrigerationDefrostCycleParameters_Impl>()->defrost6StartTime();
}

boost::optional<openstudio::Time> RefrigerationDefrostCycleParameters::defrost7StartTime() const {
  return getImpl<detail::RefrigerationDefrostCycleParameters_Impl>()->defrost7StartTime();
}

boost::optional<openstudio::Time> RefrigerationDefrostCycleParameters::defrost8StartTime() const {
  return getImpl<detail::RefrigerationDefrostCycleParameters_Impl>()->defrost8StartTime();
}

// boost::optional<int> RefrigerationDefrostCycleParameters::defrost1HourStartTime() const {
//   return getImpl<detail::RefrigerationDefrostCycleParameters_Impl>()->defrost1HourStartTime();
// }

// boost::optional<int> RefrigerationDefrostCycleParameters::defrost1MinuteStartTime() const {
//   return getImpl<detail::RefrigerationDefrostCycleParameters_Impl>()->defrost1MinuteStartTime();
// }

// boost::optional<int> RefrigerationDefrostCycleParameters::defrost2HourStartTime() const {
//   return getImpl<detail::RefrigerationDefrostCycleParameters_Impl>()->defrost2HourStartTime();
// }

// boost::optional<int> RefrigerationDefrostCycleParameters::defrost2MinuteStartTime() const {
//   return getImpl<detail::RefrigerationDefrostCycleParameters_Impl>()->defrost2MinuteStartTime();
// }

// boost::optional<int> RefrigerationDefrostCycleParameters::defrost3HourStartTime() const {
//   return getImpl<detail::RefrigerationDefrostCycleParameters_Impl>()->defrost3HourStartTime();
// }

// boost::optional<int> RefrigerationDefrostCycleParameters::defrost3MinuteStartTime() const {
//   return getImpl<detail::RefrigerationDefrostCycleParameters_Impl>()->defrost3MinuteStartTime();
// }

// boost::optional<int> RefrigerationDefrostCycleParameters::defrost4HourStartTime() const {
//   return getImpl<detail::RefrigerationDefrostCycleParameters_Impl>()->defrost4HourStartTime();
// }

// boost::optional<int> RefrigerationDefrostCycleParameters::defrost4MinuteStartTime() const {
//   return getImpl<detail::RefrigerationDefrostCycleParameters_Impl>()->defrost4MinuteStartTime();
// }

// boost::optional<int> RefrigerationDefrostCycleParameters::defrost5HourStartTime() const {
//   return getImpl<detail::RefrigerationDefrostCycleParameters_Impl>()->defrost5HourStartTime();
// }

// boost::optional<int> RefrigerationDefrostCycleParameters::defrost5MinuteStartTime() const {
//   return getImpl<detail::RefrigerationDefrostCycleParameters_Impl>()->defrost5MinuteStartTime();
// }

// boost::optional<int> RefrigerationDefrostCycleParameters::defrost6HourStartTime() const {
//   return getImpl<detail::RefrigerationDefrostCycleParameters_Impl>()->defrost6HourStartTime();
// }

// boost::optional<int> RefrigerationDefrostCycleParameters::defrost6MinuteStartTime() const {
//   return getImpl<detail::RefrigerationDefrostCycleParameters_Impl>()->defrost6MinuteStartTime();
// }

// boost::optional<int> RefrigerationDefrostCycleParameters::defrost7HourStartTime() const {
//   return getImpl<detail::RefrigerationDefrostCycleParameters_Impl>()->defrost7HourStartTime();
// }

// boost::optional<int> RefrigerationDefrostCycleParameters::defrost7MinuteStartTime() const {
//   return getImpl<detail::RefrigerationDefrostCycleParameters_Impl>()->defrost7MinuteStartTime();
// }

// boost::optional<int> RefrigerationDefrostCycleParameters::defrost8HourStartTime() const {
//   return getImpl<detail::RefrigerationDefrostCycleParameters_Impl>()->defrost8HourStartTime();
// }

// boost::optional<int> RefrigerationDefrostCycleParameters::defrost8MinuteStartTime() const {
//   return getImpl<detail::RefrigerationDefrostCycleParameters_Impl>()->defrost8MinuteStartTime();
// }

bool RefrigerationDefrostCycleParameters::setDurationofDefrostCycle(int durationofDefrostCycle) {
  return getImpl<detail::RefrigerationDefrostCycleParameters_Impl>()->setDurationofDefrostCycle(durationofDefrostCycle);
}

void RefrigerationDefrostCycleParameters::resetDurationofDefrostCycle() {
  getImpl<detail::RefrigerationDefrostCycleParameters_Impl>()->resetDurationofDefrostCycle();
}

bool RefrigerationDefrostCycleParameters::setDripDownTime(int dripDownTime) {
  return getImpl<detail::RefrigerationDefrostCycleParameters_Impl>()->setDripDownTime(dripDownTime);
}

void RefrigerationDefrostCycleParameters::resetDripDownTime() {
  getImpl<detail::RefrigerationDefrostCycleParameters_Impl>()->resetDripDownTime();
}

bool RefrigerationDefrostCycleParameters::setDefrost1StartTime(const openstudio::Time& defrost1StartTime) {
  return getImpl<detail::RefrigerationDefrostCycleParameters_Impl>()->setDefrost1StartTime(defrost1StartTime);
}

bool RefrigerationDefrostCycleParameters::setDefrost2StartTime(const openstudio::Time& defrost2StartTime) {
  return getImpl<detail::RefrigerationDefrostCycleParameters_Impl>()->setDefrost2StartTime(defrost2StartTime);
}

bool RefrigerationDefrostCycleParameters::setDefrost3StartTime(const openstudio::Time& defrost3StartTime) {
  return getImpl<detail::RefrigerationDefrostCycleParameters_Impl>()->setDefrost3StartTime(defrost3StartTime);
}

bool RefrigerationDefrostCycleParameters::setDefrost4StartTime(const openstudio::Time& defrost4StartTime) {
  return getImpl<detail::RefrigerationDefrostCycleParameters_Impl>()->setDefrost4StartTime(defrost4StartTime);
}

bool RefrigerationDefrostCycleParameters::setDefrost5StartTime(const openstudio::Time& defrost5StartTime) {
  return getImpl<detail::RefrigerationDefrostCycleParameters_Impl>()->setDefrost5StartTime(defrost5StartTime);
}

bool RefrigerationDefrostCycleParameters::setDefrost6StartTime(const openstudio::Time& defrost6StartTime) {
  return getImpl<detail::RefrigerationDefrostCycleParameters_Impl>()->setDefrost6StartTime(defrost6StartTime);
}

bool RefrigerationDefrostCycleParameters::setDefrost7StartTime(const openstudio::Time& defrost7StartTime) {
  return getImpl<detail::RefrigerationDefrostCycleParameters_Impl>()->setDefrost7StartTime(defrost7StartTime);
}

bool RefrigerationDefrostCycleParameters::setDefrost8StartTime(const openstudio::Time& defrost8StartTime) {
  return getImpl<detail::RefrigerationDefrostCycleParameters_Impl>()->setDefrost8StartTime(defrost8StartTime);
}

// bool RefrigerationDefrostCycleParameters::setDefrost1HourStartTime(int defrost1HourStartTime) {
//   return getImpl<detail::RefrigerationDefrostCycleParameters_Impl>()->setDefrost1HourStartTime(defrost1HourStartTime);
// }

// void RefrigerationDefrostCycleParameters::resetDefrost1HourStartTime() {
//   getImpl<detail::RefrigerationDefrostCycleParameters_Impl>()->resetDefrost1HourStartTime();
// }

// bool RefrigerationDefrostCycleParameters::setDefrost1MinuteStartTime(int defrost1MinuteStartTime) {
//   return getImpl<detail::RefrigerationDefrostCycleParameters_Impl>()->setDefrost1MinuteStartTime(defrost1MinuteStartTime);
// }

// void RefrigerationDefrostCycleParameters::resetDefrost1MinuteStartTime() {
//   getImpl<detail::RefrigerationDefrostCycleParameters_Impl>()->resetDefrost1MinuteStartTime();
// }

// bool RefrigerationDefrostCycleParameters::setDefrost2HourStartTime(int defrost2HourStartTime) {
//   return getImpl<detail::RefrigerationDefrostCycleParameters_Impl>()->setDefrost2HourStartTime(defrost2HourStartTime);
// }

// void RefrigerationDefrostCycleParameters::resetDefrost2HourStartTime() {
//   getImpl<detail::RefrigerationDefrostCycleParameters_Impl>()->resetDefrost2HourStartTime();
// }

// bool RefrigerationDefrostCycleParameters::setDefrost2MinuteStartTime(int defrost2MinuteStartTime) {
//   return getImpl<detail::RefrigerationDefrostCycleParameters_Impl>()->setDefrost2MinuteStartTime(defrost2MinuteStartTime);
// }

// void RefrigerationDefrostCycleParameters::resetDefrost2MinuteStartTime() {
//   getImpl<detail::RefrigerationDefrostCycleParameters_Impl>()->resetDefrost2MinuteStartTime();
// }

// bool RefrigerationDefrostCycleParameters::setDefrost3HourStartTime(int defrost3HourStartTime) {
//   return getImpl<detail::RefrigerationDefrostCycleParameters_Impl>()->setDefrost3HourStartTime(defrost3HourStartTime);
// }

// void RefrigerationDefrostCycleParameters::resetDefrost3HourStartTime() {
//   getImpl<detail::RefrigerationDefrostCycleParameters_Impl>()->resetDefrost3HourStartTime();
// }

// bool RefrigerationDefrostCycleParameters::setDefrost3MinuteStartTime(int defrost3MinuteStartTime) {
//   return getImpl<detail::RefrigerationDefrostCycleParameters_Impl>()->setDefrost3MinuteStartTime(defrost3MinuteStartTime);
// }

// void RefrigerationDefrostCycleParameters::resetDefrost3MinuteStartTime() {
//   getImpl<detail::RefrigerationDefrostCycleParameters_Impl>()->resetDefrost3MinuteStartTime();
// }

// bool RefrigerationDefrostCycleParameters::setDefrost4HourStartTime(int defrost4HourStartTime) {
//   return getImpl<detail::RefrigerationDefrostCycleParameters_Impl>()->setDefrost4HourStartTime(defrost4HourStartTime);
// }

// void RefrigerationDefrostCycleParameters::resetDefrost4HourStartTime() {
//   getImpl<detail::RefrigerationDefrostCycleParameters_Impl>()->resetDefrost4HourStartTime();
// }

// bool RefrigerationDefrostCycleParameters::setDefrost4MinuteStartTime(int defrost4MinuteStartTime) {
//   return getImpl<detail::RefrigerationDefrostCycleParameters_Impl>()->setDefrost4MinuteStartTime(defrost4MinuteStartTime);
// }

// void RefrigerationDefrostCycleParameters::resetDefrost4MinuteStartTime() {
//   getImpl<detail::RefrigerationDefrostCycleParameters_Impl>()->resetDefrost4MinuteStartTime();
// }

// bool RefrigerationDefrostCycleParameters::setDefrost5HourStartTime(int defrost5HourStartTime) {
//   return getImpl<detail::RefrigerationDefrostCycleParameters_Impl>()->setDefrost5HourStartTime(defrost5HourStartTime);
// }

// void RefrigerationDefrostCycleParameters::resetDefrost5HourStartTime() {
//   getImpl<detail::RefrigerationDefrostCycleParameters_Impl>()->resetDefrost5HourStartTime();
// }

// bool RefrigerationDefrostCycleParameters::setDefrost5MinuteStartTime(int defrost5MinuteStartTime) {
//   return getImpl<detail::RefrigerationDefrostCycleParameters_Impl>()->setDefrost5MinuteStartTime(defrost5MinuteStartTime);
// }

// void RefrigerationDefrostCycleParameters::resetDefrost5MinuteStartTime() {
//   getImpl<detail::RefrigerationDefrostCycleParameters_Impl>()->resetDefrost5MinuteStartTime();
// }

// bool RefrigerationDefrostCycleParameters::setDefrost6HourStartTime(int defrost6HourStartTime) {
//   return getImpl<detail::RefrigerationDefrostCycleParameters_Impl>()->setDefrost6HourStartTime(defrost6HourStartTime);
// }

// void RefrigerationDefrostCycleParameters::resetDefrost6HourStartTime() {
//   getImpl<detail::RefrigerationDefrostCycleParameters_Impl>()->resetDefrost6HourStartTime();
// }

// bool RefrigerationDefrostCycleParameters::setDefrost6MinuteStartTime(int defrost6MinuteStartTime) {
//   return getImpl<detail::RefrigerationDefrostCycleParameters_Impl>()->setDefrost6MinuteStartTime(defrost6MinuteStartTime);
// }

// void RefrigerationDefrostCycleParameters::resetDefrost6MinuteStartTime() {
//   getImpl<detail::RefrigerationDefrostCycleParameters_Impl>()->resetDefrost6MinuteStartTime();
// }

// bool RefrigerationDefrostCycleParameters::setDefrost7HourStartTime(int defrost7HourStartTime) {
//   return getImpl<detail::RefrigerationDefrostCycleParameters_Impl>()->setDefrost7HourStartTime(defrost7HourStartTime);
// }

// void RefrigerationDefrostCycleParameters::resetDefrost7HourStartTime() {
//   getImpl<detail::RefrigerationDefrostCycleParameters_Impl>()->resetDefrost7HourStartTime();
// }

// bool RefrigerationDefrostCycleParameters::setDefrost7MinuteStartTime(int defrost7MinuteStartTime) {
//   return getImpl<detail::RefrigerationDefrostCycleParameters_Impl>()->setDefrost7MinuteStartTime(defrost7MinuteStartTime);
// }

// void RefrigerationDefrostCycleParameters::resetDefrost7MinuteStartTime() {
//   getImpl<detail::RefrigerationDefrostCycleParameters_Impl>()->resetDefrost7MinuteStartTime();
// }

// bool RefrigerationDefrostCycleParameters::setDefrost8HourStartTime(int defrost8HourStartTime) {
//   return getImpl<detail::RefrigerationDefrostCycleParameters_Impl>()->setDefrost8HourStartTime(defrost8HourStartTime);
// }

// void RefrigerationDefrostCycleParameters::resetDefrost8HourStartTime() {
//   getImpl<detail::RefrigerationDefrostCycleParameters_Impl>()->resetDefrost8HourStartTime();
// }

// bool RefrigerationDefrostCycleParameters::setDefrost8MinuteStartTime(int defrost8MinuteStartTime) {
//   return getImpl<detail::RefrigerationDefrostCycleParameters_Impl>()->setDefrost8MinuteStartTime(defrost8MinuteStartTime);
// }

// void RefrigerationDefrostCycleParameters::resetDefrost8MinuteStartTime() {
//   getImpl<detail::RefrigerationDefrostCycleParameters_Impl>()->resetDefrost8MinuteStartTime();
// }

/// @cond
RefrigerationDefrostCycleParameters::RefrigerationDefrostCycleParameters(boost::shared_ptr<detail::RefrigerationDefrostCycleParameters_Impl> impl)
  : ModelObject(impl)
{}
/// @endcond

} // model
} // openstudio

