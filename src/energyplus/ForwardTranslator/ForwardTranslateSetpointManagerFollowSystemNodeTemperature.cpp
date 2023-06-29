/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ForwardTranslator.hpp"
#include "../../model/SetpointManagerFollowSystemNodeTemperature.hpp"
#include "../../model/Node.hpp"
#include <utilities/idd/SetpointManager_FollowSystemNodeTemperature_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject>
    ForwardTranslator::translateSetpointManagerFollowSystemNodeTemperature(SetpointManagerFollowSystemNodeTemperature& modelObject) {
    boost::optional<Node> node;
    boost::optional<std::string> s;
    boost::optional<double> d;

    // Name
    IdfObject idfObject = createRegisterAndNameIdfObject(openstudio::IddObjectType::SetpointManager_FollowSystemNodeTemperature, modelObject);

    // ControlVariable
    s = modelObject.controlVariable();
    if (s) {
      idfObject.setString(SetpointManager_FollowSystemNodeTemperatureFields::ControlVariable, s.get());
    }

    // ReferenceNodeName
    node = modelObject.referenceNode();
    if (node) {
      idfObject.setString(SetpointManager_FollowSystemNodeTemperatureFields::ReferenceNodeName, node->name().get());
    }

    // ReferenceTemperatureType
    s = modelObject.referenceTemperatureType();
    if (s) {
      idfObject.setString(SetpointManager_FollowSystemNodeTemperatureFields::ReferenceTemperatureType, s.get());
    }

    // OffsetTemperatureDifference
    d = modelObject.offsetTemperatureDifference();
    if (d) {
      idfObject.setDouble(SetpointManager_FollowSystemNodeTemperatureFields::OffsetTemperatureDifference, d.get());
    }

    // MaximumLimitSetpointTemperature
    d = modelObject.maximumLimitSetpointTemperature();
    if (d) {
      idfObject.setDouble(SetpointManager_FollowSystemNodeTemperatureFields::MaximumLimitSetpointTemperature, d.get());
    }

    // MinimumLimitSetpointTemperature
    d = modelObject.minimumLimitSetpointTemperature();
    if (d) {
      idfObject.setDouble(SetpointManager_FollowSystemNodeTemperatureFields::MinimumLimitSetpointTemperature, d.get());
    }

    // SetpointNodeorNodeListName
    node = modelObject.setpointNode();
    if (node) {
      idfObject.setString(SetpointManager_FollowSystemNodeTemperatureFields::SetpointNodeorNodeListName, node->name().get());
    }

    return idfObject;
  }

}  // namespace energyplus

}  // namespace openstudio
