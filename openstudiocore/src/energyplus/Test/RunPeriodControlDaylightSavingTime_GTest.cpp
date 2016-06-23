/**********************************************************************
*  Copyright (c) 2008-2016, Alliance for Sustainable Energy.
*  All rights reserved.
*
*  This library is free software; you can redistribute it and/or
*  modify it under the terms of the GNU Lesser General Public
*  License as published by the Free Software Foundation; either
*  version 2.1 of the License, or (at your option) any later version.
*
*  This library is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
*  Lesser General Public License for more details.
*
*  You should have received a copy of the GNU Lesser General Public
*  License along with this library; if not, write to the Free Software
*  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
**********************************************************************/

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

TEST_F(EnergyPlusFixture, ForwardTranslator_NoRunPeriodControlDaylightSavingTime)
{
  Model model;

  ForwardTranslator forwardTranslator;
  Workspace workspace = forwardTranslator.translateModel(model);

  EXPECT_EQ(0u, workspace.getObjectsByType(IddObjectType::RunPeriodControl_DaylightSavingTime).size());
}

TEST_F(EnergyPlusFixture, ForwardTranslator_RunPeriodControlDaylightSavingTime)
{
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

TEST_F(EnergyPlusFixture, ForwardTranslator_RunPeriodControlDaylightSavingTime2)
{
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

TEST_F(EnergyPlusFixture, ForwardTranslator_RunPeriodControlDaylightSavingTime3)
{
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