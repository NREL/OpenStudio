/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ForwardTranslator.hpp"
#include "../../model/SetpointManagerMultiZoneMinimumHumidityAverage.hpp"
#include "../../model/Node.hpp"
#include "../../model/Node_Impl.hpp"
#include <utilities/idd/SetpointManager_MultiZone_MinimumHumidity_Average_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject>
    ForwardTranslator::translateSetpointManagerMultiZoneMinimumHumidityAverage(SetpointManagerMultiZoneMinimumHumidityAverage& modelObject) {
    boost::optional<double> d;

    // Name
    IdfObject idfObject = createRegisterAndNameIdfObject(openstudio::IddObjectType::SetpointManager_MultiZone_MinimumHumidity_Average, modelObject);

    // HVACAirLoopName
    if (boost::optional<AirLoopHVAC> airloop = modelObject.airLoopHVAC()) {
      idfObject.setString(SetpointManager_MultiZone_MinimumHumidity_AverageFields::HVACAirLoopName, airloop->name().get());
    }

    // Minimum Setpoint Humidity Ratio
    d = modelObject.minimumSetpointHumidityRatio();
    if (d) {
      idfObject.setDouble(SetpointManager_MultiZone_MinimumHumidity_AverageFields::MinimumSetpointHumidityRatio, d.get());
    }

    // Maximum Setpoint Humidity Ratio
    d = modelObject.maximumSetpointHumidityRatio();
    if (d) {
      idfObject.setDouble(SetpointManager_MultiZone_MinimumHumidity_AverageFields::MaximumSetpointHumidityRatio, d.get());
    }

    // Setpoint Node or NodeList Name
    if (boost::optional<Node> node = modelObject.setpointNode()) {
      idfObject.setString(SetpointManager_MultiZone_MinimumHumidity_AverageFields::SetpointNodeorNodeListName, node->name().get());
    }

    return idfObject;
  }

}  // namespace energyplus

}  // namespace openstudio
