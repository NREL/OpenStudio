/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ForwardTranslator.hpp"
#include "../../model/SetpointManagerFollowOutdoorAirTemperature.hpp"
#include "../../model/Node.hpp"
#include <utilities/idd/SetpointManager_FollowOutdoorAirTemperature_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject>
    ForwardTranslator::translateSetpointManagerFollowOutdoorAirTemperature(SetpointManagerFollowOutdoorAirTemperature& modelObject) {
    boost::optional<Node> node;
    boost::optional<std::string> s;
    boost::optional<double> d;

    IdfObject idfObject(IddObjectType::SetpointManager_FollowOutdoorAirTemperature);

    // Name
    s = modelObject.name();
    if (s) {
      idfObject.setString(SetpointManager_FollowOutdoorAirTemperatureFields::Name, s.get());
    }

    // ControlVariable
    s = modelObject.controlVariable();
    if (s) {
      idfObject.setString(SetpointManager_FollowOutdoorAirTemperatureFields::ControlVariable, s.get());
    }

    // SetpointNodeorNodeListName
    node = modelObject.setpointNode();
    if (node) {
      idfObject.setString(SetpointManager_FollowOutdoorAirTemperatureFields::SetpointNodeorNodeListName, node->name().get());
    }

    // ReferenceTemperatureType
    s = modelObject.referenceTemperatureType();
    if (s) {
      idfObject.setString(SetpointManager_FollowOutdoorAirTemperatureFields::ReferenceTemperatureType, s.get());
    }

    // OffsetTemperatureDifference
    d = modelObject.offsetTemperatureDifference();
    if (d) {
      idfObject.setDouble(SetpointManager_FollowOutdoorAirTemperatureFields::OffsetTemperatureDifference, d.get());
    }

    // MaximumSetpointTemperature
    d = modelObject.maximumSetpointTemperature();
    if (d) {
      idfObject.setDouble(SetpointManager_FollowOutdoorAirTemperatureFields::MaximumSetpointTemperature, d.get());
    }

    // MinimumSetpointTemperature
    d = modelObject.minimumSetpointTemperature();
    if (d) {
      idfObject.setDouble(SetpointManager_FollowOutdoorAirTemperatureFields::MinimumSetpointTemperature, d.get());
    }

    m_idfObjects.push_back(idfObject);

    return idfObject;
  }

}  // namespace energyplus

}  // namespace openstudio
