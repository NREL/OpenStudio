/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "WaterToAirComponent.hpp"
#include "WaterToAirComponent_Impl.hpp"
#include "ControllerWaterCoil.hpp"
#include "ControllerWaterCoil_Impl.hpp"
#include "Model.hpp"

#include "AirLoopHVAC.hpp"
#include "AirLoopHVAC_Impl.hpp"
#include "PlantLoop.hpp"
#include "PlantLoop_Impl.hpp"
#include "Node.hpp"
#include "Node_Impl.hpp"
#include "Splitter.hpp"
#include "Splitter_Impl.hpp"
#include "Mixer.hpp"
#include "Mixer_Impl.hpp"
#include "../utilities/core/Assert.hpp"

namespace openstudio {

namespace model {

  namespace detail {

    WaterToAirComponent_Impl::WaterToAirComponent_Impl(IddObjectType type, Model_Impl* model) : HVACComponent_Impl(type, model) {}

    WaterToAirComponent_Impl::WaterToAirComponent_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
      : HVACComponent_Impl(idfObject, model, keepHandle) {}

    WaterToAirComponent_Impl::WaterToAirComponent_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle)
      : HVACComponent_Impl(other, model, keepHandle) {}

    WaterToAirComponent_Impl::WaterToAirComponent_Impl(const WaterToAirComponent_Impl& other, Model_Impl* model, bool keepHandles)
      : HVACComponent_Impl(other, model, keepHandles) {}

    boost::optional<Loop> WaterToAirComponent_Impl::loop() const {
      return boost::none;
    }

    boost::optional<ModelObject> WaterToAirComponent_Impl::airInletModelObject() const {
      return connectedObject(airInletPort());
    }

    boost::optional<ModelObject> WaterToAirComponent_Impl::airOutletModelObject() const {
      return connectedObject(airOutletPort());
    }

    boost::optional<ModelObject> WaterToAirComponent_Impl::waterInletModelObject() const {
      return connectedObject(waterInletPort());
    }

    boost::optional<ModelObject> WaterToAirComponent_Impl::waterOutletModelObject() const {
      return connectedObject(waterOutletPort());
    }

    std::vector<HVACComponent> WaterToAirComponent_Impl::edges(const boost::optional<HVACComponent>& prev) {
      std::vector<HVACComponent> edges;

      auto pushWaterOutletModelObject = [&]() {
        if (auto edgeModelObject = waterOutletModelObject()) {
          auto edgeHVACComponent = edgeModelObject->optionalCast<HVACComponent>();
          OS_ASSERT(edgeHVACComponent);
          edges.push_back(edgeHVACComponent.get());
        }
      };

      auto pushAirOutletModelObject = [&]() {
        if (auto edgeModelObject = airOutletModelObject()) {
          auto edgeHVACComponent = edgeModelObject->optionalCast<HVACComponent>();
          OS_ASSERT(edgeHVACComponent);
          edges.push_back(edgeHVACComponent.get());
        }
      };

      if (prev) {
        if (auto inletModelObject = waterInletModelObject()) {
          if (prev.get() == inletModelObject.get()) {
            pushWaterOutletModelObject();
            return edges;
          }
        }
        if (auto inletModelObject = airInletModelObject()) {
          if (prev.get() == inletModelObject.get()) {
            pushAirOutletModelObject();
            return edges;
          }
        }
      } else {
        pushWaterOutletModelObject();
        pushAirOutletModelObject();
        return edges;
      }

      return edges;
    }

    boost::optional<AirLoopHVAC> WaterToAirComponent_Impl::airLoopHVAC() const {
      return HVACComponent_Impl::airLoopHVAC();
    }

    boost::optional<PlantLoop> WaterToAirComponent_Impl::plantLoop() const {
      return HVACComponent_Impl::plantLoop();
    }

    bool WaterToAirComponent_Impl::addToNode(Node& node) {
      auto _model = node.model();
      auto thisModelObject = getObject<ModelObject>();
      auto t_airLoop = node.airLoopHVAC();
      auto t_plantLoop = node.plantLoop();
      auto t_oaSystem = node.airLoopHVACOutdoorAirSystem();

      boost::optional<unsigned> componentInletPort;
      boost::optional<unsigned> componentOutletPort;

      boost::optional<HVACComponent> systemStartComponent;
      boost::optional<HVACComponent> systemEndComponent;

      if (node.getImpl<Node_Impl>()->isConnected(thisModelObject)) {
        return false;
      }

      if (t_airLoop && !t_oaSystem) {
        if (t_airLoop->demandComponent(node.handle())) {
          return false;
        }

        systemStartComponent = t_airLoop->supplyInletNode();
        auto nodes = t_airLoop->supplyOutletNodes();
        OS_ASSERT(!nodes.empty());
        if ((nodes.size() == 2u) && (!t_airLoop->supplyComponents(node, nodes[1]).empty())) {
          systemEndComponent = nodes[1];
        } else {
          systemEndComponent = nodes[0];
        }
        OS_ASSERT(systemEndComponent);
        componentInletPort = airInletPort();
        componentOutletPort = airOutletPort();

        removeFromAirLoopHVAC();
      } else if (t_oaSystem) {
        if (t_oaSystem->oaComponent(node.handle())) {
          systemStartComponent = t_oaSystem->outboardOANode();
          systemEndComponent = t_oaSystem.get();
          componentInletPort = airInletPort();
          componentOutletPort = airOutletPort();
        } else if (t_oaSystem->reliefComponent(node.handle())) {
          systemStartComponent = t_oaSystem.get();
          systemEndComponent = t_oaSystem->outboardReliefNode();
          componentInletPort = airInletPort();
          componentOutletPort = airOutletPort();
        }
        removeFromAirLoopHVAC();
      } else if (t_plantLoop) {
        if (!t_plantLoop->demandComponent(node.handle())) {
          return false;
        }

        systemStartComponent = t_plantLoop->demandInletNode();
        systemEndComponent = t_plantLoop->demandOutletNode();
        componentInletPort = waterInletPort();
        componentOutletPort = waterOutletPort();

        removeFromPlantLoop();
      }

      if (systemStartComponent && systemEndComponent && componentOutletPort && componentInletPort) {
        return HVACComponent_Impl::addToNode(node, systemStartComponent.get(), systemEndComponent.get(), componentInletPort.get(),
                                             componentOutletPort.get());
      } else {
        return false;
      }
    }

    std::vector<openstudio::IdfObject> WaterToAirComponent_Impl::remove() {
      if (isRemovable()) {
        removeFromAirLoopHVAC();

        removeFromPlantLoop();

        return HVACComponent_Impl::remove();
      }

      return {};
    }

    ModelObject WaterToAirComponent_Impl::clone(Model model) const {
      auto mo = HVACComponent_Impl::clone(model).cast<WaterToAirComponent>();

      mo.setString(mo.airInletPort(), "");
      mo.setString(mo.airOutletPort(), "");

      mo.setString(mo.waterInletPort(), "");
      mo.setString(mo.waterOutletPort(), "");

      return std::move(mo);
    }

    bool WaterToAirComponent_Impl::addToSplitter(Splitter& /*splitter*/) {
      //Model _model = splitter.model();
      //WaterToAirComponent thisModelObject = getObject<ModelObject>().optionalCast<WaterToAirComponent>().get();
      //if( splitter.plantLoop() )
      //{
      //  PlantLoop plantLoop = splitter.plantLoop().get();
      //  if( plantLoop.demandComponent( splitter.handle() ) )
      //  {
      //    Mixer mixer = plantLoop.demandMixer().get();
      //    unsigned splitterPort = splitter.newOutletPortAfterBranch( splitter.nextBranchIndex() );
      //    unsigned mixerPort = mixer.newInletPortAfterBranch( mixer.nextBranchIndex() );
      //    Node inletNode(_model);
      //    Node outletNode(_model);

      //    _model.connect( splitter, splitterPort,
      //                    inletNode, inletNode.inletPort() );
      //    _model.connect( inletNode, inletNode.outletPort(),
      //                    thisModelObject, thisModelObject.waterInletPort() );
      //    _model.connect( thisModelObject, thisModelObject.waterOutletPort(),
      //                    outletNode, outletNode.inletPort() );
      //    _model.connect( outletNode, outletNode.outletPort(),
      //                    mixer, mixerPort );

      //    return true;
      //  }
      //  else
      //  {
      //    return false;
      //  }
      //}
      //else if( splitter.airLoopHVAC() )
      //{
      //  return false;
      //}
      //else
      //{
      //  return false;
      //}
      return false;
    }

    void WaterToAirComponent_Impl::disconnectWaterSide() {
      Model _model = this->model();

      auto mo = this->getObject<ModelObject>();

      _model.disconnect(mo, this->waterInletPort());

      _model.disconnect(mo, this->waterOutletPort());
    }

    void WaterToAirComponent_Impl::disconnectAirSide() {
      Model _model = this->model();

      auto mo = this->getObject<ModelObject>();

      _model.disconnect(mo, this->airInletPort());

      _model.disconnect(mo, this->airOutletPort());
    }

    void WaterToAirComponent_Impl::disconnect() {
      this->disconnectWaterSide();

      this->disconnectAirSide();
    }

    bool WaterToAirComponent_Impl::removeFromAirLoopHVAC() {
      if (auto t_oaSystem = airLoopHVACOutdoorAirSystem()) {
        if (t_oaSystem->oaComponent(handle())) {
          return HVACComponent_Impl::removeFromLoop(t_oaSystem->outboardOANode().get(), t_oaSystem.get(), airInletPort(), airOutletPort());
        } else if (t_oaSystem->reliefComponent(handle())) {
          return HVACComponent_Impl::removeFromLoop(t_oaSystem.get(), t_oaSystem->outboardReliefNode().get(), airInletPort(), airOutletPort());
        }
      } else if (auto t_airLoop = airLoopHVAC()) {
        if (t_airLoop->supplyComponent(handle())) {
          return HVACComponent_Impl::removeFromLoop(t_airLoop->supplyInletNode(), t_airLoop->supplyOutletNode(), airInletPort(), airOutletPort());
        }
      }

      return false;
    }

    bool WaterToAirComponent_Impl::removeFromPlantLoop() {
      if (auto plant = plantLoop()) {
        return HVACComponent_Impl::removeFromLoop(plant->demandInletNode(), plant->demandOutletNode(), waterInletPort(), waterOutletPort());
      }

      return false;
    }

    boost::optional<ControllerWaterCoil> WaterToAirComponent_Impl::controllerWaterCoil() {
      auto controllers = model().getConcreteModelObjects<ControllerWaterCoil>();
      auto h = handle();

      for (const auto& controller : controllers) {
        if (auto coil = controller.getImpl<detail::ControllerWaterCoil_Impl>()->waterCoil()) {
          if (coil->handle() == h) {
            return controller.optionalCast<ControllerWaterCoil>();
          }
        }
      }

      return boost::none;
    }

  }  // namespace detail

  WaterToAirComponent::WaterToAirComponent(IddObjectType type, const Model& model) : HVACComponent(type, model) {
    OS_ASSERT(getImpl<detail::WaterToAirComponent_Impl>());
  }

  WaterToAirComponent::WaterToAirComponent(std::shared_ptr<detail::WaterToAirComponent_Impl> p) : HVACComponent(std::move(p)) {}

  boost::optional<ModelObject> WaterToAirComponent::airInletModelObject() const {
    return getImpl<detail::WaterToAirComponent_Impl>()->airInletModelObject();
  }

  boost::optional<ModelObject> WaterToAirComponent::airOutletModelObject() const {
    return getImpl<detail::WaterToAirComponent_Impl>()->airOutletModelObject();
  }

  boost::optional<ModelObject> WaterToAirComponent::waterInletModelObject() const {
    return getImpl<detail::WaterToAirComponent_Impl>()->waterInletModelObject();
  }

  boost::optional<ModelObject> WaterToAirComponent::waterOutletModelObject() const {
    return getImpl<detail::WaterToAirComponent_Impl>()->waterOutletModelObject();
  }

  boost::optional<AirLoopHVAC> WaterToAirComponent::airLoopHVAC() const {
    return getImpl<detail::WaterToAirComponent_Impl>()->airLoopHVAC();
  }

  boost::optional<PlantLoop> WaterToAirComponent::plantLoop() const {
    return getImpl<detail::WaterToAirComponent_Impl>()->plantLoop();
  }

  bool WaterToAirComponent::addToNode(Node& node) {
    return getImpl<detail::WaterToAirComponent_Impl>()->addToNode(node);
  }

  std::vector<openstudio::IdfObject> WaterToAirComponent::remove() {
    return getImpl<detail::WaterToAirComponent_Impl>()->remove();
  }

  ModelObject WaterToAirComponent::clone(Model model) const {
    return getImpl<detail::WaterToAirComponent_Impl>()->clone(model);
  }

  unsigned WaterToAirComponent::airInletPort() const {
    return getImpl<detail::WaterToAirComponent_Impl>()->airInletPort();
  }

  unsigned WaterToAirComponent::airOutletPort() const {
    return getImpl<detail::WaterToAirComponent_Impl>()->airOutletPort();
  }

  unsigned WaterToAirComponent::waterInletPort() const {
    return getImpl<detail::WaterToAirComponent_Impl>()->waterInletPort();
  }

  unsigned WaterToAirComponent::waterOutletPort() const {
    return getImpl<detail::WaterToAirComponent_Impl>()->waterOutletPort();
  }

  bool WaterToAirComponent::addToSplitter(Splitter& splitter) {
    return getImpl<detail::WaterToAirComponent_Impl>()->addToSplitter(splitter);
  }

  void WaterToAirComponent::disconnectWaterSide() {
    return getImpl<detail::WaterToAirComponent_Impl>()->disconnectWaterSide();
  }

  void WaterToAirComponent::disconnectAirSide() {
    return getImpl<detail::WaterToAirComponent_Impl>()->disconnectAirSide();
  }

  bool WaterToAirComponent::removeFromAirLoopHVAC() {
    return getImpl<detail::WaterToAirComponent_Impl>()->removeFromAirLoopHVAC();
  }

  bool WaterToAirComponent::removeFromPlantLoop() {
    return getImpl<detail::WaterToAirComponent_Impl>()->removeFromPlantLoop();
  }

}  // namespace model

}  // namespace openstudio
