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

    std::vector<SetpointManager> _setpointManagers = node.setpointManagers();
    if( !_setpointManagers.empty() )
    {
      for(std::vector<SetpointManager>::iterator it = _setpointManagers.begin();
        it != _setpointManagers.end();
        ++it)
      {
        if( istringEqual(this->controlVariable(), it->controlVariable()) )
        {
          it->remove();
        }
      }
    }

    if( OptionalAirLoopHVAC airLoop = node.airLoopHVAC() )
    {
      if( airLoop->supplyComponent(node.handle()) )
      {
        return this->setSetpointNode(node);
      }
      if(OptionalAirLoopHVACOutdoorAirSystem oaSystem = airLoop->airLoopHVACOutdoorAirSystem())
      {
        if(node == oaSystem->outboardOANode().get())
        {
          return false;
        }

        if(oaSystem->oaComponent(node.handle()))
        {
          return this->setSetpointNode(node);
        }
      }
    }
    return false;
  }

  std::vector<openstudio::IdfObject> SetpointManager_Impl::remove()
  {
    return HVACComponent_Impl::remove();
  }

  ModelObject SetpointManager_Impl::clone(Model model) const
  {
    SetpointManager clonedObject = HVACComponent_Impl::clone( model ).cast<SetpointManager>();
    clonedObject.getImpl<detail::SetpointManager_Impl>()->resetSetpointNode();
    return clonedObject;
  }

  boost::optional<Loop> SetpointManager_Impl::loop() const
  {
    if( boost::optional<Node> node = setpointNode() ) {
      return node->loop();
    }
    return boost::none;
  }

  boost::optional<AirLoopHVAC> SetpointManager_Impl::airLoopHVAC() const
  {
    if( boost::optional<Node> node = setpointNode() ) {
      return node->airLoopHVAC();
    }
    return boost::none;
  }

  boost::optional<AirLoopHVACOutdoorAirSystem> SetpointManager_Impl::airLoopHVACOutdoorAirSystem() const
  {
    if( boost::optional<Node> node = setpointNode() ) {
      return node->airLoopHVACOutdoorAirSystem();
    }
    return boost::none;
  }

  boost::optional<PlantLoop> SetpointManager_Impl::plantLoop() const
  {
    if( boost::optional<Node> node = setpointNode() ) {
      return node->plantLoop();
    }
    return boost::none;
  }

} // detail
  
SetpointManager::SetpointManager(IddObjectType type, const Model& model)
  : HVACComponent(type, model)
{
  OS_ASSERT(getImpl<detail::SetpointManager_Impl>());
}

SetpointManager::SetpointManager(std::shared_ptr<detail::SetpointManager_Impl> p)
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

