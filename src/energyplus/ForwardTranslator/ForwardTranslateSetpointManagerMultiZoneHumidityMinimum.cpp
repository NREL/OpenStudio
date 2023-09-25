/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ForwardTranslator.hpp"
#include "../../model/SetpointManagerMultiZoneHumidityMinimum.hpp"
#include "../../model/Node.hpp"
#include "../../model/Node_Impl.hpp"
#include <utilities/idd/SetpointManager_MultiZone_Humidity_Minimum_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject>
    ForwardTranslator::translateSetpointManagerMultiZoneHumidityMinimum(SetpointManagerMultiZoneHumidityMinimum& modelObject) {
    boost::optional<double> d;

    // Name
    IdfObject idfObject = createRegisterAndNameIdfObject(openstudio::IddObjectType::SetpointManager_MultiZone_Humidity_Minimum, modelObject);

    // HVACAirLoopName
    if (boost::optional<AirLoopHVAC> airloop = modelObject.airLoopHVAC()) {
      idfObject.setString(SetpointManager_MultiZone_Humidity_MinimumFields::HVACAirLoopName, airloop->name().get());
    }

    // Minimum Setpoint Humidity Ratio
    d = modelObject.minimumSetpointHumidityRatio();
    if (d) {
      idfObject.setDouble(SetpointManager_MultiZone_Humidity_MinimumFields::MinimumSetpointHumidityRatio, d.get());
    }

    // Maximum Setpoint Humidity Ratio
    d = modelObject.maximumSetpointHumidityRatio();
    if (d) {
      idfObject.setDouble(SetpointManager_MultiZone_Humidity_MinimumFields::MaximumSetpointHumidityRatio, d.get());
    }

    // Setpoint Node or NodeList Name
    if (boost::optional<Node> node = modelObject.setpointNode()) {
      idfObject.setString(SetpointManager_MultiZone_Humidity_MinimumFields::SetpointNodeorNodeListName, node->name().get());
    }

    return idfObject;
  }

}  // namespace energyplus

}  // namespace openstudio
