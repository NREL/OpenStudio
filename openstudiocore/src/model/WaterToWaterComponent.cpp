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

#include "WaterToWaterComponent.hpp"
#include "WaterToWaterComponent_Impl.hpp"
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

WaterToWaterComponent_Impl::WaterToWaterComponent_Impl(IddObjectType type, Model_Impl* model)
  : HVACComponent_Impl(type,model)
{
}

WaterToWaterComponent_Impl::WaterToWaterComponent_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
  : HVACComponent_Impl(idfObject, model, keepHandle)
{
}

WaterToWaterComponent_Impl::WaterToWaterComponent_Impl(
    const openstudio::detail::WorkspaceObject_Impl& other,
    Model_Impl* model,
    bool keepHandle)
  : HVACComponent_Impl(other,model,keepHandle)
{
}

WaterToWaterComponent_Impl::WaterToWaterComponent_Impl(const WaterToWaterComponent_Impl& other,
                                               Model_Impl* model,
                                               bool keepHandles)
  : HVACComponent_Impl(other,model,keepHandles)
{
}

OptionalModelObject WaterToWaterComponent_Impl::supplyInletModelObject()
{
  return connectedObject(supplyInletPort());
}

OptionalModelObject WaterToWaterComponent_Impl::supplyOutletModelObject()
{
  return connectedObject(supplyOutletPort());
}

OptionalModelObject WaterToWaterComponent_Impl::demandInletModelObject()
{
  return connectedObject(demandInletPort());
}

OptionalModelObject WaterToWaterComponent_Impl::demandOutletModelObject()
{
  return connectedObject(demandOutletPort());
}

std::vector<HVACComponent> WaterToWaterComponent_Impl::edges(const boost::optional<HVACComponent> & prev)
{
  std::vector<HVACComponent> edges;

  auto pushSupplyOutletModelObject = [&]() {
    if( auto edgeModelObject = supplyOutletModelObject() ) {
      auto edgeHVACComponent = edgeModelObject->optionalCast<HVACComponent>();
      OS_ASSERT(edgeHVACComponent);
      edges.push_back(edgeHVACComponent.get());
    }
  };

  auto pushDemandOutletModelObject = [&]() {
    if( auto edgeModelObject = demandOutletModelObject() ) {
      auto edgeHVACComponent = edgeModelObject->optionalCast<HVACComponent>();
      OS_ASSERT(edgeHVACComponent);
      edges.push_back(edgeHVACComponent.get());
    }
  };

  auto pushTertiaryOutletModelObject = [&]() {
    if( auto edgeModelObject = tertiaryOutletModelObject() ) {
      auto edgeHVACComponent = edgeModelObject->optionalCast<HVACComponent>();
      OS_ASSERT(edgeHVACComponent);
      edges.push_back(edgeHVACComponent.get());
    }
  };

  if( prev) {
    if( auto inletModelObject = supplyInletModelObject() ) {
      if( prev.get() == inletModelObject.get() ) {
        pushSupplyOutletModelObject();
        return edges;
      }
    }
    if( auto inletModelObject = demandInletModelObject() ) {
      if( prev.get() == inletModelObject.get() ) {
        pushDemandOutletModelObject();
        return edges;
      }
    }
    if( auto inletModelObject = tertiaryInletModelObject() ) {
      if( prev.get() == inletModelObject.get() ) {
        pushTertiaryOutletModelObject();
        return edges;
      }
    }
  } else {
    pushSupplyOutletModelObject();
    pushDemandOutletModelObject();
    pushTertiaryOutletModelObject();
    return edges;
  }

  return edges;
}

std::vector<openstudio::IdfObject> WaterToWaterComponent_Impl::remove()
{
  removeFromPlantLoop();

  removeFromSecondaryPlantLoop();

  removeFromTertiaryPlantLoop();

  return HVACComponent_Impl::remove();
}

boost::optional<ParentObject> WaterToWaterComponent_Impl::parent() const
{
  return boost::optional<ParentObject>();
}

std::vector<ModelObject> WaterToWaterComponent_Impl::children() const
{
  std::vector<ModelObject> result;

  return result;
}

void WaterToWaterComponent_Impl::disconnect()
{
  ModelObject mo = this->getObject<ModelObject>();
  this->model().disconnect(mo, this->supplyInletPort());
  this->model().disconnect(mo, this->supplyOutletPort());
  this->model().disconnect(mo, this->demandInletPort());
  this->model().disconnect(mo, this->demandOutletPort());
  this->model().disconnect(mo, this->tertiaryInletPort());
  this->model().disconnect(mo, this->tertiaryOutletPort());
}

bool WaterToWaterComponent_Impl::addToNode(Node & node)
{
  auto _model = node.model();
  auto thisModelObject = getObject<ModelObject>();
  auto t_plantLoop = node.plantLoop();

  boost::optional<unsigned> componentInletPort;
  boost::optional<unsigned> componentOutletPort;

  boost::optional<HVACComponent> systemStartComponent;
  boost::optional<HVACComponent> systemEndComponent;

  if( node.getImpl<Node_Impl>()->isConnected(thisModelObject) ) return false;

  if( t_plantLoop ) {
    if( t_plantLoop->supplyComponent(node.handle()) ) {

      systemStartComponent = t_plantLoop->supplyInletNode();
      systemEndComponent = t_plantLoop->supplyOutletNode();
      componentInletPort = supplyInletPort();
      componentOutletPort = supplyOutletPort();

      removeFromPlantLoop();
    } else if( t_plantLoop->demandComponent(node.handle()) ) {

      systemStartComponent = t_plantLoop->demandInletNode();
      systemEndComponent = t_plantLoop->demandOutletNode();
      componentInletPort = demandInletPort();
      componentOutletPort = demandOutletPort();

      removeFromSecondaryPlantLoop();
    }
  }

  if( systemStartComponent && systemEndComponent && componentOutletPort && componentInletPort ) {
    return HVACComponent_Impl::addToNode(node,systemStartComponent.get(),systemEndComponent.get(),componentInletPort.get(),componentOutletPort.get());
  } else {
    return false;
  }
}

ModelObject WaterToWaterComponent_Impl::clone(Model model) const
{
  WaterToWaterComponent mo =  HVACComponent_Impl::clone( model ).cast<WaterToWaterComponent>();

  mo.setString(mo.supplyInletPort(),"");
  mo.setString(mo.supplyOutletPort(),"");
  mo.setString(mo.demandInletPort(),"");
  mo.setString(mo.demandOutletPort(),"");
  mo.setString(mo.getImpl<detail::WaterToWaterComponent_Impl>()->tertiaryInletPort(),"");
  mo.setString(mo.getImpl<detail::WaterToWaterComponent_Impl>()->tertiaryOutletPort(),"");

  return mo;
}

boost::optional<PlantLoop> WaterToWaterComponent_Impl::plantLoop() const
{
  if( m_plantLoop )
  {
    return m_plantLoop;
  }
  else
  {
    std::vector<PlantLoop> plantLoops = this->model().getConcreteModelObjects<PlantLoop>();

    for(const auto & elem : plantLoops)
    {
      OptionalPlantLoop plantLoop = elem.optionalCast<PlantLoop>();
      if(plantLoop)
      {
        if( plantLoop->supplyComponent(this->handle()) )
        {
          m_plantLoop = plantLoop;

          return plantLoop;
        }
      }
    }
  }

  return boost::none;
}

boost::optional<PlantLoop> WaterToWaterComponent_Impl::secondaryPlantLoop() const
{
  if( m_secondaryPlantLoop )
  {
    return m_secondaryPlantLoop;
  }
  else
  {
    std::vector<PlantLoop> plantLoops = this->model().getConcreteModelObjects<PlantLoop>();

    for(const auto & elem : plantLoops)
    {
      OptionalPlantLoop plantLoop = elem.optionalCast<PlantLoop>();
      if(plantLoop)
      {
        if( plantLoop->demandComponent(this->handle()) )
        {
          m_secondaryPlantLoop = plantLoop;

          return plantLoop;
        }
      }
    }
  }

  return boost::none;
}

bool WaterToWaterComponent_Impl::removeFromPlantLoop()
{
  if( auto plant = plantLoop() ) {
    return HVACComponent_Impl::removeFromLoop(plant->supplyInletNode(),
      plant->supplyOutletNode(),
      supplyInletPort(),
      supplyOutletPort());
  }

  return false;
}

bool WaterToWaterComponent_Impl::removeFromSecondaryPlantLoop()
{
  if( auto plant = secondaryPlantLoop() ) {
    return HVACComponent_Impl::removeFromLoop(plant->demandInletNode(),
      plant->demandOutletNode(),
      demandInletPort(),
      demandOutletPort());
  }

  return false;
}

unsigned WaterToWaterComponent_Impl::tertiaryInletPort() const
{
  return std::numeric_limits<unsigned>::max();
}

unsigned WaterToWaterComponent_Impl::tertiaryOutletPort() const
{
  return std::numeric_limits<unsigned>::max();
}

boost::optional<PlantLoop> WaterToWaterComponent_Impl::tertiaryPlantLoop() const
{
  if( m_tertiaryPlantLoop ) {
    return m_tertiaryPlantLoop;
  } else {
    boost::optional<HVACComponent> tertiaryOutletHVACComponent;

    if( auto t_tertiaryOutletModelObject = tertiaryOutletModelObject() ) {
      tertiaryOutletHVACComponent = t_tertiaryOutletModelObject->optionalCast<HVACComponent>();
    }

    if( tertiaryOutletHVACComponent ) {
      auto plantLoops = this->model().getConcreteModelObjects<PlantLoop>();

      // WaterToWaterComponent instances can be connected to tertiary plants
      // from the demand or supply side so check for both possibilities
      for(const auto & plantLoop : plantLoops) {
        if( ! plantLoop.supplyComponents(plantLoop.supplyInletNode(),tertiaryOutletHVACComponent.get()).empty() ) {
          m_tertiaryPlantLoop = plantLoop;
          return plantLoop;
        }
      }

      for(const auto & plantLoop : plantLoops) {
        if( ! plantLoop.demandComponents(plantLoop.demandInletNode(),tertiaryOutletHVACComponent.get()).empty() ) {
          m_tertiaryPlantLoop = plantLoop;
          return plantLoop;
        }
      }
    }
  }

  return boost::none;
}

bool WaterToWaterComponent_Impl::removeFromTertiaryPlantLoop()
{
  if( auto plant = tertiaryPlantLoop() ) {
    return HVACComponent_Impl::removeFromLoop(plant->demandInletNode(),
      plant->demandOutletNode(),
      tertiaryInletPort(),
      tertiaryOutletPort());
  }

  return false;
}

bool WaterToWaterComponent_Impl::addToTertiaryNode(Node & node)
{
  auto _model = node.model();
  auto thisModelObject = getObject<ModelObject>();
  auto t_plantLoop = node.plantLoop();

  boost::optional<unsigned> componentInletPort = tertiaryInletPort();
  boost::optional<unsigned> componentOutletPort = tertiaryOutletPort();

  boost::optional<HVACComponent> systemStartComponent;
  boost::optional<HVACComponent> systemEndComponent;

  if( node.getImpl<Node_Impl>()->isConnected(thisModelObject) ) return false;

  if( t_plantLoop ) {
    if( t_plantLoop->supplyComponent(node.handle()) ) {

      systemStartComponent = t_plantLoop->supplyInletNode();
      systemEndComponent = t_plantLoop->supplyOutletNode();

      removeFromTertiaryPlantLoop();
    } else if( t_plantLoop->demandComponent(node.handle()) ) {

      systemStartComponent = t_plantLoop->demandInletNode();
      systemEndComponent = t_plantLoop->demandOutletNode();

      removeFromTertiaryPlantLoop();
    }
  }

  if( systemStartComponent && systemEndComponent && componentOutletPort && componentInletPort ) {
    return HVACComponent_Impl::addToNode(node,systemStartComponent.get(),systemEndComponent.get(),componentInletPort.get(),componentOutletPort.get());
  } else {
    return false;
  }
}

boost::optional<ModelObject> WaterToWaterComponent_Impl::tertiaryInletModelObject() const
{
  return connectedObject(tertiaryInletPort());
}

boost::optional<ModelObject> WaterToWaterComponent_Impl::tertiaryOutletModelObject() const
{
  return connectedObject(tertiaryOutletPort());
}

} // detail

WaterToWaterComponent::WaterToWaterComponent(IddObjectType type,const Model& model)
  : HVACComponent(type,model)
{
  OS_ASSERT(getImpl<detail::WaterToWaterComponent_Impl>());
}

WaterToWaterComponent::WaterToWaterComponent(std::shared_ptr<detail::WaterToWaterComponent_Impl> p)
  : HVACComponent(std::move(p))
{}

unsigned WaterToWaterComponent::supplyInletPort()
{
  return getImpl<detail::WaterToWaterComponent_Impl>()->supplyInletPort();
}

unsigned WaterToWaterComponent::supplyOutletPort()
{
  return getImpl<detail::WaterToWaterComponent_Impl>()->supplyOutletPort();
}

OptionalModelObject WaterToWaterComponent::supplyInletModelObject()
{
  return getImpl<detail::WaterToWaterComponent_Impl>()->supplyInletModelObject();
}

OptionalModelObject WaterToWaterComponent::supplyOutletModelObject()
{
  return getImpl<detail::WaterToWaterComponent_Impl>()->supplyOutletModelObject();
}

unsigned WaterToWaterComponent::demandInletPort()
{
  return getImpl<detail::WaterToWaterComponent_Impl>()->demandInletPort();
}

unsigned WaterToWaterComponent::demandOutletPort()
{
  return getImpl<detail::WaterToWaterComponent_Impl>()->demandOutletPort();
}

OptionalModelObject WaterToWaterComponent::demandInletModelObject()
{
  return getImpl<detail::WaterToWaterComponent_Impl>()->demandInletModelObject();
}

OptionalModelObject WaterToWaterComponent::demandOutletModelObject()
{
  return getImpl<detail::WaterToWaterComponent_Impl>()->demandOutletModelObject();
}

std::vector<IdfObject> WaterToWaterComponent::remove()
{
  return getImpl<detail::WaterToWaterComponent_Impl>()->remove();
}

bool WaterToWaterComponent::addToNode(Node & node)
{
  return getImpl<detail::WaterToWaterComponent_Impl>()->addToNode( node );
}

ModelObject WaterToWaterComponent::clone(Model model) const
{
  return getImpl<detail::WaterToWaterComponent_Impl>()->clone( model );
}

void WaterToWaterComponent::disconnect()
{
  getImpl<detail::WaterToWaterComponent_Impl>()->disconnect();
}

boost::optional<PlantLoop> WaterToWaterComponent::plantLoop() const
{
  return getImpl<detail::WaterToWaterComponent_Impl>()->plantLoop();
}

boost::optional<PlantLoop> WaterToWaterComponent::secondaryPlantLoop() const
{
  return getImpl<detail::WaterToWaterComponent_Impl>()->secondaryPlantLoop();
}

bool WaterToWaterComponent::removeFromPlantLoop()
{
  return getImpl<detail::WaterToWaterComponent_Impl>()->removeFromPlantLoop();
}

bool WaterToWaterComponent::removeFromSecondaryPlantLoop()
{
  return getImpl<detail::WaterToWaterComponent_Impl>()->removeFromSecondaryPlantLoop();
}

boost::optional<PlantLoop> WaterToWaterComponent::tertiaryPlantLoop() const {
  return getImpl<detail::WaterToWaterComponent_Impl>()->tertiaryPlantLoop();
}

bool WaterToWaterComponent::removeFromTertiaryPlantLoop() {
  return getImpl<detail::WaterToWaterComponent_Impl>()->removeFromTertiaryPlantLoop();
}

bool WaterToWaterComponent::addToTertiaryNode(Node & node) {
  return getImpl<detail::WaterToWaterComponent_Impl>()->addToTertiaryNode(node);
}

boost::optional<ModelObject> WaterToWaterComponent::tertiaryInletModelObject() const {
  return getImpl<detail::WaterToWaterComponent_Impl>()->tertiaryInletModelObject();
}

boost::optional<ModelObject> WaterToWaterComponent::tertiaryOutletModelObject() const {
  return getImpl<detail::WaterToWaterComponent_Impl>()->tertiaryOutletModelObject();
}

} // model

} // openstudio

