/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "ModelFixture.hpp"

#include "../CoilCoolingDXSingleSpeedThermalStorage.hpp"
#include "../CoilCoolingDXSingleSpeedThermalStorage_Impl.hpp"

#include "../ScheduleConstant.hpp"
#include "../Curve.hpp"
#include "../Curve_Impl.hpp"
#include "../CurveBiquadratic.hpp"
#include "../CurveBiquadratic_Impl.hpp"
#include "../CurveQuadratic.hpp"
#include "../CurveQuadratic_Impl.hpp"
#include "../CurveTriquadratic.hpp"
#include "../CurveTriquadratic_Impl.hpp"
#include "../AirLoopHVACUnitarySystem.hpp"
#include "../AirLoopHVACUnitarySystem_Impl.hpp"
#include "../AirLoopHVAC.hpp"
#include "../PlantLoop.hpp"
#include "../Node.hpp"
#include "../Node_Impl.hpp"
#include "../CoilSystemCoolingDXHeatExchangerAssisted.hpp"
#include "../ZoneHVACPackagedTerminalAirConditioner.hpp"
#include "../ZoneHVACPackagedTerminalHeatPump.hpp"
#include "../ScheduleCompact.hpp"
#include "../CoilHeatingDXSingleSpeed.hpp"
#include "../FanConstantVolume.hpp"
#include "../CoilHeatingElectric.hpp"
#include "../CoilHeatingWater.hpp"
#include "../AirLoopHVACZoneSplitter.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, CoilCoolingDXSingleSpeedThermalStorage_DefaultConstructors) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  ASSERT_EXIT(
    {
      Model m;
      CoilCoolingDXSingleSpeedThermalStorage testObject = CoilCoolingDXSingleSpeedThermalStorage(m);

      exit(0);
    },
    ::testing::ExitedWithCode(0), "");
}

TEST_F(ModelFixture, CoilCoolingDXSingleSpeedThermalStorage_GettersSetters) {
  Model m;

  CoilCoolingDXSingleSpeedThermalStorage coilCoolingDXSingleSpeedThermalStorage(m);

  coilCoolingDXSingleSpeedThermalStorage.setName("My CoilCoolingDXSingleSpeedThermalStorage");

  // Availability Schedule: Required Object
  ScheduleConstant availabilitySchedule(m);
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setAvailabilitySchedule(availabilitySchedule));
  EXPECT_EQ(availabilitySchedule, coilCoolingDXSingleSpeedThermalStorage.availabilitySchedule());

  // Operating Mode Control Method: Required String
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setOperatingModeControlMethod("ScheduledModes"));
  EXPECT_EQ("ScheduledModes", coilCoolingDXSingleSpeedThermalStorage.operatingModeControlMethod());
  // Bad Value
  EXPECT_FALSE(coilCoolingDXSingleSpeedThermalStorage.setOperatingModeControlMethod("BADENUM"));
  EXPECT_EQ("ScheduledModes", coilCoolingDXSingleSpeedThermalStorage.operatingModeControlMethod());

  // Operation Mode Control Schedule: Optional Object
  ScheduleConstant operationModeControlSchedule(m);
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setOperationModeControlSchedule(operationModeControlSchedule));
  ASSERT_TRUE(coilCoolingDXSingleSpeedThermalStorage.operationModeControlSchedule());
  EXPECT_EQ(operationModeControlSchedule, coilCoolingDXSingleSpeedThermalStorage.operationModeControlSchedule().get());

  // Storage Type: Required String
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setStorageType("Water"));
  EXPECT_EQ("Water", coilCoolingDXSingleSpeedThermalStorage.storageType());
  // Bad Value
  EXPECT_FALSE(coilCoolingDXSingleSpeedThermalStorage.setStorageType("BADENUM"));
  EXPECT_EQ("Water", coilCoolingDXSingleSpeedThermalStorage.storageType());

  // Glycol Concentration: Required Integer
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setGlycolConcentration(50));
  ASSERT_TRUE(coilCoolingDXSingleSpeedThermalStorage.glycolConcentration());
  EXPECT_EQ(50, coilCoolingDXSingleSpeedThermalStorage.glycolConcentration());

  // Fluid Storage Volume: Optional Double
  // Autosize
  coilCoolingDXSingleSpeedThermalStorage.autosizeFluidStorageVolume();
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.isFluidStorageVolumeAutosized());
  // Set
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setFluidStorageVolume(0.8));
  ASSERT_TRUE(coilCoolingDXSingleSpeedThermalStorage.fluidStorageVolume());
  EXPECT_EQ(0.8, coilCoolingDXSingleSpeedThermalStorage.fluidStorageVolume().get());
  // Bad Value
  EXPECT_FALSE(coilCoolingDXSingleSpeedThermalStorage.setFluidStorageVolume(-10.0));
  ASSERT_TRUE(coilCoolingDXSingleSpeedThermalStorage.fluidStorageVolume());
  EXPECT_EQ(0.8, coilCoolingDXSingleSpeedThermalStorage.fluidStorageVolume().get());
  EXPECT_FALSE(coilCoolingDXSingleSpeedThermalStorage.isFluidStorageVolumeAutosized());

  // Ice Storage Capacity: Optional Double
  // Autosize
  coilCoolingDXSingleSpeedThermalStorage.autosizeIceStorageCapacity();
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.isIceStorageCapacityAutosized());
  // Set
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setIceStorageCapacity(0.9));
  ASSERT_TRUE(coilCoolingDXSingleSpeedThermalStorage.iceStorageCapacity());
  EXPECT_EQ(0.9, coilCoolingDXSingleSpeedThermalStorage.iceStorageCapacity().get());
  // Bad Value
  EXPECT_FALSE(coilCoolingDXSingleSpeedThermalStorage.setIceStorageCapacity(-10.0));
  ASSERT_TRUE(coilCoolingDXSingleSpeedThermalStorage.iceStorageCapacity());
  EXPECT_EQ(0.9, coilCoolingDXSingleSpeedThermalStorage.iceStorageCapacity().get());
  EXPECT_FALSE(coilCoolingDXSingleSpeedThermalStorage.isIceStorageCapacityAutosized());

  // Storage Capacity Sizing Factor: Optional Double
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setStorageCapacitySizingFactor(1.0));
  ASSERT_TRUE(coilCoolingDXSingleSpeedThermalStorage.storageCapacitySizingFactor());
  EXPECT_EQ(1.0, coilCoolingDXSingleSpeedThermalStorage.storageCapacitySizingFactor().get());

  // Storage Tank to Ambient U-value Times Area Heat Transfer Coefficient: Required Double
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setStorageTanktoAmbientUvalueTimesAreaHeatTransferCoefficient(1.2));
  EXPECT_EQ(1.2, coilCoolingDXSingleSpeedThermalStorage.storageTanktoAmbientUvalueTimesAreaHeatTransferCoefficient());
  // Bad Value
  EXPECT_FALSE(coilCoolingDXSingleSpeedThermalStorage.setStorageTanktoAmbientUvalueTimesAreaHeatTransferCoefficient(-10.0));
  EXPECT_EQ(1.2, coilCoolingDXSingleSpeedThermalStorage.storageTanktoAmbientUvalueTimesAreaHeatTransferCoefficient());

  // Fluid Storage Tank Rating Temperature: Required Double
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setFluidStorageTankRatingTemperature(1.3));
  EXPECT_EQ(1.3, coilCoolingDXSingleSpeedThermalStorage.fluidStorageTankRatingTemperature());

  // Rated Evaporator Air Flow Rate: Required Double
  // Autosize
  coilCoolingDXSingleSpeedThermalStorage.autosizeRatedEvaporatorAirFlowRate();
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.isRatedEvaporatorAirFlowRateAutosized());
  // Set
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setRatedEvaporatorAirFlowRate(1.4));
  ASSERT_TRUE(coilCoolingDXSingleSpeedThermalStorage.ratedEvaporatorAirFlowRate());
  EXPECT_EQ(1.4, coilCoolingDXSingleSpeedThermalStorage.ratedEvaporatorAirFlowRate().get());
  // Bad Value
  EXPECT_FALSE(coilCoolingDXSingleSpeedThermalStorage.setRatedEvaporatorAirFlowRate(-10.0));
  ASSERT_TRUE(coilCoolingDXSingleSpeedThermalStorage.ratedEvaporatorAirFlowRate());
  EXPECT_EQ(1.4, coilCoolingDXSingleSpeedThermalStorage.ratedEvaporatorAirFlowRate().get());
  EXPECT_FALSE(coilCoolingDXSingleSpeedThermalStorage.isRatedEvaporatorAirFlowRateAutosized());

  // Cooling Only Mode Available: Required Boolean
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setCoolingOnlyModeAvailable(true));
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.coolingOnlyModeAvailable());
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setCoolingOnlyModeAvailable(false));
  EXPECT_FALSE(coilCoolingDXSingleSpeedThermalStorage.coolingOnlyModeAvailable());

  // Cooling Only Mode Rated Total Evaporator Cooling Capacity: Optional Double
  // Autosize
  coilCoolingDXSingleSpeedThermalStorage.autosizeCoolingOnlyModeRatedTotalEvaporatorCoolingCapacity();
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.isCoolingOnlyModeRatedTotalEvaporatorCoolingCapacityAutosized());
  // Set
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setCoolingOnlyModeRatedTotalEvaporatorCoolingCapacity(1.8));
  ASSERT_TRUE(coilCoolingDXSingleSpeedThermalStorage.coolingOnlyModeRatedTotalEvaporatorCoolingCapacity());
  EXPECT_EQ(1.8, coilCoolingDXSingleSpeedThermalStorage.coolingOnlyModeRatedTotalEvaporatorCoolingCapacity().get());
  // Bad Value
  EXPECT_FALSE(coilCoolingDXSingleSpeedThermalStorage.setCoolingOnlyModeRatedTotalEvaporatorCoolingCapacity(-10.0));
  ASSERT_TRUE(coilCoolingDXSingleSpeedThermalStorage.coolingOnlyModeRatedTotalEvaporatorCoolingCapacity());
  EXPECT_EQ(1.8, coilCoolingDXSingleSpeedThermalStorage.coolingOnlyModeRatedTotalEvaporatorCoolingCapacity().get());
  EXPECT_FALSE(coilCoolingDXSingleSpeedThermalStorage.isCoolingOnlyModeRatedTotalEvaporatorCoolingCapacityAutosized());

  // Cooling Only Mode Rated Sensible Heat Ratio: Required Double
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setCoolingOnlyModeRatedSensibleHeatRatio(0.95));
  EXPECT_EQ(0.95, coilCoolingDXSingleSpeedThermalStorage.coolingOnlyModeRatedSensibleHeatRatio());
  // Bad Value
  EXPECT_FALSE(coilCoolingDXSingleSpeedThermalStorage.setCoolingOnlyModeRatedSensibleHeatRatio(-10.0));
  EXPECT_EQ(0.95, coilCoolingDXSingleSpeedThermalStorage.coolingOnlyModeRatedSensibleHeatRatio());

  // Cooling Only Mode Rated COP: Required Double
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setCoolingOnlyModeRatedCOP(2.0));
  EXPECT_EQ(2.0, coilCoolingDXSingleSpeedThermalStorage.coolingOnlyModeRatedCOP());
  // Bad Value
  EXPECT_FALSE(coilCoolingDXSingleSpeedThermalStorage.setCoolingOnlyModeRatedCOP(-10.0));
  EXPECT_EQ(2.0, coilCoolingDXSingleSpeedThermalStorage.coolingOnlyModeRatedCOP());

  // Cooling Only Mode Total Evaporator Cooling Capacity Function of Temperature Curve: Optional Object
  CurveBiquadratic coolingOnlyModeTotalEvaporatorCoolingCapacityFunctionofTemperatureCurve(m);
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setCoolingOnlyModeTotalEvaporatorCoolingCapacityFunctionofTemperatureCurve(
    coolingOnlyModeTotalEvaporatorCoolingCapacityFunctionofTemperatureCurve));
  ASSERT_TRUE(coilCoolingDXSingleSpeedThermalStorage.coolingOnlyModeTotalEvaporatorCoolingCapacityFunctionofTemperatureCurve());
  EXPECT_EQ(coolingOnlyModeTotalEvaporatorCoolingCapacityFunctionofTemperatureCurve,
            coilCoolingDXSingleSpeedThermalStorage.coolingOnlyModeTotalEvaporatorCoolingCapacityFunctionofTemperatureCurve().get());

  // Cooling Only Mode Total Evaporator Cooling Capacity Function of Flow Fraction Curve: Optional Object
  CurveQuadratic coolingOnlyModeTotalEvaporatorCoolingCapacityFunctionofFlowFractionCurve(m);
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setCoolingOnlyModeTotalEvaporatorCoolingCapacityFunctionofFlowFractionCurve(
    coolingOnlyModeTotalEvaporatorCoolingCapacityFunctionofFlowFractionCurve));
  ASSERT_TRUE(coilCoolingDXSingleSpeedThermalStorage.coolingOnlyModeTotalEvaporatorCoolingCapacityFunctionofFlowFractionCurve());
  EXPECT_EQ(coolingOnlyModeTotalEvaporatorCoolingCapacityFunctionofFlowFractionCurve,
            coilCoolingDXSingleSpeedThermalStorage.coolingOnlyModeTotalEvaporatorCoolingCapacityFunctionofFlowFractionCurve().get());

  // Cooling Only Mode Energy Input Ratio Function of Temperature Curve: Optional Object
  CurveBiquadratic coolingOnlyModeEnergyInputRatioFunctionofTemperatureCurve(m);
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setCoolingOnlyModeEnergyInputRatioFunctionofTemperatureCurve(
    coolingOnlyModeEnergyInputRatioFunctionofTemperatureCurve));
  ASSERT_TRUE(coilCoolingDXSingleSpeedThermalStorage.coolingOnlyModeEnergyInputRatioFunctionofTemperatureCurve());
  EXPECT_EQ(coolingOnlyModeEnergyInputRatioFunctionofTemperatureCurve,
            coilCoolingDXSingleSpeedThermalStorage.coolingOnlyModeEnergyInputRatioFunctionofTemperatureCurve().get());

  // Cooling Only Mode Energy Input Ratio Function of Flow Fraction Curve: Optional Object
  CurveQuadratic coolingOnlyModeEnergyInputRatioFunctionofFlowFractionCurve(m);
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setCoolingOnlyModeEnergyInputRatioFunctionofFlowFractionCurve(
    coolingOnlyModeEnergyInputRatioFunctionofFlowFractionCurve));
  ASSERT_TRUE(coilCoolingDXSingleSpeedThermalStorage.coolingOnlyModeEnergyInputRatioFunctionofFlowFractionCurve());
  EXPECT_EQ(coolingOnlyModeEnergyInputRatioFunctionofFlowFractionCurve,
            coilCoolingDXSingleSpeedThermalStorage.coolingOnlyModeEnergyInputRatioFunctionofFlowFractionCurve().get());

  // Cooling Only Mode Part Load Fraction Correlation Curve: Optional Object
  CurveQuadratic coolingOnlyModePartLoadFractionCorrelationCurve(m);
  EXPECT_TRUE(
    coilCoolingDXSingleSpeedThermalStorage.setCoolingOnlyModePartLoadFractionCorrelationCurve(coolingOnlyModePartLoadFractionCorrelationCurve));
  ASSERT_TRUE(coilCoolingDXSingleSpeedThermalStorage.coolingOnlyModePartLoadFractionCorrelationCurve());
  EXPECT_EQ(coolingOnlyModePartLoadFractionCorrelationCurve,
            coilCoolingDXSingleSpeedThermalStorage.coolingOnlyModePartLoadFractionCorrelationCurve().get());

  // Cooling Only Mode Sensible Heat Ratio Function of Temperature Curve: Optional Object
  CurveBiquadratic coolingOnlyModeSensibleHeatRatioFunctionofTemperatureCurve(m);
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setCoolingOnlyModeSensibleHeatRatioFunctionofTemperatureCurve(
    coolingOnlyModeSensibleHeatRatioFunctionofTemperatureCurve));
  ASSERT_TRUE(coilCoolingDXSingleSpeedThermalStorage.coolingOnlyModeSensibleHeatRatioFunctionofTemperatureCurve());
  EXPECT_EQ(coolingOnlyModeSensibleHeatRatioFunctionofTemperatureCurve,
            coilCoolingDXSingleSpeedThermalStorage.coolingOnlyModeSensibleHeatRatioFunctionofTemperatureCurve().get());

  // Cooling Only Mode Sensible Heat Ratio Function of Flow Fraction Curve: Optional Object
  CurveQuadratic coolingOnlyModeSensibleHeatRatioFunctionofFlowFractionCurve(m);
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setCoolingOnlyModeSensibleHeatRatioFunctionofFlowFractionCurve(
    coolingOnlyModeSensibleHeatRatioFunctionofFlowFractionCurve));
  ASSERT_TRUE(coilCoolingDXSingleSpeedThermalStorage.coolingOnlyModeSensibleHeatRatioFunctionofFlowFractionCurve());
  EXPECT_EQ(coolingOnlyModeSensibleHeatRatioFunctionofFlowFractionCurve,
            coilCoolingDXSingleSpeedThermalStorage.coolingOnlyModeSensibleHeatRatioFunctionofFlowFractionCurve().get());

  // Cooling And Charge Mode Available: Required Boolean
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setCoolingAndChargeModeAvailable(true));
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.coolingAndChargeModeAvailable());
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setCoolingAndChargeModeAvailable(false));
  EXPECT_FALSE(coilCoolingDXSingleSpeedThermalStorage.coolingAndChargeModeAvailable());

  // Cooling And Charge Mode Rated Total Evaporator Cooling Capacity: Optional Double
  // Autosize
  coilCoolingDXSingleSpeedThermalStorage.autosizeCoolingAndChargeModeRatedTotalEvaporatorCoolingCapacity();
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.isCoolingAndChargeModeRatedTotalEvaporatorCoolingCapacityAutosized());
  // Set
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setCoolingAndChargeModeRatedTotalEvaporatorCoolingCapacity(2.9));
  ASSERT_TRUE(coilCoolingDXSingleSpeedThermalStorage.coolingAndChargeModeRatedTotalEvaporatorCoolingCapacity());
  EXPECT_EQ(2.9, coilCoolingDXSingleSpeedThermalStorage.coolingAndChargeModeRatedTotalEvaporatorCoolingCapacity().get());
  // Bad Value
  EXPECT_FALSE(coilCoolingDXSingleSpeedThermalStorage.setCoolingAndChargeModeRatedTotalEvaporatorCoolingCapacity(-10.0));
  ASSERT_TRUE(coilCoolingDXSingleSpeedThermalStorage.coolingAndChargeModeRatedTotalEvaporatorCoolingCapacity());
  EXPECT_EQ(2.9, coilCoolingDXSingleSpeedThermalStorage.coolingAndChargeModeRatedTotalEvaporatorCoolingCapacity().get());
  EXPECT_FALSE(coilCoolingDXSingleSpeedThermalStorage.isCoolingAndChargeModeRatedTotalEvaporatorCoolingCapacityAutosized());

  // Cooling And Charge Mode Capacity Sizing Factor: Required Double
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setCoolingAndChargeModeCapacitySizingFactor(3.0));
  EXPECT_EQ(3.0, coilCoolingDXSingleSpeedThermalStorage.coolingAndChargeModeCapacitySizingFactor());

  // Cooling And Charge Mode Rated Storage Charging Capacity: Optional Double
  // Autosize
  coilCoolingDXSingleSpeedThermalStorage.autosizeCoolingAndChargeModeRatedStorageChargingCapacity();
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.isCoolingAndChargeModeRatedStorageChargingCapacityAutosized());
  // Set
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setCoolingAndChargeModeRatedStorageChargingCapacity(3.1));
  ASSERT_TRUE(coilCoolingDXSingleSpeedThermalStorage.coolingAndChargeModeRatedStorageChargingCapacity());
  EXPECT_EQ(3.1, coilCoolingDXSingleSpeedThermalStorage.coolingAndChargeModeRatedStorageChargingCapacity().get());
  // Bad Value
  EXPECT_FALSE(coilCoolingDXSingleSpeedThermalStorage.setCoolingAndChargeModeRatedStorageChargingCapacity(-10.0));
  ASSERT_TRUE(coilCoolingDXSingleSpeedThermalStorage.coolingAndChargeModeRatedStorageChargingCapacity());
  EXPECT_EQ(3.1, coilCoolingDXSingleSpeedThermalStorage.coolingAndChargeModeRatedStorageChargingCapacity().get());
  EXPECT_FALSE(coilCoolingDXSingleSpeedThermalStorage.isCoolingAndChargeModeRatedStorageChargingCapacityAutosized());

  // Cooling And Charge Mode Storage Capacity Sizing Factor: Required Double
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setCoolingAndChargeModeStorageCapacitySizingFactor(3.2));
  EXPECT_EQ(3.2, coilCoolingDXSingleSpeedThermalStorage.coolingAndChargeModeStorageCapacitySizingFactor());

  // Cooling And Charge Mode Rated Sensible Heat Ratio: Required Double
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setCoolingAndChargeModeRatedSensibleHeatRatio(0.971));
  EXPECT_EQ(0.971, coilCoolingDXSingleSpeedThermalStorage.coolingAndChargeModeRatedSensibleHeatRatio());
  // Bad Value
  EXPECT_FALSE(coilCoolingDXSingleSpeedThermalStorage.setCoolingAndChargeModeRatedSensibleHeatRatio(-10.0));
  EXPECT_EQ(0.971, coilCoolingDXSingleSpeedThermalStorage.coolingAndChargeModeRatedSensibleHeatRatio());

  // Cooling And Charge Mode Cooling Rated COP: Required Double
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setCoolingAndChargeModeCoolingRatedCOP(3.4));
  EXPECT_EQ(3.4, coilCoolingDXSingleSpeedThermalStorage.coolingAndChargeModeCoolingRatedCOP());
  // Bad Value
  EXPECT_FALSE(coilCoolingDXSingleSpeedThermalStorage.setCoolingAndChargeModeCoolingRatedCOP(-10.0));
  EXPECT_EQ(3.4, coilCoolingDXSingleSpeedThermalStorage.coolingAndChargeModeCoolingRatedCOP());

  // Cooling And Charge Mode Charging Rated COP: Required Double
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setCoolingAndChargeModeChargingRatedCOP(3.5));
  EXPECT_EQ(3.5, coilCoolingDXSingleSpeedThermalStorage.coolingAndChargeModeChargingRatedCOP());
  // Bad Value
  EXPECT_FALSE(coilCoolingDXSingleSpeedThermalStorage.setCoolingAndChargeModeChargingRatedCOP(-10.0));
  EXPECT_EQ(3.5, coilCoolingDXSingleSpeedThermalStorage.coolingAndChargeModeChargingRatedCOP());

  // Cooling And Charge Mode Total Evaporator Cooling Capacity Function of Temperature Curve: Optional Object
  CurveTriquadratic coolingAndChargeModeTotalEvaporatorCoolingCapacityFunctionofTemperatureCurve(m);
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setCoolingAndChargeModeTotalEvaporatorCoolingCapacityFunctionofTemperatureCurve(
    coolingAndChargeModeTotalEvaporatorCoolingCapacityFunctionofTemperatureCurve));
  ASSERT_TRUE(coilCoolingDXSingleSpeedThermalStorage.coolingAndChargeModeTotalEvaporatorCoolingCapacityFunctionofTemperatureCurve());
  EXPECT_EQ(coolingAndChargeModeTotalEvaporatorCoolingCapacityFunctionofTemperatureCurve,
            coilCoolingDXSingleSpeedThermalStorage.coolingAndChargeModeTotalEvaporatorCoolingCapacityFunctionofTemperatureCurve().get());

  // Cooling And Charge Mode Total Evaporator Cooling Capacity Function of Flow Fraction Curve: Optional Object
  CurveQuadratic coolingAndChargeModeTotalEvaporatorCoolingCapacityFunctionofFlowFractionCurve(m);
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setCoolingAndChargeModeTotalEvaporatorCoolingCapacityFunctionofFlowFractionCurve(
    coolingAndChargeModeTotalEvaporatorCoolingCapacityFunctionofFlowFractionCurve));
  ASSERT_TRUE(coilCoolingDXSingleSpeedThermalStorage.coolingAndChargeModeTotalEvaporatorCoolingCapacityFunctionofFlowFractionCurve());
  EXPECT_EQ(coolingAndChargeModeTotalEvaporatorCoolingCapacityFunctionofFlowFractionCurve,
            coilCoolingDXSingleSpeedThermalStorage.coolingAndChargeModeTotalEvaporatorCoolingCapacityFunctionofFlowFractionCurve().get());

  // Cooling And Charge Mode Evaporator Energy Input Ratio Function of Temperature Curve: Optional Object
  CurveTriquadratic coolingAndChargeModeEvaporatorEnergyInputRatioFunctionofTemperatureCurve(m);
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setCoolingAndChargeModeEvaporatorEnergyInputRatioFunctionofTemperatureCurve(
    coolingAndChargeModeEvaporatorEnergyInputRatioFunctionofTemperatureCurve));
  ASSERT_TRUE(coilCoolingDXSingleSpeedThermalStorage.coolingAndChargeModeEvaporatorEnergyInputRatioFunctionofTemperatureCurve());
  EXPECT_EQ(coolingAndChargeModeEvaporatorEnergyInputRatioFunctionofTemperatureCurve,
            coilCoolingDXSingleSpeedThermalStorage.coolingAndChargeModeEvaporatorEnergyInputRatioFunctionofTemperatureCurve().get());

  // Cooling And Charge Mode Evaporator Energy Input Ratio Function of Flow Fraction Curve: Optional Object
  CurveQuadratic coolingAndChargeModeEvaporatorEnergyInputRatioFunctionofFlowFractionCurve(m);
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setCoolingAndChargeModeEvaporatorEnergyInputRatioFunctionofFlowFractionCurve(
    coolingAndChargeModeEvaporatorEnergyInputRatioFunctionofFlowFractionCurve));
  ASSERT_TRUE(coilCoolingDXSingleSpeedThermalStorage.coolingAndChargeModeEvaporatorEnergyInputRatioFunctionofFlowFractionCurve());
  EXPECT_EQ(coolingAndChargeModeEvaporatorEnergyInputRatioFunctionofFlowFractionCurve,
            coilCoolingDXSingleSpeedThermalStorage.coolingAndChargeModeEvaporatorEnergyInputRatioFunctionofFlowFractionCurve().get());

  // Cooling And Charge Mode Evaporator Part Load Fraction Correlation Curve: Optional Object
  CurveQuadratic coolingAndChargeModeEvaporatorPartLoadFractionCorrelationCurve(m);
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setCoolingAndChargeModeEvaporatorPartLoadFractionCorrelationCurve(
    coolingAndChargeModeEvaporatorPartLoadFractionCorrelationCurve));
  ASSERT_TRUE(coilCoolingDXSingleSpeedThermalStorage.coolingAndChargeModeEvaporatorPartLoadFractionCorrelationCurve());
  EXPECT_EQ(coolingAndChargeModeEvaporatorPartLoadFractionCorrelationCurve,
            coilCoolingDXSingleSpeedThermalStorage.coolingAndChargeModeEvaporatorPartLoadFractionCorrelationCurve().get());

  // Cooling And Charge Mode Storage Charge Capacity Function of Temperature Curve: Optional Object
  CurveTriquadratic coolingAndChargeModeStorageChargeCapacityFunctionofTemperatureCurve(m);
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setCoolingAndChargeModeStorageChargeCapacityFunctionofTemperatureCurve(
    coolingAndChargeModeStorageChargeCapacityFunctionofTemperatureCurve));
  ASSERT_TRUE(coilCoolingDXSingleSpeedThermalStorage.coolingAndChargeModeStorageChargeCapacityFunctionofTemperatureCurve());
  EXPECT_EQ(coolingAndChargeModeStorageChargeCapacityFunctionofTemperatureCurve,
            coilCoolingDXSingleSpeedThermalStorage.coolingAndChargeModeStorageChargeCapacityFunctionofTemperatureCurve().get());

  // Cooling And Charge Mode Storage Charge Capacity Function of Total Evaporator PLR Curve: Optional Object
  CurveQuadratic coolingAndChargeModeStorageChargeCapacityFunctionofTotalEvaporatorPLRCurve(m);
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setCoolingAndChargeModeStorageChargeCapacityFunctionofTotalEvaporatorPLRCurve(
    coolingAndChargeModeStorageChargeCapacityFunctionofTotalEvaporatorPLRCurve));
  ASSERT_TRUE(coilCoolingDXSingleSpeedThermalStorage.coolingAndChargeModeStorageChargeCapacityFunctionofTotalEvaporatorPLRCurve());
  EXPECT_EQ(coolingAndChargeModeStorageChargeCapacityFunctionofTotalEvaporatorPLRCurve,
            coilCoolingDXSingleSpeedThermalStorage.coolingAndChargeModeStorageChargeCapacityFunctionofTotalEvaporatorPLRCurve().get());

  // Cooling And Charge Mode Storage Energy Input Ratio Function of Temperature Curve: Optional Object
  CurveTriquadratic coolingAndChargeModeStorageEnergyInputRatioFunctionofTemperatureCurve(m);
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setCoolingAndChargeModeStorageEnergyInputRatioFunctionofTemperatureCurve(
    coolingAndChargeModeStorageEnergyInputRatioFunctionofTemperatureCurve));
  ASSERT_TRUE(coilCoolingDXSingleSpeedThermalStorage.coolingAndChargeModeStorageEnergyInputRatioFunctionofTemperatureCurve());
  EXPECT_EQ(coolingAndChargeModeStorageEnergyInputRatioFunctionofTemperatureCurve,
            coilCoolingDXSingleSpeedThermalStorage.coolingAndChargeModeStorageEnergyInputRatioFunctionofTemperatureCurve().get());

  // Cooling And Charge Mode Storage Energy Input Ratio Function of Flow Fraction Curve: Optional Object
  CurveQuadratic coolingAndChargeModeStorageEnergyInputRatioFunctionofFlowFractionCurve(m);
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setCoolingAndChargeModeStorageEnergyInputRatioFunctionofFlowFractionCurve(
    coolingAndChargeModeStorageEnergyInputRatioFunctionofFlowFractionCurve));
  ASSERT_TRUE(coilCoolingDXSingleSpeedThermalStorage.coolingAndChargeModeStorageEnergyInputRatioFunctionofFlowFractionCurve());
  EXPECT_EQ(coolingAndChargeModeStorageEnergyInputRatioFunctionofFlowFractionCurve,
            coilCoolingDXSingleSpeedThermalStorage.coolingAndChargeModeStorageEnergyInputRatioFunctionofFlowFractionCurve().get());

  // Cooling And Charge Mode Storage Energy Part Load Fraction Correlation Curve: Optional Object
  CurveQuadratic coolingAndChargeModeStorageEnergyPartLoadFractionCorrelationCurve(m);
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setCoolingAndChargeModeStorageEnergyPartLoadFractionCorrelationCurve(
    coolingAndChargeModeStorageEnergyPartLoadFractionCorrelationCurve));
  ASSERT_TRUE(coilCoolingDXSingleSpeedThermalStorage.coolingAndChargeModeStorageEnergyPartLoadFractionCorrelationCurve());
  EXPECT_EQ(coolingAndChargeModeStorageEnergyPartLoadFractionCorrelationCurve,
            coilCoolingDXSingleSpeedThermalStorage.coolingAndChargeModeStorageEnergyPartLoadFractionCorrelationCurve().get());

  // Cooling And Charge Mode Sensible Heat Ratio Function of Temperature Curve: Optional Object
  CurveBiquadratic coolingAndChargeModeSensibleHeatRatioFunctionofTemperatureCurve(m);
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setCoolingAndChargeModeSensibleHeatRatioFunctionofTemperatureCurve(
    coolingAndChargeModeSensibleHeatRatioFunctionofTemperatureCurve));
  ASSERT_TRUE(coilCoolingDXSingleSpeedThermalStorage.coolingAndChargeModeSensibleHeatRatioFunctionofTemperatureCurve());
  EXPECT_EQ(coolingAndChargeModeSensibleHeatRatioFunctionofTemperatureCurve,
            coilCoolingDXSingleSpeedThermalStorage.coolingAndChargeModeSensibleHeatRatioFunctionofTemperatureCurve().get());

  // Cooling And Charge Mode Sensible Heat Ratio Function of Flow Fraction Curve: Optional Object
  CurveQuadratic coolingAndChargeModeSensibleHeatRatioFunctionofFlowFractionCurve(m);
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setCoolingAndChargeModeSensibleHeatRatioFunctionofFlowFractionCurve(
    coolingAndChargeModeSensibleHeatRatioFunctionofFlowFractionCurve));
  ASSERT_TRUE(coilCoolingDXSingleSpeedThermalStorage.coolingAndChargeModeSensibleHeatRatioFunctionofFlowFractionCurve());
  EXPECT_EQ(coolingAndChargeModeSensibleHeatRatioFunctionofFlowFractionCurve,
            coilCoolingDXSingleSpeedThermalStorage.coolingAndChargeModeSensibleHeatRatioFunctionofFlowFractionCurve().get());

  // Cooling And Discharge Mode Available: Required Boolean
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setCoolingAndDischargeModeAvailable(true));
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.coolingAndDischargeModeAvailable());
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setCoolingAndDischargeModeAvailable(false));
  EXPECT_FALSE(coilCoolingDXSingleSpeedThermalStorage.coolingAndDischargeModeAvailable());

  // Cooling And Discharge Mode Rated Total Evaporator Cooling Capacity: Optional Double
  // Autosize
  coilCoolingDXSingleSpeedThermalStorage.autosizeCoolingAndDischargeModeRatedTotalEvaporatorCoolingCapacity();
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.isCoolingAndDischargeModeRatedTotalEvaporatorCoolingCapacityAutosized());
  // Set
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setCoolingAndDischargeModeRatedTotalEvaporatorCoolingCapacity(4.9));
  ASSERT_TRUE(coilCoolingDXSingleSpeedThermalStorage.coolingAndDischargeModeRatedTotalEvaporatorCoolingCapacity());
  EXPECT_EQ(4.9, coilCoolingDXSingleSpeedThermalStorage.coolingAndDischargeModeRatedTotalEvaporatorCoolingCapacity().get());
  // Bad Value
  EXPECT_FALSE(coilCoolingDXSingleSpeedThermalStorage.setCoolingAndDischargeModeRatedTotalEvaporatorCoolingCapacity(-10.0));
  ASSERT_TRUE(coilCoolingDXSingleSpeedThermalStorage.coolingAndDischargeModeRatedTotalEvaporatorCoolingCapacity());
  EXPECT_EQ(4.9, coilCoolingDXSingleSpeedThermalStorage.coolingAndDischargeModeRatedTotalEvaporatorCoolingCapacity().get());
  EXPECT_FALSE(coilCoolingDXSingleSpeedThermalStorage.isCoolingAndDischargeModeRatedTotalEvaporatorCoolingCapacityAutosized());

  // Cooling And Discharge Mode Evaporator Capacity Sizing Factor: Required Double
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setCoolingAndDischargeModeEvaporatorCapacitySizingFactor(5.0));
  EXPECT_EQ(5.0, coilCoolingDXSingleSpeedThermalStorage.coolingAndDischargeModeEvaporatorCapacitySizingFactor());

  // Cooling And Discharge Mode Rated Storage Discharging Capacity: Optional Double
  // Autosize
  coilCoolingDXSingleSpeedThermalStorage.autosizeCoolingAndDischargeModeRatedStorageDischargingCapacity();
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.isCoolingAndDischargeModeRatedStorageDischargingCapacityAutosized());
  // Set
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setCoolingAndDischargeModeRatedStorageDischargingCapacity(5.1));
  ASSERT_TRUE(coilCoolingDXSingleSpeedThermalStorage.coolingAndDischargeModeRatedStorageDischargingCapacity());
  EXPECT_EQ(5.1, coilCoolingDXSingleSpeedThermalStorage.coolingAndDischargeModeRatedStorageDischargingCapacity().get());
  // Bad Value
  EXPECT_FALSE(coilCoolingDXSingleSpeedThermalStorage.setCoolingAndDischargeModeRatedStorageDischargingCapacity(-10.0));
  ASSERT_TRUE(coilCoolingDXSingleSpeedThermalStorage.coolingAndDischargeModeRatedStorageDischargingCapacity());
  EXPECT_EQ(5.1, coilCoolingDXSingleSpeedThermalStorage.coolingAndDischargeModeRatedStorageDischargingCapacity().get());
  EXPECT_FALSE(coilCoolingDXSingleSpeedThermalStorage.isCoolingAndDischargeModeRatedStorageDischargingCapacityAutosized());

  // Cooling And Discharge Mode Storage Discharge Capacity Sizing Factor: Required Double
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setCoolingAndDischargeModeStorageDischargeCapacitySizingFactor(5.2));
  EXPECT_EQ(5.2, coilCoolingDXSingleSpeedThermalStorage.coolingAndDischargeModeStorageDischargeCapacitySizingFactor());

  // Cooling And Discharge Mode Rated Sensible Heat Ratio: Required Double
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setCoolingAndDischargeModeRatedSensibleHeatRatio(0.981));
  EXPECT_EQ(0.981, coilCoolingDXSingleSpeedThermalStorage.coolingAndDischargeModeRatedSensibleHeatRatio());
  // Bad Value
  EXPECT_FALSE(coilCoolingDXSingleSpeedThermalStorage.setCoolingAndDischargeModeRatedSensibleHeatRatio(-10.0));
  EXPECT_EQ(0.981, coilCoolingDXSingleSpeedThermalStorage.coolingAndDischargeModeRatedSensibleHeatRatio());

  // Cooling And Discharge Mode Cooling Rated COP: Required Double
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setCoolingAndDischargeModeCoolingRatedCOP(5.4));
  EXPECT_EQ(5.4, coilCoolingDXSingleSpeedThermalStorage.coolingAndDischargeModeCoolingRatedCOP());
  // Bad Value
  EXPECT_FALSE(coilCoolingDXSingleSpeedThermalStorage.setCoolingAndDischargeModeCoolingRatedCOP(-10.0));
  EXPECT_EQ(5.4, coilCoolingDXSingleSpeedThermalStorage.coolingAndDischargeModeCoolingRatedCOP());

  // Cooling And Discharge Mode Discharging Rated COP: Required Double
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setCoolingAndDischargeModeDischargingRatedCOP(5.5));
  EXPECT_EQ(5.5, coilCoolingDXSingleSpeedThermalStorage.coolingAndDischargeModeDischargingRatedCOP());
  // Bad Value
  EXPECT_FALSE(coilCoolingDXSingleSpeedThermalStorage.setCoolingAndDischargeModeDischargingRatedCOP(-10.0));
  EXPECT_EQ(5.5, coilCoolingDXSingleSpeedThermalStorage.coolingAndDischargeModeDischargingRatedCOP());

  // Cooling And Discharge Mode Total Evaporator Cooling Capacity Function of Temperature Curve: Optional Object
  CurveTriquadratic coolingAndDischargeModeTotalEvaporatorCoolingCapacityFunctionofTemperatureCurve(m);
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setCoolingAndDischargeModeTotalEvaporatorCoolingCapacityFunctionofTemperatureCurve(
    coolingAndDischargeModeTotalEvaporatorCoolingCapacityFunctionofTemperatureCurve));
  ASSERT_TRUE(coilCoolingDXSingleSpeedThermalStorage.coolingAndDischargeModeTotalEvaporatorCoolingCapacityFunctionofTemperatureCurve());
  EXPECT_EQ(coolingAndDischargeModeTotalEvaporatorCoolingCapacityFunctionofTemperatureCurve,
            coilCoolingDXSingleSpeedThermalStorage.coolingAndDischargeModeTotalEvaporatorCoolingCapacityFunctionofTemperatureCurve().get());

  // Cooling And Discharge Mode Total Evaporator Cooling Capacity Function of Flow Fraction Curve: Optional Object
  CurveQuadratic coolingAndDischargeModeTotalEvaporatorCoolingCapacityFunctionofFlowFractionCurve(m);
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setCoolingAndDischargeModeTotalEvaporatorCoolingCapacityFunctionofFlowFractionCurve(
    coolingAndDischargeModeTotalEvaporatorCoolingCapacityFunctionofFlowFractionCurve));
  ASSERT_TRUE(coilCoolingDXSingleSpeedThermalStorage.coolingAndDischargeModeTotalEvaporatorCoolingCapacityFunctionofFlowFractionCurve());
  EXPECT_EQ(coolingAndDischargeModeTotalEvaporatorCoolingCapacityFunctionofFlowFractionCurve,
            coilCoolingDXSingleSpeedThermalStorage.coolingAndDischargeModeTotalEvaporatorCoolingCapacityFunctionofFlowFractionCurve().get());

  // Cooling And Discharge Mode Evaporator Energy Input Ratio Function of Temperature Curve: Optional Object
  CurveTriquadratic coolingAndDischargeModeEvaporatorEnergyInputRatioFunctionofTemperatureCurve(m);
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setCoolingAndDischargeModeEvaporatorEnergyInputRatioFunctionofTemperatureCurve(
    coolingAndDischargeModeEvaporatorEnergyInputRatioFunctionofTemperatureCurve));
  ASSERT_TRUE(coilCoolingDXSingleSpeedThermalStorage.coolingAndDischargeModeEvaporatorEnergyInputRatioFunctionofTemperatureCurve());
  EXPECT_EQ(coolingAndDischargeModeEvaporatorEnergyInputRatioFunctionofTemperatureCurve,
            coilCoolingDXSingleSpeedThermalStorage.coolingAndDischargeModeEvaporatorEnergyInputRatioFunctionofTemperatureCurve().get());

  // Cooling And Discharge Mode Evaporator Energy Input Ratio Function of Flow Fraction Curve: Optional Object
  CurveQuadratic coolingAndDischargeModeEvaporatorEnergyInputRatioFunctionofFlowFractionCurve(m);
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setCoolingAndDischargeModeEvaporatorEnergyInputRatioFunctionofFlowFractionCurve(
    coolingAndDischargeModeEvaporatorEnergyInputRatioFunctionofFlowFractionCurve));
  ASSERT_TRUE(coilCoolingDXSingleSpeedThermalStorage.coolingAndDischargeModeEvaporatorEnergyInputRatioFunctionofFlowFractionCurve());
  EXPECT_EQ(coolingAndDischargeModeEvaporatorEnergyInputRatioFunctionofFlowFractionCurve,
            coilCoolingDXSingleSpeedThermalStorage.coolingAndDischargeModeEvaporatorEnergyInputRatioFunctionofFlowFractionCurve().get());

  // Cooling And Discharge Mode Evaporator Part Load Fraction Correlation Curve: Optional Object
  CurveQuadratic coolingAndDischargeModeEvaporatorPartLoadFractionCorrelationCurve(m);
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setCoolingAndDischargeModeEvaporatorPartLoadFractionCorrelationCurve(
    coolingAndDischargeModeEvaporatorPartLoadFractionCorrelationCurve));
  ASSERT_TRUE(coilCoolingDXSingleSpeedThermalStorage.coolingAndDischargeModeEvaporatorPartLoadFractionCorrelationCurve());
  EXPECT_EQ(coolingAndDischargeModeEvaporatorPartLoadFractionCorrelationCurve,
            coilCoolingDXSingleSpeedThermalStorage.coolingAndDischargeModeEvaporatorPartLoadFractionCorrelationCurve().get());

  // Cooling And Discharge Mode Storage Discharge Capacity Function of Temperature Curve: Optional Object
  CurveTriquadratic coolingAndDischargeModeStorageDischargeCapacityFunctionofTemperatureCurve(m);
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setCoolingAndDischargeModeStorageDischargeCapacityFunctionofTemperatureCurve(
    coolingAndDischargeModeStorageDischargeCapacityFunctionofTemperatureCurve));
  ASSERT_TRUE(coilCoolingDXSingleSpeedThermalStorage.coolingAndDischargeModeStorageDischargeCapacityFunctionofTemperatureCurve());
  EXPECT_EQ(coolingAndDischargeModeStorageDischargeCapacityFunctionofTemperatureCurve,
            coilCoolingDXSingleSpeedThermalStorage.coolingAndDischargeModeStorageDischargeCapacityFunctionofTemperatureCurve().get());

  // Cooling And Discharge Mode Storage Discharge Capacity Function of Flow Fraction Curve: Optional Object
  CurveQuadratic coolingAndDischargeModeStorageDischargeCapacityFunctionofFlowFractionCurve(m);
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setCoolingAndDischargeModeStorageDischargeCapacityFunctionofFlowFractionCurve(
    coolingAndDischargeModeStorageDischargeCapacityFunctionofFlowFractionCurve));
  ASSERT_TRUE(coilCoolingDXSingleSpeedThermalStorage.coolingAndDischargeModeStorageDischargeCapacityFunctionofFlowFractionCurve());
  EXPECT_EQ(coolingAndDischargeModeStorageDischargeCapacityFunctionofFlowFractionCurve,
            coilCoolingDXSingleSpeedThermalStorage.coolingAndDischargeModeStorageDischargeCapacityFunctionofFlowFractionCurve().get());

  // Cooling And Discharge Mode Storage Discharge Capacity Function of Total Evaporator PLR Curve: Optional Object
  CurveQuadratic coolingAndDischargeModeStorageDischargeCapacityFunctionofTotalEvaporatorPLRCurve(m);
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setCoolingAndDischargeModeStorageDischargeCapacityFunctionofTotalEvaporatorPLRCurve(
    coolingAndDischargeModeStorageDischargeCapacityFunctionofTotalEvaporatorPLRCurve));
  ASSERT_TRUE(coilCoolingDXSingleSpeedThermalStorage.coolingAndDischargeModeStorageDischargeCapacityFunctionofTotalEvaporatorPLRCurve());
  EXPECT_EQ(coolingAndDischargeModeStorageDischargeCapacityFunctionofTotalEvaporatorPLRCurve,
            coilCoolingDXSingleSpeedThermalStorage.coolingAndDischargeModeStorageDischargeCapacityFunctionofTotalEvaporatorPLRCurve().get());

  // Cooling And Discharge Mode Storage Energy Input Ratio Function of Temperature Curve: Optional Object
  CurveTriquadratic coolingAndDischargeModeStorageEnergyInputRatioFunctionofTemperatureCurve(m);
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setCoolingAndDischargeModeStorageEnergyInputRatioFunctionofTemperatureCurve(
    coolingAndDischargeModeStorageEnergyInputRatioFunctionofTemperatureCurve));
  ASSERT_TRUE(coilCoolingDXSingleSpeedThermalStorage.coolingAndDischargeModeStorageEnergyInputRatioFunctionofTemperatureCurve());
  EXPECT_EQ(coolingAndDischargeModeStorageEnergyInputRatioFunctionofTemperatureCurve,
            coilCoolingDXSingleSpeedThermalStorage.coolingAndDischargeModeStorageEnergyInputRatioFunctionofTemperatureCurve().get());

  // Cooling And Discharge Mode Storage Energy Input Ratio Function of Flow Fraction Curve: Optional Object
  CurveQuadratic coolingAndDischargeModeStorageEnergyInputRatioFunctionofFlowFractionCurve(m);
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setCoolingAndDischargeModeStorageEnergyInputRatioFunctionofFlowFractionCurve(
    coolingAndDischargeModeStorageEnergyInputRatioFunctionofFlowFractionCurve));
  ASSERT_TRUE(coilCoolingDXSingleSpeedThermalStorage.coolingAndDischargeModeStorageEnergyInputRatioFunctionofFlowFractionCurve());
  EXPECT_EQ(coolingAndDischargeModeStorageEnergyInputRatioFunctionofFlowFractionCurve,
            coilCoolingDXSingleSpeedThermalStorage.coolingAndDischargeModeStorageEnergyInputRatioFunctionofFlowFractionCurve().get());

  // Cooling And Discharge Mode Storage Energy Part Load Fraction Correlation Curve: Optional Object
  CurveQuadratic coolingAndDischargeModeStorageEnergyPartLoadFractionCorrelationCurve(m);
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setCoolingAndDischargeModeStorageEnergyPartLoadFractionCorrelationCurve(
    coolingAndDischargeModeStorageEnergyPartLoadFractionCorrelationCurve));
  ASSERT_TRUE(coilCoolingDXSingleSpeedThermalStorage.coolingAndDischargeModeStorageEnergyPartLoadFractionCorrelationCurve());
  EXPECT_EQ(coolingAndDischargeModeStorageEnergyPartLoadFractionCorrelationCurve,
            coilCoolingDXSingleSpeedThermalStorage.coolingAndDischargeModeStorageEnergyPartLoadFractionCorrelationCurve().get());

  // Cooling And Discharge Mode Sensible Heat Ratio Function of Temperature Curve: Optional Object
  CurveBiquadratic coolingAndDischargeModeSensibleHeatRatioFunctionofTemperatureCurve(m);
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setCoolingAndDischargeModeSensibleHeatRatioFunctionofTemperatureCurve(
    coolingAndDischargeModeSensibleHeatRatioFunctionofTemperatureCurve));
  ASSERT_TRUE(coilCoolingDXSingleSpeedThermalStorage.coolingAndDischargeModeSensibleHeatRatioFunctionofTemperatureCurve());
  EXPECT_EQ(coolingAndDischargeModeSensibleHeatRatioFunctionofTemperatureCurve,
            coilCoolingDXSingleSpeedThermalStorage.coolingAndDischargeModeSensibleHeatRatioFunctionofTemperatureCurve().get());

  // Cooling And Discharge Mode Sensible Heat Ratio Function of Flow Fraction Curve: Optional Object
  CurveQuadratic coolingAndDischargeModeSensibleHeatRatioFunctionofFlowFractionCurve(m);
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setCoolingAndDischargeModeSensibleHeatRatioFunctionofFlowFractionCurve(
    coolingAndDischargeModeSensibleHeatRatioFunctionofFlowFractionCurve));
  ASSERT_TRUE(coilCoolingDXSingleSpeedThermalStorage.coolingAndDischargeModeSensibleHeatRatioFunctionofFlowFractionCurve());
  EXPECT_EQ(coolingAndDischargeModeSensibleHeatRatioFunctionofFlowFractionCurve,
            coilCoolingDXSingleSpeedThermalStorage.coolingAndDischargeModeSensibleHeatRatioFunctionofFlowFractionCurve().get());

  // Charge Only Mode Available: Required Boolean
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setChargeOnlyModeAvailable(true));
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.chargeOnlyModeAvailable());
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setChargeOnlyModeAvailable(false));
  EXPECT_FALSE(coilCoolingDXSingleSpeedThermalStorage.chargeOnlyModeAvailable());

  // Charge Only Mode Rated Storage Charging Capacity: Optional Double
  // Autosize
  coilCoolingDXSingleSpeedThermalStorage.autosizeChargeOnlyModeRatedStorageChargingCapacity();
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.isChargeOnlyModeRatedStorageChargingCapacityAutosized());
  // Set
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setChargeOnlyModeRatedStorageChargingCapacity(7.0));
  ASSERT_TRUE(coilCoolingDXSingleSpeedThermalStorage.chargeOnlyModeRatedStorageChargingCapacity());
  EXPECT_EQ(7.0, coilCoolingDXSingleSpeedThermalStorage.chargeOnlyModeRatedStorageChargingCapacity().get());
  // Bad Value
  EXPECT_FALSE(coilCoolingDXSingleSpeedThermalStorage.setChargeOnlyModeRatedStorageChargingCapacity(-10.0));
  ASSERT_TRUE(coilCoolingDXSingleSpeedThermalStorage.chargeOnlyModeRatedStorageChargingCapacity());
  EXPECT_EQ(7.0, coilCoolingDXSingleSpeedThermalStorage.chargeOnlyModeRatedStorageChargingCapacity().get());
  EXPECT_FALSE(coilCoolingDXSingleSpeedThermalStorage.isChargeOnlyModeRatedStorageChargingCapacityAutosized());

  // Charge Only Mode Capacity Sizing Factor: Required Double
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setChargeOnlyModeCapacitySizingFactor(7.1));
  EXPECT_EQ(7.1, coilCoolingDXSingleSpeedThermalStorage.chargeOnlyModeCapacitySizingFactor());

  // Charge Only Mode Charging Rated COP: Required Double
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setChargeOnlyModeChargingRatedCOP(7.2));
  EXPECT_EQ(7.2, coilCoolingDXSingleSpeedThermalStorage.chargeOnlyModeChargingRatedCOP());
  // Bad Value
  EXPECT_FALSE(coilCoolingDXSingleSpeedThermalStorage.setChargeOnlyModeChargingRatedCOP(-10.0));
  EXPECT_EQ(7.2, coilCoolingDXSingleSpeedThermalStorage.chargeOnlyModeChargingRatedCOP());

  // Charge Only Mode Storage Charge Capacity Function of Temperature Curve: Optional Object
  CurveBiquadratic chargeOnlyModeStorageChargeCapacityFunctionofTemperatureCurve(m);
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setChargeOnlyModeStorageChargeCapacityFunctionofTemperatureCurve(
    chargeOnlyModeStorageChargeCapacityFunctionofTemperatureCurve));
  ASSERT_TRUE(coilCoolingDXSingleSpeedThermalStorage.chargeOnlyModeStorageChargeCapacityFunctionofTemperatureCurve());
  EXPECT_EQ(chargeOnlyModeStorageChargeCapacityFunctionofTemperatureCurve,
            coilCoolingDXSingleSpeedThermalStorage.chargeOnlyModeStorageChargeCapacityFunctionofTemperatureCurve().get());

  // Charge Only Mode Storage Energy Input Ratio Function of Temperature Curve: Optional Object
  CurveBiquadratic chargeOnlyModeStorageEnergyInputRatioFunctionofTemperatureCurve(m);
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setChargeOnlyModeStorageEnergyInputRatioFunctionofTemperatureCurve(
    chargeOnlyModeStorageEnergyInputRatioFunctionofTemperatureCurve));
  ASSERT_TRUE(coilCoolingDXSingleSpeedThermalStorage.chargeOnlyModeStorageEnergyInputRatioFunctionofTemperatureCurve());
  EXPECT_EQ(chargeOnlyModeStorageEnergyInputRatioFunctionofTemperatureCurve,
            coilCoolingDXSingleSpeedThermalStorage.chargeOnlyModeStorageEnergyInputRatioFunctionofTemperatureCurve().get());

  // Discharge Only Mode Available: Required Boolean
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setDischargeOnlyModeAvailable(true));
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.dischargeOnlyModeAvailable());
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setDischargeOnlyModeAvailable(false));
  EXPECT_FALSE(coilCoolingDXSingleSpeedThermalStorage.dischargeOnlyModeAvailable());

  // Discharge Only Mode Rated Storage Discharging Capacity: Optional Double
  // Autosize
  coilCoolingDXSingleSpeedThermalStorage.autosizeDischargeOnlyModeRatedStorageDischargingCapacity();
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.isDischargeOnlyModeRatedStorageDischargingCapacityAutosized());
  // Set
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setDischargeOnlyModeRatedStorageDischargingCapacity(7.6));
  ASSERT_TRUE(coilCoolingDXSingleSpeedThermalStorage.dischargeOnlyModeRatedStorageDischargingCapacity());
  EXPECT_EQ(7.6, coilCoolingDXSingleSpeedThermalStorage.dischargeOnlyModeRatedStorageDischargingCapacity().get());
  // Bad Value
  EXPECT_FALSE(coilCoolingDXSingleSpeedThermalStorage.setDischargeOnlyModeRatedStorageDischargingCapacity(-10.0));
  ASSERT_TRUE(coilCoolingDXSingleSpeedThermalStorage.dischargeOnlyModeRatedStorageDischargingCapacity());
  EXPECT_EQ(7.6, coilCoolingDXSingleSpeedThermalStorage.dischargeOnlyModeRatedStorageDischargingCapacity().get());
  EXPECT_FALSE(coilCoolingDXSingleSpeedThermalStorage.isDischargeOnlyModeRatedStorageDischargingCapacityAutosized());

  // Discharge Only Mode Capacity Sizing Factor: Required Double
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setDischargeOnlyModeCapacitySizingFactor(7.7));
  EXPECT_EQ(7.7, coilCoolingDXSingleSpeedThermalStorage.dischargeOnlyModeCapacitySizingFactor());

  // Discharge Only Mode Rated Sensible Heat Ratio: Required Double
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setDischargeOnlyModeRatedSensibleHeatRatio(0.987));
  EXPECT_EQ(0.987, coilCoolingDXSingleSpeedThermalStorage.dischargeOnlyModeRatedSensibleHeatRatio());
  // Bad Value
  EXPECT_FALSE(coilCoolingDXSingleSpeedThermalStorage.setDischargeOnlyModeRatedSensibleHeatRatio(-10.0));
  EXPECT_EQ(0.987, coilCoolingDXSingleSpeedThermalStorage.dischargeOnlyModeRatedSensibleHeatRatio());

  // Discharge Only Mode Rated COP: Required Double
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setDischargeOnlyModeRatedCOP(7.9));
  EXPECT_EQ(7.9, coilCoolingDXSingleSpeedThermalStorage.dischargeOnlyModeRatedCOP());
  // Bad Value
  EXPECT_FALSE(coilCoolingDXSingleSpeedThermalStorage.setDischargeOnlyModeRatedCOP(-10.0));
  EXPECT_EQ(7.9, coilCoolingDXSingleSpeedThermalStorage.dischargeOnlyModeRatedCOP());

  // Discharge Only Mode Storage Discharge Capacity Function of Temperature Curve: Optional Object
  CurveBiquadratic dischargeOnlyModeStorageDischargeCapacityFunctionofTemperatureCurve(m);
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setDischargeOnlyModeStorageDischargeCapacityFunctionofTemperatureCurve(
    dischargeOnlyModeStorageDischargeCapacityFunctionofTemperatureCurve));
  ASSERT_TRUE(coilCoolingDXSingleSpeedThermalStorage.dischargeOnlyModeStorageDischargeCapacityFunctionofTemperatureCurve());
  EXPECT_EQ(dischargeOnlyModeStorageDischargeCapacityFunctionofTemperatureCurve,
            coilCoolingDXSingleSpeedThermalStorage.dischargeOnlyModeStorageDischargeCapacityFunctionofTemperatureCurve().get());

  // Discharge Only Mode Storage Discharge Capacity Function of Flow Fraction Curve: Optional Object
  CurveQuadratic dischargeOnlyModeStorageDischargeCapacityFunctionofFlowFractionCurve(m);
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setDischargeOnlyModeStorageDischargeCapacityFunctionofFlowFractionCurve(
    dischargeOnlyModeStorageDischargeCapacityFunctionofFlowFractionCurve));
  ASSERT_TRUE(coilCoolingDXSingleSpeedThermalStorage.dischargeOnlyModeStorageDischargeCapacityFunctionofFlowFractionCurve());
  EXPECT_EQ(dischargeOnlyModeStorageDischargeCapacityFunctionofFlowFractionCurve,
            coilCoolingDXSingleSpeedThermalStorage.dischargeOnlyModeStorageDischargeCapacityFunctionofFlowFractionCurve().get());

  // Discharge Only Mode Energy Input Ratio Function of Temperature Curve: Optional Object
  CurveBiquadratic dischargeOnlyModeEnergyInputRatioFunctionofTemperatureCurve(m);
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setDischargeOnlyModeEnergyInputRatioFunctionofTemperatureCurve(
    dischargeOnlyModeEnergyInputRatioFunctionofTemperatureCurve));
  ASSERT_TRUE(coilCoolingDXSingleSpeedThermalStorage.dischargeOnlyModeEnergyInputRatioFunctionofTemperatureCurve());
  EXPECT_EQ(dischargeOnlyModeEnergyInputRatioFunctionofTemperatureCurve,
            coilCoolingDXSingleSpeedThermalStorage.dischargeOnlyModeEnergyInputRatioFunctionofTemperatureCurve().get());

  // Discharge Only Mode Energy Input Ratio Function of Flow Fraction Curve: Optional Object
  CurveQuadratic dischargeOnlyModeEnergyInputRatioFunctionofFlowFractionCurve(m);
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setDischargeOnlyModeEnergyInputRatioFunctionofFlowFractionCurve(
    dischargeOnlyModeEnergyInputRatioFunctionofFlowFractionCurve));
  ASSERT_TRUE(coilCoolingDXSingleSpeedThermalStorage.dischargeOnlyModeEnergyInputRatioFunctionofFlowFractionCurve());
  EXPECT_EQ(dischargeOnlyModeEnergyInputRatioFunctionofFlowFractionCurve,
            coilCoolingDXSingleSpeedThermalStorage.dischargeOnlyModeEnergyInputRatioFunctionofFlowFractionCurve().get());

  // Discharge Only Mode Part Load Fraction Correlation Curve: Optional Object
  CurveQuadratic dischargeOnlyModePartLoadFractionCorrelationCurve(m);
  EXPECT_TRUE(
    coilCoolingDXSingleSpeedThermalStorage.setDischargeOnlyModePartLoadFractionCorrelationCurve(dischargeOnlyModePartLoadFractionCorrelationCurve));
  ASSERT_TRUE(coilCoolingDXSingleSpeedThermalStorage.dischargeOnlyModePartLoadFractionCorrelationCurve());
  EXPECT_EQ(dischargeOnlyModePartLoadFractionCorrelationCurve,
            coilCoolingDXSingleSpeedThermalStorage.dischargeOnlyModePartLoadFractionCorrelationCurve().get());

  // Discharge Only Mode Sensible Heat Ratio Function of Temperature Curve: Optional Object
  CurveBiquadratic dischargeOnlyModeSensibleHeatRatioFunctionofTemperatureCurve(m);
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setDischargeOnlyModeSensibleHeatRatioFunctionofTemperatureCurve(
    dischargeOnlyModeSensibleHeatRatioFunctionofTemperatureCurve));
  ASSERT_TRUE(coilCoolingDXSingleSpeedThermalStorage.dischargeOnlyModeSensibleHeatRatioFunctionofTemperatureCurve());
  EXPECT_EQ(dischargeOnlyModeSensibleHeatRatioFunctionofTemperatureCurve,
            coilCoolingDXSingleSpeedThermalStorage.dischargeOnlyModeSensibleHeatRatioFunctionofTemperatureCurve().get());

  // Discharge Only Mode Sensible Heat Ratio Function of Flow Fraction Curve: Optional Object
  CurveQuadratic dischargeOnlyModeSensibleHeatRatioFunctionofFlowFractionCurve(m);
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setDischargeOnlyModeSensibleHeatRatioFunctionofFlowFractionCurve(
    dischargeOnlyModeSensibleHeatRatioFunctionofFlowFractionCurve));
  ASSERT_TRUE(coilCoolingDXSingleSpeedThermalStorage.dischargeOnlyModeSensibleHeatRatioFunctionofFlowFractionCurve());
  EXPECT_EQ(dischargeOnlyModeSensibleHeatRatioFunctionofFlowFractionCurve,
            coilCoolingDXSingleSpeedThermalStorage.dischargeOnlyModeSensibleHeatRatioFunctionofFlowFractionCurve().get());

  // Ancillary Electric Power: Optional Double
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setAncillaryElectricPower(8.7));
  ASSERT_TRUE(coilCoolingDXSingleSpeedThermalStorage.ancillaryElectricPower());
  EXPECT_EQ(8.7, coilCoolingDXSingleSpeedThermalStorage.ancillaryElectricPower().get());
  // Bad Value
  EXPECT_FALSE(coilCoolingDXSingleSpeedThermalStorage.setAncillaryElectricPower(-10.0));
  ASSERT_TRUE(coilCoolingDXSingleSpeedThermalStorage.ancillaryElectricPower());
  EXPECT_EQ(8.7, coilCoolingDXSingleSpeedThermalStorage.ancillaryElectricPower().get());

  // Cold Weather Operation Minimum Outdoor Air Temperature: Optional Double
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setColdWeatherOperationMinimumOutdoorAirTemperature(8.8));
  ASSERT_TRUE(coilCoolingDXSingleSpeedThermalStorage.coldWeatherOperationMinimumOutdoorAirTemperature());
  EXPECT_EQ(8.8, coilCoolingDXSingleSpeedThermalStorage.coldWeatherOperationMinimumOutdoorAirTemperature().get());

  // Cold Weather Operation Ancillary Power: Optional Double
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setColdWeatherOperationAncillaryPower(8.9));
  ASSERT_TRUE(coilCoolingDXSingleSpeedThermalStorage.coldWeatherOperationAncillaryPower());
  EXPECT_EQ(8.9, coilCoolingDXSingleSpeedThermalStorage.coldWeatherOperationAncillaryPower().get());
  // Bad Value
  EXPECT_FALSE(coilCoolingDXSingleSpeedThermalStorage.setColdWeatherOperationAncillaryPower(-10.0));
  ASSERT_TRUE(coilCoolingDXSingleSpeedThermalStorage.coldWeatherOperationAncillaryPower());
  EXPECT_EQ(8.9, coilCoolingDXSingleSpeedThermalStorage.coldWeatherOperationAncillaryPower().get());

  // Condenser Design Air Flow Rate: Required Double
  // Autosize
  coilCoolingDXSingleSpeedThermalStorage.autosizeCondenserDesignAirFlowRate();
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.isCondenserDesignAirFlowRateAutosized());
  // Set
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setCondenserDesignAirFlowRate(9.2));
  ASSERT_TRUE(coilCoolingDXSingleSpeedThermalStorage.condenserDesignAirFlowRate());
  EXPECT_EQ(9.2, coilCoolingDXSingleSpeedThermalStorage.condenserDesignAirFlowRate().get());
  // Bad Value
  EXPECT_FALSE(coilCoolingDXSingleSpeedThermalStorage.setCondenserDesignAirFlowRate(-10.0));
  ASSERT_TRUE(coilCoolingDXSingleSpeedThermalStorage.condenserDesignAirFlowRate());
  EXPECT_EQ(9.2, coilCoolingDXSingleSpeedThermalStorage.condenserDesignAirFlowRate().get());
  EXPECT_FALSE(coilCoolingDXSingleSpeedThermalStorage.isCondenserDesignAirFlowRateAutosized());

  // Condenser Air Flow Sizing Factor: Required Double
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setCondenserAirFlowSizingFactor(9.3));
  EXPECT_EQ(9.3, coilCoolingDXSingleSpeedThermalStorage.condenserAirFlowSizingFactor());

  // Condenser Type: Required String
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setCondenserType("AirCooled"));
  EXPECT_EQ("AirCooled", coilCoolingDXSingleSpeedThermalStorage.condenserType());
  // Bad Value
  EXPECT_FALSE(coilCoolingDXSingleSpeedThermalStorage.setCondenserType("BADENUM"));
  EXPECT_EQ("AirCooled", coilCoolingDXSingleSpeedThermalStorage.condenserType());

  // Evaporative Condenser Effectiveness: Required Double
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setEvaporativeCondenserEffectiveness(0.99));
  EXPECT_EQ(0.99, coilCoolingDXSingleSpeedThermalStorage.evaporativeCondenserEffectiveness());
  // Bad Value
  EXPECT_FALSE(coilCoolingDXSingleSpeedThermalStorage.setEvaporativeCondenserEffectiveness(-10.0));
  EXPECT_EQ(0.99, coilCoolingDXSingleSpeedThermalStorage.evaporativeCondenserEffectiveness());

  // Evaporative Condenser Pump Rated Power Consumption: Required Double
  // Autosize
  coilCoolingDXSingleSpeedThermalStorage.autosizeEvaporativeCondenserPumpRatedPowerConsumption();
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.isEvaporativeCondenserPumpRatedPowerConsumptionAutosized());
  // Set
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setEvaporativeCondenserPumpRatedPowerConsumption(9.6));
  ASSERT_TRUE(coilCoolingDXSingleSpeedThermalStorage.evaporativeCondenserPumpRatedPowerConsumption());
  EXPECT_EQ(9.6, coilCoolingDXSingleSpeedThermalStorage.evaporativeCondenserPumpRatedPowerConsumption().get());
  // Bad Value
  EXPECT_FALSE(coilCoolingDXSingleSpeedThermalStorage.setEvaporativeCondenserPumpRatedPowerConsumption(-10.0));
  ASSERT_TRUE(coilCoolingDXSingleSpeedThermalStorage.evaporativeCondenserPumpRatedPowerConsumption());
  EXPECT_EQ(9.6, coilCoolingDXSingleSpeedThermalStorage.evaporativeCondenserPumpRatedPowerConsumption().get());
  EXPECT_FALSE(coilCoolingDXSingleSpeedThermalStorage.isEvaporativeCondenserPumpRatedPowerConsumptionAutosized());

  // Basin Heater Capacity: Required Double
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setBasinHeaterCapacity(9.7));
  EXPECT_EQ(9.7, coilCoolingDXSingleSpeedThermalStorage.basinHeaterCapacity());
  // Bad Value
  EXPECT_FALSE(coilCoolingDXSingleSpeedThermalStorage.setBasinHeaterCapacity(-10.0));
  EXPECT_EQ(9.7, coilCoolingDXSingleSpeedThermalStorage.basinHeaterCapacity());

  // Basin Heater Setpoint Temperature: Required Double
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setBasinHeaterSetpointTemperature(11.8));
  EXPECT_EQ(11.8, coilCoolingDXSingleSpeedThermalStorage.basinHeaterSetpointTemperature());
  // Bad Value
  EXPECT_FALSE(coilCoolingDXSingleSpeedThermalStorage.setBasinHeaterSetpointTemperature(-8.0));
  EXPECT_EQ(11.8, coilCoolingDXSingleSpeedThermalStorage.basinHeaterSetpointTemperature());

  // Basin Heater Availability Schedule: Optional Object
  ScheduleConstant basinHeaterAvailabilitySchedule(m);
  EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setBasinHeaterAvailabilitySchedule(basinHeaterAvailabilitySchedule));
  ASSERT_TRUE(coilCoolingDXSingleSpeedThermalStorage.basinHeaterAvailabilitySchedule());
  EXPECT_EQ(basinHeaterAvailabilitySchedule, coilCoolingDXSingleSpeedThermalStorage.basinHeaterAvailabilitySchedule().get());

  // Supply Water Storage Tank: Optional Object
  // boost::optional<WaterStorageTank> supplyWaterStorageTank(m);
  // EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setSupplyWaterStorageTank(supplyWaterStorageTank));
  // ASSERT_TRUE(coilCoolingDXSingleSpeedThermalStorage.supplyWaterStorageTank());
  // EXPECT_EQ(supplyWaterStorageTank, coilCoolingDXSingleSpeedThermalStorage.supplyWaterStorageTank().get());

  // Condensate Collection Water Storage Tank: Optional Object
  // boost::optional<WaterStorageTank> condensateCollectionWaterStorageTank(m);
  // EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setCondensateCollectionWaterStorageTank(condensateCollectionWaterStorageTank));
  // ASSERT_TRUE(coilCoolingDXSingleSpeedThermalStorage.condensateCollectionWaterStorageTank());
  // EXPECT_EQ(condensateCollectionWaterStorageTank, coilCoolingDXSingleSpeedThermalStorage.condensateCollectionWaterStorageTank().get());

  // Storage Tank Plant Connection Design Flow Rate: Optional Double
  // EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setStorageTankPlantConnectionDesignFlowRate(10.4));
  // ASSERT_TRUE(coilCoolingDXSingleSpeedThermalStorage.storageTankPlantConnectionDesignFlowRate());
  // EXPECT_EQ(10.4, coilCoolingDXSingleSpeedThermalStorage.storageTankPlantConnectionDesignFlowRate().get());
  // Bad Value
  // EXPECT_FALSE(coilCoolingDXSingleSpeedThermalStorage.setStorageTankPlantConnectionDesignFlowRate(-10.0));
  // ASSERT_TRUE(coilCoolingDXSingleSpeedThermalStorage.storageTankPlantConnectionDesignFlowRate());
  // EXPECT_EQ(10.4, coilCoolingDXSingleSpeedThermalStorage.storageTankPlantConnectionDesignFlowRate().get());

  // Storage Tank Plant Connection Heat Transfer Effectiveness: Required Double
  // EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setStorageTankPlantConnectionHeatTransferEffectiveness(0.991));
  // EXPECT_EQ(0.991, coilCoolingDXSingleSpeedThermalStorage.storageTankPlantConnectionHeatTransferEffectiveness());
  // Bad Value
  // EXPECT_FALSE(coilCoolingDXSingleSpeedThermalStorage.setStorageTankPlantConnectionHeatTransferEffectiveness(-10.0));
  // EXPECT_EQ(0.991, coilCoolingDXSingleSpeedThermalStorage.storageTankPlantConnectionHeatTransferEffectiveness());

  // Storage Tank Minimum Operating Limit Fluid Temperature: Optional Double
  // EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setStorageTankMinimumOperatingLimitFluidTemperature(10.6));
  // ASSERT_TRUE(coilCoolingDXSingleSpeedThermalStorage.storageTankMinimumOperatingLimitFluidTemperature());
  // EXPECT_EQ(10.6, coilCoolingDXSingleSpeedThermalStorage.storageTankMinimumOperatingLimitFluidTemperature().get());

  // Storage Tank Maximum Operating Limit Fluid Temperature: Optional Double
  // EXPECT_TRUE(coilCoolingDXSingleSpeedThermalStorage.setStorageTankMaximumOperatingLimitFluidTemperature(10.7));
  // ASSERT_TRUE(coilCoolingDXSingleSpeedThermalStorage.storageTankMaximumOperatingLimitFluidTemperature());
  // EXPECT_EQ(10.7, coilCoolingDXSingleSpeedThermalStorage.storageTankMaximumOperatingLimitFluidTemperature().get());
}
TEST_F(ModelFixture, CoilCoolingDXSingleSpeedThermalStorage_HeatCoolFuelTypes) {
  Model m;

  CoilCoolingDXSingleSpeedThermalStorage coilCoolingDXSingleSpeedThermalStorage(m);

  EXPECT_EQ(ComponentType(ComponentType::Cooling), coilCoolingDXSingleSpeedThermalStorage.componentType());
  testFuelTypeEquality({FuelType::Electricity}, coilCoolingDXSingleSpeedThermalStorage.coolingFuelTypes());
  testFuelTypeEquality({}, coilCoolingDXSingleSpeedThermalStorage.heatingFuelTypes());
  testAppGFuelTypeEquality({}, coilCoolingDXSingleSpeedThermalStorage.appGHeatingFuelTypes());
}

TEST_F(ModelFixture, CoilCoolingDXSingleSpeedThermalStorage_containingHVACComponent) {
  {  // AirLoopHVACUnitarySystem
    Model model;

    CoilCoolingDXSingleSpeedThermalStorage dx(model);
    EXPECT_TRUE(dx.isRemovable());

    AirLoopHVACUnitarySystem unitary(model);
    EXPECT_TRUE(unitary.setCoolingCoil(dx));
    ASSERT_TRUE(unitary.coolingCoil());
    EXPECT_EQ(dx, unitary.coolingCoil().get());

    ASSERT_TRUE(dx.containingHVACComponent());
    EXPECT_EQ(unitary, dx.containingHVACComponent().get());

    // Shouldn't be able to remove DX since it's contained in a HVAC Component
    EXPECT_FALSE(dx.isRemovable());
    auto rmed = dx.remove();
    EXPECT_EQ(0u, rmed.size());
    ASSERT_TRUE(unitary.coolingCoil());
    EXPECT_EQ(dx, unitary.coolingCoil().get());
  }
}

TEST_F(ModelFixture, CoilCoolingDXSingleSpeedThermalStorage_addToNode) {
  Model m;

  CoilCoolingDXSingleSpeedThermalStorage dx(m);

  AirLoopHVAC airLoop(m);

  Node supplyOutletNode = airLoop.supplyOutletNode();
  EXPECT_FALSE(dx.airLoopHVAC());
  EXPECT_EQ((unsigned)2, airLoop.supplyComponents().size());
  EXPECT_TRUE(dx.addToNode(supplyOutletNode));
  EXPECT_EQ((unsigned)3, airLoop.supplyComponents().size());
  EXPECT_TRUE(dx.airLoopHVAC());

  Node demandNode = airLoop.zoneSplitter().lastOutletModelObject()->cast<Node>();
  EXPECT_FALSE(dx.addToNode(demandNode));
  // 5u: inlet splitter node mixer outlet.
  EXPECT_EQ((unsigned)5, airLoop.demandComponents().size());
  EXPECT_EQ((unsigned)3, airLoop.supplyComponents().size());
  EXPECT_TRUE(dx.airLoopHVAC());

  EXPECT_TRUE(dx.removeFromLoop());
  EXPECT_EQ((unsigned)2, airLoop.supplyComponents().size());
  EXPECT_FALSE(dx.airLoopHVAC());

  PlantLoop plantLoop(m);
  supplyOutletNode = plantLoop.supplyOutletNode();
  EXPECT_FALSE(dx.addToNode(supplyOutletNode));
  EXPECT_EQ((unsigned)5, plantLoop.supplyComponents().size());

  Node demandOutletNode = plantLoop.demandOutletNode();
  EXPECT_FALSE(dx.addToNode(demandOutletNode));
  EXPECT_EQ((unsigned)5, plantLoop.demandComponents().size());
}

TEST_F(ModelFixture, CoilCoolingDXSingleSpeedThermalStorage_clone) {
  Model model;

  EXPECT_EQ(0, model.getModelObjects<Curve>().size());
  EXPECT_EQ(0, model.getConcreteModelObjects<CurveBiquadratic>().size());
  EXPECT_EQ(0, model.getConcreteModelObjects<CurveQuadratic>().size());
  EXPECT_EQ(0, model.getConcreteModelObjects<CurveTriquadratic>().size());
  EXPECT_EQ(0, model.getConcreteModelObjects<CoilCoolingDXSingleSpeedThermalStorage>().size());

  CoilCoolingDXSingleSpeedThermalStorage dx(model);
  CurveBiquadratic curve1(model);
  EXPECT_TRUE(dx.setCoolingOnlyModeTotalEvaporatorCoolingCapacityFunctionofTemperatureCurve(curve1));
  CurveQuadratic curve2(model);
  EXPECT_TRUE(dx.setCoolingOnlyModeTotalEvaporatorCoolingCapacityFunctionofFlowFractionCurve(curve2));
  CurveTriquadratic curve3(model);
  EXPECT_TRUE(dx.setCoolingAndChargeModeTotalEvaporatorCoolingCapacityFunctionofTemperatureCurve(curve3));

  {
    EXPECT_EQ(3, model.getModelObjects<Curve>().size());
    EXPECT_EQ(1, model.getConcreteModelObjects<CurveBiquadratic>().size());
    EXPECT_EQ(1, model.getConcreteModelObjects<CurveQuadratic>().size());
    EXPECT_EQ(1, model.getConcreteModelObjects<CurveTriquadratic>().size());
    EXPECT_EQ(1, model.getConcreteModelObjects<CoilCoolingDXSingleSpeedThermalStorage>().size());

    EXPECT_TRUE(dx.setStorageType("Water"));
  }

  auto dxClone = dx.clone(model).cast<CoilCoolingDXSingleSpeedThermalStorage>();
  {
    EXPECT_EQ("Water", dxClone.storageType());

    EXPECT_EQ(3, model.getModelObjects<Curve>().size());
    EXPECT_EQ(1, model.getConcreteModelObjects<CurveBiquadratic>().size());
    EXPECT_EQ(1, model.getConcreteModelObjects<CurveQuadratic>().size());
    EXPECT_EQ(1, model.getConcreteModelObjects<CurveTriquadratic>().size());
    EXPECT_EQ(2, model.getConcreteModelObjects<CoilCoolingDXSingleSpeedThermalStorage>().size());
  }

  {
    Model model2;

    EXPECT_EQ(0, model2.getModelObjects<Curve>().size());
    EXPECT_EQ(0, model2.getConcreteModelObjects<CurveBiquadratic>().size());
    EXPECT_EQ(0, model2.getConcreteModelObjects<CurveQuadratic>().size());
    EXPECT_EQ(0, model2.getConcreteModelObjects<CurveTriquadratic>().size());
    EXPECT_EQ(0, model2.getConcreteModelObjects<CoilCoolingDXSingleSpeedThermalStorage>().size());

    auto dxClone2 = dx.clone(model2).cast<CoilCoolingDXSingleSpeedThermalStorage>();

    EXPECT_EQ("Water", dxClone2.storageType());

    EXPECT_EQ(3, model2.getModelObjects<Curve>().size());
    EXPECT_EQ(1, model2.getConcreteModelObjects<CurveBiquadratic>().size());
    EXPECT_EQ(1, model2.getConcreteModelObjects<CurveQuadratic>().size());
    EXPECT_EQ(1, model2.getConcreteModelObjects<CurveTriquadratic>().size());
    EXPECT_EQ(1, model2.getConcreteModelObjects<CoilCoolingDXSingleSpeedThermalStorage>().size());
  }

  {
    dx.remove();
    EXPECT_EQ(3, model.getModelObjects<Curve>().size());
    EXPECT_EQ(1, model.getConcreteModelObjects<CurveBiquadratic>().size());
    EXPECT_EQ(1, model.getConcreteModelObjects<CurveQuadratic>().size());
    EXPECT_EQ(1, model.getConcreteModelObjects<CurveTriquadratic>().size());
    EXPECT_EQ(1, model.getConcreteModelObjects<CoilCoolingDXSingleSpeedThermalStorage>().size());

    dxClone.remove();
    EXPECT_EQ(0, model.getModelObjects<Curve>().size());
    EXPECT_EQ(0, model.getConcreteModelObjects<CurveBiquadratic>().size());
    EXPECT_EQ(0, model.getConcreteModelObjects<CurveQuadratic>().size());
    EXPECT_EQ(0, model.getConcreteModelObjects<CurveTriquadratic>().size());
    EXPECT_EQ(0, model.getConcreteModelObjects<CoilCoolingDXSingleSpeedThermalStorage>().size());
  }
}

TEST_F(ModelFixture, CoilCoolingDXSingleSpeedThermalStorage_cloneParent) {
  Model model;

  CoilCoolingDXSingleSpeedThermalStorage dx(model);
  CurveBiquadratic curve1(model);
  EXPECT_TRUE(dx.setCoolingOnlyModeTotalEvaporatorCoolingCapacityFunctionofTemperatureCurve(curve1));

  AirLoopHVACUnitarySystem unitary(model);
  EXPECT_TRUE(unitary.setCoolingCoil(dx));

  ASSERT_TRUE(dx.containingHVACComponent());
  EXPECT_EQ(unitary, dx.containingHVACComponent().get());

  ASSERT_TRUE(unitary.coolingCoil());
  EXPECT_EQ(dx, unitary.coolingCoil().get());

  EXPECT_EQ(1u, model.getConcreteModelObjects<AirLoopHVACUnitarySystem>().size());
  EXPECT_EQ(1u, model.getConcreteModelObjects<CoilCoolingDXSingleSpeedThermalStorage>().size());
  EXPECT_EQ(1u, model.getConcreteModelObjects<CurveBiquadratic>().size());

  // Cloning the unitary should clone the coolingCoil, but the clone of the DX should still share resources
  auto unitaryClone = unitary.clone(model).cast<AirLoopHVACUnitarySystem>();
  EXPECT_EQ(2u, model.getConcreteModelObjects<AirLoopHVACUnitarySystem>().size());
  EXPECT_EQ(2u, model.getConcreteModelObjects<CoilCoolingDXSingleSpeedThermalStorage>().size());
  EXPECT_EQ(1u, model.getConcreteModelObjects<CurveBiquadratic>().size());
  EXPECT_EQ(dx, unitary.coolingCoil().get());
  EXPECT_NE(dx, unitaryClone.coolingCoil().get());
}
