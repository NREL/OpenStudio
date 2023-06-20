/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>
#include <string>
#include "ModelFixture.hpp"

#include "../CoilCoolingDXCurveFitSpeed.hpp"
#include "../CoilCoolingDXCurveFitSpeed_Impl.hpp"

#include "../CoilCoolingDXCurveFitOperatingMode.hpp"
#include "../CoilCoolingDXCurveFitOperatingMode_Impl.hpp"
#include "../CurveBiquadratic.hpp"
#include "../CurveBiquadratic_Impl.hpp"
#include "../CurveQuadratic.hpp"
#include "../CurveQuadratic_Impl.hpp"

#include <algorithm>

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, CoilCoolingDXCurveFitSpeed_CoilCoolingDXCurveFitSpeed) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  ASSERT_EXIT(
    {
      // create a model to use
      Model model;

      // create a coil cooling dx curve fit speed object to use
      CoilCoolingDXCurveFitSpeed speed(model);

      exit(0);
    },
    ::testing::ExitedWithCode(0), "");

  // create a model to use
  Model model;

  // create a coil cooling dx curve fit speed object to use
  CoilCoolingDXCurveFitSpeed speed(model);

  EXPECT_EQ(1.0, speed.grossTotalCoolingCapacityFraction());
  EXPECT_EQ(1.0, speed.evaporatorAirFlowRateFraction());
  EXPECT_EQ(1.0, speed.condenserAirFlowRateFraction());
  ASSERT_FALSE(speed.grossSensibleHeatRatio());
  EXPECT_TRUE(speed.isGrossSensibleHeatRatioAutosized());
  EXPECT_EQ(3.0, speed.grossCoolingCOP());
  EXPECT_EQ(1.0, speed.activeFractionofCoilFaceArea());
  EXPECT_EQ(773.3, speed.ratedEvaporatorFanPowerPerVolumeFlowRate());
  EXPECT_EQ(1.0, speed.evaporativeCondenserPumpPowerFraction());
  EXPECT_EQ(0.9, speed.evaporativeCondenserEffectiveness());
  EXPECT_FALSE(speed.totalCoolingCapacityModifierFunctionofTemperatureCurve());
  EXPECT_FALSE(speed.totalCoolingCapacityModifierFunctionofAirFlowFractionCurve());
  EXPECT_FALSE(speed.energyInputRatioModifierFunctionofTemperatureCurve());
  EXPECT_FALSE(speed.energyInputRatioModifierFunctionofAirFlowFractionCurve());
  EXPECT_FALSE(speed.partLoadFractionCorrelationCurve());
  EXPECT_FALSE(speed.wasteHeatModifierFunctionofTemperatureCurve());
  ASSERT_FALSE(speed.sensibleHeatRatioModifierFunctionofTemperatureCurve());
  ASSERT_FALSE(speed.sensibleHeatRatioModifierFunctionofFlowFractionCurve());
  EXPECT_EQ(0, speed.coilCoolingDXCurveFitOperatingModes().size());
}

TEST_F(ModelFixture, CoilCoolingDXCurveFitSpeed_GettersSetters) {
  // create a model to use
  Model model;

  // create a coil cooling dx curve fit speed object to use
  CoilCoolingDXCurveFitSpeed speed(model);

  EXPECT_EQ(1.0, speed.grossTotalCoolingCapacityFraction());
  EXPECT_EQ(1.0, speed.evaporatorAirFlowRateFraction());
  EXPECT_EQ(1.0, speed.condenserAirFlowRateFraction());
  ASSERT_FALSE(speed.grossSensibleHeatRatio());
  EXPECT_TRUE(speed.isGrossSensibleHeatRatioAutosized());
  EXPECT_EQ(3.0, speed.grossCoolingCOP());
  EXPECT_EQ(1.0, speed.activeFractionofCoilFaceArea());
  EXPECT_EQ(773.3, speed.ratedEvaporatorFanPowerPerVolumeFlowRate());
  EXPECT_EQ(1.0, speed.evaporativeCondenserPumpPowerFraction());
  EXPECT_EQ(0.9, speed.evaporativeCondenserEffectiveness());
  EXPECT_FALSE(speed.totalCoolingCapacityModifierFunctionofTemperatureCurve());
  EXPECT_FALSE(speed.totalCoolingCapacityModifierFunctionofAirFlowFractionCurve());
  EXPECT_FALSE(speed.energyInputRatioModifierFunctionofTemperatureCurve());
  EXPECT_FALSE(speed.energyInputRatioModifierFunctionofAirFlowFractionCurve());
  EXPECT_FALSE(speed.partLoadFractionCorrelationCurve());
  EXPECT_FALSE(speed.wasteHeatModifierFunctionofTemperatureCurve());
  EXPECT_EQ(speed.ratedWasteHeatFractionofPowerInput(), 0.2);
  ASSERT_FALSE(speed.sensibleHeatRatioModifierFunctionofTemperatureCurve());
  ASSERT_FALSE(speed.sensibleHeatRatioModifierFunctionofFlowFractionCurve());
  EXPECT_EQ(speed.coilCoolingDXCurveFitOperatingModes().size(), 0u);

  speed.setGrossTotalCoolingCapacityFraction(2.0);
  speed.setEvaporatorAirFlowRateFraction(3.0);
  speed.setCondenserAirFlowRateFraction(4.0);
  speed.setGrossSensibleHeatRatio(0.5);
  speed.setGrossCoolingCOP(6.0);
  speed.setActiveFractionofCoilFaceArea(0.6);
  speed.setRatedEvaporatorFanPowerPerVolumeFlowRate(8.0);
  speed.setEvaporativeCondenserPumpPowerFraction(9.0);
  speed.setEvaporativeCondenserEffectiveness(0.7);
  CurveBiquadratic totalCoolingCapacityModifierFunctionofTemperatureCurve(model);
  speed.setTotalCoolingCapacityModifierFunctionofTemperatureCurve(totalCoolingCapacityModifierFunctionofTemperatureCurve);
  CurveQuadratic totalCoolingCapacityModifierFunctionofAirFlowFractionCurve(model);
  speed.setTotalCoolingCapacityModifierFunctionofAirFlowFractionCurve(totalCoolingCapacityModifierFunctionofAirFlowFractionCurve);
  CurveBiquadratic energyInputRatioModifierFunctionofTemperatureCurve(model);
  speed.setEnergyInputRatioModifierFunctionofTemperatureCurve(energyInputRatioModifierFunctionofTemperatureCurve);
  CurveQuadratic energyInputRatioModifierFunctionofAirFlowFractionCurve(model);
  speed.setEnergyInputRatioModifierFunctionofAirFlowFractionCurve(energyInputRatioModifierFunctionofAirFlowFractionCurve);
  CurveQuadratic partLoadFractionCorrelationCurve(model);
  speed.setPartLoadFractionCorrelationCurve(partLoadFractionCorrelationCurve);
  CurveBiquadratic wasteHeatModifierFunctionofTemperatureCurve(model);
  speed.setWasteHeatModifierFunctionofTemperatureCurve(wasteHeatModifierFunctionofTemperatureCurve);
  speed.setRatedWasteHeatFractionofPowerInput(0.8);
  CurveBiquadratic sensibleHeatRatioModifierFunctionofTemperatureCurve(model);
  speed.setSensibleHeatRatioModifierFunctionofTemperatureCurve(sensibleHeatRatioModifierFunctionofTemperatureCurve);
  CurveQuadratic sensibleHeatRatioModifierFunctionofFlowFractionCurve(model);
  speed.setSensibleHeatRatioModifierFunctionofFlowFractionCurve(sensibleHeatRatioModifierFunctionofFlowFractionCurve);

  EXPECT_EQ(2.0, speed.grossTotalCoolingCapacityFraction());
  EXPECT_EQ(3.0, speed.evaporatorAirFlowRateFraction());
  EXPECT_EQ(4.0, speed.condenserAirFlowRateFraction());
  ASSERT_TRUE(speed.grossSensibleHeatRatio());
  EXPECT_EQ(speed.grossSensibleHeatRatio().get(), 0.5);
  EXPECT_FALSE(speed.isGrossSensibleHeatRatioAutosized());
  EXPECT_EQ(6.0, speed.grossCoolingCOP());
  EXPECT_EQ(0.6, speed.activeFractionofCoilFaceArea());
  EXPECT_EQ(8.0, speed.ratedEvaporatorFanPowerPerVolumeFlowRate());
  EXPECT_EQ(9.0, speed.evaporativeCondenserPumpPowerFraction());
  EXPECT_EQ(0.7, speed.evaporativeCondenserEffectiveness());
  EXPECT_TRUE(speed.totalCoolingCapacityModifierFunctionofTemperatureCurve());
  EXPECT_TRUE(speed.totalCoolingCapacityModifierFunctionofAirFlowFractionCurve());
  EXPECT_TRUE(speed.energyInputRatioModifierFunctionofTemperatureCurve());
  EXPECT_TRUE(speed.energyInputRatioModifierFunctionofAirFlowFractionCurve());
  EXPECT_TRUE(speed.partLoadFractionCorrelationCurve());
  EXPECT_TRUE(speed.wasteHeatModifierFunctionofTemperatureCurve());
  EXPECT_EQ(speed.ratedWasteHeatFractionofPowerInput(), 0.8);
  ASSERT_TRUE(speed.sensibleHeatRatioModifierFunctionofTemperatureCurve());
  ASSERT_TRUE(speed.sensibleHeatRatioModifierFunctionofFlowFractionCurve());

  speed.autosizeGrossSensibleHeatRatio();
  speed.resetTotalCoolingCapacityModifierFunctionofTemperatureCurve();
  speed.resetTotalCoolingCapacityModifierFunctionofAirFlowFractionCurve();
  speed.resetEnergyInputRatioModifierFunctionofTemperatureCurve();
  speed.resetEnergyInputRatioModifierFunctionofAirFlowFractionCurve();
  speed.resetPartLoadFractionCorrelationCurve();
  speed.resetWasteHeatModifierFunctionofTemperatureCurve();
  speed.resetSensibleHeatRatioModifierFunctionofTemperatureCurve();
  speed.resetSensibleHeatRatioModifierFunctionofFlowFractionCurve();

  ASSERT_FALSE(speed.grossSensibleHeatRatio());
  EXPECT_TRUE(speed.isGrossSensibleHeatRatioAutosized());
  EXPECT_FALSE(speed.totalCoolingCapacityModifierFunctionofTemperatureCurve());
  EXPECT_FALSE(speed.totalCoolingCapacityModifierFunctionofAirFlowFractionCurve());
  EXPECT_FALSE(speed.energyInputRatioModifierFunctionofTemperatureCurve());
  EXPECT_FALSE(speed.energyInputRatioModifierFunctionofAirFlowFractionCurve());
  EXPECT_FALSE(speed.partLoadFractionCorrelationCurve());
  EXPECT_FALSE(speed.wasteHeatModifierFunctionofTemperatureCurve());
  ASSERT_FALSE(speed.sensibleHeatRatioModifierFunctionofTemperatureCurve());
  ASSERT_FALSE(speed.sensibleHeatRatioModifierFunctionofFlowFractionCurve());
}

TEST_F(ModelFixture, CoilCoolingDXCurveFitSpeed_coilCoolingDXCurveFitPerformances) {
  Model model;

  CoilCoolingDXCurveFitSpeed speed(model);
  CoilCoolingDXCurveFitOperatingMode operatingMode1(model);
  EXPECT_TRUE(operatingMode1.addSpeed(speed));

  EXPECT_EQ(1u, speed.directUseCount());
  auto coilCoolingDXCurveFitOperatingModes = speed.coilCoolingDXCurveFitOperatingModes();
  ASSERT_EQ(1u, coilCoolingDXCurveFitOperatingModes.size());
  EXPECT_EQ(operatingMode1, coilCoolingDXCurveFitOperatingModes[0]);

  CoilCoolingDXCurveFitOperatingMode operatingMode2(model);
  EXPECT_TRUE(operatingMode2.addSpeed(speed));

  EXPECT_EQ(2u, speed.directUseCount());
  coilCoolingDXCurveFitOperatingModes = speed.coilCoolingDXCurveFitOperatingModes();
  ASSERT_EQ(2u, coilCoolingDXCurveFitOperatingModes.size());
  EXPECT_TRUE(std::find(coilCoolingDXCurveFitOperatingModes.begin(), coilCoolingDXCurveFitOperatingModes.end(), operatingMode1)
              != coilCoolingDXCurveFitOperatingModes.end());
  EXPECT_TRUE(std::find(coilCoolingDXCurveFitOperatingModes.begin(), coilCoolingDXCurveFitOperatingModes.end(), operatingMode2)
              != coilCoolingDXCurveFitOperatingModes.end());
}

TEST_F(ModelFixture, CoilCoolingDXCurveFitSpeed_remove) {
  // If we remove a speed, we would like any CurveFitOperatingMode that use it to have their extensible groups adjusted
  Model model;

  CoilCoolingDXCurveFitSpeed speed1(model);
  CoilCoolingDXCurveFitSpeed speed2(model);

  CoilCoolingDXCurveFitOperatingMode operatingMode1(model);
  EXPECT_TRUE(operatingMode1.addSpeed(speed1));
  EXPECT_TRUE(operatingMode1.addSpeed(speed2));

  EXPECT_EQ(2u, operatingMode1.numExtensibleGroups());
  EXPECT_EQ(2u, operatingMode1.numberOfSpeeds());
  EXPECT_EQ(2u, operatingMode1.speeds().size());

  CoilCoolingDXCurveFitOperatingMode operatingMode2(model);
  EXPECT_TRUE(operatingMode2.addSpeed(speed1));

  EXPECT_EQ(1u, operatingMode2.numExtensibleGroups());
  EXPECT_EQ(1u, operatingMode2.numberOfSpeeds());
  EXPECT_EQ(1u, operatingMode2.speeds().size());

  EXPECT_EQ(2u, model.getConcreteModelObjects<CoilCoolingDXCurveFitSpeed>().size());

  speed1.remove();

  EXPECT_EQ(1u, model.getConcreteModelObjects<CoilCoolingDXCurveFitSpeed>().size());

  EXPECT_EQ(1u, operatingMode1.numExtensibleGroups());
  EXPECT_EQ(1u, operatingMode1.numberOfSpeeds());
  EXPECT_EQ(1u, operatingMode1.speeds().size());
  EXPECT_EQ(0u, operatingMode2.numExtensibleGroups());
  EXPECT_EQ(0u, operatingMode2.numberOfSpeeds());
  EXPECT_EQ(0u, operatingMode2.speeds().size());

  EXPECT_EQ(speed2, operatingMode1.speeds()[0]);
}

TEST_F(ModelFixture, CoilCoolingDXCurveFitSpeed_2017and2023) {
  Model model;

  CoilCoolingDXCurveFitSpeed speed(model);

  EXPECT_EQ(773.3, speed.ratedEvaporatorFanPowerPerVolumeFlowRate2017());
  EXPECT_EQ(934.4, speed.ratedEvaporatorFanPowerPerVolumeFlowRate2023());

  EXPECT_TRUE(speed.setRatedEvaporatorFanPowerPerVolumeFlowRate2017(8.0));
  EXPECT_TRUE(speed.setRatedEvaporatorFanPowerPerVolumeFlowRate2023(9.0));

  EXPECT_EQ(8.0, speed.ratedEvaporatorFanPowerPerVolumeFlowRate2017());
  EXPECT_EQ(9.0, speed.ratedEvaporatorFanPowerPerVolumeFlowRate2023());
}