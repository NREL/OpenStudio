/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ForwardTranslator.hpp"
#include "../../model/HeaderedPumpsVariableSpeed.hpp"
#include "../../model/HeaderedPumpsVariableSpeed_Impl.hpp"
#include "../../model/Model.hpp"
#include "../../model/Schedule.hpp"
#include "../../model/Schedule_Impl.hpp"
#include "../../model/Node.hpp"
#include "../../model/Node_Impl.hpp"
#include "../../model/ThermalZone.hpp"
#include "../../model/ThermalZone_Impl.hpp"
#include "../../utilities/core/Assert.hpp"
#include <utilities/idd/HeaderedPumps_VariableSpeed_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;
using namespace std;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateHeaderedPumpsVariableSpeed(HeaderedPumpsVariableSpeed& modelObject) {
    IdfObject idfObject(IddObjectType::HeaderedPumps_VariableSpeed);
    m_idfObjects.push_back(idfObject);

    // Name
    if (auto s = modelObject.name()) {
      idfObject.setName(*s);
    }

    // InletNodeName
    if (auto mo = modelObject.inletModelObject()) {
      if (auto node = mo->optionalCast<Node>()) {
        idfObject.setString(HeaderedPumps_VariableSpeedFields::InletNodeName, node->name().get());
      }
    }

    // OutletNodeName
    if (auto mo = modelObject.outletModelObject()) {
      if (auto node = mo->optionalCast<Node>()) {
        idfObject.setString(HeaderedPumps_VariableSpeedFields::OutletNodeName, node->name().get());
      }
    }

    // TotalRatedFlowRate
    if (modelObject.isTotalRatedFlowRateAutosized()) {
      idfObject.setString(HeaderedPumps_VariableSpeedFields::TotalDesignFlowRate, "Autosize");
    } else if (auto value = modelObject.totalRatedFlowRate()) {
      idfObject.setDouble(HeaderedPumps_VariableSpeedFields::TotalDesignFlowRate, value.get());
    }

    // NumberofPumpsinBank
    {
      auto value = modelObject.numberofPumpsinBank();
      idfObject.setInt(HeaderedPumps_VariableSpeedFields::NumberofPumpsinBank, value);
    }

    // FlowSequencingControlScheme
    {
      auto value = modelObject.flowSequencingControlScheme();
      idfObject.setString(HeaderedPumps_VariableSpeedFields::FlowSequencingControlScheme, value);
    }

    // RatedPumpHead
    {
      auto value = modelObject.ratedPumpHead();
      idfObject.setDouble(HeaderedPumps_VariableSpeedFields::DesignPumpHead, value);
    }

    // RatedPowerConsumption
    if (modelObject.isRatedPowerConsumptionAutosized()) {
      idfObject.setString(HeaderedPumps_VariableSpeedFields::DesignPowerConsumption, "Autosize");
    } else if (auto value = modelObject.ratedPowerConsumption()) {
      idfObject.setDouble(HeaderedPumps_VariableSpeedFields::DesignPowerConsumption, value.get());
    }

    // MotorEfficiency
    {
      auto value = modelObject.motorEfficiency();
      idfObject.setDouble(HeaderedPumps_VariableSpeedFields::MotorEfficiency, value);
    }

    // FractionofMotorInefficienciestoFluidStream
    {
      auto value = modelObject.fractionofMotorInefficienciestoFluidStream();
      idfObject.setDouble(HeaderedPumps_VariableSpeedFields::FractionofMotorInefficienciestoFluidStream, value);
    }

    // Coefficient1ofthePartLoadPerformanceCurve
    {
      auto value = modelObject.coefficient1ofthePartLoadPerformanceCurve();
      idfObject.setDouble(HeaderedPumps_VariableSpeedFields::Coefficient1ofthePartLoadPerformanceCurve, value);
    }

    // Coefficient2ofthePartLoadPerformanceCurve
    {
      auto value = modelObject.coefficient2ofthePartLoadPerformanceCurve();
      idfObject.setDouble(HeaderedPumps_VariableSpeedFields::Coefficient2ofthePartLoadPerformanceCurve, value);
    }

    // Coefficient3ofthePartLoadPerformanceCurve
    {
      auto value = modelObject.coefficient3ofthePartLoadPerformanceCurve();
      idfObject.setDouble(HeaderedPumps_VariableSpeedFields::Coefficient3ofthePartLoadPerformanceCurve, value);
    }

    // Coefficient4ofthePartLoadPerformanceCurve
    {
      auto value = modelObject.coefficient4ofthePartLoadPerformanceCurve();
      idfObject.setDouble(HeaderedPumps_VariableSpeedFields::Coefficient4ofthePartLoadPerformanceCurve, value);
    }

    // MinimumFlowRateFraction
    {
      auto value = modelObject.minimumFlowRateFraction();
      idfObject.setDouble(HeaderedPumps_VariableSpeedFields::MinimumFlowRateFraction, value);
    }

    // PumpControlType
    {
      auto value = modelObject.pumpControlType();
      idfObject.setString(HeaderedPumps_VariableSpeedFields::PumpControlType, value);
    }

    // PumpFlowRateScheduleName
    if (auto s = modelObject.pumpFlowRateSchedule()) {
      idfObject.setString(HeaderedPumps_VariableSpeedFields::PumpFlowRateScheduleName, s->name().get());
    }

    // ZoneName
    if (auto z = modelObject.thermalZone()) {
      idfObject.setString(HeaderedPumps_VariableSpeedFields::ZoneName, z->name().get());
    }

    // SkinLossRadiativeFraction
    {
      auto value = modelObject.skinLossRadiativeFraction();
      idfObject.setDouble(HeaderedPumps_VariableSpeedFields::SkinLossRadiativeFraction, value);
    }

    {
      auto s = modelObject.designPowerSizingMethod();
      idfObject.setString(HeaderedPumps_VariableSpeedFields::DesignPowerSizingMethod, s);
    }

    {
      auto value = modelObject.designElectricPowerPerUnitFlowRate();
      idfObject.setDouble(HeaderedPumps_VariableSpeedFields::DesignElectricPowerperUnitFlowRate, value);
    }

    {
      auto value = modelObject.designShaftPowerPerUnitFlowRatePerUnitHead();
      idfObject.setDouble(HeaderedPumps_VariableSpeedFields::DesignShaftPowerperUnitFlowRateperUnitHead, value);
    }

    // End Use Subcategory
    idfObject.setString(HeaderedPumps_VariableSpeedFields::EndUseSubcategory, modelObject.endUseSubcategory());

    return idfObject;
  }

}  // namespace energyplus
}  // namespace openstudio
