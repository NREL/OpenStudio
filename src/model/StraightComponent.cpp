/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "StraightComponent.hpp"
#include "StraightComponent_Impl.hpp"
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
#include "AirflowNetworkEquivalentDuct.hpp"
#include "AirflowNetworkEquivalentDuct_Impl.hpp"
#include "../utilities/core/Assert.hpp"

namespace openstudio {

namespace model {

  namespace detail {

    StraightComponent_Impl::StraightComponent_Impl(IddObjectType type, Model_Impl* model) : HVACComponent_Impl(type, model) {}

    StraightComponent_Impl::StraightComponent_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
      : HVACComponent_Impl(idfObject, model, keepHandle) {}

    StraightComponent_Impl::StraightComponent_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle)
      : HVACComponent_Impl(other, model, keepHandle) {}

    StraightComponent_Impl::StraightComponent_Impl(const StraightComponent_Impl& other, Model_Impl* model, bool keepHandles)
      : HVACComponent_Impl(other, model, keepHandles) {}

    OptionalModelObject StraightComponent_Impl::inletModelObject() const {
      return connectedObject(inletPort());
    }

    OptionalModelObject StraightComponent_Impl::outletModelObject() const {
      return connectedObject(outletPort());
    }
    bool StraightComponent_Impl::removeFromLoop() {
      if (auto t_oaSystem = airLoopHVACOutdoorAirSystem()) {
        if (t_oaSystem->oaComponent(handle())) {
          return HVACComponent_Impl::removeFromLoop(t_oaSystem->outboardOANode().get(), t_oaSystem.get(), inletPort(), outletPort());
        } else if (t_oaSystem->reliefComponent(handle())) {
          return HVACComponent_Impl::removeFromLoop(t_oaSystem.get(), t_oaSystem->outboardReliefNode().get(), inletPort(), outletPort());
        }
      } else if (auto t_loop = loop()) {
        if (t_loop->supplyComponent(handle())) {
          return HVACComponent_Impl::removeFromLoop(t_loop->supplyInletNode(), t_loop->supplyOutletNode(), inletPort(), outletPort());
        } else if (t_loop->demandComponent(handle())) {
          return HVACComponent_Impl::removeFromLoop(t_loop->demandInletNode(), t_loop->demandOutletNode(), inletPort(), outletPort());
        }
      }

      return false;
    }

    std::vector<openstudio::IdfObject> StraightComponent_Impl::remove() {
      removeFromLoop();

      return HVACComponent_Impl::remove();
    }

    boost::optional<ParentObject> StraightComponent_Impl::parent() const {
      return {};
    }

    std::vector<ModelObject> StraightComponent_Impl::children() const {
      std::vector<ModelObject> result;
      return result;
    }

    OptionalAirLoopHVAC StraightComponent_Impl::airLoopHVAC() const {
      return HVACComponent_Impl::airLoopHVAC();
    }

    void StraightComponent_Impl::disconnect() {
      auto mo = this->getObject<ModelObject>();
      this->model().disconnect(mo, this->inletPort());
      this->model().disconnect(mo, this->outletPort());
    }

    std::vector<HVACComponent> StraightComponent_Impl::edges(const boost::optional<HVACComponent>& /*prev*/) {
      std::vector<HVACComponent> edges;
      if (auto edgeModelObject = outletModelObject()) {
        if (auto edgeObject = edgeModelObject->optionalCast<HVACComponent>()) {
          edges.push_back(*edgeObject);
        }
      }
      return edges;
    }

    bool StraightComponent_Impl::addToNode(Node& node) {
      Model _model = node.model();
      auto thisModelObject = getObject<ModelObject>();
      auto t_loop = node.loop();
      auto t_oaSystem = node.airLoopHVACOutdoorAirSystem();

      auto componentInletPort = inletPort();
      auto componentOutletPort = outletPort();

      boost::optional<HVACComponent> systemStartComponent;
      boost::optional<HVACComponent> systemEndComponent;

      if (node.getImpl<Node_Impl>()->isConnected(thisModelObject)) {
        return false;
      }

      if (t_loop && !t_oaSystem) {
        if (t_loop->supplyComponent(node.handle())) {
          systemStartComponent = t_loop->supplyInletNode();
          auto nodes = t_loop->supplyOutletNodes();
          OS_ASSERT(!nodes.empty());
          if ((nodes.size() == 2u) && (!t_loop->supplyComponents(node, nodes[1]).empty())) {
            systemEndComponent = nodes[1];
          } else {
            systemEndComponent = nodes[0];
          }
          OS_ASSERT(systemEndComponent);
        } else if (t_loop->demandComponent(node.handle())) {
          systemStartComponent = t_loop->demandInletNode();
          systemEndComponent = t_loop->demandOutletNode();
        }
      } else if (t_oaSystem) {
        if (t_oaSystem->oaComponent(node.handle())) {
          systemStartComponent = t_oaSystem->outboardOANode();
          systemEndComponent = t_oaSystem.get();
        } else if (t_oaSystem->reliefComponent(node.handle())) {
          systemStartComponent = t_oaSystem.get();
          systemEndComponent = t_oaSystem->outboardReliefNode();
        }
      }

      if (systemStartComponent && systemEndComponent) {
        removeFromLoop();
        return HVACComponent_Impl::addToNode(node, systemStartComponent.get(), systemEndComponent.get(), componentInletPort, componentOutletPort);
      } else {
        return false;
      }
    }

    ModelObject StraightComponent_Impl::clone(Model model) const {
      auto mo = HVACComponent_Impl::clone(model).cast<StraightComponent>();

      mo.setString(mo.inletPort(), "");
      mo.setString(mo.outletPort(), "");

      return std::move(mo);
    }

  }  // namespace detail

  StraightComponent::StraightComponent(IddObjectType type, const Model& model) : HVACComponent(type, model) {
    OS_ASSERT(getImpl<detail::StraightComponent_Impl>());
  }

  StraightComponent::StraightComponent(std::shared_ptr<detail::StraightComponent_Impl> p) : HVACComponent(std::move(p)) {}

  unsigned StraightComponent::inletPort() const {
    return getImpl<detail::StraightComponent_Impl>()->inletPort();
  }

  unsigned StraightComponent::outletPort() const {
    return getImpl<detail::StraightComponent_Impl>()->outletPort();
  }

  OptionalModelObject StraightComponent::inletModelObject() const {
    return getImpl<detail::StraightComponent_Impl>()->inletModelObject();
  }

  OptionalModelObject StraightComponent::outletModelObject() const {
    return getImpl<detail::StraightComponent_Impl>()->outletModelObject();
  }

  bool StraightComponent::removeFromLoop() {
    return getImpl<detail::StraightComponent_Impl>()->removeFromLoop();
  }

  OptionalAirLoopHVAC StraightComponent::airLoopHVAC() const {
    return getImpl<detail::StraightComponent_Impl>()->airLoopHVAC();
  }

}  // namespace model

}  // namespace openstudio
