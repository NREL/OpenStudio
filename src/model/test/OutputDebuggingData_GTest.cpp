/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>

#include "ModelFixture.hpp"
#include "../OutputDebuggingData.hpp"
#include "../OutputDebuggingData_Impl.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, OutputDebuggingData_GettersSetters) {
  Model m;
  auto mo = m.getUniqueModelObject<OutputDebuggingData>();

  // Report Debugging Data: Required Boolean
  EXPECT_TRUE(mo.setReportDebuggingData(true));
  EXPECT_TRUE(mo.reportDebuggingData());
  EXPECT_TRUE(mo.setReportDebuggingData(false));
  EXPECT_FALSE(mo.reportDebuggingData());

  // Report During Warmup: Required Boolean
  EXPECT_TRUE(mo.setReportDuringWarmup(true));
  EXPECT_TRUE(mo.reportDuringWarmup());
  EXPECT_TRUE(mo.setReportDuringWarmup(false));
  EXPECT_FALSE(mo.reportDuringWarmup());

}  // End of Getter_Setters test
