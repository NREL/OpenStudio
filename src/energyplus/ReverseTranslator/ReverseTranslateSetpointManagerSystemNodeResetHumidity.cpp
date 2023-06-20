/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ReverseTranslator.hpp"

#include "../../model/SetpointManagerSystemNodeResetHumidity.hpp"

#include "../../model/Node.hpp"
#include "../../model/Node_Impl.hpp"

#include <utilities/idd/SetpointManager_SystemNodeReset_Humidity_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  boost::optional<ModelObject> ReverseTranslator::translateSetpointManagerSystemNodeResetHumidity(const WorkspaceObject& workspaceObject) {

    boost::optional<Node> setpointNode;
    if (boost::optional<std::string> setpointNodeName =
          workspaceObject.getString(SetpointManager_SystemNodeReset_HumidityFields::SetpointNodeorNodeListName)) {
      setpointNode = m_model.getConcreteModelObjectByName<Node>(setpointNodeName.get());
    }

    if (!setpointNode) {
      LOG(Error, workspaceObject.briefDescription() << " is not attached to a Setpoint Node in the model");

      return boost::none;
    }

    boost::optional<Node> referenceNode;
    if (boost::optional<std::string> referenceNodeName =
          workspaceObject.getString(SetpointManager_SystemNodeReset_HumidityFields::ReferenceNodeName)) {
      referenceNode = m_model.getConcreteModelObjectByName<Node>(referenceNodeName.get());
    }

    if (!referenceNode) {
      LOG(Error, workspaceObject.briefDescription() << " is not attached to a Reference Node in the model");

      return boost::none;
    }

    openstudio::model::SetpointManagerSystemNodeResetHumidity modelObject(m_model);

    // Name
    if (boost::optional<std::string> _name = workspaceObject.name()) {
      modelObject.setName(_name.get());
    }

    // Control Variable: Required String
    if (boost::optional<std::string> _controlVariable = workspaceObject.getString(SetpointManager_SystemNodeReset_HumidityFields::ControlVariable)) {
      modelObject.setControlVariable(_controlVariable.get());
    } else {
      LOG(Warn, "For " << workspaceObject.briefDescription() << ", cannot find required property 'Control Variable'");
    }

    // Setpoint at Low Reference Humidity Ratio: Required Double
    if (boost::optional<double> _setpointatLowReferenceHumidityRatio =
          workspaceObject.getDouble(SetpointManager_SystemNodeReset_HumidityFields::SetpointatLowReferenceHumidityRatio)) {
      modelObject.setSetpointatLowReferenceHumidityRatio(_setpointatLowReferenceHumidityRatio.get());
    } else {
      LOG(Warn, "For " << workspaceObject.briefDescription() << ", cannot find required property 'Setpoint at Low Reference Humidity Ratio'");
    }

    // Setpoint at High Reference Humidity Ratio: Required Double
    if (boost::optional<double> _setpointatHighReferenceHumidityRatio =
          workspaceObject.getDouble(SetpointManager_SystemNodeReset_HumidityFields::SetpointatHighReferenceHumidityRatio)) {
      modelObject.setSetpointatHighReferenceHumidityRatio(_setpointatHighReferenceHumidityRatio.get());
    } else {
      LOG(Warn, "For " << workspaceObject.briefDescription() << ", cannot find required property 'Setpoint at High Reference Humidity Ratio'");
    }

    // Low Reference Humidity Ratio: Required Double
    if (boost::optional<double> _lowReferenceHumidityRatio =
          workspaceObject.getDouble(SetpointManager_SystemNodeReset_HumidityFields::LowReferenceHumidityRatio)) {
      modelObject.setLowReferenceHumidityRatio(_lowReferenceHumidityRatio.get());
    } else {
      LOG(Warn, "For " << workspaceObject.briefDescription() << ", cannot find required property 'Low Reference Humidity Ratio'");
    }

    // High Reference Humidity Ratio: Required Double
    if (boost::optional<double> _highReferenceHumidityRatio =
          workspaceObject.getDouble(SetpointManager_SystemNodeReset_HumidityFields::HighReferenceHumidityRatio)) {
      modelObject.setHighReferenceHumidityRatio(_highReferenceHumidityRatio.get());
    } else {
      LOG(Warn, "For " << workspaceObject.briefDescription() << ", cannot find required property 'High Reference Humidity Ratio'");
    }

    // Reference Node Name: Required Node
    modelObject.setReferenceNode(referenceNode.get());

    // Setpoint Node or NodeList Name: Required Node
    modelObject.addToNode(setpointNode.get());

    return modelObject;
  }  // End of translate function

}  // end namespace energyplus
}  // end namespace openstudio
