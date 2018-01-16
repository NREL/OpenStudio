/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2018, Alliance for Sustainable Energy, LLC. All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 *  following conditions are met:
 *
 *  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
 *  disclaimer.
 *
 *  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *  following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 *  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote
 *  products derived from this software without specific prior written permission from the respective party.
 *
 *  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative
 *  works may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without
 *  specific prior written permission from Alliance for Sustainable Energy, LLC.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER, THE UNITED STATES GOVERNMENT, OR ANY CONTRIBUTORS BE LIABLE FOR
 *  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **********************************************************************************************************************/

#include <gtest/gtest.h>

#include "ModelFixture.hpp"

#include "../EvaporativeFluidCoolerSingleSpeed.hpp"
#include "../EvaporativeFluidCoolerSingleSpeed_Impl.hpp"

#include "../PlantLoop.hpp"
#include "../PlantLoop_Impl.hpp"
#include "../Splitter.hpp"
#include "../Splitter_Impl.hpp"
#include "../Node.hpp"
#include "../Node_Impl.hpp"
#include "../AirLoopHVAC.hpp"
#include "../AirLoopHVAC_Impl.hpp"
#include "../AirLoopHVACZoneSplitter.hpp"
#include "../AirLoopHVACZoneSplitter_Impl.hpp"
#include "../ConnectorMixer.hpp"
#include "../ConnectorMixer_Impl.hpp"
#include "../ConnectorSplitter.hpp"
#include "../ConnectorSplitter_Impl.hpp"
#include "../ThermalZone.hpp"
#include "../ThermalZone_Impl.hpp"
#include "../Schedule.hpp"
#include "../Schedule_Impl.hpp"
#include "../ScheduleCompact.hpp"
#include "../ScheduleCompact_Impl.hpp"

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
  Model m;
  EvaporativeFluidCoolerSingleSpeed testObject(m);

  AirLoopHVAC airLoop(m);

  Node supplyOutletNode = airLoop.supplyOutletNode();

  EXPECT_FALSE(testObject.addToNode(supplyOutletNode));
  EXPECT_EQ( (unsigned)2, airLoop.supplyComponents().size() );

  Node inletNode = airLoop.zoneSplitter().lastOutletModelObject()->cast<Node>();

  EXPECT_FALSE(testObject.addToNode(inletNode));
  EXPECT_EQ((unsigned)5, airLoop.demandComponents().size());

  PlantLoop plantLoop(m);
  supplyOutletNode = plantLoop.supplyOutletNode();
  EXPECT_TRUE(testObject.addToNode(supplyOutletNode));
  EXPECT_EQ( (unsigned)7, plantLoop.supplyComponents().size() );

  Node demandOutletNode = plantLoop.demandOutletNode();
  EXPECT_FALSE(testObject.addToNode(demandOutletNode));
  EXPECT_EQ( (unsigned)5, plantLoop.demandComponents().size() );

  EvaporativeFluidCoolerSingleSpeed testObjectClone = testObject.clone(m).cast<EvaporativeFluidCoolerSingleSpeed>();
  supplyOutletNode = plantLoop.supplyOutletNode();

  EXPECT_TRUE(testObjectClone.addToNode(supplyOutletNode));
  EXPECT_EQ( (unsigned)9, plantLoop.supplyComponents().size() );
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
  EXPECT_TRUE(testObject.addToNode(inletNode));
  EXPECT_EQ((unsigned)7, plantLoop.supplyComponents().size());
  inletNode = plantLoop.supplySplitter().lastOutletModelObject()->cast<Node>();
  EXPECT_TRUE(testObject2.addToNode(inletNode));
  EXPECT_EQ((unsigned)9, plantLoop.supplyComponents().size());
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


