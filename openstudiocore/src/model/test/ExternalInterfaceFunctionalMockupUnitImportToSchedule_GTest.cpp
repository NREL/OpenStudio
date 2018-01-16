/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2016, Alliance for Sustainable Energy, LLC. All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 *  following conditions are met:
 *
 *  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
 *  disclaimer.
 *
 *  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *  following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 *  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote
 *  products derived from this software without specific prior written permission from the respective party.
 *
 *  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative
 *  works may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without
 *  specific prior written permission from Alliance for Sustainable Energy, LLC.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER, THE UNITED STATES GOVERNMENT, OR ANY CONTRIBUTORS BE LIABLE FOR
 *  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **********************************************************************************************************************/

#include <gtest/gtest.h>

#include <model/test/ModelFixture.hpp>

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
  EXPECT_EQ(stl, schedule.scheduleTypeLimits());
  EXPECT_TRUE(schedule.resetScheduleTypeLimits());
  boost::optional<ScheduleTypeLimits> stl2 = schedule.scheduleTypeLimits();
  EXPECT_TRUE(!stl2.is_initialized());
}

