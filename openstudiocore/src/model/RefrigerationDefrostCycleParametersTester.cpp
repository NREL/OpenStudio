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

#include <model/RefrigerationDefrostCycleParametersTester.hpp>
#include <model/RefrigerationDefrostCycleParametersTester_Impl.hpp>

#include <utilities/idd/OS_Refrigeration_DefrostCycleParameters_FieldEnums.hxx>

#include <utilities/core/Assert.hpp>

namespace openstudio {
namespace model {

namespace detail {

  RefrigerationDefrostCycleParametersTester_Impl::RefrigerationDefrostCycleParametersTester_Impl(const IdfObject& idfObject,
                                                                                                 Model_Impl* model,
                                                                                                 bool keepHandle)
    : ModelObject_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == RefrigerationDefrostCycleParametersTester::iddObjectType());
  }

  RefrigerationDefrostCycleParametersTester_Impl::RefrigerationDefrostCycleParametersTester_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                                                 Model_Impl* model,
                                                                                                 bool keepHandle)
    : ModelObject_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == RefrigerationDefrostCycleParametersTester::iddObjectType());
  }

  RefrigerationDefrostCycleParametersTester_Impl::RefrigerationDefrostCycleParametersTester_Impl(const RefrigerationDefrostCycleParametersTester_Impl& other,
                                                                                                 Model_Impl* model,
                                                                                                 bool keepHandle)
    : ModelObject_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& RefrigerationDefrostCycleParametersTester_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType RefrigerationDefrostCycleParametersTester_Impl::iddObjectType() const {
    return RefrigerationDefrostCycleParametersTester::iddObjectType();
  }

  boost::optional<int> RefrigerationDefrostCycleParametersTester_Impl::durationofDefrostCycle() const {
    return getInt(OS_Refrigeration_DefrostCycleParametersFields::DurationofDefrostCycle,true);
  }

  boost::optional<int> RefrigerationDefrostCycleParametersTester_Impl::dripDownTime() const {
    return getInt(OS_Refrigeration_DefrostCycleParametersFields::DripDownTime,true);
  }

  boost::optional<int> RefrigerationDefrostCycleParametersTester_Impl::defrost1HourStartTime() const {
    return getInt(OS_Refrigeration_DefrostCycleParametersFields::Defrost1HourStartTime,true);
  }

  boost::optional<int> RefrigerationDefrostCycleParametersTester_Impl::defrost1MinuteStartTime() const {
    return getInt(OS_Refrigeration_DefrostCycleParametersFields::Defrost1MinuteStartTime,true);
  }

  boost::optional<int> RefrigerationDefrostCycleParametersTester_Impl::defrost2HourStartTime() const {
    return getInt(OS_Refrigeration_DefrostCycleParametersFields::Defrost2HourStartTime,true);
  }

  boost::optional<int> RefrigerationDefrostCycleParametersTester_Impl::defrost2MinuteStartTime() const {
    return getInt(OS_Refrigeration_DefrostCycleParametersFields::Defrost2MinuteStartTime,true);
  }

  boost::optional<int> RefrigerationDefrostCycleParametersTester_Impl::defrost3HourStartTime() const {
    return getInt(OS_Refrigeration_DefrostCycleParametersFields::Defrost3HourStartTime,true);
  }

  boost::optional<int> RefrigerationDefrostCycleParametersTester_Impl::defrost3MinuteStartTime() const {
    return getInt(OS_Refrigeration_DefrostCycleParametersFields::Defrost3MinuteStartTime,true);
  }

  boost::optional<int> RefrigerationDefrostCycleParametersTester_Impl::defrost4HourStartTime() const {
    return getInt(OS_Refrigeration_DefrostCycleParametersFields::Defrost4HourStartTime,true);
  }

  boost::optional<int> RefrigerationDefrostCycleParametersTester_Impl::defrost4MinuteStartTime() const {
    return getInt(OS_Refrigeration_DefrostCycleParametersFields::Defrost4MinuteStartTime,true);
  }

  boost::optional<int> RefrigerationDefrostCycleParametersTester_Impl::defrost5HourStartTime() const {
    return getInt(OS_Refrigeration_DefrostCycleParametersFields::Defrost5HourStartTime,true);
  }

  boost::optional<int> RefrigerationDefrostCycleParametersTester_Impl::defrost5MinuteStartTime() const {
    return getInt(OS_Refrigeration_DefrostCycleParametersFields::Defrost5MinuteStartTime,true);
  }

  boost::optional<int> RefrigerationDefrostCycleParametersTester_Impl::defrost6HourStartTime() const {
    return getInt(OS_Refrigeration_DefrostCycleParametersFields::Defrost6HourStartTime,true);
  }

  boost::optional<int> RefrigerationDefrostCycleParametersTester_Impl::defrost6MinuteStartTime() const {
    return getInt(OS_Refrigeration_DefrostCycleParametersFields::Defrost6MinuteStartTime,true);
  }

  boost::optional<int> RefrigerationDefrostCycleParametersTester_Impl::defrost7HourStartTime() const {
    return getInt(OS_Refrigeration_DefrostCycleParametersFields::Defrost7HourStartTime,true);
  }

  boost::optional<int> RefrigerationDefrostCycleParametersTester_Impl::defrost7MinuteStartTime() const {
    return getInt(OS_Refrigeration_DefrostCycleParametersFields::Defrost7MinuteStartTime,true);
  }

  boost::optional<int> RefrigerationDefrostCycleParametersTester_Impl::defrost8HourStartTime() const {
    return getInt(OS_Refrigeration_DefrostCycleParametersFields::Defrost8HourStartTime,true);
  }

  boost::optional<int> RefrigerationDefrostCycleParametersTester_Impl::defrost8MinuteStartTime() const {
    return getInt(OS_Refrigeration_DefrostCycleParametersFields::Defrost8MinuteStartTime,true);
  }

  bool RefrigerationDefrostCycleParametersTester_Impl::setDurationofDefrostCycle(boost::optional<int> durationofDefrostCycle) {
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

  void RefrigerationDefrostCycleParametersTester_Impl::resetDurationofDefrostCycle() {
    bool result = setString(OS_Refrigeration_DefrostCycleParametersFields::DurationofDefrostCycle, "");
    OS_ASSERT(result);
  }

  bool RefrigerationDefrostCycleParametersTester_Impl::setDripDownTime(boost::optional<int> dripDownTime) {
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

  void RefrigerationDefrostCycleParametersTester_Impl::resetDripDownTime() {
    bool result = setString(OS_Refrigeration_DefrostCycleParametersFields::DripDownTime, "");
    OS_ASSERT(result);
  }

  bool RefrigerationDefrostCycleParametersTester_Impl::setDefrost1HourStartTime(boost::optional<int> defrost1HourStartTime) {
    bool result(false);
    if (defrost1HourStartTime) {
      result = setInt(OS_Refrigeration_DefrostCycleParametersFields::Defrost1HourStartTime, defrost1HourStartTime.get());
    }
    else {
      resetDefrost1HourStartTime();
      result = true;
    }
    return result;
  }

  void RefrigerationDefrostCycleParametersTester_Impl::resetDefrost1HourStartTime() {
    bool result = setString(OS_Refrigeration_DefrostCycleParametersFields::Defrost1HourStartTime, "");
    OS_ASSERT(result);
  }

  bool RefrigerationDefrostCycleParametersTester_Impl::setDefrost1MinuteStartTime(boost::optional<int> defrost1MinuteStartTime) {
    bool result(false);
    if (defrost1MinuteStartTime) {
      result = setInt(OS_Refrigeration_DefrostCycleParametersFields::Defrost1MinuteStartTime, defrost1MinuteStartTime.get());
    }
    else {
      resetDefrost1MinuteStartTime();
      result = true;
    }
    return result;
  }

  void RefrigerationDefrostCycleParametersTester_Impl::resetDefrost1MinuteStartTime() {
    bool result = setString(OS_Refrigeration_DefrostCycleParametersFields::Defrost1MinuteStartTime, "");
    OS_ASSERT(result);
  }

  bool RefrigerationDefrostCycleParametersTester_Impl::setDefrost2HourStartTime(boost::optional<int> defrost2HourStartTime) {
    bool result(false);
    if (defrost2HourStartTime) {
      result = setInt(OS_Refrigeration_DefrostCycleParametersFields::Defrost2HourStartTime, defrost2HourStartTime.get());
    }
    else {
      resetDefrost2HourStartTime();
      result = true;
    }
    return result;
  }

  void RefrigerationDefrostCycleParametersTester_Impl::resetDefrost2HourStartTime() {
    bool result = setString(OS_Refrigeration_DefrostCycleParametersFields::Defrost2HourStartTime, "");
    OS_ASSERT(result);
  }

  bool RefrigerationDefrostCycleParametersTester_Impl::setDefrost2MinuteStartTime(boost::optional<int> defrost2MinuteStartTime) {
    bool result(false);
    if (defrost2MinuteStartTime) {
      result = setInt(OS_Refrigeration_DefrostCycleParametersFields::Defrost2MinuteStartTime, defrost2MinuteStartTime.get());
    }
    else {
      resetDefrost2MinuteStartTime();
      result = true;
    }
    return result;
  }

  void RefrigerationDefrostCycleParametersTester_Impl::resetDefrost2MinuteStartTime() {
    bool result = setString(OS_Refrigeration_DefrostCycleParametersFields::Defrost2MinuteStartTime, "");
    OS_ASSERT(result);
  }

  bool RefrigerationDefrostCycleParametersTester_Impl::setDefrost3HourStartTime(boost::optional<int> defrost3HourStartTime) {
    bool result(false);
    if (defrost3HourStartTime) {
      result = setInt(OS_Refrigeration_DefrostCycleParametersFields::Defrost3HourStartTime, defrost3HourStartTime.get());
    }
    else {
      resetDefrost3HourStartTime();
      result = true;
    }
    return result;
  }

  void RefrigerationDefrostCycleParametersTester_Impl::resetDefrost3HourStartTime() {
    bool result = setString(OS_Refrigeration_DefrostCycleParametersFields::Defrost3HourStartTime, "");
    OS_ASSERT(result);
  }

  bool RefrigerationDefrostCycleParametersTester_Impl::setDefrost3MinuteStartTime(boost::optional<int> defrost3MinuteStartTime) {
    bool result(false);
    if (defrost3MinuteStartTime) {
      result = setInt(OS_Refrigeration_DefrostCycleParametersFields::Defrost3MinuteStartTime, defrost3MinuteStartTime.get());
    }
    else {
      resetDefrost3MinuteStartTime();
      result = true;
    }
    return result;
  }

  void RefrigerationDefrostCycleParametersTester_Impl::resetDefrost3MinuteStartTime() {
    bool result = setString(OS_Refrigeration_DefrostCycleParametersFields::Defrost3MinuteStartTime, "");
    OS_ASSERT(result);
  }

  bool RefrigerationDefrostCycleParametersTester_Impl::setDefrost4HourStartTime(boost::optional<int> defrost4HourStartTime) {
    bool result(false);
    if (defrost4HourStartTime) {
      result = setInt(OS_Refrigeration_DefrostCycleParametersFields::Defrost4HourStartTime, defrost4HourStartTime.get());
    }
    else {
      resetDefrost4HourStartTime();
      result = true;
    }
    return result;
  }

  void RefrigerationDefrostCycleParametersTester_Impl::resetDefrost4HourStartTime() {
    bool result = setString(OS_Refrigeration_DefrostCycleParametersFields::Defrost4HourStartTime, "");
    OS_ASSERT(result);
  }

  bool RefrigerationDefrostCycleParametersTester_Impl::setDefrost4MinuteStartTime(boost::optional<int> defrost4MinuteStartTime) {
    bool result(false);
    if (defrost4MinuteStartTime) {
      result = setInt(OS_Refrigeration_DefrostCycleParametersFields::Defrost4MinuteStartTime, defrost4MinuteStartTime.get());
    }
    else {
      resetDefrost4MinuteStartTime();
      result = true;
    }
    return result;
  }

  void RefrigerationDefrostCycleParametersTester_Impl::resetDefrost4MinuteStartTime() {
    bool result = setString(OS_Refrigeration_DefrostCycleParametersFields::Defrost4MinuteStartTime, "");
    OS_ASSERT(result);
  }

  bool RefrigerationDefrostCycleParametersTester_Impl::setDefrost5HourStartTime(boost::optional<int> defrost5HourStartTime) {
    bool result(false);
    if (defrost5HourStartTime) {
      result = setInt(OS_Refrigeration_DefrostCycleParametersFields::Defrost5HourStartTime, defrost5HourStartTime.get());
    }
    else {
      resetDefrost5HourStartTime();
      result = true;
    }
    return result;
  }

  void RefrigerationDefrostCycleParametersTester_Impl::resetDefrost5HourStartTime() {
    bool result = setString(OS_Refrigeration_DefrostCycleParametersFields::Defrost5HourStartTime, "");
    OS_ASSERT(result);
  }

  bool RefrigerationDefrostCycleParametersTester_Impl::setDefrost5MinuteStartTime(boost::optional<int> defrost5MinuteStartTime) {
    bool result(false);
    if (defrost5MinuteStartTime) {
      result = setInt(OS_Refrigeration_DefrostCycleParametersFields::Defrost5MinuteStartTime, defrost5MinuteStartTime.get());
    }
    else {
      resetDefrost5MinuteStartTime();
      result = true;
    }
    return result;
  }

  void RefrigerationDefrostCycleParametersTester_Impl::resetDefrost5MinuteStartTime() {
    bool result = setString(OS_Refrigeration_DefrostCycleParametersFields::Defrost5MinuteStartTime, "");
    OS_ASSERT(result);
  }

  bool RefrigerationDefrostCycleParametersTester_Impl::setDefrost6HourStartTime(boost::optional<int> defrost6HourStartTime) {
    bool result(false);
    if (defrost6HourStartTime) {
      result = setInt(OS_Refrigeration_DefrostCycleParametersFields::Defrost6HourStartTime, defrost6HourStartTime.get());
    }
    else {
      resetDefrost6HourStartTime();
      result = true;
    }
    return result;
  }

  void RefrigerationDefrostCycleParametersTester_Impl::resetDefrost6HourStartTime() {
    bool result = setString(OS_Refrigeration_DefrostCycleParametersFields::Defrost6HourStartTime, "");
    OS_ASSERT(result);
  }

  bool RefrigerationDefrostCycleParametersTester_Impl::setDefrost6MinuteStartTime(boost::optional<int> defrost6MinuteStartTime) {
    bool result(false);
    if (defrost6MinuteStartTime) {
      result = setInt(OS_Refrigeration_DefrostCycleParametersFields::Defrost6MinuteStartTime, defrost6MinuteStartTime.get());
    }
    else {
      resetDefrost6MinuteStartTime();
      result = true;
    }
    return result;
  }

  void RefrigerationDefrostCycleParametersTester_Impl::resetDefrost6MinuteStartTime() {
    bool result = setString(OS_Refrigeration_DefrostCycleParametersFields::Defrost6MinuteStartTime, "");
    OS_ASSERT(result);
  }

  bool RefrigerationDefrostCycleParametersTester_Impl::setDefrost7HourStartTime(boost::optional<int> defrost7HourStartTime) {
    bool result(false);
    if (defrost7HourStartTime) {
      result = setInt(OS_Refrigeration_DefrostCycleParametersFields::Defrost7HourStartTime, defrost7HourStartTime.get());
    }
    else {
      resetDefrost7HourStartTime();
      result = true;
    }
    return result;
  }

  void RefrigerationDefrostCycleParametersTester_Impl::resetDefrost7HourStartTime() {
    bool result = setString(OS_Refrigeration_DefrostCycleParametersFields::Defrost7HourStartTime, "");
    OS_ASSERT(result);
  }

  bool RefrigerationDefrostCycleParametersTester_Impl::setDefrost7MinuteStartTime(boost::optional<int> defrost7MinuteStartTime) {
    bool result(false);
    if (defrost7MinuteStartTime) {
      result = setInt(OS_Refrigeration_DefrostCycleParametersFields::Defrost7MinuteStartTime, defrost7MinuteStartTime.get());
    }
    else {
      resetDefrost7MinuteStartTime();
      result = true;
    }
    return result;
  }

  void RefrigerationDefrostCycleParametersTester_Impl::resetDefrost7MinuteStartTime() {
    bool result = setString(OS_Refrigeration_DefrostCycleParametersFields::Defrost7MinuteStartTime, "");
    OS_ASSERT(result);
  }

  bool RefrigerationDefrostCycleParametersTester_Impl::setDefrost8HourStartTime(boost::optional<int> defrost8HourStartTime) {
    bool result(false);
    if (defrost8HourStartTime) {
      result = setInt(OS_Refrigeration_DefrostCycleParametersFields::Defrost8HourStartTime, defrost8HourStartTime.get());
    }
    else {
      resetDefrost8HourStartTime();
      result = true;
    }
    return result;
  }

  void RefrigerationDefrostCycleParametersTester_Impl::resetDefrost8HourStartTime() {
    bool result = setString(OS_Refrigeration_DefrostCycleParametersFields::Defrost8HourStartTime, "");
    OS_ASSERT(result);
  }

  bool RefrigerationDefrostCycleParametersTester_Impl::setDefrost8MinuteStartTime(boost::optional<int> defrost8MinuteStartTime) {
    bool result(false);
    if (defrost8MinuteStartTime) {
      result = setInt(OS_Refrigeration_DefrostCycleParametersFields::Defrost8MinuteStartTime, defrost8MinuteStartTime.get());
    }
    else {
      resetDefrost8MinuteStartTime();
      result = true;
    }
    return result;
  }

  void RefrigerationDefrostCycleParametersTester_Impl::resetDefrost8MinuteStartTime() {
    bool result = setString(OS_Refrigeration_DefrostCycleParametersFields::Defrost8MinuteStartTime, "");
    OS_ASSERT(result);
  }

} // detail

RefrigerationDefrostCycleParametersTester::RefrigerationDefrostCycleParametersTester(const Model& model)
  : ModelObject(RefrigerationDefrostCycleParametersTester::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::RefrigerationDefrostCycleParametersTester_Impl>());

  // TODO: Appropriately handle the following required object-list fields.
}

IddObjectType RefrigerationDefrostCycleParametersTester::iddObjectType() {
  return IddObjectType(IddObjectType::OS_Refrigeration_DefrostCycleParameters);
}

boost::optional<int> RefrigerationDefrostCycleParametersTester::durationofDefrostCycle() const {
  return getImpl<detail::RefrigerationDefrostCycleParametersTester_Impl>()->durationofDefrostCycle();
}

boost::optional<int> RefrigerationDefrostCycleParametersTester::dripDownTime() const {
  return getImpl<detail::RefrigerationDefrostCycleParametersTester_Impl>()->dripDownTime();
}

boost::optional<int> RefrigerationDefrostCycleParametersTester::defrost1HourStartTime() const {
  return getImpl<detail::RefrigerationDefrostCycleParametersTester_Impl>()->defrost1HourStartTime();
}

boost::optional<int> RefrigerationDefrostCycleParametersTester::defrost1MinuteStartTime() const {
  return getImpl<detail::RefrigerationDefrostCycleParametersTester_Impl>()->defrost1MinuteStartTime();
}

boost::optional<int> RefrigerationDefrostCycleParametersTester::defrost2HourStartTime() const {
  return getImpl<detail::RefrigerationDefrostCycleParametersTester_Impl>()->defrost2HourStartTime();
}

boost::optional<int> RefrigerationDefrostCycleParametersTester::defrost2MinuteStartTime() const {
  return getImpl<detail::RefrigerationDefrostCycleParametersTester_Impl>()->defrost2MinuteStartTime();
}

boost::optional<int> RefrigerationDefrostCycleParametersTester::defrost3HourStartTime() const {
  return getImpl<detail::RefrigerationDefrostCycleParametersTester_Impl>()->defrost3HourStartTime();
}

boost::optional<int> RefrigerationDefrostCycleParametersTester::defrost3MinuteStartTime() const {
  return getImpl<detail::RefrigerationDefrostCycleParametersTester_Impl>()->defrost3MinuteStartTime();
}

boost::optional<int> RefrigerationDefrostCycleParametersTester::defrost4HourStartTime() const {
  return getImpl<detail::RefrigerationDefrostCycleParametersTester_Impl>()->defrost4HourStartTime();
}

boost::optional<int> RefrigerationDefrostCycleParametersTester::defrost4MinuteStartTime() const {
  return getImpl<detail::RefrigerationDefrostCycleParametersTester_Impl>()->defrost4MinuteStartTime();
}

boost::optional<int> RefrigerationDefrostCycleParametersTester::defrost5HourStartTime() const {
  return getImpl<detail::RefrigerationDefrostCycleParametersTester_Impl>()->defrost5HourStartTime();
}

boost::optional<int> RefrigerationDefrostCycleParametersTester::defrost5MinuteStartTime() const {
  return getImpl<detail::RefrigerationDefrostCycleParametersTester_Impl>()->defrost5MinuteStartTime();
}

boost::optional<int> RefrigerationDefrostCycleParametersTester::defrost6HourStartTime() const {
  return getImpl<detail::RefrigerationDefrostCycleParametersTester_Impl>()->defrost6HourStartTime();
}

boost::optional<int> RefrigerationDefrostCycleParametersTester::defrost6MinuteStartTime() const {
  return getImpl<detail::RefrigerationDefrostCycleParametersTester_Impl>()->defrost6MinuteStartTime();
}

boost::optional<int> RefrigerationDefrostCycleParametersTester::defrost7HourStartTime() const {
  return getImpl<detail::RefrigerationDefrostCycleParametersTester_Impl>()->defrost7HourStartTime();
}

boost::optional<int> RefrigerationDefrostCycleParametersTester::defrost7MinuteStartTime() const {
  return getImpl<detail::RefrigerationDefrostCycleParametersTester_Impl>()->defrost7MinuteStartTime();
}

boost::optional<int> RefrigerationDefrostCycleParametersTester::defrost8HourStartTime() const {
  return getImpl<detail::RefrigerationDefrostCycleParametersTester_Impl>()->defrost8HourStartTime();
}

boost::optional<int> RefrigerationDefrostCycleParametersTester::defrost8MinuteStartTime() const {
  return getImpl<detail::RefrigerationDefrostCycleParametersTester_Impl>()->defrost8MinuteStartTime();
}

bool RefrigerationDefrostCycleParametersTester::setDurationofDefrostCycle(int durationofDefrostCycle) {
  return getImpl<detail::RefrigerationDefrostCycleParametersTester_Impl>()->setDurationofDefrostCycle(durationofDefrostCycle);
}

void RefrigerationDefrostCycleParametersTester::resetDurationofDefrostCycle() {
  getImpl<detail::RefrigerationDefrostCycleParametersTester_Impl>()->resetDurationofDefrostCycle();
}

bool RefrigerationDefrostCycleParametersTester::setDripDownTime(int dripDownTime) {
  return getImpl<detail::RefrigerationDefrostCycleParametersTester_Impl>()->setDripDownTime(dripDownTime);
}

void RefrigerationDefrostCycleParametersTester::resetDripDownTime() {
  getImpl<detail::RefrigerationDefrostCycleParametersTester_Impl>()->resetDripDownTime();
}

bool RefrigerationDefrostCycleParametersTester::setDefrost1HourStartTime(int defrost1HourStartTime) {
  return getImpl<detail::RefrigerationDefrostCycleParametersTester_Impl>()->setDefrost1HourStartTime(defrost1HourStartTime);
}

void RefrigerationDefrostCycleParametersTester::resetDefrost1HourStartTime() {
  getImpl<detail::RefrigerationDefrostCycleParametersTester_Impl>()->resetDefrost1HourStartTime();
}

bool RefrigerationDefrostCycleParametersTester::setDefrost1MinuteStartTime(int defrost1MinuteStartTime) {
  return getImpl<detail::RefrigerationDefrostCycleParametersTester_Impl>()->setDefrost1MinuteStartTime(defrost1MinuteStartTime);
}

void RefrigerationDefrostCycleParametersTester::resetDefrost1MinuteStartTime() {
  getImpl<detail::RefrigerationDefrostCycleParametersTester_Impl>()->resetDefrost1MinuteStartTime();
}

bool RefrigerationDefrostCycleParametersTester::setDefrost2HourStartTime(int defrost2HourStartTime) {
  return getImpl<detail::RefrigerationDefrostCycleParametersTester_Impl>()->setDefrost2HourStartTime(defrost2HourStartTime);
}

void RefrigerationDefrostCycleParametersTester::resetDefrost2HourStartTime() {
  getImpl<detail::RefrigerationDefrostCycleParametersTester_Impl>()->resetDefrost2HourStartTime();
}

bool RefrigerationDefrostCycleParametersTester::setDefrost2MinuteStartTime(int defrost2MinuteStartTime) {
  return getImpl<detail::RefrigerationDefrostCycleParametersTester_Impl>()->setDefrost2MinuteStartTime(defrost2MinuteStartTime);
}

void RefrigerationDefrostCycleParametersTester::resetDefrost2MinuteStartTime() {
  getImpl<detail::RefrigerationDefrostCycleParametersTester_Impl>()->resetDefrost2MinuteStartTime();
}

bool RefrigerationDefrostCycleParametersTester::setDefrost3HourStartTime(int defrost3HourStartTime) {
  return getImpl<detail::RefrigerationDefrostCycleParametersTester_Impl>()->setDefrost3HourStartTime(defrost3HourStartTime);
}

void RefrigerationDefrostCycleParametersTester::resetDefrost3HourStartTime() {
  getImpl<detail::RefrigerationDefrostCycleParametersTester_Impl>()->resetDefrost3HourStartTime();
}

bool RefrigerationDefrostCycleParametersTester::setDefrost3MinuteStartTime(int defrost3MinuteStartTime) {
  return getImpl<detail::RefrigerationDefrostCycleParametersTester_Impl>()->setDefrost3MinuteStartTime(defrost3MinuteStartTime);
}

void RefrigerationDefrostCycleParametersTester::resetDefrost3MinuteStartTime() {
  getImpl<detail::RefrigerationDefrostCycleParametersTester_Impl>()->resetDefrost3MinuteStartTime();
}

bool RefrigerationDefrostCycleParametersTester::setDefrost4HourStartTime(int defrost4HourStartTime) {
  return getImpl<detail::RefrigerationDefrostCycleParametersTester_Impl>()->setDefrost4HourStartTime(defrost4HourStartTime);
}

void RefrigerationDefrostCycleParametersTester::resetDefrost4HourStartTime() {
  getImpl<detail::RefrigerationDefrostCycleParametersTester_Impl>()->resetDefrost4HourStartTime();
}

bool RefrigerationDefrostCycleParametersTester::setDefrost4MinuteStartTime(int defrost4MinuteStartTime) {
  return getImpl<detail::RefrigerationDefrostCycleParametersTester_Impl>()->setDefrost4MinuteStartTime(defrost4MinuteStartTime);
}

void RefrigerationDefrostCycleParametersTester::resetDefrost4MinuteStartTime() {
  getImpl<detail::RefrigerationDefrostCycleParametersTester_Impl>()->resetDefrost4MinuteStartTime();
}

bool RefrigerationDefrostCycleParametersTester::setDefrost5HourStartTime(int defrost5HourStartTime) {
  return getImpl<detail::RefrigerationDefrostCycleParametersTester_Impl>()->setDefrost5HourStartTime(defrost5HourStartTime);
}

void RefrigerationDefrostCycleParametersTester::resetDefrost5HourStartTime() {
  getImpl<detail::RefrigerationDefrostCycleParametersTester_Impl>()->resetDefrost5HourStartTime();
}

bool RefrigerationDefrostCycleParametersTester::setDefrost5MinuteStartTime(int defrost5MinuteStartTime) {
  return getImpl<detail::RefrigerationDefrostCycleParametersTester_Impl>()->setDefrost5MinuteStartTime(defrost5MinuteStartTime);
}

void RefrigerationDefrostCycleParametersTester::resetDefrost5MinuteStartTime() {
  getImpl<detail::RefrigerationDefrostCycleParametersTester_Impl>()->resetDefrost5MinuteStartTime();
}

bool RefrigerationDefrostCycleParametersTester::setDefrost6HourStartTime(int defrost6HourStartTime) {
  return getImpl<detail::RefrigerationDefrostCycleParametersTester_Impl>()->setDefrost6HourStartTime(defrost6HourStartTime);
}

void RefrigerationDefrostCycleParametersTester::resetDefrost6HourStartTime() {
  getImpl<detail::RefrigerationDefrostCycleParametersTester_Impl>()->resetDefrost6HourStartTime();
}

bool RefrigerationDefrostCycleParametersTester::setDefrost6MinuteStartTime(int defrost6MinuteStartTime) {
  return getImpl<detail::RefrigerationDefrostCycleParametersTester_Impl>()->setDefrost6MinuteStartTime(defrost6MinuteStartTime);
}

void RefrigerationDefrostCycleParametersTester::resetDefrost6MinuteStartTime() {
  getImpl<detail::RefrigerationDefrostCycleParametersTester_Impl>()->resetDefrost6MinuteStartTime();
}

bool RefrigerationDefrostCycleParametersTester::setDefrost7HourStartTime(int defrost7HourStartTime) {
  return getImpl<detail::RefrigerationDefrostCycleParametersTester_Impl>()->setDefrost7HourStartTime(defrost7HourStartTime);
}

void RefrigerationDefrostCycleParametersTester::resetDefrost7HourStartTime() {
  getImpl<detail::RefrigerationDefrostCycleParametersTester_Impl>()->resetDefrost7HourStartTime();
}

bool RefrigerationDefrostCycleParametersTester::setDefrost7MinuteStartTime(int defrost7MinuteStartTime) {
  return getImpl<detail::RefrigerationDefrostCycleParametersTester_Impl>()->setDefrost7MinuteStartTime(defrost7MinuteStartTime);
}

void RefrigerationDefrostCycleParametersTester::resetDefrost7MinuteStartTime() {
  getImpl<detail::RefrigerationDefrostCycleParametersTester_Impl>()->resetDefrost7MinuteStartTime();
}

bool RefrigerationDefrostCycleParametersTester::setDefrost8HourStartTime(int defrost8HourStartTime) {
  return getImpl<detail::RefrigerationDefrostCycleParametersTester_Impl>()->setDefrost8HourStartTime(defrost8HourStartTime);
}

void RefrigerationDefrostCycleParametersTester::resetDefrost8HourStartTime() {
  getImpl<detail::RefrigerationDefrostCycleParametersTester_Impl>()->resetDefrost8HourStartTime();
}

bool RefrigerationDefrostCycleParametersTester::setDefrost8MinuteStartTime(int defrost8MinuteStartTime) {
  return getImpl<detail::RefrigerationDefrostCycleParametersTester_Impl>()->setDefrost8MinuteStartTime(defrost8MinuteStartTime);
}

void RefrigerationDefrostCycleParametersTester::resetDefrost8MinuteStartTime() {
  getImpl<detail::RefrigerationDefrostCycleParametersTester_Impl>()->resetDefrost8MinuteStartTime();
}

/// @cond
RefrigerationDefrostCycleParametersTester::RefrigerationDefrostCycleParametersTester(boost::shared_ptr<detail::RefrigerationDefrostCycleParametersTester_Impl> impl)
  : ModelObject(impl)
{}
/// @endcond

} // model
} // openstudio

