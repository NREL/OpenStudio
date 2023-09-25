/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ForwardTranslator.hpp"
#include "../../model/SetpointManagerFollowGroundTemperature.hpp"
#include "../../model/Node.hpp"
#include <utilities/idd/SetpointManager_FollowGroundTemperature_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateSetpointManagerFollowGroundTemperature(SetpointManagerFollowGroundTemperature& modelObject) {
    boost::optional<Node> node;
    boost::optional<std::string> s;
    boost::optional<double> d;

    // Name
    IdfObject idfObject = createRegisterAndNameIdfObject(openstudio::IddObjectType::SetpointManager_FollowGroundTemperature, modelObject);

    // ControlVariable
    s = modelObject.controlVariable();
    if (s) {
      idfObject.setString(SetpointManager_FollowGroundTemperatureFields::ControlVariable, s.get());
    }

    // ReferenceGroundTemperatureObjectType
    s = modelObject.referenceGroundTemperatureObjectType();
    if (s) {
      idfObject.setString(SetpointManager_FollowGroundTemperatureFields::ReferenceGroundTemperatureObjectType, s.get());
    }

    // OffsetTemperatureDifference
    d = modelObject.offsetTemperatureDifference();
    if (d) {
      idfObject.setDouble(SetpointManager_FollowGroundTemperatureFields::OffsetTemperatureDifference, d.get());
    }

    // MaximumSetpointTemperature
    d = modelObject.maximumSetpointTemperature();
    if (d) {
      idfObject.setDouble(SetpointManager_FollowGroundTemperatureFields::MaximumSetpointTemperature, d.get());
    }

    // MinimumSetpointTemperature
    d = modelObject.minimumSetpointTemperature();
    if (d) {
      idfObject.setDouble(SetpointManager_FollowGroundTemperatureFields::MinimumSetpointTemperature, d.get());
    }

    // SetpointNodeorNodeListName
    node = modelObject.setpointNode();
    if (node) {
      idfObject.setString(SetpointManager_FollowGroundTemperatureFields::SetpointNodeorNodeListName, node->name().get());
    }

    return idfObject;
  }

}  // namespace energyplus

}  // namespace openstudio
