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

#include <model/SetpointManager.hpp>
#include <model/SetpointManager_Impl.hpp>
#include <model/Model.hpp>
#include <model/Node.hpp>
#include <model/Node_Impl.hpp>
#include <model/AirLoopHVAC.hpp>
#include <model/PlantLoop.hpp>

#include <utilities/core/Assert.hpp>

namespace openstudio {

namespace model {

namespace detail{

  SetpointManager_Impl::SetpointManager_Impl(
      const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
    : HVACComponent_Impl(idfObject, model, keepHandle)
  {
  }

  SetpointManager_Impl::SetpointManager_Impl(
      const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle)
    : HVACComponent_Impl(other,model,keepHandle)
  {
  }

  SetpointManager_Impl::SetpointManager_Impl(
      const SetpointManager_Impl& other, 
      Model_Impl* model,
      bool keepHandles)
    : HVACComponent_Impl(other,model,keepHandles)
  {
  }

    SetpointManager_Impl::SetpointManager_Impl(IddObjectType type, Model_Impl* model)
    : HVACComponent_Impl(type,model)
  {
  }

  SetpointManager_Impl::~SetpointManager_Impl(){}

  const std::vector<std::string>& SetpointManager_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  boost::optional<ParentObject> SetpointManager_Impl::parent() const {
    NodeVector nodes = getObject<ModelObject>().getModelObjectSources<Node>();
    if (nodes.size() == 1u) {
      return nodes[0];
    }
    return boost::none;
  }

  std::vector<ModelObject> SetpointManager_Impl::children() const
  {
    std::vector<ModelObject> result;
    return result;
  }

  bool SetpointManager_Impl::addToNode(Node & node)
  {
    if( node.model() != this->model() )
    {
      return false;
    }

    // std::vector<SetpointManager> _setpointManagers = node.setpointManagers();
    // if( !_setpointManagers.empty() )
    // {
    //   for(std::vector<SetpointManager>::iterator it = _setpointManagers.begin();
    //     it != _setpointManagers.end();
    //     ++it)
    //   {
    //     if( istringEqual(this->controlVariable(), it->controlVariable()) )
    //     {
    //       it->remove();
    //     }
    //   }
    // }

    if( OptionalAirLoopHVAC airLoop = node.airLoopHVAC() )
    {
      if( airLoop->supplyComponent(node.handle()) )
      {
        this->setSetpointNode(node);
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
          this->setSetpointNode(node);
          return true;
        }
      }
    }
    else if( boost::optional<PlantLoop> plantLoop = node.plantLoop() )
    {
      if( plantLoop->supplyComponent(node.handle()) )
      {
        this->setSetpointNode(node);
        return true;
      }
    }
    return false;
  }

  std::vector<openstudio::IdfObject> SetpointManager_Impl::remove()
  {
    return HVACComponent_Impl::remove();
  }

  ModelObject SetpointManager_Impl::clone(Model model)
  {
    return HVACComponent_Impl::clone( model );
  }

  // bool SetpointManager_Impl::isTemperatureSetpointManager() const
  // {
  //   std::string controlVariable = this->controlVariable();
  //   return istringEqual(controlVariable, "Temperature") || istringEqual(controlVariable, "MaximumTemperature") || istringEqual(controlVariable, "MinimumTemperature");
  // }

  // bool SetpointManager_Impl::isHumiditySetpointManager() const
  // {
  //   std::string controlVariable = this->controlVariable();
  //   return istringEqual(controlVariable, "HumidityRatio") || istringEqual(controlVariable, "MaximumHumidityRatio") || istringEqual(controlVariable, "MinimumHumidityRatio");
  // }

  // boost::optional<Node> SetpointManager_Impl::setpointNode() const
  // {
  //   SetpointManager thisModelObject = this->getObject<SetpointManager>();

  //   return thisModelObject.getModelObjectTarget<Node>(OS_SetpointManager_ScheduledFields::SetpointNodeorNodeListName);
  // }

  // void SetpointManager_Impl::setSetpointNode( Node & node )
  // {
  //   SetpointManager thisModelObject = this->getObject<SetpointManager>();

  //   thisModelObject.setPointer(OS_SetpointManager_ScheduledFields::SetpointNodeorNodeListName,node.handle());
  // }

  // std::string SetpointManager_Impl::controlVariable() const
  // {
  //   boost::optional<std::string> value = getString(OS_SetpointManager_ScheduledFields::ControlVariable,true);
  //   OS_ASSERT(value);
  //   return value.get();
  // }

  // bool SetpointManager_Impl::setControlVariable(std::string value) {
  //   if (boost::optional<IddKey> key = iddObject().getField(OS_SetpointManager_ScheduledFields::ControlVariable).get().getKey(value)) {
  //     std::string currentScheduleDisplayName = scheduleDisplayName();
  //     if (currentScheduleDisplayName.empty() || (scheduleDisplayName() == scheduleDisplayName(key->name()))) {
  //       return setString(OS_SetpointManager_ScheduledFields::ControlVariable,key->name());
  //     }
  //   }
  //   return false;
  // }

} // detail
  
SetpointManager::SetpointManager(IddObjectType type, const Model& model)
  : HVACComponent(type, model)
{
  OS_ASSERT(getImpl<detail::SetpointManager_Impl>());
}

SetpointManager::SetpointManager(boost::shared_ptr<detail::SetpointManager_Impl> p)
  : HVACComponent(p)
{
}

boost::optional<Node> SetpointManager::setpointNode() const
{
  return getImpl<detail::SetpointManager_Impl>()->setpointNode();
}

std::string SetpointManager::controlVariable() const
{
  return getImpl<detail::SetpointManager_Impl>()->controlVariable();
}

bool SetpointManager::setControlVariable(const std::string & value)
{
  return getImpl<detail::SetpointManager_Impl>()->setControlVariable(value);
}

} // model
} // openstudio

