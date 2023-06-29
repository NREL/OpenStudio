/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ForwardTranslator.hpp"
#include "../../model/SetpointManagerSingleZoneHeating.hpp"
#include "../../model/ThermalZone.hpp"
#include "../../model/Node.hpp"
#include "../../model/Node_Impl.hpp"
#include "../../model/PortList.hpp"
#include <utilities/idd/SetpointManager_SingleZone_Heating_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateSetpointManagerSingleZoneHeating(SetpointManagerSingleZoneHeating& modelObject) {
    std::string s;
    double n;
    boost::optional<Node> node;

    IdfObject idfObject(IddObjectType::SetpointManager_SingleZone_Heating);

    m_idfObjects.push_back(idfObject);

    // Name
    s = modelObject.name().get();
    idfObject.setString(SetpointManager_SingleZone_HeatingFields::Name, s);

    // ControlVariable
    idfObject.setString(SetpointManager_SingleZone_HeatingFields::ControlVariable, modelObject.controlVariable());

    // MinimumSupplyAirTemperature
    n = modelObject.minimumSupplyAirTemperature();
    idfObject.setDouble(SetpointManager_SingleZone_HeatingFields::MinimumSupplyAirTemperature, n);

    // MaximumSupplyAirTemperature
    n = modelObject.maximumSupplyAirTemperature();
    idfObject.setDouble(SetpointManager_SingleZone_HeatingFields::MaximumSupplyAirTemperature, n);

    // ControlZoneName
    auto thermalZone = modelObject.controlZone();
    if (thermalZone) {
      idfObject.setString(SetpointManager_SingleZone_HeatingFields::ControlZoneName, thermalZone->name().get());
    }

    // ZoneNodeName
    if (thermalZone) {
      node = thermalZone->zoneAirNode();
      idfObject.setString(SetpointManager_SingleZone_HeatingFields::ZoneNodeName, node->name().get());
    }

    // ZoneInletNodeName
    if (thermalZone) {
      if (boost::optional<ModelObject> mo = thermalZone->inletPortList().airLoopHVACModelObject()) {
        if ((node = mo->optionalCast<Node>())) {
          idfObject.setString(SetpointManager_SingleZone_HeatingFields::ZoneInletNodeName, node->name().get());
        }
      }
    }

    // SetpointNodeorNodeListName
    node = modelObject.setpointNode();
    if (node) {
      idfObject.setString(SetpointManager_SingleZone_HeatingFields::SetpointNodeorNodeListName, node->name().get());
    }

    return idfObject;
  }

}  // namespace energyplus

}  // namespace openstudio
