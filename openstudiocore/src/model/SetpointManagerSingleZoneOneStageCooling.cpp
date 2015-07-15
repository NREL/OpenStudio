/**********************************************************************
 *  Copyright (c) 2008-2015, Alliance for Sustainable Energy.
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

#include <model/SetpointManagerSingleZoneOneStageCooling.hpp>
#include <model/SetpointManagerSingleZoneOneStageCooling_Impl.hpp>

// TODO: Check the following class names against object getters and setters.
#include <model/ThermalZone.hpp>
#include <model/ThermalZone_Impl.hpp>
#include <model/Node.hpp>
#include <model/Node_Impl.hpp>

#include <utilities/idd/IddFactory.hxx>
#include <utilities/idd/OS_SetpointManager_SingleZone_OneStageCooling_FieldEnums.hxx>

#include <utilities/units/Unit.hpp>

#include <utilities/core/Assert.hpp>

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

  boost::optional<Node> SetpointManagerSingleZoneOneStageCooling_Impl::setpointNodeorNodeList() const {
    return getObject<ModelObject>().getModelObjectTarget<Node>(OS_SetpointManager_SingleZone_OneStageCoolingFields::SetpointNodeorNodeListName);
  }

  bool SetpointManagerSingleZoneOneStageCooling_Impl::setControlVariable(std::string controlVariable) {
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

  bool SetpointManagerSingleZoneOneStageCooling_Impl::setControlZone(const boost::optional<ThermalZone>& thermalZone) {
    bool result(false);
    if (thermalZone) {
      result = setPointer(OS_SetpointManager_SingleZone_OneStageCoolingFields::ControlZoneName, thermalZone.get().handle());
    }
    else {
      resetControlZone();
      result = true;
    }
    return result;
  }

  void SetpointManagerSingleZoneOneStageCooling_Impl::resetControlZone() {
    bool result = setString(OS_SetpointManager_SingleZone_OneStageCoolingFields::ControlZoneName, "");
    OS_ASSERT(result);
  }

  bool SetpointManagerSingleZoneOneStageCooling_Impl::setSetpointNodeorNodeList(const boost::optional<Node>& node) {
    bool result(false);
    if (node) {
      result = setPointer(OS_SetpointManager_SingleZone_OneStageCoolingFields::SetpointNodeorNodeListName, node.get().handle());
    }
    else {
      resetSetpointNodeorNodeList();
      result = true;
    }
    return result;
  }

  void SetpointManagerSingleZoneOneStageCooling_Impl::resetSetpointNodeorNodeList() {
    bool result = setString(OS_SetpointManager_SingleZone_OneStageCoolingFields::SetpointNodeorNodeListName, "");
    OS_ASSERT(result);
  }

} // detail

SetpointManagerSingleZoneOneStageCooling::SetpointManagerSingleZoneOneStageCooling(const Model& model)
  : SetpointManager(SetpointManagerSingleZoneOneStageCooling::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::SetpointManagerSingleZoneOneStageCooling_Impl>());

  // TODO: Appropriately handle the following required object-list fields.
  bool ok = true;
  // ok = setHandle();
  OS_ASSERT(ok);
  // ok = setControlVariable();
  OS_ASSERT(ok);
  // setCoolingStageOnSupplyAirSetpointTemperature();
  // setCoolingStageOffSupplyAirSetpointTemperature();
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

boost::optional<Node> SetpointManagerSingleZoneOneStageCooling::setpointNodeorNodeList() const {
  return getImpl<detail::SetpointManagerSingleZoneOneStageCooling_Impl>()->setpointNodeorNodeList();
}

bool SetpointManagerSingleZoneOneStageCooling::setControlVariable(std::string controlVariable) {
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

bool SetpointManagerSingleZoneOneStageCooling::setSetpointNodeorNodeList(const Node& node) {
  return getImpl<detail::SetpointManagerSingleZoneOneStageCooling_Impl>()->setSetpointNodeorNodeList(node);
}

void SetpointManagerSingleZoneOneStageCooling::resetSetpointNodeorNodeList() {
  getImpl<detail::SetpointManagerSingleZoneOneStageCooling_Impl>()->resetSetpointNodeorNodeList();
}

/// @cond
SetpointManagerSingleZoneOneStageCooling::SetpointManagerSingleZoneOneStageCooling(std::shared_ptr<detail::SetpointManagerSingleZoneOneStageCooling_Impl> impl)
  : SetpointManager(impl)
{}
/// @endcond

} // model
} // openstudio

