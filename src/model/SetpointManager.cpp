/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "SetpointManager.hpp"
#include "SetpointManager_Impl.hpp"
#include "Model.hpp"
#include "Node.hpp"
#include "Node_Impl.hpp"
#include "AirLoopHVAC.hpp"
#include "PlantLoop.hpp"
#include "Splitter.hpp"
#include "Mixer.hpp"

#include "../utilities/core/Assert.hpp"
#include "../utilities/data/DataEnums.hpp"

#include <algorithm>

namespace openstudio {

namespace model {

  namespace detail {

    SetpointManager_Impl::SetpointManager_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
      : HVACComponent_Impl(idfObject, model, keepHandle) {}

    SetpointManager_Impl::SetpointManager_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle)
      : HVACComponent_Impl(other, model, keepHandle) {}

    SetpointManager_Impl::SetpointManager_Impl(const SetpointManager_Impl& other, Model_Impl* model, bool keepHandles)
      : HVACComponent_Impl(other, model, keepHandles) {}

    SetpointManager_Impl::SetpointManager_Impl(IddObjectType type, Model_Impl* model) : HVACComponent_Impl(type, model) {}

    const std::vector<std::string>& SetpointManager_Impl::outputVariableNames() const {
      static const std::vector<std::string> result;
      return result;
    }

    boost::optional<ParentObject> SetpointManager_Impl::parent() const {
      NodeVector nodes = getObject<ModelObject>().getModelObjectSources<Node>();
      if (nodes.size() == 1u) {
        return nodes[0];
      }
      return boost::none;
    }

    std::vector<ModelObject> SetpointManager_Impl::children() const {
      std::vector<ModelObject> result;
      return result;
    }

    /** Returns false by default. This is a virtual method which will be overriden for specific SPMs to return true
   * if they are allowed on a plantLoop
   **/
    bool SetpointManager_Impl::isAllowedOnPlantLoop() const {
      return false;
    }

    bool SetpointManager_Impl::addToNode(Node& node) {
      if (node.model() != this->model()) {
        return false;
      }

      // Erase any existing setpoint manager that has the same control variable
      // eg you can't have two temperature ones. But Humidity ones can be MaximumHumidityRatio or MinimumHumidityRatio so you can have a Min and Max
      std::vector<SetpointManager> _setpointManagers = node.setpointManagers();
      if (!_setpointManagers.empty()) {
        for (auto it = _setpointManagers.begin(); it != _setpointManagers.end(); ++it) {
          if (istringEqual(this->controlVariable(), it->controlVariable())) {
            it->remove();
          }
        }
      }

      if (OptionalAirLoopHVAC airLoop = node.airLoopHVAC()) {
        // If this is one of the regular nodes of the supply path (eg not in the AirLoopHVACOASys)
        if (airLoop->supplyComponent(node.handle())) {
          return this->setSetpointNode(node);
        }
      }

      if (OptionalAirLoopHVACOutdoorAirSystem oaSystem = node.airLoopHVACOutdoorAirSystem()) {
        // We only accept it if it's neither the relief or the OA node (doesn't make sense to place one there)
        if ((node != oaSystem->outboardReliefNode()) && (node != oaSystem->outboardOANode())) {
          return this->setSetpointNode(node);
        }
      }

      // If the specific SPM (derived class) is allowed on PlantLoop, then we allow it on the supply side,
      // or the demand side EXCEPT on a demand branch
      if (boost::optional<PlantLoop> plant = node.plantLoop()) {
        if (this->isAllowedOnPlantLoop()) {
          // If it's the supply side
          if (plant->supplyComponent(node.handle())) {
            return this->setSetpointNode(node);
          } else {
            // On the demand side
            Splitter splitter = plant->demandSplitter();
            Mixer mixer = plant->demandMixer();
            // We check that the node is NOT between the splitter and the mixer
            auto branchcomps = plant->demandComponents(splitter, mixer);
            if (std::find(branchcomps.begin(), branchcomps.end(), node) == branchcomps.end()) {
              return this->setSetpointNode(node);
            } else {
              LOG(Info, this->briefDescription() << " cannot be added on a demand branch");
            }
          }
        } else {
          LOG(Info, "This SetpointManager cannot be connected to a PlantLoop, for " << this->briefDescription());
        }
      }

      return false;
    }

    std::vector<openstudio::IdfObject> SetpointManager_Impl::remove() {
      return HVACComponent_Impl::remove();
    }

    ModelObject SetpointManager_Impl::clone(Model model) const {
      auto clonedObject = HVACComponent_Impl::clone(model).cast<SetpointManager>();
      clonedObject.getImpl<detail::SetpointManager_Impl>()->resetSetpointNode();
      return std::move(clonedObject);
    }

    boost::optional<Loop> SetpointManager_Impl::loop() const {
      if (boost::optional<Node> node = setpointNode()) {
        return node->loop();
      }
      return boost::none;
    }

    boost::optional<AirLoopHVAC> SetpointManager_Impl::airLoopHVAC() const {
      if (boost::optional<Node> node = setpointNode()) {
        return node->airLoopHVAC();
      }
      return boost::none;
    }

    boost::optional<AirLoopHVACOutdoorAirSystem> SetpointManager_Impl::airLoopHVACOutdoorAirSystem() const {
      if (boost::optional<Node> node = setpointNode()) {
        return node->airLoopHVACOutdoorAirSystem();
      }
      return boost::none;
    }

    boost::optional<PlantLoop> SetpointManager_Impl::plantLoop() const {
      if (boost::optional<Node> node = setpointNode()) {
        return node->plantLoop();
      }
      return boost::none;
    }

    ComponentType SetpointManager_Impl::componentType() const {
      return ComponentType::None;
    }

    std::vector<FuelType> SetpointManager_Impl::coolingFuelTypes() const {
      return {};
    }

    std::vector<FuelType> SetpointManager_Impl::heatingFuelTypes() const {
      return {};
    }

    std::vector<AppGFuelType> SetpointManager_Impl::appGHeatingFuelTypes() const {
      return {};
    }

  }  // namespace detail

  SetpointManager::SetpointManager(IddObjectType type, const Model& model) : HVACComponent(type, model) {
    OS_ASSERT(getImpl<detail::SetpointManager_Impl>());
  }

  SetpointManager::SetpointManager(std::shared_ptr<detail::SetpointManager_Impl> impl) : HVACComponent(std::move(impl)) {}

  boost::optional<Node> SetpointManager::setpointNode() const {
    return getImpl<detail::SetpointManager_Impl>()->setpointNode();
  }

  std::string SetpointManager::controlVariable() const {
    return getImpl<detail::SetpointManager_Impl>()->controlVariable();
  }

  bool SetpointManager::setControlVariable(const std::string& value) {
    return getImpl<detail::SetpointManager_Impl>()->setControlVariable(value);
  }

  bool SetpointManager::isAllowedOnPlantLoop() const {
    return getImpl<detail::SetpointManager_Impl>()->isAllowedOnPlantLoop();
  }

}  // namespace model
}  // namespace openstudio
