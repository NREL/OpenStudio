/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2017, Alliance for Sustainable Energy, LLC. All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 *  following conditions are met:
 *
 *  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
 *  disclaimer.
 *
 *  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *  following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 *  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote
 *  products derived from this software without specific prior written permission from the respective party.
 *
 *  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative
 *  works may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without
 *  specific prior written permission from Alliance for Sustainable Energy, LLC.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER, THE UNITED STATES GOVERNMENT, OR ANY CONTRIBUTORS BE LIABLE FOR
 *  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **********************************************************************************************************************/

#include "Node.hpp"
#include "Node_Impl.hpp"
#include "SetpointManager.hpp"
#include "SetpointManager_Impl.hpp"
#include "SetpointManagerMixedAir.hpp"
#include "SetpointManagerMixedAir_Impl.hpp"
#include "SetpointManagerOutdoorAirReset.hpp"
#include "SetpointManagerOutdoorAirReset_Impl.hpp"
#include "SetpointManagerSingleZoneReheat.hpp"
#include "SetpointManagerSingleZoneReheat_Impl.hpp"
#include "SetpointManagerScheduled.hpp"
#include "SetpointManagerScheduled_Impl.hpp"
#include "SetpointManagerFollowOutdoorAirTemperature.hpp"
#include "SetpointManagerFollowOutdoorAirTemperature_Impl.hpp"
#include "SetpointManagerWarmest.hpp"
#include "SetpointManagerWarmest_Impl.hpp"
#include "ThermalZone.hpp"
#include "PortList.hpp"
#include "PortList_Impl.hpp"
#include "Model.hpp"
#include <utilities/idd/OS_Node_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>
#include "../utilities/core/Assert.hpp"

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

  std::vector<HVACComponent> Node_Impl::edges(const boost::optional<HVACComponent> & prev)
  {
    std::vector<HVACComponent> edges;
    if( boost::optional<ModelObject> edgeModelObject = this->outletModelObject() ) {
      if( boost::optional<PortList> portList = edgeModelObject->optionalCast<PortList>() ) {
        if( boost::optional<ThermalZone> thermalZone = portList->thermalZone() ) {
          edges.push_back(*thermalZone);
        }
      }
      else if( boost::optional<HVACComponent> edgeObject = edgeModelObject->optionalCast<HVACComponent>() ) {
        edges.push_back(*edgeObject);
      }
    }
    return edges;
  }

  std::vector<IdfObject> Node_Impl::remove()
  {
    if( isRemovable() )
    {
      this->removeSetpointManagers();
      return ModelObject_Impl::remove();
    }
    else
    {
      return std::vector<IdfObject>();
    }
  }

  void Node_Impl::addSetpointManager(SetpointManagerSingleZoneReheat & singleZoneReheat)
  {
    LOG(Warn, "Node::addSetpointManager has been deprecated and will be removed in a future release, please use SetpointManagerSingleZoneReheat::addToNode");
    Node node = this->getObject<Node>();
    singleZoneReheat.addToNode(node);
  }

  void Node_Impl::removeSetpointManagerSingleZoneReheat()
  {
    LOG(Warn, "Node::removeSetpointManagerSingleZoneReheat has been deprecated and will be removed in a future release, please use SetpointManagerSingleZoneReheat::remove");
    if( boost::optional<SetpointManagerSingleZoneReheat> spm = this->getSetpointManagerSingleZoneReheat() )
    {
      spm->remove();
    }
  }

  boost::optional<SetpointManagerSingleZoneReheat> Node_Impl::getSetpointManagerSingleZoneReheat() const
  {
    LOG(Warn, "Node::getSetpointManagerSingleZoneReheat has been deprecated and will be removed in a future release, please use Node::setpointManagers");
    std::vector<SetpointManagerSingleZoneReheat> _setpointManagers = subsetCastVector<SetpointManagerSingleZoneReheat>(this->setpointManagers());
    if( !_setpointManagers.empty() ) {
      return _setpointManagers.front();
    } else {
      return boost::none;
    }
  }

  void  Node_Impl::addSetpointManager(SetpointManagerMixedAir & mixedAir)
  {
    LOG(Warn, "Node::addSetpointManager has been deprecated and will be removed in a future release, please use SetpointManagerMixedAir::addToNode");
    Node node = this->getObject<Node>();
    mixedAir.addToNode(node);
  }

  void Node_Impl::removeSetpointManagerMixedAir()
  {
    LOG(Warn, "Node::removeSetpointManagerMixedAir has been deprecated and will be removed in a future release, please use SetpointManagerMixedAir::remove");
    if( boost::optional<SetpointManagerMixedAir> spm = this->getSetpointManagerMixedAir() )
    {
      spm->remove();
    }
  }

  boost::optional<SetpointManagerMixedAir> Node_Impl::getSetpointManagerMixedAir() const
  {
    LOG(Warn, "Node::getSetpointManagerMixedAir has been deprecated and will be removed in a future release, please use Node::setpointManagers");
    std::vector<SetpointManagerMixedAir> _setpointManagers = subsetCastVector<SetpointManagerMixedAir>(this->setpointManagers());
    if( !_setpointManagers.empty() ) {
      return _setpointManagers.front();
    } else {
      return boost::none;
    }
  }

  void Node_Impl::addSetpointManager( SetpointManagerScheduled & setPointManager )
  {
    LOG(Warn, "Node::addSetpointManager has been deprecated and will be removed in a future release, please use SetpointManagerScheduled::addToNode");
    Node node = this->getObject<Node>();
    setPointManager.addToNode(node);
  }

  boost::optional<SetpointManagerScheduled> Node_Impl::setpointManagerScheduled() const
  {
    LOG(Warn, "Node::setpointManagerScheduled has been deprecated and will be removed in a future release, please use Node::setpointManagers");
    std::vector<SetpointManagerScheduled> _setpointManagers = subsetCastVector<SetpointManagerScheduled>(this->setpointManagers());
    if( !_setpointManagers.empty() ) {
      return _setpointManagers.front();
    } else {
      return boost::none;
    }
  }

  void Node_Impl::removeSetpointManagerScheduled()
  {
    LOG(Warn, "Node::removeSetpointManagerScheduled has been deprecated and will be removed in a future release, please use SetpointManagerScheduled::remove");
    if( boost::optional<SetpointManagerScheduled> spm = this->setpointManagerScheduled() )
    {
      spm->remove();
    }
  }

  /////////////

  void Node_Impl::addSetpointManager( SetpointManagerFollowOutdoorAirTemperature & setPointManager )
  {
    LOG(Warn, "Node::addSetpointManager has been deprecated and will be removed in a future release, please use SetpointManagerFollowOutdoorAirTemperature::addToNode");
    Node node = this->getObject<Node>();
    setPointManager.addToNode(node);
  }

  boost::optional<SetpointManagerFollowOutdoorAirTemperature> Node_Impl::setpointManagerFollowOutdoorAirTemperature() const
  {
    LOG(Warn, "Node::setpointManagerFollowOutdoorAirTemperature has been deprecated and will be removed in a future release, please use Node::setpointManagers");
    std::vector<SetpointManagerFollowOutdoorAirTemperature> _setpointManagers = subsetCastVector<SetpointManagerFollowOutdoorAirTemperature>(this->setpointManagers());
    if( !_setpointManagers.empty() ) {
      return _setpointManagers.front();
    } else {
      return boost::none;
    }
  }

  void Node_Impl::removeSetpointManagerFollowOutdoorAirTemperature()
  {
    LOG(Warn, "Node::removeSetpointManagerFollowOutdoorAirTemperature has been deprecated and will be removed in a future release, please use SetpointManagerFollowOutdoorAirTemperature::remove");
    if( boost::optional<SetpointManagerFollowOutdoorAirTemperature> spm = this->setpointManagerFollowOutdoorAirTemperature() )
    {
      spm->remove();
    }
  }

  /////////////

  void Node_Impl::addSetpointManager( SetpointManagerOutdoorAirReset & setPointManager )
  {
    LOG(Warn, "Node::addSetpointManager has been deprecated and will be removed in a future release, please use SetpointManagerOutdoorAirReset::addToNode");
    Node node = this->getObject<Node>();
    setPointManager.addToNode(node);
  }

  boost::optional<SetpointManagerOutdoorAirReset> Node_Impl::setpointManagerOutdoorAirReset() const
  {
    LOG(Warn, "Node::setpointManagerOutdoorAirReset has been deprecated and will be removed in a future release, please use Node::setpointManagers");
    std::vector<SetpointManagerOutdoorAirReset> _setpointManagers = subsetCastVector<SetpointManagerOutdoorAirReset>(this->setpointManagers());
    if( !_setpointManagers.empty() ) {
      return _setpointManagers.front();
    } else {
      return boost::none;
    }
  }

  void Node_Impl::removeSetpointManagerOutdoorAirReset()
  {
    LOG(Warn, "Node::removeSetpointManagerOutdoorAirReset has been deprecated and will be removed in a future release, please use SetpointManagerOutdoorAirReset::remove");
    if( boost::optional<SetpointManagerOutdoorAirReset> spm = this->setpointManagerOutdoorAirReset() )
    {
      spm->remove();
    }
  }

  void Node_Impl::addSetpointManagerWarmest( SetpointManagerWarmest & setPointManager )
  {
    LOG(Warn, "Node::addSetpointManagerWarmest has been deprecated and will be removed in a future release, please use SetpointManagerWarmest::addToNode");
    Node node = this->getObject<Node>();
    setPointManager.addToNode(node);
  }

  boost::optional<SetpointManagerWarmest> Node_Impl::setpointManagerWarmest() const
  {
    LOG(Warn, "Node::setpointManagerWarmest has been deprecated and will be removed in a future release, please use Node::setpointManagers");
    std::vector<SetpointManagerWarmest> _setpointManagers = subsetCastVector<SetpointManagerWarmest>(this->setpointManagers());
    if( !_setpointManagers.empty() ) {
      return _setpointManagers.front();
    } else {
      return boost::none;
    }
  }

  void Node_Impl::removeSetpointManagerWarmest()
  {
    LOG(Warn, "Node::removeSetpointManagerWarmest has been deprecated and will be removed in a future release, please use SetpointManagerWarmest::remove");
    if( boost::optional<SetpointManagerWarmest> spm = this->setpointManagerWarmest() )
    {
      spm->remove();
    }
  }

  std::vector<ModelObject> Node_Impl::children() const
  {
    std::vector<ModelObject> result = castVector<ModelObject>(this->setpointManagers());
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
    if( this->loop() )
    {
      return false;
    }
    else
    {
      return true;
    }
  }

  std::vector<SetpointManager> Node_Impl::setpointManagers() const
  {
    std::vector<SetpointManager> _setpointManagers;
    std::vector<SetpointManager> _modelObjects = getObject<Node>().getModelObjectSources<SetpointManager>();
    for(auto it = _modelObjects.begin();
        it != _modelObjects.end();
        ++it)
    {
      if( boost::optional<Node> setpointNode = it->setpointNode() )
      {
        if( setpointNode->handle() == this->handle() )
        {
          _setpointManagers.push_back(*it);
        }
      }
    }
    return _setpointManagers;
  }

  void Node_Impl::removeSetpointManagers()
  {
    std::vector<SetpointManager> _setpointManagers = this->setpointManagers();
    for(auto it = _setpointManagers.begin();
        it != _setpointManagers.end();
        ++it)
    {
      it->remove();
    }
  }

  bool Node_Impl::isConnected(const ModelObject & modelObject)
  {
    if( auto mo = outletModelObject() ) {
      if( modelObject.handle() == mo->handle() ) {
        return true;
      }
    }
    if( auto mo = inletModelObject() ) {
      if( modelObject.handle() == mo->handle() ) {
        return true;
      }
    }

    return false;
  }

} // detail

// create a new Node object in the model's workspace
Node::Node(const Model& model)
  : StraightComponent(Node::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::Node_Impl>());
}

Node::Node(std::shared_ptr<detail::Node_Impl> p)
  : StraightComponent(std::move(p))
{}

std::vector<SetpointManager> Node::setpointManagers() const
{
  return getImpl<detail::Node_Impl>()->setpointManagers();
}

void Node::addSetpointManager(SetpointManagerSingleZoneReheat & spm)
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

void Node::addSetpointManager(SetpointManagerMixedAir & spm)
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

