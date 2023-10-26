/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ForwardTranslator.hpp"
#include "../../model/PumpConstantSpeed.hpp"
#include "../../model/PumpConstantSpeed_Impl.hpp"
#include "../../model/Model.hpp"
#include "../../model/Schedule.hpp"
#include "../../model/Schedule_Impl.hpp"
#include "../../model/Node.hpp"
#include "../../model/Node_Impl.hpp"
#include "../../model/Curve.hpp"
#include "../../model/Curve_Impl.hpp"
#include "../../utilities/core/Assert.hpp"
#include <utilities/idd/Pump_ConstantSpeed_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;
using namespace std;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translatePumpConstantSpeed(PumpConstantSpeed& modelObject) {
    boost::optional<std::string> s;
    boost::optional<double> value;
    OptionalSchedule schedule;

    IdfObject idfObject(IddObjectType::Pump_ConstantSpeed);

    m_idfObjects.push_back(idfObject);

    // Name

    s = modelObject.name();
    if (s) {
      idfObject.setName(*s);
    }

    // InletNodeName

    if (boost::optional<ModelObject> mo = modelObject.inletModelObject()) {
      if (boost::optional<Node> node = mo->optionalCast<Node>()) {
        idfObject.setString(Pump_ConstantSpeedFields::InletNodeName, node->name().get());
      }
    }

    // OutletNodeName

    if (boost::optional<ModelObject> mo = modelObject.outletModelObject()) {
      if (boost::optional<Node> node = mo->optionalCast<Node>()) {
        idfObject.setString(Pump_ConstantSpeedFields::OutletNodeName, node->name().get());
      }
    }

    // RatedFlowRate

    if (modelObject.isRatedFlowRateAutosized()) {
      idfObject.setString(Pump_ConstantSpeedFields::DesignFlowRate, "Autosize");
    } else if ((value = modelObject.ratedFlowRate())) {
      idfObject.setDouble(Pump_ConstantSpeedFields::DesignFlowRate, value.get());
    }

    // RatedPumpHead

    if ((value = modelObject.ratedPumpHead())) {
      idfObject.setDouble(Pump_ConstantSpeedFields::DesignPumpHead, value.get());
    }

    // RatedPowerConsumption

    if (modelObject.isRatedPowerConsumptionAutosized()) {
      idfObject.setString(Pump_ConstantSpeedFields::DesignPowerConsumption, "Autosize");
    } else if ((value = modelObject.ratedPowerConsumption())) {
      idfObject.setDouble(Pump_ConstantSpeedFields::DesignPowerConsumption, value.get());
    }

    // MotorEfficiency

    if ((value = modelObject.motorEfficiency())) {
      idfObject.setDouble(Pump_ConstantSpeedFields::MotorEfficiency, value.get());
    }

    // FractionofMotorInefficienciestoFluidStream

    if ((value = modelObject.fractionofMotorInefficienciestoFluidStream())) {
      idfObject.setDouble(Pump_ConstantSpeedFields::FractionofMotorInefficienciestoFluidStream, value.get());
    }

    // PumpControlType

    if ((s = modelObject.pumpControlType())) {
      idfObject.setString(Pump_ConstantSpeedFields::PumpControlType, s.get());
    }

    // PumpFlowRateSchedule

    if ((schedule = modelObject.pumpFlowRateSchedule())) {
      idfObject.setString(Pump_ConstantSpeedFields::PumpFlowRateScheduleName, schedule->name().get());
    }

    // PumpCurve

    if (OptionalCurve curve = modelObject.pumpCurve()) {
      idfObject.setString(Pump_ConstantSpeedFields::PumpCurveName, curve->name().get());
    }

    // ImpellerDiameter

    if ((value = modelObject.impellerDiameter())) {
      idfObject.setDouble(Pump_ConstantSpeedFields::ImpellerDiameter, value.get());
    }

    // SkinLossRadiativeFraction

    if ((value = modelObject.skinLossRadiativeFraction())) {
      idfObject.setDouble(Pump_ConstantSpeedFields::SkinLossRadiativeFraction, value.get());
    }

    {
      s = modelObject.designPowerSizingMethod();
      idfObject.setString(Pump_ConstantSpeedFields::DesignPowerSizingMethod, s.get());
    }

    {
      value = modelObject.designElectricPowerPerUnitFlowRate();
      idfObject.setDouble(Pump_ConstantSpeedFields::DesignElectricPowerperUnitFlowRate, value.get());
    }

    {
      value = modelObject.designShaftPowerPerUnitFlowRatePerUnitHead();
      idfObject.setDouble(Pump_ConstantSpeedFields::DesignShaftPowerperUnitFlowRateperUnitHead, value.get());
    }

    // End Use Subcategory
    idfObject.setString(Pump_ConstantSpeedFields::EndUseSubcategory, modelObject.endUseSubcategory());

    return idfObject;
  }

}  // namespace energyplus
}  // namespace openstudio
