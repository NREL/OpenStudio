/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ForwardTranslator.hpp"
#include "../../model/SetpointManagerWarmestTemperatureFlow.hpp"
#include "../../model/ThermalZone.hpp"
#include "../../model/Node.hpp"
#include "../../model/Node_Impl.hpp"
#include "../../model/PortList.hpp"
#include <utilities/idd/SetpointManager_WarmestTemperatureFlow_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateSetpointManagerWarmestTemperatureFlow(SetpointManagerWarmestTemperatureFlow& modelObject) {
    std::string s;
    double n;
    boost::optional<Node> node;

    IdfObject idfObject(IddObjectType::SetpointManager_WarmestTemperatureFlow);

    m_idfObjects.push_back(idfObject);

    // Name
    s = modelObject.name().get();
    idfObject.setString(SetpointManager_WarmestTemperatureFlowFields::Name, s);

    // ControlVariable
    idfObject.setString(SetpointManager_WarmestTemperatureFlowFields::ControlVariable, "Temperature");

    // HVACAirLoopName
    if (auto node = modelObject.setpointNode()) {
      if (auto airLoop = node->airLoopHVAC()) {
        idfObject.setString(SetpointManager_WarmestTemperatureFlowFields::HVACAirLoopName, airLoop->name().get());
      }
    }

    // Strategy
    s = modelObject.strategy();
    idfObject.setString(SetpointManager_WarmestTemperatureFlowFields::Strategy, s);

    // MinimumSupplyAirTemperature
    n = modelObject.minimumSetpointTemperature();
    idfObject.setDouble(SetpointManager_WarmestTemperatureFlowFields::MinimumSetpointTemperature, n);

    // MaximumSupplyAirTemperature
    n = modelObject.maximumSetpointTemperature();
    idfObject.setDouble(SetpointManager_WarmestTemperatureFlowFields::MaximumSetpointTemperature, n);

    // SetpointNodeorNodeListName
    node = modelObject.setpointNode();
    if (node) {
      idfObject.setString(SetpointManager_WarmestTemperatureFlowFields::SetpointNodeorNodeListName, node->name().get());
    }

    // MinimumTurndownRatio
    n = modelObject.minimumTurndownRatio();
    idfObject.setDouble(SetpointManager_WarmestTemperatureFlowFields::MinimumTurndownRatio, n);

    return idfObject;
  }

}  // namespace energyplus

}  // namespace openstudio
