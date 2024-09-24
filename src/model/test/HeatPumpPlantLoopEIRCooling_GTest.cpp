/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>

#include "ModelFixture.hpp"

#include "../Model.hpp"
#include "../Model_Impl.hpp"

#include "../HeatPumpPlantLoopEIRHeating.hpp"
#include "../HeatPumpPlantLoopEIRHeating_Impl.hpp"
#include "../HeatPumpPlantLoopEIRCooling.hpp"
#include "../HeatPumpPlantLoopEIRCooling_Impl.hpp"
#include "../Curve.hpp"
#include "../Curve_Impl.hpp"
#include "../CurveBiquadratic.hpp"
#include "../CurveBiquadratic_Impl.hpp"
#include "../CurveQuadratic.hpp"
#include "../CurveQuadratic_Impl.hpp"
#include "../Node.hpp"
#include "../Node_Impl.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, HeatPumpPlantLoopEIRCooling_HeatPumpPlantLoopEIRCooling) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  ASSERT_EXIT(
    {
      Model m;
      HeatPumpPlantLoopEIRCooling hp(m);

      exit(0);
    },
    ::testing::ExitedWithCode(0), "");

  Model m;
  HeatPumpPlantLoopEIRCooling hp(m);

  EXPECT_EQ("AirSource", hp.condenserType());
  EXPECT_FALSE(hp.companionHeatingHeatPump());
  EXPECT_FALSE(hp.loadSideReferenceFlowRate());
  EXPECT_TRUE(hp.isLoadSideReferenceFlowRateAutosized());
  EXPECT_FALSE(hp.sourceSideReferenceFlowRate());
  EXPECT_TRUE(hp.isSourceSideReferenceFlowRateAutosized());
  EXPECT_FALSE(hp.referenceCapacity());
  EXPECT_TRUE(hp.isReferenceCapacityAutosized());
  EXPECT_EQ(7.5, hp.referenceCoefficientofPerformance());
  EXPECT_EQ(1.0, hp.sizingFactor());
  boost::optional<Curve> capacityModifierFunctionofTemperatureCurve = hp.capacityModifierFunctionofTemperatureCurve();
  EXPECT_TRUE(capacityModifierFunctionofTemperatureCurve);
  boost::optional<Curve> electricInputtoOutputRatioModifierFunctionofTemperatureCurve =
    hp.electricInputtoOutputRatioModifierFunctionofTemperatureCurve();
  EXPECT_TRUE(electricInputtoOutputRatioModifierFunctionofTemperatureCurve);
  boost::optional<Curve> electricInputtoOutputRatioModifierFunctionofPartLoadRatioCurve =
    hp.electricInputtoOutputRatioModifierFunctionofPartLoadRatioCurve();
  EXPECT_TRUE(electricInputtoOutputRatioModifierFunctionofPartLoadRatioCurve);
  EXPECT_EQ("Load", hp.controlType());
  EXPECT_EQ("ConstantFlow", hp.flowMode());
  EXPECT_EQ(0.0, hp.minimumPartLoadRatio());
  EXPECT_EQ(-100.0, hp.minimumSourceInletTemperature());
  EXPECT_EQ(100.0, hp.maximumSourceInletTemperature());
  EXPECT_FALSE(hp.minimumSupplyWaterTemperatureCurve());
  EXPECT_FALSE(hp.maximumSupplyWaterTemperatureCurve());
  EXPECT_EQ(60.0, hp.maximumHeatRecoveryOutletTemperature());
  EXPECT_FALSE(hp.heatRecoveryCapacityModifierFunctionofTemperatureCurve());
  EXPECT_FALSE(hp.heatRecoveryElectricInputtoOutputRatioModifierFunctionofTemperatureCurve());
  EXPECT_FALSE(hp.thermosiphonCapacityFractionCurve());
  EXPECT_EQ(0.0, hp.thermosiphonMinimumTemperatureDifference());
}

TEST_F(ModelFixture, HeatPumpPlantLoopEIRCooling_GettersSetters) {
  Model m;
  HeatPumpPlantLoopEIRCooling hp(m);

  EXPECT_TRUE(hp.setCondenserType("AirSource"));
  HeatPumpPlantLoopEIRHeating companionHP(m);
  EXPECT_TRUE(hp.setCompanionHeatingHeatPump(companionHP));
  EXPECT_TRUE(hp.setLoadSideReferenceFlowRate(1.0));
  EXPECT_TRUE(hp.setSourceSideReferenceFlowRate(2.0));
  EXPECT_TRUE(hp.setReferenceCapacity(3.0));
  EXPECT_TRUE(hp.setReferenceCoefficientofPerformance(4.0));
  EXPECT_TRUE(hp.setSizingFactor(5.0));
  CurveBiquadratic curve1(m);
  EXPECT_TRUE(hp.setCapacityModifierFunctionofTemperatureCurve(curve1));
  CurveBiquadratic curve2(m);
  EXPECT_TRUE(hp.setElectricInputtoOutputRatioModifierFunctionofTemperatureCurve(curve2));
  CurveQuadratic curve3(m);
  EXPECT_TRUE(hp.setElectricInputtoOutputRatioModifierFunctionofPartLoadRatioCurve(curve3));
  EXPECT_TRUE(hp.setControlType("Setpoint"));
  EXPECT_TRUE(hp.setFlowMode("VariableSpeedPumping"));
  EXPECT_TRUE(hp.setMinimumPartLoadRatio(1));
  EXPECT_TRUE(hp.setMinimumSourceInletTemperature(2));
  EXPECT_TRUE(hp.setMaximumSourceInletTemperature(3));
  CurveQuadratic curve4(m);
  EXPECT_TRUE(hp.setMinimumSupplyWaterTemperatureCurve(curve4));
  CurveQuadratic curve5(m);
  EXPECT_TRUE(hp.setMaximumSupplyWaterTemperatureCurve(curve5));
  EXPECT_TRUE(hp.setMaximumHeatRecoveryOutletTemperature(4));
  CurveBiquadratic curve6(m);
  EXPECT_TRUE(hp.setHeatRecoveryCapacityModifierFunctionofTemperatureCurve(curve6));
  CurveBiquadratic curve7(m);
  EXPECT_TRUE(hp.setHeatRecoveryElectricInputtoOutputRatioModifierFunctionofTemperatureCurve(curve7));
  CurveQuadratic curve8(m);
  EXPECT_TRUE(hp.setThermosiphonCapacityFractionCurve(curve8));
  EXPECT_TRUE(hp.setThermosiphonMinimumTemperatureDifference(5));

  EXPECT_EQ("AirSource", hp.condenserType());
  ASSERT_TRUE(hp.companionHeatingHeatPump());
  EXPECT_EQ(companionHP.handle(), hp.companionHeatingHeatPump().get().handle());
  ASSERT_TRUE(hp.loadSideReferenceFlowRate());
  EXPECT_EQ(1.0, hp.loadSideReferenceFlowRate().get());
  EXPECT_FALSE(hp.isLoadSideReferenceFlowRateAutosized());
  ASSERT_TRUE(hp.sourceSideReferenceFlowRate());
  EXPECT_EQ(2.0, hp.sourceSideReferenceFlowRate().get());
  EXPECT_FALSE(hp.isSourceSideReferenceFlowRateAutosized());
  ASSERT_TRUE(hp.referenceCapacity());
  EXPECT_EQ(3.0, hp.referenceCapacity().get());
  EXPECT_FALSE(hp.isReferenceCapacityAutosized());
  EXPECT_EQ(4.0, hp.referenceCoefficientofPerformance());
  EXPECT_EQ(5.0, hp.sizingFactor());
  EXPECT_EQ(curve1.handle(), hp.capacityModifierFunctionofTemperatureCurve().handle());
  EXPECT_EQ(curve2.handle(), hp.electricInputtoOutputRatioModifierFunctionofTemperatureCurve().handle());
  EXPECT_EQ(curve3.handle(), hp.electricInputtoOutputRatioModifierFunctionofPartLoadRatioCurve().handle());
  EXPECT_EQ("Setpoint", hp.controlType());
  EXPECT_EQ("VariableSpeedPumping", hp.flowMode());
  EXPECT_EQ(1, hp.minimumPartLoadRatio());
  EXPECT_EQ(2, hp.minimumSourceInletTemperature());
  EXPECT_EQ(3, hp.maximumSourceInletTemperature());
  ASSERT_TRUE(hp.minimumSupplyWaterTemperatureCurve());
  EXPECT_EQ(curve4.handle(), hp.minimumSupplyWaterTemperatureCurve().get().handle());
  ASSERT_TRUE(hp.maximumSupplyWaterTemperatureCurve());
  EXPECT_EQ(curve5.handle(), hp.maximumSupplyWaterTemperatureCurve().get().handle());
  EXPECT_EQ(4, hp.maximumHeatRecoveryOutletTemperature());
  ASSERT_TRUE(hp.heatRecoveryCapacityModifierFunctionofTemperatureCurve());
  EXPECT_EQ(curve6.handle(), hp.heatRecoveryCapacityModifierFunctionofTemperatureCurve().get().handle());
  ASSERT_TRUE(hp.heatRecoveryElectricInputtoOutputRatioModifierFunctionofTemperatureCurve());
  EXPECT_EQ(curve7.handle(), hp.heatRecoveryElectricInputtoOutputRatioModifierFunctionofTemperatureCurve().get().handle());
  ASSERT_TRUE(hp.thermosiphonCapacityFractionCurve());
  EXPECT_EQ(curve8.handle(), hp.thermosiphonCapacityFractionCurve().get().handle());
  EXPECT_EQ(5, hp.thermosiphonMinimumTemperatureDifference());

  hp.autosizeLoadSideReferenceFlowRate();
  EXPECT_TRUE(hp.isLoadSideReferenceFlowRateAutosized());
  EXPECT_FALSE(hp.isSourceSideReferenceFlowRateAutosized());
  EXPECT_FALSE(hp.isReferenceCapacityAutosized());

  hp.autosizeSourceSideReferenceFlowRate();
  EXPECT_TRUE(hp.isLoadSideReferenceFlowRateAutosized());
  EXPECT_TRUE(hp.isSourceSideReferenceFlowRateAutosized());
  EXPECT_FALSE(hp.isReferenceCapacityAutosized());

  hp.autosizeReferenceCapacity();
  EXPECT_TRUE(hp.isLoadSideReferenceFlowRateAutosized());
  EXPECT_TRUE(hp.isSourceSideReferenceFlowRateAutosized());
  EXPECT_TRUE(hp.isReferenceCapacityAutosized());

  hp.resetMinimumSupplyWaterTemperatureCurve();
  hp.resetMaximumSupplyWaterTemperatureCurve();
  hp.resetHeatRecoveryCapacityModifierFunctionofTemperatureCurve();
  hp.resetHeatRecoveryElectricInputtoOutputRatioModifierFunctionofTemperatureCurve();
  hp.resetThermosiphonCapacityFractionCurve();
  EXPECT_FALSE(hp.minimumSupplyWaterTemperatureCurve());
  EXPECT_FALSE(hp.maximumSupplyWaterTemperatureCurve());
  EXPECT_FALSE(hp.heatRecoveryCapacityModifierFunctionofTemperatureCurve());
  EXPECT_FALSE(hp.heatRecoveryElectricInputtoOutputRatioModifierFunctionofTemperatureCurve());
  EXPECT_FALSE(hp.thermosiphonCapacityFractionCurve());
}

TEST_F(ModelFixture, HeatPumpPlantLoopEIRCooling_remove) {
  Model m;
  HeatPumpPlantLoopEIRCooling hp(m);
  auto size = m.modelObjects().size();
  EXPECT_FALSE(hp.remove().empty());
  EXPECT_EQ(size - 1, m.modelObjects().size());
  EXPECT_EQ(0u, m.getConcreteModelObjects<HeatPumpPlantLoopEIRCooling>().size());
}

TEST_F(ModelFixture, HeatPumpPlantLoopEIRCooling_clone) {
  Model m;
  CurveBiquadratic curve1(m);
  CurveBiquadratic curve2(m);
  CurveQuadratic curve3(m);
  HeatPumpPlantLoopEIRCooling hp(m, curve1, curve2, curve3);
  EXPECT_EQ(2u, m.getConcreteModelObjects<CurveBiquadratic>().size());
  EXPECT_EQ(1u, m.getConcreteModelObjects<CurveQuadratic>().size());

  {
    auto hpClone = hp.clone(m).cast<HeatPumpPlantLoopEIRCooling>();
    EXPECT_EQ(curve1.handle(), hp.capacityModifierFunctionofTemperatureCurve().handle());
    EXPECT_EQ(curve2.handle(), hp.electricInputtoOutputRatioModifierFunctionofTemperatureCurve().handle());
    EXPECT_EQ(curve3.handle(), hp.electricInputtoOutputRatioModifierFunctionofPartLoadRatioCurve().handle());
    EXPECT_EQ(2u, m.getConcreteModelObjects<CurveBiquadratic>().size());
    EXPECT_EQ(1u, m.getConcreteModelObjects<CurveQuadratic>().size());
  }

  {
    Model m2;
    auto hpClone2 = hp.clone(m2).cast<HeatPumpPlantLoopEIRCooling>();
    EXPECT_EQ(curve1.handle(), hp.capacityModifierFunctionofTemperatureCurve().handle());
    EXPECT_EQ(curve2.handle(), hp.electricInputtoOutputRatioModifierFunctionofTemperatureCurve().handle());
    EXPECT_EQ(curve3.handle(), hp.electricInputtoOutputRatioModifierFunctionofPartLoadRatioCurve().handle());
    EXPECT_EQ(2u, m2.getConcreteModelObjects<CurveBiquadratic>().size());
    EXPECT_EQ(1u, m2.getConcreteModelObjects<CurveQuadratic>().size());
  }
}

// Check condenser type setting/defaulting
TEST_F(ModelFixture, HeatPumpPlantLoopEIRCooling_CondenserType) {
  Model m;

  PlantLoop pl1(m);
  PlantLoop pl2(m);

  HeatPumpPlantLoopEIRCooling hp(m);

  // By default, AirCooled (from IDD)
  EXPECT_EQ("AirSource", hp.condenserType());

  // Not connected to a secondary plantLoop
  EXPECT_TRUE(hp.setCondenserType("AirSource"));
  EXPECT_EQ("AirSource", hp.condenserType());

  EXPECT_FALSE(hp.setCondenserType("WaterSource"));

  // Add to primary plant loop (on supply), behavior should be the same
  EXPECT_TRUE(pl1.addSupplyBranchForComponent(hp));
  // Should have stayed
  EXPECT_EQ("AirSource", hp.condenserType());
  EXPECT_FALSE(hp.setCondenserType("WaterSource"));

  // Add to the Secondary plant loop (on demand), behavior should be reversed
  EXPECT_TRUE(pl2.addDemandBranchForComponent(hp));
  // Should have been automatically set to WaterSource
  EXPECT_EQ("WaterSource", hp.condenserType());

  EXPECT_FALSE(hp.setCondenserType("AirSource"));

  // Test the convenience functions provided for clarity
  EXPECT_EQ(hp.plantLoop().get(), hp.loadSideWaterLoop().get());
  EXPECT_EQ(hp.supplyInletModelObject()->cast<Node>(), hp.loadSideWaterInletNode().get());
  EXPECT_EQ(hp.supplyOutletModelObject()->cast<Node>(), hp.loadSideWaterOutletNode().get());

  EXPECT_EQ(hp.secondaryPlantLoop().get(), hp.sourceSideWaterLoop().get());
  EXPECT_EQ(hp.demandInletModelObject()->cast<Node>(), hp.sourceSideWaterInletNode().get());
  EXPECT_EQ(hp.demandOutletModelObject()->cast<Node>(), hp.sourceSideWaterOutletNode().get());

  // Disconnect from the secondary plant Loop
  EXPECT_TRUE(hp.removeFromSecondaryPlantLoop());
  // Should have been automatically switched to AirSource
  EXPECT_EQ("AirSource", hp.condenserType());

  EXPECT_FALSE(hp.setCondenserType("WaterSource"));

  // Test convenience functions again
  EXPECT_EQ(hp.plantLoop().get(), hp.loadSideWaterLoop().get());
  EXPECT_EQ(hp.supplyInletModelObject()->cast<Node>(), hp.loadSideWaterInletNode().get());
  EXPECT_EQ(hp.supplyOutletModelObject()->cast<Node>(), hp.loadSideWaterOutletNode().get());

  EXPECT_FALSE(hp.secondaryPlantLoop());
  EXPECT_FALSE(hp.sourceSideWaterLoop());
  EXPECT_FALSE(hp.demandInletModelObject());
  EXPECT_FALSE(hp.sourceSideWaterInletNode());
  EXPECT_FALSE(hp.demandOutletModelObject());
  EXPECT_FALSE(hp.sourceSideWaterOutletNode());
}

TEST_F(ModelFixture, HeatPumpPlantLoopEIRCooling_PlantLoopConnections_addToNodeOverride) {
  Model model;
  HeatPumpPlantLoopEIRCooling hp(model);

  PlantLoop pl1(model);
  PlantLoop pl2(model);
  PlantLoop pl3(model);

  auto c_demand_node2 = pl3.demandOutletNode();

  PlantLoop hrLoop(model);
  auto h_supply_node = hrLoop.supplyOutletNode();
  auto h_demand_node = hrLoop.demandInletNode();

  // Connect to the load side loop
  EXPECT_TRUE(pl1.addSupplyBranchForComponent(hp));
  ASSERT_TRUE(hp.loadSideWaterLoop());
  EXPECT_EQ(pl1, hp.loadSideWaterLoop().get());

  EXPECT_FALSE(hp.sourceSideWaterLoop());
  EXPECT_FALSE(hp.heatRecoveryLoop());

  // Connect to the source side loop
  EXPECT_TRUE(pl2.addDemandBranchForComponent(hp));

  ASSERT_TRUE(hp.loadSideWaterLoop());
  EXPECT_EQ(pl1, hp.loadSideWaterLoop().get());

  ASSERT_TRUE(hp.sourceSideWaterLoop());
  EXPECT_EQ(pl2, hp.sourceSideWaterLoop().get());

  EXPECT_FALSE(hp.heatRecoveryLoop());

  // Have a load side loop and no hr loop: should connect the hr loop if trying to add to demand side
  EXPECT_TRUE(hrLoop.addDemandBranchForComponent(hp));
  ASSERT_TRUE(hp.loadSideWaterLoop());
  EXPECT_EQ(pl1, hp.loadSideWaterLoop().get());
  ASSERT_TRUE(hp.sourceSideWaterLoop());
  EXPECT_EQ(pl2, hp.sourceSideWaterLoop().get());
  ASSERT_TRUE(hp.heatRecoveryLoop());
  EXPECT_EQ(hrLoop, hp.heatRecoveryLoop().get());

  // Have a source side loop and a hr loop: should reconnect the source side loop
  // Try with addToNode instead
  EXPECT_TRUE(hp.addToNode(c_demand_node2));

  ASSERT_TRUE(hp.sourceSideWaterLoop());
  EXPECT_EQ(pl3, hp.sourceSideWaterLoop().get());

  ASSERT_TRUE(hp.loadSideWaterLoop());
  EXPECT_EQ(pl1, hp.loadSideWaterLoop().get());

  ASSERT_TRUE(hp.heatRecoveryLoop());
  EXPECT_EQ(hrLoop, hp.heatRecoveryLoop().get());

  // Disconnect the tertiary (hr) loop
  EXPECT_TRUE(hp.removeFromTertiaryPlantLoop());
  EXPECT_FALSE(hp.heatRecoveryLoop());

  // Shouldn't accept tertiary connection to the supply side
  EXPECT_FALSE(hp.addToTertiaryNode(h_supply_node));
  EXPECT_FALSE(hp.heatRecoveryLoop());

  // Try addToNode to the demand side of hr plant loop, should work to connect the hr loop
  EXPECT_TRUE(hp.addToNode(h_demand_node));

  ASSERT_TRUE(hp.loadSideWaterLoop());
  EXPECT_EQ(pl1, hp.loadSideWaterLoop().get());

  ASSERT_TRUE(hp.sourceSideWaterLoop());
  EXPECT_EQ(pl3, hp.sourceSideWaterLoop().get());

  ASSERT_TRUE(hp.heatRecoveryLoop());
  EXPECT_EQ(hrLoop, hp.heatRecoveryLoop().get());
}
