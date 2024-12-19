/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "PlantLoop.hpp"
#include "PlantLoop_Impl.hpp"
#include "SizingPlant.hpp"
#include "SizingPlant_Impl.hpp"
#include "Model.hpp"
#include "Node.hpp"
#include "Node_Impl.hpp"
#include "ConnectorMixer.hpp"
#include "ConnectorMixer_Impl.hpp"
#include "ConnectorSplitter.hpp"
#include "ConnectorSplitter_Impl.hpp"
#include "Splitter.hpp"
#include "Splitter_Impl.hpp"
#include "StraightComponent.hpp"
#include "StraightComponent_Impl.hpp"
#include "WaterToAirComponent.hpp"
#include "WaterToAirComponent_Impl.hpp"
#include "WaterToWaterComponent.hpp"
#include "WaterToWaterComponent_Impl.hpp"
#include "CoilCoolingWater.hpp"
#include "CoilCoolingWater_Impl.hpp"
#include "CoilHeatingWater.hpp"
#include "CoilHeatingWater_Impl.hpp"
#include "ControllerWaterCoil.hpp"
#include "ControllerWaterCoil_Impl.hpp"
#include "PlantEquipmentOperationScheme.hpp"
#include "PlantEquipmentOperationScheme_Impl.hpp"
#include "PlantEquipmentOperationHeatingLoad.hpp"
#include "PlantEquipmentOperationHeatingLoad_Impl.hpp"
#include "PlantEquipmentOperationCoolingLoad.hpp"
#include "PlantEquipmentOperationCoolingLoad_Impl.hpp"
#include "ScheduleTypeRegistry.hpp"
#include "Schedule.hpp"
#include "Schedule_Impl.hpp"
#include "AvailabilityManagerAssignmentList.hpp"
#include "AvailabilityManagerAssignmentList_Impl.hpp"
#include "AvailabilityManager.hpp"
#include "AvailabilityManager_Impl.hpp"
#include "SetpointManager.hpp"
#include "SetpointManager_Impl.hpp"
#include "../utilities/core/Assert.hpp"
#include <utilities/idd/OS_PlantLoop_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/IddFactory.hxx>
#include <utilities/idd/OS_AvailabilityManagerAssignmentList_FieldEnums.hxx>

namespace openstudio {

namespace model {

  namespace detail {

    PlantLoop_Impl::PlantLoop_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle) : Loop_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == PlantLoop::iddObjectType());
    }

    PlantLoop_Impl::PlantLoop_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle)
      : Loop_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == PlantLoop::iddObjectType());
    }

    PlantLoop_Impl::PlantLoop_Impl(const PlantLoop_Impl& other, Model_Impl* model, bool keepHandle) : Loop_Impl(other, model, keepHandle) {}

    const std::vector<std::string>& PlantLoop_Impl::outputVariableNames() const {
      static const std::vector<std::string> result{"Plant Supply Side Cooling Demand Rate",
                                                   "Plant Supply Side Heating Demand Rate",
                                                   "Plant Supply Side Inlet Mass Flow Rate",
                                                   "Plant Supply Side Inlet Temperature",
                                                   "Plant Supply Side Outlet Temperature",
                                                   "Plant Supply Side Not Distributed Demand Rate",
                                                   "Plant Supply Side Unmet Demand Rate",
                                                   "Plant Solver Sub Iteration Count",
                                                   "Plant Solver Half Loop Calls Count",
                                                   "Debug Plant Loop Bypass Fraction",
                                                   "Debug Plant Last Simulated Loop Side",
                                                   "Plant Common Pipe Mass Flow Rate",
                                                   "Plant Common Pipe Temperature",
                                                   "Plant Common Pipe Flow Direction Status",
                                                   "Plant Common Pipe Primary Mass Flow Rate",
                                                   "Plant Common Pipe Secondary Mass Flow Rate",
                                                   "Primary Side Common Pipe Flow Direction",
                                                   "Secondary Side Common Pipe Flow Direction",
                                                   "Plant Common Pipe Primary to Secondary Mass Flow Rate",
                                                   "Plant Common Pipe Secondary to Primary Mass Flow Rate",
                                                   "Plant System Cycle On Off Status",
                                                   "Plant Demand Side Loop Pressure Difference",
                                                   "Plant Supply Side Loop Pressure Difference",
                                                   "Plant Loop Pressure Difference"};
      return result;
    }

    std::vector<openstudio::IdfObject> PlantLoop_Impl::remove() {
      sizingPlant().remove();

      ModelObjectVector modelObjects;
      ModelObjectVector::iterator it;

      availabilityManagerAssignmentList().remove();

      modelObjects = supplyComponents();
      for (it = modelObjects.begin(); it != modelObjects.end(); ++it) {
        if (boost::optional<WaterToWaterComponent> comp = it->optionalCast<WaterToWaterComponent>()) {
          comp->removeFromPlantLoop();
          if (!comp->secondaryPlantLoop()) {
            comp->remove();
          }
        }
      }

      modelObjects = demandComponents();
      for (it = modelObjects.begin(); it != modelObjects.end(); ++it) {
        if (boost::optional<WaterToWaterComponent> comp = it->optionalCast<WaterToWaterComponent>()) {
          comp->removeFromSecondaryPlantLoop();
          if (!comp->plantLoop()) {
            comp->remove();
          }
        } else if (boost::optional<WaterToAirComponent> comp = it->optionalCast<WaterToAirComponent>()) {
          comp->removeFromPlantLoop();
          if (!comp->airLoopHVAC()) {
            comp->remove();
          }
        }
      }

      modelObjects = components();

      std::vector<openstudio::IdfObject> idfObjects = ModelObject_Impl::remove();

      for (it = modelObjects.begin(); it != modelObjects.end(); ++it) {
        if (boost::optional<HVACComponent> comp = it->optionalCast<HVACComponent>()) {
          comp->disconnect();
        }
        it->remove();
      }

      return idfObjects;
    }

    IddObjectType PlantLoop_Impl::iddObjectType() const {
      return PlantLoop::iddObjectType();
    }

    ModelObject PlantLoop_Impl::clone(Model model) const {
      auto plantLoopClone = Loop_Impl::clone(model).cast<PlantLoop>();

      plantLoopClone.setString(supplyInletPort(), "");
      plantLoopClone.setString(supplyOutletPort(), "");
      plantLoopClone.setString(demandInletPort(), "");
      plantLoopClone.setString(demandOutletPort(), "");

      // Sizing:Plant was already cloned because it is declared as a child
      // And because it has the setParent method overriden, no need to do anything

      {
        // Perhaps call a clone(Loop loop) instead...
        auto avmListClone = availabilityManagerAssignmentList().clone(model).cast<AvailabilityManagerAssignmentList>();
        avmListClone.setName(plantLoopClone.name().get() + " AvailabilityManagerAssigmentList");
        plantLoopClone.setPointer(OS_PlantLoopFields::AvailabilityManagerListName, avmListClone.handle());
      }

      plantLoopClone.getImpl<detail::PlantLoop_Impl>()->createTopology();

      // Declare vectors we'll use to store the nodes in order to check if we have the same number of nodes
      // and since we store them in the same order, we'll be able to clone the SPMs too
      std::vector<Node> nodes;
      std::vector<Node> nodeClones;

      /*
   *==========================================
   *       S U P P L Y    S I D E
   *==========================================
   */

      auto outputMessagesAndAssert = [](const std::vector<Node>& nodes, const std::vector<Node>& nodeClones, bool isSupplySide, bool isInlet) {
        // Before the assert, we give out useful debugging info, or a Trace
        if (nodes.size() != nodeClones.size()) {
          LOG(Debug, "When cloning with (isSupplySide, isInlet) = (" << isSupplySide << ", " << isInlet << "),"
                                                                     << "found a difference in number of nodes: nodes.size()=" << nodes.size()
                                                                     << ", nodeClones.size()=" << nodeClones.size() << ".\n\nnodes:\n");
          for (size_t i = 0; i < nodes.size(); ++i) {
            LOG(Debug, "i=" << i << ", node=" << nodes[i].name().get());
          }

          LOG(Debug, "\n\nnodeClones:\n");
          for (size_t i = 0; i < nodeClones.size(); ++i) {
            LOG(Debug, "i=" << i << ", nodeClone=" << nodeClones[i].name().get());
          }
        } else {
          LOG(Trace, "When cloning with (isSupplySide, isInlet) = (" << isSupplySide << ", " << isInlet << ")," << "nodes.size()=" << nodes.size()
                                                                     << ", nodeClones.size()=" << nodeClones.size());

          for (size_t i = 0; i < nodes.size(); ++i) {
            LOG(Trace, "i=" << i << "node=" << nodes[i].name().get() << ", nodeClone=" << nodeClones[i].name().get());
          }
        }

        // Assert that we have the same number of nodes
        OS_ASSERT(nodes.size() == nodeClones.size());
      };

      // lambda function to deal with non branch components
      // We capture this (=original plantLoop), model (the target model), the cloned plantLoop,
      // the vectors we need to store nodes, and the lambda function to output messages and assert
      auto handleNonBranchComps = [this, &model, &plantLoopClone, &nodes, &nodeClones, &outputMessagesAndAssert](bool isSupplySide, bool isInlet) {
        std::vector<ModelObject> components;
        boost::optional<Node> targetNode;
        // Stores references to the inlet/outlet modelObjects (Node or Splitter or Mixer) so we can coun't the number of cloned Nodes later
        // to ensure that we end up with the same number
        boost::optional<HVACComponent> inletModelObjectClone;
        boost::optional<HVACComponent> outletModelObjectClone;

        // Initialize the right components and store reference to inlet/outlet objects in the cloned PlantLoop
        if (isInlet) {
          if (isSupplySide) {
            components = this->supplyComponents(this->supplyInletNode(), this->supplySplitter());
            targetNode = plantLoopClone.supplyInletNode();
            inletModelObjectClone = targetNode;
            outletModelObjectClone = plantLoopClone.supplySplitter();
          } else {
            components = this->demandComponents(this->demandInletNode(), this->demandSplitter());
            targetNode = plantLoopClone.demandInletNode();
            inletModelObjectClone = targetNode;
            outletModelObjectClone = plantLoopClone.demandSplitter();
          }

          // We pop the splitter
          components.pop_back();
          // We are going to add them to the InletNode in reverse order so that they end up correctly ordered
          // if components are ordered [A, B, C], and "o" is the inlet Node, that's what we'll do:
          // Loop on [C, B, A]
          // 0: o---C
          // 1: o---B---C
          // 2: o---A---B---C
          std::reverse(components.begin(), components.end());

        } else {
          if (isSupplySide) {
            components = this->supplyComponents(this->supplyMixer(), this->supplyOutletNode());
            targetNode = plantLoopClone.supplyOutletNode();
            inletModelObjectClone = plantLoopClone.supplyMixer();
            outletModelObjectClone = targetNode;
          } else {
            components = this->demandComponents(this->demandMixer(), this->demandOutletNode());
            targetNode = plantLoopClone.demandOutletNode();
            inletModelObjectClone = plantLoopClone.demandMixer();
            outletModelObjectClone = targetNode;
          }
          // We erase the mixer
          components.erase(components.begin());
          // We are going to add them to the outletNode, in the current order, so no need to reverse here
          // if components are ordered [A, B, C], and "o" is the **outlet** Node, that's what we'll do:
          // Loop on [A, B, C]
          // 0: A---o
          // 1: A---B---o
          // 2: A---B---C---o

        }  // END OF Initialization

        // We loop on the components of the original plant loop
        for (auto it = components.begin(); it != components.end(); ++it) {
          ModelObject comp = *it;
          if (comp.iddObjectType() == Node::iddObjectType()) {
            // If a Node, we don't clone, we just push it to the nodes vector
            nodes.push_back(comp.cast<Node>());
          } else {
            // We clone the component and add it to the targetNode we initialized above
            auto compClone = comp.clone(model).cast<HVACComponent>();
            compClone.addToNode(*targetNode);

            // If it's a WaterToWaterComponent, we try to connect it to the other PlantLoop too
            if (boost::optional<WaterToWaterComponent> hvacComp = comp.optionalCast<WaterToWaterComponent>()) {
              if (isSupplySide) {
                // If it's supplySide, we check if the component is on a secondaryPlantLoop
                if (boost::optional<PlantLoop> pl = hvacComp->secondaryPlantLoop()) {
                  // Connect the clone to the secondary plantLoop too on the demand side
                  pl->addDemandBranchForComponent(compClone);
                }
              } else {
                // If demand side, we try to put it on the supply side of the (primary) plantLoop
                if (boost::optional<PlantLoop> pl = hvacComp->plantLoop()) {
                  // Connect the clone to the plantLoop too
                  pl->addSupplyBranchForComponent(compClone);
                }
              }
            }
          }
        }

        // We get the added nodes in the cloned plant loop
        std::vector<Node> nodeClonesHere = subsetCastVector<Node>(plantLoopClone.supplyComponents(*inletModelObjectClone, *outletModelObjectClone));
        // Add the new cloned nodes to the vector
        nodeClones.insert(nodeClones.end(), nodeClonesHere.begin(), nodeClonesHere.end());

        // Output Messages and assert that we still have the same number of nodes between the original plantloop and the cloned one
        outputMessagesAndAssert(nodes, nodeClones, isSupplySide, isInlet);
      };  // END OF LAMBDA

      // Ready to start!

      /**
   * Betwen the supply inlet node and the supply Splitter
   */
      // isSupplySide = true, isInlet = true;
      handleNonBranchComps(true, true);

      /**
   * Betwen the supply Mixer node and the supply Outlet Node
   */
      handleNonBranchComps(true, false);

      /**
   * Betwen the Splitter and Mixer: branches
   */

      Splitter splitter = supplySplitter();
      Mixer mixer = supplyMixer();

      std::vector<ModelObject> splitterOutletObjects = splitter.outletModelObjects();
      // std::vector< std::vector<model::ModelObject> > allBranchComponents;

      // We'll use this vector to manually push the cloned nodes as we get them
      std::vector<Node> nodeClonesHere;

      // If there is actually something interesting between the splitter and the mixer...
      if (!(splitterOutletObjects.front() == mixer)) {
        // We loop on the splitter outlet objects, that gives us the first nodes on each branch there is
        for (ModelObject& mo : splitterOutletObjects) {

          auto comp = mo.optionalCast<model::HVACComponent>();
          OS_ASSERT(comp);

          // We get the components that are on that branch (between the first node of the branch and the mixer)
          std::vector<ModelObject> branchComponents = supplyComponents(comp.get(), mixer);
          // Pop the last component (the mixer)
          branchComponents.pop_back();

          // allBranchComponents.push_back(branchComponents);

          // Note JM 2018-12-17: There's one special case: the branch with the connector node
          // We want to avoid looping on branchComponents otherwise we'll add the connector node to the nodes vector
          // while the nodeClones will not, so it'll produce a diff
          if (branchComponents.size() == 1 && branchComponents[0].optionalCast<Node>()) {
            LOG(Trace, "On the Connector Node Branch, skipping it.");
            continue;
          }

          // Reference to the lastOutletNode
          boost::optional<Node> lastOutletNode;

          // We loop on all the branch components
          for (ModelObject& comp : branchComponents) {
            if (comp.iddObjectType() == Node::iddObjectType()) {
              // We don't clone the nodes, just push them the nodes vector
              nodes.push_back(comp.cast<Node>());
            } else {
              // Otherwise, we clone it
              auto compClone = comp.clone(model).cast<HVACComponent>();

              if (!lastOutletNode) {
                // If lastOutletNode isn't initialized yet, means we deal with the first component
                // so we add a supply branch for this component
                plantLoopClone.addSupplyBranchForComponent(compClone);

                // We are going to push the created inlet Node to the nodeClonesHere vector
                boost::optional<Node> thisInletNode;
                if (auto _c = compClone.optionalCast<StraightComponent>()) {
                  thisInletNode = _c->inletModelObject().get().cast<Node>();
                } else if (auto _c = compClone.optionalCast<WaterToAirComponent>()) {
                  thisInletNode = _c->waterInletModelObject().get().cast<Node>();
                } else if (auto _c = compClone.optionalCast<WaterToWaterComponent>()) {
                  thisInletNode = _c->supplyInletModelObject().get().cast<Node>();
                } else {
                  // Shouldn't get there
                  OS_ASSERT(false);
                }
                if (thisInletNode) {
                  nodeClonesHere.push_back(*thisInletNode);
                }

              } else {
                // Otherwise, we add the component to the lastOutletNode
                compClone.addToNode(lastOutletNode.get());
              }

              // In both cases, we capture the last outlet node
              // In order to push it to the vector of nodeClonesHere
              // And if need be, be able to add the next component to it
              if (auto _c = compClone.optionalCast<StraightComponent>()) {
                lastOutletNode = _c->outletModelObject().get().cast<Node>();
              } else if (auto _c = compClone.optionalCast<WaterToAirComponent>()) {
                lastOutletNode = _c->waterOutletModelObject().get().cast<Node>();
              } else if (auto _c = compClone.optionalCast<WaterToWaterComponent>()) {
                lastOutletNode = _c->supplyOutletModelObject().get().cast<Node>();
              } else {
                // Shouldn't get there
                OS_ASSERT(false);
              }
              nodeClonesHere.push_back(*lastOutletNode);

              // Connect to secondary PlantLoop as needed
              if (boost::optional<WaterToWaterComponent> hvacComp = comp.optionalCast<WaterToWaterComponent>()) {
                if (boost::optional<PlantLoop> pl = hvacComp->secondaryPlantLoop()) {
                  // Connect the clone to the secondary plantLoop too
                  pl->addDemandBranchForComponent(compClone);
                }
              }
            }
          }  // End of loop on all branchComponents
        }  // End of loop splitterOutletModelObjects
      }

      // Add the new cloned nodes
      nodeClones.insert(nodeClones.end(), nodeClonesHere.begin(), nodeClonesHere.end());
      outputMessagesAndAssert(nodes, nodeClones, false, false);

      /*
   *==========================================
   *       D E M A N D    S I D E
   *==========================================
   */
      // Rationale: here we'll handle everything BUT the stuff that's on the demand branches

      /**
   * Betwen the demand inlet node and the supply Splitter
   */

      // isSupplySide = false, isInlet = true;
      handleNonBranchComps(false, true);

      /**
   * Betwen the demand Mixer node and the demand Outlet Node
   */
      handleNonBranchComps(false, false);

      /*
   *==========================================
   *     S E T P O I N T   M A N A G E R S
   *==========================================
   */
      // Do another global check: check that number of nodes match
      OS_ASSERT(subsetCastVector<Node>(supplyComponents()).size() == subsetCastVector<Node>(plantLoopClone.supplyComponents()).size());
      // Check that number of components match
      OS_ASSERT(supplyComponents().size() == plantLoopClone.supplyComponents().size());

      // Clone any SPMs: At this point nodes and nodeClones store ALL the nodes we care about in the same order
      for (size_t i = 0; i < nodes.size(); ++i) {
        const auto node = nodes[i];
        auto cloneNode = nodeClones[i];

        auto spms = node.setpointManagers();
        for (const auto& spm : spms) {
          auto spmclone = spm.clone(model).cast<SetpointManager>();
          spmclone.addToNode(cloneNode);
        }
      }

      return std::move(plantLoopClone);
    }

    unsigned PlantLoop_Impl::supplyInletPort() const {
      return openstudio::OS_PlantLoopFields::PlantSideInletNodeName;
    }

    unsigned PlantLoop_Impl::supplyOutletPort() const {
      return openstudio::OS_PlantLoopFields::PlantSideOutletNodeName;
    }

    unsigned PlantLoop_Impl::demandInletPort() const {
      return openstudio::OS_PlantLoopFields::DemandSideInletNodeName;
    }

    unsigned PlantLoop_Impl::demandOutletPort() const {
      return openstudio::OS_PlantLoopFields::DemandSideOutletNodeName;
    }

    Node PlantLoop_Impl::supplyInletNode() const {
      return connectedObject(supplyInletPort())->optionalCast<Node>().get();
    }

    Node PlantLoop_Impl::supplyOutletNode() const {
      return connectedObject(supplyOutletPort())->optionalCast<Node>().get();
    }

    std::vector<Node> PlantLoop_Impl::supplyOutletNodes() const {
      return std::vector<Node>{supplyOutletNode()};
    }

    Node PlantLoop_Impl::demandInletNode() const {
      return connectedObject(demandInletPort())->optionalCast<Node>().get();
    }

    std::vector<Node> PlantLoop_Impl::demandInletNodes() const {
      return std::vector<Node>{demandInletNode()};
    }

    Node PlantLoop_Impl::demandOutletNode() const {
      return connectedObject(demandOutletPort())->optionalCast<Node>().get();
    }

    bool PlantLoop_Impl::addSupplyBranchForComponent(HVACComponent component) {
      Model _model = this->model();

      if (component.model() != _model) {
        return false;
      }

      Splitter splitter = this->supplySplitter();
      Mixer mixer = this->supplyMixer();

      if (splitter.outletModelObjects().size() == 1u) {
        if (boost::optional<ModelObject> mo = splitter.lastOutletModelObject()) {
          if (boost::optional<Node> node = mo->optionalCast<Node>()) {
            if ((node->outletModelObject().get() == mixer) && (node->inletModelObject().get() == splitter)) {
              return component.addToNode(node.get());
            }
          }
        }
      }

      int nextOutletPort;
      int nextInletPort;

      nextOutletPort = splitter.nextOutletPort();
      nextInletPort = mixer.nextInletPort();

      Node node(_model);

      _model.connect(splitter, nextOutletPort, node, node.inletPort());
      _model.connect(node, node.outletPort(), mixer, nextInletPort);

      if (component.addToNode(node)) {
        return true;
      } else {
        removeSupplyBranchWithComponent(node);

        return false;
      }

      return false;
    }

    bool PlantLoop_Impl::removeSupplyBranchWithComponent(HVACComponent component) {
      if (!supplyComponent(component.handle())) {
        return false;
      }

      return removeBranchWithComponent(component, supplySplitter(), supplyMixer(), true);
    }

    bool PlantLoop_Impl::addDemandBranchForComponent(HVACComponent component, bool tertiary) {
      Model _model = this->model();

      if (component.model() != _model) {
        return false;
      }

      Splitter splitter = this->demandSplitter();
      Mixer mixer = this->demandMixer();

      if (splitter.outletModelObjects().size() == 1u) {
        if (boost::optional<ModelObject> mo = splitter.lastOutletModelObject()) {
          if (boost::optional<Node> node = mo->optionalCast<Node>()) {
            if ((node->outletModelObject().get() == mixer) && (node->inletModelObject().get() == splitter)) {
              if (auto waterToWater = component.optionalCast<WaterToWaterComponent>()) {
                if (tertiary) {
                  return waterToWater->addToTertiaryNode(node.get());
                } else {
                  return waterToWater->addToNode(node.get());
                }
              } else {
                return component.addToNode(node.get());
              }
            }
          }
        }
      }

      int nextOutletPort;
      int nextInletPort;

      nextOutletPort = splitter.nextOutletPort();
      nextInletPort = mixer.nextInletPort();

      Node node(_model);

      _model.connect(splitter, nextOutletPort, node, node.inletPort());
      _model.connect(node, node.outletPort(), mixer, nextInletPort);

      bool result = false;

      if (auto waterToWater = component.optionalCast<WaterToWaterComponent>()) {
        if (tertiary) {
          result = waterToWater->addToTertiaryNode(node);
        } else {
          result = waterToWater->addToNode(node);
        }
      } else {
        result = component.addToNode(node);
      }

      if (result) {
        return true;
      } else {
        _model.disconnect(node, node.outletPort());
        _model.disconnect(node, node.inletPort());
        node.remove();

        return false;
      }

      return false;
    }

    bool PlantLoop_Impl::removeBranchWithComponent(HVACComponent component, Splitter splitter, Mixer mixer, bool isSupplyComponent) {
      Model _model = model();

      std::vector<ModelObject> backwardComponents = components(splitter, component);
      std::vector<ModelObject> forwardComponents = components(component, mixer);

      // Validate that the component is between the demand splitter and mixer
      if (backwardComponents.front().handle() != splitter.handle()) {
        return false;
      }

      if (backwardComponents.back().handle() != component.handle()) {
        return false;
      }

      if (forwardComponents.front().handle() != component.handle()) {
        return false;
      }

      if (forwardComponents.back().handle() != mixer.handle()) {
        return false;
      }

      // Remove the backwardComponents end because we don't want component twice
      backwardComponents.erase(backwardComponents.end() - 1);

      // Combine forwardComponents and backwardComponents for a complete vector of all components on the branch
      std::vector<ModelObject> allComponents(backwardComponents.begin(), backwardComponents.end());
      allComponents.insert(allComponents.end(), forwardComponents.begin(), forwardComponents.end());

      // Remove the splitter and mixer from the vector
      allComponents.erase(allComponents.begin());
      allComponents.erase(allComponents.end() - 1);

      for (auto& elem : allComponents) {
        if (!elem.optionalCast<Node>()) {
          if (boost::optional<StraightComponent> comp = elem.optionalCast<StraightComponent>()) {
            elem.remove();
          } else if (boost::optional<WaterToAirComponent> comp = elem.optionalCast<WaterToAirComponent>()) {
            comp->removeFromPlantLoop();

            if (!comp->airLoopHVAC()) {
              comp->remove();
            }
          } else if (boost::optional<WaterToWaterComponent> comp = elem.optionalCast<WaterToWaterComponent>()) {
            if (isSupplyComponent) {
              comp->removeFromPlantLoop();
            } else {
              comp->removeFromSecondaryPlantLoop();
            }

            if ((!comp->plantLoop()) && (!comp->secondaryPlantLoop())) {
              comp->remove();
            }
          }
        }
      }

      return true;
    }

    bool PlantLoop_Impl::removeDemandBranchWithComponent(HVACComponent component) {
      if (!demandComponent(component.handle())) {
        return false;
      }

      return removeBranchWithComponent(component, demandSplitter(), demandMixer(), false);
    }

    bool PlantLoop_Impl::isDemandBranchEmpty() {
      Splitter splitter = this->demandSplitter();

      if (splitter.lastOutletModelObject()->optionalCast<Mixer>()) {
        return true;
      } else {
        return false;
      }
    }

    Mixer PlantLoop_Impl::supplyMixer() const {
      auto result = getObject<ModelObject>().getModelObjectTarget<Mixer>(OS_PlantLoopFields::SupplyMixerName);
      if (result) {
        return result.get();
      }
      return supplyComponents(IddObjectType::OS_Connector_Mixer).front().cast<Mixer>();
    }

    bool PlantLoop_Impl::setSupplyMixer(Mixer const& mixer) {
      auto result = setPointer(OS_PlantLoopFields::SupplyMixerName, mixer.handle());
      OS_ASSERT(result);
      return result;
    }

    Splitter PlantLoop_Impl::supplySplitter() const {
      auto result = getObject<ModelObject>().getModelObjectTarget<Splitter>(OS_PlantLoopFields::SupplySplitterName);
      if (result) {
        return result.get();
      }
      return supplyComponents(IddObjectType::OS_Connector_Splitter).front().cast<Splitter>();
    }

    bool PlantLoop_Impl::setSupplySplitter(Splitter const& splitter) {
      auto result = setPointer(OS_PlantLoopFields::SupplySplitterName, splitter.handle());
      OS_ASSERT(result);
      return result;
    }

    Mixer PlantLoop_Impl::demandMixer() const {
      auto result = getObject<ModelObject>().getModelObjectTarget<Mixer>(OS_PlantLoopFields::DemandMixerName);
      if (result) {
        return result.get();
      }
      return demandComponents(IddObjectType::OS_Connector_Mixer).front().cast<Mixer>();
    }

    bool PlantLoop_Impl::setDemandMixer(Mixer const& mixer) {
      auto result = setPointer(OS_PlantLoopFields::DemandMixerName, mixer.handle());
      OS_ASSERT(result);
      return result;
    }

    Splitter PlantLoop_Impl::demandSplitter() const {
      auto result = getObject<ModelObject>().getModelObjectTarget<Splitter>(OS_PlantLoopFields::DemandSplitterName);
      if (result) {
        return result.get();
      }
      return demandComponents(IddObjectType::OS_Connector_Splitter).front().cast<Splitter>();
    }

    bool PlantLoop_Impl::setDemandSplitter(Splitter const& splitter) {
      auto result = setPointer(OS_PlantLoopFields::DemandSplitterName, splitter.handle());
      OS_ASSERT(result);
      return result;
    }

    std::string PlantLoop_Impl::loadDistributionScheme() {
      auto value = getString(OS_PlantLoopFields::LoadDistributionScheme, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool PlantLoop_Impl::setLoadDistributionScheme(const std::string& scheme) {
      auto thisScheme = scheme;
      if (istringEqual(thisScheme, "Sequential")) {
        thisScheme = "SequentialLoad";
      } else if (istringEqual(thisScheme, "Uniform")) {
        thisScheme = "UniformLoad";
      }
      return setString(OS_PlantLoopFields::LoadDistributionScheme, thisScheme);
    }

    double PlantLoop_Impl::maximumLoopTemperature() {
      return getDouble(OS_PlantLoopFields::MaximumLoopTemperature, true).get();
    }

    bool PlantLoop_Impl::setMaximumLoopTemperature(double value) {
      return setDouble(OS_PlantLoopFields::MaximumLoopTemperature, value);
    }

    double PlantLoop_Impl::minimumLoopTemperature() {
      return getDouble(OS_PlantLoopFields::MinimumLoopTemperature, true).get();
    }

    bool PlantLoop_Impl::setMinimumLoopTemperature(double value) {
      return setDouble(OS_PlantLoopFields::MinimumLoopTemperature, value);
    }

    boost::optional<double> PlantLoop_Impl::maximumLoopFlowRate() {
      return getDouble(OS_PlantLoopFields::MaximumLoopFlowRate, true);
    }

    bool PlantLoop_Impl::setMaximumLoopFlowRate(double value) {
      return setDouble(OS_PlantLoopFields::MaximumLoopFlowRate, value);
    }

    bool PlantLoop_Impl::isMaximumLoopFlowRateAutosized() {
      bool result = false;
      boost::optional<std::string> value = getString(OS_PlantLoopFields::MaximumLoopFlowRate, true);
      if (value) {
        result = openstudio::istringEqual(value.get(), "Autosize");
      }
      return result;
    }

    void PlantLoop_Impl::autosizeMaximumLoopFlowRate() {
      bool result = setString(OS_PlantLoopFields::MaximumLoopFlowRate, "Autosize");
      OS_ASSERT(result);
    }

    boost::optional<double> PlantLoop_Impl::minimumLoopFlowRate() {
      return getDouble(OS_PlantLoopFields::MinimumLoopFlowRate, true);
    }

    bool PlantLoop_Impl::setMinimumLoopFlowRate(double value) {
      return setDouble(OS_PlantLoopFields::MinimumLoopFlowRate, value);
    }

    bool PlantLoop_Impl::isMinimumLoopFlowRateAutosized() {
      bool result = false;
      boost::optional<std::string> value = getString(OS_PlantLoopFields::MinimumLoopFlowRate, true);
      if (value) {
        result = openstudio::istringEqual(value.get(), "Autosize");
      }
      return result;
    }

    void PlantLoop_Impl::autosizeMinimumLoopFlowRate() {
      bool result = setString(OS_PlantLoopFields::MinimumLoopFlowRate, "Autosize");
      OS_ASSERT(result);
    }

    boost::optional<double> PlantLoop_Impl::plantLoopVolume() {
      return getDouble(OS_PlantLoopFields::PlantLoopVolume, true);
    }

    bool PlantLoop_Impl::setPlantLoopVolume(double value) {
      return setDouble(OS_PlantLoopFields::PlantLoopVolume, value);
    }

    bool PlantLoop_Impl::isPlantLoopVolumeAutocalculated() {
      bool result = false;
      boost::optional<std::string> value = getString(OS_PlantLoopFields::PlantLoopVolume, true);
      if (value) {
        result = openstudio::istringEqual(value.get(), "Autocalculate");
      }
      return result;
    }

    void PlantLoop_Impl::autocalculatePlantLoopVolume() {
      bool result = setString(OS_PlantLoopFields::PlantLoopVolume, "Autocalculate");
      OS_ASSERT(result);
    }

    std::string PlantLoop_Impl::fluidType() {
      return getString(OS_PlantLoopFields::FluidType, true).get();
    }

    bool PlantLoop_Impl::setFluidType(const std::string& value) {
      return setString(OS_PlantLoopFields::FluidType, value);
    }

    int PlantLoop_Impl::glycolConcentration() const {
      return getInt(OS_PlantLoopFields::GlycolConcentration, true).get();
    }

    bool PlantLoop_Impl::setGlycolConcentration(int glycolConcentration) {
      return setInt(OS_PlantLoopFields::GlycolConcentration, glycolConcentration);
    }

    Node PlantLoop_Impl::loopTemperatureSetpointNode() {
      boost::optional<Node> node;

      node = getObject<ModelObject>().getModelObjectTarget<Node>(OS_PlantLoopFields::LoopTemperatureSetpointNodeName);

      OS_ASSERT(node);

      return node.get();
    }

    bool PlantLoop_Impl::setLoopTemperatureSetpointNode(Node& node) {
      if (node.model() == this->model()) {
        return setPointer(OS_PlantLoopFields::LoopTemperatureSetpointNodeName, node.handle());
      }
      return false;
    }

    std::vector<ModelObject> PlantLoop_Impl::children() const {
      std::vector<ModelObject> result;

      result.push_back(sizingPlant());

      return result;
    }

    SizingPlant PlantLoop_Impl::sizingPlant() const {
      boost::optional<SizingPlant> sizingPlant;

      std::vector<SizingPlant> sizingObjects;

      sizingObjects = model().getConcreteModelObjects<SizingPlant>();

      for (const auto& sizingObject : sizingObjects) {
        if (sizingObject.plantLoop().handle() == this->handle()) {
          sizingPlant = sizingObject;
        }
      }

      if (sizingPlant) {
        return sizingPlant.get();
      } else {
        LOG_AND_THROW("PlantLoop missing SizingPlant object");
      }
    }

    std::string PlantLoop_Impl::commonPipeSimulation() const {
      boost::optional<std::string> value = getString(OS_PlantLoopFields::CommonPipeSimulation, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool PlantLoop_Impl::isCommonPipeSimulationDefaulted() const {
      return isEmpty(OS_PlantLoopFields::CommonPipeSimulation);
    }

    bool PlantLoop_Impl::setCommonPipeSimulation(const std::string& value) {
      return setString(OS_PlantLoopFields::CommonPipeSimulation, value);
    }

    void PlantLoop_Impl::resetCommonPipeSimulation() {
      bool result = setString(OS_PlantLoopFields::CommonPipeSimulation, "");
      OS_ASSERT(result);
    }

    boost::optional<PlantEquipmentOperationHeatingLoad> PlantLoop_Impl::plantEquipmentOperationHeatingLoad() const {
      return getObject<ModelObject>().getModelObjectTarget<PlantEquipmentOperationHeatingLoad>(
        OS_PlantLoopFields::PlantEquipmentOperationHeatingLoad);
    }

    bool PlantLoop_Impl::setPlantEquipmentOperationHeatingLoad(
      const boost::optional<PlantEquipmentOperationHeatingLoad>& plantEquipmentOperationHeatingLoad) {
      bool result(false);
      if (plantEquipmentOperationHeatingLoad) {
        result = setPointer(OS_PlantLoopFields::PlantEquipmentOperationHeatingLoad, plantEquipmentOperationHeatingLoad.get().handle());
      } else {
        resetPlantEquipmentOperationHeatingLoad();
        result = true;
      }
      return result;
    }

    void PlantLoop_Impl::resetPlantEquipmentOperationHeatingLoad() {
      bool result = setString(OS_PlantLoopFields::PlantEquipmentOperationHeatingLoad, "");
      OS_ASSERT(result);
    }

    boost::optional<PlantEquipmentOperationCoolingLoad> PlantLoop_Impl::plantEquipmentOperationCoolingLoad() const {
      return getObject<ModelObject>().getModelObjectTarget<PlantEquipmentOperationCoolingLoad>(
        OS_PlantLoopFields::PlantEquipmentOperationCoolingLoad);
    }

    bool PlantLoop_Impl::setPlantEquipmentOperationCoolingLoad(
      const boost::optional<PlantEquipmentOperationCoolingLoad>& plantEquipmentOperationCoolingLoad) {
      bool result(false);
      if (plantEquipmentOperationCoolingLoad) {
        result = setPointer(OS_PlantLoopFields::PlantEquipmentOperationCoolingLoad, plantEquipmentOperationCoolingLoad.get().handle());
      } else {
        resetPlantEquipmentOperationCoolingLoad();
        result = true;
      }
      return result;
    }

    void PlantLoop_Impl::resetPlantEquipmentOperationCoolingLoad() {
      bool result = setString(OS_PlantLoopFields::PlantEquipmentOperationCoolingLoad, "");
      OS_ASSERT(result);
    }

    boost::optional<PlantEquipmentOperationScheme> PlantLoop_Impl::primaryPlantEquipmentOperationScheme() const {
      return getObject<ModelObject>().getModelObjectTarget<PlantEquipmentOperationScheme>(OS_PlantLoopFields::PrimaryPlantEquipmentOperationScheme);
    }

    bool
      PlantLoop_Impl::setPrimaryPlantEquipmentOperationScheme(const boost::optional<PlantEquipmentOperationScheme>& plantEquipmentOperationScheme) {
      bool result(false);
      if (plantEquipmentOperationScheme) {
        result = setPointer(OS_PlantLoopFields::PrimaryPlantEquipmentOperationScheme, plantEquipmentOperationScheme.get().handle());
      } else {
        resetPrimaryPlantEquipmentOperationScheme();
        result = true;
      }
      return result;
    }

    void PlantLoop_Impl::resetPrimaryPlantEquipmentOperationScheme() {
      bool result = setString(OS_PlantLoopFields::PrimaryPlantEquipmentOperationScheme, "");
      OS_ASSERT(result);
    }

    bool PlantLoop_Impl::setPlantEquipmentOperationHeatingLoadSchedule(Schedule& schedule) {
      bool result = setSchedule(OS_PlantLoopFields::PlantEquipmentOperationHeatingLoadSchedule, "PlantLoop",
                                "Plant Equipment Operation Heating Load Schedule", schedule);
      return result;
    }

    void PlantLoop_Impl::resetPlantEquipmentOperationHeatingLoadSchedule() {
      bool result = setString(OS_PlantLoopFields::PlantEquipmentOperationHeatingLoadSchedule, "");
      OS_ASSERT(result);
    }

    boost::optional<Schedule> PlantLoop_Impl::plantEquipmentOperationHeatingLoadSchedule() const {
      return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_PlantLoopFields::PlantEquipmentOperationHeatingLoadSchedule);
    }

    bool PlantLoop_Impl::setPlantEquipmentOperationCoolingLoadSchedule(Schedule& schedule) {
      bool result = setSchedule(OS_PlantLoopFields::PlantEquipmentOperationCoolingLoadSchedule, "PlantLoop",
                                "Plant Equipment Operation Cooling Load Schedule", schedule);
      return result;
    }

    boost::optional<Schedule> PlantLoop_Impl::plantEquipmentOperationCoolingLoadSchedule() const {
      return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_PlantLoopFields::PlantEquipmentOperationCoolingLoadSchedule);
    }

    void PlantLoop_Impl::resetPlantEquipmentOperationCoolingLoadSchedule() {
      bool result = setString(OS_PlantLoopFields::PlantEquipmentOperationCoolingLoadSchedule, "");
      OS_ASSERT(result);
    }

    bool PlantLoop_Impl::setPrimaryPlantEquipmentOperationSchemeSchedule(Schedule& schedule) {
      bool result = setSchedule(OS_PlantLoopFields::PrimaryPlantEquipmentOperationSchemeSchedule, "PlantLoop",
                                "Primary Plant Equipment Operation Scheme Schedule", schedule);
      return result;
    }

    void PlantLoop_Impl::resetPrimaryPlantEquipmentOperationSchemeSchedule() {
      bool result = setString(OS_PlantLoopFields::PlantEquipmentOperationCoolingLoadSchedule, "");
      OS_ASSERT(result);
    }

    boost::optional<Schedule> PlantLoop_Impl::primaryPlantEquipmentOperationSchemeSchedule() const {
      return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_PlantLoopFields::PrimaryPlantEquipmentOperationSchemeSchedule);
    }

    bool PlantLoop_Impl::setComponentSetpointOperationSchemeSchedule(Schedule& schedule) {
      bool result = setSchedule(OS_PlantLoopFields::ComponentSetpointOperationSchemeSchedule, "PlantLoop",
                                "Component Setpoint Operation Scheme Schedule", schedule);
      return result;
    }

    boost::optional<Schedule> PlantLoop_Impl::componentSetpointOperationSchemeSchedule() const {
      return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_PlantLoopFields::ComponentSetpointOperationSchemeSchedule);
    }

    void PlantLoop_Impl::resetComponentSetpointOperationSchemeSchedule() {
      bool result = setString(OS_PlantLoopFields::ComponentSetpointOperationSchemeSchedule, "");
      OS_ASSERT(result);
    }

    boost::optional<double> PlantLoop_Impl::autosizedMaximumLoopFlowRate() const {
      return getAutosizedValue("Maximum Loop Flow Rate", "m3/s");
    }

    boost::optional<double> PlantLoop_Impl::autosizedPlantLoopVolume() const {
      return getAutosizedValue("Plant Loop Volume", "m3");
    }

    void PlantLoop_Impl::autosize() {
      autosizeMaximumLoopFlowRate();
      autocalculatePlantLoopVolume();
    }

    void PlantLoop_Impl::applySizingValues() {
      boost::optional<double> val;
      val = autosizedMaximumLoopFlowRate();
      if (val) {
        setMaximumLoopFlowRate(val.get());
      }

      val = autosizedPlantLoopVolume();
      if (val) {
        setPlantLoopVolume(val.get());
      }
    }

    // AVM section
    AvailabilityManagerAssignmentList PlantLoop_Impl::availabilityManagerAssignmentList() const {
      boost::optional<AvailabilityManagerAssignmentList> avmList =
        getObject<ModelObject>().getModelObjectTarget<AvailabilityManagerAssignmentList>(OS_PlantLoopFields::AvailabilityManagerListName);
      if (avmList) {
        return avmList.get();
      } else {
        LOG_AND_THROW(briefDescription() << "doesn't have an AvailabilityManagerAssignmentList assigned, which shouldn't happen");
      }
    }

    bool PlantLoop_Impl::addAvailabilityManager(const AvailabilityManager& availabilityManager) {
      auto type = availabilityManager.iddObjectType();

      // should be all types but NightCycle and NightVentilation (AirLoopHVAC);
      // and not HybridVentilation (AirLoop, and special, stand-alone)
      if (type == IddObjectType::OS_AvailabilityManager_NightCycle || type == IddObjectType::OS_AvailabilityManager_HybridVentilation
          || type == IddObjectType::OS_AvailabilityManager_NightVentilation) {
        LOG(Warn, "Wrong AVM Type for a PlantLoop: " << availabilityManager.briefDescription());
        return false;
      } else {
        return availabilityManagerAssignmentList().addAvailabilityManager(availabilityManager);
      }
    }

    bool PlantLoop_Impl::addAvailabilityManager(const AvailabilityManager& availabilityManager, unsigned priority) {
      return availabilityManagerAssignmentList().addAvailabilityManager(availabilityManager, priority);
    }

    std::vector<AvailabilityManager> PlantLoop_Impl::availabilityManagers() const {
      // TODO: add the AVM Scheduled
      return availabilityManagerAssignmentList().availabilityManagers();
    }

    bool PlantLoop_Impl::setAvailabilityManagers(const std::vector<AvailabilityManager>& avms) {
      return availabilityManagerAssignmentList().setAvailabilityManagers(avms);
    }

    void PlantLoop_Impl::resetAvailabilityManagers() {
      // TODO: should this affect the AVM Scheduled?
      availabilityManagerAssignmentList().resetAvailabilityManagers();
    }

    bool PlantLoop_Impl::setAvailabilityManagerPriority(const AvailabilityManager& availabilityManager, unsigned priority) {
      return availabilityManagerAssignmentList().setAvailabilityManagerPriority(availabilityManager, priority);
    }

    unsigned PlantLoop_Impl::availabilityManagerPriority(const AvailabilityManager& availabilityManager) const {
      return availabilityManagerAssignmentList().availabilityManagerPriority(availabilityManager);
    }

    bool PlantLoop_Impl::removeAvailabilityManager(const AvailabilityManager& avm) {
      return availabilityManagerAssignmentList().removeAvailabilityManager(avm);
    }

    bool PlantLoop_Impl::removeAvailabilityManager(unsigned priority) {
      return availabilityManagerAssignmentList().removeAvailabilityManager(priority);
    }

    std::vector<EMSActuatorNames> PlantLoop_Impl::emsActuatorNames() const {
      std::vector<EMSActuatorNames> actuators{{"Plant Loop Overall", "On/Off Supervisory"},
                                              {"Supply Side Half Loop", "On/Off Supervisory"},
                                              {"Demand Side Half Loop", "On/Off Supervisory"},
                                              {"Plant Equipment Operation", "Distributed Load Rate"}};  //gets plantequipOperationScheme
      //DYNAMIC
      //{"Supply Side Branch", "On/Off Supervisory"}
      //{"Demand Side Branch", "On/Off Supervisory"}
      //{"Plant Component *", "On/Off Supervisory"}
      return actuators;
    }

    std::vector<std::string> PlantLoop_Impl::emsInternalVariableNames() const {
      //"Component Remaining Current Demand Rate" is dynamic for the plantloop components
      std::vector<std::string> types{"Supply Side Current Demand Rate"};
      return types;
    }

    void PlantLoop_Impl::createTopology() {

      auto m = model();
      auto obj = getObject<model::PlantLoop>();

      // supply side
      Node supplyInletNode(m);
      Node supplyOutletNode(m);
      Node connectorNode(m);

      ConnectorMixer supplyMixer(m);
      setSupplyMixer(supplyMixer);
      ConnectorSplitter supplySplitter(m);
      setSupplySplitter(supplySplitter);

      m.connect(obj, supplyInletPort(), supplyInletNode, supplyInletNode.inletPort());

      m.connect(supplyInletNode, supplyInletNode.outletPort(), supplySplitter, supplySplitter.inletPort());

      m.connect(supplySplitter, supplySplitter.nextOutletPort(), connectorNode, connectorNode.inletPort());

      m.connect(connectorNode, connectorNode.outletPort(), supplyMixer, supplyMixer.nextInletPort());

      m.connect(supplyMixer, supplyMixer.outletPort(), supplyOutletNode, supplyOutletNode.inletPort());

      m.connect(supplyOutletNode, supplyOutletNode.outletPort(), obj, supplyOutletPort());

      // demand side
      Node demandInletNode(m);
      Node demandOutletNode(m);
      Node branchNode(m);
      ConnectorMixer mixer(m);
      setDemandMixer(mixer);
      ConnectorSplitter splitter(m);
      setDemandSplitter(splitter);

      m.connect(obj, demandInletPort(), demandInletNode, demandInletNode.inletPort());

      m.connect(demandInletNode, demandInletNode.outletPort(), splitter, splitter.inletPort());

      m.connect(splitter, splitter.nextOutletPort(), branchNode, branchNode.inletPort());

      m.connect(branchNode, branchNode.outletPort(), mixer, mixer.nextInletPort());

      m.connect(mixer, mixer.outletPort(), demandOutletNode, demandOutletNode.inletPort());

      m.connect(demandOutletNode, demandOutletNode.outletPort(), obj, demandOutletPort());

      setLoopTemperatureSetpointNode(supplyOutletNode);
    }

  }  // namespace detail

  PlantLoop::PlantLoop(Model& model) : Loop(PlantLoop::iddObjectType(), model) {
    auto impl = getImpl<detail::PlantLoop_Impl>();
    OS_ASSERT(impl);

    // Create topology
    impl->createTopology();

    SizingPlant sizingPlant(model, *this);

    autocalculatePlantLoopVolume();
    setLoadDistributionScheme("Optimal");

    setGlycolConcentration(0);
    setString(OS_PlantLoopFields::DemandSideConnectorListName, "");
    setString(OS_PlantLoopFields::PlantLoopDemandCalculationScheme, "");
    setString(OS_PlantLoopFields::CommonPipeSimulation, "");
    setString(OS_PlantLoopFields::PressureSimulationType, "");

    // AvailabilityManagerAssignmentList
    AvailabilityManagerAssignmentList avmList(*this);
    setPointer(OS_PlantLoopFields::AvailabilityManagerListName, avmList.handle());
  }

  PlantLoop::PlantLoop(std::shared_ptr<detail::PlantLoop_Impl> impl) : Loop(std::move(impl)) {}

  std::vector<IdfObject> PlantLoop::remove() {
    return getImpl<detail::PlantLoop_Impl>()->remove();
  }

  ModelObject PlantLoop::clone(Model model) const {
    return getImpl<detail::PlantLoop_Impl>()->clone(model);
  }

  unsigned PlantLoop::supplyInletPort() const {
    return getImpl<detail::PlantLoop_Impl>()->supplyInletPort();
  }

  unsigned PlantLoop::supplyOutletPort() const {
    return getImpl<detail::PlantLoop_Impl>()->supplyOutletPort();
  }

  unsigned PlantLoop::demandInletPort() const {
    return getImpl<detail::PlantLoop_Impl>()->demandInletPort();
  }

  unsigned PlantLoop::demandOutletPort() const {
    return getImpl<detail::PlantLoop_Impl>()->demandOutletPort();
  }

  Node PlantLoop::supplyInletNode() const {
    return getImpl<detail::PlantLoop_Impl>()->supplyInletNode();
  }

  Node PlantLoop::supplyOutletNode() const {
    return getImpl<detail::PlantLoop_Impl>()->supplyOutletNode();
  }

  std::vector<Node> PlantLoop::supplyOutletNodes() const {
    return getImpl<detail::PlantLoop_Impl>()->supplyOutletNodes();
  }

  Node PlantLoop::demandInletNode() const {
    return getImpl<detail::PlantLoop_Impl>()->demandInletNode();
  }

  std::vector<Node> PlantLoop::demandInletNodes() const {
    return getImpl<detail::PlantLoop_Impl>()->demandInletNodes();
  }

  Node PlantLoop::demandOutletNode() const {
    return getImpl<detail::PlantLoop_Impl>()->demandOutletNode();
  }

  bool PlantLoop::addDemandBranchForComponent(HVACComponent component, bool tertiary) {
    return getImpl<detail::PlantLoop_Impl>()->addDemandBranchForComponent(component, tertiary);
  }

  bool PlantLoop::addSupplyBranchForComponent(HVACComponent component) {
    return getImpl<detail::PlantLoop_Impl>()->addSupplyBranchForComponent(component);
  }

  bool PlantLoop::removeSupplyBranchWithComponent(HVACComponent component) {
    return getImpl<detail::PlantLoop_Impl>()->removeSupplyBranchWithComponent(component);
  }

  bool PlantLoop::removeDemandBranchWithComponent(HVACComponent component) {
    return getImpl<detail::PlantLoop_Impl>()->removeDemandBranchWithComponent(component);
  }

  Mixer PlantLoop::supplyMixer() const {
    return getImpl<detail::PlantLoop_Impl>()->supplyMixer();
  }

  Splitter PlantLoop::supplySplitter() const {
    return getImpl<detail::PlantLoop_Impl>()->supplySplitter();
  }

  Mixer PlantLoop::demandMixer() {
    return getImpl<detail::PlantLoop_Impl>()->demandMixer();
  }

  Splitter PlantLoop::demandSplitter() {
    return getImpl<detail::PlantLoop_Impl>()->demandSplitter();
  }

  IddObjectType PlantLoop::iddObjectType() {
    IddObjectType result(IddObjectType::OS_PlantLoop);
    return result;
  }

  std::vector<std::string> PlantLoop::commonPipeSimulationValues() {
    return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(), OS_PlantLoopFields::CommonPipeSimulation);
  }

  std::vector<std::string> PlantLoop::validCommonPipeSimulationValues() {
    return commonPipeSimulationValues();
  }

  double PlantLoop::maximumLoopTemperature() {
    return getImpl<detail::PlantLoop_Impl>()->maximumLoopTemperature();
  }

  bool PlantLoop::setMaximumLoopTemperature(double value) {
    return getImpl<detail::PlantLoop_Impl>()->setMaximumLoopTemperature(value);
  }

  double PlantLoop::minimumLoopTemperature() {
    return getImpl<detail::PlantLoop_Impl>()->minimumLoopTemperature();
  }

  bool PlantLoop::setMinimumLoopTemperature(double value) {
    return getImpl<detail::PlantLoop_Impl>()->setMinimumLoopTemperature(value);
  }

  boost::optional<double> PlantLoop::maximumLoopFlowRate() {
    return getImpl<detail::PlantLoop_Impl>()->maximumLoopFlowRate();
  }

  bool PlantLoop::setMaximumLoopFlowRate(double value) {
    return getImpl<detail::PlantLoop_Impl>()->setMaximumLoopFlowRate(value);
  }

  bool PlantLoop::isMaximumLoopFlowRateAutosized() {
    return getImpl<detail::PlantLoop_Impl>()->isMaximumLoopFlowRateAutosized();
  }

  void PlantLoop::autosizeMaximumLoopFlowRate() {
    getImpl<detail::PlantLoop_Impl>()->autosizeMaximumLoopFlowRate();
  }

  boost::optional<double> PlantLoop::minimumLoopFlowRate() {
    return getImpl<detail::PlantLoop_Impl>()->minimumLoopFlowRate();
  }

  bool PlantLoop::setMinimumLoopFlowRate(double value) {
    return getImpl<detail::PlantLoop_Impl>()->setMinimumLoopFlowRate(value);
  }

  bool PlantLoop::isMinimumLoopFlowRateAutosized() {
    return getImpl<detail::PlantLoop_Impl>()->isMinimumLoopFlowRateAutosized();
  }

  void PlantLoop::autosizeMinimumLoopFlowRate() {
    getImpl<detail::PlantLoop_Impl>()->autosizeMinimumLoopFlowRate();
  }

  boost::optional<double> PlantLoop::plantLoopVolume() {
    return getImpl<detail::PlantLoop_Impl>()->plantLoopVolume();
  }

  bool PlantLoop::setPlantLoopVolume(double value) {
    return getImpl<detail::PlantLoop_Impl>()->setPlantLoopVolume(value);
  }

  bool PlantLoop::isPlantLoopVolumeAutocalculated() {
    return getImpl<detail::PlantLoop_Impl>()->isPlantLoopVolumeAutocalculated();
  }

  void PlantLoop::autocalculatePlantLoopVolume() {
    getImpl<detail::PlantLoop_Impl>()->autocalculatePlantLoopVolume();
  }

  std::string PlantLoop::fluidType() {
    return getImpl<detail::PlantLoop_Impl>()->fluidType();
  }

  bool PlantLoop::setFluidType(const std::string& value) {
    return getImpl<detail::PlantLoop_Impl>()->setFluidType(value);
  }

  std::vector<std::string> PlantLoop::fluidTypeValues() {
    return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(), OS_PlantLoopFields::FluidType);
  }

  std::vector<std::string> PlantLoop::validFluidTypeValues() {
    return fluidTypeValues();
  }

  int PlantLoop::glycolConcentration() const {
    return getImpl<detail::PlantLoop_Impl>()->glycolConcentration();
  }

  bool PlantLoop::setGlycolConcentration(int glycolConcentration) {
    return getImpl<detail::PlantLoop_Impl>()->setGlycolConcentration(glycolConcentration);
  }

  Node PlantLoop::loopTemperatureSetpointNode() {
    return getImpl<detail::PlantLoop_Impl>()->loopTemperatureSetpointNode();
  }

  bool PlantLoop::setLoopTemperatureSetpointNode(Node& node) {
    return getImpl<detail::PlantLoop_Impl>()->setLoopTemperatureSetpointNode(node);
  }

  SizingPlant PlantLoop::sizingPlant() const {
    return getImpl<detail::PlantLoop_Impl>()->sizingPlant();
  }

  std::string PlantLoop::commonPipeSimulation() const {
    return getImpl<detail::PlantLoop_Impl>()->commonPipeSimulation();
  }

  bool PlantLoop::isCommonPipeSimulationDefaulted() const {
    return getImpl<detail::PlantLoop_Impl>()->isCommonPipeSimulationDefaulted();
  }

  bool PlantLoop::setCommonPipeSimulation(const std::string& value) {
    return getImpl<detail::PlantLoop_Impl>()->setCommonPipeSimulation(value);
  }

  void PlantLoop::resetCommonPipeSimulation() {
    getImpl<detail::PlantLoop_Impl>()->resetCommonPipeSimulation();
  }

  boost::optional<PlantEquipmentOperationHeatingLoad> PlantLoop::plantEquipmentOperationHeatingLoad() const {
    return getImpl<detail::PlantLoop_Impl>()->plantEquipmentOperationHeatingLoad();
  }

  bool PlantLoop::setPlantEquipmentOperationHeatingLoad(const PlantEquipmentOperationHeatingLoad& plantOperation) {
    return getImpl<detail::PlantLoop_Impl>()->setPlantEquipmentOperationHeatingLoad(plantOperation);
  }

  void PlantLoop::resetPlantEquipmentOperationHeatingLoad() {
    getImpl<detail::PlantLoop_Impl>()->resetPlantEquipmentOperationHeatingLoad();
  }

  boost::optional<PlantEquipmentOperationCoolingLoad> PlantLoop::plantEquipmentOperationCoolingLoad() const {
    return getImpl<detail::PlantLoop_Impl>()->plantEquipmentOperationCoolingLoad();
  }

  bool PlantLoop::setPlantEquipmentOperationCoolingLoad(const PlantEquipmentOperationCoolingLoad& plantOperation) {
    return getImpl<detail::PlantLoop_Impl>()->setPlantEquipmentOperationCoolingLoad(plantOperation);
  }

  void PlantLoop::resetPlantEquipmentOperationCoolingLoad() {
    getImpl<detail::PlantLoop_Impl>()->resetPlantEquipmentOperationCoolingLoad();
  }

  boost::optional<PlantEquipmentOperationScheme> PlantLoop::primaryPlantEquipmentOperationScheme() const {
    return getImpl<detail::PlantLoop_Impl>()->primaryPlantEquipmentOperationScheme();
  }

  bool PlantLoop::setPrimaryPlantEquipmentOperationScheme(const PlantEquipmentOperationScheme& plantOperation) {
    return getImpl<detail::PlantLoop_Impl>()->setPrimaryPlantEquipmentOperationScheme(plantOperation);
  }

  void PlantLoop::resetPrimaryPlantEquipmentOperationScheme() {
    getImpl<detail::PlantLoop_Impl>()->resetPrimaryPlantEquipmentOperationScheme();
  }

  std::string PlantLoop::loadDistributionScheme() {
    return getImpl<detail::PlantLoop_Impl>()->loadDistributionScheme();
  }

  bool PlantLoop::setLoadDistributionScheme(const std::string& scheme) {
    return getImpl<detail::PlantLoop_Impl>()->setLoadDistributionScheme(scheme);
  }

  std::vector<std::string> PlantLoop::loadDistributionSchemeValues() {
    return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(), OS_PlantLoopFields::LoadDistributionScheme);
  }

  std::vector<std::string> PlantLoop::validLoadDistributionSchemeValues() {
    return loadDistributionSchemeValues();
  }

  bool PlantLoop::setPlantEquipmentOperationHeatingLoadSchedule(Schedule& schedule) {
    return getImpl<detail::PlantLoop_Impl>()->setPlantEquipmentOperationHeatingLoadSchedule(schedule);
  }

  void PlantLoop::resetPlantEquipmentOperationHeatingLoadSchedule() {
    getImpl<detail::PlantLoop_Impl>()->resetPlantEquipmentOperationHeatingLoadSchedule();
  }

  boost::optional<Schedule> PlantLoop::plantEquipmentOperationHeatingLoadSchedule() const {
    return getImpl<detail::PlantLoop_Impl>()->plantEquipmentOperationHeatingLoadSchedule();
  }

  bool PlantLoop::setPlantEquipmentOperationCoolingLoadSchedule(Schedule& schedule) {
    return getImpl<detail::PlantLoop_Impl>()->setPlantEquipmentOperationCoolingLoadSchedule(schedule);
  }

  boost::optional<Schedule> PlantLoop::plantEquipmentOperationCoolingLoadSchedule() const {
    return getImpl<detail::PlantLoop_Impl>()->plantEquipmentOperationCoolingLoadSchedule();
  }

  void PlantLoop::resetPlantEquipmentOperationCoolingLoadSchedule() {
    getImpl<detail::PlantLoop_Impl>()->resetPlantEquipmentOperationCoolingLoadSchedule();
  }

  bool PlantLoop::setPrimaryPlantEquipmentOperationSchemeSchedule(Schedule& schedule) {
    return getImpl<detail::PlantLoop_Impl>()->setPrimaryPlantEquipmentOperationSchemeSchedule(schedule);
  }

  void PlantLoop::resetPrimaryPlantEquipmentOperationSchemeSchedule() {
    getImpl<detail::PlantLoop_Impl>()->resetPrimaryPlantEquipmentOperationSchemeSchedule();
  }

  boost::optional<Schedule> PlantLoop::primaryPlantEquipmentOperationSchemeSchedule() const {
    return getImpl<detail::PlantLoop_Impl>()->primaryPlantEquipmentOperationSchemeSchedule();
  }

  bool PlantLoop::setComponentSetpointOperationSchemeSchedule(Schedule& schedule) {
    return getImpl<detail::PlantLoop_Impl>()->setComponentSetpointOperationSchemeSchedule(schedule);
  }

  boost::optional<Schedule> PlantLoop::componentSetpointOperationSchemeSchedule() const {
    return getImpl<detail::PlantLoop_Impl>()->componentSetpointOperationSchemeSchedule();
  }

  void PlantLoop::resetComponentSetpointOperationSchemeSchedule() {
    getImpl<detail::PlantLoop_Impl>()->resetComponentSetpointOperationSchemeSchedule();
  }

  boost::optional<double> PlantLoop::autosizedMaximumLoopFlowRate() const {
    return getImpl<detail::PlantLoop_Impl>()->autosizedMaximumLoopFlowRate();
  }

  boost::optional<double> PlantLoop::autosizedPlantLoopVolume() const {
    return getImpl<detail::PlantLoop_Impl>()->autosizedPlantLoopVolume();
  }

  /* Prefered way to interact with the Availability Managers */
  std::vector<AvailabilityManager> PlantLoop::availabilityManagers() const {
    return getImpl<detail::PlantLoop_Impl>()->availabilityManagers();
  }

  bool PlantLoop::setAvailabilityManagers(const std::vector<AvailabilityManager>& avms) {
    return getImpl<detail::PlantLoop_Impl>()->setAvailabilityManagers(avms);
  }

  void PlantLoop::resetAvailabilityManagers() {
    return getImpl<detail::PlantLoop_Impl>()->resetAvailabilityManagers();
  }

  bool PlantLoop::addAvailabilityManager(const AvailabilityManager& availabilityManager) {
    return getImpl<detail::PlantLoop_Impl>()->addAvailabilityManager(availabilityManager);
  }
  // End prefered way

  bool PlantLoop::addAvailabilityManager(const AvailabilityManager& availabilityManager, unsigned priority) {
    return getImpl<detail::PlantLoop_Impl>()->addAvailabilityManager(availabilityManager, priority);
  }

  unsigned PlantLoop::availabilityManagerPriority(const AvailabilityManager& availabilityManager) const {
    return getImpl<detail::PlantLoop_Impl>()->availabilityManagerPriority(availabilityManager);
  }

  bool PlantLoop::setAvailabilityManagerPriority(const AvailabilityManager& availabilityManager, unsigned priority) {
    return getImpl<detail::PlantLoop_Impl>()->setAvailabilityManagerPriority(availabilityManager, priority);
  }

  bool PlantLoop::removeAvailabilityManager(const AvailabilityManager& availabilityManager) {
    return getImpl<detail::PlantLoop_Impl>()->removeAvailabilityManager(availabilityManager);
  }

  bool PlantLoop::removeAvailabilityManager(const unsigned priority) {
    return getImpl<detail::PlantLoop_Impl>()->removeAvailabilityManager(priority);
  }

}  // namespace model
}  // namespace openstudio
