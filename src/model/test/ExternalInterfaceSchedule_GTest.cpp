/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>

#include "ModelFixture.hpp"

#include "../ExternalInterfaceSchedule.hpp"
#include "../ExternalInterfaceSchedule_Impl.hpp"

#include "../ScheduleTypeRegistry.hpp"
#include "../ScheduleTypeLimits.hpp"

#include "../../utilities/idf/ValidityReport.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, ExternalInterfaceSchedule) {
  Model model;
  ExternalInterfaceSchedule schedule(model);
  //Check BCVTB
  EXPECT_TRUE(schedule.exportToBCVTB());
  EXPECT_TRUE(schedule.isExportToBCVTBDefaulted());
  schedule.setExportToBCVTB(false);
  EXPECT_FALSE(schedule.exportToBCVTB());
  schedule.resetExportToBCVTB();
  EXPECT_TRUE(schedule.exportToBCVTB());

  EXPECT_EQ(0.0, schedule.initialValue());
  schedule.setInitialValue(-0.1);
  EXPECT_EQ(-0.1, schedule.initialValue());
  EXPECT_TRUE(checkOrAssignScheduleTypeLimits("Lights", "Lighting", schedule));
  ValidityReport report = schedule.validityReport(StrictnessLevel(StrictnessLevel::Final));
  ASSERT_EQ(0u, report.numErrors());
  schedule.setInitialValue(0.5);
  report = schedule.validityReport(StrictnessLevel(StrictnessLevel::Final));
  EXPECT_EQ(0u, report.numErrors());
}

TEST_F(ModelFixture, ExternalInterfaceSchedule2) {
  Model model;
  ExternalInterfaceSchedule schedule(model, 10);
  EXPECT_EQ(10.0, schedule.initialValue());
  EXPECT_EQ("External Interface Schedule 1", schedule.nameString());

  ExternalInterfaceSchedule schedule2(model);
  EXPECT_EQ("External Interface Schedule 2", schedule2.nameString());
  boost::optional<std::string> test = schedule2.setName("External Interface Schedule 1");
  //should be equal since the name setting should fail and return the original name
  EXPECT_EQ(test.get(), schedule2.nameString());
  schedule2.setName("External Interface Schedule 3");
  EXPECT_EQ("External Interface Schedule 3", schedule2.nameString());

  ScheduleTypeLimits stl(model);
  EXPECT_TRUE(schedule.setScheduleTypeLimits(stl));
  EXPECT_EQ(stl, schedule.scheduleTypeLimits().get());
  EXPECT_TRUE(schedule.resetScheduleTypeLimits());
  boost::optional<ScheduleTypeLimits> stl2 = schedule.scheduleTypeLimits();
  EXPECT_TRUE(!stl2.is_initialized());
}
