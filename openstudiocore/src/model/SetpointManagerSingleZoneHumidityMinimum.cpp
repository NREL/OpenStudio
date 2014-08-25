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

#include <model/SetpointManagerSingleZoneHumidityMinimum.hpp>
#include <model/SetpointManagerSingleZoneHumidityMinimum_Impl.hpp>

// TODO: Check the following class names against object getters and setters.
#include <model/Node.hpp>
#include <model/Node_Impl.hpp>
#include <model/ThermalZone.hpp>
#include <model/ThermalZone_Impl.hpp>

#include <utilities/idd/IddFactory.hxx>
#include <utilities/idd/OS_SetpointManager_SingleZone_Humidity_Minimum_FieldEnums.hxx>

#include <utilities/core/Assert.hpp>

namespace openstudio {
namespace model {

namespace detail {

  SetpointManagerSingleZoneHumidityMinimum_Impl::SetpointManagerSingleZoneHumidityMinimum_Impl(const IdfObject& idfObject,
                                                                                               Model_Impl* model,
                                                                                               bool keepHandle)
    : SetpointManager_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == SetpointManagerSingleZoneHumidityMinimum::iddObjectType());
  }

  SetpointManagerSingleZoneHumidityMinimum_Impl::SetpointManagerSingleZoneHumidityMinimum_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                                               Model_Impl* model,
                                                                                               bool keepHandle)
    : SetpointManager_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == SetpointManagerSingleZoneHumidityMinimum::iddObjectType());
  }

  SetpointManagerSingleZoneHumidityMinimum_Impl::SetpointManagerSingleZoneHumidityMinimum_Impl(const SetpointManagerSingleZoneHumidityMinimum_Impl& other,
                                                                                               Model_Impl* model,
                                                                                               bool keepHandle)
    : SetpointManager_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& SetpointManagerSingleZoneHumidityMinimum_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType SetpointManagerSingleZoneHumidityMinimum_Impl::iddObjectType() const {
    return SetpointManagerSingleZoneHumidityMinimum::iddObjectType();
  }

  std::string SetpointManagerSingleZoneHumidityMinimum_Impl::controlVariable() const {
    boost::optional<std::string> value = getString(OS_SetpointManager_SingleZone_Humidity_MinimumFields::ControlVariable,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool SetpointManagerSingleZoneHumidityMinimum_Impl::isControlVariableDefaulted() const {
    return isEmpty(OS_SetpointManager_SingleZone_Humidity_MinimumFields::ControlVariable);
  }

  Node SetpointManagerSingleZoneHumidityMinimum_Impl::setpointNodeorNodeList() const {
    boost::optional<Node> value = optionalSetpointNodeorNodeList();
    if (!value) {
      LOG_AND_THROW(briefDescription() << " does not have an Setpoint Nodeor Node List attached.");
    }
    return value.get();
  }

  ThermalZone SetpointManagerSingleZoneHumidityMinimum_Impl::controlZone() const {
    boost::optional<ThermalZone> value = optionalControlZone();
    if (!value) {
      LOG_AND_THROW(briefDescription() << " does not have an Control Zone attached.");
    }
    return value.get();
  }

  bool SetpointManagerSingleZoneHumidityMinimum_Impl::setControlVariable(std::string controlVariable) {
    bool result = setString(OS_SetpointManager_SingleZone_Humidity_MinimumFields::ControlVariable, controlVariable);
    return result;
  }

  void SetpointManagerSingleZoneHumidityMinimum_Impl::resetControlVariable() {
    bool result = setString(OS_SetpointManager_SingleZone_Humidity_MinimumFields::ControlVariable, "");
    OS_ASSERT(result);
  }

  bool SetpointManagerSingleZoneHumidityMinimum_Impl::setSetpointNodeorNodeList(const Node& node) {
    bool result = setPointer(OS_SetpointManager_SingleZone_Humidity_MinimumFields::SetpointNodeorNodeListName, node.handle());
    return result;
  }

  bool SetpointManagerSingleZoneHumidityMinimum_Impl::setControlZone(const ThermalZone& thermalZone) {
    bool result = setPointer(OS_SetpointManager_SingleZone_Humidity_MinimumFields::ControlZoneName, thermalZone.handle());
    return result;
  }

  boost::optional<Node> SetpointManagerSingleZoneHumidityMinimum_Impl::optionalSetpointNodeorNodeList() const {
    return getObject<ModelObject>().getModelObjectTarget<Node>(OS_SetpointManager_SingleZone_Humidity_MinimumFields::SetpointNodeorNodeListName);
  }

  boost::optional<ThermalZone> SetpointManagerSingleZoneHumidityMinimum_Impl::optionalControlZone() const {
    return getObject<ModelObject>().getModelObjectTarget<ThermalZone>(OS_SetpointManager_SingleZone_Humidity_MinimumFields::ControlZoneName);
  }

} // detail

SetpointManagerSingleZoneHumidityMinimum::SetpointManagerSingleZoneHumidityMinimum(const Model& model)
  : SetpointManager(SetpointManagerSingleZoneHumidityMinimum::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::SetpointManagerSingleZoneHumidityMinimum_Impl>());

  // TODO: Appropriately handle the following required object-list fields.
  //     OS_SetpointManager_SingleZone_Humidity_MinimumFields::SetpointNodeorNodeListName
  //     OS_SetpointManager_SingleZone_Humidity_MinimumFields::ControlZoneName
  bool ok = true;
  // ok = setHandle();
  OS_ASSERT(ok);
  // ok = setSetpointNodeorNodeList();
  OS_ASSERT(ok);
  // ok = setControlZone();
  OS_ASSERT(ok);
}

IddObjectType SetpointManagerSingleZoneHumidityMinimum::iddObjectType() {
  return IddObjectType(IddObjectType::OS_SetpointManager_SingleZone_Humidity_Minimum);
}

std::vector<std::string> SetpointManagerSingleZoneHumidityMinimum::controlVariableValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_SetpointManager_SingleZone_Humidity_MinimumFields::ControlVariable);
}

std::string SetpointManagerSingleZoneHumidityMinimum::controlVariable() const {
  return getImpl<detail::SetpointManagerSingleZoneHumidityMinimum_Impl>()->controlVariable();
}

bool SetpointManagerSingleZoneHumidityMinimum::isControlVariableDefaulted() const {
  return getImpl<detail::SetpointManagerSingleZoneHumidityMinimum_Impl>()->isControlVariableDefaulted();
}

Node SetpointManagerSingleZoneHumidityMinimum::setpointNodeorNodeList() const {
  return getImpl<detail::SetpointManagerSingleZoneHumidityMinimum_Impl>()->setpointNodeorNodeList();
}

ThermalZone SetpointManagerSingleZoneHumidityMinimum::controlZone() const {
  return getImpl<detail::SetpointManagerSingleZoneHumidityMinimum_Impl>()->controlZone();
}

bool SetpointManagerSingleZoneHumidityMinimum::setControlVariable(std::string controlVariable) {
  return getImpl<detail::SetpointManagerSingleZoneHumidityMinimum_Impl>()->setControlVariable(controlVariable);
}

void SetpointManagerSingleZoneHumidityMinimum::resetControlVariable() {
  getImpl<detail::SetpointManagerSingleZoneHumidityMinimum_Impl>()->resetControlVariable();
}

bool SetpointManagerSingleZoneHumidityMinimum::setSetpointNodeorNodeList(const Node& node) {
  return getImpl<detail::SetpointManagerSingleZoneHumidityMinimum_Impl>()->setSetpointNodeorNodeList(node);
}

bool SetpointManagerSingleZoneHumidityMinimum::setControlZone(const ThermalZone& thermalZone) {
  return getImpl<detail::SetpointManagerSingleZoneHumidityMinimum_Impl>()->setControlZone(thermalZone);
}

/// @cond
SetpointManagerSingleZoneHumidityMinimum::SetpointManagerSingleZoneHumidityMinimum(boost::shared_ptr<detail::SetpointManagerSingleZoneHumidityMinimum_Impl> impl)
  : SetpointManager(impl)
{}
/// @endcond

} // model
} // openstudio

