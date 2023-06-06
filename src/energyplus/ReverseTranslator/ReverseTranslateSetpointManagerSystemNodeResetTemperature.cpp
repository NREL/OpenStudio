/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ReverseTranslator.hpp"

#include "../../model/SetpointManagerSystemNodeResetTemperature.hpp"

#include "../../model/Node.hpp"
#include "../../model/Node_Impl.hpp"

#include <utilities/idd/SetpointManager_SystemNodeReset_Temperature_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  boost::optional<ModelObject> ReverseTranslator::translateSetpointManagerSystemNodeResetTemperature(const WorkspaceObject& workspaceObject) {

    boost::optional<Node> setpointNode;
    if (boost::optional<std::string> setpointNodeName =
          workspaceObject.getString(SetpointManager_SystemNodeReset_TemperatureFields::SetpointNodeorNodeListName)) {
      setpointNode = m_model.getConcreteModelObjectByName<Node>(setpointNodeName.get());
    }

    if (!setpointNode) {
      LOG(Error, workspaceObject.briefDescription() << " is not attached to a Setpoint Node in the model");

      return boost::none;
    }

    boost::optional<Node> referenceNode;
    if (boost::optional<std::string> referenceNodeName =
          workspaceObject.getString(SetpointManager_SystemNodeReset_TemperatureFields::ReferenceNodeName)) {
      referenceNode = m_model.getConcreteModelObjectByName<Node>(referenceNodeName.get());
    }

    if (!referenceNode) {
      LOG(Error, workspaceObject.briefDescription() << " is not attached to a Reference Node in the model");

      return boost::none;
    }

    openstudio::model::SetpointManagerSystemNodeResetTemperature modelObject(m_model);

    // Name
    if (boost::optional<std::string> _name = workspaceObject.name()) {
      modelObject.setName(_name.get());
    }

    // Control Variable: Required String
    if (boost::optional<std::string> _controlVariable =
          workspaceObject.getString(SetpointManager_SystemNodeReset_TemperatureFields::ControlVariable)) {
      modelObject.setControlVariable(_controlVariable.get());
    } else {
      LOG(Warn, "For " << workspaceObject.briefDescription() << ", cannot find required property 'Control Variable'");
    }

    // Setpoint at Low Reference Temperature: Required Double
    if (boost::optional<double> _setpointatLowReferenceTemperature =
          workspaceObject.getDouble(SetpointManager_SystemNodeReset_TemperatureFields::SetpointatLowReferenceTemperature)) {
      modelObject.setSetpointatLowReferenceTemperature(_setpointatLowReferenceTemperature.get());
    } else {
      LOG(Warn, "For " << workspaceObject.briefDescription() << ", cannot find required property 'Setpoint at Low Reference Temperature'");
    }

    // Setpoint at High Reference Temperature: Required Double
    if (boost::optional<double> _setpointatHighReferenceTemperature =
          workspaceObject.getDouble(SetpointManager_SystemNodeReset_TemperatureFields::SetpointatHighReferenceTemperature)) {
      modelObject.setSetpointatHighReferenceTemperature(_setpointatHighReferenceTemperature.get());
    } else {
      LOG(Warn, "For " << workspaceObject.briefDescription() << ", cannot find required property 'Setpoint at High Reference Temperature'");
    }

    // Low Reference Temperature: Required Double
    if (boost::optional<double> _lowReferenceTemperature =
          workspaceObject.getDouble(SetpointManager_SystemNodeReset_TemperatureFields::LowReferenceTemperature)) {
      modelObject.setLowReferenceTemperature(_lowReferenceTemperature.get());
    } else {
      LOG(Warn, "For " << workspaceObject.briefDescription() << ", cannot find required property 'Low Reference Temperature'");
    }

    // High Reference Temperature: Required Double
    if (boost::optional<double> _highReferenceTemperature =
          workspaceObject.getDouble(SetpointManager_SystemNodeReset_TemperatureFields::HighReferenceTemperature)) {
      modelObject.setHighReferenceTemperature(_highReferenceTemperature.get());
    } else {
      LOG(Warn, "For " << workspaceObject.briefDescription() << ", cannot find required property 'High Reference Temperature'");
    }

    // Reference Node Name: Required Node
    modelObject.setReferenceNode(referenceNode.get());

    // Setpoint Node or NodeList Name: Required Node
    modelObject.addToNode(setpointNode.get());

    return modelObject;
  }  // End of translate function

}  // end namespace energyplus
}  // end namespace openstudio
