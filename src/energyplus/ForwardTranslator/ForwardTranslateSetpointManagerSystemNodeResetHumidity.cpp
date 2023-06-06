/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ForwardTranslator.hpp"
#include "../../model/Model.hpp"

#include "../../model/SetpointManagerSystemNodeResetHumidity.hpp"

#include "../../model/Node.hpp"

#include <utilities/idd/SetpointManager_SystemNodeReset_Humidity_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject>
    ForwardTranslator::translateSetpointManagerSystemNodeResetHumidity(model::SetpointManagerSystemNodeResetHumidity& modelObject) {

    // Translation is triggered via the setpointNode already, so no need to check
    if (!modelObject.referenceNode()) {
      LOG(Error, modelObject.briefDescription() << " does not have a Reference Node attached, it will be not be translated.");
      return boost::none;
    }

    // Instantiate an IdfObject of the class to store the values
    IdfObject idfObject = createRegisterAndNameIdfObject(openstudio::IddObjectType::SetpointManager_SystemNodeReset_Humidity, modelObject);

    // Control Variable: Required String
    std::string controlVariable = modelObject.controlVariable();
    idfObject.setString(SetpointManager_SystemNodeReset_HumidityFields::ControlVariable, controlVariable);

    // Setpoint at Low Reference Humidity Ratio: Required Double
    double setpointatLowReferenceHumidityRatio = modelObject.setpointatLowReferenceHumidityRatio();
    idfObject.setDouble(SetpointManager_SystemNodeReset_HumidityFields::SetpointatLowReferenceHumidityRatio, setpointatLowReferenceHumidityRatio);

    // Setpoint at High Reference Humidity Ratio: Required Double
    double setpointatHighReferenceHumidityRatio = modelObject.setpointatHighReferenceHumidityRatio();
    idfObject.setDouble(SetpointManager_SystemNodeReset_HumidityFields::SetpointatHighReferenceHumidityRatio, setpointatHighReferenceHumidityRatio);

    // Low Reference Humidity Ratio: Required Double
    double lowReferenceHumidityRatio = modelObject.lowReferenceHumidityRatio();
    idfObject.setDouble(SetpointManager_SystemNodeReset_HumidityFields::LowReferenceHumidityRatio, lowReferenceHumidityRatio);

    // High Reference Humidity Ratio: Required Double
    double highReferenceHumidityRatio = modelObject.highReferenceHumidityRatio();
    idfObject.setDouble(SetpointManager_SystemNodeReset_HumidityFields::HighReferenceHumidityRatio, highReferenceHumidityRatio);

    // Reference Node Name: Required Node (but we checked it above)
    idfObject.setString(SetpointManager_SystemNodeReset_HumidityFields::ReferenceNodeName, modelObject.referenceNode()->nameString());

    // Setpoint Node or NodeList Name: Required Node (but we know it exists)
    idfObject.setString(SetpointManager_SystemNodeReset_HumidityFields::SetpointNodeorNodeListName, modelObject.setpointNode()->nameString());

    return idfObject;
  }  // End of translate function

}  // end namespace energyplus
}  // end namespace openstudio
