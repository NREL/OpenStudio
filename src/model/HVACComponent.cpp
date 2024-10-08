/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "HVACComponent.hpp"
#include "HVACComponent_Impl.hpp"
#include "ZoneHVACComponent.hpp"
#include "ZoneHVACComponent_Impl.hpp"
#include "StraightComponent.hpp"
#include "StraightComponent_Impl.hpp"
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
#include "AirLoopHVACOutdoorAirSystem.hpp"
#include "AirLoopHVACOutdoorAirSystem_Impl.hpp"
#include "Model.hpp"
#include "Model_Impl.hpp"

#include "../utilities/core/Assert.hpp"
// #include "../utilities/data/DataEnums.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    HVACComponent_Impl::HVACComponent_Impl(IddObjectType type, Model_Impl* model) : ParentObject_Impl(type, model) {}

    HVACComponent_Impl::HVACComponent_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
      : ParentObject_Impl(idfObject, model, keepHandle) {}

    HVACComponent_Impl::HVACComponent_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle)
      : ParentObject_Impl(other, model, keepHandle) {}

    HVACComponent_Impl::HVACComponent_Impl(const HVACComponent_Impl& other, Model_Impl* model, bool keepHandles)
      : ParentObject_Impl(other, model, keepHandles) {}

    boost::optional<Loop> HVACComponent_Impl::loop() const {
      if (boost::optional<AirLoopHVAC> airLoopHVAC = this->airLoopHVAC()) {
        return airLoopHVAC->optionalCast<Loop>();
      } else if (boost::optional<PlantLoop> plantLoop = this->plantLoop()) {
        return plantLoop->optionalCast<Loop>();
      } else {
        return boost::none;
      }
    }

    boost::optional<AirLoopHVAC> HVACComponent_Impl::airLoopHVAC() const {
      if (m_airLoopHVAC) {
        return m_airLoopHVAC;
      } else {
        AirLoopHVACVector airLoops = this->model().getConcreteModelObjects<AirLoopHVAC>();

        for (auto& airLoop : airLoops) {
          if (airLoop.component(this->handle())) {
            m_airLoopHVAC = airLoop;
            return airLoop;
          }
          if (OptionalAirLoopHVACOutdoorAirSystem oaSystem = airLoop.airLoopHVACOutdoorAirSystem()) {
            if (oaSystem->component(this->handle())) {
              m_airLoopHVAC = airLoop;
              return airLoop;
            }
          }
        }
      }

      return boost::none;
    }

    boost::optional<AirLoopHVACOutdoorAirSystem> HVACComponent_Impl::airLoopHVACOutdoorAirSystem() const {
      if (m_airLoopHVACOutdoorAirSystem) {
        return m_airLoopHVACOutdoorAirSystem;
      } else {
        AirLoopHVACOutdoorAirSystemVector oaLoops = this->model().getConcreteModelObjects<AirLoopHVACOutdoorAirSystem>();

        for (auto& oaLoop : oaLoops) {
          if (oaLoop.component(this->handle())) {
            m_airLoopHVACOutdoorAirSystem = oaLoop;
            return oaLoop;
          }
        }
      }

      return boost::none;
    }

    boost::optional<PlantLoop> HVACComponent_Impl::plantLoop() const {
      if (m_plantLoop) {
        return m_plantLoop;
      } else {
        std::vector<PlantLoop> plantLoops = this->model().getConcreteModelObjects<PlantLoop>();

        for (auto& plantLoop : plantLoops) {
          if (plantLoop.component(this->handle())) {
            m_plantLoop = plantLoop;
            return plantLoop;
          }
        }
      }

      return boost::none;
    }

    bool HVACComponent_Impl::removeFromLoop(const HVACComponent& systemStartComponent, const HVACComponent& systemEndComponent,
                                            unsigned componentInletPort, unsigned componentOutletPort) {
      auto _model = model();
      auto thisObject = getObject<HVACComponent>();

      if (systemStartComponent.model() != _model) {
        return false;
      }
      if (systemEndComponent.model() != _model) {
        return false;
      }

      auto inletComponent = connectedObject(componentInletPort);
      auto outletComponent = connectedObject(componentOutletPort);
      auto inletComponentOutletPort = connectedObjectPort(componentInletPort);
      auto outletComponentInletPort = connectedObjectPort(componentOutletPort);

      if (!inletComponent) {
        return false;
      }
      if (!outletComponent) {
        return false;
      }
      if (!inletComponentOutletPort) {
        return false;
      }
      if (!outletComponentInletPort) {
        return false;
      }

      auto inletNode = inletComponent->optionalCast<Node>();
      auto outletNode = outletComponent->optionalCast<Node>();
      boost::optional<Splitter> splitter;
      boost::optional<Mixer> mixer;

      if (inletNode) {
        if (auto mo = inletNode->inletModelObject()) {
          splitter = mo->optionalCast<Splitter>();
        }
      }
      if (outletNode) {
        if (auto mo = outletNode->outletModelObject()) {
          mixer = mo->optionalCast<Mixer>();
        }
      }

      if (systemStartComponent.handle() == inletComponent->handle() && systemEndComponent.handle() == outletComponent->handle()) {
        // This component is between the systemStartComponent and the systemEndComponent
        // ie. the supply or demand inlet or outlet Nodes,
        // or the oa system end points on either the relief or inlet air streams.
        _model.disconnect(thisObject, componentInletPort);
        _model.disconnect(thisObject, componentOutletPort);

        _model.connect(inletComponent.get(), inletComponentOutletPort.get(), outletComponent.get(), outletComponentInletPort.get());

        return true;
      } else if (systemEndComponent.handle() == outletComponent->handle()) {
        // Here the systemEndComponent is immediately downstream of this component,
        // but there are other components (besides systemStartComponent) upstream.
        boost::optional<ModelObject> newInletComponent;
        boost::optional<unsigned> newInletComponentOutletPort;

        // Make sure we don't end up with two nodes next to each other after this component is removed
        if (inletNode && outletNode) {
          newInletComponent = inletNode->inletModelObject();
          newInletComponentOutletPort = inletNode->connectedObjectPort(inletNode->inletPort());
          OS_ASSERT(newInletComponent);
          OS_ASSERT(newInletComponentOutletPort);
        } else {
          newInletComponent = inletComponent;
          newInletComponentOutletPort = inletComponentOutletPort;
        }

        _model.disconnect(thisObject, componentInletPort);
        _model.disconnect(thisObject, componentOutletPort);

        // inletNode->remove() would have failed if we did it before the disconnect
        if (inletNode && outletNode) {
          inletNode->remove();
        }

        _model.connect(newInletComponent.get(), newInletComponentOutletPort.get(), outletComponent.get(), outletComponentInletPort.get());
        return true;
      } else if (splitter && mixer) {
        // If the component is the only component (except nodes) between a splitter mixer pair
        OS_ASSERT(inletNode);
        OS_ASSERT(outletNode);

        int i = splitter->branchIndexForOutletModelObject(inletNode.get());
        int j = mixer->branchIndexForInletModelObject(outletNode.get());

        OS_ASSERT(i == j);

        splitter->removePortForBranch(i);
        mixer->removePortForBranch(i);

        _model.disconnect(thisObject, componentInletPort);
        _model.disconnect(thisObject, componentOutletPort);

        inletNode->remove();
        outletNode->remove();

        if (!splitter->lastOutletModelObject()) {
          Node newNode(_model);
          _model.connect(splitter.get(), splitter->nextOutletPort(), newNode, newNode.inletPort());
          _model.connect(newNode, newNode.outletPort(), mixer.get(), mixer->nextInletPort());
        }

        return true;
      } else {
        boost::optional<ModelObject> newOutletComponent;
        boost::optional<unsigned> newOutletComponentInletPort;

        if (inletNode && outletNode) {
          newOutletComponent = outletNode->outletModelObject();
          newOutletComponentInletPort = outletNode->connectedObjectPort(outletNode->outletPort());
        }

        if (!newOutletComponent) {
          newOutletComponent = outletComponent;
        }
        if (!newOutletComponentInletPort) {
          newOutletComponentInletPort = outletComponentInletPort;
        }

        _model.disconnect(thisObject, componentInletPort);
        _model.disconnect(thisObject, componentOutletPort);

        // outletNode->remove() would have failed if we did it before the disconnect
        if (inletNode && outletNode) {
          outletNode->remove();
        }

        model().connect(inletComponent.get(), inletComponentOutletPort.get(), newOutletComponent.get(), newOutletComponentInletPort.get());

        return true;
      }

      return false;
    }

    bool HVACComponent_Impl::addToNode(Node& node, const HVACComponent& systemStartComponent, const HVACComponent& systemEndComponent,
                                       unsigned componentInletPort, unsigned componentOutletPort) {
      Model _model = model();
      auto thisModelObject = getObject<ModelObject>();

      if (node.model() != _model) {
        return false;
      }
      if (systemStartComponent.model() != _model) {
        return false;
      }
      if (systemEndComponent.model() != _model) {
        return false;
      }

      if ((node == systemEndComponent) && (node.inletModelObject().get() == systemStartComponent)) {
        unsigned oldOutletPort = node.connectedObjectPort(node.inletPort()).get();
        unsigned oldInletPort = node.inletPort();
        ModelObject oldSourceModelObject = node.connectedObject(node.inletPort()).get();
        ModelObject oldTargetModelObject = node;

        // If systemStartComponent is not a node we need to insert one so that there
        // is a node between systemStartComponent and this component we are adding
        // This situation occurs in the oa system relief stream.
        if (!systemStartComponent.optionalCast<Node>()) {
          Node newNode(_model);
          _model.connect(oldSourceModelObject, oldOutletPort, newNode, newNode.inletPort());
          oldOutletPort = newNode.outletPort();
          oldSourceModelObject = newNode;
        }

        _model.connect(oldSourceModelObject, oldOutletPort, thisModelObject, componentInletPort);
        _model.connect(thisModelObject, componentOutletPort, oldTargetModelObject, oldInletPort);
        return true;
      } else if ((node == systemStartComponent) && (node.outletModelObject().get() == systemEndComponent)) {
        unsigned oldInletPort = node.connectedObjectPort(node.outletPort()).get();
        unsigned oldOutletPort = node.outletPort();
        ModelObject oldTargetModelObject = node.connectedObject(node.outletPort()).get();
        ModelObject oldSourceModelObject = node;

        // If systemEndComponent is not a node we need to insert one so that there
        // is a node between systemEndComponent and this component we are adding
        // This situation occurs in the oa system outdoor air stream.
        if (!systemEndComponent.optionalCast<Node>()) {
          Node newNode(_model);
          _model.connect(newNode, newNode.outletPort(), oldTargetModelObject, oldInletPort);
          oldInletPort = newNode.inletPort();
          oldTargetModelObject = newNode;
        }

        _model.connect(oldSourceModelObject, oldOutletPort, thisModelObject, componentInletPort);
        _model.connect(thisModelObject, componentOutletPort, oldTargetModelObject, oldInletPort);
        return true;
      } else if (node == systemEndComponent) {
        unsigned oldOutletPort = node.connectedObjectPort(node.inletPort()).get();
        unsigned oldInletPort = node.inletPort();
        ModelObject oldSourceModelObject = node.connectedObject(node.inletPort()).get();
        ModelObject oldTargetModelObject = node;

        Node newNode(_model);
        _model.connect(oldSourceModelObject, oldOutletPort, newNode, newNode.inletPort());
        _model.connect(newNode, newNode.outletPort(), thisModelObject, componentInletPort);
        _model.connect(thisModelObject, componentOutletPort, oldTargetModelObject, oldInletPort);
        return true;
      } else {
        unsigned oldOutletPort = node.outletPort();
        unsigned oldInletPort = node.connectedObjectPort(node.outletPort()).get();
        ModelObject oldSourceModelObject = node;
        ModelObject oldTargetModelObject = node.connectedObject(node.outletPort()).get();

        Node newNode(_model);
        _model.connect(oldSourceModelObject, oldOutletPort, thisModelObject, componentInletPort);
        _model.connect(thisModelObject, componentOutletPort, newNode, newNode.inletPort());
        _model.connect(newNode, newNode.outletPort(), oldTargetModelObject, oldInletPort);
        return true;
      }

      return false;
    }

    bool HVACComponent_Impl::addToNode(Node& /*node*/) {
      return false;
    }

    std::vector<ModelObject> HVACComponent_Impl::children() const {
      return {};
    }

    boost::optional<ParentObject> HVACComponent_Impl::parent() const {
      return {};
    }

    bool HVACComponent_Impl::addToSplitter(Splitter& /*splitter*/) {
      return false;
    }

    const std::vector<std::string>& HVACComponent_Impl::outputVariableNames() const {
      static const std::vector<std::string> result;
      return result;
    }

    void HVACComponent_Impl::disconnect() {
      return;
    }

    bool HVACComponent_Impl::isRemovable() const {
      if (containingHVACComponent()) {
        return false;
      } else if (containingZoneHVACComponent()) {
        return false;
      } else if (containingStraightComponent()) {
        return false;
      } else {
        return true;
      }
    }

    std::vector<IdfObject> HVACComponent_Impl::remove() {
      if (this->isRemovable()) {
        return ParentObject_Impl::remove();
      } else {
        return {};
      }
    }

    boost::optional<ModelObject> HVACComponent_Impl::airLoopHVACAsModelObject() const {
      if (OptionalAirLoopHVAC intermediate = airLoopHVAC()) {
        return *intermediate;
      }
      return boost::none;
    }

    boost::optional<ModelObject> HVACComponent_Impl::plantLoopAsModelObject() const {
      if (OptionalPlantLoop intermediate = plantLoop()) {
        return *intermediate;
      }
      return boost::none;
    }

    boost::optional<ModelObject> HVACComponent_Impl::airLoopHVACOutdoorAirSystemAsModelObject() const {
      if (OptionalAirLoopHVACOutdoorAirSystem intermediate = airLoopHVACOutdoorAirSystem()) {
        return *intermediate;
      }
      return boost::none;
    }

    ModelObject HVACComponent_Impl::clone(Model model) const {
      // Note: JM 2018-08-29: children controls what we see in the Inspector in OS App, and it's not necessarilly what we want to clone
      // So the parent/children link is broken on purpose here
      return ModelObject_Impl::clone(model);
    }

    std::vector<HVACComponent> HVACComponent_Impl::edges(const boost::optional<HVACComponent>& /*previous*/) {
      return {};
    }

    boost::optional<HVACComponent> HVACComponent_Impl::containingHVACComponent() const {
      return boost::none;
    }

    boost::optional<ZoneHVACComponent> HVACComponent_Impl::containingZoneHVACComponent() const {
      return boost::none;
    }

    boost::optional<StraightComponent> HVACComponent_Impl::containingStraightComponent() const {
      return boost::none;
    }

    // default implementation does nothing.
    // should only be used by objects that have
    // no autosized fields
    void HVACComponent_Impl::autosize() {
      return;
    }

    // default implementation does nothing.
    // should only be used by objects that have
    // no autosized fields
    void HVACComponent_Impl::applySizingValues() {
      return;
    }

    std::vector<EMSActuatorNames> HVACComponent_Impl::emsActuatorNames() const {
      return {};
    }

    std::vector<std::string> HVACComponent_Impl::emsInternalVariableNames() const {
      return {};
    }

    // ComponentType HVACComponent_Impl::componentType() const {
    //   return ComponentType::None;
    // }

    // std::vector<FuelType> HVACComponent_Impl::coolingFuelTypes() const {
    //   return {};
    // }

    // std::vector<FuelType> HVACComponent_Impl::heatingFuelTypes() const {
    //   return {};
    // }

    // std::vector<AppGFuelType> HVACComponent_Impl::appGHeatingFuelTypes() const {
    //   return {};
    // }

  }  // namespace detail

  HVACComponent::HVACComponent(std::shared_ptr<detail::HVACComponent_Impl> p) : ParentObject(std::move(p)) {}

  HVACComponent::HVACComponent(IddObjectType type, const Model& model) : ParentObject(type, model) {
    OS_ASSERT(getImpl<detail::HVACComponent_Impl>());
  }

  boost::optional<Loop> HVACComponent::loop() const {
    return getImpl<detail::HVACComponent_Impl>()->loop();
  }

  boost::optional<AirLoopHVAC> HVACComponent::airLoopHVAC() const {
    return getImpl<detail::HVACComponent_Impl>()->airLoopHVAC();
  }

  boost::optional<AirLoopHVACOutdoorAirSystem> HVACComponent::airLoopHVACOutdoorAirSystem() const {
    return getImpl<detail::HVACComponent_Impl>()->airLoopHVACOutdoorAirSystem();
  }

  boost::optional<PlantLoop> HVACComponent::plantLoop() const {
    return getImpl<detail::HVACComponent_Impl>()->plantLoop();
  }

  bool HVACComponent::addToNode(Node& node) {
    return getImpl<detail::HVACComponent_Impl>()->addToNode(node);
  }

  bool HVACComponent::addToSplitter(Splitter& splitter) {
    return getImpl<detail::HVACComponent_Impl>()->addToSplitter(splitter);
  }

  void HVACComponent::disconnect() {
    return getImpl<detail::HVACComponent_Impl>()->disconnect();
  }

  bool HVACComponent::isRemovable() const {
    return getImpl<detail::HVACComponent_Impl>()->isRemovable();
  }

  std::vector<IdfObject> HVACComponent::remove() {
    return getImpl<detail::HVACComponent_Impl>()->remove();
  }

  boost::optional<HVACComponent> HVACComponent::containingHVACComponent() const {
    return getImpl<detail::HVACComponent_Impl>()->containingHVACComponent();
  }

  boost::optional<ZoneHVACComponent> HVACComponent::containingZoneHVACComponent() const {
    return getImpl<detail::HVACComponent_Impl>()->containingZoneHVACComponent();
  }

  boost::optional<StraightComponent> HVACComponent::containingStraightComponent() const {
    return getImpl<detail::HVACComponent_Impl>()->containingStraightComponent();
  }

  void HVACComponent::autosize() {
    return getImpl<detail::HVACComponent_Impl>()->autosize();
  }

  void HVACComponent::applySizingValues() {
    return getImpl<detail::HVACComponent_Impl>()->applySizingValues();
  }

  ComponentType HVACComponent::componentType() const {
    return getImpl<detail::HVACComponent_Impl>()->componentType();
  }

  std::vector<FuelType> HVACComponent::coolingFuelTypes() const {
    return getImpl<detail::HVACComponent_Impl>()->coolingFuelTypes();
  }

  std::vector<FuelType> HVACComponent::heatingFuelTypes() const {
    return getImpl<detail::HVACComponent_Impl>()->heatingFuelTypes();
  }

  std::vector<AppGFuelType> HVACComponent::appGHeatingFuelTypes() const {
    return getImpl<detail::HVACComponent_Impl>()->appGHeatingFuelTypes();
  }

}  // namespace model

}  // namespace openstudio
