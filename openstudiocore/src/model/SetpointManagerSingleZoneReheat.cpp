/**********************************************************************
 *  Copyright (c) 2008-2013, Alliance for Sustainable Energy.  
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

#include <model/SetpointManagerSingleZoneReheat.hpp>
#include <model/SetpointManagerSingleZoneReheat_Impl.hpp>
#include <model/Model.hpp>
#include <model/Node.hpp>
#include <model/Node_Impl.hpp>
#include <model/AirLoopHVAC.hpp>
#include <model/AirLoopHVAC_Impl.hpp>
#include <model/AirLoopHVACOutdoorAirSystem.hpp>
#include <model/AirLoopHVACOutdoorAirSystem_Impl.hpp>
#include <model/ThermalZone.hpp>
#include <model/ThermalZone_Impl.hpp>
#include <utilities/core/Compare.hpp>
#include <utilities/core/Assert.hpp>
#include <utilities/idd/OS_SetpointManager_SingleZone_Reheat_FieldEnums.hxx>
#include <boost/foreach.hpp>

using openstudio::Handle;
using openstudio::OptionalHandle;
using openstudio::HandleVector;
using openstudio::IdfObject;
using openstudio::WorkspaceObject;
using openstudio::OptionalWorkspaceObject;
using openstudio::WorkspaceObjectVector;
using openstudio::Workspace;
using openstudio::istringEqual;

namespace openstudio {

namespace model {

namespace detail{

  SetpointManagerSingleZoneReheat_Impl::SetpointManagerSingleZoneReheat_Impl(
      const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
    : HVACComponent_Impl(idfObject, model, keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == SetpointManagerSingleZoneReheat::iddObjectType());
  }

  SetpointManagerSingleZoneReheat_Impl::SetpointManagerSingleZoneReheat_Impl(
      const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle)
    : HVACComponent_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == SetpointManagerSingleZoneReheat::iddObjectType());
  }

  SetpointManagerSingleZoneReheat_Impl::SetpointManagerSingleZoneReheat_Impl(
      const SetpointManagerSingleZoneReheat_Impl& other, 
      Model_Impl* model,
      bool keepHandles)
    : HVACComponent_Impl(other,model,keepHandles)
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

  boost::optional<ParentObject> SetpointManagerSingleZoneReheat_Impl::parent() const
  {
    NodeVector nodes = getObject<ModelObject>().getModelObjectSources<Node>();
    if (nodes.size() == 1u) {
      return nodes[0];
    }
    return boost::none;
  }

  std::vector<ModelObject> SetpointManagerSingleZoneReheat_Impl::children() const
  {
    std::vector<ModelObject> result;
    return result;
  }

  bool SetpointManagerSingleZoneReheat_Impl::addToNode(Node & node)
  {
    if( OptionalAirLoopHVAC airLoop = node.airLoopHVAC() )
    {
      if( airLoop->supplyComponent(node.handle()) )
      {
        node.addSetpointManager(this->getObject<SetpointManagerSingleZoneReheat>());

        return true;
      }
      if(OptionalAirLoopHVACOutdoorAirSystem oaSystem = airLoop->airLoopHVACOutdoorAirSystem())
      {
        if(node == oaSystem->outboardOANode().get())
        {
          return false;
        }

        if(oaSystem->oaComponent(node.handle()))
        {
          node.addSetpointManager(this->getObject<SetpointManagerSingleZoneReheat>());
        
          return true;
        }
      }
    }

    return false;
  }

  std::vector<openstudio::IdfObject> SetpointManagerSingleZoneReheat_Impl::remove()
  {
    return HVACComponent_Impl::remove();
  }

  ModelObject SetpointManagerSingleZoneReheat_Impl::clone(Model model)
  {
    return HVACComponent_Impl::clone( model );
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

  boost::optional<Node> SetpointManagerSingleZoneReheat_Impl::setpointNode()
  {
    SetpointManagerSingleZoneReheat thisModelObject = this->getObject<SetpointManagerSingleZoneReheat>();

    return thisModelObject.getModelObjectTarget<Node>(OS_SetpointManager_SingleZone_ReheatFields::SetpointNodeorNodeListName);
  }

  void SetpointManagerSingleZoneReheat_Impl::setSetpointNode( Node & node )
  {
    SetpointManagerSingleZoneReheat thisModelObject = this->getObject<SetpointManagerSingleZoneReheat>();

    thisModelObject.setPointer(OS_SetpointManager_SingleZone_ReheatFields::SetpointNodeorNodeListName,node.handle());
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
  : HVACComponent(SetpointManagerSingleZoneReheat::iddObjectType(),model) 
{
  OS_ASSERT(getImpl<detail::SetpointManagerSingleZoneReheat_Impl>());

  setMinimumSupplyAirTemperature(-99.0);

  setMaximumSupplyAirTemperature(99.0);
}

SetpointManagerSingleZoneReheat::SetpointManagerSingleZoneReheat(boost::shared_ptr<detail::SetpointManagerSingleZoneReheat_Impl> p)
  : HVACComponent(p)
{
}

bool SetpointManagerSingleZoneReheat::addToNode(Node & node)
{
  return getImpl<detail::SetpointManagerSingleZoneReheat_Impl>()->addToNode( node );
}

std::vector<openstudio::IdfObject> SetpointManagerSingleZoneReheat::remove()
{
  return getImpl<detail::SetpointManagerSingleZoneReheat_Impl>()->remove();
}

ModelObject SetpointManagerSingleZoneReheat::clone(Model model) const
{
  return getImpl<detail::SetpointManagerSingleZoneReheat_Impl>()->clone( model );
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

boost::optional<Node> SetpointManagerSingleZoneReheat::setpointNode()
{
  return getImpl<detail::SetpointManagerSingleZoneReheat_Impl>()->setpointNode();
}

void SetpointManagerSingleZoneReheat::setSetpointNode( Node & node )
{
  getImpl<detail::SetpointManagerSingleZoneReheat_Impl>()->setSetpointNode(node);
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

} // model
} // openstudio
