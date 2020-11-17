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

#include "../ForwardTranslator.hpp"
#include "../ReverseTranslator.hpp"

#include "../../utilities/data/TimeSeries.hpp"

#include "../../model/Model.hpp"
#include "../../model/ScheduleRuleset.hpp"
#include "../../model/ScheduleRuleset_Impl.hpp"
#include "../../model/ScheduleRule.hpp"
#include "../../model/ScheduleRule_Impl.hpp"
#include "../../model/ScheduleWeek.hpp"
#include "../../model/ScheduleWeek_Impl.hpp"
#include "../../model/ScheduleYear.hpp"
#include "../../model/ScheduleYear_Impl.hpp"
#include "../../model/ScheduleDay.hpp"
#include "../../model/ScheduleDay_Impl.hpp"
#include "../../model/YearDescription.hpp"
#include "../../model/YearDescription_Impl.hpp"

#include "../../utilities/idf/IdfExtensibleGroup.hpp"
#include "../../utilities/idf/WorkspaceExtensibleGroup.hpp"
#include <utilities/idd/Schedule_Year_FieldEnums.hxx>
#include <utilities/idd/Schedule_Week_Compact_FieldEnums.hxx>
#include <utilities/idd/Schedule_Week_Daily_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include <sstream>

using namespace openstudio::energyplus;
using namespace openstudio::model;
using namespace openstudio;

TEST_F(EnergyPlusFixture, ForwardTranslator_ScheduleRuleset_Simple) {
  Model model;
  ScheduleRuleset scheduleRuleset(model);

  // annual weekday rule
  ScheduleRule rule2(scheduleRuleset);
  rule2.setApplySunday(false);
  rule2.setApplyMonday(true);
  rule2.setApplyTuesday(true);
  rule2.setApplyWednesday(true);
  rule2.setApplyThursday(true);
  rule2.setApplyFriday(true);
  rule2.setApplySaturday(false);

  // annual weekend rule
  ScheduleRule rule1(scheduleRuleset);
  rule1.setApplySunday(true);
  rule1.setApplyMonday(false);
  rule1.setApplyTuesday(false);
  rule1.setApplyWednesday(false);
  rule1.setApplyThursday(false);
  rule1.setApplyFriday(false);
  rule1.setApplySaturday(true);

  ForwardTranslator ft;
  Workspace workspace = ft.translateModel(model);

  std::vector<WorkspaceObject> scheduleYears = workspace.getObjectsByType(IddObjectType::Schedule_Year);
  ASSERT_EQ(1u, scheduleYears.size());
  std::vector<IdfExtensibleGroup> extensibleGroups = scheduleYears[0].extensibleGroups();
  ASSERT_EQ(2u, extensibleGroups.size());

  // based on 2009 assumed year

  // 1/1-1/3
  ASSERT_TRUE(extensibleGroups[0].getInt(Schedule_YearExtensibleFields::StartMonth));
  EXPECT_EQ(1, extensibleGroups[0].getInt(Schedule_YearExtensibleFields::StartMonth).get());
  ASSERT_TRUE(extensibleGroups[0].getInt(Schedule_YearExtensibleFields::StartDay));
  EXPECT_EQ(1, extensibleGroups[0].getInt(Schedule_YearExtensibleFields::StartDay).get());
  ASSERT_TRUE(extensibleGroups[0].getInt(Schedule_YearExtensibleFields::EndMonth));
  EXPECT_EQ(1, extensibleGroups[0].getInt(Schedule_YearExtensibleFields::EndMonth).get());
  ASSERT_TRUE(extensibleGroups[0].getInt(Schedule_YearExtensibleFields::EndDay));
  EXPECT_EQ(3, extensibleGroups[0].getInt(Schedule_YearExtensibleFields::EndDay).get());

  // 1/4-12/31
  ASSERT_TRUE(extensibleGroups[1].getInt(Schedule_YearExtensibleFields::StartMonth));
  EXPECT_EQ(1, extensibleGroups[1].getInt(Schedule_YearExtensibleFields::StartMonth).get());
  ASSERT_TRUE(extensibleGroups[1].getInt(Schedule_YearExtensibleFields::StartDay));
  EXPECT_EQ(4, extensibleGroups[1].getInt(Schedule_YearExtensibleFields::StartDay).get());
  ASSERT_TRUE(extensibleGroups[1].getInt(Schedule_YearExtensibleFields::EndMonth));
  EXPECT_EQ(12, extensibleGroups[1].getInt(Schedule_YearExtensibleFields::EndMonth).get());
  ASSERT_TRUE(extensibleGroups[1].getInt(Schedule_YearExtensibleFields::EndDay));
  EXPECT_EQ(31, extensibleGroups[1].getInt(Schedule_YearExtensibleFields::EndDay).get());
}

TEST_F(EnergyPlusFixture, ForwardTranslator_ScheduleRuleset_Bug804) {
  Model model;
  ScheduleRuleset scheduleRuleset(model);

  // annual weekday rule
  ScheduleRule rule4(scheduleRuleset);
  rule4.setApplySunday(false);
  rule4.setApplyMonday(true);
  rule4.setApplyTuesday(true);
  rule4.setApplyWednesday(true);
  rule4.setApplyThursday(true);
  rule4.setApplyFriday(true);
  rule4.setApplySaturday(false);

  // annual weekend rule
  ScheduleRule rule3(scheduleRuleset);
  rule3.setApplySunday(true);
  rule3.setApplyMonday(false);
  rule3.setApplyTuesday(false);
  rule3.setApplyWednesday(false);
  rule3.setApplyThursday(false);
  rule3.setApplyFriday(false);
  rule3.setApplySaturday(true);

  // special weekday rule
  ScheduleRule rule2(scheduleRuleset);
  rule2.setApplySunday(false);
  rule2.setApplyMonday(true);
  rule2.setApplyTuesday(true);
  rule2.setApplyWednesday(true);
  rule2.setApplyThursday(true);
  rule2.setApplyFriday(true);
  rule2.setApplySaturday(false);
  rule2.setStartDate(Date(5, 1));
  rule2.setEndDate(Date(12, 13));

  // special weekend rule
  ScheduleRule rule1(scheduleRuleset);
  rule1.setApplySunday(true);
  rule1.setApplyMonday(false);
  rule1.setApplyTuesday(false);
  rule1.setApplyWednesday(false);
  rule1.setApplyThursday(false);
  rule1.setApplyFriday(false);
  rule1.setApplySaturday(true);
  rule1.setStartDate(Date(5, 1));
  rule1.setEndDate(Date(12, 13));

  ForwardTranslator ft;
  Workspace workspace = ft.translateModel(model);

  std::vector<WorkspaceObject> scheduleYears = workspace.getObjectsByType(IddObjectType::Schedule_Year);
  ASSERT_EQ(1u, scheduleYears.size());
  std::vector<IdfExtensibleGroup> extensibleGroups = scheduleYears[0].extensibleGroups();
  ASSERT_EQ(6u, extensibleGroups.size());

  // based on 2009 assumed year

  // 1/1-1/3
  ASSERT_TRUE(extensibleGroups[0].getInt(Schedule_YearExtensibleFields::StartMonth));
  EXPECT_EQ(1, extensibleGroups[0].getInt(Schedule_YearExtensibleFields::StartMonth).get());
  ASSERT_TRUE(extensibleGroups[0].getInt(Schedule_YearExtensibleFields::StartDay));
  EXPECT_EQ(1, extensibleGroups[0].getInt(Schedule_YearExtensibleFields::StartDay).get());
  ASSERT_TRUE(extensibleGroups[0].getInt(Schedule_YearExtensibleFields::EndMonth));
  EXPECT_EQ(1, extensibleGroups[0].getInt(Schedule_YearExtensibleFields::EndMonth).get());
  ASSERT_TRUE(extensibleGroups[0].getInt(Schedule_YearExtensibleFields::EndDay));
  EXPECT_EQ(3, extensibleGroups[0].getInt(Schedule_YearExtensibleFields::EndDay).get());

  // 1/4-4/25
  ASSERT_TRUE(extensibleGroups[1].getInt(Schedule_YearExtensibleFields::StartMonth));
  EXPECT_EQ(1, extensibleGroups[1].getInt(Schedule_YearExtensibleFields::StartMonth).get());
  ASSERT_TRUE(extensibleGroups[1].getInt(Schedule_YearExtensibleFields::StartDay));
  EXPECT_EQ(4, extensibleGroups[1].getInt(Schedule_YearExtensibleFields::StartDay).get());
  ASSERT_TRUE(extensibleGroups[1].getInt(Schedule_YearExtensibleFields::EndMonth));
  EXPECT_EQ(4, extensibleGroups[1].getInt(Schedule_YearExtensibleFields::EndMonth).get());
  ASSERT_TRUE(extensibleGroups[1].getInt(Schedule_YearExtensibleFields::EndDay));
  EXPECT_EQ(25, extensibleGroups[1].getInt(Schedule_YearExtensibleFields::EndDay).get());

  // 4/26-5/2
  ASSERT_TRUE(extensibleGroups[2].getInt(Schedule_YearExtensibleFields::StartMonth));
  EXPECT_EQ(4, extensibleGroups[2].getInt(Schedule_YearExtensibleFields::StartMonth).get());
  ASSERT_TRUE(extensibleGroups[2].getInt(Schedule_YearExtensibleFields::StartDay));
  EXPECT_EQ(26, extensibleGroups[2].getInt(Schedule_YearExtensibleFields::StartDay).get());
  ASSERT_TRUE(extensibleGroups[2].getInt(Schedule_YearExtensibleFields::EndMonth));
  EXPECT_EQ(5, extensibleGroups[2].getInt(Schedule_YearExtensibleFields::EndMonth).get());
  ASSERT_TRUE(extensibleGroups[2].getInt(Schedule_YearExtensibleFields::EndDay));
  EXPECT_EQ(2, extensibleGroups[2].getInt(Schedule_YearExtensibleFields::EndDay).get());

  // 5/3-12/12
  ASSERT_TRUE(extensibleGroups[3].getInt(Schedule_YearExtensibleFields::StartMonth));
  EXPECT_EQ(5, extensibleGroups[3].getInt(Schedule_YearExtensibleFields::StartMonth).get());
  ASSERT_TRUE(extensibleGroups[3].getInt(Schedule_YearExtensibleFields::StartDay));
  EXPECT_EQ(3, extensibleGroups[3].getInt(Schedule_YearExtensibleFields::StartDay).get());
  ASSERT_TRUE(extensibleGroups[3].getInt(Schedule_YearExtensibleFields::EndMonth));
  EXPECT_EQ(12, extensibleGroups[3].getInt(Schedule_YearExtensibleFields::EndMonth).get());
  ASSERT_TRUE(extensibleGroups[3].getInt(Schedule_YearExtensibleFields::EndDay));
  EXPECT_EQ(12, extensibleGroups[3].getInt(Schedule_YearExtensibleFields::EndDay).get());

  // 12/13-12/19
  ASSERT_TRUE(extensibleGroups[4].getInt(Schedule_YearExtensibleFields::StartMonth));
  EXPECT_EQ(12, extensibleGroups[4].getInt(Schedule_YearExtensibleFields::StartMonth).get());
  ASSERT_TRUE(extensibleGroups[4].getInt(Schedule_YearExtensibleFields::StartDay));
  EXPECT_EQ(13, extensibleGroups[4].getInt(Schedule_YearExtensibleFields::StartDay).get());
  ASSERT_TRUE(extensibleGroups[4].getInt(Schedule_YearExtensibleFields::EndMonth));
  EXPECT_EQ(12, extensibleGroups[4].getInt(Schedule_YearExtensibleFields::EndMonth).get());
  ASSERT_TRUE(extensibleGroups[4].getInt(Schedule_YearExtensibleFields::EndDay));
  EXPECT_EQ(19, extensibleGroups[4].getInt(Schedule_YearExtensibleFields::EndDay).get());

  // 12/20-12/31
  ASSERT_TRUE(extensibleGroups[5].getInt(Schedule_YearExtensibleFields::StartMonth));
  EXPECT_EQ(12, extensibleGroups[5].getInt(Schedule_YearExtensibleFields::StartMonth).get());
  ASSERT_TRUE(extensibleGroups[5].getInt(Schedule_YearExtensibleFields::StartDay));
  EXPECT_EQ(20, extensibleGroups[5].getInt(Schedule_YearExtensibleFields::StartDay).get());
  ASSERT_TRUE(extensibleGroups[5].getInt(Schedule_YearExtensibleFields::EndMonth));
  EXPECT_EQ(12, extensibleGroups[5].getInt(Schedule_YearExtensibleFields::EndMonth).get());
  ASSERT_TRUE(extensibleGroups[5].getInt(Schedule_YearExtensibleFields::EndDay));
  EXPECT_EQ(31, extensibleGroups[5].getInt(Schedule_YearExtensibleFields::EndDay).get());
}

TEST_F(EnergyPlusFixture, ForwardTranslator_ScheduleWeek_Bug2322) {
  Model model;
  model::YearDescription yd = model.getUniqueModelObject<model::YearDescription>();
  EXPECT_TRUE(yd.setDayofWeekforStartDay("Tuesday"));

  ScheduleWeek weeklySchd(model);
  weeklySchd.setName("TestWeek");

  // DLM: this date will not work since it is not consistent with the YearDescription
  Date endDate(MonthOfYear(12), 31);

  // DLM: this date will work since it is consistent with the YearDescription
  endDate = yd.makeDate(MonthOfYear(12), 31);

  ScheduleYear yearSchd(model);
  yearSchd.setName("TestSched");
  EXPECT_TRUE(yearSchd.addScheduleWeek(endDate, weeklySchd));

  ForwardTranslator ft;
  Workspace workspace = ft.translateModel(model);

  ASSERT_EQ(1u, workspace.getObjectsByType(IddObjectType::Schedule_Year).size());
  ASSERT_EQ(1u, workspace.getObjectsByType(IddObjectType::Schedule_Week_Daily).size());
  EXPECT_EQ("TestWeek", workspace.getObjectsByType(IddObjectType::Schedule_Week_Daily)[0].nameString());
  ASSERT_EQ(1u, workspace.getObjectsByType(IddObjectType::Schedule_Year)[0].extensibleGroups().size());
  ASSERT_TRUE(
    workspace.getObjectsByType(IddObjectType::Schedule_Year)[0].extensibleGroups()[0].getString(Schedule_YearExtensibleFields::Schedule_WeekName));
  EXPECT_EQ("TestWeek", workspace.getObjectsByType(IddObjectType::Schedule_Year)[0]
                          .extensibleGroups()[0]
                          .getString(Schedule_YearExtensibleFields::Schedule_WeekName)
                          .get());

  //std::stringstream ss;
  //ss << workspace;
  //std::string s = ss.str();
}

TEST_F(EnergyPlusFixture, ForwardTranslator_ScheduleWeek_Bug243) {
  Model model;

  // Make start day a Wednesday, before fix it would create a rule for Jan1-Jan4 as a result
  model::YearDescription yd = model.getUniqueModelObject<model::YearDescription>();
  EXPECT_TRUE(yd.setDayofWeekforStartDay("Wednesday"));

  ScheduleRuleset scheduleRuleset(model);
  scheduleRuleset.setName("Schedule Ruleset");
  ScheduleDay defaultDaySchedule = scheduleRuleset.defaultDaySchedule();
  defaultDaySchedule.setName("Default Day Schedule");

  ASSERT_EQ(1u, model.getModelObjects<ScheduleRuleset>().size());
  ASSERT_EQ(1u, model.getModelObjects<ScheduleDay>().size());

  ScheduleRule rule(scheduleRuleset);
  rule.setName("All Days Rule");
  ScheduleDay ruleSchedule = rule.daySchedule();
  ruleSchedule.setName("All Days Schedule");

  rule.setApplySunday(true);
  rule.setApplyMonday(true);
  rule.setApplyTuesday(true);
  rule.setApplyWednesday(true);
  rule.setApplyThursday(true);
  rule.setApplyFriday(true);
  rule.setApplySaturday(true);
  rule.setStartDate(Date(1, 1));
  rule.setEndDate(Date(12, 31));

  ASSERT_EQ(1u, model.getModelObjects<ScheduleRule>().size());
  ASSERT_EQ(2u, model.getModelObjects<ScheduleDay>().size());

  ForwardTranslator ft;
  Workspace workspace = ft.translateModel(model);

  std::vector<WorkspaceObject> scheduleYears = workspace.getObjectsByType(IddObjectType::Schedule_Year);
  ASSERT_EQ(1u, scheduleYears.size());
  std::vector<IdfExtensibleGroup> extensibleGroups = scheduleYears[0].extensibleGroups();
  ASSERT_EQ(1u, extensibleGroups.size());

  std::vector<WorkspaceObject> scheduleWeekDailys = workspace.getObjectsByType(IddObjectType::Schedule_Week_Daily);
  ASSERT_EQ(1u, scheduleWeekDailys.size());
  WorkspaceObject scheduleWeekDaily(scheduleWeekDailys[0]);

  // 1/1 to 12/31
  ASSERT_TRUE(extensibleGroups[0].getInt(Schedule_YearExtensibleFields::StartMonth));
  EXPECT_EQ(1, extensibleGroups[0].getInt(Schedule_YearExtensibleFields::StartMonth).get());
  ASSERT_TRUE(extensibleGroups[0].getInt(Schedule_YearExtensibleFields::StartDay));
  EXPECT_EQ(1, extensibleGroups[0].getInt(Schedule_YearExtensibleFields::StartDay).get());
  ASSERT_TRUE(extensibleGroups[0].getInt(Schedule_YearExtensibleFields::EndMonth));
  EXPECT_EQ(12, extensibleGroups[0].getInt(Schedule_YearExtensibleFields::EndMonth).get());
  ASSERT_TRUE(extensibleGroups[0].getInt(Schedule_YearExtensibleFields::EndDay));
  EXPECT_EQ(31, extensibleGroups[0].getInt(Schedule_YearExtensibleFields::EndDay).get());

  EXPECT_EQ(ruleSchedule.nameString(), scheduleWeekDaily.getString(Schedule_Week_DailyFields::SundaySchedule_DayName).get());
  EXPECT_EQ(ruleSchedule.nameString(), scheduleWeekDaily.getString(Schedule_Week_DailyFields::MondaySchedule_DayName).get());
  EXPECT_EQ(ruleSchedule.nameString(), scheduleWeekDaily.getString(Schedule_Week_DailyFields::TuesdaySchedule_DayName).get());
  EXPECT_EQ(ruleSchedule.nameString(), scheduleWeekDaily.getString(Schedule_Week_DailyFields::WednesdaySchedule_DayName).get());
  EXPECT_EQ(ruleSchedule.nameString(), scheduleWeekDaily.getString(Schedule_Week_DailyFields::ThursdaySchedule_DayName).get());
  EXPECT_EQ(ruleSchedule.nameString(), scheduleWeekDaily.getString(Schedule_Week_DailyFields::FridaySchedule_DayName).get());
  EXPECT_EQ(ruleSchedule.nameString(), scheduleWeekDaily.getString(Schedule_Week_DailyFields::SaturdaySchedule_DayName).get());

  EXPECT_EQ("Schedule Ruleset Week Rule - Jan1-Dec31", scheduleWeekDaily.nameString());
}

TEST_F(EnergyPlusFixture, ForwardTranslator_ScheduleWeek_Bug243_2) {
  Model model;

  model::YearDescription yd = model.getUniqueModelObject<model::YearDescription>();
  EXPECT_TRUE(yd.setDayofWeekforStartDay("Wednesday"));

  ScheduleRuleset scheduleRuleset(model);
  scheduleRuleset.setName("Schedule Ruleset");
  ScheduleDay defaultDaySchedule = scheduleRuleset.defaultDaySchedule();
  defaultDaySchedule.setName("Default Day Schedule");

  ASSERT_EQ(1u, model.getModelObjects<ScheduleRuleset>().size());
  ASSERT_EQ(1u, model.getModelObjects<ScheduleDay>().size());

  ScheduleRule rule(scheduleRuleset);
  rule.setName("All Days Except Mon and Wed Rule");
  ScheduleDay ruleSchedule = rule.daySchedule();
  ruleSchedule.setName("All Days Except Mon and Wed Schedule");

  rule.setApplySunday(true);
  rule.setApplyMonday(false);
  rule.setApplyTuesday(true);
  rule.setApplyWednesday(false);
  rule.setApplyThursday(true);
  rule.setApplyFriday(true);
  rule.setApplySaturday(true);

  ASSERT_EQ(1u, model.getModelObjects<ScheduleRule>().size());
  ASSERT_EQ(2u, model.getModelObjects<ScheduleDay>().size());

  ForwardTranslator ft;
  Workspace workspace = ft.translateModel(model);

  std::vector<WorkspaceObject> scheduleYears = workspace.getObjectsByType(IddObjectType::Schedule_Year);
  ASSERT_EQ(1u, scheduleYears.size());
  std::vector<IdfExtensibleGroup> extensibleGroups = scheduleYears[0].extensibleGroups();
  ASSERT_EQ(2u, extensibleGroups.size());
  std::vector<WorkspaceExtensibleGroup> workspaceExtensibleGroups = subsetCastVector<WorkspaceExtensibleGroup>(extensibleGroups);

  std::vector<WorkspaceObject> scheduleWeekDailys = workspace.getObjectsByType(IddObjectType::Schedule_Week_Daily);
  ASSERT_EQ(2u, scheduleWeekDailys.size());

  // 1/1 to 1/4
  WorkspaceExtensibleGroup w_eg(workspaceExtensibleGroups[0]);
  ASSERT_TRUE(extensibleGroups[0].getInt(Schedule_YearExtensibleFields::StartMonth));
  EXPECT_EQ(1, extensibleGroups[0].getInt(Schedule_YearExtensibleFields::StartMonth).get());
  ASSERT_TRUE(extensibleGroups[0].getInt(Schedule_YearExtensibleFields::StartDay));
  EXPECT_EQ(1, extensibleGroups[0].getInt(Schedule_YearExtensibleFields::StartDay).get());
  ASSERT_TRUE(extensibleGroups[0].getInt(Schedule_YearExtensibleFields::EndMonth));
  EXPECT_EQ(1, extensibleGroups[0].getInt(Schedule_YearExtensibleFields::EndMonth).get());
  ASSERT_TRUE(extensibleGroups[0].getInt(Schedule_YearExtensibleFields::EndDay));
  EXPECT_EQ(4, extensibleGroups[0].getInt(Schedule_YearExtensibleFields::EndDay).get());

  boost::optional<WorkspaceObject> _scheduleWeekDaily1 = w_eg.getTarget(Schedule_YearExtensibleFields::Schedule_WeekName);
  EXPECT_EQ(defaultDaySchedule.nameString(), _scheduleWeekDaily1->getString(Schedule_Week_DailyFields::SundaySchedule_DayName).get());
  EXPECT_EQ(defaultDaySchedule.nameString(), _scheduleWeekDaily1->getString(Schedule_Week_DailyFields::MondaySchedule_DayName).get());
  EXPECT_EQ(defaultDaySchedule.nameString(), _scheduleWeekDaily1->getString(Schedule_Week_DailyFields::TuesdaySchedule_DayName).get());
  EXPECT_EQ(defaultDaySchedule.nameString(), _scheduleWeekDaily1->getString(Schedule_Week_DailyFields::WednesdaySchedule_DayName).get());
  EXPECT_EQ(ruleSchedule.nameString(), _scheduleWeekDaily1->getString(Schedule_Week_DailyFields::ThursdaySchedule_DayName).get());
  EXPECT_EQ(ruleSchedule.nameString(), _scheduleWeekDaily1->getString(Schedule_Week_DailyFields::FridaySchedule_DayName).get());
  EXPECT_EQ(ruleSchedule.nameString(), _scheduleWeekDaily1->getString(Schedule_Week_DailyFields::SaturdaySchedule_DayName).get());

  EXPECT_EQ("Schedule Ruleset Week Rule - Jan1-Jan4", _scheduleWeekDaily1->nameString());

  // 1/4 to 12/31
  w_eg = workspaceExtensibleGroups[1];
  ASSERT_TRUE(extensibleGroups[1].getInt(Schedule_YearExtensibleFields::StartMonth));
  EXPECT_EQ(1, extensibleGroups[1].getInt(Schedule_YearExtensibleFields::StartMonth).get());
  ASSERT_TRUE(extensibleGroups[1].getInt(Schedule_YearExtensibleFields::StartDay));
  EXPECT_EQ(5, extensibleGroups[1].getInt(Schedule_YearExtensibleFields::StartDay).get());
  ASSERT_TRUE(extensibleGroups[1].getInt(Schedule_YearExtensibleFields::EndMonth));
  EXPECT_EQ(12, extensibleGroups[1].getInt(Schedule_YearExtensibleFields::EndMonth).get());
  ASSERT_TRUE(extensibleGroups[1].getInt(Schedule_YearExtensibleFields::EndDay));
  EXPECT_EQ(31, extensibleGroups[1].getInt(Schedule_YearExtensibleFields::EndDay).get());

  boost::optional<WorkspaceObject> _scheduleWeekDaily2 = w_eg.getTarget(Schedule_YearExtensibleFields::Schedule_WeekName);
  EXPECT_EQ(ruleSchedule.nameString(), _scheduleWeekDaily2->getString(Schedule_Week_DailyFields::SundaySchedule_DayName).get());
  EXPECT_EQ(defaultDaySchedule.nameString(), _scheduleWeekDaily2->getString(Schedule_Week_DailyFields::MondaySchedule_DayName).get());
  EXPECT_EQ(ruleSchedule.nameString(), _scheduleWeekDaily2->getString(Schedule_Week_DailyFields::TuesdaySchedule_DayName).get());
  EXPECT_EQ(defaultDaySchedule.nameString(), _scheduleWeekDaily2->getString(Schedule_Week_DailyFields::WednesdaySchedule_DayName).get());
  EXPECT_EQ(ruleSchedule.nameString(), _scheduleWeekDaily2->getString(Schedule_Week_DailyFields::ThursdaySchedule_DayName).get());
  EXPECT_EQ(ruleSchedule.nameString(), _scheduleWeekDaily2->getString(Schedule_Week_DailyFields::FridaySchedule_DayName).get());
  EXPECT_EQ(ruleSchedule.nameString(), _scheduleWeekDaily2->getString(Schedule_Week_DailyFields::SaturdaySchedule_DayName).get());

  EXPECT_EQ("Schedule Ruleset Week Rule - Jan5-Dec31", _scheduleWeekDaily2->nameString());
}

TEST_F(EnergyPlusFixture, ForwardTranslator_ScheduleWeek_Bug243_3) {
  Model model;

  model::YearDescription yd = model.getUniqueModelObject<model::YearDescription>();
  EXPECT_TRUE(yd.setDayofWeekforStartDay("Wednesday"));

  ScheduleRuleset scheduleRuleset(model);
  scheduleRuleset.setName("Schedule Ruleset");
  ScheduleDay defaultDaySchedule = scheduleRuleset.defaultDaySchedule();
  defaultDaySchedule.setName("Default Day Schedule");

  ASSERT_EQ(1u, model.getModelObjects<ScheduleRuleset>().size());
  ASSERT_EQ(1u, model.getModelObjects<ScheduleDay>().size());

  ScheduleRule rule(scheduleRuleset);
  rule.setName("1/3 - 12/29 Days Except Mon and Wed Rule");
  ScheduleDay ruleSchedule = rule.daySchedule();
  ruleSchedule.setName("All Days Except Mon and Wed Schedule");

  rule.setApplySunday(true);
  rule.setApplyMonday(false);
  rule.setApplyTuesday(true);
  rule.setApplyWednesday(false);
  rule.setApplyThursday(true);
  rule.setApplyFriday(true);
  rule.setApplySaturday(true);
  rule.setStartDate(Date(1, 3));
  rule.setEndDate(Date(12, 29));

  ASSERT_EQ(1u, model.getModelObjects<ScheduleRule>().size());
  ASSERT_EQ(2u, model.getModelObjects<ScheduleDay>().size());

  ForwardTranslator ft;
  Workspace workspace = ft.translateModel(model);

  std::vector<WorkspaceObject> scheduleYears = workspace.getObjectsByType(IddObjectType::Schedule_Year);
  ASSERT_EQ(1u, scheduleYears.size());
  std::vector<IdfExtensibleGroup> extensibleGroups = scheduleYears[0].extensibleGroups();
  ASSERT_EQ(3u, extensibleGroups.size());
  std::vector<WorkspaceExtensibleGroup> workspaceExtensibleGroups = subsetCastVector<WorkspaceExtensibleGroup>(extensibleGroups);

  std::vector<WorkspaceObject> scheduleWeekDailys = workspace.getObjectsByType(IddObjectType::Schedule_Week_Daily);
  ASSERT_EQ(3u, scheduleWeekDailys.size());

  // 1/1 to 1/4
  WorkspaceExtensibleGroup w_eg(workspaceExtensibleGroups[0]);
  ASSERT_TRUE(w_eg.getInt(Schedule_YearExtensibleFields::StartMonth));
  EXPECT_EQ(1, w_eg.getInt(Schedule_YearExtensibleFields::StartMonth).get());
  ASSERT_TRUE(w_eg.getInt(Schedule_YearExtensibleFields::StartDay));
  EXPECT_EQ(1, w_eg.getInt(Schedule_YearExtensibleFields::StartDay).get());
  ASSERT_TRUE(w_eg.getInt(Schedule_YearExtensibleFields::EndMonth));
  EXPECT_EQ(1, w_eg.getInt(Schedule_YearExtensibleFields::EndMonth).get());
  ASSERT_TRUE(w_eg.getInt(Schedule_YearExtensibleFields::EndDay));
  EXPECT_EQ(4, w_eg.getInt(Schedule_YearExtensibleFields::EndDay).get());

  boost::optional<WorkspaceObject> _scheduleWeekDaily1 = w_eg.getTarget(Schedule_YearExtensibleFields::Schedule_WeekName);
  EXPECT_EQ(defaultDaySchedule.nameString(), _scheduleWeekDaily1->getString(Schedule_Week_DailyFields::SundaySchedule_DayName).get());
  EXPECT_EQ(defaultDaySchedule.nameString(), _scheduleWeekDaily1->getString(Schedule_Week_DailyFields::MondaySchedule_DayName).get());
  EXPECT_EQ(defaultDaySchedule.nameString(), _scheduleWeekDaily1->getString(Schedule_Week_DailyFields::TuesdaySchedule_DayName).get());
  EXPECT_EQ(defaultDaySchedule.nameString(), _scheduleWeekDaily1->getString(Schedule_Week_DailyFields::WednesdaySchedule_DayName).get());
  EXPECT_EQ(defaultDaySchedule.nameString(), _scheduleWeekDaily1->getString(Schedule_Week_DailyFields::ThursdaySchedule_DayName).get());
  EXPECT_EQ(ruleSchedule.nameString(), _scheduleWeekDaily1->getString(Schedule_Week_DailyFields::FridaySchedule_DayName).get());
  EXPECT_EQ(ruleSchedule.nameString(), _scheduleWeekDaily1->getString(Schedule_Week_DailyFields::SaturdaySchedule_DayName).get());

  EXPECT_EQ("Schedule Ruleset Week Rule - Jan1-Jan4", _scheduleWeekDaily1->nameString());

  // 1/5 to 12/27
  w_eg = workspaceExtensibleGroups[1];
  ASSERT_TRUE(w_eg.getInt(Schedule_YearExtensibleFields::StartMonth));
  EXPECT_EQ(1, w_eg.getInt(Schedule_YearExtensibleFields::StartMonth).get());
  ASSERT_TRUE(w_eg.getInt(Schedule_YearExtensibleFields::StartDay));
  EXPECT_EQ(5, w_eg.getInt(Schedule_YearExtensibleFields::StartDay).get());
  ASSERT_TRUE(w_eg.getInt(Schedule_YearExtensibleFields::EndMonth));
  EXPECT_EQ(12, w_eg.getInt(Schedule_YearExtensibleFields::EndMonth).get());
  ASSERT_TRUE(w_eg.getInt(Schedule_YearExtensibleFields::EndDay));
  EXPECT_EQ(27, w_eg.getInt(Schedule_YearExtensibleFields::EndDay).get());

  boost::optional<WorkspaceObject> _scheduleWeekDaily2 = w_eg.getTarget(Schedule_YearExtensibleFields::Schedule_WeekName);
  EXPECT_EQ(ruleSchedule.nameString(), _scheduleWeekDaily2->getString(Schedule_Week_DailyFields::SundaySchedule_DayName).get());
  EXPECT_EQ(defaultDaySchedule.nameString(), _scheduleWeekDaily2->getString(Schedule_Week_DailyFields::MondaySchedule_DayName).get());
  EXPECT_EQ(ruleSchedule.nameString(), _scheduleWeekDaily2->getString(Schedule_Week_DailyFields::TuesdaySchedule_DayName).get());
  EXPECT_EQ(defaultDaySchedule.nameString(), _scheduleWeekDaily2->getString(Schedule_Week_DailyFields::WednesdaySchedule_DayName).get());
  EXPECT_EQ(ruleSchedule.nameString(), _scheduleWeekDaily2->getString(Schedule_Week_DailyFields::ThursdaySchedule_DayName).get());
  EXPECT_EQ(ruleSchedule.nameString(), _scheduleWeekDaily2->getString(Schedule_Week_DailyFields::FridaySchedule_DayName).get());
  EXPECT_EQ(ruleSchedule.nameString(), _scheduleWeekDaily2->getString(Schedule_Week_DailyFields::SaturdaySchedule_DayName).get());

  EXPECT_EQ("Schedule Ruleset Week Rule - Jan5-Dec27", _scheduleWeekDaily2->nameString());

  // 12/28 to 12/31
  w_eg = workspaceExtensibleGroups[2];
  ASSERT_TRUE(w_eg.getInt(Schedule_YearExtensibleFields::StartMonth));
  EXPECT_EQ(12, w_eg.getInt(Schedule_YearExtensibleFields::StartMonth).get());
  ASSERT_TRUE(w_eg.getInt(Schedule_YearExtensibleFields::StartDay));
  EXPECT_EQ(28, w_eg.getInt(Schedule_YearExtensibleFields::StartDay).get());
  ASSERT_TRUE(w_eg.getInt(Schedule_YearExtensibleFields::EndMonth));
  EXPECT_EQ(12, w_eg.getInt(Schedule_YearExtensibleFields::EndMonth).get());
  ASSERT_TRUE(w_eg.getInt(Schedule_YearExtensibleFields::EndDay));
  EXPECT_EQ(31, w_eg.getInt(Schedule_YearExtensibleFields::EndDay).get());

  boost::optional<WorkspaceObject> _scheduleWeekDaily3 = w_eg.getTarget(Schedule_YearExtensibleFields::Schedule_WeekName);
  EXPECT_EQ(ruleSchedule.nameString(), _scheduleWeekDaily3->getString(Schedule_Week_DailyFields::SundaySchedule_DayName).get());
  EXPECT_EQ(defaultDaySchedule.nameString(), _scheduleWeekDaily3->getString(Schedule_Week_DailyFields::MondaySchedule_DayName).get());
  EXPECT_EQ(defaultDaySchedule.nameString(), _scheduleWeekDaily3->getString(Schedule_Week_DailyFields::TuesdaySchedule_DayName).get());
  EXPECT_EQ(defaultDaySchedule.nameString(), _scheduleWeekDaily3->getString(Schedule_Week_DailyFields::WednesdaySchedule_DayName).get());
  EXPECT_EQ(ruleSchedule.nameString(), _scheduleWeekDaily3->getString(Schedule_Week_DailyFields::ThursdaySchedule_DayName).get());
  EXPECT_EQ(ruleSchedule.nameString(), _scheduleWeekDaily3->getString(Schedule_Week_DailyFields::FridaySchedule_DayName).get());
  EXPECT_EQ(ruleSchedule.nameString(), _scheduleWeekDaily3->getString(Schedule_Week_DailyFields::SaturdaySchedule_DayName).get());

  EXPECT_EQ("Schedule Ruleset Week Rule - Dec28-Dec31", _scheduleWeekDaily3->nameString());
}

TEST_F(EnergyPlusFixture, ForwardTranslator_SpecialDays) {

  Model model;
  model::YearDescription yd = model.getUniqueModelObject<model::YearDescription>();
  EXPECT_TRUE(yd.setDayofWeekforStartDay("Sunday"));

  ScheduleRuleset scheduleRuleset(model);
  scheduleRuleset.setName("Schedule Ruleset");
  ScheduleDay defaultDaySchedule = scheduleRuleset.defaultDaySchedule();
  defaultDaySchedule.setName("Default Day Schedule");

  // Add Special Days schedules
  scheduleRuleset.setSummerDesignDaySchedule(defaultDaySchedule);
  ScheduleDay summerDesignDaySchedule = scheduleRuleset.summerDesignDaySchedule();
  summerDesignDaySchedule.setName("Summer Design Day Schedule");

  scheduleRuleset.setWinterDesignDaySchedule(defaultDaySchedule);
  ScheduleDay winterDesignDaySchedule = scheduleRuleset.winterDesignDaySchedule();
  winterDesignDaySchedule.setName("Winter Design Day Schedule");

  scheduleRuleset.setHolidaySchedule(defaultDaySchedule);
  ScheduleDay holidaySchedule = scheduleRuleset.holidaySchedule();
  holidaySchedule.setName("Holiday Schedule");

  // annual weekday rule
  ScheduleRule weekdayRule(scheduleRuleset);
  weekdayRule.setName("All Year Weekdays Rule");
  ScheduleDay weekdaySchedule = weekdayRule.daySchedule();
  weekdaySchedule.setName("Weekdays Schedule");
  weekdayRule.setApplySunday(false);
  weekdayRule.setApplyMonday(true);
  weekdayRule.setApplyTuesday(true);
  weekdayRule.setApplyWednesday(true);
  weekdayRule.setApplyThursday(true);
  weekdayRule.setApplyFriday(true);
  weekdayRule.setApplySaturday(false);

  // annual weekend rule
  ScheduleRule weekendRule(scheduleRuleset);
  weekendRule.setName("All Year Weekends Rule");
  ScheduleDay weekendSchedule = weekendRule.daySchedule();
  weekendSchedule.setName("Weekends Schedule");
  weekendRule.setApplyMonday(false);
  weekendRule.setApplyTuesday(false);
  weekendRule.setApplyWednesday(false);
  weekendRule.setApplyThursday(false);
  weekendRule.setApplyFriday(false);
  weekendRule.setApplySaturday(true);
  weekendRule.setApplySunday(true);

  ForwardTranslator ft;
  Workspace workspace = ft.translateModel(model);

  std::vector<WorkspaceObject> scheduleYears = workspace.getObjectsByType(IddObjectType::Schedule_Year);
  ASSERT_EQ(1u, scheduleYears.size());
  std::vector<IdfExtensibleGroup> extensibleGroups = scheduleYears[0].extensibleGroups();
  ASSERT_EQ(1u, extensibleGroups.size());
  std::vector<WorkspaceObject> scheduleWeekDailys = workspace.getObjectsByType(IddObjectType::Schedule_Week_Daily);
  ASSERT_EQ(1u, scheduleWeekDailys.size());
  WorkspaceObject scheduleWeekDaily(scheduleWeekDailys[0]);

  // Weekdays
  EXPECT_EQ(weekdaySchedule.nameString(), scheduleWeekDaily.getString(Schedule_Week_DailyFields::MondaySchedule_DayName).get());
  EXPECT_EQ(weekdaySchedule.nameString(), scheduleWeekDaily.getString(Schedule_Week_DailyFields::TuesdaySchedule_DayName).get());
  EXPECT_EQ(weekdaySchedule.nameString(), scheduleWeekDaily.getString(Schedule_Week_DailyFields::WednesdaySchedule_DayName).get());
  EXPECT_EQ(weekdaySchedule.nameString(), scheduleWeekDaily.getString(Schedule_Week_DailyFields::ThursdaySchedule_DayName).get());
  EXPECT_EQ(weekdaySchedule.nameString(), scheduleWeekDaily.getString(Schedule_Week_DailyFields::FridaySchedule_DayName).get());

  // Weekends
  EXPECT_EQ(weekendSchedule.nameString(), scheduleWeekDaily.getString(Schedule_Week_DailyFields::SaturdaySchedule_DayName).get());
  EXPECT_EQ(weekendSchedule.nameString(), scheduleWeekDaily.getString(Schedule_Week_DailyFields::SundaySchedule_DayName).get());

  // Special Days
  EXPECT_EQ(holidaySchedule.nameString(), scheduleWeekDaily.getString(Schedule_Week_DailyFields::HolidaySchedule_DayName).get());
  EXPECT_EQ(summerDesignDaySchedule.nameString(), scheduleWeekDaily.getString(Schedule_Week_DailyFields::SummerDesignDaySchedule_DayName).get());
  EXPECT_EQ(winterDesignDaySchedule.nameString(), scheduleWeekDaily.getString(Schedule_Week_DailyFields::WinterDesignDaySchedule_DayName).get());
  // Not Exposed yet
  EXPECT_EQ(defaultDaySchedule.nameString(), scheduleWeekDaily.getString(Schedule_Week_DailyFields::CustomDay1Schedule_DayName).get());
  EXPECT_EQ(defaultDaySchedule.nameString(), scheduleWeekDaily.getString(Schedule_Week_DailyFields::CustomDay2Schedule_DayName).get());
}