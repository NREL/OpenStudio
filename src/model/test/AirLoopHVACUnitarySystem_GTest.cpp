/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>

#include "ModelFixture.hpp"

#include "../AirLoopHVACUnitarySystem.hpp"
#include "../AirLoopHVACUnitarySystem_Impl.hpp"
#include "../Schedule.hpp"
#include "../Schedule_Impl.hpp"
#include "../Node.hpp"
#include "../Node_Impl.hpp"
#include "../Splitter.hpp"
#include "../Splitter_Impl.hpp"
#include "../AirLoopHVACZoneSplitter.hpp"
#include "../AirLoopHVACZoneSplitter_Impl.hpp"
#include "../HVACComponent.hpp"
#include "../HVACComponent_Impl.hpp"
#include "../FanVariableVolume.hpp"
#include "../FanVariableVolume_Impl.hpp"
#include "../FanConstantVolume.hpp"
#include "../FanConstantVolume_Impl.hpp"
#include "../FanOnOff.hpp"
#include "../FanOnOff_Impl.hpp"
#include "../CoilHeatingWater.hpp"
#include "../CoilHeatingGas.hpp"
#include "../CoilHeatingElectric.hpp"
#include "../CoilHeatingElectric_Impl.hpp"
#include "../CoilCoolingWater.hpp"
#include "../CoilCoolingWater_Impl.hpp"
#include "../CoilCoolingDXSingleSpeed.hpp"
#include "../CoilHeatingDXSingleSpeed.hpp"
#include "../CoilCoolingWaterToAirHeatPumpEquationFit.hpp"
#include "../CoilHeatingWaterToAirHeatPumpEquationFit.hpp"
#include "../CoilHeatingDesuperheater.hpp"
#include "../CoilCoolingDXTwoSpeed.hpp"
#include "../Curve.hpp"
#include "../CurveQuadratic.hpp"
#include "../CurveCubic.hpp"
#include "../CurveExponent.hpp"
#include "../CurveBiquadratic.hpp"
#include "../AirLoopHVAC.hpp"
#include "../AirLoopHVAC_Impl.hpp"
#include "../CoilHeatingDXVariableSpeed.hpp"
#include "../CoilHeatingDXMultiSpeed.hpp"
#include "../CoilCoolingDXVariableSpeed.hpp"
#include "../CoilCoolingDXMultiSpeed.hpp"

#include <utilities/idd/IddEnums.hxx>

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, AirLoopHVACUnitarySystem_DefaultConstructors) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  ASSERT_EXIT(
    {
      Model m;
      AirLoopHVACUnitarySystem testObject = AirLoopHVACUnitarySystem(m);

      exit(0);
    },
    ::testing::ExitedWithCode(0), "");

  Model m;
  AirLoopHVACUnitarySystem testObject = AirLoopHVACUnitarySystem(m);

  EXPECT_TRUE(testObject.noLoadSupplyAirFlowRateControlSetToLowSpeed());
  EXPECT_TRUE(testObject.setNoLoadSupplyAirFlowRateControlSetToLowSpeed(false));
  EXPECT_FALSE(testObject.noLoadSupplyAirFlowRateControlSetToLowSpeed());
}

TEST_F(ModelFixture, AirLoopHVACUnitarySystem_Remove) {
  Model m;
  Schedule s = m.alwaysOnDiscreteSchedule();
  AirLoopHVACUnitarySystem testObject = AirLoopHVACUnitarySystem(m);
  FanOnOff fan = FanOnOff(m, s);
  CoilHeatingElectric heatingCoil = CoilHeatingElectric(m, s);
  CoilHeatingElectric suppHeatingCoil = CoilHeatingElectric(m, s);
  CoilCoolingWater coolingCoil = CoilCoolingWater(m, s);

  testObject.setSupplyFan(fan);
  testObject.setCoolingCoil(coolingCoil);
  testObject.setHeatingCoil(heatingCoil);
  testObject.setSupplementalHeatingCoil(suppHeatingCoil);

  std::vector<AirLoopHVACUnitarySystem> unitarySystem = m.getConcreteModelObjects<AirLoopHVACUnitarySystem>();
  EXPECT_EQ(1, unitarySystem.size());

  std::vector<FanOnOff> fans = m.getConcreteModelObjects<FanOnOff>();
  EXPECT_EQ(1, fans.size());

  std::vector<CoilHeatingElectric> heatingCoils = m.getConcreteModelObjects<CoilHeatingElectric>();
  EXPECT_EQ(2, heatingCoils.size());

  std::vector<CoilCoolingWater> coolingCoils = m.getConcreteModelObjects<CoilCoolingWater>();
  EXPECT_EQ(1, coolingCoils.size());

  testObject.remove();

  unitarySystem = m.getConcreteModelObjects<AirLoopHVACUnitarySystem>();
  EXPECT_EQ(0, unitarySystem.size());

  fans = m.getConcreteModelObjects<FanOnOff>();
  EXPECT_EQ(0, fans.size());

  heatingCoils = m.getConcreteModelObjects<CoilHeatingElectric>();
  EXPECT_EQ(0, heatingCoils.size());

  coolingCoils = m.getConcreteModelObjects<CoilCoolingWater>();
  EXPECT_EQ(0, coolingCoils.size());
}

TEST_F(ModelFixture, AirLoopHVACUnitarySystem_CloneOneModelWithDefaultData) {
  Model m;
  Schedule s = m.alwaysOnDiscreteSchedule();
  AirLoopHVACUnitarySystem testObject = AirLoopHVACUnitarySystem(m);
  FanOnOff fan = FanOnOff(m, s);
  CoilHeatingElectric heatingCoil = CoilHeatingElectric(m, s);
  CoilHeatingElectric suppHeatingCoil = CoilHeatingElectric(m, s);
  CoilCoolingWater coolingCoil = CoilCoolingWater(m, s);

  testObject.setSupplyFan(fan);
  testObject.setCoolingCoil(coolingCoil);
  testObject.setHeatingCoil(heatingCoil);
  testObject.setSupplementalHeatingCoil(suppHeatingCoil);
  testObject.autosizeSupplyAirFlowRateWhenNoCoolingorHeatingisRequired();

  auto testObjectClone = testObject.clone(m).cast<AirLoopHVACUnitarySystem>();

  // EXPECT_EQ("Load", testObjectClone.controlType());
  EXPECT_EQ("None", testObjectClone.dehumidificationControlType());
  EXPECT_DOUBLE_EQ(1.0, testObjectClone.dXHeatingCoilSizingRatio());
  EXPECT_FALSE(testObjectClone.useDOASDXCoolingCoil());
  EXPECT_DOUBLE_EQ(2.0, testObjectClone.dOASDXCoolingCoilLeavingMinimumAirTemperature());
  EXPECT_EQ("SensibleOnlyLoadControl", testObjectClone.latentLoadControl());
  EXPECT_TRUE(testObjectClone.isSupplyAirFlowRateDuringCoolingOperationAutosized());
  EXPECT_TRUE(testObjectClone.isSupplyAirFlowRateDuringHeatingOperationAutosized());
  EXPECT_TRUE(testObjectClone.isSupplyAirFlowRateWhenNoCoolingorHeatingisRequiredAutosized());
  EXPECT_DOUBLE_EQ(80.0, testObjectClone.maximumSupplyAirTemperature().get());
  EXPECT_DOUBLE_EQ(21.0, testObjectClone.maximumOutdoorDryBulbTemperatureforSupplementalHeaterOperation());
  EXPECT_DOUBLE_EQ(0.0, testObjectClone.ancilliaryOnCycleElectricPower());
  EXPECT_DOUBLE_EQ(0.0, testObjectClone.ancilliaryOffCycleElectricPower());
  // EXPECT_DOUBLE_EQ(80.0, testObjectClone.maximumTemperatureforHeatRecovery());

  EXPECT_NE(testObject.supplyFan().get(), testObjectClone.supplyFan().get());
  EXPECT_NE(testObject.coolingCoil().get(), testObjectClone.coolingCoil().get());
  EXPECT_NE(testObject.heatingCoil().get(), testObjectClone.heatingCoil().get());
  EXPECT_NE(testObject.supplementalHeatingCoil().get(), testObjectClone.supplementalHeatingCoil().get());

  std::vector<AirLoopHVACUnitarySystem> unitarySystem = m.getConcreteModelObjects<AirLoopHVACUnitarySystem>();
  EXPECT_EQ(2, unitarySystem.size());

  std::vector<FanOnOff> fans = m.getConcreteModelObjects<FanOnOff>();
  EXPECT_EQ(2, fans.size());

  std::vector<CoilHeatingElectric> heatingCoils = m.getConcreteModelObjects<CoilHeatingElectric>();
  EXPECT_EQ(4, heatingCoils.size());

  std::vector<CoilCoolingWater> coolingCoils = m.getConcreteModelObjects<CoilCoolingWater>();
  EXPECT_EQ(2, coolingCoils.size());
}

TEST_F(ModelFixture, AirLoopHVACUnitarySystem_CloneOneModelWithCustomData) {
  Model m;
  Schedule s = m.alwaysOnDiscreteSchedule();
  AirLoopHVACUnitarySystem testObject = AirLoopHVACUnitarySystem(m);
  FanOnOff fan = FanOnOff(m, s);
  CoilHeatingElectric heatingCoil = CoilHeatingElectric(m, s);
  CoilHeatingElectric suppHeatingCoil = CoilHeatingElectric(m, s);
  CoilCoolingWater coolingCoil = CoilCoolingWater(m, s);

  testObject.setSupplyFan(fan);
  testObject.setCoolingCoil(coolingCoil);
  testObject.setHeatingCoil(heatingCoil);
  testObject.setSupplementalHeatingCoil(suppHeatingCoil);

  // testObject.setControlType("SetPoint");
  testObject.setDehumidificationControlType("CoolReheat");
  testObject.setFanPlacement("BlowThrough");
  testObject.setDXHeatingCoilSizingRatio(999.0);
  testObject.setUseDOASDXCoolingCoil(true);

  testObject.resetDOASDXCoolingCoilLeavingMinimumAirTemperature();
  EXPECT_TRUE(testObject.isDOASDXCoolingCoilLeavingMinimumAirTemperatureDefaulted());
  EXPECT_FALSE(testObject.isDOASDXCoolingCoilLeavingMinimumAirTemperatureAutosized());
  testObject.autosizeDOASDXCoolingCoilLeavingMinimumAirTemperature();
  EXPECT_FALSE(testObject.isDOASDXCoolingCoilLeavingMinimumAirTemperatureDefaulted());
  EXPECT_TRUE(testObject.isDOASDXCoolingCoilLeavingMinimumAirTemperatureAutosized());
  testObject.setDOASDXCoolingCoilLeavingMinimumAirTemperature(7.0);
  EXPECT_FALSE(testObject.isDOASDXCoolingCoilLeavingMinimumAirTemperatureDefaulted());
  EXPECT_FALSE(testObject.isDOASDXCoolingCoilLeavingMinimumAirTemperatureAutosized());

  testObject.setLatentLoadControl("LatentWithSensibleLoadControl");
  testObject.autosizeSupplyAirFlowRateDuringCoolingOperation();
  testObject.autosizeSupplyAirFlowRateDuringHeatingOperation();
  testObject.autosizeSupplyAirFlowRateWhenNoCoolingorHeatingisRequired();
  testObject.autosizeMaximumSupplyAirTemperature();
  testObject.setMaximumOutdoorDryBulbTemperatureforSupplementalHeaterOperation(999.0);
  testObject.setAncilliaryOnCycleElectricPower(999.0);
  testObject.setAncilliaryOffCycleElectricPower(999.0);
  // testObject.setMaximumTemperatureforHeatRecovery(100.0);

  auto testObjectClone = testObject.clone(m).cast<AirLoopHVACUnitarySystem>();

  // EXPECT_EQ("SetPoint", testObjectClone.controlType());
  EXPECT_EQ("CoolReheat", testObjectClone.dehumidificationControlType());
  EXPECT_EQ("BlowThrough", testObjectClone.fanPlacement().get());
  EXPECT_DOUBLE_EQ(999.0, testObjectClone.dXHeatingCoilSizingRatio());
  EXPECT_TRUE(testObjectClone.useDOASDXCoolingCoil());
  EXPECT_DOUBLE_EQ(7.0, testObjectClone.dOASDXCoolingCoilLeavingMinimumAirTemperature());
  EXPECT_EQ("LatentWithSensibleLoadControl", testObjectClone.latentLoadControl());
  EXPECT_TRUE(testObjectClone.isSupplyAirFlowRateDuringCoolingOperationAutosized());
  EXPECT_TRUE(testObjectClone.isSupplyAirFlowRateDuringHeatingOperationAutosized());
  EXPECT_TRUE(testObjectClone.isSupplyAirFlowRateWhenNoCoolingorHeatingisRequiredAutosized());
  EXPECT_TRUE(testObjectClone.isMaximumSupplyAirTemperatureAutosized());
  EXPECT_DOUBLE_EQ(999.0, testObjectClone.maximumOutdoorDryBulbTemperatureforSupplementalHeaterOperation());
  EXPECT_DOUBLE_EQ(999.0, testObjectClone.ancilliaryOnCycleElectricPower());
  EXPECT_DOUBLE_EQ(999.0, testObjectClone.ancilliaryOffCycleElectricPower());
  // EXPECT_DOUBLE_EQ(100.0, testObjectClone.maximumTemperatureforHeatRecovery());

  EXPECT_NE(testObject.supplyFan().get(), testObjectClone.supplyFan().get());
  EXPECT_NE(testObject.coolingCoil().get(), testObjectClone.coolingCoil().get());
  EXPECT_NE(testObject.heatingCoil().get(), testObjectClone.heatingCoil().get());
  EXPECT_NE(testObject.supplementalHeatingCoil().get(), testObjectClone.supplementalHeatingCoil().get());

  std::vector<AirLoopHVACUnitarySystem> unitarySystem = m.getConcreteModelObjects<AirLoopHVACUnitarySystem>();
  EXPECT_EQ(2, unitarySystem.size());

  std::vector<FanOnOff> fans = m.getConcreteModelObjects<FanOnOff>();
  EXPECT_EQ(2, fans.size());

  std::vector<CoilHeatingElectric> heatingCoils = m.getConcreteModelObjects<CoilHeatingElectric>();
  EXPECT_EQ(4, heatingCoils.size());

  std::vector<CoilCoolingWater> coolingCoils = m.getConcreteModelObjects<CoilCoolingWater>();
  EXPECT_EQ(2, coolingCoils.size());
}

TEST_F(ModelFixture, AirLoopHVACUnitarySystem_CloneTwoModelsWithCustomData) {
  Model m;
  Schedule s = m.alwaysOnDiscreteSchedule();
  AirLoopHVACUnitarySystem testObject = AirLoopHVACUnitarySystem(m);
  FanOnOff fan = FanOnOff(m, s);
  CoilHeatingElectric heatingCoil = CoilHeatingElectric(m, s);
  CoilHeatingElectric suppHeatingCoil = CoilHeatingElectric(m, s);
  CoilCoolingWater coolingCoil = CoilCoolingWater(m, s);

  testObject.setSupplyFan(fan);
  testObject.setCoolingCoil(coolingCoil);
  testObject.setHeatingCoil(heatingCoil);
  testObject.setSupplementalHeatingCoil(suppHeatingCoil);

  auto testObjectClone = testObject.clone(m).cast<AirLoopHVACUnitarySystem>();

  EXPECT_NE(testObject.supplyFan().get(), testObjectClone.supplyFan().get());
  EXPECT_NE(testObject.coolingCoil().get(), testObjectClone.coolingCoil().get());
  EXPECT_NE(testObject.heatingCoil().get(), testObjectClone.heatingCoil().get());
  EXPECT_NE(testObject.supplementalHeatingCoil().get(), testObjectClone.supplementalHeatingCoil().get());

  std::vector<AirLoopHVACUnitarySystem> unitarySystem = m.getConcreteModelObjects<AirLoopHVACUnitarySystem>();
  EXPECT_EQ(2, unitarySystem.size());

  std::vector<FanOnOff> fans = m.getConcreteModelObjects<FanOnOff>();
  EXPECT_EQ(2, fans.size());

  std::vector<CoilHeatingElectric> heatingCoils = m.getConcreteModelObjects<CoilHeatingElectric>();
  EXPECT_EQ(4, heatingCoils.size());

  std::vector<CoilCoolingWater> coolingCoils = m.getConcreteModelObjects<CoilCoolingWater>();
  EXPECT_EQ(2, coolingCoils.size());

  Model m2;
  auto testObjectClone2 = testObject.clone(m2).cast<AirLoopHVACUnitarySystem>();

  unitarySystem = m2.getConcreteModelObjects<AirLoopHVACUnitarySystem>();
  EXPECT_EQ(1, unitarySystem.size());

  fans = m2.getConcreteModelObjects<FanOnOff>();
  EXPECT_EQ(1, fans.size());

  heatingCoils = m2.getConcreteModelObjects<CoilHeatingElectric>();
  EXPECT_EQ(2, heatingCoils.size());

  coolingCoils = m2.getConcreteModelObjects<CoilCoolingWater>();
  EXPECT_EQ(1, coolingCoils.size());
}

TEST_F(ModelFixture, AirLoopHVACUnitarySystem_addToNode) {
  Model m;
  AirLoopHVACUnitarySystem testObject = AirLoopHVACUnitarySystem(m);
  AirLoopHVAC airLoop(m);

  Node supplyOutletNode = airLoop.supplyOutletNode();

  EXPECT_TRUE(testObject.addToNode(supplyOutletNode));

  EXPECT_EQ((unsigned)3, airLoop.supplyComponents().size());

  EXPECT_TRUE(testObject.inletPort());
  EXPECT_TRUE(testObject.outletPort());
}

TEST_F(ModelFixture, AirLoopHVACUnitarySystem_AddToNodeTwoSameObjects) {
  Model m;
  AirLoopHVACUnitarySystem testObject = AirLoopHVACUnitarySystem(m);
  AirLoopHVAC airLoop(m);

  Node supplyOutletNode = airLoop.supplyOutletNode();
  testObject.addToNode(supplyOutletNode);
  supplyOutletNode = airLoop.supplyOutletNode();
  EXPECT_FALSE(testObject.addToNode(supplyOutletNode));

  EXPECT_TRUE(testObject.inletPort());
  EXPECT_TRUE(testObject.outletPort());
}

TEST_F(ModelFixture, AirLoopHVACUnitarySystem_AddToNodeAirLoopDemandSide) {
  Model m;
  AirLoopHVACUnitarySystem testObject = AirLoopHVACUnitarySystem(m);
  AirLoopHVAC airLoop(m);

  Node inletNode = airLoop.zoneSplitter().lastOutletModelObject()->cast<Node>();

  EXPECT_FALSE(testObject.addToNode(inletNode));

  EXPECT_EQ((unsigned)5, airLoop.demandComponents().size());
}

TEST_F(ModelFixture, AirLoopHVACUnitarySystem_AddToNodePlantLoop) {
  // As of right now you cannot add the unitary to a plant.
  // Some heat recovery configurations may enable it, but more likely
  // the inner components will be added to the plant.

  Model m;
  AirLoopHVACUnitarySystem testObject = AirLoopHVACUnitarySystem(m);
  PlantLoop plantLoop(m);

  EXPECT_EQ((unsigned)5, plantLoop.demandComponents().size());

  Node demandInletNode = plantLoop.demandSplitter().lastOutletModelObject()->cast<Node>();

  EXPECT_FALSE(testObject.addToNode(demandInletNode));
  EXPECT_FALSE(plantLoop.addDemandBranchForComponent(testObject));
  EXPECT_EQ((unsigned)5, plantLoop.demandComponents().size());

  Node supplyInletNode = plantLoop.supplySplitter().lastOutletModelObject()->cast<Node>();

  EXPECT_FALSE(testObject.addToNode(supplyInletNode));
  EXPECT_EQ((unsigned)5, plantLoop.supplyComponents().size());
  EXPECT_FALSE(plantLoop.addSupplyBranchForComponent(testObject));
  EXPECT_EQ((unsigned)5, plantLoop.supplyComponents().size());
  ASSERT_FALSE(testObject.plantLoop());
  // EXPECT_EQ(plantLoop, testObject.plantLoop().get());
}

TEST_F(ModelFixture, AirLoopHVACUnitarySystem_WaterHeatingCoilToPlant) {
  Model m;
  Schedule s = m.alwaysOnDiscreteSchedule();
  AirLoopHVACUnitarySystem testObject = AirLoopHVACUnitarySystem(m);
  FanOnOff fan = FanOnOff(m, s);
  CoilHeatingWater heatingCoil = CoilHeatingWater(m, s);
  CoilHeatingWater suppHeatingCoil = CoilHeatingWater(m, s);
  CoilCoolingWater coolingCoil = CoilCoolingWater(m, s);

  testObject.setSupplyFan(fan);
  testObject.setCoolingCoil(coolingCoil);
  testObject.setHeatingCoil(heatingCoil);
  testObject.setSupplementalHeatingCoil(suppHeatingCoil);
  PlantLoop plantLoop(m);

  EXPECT_TRUE(plantLoop.addDemandBranchForComponent(heatingCoil));
  EXPECT_TRUE(plantLoop.addDemandBranchForComponent(suppHeatingCoil));
  EXPECT_EQ((unsigned)10, plantLoop.demandComponents().size());
  EXPECT_NE((unsigned)7, plantLoop.demandComponents().size());

  testObject.remove();
  EXPECT_EQ((unsigned)5, plantLoop.demandComponents().size());
}

TEST_F(ModelFixture, AirLoopHVACUnitarySystem_RemoveWaterHeatingCoilFromPlant) {
  Model m;
  Schedule s = m.alwaysOnDiscreteSchedule();
  AirLoopHVACUnitarySystem testObject = AirLoopHVACUnitarySystem(m);
  FanOnOff fan = FanOnOff(m, s);
  CoilHeatingWater heatingCoil = CoilHeatingWater(m, s);
  CoilHeatingWater suppHeatingCoil = CoilHeatingWater(m, s);
  CoilCoolingWater coolingCoil = CoilCoolingWater(m, s);

  testObject.setSupplyFan(fan);
  testObject.setCoolingCoil(coolingCoil);
  testObject.setHeatingCoil(heatingCoil);
  testObject.setSupplementalHeatingCoil(suppHeatingCoil);
  PlantLoop plantLoop(m);

  EXPECT_TRUE(plantLoop.addDemandBranchForComponent(heatingCoil));
  EXPECT_TRUE(plantLoop.addDemandBranchForComponent(suppHeatingCoil));
  EXPECT_TRUE(plantLoop.removeDemandBranchWithComponent(heatingCoil));
  EXPECT_TRUE(plantLoop.removeDemandBranchWithComponent(suppHeatingCoil));
  EXPECT_EQ((unsigned)5, plantLoop.demandComponents().size());
  EXPECT_NE((unsigned)7, plantLoop.demandComponents().size());
}

TEST_F(ModelFixture, AirLoopHVACUnitarySystem_containingHVACComponent) {
  Model m;
  Schedule s = m.alwaysOnDiscreteSchedule();
  CurveBiquadratic c1(m);
  CurveQuadratic c2(m);
  CurveBiquadratic c3(m);
  CurveQuadratic c4(m);
  CurveQuadratic c5(m);
  CurveBiquadratic c6(m);
  CurveBiquadratic c7(m);

  FanOnOff fanOnOff = FanOnOff(m, s);
  CoilHeatingWaterToAirHeatPumpEquationFit coilHeatingWaterToAirHeatPumpEquationFit(m);
  CoilCoolingWaterToAirHeatPumpEquationFit coilCoolingWaterToAirHeatPumpEquationFit(m);
  CoilHeatingElectric coilHeatingElectricSupp(m, s);

  AirLoopHVACUnitarySystem testObject(m);

  EXPECT_TRUE(testObject.setSupplyFan(fanOnOff));
  EXPECT_TRUE(testObject.setCoolingCoil(coilCoolingWaterToAirHeatPumpEquationFit));
  EXPECT_TRUE(testObject.setHeatingCoil(coilHeatingWaterToAirHeatPumpEquationFit));
  EXPECT_TRUE(testObject.setSupplementalHeatingCoil(coilHeatingElectricSupp));

  boost::optional<HVACComponent> component = fanOnOff.containingHVACComponent();
  ASSERT_TRUE(component);
  EXPECT_EQ(*component, testObject);
  component = coilHeatingWaterToAirHeatPumpEquationFit.containingHVACComponent();
  ASSERT_TRUE(component);
  EXPECT_EQ(*component, testObject);
  component = coilCoolingWaterToAirHeatPumpEquationFit.containingHVACComponent();
  ASSERT_TRUE(component);
  EXPECT_EQ(*component, testObject);
  component = coilHeatingElectricSupp.containingHVACComponent();
  ASSERT_TRUE(component);
  EXPECT_EQ(*component, testObject);

  FanConstantVolume fanConstantVolume = FanConstantVolume(m, s);
  CoilHeatingWater coilHeatingWater(m, s);
  CoilCoolingWater coilCoolingWater(m, s);
  CoilHeatingGas coilHeatingGasSupp(m, s);

  EXPECT_TRUE(testObject.setSupplyFan(fanConstantVolume));
  EXPECT_TRUE(testObject.setCoolingCoil(coilCoolingWater));
  EXPECT_TRUE(testObject.setHeatingCoil(coilHeatingWater));
  EXPECT_TRUE(testObject.setSupplementalHeatingCoil(coilHeatingGasSupp));

  component = fanConstantVolume.containingHVACComponent();
  ASSERT_TRUE(component);
  EXPECT_EQ(*component, testObject);
  component = coilHeatingWater.containingHVACComponent();
  ASSERT_TRUE(component);
  EXPECT_EQ(*component, testObject);
  component = coilCoolingWater.containingHVACComponent();
  ASSERT_TRUE(component);
  EXPECT_EQ(*component, testObject);
  component = coilHeatingGasSupp.containingHVACComponent();
  ASSERT_TRUE(component);
  EXPECT_EQ(*component, testObject);

  FanVariableVolume fanVariableVolume = FanVariableVolume(m, s);
  CoilHeatingDXSingleSpeed coilHeatingDXSingleSpeed(m, s, c1, c2, c3, c4, c5);
  CoilCoolingDXSingleSpeed coilCoolingDXSingleSpeed(m, s, c1, c2, c3, c4, c5);
  CoilHeatingWater coilHeatingWaterSupp(m, s);

  EXPECT_TRUE(testObject.setSupplyFan(fanVariableVolume));
  EXPECT_TRUE(testObject.setCoolingCoil(coilCoolingDXSingleSpeed));
  EXPECT_TRUE(testObject.setHeatingCoil(coilHeatingDXSingleSpeed));
  EXPECT_TRUE(testObject.setSupplementalHeatingCoil(coilHeatingWaterSupp));

  component = fanVariableVolume.containingHVACComponent();
  ASSERT_TRUE(component);
  EXPECT_EQ(*component, testObject);
  component = coilHeatingDXSingleSpeed.containingHVACComponent();
  ASSERT_TRUE(component);
  EXPECT_EQ(*component, testObject);
  component = coilCoolingDXSingleSpeed.containingHVACComponent();
  ASSERT_TRUE(component);
  EXPECT_EQ(*component, testObject);
  component = coilHeatingWaterSupp.containingHVACComponent();
  ASSERT_TRUE(component);
  EXPECT_EQ(*component, testObject);

  CoilHeatingDesuperheater coilHeatingDesupeheater(m);
  CoilCoolingDXTwoSpeed coilCoolingDXTwoSpeed(m, s, c1, c2, c3, c4, c5, c6, c7);
  CoilHeatingDesuperheater coilHeatingDesupeheaterSupp(m);

  EXPECT_TRUE(testObject.setSupplyFan(fanOnOff));
  EXPECT_TRUE(testObject.setCoolingCoil(coilCoolingDXTwoSpeed));
  EXPECT_TRUE(testObject.setHeatingCoil(coilHeatingDesupeheater));
  EXPECT_TRUE(testObject.setSupplementalHeatingCoil(coilHeatingDesupeheaterSupp));

  component = coilHeatingDesupeheater.containingHVACComponent();
  ASSERT_TRUE(component);
  EXPECT_EQ(*component, testObject);
  component = coilCoolingDXTwoSpeed.containingHVACComponent();
  ASSERT_TRUE(component);
  EXPECT_EQ(*component, testObject);
  component = coilHeatingDesupeheaterSupp.containingHVACComponent();
  ASSERT_TRUE(component);
  EXPECT_EQ(*component, testObject);

  CoilHeatingElectric coilHeatingElectric(m, s);

  EXPECT_TRUE(testObject.setSupplyFan(fanOnOff));
  EXPECT_TRUE(testObject.setCoolingCoil(coilCoolingWater));
  EXPECT_TRUE(testObject.setHeatingCoil(coilHeatingElectric));
  EXPECT_TRUE(testObject.setSupplementalHeatingCoil(coilHeatingWaterSupp));

  component = coilHeatingElectric.containingHVACComponent();
  ASSERT_TRUE(component);
  EXPECT_EQ(*component, testObject);

  CoilHeatingGas coilHeatingGas(m, s);

  EXPECT_TRUE(testObject.setSupplyFan(fanOnOff));
  EXPECT_TRUE(testObject.setCoolingCoil(coilCoolingWater));
  EXPECT_TRUE(testObject.setHeatingCoil(coilHeatingGas));
  EXPECT_TRUE(testObject.setSupplementalHeatingCoil(coilHeatingWaterSupp));

  component = coilHeatingGas.containingHVACComponent();
  ASSERT_TRUE(component);
  EXPECT_EQ(*component, testObject);

  CoilHeatingDXVariableSpeed coilHeatingDXVariableSpeed(m);
  CoilCoolingDXVariableSpeed coilCoolingDXVariableSpeed(m);

  EXPECT_TRUE(testObject.setSupplyFan(fanOnOff));
  EXPECT_TRUE(testObject.setCoolingCoil(coilCoolingDXVariableSpeed));
  EXPECT_TRUE(testObject.setHeatingCoil(coilHeatingDXVariableSpeed));
  EXPECT_TRUE(testObject.setSupplementalHeatingCoil(coilHeatingWaterSupp));

  component = coilHeatingDXVariableSpeed.containingHVACComponent();
  ASSERT_TRUE(component);
  EXPECT_EQ(*component, testObject);
  component = coilCoolingDXVariableSpeed.containingHVACComponent();
  ASSERT_TRUE(component);
  EXPECT_EQ(*component, testObject);

  CoilHeatingDXMultiSpeed coilHeatingDXMultiSpeed(m);
  CoilCoolingDXMultiSpeed coilCoolingDXMultiSpeed(m);

  EXPECT_TRUE(testObject.setSupplyFan(fanOnOff));
  EXPECT_TRUE(testObject.setCoolingCoil(coilCoolingDXMultiSpeed));
  EXPECT_TRUE(testObject.setHeatingCoil(coilHeatingDXMultiSpeed));
  EXPECT_TRUE(testObject.setSupplementalHeatingCoil(coilHeatingWaterSupp));

  component = coilHeatingDXMultiSpeed.containingHVACComponent();
  ASSERT_TRUE(component);
  EXPECT_EQ(*component, testObject);
  component = coilCoolingDXMultiSpeed.containingHVACComponent();
  ASSERT_TRUE(component);
  EXPECT_EQ(*component, testObject);
}

TEST_F(ModelFixture, AirLoopHVACUnitarySystem_ControlType) {
  Model m;
  AirLoopHVACUnitarySystem a = AirLoopHVACUnitarySystem(m);

  // Tests constructor
  EXPECT_EQ("Load", a.controlType());
  EXPECT_FALSE(a.isControlTypeDefaulted());

  ASSERT_TRUE(a.setControlType("Setpoint"));
  ASSERT_FALSE(a.isControlTypeDefaulted());

  a.resetControlType();
  ASSERT_TRUE(a.isControlTypeDefaulted());
  ASSERT_EQ("Load", a.controlType());
}

TEST_F(ModelFixture, AirLoopHVACUnitarySystem_clone_Nodes) {

  // Test for #4335 - when cloning a UnitarySystem, the clone should have its inlet/outlet nodes cleared out
  Model m;
  AirLoopHVACUnitarySystem unitary = AirLoopHVACUnitarySystem(m);
  AirLoopHVAC a(m);

  Node supplyOutletNode = a.supplyOutletNode();
  EXPECT_TRUE(unitary.addToNode(supplyOutletNode));

  auto unitaryClone = unitary.clone(m).cast<AirLoopHVACUnitarySystem>();

  EXPECT_TRUE(unitary.inletNode());
  EXPECT_TRUE(unitary.outletNode());
  EXPECT_FALSE(unitaryClone.inletNode());
  EXPECT_FALSE(unitaryClone.outletNode());
  EXPECT_FALSE(unitaryClone.getString(unitaryClone.inletPort(), true, true));
  EXPECT_FALSE(unitaryClone.getString(unitaryClone.outletPort(), true, true));
}

TEST_F(ModelFixture, AirLoopHVACUnitarySystem_cloneAirLoopHVAC_Nodes) {

  // Test for #4335 -  when cloning an AirLoopHVAC with a unitary sys, the original one should still be connected
  Model m;
  AirLoopHVACUnitarySystem unitary = AirLoopHVACUnitarySystem(m);
  AirLoopHVAC a(m);

  Node supplyOutletNode = a.supplyOutletNode();
  EXPECT_TRUE(unitary.addToNode(supplyOutletNode));

  auto aClone = a.clone(m).cast<AirLoopHVAC>();

  auto unitarys = m.getConcreteModelObjects<AirLoopHVACUnitarySystem>();
  ASSERT_EQ(2, unitarys.size());
  auto unitaryClone = (unitarys[0] == unitary) ? unitarys[1] : unitarys[0];

  EXPECT_TRUE(unitaryClone.inletNode());
  EXPECT_TRUE(unitaryClone.outletNode());
  EXPECT_TRUE(unitary.inletNode());
  EXPECT_TRUE(unitary.outletNode());
  EXPECT_NE(unitary.inletNode().get(), unitaryClone.inletNode().get());
  EXPECT_NE(unitary.outletNode().get(), unitaryClone.outletNode().get());

  ASSERT_EQ(1u, aClone.supplyComponents(openstudio::IddObjectType::OS_AirLoopHVAC_UnitarySystem).size());
  EXPECT_EQ(unitaryClone, aClone.supplyComponents(openstudio::IddObjectType::OS_AirLoopHVAC_UnitarySystem)[0]);
  ASSERT_EQ(1u, a.supplyComponents(openstudio::IddObjectType::OS_AirLoopHVAC_UnitarySystem).size());
  EXPECT_EQ(unitary, a.supplyComponents(openstudio::IddObjectType::OS_AirLoopHVAC_UnitarySystem)[0]);
}

TEST_F(ModelFixture, AirLoopHVACUnitarySystem_SupplyAirFlowRateMethodDuringOperation_Cooling) {
  // Test for #4695 - AirLoopHVACUnitarySystem: Supply Air Flow Rate Method During <XXX> Operation should be set via related setters/autosize

  Model m;
  AirLoopHVACUnitarySystem unitary = AirLoopHVACUnitarySystem(m);

  EXPECT_EQ("None", unitary.supplyAirFlowRateMethodDuringCoolingOperation());

  EXPECT_FALSE(unitary.isSupplyAirFlowRateDuringCoolingOperationAutosized());
  EXPECT_FALSE(unitary.supplyAirFlowRateDuringCoolingOperation());
  EXPECT_FALSE(unitary.supplyAirFlowRatePerFloorAreaDuringCoolingOperation());
  EXPECT_FALSE(unitary.fractionofAutosizedDesignCoolingSupplyAirFlowRate());
  EXPECT_FALSE(unitary.designSupplyAirFlowRatePerUnitofCapacityDuringCoolingOperation());

  // Setting a coil changes things
  CoilCoolingDXSingleSpeed cc(m);
  EXPECT_TRUE(unitary.setCoolingCoil(cc));
  EXPECT_EQ("SupplyAirFlowRate", unitary.supplyAirFlowRateMethodDuringCoolingOperation());
  EXPECT_TRUE(unitary.isSupplyAirFlowRateDuringCoolingOperationAutosized());
  EXPECT_FALSE(unitary.supplyAirFlowRateDuringCoolingOperation());
  EXPECT_FALSE(unitary.supplyAirFlowRatePerFloorAreaDuringCoolingOperation());
  EXPECT_FALSE(unitary.fractionofAutosizedDesignCoolingSupplyAirFlowRate());
  EXPECT_FALSE(unitary.designSupplyAirFlowRatePerUnitofCapacityDuringCoolingOperation());

  unitary.resetCoolingCoil();
  EXPECT_EQ("None", unitary.supplyAirFlowRateMethodDuringCoolingOperation());
  EXPECT_FALSE(unitary.isSupplyAirFlowRateDuringCoolingOperationAutosized());
  EXPECT_FALSE(unitary.supplyAirFlowRateDuringCoolingOperation());
  EXPECT_FALSE(unitary.supplyAirFlowRatePerFloorAreaDuringCoolingOperation());
  EXPECT_FALSE(unitary.fractionofAutosizedDesignCoolingSupplyAirFlowRate());
  EXPECT_FALSE(unitary.designSupplyAirFlowRatePerUnitofCapacityDuringCoolingOperation());
  // Put it back
  EXPECT_TRUE(unitary.setCoolingCoil(cc));

  // SupplyAirFlowRate
  EXPECT_TRUE(unitary.setSupplyAirFlowRateDuringCoolingOperation(1.0));
  EXPECT_EQ("SupplyAirFlowRate", unitary.supplyAirFlowRateMethodDuringCoolingOperation());
  EXPECT_FALSE(unitary.isSupplyAirFlowRateDuringCoolingOperationAutosized());
  ASSERT_TRUE(unitary.supplyAirFlowRateDuringCoolingOperation());
  EXPECT_EQ(1.0, unitary.supplyAirFlowRateDuringCoolingOperation().get());
  EXPECT_FALSE(unitary.supplyAirFlowRatePerFloorAreaDuringCoolingOperation());
  EXPECT_FALSE(unitary.fractionofAutosizedDesignCoolingSupplyAirFlowRate());
  EXPECT_FALSE(unitary.designSupplyAirFlowRatePerUnitofCapacityDuringCoolingOperation());

  // FlowPerFloorArea
  EXPECT_TRUE(unitary.setSupplyAirFlowRatePerFloorAreaDuringCoolingOperation(1.0));
  EXPECT_EQ("FlowPerFloorArea", unitary.supplyAirFlowRateMethodDuringCoolingOperation());
  EXPECT_FALSE(unitary.isSupplyAirFlowRateDuringCoolingOperationAutosized());
  EXPECT_FALSE(unitary.supplyAirFlowRateDuringCoolingOperation());
  ASSERT_TRUE(unitary.supplyAirFlowRatePerFloorAreaDuringCoolingOperation());
  EXPECT_EQ(1.0, unitary.supplyAirFlowRatePerFloorAreaDuringCoolingOperation().get());
  EXPECT_FALSE(unitary.fractionofAutosizedDesignCoolingSupplyAirFlowRate());
  EXPECT_FALSE(unitary.designSupplyAirFlowRatePerUnitofCapacityDuringCoolingOperation());

  // FractionOfAutosizedCoolingValue
  EXPECT_TRUE(unitary.setFractionofAutosizedDesignCoolingSupplyAirFlowRate(1.0));
  EXPECT_EQ("FractionOfAutosizedCoolingValue", unitary.supplyAirFlowRateMethodDuringCoolingOperation());
  EXPECT_FALSE(unitary.isSupplyAirFlowRateDuringCoolingOperationAutosized());
  EXPECT_FALSE(unitary.supplyAirFlowRateDuringCoolingOperation());
  EXPECT_FALSE(unitary.supplyAirFlowRatePerFloorAreaDuringCoolingOperation());
  ASSERT_TRUE(unitary.fractionofAutosizedDesignCoolingSupplyAirFlowRate());
  EXPECT_EQ(1.0, unitary.fractionofAutosizedDesignCoolingSupplyAirFlowRate().get());
  EXPECT_FALSE(unitary.designSupplyAirFlowRatePerUnitofCapacityDuringCoolingOperation());

  // FlowPerCoolingCapacity
  EXPECT_TRUE(unitary.setDesignSupplyAirFlowRatePerUnitofCapacityDuringCoolingOperation(1.0));
  EXPECT_EQ("FlowPerCoolingCapacity", unitary.supplyAirFlowRateMethodDuringCoolingOperation());
  EXPECT_FALSE(unitary.isSupplyAirFlowRateDuringCoolingOperationAutosized());
  EXPECT_FALSE(unitary.supplyAirFlowRateDuringCoolingOperation());
  EXPECT_FALSE(unitary.supplyAirFlowRatePerFloorAreaDuringCoolingOperation());
  EXPECT_FALSE(unitary.fractionofAutosizedDesignCoolingSupplyAirFlowRate());
  ASSERT_TRUE(unitary.designSupplyAirFlowRatePerUnitofCapacityDuringCoolingOperation());
  EXPECT_EQ(1.0, unitary.designSupplyAirFlowRatePerUnitofCapacityDuringCoolingOperation().get());
}

TEST_F(ModelFixture, AirLoopHVACUnitarySystem_SupplyAirFlowRateMethodDuringOperation_Heating) {
  // Test for #4695 - AirLoopHVACUnitarySystem: Supply Air Flow Rate Method During <XXX> Operation should be set via related setters/autosize

  Model m;
  AirLoopHVACUnitarySystem unitary = AirLoopHVACUnitarySystem(m);

  EXPECT_EQ("None", unitary.supplyAirFlowRateMethodDuringHeatingOperation());

  EXPECT_FALSE(unitary.isSupplyAirFlowRateDuringHeatingOperationAutosized());
  EXPECT_FALSE(unitary.supplyAirFlowRateDuringHeatingOperation());
  EXPECT_FALSE(unitary.supplyAirFlowRatePerFloorAreaduringHeatingOperation());
  EXPECT_FALSE(unitary.fractionofAutosizedDesignHeatingSupplyAirFlowRate());
  EXPECT_FALSE(unitary.designSupplyAirFlowRatePerUnitofCapacityDuringHeatingOperation());

  // Setting a coil changes things
  CoilHeatingDXSingleSpeed hc(m);
  EXPECT_TRUE(unitary.setHeatingCoil(hc));
  EXPECT_EQ("SupplyAirFlowRate", unitary.supplyAirFlowRateMethodDuringHeatingOperation());
  EXPECT_TRUE(unitary.isSupplyAirFlowRateDuringHeatingOperationAutosized());
  EXPECT_FALSE(unitary.supplyAirFlowRateDuringHeatingOperation());
  EXPECT_FALSE(unitary.supplyAirFlowRatePerFloorAreaduringHeatingOperation());
  EXPECT_FALSE(unitary.fractionofAutosizedDesignHeatingSupplyAirFlowRate());
  EXPECT_FALSE(unitary.designSupplyAirFlowRatePerUnitofCapacityDuringHeatingOperation());

  unitary.resetHeatingCoil();
  EXPECT_EQ("None", unitary.supplyAirFlowRateMethodDuringHeatingOperation());
  EXPECT_FALSE(unitary.isSupplyAirFlowRateDuringHeatingOperationAutosized());
  EXPECT_FALSE(unitary.supplyAirFlowRateDuringHeatingOperation());
  EXPECT_FALSE(unitary.supplyAirFlowRatePerFloorAreaduringHeatingOperation());
  EXPECT_FALSE(unitary.fractionofAutosizedDesignHeatingSupplyAirFlowRate());
  EXPECT_FALSE(unitary.designSupplyAirFlowRatePerUnitofCapacityDuringHeatingOperation());
  // Put it back
  EXPECT_TRUE(unitary.setHeatingCoil(hc));

  // SupplyAirFlowRate
  EXPECT_TRUE(unitary.setSupplyAirFlowRateDuringHeatingOperation(1.0));
  EXPECT_EQ("SupplyAirFlowRate", unitary.supplyAirFlowRateMethodDuringHeatingOperation());
  EXPECT_FALSE(unitary.isSupplyAirFlowRateDuringHeatingOperationAutosized());
  ASSERT_TRUE(unitary.supplyAirFlowRateDuringHeatingOperation());
  EXPECT_EQ(1.0, unitary.supplyAirFlowRateDuringHeatingOperation().get());
  EXPECT_FALSE(unitary.supplyAirFlowRatePerFloorAreaduringHeatingOperation());
  EXPECT_FALSE(unitary.fractionofAutosizedDesignHeatingSupplyAirFlowRate());
  EXPECT_FALSE(unitary.designSupplyAirFlowRatePerUnitofCapacityDuringHeatingOperation());

  // FlowPerFloorArea
  EXPECT_TRUE(unitary.setSupplyAirFlowRatePerFloorAreaduringHeatingOperation(1.0));
  EXPECT_EQ("FlowPerFloorArea", unitary.supplyAirFlowRateMethodDuringHeatingOperation());
  EXPECT_FALSE(unitary.isSupplyAirFlowRateDuringHeatingOperationAutosized());
  EXPECT_FALSE(unitary.supplyAirFlowRateDuringHeatingOperation());
  ASSERT_TRUE(unitary.supplyAirFlowRatePerFloorAreaduringHeatingOperation());
  EXPECT_EQ(1.0, unitary.supplyAirFlowRatePerFloorAreaduringHeatingOperation().get());
  EXPECT_FALSE(unitary.fractionofAutosizedDesignHeatingSupplyAirFlowRate());
  EXPECT_FALSE(unitary.designSupplyAirFlowRatePerUnitofCapacityDuringHeatingOperation());

  // FractionOfAutosizedHeatingValue
  EXPECT_TRUE(unitary.setFractionofAutosizedDesignHeatingSupplyAirFlowRate(1.0));
  EXPECT_EQ("FractionOfAutosizedHeatingValue", unitary.supplyAirFlowRateMethodDuringHeatingOperation());
  EXPECT_FALSE(unitary.isSupplyAirFlowRateDuringHeatingOperationAutosized());
  EXPECT_FALSE(unitary.supplyAirFlowRateDuringHeatingOperation());
  EXPECT_FALSE(unitary.supplyAirFlowRatePerFloorAreaduringHeatingOperation());
  ASSERT_TRUE(unitary.fractionofAutosizedDesignHeatingSupplyAirFlowRate());
  EXPECT_EQ(1.0, unitary.fractionofAutosizedDesignHeatingSupplyAirFlowRate().get());
  EXPECT_FALSE(unitary.designSupplyAirFlowRatePerUnitofCapacityDuringHeatingOperation());

  // FlowPerHeatingCapacity
  EXPECT_TRUE(unitary.setDesignSupplyAirFlowRatePerUnitofCapacityDuringHeatingOperation(1.0));
  EXPECT_EQ("FlowPerHeatingCapacity", unitary.supplyAirFlowRateMethodDuringHeatingOperation());
  EXPECT_FALSE(unitary.isSupplyAirFlowRateDuringHeatingOperationAutosized());
  EXPECT_FALSE(unitary.supplyAirFlowRateDuringHeatingOperation());
  EXPECT_FALSE(unitary.supplyAirFlowRatePerFloorAreaduringHeatingOperation());
  EXPECT_FALSE(unitary.fractionofAutosizedDesignHeatingSupplyAirFlowRate());
  ASSERT_TRUE(unitary.designSupplyAirFlowRatePerUnitofCapacityDuringHeatingOperation());
  EXPECT_EQ(1.0, unitary.designSupplyAirFlowRatePerUnitofCapacityDuringHeatingOperation().get());
}

TEST_F(ModelFixture, AirLoopHVACUnitarySystem_SupplyAirFlowRateMethodDuringOperation_NoCoolHeat) {
  // Test for #4695 - AirLoopHVACUnitarySystem: Supply Air Flow Rate Method During <XXX> Operation should be set via related setters/autosize

  Model m;
  AirLoopHVACUnitarySystem unitary = AirLoopHVACUnitarySystem(m);
  CoilCoolingDXSingleSpeed cc(m);
  EXPECT_TRUE(unitary.setCoolingCoil(cc));
  CoilHeatingDXSingleSpeed hc(m);
  EXPECT_TRUE(unitary.setHeatingCoil(hc));

  EXPECT_EQ("None", unitary.supplyAirFlowRateMethodWhenNoCoolingorHeatingisRequired());
  EXPECT_FALSE(unitary.isSupplyAirFlowRateWhenNoCoolingorHeatingisRequiredAutosized());
  EXPECT_FALSE(unitary.supplyAirFlowRateWhenNoCoolingorHeatingisRequired());
  EXPECT_FALSE(unitary.supplyAirFlowRatePerFloorAreaWhenNoCoolingorHeatingisRequired());
  EXPECT_FALSE(unitary.fractionofAutosizedDesignCoolingSupplyAirFlowRateWhenNoCoolingorHeatingisRequired());
  EXPECT_FALSE(unitary.fractionofAutosizedDesignHeatingSupplyAirFlowRateWhenNoCoolingorHeatingisRequired());
  EXPECT_FALSE(unitary.designSupplyAirFlowRatePerUnitofCapacityDuringCoolingOperationWhenNoCoolingorHeatingisRequired());
  EXPECT_FALSE(unitary.designSupplyAirFlowRatePerUnitofCapacityDuringHeatingOperationWhenNoCoolingorHeatingisRequired());

  EXPECT_TRUE(unitary.setSupplyAirFlowRateWhenNoCoolingorHeatingisRequired(1.0));
  EXPECT_EQ("SupplyAirFlowRate", unitary.supplyAirFlowRateMethodWhenNoCoolingorHeatingisRequired());
  EXPECT_FALSE(unitary.isSupplyAirFlowRateWhenNoCoolingorHeatingisRequiredAutosized());
  ASSERT_TRUE(unitary.supplyAirFlowRateWhenNoCoolingorHeatingisRequired());
  EXPECT_EQ(1.0, unitary.supplyAirFlowRateWhenNoCoolingorHeatingisRequired().get());
  EXPECT_FALSE(unitary.supplyAirFlowRatePerFloorAreaWhenNoCoolingorHeatingisRequired());
  EXPECT_FALSE(unitary.fractionofAutosizedDesignCoolingSupplyAirFlowRateWhenNoCoolingorHeatingisRequired());
  EXPECT_FALSE(unitary.fractionofAutosizedDesignHeatingSupplyAirFlowRateWhenNoCoolingorHeatingisRequired());
  EXPECT_FALSE(unitary.designSupplyAirFlowRatePerUnitofCapacityDuringCoolingOperationWhenNoCoolingorHeatingisRequired());
  EXPECT_FALSE(unitary.designSupplyAirFlowRatePerUnitofCapacityDuringHeatingOperationWhenNoCoolingorHeatingisRequired());

  EXPECT_TRUE(unitary.setSupplyAirFlowRatePerFloorAreaWhenNoCoolingorHeatingisRequired(1.0));
  EXPECT_EQ("FlowPerFloorArea", unitary.supplyAirFlowRateMethodWhenNoCoolingorHeatingisRequired());
  EXPECT_FALSE(unitary.isSupplyAirFlowRateWhenNoCoolingorHeatingisRequiredAutosized());
  EXPECT_FALSE(unitary.supplyAirFlowRateWhenNoCoolingorHeatingisRequired());
  ASSERT_TRUE(unitary.supplyAirFlowRatePerFloorAreaWhenNoCoolingorHeatingisRequired());
  EXPECT_EQ(1.0, unitary.supplyAirFlowRatePerFloorAreaWhenNoCoolingorHeatingisRequired().get());
  EXPECT_FALSE(unitary.fractionofAutosizedDesignCoolingSupplyAirFlowRateWhenNoCoolingorHeatingisRequired());
  EXPECT_FALSE(unitary.fractionofAutosizedDesignHeatingSupplyAirFlowRateWhenNoCoolingorHeatingisRequired());
  EXPECT_FALSE(unitary.designSupplyAirFlowRatePerUnitofCapacityDuringCoolingOperationWhenNoCoolingorHeatingisRequired());
  EXPECT_FALSE(unitary.designSupplyAirFlowRatePerUnitofCapacityDuringHeatingOperationWhenNoCoolingorHeatingisRequired());

  unitary.autosizeSupplyAirFlowRateWhenNoCoolingorHeatingisRequired();
  EXPECT_EQ("SupplyAirFlowRate", unitary.supplyAirFlowRateMethodWhenNoCoolingorHeatingisRequired());
  EXPECT_TRUE(unitary.isSupplyAirFlowRateWhenNoCoolingorHeatingisRequiredAutosized());
  EXPECT_FALSE(unitary.supplyAirFlowRateWhenNoCoolingorHeatingisRequired());
  EXPECT_FALSE(unitary.supplyAirFlowRatePerFloorAreaWhenNoCoolingorHeatingisRequired());
  EXPECT_FALSE(unitary.fractionofAutosizedDesignCoolingSupplyAirFlowRateWhenNoCoolingorHeatingisRequired());
  EXPECT_FALSE(unitary.fractionofAutosizedDesignHeatingSupplyAirFlowRateWhenNoCoolingorHeatingisRequired());
  EXPECT_FALSE(unitary.designSupplyAirFlowRatePerUnitofCapacityDuringCoolingOperationWhenNoCoolingorHeatingisRequired());
  EXPECT_FALSE(unitary.designSupplyAirFlowRatePerUnitofCapacityDuringHeatingOperationWhenNoCoolingorHeatingisRequired());

  EXPECT_TRUE(unitary.setFractionofAutosizedDesignCoolingSupplyAirFlowRateWhenNoCoolingorHeatingisRequired(1.0));
  EXPECT_EQ("FractionOfAutosizedCoolingValue", unitary.supplyAirFlowRateMethodWhenNoCoolingorHeatingisRequired());
  EXPECT_FALSE(unitary.isSupplyAirFlowRateWhenNoCoolingorHeatingisRequiredAutosized());
  EXPECT_FALSE(unitary.supplyAirFlowRateWhenNoCoolingorHeatingisRequired());
  EXPECT_FALSE(unitary.supplyAirFlowRatePerFloorAreaWhenNoCoolingorHeatingisRequired());
  ASSERT_TRUE(unitary.fractionofAutosizedDesignCoolingSupplyAirFlowRateWhenNoCoolingorHeatingisRequired());
  EXPECT_EQ(1.0, unitary.fractionofAutosizedDesignCoolingSupplyAirFlowRateWhenNoCoolingorHeatingisRequired().get());
  EXPECT_FALSE(unitary.fractionofAutosizedDesignHeatingSupplyAirFlowRateWhenNoCoolingorHeatingisRequired());
  EXPECT_FALSE(unitary.designSupplyAirFlowRatePerUnitofCapacityDuringCoolingOperationWhenNoCoolingorHeatingisRequired());
  EXPECT_FALSE(unitary.designSupplyAirFlowRatePerUnitofCapacityDuringHeatingOperationWhenNoCoolingorHeatingisRequired());

  EXPECT_TRUE(unitary.setFractionofAutosizedDesignHeatingSupplyAirFlowRateWhenNoCoolingorHeatingisRequired(1.0));
  EXPECT_EQ("FractionOfAutosizedHeatingValue", unitary.supplyAirFlowRateMethodWhenNoCoolingorHeatingisRequired());
  EXPECT_FALSE(unitary.isSupplyAirFlowRateWhenNoCoolingorHeatingisRequiredAutosized());
  EXPECT_FALSE(unitary.supplyAirFlowRateWhenNoCoolingorHeatingisRequired());
  EXPECT_FALSE(unitary.supplyAirFlowRatePerFloorAreaWhenNoCoolingorHeatingisRequired());
  EXPECT_FALSE(unitary.fractionofAutosizedDesignCoolingSupplyAirFlowRateWhenNoCoolingorHeatingisRequired());
  ASSERT_TRUE(unitary.fractionofAutosizedDesignHeatingSupplyAirFlowRateWhenNoCoolingorHeatingisRequired());
  EXPECT_EQ(1.0, unitary.fractionofAutosizedDesignHeatingSupplyAirFlowRateWhenNoCoolingorHeatingisRequired().get());
  EXPECT_FALSE(unitary.designSupplyAirFlowRatePerUnitofCapacityDuringCoolingOperationWhenNoCoolingorHeatingisRequired());
  EXPECT_FALSE(unitary.designSupplyAirFlowRatePerUnitofCapacityDuringHeatingOperationWhenNoCoolingorHeatingisRequired());

  EXPECT_TRUE(unitary.setDesignSupplyAirFlowRatePerUnitofCapacityDuringCoolingOperationWhenNoCoolingorHeatingisRequired(1.0));
  EXPECT_EQ("FlowPerCoolingCapacity", unitary.supplyAirFlowRateMethodWhenNoCoolingorHeatingisRequired());
  EXPECT_FALSE(unitary.isSupplyAirFlowRateWhenNoCoolingorHeatingisRequiredAutosized());
  EXPECT_FALSE(unitary.supplyAirFlowRateWhenNoCoolingorHeatingisRequired());
  EXPECT_FALSE(unitary.supplyAirFlowRatePerFloorAreaWhenNoCoolingorHeatingisRequired());
  EXPECT_FALSE(unitary.fractionofAutosizedDesignCoolingSupplyAirFlowRateWhenNoCoolingorHeatingisRequired());
  EXPECT_FALSE(unitary.fractionofAutosizedDesignHeatingSupplyAirFlowRateWhenNoCoolingorHeatingisRequired());
  ASSERT_TRUE(unitary.designSupplyAirFlowRatePerUnitofCapacityDuringCoolingOperationWhenNoCoolingorHeatingisRequired());
  EXPECT_EQ(1.0, unitary.designSupplyAirFlowRatePerUnitofCapacityDuringCoolingOperationWhenNoCoolingorHeatingisRequired().get());
  EXPECT_FALSE(unitary.designSupplyAirFlowRatePerUnitofCapacityDuringHeatingOperationWhenNoCoolingorHeatingisRequired());

  EXPECT_TRUE(unitary.setDesignSupplyAirFlowRatePerUnitofCapacityDuringHeatingOperationWhenNoCoolingorHeatingisRequired(1.0));
  EXPECT_EQ("FlowPerHeatingCapacity", unitary.supplyAirFlowRateMethodWhenNoCoolingorHeatingisRequired());
  EXPECT_FALSE(unitary.isSupplyAirFlowRateWhenNoCoolingorHeatingisRequiredAutosized());
  EXPECT_FALSE(unitary.supplyAirFlowRateWhenNoCoolingorHeatingisRequired());
  EXPECT_FALSE(unitary.supplyAirFlowRatePerFloorAreaWhenNoCoolingorHeatingisRequired());
  EXPECT_FALSE(unitary.fractionofAutosizedDesignCoolingSupplyAirFlowRateWhenNoCoolingorHeatingisRequired());
  EXPECT_FALSE(unitary.fractionofAutosizedDesignHeatingSupplyAirFlowRateWhenNoCoolingorHeatingisRequired());
  EXPECT_FALSE(unitary.designSupplyAirFlowRatePerUnitofCapacityDuringCoolingOperationWhenNoCoolingorHeatingisRequired());
  ASSERT_TRUE(unitary.designSupplyAirFlowRatePerUnitofCapacityDuringHeatingOperationWhenNoCoolingorHeatingisRequired());
  EXPECT_EQ(1.0, unitary.designSupplyAirFlowRatePerUnitofCapacityDuringHeatingOperationWhenNoCoolingorHeatingisRequired().get());

  // Reset both coils => back to None
  unitary.resetHeatingCoil();
  EXPECT_EQ("FlowPerHeatingCapacity", unitary.supplyAirFlowRateMethodWhenNoCoolingorHeatingisRequired());

  unitary.resetCoolingCoil();
  EXPECT_EQ("None", unitary.supplyAirFlowRateMethodWhenNoCoolingorHeatingisRequired());
  EXPECT_FALSE(unitary.isSupplyAirFlowRateWhenNoCoolingorHeatingisRequiredAutosized());
  EXPECT_FALSE(unitary.supplyAirFlowRateWhenNoCoolingorHeatingisRequired());
  EXPECT_FALSE(unitary.supplyAirFlowRatePerFloorAreaWhenNoCoolingorHeatingisRequired());
  EXPECT_FALSE(unitary.fractionofAutosizedDesignCoolingSupplyAirFlowRateWhenNoCoolingorHeatingisRequired());
  EXPECT_FALSE(unitary.fractionofAutosizedDesignHeatingSupplyAirFlowRateWhenNoCoolingorHeatingisRequired());
  EXPECT_FALSE(unitary.designSupplyAirFlowRatePerUnitofCapacityDuringCoolingOperationWhenNoCoolingorHeatingisRequired());
  EXPECT_FALSE(unitary.designSupplyAirFlowRatePerUnitofCapacityDuringHeatingOperationWhenNoCoolingorHeatingisRequired());

  EXPECT_TRUE(unitary.setHeatingCoil(hc));
  EXPECT_TRUE(unitary.setDesignSupplyAirFlowRatePerUnitofCapacityDuringHeatingOperationWhenNoCoolingorHeatingisRequired(1.0));
  EXPECT_EQ("FlowPerHeatingCapacity", unitary.supplyAirFlowRateMethodWhenNoCoolingorHeatingisRequired());
  unitary.resetSupplyAirFlowRateMethodWhenNoCoolingorHeatingisRequired();
  EXPECT_EQ("None", unitary.supplyAirFlowRateMethodWhenNoCoolingorHeatingisRequired());
  EXPECT_FALSE(unitary.isSupplyAirFlowRateWhenNoCoolingorHeatingisRequiredAutosized());
  EXPECT_FALSE(unitary.supplyAirFlowRateWhenNoCoolingorHeatingisRequired());
  EXPECT_FALSE(unitary.supplyAirFlowRatePerFloorAreaWhenNoCoolingorHeatingisRequired());
  EXPECT_FALSE(unitary.fractionofAutosizedDesignCoolingSupplyAirFlowRateWhenNoCoolingorHeatingisRequired());
  EXPECT_FALSE(unitary.fractionofAutosizedDesignHeatingSupplyAirFlowRateWhenNoCoolingorHeatingisRequired());
  EXPECT_FALSE(unitary.designSupplyAirFlowRatePerUnitofCapacityDuringCoolingOperationWhenNoCoolingorHeatingisRequired());
  EXPECT_FALSE(unitary.designSupplyAirFlowRatePerUnitofCapacityDuringHeatingOperationWhenNoCoolingorHeatingisRequired());
}

TEST_F(ModelFixture, AirLoopHVACUnitarySystem_needToRemoveDeprecated) {
  Model m;
  AirLoopHVACUnitarySystem unitary = AirLoopHVACUnitarySystem(m);

  EXPECT_NO_THROW(unitary.setSupplyAirFlowRateMethodDuringCoolingOperation(""));
  EXPECT_NO_THROW(unitary.resetSupplyAirFlowRateMethodDuringCoolingOperation());
  EXPECT_NO_THROW(unitary.resetSupplyAirFlowRateDuringCoolingOperation());
  EXPECT_NO_THROW(unitary.resetSupplyAirFlowRatePerFloorAreaDuringCoolingOperation());
  EXPECT_NO_THROW(unitary.resetFractionofAutosizedDesignCoolingSupplyAirFlowRate());
  EXPECT_NO_THROW(unitary.resetDesignSupplyAirFlowRatePerUnitofCapacityDuringCoolingOperation());

  EXPECT_NO_THROW(unitary.setSupplyAirFlowRateMethodDuringHeatingOperation(""));
  EXPECT_NO_THROW(unitary.resetSupplyAirFlowRateMethodDuringHeatingOperation());
  EXPECT_NO_THROW(unitary.resetSupplyAirFlowRateDuringHeatingOperation());
  EXPECT_NO_THROW(unitary.resetSupplyAirFlowRatePerFloorAreaduringHeatingOperation());
  EXPECT_NO_THROW(unitary.resetFractionofAutosizedDesignHeatingSupplyAirFlowRate());
  EXPECT_NO_THROW(unitary.resetDesignSupplyAirFlowRatePerUnitofCapacityDuringHeatingOperation());
}
