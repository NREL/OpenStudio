/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ForwardTranslator.hpp"
#include "../../model/Model.hpp"
#include "../../model/AirTerminalDualDuctVAV.hpp"
#include "../../model/AirTerminalDualDuctVAV_Impl.hpp"
#include "../../model/HVACComponent.hpp"
#include "../../model/Schedule.hpp"
#include "../../model/Schedule_Impl.hpp"
#include "../../model/Node.hpp"
#include "../../model/Node_Impl.hpp"
#include "../../model/DesignSpecificationOutdoorAir.hpp"
#include "../../model/DesignSpecificationOutdoorAir_Impl.hpp"
#include <utilities/idd/AirTerminal_DualDuct_VAV_FieldEnums.hxx>
#include <utilities/idd/ZoneHVAC_AirDistributionUnit_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/IddFactory.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateAirTerminalDualDuctVAV(AirTerminalDualDuctVAV& modelObject) {
    OptionalModelObject temp;
    OptionalString optS;
    boost::optional<std::string> s;

    std::string baseName = modelObject.name().get();

    IdfObject _airDistributionUnit(openstudio::IddObjectType::ZoneHVAC_AirDistributionUnit);
    _airDistributionUnit.setName("ADU " + baseName);  //ADU: Air Distribution Unit

    IdfObject idfObject(openstudio::IddObjectType::AirTerminal_DualDuct_VAV);
    idfObject.setName(baseName);

    m_idfObjects.push_back(_airDistributionUnit);
    m_idfObjects.push_back(idfObject);

    if (auto schedule = modelObject.availabilitySchedule()) {
      if (auto idf = translateAndMapModelObject(schedule.get())) {
        idfObject.setString(AirTerminal_DualDuct_VAVFields::AvailabilityScheduleName, idf->name().get());
      }
    }

    if (auto mo = modelObject.outletModelObject()) {
      idfObject.setString(AirTerminal_DualDuct_VAVFields::AirOutletNodeName, mo->name().get());
    }

    if (auto mo = modelObject.inletModelObject(0)) {
      idfObject.setString(AirTerminal_DualDuct_VAVFields::HotAirInletNodeName, mo->name().get());
    }

    if (auto mo = modelObject.inletModelObject(1)) {
      idfObject.setString(AirTerminal_DualDuct_VAVFields::ColdAirInletNodeName, mo->name().get());
    }

    if (modelObject.isMaximumDamperAirFlowRateAutosized()) {
      idfObject.setString(AirTerminal_DualDuct_VAVFields::MaximumDamperAirFlowRate, "Autosize");
    } else if (auto value = modelObject.maximumDamperAirFlowRate()) {
      idfObject.setDouble(AirTerminal_DualDuct_VAVFields::MaximumDamperAirFlowRate, value.get());
    }

    {
      auto value = modelObject.zoneMinimumAirFlowFraction();
      idfObject.setDouble(AirTerminal_DualDuct_VAVFields::ZoneMinimumAirFlowFraction, value);
    }

    if (auto designOA = modelObject.designSpecificationOutdoorAirObject()) {
      if (auto idf = translateAndMapModelObject(designOA.get())) {
        idfObject.setString(AirTerminal_DualDuct_VAVFields::DesignSpecificationOutdoorAirObjectName, idf->name().get());
      }
    }

    // MinimumAirFlowTurndownScheduleName
    if (boost::optional<Schedule> minAirFlowTurndownSchedule = modelObject.minimumAirFlowTurndownSchedule()) {
      if (boost::optional<IdfObject> _minAirFlowTurndownSchedule = translateAndMapModelObject(minAirFlowTurndownSchedule.get())) {
        idfObject.setString(AirTerminal_DualDuct_VAVFields::MinimumAirFlowTurndownScheduleName, _minAirFlowTurndownSchedule->nameString());
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
