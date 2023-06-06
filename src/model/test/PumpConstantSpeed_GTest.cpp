/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>
#include "ModelFixture.hpp"
#include "../PumpConstantSpeed.hpp"
#include "../PumpConstantSpeed_Impl.hpp"
#include "../AirLoopHVAC.hpp"
#include "../PlantLoop.hpp"
#include "../Node.hpp"
#include "../Node_Impl.hpp"
#include "../CurveLinear.hpp"
#include "../ScheduleCompact.hpp"
#include "../ThermalZone.hpp"
#include "../AirLoopHVACZoneSplitter.hpp"
#include "../../utilities/units/Unit.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, PumpConstantSpeed_PumpConstantSpeed) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  ASSERT_EXIT(
    {
      Model m;
      PumpConstantSpeed pump(m);

      exit(0);
    },
    ::testing::ExitedWithCode(0), "");
}

TEST_F(ModelFixture, PumpConstantSpeed_GettersSetters) {

  Model m;
  PumpConstantSpeed pump(m);

  // Inlet Node Name:  Object
  // Outlet Node Name:  Object

  // Rated Flow Rate: Optional Double
  // Defaults to autosize in Ctor
  EXPECT_TRUE(pump.isRatedFlowRateAutosized());
  EXPECT_TRUE(pump.setRatedFlowRate(1.0));
  ASSERT_TRUE(pump.ratedFlowRate());
  EXPECT_EQ(1.0, pump.ratedFlowRate().get());
  pump.resetRatedFlowRate();
  EXPECT_FALSE(pump.ratedFlowRate());
  // Test autosizing
  EXPECT_TRUE(pump.setRatedFlowRate(1.0));
  EXPECT_FALSE(pump.isRatedFlowRateAutosized());
  pump.autosizeRatedFlowRate();
  EXPECT_TRUE(pump.isRatedFlowRateAutosized());

  // Rated Pump Head:  Double
  // Check Idd default: 179352 (overriden in Ctor by same value)
  EXPECT_EQ(179352, pump.ratedPumpHead());
  EXPECT_TRUE(pump.setRatedPumpHead(179351.0));
  EXPECT_EQ(179351.0, pump.ratedPumpHead());
  pump.resetRatedPumpHead();
  EXPECT_EQ(179352, pump.ratedPumpHead());

  // Rated Power Consumption: Optional Double
  // Defaults to autosize in Ctor
  EXPECT_TRUE(pump.isRatedPowerConsumptionAutosized());
  EXPECT_TRUE(pump.setRatedPowerConsumption(10.03));
  ASSERT_TRUE(pump.ratedPowerConsumption());
  EXPECT_EQ(10.03, pump.ratedPowerConsumption().get());
  pump.resetRatedPowerConsumption();
  EXPECT_FALSE(pump.ratedPowerConsumption());
  // Test autosizing
  EXPECT_TRUE(pump.setRatedPowerConsumption(10.03));
  EXPECT_FALSE(pump.isRatedPowerConsumptionAutosized());
  pump.autosizeRatedPowerConsumption();
  EXPECT_TRUE(pump.isRatedPowerConsumptionAutosized());

  // Motor Efficiency:  Double
  // Check Idd default: 0.9 (overriden in Ctor by same value)
  EXPECT_EQ(0.9, pump.motorEfficiency());
  EXPECT_TRUE(pump.setMotorEfficiency(0.45));
  EXPECT_EQ(0.45, pump.motorEfficiency());
  pump.resetMotorEfficiency();
  EXPECT_EQ(0.9, pump.motorEfficiency());

  // Fraction of Motor Inefficiencies to Fluid Stream:  Double
  // Check Idd default: 0.0 (overriden in Ctor by same value)
  EXPECT_EQ(0.0, pump.fractionofMotorInefficienciestoFluidStream());
  EXPECT_TRUE(pump.setFractionofMotorInefficienciestoFluidStream(0.5));
  EXPECT_EQ(0.5, pump.fractionofMotorInefficienciestoFluidStream());
  pump.resetFractionofMotorInefficienciestoFluidStream();
  EXPECT_EQ(0.0, pump.fractionofMotorInefficienciestoFluidStream());

  // Pump Control Type:  String
  // Check Idd default: "Continuous", which is overriden by Ctor as Intermittent actually...
  EXPECT_EQ("Intermittent", pump.pumpControlType());
  // Test a valid choice
  EXPECT_TRUE(pump.setPumpControlType("Continuous"));
  EXPECT_EQ("Continuous", pump.pumpControlType());
  // Test an invalid choice
  EXPECT_FALSE(pump.setPumpControlType("BadChoice"));
  EXPECT_EQ("Continuous", pump.pumpControlType());

  // Pump Flow Rate Schedule: Optional Object
  // No Default
  EXPECT_FALSE(pump.pumpFlowRateSchedule());
  ScheduleCompact sch_pump(m);
  EXPECT_TRUE(pump.setPumpFlowRateSchedule(sch_pump));
  ASSERT_TRUE(pump.pumpFlowRateSchedule());
  EXPECT_EQ(sch_pump, pump.pumpFlowRateSchedule().get());

  // Pump Curve: Optional Object
  // No Default
  EXPECT_FALSE(pump.pumpCurve());
  CurveLinear c(m);
  EXPECT_TRUE(pump.setPumpCurve(c));
  ASSERT_TRUE(pump.pumpCurve());
  EXPECT_EQ(c, pump.pumpCurve().get());

  // Impeller Diameter: Optional Double
  // No Default
  EXPECT_TRUE(pump.setImpellerDiameter(10.03));
  ASSERT_TRUE(pump.impellerDiameter());
  EXPECT_EQ(10.03, pump.impellerDiameter().get());
  pump.resetImpellerDiameter();
  EXPECT_FALSE(pump.impellerDiameter());

  // Rotational Speed: Optional Double
  // No Default
  EXPECT_TRUE(pump.setRotationalSpeed(10.03));
  ASSERT_TRUE(pump.rotationalSpeed());
  EXPECT_EQ(10.03, pump.rotationalSpeed().get());
  pump.resetRotationalSpeed();
  EXPECT_FALSE(pump.rotationalSpeed());

  // Zone: Optional Object
  // No Default
  EXPECT_FALSE(pump.zone());
  ThermalZone z(m);
  EXPECT_TRUE(pump.setZone(z));
  ASSERT_TRUE(pump.zone());
  EXPECT_EQ(z, pump.zone().get());

  // Skin Loss Radiative Fraction:  Double
  // No Default
  EXPECT_TRUE(pump.setSkinLossRadiativeFraction(0.5));
  EXPECT_EQ(0.5, pump.skinLossRadiativeFraction().get());

  // Design Power Sizing Method:  String
  // Default to PowerPerFlowPerPressure in Ctor
  EXPECT_EQ("PowerPerFlowPerPressure", pump.designPowerSizingMethod());
  // Test a valid choice
  EXPECT_TRUE(pump.setDesignPowerSizingMethod("PowerPerFlow"));
  EXPECT_EQ("PowerPerFlow", pump.designPowerSizingMethod());
  // Test an invalid choice
  EXPECT_FALSE(pump.setDesignPowerSizingMethod("BadChoice"));
  EXPECT_EQ("PowerPerFlow", pump.designPowerSizingMethod());

  // Design Electric Power per Unit Flow Rate:  Double
  // Defaults in Ctor
  EXPECT_EQ(348701.1, pump.designElectricPowerPerUnitFlowRate());
  EXPECT_TRUE(pump.setDesignElectricPowerPerUnitFlowRate(1.0));
  EXPECT_EQ(1.0, pump.designElectricPowerPerUnitFlowRate());

  // Design Shaft Power per Unit Flow Rate per Unit Head:  Double
  // Defaults in Ctor
  EXPECT_EQ(1.282051282, pump.designShaftPowerPerUnitFlowRatePerUnitHead());
  EXPECT_TRUE(pump.setDesignShaftPowerPerUnitFlowRatePerUnitHead(1.0));
  EXPECT_EQ(1.0, pump.designShaftPowerPerUnitFlowRatePerUnitHead());
}

TEST_F(ModelFixture, PumpConstantSpeed_flowRateSchedule) {
  Model m;
  PumpConstantSpeed pump(m);
  auto alwaysOnSchedule = m.alwaysOnDiscreteSchedule();
  EXPECT_TRUE(pump.setPumpFlowRateSchedule(alwaysOnSchedule));
  auto s = pump.pumpFlowRateSchedule();
  EXPECT_TRUE(s);
  EXPECT_EQ(alwaysOnSchedule, s.get());
}

TEST_F(ModelFixture, PumpConstantSpeed_addToNode) {
  Model m;
  PumpConstantSpeed testObject(m);

  AirLoopHVAC airLoop(m);

  Node supplyOutletNode = airLoop.supplyOutletNode();

  EXPECT_FALSE(testObject.addToNode(supplyOutletNode));
  EXPECT_EQ((unsigned)2, airLoop.supplyComponents().size());

  Node inletNode = airLoop.zoneSplitter().lastOutletModelObject()->cast<Node>();

  EXPECT_FALSE(testObject.addToNode(inletNode));
  EXPECT_EQ((unsigned)5, airLoop.demandComponents().size());

  PlantLoop plantLoop(m);
  supplyOutletNode = plantLoop.supplyOutletNode();
  EXPECT_TRUE(testObject.addToNode(supplyOutletNode));
  EXPECT_EQ((unsigned)7, plantLoop.supplyComponents().size());

  Node demandOutletNode = plantLoop.demandOutletNode();
  EXPECT_TRUE(testObject.addToNode(demandOutletNode));
  EXPECT_EQ((unsigned)7, plantLoop.demandComponents().size());

  PumpConstantSpeed testObject2(m);

  EXPECT_TRUE(testObject2.addToNode(demandOutletNode));
  EXPECT_EQ((unsigned)9, plantLoop.demandComponents().size());

  PlantLoop plantLoop2(m);
  demandOutletNode = plantLoop2.demandOutletNode();
  EXPECT_TRUE(testObject.addToNode(demandOutletNode));
  EXPECT_EQ((unsigned)7, plantLoop2.demandComponents().size());

  auto testObjectClone = testObject.clone(m).cast<PumpConstantSpeed>();
  supplyOutletNode = plantLoop.supplyOutletNode();

  EXPECT_TRUE(testObjectClone.addToNode(supplyOutletNode));
  EXPECT_EQ((unsigned)7, plantLoop.supplyComponents().size());
}
