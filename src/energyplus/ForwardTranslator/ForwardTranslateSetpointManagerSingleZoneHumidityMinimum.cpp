/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ForwardTranslator.hpp"
#include "../../model/SetpointManagerSingleZoneHumidityMinimum.hpp"
#include "../../model/ThermalZone.hpp"
#include "../../model/Node.hpp"
#include "../../model/Node_Impl.hpp"
#include "../../model/PortList.hpp"
#include <utilities/idd/SetpointManager_SingleZone_Humidity_Minimum_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject>
    ForwardTranslator::translateSetpointManagerSingleZoneHumidityMinimum(SetpointManagerSingleZoneHumidityMinimum& modelObject) {
    boost::optional<double> d;

    // Name
    IdfObject idfObject = createRegisterAndNameIdfObject(openstudio::IddObjectType::SetpointManager_SingleZone_Humidity_Minimum, modelObject);

    // ControlVariable \deprecated
    // ScheduleName \deprecated

    // ControlZoneAirNodeName
    if (boost::optional<ThermalZone> thermalZone = modelObject.controlZone()) {
      auto node = thermalZone->zoneAirNode();
      idfObject.setString(SetpointManager_SingleZone_Humidity_MinimumFields::ControlZoneAirNodeName, node.name().get());
    }

    // Setpoint Node or NodeList Name
    if (boost::optional<Node> node = modelObject.setpointNode()) {
      idfObject.setString(SetpointManager_SingleZone_Humidity_MinimumFields::SetpointNodeorNodeListName, node->name().get());
    }

    return idfObject;
  }

}  // namespace energyplus

}  // namespace openstudio
