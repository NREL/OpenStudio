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

TEST_F(ModelFixture, HeatPumpPlantLoopEIRHeating_HeatPumpPlantLoopEIRHeating) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  ASSERT_EXIT(
    {
      Model m;
      HeatPumpPlantLoopEIRHeating hp(m);

      exit(0);
    },
    ::testing::ExitedWithCode(0), "");

  Model m;
  HeatPumpPlantLoopEIRHeating hp(m);

  EXPECT_EQ("AirSource", hp.condenserType());
  EXPECT_FALSE(hp.companionCoolingHeatPump());
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
}

TEST_F(ModelFixture, HeatPumpPlantLoopEIRHeating_GettersSetters) {
  Model m;
  HeatPumpPlantLoopEIRHeating hp(m);

  EXPECT_TRUE(hp.setCondenserType("AirSource"));
  HeatPumpPlantLoopEIRCooling companionHP(m);
  EXPECT_TRUE(hp.setCompanionCoolingHeatPump(companionHP));
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

  EXPECT_EQ("AirSource", hp.condenserType());
  ASSERT_TRUE(hp.companionCoolingHeatPump());
  EXPECT_EQ(companionHP.handle(), hp.companionCoolingHeatPump().get().handle());
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
}

TEST_F(ModelFixture, HeatPumpPlantLoopEIRHeating_remove) {
  Model m;
  HeatPumpPlantLoopEIRHeating hp(m);
  auto size = m.modelObjects().size();
  EXPECT_FALSE(hp.remove().empty());
  EXPECT_EQ(size - 1, m.modelObjects().size());
  EXPECT_EQ(0u, m.getConcreteModelObjects<HeatPumpPlantLoopEIRHeating>().size());
}

TEST_F(ModelFixture, HeatPumpPlantLoopEIRHeating_clone) {
  Model m;
  CurveBiquadratic curve1(m);
  CurveBiquadratic curve2(m);
  CurveQuadratic curve3(m);
  HeatPumpPlantLoopEIRHeating hp(m, curve1, curve2, curve3);
  EXPECT_EQ(2u, m.getConcreteModelObjects<CurveBiquadratic>().size());
  EXPECT_EQ(1u, m.getConcreteModelObjects<CurveQuadratic>().size());

  {
    auto hpClone = hp.clone(m).cast<HeatPumpPlantLoopEIRHeating>();
    EXPECT_EQ(curve1.handle(), hp.capacityModifierFunctionofTemperatureCurve().handle());
    EXPECT_EQ(curve2.handle(), hp.electricInputtoOutputRatioModifierFunctionofTemperatureCurve().handle());
    EXPECT_EQ(curve3.handle(), hp.electricInputtoOutputRatioModifierFunctionofPartLoadRatioCurve().handle());
    EXPECT_EQ(2u, m.getConcreteModelObjects<CurveBiquadratic>().size());
    EXPECT_EQ(1u, m.getConcreteModelObjects<CurveQuadratic>().size());
  }

  {
    Model m2;
    auto hpClone2 = hp.clone(m2).cast<HeatPumpPlantLoopEIRHeating>();
    EXPECT_EQ(curve1.handle(), hp.capacityModifierFunctionofTemperatureCurve().handle());
    EXPECT_EQ(curve2.handle(), hp.electricInputtoOutputRatioModifierFunctionofTemperatureCurve().handle());
    EXPECT_EQ(curve3.handle(), hp.electricInputtoOutputRatioModifierFunctionofPartLoadRatioCurve().handle());
    EXPECT_EQ(2u, m2.getConcreteModelObjects<CurveBiquadratic>().size());
    EXPECT_EQ(1u, m2.getConcreteModelObjects<CurveQuadratic>().size());
  }
}

// Check condenser type setting/defaulting
TEST_F(ModelFixture, HeatPumpPlantLoopEIRHeating_CondenserType) {
  Model m;

  PlantLoop pl1(m);
  PlantLoop pl2(m);

  HeatPumpPlantLoopEIRHeating hp(m);

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
