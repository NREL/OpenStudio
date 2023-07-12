/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ForwardTranslator.hpp"
#include "../../model/SetpointManagerOutdoorAirPretreat.hpp"
#include "../../model/Node.hpp"
#include "../../model/Node_Impl.hpp"
#include "../../model/AirLoopHVACOutdoorAirSystem.hpp"
#include <utilities/idd/SetpointManager_OutdoorAirPretreat_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateSetpointManagerOutdoorAirPretreat(SetpointManagerOutdoorAirPretreat& modelObject) {
    boost::optional<Node> node;
    boost::optional<ModelObject> mo;
    boost::optional<std::string> s;
    boost::optional<double> d;

    boost::optional<Node> referenceNode;
    boost::optional<Node> mixedAirNode;
    boost::optional<Node> outdoorAirNode;
    boost::optional<Node> returnAirNode;

    boost::optional<AirLoopHVAC> airloop = modelObject.airLoopHVAC();
    boost::optional<AirLoopHVACOutdoorAirSystem> outdoorAirSystem;
    if (airloop) {
      outdoorAirSystem = airloop->airLoopHVACOutdoorAirSystem();
    }

    // Name
    IdfObject idfObject = createRegisterAndNameIdfObject(openstudio::IddObjectType::SetpointManager_OutdoorAirPretreat, modelObject);

    // ControlVariable
    s = modelObject.controlVariable();
    if (s) {
      idfObject.setString(SetpointManager_OutdoorAirPretreatFields::ControlVariable, s.get());
    }

    // Minimum Setpoint Temperature
    d = modelObject.minimumSetpointTemperature();
    if (d) {
      idfObject.setDouble(SetpointManager_OutdoorAirPretreatFields::MinimumSetpointTemperature, d.get());
    }

    // Maximum Setpoint Temperature
    d = modelObject.maximumSetpointTemperature();
    if (d) {
      idfObject.setDouble(SetpointManager_OutdoorAirPretreatFields::MaximumSetpointTemperature, d.get());
    }

    // Minimum Setpoint Humidity Ratio
    d = modelObject.minimumSetpointHumidityRatio();
    if (d) {
      idfObject.setDouble(SetpointManager_OutdoorAirPretreatFields::MinimumSetpointHumidityRatio, d.get());
    }

    // Maximum Setpoint Humidity Ratio
    d = modelObject.maximumSetpointHumidityRatio();
    if (d) {
      idfObject.setDouble(SetpointManager_OutdoorAirPretreatFields::MaximumSetpointHumidityRatio, d.get());
    }

    // Reference Setpoint Node Name
    // Mixed Air Stream Node Name
    mixedAirNode = modelObject.mixedAirStreamNode();
    if (!mixedAirNode) {
      if (outdoorAirSystem && (mo = outdoorAirSystem->mixedAirModelObject())) {
        mixedAirNode = mo->optionalCast<Node>();
      }
    }

    referenceNode = modelObject.referenceSetpointNode();
    if (!referenceNode) {
      referenceNode = mixedAirNode;
    }
    if (referenceNode) {
      idfObject.setString(SetpointManager_OutdoorAirPretreatFields::ReferenceSetpointNodeName, referenceNode->name().get());
    }

    if (mixedAirNode) {
      idfObject.setString(SetpointManager_OutdoorAirPretreatFields::MixedAirStreamNodeName, mixedAirNode->name().get());
    }

    // Outdoor Air Stream Node Name
    outdoorAirNode = modelObject.outdoorAirStreamNode();
    if (!outdoorAirNode) {
      if (outdoorAirSystem && (mo = outdoorAirSystem->outdoorAirModelObject())) {
        outdoorAirNode = mo->optionalCast<Node>();
      }
    }
    if (outdoorAirNode) {
      idfObject.setString(SetpointManager_OutdoorAirPretreatFields::OutdoorAirStreamNodeName, outdoorAirNode->name().get());
    }

    // Return Air Stream Node Name
    returnAirNode = modelObject.returnAirStreamNode();
    if (!returnAirNode) {
      if (outdoorAirSystem && (mo = outdoorAirSystem->returnAirModelObject())) {
        returnAirNode = mo->optionalCast<Node>();
      }
    }
    if (returnAirNode) {
      idfObject.setString(SetpointManager_OutdoorAirPretreatFields::ReturnAirStreamNodeName, returnAirNode->name().get());
    }

    // Setpoint Node or NodeList Name
    node = modelObject.setpointNode();
    if (node) {
      idfObject.setString(SetpointManager_OutdoorAirPretreatFields::SetpointNodeorNodeListName, node->name().get());
    }

    return idfObject;
  }

}  // namespace energyplus

}  // namespace openstudio
