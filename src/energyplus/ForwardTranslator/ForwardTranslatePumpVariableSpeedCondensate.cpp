/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ForwardTranslator.hpp"

#include "../../model/PumpVariableSpeedCondensate.hpp"
#include "../../model/PumpVariableSpeedCondensate_Impl.hpp"
#include "../../model/Model.hpp"
#include "../../model/Schedule.hpp"
#include "../../model/Schedule_Impl.hpp"
#include "../../model/Node.hpp"
#include "../../model/Node_Impl.hpp"
#include "../../model/Curve.hpp"
#include "../../model/Curve_Impl.hpp"

#include "../../utilities/core/Assert.hpp"

#include <utilities/idd/Pump_VariableSpeed_Condensate_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

using namespace std;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translatePumpVariableSpeedCondensate(PumpVariableSpeedCondensate& modelObject) {
    boost::optional<std::string> s;
    boost::optional<double> value;
    OptionalSchedule schedule;

    IdfObject idfObject(IddObjectType::Pump_VariableSpeed_Condensate);

    m_idfObjects.push_back(idfObject);

    // Name

    s = modelObject.name();
    if (s) {
      idfObject.setName(*s);
    }

    // InletNodeName

    if (boost::optional<ModelObject> mo = modelObject.inletModelObject()) {
      if (boost::optional<Node> node = mo->optionalCast<Node>()) {
        idfObject.setString(Pump_VariableSpeed_CondensateFields::InletNodeName, node->name().get());
      }
    }

    // OutletNodeName

    if (boost::optional<ModelObject> mo = modelObject.outletModelObject()) {
      if (boost::optional<Node> node = mo->optionalCast<Node>()) {
        idfObject.setString(Pump_VariableSpeed_CondensateFields::OutletNodeName, node->name().get());
      }
    }

    // RatedSteamVolumeFlowRate

    if (modelObject.isRatedSteamVolumeFlowRateAutosized()) {
      idfObject.setString(Pump_VariableSpeed_CondensateFields::DesignSteamVolumeFlowRate, "Autosize");
    } else if ((value = modelObject.ratedSteamVolumeFlowRate())) {
      idfObject.setDouble(Pump_VariableSpeed_CondensateFields::DesignSteamVolumeFlowRate, value.get());
    }

    // RatedPumpHead

    if ((value = modelObject.ratedPumpHead())) {
      idfObject.setDouble(Pump_VariableSpeed_CondensateFields::DesignPumpHead, value.get());
    }

    // RatedPowerConsumption

    if (modelObject.isRatedPowerConsumptionAutosized()) {
      idfObject.setString(Pump_VariableSpeed_CondensateFields::DesignPowerConsumption, "Autosize");
    } else if ((value = modelObject.ratedPowerConsumption())) {
      idfObject.setDouble(Pump_VariableSpeed_CondensateFields::DesignPowerConsumption, value.get());
    }

    // MotorEfficiency

    if ((value = modelObject.motorEfficiency())) {
      idfObject.setDouble(Pump_VariableSpeed_CondensateFields::MotorEfficiency, value.get());
    }

    // FractionofMotorInefficienciestoFluidStream

    if ((value = modelObject.fractionofMotorInefficienciestoFluidStream())) {
      idfObject.setDouble(Pump_VariableSpeed_CondensateFields::FractionofMotorInefficienciestoFluidStream, value.get());
    }

    // Coefficient1ofthePartLoadPerformanceCurve

    if ((value = modelObject.coefficient1ofthePartLoadPerformanceCurve())) {
      idfObject.setDouble(Pump_VariableSpeed_CondensateFields::Coefficient1ofthePartLoadPerformanceCurve, value.get());
    }

    // Coefficient2ofthePartLoadPerformanceCurve

    if ((value = modelObject.coefficient2ofthePartLoadPerformanceCurve())) {
      idfObject.setDouble(Pump_VariableSpeed_CondensateFields::Coefficient2ofthePartLoadPerformanceCurve, value.get());
    }

    // Coefficient3ofthePartLoadPerformanceCurve

    if ((value = modelObject.coefficient3ofthePartLoadPerformanceCurve())) {
      idfObject.setDouble(Pump_VariableSpeed_CondensateFields::Coefficient3ofthePartLoadPerformanceCurve, value.get());
    }

    // Coefficient4ofthePartLoadPerformanceCurve

    if ((value = modelObject.coefficient4ofthePartLoadPerformanceCurve())) {
      idfObject.setDouble(Pump_VariableSpeed_CondensateFields::Coefficient4ofthePartLoadPerformanceCurve, value.get());
    }

    // PumpFlowRateSchedule

    if ((schedule = modelObject.pumpFlowRateSchedule())) {
      idfObject.setString(Pump_VariableSpeed_CondensateFields::PumpFlowRateScheduleName, schedule->name().get());
    }

    // SkinLossRadiativeFraction

    if ((value = modelObject.skinLossRadiativeFraction())) {
      idfObject.setDouble(Pump_VariableSpeed_CondensateFields::SkinLossRadiativeFraction, value.get());
    }

    // DesignPowerSizingMethod

    {
      s = modelObject.designPowerSizingMethod();
      idfObject.setString(Pump_VariableSpeed_CondensateFields::DesignPowerSizingMethod, s.get());
    }

    // DesignElectricPowerperUnitFlowRate

    {
      value = modelObject.designElectricPowerPerUnitFlowRate();
      idfObject.setDouble(Pump_VariableSpeed_CondensateFields::DesignElectricPowerperUnitFlowRate, value.get());
    }

    // DesignShaftPowerperUnitFlowRateperUnitHead

    {
      value = modelObject.designShaftPowerPerUnitFlowRatePerUnitHead();
      idfObject.setDouble(Pump_VariableSpeed_CondensateFields::DesignShaftPowerperUnitFlowRateperUnitHead, value.get());
    }

    // EndUseSubcategory

    idfObject.setString(Pump_VariableSpeed_CondensateFields::EndUseSubcategory, modelObject.endUseSubcategory());

    return idfObject;
  }

}  // namespace energyplus
}  // namespace openstudio
