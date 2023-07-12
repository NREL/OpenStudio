/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ForwardTranslator.hpp"
#include "../../model/Model.hpp"

#include "../../model/SetpointManagerSystemNodeResetTemperature.hpp"

#include "../../model/Node.hpp"

#include <utilities/idd/SetpointManager_SystemNodeReset_Temperature_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject>
    ForwardTranslator::translateSetpointManagerSystemNodeResetTemperature(model::SetpointManagerSystemNodeResetTemperature& modelObject) {

    // Translation is triggered via the setpointNode already, so no need to check
    if (!modelObject.referenceNode()) {
      LOG(Error, modelObject.briefDescription() << " does not have a Reference Node attached, it will be not be translated.");
      return boost::none;
    }

    // Instantiate an IdfObject of the class to store the values
    IdfObject idfObject = createRegisterAndNameIdfObject(openstudio::IddObjectType::SetpointManager_SystemNodeReset_Temperature, modelObject);

    // Control Variable: Required String
    std::string controlVariable = modelObject.controlVariable();
    idfObject.setString(SetpointManager_SystemNodeReset_TemperatureFields::ControlVariable, controlVariable);

    // Setpoint at Low Reference Temperature: Required Double
    double setpointatLowReferenceTemperature = modelObject.setpointatLowReferenceTemperature();
    idfObject.setDouble(SetpointManager_SystemNodeReset_TemperatureFields::SetpointatLowReferenceTemperature, setpointatLowReferenceTemperature);

    // Setpoint at High Reference Temperature: Required Double
    double setpointatHighReferenceTemperature = modelObject.setpointatHighReferenceTemperature();
    idfObject.setDouble(SetpointManager_SystemNodeReset_TemperatureFields::SetpointatHighReferenceTemperature, setpointatHighReferenceTemperature);

    // Low Reference Temperature: Required Double
    double lowReferenceTemperature = modelObject.lowReferenceTemperature();
    idfObject.setDouble(SetpointManager_SystemNodeReset_TemperatureFields::LowReferenceTemperature, lowReferenceTemperature);

    // High Reference Temperature: Required Double
    double highReferenceTemperature = modelObject.highReferenceTemperature();
    idfObject.setDouble(SetpointManager_SystemNodeReset_TemperatureFields::HighReferenceTemperature, highReferenceTemperature);

    // Reference Node Name: Required Node (but we checked it above)
    idfObject.setString(SetpointManager_SystemNodeReset_TemperatureFields::ReferenceNodeName, modelObject.referenceNode()->nameString());

    // Setpoint Node or NodeList Name: Required Node (but we know it exists)
    idfObject.setString(SetpointManager_SystemNodeReset_TemperatureFields::SetpointNodeorNodeListName, modelObject.setpointNode()->nameString());

    return idfObject;
  }  // End of translate function

}  // end namespace energyplus
}  // end namespace openstudio
