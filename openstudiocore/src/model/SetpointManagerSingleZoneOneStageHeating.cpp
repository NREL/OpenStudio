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

#include "SetpointManagerSingleZoneOneStageHeating.hpp"
#include "SetpointManagerSingleZoneOneStageHeating_Impl.hpp"

#include "ThermalZone.hpp"
#include "ThermalZone_Impl.hpp"
#include "Node.hpp"
#include "Node_Impl.hpp"
#include "Model.hpp"
#include "AirLoopHVAC.hpp"

#include <utilities/idd/IddFactory.hxx>
#include <utilities/idd/OS_SetpointManager_SingleZone_OneStageHeating_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>
#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  SetpointManagerSingleZoneOneStageHeating_Impl::SetpointManagerSingleZoneOneStageHeating_Impl(const IdfObject& idfObject,
                                                                                               Model_Impl* model,
                                                                                               bool keepHandle)
    : SetpointManager_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == SetpointManagerSingleZoneOneStageHeating::iddObjectType());
  }

  SetpointManagerSingleZoneOneStageHeating_Impl::SetpointManagerSingleZoneOneStageHeating_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                                               Model_Impl* model,
                                                                                               bool keepHandle)
    : SetpointManager_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == SetpointManagerSingleZoneOneStageHeating::iddObjectType());
  }

  SetpointManagerSingleZoneOneStageHeating_Impl::SetpointManagerSingleZoneOneStageHeating_Impl(const SetpointManagerSingleZoneOneStageHeating_Impl& other,
                                                                                               Model_Impl* model,
                                                                                               bool keepHandle)
    : SetpointManager_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& SetpointManagerSingleZoneOneStageHeating_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType SetpointManagerSingleZoneOneStageHeating_Impl::iddObjectType() const {
    return SetpointManagerSingleZoneOneStageHeating::iddObjectType();
  }

  bool SetpointManagerSingleZoneOneStageHeating_Impl::addToNode(Node & node)
  {
    bool added = SetpointManager_Impl::addToNode( node );
    if( added ) {
      if( boost::optional<AirLoopHVAC> _airLoop = node.airLoopHVAC() ) {
        ModelObjectVector modelObjectVector = _airLoop->demandComponents(openstudio::IddObjectType::OS_ThermalZone);
        if( !modelObjectVector.empty() ) {
          ModelObject mo = modelObjectVector.front();
          ThermalZone thermalZone = mo.cast<ThermalZone>();
          this->setControlZone(thermalZone);
        }
        return true;
      }
    }
    return false;
  }

  ModelObject SetpointManagerSingleZoneOneStageHeating_Impl::clone(Model model) const
  {
    SetpointManagerSingleZoneOneStageHeating clonedObject = SetpointManager_Impl::clone( model ).cast<SetpointManagerSingleZoneOneStageHeating>();
    clonedObject.resetControlZone();
    return clonedObject;
  }

  std::string SetpointManagerSingleZoneOneStageHeating_Impl::controlVariable() const {
    boost::optional<std::string> value = getString(OS_SetpointManager_SingleZone_OneStageHeatingFields::ControlVariable,true);
    OS_ASSERT(value);
    return value.get();
  }

  double SetpointManagerSingleZoneOneStageHeating_Impl::heatingStageOnSupplyAirSetpointTemperature() const {
    boost::optional<double> value = getDouble(OS_SetpointManager_SingleZone_OneStageHeatingFields::HeatingStageOnSupplyAirSetpointTemperature,true);
    OS_ASSERT(value);
    return value.get();
  }

  double SetpointManagerSingleZoneOneStageHeating_Impl::heatingStageOffSupplyAirSetpointTemperature() const {
    boost::optional<double> value = getDouble(OS_SetpointManager_SingleZone_OneStageHeatingFields::HeatingStageOffSupplyAirSetpointTemperature,true);
    OS_ASSERT(value);
    return value.get();
  }

  boost::optional<ThermalZone> SetpointManagerSingleZoneOneStageHeating_Impl::controlZone() const {
    return getObject<ModelObject>().getModelObjectTarget<ThermalZone>(OS_SetpointManager_SingleZone_OneStageHeatingFields::ControlZoneName);
  }

  boost::optional<Node> SetpointManagerSingleZoneOneStageHeating_Impl::setpointNode() const {
    return getObject<ModelObject>().getModelObjectTarget<Node>(OS_SetpointManager_SingleZone_OneStageHeatingFields::SetpointNodeorNodeListName);
  }

  bool SetpointManagerSingleZoneOneStageHeating_Impl::setControlVariable(const std::string& controlVariable) {
    bool result = setString(OS_SetpointManager_SingleZone_OneStageHeatingFields::ControlVariable, controlVariable);
    return result;
  }

  void SetpointManagerSingleZoneOneStageHeating_Impl::setHeatingStageOnSupplyAirSetpointTemperature(double heatingStageOnSupplyAirSetpointTemperature) {
    bool result = setDouble(OS_SetpointManager_SingleZone_OneStageHeatingFields::HeatingStageOnSupplyAirSetpointTemperature, heatingStageOnSupplyAirSetpointTemperature);
    OS_ASSERT(result);
  }

  void SetpointManagerSingleZoneOneStageHeating_Impl::setHeatingStageOffSupplyAirSetpointTemperature(double heatingStageOffSupplyAirSetpointTemperature) {
    bool result = setDouble(OS_SetpointManager_SingleZone_OneStageHeatingFields::HeatingStageOffSupplyAirSetpointTemperature, heatingStageOffSupplyAirSetpointTemperature);
    OS_ASSERT(result);
  }

  bool SetpointManagerSingleZoneOneStageHeating_Impl::setControlZone(const ThermalZone& thermalZone) {
    bool result = setPointer(OS_SetpointManager_SingleZone_OneStageHeatingFields::ControlZoneName, thermalZone.handle());
    return result;
  }

  void SetpointManagerSingleZoneOneStageHeating_Impl::resetControlZone() {
    bool result = setString(OS_SetpointManager_SingleZone_OneStageHeatingFields::ControlZoneName, "");
    OS_ASSERT(result);
  }

  bool SetpointManagerSingleZoneOneStageHeating_Impl::setSetpointNode(const Node& node) {
    bool result = setPointer(OS_SetpointManager_SingleZone_OneStageHeatingFields::SetpointNodeorNodeListName, node.handle());
    return result;
  }

  void SetpointManagerSingleZoneOneStageHeating_Impl::resetSetpointNode() {
    bool result = setString(OS_SetpointManager_SingleZone_OneStageHeatingFields::SetpointNodeorNodeListName, "");
    OS_ASSERT(result);
  }

} // detail

SetpointManagerSingleZoneOneStageHeating::SetpointManagerSingleZoneOneStageHeating(const Model& model)
  : SetpointManager(SetpointManagerSingleZoneOneStageHeating::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::SetpointManagerSingleZoneOneStageHeating_Impl>());

  setControlVariable("Temperature");
  setHeatingStageOnSupplyAirSetpointTemperature(99);
  setHeatingStageOffSupplyAirSetpointTemperature(-99);
}

IddObjectType SetpointManagerSingleZoneOneStageHeating::iddObjectType() {
  return IddObjectType(IddObjectType::OS_SetpointManager_SingleZone_OneStageHeating);
}

std::vector<std::string> SetpointManagerSingleZoneOneStageHeating::controlVariableValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_SetpointManager_SingleZone_OneStageHeatingFields::ControlVariable);
}

std::string SetpointManagerSingleZoneOneStageHeating::controlVariable() const {
  return getImpl<detail::SetpointManagerSingleZoneOneStageHeating_Impl>()->controlVariable();
}

double SetpointManagerSingleZoneOneStageHeating::heatingStageOnSupplyAirSetpointTemperature() const {
  return getImpl<detail::SetpointManagerSingleZoneOneStageHeating_Impl>()->heatingStageOnSupplyAirSetpointTemperature();
}

double SetpointManagerSingleZoneOneStageHeating::heatingStageOffSupplyAirSetpointTemperature() const {
  return getImpl<detail::SetpointManagerSingleZoneOneStageHeating_Impl>()->heatingStageOffSupplyAirSetpointTemperature();
}

boost::optional<ThermalZone> SetpointManagerSingleZoneOneStageHeating::controlZone() const {
  return getImpl<detail::SetpointManagerSingleZoneOneStageHeating_Impl>()->controlZone();
}

boost::optional<Node> SetpointManagerSingleZoneOneStageHeating::setpointNode() const {
  return getImpl<detail::SetpointManagerSingleZoneOneStageHeating_Impl>()->setpointNode();
}

bool SetpointManagerSingleZoneOneStageHeating::setControlVariable(const std::string& controlVariable) {
  return getImpl<detail::SetpointManagerSingleZoneOneStageHeating_Impl>()->setControlVariable(controlVariable);
}

void SetpointManagerSingleZoneOneStageHeating::setHeatingStageOnSupplyAirSetpointTemperature(double heatingStageOnSupplyAirSetpointTemperature) {
  getImpl<detail::SetpointManagerSingleZoneOneStageHeating_Impl>()->setHeatingStageOnSupplyAirSetpointTemperature(heatingStageOnSupplyAirSetpointTemperature);
}

void SetpointManagerSingleZoneOneStageHeating::setHeatingStageOffSupplyAirSetpointTemperature(double heatingStageOffSupplyAirSetpointTemperature) {
  getImpl<detail::SetpointManagerSingleZoneOneStageHeating_Impl>()->setHeatingStageOffSupplyAirSetpointTemperature(heatingStageOffSupplyAirSetpointTemperature);
}

bool SetpointManagerSingleZoneOneStageHeating::setControlZone(const ThermalZone& thermalZone) {
  return getImpl<detail::SetpointManagerSingleZoneOneStageHeating_Impl>()->setControlZone(thermalZone);
}

void SetpointManagerSingleZoneOneStageHeating::resetControlZone() {
  getImpl<detail::SetpointManagerSingleZoneOneStageHeating_Impl>()->resetControlZone();
}

/// @cond
SetpointManagerSingleZoneOneStageHeating::SetpointManagerSingleZoneOneStageHeating(std::shared_ptr<detail::SetpointManagerSingleZoneOneStageHeating_Impl> impl)
  : SetpointManager(std::move(impl))
{}
/// @endcond

} // model
} // openstudio

