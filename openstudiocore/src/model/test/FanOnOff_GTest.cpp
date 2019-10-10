/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2019, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
*  following conditions are met:
*
*  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
*  disclaimer.
*
*  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
*  disclaimer in the documentation and/or other materials provided with the distribution.
*
*  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote products
*  derived from this software without specific prior written permission from the respective party.
*
*  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative works
*  may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without specific prior
*  written permission from Alliance for Sustainable Energy, LLC.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER(S) AND ANY CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
*  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
*  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER(S), ANY CONTRIBUTORS, THE UNITED STATES GOVERNMENT, OR THE UNITED
*  STATES DEPARTMENT OF ENERGY, NOR ANY OF THEIR EMPLOYEES, BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
*  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
*  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
*  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
*  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
***********************************************************************************************************************/

#include <gtest/gtest.h>
#include "ModelFixture.hpp"
#include "../FanOnOff.hpp"
#include "../FanOnOff_Impl.hpp"
#include "../AirLoopHVAC.hpp"
#include "../PlantLoop.hpp"
#include "../Node.hpp"
#include "../Node_Impl.hpp"
#include "../AirLoopHVACZoneSplitter.hpp"
#include "../Schedule.hpp"
#include "../ScheduleConstant.hpp"
#include "../Curve.hpp"
#include "../Curve_Impl.hpp"
#include "../CurveQuadratic.hpp"
#include "../CurveQuadratic_Impl.hpp"
#include "../CurveCubic.hpp"
#include "../CurveCubic_Impl.hpp"
#include "../CurveExponent.hpp"
#include "../CurveExponent_Impl.hpp"

#include "../HVACComponent.hpp"
#include "../CurveBiquadratic.hpp"
#include "../CoilHeatingWater.hpp"
#include "../CoilHeatingElectric.hpp"
#include "../CoilCoolingWater.hpp"
#include "../CoilCoolingDXSingleSpeed.hpp"
#include "../CoilHeatingDXSingleSpeed.hpp"
#include "../CoilCoolingWaterToAirHeatPumpEquationFit.hpp"
#include "../CoilHeatingWaterToAirHeatPumpEquationFit.hpp"
#include "../ZoneHVACWaterToAirHeatPump.hpp"
#include "../ZoneHVACPackagedTerminalAirConditioner.hpp"
#include "../ZoneHVACFourPipeFanCoil.hpp"
#include "../ZoneHVACPackagedTerminalHeatPump.hpp"
#include "../ZoneHVACTerminalUnitVariableRefrigerantFlow.hpp"
#include "../ZoneHVACUnitHeater.hpp"
#include "../ZoneHVACBaseboardConvectiveElectric.hpp"
#include "../ZoneHVACLowTemperatureRadiantElectric.hpp"
#include "../AirLoopHVACUnitaryHeatPumpAirToAir.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, FanOnOff_DefaultConstructors)
{
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  ASSERT_EXIT (
  {
    Model m;
    Schedule s = m.alwaysOnDiscreteSchedule();
    FanOnOff testObject = FanOnOff(m, s);

    exit(0);
  } ,
    ::testing::ExitedWithCode(0), "" );

  ASSERT_EXIT (
  {
    Model m;
    Schedule s = m.alwaysOnDiscreteSchedule();
    CurveExponent fanPowerFuncSpeedCurve(m);
    CurveCubic fanEfficiencyFuncSpeedCurve(m);
    FanOnOff testObject = FanOnOff(m, s, fanPowerFuncSpeedCurve, fanEfficiencyFuncSpeedCurve);

    exit(0);
  } ,
    ::testing::ExitedWithCode(0), "" );

  ASSERT_EXIT (
  {
    Model m;
    Schedule s = m.alwaysOnDiscreteSchedule();
    CurveExponent fanPowerFuncSpeedCurve(m);
    CurveQuadratic fanEfficiencyFuncSpeedCurve(m);
    FanOnOff testObject = FanOnOff(m, s, fanPowerFuncSpeedCurve, fanEfficiencyFuncSpeedCurve);

    exit(0);
  } ,
    ::testing::ExitedWithCode(0), "" );
}

TEST_F(ModelFixture, FanOnOff_Remove)
{
  Model m;
  Schedule s = m.alwaysOnDiscreteSchedule();
  FanOnOff testObject = FanOnOff(m, s);

  std::vector<FanOnOff> fans = m.getModelObjects<FanOnOff>();
  EXPECT_EQ(1, fans.size());

  std::vector<CurveExponent> curveExponents = m.getModelObjects<CurveExponent>();
  EXPECT_EQ(1, curveExponents.size());

  std::vector<CurveCubic> curveCubics = m.getModelObjects<CurveCubic>();
  EXPECT_EQ(1, curveCubics.size());

  testObject.remove();

  fans = m.getModelObjects<FanOnOff>();
  EXPECT_EQ(0, fans.size());

  curveExponents = m.getModelObjects<CurveExponent>();
  EXPECT_EQ(1, curveExponents.size());

  curveCubics = m.getModelObjects<CurveCubic>();
  EXPECT_EQ(1, curveCubics.size());
}

TEST_F(ModelFixture,FanOnOff_addToNode)
{
  Model m;
  Schedule s = m.alwaysOnDiscreteSchedule();
  FanOnOff testObject = FanOnOff(m, s);

  AirLoopHVAC airLoop(m);

  Node supplyOutletNode = airLoop.supplyOutletNode();

  EXPECT_FALSE(testObject.addToNode(supplyOutletNode));
  EXPECT_EQ( (unsigned)2, airLoop.supplyComponents().size() );

  Node inletNode = airLoop.zoneSplitter().lastOutletModelObject()->cast<Node>();

  EXPECT_FALSE(testObject.addToNode(inletNode));
  EXPECT_EQ((unsigned)5, airLoop.demandComponents().size());

  PlantLoop plantLoop(m);
  supplyOutletNode = plantLoop.supplyOutletNode();
  EXPECT_FALSE(testObject.addToNode(supplyOutletNode));
  EXPECT_EQ( (unsigned)5, plantLoop.supplyComponents().size() );

  Node demandOutletNode = plantLoop.demandOutletNode();
  EXPECT_FALSE(testObject.addToNode(demandOutletNode));
  EXPECT_EQ( (unsigned)5, plantLoop.demandComponents().size() );
}

TEST_F(ModelFixture, FanOnOff_CloneOneModelWithDefaultData)
{
  Model m;
  Schedule s = m.alwaysOnDiscreteSchedule();
  FanOnOff testObject = FanOnOff(m, s);

  FanOnOff testObjectClone = testObject.clone(m).cast<FanOnOff>();

  std::vector<CurveExponent> powerCurves = m.getModelObjects<CurveExponent>();
  for(auto it = powerCurves.begin(); it != powerCurves.end(); ++it) {
    EXPECT_TRUE(it->parent());
  }

  std::vector<CurveCubic> efficiencyCurves = m.getModelObjects<CurveCubic>();
  for(auto it = efficiencyCurves.begin(); it != efficiencyCurves.end(); ++it) {
    EXPECT_TRUE(it->parent());
  }

  EXPECT_DOUBLE_EQ(0.6, testObjectClone.fanTotalEfficiency());
  EXPECT_DOUBLE_EQ(300, testObjectClone.pressureRise());
  EXPECT_DOUBLE_EQ(0.8, testObjectClone.motorEfficiency());
  EXPECT_TRUE(testObjectClone.isMaximumFlowRateAutosized());
  EXPECT_DOUBLE_EQ(1.0, testObjectClone.motorInAirstreamFraction().get());
  EXPECT_EQ(testObject.fanPowerRatioFunctionofSpeedRatioCurve().handle(), testObjectClone.fanPowerRatioFunctionofSpeedRatioCurve().handle());
  EXPECT_EQ(testObject.fanEfficiencyRatioFunctionofSpeedRatioCurve().handle(), testObjectClone.fanEfficiencyRatioFunctionofSpeedRatioCurve().handle());

  EXPECT_EQ(1, m.getModelObjects<CurveExponent>().size());
  EXPECT_EQ(1, m.getModelObjects<CurveCubic>().size());

  testObjectClone.remove();

  EXPECT_EQ(1, m.getModelObjects<CurveExponent>().size());
  EXPECT_EQ(1, m.getModelObjects<CurveCubic>().size());
}

TEST_F(ModelFixture, FanOnOff_CloneOneModelWithCustomData)
{
  Model m;
  Schedule s = m.alwaysOnDiscreteSchedule();
  FanOnOff testObject = FanOnOff(m, s);
  testObject.setPressureRise(999.0);
  testObject.setFanTotalEfficiency(0.99);
  testObject.setMaximumFlowRate(999.0);

  CurveExponent fanPowerFuncSpeedCurve(m);
  CurveCubic fanEfficiencyFuncSpeedCurve(m);

  testObject.setFanPowerRatioFunctionofSpeedRatioCurve(fanPowerFuncSpeedCurve);
  testObject.setFanEfficiencyRatioFunctionofSpeedRatioCurve(fanEfficiencyFuncSpeedCurve);

  FanOnOff testObjectClone = testObject.clone(m).cast<FanOnOff>();
  EXPECT_DOUBLE_EQ(999.0, testObjectClone.pressureRise());
  EXPECT_DOUBLE_EQ(0.99, testObjectClone.fanTotalEfficiency());
  EXPECT_DOUBLE_EQ(999.0, testObjectClone.maximumFlowRate().get());
  EXPECT_EQ(testObject.fanPowerRatioFunctionofSpeedRatioCurve().handle(), testObjectClone.fanPowerRatioFunctionofSpeedRatioCurve().handle());
  EXPECT_EQ(fanPowerFuncSpeedCurve.handle(), testObjectClone.fanPowerRatioFunctionofSpeedRatioCurve().handle());
  EXPECT_EQ(testObject.fanEfficiencyRatioFunctionofSpeedRatioCurve().handle(), testObjectClone.fanEfficiencyRatioFunctionofSpeedRatioCurve().handle());
  EXPECT_EQ(fanEfficiencyFuncSpeedCurve.handle(), testObjectClone.fanEfficiencyRatioFunctionofSpeedRatioCurve().handle());
}

TEST_F(ModelFixture, FanOnOff_CloneTwoModelsWithDefaultData)
{
  Model m;
  Schedule s = m.alwaysOnDiscreteSchedule();
  FanOnOff testObject = FanOnOff(m, s);

  FanOnOff testObjectClone = testObject.clone(m).cast<FanOnOff>();

  Model m2;

  std::vector<CurveExponent> powerCurves = m.getModelObjects<CurveExponent>();
  EXPECT_EQ(1, powerCurves.size());
  std::vector<CurveExponent> powerCurves2 = m2.getModelObjects<CurveExponent>();
  EXPECT_EQ(0, powerCurves2.size());

  std::vector<CurveCubic> efficiencyCurves = m.getModelObjects<CurveCubic>();
  EXPECT_EQ(1, efficiencyCurves.size());
  std::vector<CurveCubic> efficiencyCurves2 = m2.getModelObjects<CurveCubic>();
  EXPECT_EQ(0, efficiencyCurves2.size());

  FanOnOff testObjectClone2 = testObject.clone(m2).cast<FanOnOff>();

  powerCurves2 = m2.getModelObjects<CurveExponent>();
  EXPECT_EQ(1, powerCurves2.size());

  efficiencyCurves2 = m2.getModelObjects<CurveCubic>();
  EXPECT_EQ(1, efficiencyCurves2.size());


  for(auto it = powerCurves.begin(); it != powerCurves.end(); ++it) {
    EXPECT_TRUE(it->parent());
  }
  for(auto it = powerCurves2.begin(); it != powerCurves2.end(); ++it) {
    EXPECT_TRUE(it->parent());
  }

  for(auto it = efficiencyCurves.begin(); it != efficiencyCurves.end(); ++it) {
    EXPECT_TRUE(it->parent());
  }
  for(auto it = efficiencyCurves2.begin(); it != efficiencyCurves2.end(); ++it) {
    EXPECT_TRUE(it->parent());
  }

  EXPECT_DOUBLE_EQ(0.6, testObjectClone2.fanTotalEfficiency());
  EXPECT_DOUBLE_EQ(300, testObjectClone2.pressureRise());
  EXPECT_DOUBLE_EQ(0.8, testObjectClone2.motorEfficiency());
  EXPECT_TRUE(testObjectClone2.isMaximumFlowRateAutosized());
  EXPECT_DOUBLE_EQ(1.0, testObjectClone2.motorInAirstreamFraction().get());
  EXPECT_NE(testObject.fanPowerRatioFunctionofSpeedRatioCurve().handle(), testObjectClone2.fanPowerRatioFunctionofSpeedRatioCurve().handle());
  EXPECT_NE(testObject.fanEfficiencyRatioFunctionofSpeedRatioCurve().handle(), testObjectClone2.fanEfficiencyRatioFunctionofSpeedRatioCurve().handle());
  EXPECT_NE(testObjectClone2, testObjectClone);
  EXPECT_NE(testObjectClone2.handle(), testObjectClone.handle());
}

TEST_F(ModelFixture, FanOnOff_CloneTwoModelsWithCustomData)
{
  Model m;
  Schedule s = m.alwaysOnDiscreteSchedule();
  FanOnOff testObject = FanOnOff(m, s);
  testObject.setPressureRise(999.0);
  testObject.setFanTotalEfficiency(0.99);
  testObject.setMaximumFlowRate(999.0);

  CurveExponent fanPowerFuncSpeedCurve(m);
  CurveCubic fanEfficiencyFuncSpeedCurve(m);

  testObject.setFanPowerRatioFunctionofSpeedRatioCurve(fanPowerFuncSpeedCurve);
  testObject.setFanEfficiencyRatioFunctionofSpeedRatioCurve(fanEfficiencyFuncSpeedCurve);

  FanOnOff testObjectClone = testObject.clone(m).cast<FanOnOff>();

  Model m2;

  std::vector<CurveExponent> powerCurves = m.getModelObjects<CurveExponent>();
  EXPECT_EQ(2, powerCurves.size());
  std::vector<CurveExponent> powerCurves2 = m2.getModelObjects<CurveExponent>();
  EXPECT_EQ(0, powerCurves2.size());

  std::vector<CurveCubic> efficiencyCurves = m.getModelObjects<CurveCubic>();
  EXPECT_EQ(2, efficiencyCurves.size());
  std::vector<CurveCubic> efficiencyCurves2 = m2.getModelObjects<CurveCubic>();
  EXPECT_EQ(0, efficiencyCurves2.size());

  FanOnOff testObjectClone2 = testObject.clone(m2).cast<FanOnOff>();

  powerCurves2 = m2.getModelObjects<CurveExponent>();
  EXPECT_EQ(1, powerCurves2.size());

  efficiencyCurves2 = m2.getModelObjects<CurveCubic>();
  EXPECT_EQ(1, efficiencyCurves2.size());


  for(auto it = powerCurves.begin(); it != powerCurves.end(); ++it) {
    if (testObject.fanPowerRatioFunctionofSpeedRatioCurve().handle() == it->handle()) {
      EXPECT_TRUE(it->parent());
    }
    else if (testObjectClone.fanPowerRatioFunctionofSpeedRatioCurve().handle() == it->handle()) {
      EXPECT_TRUE(it->parent());
    }
    else {
      EXPECT_FALSE(it->parent());
    }
  }
  for(auto it = powerCurves2.begin(); it != powerCurves2.end(); ++it) {
    EXPECT_TRUE(it->parent());
  }

  for(auto it = efficiencyCurves.begin(); it != efficiencyCurves.end(); ++it) {
    if (testObject.fanEfficiencyRatioFunctionofSpeedRatioCurve().handle() == it->handle()) {
      EXPECT_TRUE(it->parent());
    }
    else if (testObjectClone.fanEfficiencyRatioFunctionofSpeedRatioCurve().handle() == it->handle()) {
      EXPECT_TRUE(it->parent());
    }
    else {
      EXPECT_FALSE(it->parent());
    }
  }
  for(auto it = efficiencyCurves2.begin(); it != efficiencyCurves2.end(); ++it) {
    EXPECT_TRUE(it->parent());
  }

  EXPECT_DOUBLE_EQ(999.0, testObjectClone2.pressureRise());
  EXPECT_DOUBLE_EQ(0.99, testObjectClone2.fanTotalEfficiency());
  EXPECT_DOUBLE_EQ(999.0, testObjectClone2.maximumFlowRate().get());
  EXPECT_NE(testObject.fanPowerRatioFunctionofSpeedRatioCurve().handle(), testObjectClone2.fanPowerRatioFunctionofSpeedRatioCurve().handle());
  EXPECT_NE(fanPowerFuncSpeedCurve.handle(), testObjectClone2.fanPowerRatioFunctionofSpeedRatioCurve().handle());
  EXPECT_NE(testObject.fanEfficiencyRatioFunctionofSpeedRatioCurve().handle(), testObjectClone2.fanEfficiencyRatioFunctionofSpeedRatioCurve().handle());
  EXPECT_NE(fanEfficiencyFuncSpeedCurve.handle(), testObjectClone2.fanEfficiencyRatioFunctionofSpeedRatioCurve().handle());
  EXPECT_NE(testObjectClone2, testObjectClone);
  EXPECT_NE(testObjectClone2.handle(), testObjectClone.handle());
}

TEST_F(ModelFixture,FanOnOff_Test_Setters_and_Getters)
{
  Model m;
  Schedule s = m.alwaysOnDiscreteSchedule();
  FanOnOff testObject = FanOnOff(m, s);

  // Field Availability Schedule Name
  ScheduleConstant test_sched(m);
  test_sched.setValue(1.0);
  EXPECT_TRUE(testObject.setAvailabilitySchedule(test_sched));
  EXPECT_EQ(test_sched, testObject.availabilitySchedule());

  // Field Fan Efficiency
  EXPECT_TRUE(testObject.setFanTotalEfficiency(0.8));
  EXPECT_DOUBLE_EQ(0.8, testObject.fanTotalEfficiency());
  EXPECT_FALSE(testObject.isFanTotalEfficiencyDefaulted());

  testObject.resetFanTotalEfficiency();
  EXPECT_TRUE(testObject.isFanTotalEfficiencyDefaulted());

  // Test the deprecated older name

  EXPECT_TRUE(testObject.setFanEfficiency(0.7));
  EXPECT_DOUBLE_EQ(0.7, testObject.fanEfficiency());
  EXPECT_FALSE(testObject.isFanEfficiencyDefaulted());

  testObject.resetFanEfficiency();
  EXPECT_TRUE(testObject.isFanEfficiencyDefaulted());


  // Field Pressure Rise

  EXPECT_DOUBLE_EQ(300, testObject.pressureRise());

  testObject.setPressureRise(100);
  EXPECT_DOUBLE_EQ(100, testObject.pressureRise());

  // Field Maximum Flow Rate

  EXPECT_TRUE(testObject.setMaximumFlowRate(100));
  EXPECT_DOUBLE_EQ(100, testObject.maximumFlowRate().get());
  EXPECT_FALSE(testObject.isMaximumFlowRateAutosized());

  testObject.resetMaximumFlowRate();
  EXPECT_FALSE(testObject.maximumFlowRate());

  testObject.autosizeMaximumFlowRate();
  EXPECT_TRUE(testObject.isMaximumFlowRateAutosized());

  // Field Motor Efficiency

  EXPECT_TRUE(testObject.setMotorEfficiency(0.9));
  EXPECT_DOUBLE_EQ(0.9, testObject.motorEfficiency());
  EXPECT_FALSE(testObject.isMotorEfficiencyDefaulted());

  testObject.resetMotorEfficiency();
  EXPECT_TRUE(testObject.isMotorEfficiencyDefaulted());
  EXPECT_DOUBLE_EQ(0.8, testObject.motorEfficiency());

  // Field Motor In Airstream Fraction

  EXPECT_TRUE(testObject.setMotorInAirstreamFraction(0.9));
  EXPECT_DOUBLE_EQ(0.9, testObject.motorInAirstreamFraction().get());

  testObject.resetMotorInAirstreamFraction();
  EXPECT_DOUBLE_EQ(1.0, testObject.motorInAirstreamFraction().get());

  // Field End-Use Subcategory

  testObject.setEndUseSubcategory("test");
  EXPECT_EQ("test", testObject.endUseSubcategory());
  EXPECT_FALSE(testObject.isEndUseSubcategoryDefaulted());

  testObject.resetEndUseSubcategory();
  EXPECT_TRUE(testObject.isEndUseSubcategoryDefaulted());

  // Curves

  CurveExponent newCurve1(m);
  EXPECT_TRUE(testObject.setFanPowerRatioFunctionofSpeedRatioCurve(newCurve1));

  CurveQuadratic newCurve2(m);
  EXPECT_TRUE(testObject.setFanEfficiencyRatioFunctionofSpeedRatioCurve(newCurve2));

  CurveCubic newCurve3(m);
  EXPECT_TRUE(testObject.setFanEfficiencyRatioFunctionofSpeedRatioCurve(newCurve3));

  EXPECT_FALSE(testObject.setFanPowerRatioFunctionofSpeedRatioCurve(newCurve2));
  EXPECT_TRUE(testObject.setFanPowerRatioFunctionofSpeedRatioCurve(newCurve3));
  EXPECT_FALSE(testObject.setFanEfficiencyRatioFunctionofSpeedRatioCurve(newCurve1));
}

TEST_F(ModelFixture,FanOnOff_containingZoneHVACComponent_ZoneHVACWaterToAirHeatPump)
{
  Model m;
  Schedule s = m.alwaysOnDiscreteSchedule();
  FanOnOff fan = FanOnOff(m, s);

  CoilHeatingWaterToAirHeatPumpEquationFit heatingCoil(m);
  CoilCoolingWaterToAirHeatPumpEquationFit coolingCoil(m);
  CoilHeatingElectric supplementalHeatingCoil(m, s);

  ZoneHVACWaterToAirHeatPump zoneHVACWaterToAirHeatPump(m, s, fan, heatingCoil, coolingCoil, supplementalHeatingCoil);

  boost::optional<ZoneHVACComponent> component = fan.containingZoneHVACComponent();
  ASSERT_TRUE(component);
  EXPECT_EQ(component.get().handle(), zoneHVACWaterToAirHeatPump.handle());
}

TEST_F(ModelFixture,FanOnOff_containingZoneHVACComponent_ZoneHVACFourPipeFanCoil)
{
  Model m;
  Schedule s = m.alwaysOnDiscreteSchedule();
  FanOnOff fan = FanOnOff(m, s);

  CoilHeatingWater heatingCoil(m, s);
  CoilCoolingWater coolingCoil(m, s);

  ZoneHVACFourPipeFanCoil zoneHVACFourPipeFanCoil(m, s, fan, coolingCoil, heatingCoil);

  boost::optional<ZoneHVACComponent> component = fan.containingZoneHVACComponent();
  ASSERT_TRUE(component);
  EXPECT_EQ(component.get().handle(), zoneHVACFourPipeFanCoil.handle());
}

TEST_F(ModelFixture,FanOnOff_containingZoneHVACComponent_ZoneHVACPackagedTerminalAirConditioner)
{
  Model m;
  Schedule s = m.alwaysOnDiscreteSchedule();
  FanOnOff fan = FanOnOff(m, s);

  CurveBiquadratic c1(m);
  CurveQuadratic c2(m);
  CurveBiquadratic c3(m);
  CurveQuadratic c4(m);
  CurveQuadratic c5(m);

  CoilHeatingWater heatingCoil(m, s);
  CoilCoolingDXSingleSpeed coolingCoil(m, s, c1, c2, c3, c4, c5);

  ZoneHVACPackagedTerminalAirConditioner zoneHVACPackagedTerminalAirConditioner(m, s, fan, heatingCoil, coolingCoil);

  boost::optional<ZoneHVACComponent> component = fan.containingZoneHVACComponent();
  ASSERT_TRUE(component);
  EXPECT_EQ(component.get().handle(), zoneHVACPackagedTerminalAirConditioner.handle());
}

TEST_F(ModelFixture,FanOnOff_containingZoneHVACComponent_ZoneHVACPackagedTerminalHeatPump)
{
  Model m;
  Schedule s = m.alwaysOnDiscreteSchedule();
  FanOnOff fan = FanOnOff(m, s);

  CurveBiquadratic c1(m);
  CurveQuadratic c2(m);
  CurveBiquadratic c3(m);
  CurveQuadratic c4(m);
  CurveQuadratic c5(m);

  CoilHeatingDXSingleSpeed heatingCoil(m, s, c1, c2, c3, c4, c5);
  CoilCoolingDXSingleSpeed coolingCoil(m, s, c1, c2, c3, c4, c5);
  CoilHeatingElectric supplementalHeatingCoil(m, s);

  ZoneHVACPackagedTerminalHeatPump zoneHVACPackagedTerminalHeatPump(m, s, fan, heatingCoil, coolingCoil, supplementalHeatingCoil);

  boost::optional<ZoneHVACComponent> component = fan.containingZoneHVACComponent();
  ASSERT_TRUE(component);
  EXPECT_EQ(component.get().handle(), zoneHVACPackagedTerminalHeatPump.handle());
}

TEST_F(ModelFixture,FanOnOff_containingZoneHVACComponent_ZoneHVACUnitHeater)
{
  Model m;
  Schedule s = m.alwaysOnDiscreteSchedule();
  FanOnOff fan = FanOnOff(m, s);

  CoilHeatingWater heatingCoil(m, s);

  ZoneHVACUnitHeater zoneHVACUnitHeater(m, s, fan, heatingCoil);

  boost::optional<ZoneHVACComponent> component = fan.containingZoneHVACComponent();
  EXPECT_TRUE(component);
}

TEST_F(ModelFixture,FanOnOff_containingZoneHVACComponent_ZoneHVACTerminalUnitVariableRefrigerantFlow)
{
  Model m;
  ZoneHVACTerminalUnitVariableRefrigerantFlow zoneHVACTerminalUnitVariableRefrigerantFlow(m);

  std::vector<FanOnOff> fans = m.getModelObjects<FanOnOff>();
  EXPECT_EQ(1, fans.size());

  boost::optional<ZoneHVACComponent> component = fans[0].containingZoneHVACComponent();
  ASSERT_TRUE(component);
  EXPECT_EQ(component.get().handle(), zoneHVACTerminalUnitVariableRefrigerantFlow.handle());
}

TEST_F(ModelFixture,FanOnOff_containingZoneHVACComponent_ZoneHVACBaseboardConvectiveElectric)
{
  Model m;
  Schedule s = m.alwaysOnDiscreteSchedule();
  FanOnOff fan = FanOnOff(m, s);

  ZoneHVACBaseboardConvectiveElectric zoneHVACBaseboardConvectiveElectric(m);

  boost::optional<ZoneHVACComponent> component = fan.containingZoneHVACComponent();
  EXPECT_FALSE(component);
}

TEST_F(ModelFixture,FanOnOff_containingZoneHVACComponent_ZoneHVACLowTemperatureRadiantElectric)
{
  Model m;
  Schedule s = m.alwaysOnDiscreteSchedule();
  FanOnOff fan = FanOnOff(m, s);

  ZoneHVACLowTemperatureRadiantElectric zoneHVACLowTemperatureRadiantElectric(m, s, s);

  boost::optional<ZoneHVACComponent> component = fan.containingZoneHVACComponent();
  EXPECT_FALSE(component);
}

TEST_F(ModelFixture,FanOnOff_containingHVACComponent_AirLoopHVACUnitaryHeatPumpAirToAir)
{
  Model m;
  Schedule s = m.alwaysOnDiscreteSchedule();
  FanOnOff fan = FanOnOff(m, s);

  CurveBiquadratic c1(m);
  CurveQuadratic c2(m);
  CurveBiquadratic c3(m);
  CurveQuadratic c4(m);
  CurveQuadratic c5(m);

  CoilHeatingDXSingleSpeed heatingCoil(m, s, c1, c2, c3, c4, c5);
  CoilCoolingDXSingleSpeed coolingCoil(m, s, c1, c2, c3, c4, c5);
  CoilHeatingElectric supplementalHeatingCoil(m, s);

  AirLoopHVACUnitaryHeatPumpAirToAir airLoopHVACUnitaryHeatPumpAirToAir(m, s, fan, heatingCoil, coolingCoil, supplementalHeatingCoil);

  boost::optional<HVACComponent> component = fan.containingHVACComponent();
  ASSERT_TRUE(component);
  EXPECT_EQ(component.get().handle(), airLoopHVACUnitaryHeatPumpAirToAir.handle());
}
