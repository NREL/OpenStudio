/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>
#include "ModelFixture.hpp"
#include "../HeatExchangerAirToAirSensibleAndLatent.hpp"
#include "../HeatExchangerAirToAirSensibleAndLatent_Impl.hpp"
#include "../AirLoopHVAC.hpp"
#include "../AirLoopHVAC_Impl.hpp"
#include "../Schedule.hpp"
#include "../Schedule_Impl.hpp"
#include "../Node.hpp"
#include "../Node_Impl.hpp"
#include "../AirLoopHVACOutdoorAirSystem.hpp"
#include "../AirLoopHVACOutdoorAirSystem_Impl.hpp"
#include "../EvaporativeCoolerDirectResearchSpecial.hpp"
#include "../EvaporativeCoolerDirectResearchSpecial_Impl.hpp"
#include "../HVACTemplates.hpp"
#include "../ScheduleCompact.hpp"
#include "../Curve.hpp"
#include "../TableLookup.hpp"
#include "../TableLookup_Impl.hpp"
#include "../TableIndependentVariable.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, HeatExchangerAirToAirSensibleAndLatent_Ctor) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  ASSERT_EXIT(
    {
      Model m;
      HeatExchangerAirToAirSensibleAndLatent hx(m);

      exit(0);
    },
    ::testing::ExitedWithCode(0), "");
}

TEST_F(ModelFixture, HeatExchangerAirToAirSensibleAndLatent_GettersSetters) {
  Model m;
  HeatExchangerAirToAirSensibleAndLatent hx(m);

  // Availability Schedule:  Object
  // Default in Ctor is m.alwaysOnDiscreteSchedule
  EXPECT_EQ(m.alwaysOnDiscreteSchedule(), hx.availabilitySchedule());
  ScheduleCompact sch(m);
  EXPECT_TRUE(hx.setAvailabilitySchedule(sch));
  EXPECT_EQ(sch, hx.availabilitySchedule());

  // Nominal Supply Air Flow Rate: Optional Double
  // No Default
  EXPECT_TRUE(hx.setNominalSupplyAirFlowRate(1.0));
  ASSERT_TRUE(hx.nominalSupplyAirFlowRate());
  EXPECT_EQ(1.0, hx.nominalSupplyAirFlowRate().get());

  // Sensible Effectiveness at 100% Heating Air Flow:  Double
  // No Default
  EXPECT_TRUE(hx.setSensibleEffectivenessat100HeatingAirFlow(0.5));
  EXPECT_EQ(0.5, hx.sensibleEffectivenessat100HeatingAirFlow());

  // Latent Effectiveness at 100% Heating Air Flow:  Double
  // No Default
  EXPECT_TRUE(hx.setLatentEffectivenessat100HeatingAirFlow(0.55));
  EXPECT_EQ(0.55, hx.latentEffectivenessat100HeatingAirFlow());

  // Sensible Effectiveness at 100% Cooling Air Flow:  Double
  // No Default
  EXPECT_TRUE(hx.setSensibleEffectivenessat100CoolingAirFlow(0.7));
  EXPECT_EQ(0.7, hx.sensibleEffectivenessat100CoolingAirFlow());

  // Latent Effectiveness at 100% Cooling Air Flow:  Double
  // No Default
  EXPECT_TRUE(hx.setLatentEffectivenessat100CoolingAirFlow(0.75));
  EXPECT_EQ(0.75, hx.latentEffectivenessat100CoolingAirFlow());

  // Supply Air Inlet Node: Optional Object

  // Supply Air Outlet Node: Optional Object

  // Exhaust Air Inlet Node: Optional Object

  // Exhaust Air Outlet Node: Optional Object

  // Nominal Electric Power:  Double
  // No Default
  EXPECT_TRUE(hx.setNominalElectricPower(1.0));
  EXPECT_EQ(1.0, hx.nominalElectricPower());

  // Supply Air Outlet Temperature Control:  Boolean
  // No Default
  EXPECT_TRUE(hx.setSupplyAirOutletTemperatureControl(true));
  EXPECT_EQ(true, hx.supplyAirOutletTemperatureControl());
  EXPECT_TRUE(hx.setSupplyAirOutletTemperatureControl(false));
  EXPECT_EQ(false, hx.supplyAirOutletTemperatureControl());

  // Heat Exchanger Type:  String
  // No Default
  // Test a valid choice
  EXPECT_TRUE(hx.setHeatExchangerType("Plate"));
  EXPECT_EQ("Plate", hx.heatExchangerType());
  // Test an invalid choice
  EXPECT_FALSE(hx.setHeatExchangerType("BadChoice"));
  EXPECT_EQ("Plate", hx.heatExchangerType());

  // Frost Control Type:  String
  // No Default
  // Test a valid choice
  EXPECT_TRUE(hx.setFrostControlType("None"));
  EXPECT_EQ("None", hx.frostControlType());
  // Test an invalid choice
  EXPECT_FALSE(hx.setFrostControlType("BadChoice"));
  EXPECT_EQ("None", hx.frostControlType());

  // Threshold Temperature:  Double
  // Check Idd default: 1.7
  EXPECT_EQ(1.7, hx.thresholdTemperature());
  EXPECT_TRUE(hx.setThresholdTemperature(0.7));
  EXPECT_EQ(0.7, hx.thresholdTemperature());
  hx.resetThresholdTemperature();
  EXPECT_EQ(1.7, hx.thresholdTemperature());

  // Initial Defrost Time Fraction: Optional Double
  // No Default
  EXPECT_TRUE(hx.setInitialDefrostTimeFraction(0.5));
  ASSERT_TRUE(hx.initialDefrostTimeFraction());
  EXPECT_EQ(0.5, hx.initialDefrostTimeFraction().get());
  hx.resetInitialDefrostTimeFraction();
  EXPECT_FALSE(hx.initialDefrostTimeFraction());

  // Rate of Defrost Time Fraction Increase: Optional Double
  // No Default
  EXPECT_TRUE(hx.setRateofDefrostTimeFractionIncrease(1.0));
  ASSERT_TRUE(hx.rateofDefrostTimeFractionIncrease());
  EXPECT_EQ(1.0, hx.rateofDefrostTimeFractionIncrease().get());
  hx.resetRateofDefrostTimeFractionIncrease();
  EXPECT_FALSE(hx.rateofDefrostTimeFractionIncrease());

  // Economizer Lockout:  Boolean
  // No Default
  EXPECT_TRUE(hx.setEconomizerLockout(true));
  EXPECT_EQ(true, hx.economizerLockout());
  EXPECT_TRUE(hx.setEconomizerLockout(false));
  EXPECT_EQ(false, hx.economizerLockout());
}

TEST_F(ModelFixture, HeatExchangerAirToAirSensibleAndLatent_addToNode) {
  Model model;
  HeatExchangerAirToAirSensibleAndLatent heatExchangerAirToAirSensibleAndLatent(model);

  auto loop = addSystemType3(model).cast<AirLoopHVAC>();

  AirLoopHVACOutdoorAirSystem oaSystem = loop.airLoopHVACOutdoorAirSystem().get();

  Node oaNode = oaSystem.outboardOANode().get();
  Node reliefNode = oaSystem.outboardReliefNode().get();

  HeatExchangerAirToAirSensibleAndLatent hx1(model);
  EXPECT_TRUE(hx1.addToNode(oaNode));

  HeatExchangerAirToAirSensibleAndLatent hx2(model);
  EXPECT_TRUE(hx2.addToNode(oaNode));

  HeatExchangerAirToAirSensibleAndLatent hx3(model);
  EXPECT_TRUE(hx3.addToNode(reliefNode));

  oaNode = oaSystem.outdoorAirModelObject()->cast<Node>();
  reliefNode = oaSystem.reliefAirModelObject()->cast<Node>();

  HeatExchangerAirToAirSensibleAndLatent hx4(model);
  EXPECT_TRUE(hx4.addToNode(reliefNode));

  HeatExchangerAirToAirSensibleAndLatent hx5(model);
  EXPECT_TRUE(hx5.addToNode(oaNode));

  Schedule schedule = model.alwaysOnDiscreteSchedule();
  EvaporativeCoolerDirectResearchSpecial evap(model, schedule);
  EXPECT_TRUE(evap.addToNode(oaNode));
  Node inletNode = evap.inletModelObject()->cast<Node>();
  Node outletNode = evap.outletModelObject()->cast<Node>();

  HeatExchangerAirToAirSensibleAndLatent hx6(model);
  EXPECT_TRUE(hx6.addToNode(inletNode));

  HeatExchangerAirToAirSensibleAndLatent hx7(model);
  EXPECT_TRUE(hx7.addToNode(outletNode));

  hx1.remove();
  hx2.remove();
  hx3.remove();
  hx4.remove();
  hx5.remove();
  hx6.remove();
  hx7.remove();
  evap.remove();

  EXPECT_EQ(1u, oaSystem.oaComponents().size());
  EXPECT_EQ(1u, oaSystem.reliefComponents().size());
}

TEST_F(ModelFixture, HeatExchangerAirToAirSensibleAndLatent_remove) {
  Model model;
  HeatExchangerAirToAirSensibleAndLatent heatExchangerAirToAirSensibleAndLatent(model);

  auto loop = addSystemType3(model).cast<AirLoopHVAC>();

  AirLoopHVACOutdoorAirSystem oaSystem = loop.airLoopHVACOutdoorAirSystem().get();

  Node oaNode = oaSystem.outboardOANode().get();
  Node reliefNode = oaSystem.outboardReliefNode().get();

  HeatExchangerAirToAirSensibleAndLatent hx1(model);
  EXPECT_TRUE(hx1.addToNode(oaNode));

  HeatExchangerAirToAirSensibleAndLatent hx2(model);
  EXPECT_TRUE(hx2.addToNode(oaNode));

  HeatExchangerAirToAirSensibleAndLatent hx3(model);
  EXPECT_TRUE(hx3.addToNode(reliefNode));

  oaNode = oaSystem.outdoorAirModelObject()->cast<Node>();
  reliefNode = oaSystem.reliefAirModelObject()->cast<Node>();

  HeatExchangerAirToAirSensibleAndLatent hx4(model);
  EXPECT_TRUE(hx4.addToNode(reliefNode));

  HeatExchangerAirToAirSensibleAndLatent hx5(model);
  EXPECT_TRUE(hx5.addToNode(oaNode));

  Schedule schedule = model.alwaysOnDiscreteSchedule();
  EvaporativeCoolerDirectResearchSpecial evap(model, schedule);
  EXPECT_TRUE(evap.addToNode(oaNode));
  Node inletNode = evap.inletModelObject()->cast<Node>();
  Node outletNode = evap.outletModelObject()->cast<Node>();

  HeatExchangerAirToAirSensibleAndLatent hx6(model);
  EXPECT_TRUE(hx6.addToNode(inletNode));

  HeatExchangerAirToAirSensibleAndLatent hx7(model);
  EXPECT_TRUE(hx7.addToNode(outletNode));

  EXPECT_TRUE(!loop.remove().empty());
}

TEST_F(ModelFixture, HeatExchangerAirToAirSensibleAndLatent_EffectivenessCurves) {
  Model m;
  HeatExchangerAirToAirSensibleAndLatent hx(m);

  {
    hx.resetSensibleEffectivenessofHeatingAirFlowCurve();
    EXPECT_FALSE(hx.sensibleEffectivenessofHeatingAirFlowCurve());

    TableLookup sensibleHeatingEff(m);
    EXPECT_TRUE(hx.setSensibleEffectivenessofHeatingAirFlowCurve(sensibleHeatingEff));
    ASSERT_TRUE(hx.sensibleEffectivenessofHeatingAirFlowCurve());
    EXPECT_EQ(sensibleHeatingEff, hx.sensibleEffectivenessofHeatingAirFlowCurve().get());

    hx.resetSensibleEffectivenessofHeatingAirFlowCurve();
    EXPECT_FALSE(hx.sensibleEffectivenessofHeatingAirFlowCurve());
  }

  {
    hx.resetLatentEffectivenessofHeatingAirFlowCurve();
    EXPECT_FALSE(hx.latentEffectivenessofHeatingAirFlowCurve());

    TableLookup latentHeatingEff(m);
    EXPECT_TRUE(hx.setLatentEffectivenessofHeatingAirFlowCurve(latentHeatingEff));
    ASSERT_TRUE(hx.latentEffectivenessofHeatingAirFlowCurve());
    EXPECT_EQ(latentHeatingEff, hx.latentEffectivenessofHeatingAirFlowCurve().get());

    hx.resetLatentEffectivenessofHeatingAirFlowCurve();
    EXPECT_FALSE(hx.latentEffectivenessofHeatingAirFlowCurve());
  }

  {
    hx.resetSensibleEffectivenessofCoolingAirFlowCurve();
    EXPECT_FALSE(hx.sensibleEffectivenessofCoolingAirFlowCurve());

    TableLookup sensibleCoolingEff(m);
    EXPECT_TRUE(hx.setSensibleEffectivenessofCoolingAirFlowCurve(sensibleCoolingEff));
    ASSERT_TRUE(hx.sensibleEffectivenessofCoolingAirFlowCurve());
    EXPECT_EQ(sensibleCoolingEff, hx.sensibleEffectivenessofCoolingAirFlowCurve().get());

    hx.resetSensibleEffectivenessofCoolingAirFlowCurve();
    EXPECT_FALSE(hx.sensibleEffectivenessofCoolingAirFlowCurve());
  }

  {
    hx.resetLatentEffectivenessofCoolingAirFlowCurve();
    EXPECT_FALSE(hx.latentEffectivenessofCoolingAirFlowCurve());

    TableLookup latentCoolingEff(m);
    EXPECT_TRUE(hx.setLatentEffectivenessofCoolingAirFlowCurve(latentCoolingEff));
    ASSERT_TRUE(hx.latentEffectivenessofCoolingAirFlowCurve());
    EXPECT_EQ(latentCoolingEff, hx.latentEffectivenessofCoolingAirFlowCurve().get());

    hx.resetLatentEffectivenessofCoolingAirFlowCurve();
    EXPECT_FALSE(hx.latentEffectivenessofCoolingAirFlowCurve());
  }
}

TEST_F(ModelFixture, HeatExchangerAirToAirSensibleAndLatent_Deprecated75Effectiveness) {
  Model m;
  HeatExchangerAirToAirSensibleAndLatent hx(m);

  // Ensure no curves assigned for now
  hx.resetSensibleEffectivenessofHeatingAirFlowCurve();
  hx.resetLatentEffectivenessofHeatingAirFlowCurve();
  hx.resetSensibleEffectivenessofCoolingAirFlowCurve();
  hx.resetLatentEffectivenessofCoolingAirFlowCurve();

  constexpr double sensibleHeatingEff100 = 0.76;
  constexpr double latentHeatingEff100 = 0.68;
  constexpr double sensibleCoolingEff100 = 0.72;
  constexpr double latentCoolingEff100 = 0.63;
  constexpr double sensibleHeatingEff75 = sensibleHeatingEff100 * 1.05;
  constexpr double latentHeatingEff75 = latentHeatingEff100 * 1.05;
  constexpr double sensibleCoolingEff75 = sensibleCoolingEff100 * 1.05;
  constexpr double latentCoolingEff75 = latentCoolingEff100 * 1.05;

  EXPECT_TRUE(hx.setSensibleEffectivenessat100HeatingAirFlow(sensibleHeatingEff100));
  EXPECT_TRUE(hx.setLatentEffectivenessat100HeatingAirFlow(latentHeatingEff100));
  EXPECT_TRUE(hx.setSensibleEffectivenessat100CoolingAirFlow(sensibleCoolingEff100));
  EXPECT_TRUE(hx.setLatentEffectivenessat100CoolingAirFlow(latentCoolingEff100));

  // When no curves assigned, the effectiveness is constant
  EXPECT_EQ(sensibleHeatingEff100, hx.sensibleEffectivenessat75HeatingAirFlow());
  EXPECT_EQ(latentHeatingEff100, hx.latentEffectivenessat75HeatingAirFlow());
  EXPECT_EQ(sensibleCoolingEff100, hx.sensibleEffectivenessat75CoolingAirFlow());
  EXPECT_EQ(latentCoolingEff100, hx.latentEffectivenessat75CoolingAirFlow());

  const std::vector<double> expectedIndVals{0.75, 1.0};

  // Sensible Effectiveness at 75% Heating Air Flow: Double
  {
    EXPECT_FALSE(hx.sensibleEffectivenessofHeatingAirFlowCurve());
    EXPECT_TRUE(hx.setSensibleEffectivenessat75HeatingAirFlow(sensibleHeatingEff75));
    ASSERT_TRUE(hx.sensibleEffectivenessofHeatingAirFlowCurve());
    ASSERT_TRUE(hx.sensibleEffectivenessofHeatingAirFlowCurve()->optionalCast<TableLookup>());
    auto tableLookup = hx.sensibleEffectivenessofHeatingAirFlowCurve()->cast<TableLookup>();
    EXPECT_EQ(1, tableLookup.numVariables());
    ASSERT_EQ(expectedIndVals, tableLookup.independentVariables().front().values());
    EXPECT_DOUBLE_EQ(sensibleHeatingEff75, hx.sensibleEffectivenessat75HeatingAirFlow());

    // Works to modify when an existing table lookup is in the expected format
    auto val2 = sensibleHeatingEff75 + 0.025;
    EXPECT_TRUE(hx.setSensibleEffectivenessat75HeatingAirFlow(val2));
    EXPECT_DOUBLE_EQ(val2, hx.sensibleEffectivenessat75HeatingAirFlow());
  }

  // Latent Effectiveness at 75% Heating Air Flow: Double
  {
    EXPECT_FALSE(hx.latentEffectivenessofHeatingAirFlowCurve());
    EXPECT_TRUE(hx.setLatentEffectivenessat75HeatingAirFlow(latentHeatingEff75));
    ASSERT_TRUE(hx.latentEffectivenessofHeatingAirFlowCurve());
    ASSERT_TRUE(hx.latentEffectivenessofHeatingAirFlowCurve()->optionalCast<TableLookup>());
    auto tableLookup = hx.latentEffectivenessofHeatingAirFlowCurve()->cast<TableLookup>();
    EXPECT_EQ(1, tableLookup.numVariables());
    ASSERT_EQ(expectedIndVals, tableLookup.independentVariables().front().values());
    EXPECT_DOUBLE_EQ(latentHeatingEff75, hx.latentEffectivenessat75HeatingAirFlow());

    // Works to modify when an existing table lookup is in the expected format
    auto val2 = latentHeatingEff75 + 0.025;
    EXPECT_TRUE(hx.setLatentEffectivenessat75HeatingAirFlow(val2));
    EXPECT_DOUBLE_EQ(val2, hx.latentEffectivenessat75HeatingAirFlow());
  }

  // Sensible Effectiveness at 75% Cooling Air Flow: Double
  {
    EXPECT_FALSE(hx.sensibleEffectivenessofCoolingAirFlowCurve());
    EXPECT_TRUE(hx.setSensibleEffectivenessat75CoolingAirFlow(sensibleCoolingEff75));
    ASSERT_TRUE(hx.sensibleEffectivenessofCoolingAirFlowCurve());
    ASSERT_TRUE(hx.sensibleEffectivenessofCoolingAirFlowCurve()->optionalCast<TableLookup>());
    auto tableLookup = hx.sensibleEffectivenessofCoolingAirFlowCurve()->cast<TableLookup>();
    EXPECT_EQ(1, tableLookup.numVariables());
    ASSERT_EQ(expectedIndVals, tableLookup.independentVariables().front().values());
    EXPECT_DOUBLE_EQ(sensibleCoolingEff75, hx.sensibleEffectivenessat75CoolingAirFlow());

    // Works to modify when an existing table lookup is in the expected format
    auto val2 = sensibleCoolingEff75 + 0.025;
    EXPECT_TRUE(hx.setSensibleEffectivenessat75CoolingAirFlow(val2));
    EXPECT_DOUBLE_EQ(val2, hx.sensibleEffectivenessat75CoolingAirFlow());
  }

  // Latent Effectiveness at 75% Cooling Air Flow: Double
  {
    EXPECT_FALSE(hx.latentEffectivenessofCoolingAirFlowCurve());
    EXPECT_TRUE(hx.setLatentEffectivenessat75CoolingAirFlow(latentCoolingEff75));
    ASSERT_TRUE(hx.latentEffectivenessofCoolingAirFlowCurve());
    ASSERT_TRUE(hx.latentEffectivenessofCoolingAirFlowCurve()->optionalCast<TableLookup>());
    auto tableLookup = hx.latentEffectivenessofCoolingAirFlowCurve()->cast<TableLookup>();
    EXPECT_EQ(1, tableLookup.numVariables());
    ASSERT_EQ(expectedIndVals, tableLookup.independentVariables().front().values());
    EXPECT_DOUBLE_EQ(latentCoolingEff75, hx.latentEffectivenessat75CoolingAirFlow());

    // Works to modify when an existing table lookup is in the expected format
    auto val2 = latentCoolingEff75 + 0.025;
    EXPECT_TRUE(hx.setLatentEffectivenessat75CoolingAirFlow(val2));
    EXPECT_DOUBLE_EQ(val2, hx.latentEffectivenessat75CoolingAirFlow());
  }

  // Ensure we protect users against dumb issues that lead to a normalization divisor of zero
  {
    // Ensure no curves assigned for now
    hx.resetSensibleEffectivenessofHeatingAirFlowCurve();
    hx.resetLatentEffectivenessofHeatingAirFlowCurve();
    hx.resetSensibleEffectivenessofCoolingAirFlowCurve();
    hx.resetLatentEffectivenessofCoolingAirFlowCurve();

    // IMHO this doesn't make sense (IDD should be `\minimum> 0.0`), but the E+ IDD and OS IDD have always allowed it, so leaving it untouched
    EXPECT_TRUE(hx.setSensibleEffectivenessat100HeatingAirFlow(0.0));
    EXPECT_TRUE(hx.setLatentEffectivenessat100HeatingAirFlow(0.0));
    EXPECT_TRUE(hx.setSensibleEffectivenessat100CoolingAirFlow(0.0));
    EXPECT_TRUE(hx.setLatentEffectivenessat100CoolingAirFlow(0.0));

    EXPECT_ANY_THROW(hx.setSensibleEffectivenessat75HeatingAirFlow(0.5));
    EXPECT_ANY_THROW(hx.setLatentEffectivenessat75HeatingAirFlow(0.5));
    EXPECT_ANY_THROW(hx.setSensibleEffectivenessat75CoolingAirFlow(0.5));
    EXPECT_ANY_THROW(hx.setLatentEffectivenessat75CoolingAirFlow(0.5));
  }
}

TEST_F(ModelFixture, HeatExchangerAirToAirSensibleAndLatent_assignHistoricalEffectivenessCurves) {

  Model m;
  HeatExchangerAirToAirSensibleAndLatent hx(m);

  // Ensure no curves assigned for now
  hx.resetSensibleEffectivenessofHeatingAirFlowCurve();
  hx.resetLatentEffectivenessofHeatingAirFlowCurve();
  hx.resetSensibleEffectivenessofCoolingAirFlowCurve();
  hx.resetLatentEffectivenessofCoolingAirFlowCurve();

  EXPECT_TRUE(hx.assignHistoricalEffectivenessCurves());

  constexpr double sensibleHeatingEff75 = 0.81;
  constexpr double latentHeatingEff75 = 0.73;
  constexpr double sensibleCoolingEff75 = 0.81;
  constexpr double latentCoolingEff75 = 0.73;

  const std::vector<double> expectedIndVals{0.75, 1.0};

  // Sensible Effectiveness at 75% Heating Air Flow: Double
  {
    ASSERT_TRUE(hx.sensibleEffectivenessofHeatingAirFlowCurve());
    ASSERT_TRUE(hx.sensibleEffectivenessofHeatingAirFlowCurve()->optionalCast<TableLookup>());
    auto tableLookup = hx.sensibleEffectivenessofHeatingAirFlowCurve()->cast<TableLookup>();
    EXPECT_EQ(1, tableLookup.numVariables());
    ASSERT_EQ(expectedIndVals, tableLookup.independentVariables().front().values());
    auto val100 = hx.sensibleEffectivenessat100HeatingAirFlow();
    EXPECT_EQ("DivisorOnly", tableLookup.normalizationMethod());
    EXPECT_DOUBLE_EQ(val100, tableLookup.normalizationDivisor());
    const std::vector<double> expectedOutVals = {sensibleHeatingEff75, val100};
    ASSERT_EQ(expectedOutVals, tableLookup.outputValues());

    EXPECT_DOUBLE_EQ(sensibleHeatingEff75, hx.sensibleEffectivenessat75HeatingAirFlow());  // DEPRECATED
  }

  // Latent Effectiveness at 75% Heating Air Flow: Double
  {
    ASSERT_TRUE(hx.latentEffectivenessofHeatingAirFlowCurve());
    ASSERT_TRUE(hx.latentEffectivenessofHeatingAirFlowCurve()->optionalCast<TableLookup>());
    auto tableLookup = hx.latentEffectivenessofHeatingAirFlowCurve()->cast<TableLookup>();
    EXPECT_EQ(1, tableLookup.numVariables());
    ASSERT_EQ(expectedIndVals, tableLookup.independentVariables().front().values());
    auto val100 = hx.latentEffectivenessat100HeatingAirFlow();
    EXPECT_EQ("DivisorOnly", tableLookup.normalizationMethod());
    EXPECT_DOUBLE_EQ(val100, tableLookup.normalizationDivisor());
    const std::vector<double> expectedOutVals = {latentHeatingEff75, val100};
    ASSERT_EQ(expectedOutVals, tableLookup.outputValues());

    EXPECT_DOUBLE_EQ(latentHeatingEff75, hx.latentEffectivenessat75HeatingAirFlow());  // DEPRECATED
  }

  // Sensible Effectiveness at 75% Cooling Air Flow: Double
  {
    ASSERT_TRUE(hx.sensibleEffectivenessofCoolingAirFlowCurve());
    ASSERT_TRUE(hx.sensibleEffectivenessofCoolingAirFlowCurve()->optionalCast<TableLookup>());
    auto tableLookup = hx.sensibleEffectivenessofCoolingAirFlowCurve()->cast<TableLookup>();
    EXPECT_EQ(1, tableLookup.numVariables());
    ASSERT_EQ(expectedIndVals, tableLookup.independentVariables().front().values());
    auto val100 = hx.sensibleEffectivenessat100CoolingAirFlow();
    EXPECT_EQ("DivisorOnly", tableLookup.normalizationMethod());
    EXPECT_DOUBLE_EQ(val100, tableLookup.normalizationDivisor());
    const std::vector<double> expectedOutVals = {sensibleCoolingEff75, val100};
    ASSERT_EQ(expectedOutVals, tableLookup.outputValues());

    EXPECT_DOUBLE_EQ(sensibleCoolingEff75, hx.sensibleEffectivenessat75CoolingAirFlow());  // DEPRECATED
  }

  // Latent Effectiveness at 75% Cooling Air Flow: Double
  {
    ASSERT_TRUE(hx.latentEffectivenessofCoolingAirFlowCurve());
    ASSERT_TRUE(hx.latentEffectivenessofCoolingAirFlowCurve()->optionalCast<TableLookup>());
    auto tableLookup = hx.latentEffectivenessofCoolingAirFlowCurve()->cast<TableLookup>();
    EXPECT_EQ(1, tableLookup.numVariables());
    ASSERT_EQ(expectedIndVals, tableLookup.independentVariables().front().values());
    auto val100 = hx.latentEffectivenessat100CoolingAirFlow();
    EXPECT_EQ("DivisorOnly", tableLookup.normalizationMethod());
    EXPECT_DOUBLE_EQ(val100, tableLookup.normalizationDivisor());
    const std::vector<double> expectedOutVals = {latentCoolingEff75, val100};
    ASSERT_EQ(expectedOutVals, tableLookup.outputValues());

    EXPECT_DOUBLE_EQ(latentCoolingEff75, hx.latentEffectivenessat75CoolingAirFlow());  // DEPRECATED
  }
}
