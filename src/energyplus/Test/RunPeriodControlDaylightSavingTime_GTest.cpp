/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2020, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
*  following conditions are met:
*
*  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
*  disclaimer.
*
*  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
*  disclaimer in the documentation and/or other materials provided with the distribution.
*
*  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote products
*  derived from this software without specific prior written permission from the respective party.
*
*  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative works
*  may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without specific prior
*  written permission from Alliance for Sustainable Energy, LLC.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER(S) AND ANY CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
*  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
*  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER(S), ANY CONTRIBUTORS, THE UNITED STATES GOVERNMENT, OR THE UNITED
*  STATES DEPARTMENT OF ENERGY, NOR ANY OF THEIR EMPLOYEES, BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
*  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
*  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
*  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
*  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
***********************************************************************************************************************/

#include <gtest/gtest.h>
#include "EnergyPlusFixture.hpp"

#include "../ErrorFile.hpp"
#include "../ForwardTranslator.hpp"
#include "../ReverseTranslator.hpp"

#include "../../model/Model.hpp"
#include "../../model/RunPeriodControlDaylightSavingTime.hpp"
#include "../../model/RunPeriodControlDaylightSavingTime_Impl.hpp"

#include "../../utilities/time/Date.hpp"

#include <utilities/idd/OS_RunPeriodControl_DaylightSavingTime_FieldEnums.hxx>
#include <utilities/idd/RunPeriodControl_DaylightSavingTime_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include <resources.hxx>

#include <sstream>

using namespace openstudio;
using namespace openstudio::model;
using openstudio::energyplus::ForwardTranslator;
using openstudio::energyplus::ReverseTranslator;

/*
! U.S. Standard for Daylight Saving
RunPeriodControl:DaylightSavingTime,2nd Sunday in March, 1st Sunday in November; !2007
! Brazil standard
RunPeriodControl:DaylightSavingTime,1st Sunday in October, Last Sunday in February;
! European Standard
RunPeriodControl:DaylightSavingTime, Last Sunday in March, Last Sunday in October;
! Syria Standard
RunPeriodControl:DaylightSavingTime, 4/1, 10/1;
*/

TEST_F(EnergyPlusFixture, ForwardTranslator_NoRunPeriodControlDaylightSavingTime) {
  Model model;

  ForwardTranslator forwardTranslator;
  Workspace workspace = forwardTranslator.translateModel(model);

  EXPECT_EQ(0u, workspace.getObjectsByType(IddObjectType::RunPeriodControl_DaylightSavingTime).size());
}

TEST_F(EnergyPlusFixture, ForwardTranslator_RunPeriodControlDaylightSavingTime) {
  Model model;
  RunPeriodControlDaylightSavingTime dst = model.getUniqueModelObject<RunPeriodControlDaylightSavingTime>();
  // defaults to US

  ForwardTranslator forwardTranslator;
  Workspace workspace = forwardTranslator.translateModel(model);

  ASSERT_EQ(1u, workspace.getObjectsByType(IddObjectType::RunPeriodControl_DaylightSavingTime).size());

  WorkspaceObject object = workspace.getObjectsByType(IddObjectType::RunPeriodControl_DaylightSavingTime)[0];
  ASSERT_TRUE(object.getString(RunPeriodControl_DaylightSavingTimeFields::StartDate));
  EXPECT_EQ("2nd Sunday in March", object.getString(RunPeriodControl_DaylightSavingTimeFields::StartDate).get());
  ASSERT_TRUE(object.getString(RunPeriodControl_DaylightSavingTimeFields::EndDate));
  EXPECT_EQ("1st Sunday in November", object.getString(RunPeriodControl_DaylightSavingTimeFields::EndDate).get());

  ReverseTranslator reverseTranslator;
  Model model2 = reverseTranslator.translateWorkspace(workspace);

  ASSERT_TRUE(model2.getOptionalUniqueModelObject<RunPeriodControlDaylightSavingTime>());
  RunPeriodControlDaylightSavingTime dst2 = model2.getUniqueModelObject<RunPeriodControlDaylightSavingTime>();

  ASSERT_TRUE(dst2.getString(OS_RunPeriodControl_DaylightSavingTimeFields::StartDate));
  EXPECT_EQ("2nd Sunday in March", dst2.getString(OS_RunPeriodControl_DaylightSavingTimeFields::StartDate).get());
  ASSERT_TRUE(dst2.startNthDayOfWeekInMonth());
  EXPECT_EQ(NthDayOfWeekInMonth::second, dst2.startNthDayOfWeekInMonth()->value());
  ASSERT_TRUE(dst2.getString(OS_RunPeriodControl_DaylightSavingTimeFields::EndDate));
  EXPECT_EQ("1st Sunday in November", dst2.getString(OS_RunPeriodControl_DaylightSavingTimeFields::EndDate).get());
  ASSERT_TRUE(dst2.endNthDayOfWeekInMonth());
  EXPECT_EQ(NthDayOfWeekInMonth::first, dst2.endNthDayOfWeekInMonth()->value());
}

TEST_F(EnergyPlusFixture, ForwardTranslator_RunPeriodControlDaylightSavingTime2) {
  Model model;
  RunPeriodControlDaylightSavingTime dst = model.getUniqueModelObject<RunPeriodControlDaylightSavingTime>();
  //European Last Sunday in March, Last Sunday in October;
  dst.setStartDate(NthDayOfWeekInMonth::fifth, DayOfWeek::Sunday, MonthOfYear::Mar);
  dst.setEndDate(NthDayOfWeekInMonth::fifth, DayOfWeek::Sunday, MonthOfYear::Oct);

  ForwardTranslator forwardTranslator;
  Workspace workspace = forwardTranslator.translateModel(model);

  ASSERT_EQ(1u, workspace.getObjectsByType(IddObjectType::RunPeriodControl_DaylightSavingTime).size());

  WorkspaceObject object = workspace.getObjectsByType(IddObjectType::RunPeriodControl_DaylightSavingTime)[0];
  ASSERT_TRUE(object.getString(RunPeriodControl_DaylightSavingTimeFields::StartDate));
  EXPECT_EQ("Last Sunday in March", object.getString(RunPeriodControl_DaylightSavingTimeFields::StartDate).get());
  ASSERT_TRUE(object.getString(RunPeriodControl_DaylightSavingTimeFields::EndDate));
  EXPECT_EQ("Last Sunday in October", object.getString(RunPeriodControl_DaylightSavingTimeFields::EndDate).get());

  ReverseTranslator reverseTranslator;
  Model model2 = reverseTranslator.translateWorkspace(workspace);

  ASSERT_TRUE(model2.getOptionalUniqueModelObject<RunPeriodControlDaylightSavingTime>());
  RunPeriodControlDaylightSavingTime dst2 = model2.getUniqueModelObject<RunPeriodControlDaylightSavingTime>();

  ASSERT_TRUE(dst2.getString(OS_RunPeriodControl_DaylightSavingTimeFields::StartDate));
  EXPECT_EQ("Last Sunday in March", dst2.getString(OS_RunPeriodControl_DaylightSavingTimeFields::StartDate).get());
  ASSERT_TRUE(dst2.startNthDayOfWeekInMonth());
  EXPECT_EQ(NthDayOfWeekInMonth::fifth, dst2.startNthDayOfWeekInMonth()->value());
  ASSERT_TRUE(dst2.getString(OS_RunPeriodControl_DaylightSavingTimeFields::EndDate));
  EXPECT_EQ("Last Sunday in October", dst2.getString(OS_RunPeriodControl_DaylightSavingTimeFields::EndDate).get());
  ASSERT_TRUE(dst2.endNthDayOfWeekInMonth());
  EXPECT_EQ(NthDayOfWeekInMonth::fifth, dst2.endNthDayOfWeekInMonth()->value());
}

TEST_F(EnergyPlusFixture, ForwardTranslator_RunPeriodControlDaylightSavingTime3) {
  Model model;
  RunPeriodControlDaylightSavingTime dst = model.getUniqueModelObject<RunPeriodControlDaylightSavingTime>();
  //Syrian 4/1, 10/1;
  dst.setStartDate(MonthOfYear::Mar, 1);
  dst.setEndDate(MonthOfYear::Oct, 1);

  ForwardTranslator forwardTranslator;
  Workspace workspace = forwardTranslator.translateModel(model);

  ASSERT_EQ(1u, workspace.getObjectsByType(IddObjectType::RunPeriodControl_DaylightSavingTime).size());

  WorkspaceObject object = workspace.getObjectsByType(IddObjectType::RunPeriodControl_DaylightSavingTime)[0];
  ASSERT_TRUE(object.getString(RunPeriodControl_DaylightSavingTimeFields::StartDate));
  EXPECT_EQ("3/1", object.getString(RunPeriodControl_DaylightSavingTimeFields::StartDate).get());
  ASSERT_TRUE(object.getString(RunPeriodControl_DaylightSavingTimeFields::EndDate));
  EXPECT_EQ("10/1", object.getString(RunPeriodControl_DaylightSavingTimeFields::EndDate).get());

  ReverseTranslator reverseTranslator;
  Model model2 = reverseTranslator.translateWorkspace(workspace);

  ASSERT_TRUE(model2.getOptionalUniqueModelObject<RunPeriodControlDaylightSavingTime>());
  RunPeriodControlDaylightSavingTime dst2 = model2.getUniqueModelObject<RunPeriodControlDaylightSavingTime>();

  ASSERT_TRUE(dst2.getString(OS_RunPeriodControl_DaylightSavingTimeFields::StartDate));
  EXPECT_EQ("3/1", dst2.getString(OS_RunPeriodControl_DaylightSavingTimeFields::StartDate).get());
  EXPECT_FALSE(dst2.startNthDayOfWeekInMonth());
  ASSERT_TRUE(dst2.getString(OS_RunPeriodControl_DaylightSavingTimeFields::EndDate));
  EXPECT_EQ("10/1", dst2.getString(OS_RunPeriodControl_DaylightSavingTimeFields::EndDate).get());
  EXPECT_FALSE(dst2.endNthDayOfWeekInMonth());
}