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

#include "SetpointManagerSingleZoneHumidityMaximum.hpp"
#include "SetpointManagerSingleZoneHumidityMaximum_Impl.hpp"

#include "ThermalZone.hpp"
#include "ThermalZone_Impl.hpp"
#include "Node.hpp"
#include "Node_Impl.hpp"
#include "Model.hpp"
#include "AirLoopHVAC.hpp"

#include <utilities/idd/IddFactory.hxx>
#include <utilities/idd/OS_SetpointManager_SingleZone_Humidity_Maximum_FieldEnums.hxx>

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  SetpointManagerSingleZoneHumidityMaximum_Impl::SetpointManagerSingleZoneHumidityMaximum_Impl(const IdfObject& idfObject,
                                                                                               Model_Impl* model,
                                                                                               bool keepHandle)
    : SetpointManager_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == SetpointManagerSingleZoneHumidityMaximum::iddObjectType());
  }

  SetpointManagerSingleZoneHumidityMaximum_Impl::SetpointManagerSingleZoneHumidityMaximum_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                                               Model_Impl* model,
                                                                                               bool keepHandle)
    : SetpointManager_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == SetpointManagerSingleZoneHumidityMaximum::iddObjectType());
  }

  SetpointManagerSingleZoneHumidityMaximum_Impl::SetpointManagerSingleZoneHumidityMaximum_Impl(const SetpointManagerSingleZoneHumidityMaximum_Impl& other,
                                                                                               Model_Impl* model,
                                                                                               bool keepHandle)
    : SetpointManager_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& SetpointManagerSingleZoneHumidityMaximum_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType SetpointManagerSingleZoneHumidityMaximum_Impl::iddObjectType() const {
    return SetpointManagerSingleZoneHumidityMaximum::iddObjectType();
  }

  bool SetpointManagerSingleZoneHumidityMaximum_Impl::addToNode(Node & node)
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

  ModelObject SetpointManagerSingleZoneHumidityMaximum_Impl::clone(Model model) const
  {
    SetpointManagerSingleZoneHumidityMaximum clonedObject = SetpointManager_Impl::clone( model ).cast<SetpointManagerSingleZoneHumidityMaximum>();
    clonedObject.resetControlZone();
    return clonedObject;
  }

  std::string SetpointManagerSingleZoneHumidityMaximum_Impl::controlVariable() const {
    boost::optional<std::string> value = getString(OS_SetpointManager_SingleZone_Humidity_MaximumFields::ControlVariable,true);
    OS_ASSERT(value);
    return value.get();
  }

  boost::optional<ThermalZone> SetpointManagerSingleZoneHumidityMaximum_Impl::controlZone() const {
    return getObject<ModelObject>().getModelObjectTarget<ThermalZone>(OS_SetpointManager_SingleZone_Humidity_MaximumFields::ControlZoneName);
  }

  boost::optional<Node> SetpointManagerSingleZoneHumidityMaximum_Impl::setpointNode() const {
    return getObject<ModelObject>().getModelObjectTarget<Node>(OS_SetpointManager_SingleZone_Humidity_MaximumFields::SetpointNodeorNodeListName);
  }

  bool SetpointManagerSingleZoneHumidityMaximum_Impl::setControlVariable(const std::string& controlVariable) {
    bool result = setString(OS_SetpointManager_SingleZone_Humidity_MaximumFields::ControlVariable, controlVariable);
    return result;
  }

  bool SetpointManagerSingleZoneHumidityMaximum_Impl::setControlZone(const ThermalZone& thermalZone) {
    bool result = setPointer(OS_SetpointManager_SingleZone_Humidity_MaximumFields::ControlZoneName, thermalZone.handle());
    return result;
  }

  void SetpointManagerSingleZoneHumidityMaximum_Impl::resetControlZone() {
    bool result = setString(OS_SetpointManager_SingleZone_Humidity_MaximumFields::ControlZoneName, "");
    OS_ASSERT(result);
  }

  bool SetpointManagerSingleZoneHumidityMaximum_Impl::setSetpointNode(const Node& node) {
    bool result = setPointer(OS_SetpointManager_SingleZone_Humidity_MaximumFields::SetpointNodeorNodeListName, node.handle());
    return result;
  }

  void SetpointManagerSingleZoneHumidityMaximum_Impl::resetSetpointNode() {
    bool result = setString(OS_SetpointManager_SingleZone_Humidity_MaximumFields::SetpointNodeorNodeListName, "");
    OS_ASSERT(result);
  }

} // detail

SetpointManagerSingleZoneHumidityMaximum::SetpointManagerSingleZoneHumidityMaximum(const Model& model)
  : SetpointManager(SetpointManagerSingleZoneHumidityMaximum::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::SetpointManagerSingleZoneHumidityMaximum_Impl>());

  setControlVariable("MaximumHumidityRatio");
}

IddObjectType SetpointManagerSingleZoneHumidityMaximum::iddObjectType() {
  return IddObjectType(IddObjectType::OS_SetpointManager_SingleZone_Humidity_Maximum);
}

std::vector<std::string> SetpointManagerSingleZoneHumidityMaximum::controlVariableValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_SetpointManager_SingleZone_Humidity_MaximumFields::ControlVariable);
}

std::string SetpointManagerSingleZoneHumidityMaximum::controlVariable() const {
  return getImpl<detail::SetpointManagerSingleZoneHumidityMaximum_Impl>()->controlVariable();
}

boost::optional<ThermalZone> SetpointManagerSingleZoneHumidityMaximum::controlZone() const {
  return getImpl<detail::SetpointManagerSingleZoneHumidityMaximum_Impl>()->controlZone();
}

boost::optional<Node> SetpointManagerSingleZoneHumidityMaximum::setpointNode() const {
  return getImpl<detail::SetpointManagerSingleZoneHumidityMaximum_Impl>()->setpointNode();
}

bool SetpointManagerSingleZoneHumidityMaximum::setControlVariable(const std::string& controlVariable) {
  return getImpl<detail::SetpointManagerSingleZoneHumidityMaximum_Impl>()->setControlVariable(controlVariable);
}

bool SetpointManagerSingleZoneHumidityMaximum::setControlZone(const ThermalZone& thermalZone) {
  return getImpl<detail::SetpointManagerSingleZoneHumidityMaximum_Impl>()->setControlZone(thermalZone);
}

void SetpointManagerSingleZoneHumidityMaximum::resetControlZone() {
  getImpl<detail::SetpointManagerSingleZoneHumidityMaximum_Impl>()->resetControlZone();
}

/// @cond
SetpointManagerSingleZoneHumidityMaximum::SetpointManagerSingleZoneHumidityMaximum(std::shared_ptr<detail::SetpointManagerSingleZoneHumidityMaximum_Impl> impl)
  : SetpointManager(impl)
{}
/// @endcond

} // model
} // openstudio

