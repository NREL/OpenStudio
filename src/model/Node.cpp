/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

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
#include "AirflowNetworkDistributionNode.hpp"
#include "AirflowNetworkDistributionNode_Impl.hpp"
#include "ThermalZone.hpp"
#include "PortList.hpp"
#include "PortList_Impl.hpp"
#include "Model.hpp"

#include "../utilities/core/Assert.hpp"
#include "../utilities/data/DataEnums.hpp"

#include <utilities/idd/OS_Node_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

namespace openstudio {

namespace model {

  namespace detail {

    Node_Impl::Node_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle) : StraightComponent_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == Node::iddObjectType());
    }

    Node_Impl::Node_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle)
      : StraightComponent_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == Node::iddObjectType());
    }

    Node_Impl::Node_Impl(const Node_Impl& other, Model_Impl* model, bool keepHandle) : StraightComponent_Impl(other, model, keepHandle) {}

    // virtual destructor

    // Get all output variable names that could be associated with this object.
    const std::vector<std::string>& Node_Impl::outputVariableNames() const {
      static const std::vector<std::string> result{
        // Common Variables
        "System Node Temperature", "System Node Last Timestep Temperature", "System Node Mass Flow Rate", "System Node Humidity Ratio",
        "System Node Setpoint Temperature", "System Node Setpoint High Temperature", "System Node Setpoint Low Temperature",
        "System Node Setpoint Humidity Ratio", "System Node Setpoint Minimum Humidity Ratio", "System Node Setpoint Maximum Humidity Ratio",
        "System Node Relative Humidity", "System Node Pressure", "System Node Standard Density Volume Flow Rate", "System Node Enthalpy",
        "System Node Last Timestep Enthalpy", "System Node Wetbulb Temperature", "System Node Dewpoint Temperature", "System Node Quality",
        "System Node Height", "System Node Specific Heat",

        // The following node variable is also available for system nodes that are for “air”:
        // TODO: implement check? If no, make result non static and remove the result.empty() check
        "System Node Current Density Volume Flow Rate", "System Node Current Density"

        // The following node variables are “advanced” and normally used for debugging unusual cases:
        //"System Node Minimum Temperature",
        //"System Node Maximum Temperature",
        //"System Node Minimum Limit Mass Flow Rate",
        //"System Node Maximum Limit Mass Flow Rate",
        //"System Node Minimum Available Mass Flow Rate",
        //"System Node Maximum Available Mass Flow Rate",
        //"System Node Requested Mass Flow Rate",
        //"System Node Setpoint Mass Flow Rate",

        // The following node variable reports node carbon dioxide concentration when carbon dioxide is simulated (ref. ZoneAirContaminantBalance):
        // "System Node CO2 Concentration",

        // The following node variable reports node generic contaminant concentration when generic contaminant is simulated (ref. ZoneAirContaminantBalance):
        // "System Node Generic Air Contaminant Concentration"

      };

      return result;
    }

    IddObjectType Node_Impl::iddObjectType() const {
      return Node::iddObjectType();
    }

    unsigned Node_Impl::inletPort() const {
      return OS_NodeFields::InletPort;
    }

    unsigned Node_Impl::outletPort() const {
      return OS_NodeFields::OutletPort;
    }

    std::vector<HVACComponent> Node_Impl::edges(const boost::optional<HVACComponent>& /*prev*/) {
      std::vector<HVACComponent> edges;
      if (boost::optional<ModelObject> edgeModelObject = this->outletModelObject()) {
        if (boost::optional<PortList> portList = edgeModelObject->optionalCast<PortList>()) {
          if (boost::optional<ThermalZone> thermalZone = portList->thermalZone()) {
            edges.push_back(*thermalZone);
          }
        } else if (boost::optional<HVACComponent> edgeObject = edgeModelObject->optionalCast<HVACComponent>()) {
          edges.push_back(*edgeObject);
        }
      }
      return edges;
    }

    std::vector<IdfObject> Node_Impl::remove() {
      if (isRemovable()) {
        this->removeSetpointManagers();
        return ModelObject_Impl::remove();
      } else {
        return {};
      }
    }

    std::vector<ModelObject> Node_Impl::children() const {
      std::vector<ModelObject> result = castVector<ModelObject>(this->setpointManagers());
      std::vector<AirflowNetworkDistributionNode> myAFNItems =
        getObject<ModelObject>().getModelObjectSources<AirflowNetworkDistributionNode>(AirflowNetworkDistributionNode::iddObjectType());
      result.insert(result.end(), myAFNItems.begin(), myAFNItems.end());
      return result;
    }

    bool Node_Impl::addToNode(Node& /*node*/) {
      return false;
    }

    ModelObject Node_Impl::clone(Model model) const {
      return StraightComponent_Impl::clone(model);
    }

    bool Node_Impl::isRemovable() const {
      if (this->loop()) {
        return false;
      } else {
        return true;
      }
    }

    std::vector<SetpointManager> Node_Impl::setpointManagers() const {
      std::vector<SetpointManager> _setpointManagers;
      std::vector<SetpointManager> _modelObjects = getObject<Node>().getModelObjectSources<SetpointManager>();
      for (auto it = _modelObjects.begin(); it != _modelObjects.end(); ++it) {
        if (boost::optional<Node> setpointNode = it->setpointNode()) {
          if (setpointNode->handle() == this->handle()) {
            _setpointManagers.push_back(*it);
          }
        }
      }
      return _setpointManagers;
    }

    void Node_Impl::removeSetpointManagers() {
      std::vector<SetpointManager> _setpointManagers = this->setpointManagers();
      for (auto it = _setpointManagers.begin(); it != _setpointManagers.end(); ++it) {
        it->remove();
      }
    }

    bool Node_Impl::isConnected(const ModelObject& modelObject) {
      if (auto mo = outletModelObject()) {
        if (modelObject.handle() == mo->handle()) {
          return true;
        }
      }
      if (auto mo = inletModelObject()) {
        if (modelObject.handle() == mo->handle()) {
          return true;
        }
      }

      return false;
    }

    std::vector<EMSActuatorNames> Node_Impl::emsActuatorNames() const {
      std::vector<EMSActuatorNames> actuators{{"System Node Setpoint", "Temperature Setpoint"},
                                              {"System Node Setpoint", "Temperature Minimum Setpoint"},
                                              {"System Node Setpoint", "Temperature Maximum Setpoint"},
                                              {"System Node Setpoint", "Humidity Ratio Setpoint"},
                                              {"System Node Setpoint", "Humidity Ratio Maximum Setpoint"},
                                              {"System Node Setpoint", "Humidity Ratio Minimum Setpoint"},
                                              {"System Node Setpoint", "Mass Flow Rate Setpoint"},
                                              {"System Node Setpoint", "Mass Flow Rate Maximum Available Setpoint"},
                                              {"System Node Setpoint", "Mass Flow Rate Minimum Available Setpoint"},
                                              {"Outdoor Air System Node", "Drybulb Temperature"},
                                              {"Outdoor Air System Node", "Wetbulb Temperature"},
                                              {"Outdoor Air System Node", "Wind Speed"},
                                              {"Outdoor Air System Node", "Wind Direction"}};
      return actuators;
    }

    std::vector<std::string> Node_Impl::emsInternalVariableNames() const {
      std::vector<std::string> types;
      return types;
    }

    AirflowNetworkDistributionNode Node_Impl::getAirflowNetworkDistributionNode() {
      boost::optional<AirflowNetworkDistributionNode> opt = airflowNetworkDistributionNode();
      if (opt) {
        return opt.get();
      }
      return AirflowNetworkDistributionNode(model(), handle());
    }

    boost::optional<AirflowNetworkDistributionNode> Node_Impl::airflowNetworkDistributionNode() const {
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

    ComponentType Node_Impl::componentType() const {
      return ComponentType::None;
    }

    std::vector<FuelType> Node_Impl::coolingFuelTypes() const {
      return {};
    }

    std::vector<FuelType> Node_Impl::heatingFuelTypes() const {
      return {};
    }

    std::vector<AppGFuelType> Node_Impl::appGHeatingFuelTypes() const {
      return {};
    }

  }  // namespace detail

  // create a new Node object in the model's workspace
  Node::Node(const Model& model) : StraightComponent(Node::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::Node_Impl>());
  }

  Node::Node(std::shared_ptr<detail::Node_Impl> impl) : StraightComponent(std::move(impl)) {}

  std::vector<SetpointManager> Node::setpointManagers() const {
    return getImpl<detail::Node_Impl>()->setpointManagers();
  }

  IddObjectType Node::iddObjectType() {
    IddObjectType result(IddObjectType::OS_Node);
    return result;
  }

  AirflowNetworkDistributionNode Node::getAirflowNetworkDistributionNode() {
    return getImpl<detail::Node_Impl>()->getAirflowNetworkDistributionNode();
  }

  boost::optional<AirflowNetworkDistributionNode> Node::airflowNetworkDistributionNode() const {
    return getImpl<detail::Node_Impl>()->airflowNetworkDistributionNode();
  }

}  // namespace model
}  // namespace openstudio
