/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ForwardTranslator.hpp"
#include "../../model/SetpointManagerSingleZoneReheat.hpp"
#include "../../model/ThermalZone.hpp"
#include "../../model/Node.hpp"
#include "../../model/Node_Impl.hpp"
#include "../../model/PortList.hpp"
#include <utilities/idd/SetpointManager_SingleZone_Reheat_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateSetpointManagerSingleZoneReheat(SetpointManagerSingleZoneReheat& modelObject) {
    std::string s;
    double n;
    boost::optional<Node> node;

    IdfObject idfObject(IddObjectType::SetpointManager_SingleZone_Reheat);

    m_idfObjects.push_back(idfObject);

    // Name
    s = modelObject.name().get();
    idfObject.setString(SetpointManager_SingleZone_ReheatFields::Name, s);

    // ControlVariable
    idfObject.setString(SetpointManager_SingleZone_ReheatFields::ControlVariable, "Temperature");

    // MinimumSupplyAirTemperature
    n = modelObject.minimumSupplyAirTemperature();
    idfObject.setDouble(SetpointManager_SingleZone_ReheatFields::MinimumSupplyAirTemperature, n);

    // MaximumSupplyAirTemperature
    n = modelObject.maximumSupplyAirTemperature();
    idfObject.setDouble(SetpointManager_SingleZone_ReheatFields::MaximumSupplyAirTemperature, n);

    // ControlZoneName
    boost::optional<ThermalZone> thermalZone = modelObject.controlZone();
    if (thermalZone) {
      idfObject.setString(SetpointManager_SingleZone_ReheatFields::ControlZoneName, thermalZone->name().get());
    }

    // ZoneNodeName
    if (thermalZone) {
      node = thermalZone->zoneAirNode();
      idfObject.setString(SetpointManager_SingleZone_ReheatFields::ZoneNodeName, node->name().get());
    }

    // ZoneInletNodeName
    if (thermalZone) {
      if (boost::optional<ModelObject> mo = thermalZone->inletPortList().airLoopHVACModelObject()) {
        if ((node = mo->optionalCast<Node>())) {
          idfObject.setString(SetpointManager_SingleZone_ReheatFields::ZoneInletNodeName, node->name().get());
        }
      }
    }

    // SetpointNodeorNodeListName
    node = modelObject.setpointNode();
    if (node) {
      idfObject.setString(SetpointManager_SingleZone_ReheatFields::SetpointNodeorNodeListName, node->name().get());
    }

    return idfObject;
  }

}  // namespace energyplus

}  // namespace openstudio
