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

#include "../RefrigerationCondenserEvaporativeCooled.hpp"
#include "../RefrigerationCondenserEvaporativeCooled_Impl.hpp"
#include "../Schedule.hpp"
#include "../Schedule_Impl.hpp"
#include "../ScheduleTypeLimits.hpp"
#include "../ScheduleTypeRegistry.hpp"

using namespace openstudio;
using namespace openstudio::model;

//Test construction of evaporative cooled condenser
TEST_F(ModelFixture, RefrigerationCondenserEvaporativeCooled_DefaultConstructor)
{
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  ASSERT_EXIT ( 
  {  
    Model model;
    RefrigerationCondenserEvaporativeCooled testObject = RefrigerationCondenserEvaporativeCooled(model);

    exit(0); 
  } ,
    ::testing::ExitedWithCode(0), "" );
}

//Test removal of evaporative cooled condenser
TEST_F(ModelFixture, RefrigerationCondenserEvaporativeCooled_Remove)
{
  Model model;
  RefrigerationCondenserEvaporativeCooled testObject = RefrigerationCondenserEvaporativeCooled(model);

  std::vector<RefrigerationCondenserEvaporativeCooled> refrigerationEvaporativeCooledCondensers = model.getModelObjects<RefrigerationCondenserEvaporativeCooled>();
  EXPECT_EQ(1, refrigerationEvaporativeCooledCondensers.size());

  testObject.remove();

  refrigerationEvaporativeCooledCondensers = model.getModelObjects<RefrigerationCondenserEvaporativeCooled>();
  EXPECT_EQ(0, refrigerationEvaporativeCooledCondensers.size());
}

//Test the methods that set and get the fields
TEST_F(ModelFixture,RefrigerationCondenserEvaporativeCooled_set_get)
{
  Model model;

  RefrigerationCondenserEvaporativeCooled evapcond(model);

  evapcond.setRatedEffectiveTotalHeatRejectionRate(999.0);
  evapcond.setRatedSubcoolingTemperatureDifference(999.0);
  evapcond.setFanSpeedControlType("VariableSpeed");
  evapcond.setRatedFanPower(999.0);
  evapcond.setMinimumFanAirFlowRatio(0.999);
  evapcond.setApproachTemperatureConstantTerm(9.0);
  evapcond.setApproachTemperatureCoefficient2(9.0);
  evapcond.setApproachTemperatureCoefficient3(9.0);
  evapcond.setApproachTemperatureCoefficient4(9.0);
  evapcond.setMinimumCapacityFactor(0.9);
  evapcond.setMaximumCapacityFactor(9.0);
  evapcond.setRatedAirFlowRate(999.0);
  evapcond.setBasinHeaterCapacity(999.0);
  evapcond.setBasinHeaterSetpointTemperature(999.0);
  evapcond.setRatedWaterPumpPower(999.0);
  evapcond.setEndUseSubcategory("EndUseCat");
  evapcond.setCondenserRefrigerantOperatingChargeInventory(999.0);
  evapcond.setCondensateReceiverRefrigerantInventory(999.0);
  evapcond.setCondensatePipingRefrigerantInventory(999.0);

  EXPECT_DOUBLE_EQ(evapcond.ratedEffectiveTotalHeatRejectionRate(),999.0);
  EXPECT_DOUBLE_EQ(evapcond.ratedSubcoolingTemperatureDifference(),999.0);
  EXPECT_EQ(evapcond.fanSpeedControlType(),"VariableSpeed");
  EXPECT_DOUBLE_EQ(evapcond.ratedFanPower(),999.0);
  EXPECT_DOUBLE_EQ(evapcond.minimumFanAirFlowRatio(),0.999);
  EXPECT_DOUBLE_EQ(evapcond.approachTemperatureConstantTerm(),9.0);
  EXPECT_DOUBLE_EQ(evapcond.approachTemperatureCoefficient2(),9.0);
  EXPECT_DOUBLE_EQ(evapcond.approachTemperatureCoefficient3(),9.0);
  EXPECT_DOUBLE_EQ(evapcond.approachTemperatureCoefficient4(),9.0);
  EXPECT_DOUBLE_EQ(evapcond.minimumCapacityFactor(),0.9);
  EXPECT_DOUBLE_EQ(evapcond.maximumCapacityFactor(),9.0);
  EXPECT_DOUBLE_EQ(evapcond.ratedAirFlowRate().get(),999.0);
  EXPECT_DOUBLE_EQ(evapcond.basinHeaterCapacity(),999.0);
  EXPECT_DOUBLE_EQ(evapcond.basinHeaterSetpointTemperature(),999.0);
  EXPECT_DOUBLE_EQ(evapcond.ratedWaterPumpPower().get(),999.0);
  EXPECT_EQ(evapcond.endUseSubcategory(),"EndUseCat");
  EXPECT_DOUBLE_EQ(evapcond.condenserRefrigerantOperatingChargeInventory(),999.0);
  EXPECT_DOUBLE_EQ(evapcond.condensateReceiverRefrigerantInventory(),999.0);
  EXPECT_DOUBLE_EQ(evapcond.condensatePipingRefrigerantInventory(),999.0);
}

//Test RatedEffectiveTotalHeatRejectionRate
TEST_F(ModelFixture,RefrigerationCondenserEvaporativeCooled_RatedEffectiveTotalHeatRejectionRate)
{
  Model model;

  RefrigerationCondenserEvaporativeCooled evapcond(model);

//     \minimum 0.0

  EXPECT_DOUBLE_EQ(evapcond.ratedEffectiveTotalHeatRejectionRate(), 43100.0);

  EXPECT_TRUE(evapcond.setRatedEffectiveTotalHeatRejectionRate(10));     // valid value
  EXPECT_DOUBLE_EQ(evapcond.ratedEffectiveTotalHeatRejectionRate(), 10);

  EXPECT_FALSE(evapcond.setRatedEffectiveTotalHeatRejectionRate(-10));     // invalid value
}

//Test RatedSubcoolingTemperatureDifference
TEST_F(ModelFixture,RefrigerationCondenserEvaporativeCooled_RatedSubcoolingTemperatureDifference)
{
  Model model;

  RefrigerationCondenserEvaporativeCooled evapcond(model);

//     \minimum 0.0
//     \default 0.0

  EXPECT_DOUBLE_EQ(evapcond.ratedSubcoolingTemperatureDifference(), 0.0);     // IDD default value

  EXPECT_TRUE(evapcond.setRatedSubcoolingTemperatureDifference(10));     // valid value
  EXPECT_DOUBLE_EQ(evapcond.ratedSubcoolingTemperatureDifference(), 10);

  EXPECT_FALSE(evapcond.setRatedSubcoolingTemperatureDifference(-10));     // invalid value

  evapcond.setRatedSubcoolingTemperatureDifference(10.0);
  evapcond.resetRatedSubcoolingTemperatureDifference();
  EXPECT_DOUBLE_EQ(evapcond.ratedSubcoolingTemperatureDifference(), 0.0);     // IDD default value
}

//Test FanSpeedControlType
TEST_F(ModelFixture,RefrigerationCondenserEvaporativeCooled_FanSpeedControlType)
{
  Model model;

  RefrigerationCondenserEvaporativeCooled evapcond(model);

//     \key Fixed
//     \key FixedLinear
//     \key VariableSpeed
//     \key TwoSpeed
//     \default Fixed

  EXPECT_EQ(evapcond.fanSpeedControlType(), "VariableSpeed");     // IDD default value

  EXPECT_TRUE(evapcond.setFanSpeedControlType("Fixed"));     // valid value
  EXPECT_EQ(evapcond.fanSpeedControlType(), "Fixed");
  EXPECT_TRUE(evapcond.setFanSpeedControlType("FixedLinear"));     // valid value
  EXPECT_EQ(evapcond.fanSpeedControlType(), "FixedLinear");
  EXPECT_TRUE(evapcond.setFanSpeedControlType("VariableSpeed"));     // valid value
  EXPECT_EQ(evapcond.fanSpeedControlType(), "VariableSpeed");
  EXPECT_TRUE(evapcond.setFanSpeedControlType("TwoSpeed"));     // valid value
  EXPECT_EQ(evapcond.fanSpeedControlType(), "TwoSpeed");

  EXPECT_FALSE(evapcond.setFanSpeedControlType("invalid"));     // invalid value

  evapcond.setFanSpeedControlType("TwoSpeed");
  evapcond.resetFanSpeedControlType();
  EXPECT_EQ(evapcond.fanSpeedControlType(), "Fixed");     // IDD default value
}

//Test RatedFanPower
TEST_F(ModelFixture,RefrigerationCondenserEvaporativeCooled_RatedFanPower)
{
  Model model;

  RefrigerationCondenserEvaporativeCooled evapcond(model);

//     \minimum 0.0

  EXPECT_DOUBLE_EQ(evapcond.ratedFanPower(), 373.0);

  EXPECT_TRUE(evapcond.setRatedFanPower(10));     // valid value
  EXPECT_DOUBLE_EQ(evapcond.ratedFanPower(), 10);

  EXPECT_FALSE(evapcond.setRatedFanPower(-10));     // invalid value
}

//Test MinimumFanAirFlowRatio
TEST_F(ModelFixture,RefrigerationCondenserEvaporativeCooled_MinimumFanAirFlowRatio)
{
  Model model;

  RefrigerationCondenserEvaporativeCooled evapcond(model);

//     \minimum 0.0
//     \default 0.2

  EXPECT_DOUBLE_EQ(evapcond.minimumFanAirFlowRatio(), 0.2);     // IDD default value

  EXPECT_TRUE(evapcond.setMinimumFanAirFlowRatio(0.5));     // valid value
  EXPECT_DOUBLE_EQ(evapcond.minimumFanAirFlowRatio(), 0.5);

  EXPECT_FALSE(evapcond.setMinimumFanAirFlowRatio(-0.1));     // invalid value

  evapcond.setMinimumFanAirFlowRatio(0.5);
  evapcond.resetMinimumFanAirFlowRatio();
  EXPECT_DOUBLE_EQ(evapcond.minimumFanAirFlowRatio(), 0.2);     // IDD default value
}

//Test ApproachTemperatureConstantTerm
TEST_F(ModelFixture,RefrigerationCondenserEvaporativeCooled_ApproachTemperatureConstantTerm)
{
  Model model;

  RefrigerationCondenserEvaporativeCooled evapcond(model);

//     \minimum 0.0
//     \maximum 20.0
//     \default 6.63

  EXPECT_DOUBLE_EQ(evapcond.approachTemperatureConstantTerm(), 6.63);     // IDD default value

  EXPECT_TRUE(evapcond.setApproachTemperatureConstantTerm(10));     // valid value
  EXPECT_DOUBLE_EQ(evapcond.approachTemperatureConstantTerm(), 10);

  EXPECT_FALSE(evapcond.setApproachTemperatureConstantTerm(-10));     // invalid value
  EXPECT_FALSE(evapcond.setApproachTemperatureConstantTerm(30));     // invalid value

  EXPECT_FALSE(evapcond.setApproachTemperatureConstantTerm(-0.1));     // boundary check
  EXPECT_FALSE(evapcond.setApproachTemperatureConstantTerm(20.1));     // boundary check

  evapcond.setApproachTemperatureConstantTerm(10.0);
  evapcond.resetApproachTemperatureConstantTerm();
  EXPECT_DOUBLE_EQ(evapcond.approachTemperatureConstantTerm(), 6.63);     // IDD default value
}
//Test ApproachTemperatureCoefficient2
TEST_F(ModelFixture,RefrigerationCondenserEvaporativeCooled_ApproachTemperatureCoefficient2)
{
  Model model;

  RefrigerationCondenserEvaporativeCooled evapcond(model);

//     \minimum 0.0
//     \maximum 20.0
//     \default 0.468

  EXPECT_DOUBLE_EQ(evapcond.approachTemperatureCoefficient2(), 0.468);     // IDD default value

  EXPECT_TRUE(evapcond.setApproachTemperatureCoefficient2(10));     // valid value
  EXPECT_DOUBLE_EQ(evapcond.approachTemperatureCoefficient2(), 10);

  EXPECT_FALSE(evapcond.setApproachTemperatureCoefficient2(-10));     // invalid value
  EXPECT_FALSE(evapcond.setApproachTemperatureCoefficient2(30));     // invalid value

  EXPECT_FALSE(evapcond.setApproachTemperatureCoefficient2(-0.1));     // boundary check
  EXPECT_FALSE(evapcond.setApproachTemperatureCoefficient2(20.1));     // boundary check

  evapcond.setApproachTemperatureCoefficient2(10.0);
  evapcond.resetApproachTemperatureCoefficient2();
  EXPECT_DOUBLE_EQ(evapcond.approachTemperatureCoefficient2(), 0.468);     // IDD default value
}
//Test ApproachTemperatureCoefficient3
TEST_F(ModelFixture,RefrigerationCondenserEvaporativeCooled_ApproachTemperatureCoefficient3)
{
  Model model;

  RefrigerationCondenserEvaporativeCooled evapcond(model);

//     \minimum 0.0
//     \maximum 30.0
//     \default 17.93

  EXPECT_DOUBLE_EQ(evapcond.approachTemperatureCoefficient3(), 17.93);     // IDD default value

  EXPECT_TRUE(evapcond.setApproachTemperatureCoefficient3(10));     // valid value
  EXPECT_DOUBLE_EQ(evapcond.approachTemperatureCoefficient3(), 10);

  EXPECT_FALSE(evapcond.setApproachTemperatureCoefficient3(-10));     // invalid value
  EXPECT_FALSE(evapcond.setApproachTemperatureCoefficient3(40));     // invalid value

  EXPECT_FALSE(evapcond.setApproachTemperatureCoefficient3(-0.1));     // boundary check
  EXPECT_FALSE(evapcond.setApproachTemperatureCoefficient3(30.1));     // boundary check

  evapcond.setApproachTemperatureCoefficient3(10.0);
  evapcond.resetApproachTemperatureCoefficient3();
  EXPECT_DOUBLE_EQ(evapcond.approachTemperatureCoefficient3(), 17.93);     // IDD default value
}
//Test ApproachTemperatureCoefficient4
TEST_F(ModelFixture,RefrigerationCondenserEvaporativeCooled_ApproachTemperatureCoefficient4)
{
  Model model;

  RefrigerationCondenserEvaporativeCooled evapcond(model);

//        \minimum -20.0
//        \maximum 20.0
//        \default -0.322

  EXPECT_DOUBLE_EQ(evapcond.approachTemperatureCoefficient4(), -0.322);     // IDD default value

  EXPECT_TRUE(evapcond.setApproachTemperatureCoefficient4(10));     // valid value
  EXPECT_DOUBLE_EQ(evapcond.approachTemperatureCoefficient4(), 10);

  EXPECT_FALSE(evapcond.setApproachTemperatureCoefficient4(-30));     // invalid value
  EXPECT_FALSE(evapcond.setApproachTemperatureCoefficient4(30));     // invalid value

  EXPECT_FALSE(evapcond.setApproachTemperatureCoefficient4(-20.1));     // boundary check
  EXPECT_FALSE(evapcond.setApproachTemperatureCoefficient4(20.1));     // boundary check

  evapcond.setApproachTemperatureCoefficient4(10.0);
  evapcond.resetApproachTemperatureCoefficient4();
  EXPECT_DOUBLE_EQ(evapcond.approachTemperatureCoefficient4(), -0.322);     // IDD default value
}

//Test MinimumCapacityFactor
TEST_F(ModelFixture,RefrigerationCondenserEvaporativeCooled_MinimumCapacityFactor)
{
  Model model;

  RefrigerationCondenserEvaporativeCooled evapcond(model);

//     \default 0.5

  EXPECT_DOUBLE_EQ(evapcond.minimumCapacityFactor(), 0.6);     // IDD default value

  evapcond.setMinimumCapacityFactor(2.0);
  evapcond.resetMinimumCapacityFactor();
  EXPECT_DOUBLE_EQ(evapcond.minimumCapacityFactor(), 0.5);     // IDD default value
}

//Test MaximumCapacityFactor
TEST_F(ModelFixture,RefrigerationCondenserEvaporativeCooled_MaximumCapacityFactor)
{
  Model model;

  RefrigerationCondenserEvaporativeCooled evapcond(model);

//     \default 5.0

  EXPECT_DOUBLE_EQ(evapcond.maximumCapacityFactor(), 4.8);     // IDD default value

  evapcond.setMaximumCapacityFactor(2.0);
  evapcond.resetMaximumCapacityFactor();
  EXPECT_DOUBLE_EQ(evapcond.maximumCapacityFactor(), 5.0);     // IDD default value
}

//Test BasinHeaterCapacity
TEST_F(ModelFixture,RefrigerationCondenserEvaporativeCooled_BasinHeaterCapacity)
{
  Model model;

  RefrigerationCondenserEvaporativeCooled evapcond(model);

//     \minimum 0.0
//     \default 200.0

  EXPECT_DOUBLE_EQ(evapcond.basinHeaterCapacity(), 66.7);     // IDD default value

  EXPECT_TRUE(evapcond.setBasinHeaterCapacity(999));     // valid value
  EXPECT_DOUBLE_EQ(evapcond.basinHeaterCapacity(), 999);

  EXPECT_FALSE(evapcond.setBasinHeaterCapacity(-0.1));     // invalid value

  evapcond.setBasinHeaterCapacity(500);
  evapcond.resetBasinHeaterCapacity();
  EXPECT_DOUBLE_EQ(evapcond.basinHeaterCapacity(), 200);     // IDD default value
}

//Test BasinHeaterSetpointTemperature
TEST_F(ModelFixture,RefrigerationCondenserEvaporativeCooled_BasinHeaterSetpointTemperature)
{
  Model model;

  RefrigerationCondenserEvaporativeCooled evapcond(model);

//     \minimum 2.0
//     \default 2.0

  EXPECT_DOUBLE_EQ(evapcond.basinHeaterSetpointTemperature(), 2.0);     // IDD default value

  EXPECT_TRUE(evapcond.setBasinHeaterSetpointTemperature(999));     // valid value
  EXPECT_DOUBLE_EQ(evapcond.basinHeaterSetpointTemperature(), 999);

  EXPECT_FALSE(evapcond.setBasinHeaterSetpointTemperature(1.0));     // invalid value

  evapcond.setBasinHeaterSetpointTemperature(5.0);
  evapcond.resetBasinHeaterSetpointTemperature();
  EXPECT_DOUBLE_EQ(evapcond.basinHeaterSetpointTemperature(), 2.0);     // IDD default value
}

//Test RatedWaterPumpPower
TEST_F(ModelFixture,RefrigerationCondenserEvaporativeCooled_RatedWaterPumpPower)
{
  Model model;

  RefrigerationCondenserEvaporativeCooled evapcond(model);

//     \default 1000.0

  EXPECT_DOUBLE_EQ(evapcond.ratedWaterPumpPower().get(), 250.0);     // IDD default value

  evapcond.setRatedWaterPumpPower(99);
  EXPECT_DOUBLE_EQ(evapcond.ratedWaterPumpPower().get(), 99.0);
  evapcond.autocalculateRatedWaterPumpPower();
  EXPECT_TRUE(evapcond.isRatedWaterPumpPowerAutocalculated());
  evapcond.resetRatedWaterPumpPower();
  EXPECT_DOUBLE_EQ(evapcond.ratedWaterPumpPower().get(), 1000.0);     // IDD default value
}

//Test EndUseSubcategory
TEST_F(ModelFixture,RefrigerationCondenserEvaporativeCooled_EndUseSubcategory)
{
  Model model;

  RefrigerationCondenserEvaporativeCooled evapcond(model);

//     \default General

  EXPECT_EQ(evapcond.endUseSubcategory(), "General");     // IDD default value

  evapcond.setEndUseSubcategory("Scratch");
  evapcond.resetEndUseSubcategory();
  EXPECT_EQ(evapcond.endUseSubcategory(), "General");     // IDD default value
}

//Test CondenserRefrigerantOperatingChargeInventory
TEST_F(ModelFixture,RefrigerationCondenserEvaporativeCooled_CondenserRefrigerantOperatingChargeInventory)
{
  Model model;

  RefrigerationCondenserEvaporativeCooled evapcond(model);

//     \default 0.0

  EXPECT_DOUBLE_EQ(evapcond.condenserRefrigerantOperatingChargeInventory(), 0.0);     // IDD default value

  evapcond.setCondenserRefrigerantOperatingChargeInventory(999);
  evapcond.resetCondenserRefrigerantOperatingChargeInventory();
  EXPECT_DOUBLE_EQ(evapcond.condenserRefrigerantOperatingChargeInventory(), 0.0);     // IDD default value
}

//Test CondensateReceiverRefrigerantInventory
TEST_F(ModelFixture,RefrigerationCondenserEvaporativeCooled_CondensateReceiverRefrigerantInventory)
{
  Model model;

  RefrigerationCondenserEvaporativeCooled evapcond(model);

//     \default 0.0

  EXPECT_DOUBLE_EQ(evapcond.condensateReceiverRefrigerantInventory(), 0.0);     // IDD default value

  evapcond.setCondensateReceiverRefrigerantInventory(999);
  evapcond.resetCondensateReceiverRefrigerantInventory();
  EXPECT_DOUBLE_EQ(evapcond.condensateReceiverRefrigerantInventory(), 0.0);     // IDD default value
}

//Test CondensatePipingRefrigerantInventory
TEST_F(ModelFixture,RefrigerationCondenserEvaporativeCooled_CondensatePipingRefrigerantInventory)
{
  Model model;

  RefrigerationCondenserEvaporativeCooled evapcond(model);

//     \default 0.0

  EXPECT_DOUBLE_EQ(evapcond.condensatePipingRefrigerantInventory(), 0.0);     // IDD default value

  evapcond.setCondensatePipingRefrigerantInventory(999);
  evapcond.resetCondensatePipingRefrigerantInventory();
  EXPECT_DOUBLE_EQ(evapcond.condensatePipingRefrigerantInventory(), 0.0);     // IDD default value
}

//Test clone of Evaporative Cooled Condenser with default data
TEST_F(ModelFixture, RefrigerationCondenserEvaporativeCooled_CloneOneModelWithDefaultData)
{
  Model model; 
  RefrigerationCondenserEvaporativeCooled testObject = RefrigerationCondenserEvaporativeCooled(model);

  RefrigerationCondenserEvaporativeCooled testObjectClone = testObject.clone(model).cast<RefrigerationCondenserEvaporativeCooled>();

  EXPECT_DOUBLE_EQ(testObjectClone.ratedSubcoolingTemperatureDifference(),0.0);
  EXPECT_DOUBLE_EQ(testObjectClone.ratedEffectiveTotalHeatRejectionRate(),43100.0);
  EXPECT_EQ(testObjectClone.fanSpeedControlType(),"VariableSpeed");
  EXPECT_DOUBLE_EQ(testObjectClone.minimumFanAirFlowRatio(),0.2);
  EXPECT_DOUBLE_EQ(testObjectClone.approachTemperatureConstantTerm(),6.63);
  EXPECT_DOUBLE_EQ(testObjectClone.approachTemperatureCoefficient2(),0.468);
  EXPECT_DOUBLE_EQ(testObjectClone.approachTemperatureCoefficient3(),17.93);
  EXPECT_DOUBLE_EQ(testObjectClone.approachTemperatureCoefficient4(),-0.322);
  EXPECT_DOUBLE_EQ(testObjectClone.minimumCapacityFactor(),0.6);
  EXPECT_DOUBLE_EQ(testObjectClone.maximumCapacityFactor(),4.8);
  EXPECT_DOUBLE_EQ(testObjectClone.basinHeaterCapacity(),66.7);
  EXPECT_DOUBLE_EQ(testObjectClone.basinHeaterSetpointTemperature(),2.0);
  EXPECT_DOUBLE_EQ(testObjectClone.ratedWaterPumpPower().get(),250.0);
  EXPECT_DOUBLE_EQ(testObjectClone.ratedAirFlowRate().get(),1.37);
}

TEST_F(ModelFixture, RefrigerationCondenserEvaporativeCooled_CloneOneModelWithCustomData)
{
  Model model;
  Schedule evaporativeCondenserSchedule = model.alwaysOnDiscreteSchedule();
  RefrigerationCondenserEvaporativeCooled testObject = RefrigerationCondenserEvaporativeCooled(model);

  testObject.setRatedEffectiveTotalHeatRejectionRate(999.0);
  testObject.setRatedSubcoolingTemperatureDifference(999.0);
  testObject.setFanSpeedControlType("Fixed");
  testObject.setRatedFanPower(999.0);
  testObject.setMinimumFanAirFlowRatio(0.999);
  testObject.setApproachTemperatureConstantTerm(9.0);
  testObject.setApproachTemperatureCoefficient2(9.0);
  testObject.setApproachTemperatureCoefficient3(9.0);
  testObject.setApproachTemperatureCoefficient4(9.0);
  testObject.setMinimumCapacityFactor(0.9);
  testObject.setMaximumCapacityFactor(9.0);
  testObject.autocalculateRatedAirFlowRate();
  testObject.setBasinHeaterCapacity(999.0);
  testObject.setBasinHeaterSetpointTemperature(999.0);
  testObject.autocalculateRatedWaterPumpPower();
  testObject.setEvaporativeCondenserAvailabilitySchedule(evaporativeCondenserSchedule);
  testObject.setEndUseSubcategory("EndUseCat");
  testObject.setCondenserRefrigerantOperatingChargeInventory(999.0);
  testObject.setCondensateReceiverRefrigerantInventory(999.0);
  testObject.setCondensatePipingRefrigerantInventory(999.0);

  RefrigerationCondenserEvaporativeCooled testObjectClone = testObject.clone(model).cast<RefrigerationCondenserEvaporativeCooled>();

  EXPECT_DOUBLE_EQ(testObjectClone.ratedEffectiveTotalHeatRejectionRate(),999.0);
  EXPECT_DOUBLE_EQ(testObjectClone.ratedSubcoolingTemperatureDifference(),999.0);
  EXPECT_EQ(testObjectClone.fanSpeedControlType(),"Fixed");
  EXPECT_DOUBLE_EQ(testObjectClone.ratedFanPower(),999.0);
  EXPECT_DOUBLE_EQ(testObjectClone.minimumFanAirFlowRatio(),0.999);
  EXPECT_DOUBLE_EQ(testObjectClone.approachTemperatureConstantTerm(),9.0);
  EXPECT_DOUBLE_EQ(testObjectClone.approachTemperatureCoefficient2(),9.0);
  EXPECT_DOUBLE_EQ(testObjectClone.approachTemperatureCoefficient3(),9.0);
  EXPECT_DOUBLE_EQ(testObjectClone.approachTemperatureCoefficient4(),9.0);
  EXPECT_DOUBLE_EQ(testObjectClone.minimumCapacityFactor(),0.9);
  EXPECT_DOUBLE_EQ(testObjectClone.maximumCapacityFactor(),9.0);
  EXPECT_TRUE(testObjectClone.isRatedAirFlowRateAutocalculated());
  EXPECT_DOUBLE_EQ(testObjectClone.basinHeaterCapacity(),999.0);
  EXPECT_DOUBLE_EQ(testObjectClone.basinHeaterSetpointTemperature(),999.0);
  EXPECT_TRUE(testObjectClone.isRatedWaterPumpPowerAutocalculated());
  EXPECT_EQ(testObjectClone.endUseSubcategory(),"EndUseCat");
  EXPECT_DOUBLE_EQ(testObjectClone.condenserRefrigerantOperatingChargeInventory(),999.0);
  EXPECT_DOUBLE_EQ(testObjectClone.condensateReceiverRefrigerantInventory(),999.0);
  EXPECT_DOUBLE_EQ(testObjectClone.condensatePipingRefrigerantInventory(),999.0);

  EXPECT_EQ(testObject.evaporativeCondenserAvailabilitySchedule().get().handle(), testObjectClone.evaporativeCondenserAvailabilitySchedule().get().handle());
  EXPECT_EQ(evaporativeCondenserSchedule.handle(), testObjectClone.evaporativeCondenserAvailabilitySchedule().get().handle());
}

TEST_F(ModelFixture, RefrigerationCondenserEvaporativeCooled_CloneTwoModelsWithDefaultData)
{
  Model model;
  RefrigerationCondenserEvaporativeCooled testObject = RefrigerationCondenserEvaporativeCooled(model);
  RefrigerationCondenserEvaporativeCooled testObjectClone = testObject.clone(model).cast<RefrigerationCondenserEvaporativeCooled>();

  Model model2;
  RefrigerationCondenserEvaporativeCooled testObjectClone2 = testObject.clone(model2).cast<RefrigerationCondenserEvaporativeCooled>();

  EXPECT_DOUBLE_EQ(testObjectClone2.ratedSubcoolingTemperatureDifference(),0.0);
  EXPECT_DOUBLE_EQ(testObjectClone2.ratedEffectiveTotalHeatRejectionRate(),43100.0);
  EXPECT_EQ(testObjectClone2.fanSpeedControlType(),"VariableSpeed");
  EXPECT_DOUBLE_EQ(testObjectClone2.minimumFanAirFlowRatio(),0.2);
  EXPECT_DOUBLE_EQ(testObjectClone2.approachTemperatureConstantTerm(),6.63);
  EXPECT_DOUBLE_EQ(testObjectClone2.approachTemperatureCoefficient2(),0.468);
  EXPECT_DOUBLE_EQ(testObjectClone2.approachTemperatureCoefficient3(),17.93);
  EXPECT_DOUBLE_EQ(testObjectClone2.approachTemperatureCoefficient4(),-0.322);
  EXPECT_DOUBLE_EQ(testObjectClone2.minimumCapacityFactor(),0.6);
  EXPECT_DOUBLE_EQ(testObjectClone2.maximumCapacityFactor(),4.8);
  EXPECT_DOUBLE_EQ(testObjectClone2.basinHeaterCapacity(),66.7);
  EXPECT_DOUBLE_EQ(testObjectClone2.basinHeaterSetpointTemperature(),2.0);
  EXPECT_DOUBLE_EQ(testObjectClone2.ratedWaterPumpPower().get(),250.0);
  EXPECT_DOUBLE_EQ(testObjectClone2.ratedAirFlowRate().get(),1.37);
  EXPECT_NE(testObjectClone2, testObjectClone);
  EXPECT_NE(testObjectClone2.handle(), testObjectClone.handle());
}
