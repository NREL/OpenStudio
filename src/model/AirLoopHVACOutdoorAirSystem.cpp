/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2023, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

#include "AirLoopHVACOutdoorAirSystem.hpp"
#include "AirLoopHVACOutdoorAirSystem_Impl.hpp"
#include "AirLoopHVACDedicatedOutdoorAirSystem.hpp"
#include "AirLoopHVACDedicatedOutdoorAirSystem_Impl.hpp"
#include "AirToAirComponent.hpp"
#include "AirToAirComponent_Impl.hpp"
#include "WaterToAirComponent.hpp"
#include "WaterToAirComponent_Impl.hpp"
#include "ZoneHVACComponent.hpp"
#include "ZoneHVACComponent_Impl.hpp"
#include "ControllerOutdoorAir.hpp"
#include "ControllerOutdoorAir_Impl.hpp"
#include "Node.hpp"
#include "Node_Impl.hpp"
#include "AirLoopHVAC.hpp"
#include "AirLoopHVAC_Impl.hpp"
#include "AirflowNetworkDistributionNode.hpp"
#include "AirflowNetworkDistributionNode_Impl.hpp"
#include "Model.hpp"
#include "Model_Impl.hpp"
#include "ModelExtensibleGroup.hpp"
#include "SetpointManager.hpp"
#include "SetpointManager_Impl.hpp"

#include "../utilities/core/Assert.hpp"
#include "../utilities/core/Compare.hpp"
#include "../utilities/core/ContainersMove.hpp"
#include "../utilities/data/DataEnums.hpp"
#include "../utilities/idf/IdfExtensibleGroup.hpp"

#include <utilities/idd/OS_AirLoopHVAC_OutdoorAirSystem_FieldEnums.hxx>
#include <utilities/idd/OS_AvailabilityManagerAssignmentList_FieldEnums.hxx>
#include <utilities/idd/OS_Controller_OutdoorAir_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include <algorithm>
#include <utility>

namespace openstudio {

namespace model {

  namespace detail {

    // Helpers
    std::pair<bool, boost::optional<ModelObject>> getOutletNodeForComponent(const ModelObject& modelObject) {
      if (auto comp_ = modelObject.optionalCast<StraightComponent>()) {
        return {true, comp_->outletModelObject()};
      } else if (auto comp_ = modelObject.optionalCast<AirToAirComponent>()) {
        return {true, comp_->secondaryAirOutletModelObject()};
      } else if (auto comp_ = modelObject.optionalCast<WaterToAirComponent>()) {
        return {true, comp_->airOutletModelObject()};
      } else if (auto comp_ = modelObject.optionalCast<ZoneHVACComponent>()) {
        return {true, comp_->airOutletModelObject()};
      }
      return {false, boost::none};
    }

    AirLoopHVACOutdoorAirSystem_Impl::AirLoopHVACOutdoorAirSystem_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
      : HVACComponent_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == AirLoopHVACOutdoorAirSystem::iddObjectType());
    }

    AirLoopHVACOutdoorAirSystem_Impl::AirLoopHVACOutdoorAirSystem_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model,
                                                                       bool keepHandle)
      : HVACComponent_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == AirLoopHVACOutdoorAirSystem::iddObjectType());
    }

    AirLoopHVACOutdoorAirSystem_Impl::AirLoopHVACOutdoorAirSystem_Impl(const AirLoopHVACOutdoorAirSystem_Impl& other, Model_Impl* model,
                                                                       bool keepHandle)
      : HVACComponent_Impl(other, model, keepHandle) {}

    // virtual destructor

    // return the parent object in the hierarchy
    boost::optional<ParentObject> AirLoopHVACOutdoorAirSystem_Impl::parent() const {
      return {};
    }

    std::vector<ModelObject> AirLoopHVACOutdoorAirSystem_Impl::children() const {
      std::vector<ModelObject> result;
      result.push_back(this->getControllerOutdoorAir());
      std::vector<AirflowNetworkDistributionNode> myAFNItems =
        getObject<ModelObject>().getModelObjectSources<AirflowNetworkDistributionNode>(AirflowNetworkDistributionNode::iddObjectType());
      result.insert(result.end(), myAFNItems.begin(), myAFNItems.end());
      return result;
    }

    // Get all output variable names that could be associated with this object.
    const std::vector<std::string>& AirLoopHVACOutdoorAirSystem_Impl::outputVariableNames() const {
      static const std::vector<std::string> result;
      // Not Appropriate: No variables available
      // The impact of using outside air/mechanical ventilation is described in the section: Outdoor Air Ventilation Outputs.
      return result;
    }

    IddObjectType AirLoopHVACOutdoorAirSystem_Impl::iddObjectType() const {
      return AirLoopHVACOutdoorAirSystem::iddObjectType();
    }

    unsigned AirLoopHVACOutdoorAirSystem_Impl::returnAirPort() const {
      return openstudio::OS_AirLoopHVAC_OutdoorAirSystemFields::ReturnAirStreamNodeName;
    }

    unsigned AirLoopHVACOutdoorAirSystem_Impl::outdoorAirPort() const {
      return openstudio::OS_AirLoopHVAC_OutdoorAirSystemFields::OutdoorAirStreamNodeName;
    }

    unsigned AirLoopHVACOutdoorAirSystem_Impl::reliefAirPort() const {
      return openstudio::OS_AirLoopHVAC_OutdoorAirSystemFields::ReliefAirStreamNodeName;
    }

    unsigned AirLoopHVACOutdoorAirSystem_Impl::mixedAirPort() const {
      return openstudio::OS_AirLoopHVAC_OutdoorAirSystemFields::MixedAirNodeName;
    }

    OptionalModelObject AirLoopHVACOutdoorAirSystem_Impl::returnAirModelObject() const {
      return this->connectedObject(this->returnAirPort());
    }

    OptionalModelObject AirLoopHVACOutdoorAirSystem_Impl::outdoorAirModelObject() const {
      return this->connectedObject(this->outdoorAirPort());
    }

    OptionalModelObject AirLoopHVACOutdoorAirSystem_Impl::reliefAirModelObject() const {
      return this->connectedObject(this->reliefAirPort());
    }

    OptionalModelObject AirLoopHVACOutdoorAirSystem_Impl::mixedAirModelObject() const {
      return this->connectedObject(this->mixedAirPort());
    }

    ControllerOutdoorAir AirLoopHVACOutdoorAirSystem_Impl::getControllerOutdoorAir() const {
      return this->getTarget(openstudio::OS_AirLoopHVAC_OutdoorAirSystemFields::ControllerName)->cast<ControllerOutdoorAir>();
    }

    bool AirLoopHVACOutdoorAirSystem_Impl::setControllerOutdoorAir(const ControllerOutdoorAir& controllerOutdoorAir) {
      return this->setPointer(openstudio::OS_AirLoopHVAC_OutdoorAirSystemFields::ControllerName, controllerOutdoorAir.handle());
    }

    ModelObject AirLoopHVACOutdoorAirSystem_Impl::clone(Model model) const {
      auto oaclone = ModelObject_Impl::clone(model).cast<AirLoopHVACOutdoorAirSystem>();

      // Clone OA controller
      auto controllerOutdoorAir = getControllerOutdoorAir().clone(model).cast<ControllerOutdoorAir>();
      oaclone.setControllerOutdoorAir(controllerOutdoorAir);

      oaclone.setString(oaclone.mixedAirPort(), "");
      oaclone.setString(oaclone.returnAirPort(), "");
      oaclone.setString(oaclone.outdoorAirPort(), "");
      oaclone.setString(oaclone.reliefAirPort(), "");

      Node oaNodeClone(model);
      model.connect(oaNodeClone, oaNodeClone.outletPort(), oaclone, oaclone.outdoorAirPort());

      Node reliefNodeClone(model);
      model.connect(oaclone, oaclone.reliefAirPort(), reliefNodeClone, reliefNodeClone.inletPort());

      // Clone oa stream comps
      // Note that in the case of AirToAirComponent, this also does stuff to the relief side!
      auto oaComps = oaComponents();
      std::reverse(oaComps.begin(), oaComps.end());
      std::vector<Node> oaNodes;

      for (const auto& comp : oaComps) {
        if (comp.iddObjectType() == Node::iddObjectType()) {
          oaNodes.push_back(comp.cast<Node>());
        } else {
          auto compClone = comp.clone(model).cast<HVACComponent>();
          compClone.addToNode(oaNodeClone);
        }
      }

      auto oaNodeClones = subsetCastVector<Node>(oaclone.oaComponents());
      std::reverse(oaNodeClones.begin(), oaNodeClones.end());

      if (oaNodes.size() == oaNodeClones.size()) {
        for (size_t i = 0; i < oaNodes.size(); ++i) {
          const auto node = oaNodes[i];
          auto cloneNode = oaNodeClones[i];

          auto spms = node.setpointManagers();
          for (const auto& spm : spms) {
            auto spmclone = spm.clone(model).cast<SetpointManager>();
            spmclone.addToNode(cloneNode);
          }
        }
      }

      // Clone relief stream comps

      auto reliefComps = reliefComponents();

      auto reliefAirToAirComps = subsetCastVector<AirToAirComponent>(reliefComps);

      std::vector<Node> reliefNodes;

      if (reliefAirToAirComps.empty()) {
        // Business as usual, but this is faster this way as we keep the same node to connect to throughout
        std::vector<Node> reliefNodes;

        for (const auto& comp : reliefComps) {
          if (comp.iddObjectType() == Node::iddObjectType()) {
            reliefNodes.push_back(comp.cast<Node>());
          } else {
            auto compClone = comp.clone(model).cast<HVACComponent>();
            compClone.addToNode(reliefNodeClone);
          }
        }
      } else {
        auto targetNode = oaclone.reliefAirModelObject()->cast<Node>();

        for (const auto& comp : reliefComps) {
          if (comp.iddObjectType() == Node::iddObjectType()) {
            reliefNodes.push_back(comp.cast<Node>());
          } else if (comp.optionalCast<AirToAirComponent>()) {
            auto reliefCloneMo_ = targetNode.outletModelObject();
            OS_ASSERT(reliefCloneMo_);
            auto reliefCloneAirToAir_ = reliefCloneMo_->optionalCast<AirToAirComponent>();
            OS_ASSERT(reliefCloneAirToAir_);
            auto mo_ = reliefCloneAirToAir_->secondaryAirOutletModelObject();
            OS_ASSERT(mo_);
            targetNode = mo_->cast<Node>();
          } else {
            auto compClone = comp.clone(model).cast<HVACComponent>();
            compClone.addToNode(targetNode);
            auto [ok, mo_] = getOutletNodeForComponent(compClone);
            if (!ok) {
              LOG_AND_THROW("For " << briefDescription() << ", unexpected reliefComponent found: " << compClone.briefDescription());
            }
            OS_ASSERT(mo_);
            targetNode = mo_->cast<Node>();
          }
        }
      }

      auto reliefNodeClones = subsetCastVector<Node>(oaclone.reliefComponents());

      if (reliefNodes.size() == reliefNodeClones.size()) {
        for (size_t i = 0; i < reliefNodes.size(); ++i) {
          const auto node = reliefNodes[i];
          auto cloneNode = reliefNodeClones[i];

          auto spms = node.setpointManagers();
          for (const auto& spm : spms) {
            auto spmclone = spm.clone(model).cast<SetpointManager>();
            spmclone.addToNode(cloneNode);
          }
        }
      }

      return std::move(oaclone);
    }

    std::vector<IdfObject> AirLoopHVACOutdoorAirSystem_Impl::remove() {
      // Handle h = this->handle();

      ModelObjectVector modelObjects;
      ModelObjectVector::iterator it;
      modelObjects = this->components();
      for (it = modelObjects.begin(); it != modelObjects.end(); ++it) {
        if (OptionalHVACComponent comp = it->optionalCast<HVACComponent>()) {
          if (!it->handle().isNull()) {
            comp->disconnect();
            comp->remove();
          }
        }
      }

      //reliefAirModelObject()->remove();
      //outdoorAirModelObject()->remove();
      model().disconnect(getObject<ModelObject>(), outdoorAirPort());
      model().disconnect(getObject<ModelObject>(), reliefAirPort());

      if (auto airLoop_ = this->airLoopHVAC()) {
        OptionalNode targetModelObject;
        OptionalNode sourceModelObject;
        targetModelObject = this->mixedAirModelObject()->optionalCast<Node>();
        sourceModelObject = this->returnAirModelObject()->optionalCast<Node>();
        // OptionalUnsigned targetPort = this->connectedObjectPort(openstudio::OS_AirLoopHVAC_OutdoorAirSystemFields::MixedAirNodeName);
        OptionalUnsigned sourcePort = this->connectedObjectPort(openstudio::OS_AirLoopHVAC_OutdoorAirSystemFields::ReturnAirStreamNodeName);

        OptionalModelObject target2ModelObject = targetModelObject->outletModelObject();
        OptionalModelObject source2ModelObject = sourceModelObject->inletModelObject();
        OptionalUnsigned target2Port = targetModelObject->connectedObjectPort(targetModelObject->outletPort());
        OptionalUnsigned source2Port = sourceModelObject->connectedObjectPort(sourceModelObject->inletPort());

        OptionalNode supplyInletNode = airLoop_->supplyInletNode();
        OptionalNode supplyOutletNode = OptionalNode(airLoop_->supplyOutletNodes().front());

        model().disconnect(getObject<ModelObject>(), returnAirPort());
        model().disconnect(getObject<ModelObject>(), mixedAirPort());

        // If the component is stuck directly between the inlet and outlet node.
        if (supplyInletNode->handle() == sourceModelObject->handle() && supplyOutletNode->handle() == targetModelObject->handle()) {
          this->model().connect(supplyInletNode.get(), supplyInletNode->outletPort(), supplyOutletNode.get(), supplyOutletNode->inletPort());
        }
        // If the component is at the very end of the supply path, but there is another component
        // preceding this one.
        else if (supplyOutletNode->handle() == targetModelObject->handle()) {
          sourceModelObject->remove();
          this->model().connect(source2ModelObject.get(), source2Port.get(), supplyOutletNode.get(), supplyOutletNode->inletPort());
        }
        // else remove the component and the outlet node
        else {
          targetModelObject->remove();
          this->model().connect(sourceModelObject.get(), sourcePort.get(), target2ModelObject.get(), target2Port.get());
        }
      }

      getControllerOutdoorAir().remove();

      return ModelObject_Impl::remove();  // NOLINT(bugprone-parent-virtual-call)
    }

    std::vector<HVACComponent> AirLoopHVACOutdoorAirSystem_Impl::edges(const boost::optional<HVACComponent>& /*prev*/) {
      std::vector<HVACComponent> edges;
      if (auto edgeModelObject = this->mixedAirModelObject()) {
        if (auto edgeObject = edgeModelObject->optionalCast<HVACComponent>()) {
          edges.push_back(*edgeObject);
        }
      }
      return edges;
    }

    std::vector<ModelObject> AirLoopHVACOutdoorAirSystem_Impl::oaComponents(openstudio::IddObjectType type) const {
      std::vector<ModelObject> modelObjects;
      modelObjects.reserve(10);  // Abritrary, but it wouldn't be common to have more objects than that

      OptionalModelObject modelObject = this->outdoorAirModelObject();

      while (modelObject) {
        if (auto comp_ = modelObject->optionalCast<StraightComponent>()) {
          modelObject = comp_->inletModelObject();
          modelObjects.insert(modelObjects.begin(), std::move(*comp_));
        } else if (auto comp_ = modelObject->optionalCast<AirToAirComponent>()) {
          modelObject = comp_->primaryAirInletModelObject();
          modelObjects.insert(modelObjects.begin(), std::move(*comp_));
        } else if (auto comp_ = modelObject->optionalCast<WaterToAirComponent>()) {
          modelObject = comp_->airInletModelObject();
          modelObjects.insert(modelObjects.begin(), std::move(*comp_));
        } else if (auto comp_ = modelObject->optionalCast<ZoneHVACComponent>()) {
          // For AirLoopHVACUnitarySystem and ZoneHVACTerminalUnitVariableRefrigerantFlow
          modelObject = comp_->inletNode();
          modelObjects.insert(modelObjects.begin(), std::move(*comp_));
        } else {
          LOG_AND_THROW("For " << briefDescription() << ", unexpected oaComponent found: " << modelObject->briefDescription());
        }
      }
      if (type != IddObjectType::Catchall) {
        modelObjects.erase(
          std::remove_if(modelObjects.begin(), modelObjects.end(), [&](const auto& mo) -> bool { return mo.iddObjectType() != type; }),
          modelObjects.end());
      }
      return modelObjects;
    }

    std::vector<ModelObject> AirLoopHVACOutdoorAirSystem_Impl::reliefComponents(openstudio::IddObjectType type) const {
      std::vector<ModelObject> modelObjects;
      modelObjects.reserve(10);  // Abritrary, but it wouldn't be common to have more objects than that

      OptionalModelObject modelObject = this->reliefAirModelObject();

      while (modelObject) {
        auto [ok, mo_] = getOutletNodeForComponent(*modelObject);
        if (!ok) {
          LOG_AND_THROW("For " << briefDescription() << ", unexpected reliefComponent found: " << modelObject->briefDescription());
        }
        modelObjects.emplace_back(std::move(*modelObject));
        modelObject = mo_;
      }

      if (type != IddObjectType::Catchall) {
        modelObjects.erase(
          std::remove_if(modelObjects.begin(), modelObjects.end(), [&](const auto& mo) -> bool { return mo.iddObjectType() != type; }),
          modelObjects.end());
      }
      return modelObjects;
    }

    boost::optional<Node> AirLoopHVACOutdoorAirSystem_Impl::outboardOANode() const {
      auto modelObjects = this->oaComponents();

      if (!modelObjects.empty()) {
        return modelObjects.front().optionalCast<Node>();
      }

      return boost::none;
    }

    boost::optional<Node> AirLoopHVACOutdoorAirSystem_Impl::outboardReliefNode() const {
      auto modelObjects = this->reliefComponents();

      if (!modelObjects.empty()) {
        return modelObjects.back().optionalCast<Node>();
      }

      return boost::none;
    }

    boost::optional<AirLoopHVAC> AirLoopHVACOutdoorAirSystem_Impl::airLoopHVAC() const {

      if (m_airLoopHVAC) {  // HVACComponent_Impl caches
        return m_airLoopHVAC;
      }

      for (auto& airLoop : this->model().getConcreteModelObjects<AirLoopHVAC>()) {
        if (airLoop.supplyComponent(this->handle())) {
          m_airLoopHVAC = airLoop;
          return airLoop;
        }
      }

      return boost::none;
    }

    std::vector<ModelObject> AirLoopHVACOutdoorAirSystem_Impl::components(openstudio::IddObjectType type) const {
      return openstudio::concat<ModelObject>(this->oaComponents(type), this->reliefComponents(type));
    }

    boost::optional<ModelObject> AirLoopHVACOutdoorAirSystem_Impl::component(openstudio::Handle handle) const {
      auto comps = components();

      auto it = std::find_if(comps.begin(), comps.end(), [&](const ModelObject& comp) { return (comp.handle() == handle); });
      if (it != comps.end()) {
        return *it;
      }

      return boost::none;
    }

    boost::optional<ModelObject> AirLoopHVACOutdoorAirSystem_Impl::oaComponent(openstudio::Handle handle) const {
      auto comps = oaComponents();

      auto it = std::find_if(comps.begin(), comps.end(), [&](const ModelObject& comp) { return (comp.handle() == handle); });
      if (it != comps.end()) {
        return *it;
      }

      return boost::none;
    }

    boost::optional<ModelObject> AirLoopHVACOutdoorAirSystem_Impl::reliefComponent(openstudio::Handle handle) const {
      auto comps = reliefComponents();

      auto it = std::find_if(comps.begin(), comps.end(), [&](const ModelObject& comp) { return (comp.handle() == handle); });
      if (it != comps.end()) {
        return *it;
      }

      return boost::none;
    }

    bool AirLoopHVACOutdoorAirSystem_Impl::addToNode(Node& node) {
      Model _model = node.model();
      auto thisModelObject = getObject<ModelObject>();

      auto airLoop_ = node.airLoopHVAC();

      if (!airLoop_) {
        return false;
      }

      if (!airLoop_->supplyComponents(this->iddObjectType()).empty()) {
        LOG(Warn, "AirLoopHVAC " << airLoop_->nameString() << " already has an AirLoopHVACOutdoorAirSystem assigned.");
        return false;
      }

      if (!airLoop_->supplyComponent(node.handle())) {
        return false;
      }

      if (node == airLoop_->supplyOutletNodes().front() && node.inletModelObject().get() == airLoop_->supplyInletNode()) {
        const unsigned oldOutletPort = node.connectedObjectPort(node.inletPort()).get();
        const unsigned oldInletPort = node.inletPort();
        ModelObject oldSourceModelObject = node.connectedObject(node.inletPort()).get();
        ModelObject oldTargetModelObject = node;

        _model.connect(std::move(oldSourceModelObject), oldOutletPort, thisModelObject, returnAirPort());
        _model.connect(std::move(thisModelObject), mixedAirPort(), std::move(oldTargetModelObject), oldInletPort);
        return true;
      }

      if (node == airLoop_->supplyInletNode() && node.outletModelObject().get() == airLoop_->supplyOutletNodes().front()) {
        const unsigned oldOutletPort = node.outletPort();
        const unsigned oldInletPort = node.connectedObjectPort(node.outletPort()).get();
        ModelObject oldSourceModelObject = node;
        ModelObject oldTargetModelObject = node.outletModelObject().get();

        _model.connect(std::move(oldSourceModelObject), oldOutletPort, thisModelObject, returnAirPort());
        _model.connect(std::move(thisModelObject), mixedAirPort(), std::move(oldTargetModelObject), oldInletPort);
        return true;
      }

      if (node == airLoop_->supplyOutletNodes().front()) {
        const unsigned oldOutletPort = node.connectedObjectPort(node.inletPort()).get();
        const unsigned oldInletPort = node.inletPort();
        ModelObject oldSourceModelObject = node.connectedObject(node.inletPort()).get();
        ModelObject oldTargetModelObject = node;

        Node newNode(_model);
        const unsigned newNodeInletPort = newNode.inletPort();
        const unsigned newNodeOutletPort = newNode.outletPort();
        _model.connect(std::move(oldSourceModelObject), oldOutletPort, newNode, newNodeInletPort);
        _model.connect(std::move(newNode), newNodeOutletPort, thisModelObject, returnAirPort());
        _model.connect(std::move(thisModelObject), mixedAirPort(), std::move(oldTargetModelObject), oldInletPort);
        return true;
      }

      {
        const unsigned oldOutletPort = node.outletPort();
        const unsigned oldInletPort = node.connectedObjectPort(node.outletPort()).get();
        ModelObject oldSourceModelObject = node;
        ModelObject oldTargetModelObject = node.connectedObject(node.outletPort()).get();

        Node newNode(_model);
        const unsigned newNodeInletPort = newNode.inletPort();
        const unsigned newNodeOutletPort = newNode.outletPort();
        _model.connect(std::move(oldSourceModelObject), oldOutletPort, thisModelObject, returnAirPort());
        _model.connect(std::move(thisModelObject), mixedAirPort(), newNode, newNodeInletPort);
        _model.connect(std::move(newNode), newNodeOutletPort, std::move(oldTargetModelObject), oldInletPort);
        return true;
      }
    }

    boost::optional<ModelObject> AirLoopHVACOutdoorAirSystem_Impl::controllerOutdoorAirAsModelObject() const {
      OptionalModelObject result = getControllerOutdoorAir();
      return result;
    }

    boost::optional<ModelObject> AirLoopHVACOutdoorAirSystem_Impl::outboardOANodeAsModelObject() const {
      if (auto mo_ = this->outboardOANode()) {
        return std::move(*mo_);
      }
      return boost::none;
    }

    boost::optional<ModelObject> AirLoopHVACOutdoorAirSystem_Impl::outboardReliefNodeAsModelObject() const {
      if (auto mo_ = this->outboardReliefNode()) {
        return std::move(*mo_);
      }
      return boost::none;
    }

    boost::optional<ModelObject> AirLoopHVACOutdoorAirSystem_Impl::airLoopAsModelObject() const {
      if (auto mo_ = this->airLoopHVAC()) {
        return std::move(*mo_);
      }
      return boost::none;
    }

    bool AirLoopHVACOutdoorAirSystem_Impl::setControllerOutdoorAirAsModelObject(const boost::optional<ModelObject>& modelObject) {
      if (modelObject) {
        OptionalControllerOutdoorAir intermediate = modelObject->optionalCast<ControllerOutdoorAir>();
        if (intermediate) {
          setControllerOutdoorAir(*intermediate);
          return true;
        }
      }
      return false;
    }

    AirflowNetworkDistributionNode AirLoopHVACOutdoorAirSystem_Impl::getAirflowNetworkDistributionNode() {
      boost::optional<AirflowNetworkDistributionNode> opt = airflowNetworkDistributionNode();
      if (opt) {
        return opt.get();
      }
      return {model(), handle()};
    }

    boost::optional<AirflowNetworkDistributionNode> AirLoopHVACOutdoorAirSystem_Impl::airflowNetworkDistributionNode() const {
      std::vector<AirflowNetworkDistributionNode> myAFNItems =
        getObject<ModelObject>().getModelObjectSources<AirflowNetworkDistributionNode>(AirflowNetworkDistributionNode::iddObjectType());
      auto count = myAFNItems.size();
      if (count == 1) {
        return myAFNItems[0];
      } else if (count > 1) {
        LOG(Warn, briefDescription() << " has more than one AirflowNetwork DistributionNode attached, returning first.");
        return myAFNItems[0];
      }
      return boost::none;
    }

    boost::optional<AirLoopHVACDedicatedOutdoorAirSystem> AirLoopHVACOutdoorAirSystem_Impl::airLoopHVACDedicatedOutdoorAirSystem() const {
      auto thisOASystem = getObject<AirLoopHVACOutdoorAirSystem>();
      std::vector<AirLoopHVACDedicatedOutdoorAirSystem> doaSystems =
        thisOASystem.getModelObjectSources<AirLoopHVACDedicatedOutdoorAirSystem>(AirLoopHVACDedicatedOutdoorAirSystem::iddObjectType());
      if (doaSystems.empty()) {
        return boost::none;
      } else if (doaSystems.size() == 1) {
        return doaSystems.at(0);
      } else {
        LOG(Error, "More than one AirLoopHVACDedicatedOutdoorAirSystem points to this AirLoopHVACOutdoorAirSystem");
        return boost::none;
      }
    }

    ComponentType AirLoopHVACOutdoorAirSystem_Impl::componentType() const {
      bool has_cooling = false;
      bool has_heating = false;
      for (const auto& comp : subsetCastVector<HVACComponent>(components())) {
        auto compType = comp.componentType();
        if (compType == ComponentType::Cooling) {
          has_cooling = true;
        } else if (compType == ComponentType::Heating) {
          has_heating = true;
        } else if (compType == ComponentType::Both) {
          has_cooling = true;
          has_heating = true;
        }
      }

      if (has_cooling && has_heating) {
        return ComponentType::Both;
      } else if (has_cooling) {
        return ComponentType::Cooling;
      } else if (has_heating) {
        return ComponentType::Heating;
      }
      return ComponentType::None;
    }

    std::vector<FuelType> AirLoopHVACOutdoorAirSystem_Impl::coolingFuelTypes() const {
      std::set<FuelType> result;
      for (const auto& comp : subsetCastVector<HVACComponent>(components())) {
        for (auto& ft : comp.coolingFuelTypes()) {
          result.insert(ft);
        }
      }
      return {result.begin(), result.end()};
    }

    std::vector<FuelType> AirLoopHVACOutdoorAirSystem_Impl::heatingFuelTypes() const {
      std::set<FuelType> result;
      for (const auto& comp : subsetCastVector<HVACComponent>(components())) {
        for (auto& ft : comp.heatingFuelTypes()) {
          result.insert(ft);
        }
      }
      return {result.begin(), result.end()};
    }

    std::vector<AppGFuelType> AirLoopHVACOutdoorAirSystem_Impl::appGHeatingFuelTypes() const {
      std::set<AppGFuelType> result;
      for (const auto& comp : subsetCastVector<HVACComponent>(components())) {
        for (auto& ft : comp.appGHeatingFuelTypes()) {
          result.insert(ft);
        }
      }
      return {result.begin(), result.end()};
    }

  }  // namespace detail

  // create a new AirLoopHVACOutdoorAirSystem object in the model's workspace
  AirLoopHVACOutdoorAirSystem::AirLoopHVACOutdoorAirSystem(Model& model, const ControllerOutdoorAir& controller)
    : HVACComponent(AirLoopHVACOutdoorAirSystem::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::AirLoopHVACOutdoorAirSystem_Impl>());

    setControllerOutdoorAir(controller);

    Node oaNode(model);
    oaNode.setName("Outboard OA Node");
    model.connect(oaNode, oaNode.outletPort(), *this, outdoorAirPort());

    Node reliefNode(model);
    reliefNode.setName("Relief Node");
    model.connect(*this, reliefAirPort(), reliefNode, reliefNode.inletPort());
  }

  AirLoopHVACOutdoorAirSystem::AirLoopHVACOutdoorAirSystem(Model& model) : HVACComponent(AirLoopHVACOutdoorAirSystem::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::AirLoopHVACOutdoorAirSystem_Impl>());

    ControllerOutdoorAir controller(model);
    setControllerOutdoorAir(controller);

    Node oaNode(model);
    oaNode.setName("Outboard OA Node");
    model.connect(oaNode, oaNode.outletPort(), *this, outdoorAirPort());

    Node reliefNode(model);
    reliefNode.setName("Relief Node");
    model.connect(*this, reliefAirPort(), reliefNode, reliefNode.inletPort());
  }

  AirLoopHVACOutdoorAirSystem::AirLoopHVACOutdoorAirSystem(std::shared_ptr<detail::AirLoopHVACOutdoorAirSystem_Impl> impl)
    : HVACComponent(std::move(impl)) {}

  unsigned AirLoopHVACOutdoorAirSystem::returnAirPort() const {
    return getImpl<detail::AirLoopHVACOutdoorAirSystem_Impl>()->returnAirPort();
  }

  unsigned AirLoopHVACOutdoorAirSystem::outdoorAirPort() const {
    return getImpl<detail::AirLoopHVACOutdoorAirSystem_Impl>()->outdoorAirPort();
  }

  unsigned AirLoopHVACOutdoorAirSystem::reliefAirPort() const {
    return getImpl<detail::AirLoopHVACOutdoorAirSystem_Impl>()->reliefAirPort();
  }

  unsigned AirLoopHVACOutdoorAirSystem::mixedAirPort() const {
    return getImpl<detail::AirLoopHVACOutdoorAirSystem_Impl>()->mixedAirPort();
  }

  OptionalModelObject AirLoopHVACOutdoorAirSystem::returnAirModelObject() {
    return getImpl<detail::AirLoopHVACOutdoorAirSystem_Impl>()->returnAirModelObject();
  }

  OptionalModelObject AirLoopHVACOutdoorAirSystem::outdoorAirModelObject() {
    return getImpl<detail::AirLoopHVACOutdoorAirSystem_Impl>()->outdoorAirModelObject();
  }

  OptionalModelObject AirLoopHVACOutdoorAirSystem::reliefAirModelObject() {
    return getImpl<detail::AirLoopHVACOutdoorAirSystem_Impl>()->reliefAirModelObject();
  }

  OptionalModelObject AirLoopHVACOutdoorAirSystem::mixedAirModelObject() {
    return getImpl<detail::AirLoopHVACOutdoorAirSystem_Impl>()->mixedAirModelObject();
  }

  ControllerOutdoorAir AirLoopHVACOutdoorAirSystem::getControllerOutdoorAir() const {
    return getImpl<detail::AirLoopHVACOutdoorAirSystem_Impl>()->getControllerOutdoorAir();
  }

  bool AirLoopHVACOutdoorAirSystem::setControllerOutdoorAir(const ControllerOutdoorAir& controllerOutdoorAir) {
    return getImpl<detail::AirLoopHVACOutdoorAirSystem_Impl>()->setControllerOutdoorAir(controllerOutdoorAir);
  }

  boost::optional<Node> AirLoopHVACOutdoorAirSystem::outboardOANode() const {
    return getImpl<detail::AirLoopHVACOutdoorAirSystem_Impl>()->outboardOANode();
  }

  boost::optional<Node> AirLoopHVACOutdoorAirSystem::outboardReliefNode() const {
    return getImpl<detail::AirLoopHVACOutdoorAirSystem_Impl>()->outboardReliefNode();
  }

  std::vector<ModelObject> AirLoopHVACOutdoorAirSystem::oaComponents(openstudio::IddObjectType type) const {
    return getImpl<detail::AirLoopHVACOutdoorAirSystem_Impl>()->oaComponents(type);
  }

  std::vector<ModelObject> AirLoopHVACOutdoorAirSystem::reliefComponents(openstudio::IddObjectType type) const {
    return getImpl<detail::AirLoopHVACOutdoorAirSystem_Impl>()->reliefComponents(type);
  }

  std::vector<ModelObject> AirLoopHVACOutdoorAirSystem::components(openstudio::IddObjectType type) const {
    return getImpl<detail::AirLoopHVACOutdoorAirSystem_Impl>()->components(type);
  }

  boost::optional<ModelObject> AirLoopHVACOutdoorAirSystem::component(openstudio::Handle handle) const {
    return getImpl<detail::AirLoopHVACOutdoorAirSystem_Impl>()->component(handle);
  }

  boost::optional<ModelObject> AirLoopHVACOutdoorAirSystem::oaComponent(openstudio::Handle handle) const {
    return getImpl<detail::AirLoopHVACOutdoorAirSystem_Impl>()->oaComponent(handle);
  }

  boost::optional<ModelObject> AirLoopHVACOutdoorAirSystem::reliefComponent(openstudio::Handle handle) const {
    return getImpl<detail::AirLoopHVACOutdoorAirSystem_Impl>()->reliefComponent(handle);
  }

  boost::optional<AirLoopHVAC> AirLoopHVACOutdoorAirSystem::airLoop() const {
    return getImpl<detail::AirLoopHVACOutdoorAirSystem_Impl>()->airLoopHVAC();
  }

  IddObjectType AirLoopHVACOutdoorAirSystem::iddObjectType() {
    return {IddObjectType::OS_AirLoopHVAC_OutdoorAirSystem};
  }

  AirflowNetworkDistributionNode AirLoopHVACOutdoorAirSystem::getAirflowNetworkDistributionNode() {
    return getImpl<detail::AirLoopHVACOutdoorAirSystem_Impl>()->getAirflowNetworkDistributionNode();
  }

  boost::optional<AirflowNetworkDistributionNode> AirLoopHVACOutdoorAirSystem::airflowNetworkDistributionNode() const {
    return getImpl<detail::AirLoopHVACOutdoorAirSystem_Impl>()->airflowNetworkDistributionNode();
  }

  boost::optional<AirLoopHVACDedicatedOutdoorAirSystem> AirLoopHVACOutdoorAirSystem::airLoopHVACDedicatedOutdoorAirSystem() const {
    return getImpl<detail::AirLoopHVACOutdoorAirSystem_Impl>()->airLoopHVACDedicatedOutdoorAirSystem();
  }

}  // namespace model
}  // namespace openstudio
