/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2020, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
*  following conditions are met:
*
*  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
*  disclaimer.
*
*  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
*  disclaimer in the documentation and/or other materials provided with the distribution.
*
*  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote products
*  derived from this software without specific prior written permission from the respective party.
*
*  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative works
*  may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without specific prior
*  written permission from Alliance for Sustainable Energy, LLC.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER(S) AND ANY CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
*  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
*  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER(S), ANY CONTRIBUTORS, THE UNITED STATES GOVERNMENT, OR THE UNITED
*  STATES DEPARTMENT OF ENERGY, NOR ANY OF THEIR EMPLOYEES, BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
*  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
*  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
*  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
*  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
***********************************************************************************************************************/

#include "AirflowNetworkComponent.hpp"
#include "AirflowNetworkComponent_Impl.hpp"
#include "AirLoopHVAC.hpp"
#include "AirLoopHVAC_Impl.hpp"
#include "PlantLoop.hpp"
#include "PlantLoop_Impl.hpp"
#include "Node.hpp"
#include "Node_Impl.hpp"
#include "AirLoopHVACOutdoorAirSystem.hpp"
#include "AirLoopHVACOutdoorAirSystem_Impl.hpp"
#include "Model.hpp"
#include "Model_Impl.hpp"
#include "Splitter.hpp"
#include "Splitter_Impl.hpp"
#include "Mixer.hpp"
#include "Mixer_Impl.hpp"
#include "../utilities/core/Assert.hpp"

namespace openstudio {

namespace model {

  namespace detail {

    AirflowNetworkComponent_Impl::AirflowNetworkComponent_Impl(IddObjectType type, Model_Impl* model) : ModelObject_Impl(type, model) {}

    AirflowNetworkComponent_Impl::AirflowNetworkComponent_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
      : ModelObject_Impl(idfObject, model, keepHandle) {}

    AirflowNetworkComponent_Impl::AirflowNetworkComponent_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model,
                                                               bool keepHandle)
      : ModelObject_Impl(other, model, keepHandle) {}

    AirflowNetworkComponent_Impl::AirflowNetworkComponent_Impl(const AirflowNetworkComponent_Impl& other, Model_Impl* model, bool keepHandles)
      : ModelObject_Impl(other, model, keepHandles) {}

    boost::optional<ModelObject> AirflowNetworkComponent_Impl::componentModelObject() const {
      // base class implementation
      return boost::none;
    }

    /*
OptionalModelObject AirflowNetworkComponent_Impl::inletModelObject() const
{
  return connectedObject(inletPort());
}

OptionalModelObject AirflowNetworkComponent_Impl::outletModelObject() const
{
  return connectedObject(outletPort());
}
bool AirflowNetworkComponent_Impl::removeFromLoop()
{
  if( auto t_oaSystem = airLoopHVACOutdoorAirSystem() ) {
    if( t_oaSystem->oaComponent(handle()) ) {
      return HVACComponent_Impl::removeFromLoop(t_oaSystem->outboardOANode().get(),
             t_oaSystem.get(),
             inletPort(),
             outletPort());
    } else if( t_oaSystem->reliefComponent(handle()) ) {
      return HVACComponent_Impl::removeFromLoop(t_oaSystem.get(),
             t_oaSystem->outboardReliefNode().get(),
             inletPort(),
             outletPort());
    }
  } else if( auto t_loop = loop() ) {
    if( t_loop->supplyComponent(handle()) ) {
      return HVACComponent_Impl::removeFromLoop(t_loop->supplyInletNode(),
             t_loop->supplyOutletNode(),
             inletPort(),
             outletPort());
    } else if( t_loop->demandComponent(handle()) ) {
      return HVACComponent_Impl::removeFromLoop(t_loop->demandInletNode(),
             t_loop->demandOutletNode(),
             inletPort(),
             outletPort());
    }
  }

  return false;
}

std::vector<openstudio::IdfObject> AirflowNetworkComponent_Impl::remove()
{
  removeFromLoop();

  return HVACComponent_Impl::remove();
}

boost::optional<ParentObject> AirflowNetworkComponent_Impl::parent() const
{
  return boost::optional<ParentObject>();
}

std::vector<ModelObject> AirflowNetworkComponent_Impl::children() const
{
  std::vector<ModelObject> result;
  return result;
}

OptionalAirLoopHVAC AirflowNetworkComponent_Impl::airLoopHVAC() const
{
  return HVACComponent_Impl::airLoopHVAC();
}

void AirflowNetworkComponent_Impl::disconnect()
{
  ModelObject mo = this->getObject<ModelObject>();
  this->model().disconnect(mo, this->inletPort());
  this->model().disconnect(mo, this->outletPort());
}

std::vector<HVACComponent> AirflowNetworkComponent_Impl::edges(const boost::optional<HVACComponent> & prev)
{
  std::vector<HVACComponent> edges;
  if( auto edgeModelObject = outletModelObject() ) {
    if( auto edgeObject = edgeModelObject->optionalCast<HVACComponent>() ) {
      edges.push_back(*edgeObject);
    }
  }
  return edges;
}

bool AirflowNetworkComponent_Impl::addToNode(Node & node)
{
  Model _model = node.model();
  ModelObject thisModelObject = getObject<ModelObject>();
  auto t_loop = node.loop();
  auto t_oaSystem = node.airLoopHVACOutdoorAirSystem();

  auto componentInletPort = inletPort();
  auto componentOutletPort = outletPort();

  boost::optional<HVACComponent> systemStartComponent;
  boost::optional<HVACComponent> systemEndComponent;

  if( node.getImpl<Node_Impl>()->isConnected(thisModelObject) ) return false;

  if( t_loop && ! t_oaSystem ) {
    if( t_loop->supplyComponent(node.handle()) ) {
      systemStartComponent = t_loop->supplyInletNode();
      auto nodes = t_loop->supplyOutletNodes();
      OS_ASSERT( ! nodes.empty() );
      if( (nodes.size() == 2u) && (! t_loop->supplyComponents(node,nodes[1]).empty()) ) {
        systemEndComponent = nodes[1];
      } else {
        systemEndComponent = nodes[0];
      }
      OS_ASSERT(systemEndComponent);
    } else if( t_loop->demandComponent(node.handle()) ) {
      systemStartComponent = t_loop->demandInletNode();
      systemEndComponent = t_loop->demandOutletNode();
    }
  } else if( t_oaSystem ) {
    if( t_oaSystem->oaComponent(node.handle()) ) {
      systemStartComponent = t_oaSystem->outboardOANode();
      systemEndComponent = t_oaSystem.get();
    } else if( t_oaSystem->reliefComponent(node.handle()) ) {
      systemStartComponent = t_oaSystem.get();
      systemEndComponent = t_oaSystem->outboardReliefNode();
    }
  }

  if( systemStartComponent && systemEndComponent ) {
    removeFromLoop();
    return HVACComponent_Impl::addToNode(node,systemStartComponent.get(),systemEndComponent.get(),componentInletPort,componentOutletPort);
  } else {
    return false;
  }
}

ModelObject AirflowNetworkComponent_Impl::clone(Model model) const
{
  AirflowNetworkComponent mo =  HVACComponent_Impl::clone( model ).cast<AirflowNetworkComponent>();

  mo.setString(mo.inletPort(),"");
  mo.setString(mo.outletPort(),"");

  return mo;
}
*/

  }  // namespace detail

  AirflowNetworkComponent::AirflowNetworkComponent(IddObjectType type, const Model& model) : ModelObject(type, model) {
    OS_ASSERT(getImpl<detail::AirflowNetworkComponent_Impl>());
  }

  AirflowNetworkComponent::AirflowNetworkComponent(std::shared_ptr<detail::AirflowNetworkComponent_Impl> p) : ModelObject(p) {}

  OptionalModelObject AirflowNetworkComponent::componentModelObject() const {
    return getImpl<detail::AirflowNetworkComponent_Impl>()->componentModelObject();
  }

  /*
OptionalModelObject AirflowNetworkComponent::outletModelObject()
{
  return getImpl<detail::AirflowNetworkComponent_Impl>()->outletModelObject();
}

std::vector<IdfObject> AirflowNetworkComponent::remove()
{
  return getImpl<detail::AirflowNetworkComponent_Impl>()->remove();
}

bool AirflowNetworkComponent::removeFromLoop()
{
  return getImpl<detail::AirflowNetworkComponent_Impl>()->removeFromLoop();
}

OptionalAirLoopHVAC AirflowNetworkComponent::airLoopHVAC() const
{
  return getImpl<detail::AirflowNetworkComponent_Impl>()->airLoopHVAC();
}

bool AirflowNetworkComponent::addToNode(Node & node)
{
  return getImpl<detail::AirflowNetworkComponent_Impl>()->addToNode( node );
}

ModelObject AirflowNetworkComponent::clone(Model model) const
{
  return getImpl<detail::AirflowNetworkComponent_Impl>()->clone( model );
}

void AirflowNetworkComponent::disconnect()
{
  getImpl<detail::AirflowNetworkComponent_Impl>()->disconnect();
}
*/

}  // namespace model

}  // namespace openstudio
