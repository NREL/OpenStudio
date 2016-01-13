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

#include "SetpointManagerSingleZoneReheat.hpp"
#include "SetpointManagerSingleZoneReheat_Impl.hpp"
#include "Model.hpp"
#include "Node.hpp"
#include "Node_Impl.hpp"
#include "AirLoopHVAC.hpp"
#include "ThermalZone.hpp"
#include "ThermalZone_Impl.hpp"
#include "../utilities/core/Assert.hpp"
#include <utilities/idd/IddFactory.hxx>

#include <utilities/idd/OS_SetpointManager_SingleZone_Reheat_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

namespace openstudio {

namespace model {

namespace detail{

  SetpointManagerSingleZoneReheat_Impl::SetpointManagerSingleZoneReheat_Impl(
      const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
    : SetpointManager_Impl(idfObject, model, keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == SetpointManagerSingleZoneReheat::iddObjectType());
  }

  SetpointManagerSingleZoneReheat_Impl::SetpointManagerSingleZoneReheat_Impl(
      const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle)
    : SetpointManager_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == SetpointManagerSingleZoneReheat::iddObjectType());
  }

  SetpointManagerSingleZoneReheat_Impl::SetpointManagerSingleZoneReheat_Impl(
      const SetpointManagerSingleZoneReheat_Impl& other, 
      Model_Impl* model,
      bool keepHandles)
    : SetpointManager_Impl(other,model,keepHandles)
  {
  }

  SetpointManagerSingleZoneReheat_Impl::~SetpointManagerSingleZoneReheat_Impl(){}

  const std::vector<std::string>& SetpointManagerSingleZoneReheat_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType SetpointManagerSingleZoneReheat_Impl::iddObjectType() const {
    return SetpointManagerSingleZoneReheat::iddObjectType();
  }

  bool SetpointManagerSingleZoneReheat_Impl::addToNode(Node & node)
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

  ModelObject SetpointManagerSingleZoneReheat_Impl::clone(Model model) const
  {
    SetpointManagerSingleZoneReheat clonedObject = SetpointManager_Impl::clone( model ).cast<SetpointManagerSingleZoneReheat>();
    clonedObject.resetControlZone();
    return clonedObject;
  }

  std::string SetpointManagerSingleZoneReheat_Impl::controlVariable() const {
    boost::optional<std::string> value = getString(OS_SetpointManager_SingleZone_ReheatFields::ControlVariable,true);
    OS_ASSERT(value);
    return value.get();
  }

  double SetpointManagerSingleZoneReheat_Impl::minimumSupplyAirTemperature()
  {
    return getDouble(OS_SetpointManager_SingleZone_ReheatFields::MinimumSupplyAirTemperature).get();
  }

  void SetpointManagerSingleZoneReheat_Impl::setMinimumSupplyAirTemperature( double value )
  {
    setDouble(OS_SetpointManager_SingleZone_ReheatFields::MinimumSupplyAirTemperature,value);
  }

  double SetpointManagerSingleZoneReheat_Impl::maximumSupplyAirTemperature()
  {
    return getDouble(OS_SetpointManager_SingleZone_ReheatFields::MaximumSupplyAirTemperature).get();
  }

  void SetpointManagerSingleZoneReheat_Impl::setMaximumSupplyAirTemperature( double value )
  {
    setDouble(OS_SetpointManager_SingleZone_ReheatFields::MaximumSupplyAirTemperature,value);
  }

  boost::optional<Node> SetpointManagerSingleZoneReheat_Impl::setpointNode() const
  {
    return getObject<ModelObject>().getModelObjectTarget<Node>(OS_SetpointManager_SingleZone_ReheatFields::SetpointNodeorNodeListName);
  }

  bool SetpointManagerSingleZoneReheat_Impl::setSetpointNode( const Node & node )
  {
   return setPointer(OS_SetpointManager_SingleZone_ReheatFields::SetpointNodeorNodeListName, node.handle());
  }

  void SetpointManagerSingleZoneReheat_Impl::resetSetpointNode()
  {
    bool result = setString(OS_SetpointManager_SingleZone_ReheatFields::SetpointNodeorNodeListName,"");
    OS_ASSERT(result);
  }

  boost::optional<ThermalZone> SetpointManagerSingleZoneReheat_Impl::controlZone()
  {
    SetpointManagerSingleZoneReheat thisModelObject = this->getObject<SetpointManagerSingleZoneReheat>();

    return thisModelObject.getModelObjectTarget<ThermalZone>(OS_SetpointManager_SingleZone_ReheatFields::ControlZoneName);
  }

  void SetpointManagerSingleZoneReheat_Impl::setControlZone(ThermalZone & thermalZone)
  {
    SetpointManagerSingleZoneReheat thisModelObject = this->getObject<SetpointManagerSingleZoneReheat>();

    thisModelObject.setPointer(OS_SetpointManager_SingleZone_ReheatFields::ControlZoneName,thermalZone.handle());
  }

  bool SetpointManagerSingleZoneReheat_Impl::setControlVariable(const std::string& controlVariable) {
    return setString(OS_SetpointManager_SingleZone_ReheatFields::ControlVariable, controlVariable);
  }

  boost::optional<ModelObject> SetpointManagerSingleZoneReheat_Impl::controlZoneAsModelObject() {
    OptionalModelObject result;
    OptionalThermalZone intermediate = controlZone();
    if (intermediate) {
      result = *intermediate;
    }
    return result;
  }

  bool SetpointManagerSingleZoneReheat_Impl::setControlZoneAsModelObject(const boost::optional<ModelObject>& modelObject) {
    if (modelObject) {
      OptionalThermalZone intermediate = modelObject->optionalCast<ThermalZone>();
      if (intermediate) {
        setControlZone(*intermediate);
        return true;
      }
    }
    return false;
  }

  void SetpointManagerSingleZoneReheat_Impl::resetControlZone()
  {
    setString(OS_SetpointManager_SingleZone_ReheatFields::ControlZoneName,"");
  }

} // detail
  
SetpointManagerSingleZoneReheat::SetpointManagerSingleZoneReheat(const Model& model)
  : SetpointManager(SetpointManagerSingleZoneReheat::iddObjectType(),model) 
{
  OS_ASSERT(getImpl<detail::SetpointManagerSingleZoneReheat_Impl>());

  setMinimumSupplyAirTemperature(-99.0);

  setMaximumSupplyAirTemperature(99.0);
}

SetpointManagerSingleZoneReheat::SetpointManagerSingleZoneReheat(std::shared_ptr<detail::SetpointManagerSingleZoneReheat_Impl> p)
  : SetpointManager(p)
{
}

std::vector<std::string> SetpointManagerSingleZoneReheat::controlVariableValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_SetpointManager_SingleZone_ReheatFields::ControlVariable);
}

IddObjectType SetpointManagerSingleZoneReheat::iddObjectType() {
  IddObjectType result(IddObjectType::OS_SetpointManager_SingleZone_Reheat);
  return result;
}

double SetpointManagerSingleZoneReheat::minimumSupplyAirTemperature()
{
  return getImpl<detail::SetpointManagerSingleZoneReheat_Impl>()->minimumSupplyAirTemperature();
}

void SetpointManagerSingleZoneReheat::setMinimumSupplyAirTemperature( double value )
{
  getImpl<detail::SetpointManagerSingleZoneReheat_Impl>()->setMinimumSupplyAirTemperature(value);
}

double SetpointManagerSingleZoneReheat::maximumSupplyAirTemperature()
{
  return getImpl<detail::SetpointManagerSingleZoneReheat_Impl>()->maximumSupplyAirTemperature();
}

void SetpointManagerSingleZoneReheat::setMaximumSupplyAirTemperature( double value )
{
  getImpl<detail::SetpointManagerSingleZoneReheat_Impl>()->setMaximumSupplyAirTemperature(value);
}

boost::optional<Node> SetpointManagerSingleZoneReheat::setpointNode() const
{
  return getImpl<detail::SetpointManagerSingleZoneReheat_Impl>()->setpointNode();
}

boost::optional<ThermalZone> SetpointManagerSingleZoneReheat::controlZone()
{
  return getImpl<detail::SetpointManagerSingleZoneReheat_Impl>()->controlZone();
}

void SetpointManagerSingleZoneReheat::setControlZone(ThermalZone & thermalZone)
{
  getImpl<detail::SetpointManagerSingleZoneReheat_Impl>()->setControlZone(thermalZone);
}

void SetpointManagerSingleZoneReheat::resetControlZone()
{
  getImpl<detail::SetpointManagerSingleZoneReheat_Impl>()->resetControlZone();
}

std::string SetpointManagerSingleZoneReheat::controlVariable() const
{
  return getImpl<detail::SetpointManagerSingleZoneReheat_Impl>()->controlVariable();
}

bool SetpointManagerSingleZoneReheat::setControlVariable( const std::string& controlVariable )
{
  return getImpl<detail::SetpointManagerSingleZoneReheat_Impl>()->setControlVariable(controlVariable);
}

} // model
} // openstudio
