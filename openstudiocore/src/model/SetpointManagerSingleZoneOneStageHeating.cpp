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

#include <model/SetpointManagerSingleZoneOneStageHeating.hpp>
#include <model/SetpointManagerSingleZoneOneStageHeating_Impl.hpp>

// TODO: Check the following class names against object getters and setters.
#include <model/ThermalZone.hpp>
#include <model/ThermalZone_Impl.hpp>
#include <model/Node.hpp>
#include <model/Node_Impl.hpp>

#include <utilities/idd/IddFactory.hxx>
#include <utilities/idd/OS_SetpointManager_SingleZone_OneStageHeating_FieldEnums.hxx>

#include <utilities/units/Unit.hpp>

#include <utilities/core/Assert.hpp>

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

  boost::optional<Node> SetpointManagerSingleZoneOneStageHeating_Impl::setpointNodeorNodeList() const {
    return getObject<ModelObject>().getModelObjectTarget<Node>(OS_SetpointManager_SingleZone_OneStageHeatingFields::SetpointNodeorNodeListName);
  }

  bool SetpointManagerSingleZoneOneStageHeating_Impl::setControlVariable(std::string controlVariable) {
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

  bool SetpointManagerSingleZoneOneStageHeating_Impl::setControlZone(const boost::optional<ThermalZone>& thermalZone) {
    bool result(false);
    if (thermalZone) {
      result = setPointer(OS_SetpointManager_SingleZone_OneStageHeatingFields::ControlZoneName, thermalZone.get().handle());
    }
    else {
      resetControlZone();
      result = true;
    }
    return result;
  }

  void SetpointManagerSingleZoneOneStageHeating_Impl::resetControlZone() {
    bool result = setString(OS_SetpointManager_SingleZone_OneStageHeatingFields::ControlZoneName, "");
    OS_ASSERT(result);
  }

  bool SetpointManagerSingleZoneOneStageHeating_Impl::setSetpointNodeorNodeList(const boost::optional<Node>& node) {
    bool result(false);
    if (node) {
      result = setPointer(OS_SetpointManager_SingleZone_OneStageHeatingFields::SetpointNodeorNodeListName, node.get().handle());
    }
    else {
      resetSetpointNodeorNodeList();
      result = true;
    }
    return result;
  }

  void SetpointManagerSingleZoneOneStageHeating_Impl::resetSetpointNodeorNodeList() {
    bool result = setString(OS_SetpointManager_SingleZone_OneStageHeatingFields::SetpointNodeorNodeListName, "");
    OS_ASSERT(result);
  }

} // detail

SetpointManagerSingleZoneOneStageHeating::SetpointManagerSingleZoneOneStageHeating(const Model& model)
  : SetpointManager(SetpointManagerSingleZoneOneStageHeating::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::SetpointManagerSingleZoneOneStageHeating_Impl>());

  // TODO: Appropriately handle the following required object-list fields.
  bool ok = true;
  // ok = setHandle();
  OS_ASSERT(ok);
  // ok = setControlVariable();
  OS_ASSERT(ok);
  // setHeatingStageOnSupplyAirSetpointTemperature();
  // setHeatingStageOffSupplyAirSetpointTemperature();
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

boost::optional<Node> SetpointManagerSingleZoneOneStageHeating::setpointNodeorNodeList() const {
  return getImpl<detail::SetpointManagerSingleZoneOneStageHeating_Impl>()->setpointNodeorNodeList();
}

bool SetpointManagerSingleZoneOneStageHeating::setControlVariable(std::string controlVariable) {
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

bool SetpointManagerSingleZoneOneStageHeating::setSetpointNodeorNodeList(const Node& node) {
  return getImpl<detail::SetpointManagerSingleZoneOneStageHeating_Impl>()->setSetpointNodeorNodeList(node);
}

void SetpointManagerSingleZoneOneStageHeating::resetSetpointNodeorNodeList() {
  getImpl<detail::SetpointManagerSingleZoneOneStageHeating_Impl>()->resetSetpointNodeorNodeList();
}

/// @cond
SetpointManagerSingleZoneOneStageHeating::SetpointManagerSingleZoneOneStageHeating(std::shared_ptr<detail::SetpointManagerSingleZoneOneStageHeating_Impl> impl)
  : SetpointManager(impl)
{}
/// @endcond

} // model
} // openstudio

