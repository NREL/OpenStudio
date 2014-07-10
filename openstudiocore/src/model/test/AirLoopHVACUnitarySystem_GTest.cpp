/**********************************************************************
 *  Copyright (c) 2008-2014, Alliance for Sustainable Energy.
 *  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 **********************************************************************/

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

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, AirLoopHVACUnitarySystem_DefaultConstructors)
{
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  ASSERT_EXIT ( 
  {  
    Model m;
    AirLoopHVACUnitarySystem testObject = AirLoopHVACUnitarySystem(m);

    exit(0); 
  } ,
    ::testing::ExitedWithCode(0), "" );
}

TEST_F(ModelFixture, AirLoopHVACUnitarySystem_Remove)
{
  Model m;
  Schedule s = m.alwaysOnDiscreteSchedule();
  AirLoopHVACUnitarySystem testObject = AirLoopHVACUnitarySystem(m);
  FanOnOff fan = FanOnOff(m,s);
  CoilHeatingElectric heatingCoil = CoilHeatingElectric(m,s);
  CoilHeatingElectric suppHeatingCoil = CoilHeatingElectric(m,s);
  CoilCoolingWater coolingCoil = CoilCoolingWater(m,s);

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

TEST_F(ModelFixture, AirLoopHVACUnitarySystem_CloneOneModelWithDefaultData)
{
  Model m;
  Schedule s = m.alwaysOnDiscreteSchedule();
  AirLoopHVACUnitarySystem testObject = AirLoopHVACUnitarySystem(m);
  FanOnOff fan = FanOnOff(m,s);
  CoilHeatingElectric heatingCoil = CoilHeatingElectric(m,s);
  CoilHeatingElectric suppHeatingCoil = CoilHeatingElectric(m,s);
  CoilCoolingWater coolingCoil = CoilCoolingWater(m,s);

  testObject.setSupplyFan(fan);
  testObject.setCoolingCoil(coolingCoil);
  testObject.setHeatingCoil(heatingCoil);
  testObject.setSupplementalHeatingCoil(suppHeatingCoil);

  AirLoopHVACUnitarySystem testObjectClone = testObject.clone(m).cast<AirLoopHVACUnitarySystem>();
  
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
  EXPECT_DOUBLE_EQ(2.5, testObjectClone.maximumCyclingRate());
  EXPECT_DOUBLE_EQ(60.0, testObjectClone.heatPumpTimeConstant());
  EXPECT_DOUBLE_EQ(0.01, testObjectClone.fractionofOnCyclePowerUse());
  EXPECT_DOUBLE_EQ(60, testObjectClone.heatPumpFanDelayTime());
  EXPECT_DOUBLE_EQ(0.0, testObjectClone.ancilliaryOnCycleElectricPower());
  EXPECT_DOUBLE_EQ(0.0, testObjectClone.ancilliaryOffCycleElectricPower());
  // EXPECT_DOUBLE_EQ(80.0, testObjectClone.maximumTemperatureforHeatRecovery());

  EXPECT_NE(testObject.supplyFan(), testObjectClone.supplyFan());
  EXPECT_NE(testObject.coolingCoil(), testObjectClone.coolingCoil());
  EXPECT_NE(testObject.heatingCoil(), testObjectClone.heatingCoil());
  EXPECT_NE(testObject.supplementalHeatingCoil(), testObjectClone.supplementalHeatingCoil());

  std::vector<AirLoopHVACUnitarySystem> unitarySystem = m.getConcreteModelObjects<AirLoopHVACUnitarySystem>();
  EXPECT_EQ(2, unitarySystem.size());

  std::vector<FanOnOff> fans = m.getConcreteModelObjects<FanOnOff>();
  EXPECT_EQ(2, fans.size());

  std::vector<CoilHeatingElectric> heatingCoils = m.getConcreteModelObjects<CoilHeatingElectric>();
  EXPECT_EQ(4, heatingCoils.size());

  std::vector<CoilCoolingWater> coolingCoils = m.getConcreteModelObjects<CoilCoolingWater>();
  EXPECT_EQ(2, coolingCoils.size());
}

TEST_F(ModelFixture, AirLoopHVACUnitarySystem_CloneOneModelWithCustomData)
{
  Model m;
  Schedule s = m.alwaysOnDiscreteSchedule();
  AirLoopHVACUnitarySystem testObject = AirLoopHVACUnitarySystem(m);
  FanOnOff fan = FanOnOff(m,s);
  CoilHeatingElectric heatingCoil = CoilHeatingElectric(m,s);
  CoilHeatingElectric suppHeatingCoil = CoilHeatingElectric(m,s);
  CoilCoolingWater coolingCoil = CoilCoolingWater(m,s);

  testObject.setSupplyFan(fan);
  testObject.setCoolingCoil(coolingCoil);
  testObject.setHeatingCoil(heatingCoil);
  testObject.setSupplementalHeatingCoil(suppHeatingCoil);

  // testObject.setControlType("SetPoint");
  testObject.setDehumidificationControlType("CoolReheat");
  testObject.setFanPlacement("BlowThrough");
  testObject.setDXHeatingCoilSizingRatio(999.0);
  testObject.setUseDOASDXCoolingCoil(true);
  testObject.setDOASDXCoolingCoilLeavingMinimumAirTemperature(7.0);
  testObject.setLatentLoadControl("LatentWithSensibleLoadControl");
  testObject.autosizeSupplyAirFlowRateDuringCoolingOperation();
  testObject.autosizeSupplyAirFlowRateDuringHeatingOperation();
  testObject.autosizeSupplyAirFlowRateWhenNoCoolingorHeatingisRequired();
  testObject.autosizeMaximumSupplyAirTemperature();
  testObject.setMaximumOutdoorDryBulbTemperatureforSupplementalHeaterOperation(999.0);
  testObject.setMaximumCyclingRate(5.0);
  testObject.setHeatPumpTimeConstant(500.0);
  testObject.setFractionofOnCyclePowerUse(0.05);
  testObject.setHeatPumpFanDelayTime(999.0);
  testObject.setAncilliaryOnCycleElectricPower(999.0);
  testObject.setAncilliaryOffCycleElectricPower(999.0);
  // testObject.setMaximumTemperatureforHeatRecovery(100.0);

  AirLoopHVACUnitarySystem testObjectClone = testObject.clone(m).cast<AirLoopHVACUnitarySystem>();

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
  EXPECT_DOUBLE_EQ(5.0, testObjectClone.maximumCyclingRate());
  EXPECT_DOUBLE_EQ(500.0, testObjectClone.heatPumpTimeConstant());
  EXPECT_DOUBLE_EQ(0.05, testObjectClone.fractionofOnCyclePowerUse());
  EXPECT_DOUBLE_EQ(999.0, testObjectClone.heatPumpFanDelayTime());
  EXPECT_DOUBLE_EQ(999.0, testObjectClone.ancilliaryOnCycleElectricPower());
  EXPECT_DOUBLE_EQ(999.0, testObjectClone.ancilliaryOffCycleElectricPower());
  // EXPECT_DOUBLE_EQ(100.0, testObjectClone.maximumTemperatureforHeatRecovery());

  EXPECT_NE(testObject.supplyFan(), testObjectClone.supplyFan());
  EXPECT_NE(testObject.coolingCoil(), testObjectClone.coolingCoil());
  EXPECT_NE(testObject.heatingCoil(), testObjectClone.heatingCoil());
  EXPECT_NE(testObject.supplementalHeatingCoil(), testObjectClone.supplementalHeatingCoil());

  std::vector<AirLoopHVACUnitarySystem> unitarySystem = m.getConcreteModelObjects<AirLoopHVACUnitarySystem>();
  EXPECT_EQ(2, unitarySystem.size());

  std::vector<FanOnOff> fans = m.getConcreteModelObjects<FanOnOff>();
  EXPECT_EQ(2, fans.size());

  std::vector<CoilHeatingElectric> heatingCoils = m.getConcreteModelObjects<CoilHeatingElectric>();
  EXPECT_EQ(4, heatingCoils.size());

  std::vector<CoilCoolingWater> coolingCoils = m.getConcreteModelObjects<CoilCoolingWater>();
  EXPECT_EQ(2, coolingCoils.size());
}

TEST_F(ModelFixture, AirLoopHVACUnitarySystem_CloneTwoModelsWithCustomData)
{
  Model m;
  Schedule s = m.alwaysOnDiscreteSchedule();
  AirLoopHVACUnitarySystem testObject = AirLoopHVACUnitarySystem(m);
  FanOnOff fan = FanOnOff(m,s);
  CoilHeatingElectric heatingCoil = CoilHeatingElectric(m,s);
  CoilHeatingElectric suppHeatingCoil = CoilHeatingElectric(m,s);
  CoilCoolingWater coolingCoil = CoilCoolingWater(m,s);

  testObject.setSupplyFan(fan);
  testObject.setCoolingCoil(coolingCoil);
  testObject.setHeatingCoil(heatingCoil);
  testObject.setSupplementalHeatingCoil(suppHeatingCoil);

  AirLoopHVACUnitarySystem testObjectClone = testObject.clone(m).cast<AirLoopHVACUnitarySystem>();

  EXPECT_NE(testObject.supplyFan(), testObjectClone.supplyFan());
  EXPECT_NE(testObject.coolingCoil(), testObjectClone.coolingCoil());
  EXPECT_NE(testObject.heatingCoil(), testObjectClone.heatingCoil());
  EXPECT_NE(testObject.supplementalHeatingCoil(), testObjectClone.supplementalHeatingCoil());

  std::vector<AirLoopHVACUnitarySystem> unitarySystem = m.getConcreteModelObjects<AirLoopHVACUnitarySystem>();
  EXPECT_EQ(2, unitarySystem.size());

  std::vector<FanOnOff> fans = m.getConcreteModelObjects<FanOnOff>();
  EXPECT_EQ(2, fans.size());

  std::vector<CoilHeatingElectric> heatingCoils = m.getConcreteModelObjects<CoilHeatingElectric>();
  EXPECT_EQ(4, heatingCoils.size());

  std::vector<CoilCoolingWater> coolingCoils = m.getConcreteModelObjects<CoilCoolingWater>();
  EXPECT_EQ(2, coolingCoils.size());

  Model m2;
  AirLoopHVACUnitarySystem testObjectClone2 = testObject.clone(m2).cast<AirLoopHVACUnitarySystem>();

  unitarySystem = m2.getConcreteModelObjects<AirLoopHVACUnitarySystem>();
  EXPECT_EQ(1, unitarySystem.size());

  fans = m2.getConcreteModelObjects<FanOnOff>();
  EXPECT_EQ(1, fans.size());

  heatingCoils = m2.getConcreteModelObjects<CoilHeatingElectric>();
  EXPECT_EQ(2, heatingCoils.size());

  coolingCoils = m2.getConcreteModelObjects<CoilCoolingWater>();
  EXPECT_EQ(1, coolingCoils.size());
}

TEST_F(ModelFixture,AirLoopHVACUnitarySystem_addToNode)
{
  Model m;
  AirLoopHVACUnitarySystem testObject = AirLoopHVACUnitarySystem(m);
  AirLoopHVAC airLoop(m);

  Node supplyOutletNode = airLoop.supplyOutletNode();

  EXPECT_TRUE(testObject.addToNode(supplyOutletNode));

  EXPECT_EQ( (unsigned)3, airLoop.supplyComponents().size() );

  EXPECT_TRUE(testObject.airInletPort());
  EXPECT_TRUE(testObject.airOutletPort());
}

TEST_F(ModelFixture, AirLoopHVACUnitarySystem_AddToNodeTwoSameObjects)
{
  Model m;
  AirLoopHVACUnitarySystem testObject = AirLoopHVACUnitarySystem(m);
  AirLoopHVAC airLoop(m);

  Node supplyOutletNode = airLoop.supplyOutletNode();
  testObject.addToNode(supplyOutletNode);
  supplyOutletNode = airLoop.supplyOutletNode();
  EXPECT_TRUE(testObject.addToNode(supplyOutletNode));
  
  EXPECT_TRUE(testObject.airInletPort());
  EXPECT_TRUE(testObject.airOutletPort());   
}

TEST_F(ModelFixture, AirLoopHVACUnitarySystem_AddToNodeAirLoopDemandSide)
{
  Model m;
  AirLoopHVACUnitarySystem testObject = AirLoopHVACUnitarySystem(m);
  AirLoopHVAC airLoop(m);

  Node inletNode = airLoop.zoneSplitter().lastOutletModelObject()->cast<Node>();

  EXPECT_FALSE(testObject.addToNode(inletNode));

  EXPECT_EQ((unsigned)5, airLoop.demandComponents().size()); 
}

TEST_F(ModelFixture, AirLoopHVACUnitarySystem_AddToNodePlantLoop)
{
  Model m;
  AirLoopHVACUnitarySystem testObject = AirLoopHVACUnitarySystem(m);
  PlantLoop plantLoop(m);

  EXPECT_EQ( (unsigned)5,plantLoop.demandComponents().size() );

  Node demandInletNode = plantLoop.demandSplitter().lastOutletModelObject()->cast<Node>();

  EXPECT_FALSE(testObject.addToNode(demandInletNode));
  EXPECT_FALSE(plantLoop.addDemandBranchForComponent(testObject));
  EXPECT_EQ((unsigned)5, plantLoop.demandComponents().size());  

  Node supplyInletNode = plantLoop.supplySplitter().lastOutletModelObject()->cast<Node>();

  EXPECT_FALSE(testObject.addToNode(supplyInletNode));
  EXPECT_EQ((unsigned)5, plantLoop.supplyComponents().size());
  EXPECT_FALSE(plantLoop.addSupplyBranchForComponent(testObject));
  EXPECT_EQ((unsigned)6, plantLoop.supplyComponents().size());
  ASSERT_FALSE(testObject.plantLoop());
  // EXPECT_EQ(plantLoop, testObject.plantLoop().get());
}

TEST_F(ModelFixture, AirLoopHVACUnitarySystem_WaterHeatingCoilToPlant)
{
  Model m;
  Schedule s = m.alwaysOnDiscreteSchedule();
  AirLoopHVACUnitarySystem testObject = AirLoopHVACUnitarySystem(m);
  FanOnOff fan = FanOnOff(m,s);
  CoilHeatingWater heatingCoil = CoilHeatingWater(m,s);
  CoilHeatingWater suppHeatingCoil = CoilHeatingWater(m,s);
  CoilCoolingWater coolingCoil = CoilCoolingWater(m,s);

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

TEST_F(ModelFixture, AirLoopHVACUnitarySystem_RemoveWaterHeatingCoilFromPlant)
{
  Model m;
  Schedule s = m.alwaysOnDiscreteSchedule();
  AirLoopHVACUnitarySystem testObject = AirLoopHVACUnitarySystem(m);
  FanOnOff fan = FanOnOff(m,s);
  CoilHeatingWater heatingCoil = CoilHeatingWater(m,s);
  CoilHeatingWater suppHeatingCoil = CoilHeatingWater(m,s);
  CoilCoolingWater coolingCoil = CoilCoolingWater(m,s);

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

TEST_F(ModelFixture,AirLoopHVACUnitarySystem_containingHVACComponent)
{
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
}
