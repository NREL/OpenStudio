/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>

#include "ModelFixture.hpp"

#include "../ExternalInterfaceFunctionalMockupUnitExportToSchedule.hpp"
#include "../ExternalInterfaceFunctionalMockupUnitExportToSchedule_Impl.hpp"

#include "../ScheduleTypeRegistry.hpp"
#include "../ScheduleTypeLimits.hpp"

#include "../../utilities/idf/ValidityReport.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, ExternalInterfaceFunctionalMockupUnitExportToSchedule) {
  Model model;

  ExternalInterfaceFunctionalMockupUnitExportToSchedule schedule(model, "FMU name", 10);
  EXPECT_EQ(10.0, schedule.initialValue());
  schedule.setInitialValue(-0.1);
  EXPECT_EQ(-0.1, schedule.initialValue());
  EXPECT_TRUE(checkOrAssignScheduleTypeLimits("Lights", "Lighting", schedule));
  ValidityReport report = schedule.validityReport(StrictnessLevel(StrictnessLevel::Final));
  ASSERT_EQ(0u, report.numErrors());
  schedule.setInitialValue(0.5);
  report = schedule.validityReport(StrictnessLevel(StrictnessLevel::Final));
  EXPECT_EQ(0u, report.numErrors());

  EXPECT_EQ("FMU name", schedule.fMUVariableName());
  schedule.setFMUVariableName("test 2");
  EXPECT_EQ("test 2", schedule.fMUVariableName());
}

TEST_F(ModelFixture, ExternalInterfaceFunctionalMockupUnitExportToSchedule2) {
  Model model;

  ExternalInterfaceFunctionalMockupUnitExportToSchedule schedule(model, "FMU name", 10);
  EXPECT_EQ(10.0, schedule.initialValue());
  EXPECT_EQ("External Interface Functional Mockup Unit Export To Schedule 1", schedule.nameString());

  ExternalInterfaceFunctionalMockupUnitExportToSchedule schedule2(model, "FMU 2 name", 10);
  EXPECT_EQ("External Interface Functional Mockup Unit Export To Schedule 2", schedule2.nameString());
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
