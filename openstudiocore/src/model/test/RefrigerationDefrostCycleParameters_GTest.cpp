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

#include <model/test/ModelFixture.hpp>

#include <model/RefrigerationDefrostCycleParameters.hpp>
#include <model/RefrigerationDefrostCycleParameters_Impl.hpp>

#include <utilities/time/Time.hpp>

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, RefrigerationDefrostCycleParameters_DefaultConstructor)
{
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  ASSERT_EXIT ( 
  {  
    Model model;
    RefrigerationDefrostCycleParameters testObject = RefrigerationDefrostCycleParameters(model);

    exit(0); 
  } ,
    ::testing::ExitedWithCode(0), "" );
}

TEST_F(ModelFixture, RefrigerationDefrostCycleParameters_SettersAndGetters)
{
  Model model;
  RefrigerationDefrostCycleParameters testObject = RefrigerationDefrostCycleParameters(model);

  EXPECT_TRUE(testObject.setDurationofDefrostCycle(10));
  EXPECT_DOUBLE_EQ(10, *testObject.durationofDefrostCycle());

  EXPECT_TRUE(testObject.setDripDownTime(5));
  EXPECT_DOUBLE_EQ(5, *testObject.dripDownTime());

  Time testTime = Time(0, 1, 15);
  EXPECT_TRUE(testObject.setDefrost1StartTime(Time(0, 1, 15)));
  EXPECT_DOUBLE_EQ(testTime.hours(), testObject.defrost1StartTime()->hours());
  EXPECT_DOUBLE_EQ(testTime.minutes(), testObject.defrost1StartTime()->minutes());

  testTime = Time(0, 2, 16);
  EXPECT_TRUE(testObject.setDefrost2StartTime(Time(0, 2, 16)));
  EXPECT_DOUBLE_EQ(testTime.hours(), testObject.defrost2StartTime()->hours());
  EXPECT_DOUBLE_EQ(testTime.minutes(), testObject.defrost2StartTime()->minutes());

  testTime = Time(0, 3, 17);
  EXPECT_TRUE(testObject.setDefrost3StartTime(Time(0, 3, 17)));
  EXPECT_DOUBLE_EQ(testTime.hours(), testObject.defrost3StartTime()->hours());
  EXPECT_DOUBLE_EQ(testTime.minutes(), testObject.defrost3StartTime()->minutes());

  testTime = Time(0, 4, 18);
  EXPECT_TRUE(testObject.setDefrost4StartTime(Time(0, 4, 18)));
  EXPECT_DOUBLE_EQ(testTime.hours(), testObject.defrost4StartTime()->hours());
  EXPECT_DOUBLE_EQ(testTime.minutes(), testObject.defrost4StartTime()->minutes());

  testTime = Time(0, 5, 19);
  EXPECT_TRUE(testObject.setDefrost5StartTime(Time(0, 5, 19)));
  EXPECT_DOUBLE_EQ(testTime.hours(), testObject.defrost5StartTime()->hours());
  EXPECT_DOUBLE_EQ(testTime.minutes(), testObject.defrost5StartTime()->minutes());

  testTime = Time(0, 6, 20);
  EXPECT_TRUE(testObject.setDefrost6StartTime(Time(0, 6, 20)));
  EXPECT_DOUBLE_EQ(testTime.hours(), testObject.defrost6StartTime()->hours());
  EXPECT_DOUBLE_EQ(testTime.minutes(), testObject.defrost6StartTime()->minutes());

  testTime = Time(0, 7, 21);
  EXPECT_TRUE(testObject.setDefrost7StartTime(Time(0, 7, 21)));
  EXPECT_DOUBLE_EQ(testTime.hours(), testObject.defrost7StartTime()->hours());
  EXPECT_DOUBLE_EQ(testTime.minutes(), testObject.defrost7StartTime()->minutes());

  testTime = Time(0, 8, 22);
  EXPECT_TRUE(testObject.setDefrost8StartTime(Time(0, 8, 22)));
  EXPECT_DOUBLE_EQ(testTime.hours(), testObject.defrost8StartTime()->hours());
  EXPECT_DOUBLE_EQ(testTime.minutes(), testObject.defrost8StartTime()->minutes());

}
