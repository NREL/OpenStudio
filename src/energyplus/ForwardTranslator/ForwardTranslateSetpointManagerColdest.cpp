/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ForwardTranslator.hpp"
#include "../../model/SetpointManagerColdest.hpp"
#include "../../model/Node.hpp"
#include "../../model/AirLoopHVAC.hpp"
#include <utilities/idd/SetpointManager_Coldest_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateSetpointManagerColdest(SetpointManagerColdest& modelObject) {
    boost::optional<Node> node;
    boost::optional<double> value;
    std::string s;

    // Name
    IdfObject idfObject = createRegisterAndNameIdfObject(openstudio::IddObjectType::SetpointManager_Coldest, modelObject);

    // ControlVariable
    s = modelObject.controlVariable();
    idfObject.setString(SetpointManager_ColdestFields::ControlVariable, s);

    // MinimumSetpointTemperature
    value = modelObject.minimumSetpointTemperature();
    if (value) {
      idfObject.setDouble(SetpointManager_ColdestFields::MinimumSetpointTemperature, value.get());
    }

    // MaximumSetpointTemperature
    value = modelObject.maximumSetpointTemperature();
    if (value) {
      idfObject.setDouble(SetpointManager_ColdestFields::MaximumSetpointTemperature, value.get());
    }

    // Strategy
    s = modelObject.strategy();
    idfObject.setString(SetpointManager_ColdestFields::Strategy, s);

    // SetpointNodeorNodeListName
    node = modelObject.setpointNode();
    if (node) {
      idfObject.setString(SetpointManager_ColdestFields::SetpointNodeorNodeListName, node->name().get());

      // HVACAirLoopName
      if (boost::optional<model::AirLoopHVAC> airLoop = node->airLoopHVAC()) {
        idfObject.setString(SetpointManager_ColdestFields::HVACAirLoopName, airLoop->name().get());
      }
    }

    return idfObject;
  }

}  // namespace energyplus

}  // namespace openstudio
