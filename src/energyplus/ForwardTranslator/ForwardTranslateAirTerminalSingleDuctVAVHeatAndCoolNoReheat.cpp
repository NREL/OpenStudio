/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

// copy from AirTerminalSingleDuctVAVReheat
#include "../ForwardTranslator.hpp"
#include "../../model/Model.hpp"
#include "../../model/AirTerminalSingleDuctVAVHeatAndCoolNoReheat.hpp"
#include "../../model/AirTerminalSingleDuctVAVHeatAndCoolNoReheat_Impl.hpp"
#include "../../model/HVACComponent.hpp"
#include "../../model/Schedule.hpp"
#include "../../model/Schedule_Impl.hpp"
#include "../../model/Node.hpp"
#include "../../model/Node_Impl.hpp"
#include <utilities/idd/AirTerminal_SingleDuct_VAV_HeatAndCool_NoReheat_FieldEnums.hxx>
#include <utilities/idd/ZoneHVAC_AirDistributionUnit_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/IddFactory.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject>
    ForwardTranslator::translateAirTerminalSingleDuctVAVHeatAndCoolNoReheat(AirTerminalSingleDuctVAVHeatAndCoolNoReheat& modelObject) {
    OptionalModelObject temp;
    OptionalString optS;
    boost::optional<std::string> s;
    boost::optional<double> value;

    std::string baseName = modelObject.name().get();

    IdfObject _airDistributionUnit(openstudio::IddObjectType::ZoneHVAC_AirDistributionUnit);
    _airDistributionUnit.setName("ADU " + baseName);  //ADU: Air Distribution Unit

    IdfObject idfObject(openstudio::IddObjectType::AirTerminal_SingleDuct_VAV_HeatAndCool_NoReheat);
    idfObject.setName(baseName);
    m_idfObjects.push_back(_airDistributionUnit);
    m_idfObjects.push_back(idfObject);

    boost::optional<std::string> inletNodeName;
    boost::optional<std::string> outletNodeName;

    if (boost::optional<ModelObject> inletModelObject = modelObject.inletModelObject()) {
      if (boost::optional<Node> inletNode = inletModelObject->optionalCast<Node>()) {
        inletNodeName = inletNode->name().get();
      }
    }

    if (boost::optional<ModelObject> outletModelObject = modelObject.outletModelObject()) {
      if (boost::optional<Node> outletNode = outletModelObject->optionalCast<Node>()) {
        outletNodeName = outletNode->name().get();
      }
    }

    if (outletNodeName && inletNodeName) {
      idfObject.setString(AirTerminal_SingleDuct_VAV_HeatAndCool_NoReheatFields::AirOutletNodeName, outletNodeName.get());
      idfObject.setString(AirTerminal_SingleDuct_VAV_HeatAndCool_NoReheatFields::AirInletNodeName, inletNodeName.get());
    }

    // AvailabilityScheduleName
    if (auto schedule = modelObject.availabilitySchedule()) {
      if (auto _schedule = translateAndMapModelObject(schedule.get())) {
        idfObject.setString(AirTerminal_SingleDuct_VAV_HeatAndCool_NoReheatFields::AvailabilityScheduleName, _schedule->name().get());
      }
    }

    // MaximumAirFlowRate
    if (modelObject.isMaximumAirFlowRateAutosized()) {
      idfObject.setString(AirTerminal_SingleDuct_VAV_HeatAndCool_NoReheatFields::MaximumAirFlowRate, "AutoSize");
    } else if ((value = modelObject.maximumAirFlowRate())) {
      idfObject.setDouble(AirTerminal_SingleDuct_VAV_HeatAndCool_NoReheatFields::MaximumAirFlowRate, value.get());
    }

    // ZoneMinimumAirFlowFraction
    if ((value = modelObject.zoneMinimumAirFlowFraction())) {
      idfObject.setDouble(AirTerminal_SingleDuct_VAV_HeatAndCool_NoReheatFields::ZoneMinimumAirFlowFraction, value.get());
    }

    // MinimumAirFlowTurndownScheduleName
    if (boost::optional<Schedule> minAirFlowTurndownSchedule = modelObject.minimumAirFlowTurndownSchedule()) {
      if (boost::optional<IdfObject> _minAirFlowTurndownSchedule = translateAndMapModelObject(minAirFlowTurndownSchedule.get())) {
        idfObject.setString(AirTerminal_SingleDuct_VAV_HeatAndCool_NoReheatFields::MinimumAirFlowTurndownScheduleName,
                            _minAirFlowTurndownSchedule->nameString());
      }
    }

    // Populate fields for AirDistributionUnit
    if (boost::optional<ModelObject> outletNode = modelObject.outletModelObject()) {
      _airDistributionUnit.setString(ZoneHVAC_AirDistributionUnitFields::AirDistributionUnitOutletNodeName, outletNode->name().get());
    }
    _airDistributionUnit.setString(ZoneHVAC_AirDistributionUnitFields::AirTerminalObjectType, idfObject.iddObject().name());
    _airDistributionUnit.setString(ZoneHVAC_AirDistributionUnitFields::AirTerminalName, idfObject.name().get());

    return _airDistributionUnit;
  }

}  // namespace energyplus

}  // namespace openstudio
