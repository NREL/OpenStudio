/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>

#include "ModelFixture.hpp"

#include "../ScheduleTypeLimits.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, ScheduleTypeLimits_Ctor) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  ASSERT_EXIT(
    {
      Model m;
      ScheduleTypeLimits sch_lim(m);

      exit(0);
    },
    ::testing::ExitedWithCode(0), "");
}

TEST_F(ModelFixture, ScheduleTypeLimits_GettersSetters) {
  Model m;
  ScheduleTypeLimits sch_lim(m);

  // Lower Limit Value: Optional Double
  // No Default
  EXPECT_TRUE(sch_lim.setLowerLimitValue(0.03));
  ASSERT_TRUE(sch_lim.lowerLimitValue());
  EXPECT_EQ(0.03, sch_lim.lowerLimitValue().get());
  sch_lim.resetLowerLimitValue();
  EXPECT_FALSE(sch_lim.lowerLimitValue());

  // Upper Limit Value: Optional Double
  // No Default
  EXPECT_TRUE(sch_lim.setUpperLimitValue(10.03));
  ASSERT_TRUE(sch_lim.upperLimitValue());
  EXPECT_EQ(10.03, sch_lim.upperLimitValue().get());
  sch_lim.resetUpperLimitValue();
  EXPECT_FALSE(sch_lim.upperLimitValue());

  // Numeric Type: Optional String
  // No Default
  // Test a valid choice
  EXPECT_TRUE(sch_lim.setNumericType("Continuous"));
  ASSERT_TRUE(sch_lim.numericType());
  EXPECT_EQ("Continuous", sch_lim.numericType().get());
  // Test an invalid choice
  EXPECT_FALSE(sch_lim.setNumericType("BadChoice"));
  EXPECT_EQ("Continuous", sch_lim.numericType().get());

  // Unit Type:  String
  // Check Idd default: "Dimensionless"
  EXPECT_EQ("Dimensionless", sch_lim.unitType());
  // Test a valid choice
  EXPECT_TRUE(sch_lim.setUnitType("Temperature"));
  EXPECT_EQ("Temperature", sch_lim.unitType());
  // Test an invalid choice
  EXPECT_FALSE(sch_lim.setUnitType("BadChoice"));
  EXPECT_EQ("Temperature", sch_lim.unitType());
}
