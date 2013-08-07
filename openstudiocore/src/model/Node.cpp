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

#include <model/Node.hpp>
#include <model/Node_Impl.hpp>
#include <model/SetpointManagerMixedAir.hpp>
#include <model/SetpointManagerMixedAir_Impl.hpp>
#include <model/SetpointManagerOutdoorAirReset.hpp>
#include <model/SetpointManagerOutdoorAirReset_Impl.hpp>
#include <model/SetpointManagerSingleZoneReheat.hpp>
#include <model/SetpointManagerSingleZoneReheat_Impl.hpp>
#include <model/SetpointManagerScheduled.hpp>
#include <model/SetpointManagerScheduled_Impl.hpp>
#include <model/SetpointManagerFollowOutdoorAirTemperature.hpp>
#include <model/SetpointManagerFollowOutdoorAirTemperature_Impl.hpp>
#include <model/SetpointManagerWarmest.hpp>
#include <model/SetpointManagerWarmest_Impl.hpp>
#include <model/AirLoopHVAC.hpp>
#include <model/AirLoopHVAC_Impl.hpp>
#include <model/FanConstantVolume.hpp>
#include <model/FanConstantVolume_Impl.hpp>
#include <model/FanVariableVolume.hpp>
#include <model/FanVariableVolume_Impl.hpp>
#include <model/ThermalZone.hpp>
#include <model/ThermalZone_Impl.hpp>
#include <model/Model.hpp>
#include <model/Model_Impl.hpp>
#include <utilities/idd/OS_Node_FieldEnums.hxx>
#include <utilities/idd/OS_SetpointManager_SingleZone_Reheat_FieldEnums.hxx>
#include <utilities/idd/OS_SetpointManager_MixedAir_FieldEnums.hxx>
#include <utilities/idd/OS_Fan_ConstantVolume_FieldEnums.hxx>
#include <utilities/idd/OS_ThermalZone_FieldEnums.hxx>
#include <utilities/core/Compare.hpp>
#include <utilities/core/Assert.hpp>
#include <boost/foreach.hpp>

using openstudio::detail::WorkspaceObject_Impl;

namespace openstudio {

namespace model {

namespace detail{

  Node_Impl::Node_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
    : StraightComponent_Impl(idfObject, model, keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == Node::iddObjectType());
  }

  Node_Impl::Node_Impl(const openstudio::detail::WorkspaceObject_Impl& other, 
                       Model_Impl* model, 
                       bool keepHandle)
    : StraightComponent_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == Node::iddObjectType());
  }

  Node_Impl::Node_Impl(const Node_Impl& other, Model_Impl* model, bool keepHandle)
    : StraightComponent_Impl(other,model,keepHandle)
  {}

  // virtual destructor
  Node_Impl::~Node_Impl(){}

  // Get all output variable names that could be associated with this object.
  const std::vector<std::string>& Node_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType Node_Impl::iddObjectType() const {
    return Node::iddObjectType();
  }

  unsigned Node_Impl::inletPort()
  {
    return OS_NodeFields::InletPort;
  }

  unsigned Node_Impl::outletPort()
  {
    return OS_NodeFields::OutletPort;
  }

  std::vector<IdfObject> Node_Impl::remove()
  {
    if( isRemovable() )
    {
      removeSetpointManagerSingleZoneReheat();

      removeSetpointManagerMixedAir();

      removeSetpointManagerScheduled();

      removeSetpointManagerFollowOutdoorAirTemperature();

      removeSetpointManagerOutdoorAirReset();

      removeSetpointManagerWarmest();

      return ModelObject_Impl::remove();
    }
    else
    {
      return std::vector<IdfObject>();
    }
  }

  void Node_Impl::addSetpointManager(SetpointManagerSingleZoneReheat singleZoneReheat)
  {
    removeSetpointManagerSingleZoneReheat();

    removeSetpointManagerMixedAir();

    removeSetpointManagerScheduled();

    removeSetpointManagerFollowOutdoorAirTemperature();

    removeSetpointManagerOutdoorAirReset();

    removeSetpointManagerWarmest();

    std::string s;

    singleZoneReheat.setString(OS_SetpointManager_SingleZone_ReheatFields::SetpointNodeorNodeListName,
                               this->name().get());
    // Get the AirLoop
    boost::optional<AirLoopHVAC> _airLoop;
    _airLoop = airLoopHVAC();

    if( _airLoop )
    {
      ModelObjectVector modelObjectVector;

      modelObjectVector = _airLoop->demandComponents(openstudio::IddObjectType::OS_ThermalZone);
      if(modelObjectVector.size() > 0)
      {
        ModelObject mo = modelObjectVector.front();
        OptionalThermalZone thermalZone = mo.optionalCast<ThermalZone>();

        s = thermalZone->name().get();
        singleZoneReheat.setString( OS_SetpointManager_SingleZone_ReheatFields::ControlZoneName,s );
      }
    }
  }

  void Node_Impl::removeSetpointManagerSingleZoneReheat()
  {
    boost::optional<SetpointManagerSingleZoneReheat> opt(this->getSetpointManagerSingleZoneReheat());
    if( opt )
    {
      openstudio::Handle h = opt->handle();
      this->model().removeObject(h);
    }
  }

  boost::optional<SetpointManagerSingleZoneReheat> Node_Impl::getSetpointManagerSingleZoneReheat() const
  {
    /*
    SetpointManagerSingleZoneReheatVector modelObjects;
    modelObjects = this->model().getModelObjects<SetpointManagerSingleZoneReheat>();
    for( SetpointManagerSingleZoneReheatVector::iterator it = modelObjects.begin(),itEnd= modelObjects.end();
         it!=itEnd;
         ++it )
    {
      if( it->getString(OS_SetpointManager_SingleZone_ReheatFields::SetpointNodeorNodeListName).get() ==
          this->name().get() )
      {
        return *it;
      }

    }
    return OptionalSetpointManagerSingleZoneReheat();
    */

    SetpointManagerSingleZoneReheatVector modelObjects = this->getObject<Node>().getModelObjectSources<SetpointManagerSingleZoneReheat>();
    for( SetpointManagerSingleZoneReheatVector::iterator it = modelObjects.begin(),itEnd= modelObjects.end();
         it!=itEnd;
         ++it )
    {
      if( it->getString(OS_SetpointManager_SingleZone_ReheatFields::SetpointNodeorNodeListName).get() ==
          this->name().get() )
      {
        return *it;
      }

    }
    return boost::none;
  }

  void  Node_Impl::addSetpointManager(SetpointManagerMixedAir mixedAir)
  {
    removeSetpointManagerMixedAir();

    removeSetpointManagerSingleZoneReheat();

    removeSetpointManagerScheduled();

    removeSetpointManagerFollowOutdoorAirTemperature();

    removeSetpointManagerOutdoorAirReset();

    removeSetpointManagerWarmest();

    std::string s;

    mixedAir.setString(OS_SetpointManager_MixedAirFields::SetpointNodeorNodeListName,
                               this->name().get());

    // Get the AirLoop
    boost::optional<AirLoopHVAC> _airLoop;
    _airLoop = airLoopHVAC();

    if( _airLoop )
    {
      ModelObjectVector modelObjectVector;

      // Get any OS_Fan_ConstantVolume fans
      modelObjectVector = _airLoop->supplyComponents(openstudio::IddObjectType::OS_Fan_ConstantVolume);

      if(modelObjectVector.size() > 0)
      {
        ModelObject mo = modelObjectVector.front();
        OptionalFanConstantVolume fanConstantVolume = mo.optionalCast<FanConstantVolume>();

        // Get the fan inlet node
        s = fanConstantVolume->inletModelObject()->name().get();
        mixedAir.setString( OS_SetpointManager_MixedAirFields::FanInletNodeName,s );

        // Get the fan outlet node
        s = fanConstantVolume->outletModelObject()->name().get();
        mixedAir.setString( OS_SetpointManager_MixedAirFields::FanOutletNodeName,s );
      }

      // Get any OS_Fan_VairableVolume fans
      modelObjectVector = _airLoop->supplyComponents(openstudio::IddObjectType::OS_Fan_VariableVolume);

      if(modelObjectVector.size() > 0)
      {
        ModelObject mo = modelObjectVector.front();
        OptionalFanVariableVolume fanVariableVolume = mo.optionalCast<FanVariableVolume>();

        // Get the fan inlet node
        s = fanVariableVolume->inletModelObject()->name().get();
        mixedAir.setString( OS_SetpointManager_MixedAirFields::FanInletNodeName,s );

        // Get the fan outlet node
        s = fanVariableVolume->outletModelObject()->name().get();
        mixedAir.setString( OS_SetpointManager_MixedAirFields::FanOutletNodeName,s );
      }

      // Get the supply outlet node
      s = _airLoop->supplyOutletNodes().front().name().get();
      mixedAir.setString( OS_SetpointManager_MixedAirFields::ReferenceSetpointNodeName,s );
    }
  }

  void Node_Impl::removeSetpointManagerMixedAir()
  {
    boost::optional<SetpointManagerMixedAir> opt(this->getSetpointManagerMixedAir());
    if( opt )
    {
      openstudio::Handle h = opt->handle();
      this->model().removeObject(h);
    }
  }

  boost::optional<SetpointManagerMixedAir> Node_Impl::getSetpointManagerMixedAir() const
  {
    SetpointManagerMixedAirVector modelObjects = this->getObject<Node>().getModelObjectSources<SetpointManagerMixedAir>();
    for( SetpointManagerMixedAirVector::iterator it = modelObjects.begin(),itEnd= modelObjects.end();
         it !=itEnd;
         ++it )
    {

      if( it->getString(OS_SetpointManager_MixedAirFields::SetpointNodeorNodeListName).get() ==
          this->name().get() )
      {
        return *it;
      }

    }
    return boost::none;
  }

  void Node_Impl::addSetpointManager( SetpointManagerScheduled & setPointManager )
  {
    Node node = this->getObject<Node>();

    setPointManager.addToNode(node);
  }

  boost::optional<SetpointManagerScheduled> Node_Impl::setpointManagerScheduled() const
  {
    std::vector<SetpointManagerScheduled> modelObjects = this->getObject<Node>().getModelObjectSources<SetpointManagerScheduled>();
    for( std::vector<SetpointManagerScheduled>::iterator it = modelObjects.begin(),itEnd= modelObjects.end();
         it !=itEnd;
         ++it )
    {

      if( boost::optional<Node> setpointNode = it->setpointNode() )
      {
        if( setpointNode->handle() == this->handle() )
        {
          return *it;
        }
      }
    }
    return boost::none;
  }

  void Node_Impl::removeSetpointManagerScheduled()
  {
    boost::optional<SetpointManagerScheduled> opt(this->setpointManagerScheduled());
    if( opt )
    {
      openstudio::Handle h = opt->handle();
      this->model().removeObject(h);
    }
  }

  /////////////

  void Node_Impl::addSetpointManager( SetpointManagerFollowOutdoorAirTemperature & setPointManager )
  {
    Node node = this->getObject<Node>();

    setPointManager.addToNode(node);
  }

  boost::optional<SetpointManagerFollowOutdoorAirTemperature> Node_Impl::setpointManagerFollowOutdoorAirTemperature() const
  {
    std::vector<SetpointManagerFollowOutdoorAirTemperature> modelObjects = 
      getObject<Node>().getModelObjectSources<SetpointManagerFollowOutdoorAirTemperature>();
    for( std::vector<SetpointManagerFollowOutdoorAirTemperature>::iterator it = modelObjects.begin(),itEnd= modelObjects.end();
         it !=itEnd;
         ++it )
    {

      if( boost::optional<Node> setpointNode = it->setpointNode() )
      {
        if( setpointNode->handle() == this->handle() )
        {
          return *it;
        }
      }
    }
    return boost::none;
  }

  void Node_Impl::removeSetpointManagerFollowOutdoorAirTemperature()
  {
    boost::optional<SetpointManagerFollowOutdoorAirTemperature> opt(setpointManagerFollowOutdoorAirTemperature());
    if( opt )
    {
      openstudio::Handle h = opt->handle();
      this->model().removeObject(h);
    }
  }

  /////////////

  void Node_Impl::addSetpointManager( SetpointManagerOutdoorAirReset & setPointManager )
  {
    Node node = this->getObject<Node>();

    setPointManager.addToNode(node);
  }

  boost::optional<SetpointManagerOutdoorAirReset> Node_Impl::setpointManagerOutdoorAirReset() const
  {
    std::vector<SetpointManagerOutdoorAirReset> modelObjects = 
      getObject<Node>().getModelObjectSources<SetpointManagerOutdoorAirReset>();
    for( std::vector<SetpointManagerOutdoorAirReset>::iterator it = modelObjects.begin(),itEnd= modelObjects.end();
         it !=itEnd;
         ++it )
    {

      if( boost::optional<Node> setpointNode = it->setpointNode() )
      {
        if( setpointNode->handle() == this->handle() )
        {
          return *it;
        }
      }
    }
    return boost::none;
  }

  void Node_Impl::removeSetpointManagerOutdoorAirReset()
  {
    boost::optional<SetpointManagerOutdoorAirReset> opt(setpointManagerOutdoorAirReset());
    if( opt )
    {
      openstudio::Handle h = opt->handle();
      this->model().removeObject(h);
    }
  }

  void Node_Impl::addSetpointManagerWarmest( SetpointManagerWarmest & setPointManager )
  {
    Node node = this->getObject<Node>();

    setPointManager.addToNode(node);
  }

  boost::optional<SetpointManagerWarmest> Node_Impl::setpointManagerWarmest() const
  {
    std::vector<SetpointManagerWarmest> modelObjects = 
      getObject<Node>().getModelObjectSources<SetpointManagerWarmest>();
    for( std::vector<SetpointManagerWarmest>::iterator it = modelObjects.begin(),itEnd= modelObjects.end();
         it !=itEnd;
         ++it )
    {

      if( boost::optional<Node> setpointNode = it->setpointNode() )
      {
        if( setpointNode->handle() == this->handle() )
        {
          return *it;
        }
      }
    }
    return boost::none;
  }

  void Node_Impl::removeSetpointManagerWarmest()
  {
    boost::optional<SetpointManagerWarmest> opt(setpointManagerWarmest());
    if( opt )
    {
      openstudio::Handle h = opt->handle();
      this->model().removeObject(h);
    }
  }

  std::vector<ModelObject> Node_Impl::children() const
  {
    std::vector<ModelObject> result;
    if(getSetpointManagerSingleZoneReheat())
    {
      result.push_back(getSetpointManagerSingleZoneReheat().get());
    }
    if(this->getSetpointManagerMixedAir())
    {
      result.push_back(getSetpointManagerMixedAir().get());
    }
    if(boost::optional<SetpointManagerScheduled> setpointManagerScheduled = this->setpointManagerScheduled())
    {
      result.push_back(setpointManagerScheduled.get());
    }
    if( boost::optional<SetpointManagerFollowOutdoorAirTemperature> setpointManagerFollowOutdoorAirTemperature = 
        this->setpointManagerFollowOutdoorAirTemperature() )
    {
      result.push_back(setpointManagerFollowOutdoorAirTemperature.get());
    }
    if( boost::optional<SetpointManagerOutdoorAirReset> setpointManagerOutdoorAirReset = 
        this->setpointManagerOutdoorAirReset() )
    {
      result.push_back(setpointManagerOutdoorAirReset.get());
    }
    if( boost::optional<SetpointManagerWarmest> setpointManagerWarmest = 
        this->setpointManagerWarmest() )
    {
      result.push_back(setpointManagerWarmest.get());
    }
    return result;
  }

  bool Node_Impl::addToNode(Node & node)
  {
    return false;
  }
  
  ModelObject Node_Impl::clone(Model model) const
  {
    return StraightComponent_Impl::clone( model );
  }
  
  bool Node_Impl::isRemovable() const
  {
    if( airLoopHVAC() || plantLoop() )
    {
      return false;
    }
    else
    {
      return true;
    }
  }

} // detail

// create a new Node object in the model's workspace
Node::Node(const Model& model)
  : StraightComponent(Node::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::Node_Impl>());
}

Node::Node(boost::shared_ptr<detail::Node_Impl> p)
  : StraightComponent(p)
{}

void Node::addSetpointManager(SetpointManagerSingleZoneReheat spm)
{
  getImpl<detail::Node_Impl>()->addSetpointManager(spm);
}

void Node::removeSetpointManagerSingleZoneReheat()
{
  getImpl<detail::Node_Impl>()->removeSetpointManagerSingleZoneReheat();
}

boost::optional<SetpointManagerSingleZoneReheat> Node::getSetpointManagerSingleZoneReheat()
{
  return getImpl<detail::Node_Impl>()->getSetpointManagerSingleZoneReheat();
}

void Node::addSetpointManager(SetpointManagerMixedAir spm)
{
  return getImpl<detail::Node_Impl>()->addSetpointManager(spm);
}

void Node::removeSetpointManagerMixedAir()
{
  getImpl<detail::Node_Impl>()->removeSetpointManagerMixedAir();
}

boost::optional<SetpointManagerMixedAir> Node::getSetpointManagerMixedAir()
{
  return getImpl<detail::Node_Impl>()->getSetpointManagerMixedAir();
}

boost::optional<SetpointManagerScheduled> Node::setpointManagerScheduled() const
{
  return getImpl<detail::Node_Impl>()->setpointManagerScheduled();
}

void Node::removeSetpointManagerScheduled()
{
  getImpl<detail::Node_Impl>()->removeSetpointManagerScheduled();
}

void Node::addSetpointManager(SetpointManagerScheduled & setpointManager)
{
  getImpl<detail::Node_Impl>()->addSetpointManager(setpointManager);
}

boost::optional<SetpointManagerFollowOutdoorAirTemperature> Node::setpointManagerFollowOutdoorAirTemperature() const
{
  return getImpl<detail::Node_Impl>()->setpointManagerFollowOutdoorAirTemperature();
}

void Node::removeSetpointManagerFollowOutdoorAirTemperature()
{
  getImpl<detail::Node_Impl>()->removeSetpointManagerFollowOutdoorAirTemperature();
}

void Node::addSetpointManager(SetpointManagerFollowOutdoorAirTemperature & setpointManager)
{
  getImpl<detail::Node_Impl>()->addSetpointManager(setpointManager);
}

boost::optional<SetpointManagerOutdoorAirReset> Node::setpointManagerOutdoorAirReset() const
{
  return getImpl<detail::Node_Impl>()->setpointManagerOutdoorAirReset();
}

void Node::removeSetpointManagerOutdoorAirReset()
{
  getImpl<detail::Node_Impl>()->removeSetpointManagerOutdoorAirReset();
}

void Node::addSetpointManager(SetpointManagerOutdoorAirReset & setpointManager)
{
  getImpl<detail::Node_Impl>()->addSetpointManager(setpointManager);
}

boost::optional<SetpointManagerWarmest> Node::setpointManagerWarmest() const
{
  return getImpl<detail::Node_Impl>()->setpointManagerWarmest();
}

void Node::removeSetpointManagerWarmest()
{
  getImpl<detail::Node_Impl>()->removeSetpointManagerWarmest();
}

void Node::addSetpointManagerWarmest(SetpointManagerWarmest & setpointManager)
{
  getImpl<detail::Node_Impl>()->addSetpointManagerWarmest(setpointManager);
}

bool Node::addToNode(Node & node)
{
  return getImpl<detail::Node_Impl>()->addToNode( node );
}

bool Node::isRemovable() const
{
  return getImpl<detail::Node_Impl>()->isRemovable();
}

std::vector<IdfObject> Node::remove()
{
  return getImpl<detail::Node_Impl>()->remove();
}

ModelObject Node::clone(Model model) const
{
  return getImpl<detail::Node_Impl>()->clone( model );
}

IddObjectType Node::iddObjectType() {
  IddObjectType result(IddObjectType::OS_Node);
  return result;
}

} // model
} // openstudio

