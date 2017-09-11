/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2017, Alliance for Sustainable Energy, LLC. All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 *  following conditions are met:
 *
 *  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
 *  disclaimer.
 *
 *  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *  following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 *  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote
 *  products derived from this software without specific prior written permission from the respective party.
 *
 *  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative
 *  works may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without
 *  specific prior written permission from Alliance for Sustainable Energy, LLC.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER, THE UNITED STATES GOVERNMENT, OR ANY CONTRIBUTORS BE LIABLE FOR
 *  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **********************************************************************************************************************/

#include "SetpointManagerOutdoorAirReset.hpp"
#include "SetpointManagerOutdoorAirReset_Impl.hpp"

#include "Node.hpp"
#include "Node_Impl.hpp"
#include "Model.hpp"
#include "Schedule.hpp"
#include "Schedule_Impl.hpp"
#include "ScheduleTypeLimits.hpp"
#include "ScheduleTypeRegistry.hpp"

#include <utilities/idd/IddFactory.hxx>

#include <utilities/idd/OS_SetpointManager_OutdoorAirReset_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include "../utilities/units/Unit.hpp"
#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  SetpointManagerOutdoorAirReset_Impl::SetpointManagerOutdoorAirReset_Impl(const IdfObject& idfObject,
                                                                           Model_Impl* model,
                                                                           bool keepHandle)
    : SetpointManager_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == SetpointManagerOutdoorAirReset::iddObjectType());
  }

  SetpointManagerOutdoorAirReset_Impl::SetpointManagerOutdoorAirReset_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                           Model_Impl* model,
                                                                           bool keepHandle)
    : SetpointManager_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == SetpointManagerOutdoorAirReset::iddObjectType());
  }

  SetpointManagerOutdoorAirReset_Impl::SetpointManagerOutdoorAirReset_Impl(const SetpointManagerOutdoorAirReset_Impl& other,
                                                                           Model_Impl* model,
                                                                           bool keepHandle)
    : SetpointManager_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& SetpointManagerOutdoorAirReset_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType SetpointManagerOutdoorAirReset_Impl::iddObjectType() const {
    return SetpointManagerOutdoorAirReset::iddObjectType();
  }

  bool SetpointManagerOutdoorAirReset_Impl::addToNode(Node & node) {
    bool added = SetpointManager_Impl::addToNode( node );
    if( added ) {
      return added;
    } else if( boost::optional<PlantLoop> plantLoop = node.plantLoop() ) {
      if( plantLoop->supplyComponent(node.handle()) ) {
        return this->setSetpointNode(node);
      }
    }
    return added;
  }

  std::vector<ScheduleTypeKey> SetpointManagerOutdoorAirReset_Impl::getScheduleTypeKeys(const Schedule& schedule) const
  {
    // TODO: Check schedule display names.
    std::vector<ScheduleTypeKey> result;
    UnsignedVector fieldIndices = getSourceIndices(schedule.handle());
    UnsignedVector::const_iterator b(fieldIndices.begin()), e(fieldIndices.end());
    if (std::find(b,e,OS_SetpointManager_OutdoorAirResetFields::ScheduleName) != e)
    {
      result.push_back(ScheduleTypeKey("SetpointManagerOutdoorAirReset","Setpoint Manager Outdoor Air Reset"));
    }
    return result;
  }

  std::string SetpointManagerOutdoorAirReset_Impl::controlVariable() const {
    boost::optional<std::string> value = getString(OS_SetpointManager_OutdoorAirResetFields::ControlVariable,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool SetpointManagerOutdoorAirReset_Impl::isControlVariableDefaulted() const {
    return isEmpty(OS_SetpointManager_OutdoorAirResetFields::ControlVariable);
  }

  double SetpointManagerOutdoorAirReset_Impl::setpointatOutdoorLowTemperature() const {
    boost::optional<double> value = getDouble(OS_SetpointManager_OutdoorAirResetFields::SetpointatOutdoorLowTemperature,true);
    OS_ASSERT(value);
    return value.get();
  }

  Quantity SetpointManagerOutdoorAirReset_Impl::getSetpointatOutdoorLowTemperature(bool returnIP) const {
    OptionalDouble value = setpointatOutdoorLowTemperature();
    OSOptionalQuantity result = getQuantityFromDouble(OS_SetpointManager_OutdoorAirResetFields::SetpointatOutdoorLowTemperature, value, returnIP);
    OS_ASSERT(result.isSet());
    return result.get();
  }

  double SetpointManagerOutdoorAirReset_Impl::outdoorLowTemperature() const {
    boost::optional<double> value = getDouble(OS_SetpointManager_OutdoorAirResetFields::OutdoorLowTemperature,true);
    OS_ASSERT(value);
    return value.get();
  }

  Quantity SetpointManagerOutdoorAirReset_Impl::getOutdoorLowTemperature(bool returnIP) const {
    OptionalDouble value = outdoorLowTemperature();
    OSOptionalQuantity result = getQuantityFromDouble(OS_SetpointManager_OutdoorAirResetFields::OutdoorLowTemperature, value, returnIP);
    OS_ASSERT(result.isSet());
    return result.get();
  }

  double SetpointManagerOutdoorAirReset_Impl::setpointatOutdoorHighTemperature() const {
    boost::optional<double> value = getDouble(OS_SetpointManager_OutdoorAirResetFields::SetpointatOutdoorHighTemperature,true);
    OS_ASSERT(value);
    return value.get();
  }

  Quantity SetpointManagerOutdoorAirReset_Impl::getSetpointatOutdoorHighTemperature(bool returnIP) const {
    OptionalDouble value = setpointatOutdoorHighTemperature();
    OSOptionalQuantity result = getQuantityFromDouble(OS_SetpointManager_OutdoorAirResetFields::SetpointatOutdoorHighTemperature, value, returnIP);
    OS_ASSERT(result.isSet());
    return result.get();
  }

  double SetpointManagerOutdoorAirReset_Impl::outdoorHighTemperature() const {
    boost::optional<double> value = getDouble(OS_SetpointManager_OutdoorAirResetFields::OutdoorHighTemperature,true);
    OS_ASSERT(value);
    return value.get();
  }

  Quantity SetpointManagerOutdoorAirReset_Impl::getOutdoorHighTemperature(bool returnIP) const {
    OptionalDouble value = outdoorHighTemperature();
    OSOptionalQuantity result = getQuantityFromDouble(OS_SetpointManager_OutdoorAirResetFields::OutdoorHighTemperature, value, returnIP);
    OS_ASSERT(result.isSet());
    return result.get();
  }

  boost::optional<Node> SetpointManagerOutdoorAirReset_Impl::setpointNode() const {
    return getObject<ModelObject>().getModelObjectTarget<Node>(OS_SetpointManager_OutdoorAirResetFields::SetpointNodeorNodeListName);
  }

  boost::optional<Schedule> SetpointManagerOutdoorAirReset_Impl::schedule() const {
    return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_SetpointManager_OutdoorAirResetFields::ScheduleName);
  }

  boost::optional<double> SetpointManagerOutdoorAirReset_Impl::setpointatOutdoorLowTemperature2() const {
    return getDouble(OS_SetpointManager_OutdoorAirResetFields::SetpointatOutdoorLowTemperature2,true);
  }

  OSOptionalQuantity SetpointManagerOutdoorAirReset_Impl::getSetpointatOutdoorLowTemperature2(bool returnIP) const {
    OptionalDouble value = setpointatOutdoorLowTemperature2();
    return getQuantityFromDouble(OS_SetpointManager_OutdoorAirResetFields::SetpointatOutdoorLowTemperature2, value, returnIP);
  }

  boost::optional<double> SetpointManagerOutdoorAirReset_Impl::outdoorLowTemperature2() const {
    return getDouble(OS_SetpointManager_OutdoorAirResetFields::OutdoorLowTemperature2,true);
  }

  OSOptionalQuantity SetpointManagerOutdoorAirReset_Impl::getOutdoorLowTemperature2(bool returnIP) const {
    OptionalDouble value = outdoorLowTemperature2();
    return getQuantityFromDouble(OS_SetpointManager_OutdoorAirResetFields::OutdoorLowTemperature2, value, returnIP);
  }

  boost::optional<double> SetpointManagerOutdoorAirReset_Impl::setpointatOutdoorHighTemperature2() const {
    return getDouble(OS_SetpointManager_OutdoorAirResetFields::SetpointatOutdoorHighTemperature2,true);
  }

  OSOptionalQuantity SetpointManagerOutdoorAirReset_Impl::getSetpointatOutdoorHighTemperature2(bool returnIP) const {
    OptionalDouble value = setpointatOutdoorHighTemperature2();
    return getQuantityFromDouble(OS_SetpointManager_OutdoorAirResetFields::SetpointatOutdoorHighTemperature2, value, returnIP);
  }

  boost::optional<double> SetpointManagerOutdoorAirReset_Impl::outdoorHighTemperature2() const {
    return getDouble(OS_SetpointManager_OutdoorAirResetFields::OutdoorHighTemperature2,true);
  }

  OSOptionalQuantity SetpointManagerOutdoorAirReset_Impl::getOutdoorHighTemperature2(bool returnIP) const {
    OptionalDouble value = outdoorHighTemperature2();
    return getQuantityFromDouble(OS_SetpointManager_OutdoorAirResetFields::OutdoorHighTemperature2, value, returnIP);
  }

  bool SetpointManagerOutdoorAirReset_Impl::setControlVariable(const std::string& controlVariable) {
    return setString(OS_SetpointManager_OutdoorAirResetFields::ControlVariable, controlVariable);
  }

  void SetpointManagerOutdoorAirReset_Impl::resetControlVariable() {
    bool result = setString(OS_SetpointManager_OutdoorAirResetFields::ControlVariable, "");
    OS_ASSERT(result);
  }

  void SetpointManagerOutdoorAirReset_Impl::setSetpointatOutdoorLowTemperature(double setpointatOutdoorLowTemperature) {
    bool result = setDouble(OS_SetpointManager_OutdoorAirResetFields::SetpointatOutdoorLowTemperature, setpointatOutdoorLowTemperature);
    OS_ASSERT(result);
  }

  bool SetpointManagerOutdoorAirReset_Impl::setSetpointatOutdoorLowTemperature(const Quantity& setpointatOutdoorLowTemperature) {
    OptionalDouble value = getDoubleFromQuantity(OS_SetpointManager_OutdoorAirResetFields::SetpointatOutdoorLowTemperature,setpointatOutdoorLowTemperature);
    if (!value) {
      return false;
    }
    setSetpointatOutdoorLowTemperature(value.get());
    return true;
  }

  void SetpointManagerOutdoorAirReset_Impl::setOutdoorLowTemperature(double outdoorLowTemperature) {
    bool result = setDouble(OS_SetpointManager_OutdoorAirResetFields::OutdoorLowTemperature, outdoorLowTemperature);
    OS_ASSERT(result);
  }

  bool SetpointManagerOutdoorAirReset_Impl::setOutdoorLowTemperature(const Quantity& outdoorLowTemperature) {
    OptionalDouble value = getDoubleFromQuantity(OS_SetpointManager_OutdoorAirResetFields::OutdoorLowTemperature,outdoorLowTemperature);
    if (!value) {
      return false;
    }
    setOutdoorLowTemperature(value.get());
    return true;
  }

  void SetpointManagerOutdoorAirReset_Impl::setSetpointatOutdoorHighTemperature(double setpointatOutdoorHighTemperature) {
    bool result = setDouble(OS_SetpointManager_OutdoorAirResetFields::SetpointatOutdoorHighTemperature, setpointatOutdoorHighTemperature);
    OS_ASSERT(result);
  }

  bool SetpointManagerOutdoorAirReset_Impl::setSetpointatOutdoorHighTemperature(const Quantity& setpointatOutdoorHighTemperature) {
    OptionalDouble value = getDoubleFromQuantity(OS_SetpointManager_OutdoorAirResetFields::SetpointatOutdoorHighTemperature,setpointatOutdoorHighTemperature);
    if (!value) {
      return false;
    }
    setSetpointatOutdoorHighTemperature(value.get());
    return true;
  }

  void SetpointManagerOutdoorAirReset_Impl::setOutdoorHighTemperature(double outdoorHighTemperature) {
    bool result = setDouble(OS_SetpointManager_OutdoorAirResetFields::OutdoorHighTemperature, outdoorHighTemperature);
    OS_ASSERT(result);
  }

  bool SetpointManagerOutdoorAirReset_Impl::setOutdoorHighTemperature(const Quantity& outdoorHighTemperature) {
    OptionalDouble value = getDoubleFromQuantity(OS_SetpointManager_OutdoorAirResetFields::OutdoorHighTemperature,outdoorHighTemperature);
    if (!value) {
      return false;
    }
    setOutdoorHighTemperature(value.get());
    return true;
  }

  bool SetpointManagerOutdoorAirReset_Impl::setSetpointNode(const Node& node) {
    return setPointer(OS_SetpointManager_OutdoorAirResetFields::SetpointNodeorNodeListName, node.handle());
  }

  void SetpointManagerOutdoorAirReset_Impl::resetSetpointNode() {
    bool result = setString(OS_SetpointManager_OutdoorAirResetFields::SetpointNodeorNodeListName, "");
    OS_ASSERT(result);
  }

  bool SetpointManagerOutdoorAirReset_Impl::setSchedule(Schedule& schedule) {
    bool result = ModelObject_Impl::setSchedule(OS_SetpointManager_OutdoorAirResetFields::ScheduleName,
                                                "SetpointManagerOutdoorAirReset",
                                                "Setpoint Manager Outdoor Air Reset",
                                                schedule);
    return result;
  }

  void SetpointManagerOutdoorAirReset_Impl::resetSchedule() {
    bool result = setString(OS_SetpointManager_OutdoorAirResetFields::ScheduleName, "");
    OS_ASSERT(result);
  }

  void SetpointManagerOutdoorAirReset_Impl::setSetpointatOutdoorLowTemperature2(boost::optional<double> setpointatOutdoorLowTemperature2) {
    bool result(false);
    if (setpointatOutdoorLowTemperature2) {
      result = setDouble(OS_SetpointManager_OutdoorAirResetFields::SetpointatOutdoorLowTemperature2, setpointatOutdoorLowTemperature2.get());
    }
    else {
      resetSetpointatOutdoorLowTemperature2();
      result = true;
    }
    OS_ASSERT(result);
  }

  bool SetpointManagerOutdoorAirReset_Impl::setSetpointatOutdoorLowTemperature2(const OSOptionalQuantity& setpointatOutdoorLowTemperature2) {
    bool result(false);
    OptionalDouble value;
    if (setpointatOutdoorLowTemperature2.isSet()) {
      value = getDoubleFromQuantity(OS_SetpointManager_OutdoorAirResetFields::SetpointatOutdoorLowTemperature2,setpointatOutdoorLowTemperature2.get());
      if (value) {
        setSetpointatOutdoorLowTemperature2(value);
        result = true;
      }
    }
    else {
      setSetpointatOutdoorLowTemperature2(value);
      result = true;
    }
    return result;
  }

  void SetpointManagerOutdoorAirReset_Impl::resetSetpointatOutdoorLowTemperature2() {
    bool result = setString(OS_SetpointManager_OutdoorAirResetFields::SetpointatOutdoorLowTemperature2, "");
    OS_ASSERT(result);
  }

  void SetpointManagerOutdoorAirReset_Impl::setOutdoorLowTemperature2(boost::optional<double> outdoorLowTemperature2) {
    bool result(false);
    if (outdoorLowTemperature2) {
      result = setDouble(OS_SetpointManager_OutdoorAirResetFields::OutdoorLowTemperature2, outdoorLowTemperature2.get());
    }
    else {
      resetOutdoorLowTemperature2();
      result = true;
    }
    OS_ASSERT(result);
  }

  bool SetpointManagerOutdoorAirReset_Impl::setOutdoorLowTemperature2(const OSOptionalQuantity& outdoorLowTemperature2) {
    bool result(false);
    OptionalDouble value;
    if (outdoorLowTemperature2.isSet()) {
      value = getDoubleFromQuantity(OS_SetpointManager_OutdoorAirResetFields::OutdoorLowTemperature2,outdoorLowTemperature2.get());
      if (value) {
        setOutdoorLowTemperature2(value);

        result = true;
      }
    }
    else {
      setOutdoorLowTemperature2(value);
      result = true;
    }
    return result;
  }

  void SetpointManagerOutdoorAirReset_Impl::resetOutdoorLowTemperature2() {
    bool result = setString(OS_SetpointManager_OutdoorAirResetFields::OutdoorLowTemperature2, "");
    OS_ASSERT(result);
  }

  void SetpointManagerOutdoorAirReset_Impl::setSetpointatOutdoorHighTemperature2(boost::optional<double> setpointatOutdoorHighTemperature2) {
    bool result(false);
    if (setpointatOutdoorHighTemperature2) {
      result = setDouble(OS_SetpointManager_OutdoorAirResetFields::SetpointatOutdoorHighTemperature2, setpointatOutdoorHighTemperature2.get());
    }
    else {
      resetSetpointatOutdoorHighTemperature2();
      result = true;
    }
    OS_ASSERT(result);
  }

  bool SetpointManagerOutdoorAirReset_Impl::setSetpointatOutdoorHighTemperature2(const OSOptionalQuantity& setpointatOutdoorHighTemperature2) {
    bool result(false);
    OptionalDouble value;
    if (setpointatOutdoorHighTemperature2.isSet()) {
      value = getDoubleFromQuantity(OS_SetpointManager_OutdoorAirResetFields::SetpointatOutdoorHighTemperature2,setpointatOutdoorHighTemperature2.get());
      if (value) {
        setSetpointatOutdoorHighTemperature2(value);

        result = true;
      }
    }
    else {
      setSetpointatOutdoorHighTemperature2(value);
      result = true;
    }
    return result;
  }

  void SetpointManagerOutdoorAirReset_Impl::resetSetpointatOutdoorHighTemperature2() {
    bool result = setString(OS_SetpointManager_OutdoorAirResetFields::SetpointatOutdoorHighTemperature2, "");
    OS_ASSERT(result);
  }

  void SetpointManagerOutdoorAirReset_Impl::setOutdoorHighTemperature2(boost::optional<double> outdoorHighTemperature2) {
    bool result(false);
    if (outdoorHighTemperature2) {
      result = setDouble(OS_SetpointManager_OutdoorAirResetFields::OutdoorHighTemperature2, outdoorHighTemperature2.get());
    }
    else {
      resetOutdoorHighTemperature2();
      result = true;
    }
    OS_ASSERT(result);
  }

  bool SetpointManagerOutdoorAirReset_Impl::setOutdoorHighTemperature2(const OSOptionalQuantity& outdoorHighTemperature2) {
    bool result(false);
    OptionalDouble value;
    if (outdoorHighTemperature2.isSet()) {
      value = getDoubleFromQuantity(OS_SetpointManager_OutdoorAirResetFields::OutdoorHighTemperature2,outdoorHighTemperature2.get());
      if (value) {
        setOutdoorHighTemperature2(value);

        result = true;
      }
    }
    else {
      setOutdoorHighTemperature2(value);
      result = true;
    }
    return result;
  }

  void SetpointManagerOutdoorAirReset_Impl::resetOutdoorHighTemperature2() {
    bool result = setString(OS_SetpointManager_OutdoorAirResetFields::OutdoorHighTemperature2, "");
    OS_ASSERT(result);
  }

  std::vector<std::string> SetpointManagerOutdoorAirReset_Impl::controlVariableValues() const {
    return SetpointManagerOutdoorAirReset::controlVariableValues();
  }

  openstudio::Quantity SetpointManagerOutdoorAirReset_Impl::setpointatOutdoorLowTemperature_SI() const {
    return getSetpointatOutdoorLowTemperature(false);
  }

  openstudio::Quantity SetpointManagerOutdoorAirReset_Impl::setpointatOutdoorLowTemperature_IP() const {
    return getSetpointatOutdoorLowTemperature(true);
  }

  openstudio::Quantity SetpointManagerOutdoorAirReset_Impl::outdoorLowTemperature_SI() const {
    return getOutdoorLowTemperature(false);
  }

  openstudio::Quantity SetpointManagerOutdoorAirReset_Impl::outdoorLowTemperature_IP() const {
    return getOutdoorLowTemperature(true);
  }

  openstudio::Quantity SetpointManagerOutdoorAirReset_Impl::setpointatOutdoorHighTemperature_SI() const {
    return getSetpointatOutdoorHighTemperature(false);
  }

  openstudio::Quantity SetpointManagerOutdoorAirReset_Impl::setpointatOutdoorHighTemperature_IP() const {
    return getSetpointatOutdoorHighTemperature(true);
  }

  openstudio::Quantity SetpointManagerOutdoorAirReset_Impl::outdoorHighTemperature_SI() const {
    return getOutdoorHighTemperature(false);
  }

  openstudio::Quantity SetpointManagerOutdoorAirReset_Impl::outdoorHighTemperature_IP() const {
    return getOutdoorHighTemperature(true);
  }

  openstudio::OSOptionalQuantity SetpointManagerOutdoorAirReset_Impl::setpointatOutdoorLowTemperature2_SI() const {
    return getSetpointatOutdoorLowTemperature2(false);
  }

  openstudio::OSOptionalQuantity SetpointManagerOutdoorAirReset_Impl::setpointatOutdoorLowTemperature2_IP() const {
    return getSetpointatOutdoorLowTemperature2(true);
  }

  openstudio::OSOptionalQuantity SetpointManagerOutdoorAirReset_Impl::outdoorLowTemperature2_SI() const {
    return getOutdoorLowTemperature2(false);
  }

  openstudio::OSOptionalQuantity SetpointManagerOutdoorAirReset_Impl::outdoorLowTemperature2_IP() const {
    return getOutdoorLowTemperature2(true);
  }

  openstudio::OSOptionalQuantity SetpointManagerOutdoorAirReset_Impl::setpointatOutdoorHighTemperature2_SI() const {
    return getSetpointatOutdoorHighTemperature2(false);
  }

  openstudio::OSOptionalQuantity SetpointManagerOutdoorAirReset_Impl::setpointatOutdoorHighTemperature2_IP() const {
    return getSetpointatOutdoorHighTemperature2(true);
  }

  openstudio::OSOptionalQuantity SetpointManagerOutdoorAirReset_Impl::outdoorHighTemperature2_SI() const {
    return getOutdoorHighTemperature2(false);
  }

  openstudio::OSOptionalQuantity SetpointManagerOutdoorAirReset_Impl::outdoorHighTemperature2_IP() const {
    return getOutdoorHighTemperature2(true);
  }

  boost::optional<ModelObject> SetpointManagerOutdoorAirReset_Impl::setpointNodeAsModelObject() const {
    OptionalModelObject result;
    OptionalNode intermediate = setpointNode();
    if (intermediate) {
      result = *intermediate;
    }
    return result;
  }

  boost::optional<ModelObject> SetpointManagerOutdoorAirReset_Impl::scheduleAsModelObject() const {
    OptionalModelObject result;
    OptionalSchedule intermediate = schedule();
    if (intermediate) {
      result = *intermediate;
    }
    return result;
  }

  bool SetpointManagerOutdoorAirReset_Impl::setSetpointNodeAsModelObject(const boost::optional<ModelObject>& modelObject) {
    if (modelObject) {
      OptionalNode intermediate = modelObject->optionalCast<Node>();
      if (intermediate) {
        Node node(*intermediate);
        return setSetpointNode(node);
      }
      else {
        return false;
      }
    }
    else {
      resetSetpointNode();
    }
    return true;
  }

  bool SetpointManagerOutdoorAirReset_Impl::setScheduleAsModelObject(const boost::optional<ModelObject>& modelObject) {
    if (modelObject) {
      OptionalSchedule intermediate = modelObject->optionalCast<Schedule>();
      if (intermediate) {
        Schedule schedule(*intermediate);
        return setSchedule(schedule);
      }
      else {
        return false;
      }
    }
    else {
      resetSchedule();
    }
    return true;
  }

} // detail

SetpointManagerOutdoorAirReset::SetpointManagerOutdoorAirReset(const Model& model)
  : SetpointManager(SetpointManagerOutdoorAirReset::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::SetpointManagerOutdoorAirReset_Impl>());

  setControlVariable("Temperature");
  setSetpointatOutdoorLowTemperature(22.0);
  setOutdoorLowTemperature(10.0);
  setSetpointatOutdoorHighTemperature(10.0);
  setOutdoorHighTemperature(24.0);
  resetSchedule();
  resetSetpointatOutdoorLowTemperature2();
  resetOutdoorLowTemperature2();
  resetSetpointatOutdoorHighTemperature2();
  resetOutdoorHighTemperature2();
}

IddObjectType SetpointManagerOutdoorAirReset::iddObjectType() {
  return IddObjectType(IddObjectType::OS_SetpointManager_OutdoorAirReset);
}

std::vector<std::string> SetpointManagerOutdoorAirReset::controlVariableValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_SetpointManager_OutdoorAirResetFields::ControlVariable);
}

std::string SetpointManagerOutdoorAirReset::controlVariable() const {
  return getImpl<detail::SetpointManagerOutdoorAirReset_Impl>()->controlVariable();
}

bool SetpointManagerOutdoorAirReset::isControlVariableDefaulted() const {
  return getImpl<detail::SetpointManagerOutdoorAirReset_Impl>()->isControlVariableDefaulted();
}

double SetpointManagerOutdoorAirReset::setpointatOutdoorLowTemperature() const {
  return getImpl<detail::SetpointManagerOutdoorAirReset_Impl>()->setpointatOutdoorLowTemperature();
}

Quantity SetpointManagerOutdoorAirReset::getSetpointatOutdoorLowTemperature(bool returnIP) const {
  return getImpl<detail::SetpointManagerOutdoorAirReset_Impl>()->getSetpointatOutdoorLowTemperature(returnIP);
}

double SetpointManagerOutdoorAirReset::outdoorLowTemperature() const {
  return getImpl<detail::SetpointManagerOutdoorAirReset_Impl>()->outdoorLowTemperature();
}

Quantity SetpointManagerOutdoorAirReset::getOutdoorLowTemperature(bool returnIP) const {
  return getImpl<detail::SetpointManagerOutdoorAirReset_Impl>()->getOutdoorLowTemperature(returnIP);
}

double SetpointManagerOutdoorAirReset::setpointatOutdoorHighTemperature() const {
  return getImpl<detail::SetpointManagerOutdoorAirReset_Impl>()->setpointatOutdoorHighTemperature();
}

Quantity SetpointManagerOutdoorAirReset::getSetpointatOutdoorHighTemperature(bool returnIP) const {
  return getImpl<detail::SetpointManagerOutdoorAirReset_Impl>()->getSetpointatOutdoorHighTemperature(returnIP);
}

double SetpointManagerOutdoorAirReset::outdoorHighTemperature() const {
  return getImpl<detail::SetpointManagerOutdoorAirReset_Impl>()->outdoorHighTemperature();
}

Quantity SetpointManagerOutdoorAirReset::getOutdoorHighTemperature(bool returnIP) const {
  return getImpl<detail::SetpointManagerOutdoorAirReset_Impl>()->getOutdoorHighTemperature(returnIP);
}

boost::optional<Schedule> SetpointManagerOutdoorAirReset::schedule() const {
  return getImpl<detail::SetpointManagerOutdoorAirReset_Impl>()->schedule();
}

boost::optional<double> SetpointManagerOutdoorAirReset::setpointatOutdoorLowTemperature2() const {
  return getImpl<detail::SetpointManagerOutdoorAirReset_Impl>()->setpointatOutdoorLowTemperature2();
}

OSOptionalQuantity SetpointManagerOutdoorAirReset::getSetpointatOutdoorLowTemperature2(bool returnIP) const {
  return getImpl<detail::SetpointManagerOutdoorAirReset_Impl>()->getSetpointatOutdoorLowTemperature2(returnIP);
}

boost::optional<double> SetpointManagerOutdoorAirReset::outdoorLowTemperature2() const {
  return getImpl<detail::SetpointManagerOutdoorAirReset_Impl>()->outdoorLowTemperature2();
}

OSOptionalQuantity SetpointManagerOutdoorAirReset::getOutdoorLowTemperature2(bool returnIP) const {
  return getImpl<detail::SetpointManagerOutdoorAirReset_Impl>()->getOutdoorLowTemperature2(returnIP);
}

boost::optional<double> SetpointManagerOutdoorAirReset::setpointatOutdoorHighTemperature2() const {
  return getImpl<detail::SetpointManagerOutdoorAirReset_Impl>()->setpointatOutdoorHighTemperature2();
}

OSOptionalQuantity SetpointManagerOutdoorAirReset::getSetpointatOutdoorHighTemperature2(bool returnIP) const {
  return getImpl<detail::SetpointManagerOutdoorAirReset_Impl>()->getSetpointatOutdoorHighTemperature2(returnIP);
}

boost::optional<double> SetpointManagerOutdoorAirReset::outdoorHighTemperature2() const {
  return getImpl<detail::SetpointManagerOutdoorAirReset_Impl>()->outdoorHighTemperature2();
}

OSOptionalQuantity SetpointManagerOutdoorAirReset::getOutdoorHighTemperature2(bool returnIP) const {
  return getImpl<detail::SetpointManagerOutdoorAirReset_Impl>()->getOutdoorHighTemperature2(returnIP);
}

bool SetpointManagerOutdoorAirReset::setControlVariable(const std::string& controlVariable) {
  return getImpl<detail::SetpointManagerOutdoorAirReset_Impl>()->setControlVariable(controlVariable);
}

void SetpointManagerOutdoorAirReset::resetControlVariable() {
  getImpl<detail::SetpointManagerOutdoorAirReset_Impl>()->resetControlVariable();
}

void SetpointManagerOutdoorAirReset::setSetpointatOutdoorLowTemperature(double setpointatOutdoorLowTemperature) {
  getImpl<detail::SetpointManagerOutdoorAirReset_Impl>()->setSetpointatOutdoorLowTemperature(setpointatOutdoorLowTemperature);
}

bool SetpointManagerOutdoorAirReset::setSetpointatOutdoorLowTemperature(const Quantity& setpointatOutdoorLowTemperature) {
  return getImpl<detail::SetpointManagerOutdoorAirReset_Impl>()->setSetpointatOutdoorLowTemperature(setpointatOutdoorLowTemperature);
}

void SetpointManagerOutdoorAirReset::setOutdoorLowTemperature(double outdoorLowTemperature) {
  getImpl<detail::SetpointManagerOutdoorAirReset_Impl>()->setOutdoorLowTemperature(outdoorLowTemperature);
}

bool SetpointManagerOutdoorAirReset::setOutdoorLowTemperature(const Quantity& outdoorLowTemperature) {
  return getImpl<detail::SetpointManagerOutdoorAirReset_Impl>()->setOutdoorLowTemperature(outdoorLowTemperature);
}

void SetpointManagerOutdoorAirReset::setSetpointatOutdoorHighTemperature(double setpointatOutdoorHighTemperature) {
  getImpl<detail::SetpointManagerOutdoorAirReset_Impl>()->setSetpointatOutdoorHighTemperature(setpointatOutdoorHighTemperature);
}

bool SetpointManagerOutdoorAirReset::setSetpointatOutdoorHighTemperature(const Quantity& setpointatOutdoorHighTemperature) {
  return getImpl<detail::SetpointManagerOutdoorAirReset_Impl>()->setSetpointatOutdoorHighTemperature(setpointatOutdoorHighTemperature);
}

void SetpointManagerOutdoorAirReset::setOutdoorHighTemperature(double outdoorHighTemperature) {
  getImpl<detail::SetpointManagerOutdoorAirReset_Impl>()->setOutdoorHighTemperature(outdoorHighTemperature);
}

bool SetpointManagerOutdoorAirReset::setOutdoorHighTemperature(const Quantity& outdoorHighTemperature) {
  return getImpl<detail::SetpointManagerOutdoorAirReset_Impl>()->setOutdoorHighTemperature(outdoorHighTemperature);
}

bool SetpointManagerOutdoorAirReset::setSchedule(Schedule& schedule) {
  return getImpl<detail::SetpointManagerOutdoorAirReset_Impl>()->setSchedule(schedule);
}

void SetpointManagerOutdoorAirReset::resetSchedule() {
  getImpl<detail::SetpointManagerOutdoorAirReset_Impl>()->resetSchedule();
}

void SetpointManagerOutdoorAirReset::setSetpointatOutdoorLowTemperature2(double setpointatOutdoorLowTemperature2) {
  getImpl<detail::SetpointManagerOutdoorAirReset_Impl>()->setSetpointatOutdoorLowTemperature2(setpointatOutdoorLowTemperature2);
}

bool SetpointManagerOutdoorAirReset::setSetpointatOutdoorLowTemperature2(const Quantity& setpointatOutdoorLowTemperature2) {
  return getImpl<detail::SetpointManagerOutdoorAirReset_Impl>()->setSetpointatOutdoorLowTemperature2(setpointatOutdoorLowTemperature2);
}

void SetpointManagerOutdoorAirReset::resetSetpointatOutdoorLowTemperature2() {
  getImpl<detail::SetpointManagerOutdoorAirReset_Impl>()->resetSetpointatOutdoorLowTemperature2();
}

void SetpointManagerOutdoorAirReset::setOutdoorLowTemperature2(double outdoorLowTemperature2) {
  getImpl<detail::SetpointManagerOutdoorAirReset_Impl>()->setOutdoorLowTemperature2(outdoorLowTemperature2);
}

bool SetpointManagerOutdoorAirReset::setOutdoorLowTemperature2(const Quantity& outdoorLowTemperature2) {
  return getImpl<detail::SetpointManagerOutdoorAirReset_Impl>()->setOutdoorLowTemperature2(outdoorLowTemperature2);
}

void SetpointManagerOutdoorAirReset::resetOutdoorLowTemperature2() {
  getImpl<detail::SetpointManagerOutdoorAirReset_Impl>()->resetOutdoorLowTemperature2();
}

void SetpointManagerOutdoorAirReset::setSetpointatOutdoorHighTemperature2(double setpointatOutdoorHighTemperature2) {
  getImpl<detail::SetpointManagerOutdoorAirReset_Impl>()->setSetpointatOutdoorHighTemperature2(setpointatOutdoorHighTemperature2);
}

bool SetpointManagerOutdoorAirReset::setSetpointatOutdoorHighTemperature2(const Quantity& setpointatOutdoorHighTemperature2) {
  return getImpl<detail::SetpointManagerOutdoorAirReset_Impl>()->setSetpointatOutdoorHighTemperature2(setpointatOutdoorHighTemperature2);
}

void SetpointManagerOutdoorAirReset::resetSetpointatOutdoorHighTemperature2() {
  getImpl<detail::SetpointManagerOutdoorAirReset_Impl>()->resetSetpointatOutdoorHighTemperature2();
}

void SetpointManagerOutdoorAirReset::setOutdoorHighTemperature2(double outdoorHighTemperature2) {
  getImpl<detail::SetpointManagerOutdoorAirReset_Impl>()->setOutdoorHighTemperature2(outdoorHighTemperature2);
}

bool SetpointManagerOutdoorAirReset::setOutdoorHighTemperature2(const Quantity& outdoorHighTemperature2) {
  return getImpl<detail::SetpointManagerOutdoorAirReset_Impl>()->setOutdoorHighTemperature2(outdoorHighTemperature2);
}

void SetpointManagerOutdoorAirReset::resetOutdoorHighTemperature2() {
  getImpl<detail::SetpointManagerOutdoorAirReset_Impl>()->resetOutdoorHighTemperature2();
}

boost::optional<Node> SetpointManagerOutdoorAirReset::setpointNode() const
{
  return getImpl<detail::SetpointManagerOutdoorAirReset_Impl>()->setpointNode();
}

/// @cond
SetpointManagerOutdoorAirReset::SetpointManagerOutdoorAirReset(std::shared_ptr<detail::SetpointManagerOutdoorAirReset_Impl> impl)
  : SetpointManager(std::move(impl))
{}
/// @endcond

} // model
} // openstudio

