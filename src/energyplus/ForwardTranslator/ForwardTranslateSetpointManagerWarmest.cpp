/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ForwardTranslator.hpp"
#include "../../model/SetpointManagerWarmest.hpp"
#include "../../model/Node.hpp"
#include "../../model/AirLoopHVAC.hpp"
#include <utilities/idd/SetpointManager_Warmest_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateSetpointManagerWarmest(SetpointManagerWarmest& modelObject) {
    boost::optional<Node> node;
    boost::optional<double> value;
    std::string s;

    IdfObject idfObject(IddObjectType::SetpointManager_Warmest);

    // Name
    s = modelObject.name().get();
    idfObject.setString(SetpointManager_WarmestFields::Name, s);

    // ControlVariable
    s = modelObject.controlVariable();
    idfObject.setString(SetpointManager_WarmestFields::ControlVariable, s);

    // MinimumSetpointTemperature
    value = modelObject.minimumSetpointTemperature();
    if (value) {
      idfObject.setDouble(SetpointManager_WarmestFields::MinimumSetpointTemperature, value.get());
    }

    // MaximumSetpointTemperature
    value = modelObject.maximumSetpointTemperature();
    if (value) {
      idfObject.setDouble(SetpointManager_WarmestFields::MaximumSetpointTemperature, value.get());
    }

    // Strategy
    s = modelObject.strategy();
    idfObject.setString(SetpointManager_WarmestFields::Strategy, s);

    // SetpointNodeorNodeListName
    node = modelObject.setpointNode();
    if (node) {
      idfObject.setString(SetpointManager_WarmestFields::SetpointNodeorNodeListName, node->name().get());

      // HVACAirLoopName
      if (boost::optional<model::AirLoopHVAC> airLoop = node->airLoopHVAC()) {
        idfObject.setString(SetpointManager_WarmestFields::HVACAirLoopName, airLoop->name().get());
      }
    }

    m_idfObjects.push_back(idfObject);

    return idfObject;
  }

}  // namespace energyplus

}  // namespace openstudio
