/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>

#include "ModelFixture.hpp"
#include "../ScheduleConstant.hpp"
#include "../ScheduleConstant_Impl.hpp"
#include "../ScheduleTypeRegistry.hpp"

#include "../../utilities/idf/ValidityReport.hpp"

using namespace openstudio::model;
using namespace openstudio;

TEST_F(ModelFixture, Schedule_FinalStrictness) {
  Model model;
  ScheduleConstant schedule(model);
  schedule.setValue(-0.1);
  EXPECT_TRUE(checkOrAssignScheduleTypeLimits("Lights", "Lighting", schedule));
  ValidityReport report = schedule.validityReport(StrictnessLevel(StrictnessLevel::Final));
  ASSERT_EQ(0u, report.numErrors());
  schedule.setValue(0.5);
  report = schedule.validityReport(StrictnessLevel(StrictnessLevel::Final));
  EXPECT_EQ(0u, report.numErrors());
}
