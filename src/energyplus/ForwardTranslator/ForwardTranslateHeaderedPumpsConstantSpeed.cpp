/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ForwardTranslator.hpp"
#include "../../model/HeaderedPumpsConstantSpeed.hpp"
#include "../../model/HeaderedPumpsConstantSpeed_Impl.hpp"
#include "../../model/Model.hpp"
#include "../../model/Schedule.hpp"
#include "../../model/Schedule_Impl.hpp"
#include "../../model/Node.hpp"
#include "../../model/Node_Impl.hpp"
#include "../../model/ThermalZone.hpp"
#include "../../model/ThermalZone_Impl.hpp"
#include "../../utilities/core/Assert.hpp"
#include <utilities/idd/HeaderedPumps_ConstantSpeed_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;
using namespace std;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateHeaderedPumpsConstantSpeed(HeaderedPumpsConstantSpeed& modelObject) {
    IdfObject idfObject(IddObjectType::HeaderedPumps_ConstantSpeed);
    m_idfObjects.push_back(idfObject);

    // Name
    if (auto s = modelObject.name()) {
      idfObject.setName(*s);
    }

    // InletNodeName
    if (auto mo = modelObject.inletModelObject()) {
      if (auto node = mo->optionalCast<Node>()) {
        idfObject.setString(HeaderedPumps_ConstantSpeedFields::InletNodeName, node->name().get());
      }
    }

    // OutletNodeName
    if (auto mo = modelObject.outletModelObject()) {
      if (auto node = mo->optionalCast<Node>()) {
        idfObject.setString(HeaderedPumps_ConstantSpeedFields::OutletNodeName, node->name().get());
      }
    }

    // TotalRatedFlowRate
    if (modelObject.isTotalRatedFlowRateAutosized()) {
      idfObject.setString(HeaderedPumps_ConstantSpeedFields::TotalDesignFlowRate, "Autosize");
    } else if (auto value = modelObject.totalRatedFlowRate()) {
      idfObject.setDouble(HeaderedPumps_ConstantSpeedFields::TotalDesignFlowRate, value.get());
    }

    // NumberofPumpsinBank
    {
      auto value = modelObject.numberofPumpsinBank();
      idfObject.setInt(HeaderedPumps_ConstantSpeedFields::NumberofPumpsinBank, value);
    }

    // FlowSequencingControlScheme
    {
      auto value = modelObject.flowSequencingControlScheme();
      idfObject.setString(HeaderedPumps_ConstantSpeedFields::FlowSequencingControlScheme, value);
    }

    // RatedPumpHead
    {
      auto value = modelObject.ratedPumpHead();
      idfObject.setDouble(HeaderedPumps_ConstantSpeedFields::DesignPumpHead, value);
    }

    // RatedPowerConsumption
    if (modelObject.isRatedPowerConsumptionAutosized()) {
      idfObject.setString(HeaderedPumps_ConstantSpeedFields::DesignPowerConsumption, "Autosize");
    } else if (auto value = modelObject.ratedPowerConsumption()) {
      idfObject.setDouble(HeaderedPumps_ConstantSpeedFields::DesignPowerConsumption, value.get());
    }

    // MotorEfficiency
    {
      auto value = modelObject.motorEfficiency();
      idfObject.setDouble(HeaderedPumps_ConstantSpeedFields::MotorEfficiency, value);
    }

    // FractionofMotorInefficienciestoFluidStream
    {
      auto value = modelObject.fractionofMotorInefficienciestoFluidStream();
      idfObject.setDouble(HeaderedPumps_ConstantSpeedFields::FractionofMotorInefficienciestoFluidStream, value);
    }

    // PumpControlType
    {
      auto value = modelObject.pumpControlType();
      idfObject.setString(HeaderedPumps_ConstantSpeedFields::PumpControlType, value);
    }

    // PumpFlowRateScheduleName
    if (auto s = modelObject.pumpFlowRateSchedule()) {
      idfObject.setString(HeaderedPumps_ConstantSpeedFields::PumpFlowRateScheduleName, s->name().get());
    }

    // ZoneName
    if (auto z = modelObject.thermalZone()) {
      idfObject.setString(HeaderedPumps_ConstantSpeedFields::ZoneName, z->name().get());
    }

    // SkinLossRadiativeFraction
    {
      auto value = modelObject.skinLossRadiativeFraction();
      idfObject.setDouble(HeaderedPumps_ConstantSpeedFields::SkinLossRadiativeFraction, value);
    }

    {
      auto s = modelObject.designPowerSizingMethod();
      idfObject.setString(HeaderedPumps_ConstantSpeedFields::DesignPowerSizingMethod, s);
    }

    {
      auto value = modelObject.designElectricPowerPerUnitFlowRate();
      idfObject.setDouble(HeaderedPumps_ConstantSpeedFields::DesignElectricPowerperUnitFlowRate, value);
    }

    {
      auto value = modelObject.designShaftPowerPerUnitFlowRatePerUnitHead();
      idfObject.setDouble(HeaderedPumps_ConstantSpeedFields::DesignShaftPowerperUnitFlowRateperUnitHead, value);
    }

    // End Use Subcategory
    idfObject.setString(HeaderedPumps_ConstantSpeedFields::EndUseSubcategory, modelObject.endUseSubcategory());

    return idfObject;
  }

}  // namespace energyplus
}  // namespace openstudio
