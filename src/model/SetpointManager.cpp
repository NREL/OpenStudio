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

#include "SetpointManager.hpp"
#include "SetpointManager_Impl.hpp"
#include "Model.hpp"
#include "Node.hpp"
#include "Node_Impl.hpp"
#include "AirLoopHVAC.hpp"
#include "PlantLoop.hpp"
#include "Splitter.hpp"
#include "Mixer.hpp"
#include <algorithm>

#include "../utilities/core/Assert.hpp"

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

    SetpointManager_Impl::~SetpointManager_Impl() {}

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
      SetpointManager clonedObject = HVACComponent_Impl::clone(model).cast<SetpointManager>();
      clonedObject.getImpl<detail::SetpointManager_Impl>()->resetSetpointNode();
      return clonedObject;
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

  }  // namespace detail

  SetpointManager::SetpointManager(IddObjectType type, const Model& model) : HVACComponent(type, model) {
    OS_ASSERT(getImpl<detail::SetpointManager_Impl>());
  }

  SetpointManager::SetpointManager(std::shared_ptr<detail::SetpointManager_Impl> p) : HVACComponent(std::move(p)) {}

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
