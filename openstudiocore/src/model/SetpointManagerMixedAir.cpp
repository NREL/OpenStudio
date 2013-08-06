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

#include <model/SetpointManagerMixedAir.hpp>
#include <model/SetpointManagerMixedAir_Impl.hpp>
#include <model/Model.hpp>
#include <model/Node.hpp>
#include <model/Node_Impl.hpp>
#include <model/FanConstantVolume.hpp>
#include <model/FanConstantVolume_Impl.hpp>
#include <model/FanVariableVolume.hpp>
#include <model/FanVariableVolume_Impl.hpp>
#include <model/FanOnOff.hpp>
#include <model/FanOnOff_Impl.hpp>
#include <model/AirLoopHVAC.hpp>
#include <model/AirLoopHVAC_Impl.hpp>
#include <model/AirLoopHVACOutdoorAirSystem.hpp>
#include <model/AirLoopHVACOutdoorAirSystem_Impl.hpp>
#include <utilities/idd/OS_SetpointManager_MixedAir_FieldEnums.hxx>
#include <utilities/core/Compare.hpp>
#include <utilities/core/Assert.hpp>
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

SetpointManagerMixedAir_Impl::SetpointManagerMixedAir_Impl(const IdfObject& idfObject, 
                                                           Model_Impl* model, 
                                                           bool keepHandle)
  : HVACComponent_Impl(idfObject, model, keepHandle)
{
  OS_ASSERT(idfObject.iddObject().type() == SetpointManagerMixedAir::iddObjectType());
}

SetpointManagerMixedAir_Impl::SetpointManagerMixedAir_Impl(
    const openstudio::detail::WorkspaceObject_Impl& other,Model_Impl* model,bool keepHandle)
    : HVACComponent_Impl(other,model,keepHandle)
{
  OS_ASSERT(other.iddObject().type() == SetpointManagerMixedAir::iddObjectType());
}

SetpointManagerMixedAir_Impl::SetpointManagerMixedAir_Impl(
    const SetpointManagerMixedAir_Impl& other, 
    Model_Impl* model,
    bool keepHandles)
  : HVACComponent_Impl(other,model,keepHandles)
{
}

SetpointManagerMixedAir_Impl::~SetpointManagerMixedAir_Impl(){}

const std::vector<std::string>& SetpointManagerMixedAir_Impl::outputVariableNames() const
{
  static std::vector<std::string> result;
  if (result.empty()){
  }
  return result;
}

IddObjectType SetpointManagerMixedAir_Impl::iddObjectType() const {
  return SetpointManagerMixedAir::iddObjectType();
}

boost::optional<ParentObject> SetpointManagerMixedAir_Impl::parent() const {
  NodeVector nodes = getObject<ModelObject>().getModelObjectSources<Node>();
  if (nodes.size() == 1u) {
    return nodes[0];
  }
  return boost::none;
}

std::vector<ModelObject> SetpointManagerMixedAir_Impl::children() const
{
  std::vector<ModelObject> result;
  return result;
}

bool SetpointManagerMixedAir_Impl::addToNode(Node & node)
{
  if( OptionalAirLoopHVAC airLoop = node.airLoopHVAC() )
  {
    if( airLoop->supplyComponent(node.handle()) )
    {
      node.addSetpointManager(this->getObject<SetpointManagerMixedAir>());

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
        node.addSetpointManager(this->getObject<SetpointManagerMixedAir>());
      
        return true;
      }
    }
  }

  return false;
}

std::vector<openstudio::IdfObject> SetpointManagerMixedAir_Impl::remove()
{
  return HVACComponent_Impl::remove();
}

ModelObject SetpointManagerMixedAir_Impl::clone(Model model) const
{
  return HVACComponent_Impl::clone( model );
}

std::string SetpointManagerMixedAir_Impl::controlVariable()
{
  return getString(OS_SetpointManager_MixedAirFields::ControlVariable).get();
}

void SetpointManagerMixedAir_Impl::setControlVariable( std::string value )
{
  setString(OS_SetpointManager_MixedAirFields::ControlVariable,value);
}

boost::optional<Node> SetpointManagerMixedAir_Impl::referenceSetpointNode()
{
  SetpointManagerMixedAir thisModelObject = this->getObject<SetpointManagerMixedAir>();

  return thisModelObject.getModelObjectTarget<Node>(OS_SetpointManager_MixedAirFields::ReferenceSetpointNodeName);
}

void SetpointManagerMixedAir_Impl::setReferenceSetpointNode( Node & node )
{
  SetpointManagerMixedAir thisModelObject = this->getObject<SetpointManagerMixedAir>();

  thisModelObject.setPointer(OS_SetpointManager_MixedAirFields::ReferenceSetpointNodeName,node.handle());
}

boost::optional<Node> SetpointManagerMixedAir_Impl::fanInletNode()
{
  SetpointManagerMixedAir thisModelObject = this->getObject<SetpointManagerMixedAir>();

  return thisModelObject.getModelObjectTarget<Node>(OS_SetpointManager_MixedAirFields::FanInletNodeName);
}

void SetpointManagerMixedAir_Impl::setFanInletNode( Node & node )
{
  SetpointManagerMixedAir thisModelObject = this->getObject<SetpointManagerMixedAir>();

  thisModelObject.setPointer(OS_SetpointManager_MixedAirFields::FanInletNodeName,node.handle());
}

boost::optional<Node> SetpointManagerMixedAir_Impl::fanOutletNode()
{
  SetpointManagerMixedAir thisModelObject = this->getObject<SetpointManagerMixedAir>();

  return thisModelObject.getModelObjectTarget<Node>(OS_SetpointManager_MixedAirFields::FanOutletNodeName);
}

void SetpointManagerMixedAir_Impl::setFanOutletNode( Node & node )
{
  SetpointManagerMixedAir thisModelObject = this->getObject<SetpointManagerMixedAir>();

  thisModelObject.setPointer(OS_SetpointManager_MixedAirFields::FanOutletNodeName,node.handle());
}

boost::optional<Node> SetpointManagerMixedAir_Impl::setpointNode()
{
  SetpointManagerMixedAir thisModelObject = this->getObject<SetpointManagerMixedAir>();

  return thisModelObject.getModelObjectTarget<Node>(OS_SetpointManager_MixedAirFields::SetpointNodeorNodeListName);
}

void SetpointManagerMixedAir_Impl::setSetpointNode( Node & node )
{
  SetpointManagerMixedAir thisModelObject = this->getObject<SetpointManagerMixedAir>();

  thisModelObject.setPointer(OS_SetpointManager_MixedAirFields::SetpointNodeorNodeListName,node.handle());
}

} // detail

SetpointManagerMixedAir::SetpointManagerMixedAir(const Model& model)
  : HVACComponent(SetpointManagerMixedAir::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::SetpointManagerMixedAir_Impl>());
}

SetpointManagerMixedAir::SetpointManagerMixedAir(
    boost::shared_ptr<detail::SetpointManagerMixedAir_Impl> p)
  : HVACComponent(p)
{}

bool SetpointManagerMixedAir::addToNode(Node & node)
{
  return getImpl<detail::SetpointManagerMixedAir_Impl>()->addToNode( node );
}

std::vector<openstudio::IdfObject> SetpointManagerMixedAir::remove()
{
  return getImpl<detail::SetpointManagerMixedAir_Impl>()->remove();
}

ModelObject SetpointManagerMixedAir::clone(Model model) const
{
  return getImpl<detail::SetpointManagerMixedAir_Impl>()->clone( model );
}

IddObjectType SetpointManagerMixedAir::iddObjectType() {
  IddObjectType result(IddObjectType::OS_SetpointManager_MixedAir);
  return result;
}

std::string SetpointManagerMixedAir::controlVariable()
{
  return getImpl<detail::SetpointManagerMixedAir_Impl>()->controlVariable();
}

void SetpointManagerMixedAir::setControlVariable( std::string value )
{
  getImpl<detail::SetpointManagerMixedAir_Impl>()->setControlVariable(value);
}

boost::optional<Node> SetpointManagerMixedAir::referenceSetpointNode()
{
  return getImpl<detail::SetpointManagerMixedAir_Impl>()->referenceSetpointNode();
}

void SetpointManagerMixedAir::setReferenceSetpointNode( Node & node )
{
  getImpl<detail::SetpointManagerMixedAir_Impl>()->setReferenceSetpointNode(node);
}

boost::optional<Node> SetpointManagerMixedAir::fanInletNode()
{
  return getImpl<detail::SetpointManagerMixedAir_Impl>()->fanInletNode();
}

void SetpointManagerMixedAir::setFanInletNode( Node & node )
{
  getImpl<detail::SetpointManagerMixedAir_Impl>()->setFanInletNode(node);
}

boost::optional<Node> SetpointManagerMixedAir::fanOutletNode()
{
  return getImpl<detail::SetpointManagerMixedAir_Impl>()->fanOutletNode();
}

void SetpointManagerMixedAir::setFanOutletNode( Node & node )
{
  getImpl<detail::SetpointManagerMixedAir_Impl>()->setFanOutletNode(node);
}

boost::optional<Node> SetpointManagerMixedAir::setpointNode()
{
  return getImpl<detail::SetpointManagerMixedAir_Impl>()->setpointNode();
}

void SetpointManagerMixedAir::setSetpointNode( Node & node )
{
  return getImpl<detail::SetpointManagerMixedAir_Impl>()->setSetpointNode(node);
}

void SetpointManagerMixedAir::updateFanInletOutletNodes(AirLoopHVAC & airLoopHVAC)
{
  boost::optional<Node> fanInletNode;
  boost::optional<Node> fanOutletNode;

  std::vector<StraightComponent> fans;

  std::vector<ModelObject> supplyComponents = airLoopHVAC.supplyComponents();

  std::vector<FanConstantVolume> constantFans = subsetCastVector<FanConstantVolume>(supplyComponents);
  std::vector<FanVariableVolume> variableFans = subsetCastVector<FanVariableVolume>(supplyComponents);
  std::vector<FanOnOff> onoffFans = subsetCastVector<FanOnOff>(supplyComponents);

  fans.insert(fans.begin(),constantFans.begin(),constantFans.end());
  fans.insert(fans.begin(),variableFans.begin(),variableFans.end());
  fans.insert(fans.begin(),onoffFans.begin(),onoffFans.end());

  if( fans.size() > 0 )
  {
    boost::optional<ModelObject> mo;

    mo = fans.front().inletModelObject();
    if( mo )
    {
      fanInletNode = mo->optionalCast<Node>();
    }

    mo = fans.front().outletModelObject();
    if( mo )
    {
      fanOutletNode = mo->optionalCast<Node>();
    }
  }

  if( fanInletNode && fanOutletNode )
  {
    std::vector<model::Node> nodes = subsetCastVector<model::Node>(airLoopHVAC.supplyComponents());

    for( std::vector<model::Node>::iterator it = nodes.begin();
         it != nodes.end();
         it++ )
    {
      if( boost::optional<model::SetpointManagerMixedAir> spm = it->getSetpointManagerMixedAir() )
      {
        spm->setFanInletNode(fanInletNode.get());

        spm->setFanOutletNode(fanOutletNode.get());
      }
    }
  }
}

} // model

} // openstudio

