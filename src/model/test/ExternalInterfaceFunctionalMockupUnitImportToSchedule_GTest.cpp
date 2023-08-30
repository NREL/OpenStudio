/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>

#include "ModelFixture.hpp"

#include "../ExternalInterfaceFunctionalMockupUnitImportToSchedule.hpp"
#include "../ExternalInterfaceFunctionalMockupUnitImportToSchedule_Impl.hpp"
#include "../ExternalInterfaceFunctionalMockupUnitImport.hpp"
#include "../ExternalInterfaceFunctionalMockupUnitImport_Impl.hpp"

#include "../ScheduleTypeRegistry.hpp"
#include "../ScheduleTypeLimits.hpp"

#include "../../utilities/idf/ValidityReport.hpp"

using namespace openstudio;
using namespace openstudio::model;
TEST_F(ModelFixture, ExternalInterfaceFunctionalMockupUnitImportToSchedule) {
  Model model;
  // add FMU
  ExternalInterfaceFunctionalMockupUnitImport eifmui(model, "test name");

  ExternalInterfaceFunctionalMockupUnitImportToSchedule schedule(model, eifmui, "FMU", "FMU name", 10);
  EXPECT_EQ(10.0, schedule.initialValue());
  schedule.setInitialValue(-0.1);
  EXPECT_EQ(-0.1, schedule.initialValue());
  EXPECT_TRUE(checkOrAssignScheduleTypeLimits("Lights", "Lighting", schedule));
  ValidityReport report = schedule.validityReport(StrictnessLevel(StrictnessLevel::Final));
  ASSERT_EQ(0u, report.numErrors());
  schedule.setInitialValue(0.5);
  report = schedule.validityReport(StrictnessLevel(StrictnessLevel::Final));
  EXPECT_EQ(0u, report.numErrors());

  ExternalInterfaceFunctionalMockupUnitImport temp = schedule.fMUFile();
  EXPECT_EQ(eifmui, schedule.fMUFile());
  EXPECT_EQ("FMU", schedule.fMUInstanceName());
  EXPECT_EQ("FMU name", schedule.fMUVariableName());
  schedule.setFMUInstanceName("test 1");
  schedule.setFMUVariableName("test 2");
  EXPECT_EQ("test 1", schedule.fMUInstanceName());
  EXPECT_EQ("test 2", schedule.fMUVariableName());
  ExternalInterfaceFunctionalMockupUnitImport eifmui2(model, "test name 2");
  EXPECT_TRUE(schedule.setFMUFile(eifmui2));
  EXPECT_EQ(eifmui2, schedule.fMUFile());
}

TEST_F(ModelFixture, ExternalInterfaceFunctionalMockupUnitImportToSchedule2) {
  Model model;
  // add FMU
  ExternalInterfaceFunctionalMockupUnitImport eifmui(model, "test name");

  ExternalInterfaceFunctionalMockupUnitImportToSchedule schedule(model, eifmui, "FMU", "FMU name", 10);
  EXPECT_EQ(10.0, schedule.initialValue());
  EXPECT_EQ("External Interface Functional Mockup Unit Import To Schedule 1", schedule.nameString());

  ExternalInterfaceFunctionalMockupUnitImportToSchedule schedule2(model, eifmui, "FMU", "FMU 2 name", 10);
  EXPECT_EQ("External Interface Functional Mockup Unit Import To Schedule 2", schedule2.nameString());
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
