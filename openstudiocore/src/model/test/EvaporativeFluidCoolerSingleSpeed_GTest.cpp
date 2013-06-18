/**********************************************************************
 *  Copyright (c) 2008-2013, Alliance for Sustainable Energy.
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

#include <model/test/ModelFixture.hpp>

#include <model/EvaporativeFluidCoolerSingleSpeed.hpp>
#include <model/EvaporativeFluidCoolerSingleSpeed_Impl.hpp>

#include <model/PlantLoop.hpp>
#include <model/PlantLoop_Impl.hpp>
#include <model/Splitter.hpp>
#include <model/Splitter_Impl.hpp>
#include <model/Node.hpp>
#include <model/Node_Impl.hpp>
#include <model/AirLoopHVAC.hpp>
#include <model/AirLoopHVAC_Impl.hpp>
#include <model/AirLoopHVACZoneSplitter.hpp>
#include <model/AirLoopHVACZoneSplitter_Impl.hpp>
#include <model/ConnectorMixer.hpp>
#include <model/ConnectorMixer_Impl.hpp>
#include <model/ConnectorSplitter.hpp>
#include <model/ConnectorSplitter_Impl.hpp>
#include <model/ThermalZone.hpp>
#include <model/ThermalZone_Impl.hpp>
#include <model/Schedule.hpp>
#include <model/Schedule_Impl.hpp>
#include <model/ScheduleCompact.hpp>
#include <model/ScheduleCompact_Impl.hpp>

//using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, EvaporativeFluidCoolerSingleSpeed_DefaultConstructor)
{
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  ASSERT_EXIT ( 
  {  
    Model model;
    
    EvaporativeFluidCoolerSingleSpeed testObject = EvaporativeFluidCoolerSingleSpeed(model);

    EXPECT_TRUE(testObject.isDesignAirFlowRateAutosized());
    EXPECT_TRUE(testObject.isFanPoweratDesignAirFlowRateAutosized());
    EXPECT_DOUBLE_EQ(0.03, testObject.designSprayWaterFlowRate());
    EXPECT_EQ("UFactorTimesAreaAndDesignWaterFlowRate", testObject.performanceInputMethod().get());
    EXPECT_FALSE(testObject.standardDesignCapacity());
    EXPECT_TRUE(testObject.isUfactorTimesAreaValueatDesignAirFlowRateAutosized());
    EXPECT_TRUE(testObject.isDesignWaterFlowRateAutosized());
    EXPECT_FALSE(testObject.userSpecifiedDesignCapacity());
    EXPECT_FALSE(testObject.designEnteringWaterTemperature());
    EXPECT_FALSE(testObject.designEnteringAirTemperature());
    EXPECT_FALSE(testObject.designEnteringAirWetbulbTemperature());
    EXPECT_EQ("FanCycling", testObject.capacityControl());
    EXPECT_DOUBLE_EQ(1.0, testObject.sizingFactor());
    EXPECT_EQ("SaturatedExit", testObject.evaporationLossMode());
    EXPECT_DOUBLE_EQ(0.008, testObject.driftLossPercent());
    EXPECT_EQ("ConcentrationRatio", testObject.blowdownCalculationMode());
    EXPECT_DOUBLE_EQ(3.0, testObject.blowdownConcentrationRatio());
    EXPECT_FALSE(testObject.blowdownMakeupWaterUsageSchedule());

    exit(0); 
  } ,
    ::testing::ExitedWithCode(0), "" );
}

TEST_F(ModelFixture, EvaporativeFluidCoolerSingleSpeed_AddToNode)
{
  Model model;
  PlantLoop plantLoop(model);

  EXPECT_EQ( (unsigned)5,plantLoop.supplyComponents().size() );

  Node inletNode = plantLoop.supplySplitter().lastOutletModelObject()->cast<Node>();
  
  EvaporativeFluidCoolerSingleSpeed testObject = EvaporativeFluidCoolerSingleSpeed(model);

  EXPECT_TRUE(testObject.addToNode(inletNode));

  EXPECT_EQ((unsigned)7, plantLoop.supplyComponents().size());
  
  EXPECT_TRUE(testObject.inletPort());
  EXPECT_TRUE(testObject.outletPort());   
}

TEST_F(ModelFixture, EvaporativeFluidCoolerSingleSpeed_AddToNodePlantLoopDemand)
{
  Model model;
  PlantLoop plantLoop(model);

  EXPECT_EQ( (unsigned)5,plantLoop.demandComponents().size() );

  Node inletNode = plantLoop.demandSplitter().lastOutletModelObject()->cast<Node>();
  
  EvaporativeFluidCoolerSingleSpeed testObject = EvaporativeFluidCoolerSingleSpeed(model);

  EXPECT_FALSE(testObject.addToNode(inletNode));
  EXPECT_EQ((unsigned)5, plantLoop.demandComponents().size());  
}

TEST_F(ModelFixture, EvaporativeFluidCoolerSingleSpeed_AddToNodeAirLoop)
{
  Model model;
  AirLoopHVAC airLoop(model);
  ThermalZone thermalZone(model);

  EvaporativeFluidCoolerSingleSpeed testObject = EvaporativeFluidCoolerSingleSpeed(model);

  Node inletNode = airLoop.zoneSplitter().lastOutletModelObject()->cast<Node>();

  EXPECT_FALSE(testObject.addToNode(inletNode));
  EXPECT_EQ((unsigned)5, airLoop.demandComponents().size());

  Node supplyOutletNode = airLoop.supplyOutletNode();

  EXPECT_FALSE(testObject.addToNode(supplyOutletNode));
  EXPECT_EQ((unsigned)2, airLoop.supplyComponents().size());

  //The following test returns false BUT it doesn't remove the object from air loop causing the number of components to be wrong
  //EXPECT_FALSE(airLoop.addBranchForHVACComponent(testObject));
  //EXPECT_EQ((unsigned)5, airLoop.demandComponents().size());
  //The following test does not fail and adds the thermalZone to the airloop it appears
  //EXPECT_FALSE(airLoop.addBranchForZone(thermalZone, testObject));
  //EXPECT_EQ((unsigned)5, airLoop.demandComponents().size());
}

TEST_F(ModelFixture, EvaporativeFluidCoolerSingleSpeed_AddObjectByPlantLoopAddSupplyBranchForComponent)
{
  Model model;
  PlantLoop plantLoop(model);
  
  EvaporativeFluidCoolerSingleSpeed testObject = EvaporativeFluidCoolerSingleSpeed(model);

  EXPECT_TRUE(plantLoop.addSupplyBranchForComponent(testObject));
  EXPECT_EQ((unsigned)7, plantLoop.supplyComponents().size());
  
  EXPECT_TRUE(testObject.inletPort());
  EXPECT_TRUE(testObject.outletPort());   
}

TEST_F(ModelFixture, EvaporativeFluidCoolerSingleSpeed_AddToNodeTwoSameObjects)
{
  Model model;
  PlantLoop plantLoop(model);
  
  EvaporativeFluidCoolerSingleSpeed testObject = EvaporativeFluidCoolerSingleSpeed(model);

  Node inletNode = plantLoop.supplySplitter().lastOutletModelObject()->cast<Node>();
  testObject.addToNode(inletNode);
  inletNode = plantLoop.supplySplitter().lastOutletModelObject()->cast<Node>();
  EXPECT_FALSE(testObject.addToNode(inletNode));
  EXPECT_EQ((unsigned)7, plantLoop.supplyComponents().size());
  
  EXPECT_TRUE(testObject.inletPort());
  EXPECT_TRUE(testObject.outletPort());   
}

TEST_F(ModelFixture, EvaporativeFluidCoolerSingleSpeed_AddToNodeTwoUniqueObjects)
{
  Model model;
  PlantLoop plantLoop(model);
  
  EvaporativeFluidCoolerSingleSpeed testObject = EvaporativeFluidCoolerSingleSpeed(model);
  EvaporativeFluidCoolerSingleSpeed testObject2 = EvaporativeFluidCoolerSingleSpeed(model);

  Node inletNode = plantLoop.supplySplitter().lastOutletModelObject()->cast<Node>();
  testObject.addToNode(inletNode);
  inletNode = plantLoop.supplySplitter().lastOutletModelObject()->cast<Node>();
  EXPECT_FALSE(testObject2.addToNode(inletNode)); 
  EXPECT_EQ((unsigned)7, plantLoop.supplyComponents().size()); 
}

TEST_F(ModelFixture, EvaporativeFluidCoolerSingleSpeed_IsRemovable)
{
  Model model;
  
  EvaporativeFluidCoolerSingleSpeed testObject = EvaporativeFluidCoolerSingleSpeed(model);

  EXPECT_TRUE(testObject.isRemovable());
}

TEST_F(ModelFixture, EvaporativeFluidCoolerSingleSpeed_Remove)
{
  Model model;
  PlantLoop plantLoop(model);
  
  EvaporativeFluidCoolerSingleSpeed testObject = EvaporativeFluidCoolerSingleSpeed(model);

  Node inletNode = plantLoop.supplySplitter().lastOutletModelObject()->cast<Node>();
  testObject.addToNode(inletNode);

  EXPECT_EQ((unsigned)7, plantLoop.supplyComponents().size());

  testObject.remove();
  EXPECT_EQ((unsigned)5, plantLoop.supplyComponents().size());
}

TEST_F(ModelFixture, EvaporativeFluidCoolerSingleSpeed_RemoveByPlantLoopRemoveSupplyBranchWithComponent)
{
  Model model;
  PlantLoop plantLoop(model);
  
  EvaporativeFluidCoolerSingleSpeed testObject = EvaporativeFluidCoolerSingleSpeed(model);

  plantLoop.addSupplyBranchForComponent(testObject);

  EXPECT_EQ((unsigned)7, plantLoop.supplyComponents().size());

  plantLoop.removeSupplyBranchWithComponent(testObject);
  EXPECT_EQ((unsigned)5, plantLoop.supplyComponents().size());
}

TEST_F(ModelFixture, EvaporativeFluidCoolerSingleSpeed_CloneOneModelWithDefaultData)
{
  Model model;
  EvaporativeFluidCoolerSingleSpeed testObject = EvaporativeFluidCoolerSingleSpeed(model);

  EvaporativeFluidCoolerSingleSpeed testObjectClone = testObject.clone(model).cast<EvaporativeFluidCoolerSingleSpeed>();

  EXPECT_TRUE(testObjectClone.isDesignAirFlowRateAutosized());
  EXPECT_TRUE(testObjectClone.isFanPoweratDesignAirFlowRateAutosized());
  EXPECT_DOUBLE_EQ(0.03, testObjectClone.designSprayWaterFlowRate());
  EXPECT_EQ("UFactorTimesAreaAndDesignWaterFlowRate", testObjectClone.performanceInputMethod().get());
  EXPECT_FALSE(testObjectClone.standardDesignCapacity());
  EXPECT_TRUE(testObjectClone.isUfactorTimesAreaValueatDesignAirFlowRateAutosized());
  EXPECT_TRUE(testObjectClone.isDesignWaterFlowRateAutosized());
  EXPECT_FALSE(testObjectClone.userSpecifiedDesignCapacity());
  EXPECT_FALSE(testObjectClone.designEnteringWaterTemperature());
  EXPECT_FALSE(testObjectClone.designEnteringAirTemperature());
  EXPECT_FALSE(testObjectClone.designEnteringAirWetbulbTemperature());
  EXPECT_EQ("FanCycling", testObjectClone.capacityControl());
  EXPECT_DOUBLE_EQ(1.0, testObjectClone.sizingFactor());
  EXPECT_EQ("SaturatedExit", testObjectClone.evaporationLossMode());
  EXPECT_DOUBLE_EQ(0.008, testObjectClone.driftLossPercent());
  EXPECT_EQ("ConcentrationRatio", testObjectClone.blowdownCalculationMode());
  EXPECT_DOUBLE_EQ(3.0, testObjectClone.blowdownConcentrationRatio());
  EXPECT_FALSE(testObjectClone.blowdownMakeupWaterUsageSchedule());
}

TEST_F(ModelFixture, EvaporativeFluidCoolerSingleSpeed_CloneOneModelWithCustomData)
{
  Model model;
  EvaporativeFluidCoolerSingleSpeed testObject = EvaporativeFluidCoolerSingleSpeed(model);

  testObject.setDesignAirFlowRate(999.0);
  testObject.setFanPoweratDesignAirFlowRate(999.0);
  testObject.setPerformanceInputMethod("StandardDesignCapacity");
  testObject.setDesignSprayWaterFlowRate(999.0);
  testObject.setStandardDesignCapacity(1.0);

  EvaporativeFluidCoolerSingleSpeed testObjectClone = testObject.clone(model).cast<EvaporativeFluidCoolerSingleSpeed>();
  EXPECT_DOUBLE_EQ(999.0, testObjectClone.designAirFlowRate().get());
  EXPECT_DOUBLE_EQ(999.0, testObjectClone.fanPoweratDesignAirFlowRate().get());
  EXPECT_DOUBLE_EQ(999.0, testObjectClone.designSprayWaterFlowRate());
  EXPECT_EQ("StandardDesignCapacity", testObjectClone.performanceInputMethod().get());
  EXPECT_DOUBLE_EQ(1.0, testObjectClone.standardDesignCapacity().get());
}

TEST_F(ModelFixture, EvaporativeFluidCoolerSingleSpeed_CloneTwoModelsWithDefaultData)
{
  Model model;
  EvaporativeFluidCoolerSingleSpeed testObject = EvaporativeFluidCoolerSingleSpeed(model);

  EvaporativeFluidCoolerSingleSpeed testObjectClone = testObject.clone(model).cast<EvaporativeFluidCoolerSingleSpeed>();

  Model model2;

  EvaporativeFluidCoolerSingleSpeed testObjectClone2 = testObject.clone(model2).cast<EvaporativeFluidCoolerSingleSpeed>();
  EXPECT_TRUE(testObjectClone2.isDesignAirFlowRateAutosized());
  EXPECT_TRUE(testObjectClone2.isFanPoweratDesignAirFlowRateAutosized());
  EXPECT_DOUBLE_EQ(0.03, testObjectClone2.designSprayWaterFlowRate());
  EXPECT_EQ("UFactorTimesAreaAndDesignWaterFlowRate", testObjectClone2.performanceInputMethod().get());
  EXPECT_FALSE(testObjectClone2.standardDesignCapacity());
  EXPECT_TRUE(testObjectClone2.isUfactorTimesAreaValueatDesignAirFlowRateAutosized());
  EXPECT_TRUE(testObjectClone2.isDesignWaterFlowRateAutosized());
  EXPECT_FALSE(testObjectClone2.userSpecifiedDesignCapacity());
  EXPECT_FALSE(testObjectClone2.designEnteringWaterTemperature());
  EXPECT_FALSE(testObjectClone2.designEnteringAirTemperature());
  EXPECT_FALSE(testObjectClone2.designEnteringAirWetbulbTemperature());
  EXPECT_EQ("FanCycling", testObjectClone2.capacityControl());
  EXPECT_DOUBLE_EQ(1.0, testObjectClone2.sizingFactor());
  EXPECT_EQ("SaturatedExit", testObjectClone2.evaporationLossMode());
  EXPECT_DOUBLE_EQ(0.008, testObjectClone2.driftLossPercent());
  EXPECT_EQ("ConcentrationRatio", testObjectClone2.blowdownCalculationMode());
  EXPECT_DOUBLE_EQ(3.0, testObjectClone2.blowdownConcentrationRatio());
  EXPECT_FALSE(testObjectClone.blowdownMakeupWaterUsageSchedule());
  EXPECT_NE(testObjectClone2, testObjectClone);
}

TEST_F(ModelFixture, EvaporativeFluidCoolerSingleSpeed_DesignAirFlowRate) 
{
  Model model;
  EvaporativeFluidCoolerSingleSpeed testObject = EvaporativeFluidCoolerSingleSpeed(model);

  EXPECT_TRUE(testObject.isDesignAirFlowRateAutosized());

  testObject.setDesignAirFlowRate(999.0);
  EXPECT_DOUBLE_EQ(999.0, testObject.designAirFlowRate().get());

  EXPECT_FALSE(testObject.setDesignAirFlowRate(0.0));
  EXPECT_DOUBLE_EQ(999.0, testObject.designAirFlowRate().get());

  EXPECT_FALSE(testObject.setDesignAirFlowRate(-1.0));

  testObject.autosizeDesignAirFlowRate();
  EXPECT_TRUE(testObject.isDesignAirFlowRateAutosized());
}

TEST_F(ModelFixture, EvaporativeFluidCoolerSingleSpeed_FanPoweratDesignAirFlowRate) 
{
  Model model;
  EvaporativeFluidCoolerSingleSpeed testObject = EvaporativeFluidCoolerSingleSpeed(model);

  EXPECT_TRUE(testObject.isFanPoweratDesignAirFlowRateAutosized());

  testObject.setFanPoweratDesignAirFlowRate(999.0);
  EXPECT_DOUBLE_EQ(999.0, testObject.fanPoweratDesignAirFlowRate().get());

  EXPECT_FALSE(testObject.setFanPoweratDesignAirFlowRate(0.0));
  EXPECT_DOUBLE_EQ(999.0, testObject.fanPoweratDesignAirFlowRate().get());

  EXPECT_FALSE(testObject.setFanPoweratDesignAirFlowRate(-1.0));

  testObject.autosizeFanPoweratDesignAirFlowRate();
  EXPECT_TRUE(testObject.isFanPoweratDesignAirFlowRateAutosized());
}

TEST_F(ModelFixture, EvaporativeFluidCoolerSingleSpeed_DesignSprayWaterFlowRate) 
{
  Model model;
  EvaporativeFluidCoolerSingleSpeed testObject = EvaporativeFluidCoolerSingleSpeed(model);

  EXPECT_DOUBLE_EQ(0.03, testObject.designSprayWaterFlowRate());

  testObject.setDesignSprayWaterFlowRate(999.0);
  EXPECT_DOUBLE_EQ(999.0, testObject.designSprayWaterFlowRate());

  EXPECT_FALSE(testObject.setDesignSprayWaterFlowRate(0.0));
  EXPECT_DOUBLE_EQ(999.0, testObject.designSprayWaterFlowRate());

  EXPECT_FALSE(testObject.setDesignSprayWaterFlowRate(-1.0));
}

TEST_F(ModelFixture, EvaporativeFluidCoolerSingleSpeed_PerformanceInputMethod) 
{
  Model model;
  EvaporativeFluidCoolerSingleSpeed testObject = EvaporativeFluidCoolerSingleSpeed(model);

  EXPECT_EQ("UFactorTimesAreaAndDesignWaterFlowRate", testObject.performanceInputMethod().get());

  testObject.setPerformanceInputMethod("StandardDesignCapacity");
  EXPECT_EQ("StandardDesignCapacity", testObject.performanceInputMethod().get());

  testObject.setPerformanceInputMethod("UserSpecifiedDesignCapacity");
  EXPECT_EQ("UserSpecifiedDesignCapacity", testObject.performanceInputMethod().get());

  EXPECT_FALSE(testObject.setPerformanceInputMethod("Not Valid Entry"));
}

TEST_F(ModelFixture, EvaporativeFluidCoolerSingleSpeed_CapacityControl) 
{
  Model model;
  EvaporativeFluidCoolerSingleSpeed testObject = EvaporativeFluidCoolerSingleSpeed(model);

  EXPECT_EQ("FanCycling", testObject.capacityControl());

  testObject.setCapacityControl("FluidBypass");
  EXPECT_EQ("FluidBypass", testObject.capacityControl());

  EXPECT_FALSE(testObject.setCapacityControl("Not Valid Entry"));
}

TEST_F(ModelFixture, EvaporativeFluidCoolerSingleSpeed_EvaporationLossMode) 
{
  Model model;
  EvaporativeFluidCoolerSingleSpeed testObject = EvaporativeFluidCoolerSingleSpeed(model);

  EXPECT_EQ("SaturatedExit", testObject.evaporationLossMode());

  testObject.setEvaporationLossMode("LossFactor");
  EXPECT_EQ("LossFactor", testObject.evaporationLossMode());

  EXPECT_FALSE(testObject.setEvaporationLossMode("Not Valid Entry"));
}

TEST_F(ModelFixture, EvaporativeFluidCoolerSingleSpeed_BlowdownCalculationMode) 
{
  Model model;
  EvaporativeFluidCoolerSingleSpeed testObject = EvaporativeFluidCoolerSingleSpeed(model);

  EXPECT_EQ("ConcentrationRatio", testObject.blowdownCalculationMode());

  testObject.setBlowdownCalculationMode("ScheduledRate");
  EXPECT_EQ("ScheduledRate", testObject.blowdownCalculationMode());

  EXPECT_FALSE(testObject.setBlowdownCalculationMode("Not Valid Entry"));
}

TEST_F(ModelFixture, EvaporativeFluidCoolerSingleSpeed_BlowdownMakeupWaterUsageSchedule) 
{
  Model model;
  EvaporativeFluidCoolerSingleSpeed testObject = EvaporativeFluidCoolerSingleSpeed(model);

  EXPECT_FALSE(testObject.blowdownMakeupWaterUsageSchedule());

  Schedule alwaysOn = model.alwaysOnDiscreteSchedule();

  ScheduleCompact alwaysOnSchedule(model);
  alwaysOnSchedule.setName("ALWAYS_ON");
  alwaysOnSchedule.setString(3,"Through: 12/31");
  alwaysOnSchedule.setString(4,"For: AllDays");
  alwaysOnSchedule.setString(5,"Until: 24:00");
  alwaysOnSchedule.setString(6,"1");

  EXPECT_FALSE(testObject.setBlowdownMakeupWaterUsageSchedule(alwaysOn));
  EXPECT_FALSE(testObject.blowdownMakeupWaterUsageSchedule());

  EXPECT_TRUE(testObject.setBlowdownMakeupWaterUsageSchedule(alwaysOnSchedule));
  EXPECT_TRUE(testObject.blowdownMakeupWaterUsageSchedule());
  EXPECT_EQ(alwaysOnSchedule, testObject.blowdownMakeupWaterUsageSchedule().get());
}


