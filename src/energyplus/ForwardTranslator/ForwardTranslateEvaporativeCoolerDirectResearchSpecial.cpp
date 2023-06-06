/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ForwardTranslator.hpp"
#include "../../model/Model.hpp"
#include "../../model/EvaporativeCoolerDirectResearchSpecial.hpp"
#include "../../model/EvaporativeCoolerDirectResearchSpecial_Impl.hpp"
#include "../../model/Node.hpp"
#include "../../model/Node_Impl.hpp"
#include "../../model/Curve.hpp"
#include "../../model/Curve_Impl.hpp"
#include "../../model/Schedule.hpp"
#include "../../utilities/idf/Workspace.hpp"
#include "../../utilities/core/Logger.hpp"
#include <utilities/idd/EvaporativeCooler_Direct_ResearchSpecial_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateEvaporativeCoolerDirectResearchSpecial(EvaporativeCoolerDirectResearchSpecial& modelObject) {
    OptionalString s;
    OptionalDouble optD;
    OptionalModelObject temp;
    double value;

    IdfObject idfObject(IddObjectType::EvaporativeCooler_Direct_ResearchSpecial);

    m_idfObjects.push_back(idfObject);

    // Name
    s = modelObject.name();
    if (s) {
      idfObject.setName(*s);
    }

    // AvailabilityScheduleName
    Schedule sched = modelObject.availableSchedule();
    translateAndMapModelObject(sched);
    idfObject.setString(EvaporativeCooler_Direct_ResearchSpecialFields::AvailabilityScheduleName, sched.name().get());

    // CoolerEffectiveness
    value = modelObject.coolerDesignEffectiveness();
    idfObject.setDouble(EvaporativeCooler_Direct_ResearchSpecialFields::CoolerDesignEffectiveness, value);

    // RecirculatingWaterPumpPowerConsumption
    if (modelObject.isRecirculatingWaterPumpPowerConsumptionAutosized()) {
      idfObject.setString(EvaporativeCooler_Direct_ResearchSpecialFields::RecirculatingWaterPumpDesignPower, "autosize");
    } else if ((optD = modelObject.recirculatingWaterPumpPowerConsumption())) {
      idfObject.setDouble(EvaporativeCooler_Direct_ResearchSpecialFields::RecirculatingWaterPumpDesignPower, optD.get());
    }

    // Primary Air Design Flow Rate
    if (modelObject.isPrimaryAirDesignFlowRateAutosized()) {
      idfObject.setString(EvaporativeCooler_Direct_ResearchSpecialFields::PrimaryAirDesignFlowRate, "Autosize");
    } else if ((optD = modelObject.primaryAirDesignFlowRate())) {
      idfObject.setDouble(EvaporativeCooler_Direct_ResearchSpecialFields::PrimaryAirDesignFlowRate, optD.get());
    }

    // AirInletNodeName
    temp = modelObject.inletModelObject();
    if (temp) {
      s = temp->name();
      if (s) {
        idfObject.setString(EvaporativeCooler_Direct_ResearchSpecialFields::AirInletNodeName, *s);
      }
    }

    // AirOutletNodeName
    temp = modelObject.outletModelObject();
    if (temp) {
      s = temp->name();
      if (s) {
        idfObject.setString(EvaporativeCooler_Direct_ResearchSpecialFields::AirOutletNodeName, *s);
      }
    }

    // SensorNodeName
    temp = modelObject.sensorNode();
    if (temp) {
      s = temp->name();
      if (s) {
        idfObject.setString(EvaporativeCooler_Direct_ResearchSpecialFields::SensorNodeName, *s);
      }
    }

    // DriftLossFraction
    value = modelObject.driftLossFraction();
    idfObject.setDouble(EvaporativeCooler_Direct_ResearchSpecialFields::DriftLossFraction, value);

    // BlowdownConcentrationRatio
    value = modelObject.blowdownConcentrationRatio();
    if (value < 2) {
      idfObject.setDouble(EvaporativeCooler_Direct_ResearchSpecialFields::BlowdownConcentrationRatio, 2.0);
    } else {
      idfObject.setDouble(EvaporativeCooler_Direct_ResearchSpecialFields::BlowdownConcentrationRatio, value);
    }

    // EffectivenessFlowRatioModifierCurveName
    if (auto curve = modelObject.effectivenessFlowRatioModifierCurve()) {
      auto _curve = translateAndMapModelObject(curve.get());
      OS_ASSERT(_curve);
      idfObject.setString(EvaporativeCooler_Direct_ResearchSpecialFields::EffectivenessFlowRatioModifierCurveName, _curve->name().get());
    }

    // WaterPumpPowerSizingFactor
    value = modelObject.waterPumpPowerSizingFactor();
    idfObject.setDouble(EvaporativeCooler_Direct_ResearchSpecialFields::WaterPumpPowerSizingFactor, value);

    // WaterPumpPowerModifierCurveName
    if (auto curve = modelObject.waterPumpPowerModifierCurve()) {
      auto _curve = translateAndMapModelObject(curve.get());
      OS_ASSERT(_curve);
      idfObject.setString(EvaporativeCooler_Direct_ResearchSpecialFields::WaterPumpPowerModifierCurveName, _curve->name().get());
    }

    // Evaporative Operation Minimum Drybulb Temperature (Double)
    idfObject.setDouble(EvaporativeCooler_Direct_ResearchSpecialFields::EvaporativeOperationMinimumDrybulbTemperature,
                        modelObject.evaporativeOperationMinimumDrybulbTemperature());

    // Evaporative Operation Maximum Limit Wetbulb Temperature (Double)
    idfObject.setDouble(EvaporativeCooler_Direct_ResearchSpecialFields::EvaporativeOperationMaximumLimitWetbulbTemperature,
                        modelObject.evaporativeOperationMaximumLimitWetbulbTemperature());

    // Evaporative Operation Maximum Limit Drybulb Temperature (Double)
    idfObject.setDouble(EvaporativeCooler_Direct_ResearchSpecialFields::EvaporativeOperationMaximumLimitDrybulbTemperature,
                        modelObject.evaporativeOperationMaximumLimitDrybulbTemperature());

    return boost::optional<IdfObject>(idfObject);
  }

}  // namespace energyplus

}  // namespace openstudio
