/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>

#include "ModelFixture.hpp"

#include "../LoadProfilePlant.hpp"
#include "../LoadProfilePlant_Impl.hpp"
#include "../ScheduleRuleset.hpp"
#include "../ScheduleRuleset_Impl.hpp"
#include "../ScheduleConstant.hpp"
#include "../ScheduleConstant_Impl.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, LoadProfilePlant_LoadProfilePlant) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  ASSERT_EXIT(
    {
      Model m;
      LoadProfilePlant lpp(m);

      exit(0);
    },
    ::testing::ExitedWithCode(0), "");

  Model m;
  LoadProfilePlant lpp(m);

  ASSERT_TRUE(lpp.loadSchedule().optionalCast<ScheduleRuleset>());
  EXPECT_EQ(0.002, lpp.peakFlowRate());
  ASSERT_TRUE(lpp.flowRateFractionSchedule().optionalCast<ScheduleConstant>());
  EXPECT_EQ("Water", lpp.plantLoopFluidType());
  EXPECT_EQ(5.0, lpp.degreeofSubCooling());
  EXPECT_EQ(20.0, lpp.degreeofLoopSubCooling());
}

TEST_F(ModelFixture, LoadProfilePlant_GettersSetters) {
  Model m;
  LoadProfilePlant lpp(m);

  ScheduleConstant scheduleConstant1(m);
  EXPECT_TRUE(lpp.setLoadSchedule(scheduleConstant1));
  EXPECT_TRUE(lpp.setPeakFlowRate(0.005));
  ScheduleConstant scheduleConstant2(m);
  EXPECT_TRUE(hpp.setFLowRateFractionSchedule(scheduleConstant2));
  EXPECT_TRUE(lpp.setPlantLoopFluidType("Steam"));
  EXPECT_TRUE(lpp.setDegreeofSubCooling(6.0));
  EXPECT_TRUE(lpp.setDegreeofLoopSubCooling(21.0));

  ASSERT_TRUE(lpp.loadSchedule().optionalCast<ScheduleConstant>());
  EXPECT_EQ(0.005, lpp.peakFlowRate());
  ASSERT_TRUE(lpp.flowRateFractionSchedule().optionalCast<ScheduleConstant>());
  EXPECT_EQ("Steam", lpp.plantLoopFluidType());
  EXPECT_EQ(6.0, lpp.degreeofSubCooling());
  EXPECT_EQ(21.0, lpp.degreeofLoopSubCooling());
}
