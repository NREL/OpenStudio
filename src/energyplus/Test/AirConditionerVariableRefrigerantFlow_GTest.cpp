/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>
#include "EnergyPlusFixture.hpp"

#include "../ForwardTranslator.hpp"
#include "../ReverseTranslator.hpp"

#include "../../model/Model.hpp"
#include "../../model/AirConditionerVariableRefrigerantFlow.hpp"
#include "../../model/ZoneHVACTerminalUnitVariableRefrigerantFlow.hpp"
#include "../../model/PlantLoop.hpp"
#include "../../model/Node.hpp"
#include "../../model/ScheduleConstant.hpp"
#include "../../model/ScheduleConstant_Impl.hpp"
#include "../../model/CurveBiquadratic.hpp"
#include "../../model/CurveBiquadratic_Impl.hpp"
#include "../../model/CurveCubic.hpp"
#include "../../model/CurveCubic_Impl.hpp"
#include "../../model/Space.hpp"
#include "../../model/Space_Impl.hpp"
#include "../../model/ThermalZone.hpp"
#include "../../model/ThermalZone_Impl.hpp"

#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/AirConditioner_VariableRefrigerantFlow_FieldEnums.hxx>

#include <resources.hxx>

#include <sstream>

using namespace openstudio::energyplus;
using namespace openstudio::model;
using namespace openstudio;

TEST_F(EnergyPlusFixture, ForwardTranslator_AirConditionerVariableRefrigerantFlow) {
  Model model;
  AirConditionerVariableRefrigerantFlow vrf(model);
  ThermalZone zone(model);
  Space space(model);
  space.setThermalZone(zone);

  ScheduleConstant scheduleConstant(model);
  scheduleConstant.setValue(0.5);
  EXPECT_TRUE(vrf.setAvailabilitySchedule(scheduleConstant));
  EXPECT_TRUE(vrf.setGrossRatedTotalCoolingCapacity(1));
  EXPECT_TRUE(vrf.setGrossRatedCoolingCOP(2));
  EXPECT_TRUE(vrf.setMinimumOutdoorTemperatureinCoolingMode(3));
  EXPECT_TRUE(vrf.setMaximumOutdoorTemperatureinCoolingMode(4));
  CurveBiquadratic curve1(model);
  EXPECT_TRUE(vrf.setCoolingCapacityRatioModifierFunctionofLowTemperatureCurve(curve1));  // bivariate
  CurveCubic curve2(model);
  EXPECT_TRUE(vrf.setCoolingCapacityRatioBoundaryCurve(curve2));  // univariate
  CurveBiquadratic curve3(model);
  EXPECT_TRUE(vrf.setCoolingCapacityRatioModifierFunctionofHighTemperatureCurve(curve3));  // bivariate
  CurveBiquadratic curve4(model);
  EXPECT_TRUE(vrf.setCoolingEnergyInputRatioModifierFunctionofLowTemperatureCurve(curve4));  // bivariate
  CurveCubic curve5(model);
  EXPECT_TRUE(vrf.setCoolingEnergyInputRatioBoundaryCurve(curve5));  // univariate
  CurveBiquadratic curve6(model);
  EXPECT_TRUE(vrf.setCoolingEnergyInputRatioModifierFunctionofHighTemperatureCurve(curve6));  // bivariate
  CurveCubic curve7(model);
  EXPECT_TRUE(vrf.setCoolingEnergyInputRatioModifierFunctionofLowPartLoadRatioCurve(curve7));  // univariate
  CurveCubic curve8(model);
  EXPECT_TRUE(vrf.setCoolingEnergyInputRatioModifierFunctionofHighPartLoadRatioCurve(curve8));  // univariate
  CurveCubic curve9(model);
  EXPECT_TRUE(vrf.setCoolingCombinationRatioCorrectionFactorCurve(curve9));  // univariate
  CurveCubic curve10(model);
  EXPECT_TRUE(vrf.setCoolingPartLoadFractionCorrelationCurve(curve10));  // univariate
  EXPECT_TRUE(vrf.setGrossRatedHeatingCapacity(5));
  EXPECT_TRUE(vrf.setRatedHeatingCapacitySizingRatio(6));
  EXPECT_TRUE(vrf.setRatedHeatingCOP(7));
  EXPECT_TRUE(vrf.setMinimumOutdoorTemperatureinHeatingMode(8));
  EXPECT_TRUE(vrf.setMaximumOutdoorTemperatureinHeatingMode(9));
  CurveBiquadratic curve11(model);
  EXPECT_TRUE(vrf.setHeatingCapacityRatioModifierFunctionofLowTemperatureCurve(curve11));  // bivariate
  CurveCubic curve12(model);
  EXPECT_TRUE(vrf.setHeatingCapacityRatioBoundaryCurve(curve12));  // univariate
  CurveBiquadratic curve13(model);
  EXPECT_TRUE(vrf.setHeatingCapacityRatioModifierFunctionofHighTemperatureCurve(curve13));  // bivariate
  CurveBiquadratic curve14(model);
  EXPECT_TRUE(vrf.setHeatingEnergyInputRatioModifierFunctionofLowTemperatureCurve(curve14));  // bivariate
  CurveCubic curve15(model);
  EXPECT_TRUE(vrf.setHeatingEnergyInputRatioBoundaryCurve(curve15));  // univariate
  CurveBiquadratic curve16(model);
  EXPECT_TRUE(vrf.setHeatingEnergyInputRatioModifierFunctionofHighTemperatureCurve(curve16));  // bivariate
  EXPECT_TRUE(vrf.setHeatingPerformanceCurveOutdoorTemperatureType("WetBulbTemperature"));
  CurveCubic curve17(model);
  EXPECT_TRUE(vrf.setHeatingEnergyInputRatioModifierFunctionofLowPartLoadRatioCurve(curve17));  // univariate
  CurveCubic curve18(model);
  EXPECT_TRUE(vrf.setHeatingEnergyInputRatioModifierFunctionofHighPartLoadRatioCurve(curve18));  // univariate
  CurveCubic curve19(model);
  EXPECT_TRUE(vrf.setHeatingCombinationRatioCorrectionFactorCurve(curve19));  // univariate
  CurveCubic curve20(model);
  EXPECT_TRUE(vrf.setHeatingPartLoadFractionCorrelationCurve(curve20));  // univariate
  EXPECT_TRUE(vrf.setMinimumHeatPumpPartLoadRatio(10));
  EXPECT_TRUE(vrf.setZoneforMasterThermostatLocation(zone));
  EXPECT_TRUE(vrf.setMasterThermostatPriorityControlType("Scheduled"));
  ScheduleConstant scheduleConstant2(model);
  scheduleConstant2.setValue(0.7);
  EXPECT_TRUE(vrf.setThermostatPrioritySchedule(scheduleConstant2));
  EXPECT_TRUE(vrf.setHeatPumpWasteHeatRecovery(false));
  EXPECT_TRUE(vrf.setEquivalentPipingLengthusedforPipingCorrectionFactorinCoolingMode(11));
  EXPECT_TRUE(vrf.setVerticalHeightusedforPipingCorrectionFactor(12));
  CurveBiquadratic curve21(model);
  EXPECT_TRUE(vrf.setPipingCorrectionFactorforLengthinCoolingModeCurve(curve21));  // univariate, bivariate
  EXPECT_TRUE(vrf.setPipingCorrectionFactorforHeightinCoolingModeCoefficient(13));
  EXPECT_TRUE(vrf.setEquivalentPipingLengthusedforPipingCorrectionFactorinHeatingMode(14));
  CurveBiquadratic curve22(model);
  EXPECT_TRUE(vrf.setPipingCorrectionFactorforLengthinHeatingModeCurve(curve22));  // univariate, bivariate
  EXPECT_TRUE(vrf.setPipingCorrectionFactorforHeightinHeatingModeCoefficient(15));
  EXPECT_TRUE(vrf.setCrankcaseHeaterPowerperCompressor(16));
  EXPECT_TRUE(vrf.setNumberofCompressors(17));
  EXPECT_TRUE(vrf.setRatioofCompressorSizetoTotalCompressorCapacity(18));
  EXPECT_TRUE(vrf.setMaximumOutdoorDrybulbTemperatureforCrankcaseHeater(19));
  EXPECT_TRUE(vrf.setDefrostStrategy("Resistive"));
  EXPECT_TRUE(vrf.setDefrostControl("OnDemand"));
  CurveBiquadratic curve23(model);
  EXPECT_TRUE(vrf.setDefrostEnergyInputRatioModifierFunctionofTemperatureCurve(curve23));  // bivariate
  EXPECT_TRUE(vrf.setDefrostTimePeriodFraction(20));
  EXPECT_TRUE(vrf.setResistiveDefrostHeaterCapacity(21));
  EXPECT_TRUE(vrf.setMaximumOutdoorDrybulbTemperatureforDefrostOperation(22));
  EXPECT_TRUE(vrf.setCondenserType("EvaporativelyCooled"));
  EXPECT_TRUE(vrf.setWaterCondenserVolumeFlowRate(23));
  EXPECT_TRUE(vrf.setEvaporativeCondenserEffectiveness(0.2));
  EXPECT_TRUE(vrf.setEvaporativeCondenserAirFlowRate(24));
  EXPECT_TRUE(vrf.setEvaporativeCondenserPumpRatedPowerConsumption(25));
  EXPECT_TRUE(vrf.setBasinHeaterCapacity(26));
  EXPECT_TRUE(vrf.setBasinHeaterSetpointTemperature(27));
  ScheduleConstant scheduleConstant3(model);
  scheduleConstant3.setValue(0.9);
  EXPECT_TRUE(vrf.setBasinHeaterOperatingSchedule(scheduleConstant3));
  EXPECT_TRUE(vrf.setFuelType("NaturalGas"));
  EXPECT_TRUE(vrf.setMinimumOutdoorTemperatureinHeatRecoveryMode(28));
  EXPECT_TRUE(vrf.setMaximumOutdoorTemperatureinHeatRecoveryMode(29));
  CurveBiquadratic curve24(model);
  EXPECT_TRUE(vrf.setHeatRecoveryCoolingCapacityModifierCurve(curve24));  // bivariate
  EXPECT_TRUE(vrf.setInitialHeatRecoveryCoolingCapacityFraction(30));
  EXPECT_TRUE(vrf.setHeatRecoveryCoolingCapacityTimeConstant(31));
  CurveBiquadratic curve25(model);
  EXPECT_TRUE(vrf.setHeatRecoveryCoolingEnergyModifierCurve(curve25));  // bivariate
  EXPECT_TRUE(vrf.setInitialHeatRecoveryCoolingEnergyFraction(32));
  EXPECT_TRUE(vrf.setHeatRecoveryCoolingEnergyTimeConstant(33));
  CurveBiquadratic curve26(model);
  EXPECT_TRUE(vrf.setHeatRecoveryHeatingCapacityModifierCurve(curve26));  // bivariate
  EXPECT_TRUE(vrf.setInitialHeatRecoveryHeatingCapacityFraction(34));
  EXPECT_TRUE(vrf.setHeatRecoveryHeatingCapacityTimeConstant(35));
  CurveBiquadratic curve27(model);
  EXPECT_TRUE(vrf.setHeatRecoveryHeatingEnergyModifierCurve(curve27));  // bivariate
  EXPECT_TRUE(vrf.setInitialHeatRecoveryHeatingEnergyFraction(36));
  EXPECT_TRUE(vrf.setHeatRecoveryHeatingEnergyTimeConstant(37));

  ForwardTranslator ft;
  Workspace workspace = ft.translateModel(model);

  ZoneHVACTerminalUnitVariableRefrigerantFlow term(model);
  EXPECT_TRUE(vrf.addTerminal(term));
  workspace = ft.translateModel(model);

  EXPECT_EQ(1u, workspace.getObjectsByType(IddObjectType::AirConditioner_VariableRefrigerantFlow).size());
  EXPECT_EQ(1u, workspace.getObjectsByType(IddObjectType::ZoneHVAC_TerminalUnit_VariableRefrigerantFlow).size());
  EXPECT_EQ(1u, workspace.getObjectsByType(IddObjectType::Zone).size());
  EXPECT_EQ(26u, workspace.getObjectsByType(IddObjectType::Curve_Biquadratic).size());
  EXPECT_EQ(24u, workspace.getObjectsByType(IddObjectType::Curve_Cubic).size());

  IdfObject idf_vrf = workspace.getObjectsByType(IddObjectType::AirConditioner_VariableRefrigerantFlow)[0];

  EXPECT_EQ(vrf.nameString(), idf_vrf.getString(AirConditioner_VariableRefrigerantFlowFields::Name, false).get());
  EXPECT_EQ(scheduleConstant.nameString(), idf_vrf.getString(AirConditioner_VariableRefrigerantFlowFields::AvailabilityScheduleName, false).get());
  EXPECT_EQ(1, idf_vrf.getDouble(AirConditioner_VariableRefrigerantFlowFields::GrossRatedTotalCoolingCapacity, false).get());
  EXPECT_EQ(2, idf_vrf.getDouble(AirConditioner_VariableRefrigerantFlowFields::GrossRatedCoolingCOP, false).get());
  EXPECT_EQ(3, idf_vrf.getDouble(AirConditioner_VariableRefrigerantFlowFields::MinimumCondenserInletNodeTemperatureinCoolingMode, false).get());
  EXPECT_EQ(4, idf_vrf.getDouble(AirConditioner_VariableRefrigerantFlowFields::MaximumCondenserInletNodeTemperatureinCoolingMode, false).get());
  EXPECT_EQ(
    curve1.nameString(),
    idf_vrf.getString(AirConditioner_VariableRefrigerantFlowFields::CoolingCapacityRatioModifierFunctionofLowTemperatureCurveName, false).get());
  EXPECT_EQ(curve2.nameString(), idf_vrf.getString(AirConditioner_VariableRefrigerantFlowFields::CoolingCapacityRatioBoundaryCurveName, false).get());
  EXPECT_EQ(
    curve3.nameString(),
    idf_vrf.getString(AirConditioner_VariableRefrigerantFlowFields::CoolingCapacityRatioModifierFunctionofHighTemperatureCurveName, false).get());
  EXPECT_EQ(
    curve4.nameString(),
    idf_vrf.getString(AirConditioner_VariableRefrigerantFlowFields::CoolingEnergyInputRatioModifierFunctionofLowTemperatureCurveName, false).get());
  EXPECT_EQ(curve5.nameString(),
            idf_vrf.getString(AirConditioner_VariableRefrigerantFlowFields::CoolingEnergyInputRatioBoundaryCurveName, false).get());
  EXPECT_EQ(
    curve6.nameString(),
    idf_vrf.getString(AirConditioner_VariableRefrigerantFlowFields::CoolingEnergyInputRatioModifierFunctionofHighTemperatureCurveName, false).get());
  EXPECT_EQ(
    curve7.nameString(),
    idf_vrf.getString(AirConditioner_VariableRefrigerantFlowFields::CoolingEnergyInputRatioModifierFunctionofLowPartLoadRatioCurveName, false).get());
  EXPECT_EQ(
    curve8.nameString(),
    idf_vrf.getString(AirConditioner_VariableRefrigerantFlowFields::CoolingEnergyInputRatioModifierFunctionofHighPartLoadRatioCurveName, false)
      .get());
  EXPECT_EQ(curve9.nameString(),
            idf_vrf.getString(AirConditioner_VariableRefrigerantFlowFields::CoolingCombinationRatioCorrectionFactorCurveName, false).get());
  EXPECT_EQ(curve10.nameString(),
            idf_vrf.getString(AirConditioner_VariableRefrigerantFlowFields::CoolingPartLoadFractionCorrelationCurveName, false).get());
  EXPECT_EQ(5, idf_vrf.getDouble(AirConditioner_VariableRefrigerantFlowFields::GrossRatedHeatingCapacity, false).get());
  EXPECT_EQ(6, idf_vrf.getDouble(AirConditioner_VariableRefrigerantFlowFields::RatedHeatingCapacitySizingRatio, false).get());
  EXPECT_EQ(7, idf_vrf.getDouble(AirConditioner_VariableRefrigerantFlowFields::GrossRatedHeatingCOP, false).get());
  EXPECT_EQ(8, idf_vrf.getDouble(AirConditioner_VariableRefrigerantFlowFields::MinimumCondenserInletNodeTemperatureinHeatingMode, false).get());
  EXPECT_EQ(9, idf_vrf.getDouble(AirConditioner_VariableRefrigerantFlowFields::MaximumCondenserInletNodeTemperatureinHeatingMode, false).get());
  EXPECT_EQ(
    curve11.nameString(),
    idf_vrf.getString(AirConditioner_VariableRefrigerantFlowFields::HeatingCapacityRatioModifierFunctionofLowTemperatureCurveName, false).get());
  EXPECT_EQ(curve12.nameString(),
            idf_vrf.getString(AirConditioner_VariableRefrigerantFlowFields::HeatingCapacityRatioBoundaryCurveName, false).get());
  EXPECT_EQ(
    curve13.nameString(),
    idf_vrf.getString(AirConditioner_VariableRefrigerantFlowFields::HeatingCapacityRatioModifierFunctionofHighTemperatureCurveName, false).get());
  EXPECT_EQ(
    curve14.nameString(),
    idf_vrf.getString(AirConditioner_VariableRefrigerantFlowFields::HeatingEnergyInputRatioModifierFunctionofLowTemperatureCurveName, false).get());
  EXPECT_EQ(curve15.nameString(),
            idf_vrf.getString(AirConditioner_VariableRefrigerantFlowFields::HeatingEnergyInputRatioBoundaryCurveName, false).get());
  EXPECT_EQ(
    curve16.nameString(),
    idf_vrf.getString(AirConditioner_VariableRefrigerantFlowFields::HeatingEnergyInputRatioModifierFunctionofHighTemperatureCurveName, false).get());
  EXPECT_EQ("WetBulbTemperature",
            idf_vrf.getString(AirConditioner_VariableRefrigerantFlowFields::HeatingPerformanceCurveOutdoorTemperatureType, false).get());
  EXPECT_EQ(
    curve17.nameString(),
    idf_vrf.getString(AirConditioner_VariableRefrigerantFlowFields::HeatingEnergyInputRatioModifierFunctionofLowPartLoadRatioCurveName, false).get());
  EXPECT_EQ(
    curve18.nameString(),
    idf_vrf.getString(AirConditioner_VariableRefrigerantFlowFields::HeatingEnergyInputRatioModifierFunctionofHighPartLoadRatioCurveName, false)
      .get());
  EXPECT_EQ(curve19.nameString(),
            idf_vrf.getString(AirConditioner_VariableRefrigerantFlowFields::HeatingCombinationRatioCorrectionFactorCurveName, false).get());
  EXPECT_EQ(curve20.nameString(),
            idf_vrf.getString(AirConditioner_VariableRefrigerantFlowFields::HeatingPartLoadFractionCorrelationCurveName, false).get());
  EXPECT_EQ(10, idf_vrf.getDouble(AirConditioner_VariableRefrigerantFlowFields::MinimumHeatPumpPartLoadRatio, false).get());
  EXPECT_EQ("Thermal Zone 1", idf_vrf.getString(AirConditioner_VariableRefrigerantFlowFields::ZoneNameforMasterThermostatLocation, false).get());
  EXPECT_EQ("Scheduled", idf_vrf.getString(AirConditioner_VariableRefrigerantFlowFields::MasterThermostatPriorityControlType, false).get());
  EXPECT_EQ(scheduleConstant2.nameString(),
            idf_vrf.getString(AirConditioner_VariableRefrigerantFlowFields::ThermostatPriorityScheduleName, false).get());
  EXPECT_EQ(vrf.nameString() + " Terminal List",
            idf_vrf.getString(AirConditioner_VariableRefrigerantFlowFields::ZoneTerminalUnitListName, false).get());
  EXPECT_EQ("No", idf_vrf.getString(AirConditioner_VariableRefrigerantFlowFields::HeatPumpWasteHeatRecovery, false).get());
  EXPECT_EQ(
    11,
    idf_vrf.getDouble(AirConditioner_VariableRefrigerantFlowFields::EquivalentPipingLengthusedforPipingCorrectionFactorinCoolingMode, false).get());
  EXPECT_EQ(12, idf_vrf.getDouble(AirConditioner_VariableRefrigerantFlowFields::VerticalHeightusedforPipingCorrectionFactor, false).get());
  EXPECT_EQ(curve21.nameString(),
            idf_vrf.getString(AirConditioner_VariableRefrigerantFlowFields::PipingCorrectionFactorforLengthinCoolingModeCurveName, false).get());
  EXPECT_EQ(13,
            idf_vrf.getDouble(AirConditioner_VariableRefrigerantFlowFields::PipingCorrectionFactorforHeightinCoolingModeCoefficient, false).get());
  EXPECT_EQ(
    14,
    idf_vrf.getDouble(AirConditioner_VariableRefrigerantFlowFields::EquivalentPipingLengthusedforPipingCorrectionFactorinHeatingMode, false).get());
  EXPECT_EQ(curve22.nameString(),
            idf_vrf.getString(AirConditioner_VariableRefrigerantFlowFields::PipingCorrectionFactorforLengthinHeatingModeCurveName, false).get());
  EXPECT_EQ(15,
            idf_vrf.getDouble(AirConditioner_VariableRefrigerantFlowFields::PipingCorrectionFactorforHeightinHeatingModeCoefficient, false).get());
  EXPECT_EQ(16, idf_vrf.getDouble(AirConditioner_VariableRefrigerantFlowFields::CrankcaseHeaterPowerperCompressor, false).get());
  EXPECT_EQ(17, idf_vrf.getDouble(AirConditioner_VariableRefrigerantFlowFields::NumberofCompressors, false).get());
  EXPECT_EQ(18, idf_vrf.getDouble(AirConditioner_VariableRefrigerantFlowFields::RatioofCompressorSizetoTotalCompressorCapacity, false).get());
  EXPECT_EQ(19, idf_vrf.getDouble(AirConditioner_VariableRefrigerantFlowFields::MaximumOutdoorDryBulbTemperatureforCrankcaseHeater, false).get());
  EXPECT_EQ("Resistive", idf_vrf.getString(AirConditioner_VariableRefrigerantFlowFields::DefrostStrategy, false).get());
  EXPECT_EQ("OnDemand", idf_vrf.getString(AirConditioner_VariableRefrigerantFlowFields::DefrostControl, false).get());
  EXPECT_EQ(
    curve23.nameString(),
    idf_vrf.getString(AirConditioner_VariableRefrigerantFlowFields::DefrostEnergyInputRatioModifierFunctionofTemperatureCurveName, false).get());
  EXPECT_EQ(20, idf_vrf.getDouble(AirConditioner_VariableRefrigerantFlowFields::DefrostTimePeriodFraction, false).get());
  EXPECT_EQ(21, idf_vrf.getDouble(AirConditioner_VariableRefrigerantFlowFields::ResistiveDefrostHeaterCapacity, false).get());
  EXPECT_EQ(22, idf_vrf.getDouble(AirConditioner_VariableRefrigerantFlowFields::MaximumOutdoorDrybulbTemperatureforDefrostOperation, false).get());
  EXPECT_EQ("EvaporativelyCooled", idf_vrf.getString(AirConditioner_VariableRefrigerantFlowFields::CondenserType, false).get());
  EXPECT_EQ("", idf_vrf.getString(AirConditioner_VariableRefrigerantFlowFields::CondenserInletNodeName, false).get());
  EXPECT_EQ("", idf_vrf.getString(AirConditioner_VariableRefrigerantFlowFields::CondenserOutletNodeName, false).get());
  EXPECT_EQ(23, idf_vrf.getDouble(AirConditioner_VariableRefrigerantFlowFields::WaterCondenserVolumeFlowRate, false).get());
  EXPECT_EQ(0.2, idf_vrf.getDouble(AirConditioner_VariableRefrigerantFlowFields::EvaporativeCondenserEffectiveness, false).get());
  EXPECT_EQ(24, idf_vrf.getDouble(AirConditioner_VariableRefrigerantFlowFields::EvaporativeCondenserAirFlowRate, false).get());
  EXPECT_EQ(25, idf_vrf.getDouble(AirConditioner_VariableRefrigerantFlowFields::EvaporativeCondenserPumpRatedPowerConsumption, false).get());
  EXPECT_EQ(26, idf_vrf.getDouble(AirConditioner_VariableRefrigerantFlowFields::BasinHeaterCapacity, false).get());
  EXPECT_EQ(27, idf_vrf.getDouble(AirConditioner_VariableRefrigerantFlowFields::BasinHeaterSetpointTemperature, false).get());
  EXPECT_EQ(scheduleConstant3.nameString(),
            idf_vrf.getString(AirConditioner_VariableRefrigerantFlowFields::BasinHeaterOperatingScheduleName, false).get());
  EXPECT_EQ("NaturalGas", idf_vrf.getString(AirConditioner_VariableRefrigerantFlowFields::FuelType, false).get());
  EXPECT_EQ(28, idf_vrf.getDouble(AirConditioner_VariableRefrigerantFlowFields::MinimumCondenserInletNodeTemperatureinHeatRecoveryMode, false).get());
  EXPECT_EQ(29, idf_vrf.getDouble(AirConditioner_VariableRefrigerantFlowFields::MaximumCondenserInletNodeTemperatureinHeatRecoveryMode, false).get());
  EXPECT_EQ(curve24.nameString(),
            idf_vrf.getString(AirConditioner_VariableRefrigerantFlowFields::HeatRecoveryCoolingCapacityModifierCurveName, false).get());
  EXPECT_EQ(30, idf_vrf.getDouble(AirConditioner_VariableRefrigerantFlowFields::InitialHeatRecoveryCoolingCapacityFraction, false).get());
  EXPECT_EQ(31, idf_vrf.getDouble(AirConditioner_VariableRefrigerantFlowFields::HeatRecoveryCoolingCapacityTimeConstant, false).get());
  EXPECT_EQ(curve25.nameString(),
            idf_vrf.getString(AirConditioner_VariableRefrigerantFlowFields::HeatRecoveryCoolingEnergyModifierCurveName, false).get());
  EXPECT_EQ(32, idf_vrf.getDouble(AirConditioner_VariableRefrigerantFlowFields::InitialHeatRecoveryCoolingEnergyFraction, false).get());
  EXPECT_EQ(33, idf_vrf.getDouble(AirConditioner_VariableRefrigerantFlowFields::HeatRecoveryCoolingEnergyTimeConstant, false).get());
  EXPECT_EQ(curve26.nameString(),
            idf_vrf.getString(AirConditioner_VariableRefrigerantFlowFields::HeatRecoveryHeatingCapacityModifierCurveName, false).get());
  EXPECT_EQ(34, idf_vrf.getDouble(AirConditioner_VariableRefrigerantFlowFields::InitialHeatRecoveryHeatingCapacityFraction, false).get());
  EXPECT_EQ(35, idf_vrf.getDouble(AirConditioner_VariableRefrigerantFlowFields::HeatRecoveryHeatingCapacityTimeConstant, false).get());
  EXPECT_EQ(curve27.nameString(),
            idf_vrf.getString(AirConditioner_VariableRefrigerantFlowFields::HeatRecoveryHeatingEnergyModifierCurveName, false).get());
  EXPECT_EQ(36, idf_vrf.getDouble(AirConditioner_VariableRefrigerantFlowFields::InitialHeatRecoveryHeatingEnergyFraction, false).get());
  EXPECT_EQ(37, idf_vrf.getDouble(AirConditioner_VariableRefrigerantFlowFields::HeatRecoveryHeatingEnergyTimeConstant, false).get());
}

TEST_F(EnergyPlusFixture, ForwardTranslator_AirConditionerVariableRefrigerantFlow_harcodedCondenserType) {

  // Lambda to dry up code
  auto translateAndCheckOneVRFAndCondenserType = [](const Model& m, const std::string& expectedCondenserType, unsigned expectedNumberOfErrors) {
    ForwardTranslator ft;

    // Translate
    Workspace w = ft.translateModel(m);

    // No Errors
    EXPECT_EQ(expectedNumberOfErrors, ft.errors().size());

    std::vector<WorkspaceObject> objs = w.getObjectsByType(IddObjectType::AirConditioner_VariableRefrigerantFlow);
    EXPECT_EQ(1u, objs.size());

    WorkspaceObject i_vrf = objs[0];

    EXPECT_EQ(i_vrf.getString(AirConditioner_VariableRefrigerantFlowFields::CondenserType).get(), expectedCondenserType);
  };

  std::vector<std::string> noWaterCondenserTypes({"AirCooled", "EvaporativelyCooled"});

  for (const std::string& condenserType : noWaterCondenserTypes) {
    for (bool hasPlantLoop : {false, true}) {

      Model m;
      AirConditionerVariableRefrigerantFlow vrf(m);
      vrf.setCondenserType(condenserType);

      // need at least one terminal
      auto vrfTerminal = ZoneHVACTerminalUnitVariableRefrigerantFlow(m, false);
      EXPECT_TRUE(vrf.addTerminal(vrfTerminal));

      if (hasPlantLoop) {
        PlantLoop p(m);
        EXPECT_TRUE(p.addDemandBranchForComponent(vrf));

        // One Error, still harcoded
        translateAndCheckOneVRFAndCondenserType(m, condenserType, 1);

      } else {
        // No Error
        translateAndCheckOneVRFAndCondenserType(m, condenserType, 0);
      }
    }
  }

  // WaterCooled
  std::string condenserType("WaterCooled");

  for (bool hasPlantLoop : {false, true}) {

    Model m;
    AirConditionerVariableRefrigerantFlow vrf(m);
    vrf.setCondenserType(condenserType);

    auto vrfTerminal = ZoneHVACTerminalUnitVariableRefrigerantFlow(m, false);
    EXPECT_TRUE(vrf.addTerminal(vrfTerminal));

    if (hasPlantLoop) {
      PlantLoop p(m);
      EXPECT_TRUE(p.addDemandBranchForComponent(vrf));

      // No Error
      translateAndCheckOneVRFAndCondenserType(m, condenserType, 0);

    } else {
      // One Error, still harcoded
      translateAndCheckOneVRFAndCondenserType(m, condenserType, 1);
    }
  }
}

TEST_F(EnergyPlusFixture, ForwardTranslator_AirConditionerVariableRefrigerantFlow_defaultedCondenserType) {

  ForwardTranslator ft;

  // No Plant Loop => AirCooled
  {
    Model m;
    AirConditionerVariableRefrigerantFlow vrf(m);
    vrf.resetCondenserType();

    EXPECT_TRUE(vrf.isCondenserTypeDefaulted());
    EXPECT_EQ("AirCooled", vrf.condenserType());

    auto vrfTerminal = ZoneHVACTerminalUnitVariableRefrigerantFlow(m, false);
    EXPECT_TRUE(vrf.addTerminal(vrfTerminal));

    // Translate
    Workspace w = ft.translateModel(m);

    // No Errors
    EXPECT_EQ(0u, ft.errors().size());

    std::vector<WorkspaceObject> objs = w.getObjectsByType(IddObjectType::AirConditioner_VariableRefrigerantFlow);
    EXPECT_EQ(1u, objs.size());

    WorkspaceObject i_vrf = objs[0];

    EXPECT_EQ(i_vrf.getString(AirConditioner_VariableRefrigerantFlowFields::CondenserType).get(), "AirCooled");
  }

  // Plant Loop => WaterCooled
  {
    Model m;
    AirConditionerVariableRefrigerantFlow vrf(m);
    vrf.resetCondenserType();

    auto vrfTerminal = ZoneHVACTerminalUnitVariableRefrigerantFlow(m, false);
    EXPECT_TRUE(vrf.addTerminal(vrfTerminal));

    PlantLoop p(m);
    EXPECT_TRUE(p.addDemandBranchForComponent(vrf));

    EXPECT_TRUE(vrf.isCondenserTypeDefaulted());
    EXPECT_EQ("WaterCooled", vrf.condenserType());

    // Translate
    Workspace w = ft.translateModel(m);

    // No Errors
    EXPECT_EQ(0u, ft.errors().size());

    std::vector<WorkspaceObject> objs = w.getObjectsByType(IddObjectType::AirConditioner_VariableRefrigerantFlow);
    EXPECT_EQ(1u, objs.size());

    WorkspaceObject i_vrf = objs[0];

    EXPECT_EQ(i_vrf.getString(AirConditioner_VariableRefrigerantFlowFields::CondenserType).get(), "WaterCooled");
  }
}

TEST_F(EnergyPlusFixture, ForwardTranslator_AirConditionerVariableRefrigerantFlow_NoTerminals) {
  Model model;
  AirConditionerVariableRefrigerantFlow vrf(model);

  ForwardTranslator ft;
  Workspace workspace = ft.translateModel(model);

  EXPECT_EQ(0u, workspace.getObjectsByType(IddObjectType::AirConditioner_VariableRefrigerantFlow).size());
  EXPECT_EQ(0u, workspace.getObjectsByType(IddObjectType::ZoneHVAC_TerminalUnit_VariableRefrigerantFlow).size());
  EXPECT_EQ(9u, workspace.getObjectsByType(IddObjectType::Curve_Biquadratic).size());
  EXPECT_EQ(11u, workspace.getObjectsByType(IddObjectType::Curve_Cubic).size());
  EXPECT_EQ(0u, workspace.getObjectsByType(IddObjectType::Coil_Cooling_DX_VariableRefrigerantFlow).size());
  EXPECT_EQ(0u, workspace.getObjectsByType(IddObjectType::Coil_Heating_DX_VariableRefrigerantFlow).size());
  EXPECT_EQ(0u, workspace.getObjectsByType(IddObjectType::Fan_VariableVolume).size());

  ASSERT_EQ(1, ft.warnings().size());
  EXPECT_NE(std::string::npos, ft.warnings().front().logMessage().find("will not be translated as it has no terminals"));
  EXPECT_EQ(0, workspace.getObjectsByType(IddObjectType::AirConditioner_VariableRefrigerantFlow).size());

  ZoneHVACTerminalUnitVariableRefrigerantFlow term(model);
  EXPECT_TRUE(vrf.addTerminal(term));
  workspace = ft.translateModel(model);

  EXPECT_EQ(0, ft.errors().size());
  EXPECT_EQ(1, workspace.getObjectsByType(IddObjectType::AirConditioner_VariableRefrigerantFlow).size());
}
