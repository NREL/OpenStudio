/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>
#include "ModelFixture.hpp"
#include "../CoilWaterHeatingDesuperheater.hpp"
#include "../CoilWaterHeatingDesuperheater_Impl.hpp"
#include "../RefrigerationCondenserAirCooled.hpp"
#include "../RefrigerationCondenserAirCooled_Impl.hpp"
#include "../CoilCoolingWaterToAirHeatPumpEquationFit.hpp"
#include "../CoilCoolingWaterToAirHeatPumpEquationFit_Impl.hpp"
#include "../CoilCoolingDXMultiSpeed.hpp"
#include "../CoilCoolingDXMultiSpeed_Impl.hpp"
#include "../WaterHeaterMixed.hpp"
#include "../WaterHeaterMixed_Impl.hpp"
#include "../WaterHeaterStratified.hpp"
#include "../WaterHeaterStratified_Impl.hpp"
#include "../ScheduleCompact.hpp"
#include "../ScheduleCompact_Impl.hpp"
#include "../CurveBiquadratic.hpp"
#include "../CurveBiquadratic_Impl.hpp"
#include "../AirLoopHVAC.hpp"
#include "../PlantLoop.hpp"
#include "../Node.hpp"
#include "../Node_Impl.hpp"
#include "../AirLoopHVACZoneSplitter.hpp"

using namespace openstudio;
using namespace openstudio::model;

//Test construction of Coil:Heating:Desuperheater
TEST_F(ModelFixture, CoilWaterHeatingDesuperheater_DefaultConstructor) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  ASSERT_EXIT(
    {
      Model model;
      ScheduleCompact schedule = ScheduleCompact(model);
      CoilWaterHeatingDesuperheater testObject = CoilWaterHeatingDesuperheater(model, schedule);

      exit(0);
    },
    ::testing::ExitedWithCode(0), "");

  // Passing an incompatible schedule: we expect the Ctor to throw, and no object to be created
  Model model;
  EXPECT_EQ(0, model.getConcreteModelObjects<CoilWaterHeatingDesuperheater>().size());
  auto alwaysOn = model.alwaysOnDiscreteSchedule();
  EXPECT_ANY_THROW(CoilWaterHeatingDesuperheater(model, alwaysOn));
  EXPECT_EQ(0, model.getConcreteModelObjects<CoilWaterHeatingDesuperheater>().size());
}

TEST_F(ModelFixture, CoilWaterHeatingDesuperheater_Remove1) {
  Model model;
  ScheduleCompact schedule = ScheduleCompact(model);
  CoilWaterHeatingDesuperheater testObject = CoilWaterHeatingDesuperheater(model, schedule);
  RefrigerationCondenserAirCooled condenser = RefrigerationCondenserAirCooled(model);
  WaterHeaterMixed heatRejection = WaterHeaterMixed(model);
  CurveBiquadratic curve = CurveBiquadratic(model);

  EXPECT_TRUE(testObject.setHeatingSource(condenser));
  EXPECT_TRUE(testObject.addToHeatRejectionTarget(heatRejection));
  EXPECT_TRUE(testObject.setHeatReclaimEfficiencyFunctionofTemperatureCurve(curve));

  std::vector<CoilWaterHeatingDesuperheater> coilWaterHeatingDesuperheaters = model.getConcreteModelObjects<CoilWaterHeatingDesuperheater>();
  EXPECT_EQ(1, coilWaterHeatingDesuperheaters.size());

  std::vector<RefrigerationCondenserAirCooled> refrigerationCondensers = model.getConcreteModelObjects<RefrigerationCondenserAirCooled>();
  EXPECT_EQ(1, refrigerationCondensers.size());

  std::vector<WaterHeaterMixed> heatRejectionTargets = model.getConcreteModelObjects<WaterHeaterMixed>();
  EXPECT_EQ(1, heatRejectionTargets.size());

  std::vector<CurveBiquadratic> curves = model.getConcreteModelObjects<CurveBiquadratic>();
  EXPECT_EQ(1, curves.size());

  std::vector<Node> nodes = model.getConcreteModelObjects<Node>();
  EXPECT_EQ(2, nodes.size());

  testObject.remove();

  coilWaterHeatingDesuperheaters = model.getConcreteModelObjects<CoilWaterHeatingDesuperheater>();
  EXPECT_EQ(0, coilWaterHeatingDesuperheaters.size());

  refrigerationCondensers = model.getConcreteModelObjects<RefrigerationCondenserAirCooled>();
  EXPECT_EQ(1, refrigerationCondensers.size());

  heatRejectionTargets = model.getConcreteModelObjects<WaterHeaterMixed>();
  EXPECT_EQ(1, heatRejectionTargets.size());

  // Curve was used only by this object, so should have been removed
  curves = model.getConcreteModelObjects<CurveBiquadratic>();
  EXPECT_EQ(0, curves.size());

  nodes = model.getConcreteModelObjects<Node>();
  EXPECT_EQ(0, nodes.size());

  EXPECT_FALSE(heatRejection.demandInletModelObject());
  EXPECT_FALSE(heatRejection.demandOutletModelObject());
}

TEST_F(ModelFixture, CoilWaterHeatingDesuperheater_Remove2) {
  Model model;
  ScheduleCompact schedule = ScheduleCompact(model);
  CoilWaterHeatingDesuperheater testObject = CoilWaterHeatingDesuperheater(model, schedule);
  WaterHeaterStratified heatRejection = WaterHeaterStratified(model);
  CoilCoolingWaterToAirHeatPumpEquationFit heatingSource = CoilCoolingWaterToAirHeatPumpEquationFit(model);
  CurveBiquadratic curve = CurveBiquadratic(model);

  EXPECT_TRUE(testObject.addToHeatRejectionTarget(heatRejection));
  EXPECT_TRUE(testObject.setHeatingSource(heatingSource));
  EXPECT_TRUE(testObject.setHeatReclaimEfficiencyFunctionofTemperatureCurve(curve));

  std::vector<CoilWaterHeatingDesuperheater> coilWaterHeatingDesuperheaters = model.getConcreteModelObjects<CoilWaterHeatingDesuperheater>();
  EXPECT_EQ(1, coilWaterHeatingDesuperheaters.size());

  std::vector<CoilCoolingWaterToAirHeatPumpEquationFit> coilEquationFits = model.getConcreteModelObjects<CoilCoolingWaterToAirHeatPumpEquationFit>();
  EXPECT_EQ(1, coilEquationFits.size());

  std::vector<WaterHeaterStratified> heatRejectionTargets = model.getConcreteModelObjects<WaterHeaterStratified>();
  EXPECT_EQ(1, heatRejectionTargets.size());
  ASSERT_TRUE(testObject.heatRejectionTarget());

  std::vector<CoilCoolingWaterToAirHeatPumpEquationFit> heatingSources = model.getConcreteModelObjects<CoilCoolingWaterToAirHeatPumpEquationFit>();
  EXPECT_EQ(1, heatingSources.size());
  ASSERT_TRUE(testObject.heatingSource());

  std::vector<CurveBiquadratic> curves = model.getConcreteModelObjects<CurveBiquadratic>();
  EXPECT_EQ(1, curves.size());

  std::vector<Node> nodes = model.getConcreteModelObjects<Node>();
  EXPECT_EQ(2, nodes.size());

  testObject.remove();

  coilWaterHeatingDesuperheaters = model.getConcreteModelObjects<CoilWaterHeatingDesuperheater>();
  EXPECT_EQ(0, coilWaterHeatingDesuperheaters.size());

  coilEquationFits = model.getConcreteModelObjects<CoilCoolingWaterToAirHeatPumpEquationFit>();
  EXPECT_EQ(1, coilEquationFits.size());

  heatRejectionTargets = model.getConcreteModelObjects<WaterHeaterStratified>();
  EXPECT_EQ(1, heatRejectionTargets.size());

  heatingSources = model.getConcreteModelObjects<CoilCoolingWaterToAirHeatPumpEquationFit>();
  EXPECT_EQ(1, heatingSources.size());

  // Curve was used only by this object, so should have been removed
  curves = model.getConcreteModelObjects<CurveBiquadratic>();
  EXPECT_EQ(0, curves.size());

  nodes = model.getConcreteModelObjects<Node>();
  EXPECT_EQ(0, nodes.size());

  EXPECT_FALSE(heatRejection.demandInletModelObject());
  EXPECT_FALSE(heatRejection.demandOutletModelObject());
}

TEST_F(ModelFixture, CoilWaterHeatingDesuperheater_Remove3) {
  Model model;
  ScheduleCompact schedule = ScheduleCompact(model);
  CoilWaterHeatingDesuperheater testObject = CoilWaterHeatingDesuperheater(model, schedule);
  WaterHeaterStratified heatRejection = WaterHeaterStratified(model);
  CoilCoolingDXMultiSpeed heatingSource = CoilCoolingDXMultiSpeed(model);
  CurveBiquadratic curve = CurveBiquadratic(model);

  EXPECT_TRUE(testObject.addToHeatRejectionTarget(heatRejection));
  EXPECT_TRUE(testObject.setHeatingSource(heatingSource));
  EXPECT_TRUE(testObject.setHeatReclaimEfficiencyFunctionofTemperatureCurve(curve));

  std::vector<CoilWaterHeatingDesuperheater> coilWaterHeatingDesuperheaters = model.getConcreteModelObjects<CoilWaterHeatingDesuperheater>();
  EXPECT_EQ(1, coilWaterHeatingDesuperheaters.size());

  std::vector<CoilCoolingDXMultiSpeed> coilMultiSpeeds = model.getConcreteModelObjects<CoilCoolingDXMultiSpeed>();
  EXPECT_EQ(1, coilMultiSpeeds.size());

  std::vector<WaterHeaterStratified> heatRejectionTargets = model.getConcreteModelObjects<WaterHeaterStratified>();
  EXPECT_EQ(1, heatRejectionTargets.size());
  ASSERT_TRUE(testObject.heatRejectionTarget());

  std::vector<CoilCoolingDXMultiSpeed> heatingSources = model.getConcreteModelObjects<CoilCoolingDXMultiSpeed>();
  EXPECT_EQ(1, heatingSources.size());
  ASSERT_TRUE(testObject.heatingSource());

  std::vector<CurveBiquadratic> curves = model.getConcreteModelObjects<CurveBiquadratic>();
  EXPECT_EQ(1, curves.size());

  std::vector<Node> nodes = model.getConcreteModelObjects<Node>();
  EXPECT_EQ(2, nodes.size());

  testObject.remove();

  coilWaterHeatingDesuperheaters = model.getConcreteModelObjects<CoilWaterHeatingDesuperheater>();
  EXPECT_EQ(0, coilWaterHeatingDesuperheaters.size());

  coilMultiSpeeds = model.getConcreteModelObjects<CoilCoolingDXMultiSpeed>();
  EXPECT_EQ(1, coilMultiSpeeds.size());

  heatRejectionTargets = model.getConcreteModelObjects<WaterHeaterStratified>();
  EXPECT_EQ(1, heatRejectionTargets.size());

  heatingSources = model.getConcreteModelObjects<CoilCoolingDXMultiSpeed>();
  EXPECT_EQ(1, heatingSources.size());

  // Curve was used only by this object, so should have been removed
  curves = model.getConcreteModelObjects<CurveBiquadratic>();
  EXPECT_EQ(0, curves.size());

  nodes = model.getConcreteModelObjects<Node>();
  EXPECT_EQ(0, nodes.size());

  EXPECT_FALSE(heatRejection.demandInletModelObject());
  EXPECT_FALSE(heatRejection.demandOutletModelObject());
}

TEST_F(ModelFixture, CoilWaterHeatingDesuperheater_addToNode) {
  Model m;
  ScheduleCompact s(m);
  CoilWaterHeatingDesuperheater testObject(m, s);

  AirLoopHVAC airLoop(m);

  Node supplyOutletNode = airLoop.supplyOutletNode();

  EXPECT_FALSE(testObject.addToNode(supplyOutletNode));
  EXPECT_EQ((unsigned)2, airLoop.supplyComponents().size());

  Node inletNode = airLoop.zoneSplitter().lastOutletModelObject()->cast<Node>();

  EXPECT_FALSE(testObject.addToNode(inletNode));
  EXPECT_EQ((unsigned)5, airLoop.demandComponents().size());

  PlantLoop plantLoop(m);
  supplyOutletNode = plantLoop.supplyOutletNode();
  EXPECT_FALSE(testObject.addToNode(supplyOutletNode));
  EXPECT_EQ((unsigned)5, plantLoop.supplyComponents().size());

  Node demandOutletNode = plantLoop.demandOutletNode();
  EXPECT_FALSE(testObject.addToNode(demandOutletNode));
  EXPECT_EQ((unsigned)5, plantLoop.demandComponents().size());
}

TEST_F(ModelFixture, CoilWaterHeatingDesuperheater_AddBranchForHVACComponent_AirLoop) {
  Model model;
  ScheduleCompact schedule = ScheduleCompact(model);
  CoilWaterHeatingDesuperheater desuperheater(model, schedule);
  AirLoopHVAC airLoop = AirLoopHVAC(model);

  EXPECT_FALSE(airLoop.addBranchForHVACComponent(desuperheater));
}

TEST_F(ModelFixture, CoilWaterHeatingDesuperheater_AddDemandBranchForComponent_PlantLoop) {
  Model model;
  ScheduleCompact schedule = ScheduleCompact(model);
  CoilWaterHeatingDesuperheater desuperheater(model, schedule);
  PlantLoop plantLoop = PlantLoop(model);

  EXPECT_FALSE(plantLoop.addDemandBranchForComponent(desuperheater));
}

TEST_F(ModelFixture, CoilWaterHeatingDesuperheater_AddSupplyBranchForComponent_PlantLoop) {
  Model model;
  ScheduleCompact schedule = ScheduleCompact(model);
  CoilWaterHeatingDesuperheater desuperheater(model, schedule);
  PlantLoop plantLoop = PlantLoop(model);

  EXPECT_FALSE(plantLoop.addSupplyBranchForComponent(desuperheater));
}

TEST_F(ModelFixture, CoilWaterHeatingDesuperheater_CloneModelWithDefaultData) {
  Model model;
  ScheduleCompact schedule = ScheduleCompact(model);
  CoilWaterHeatingDesuperheater testObject = CoilWaterHeatingDesuperheater(model, schedule);

  auto testObjectClone = testObject.clone(model).cast<CoilWaterHeatingDesuperheater>();

  EXPECT_DOUBLE_EQ(5.0, testObjectClone.deadBandTemperatureDifference());
  EXPECT_FALSE(testObjectClone.ratedHeatReclaimRecoveryEfficiency());
  EXPECT_DOUBLE_EQ(50.0, testObjectClone.ratedInletWaterTemperature());
  EXPECT_DOUBLE_EQ(35.0, testObjectClone.ratedOutdoorAirTemperature());
  EXPECT_DOUBLE_EQ(60.0, testObjectClone.maximumInletWaterTemperatureforHeatReclaim());
  EXPECT_DOUBLE_EQ(0.0001, testObjectClone.waterFlowRate());
  EXPECT_DOUBLE_EQ(100.0, testObjectClone.waterPumpPower());
  EXPECT_DOUBLE_EQ(0.2, testObjectClone.fractionofPumpHeattoWater());
  EXPECT_DOUBLE_EQ(0.0, testObjectClone.onCycleParasiticElectricLoad());
  EXPECT_DOUBLE_EQ(0.0, testObjectClone.offCycleParasiticElectricLoad());
}

TEST_F(ModelFixture, CoilWaterHeatingDesuperheater_CloneModelWithCustomData) {
  Model model;
  ScheduleCompact schedule = ScheduleCompact(model);
  CoilWaterHeatingDesuperheater testObject = CoilWaterHeatingDesuperheater(model, schedule);
  RefrigerationCondenserAirCooled condenser = RefrigerationCondenserAirCooled(model);
  WaterHeaterMixed heatRejection = WaterHeaterMixed(model);
  CurveBiquadratic curve = CurveBiquadratic(model);

  EXPECT_TRUE(testObject.setDeadBandTemperatureDifference(20.0));
  EXPECT_TRUE(testObject.setRatedHeatReclaimRecoveryEfficiency(0.89));
  EXPECT_TRUE(testObject.setRatedInletWaterTemperature(999.0));
  EXPECT_TRUE(testObject.setRatedOutdoorAirTemperature(999.0));
  EXPECT_TRUE(testObject.setMaximumInletWaterTemperatureforHeatReclaim(999.0));
  EXPECT_TRUE(testObject.setWaterFlowRate(999.0));
  EXPECT_TRUE(testObject.setWaterPumpPower(999.0));
  EXPECT_TRUE(testObject.setFractionofPumpHeattoWater(0.99));
  EXPECT_TRUE(testObject.setOnCycleParasiticElectricLoad(999.0));
  EXPECT_TRUE(testObject.setOffCycleParasiticElectricLoad(999.0));
  EXPECT_TRUE(testObject.setHeatingSource(condenser));
  EXPECT_TRUE(testObject.addToHeatRejectionTarget(heatRejection));
  EXPECT_TRUE(testObject.setHeatReclaimEfficiencyFunctionofTemperatureCurve(curve));

  auto testObjectClone = testObject.clone(model).cast<CoilWaterHeatingDesuperheater>();

  EXPECT_DOUBLE_EQ(20.0, testObjectClone.deadBandTemperatureDifference());
  EXPECT_DOUBLE_EQ(0.89, testObjectClone.ratedHeatReclaimRecoveryEfficiency().get());
  EXPECT_DOUBLE_EQ(999.0, testObjectClone.ratedInletWaterTemperature());
  EXPECT_DOUBLE_EQ(999.0, testObjectClone.ratedOutdoorAirTemperature());
  EXPECT_DOUBLE_EQ(999.0, testObjectClone.maximumInletWaterTemperatureforHeatReclaim());
  EXPECT_DOUBLE_EQ(999.0, testObjectClone.waterFlowRate());
  EXPECT_DOUBLE_EQ(999.0, testObjectClone.waterPumpPower());
  EXPECT_DOUBLE_EQ(0.99, testObjectClone.fractionofPumpHeattoWater());
  EXPECT_DOUBLE_EQ(999.0, testObjectClone.onCycleParasiticElectricLoad());
  EXPECT_DOUBLE_EQ(999.0, testObjectClone.offCycleParasiticElectricLoad());
  EXPECT_FALSE(testObjectClone.heatingSource());
  EXPECT_FALSE(testObjectClone.heatRejectionTarget());
  ASSERT_TRUE(testObjectClone.heatReclaimEfficiencyFunctionofTemperatureCurve());
  EXPECT_EQ(curve, testObjectClone.heatReclaimEfficiencyFunctionofTemperatureCurve().get());
}

TEST_F(ModelFixture, CoilWaterHeatingDesuperheater_CloneTwoModelWithCustomData) {
  Model model;
  ScheduleCompact schedule = ScheduleCompact(model);
  CoilWaterHeatingDesuperheater testObject = CoilWaterHeatingDesuperheater(model, schedule);
  RefrigerationCondenserAirCooled condenser = RefrigerationCondenserAirCooled(model);
  WaterHeaterMixed heatRejection = WaterHeaterMixed(model);
  CurveBiquadratic curve = CurveBiquadratic(model);

  EXPECT_TRUE(testObject.setDeadBandTemperatureDifference(20.0));
  EXPECT_TRUE(testObject.setRatedHeatReclaimRecoveryEfficiency(0.89));
  EXPECT_TRUE(testObject.setRatedInletWaterTemperature(999.0));
  EXPECT_TRUE(testObject.setRatedOutdoorAirTemperature(999.0));
  EXPECT_TRUE(testObject.setMaximumInletWaterTemperatureforHeatReclaim(999.0));
  EXPECT_TRUE(testObject.setWaterFlowRate(999.0));
  EXPECT_TRUE(testObject.setWaterPumpPower(999.0));
  EXPECT_TRUE(testObject.setFractionofPumpHeattoWater(0.99));
  EXPECT_TRUE(testObject.setOnCycleParasiticElectricLoad(999.0));
  EXPECT_TRUE(testObject.setOffCycleParasiticElectricLoad(999.0));
  EXPECT_TRUE(testObject.setHeatingSource(condenser));
  EXPECT_TRUE(testObject.addToHeatRejectionTarget(heatRejection));
  EXPECT_TRUE(testObject.setHeatReclaimEfficiencyFunctionofTemperatureCurve(curve));

  auto testObjectClone = testObject.clone(model).cast<CoilWaterHeatingDesuperheater>();

  Model model2;
  auto testObjectClone2 = testObject.clone(model2).cast<CoilWaterHeatingDesuperheater>();

  EXPECT_DOUBLE_EQ(20.0, testObjectClone2.deadBandTemperatureDifference());
  EXPECT_DOUBLE_EQ(0.89, testObjectClone2.ratedHeatReclaimRecoveryEfficiency().get());
  EXPECT_DOUBLE_EQ(999.0, testObjectClone2.ratedInletWaterTemperature());
  EXPECT_DOUBLE_EQ(999.0, testObjectClone2.ratedOutdoorAirTemperature());
  EXPECT_DOUBLE_EQ(999.0, testObjectClone2.maximumInletWaterTemperatureforHeatReclaim());
  EXPECT_DOUBLE_EQ(999.0, testObjectClone2.waterFlowRate());
  EXPECT_DOUBLE_EQ(999.0, testObjectClone2.waterPumpPower());
  EXPECT_DOUBLE_EQ(0.99, testObjectClone2.fractionofPumpHeattoWater());
  EXPECT_DOUBLE_EQ(999.0, testObjectClone2.onCycleParasiticElectricLoad());
  EXPECT_DOUBLE_EQ(999.0, testObjectClone2.offCycleParasiticElectricLoad());
  EXPECT_FALSE(testObjectClone2.heatingSource());
  EXPECT_FALSE(testObjectClone2.heatRejectionTarget());
  ASSERT_TRUE(testObjectClone2.heatReclaimEfficiencyFunctionofTemperatureCurve());
  EXPECT_NE(curve, testObjectClone2.heatReclaimEfficiencyFunctionofTemperatureCurve().get());
  EXPECT_NE(testObjectClone2, testObjectClone);
  EXPECT_NE(testObjectClone, testObject);
}

TEST_F(ModelFixture, CoilWaterHeatingDesuperheater_HeatReclaimEfficiency) {
  Model model;
  ScheduleCompact schedule = ScheduleCompact(model);
  CoilWaterHeatingDesuperheater c = CoilWaterHeatingDesuperheater(model, schedule);

  // Ctor doesn't initialize it
  EXPECT_FALSE(c.ratedHeatReclaimRecoveryEfficiency());

  // No heating source type yet: max Rated Heat Reclaim Efficiency is per IDD at 0.9
  EXPECT_FALSE(c.heatingSource());
  EXPECT_TRUE(c.setRatedHeatReclaimRecoveryEfficiency(0.9));
  EXPECT_EQ(0.9, c.ratedHeatReclaimRecoveryEfficiency().get());
  EXPECT_FALSE(c.setRatedHeatReclaimRecoveryEfficiency(0.95));
  EXPECT_EQ(0.90, c.ratedHeatReclaimRecoveryEfficiency().get());

  // Adding a Heating Source of Refrigeration type: limit should be 0.9
  RefrigerationCondenserAirCooled condenser = RefrigerationCondenserAirCooled(model);
  EXPECT_TRUE(c.setHeatingSource(condenser));
  EXPECT_EQ(0.90, c.ratedHeatReclaimRecoveryEfficiency().get());
  // It should also enforce on the setter
  EXPECT_TRUE(c.setRatedHeatReclaimRecoveryEfficiency(0.85));
  EXPECT_EQ(0.85, c.ratedHeatReclaimRecoveryEfficiency().get());
  EXPECT_FALSE(c.setRatedHeatReclaimRecoveryEfficiency(0.95));
  EXPECT_EQ(0.85, c.ratedHeatReclaimRecoveryEfficiency().get());

  // Now add a Heating Source of non refrigeration type: limit should be 0.3, and it should reset to default of 0.25
  CoilCoolingDXMultiSpeed dxCoil = CoilCoolingDXMultiSpeed(model);
  EXPECT_TRUE(c.setHeatingSource(dxCoil));
  EXPECT_EQ(0.25, c.ratedHeatReclaimRecoveryEfficiency().get()) << c;
  // It should also enforce on the setter
  EXPECT_TRUE(c.setRatedHeatReclaimRecoveryEfficiency(0.3));
  EXPECT_EQ(0.3, c.ratedHeatReclaimRecoveryEfficiency().get());
  EXPECT_FALSE(c.setRatedHeatReclaimRecoveryEfficiency(0.35));
  EXPECT_EQ(0.3, c.ratedHeatReclaimRecoveryEfficiency().get());
}
