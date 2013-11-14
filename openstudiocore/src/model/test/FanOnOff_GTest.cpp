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
#include <model/Model.hpp>
#include <model/Node.hpp>
#include <model/Node_Impl.hpp>
#include <model/StraightComponent.hpp>
#include <model/StraightComponent_Impl.hpp>
#include <model/Schedule.hpp>
#include <model/Schedule_Impl.hpp>
#include <model/ScheduleConstant.hpp>
#include <model/Curve.hpp>
#include <model/Curve_Impl.hpp>
#include <model/CurveQuadratic.hpp>
#include <model/CurveQuadratic_Impl.hpp>
#include <model/CurveCubic.hpp>
#include <model/CurveCubic_Impl.hpp>
#include <model/CurveExponent.hpp>
#include <model/CurveExponent_Impl.hpp>
#include <model/FanOnOff.hpp>
#include <model/FanOnOff_Impl.hpp>
#include <utilities/units/Unit.hpp>

using namespace openstudio;
using namespace openstudio::model;

//Test construction of the fan

TEST_F(ModelFixture,FanOnOff_Check_Constructor)
{
::testing::FLAGS_gtest_death_test_style = "threadsafe";

ASSERT_EXIT (
  {
    //create a model, schedule and the curves to use in the constructor
    Model model;
    ScheduleConstant sched(model);
    sched.setValue(1.0); // Always on
    CurveExponent fprfsrc(model);
    CurveQuadratic ferfsrc(model);

    // Make a fan
    FanOnOff fan(model,sched,fprfsrc,ferfsrc);

    exit(0);
  },
  ::testing::ExitedWithCode(0), "" );
}

// Test add to and remove from node

TEST_F(ModelFixture,FanOnOff_addToNode)
{
  Model model;
  ScheduleConstant sched(model);
  sched.setValue(1.0); // Always on
  CurveExponent fprfsrc(model);
  CurveQuadratic ferfsrc(model);
  FanOnOff fan(model,sched,fprfsrc,ferfsrc);

  AirLoopHVAC airLoop(model);

  Node supplyOutletNode = airLoop.supplyOutletNode();

  EXPECT_TRUE(fan.addToNode(supplyOutletNode));

  ASSERT_EQ( (unsigned)3, airLoop.supplyComponents().size() );

  // inlet and outlet ports
  EXPECT_TRUE(fan.inletPort());
  EXPECT_TRUE(fan.outletPort());

}


TEST_F(ModelFixture,FanOnOff_Test_Setters_and_Getters)
{
  // Make a fan
  Model model;
  ScheduleConstant sched(model);
  sched.setValue(1.0); // Always on
  CurveExponent fprfsrc(model);
  CurveQuadratic ferfsrc(model);
  FanOnOff fan(model,sched,fprfsrc,ferfsrc);

  // Field Availability Schedule Name
  ScheduleConstant test_sched(model);
  test_sched.setValue(1.0);
  EXPECT_TRUE(fan.setAvailabilitySchedule(test_sched));
  EXPECT_EQ(fan.availabilitySchedule(),test_sched);

  // Field Fan Efficiency

  EXPECT_TRUE(fan.setFanEfficiency(0.60));
  boost::optional<double> testFanEfficiency = fan.fanEfficiency();

  EXPECT_EQ((*testFanEfficiency),0.60);
  EXPECT_FALSE(fan.isFanEfficiencyDefaulted());

  fan.resetFanEfficiency();
  EXPECT_TRUE(fan.isFanEfficiencyDefaulted());

  // Field Pressure Rise

  EXPECT_EQ(fan.pressureRise(),250); // verify default

  fan.setPressureRise(100);
  boost::optional<double> testPressureRise = fan.pressureRise();
  EXPECT_EQ((*testPressureRise),100);

  // Field Maximum Flow Rate

  EXPECT_TRUE(fan.setMaximumFlowRate(100));
  boost::optional<double> testMaximumFlowRate = fan.maximumFlowRate();

  EXPECT_EQ((*testMaximumFlowRate),100);
  EXPECT_FALSE(fan.isMaximumFlowRateAutosized());

  fan.resetMaximumFlowRate();
  EXPECT_FALSE(fan.maximumFlowRate());

  fan.autosizeMaximumFlowRate();
  EXPECT_TRUE(fan.isMaximumFlowRateAutosized());

  // Field Motor Efficiency

  EXPECT_TRUE(fan.setMotorEfficiency(0.9));
  boost::optional<double> testMotorEfficiency = fan.motorEfficiency();

  EXPECT_EQ((*testMotorEfficiency),0.9);
  EXPECT_FALSE(fan.isMotorEfficiencyDefaulted());

  fan.resetMotorEfficiency();
  EXPECT_TRUE(fan.isMotorEfficiencyDefaulted());
  EXPECT_EQ(fan.motorEfficiency(),0.8); // verify default

  // Field Motor In Airstream Fraction

  EXPECT_TRUE(fan.setMotorInAirstreamFraction(0.9));
  boost::optional<double> testMotorInAirstreamFraction = fan.motorInAirstreamFraction();

  EXPECT_EQ((*testMotorInAirstreamFraction),0.9);

  fan.resetMotorInAirstreamFraction();
  EXPECT_EQ(fan.motorInAirstreamFraction(),1.0); // verify default

  // Field End-Use Subcategory

  fan.setEndUseSubcategory("test");
  boost::optional<std::string> testEndUseSubcategory = fan.endUseSubcategory();

  EXPECT_EQ((*testEndUseSubcategory),"test");
  EXPECT_FALSE(fan.isEndUseSubcategoryDefaulted());

  fan.resetEndUseSubcategory();
  EXPECT_TRUE(fan.isEndUseSubcategoryDefaulted());
  EXPECT_EQ(fan.endUseSubcategory(),"General"); // verify default

  // Curves

  boost::optional<CurveExponent> newCurve1(model);
  EXPECT_TRUE(fan.setFanPowerRatioFunctionofSpeedRatioCurve(*newCurve1));

  boost::optional<CurveQuadratic> newCurve2(model);
  EXPECT_TRUE(fan.setFanEfficiencyRatioFunctionofSpeedRatioCurve(*newCurve2));

}
