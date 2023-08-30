/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ForwardTranslator.hpp"
#include "../../model/SetpointManagerSingleZoneOneStageCooling.hpp"
#include "../../model/ThermalZone.hpp"
#include "../../model/Node.hpp"
#include "../../model/Node_Impl.hpp"
#include <utilities/idd/SetpointManager_SingleZone_OneStageCooling_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject>
    ForwardTranslator::translateSetpointManagerSingleZoneOneStageCooling(SetpointManagerSingleZoneOneStageCooling& modelObject) {
    std::string s;
    double n;
    boost::optional<Node> node;

    // Name
    IdfObject idfObject = createRegisterAndNameIdfObject(openstudio::IddObjectType::SetpointManager_SingleZone_OneStageCooling, modelObject);

    // CoolingStageOnSupplyAirSetpointTemperature
    n = modelObject.coolingStageOnSupplyAirSetpointTemperature();
    idfObject.setDouble(SetpointManager_SingleZone_OneStageCoolingFields::CoolingStageOnSupplyAirSetpointTemperature, n);

    // CoolingStageOffSupplyAirSetpointTemperature
    n = modelObject.coolingStageOffSupplyAirSetpointTemperature();
    idfObject.setDouble(SetpointManager_SingleZone_OneStageCoolingFields::CoolingStageOffSupplyAirSetpointTemperature, n);

    // ControlZoneName
    boost::optional<ThermalZone> thermalZone = modelObject.controlZone();
    if (thermalZone) {
      idfObject.setString(SetpointManager_SingleZone_OneStageCoolingFields::ControlZoneName, thermalZone->name().get());
    }

    // SetpointNodeorNodeListName
    node = modelObject.setpointNode();
    if (node) {
      idfObject.setString(SetpointManager_SingleZone_OneStageCoolingFields::SetpointNodeorNodeListName, node->name().get());
    }

    return idfObject;
  }

}  // namespace energyplus

}  // namespace openstudio
