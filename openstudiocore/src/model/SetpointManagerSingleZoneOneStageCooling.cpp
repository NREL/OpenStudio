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

#include "SetpointManagerSingleZoneOneStageCooling.hpp"
#include "SetpointManagerSingleZoneOneStageCooling_Impl.hpp"

#include "ThermalZone.hpp"
#include "ThermalZone_Impl.hpp"
#include "Node.hpp"
#include "Node_Impl.hpp"
#include "Model.hpp"
#include "AirLoopHVAC.hpp"

#include <utilities/idd/IddFactory.hxx>
#include <utilities/idd/OS_SetpointManager_SingleZone_OneStageCooling_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>
#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  SetpointManagerSingleZoneOneStageCooling_Impl::SetpointManagerSingleZoneOneStageCooling_Impl(const IdfObject& idfObject,
                                                                                               Model_Impl* model,
                                                                                               bool keepHandle)
    : SetpointManager_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == SetpointManagerSingleZoneOneStageCooling::iddObjectType());
  }

  SetpointManagerSingleZoneOneStageCooling_Impl::SetpointManagerSingleZoneOneStageCooling_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                                               Model_Impl* model,
                                                                                               bool keepHandle)
    : SetpointManager_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == SetpointManagerSingleZoneOneStageCooling::iddObjectType());
  }

  SetpointManagerSingleZoneOneStageCooling_Impl::SetpointManagerSingleZoneOneStageCooling_Impl(const SetpointManagerSingleZoneOneStageCooling_Impl& other,
                                                                                               Model_Impl* model,
                                                                                               bool keepHandle)
    : SetpointManager_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& SetpointManagerSingleZoneOneStageCooling_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType SetpointManagerSingleZoneOneStageCooling_Impl::iddObjectType() const {
    return SetpointManagerSingleZoneOneStageCooling::iddObjectType();
  }

  bool SetpointManagerSingleZoneOneStageCooling_Impl::addToNode(Node & node)
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

  ModelObject SetpointManagerSingleZoneOneStageCooling_Impl::clone(Model model) const
  {
    SetpointManagerSingleZoneOneStageCooling clonedObject = SetpointManager_Impl::clone( model ).cast<SetpointManagerSingleZoneOneStageCooling>();
    clonedObject.resetControlZone();
    return clonedObject;
  }

  std::string SetpointManagerSingleZoneOneStageCooling_Impl::controlVariable() const {
    boost::optional<std::string> value = getString(OS_SetpointManager_SingleZone_OneStageCoolingFields::ControlVariable,true);
    OS_ASSERT(value);
    return value.get();
  }

  double SetpointManagerSingleZoneOneStageCooling_Impl::coolingStageOnSupplyAirSetpointTemperature() const {
    boost::optional<double> value = getDouble(OS_SetpointManager_SingleZone_OneStageCoolingFields::CoolingStageOnSupplyAirSetpointTemperature,true);
    OS_ASSERT(value);
    return value.get();
  }

  double SetpointManagerSingleZoneOneStageCooling_Impl::coolingStageOffSupplyAirSetpointTemperature() const {
    boost::optional<double> value = getDouble(OS_SetpointManager_SingleZone_OneStageCoolingFields::CoolingStageOffSupplyAirSetpointTemperature,true);
    OS_ASSERT(value);
    return value.get();
  }

  boost::optional<ThermalZone> SetpointManagerSingleZoneOneStageCooling_Impl::controlZone() const {
    return getObject<ModelObject>().getModelObjectTarget<ThermalZone>(OS_SetpointManager_SingleZone_OneStageCoolingFields::ControlZoneName);
  }

  boost::optional<Node> SetpointManagerSingleZoneOneStageCooling_Impl::setpointNode() const {
    return getObject<ModelObject>().getModelObjectTarget<Node>(OS_SetpointManager_SingleZone_OneStageCoolingFields::SetpointNodeorNodeListName);
  }

  bool SetpointManagerSingleZoneOneStageCooling_Impl::setControlVariable(const std::string& controlVariable) {
    bool result = setString(OS_SetpointManager_SingleZone_OneStageCoolingFields::ControlVariable, controlVariable);
    return result;
  }

  void SetpointManagerSingleZoneOneStageCooling_Impl::setCoolingStageOnSupplyAirSetpointTemperature(double coolingStageOnSupplyAirSetpointTemperature) {
    bool result = setDouble(OS_SetpointManager_SingleZone_OneStageCoolingFields::CoolingStageOnSupplyAirSetpointTemperature, coolingStageOnSupplyAirSetpointTemperature);
    OS_ASSERT(result);
  }

  void SetpointManagerSingleZoneOneStageCooling_Impl::setCoolingStageOffSupplyAirSetpointTemperature(double coolingStageOffSupplyAirSetpointTemperature) {
    bool result = setDouble(OS_SetpointManager_SingleZone_OneStageCoolingFields::CoolingStageOffSupplyAirSetpointTemperature, coolingStageOffSupplyAirSetpointTemperature);
    OS_ASSERT(result);
  }

  bool SetpointManagerSingleZoneOneStageCooling_Impl::setControlZone(const ThermalZone& thermalZone) {
    bool result = setPointer(OS_SetpointManager_SingleZone_OneStageCoolingFields::ControlZoneName, thermalZone.handle());
    return result;
  }

  void SetpointManagerSingleZoneOneStageCooling_Impl::resetControlZone() {
    bool result = setString(OS_SetpointManager_SingleZone_OneStageCoolingFields::ControlZoneName, "");
    OS_ASSERT(result);
  }

  bool SetpointManagerSingleZoneOneStageCooling_Impl::setSetpointNode(const Node& node) {
    bool result  = setPointer(OS_SetpointManager_SingleZone_OneStageCoolingFields::SetpointNodeorNodeListName, node.handle());
    return result;
  }

  void SetpointManagerSingleZoneOneStageCooling_Impl::resetSetpointNode() {
    bool result = setString(OS_SetpointManager_SingleZone_OneStageCoolingFields::SetpointNodeorNodeListName, "");
    OS_ASSERT(result);
  }

} // detail

SetpointManagerSingleZoneOneStageCooling::SetpointManagerSingleZoneOneStageCooling(const Model& model)
  : SetpointManager(SetpointManagerSingleZoneOneStageCooling::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::SetpointManagerSingleZoneOneStageCooling_Impl>());

  setControlVariable("Temperature");
  setCoolingStageOnSupplyAirSetpointTemperature(-99);
  setCoolingStageOffSupplyAirSetpointTemperature(99);
}

IddObjectType SetpointManagerSingleZoneOneStageCooling::iddObjectType() {
  return IddObjectType(IddObjectType::OS_SetpointManager_SingleZone_OneStageCooling);
}

std::vector<std::string> SetpointManagerSingleZoneOneStageCooling::controlVariableValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_SetpointManager_SingleZone_OneStageCoolingFields::ControlVariable);
}

std::string SetpointManagerSingleZoneOneStageCooling::controlVariable() const {
  return getImpl<detail::SetpointManagerSingleZoneOneStageCooling_Impl>()->controlVariable();
}

double SetpointManagerSingleZoneOneStageCooling::coolingStageOnSupplyAirSetpointTemperature() const {
  return getImpl<detail::SetpointManagerSingleZoneOneStageCooling_Impl>()->coolingStageOnSupplyAirSetpointTemperature();
}

double SetpointManagerSingleZoneOneStageCooling::coolingStageOffSupplyAirSetpointTemperature() const {
  return getImpl<detail::SetpointManagerSingleZoneOneStageCooling_Impl>()->coolingStageOffSupplyAirSetpointTemperature();
}

boost::optional<ThermalZone> SetpointManagerSingleZoneOneStageCooling::controlZone() const {
  return getImpl<detail::SetpointManagerSingleZoneOneStageCooling_Impl>()->controlZone();
}

boost::optional<Node> SetpointManagerSingleZoneOneStageCooling::setpointNode() const {
  return getImpl<detail::SetpointManagerSingleZoneOneStageCooling_Impl>()->setpointNode();
}

bool SetpointManagerSingleZoneOneStageCooling::setControlVariable(const std::string& controlVariable) {
  return getImpl<detail::SetpointManagerSingleZoneOneStageCooling_Impl>()->setControlVariable(controlVariable);
}

void SetpointManagerSingleZoneOneStageCooling::setCoolingStageOnSupplyAirSetpointTemperature(double coolingStageOnSupplyAirSetpointTemperature) {
  getImpl<detail::SetpointManagerSingleZoneOneStageCooling_Impl>()->setCoolingStageOnSupplyAirSetpointTemperature(coolingStageOnSupplyAirSetpointTemperature);
}

void SetpointManagerSingleZoneOneStageCooling::setCoolingStageOffSupplyAirSetpointTemperature(double coolingStageOffSupplyAirSetpointTemperature) {
  getImpl<detail::SetpointManagerSingleZoneOneStageCooling_Impl>()->setCoolingStageOffSupplyAirSetpointTemperature(coolingStageOffSupplyAirSetpointTemperature);
}

bool SetpointManagerSingleZoneOneStageCooling::setControlZone(const ThermalZone& thermalZone) {
  return getImpl<detail::SetpointManagerSingleZoneOneStageCooling_Impl>()->setControlZone(thermalZone);
}

void SetpointManagerSingleZoneOneStageCooling::resetControlZone() {
  getImpl<detail::SetpointManagerSingleZoneOneStageCooling_Impl>()->resetControlZone();
}

/// @cond
SetpointManagerSingleZoneOneStageCooling::SetpointManagerSingleZoneOneStageCooling(std::shared_ptr<detail::SetpointManagerSingleZoneOneStageCooling_Impl> impl)
  : SetpointManager(std::move(impl))
{}
/// @endcond

} // model
} // openstudio

