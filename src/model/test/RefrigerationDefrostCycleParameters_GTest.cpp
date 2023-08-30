/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>

#include "ModelFixture.hpp"

#include "../RefrigerationDefrostCycleParameters.hpp"
#include "../RefrigerationDefrostCycleParameters_Impl.hpp"

#include "../../utilities/time/Time.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, RefrigerationDefrostCycleParameters_DefaultConstructor) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  ASSERT_EXIT(
    {
      Model model;
      RefrigerationDefrostCycleParameters testObject = RefrigerationDefrostCycleParameters(model);

      exit(0);
    },
    ::testing::ExitedWithCode(0), "");
}

TEST_F(ModelFixture, RefrigerationDefrostCycleParameters_SettersAndGetters) {
  Model model;
  RefrigerationDefrostCycleParameters testObject = RefrigerationDefrostCycleParameters(model);
  std::shared_ptr<openstudio::model::detail::RefrigerationDefrostCycleParameters_Impl> testObjectImpl =
    testObject.getImpl<openstudio::model::detail::RefrigerationDefrostCycleParameters_Impl>();

  EXPECT_TRUE(testObjectImpl->setDurationofDefrostCycle(10));
  EXPECT_DOUBLE_EQ(10, *testObjectImpl->durationofDefrostCycle());

  EXPECT_TRUE(testObjectImpl->setDripDownTime(5));
  EXPECT_DOUBLE_EQ(5, *testObjectImpl->dripDownTime());

  Time testTime = Time(0, 1, 15);
  EXPECT_TRUE(testObjectImpl->setDefrost1StartTime(Time(0, 1, 15)));
  EXPECT_DOUBLE_EQ(testTime.hours(), testObjectImpl->defrost1StartTime()->hours());
  EXPECT_DOUBLE_EQ(testTime.minutes(), testObjectImpl->defrost1StartTime()->minutes());

  testTime = Time(0, 2, 16);
  EXPECT_TRUE(testObjectImpl->setDefrost2StartTime(Time(0, 2, 16)));
  EXPECT_DOUBLE_EQ(testTime.hours(), testObjectImpl->defrost2StartTime()->hours());
  EXPECT_DOUBLE_EQ(testTime.minutes(), testObjectImpl->defrost2StartTime()->minutes());

  testTime = Time(0, 3, 17);
  EXPECT_TRUE(testObjectImpl->setDefrost3StartTime(Time(0, 3, 17)));
  EXPECT_DOUBLE_EQ(testTime.hours(), testObjectImpl->defrost3StartTime()->hours());
  EXPECT_DOUBLE_EQ(testTime.minutes(), testObjectImpl->defrost3StartTime()->minutes());

  testTime = Time(0, 4, 18);
  EXPECT_TRUE(testObjectImpl->setDefrost4StartTime(Time(0, 4, 18)));
  EXPECT_DOUBLE_EQ(testTime.hours(), testObjectImpl->defrost4StartTime()->hours());
  EXPECT_DOUBLE_EQ(testTime.minutes(), testObjectImpl->defrost4StartTime()->minutes());

  testTime = Time(0, 5, 19);
  EXPECT_TRUE(testObjectImpl->setDefrost5StartTime(Time(0, 5, 19)));
  EXPECT_DOUBLE_EQ(testTime.hours(), testObjectImpl->defrost5StartTime()->hours());
  EXPECT_DOUBLE_EQ(testTime.minutes(), testObjectImpl->defrost5StartTime()->minutes());

  testTime = Time(0, 6, 20);
  EXPECT_TRUE(testObjectImpl->setDefrost6StartTime(Time(0, 6, 20)));
  EXPECT_DOUBLE_EQ(testTime.hours(), testObjectImpl->defrost6StartTime()->hours());
  EXPECT_DOUBLE_EQ(testTime.minutes(), testObjectImpl->defrost6StartTime()->minutes());

  testTime = Time(0, 7, 21);
  EXPECT_TRUE(testObjectImpl->setDefrost7StartTime(Time(0, 7, 21)));
  EXPECT_DOUBLE_EQ(testTime.hours(), testObjectImpl->defrost7StartTime()->hours());
  EXPECT_DOUBLE_EQ(testTime.minutes(), testObjectImpl->defrost7StartTime()->minutes());

  testTime = Time(0, 8, 22);
  EXPECT_TRUE(testObjectImpl->setDefrost8StartTime(Time(0, 8, 22)));
  EXPECT_DOUBLE_EQ(testTime.hours(), testObjectImpl->defrost8StartTime()->hours());
  EXPECT_DOUBLE_EQ(testTime.minutes(), testObjectImpl->defrost8StartTime()->minutes());
}
