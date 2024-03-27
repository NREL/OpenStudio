/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>
#include "EnergyPlusFixture.hpp"

#include "../ForwardTranslator.hpp"
#include "../ReverseTranslator.hpp"

#include "../../utilities/data/TimeSeries.hpp"

#include "../../model/Model.hpp"
#include "../../model/ScheduleTypeLimits.hpp"
#include "../../model/ScheduleTypeLimits_Impl.hpp"
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

  ASSERT_EQ(1u, model.getConcreteModelObjects<ScheduleRuleset>().size());
  ASSERT_EQ(1u, model.getConcreteModelObjects<ScheduleDay>().size());

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

  ASSERT_EQ(1u, model.getConcreteModelObjects<ScheduleRule>().size());
  ASSERT_EQ(2u, model.getConcreteModelObjects<ScheduleDay>().size());

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

  ASSERT_EQ(1u, model.getConcreteModelObjects<ScheduleRuleset>().size());
  ASSERT_EQ(1u, model.getConcreteModelObjects<ScheduleDay>().size());

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

  ASSERT_EQ(1u, model.getConcreteModelObjects<ScheduleRule>().size());
  ASSERT_EQ(2u, model.getConcreteModelObjects<ScheduleDay>().size());

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

  ASSERT_EQ(1u, model.getConcreteModelObjects<ScheduleRuleset>().size());
  ASSERT_EQ(1u, model.getConcreteModelObjects<ScheduleDay>().size());

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

  ASSERT_EQ(1u, model.getConcreteModelObjects<ScheduleRule>().size());
  ASSERT_EQ(2u, model.getConcreteModelObjects<ScheduleDay>().size());

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

  scheduleRuleset.setCustomDay1Schedule(defaultDaySchedule);
  ScheduleDay customDay1Schedule = scheduleRuleset.customDay1Schedule();
  customDay1Schedule.setName("Custom Day 1 Schedule");

  scheduleRuleset.setCustomDay2Schedule(defaultDaySchedule);
  ScheduleDay customDay2Schedule = scheduleRuleset.customDay2Schedule();
  customDay2Schedule.setName("Custom Day 2 Schedule");

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
  EXPECT_EQ(customDay1Schedule.nameString(), scheduleWeekDaily.getString(Schedule_Week_DailyFields::CustomDay1Schedule_DayName).get());
  EXPECT_EQ(customDay2Schedule.nameString(), scheduleWeekDaily.getString(Schedule_Week_DailyFields::CustomDay2Schedule_DayName).get());
}

TEST_F(EnergyPlusFixture, ReverseTranslator_ScheduleYearWeekDailyToRulesetSimple) {
  openstudio::Workspace workspace(openstudio::StrictnessLevel::Minimal, openstudio::IddFileType::EnergyPlus);

  openstudio::IdfObject idfObject0(openstudio::IddObjectType::ScheduleTypeLimits);
  idfObject0.setString(0, "Fractional");
  idfObject0.setString(1, "0");
  idfObject0.setString(2, "1");
  idfObject0.setString(3, "Continuous");

  WorkspaceObject epScheduleTypeLimits = workspace.addObject(idfObject0).get();

  openstudio::IdfObject idfObject1(openstudio::IddObjectType::Schedule_Day_Interval);
  idfObject1.setString(0, "occupants schedule allday1");
  idfObject1.setString(1, "Fractional");
  idfObject1.setString(2, "No");
  idfObject1.setString(3, "07:00");
  idfObject1.setString(4, "1");
  idfObject1.setString(5, "08:00");
  idfObject1.setString(6, "0.868852459016393");
  idfObject1.setString(7, "09:00");
  idfObject1.setString(8, "0.409836065573771");
  idfObject1.setString(9, "16:00");
  idfObject1.setString(10, "0.245901639344262");
  idfObject1.setString(11, "17:00");
  idfObject1.setString(12, "0.295081967213115");
  idfObject1.setString(13, "18:00");
  idfObject1.setString(14, "0.540983606557377");
  idfObject1.setString(15, "21:00");
  idfObject1.setString(16, "0.885245901639344");
  idfObject1.setString(17, "24:00");
  idfObject1.setString(18, "1");

  WorkspaceObject epScheduleDayInterval1 = workspace.addObject(idfObject1).get();

  openstudio::IdfObject idfObject4(openstudio::IddObjectType::Schedule_Day_Interval);
  idfObject4.setString(0, "Schedule Day 5");
  idfObject4.setString(1, "Fractional");
  idfObject4.setString(2, "No");
  idfObject4.setString(3, "24:00");
  idfObject4.setString(4, "0");

  WorkspaceObject epScheduleDayInterval2 = workspace.addObject(idfObject4).get();

  openstudio::IdfObject idfObject2(openstudio::IddObjectType::Schedule_Week_Daily);
  idfObject2.setString(0, "occupants schedule Week Rule - Jan1-Dec31");
  idfObject2.setString(1, "occupants schedule allday1");
  idfObject2.setString(2, "occupants schedule allday1");
  idfObject2.setString(3, "occupants schedule allday1");
  idfObject2.setString(4, "occupants schedule allday1");
  idfObject2.setString(5, "occupants schedule allday1");
  idfObject2.setString(6, "occupants schedule allday1");
  idfObject2.setString(7, "occupants schedule allday1");
  idfObject2.setString(8, "Schedule Day 5");
  idfObject2.setString(9, "Schedule Day 5");
  idfObject2.setString(10, "Schedule Day 5");
  idfObject2.setString(11, "Schedule Day 5");
  idfObject2.setString(12, "Schedule Day 5");

  WorkspaceObject epScheduleWeekDaily = workspace.addObject(idfObject2).get();

  openstudio::IdfObject idfObject3(openstudio::IddObjectType::Schedule_Year);
  idfObject3.setString(0, "occupants schedule");
  idfObject3.setString(1, "Fractional");
  idfObject3.setString(2, "occupants schedule Week Rule - Jan1-Dec31");
  idfObject3.setString(3, "1");
  idfObject3.setString(4, "1");
  idfObject3.setString(5, "12");
  idfObject3.setString(6, "31");

  WorkspaceObject epScheduleYear = workspace.addObject(idfObject3).get();

  ReverseTranslator trans;
  ASSERT_NO_THROW(trans.translateWorkspace(workspace));
  Model model = trans.translateWorkspace(workspace);

  std::vector<ScheduleRuleset> scheduleRulesets = model.getConcreteModelObjects<ScheduleRuleset>();
  ASSERT_EQ(1u, scheduleRulesets.size());
  ScheduleRuleset scheduleRuleset = scheduleRulesets[0];
  EXPECT_EQ("occupants schedule", scheduleRuleset.nameString());
  EXPECT_TRUE(scheduleRuleset.scheduleTypeLimits());
  EXPECT_EQ(1, scheduleRuleset.scheduleRules().size());
  EXPECT_EQ("Schedule Day 1", scheduleRuleset.defaultDaySchedule().nameString());  // FIXME: we want this to somehow be Schedule Day 5?
  EXPECT_EQ("Schedule Day 6", scheduleRuleset.holidaySchedule().nameString());
  EXPECT_EQ("Schedule Day 7", scheduleRuleset.summerDesignDaySchedule().nameString());
  EXPECT_EQ("Schedule Day 8", scheduleRuleset.winterDesignDaySchedule().nameString());
  EXPECT_EQ("Schedule Day 9", scheduleRuleset.customDay1Schedule().nameString());
  EXPECT_EQ("Schedule Day 10", scheduleRuleset.customDay2Schedule().nameString());

  std::vector<ScheduleRule> scheduleRules = model.getConcreteModelObjects<ScheduleRule>();
  ASSERT_EQ(1u, scheduleRules.size());
  ScheduleRule scheduleRule = scheduleRules[0];
  EXPECT_EQ(scheduleRule.scheduleRuleset().handle(), scheduleRuleset.handle());
  EXPECT_EQ(0, scheduleRule.ruleIndex());
  ScheduleDay daySchedule = scheduleRule.daySchedule();
  EXPECT_EQ(daySchedule.nameString(), "occupants schedule allday1 1");
  EXPECT_EQ("No", daySchedule.interpolatetoTimestep());
  EXPECT_EQ(8u, daySchedule.values().size());
  EXPECT_TRUE(scheduleRule.applySunday());
  EXPECT_TRUE(scheduleRule.applyMonday());
  EXPECT_TRUE(scheduleRule.applyTuesday());
  EXPECT_TRUE(scheduleRule.applyWednesday());
  EXPECT_TRUE(scheduleRule.applyThursday());
  EXPECT_TRUE(scheduleRule.applyFriday());
  EXPECT_TRUE(scheduleRule.applySaturday());
  EXPECT_EQ("DateRange", scheduleRule.dateSpecificationType());
  ASSERT_TRUE(scheduleRule.startDate());
  EXPECT_EQ(1, scheduleRule.startDate().get().monthOfYear().value());
  EXPECT_EQ(1, scheduleRule.startDate().get().dayOfMonth());
  ASSERT_TRUE(scheduleRule.endDate());
  EXPECT_EQ(12, scheduleRule.endDate().get().monthOfYear().value());
  EXPECT_EQ(31, scheduleRule.endDate().get().dayOfMonth());

  std::vector<ScheduleDay> scheduleDays = model.getConcreteModelObjects<ScheduleDay>();
  // 1: "occupants schedule allday1"
  // 1: "Schedule Day 5" (orphaned?)
  // 1: "Schedule Day 1" (created by ScheduleRuleset ctor)
  // 1: clone of "occupants schedule allday1" created by ScheduleRule ctor (orphaned?)
  // 5: clones of "Schedule Day 5" (created by set methods for holiday, design day, custom day schedule setters)
  ASSERT_EQ(9u, scheduleDays.size());

  std::vector<ScheduleYear> scheduleYears = model.getConcreteModelObjects<ScheduleYear>();
  ASSERT_EQ(0u, scheduleYears.size());

  std::vector<ScheduleWeek> scheduleWeeks = model.getConcreteModelObjects<ScheduleWeek>();
  ASSERT_EQ(1u, scheduleWeeks.size());  // RT of Schedule:Week:Daily called from ReverseTranslator.cpp directly
}

TEST_F(EnergyPlusFixture, ReverseTranslator_ScheduleYearWeekDailyToRulesetComplex) {
  openstudio::Workspace workspace(openstudio::StrictnessLevel::Minimal, openstudio::IddFileType::EnergyPlus);

  openstudio::IdfObject idfObject0(openstudio::IddObjectType::ScheduleTypeLimits);
  idfObject0.setString(0, "Fractional");
  idfObject0.setString(1, "0");
  idfObject0.setString(2, "1");
  idfObject0.setString(3, "Continuous");

  WorkspaceObject epScheduleTypeLimits = workspace.addObject(idfObject0).get();

  openstudio::IdfObject idfObject1(openstudio::IddObjectType::Schedule_Day_Interval);
  idfObject1.setString(0, "occupants schedule allday1");
  idfObject1.setString(1, "Fractional");
  idfObject1.setString(2, "No");
  idfObject1.setString(3, "07:00");
  idfObject1.setString(4, "1");
  idfObject1.setString(5, "08:00");
  idfObject1.setString(6, "0.868852459016393");
  idfObject1.setString(7, "09:00");
  idfObject1.setString(8, "0.409836065573771");
  idfObject1.setString(9, "16:00");
  idfObject1.setString(10, "0.245901639344262");
  idfObject1.setString(11, "17:00");
  idfObject1.setString(12, "0.295081967213115");
  idfObject1.setString(13, "18:00");
  idfObject1.setString(14, "0.540983606557377");
  idfObject1.setString(15, "21:00");
  idfObject1.setString(16, "0.885245901639344");
  idfObject1.setString(17, "24:00");
  idfObject1.setString(18, "1");

  WorkspaceObject epScheduleDayInterval1 = workspace.addObject(idfObject1).get();

  openstudio::IdfObject idfObject2(openstudio::IddObjectType::Schedule_Day_Interval);
  idfObject2.setString(0, "occupants schedule allday2");
  idfObject2.setString(1, "Fractional");
  idfObject2.setString(2, "No");
  idfObject2.setString(3, "07:00");
  idfObject2.setString(4, "1");
  idfObject2.setString(5, "08:00");
  idfObject2.setString(6, "0.868852459016393");
  idfObject2.setString(7, "09:00");
  idfObject2.setString(8, "0.409836065573771");
  idfObject2.setString(9, "16:00");
  idfObject2.setString(10, "0.245901639344262");
  idfObject2.setString(11, "17:00");
  idfObject2.setString(12, "0.295081967213115");
  idfObject2.setString(13, "18:00");
  idfObject2.setString(14, "0.540983606557377");
  idfObject2.setString(15, "21:00");
  idfObject2.setString(16, "0.885245901639344");
  idfObject2.setString(17, "24:00");
  idfObject2.setString(18, "1");

  WorkspaceObject epScheduleDayInterval2 = workspace.addObject(idfObject2).get();

  openstudio::IdfObject idfObject3(openstudio::IddObjectType::Schedule_Week_Daily);
  idfObject3.setString(0, "occupants schedule Week Rule - Jan1-Jan27");
  idfObject3.setString(1, "occupants schedule allday1");
  idfObject3.setString(2, "occupants schedule allday1");
  idfObject3.setString(3, "occupants schedule allday1");
  idfObject3.setString(4, "occupants schedule allday1");
  idfObject3.setString(5, "occupants schedule allday1");
  idfObject3.setString(6, "occupants schedule allday1");
  idfObject3.setString(7, "occupants schedule allday1");
  idfObject3.setString(8, "occupants schedule allday2");
  idfObject3.setString(9, "occupants schedule allday1");
  idfObject3.setString(10, "occupants schedule allday2");
  idfObject3.setString(11, "occupants schedule allday1");
  idfObject3.setString(12, "occupants schedule allday2");

  WorkspaceObject epScheduleWeekDaily1 = workspace.addObject(idfObject3).get();

  openstudio::IdfObject idfObject4(openstudio::IddObjectType::Schedule_Week_Daily);
  idfObject4.setString(0, "occupants schedule Week Rule - Jan28-Feb3");
  idfObject4.setString(1, "occupants schedule allday1");
  idfObject4.setString(2, "occupants schedule allday1");
  idfObject4.setString(3, "occupants schedule allday1");
  idfObject4.setString(4, "occupants schedule allday1");
  idfObject4.setString(5, "occupants schedule allday2");
  idfObject4.setString(6, "occupants schedule allday2");
  idfObject4.setString(7, "occupants schedule allday2");
  idfObject4.setString(8, "occupants schedule allday2");
  idfObject4.setString(9, "occupants schedule allday1");
  idfObject4.setString(10, "occupants schedule allday2");
  idfObject4.setString(11, "occupants schedule allday1");
  idfObject4.setString(12, "occupants schedule allday2");

  WorkspaceObject epScheduleWeekDaily2 = workspace.addObject(idfObject4).get();

  openstudio::IdfObject idfObject5(openstudio::IddObjectType::Schedule_Year);
  idfObject5.setString(0, "occupants schedule");
  idfObject5.setString(1, "Fractional");
  idfObject5.setString(2, "occupants schedule Week Rule - Jan1-Jan27");
  idfObject5.setString(3, "1");
  idfObject5.setString(4, "1");
  idfObject5.setString(5, "1");
  idfObject5.setString(6, "27");
  idfObject5.setString(7, "occupants schedule Week Rule - Jan28-Feb3");
  idfObject5.setString(8, "1");
  idfObject5.setString(9, "28");
  idfObject5.setString(10, "2");
  idfObject5.setString(11, "3");

  WorkspaceObject epScheduleYear = workspace.addObject(idfObject5).get();

  ReverseTranslator trans;
  ASSERT_NO_THROW(trans.translateWorkspace(workspace));
  Model model = trans.translateWorkspace(workspace);

  std::vector<ScheduleRuleset> scheduleRulesets = model.getConcreteModelObjects<ScheduleRuleset>();
  ASSERT_EQ(1u, scheduleRulesets.size());
  ScheduleRuleset scheduleRuleset = scheduleRulesets[0];
  EXPECT_EQ("occupants schedule", scheduleRuleset.nameString());
  EXPECT_TRUE(scheduleRuleset.scheduleTypeLimits());
  EXPECT_EQ(3, scheduleRuleset.scheduleRules().size());
  EXPECT_EQ("Schedule Day 1", scheduleRuleset.defaultDaySchedule().nameString());
  EXPECT_EQ("occupants schedule allday2 2", scheduleRuleset.holidaySchedule().nameString());
  EXPECT_EQ("occupants schedule allday1 3", scheduleRuleset.summerDesignDaySchedule().nameString());
  EXPECT_EQ("occupants schedule allday2 3", scheduleRuleset.winterDesignDaySchedule().nameString());
  EXPECT_EQ("occupants schedule allday1 4", scheduleRuleset.customDay1Schedule().nameString());
  EXPECT_EQ("occupants schedule allday2 4", scheduleRuleset.customDay2Schedule().nameString());

  std::vector<ScheduleRule> scheduleRules = model.getConcreteModelObjects<ScheduleRule>();
  ASSERT_EQ(3u, scheduleRules.size());

  boost::optional<ScheduleRule> optscheduleRule1 = model.getConcreteModelObjectByName<ScheduleRule>("occupants schedule Week Rule - Jan1-Jan27");
  ASSERT_TRUE(optscheduleRule1);
  ScheduleRule scheduleRule1 = optscheduleRule1.get();
  EXPECT_EQ(scheduleRule1.scheduleRuleset().handle(), scheduleRuleset.handle());
  EXPECT_EQ(2, scheduleRule1.ruleIndex());
  ScheduleDay daySchedule1 = scheduleRule1.daySchedule();
  EXPECT_EQ(daySchedule1.nameString(), "occupants schedule allday1 1");
  EXPECT_EQ("No", daySchedule1.interpolatetoTimestep());
  EXPECT_EQ(8u, daySchedule1.values().size());
  EXPECT_TRUE(scheduleRule1.applySunday());
  EXPECT_TRUE(scheduleRule1.applyMonday());
  EXPECT_TRUE(scheduleRule1.applyTuesday());
  EXPECT_TRUE(scheduleRule1.applyWednesday());
  EXPECT_TRUE(scheduleRule1.applyThursday());
  EXPECT_TRUE(scheduleRule1.applyFriday());
  EXPECT_TRUE(scheduleRule1.applySaturday());
  EXPECT_EQ("DateRange", scheduleRule1.dateSpecificationType());
  ASSERT_TRUE(scheduleRule1.startDate());
  EXPECT_EQ(1, scheduleRule1.startDate().get().monthOfYear().value());
  EXPECT_EQ(1, scheduleRule1.startDate().get().dayOfMonth());
  ASSERT_TRUE(scheduleRule1.endDate());
  EXPECT_EQ(1, scheduleRule1.endDate().get().monthOfYear().value());
  EXPECT_EQ(27, scheduleRule1.endDate().get().dayOfMonth());

  boost::optional<ScheduleRule> optscheduleRule2 = model.getConcreteModelObjectByName<ScheduleRule>("occupants schedule Week Rule - Jan28-Feb3");
  ASSERT_TRUE(optscheduleRule2);
  ScheduleRule scheduleRule2 = optscheduleRule2.get();
  EXPECT_EQ(scheduleRule2.scheduleRuleset().handle(), scheduleRuleset.handle());
  EXPECT_EQ(1, scheduleRule2.ruleIndex());
  ScheduleDay daySchedule2 = scheduleRule2.daySchedule();
  EXPECT_EQ(daySchedule2.nameString(), "occupants schedule allday1 2");
  EXPECT_EQ("No", daySchedule2.interpolatetoTimestep());
  EXPECT_EQ(8u, daySchedule2.values().size());
  EXPECT_TRUE(scheduleRule2.applySunday());
  EXPECT_TRUE(scheduleRule2.applyMonday());
  EXPECT_TRUE(scheduleRule2.applyTuesday());
  EXPECT_TRUE(scheduleRule2.applyWednesday());
  EXPECT_FALSE(scheduleRule2.applyThursday());
  EXPECT_FALSE(scheduleRule2.applyFriday());
  EXPECT_FALSE(scheduleRule2.applySaturday());
  EXPECT_EQ("DateRange", scheduleRule2.dateSpecificationType());
  ASSERT_TRUE(scheduleRule2.startDate());
  EXPECT_EQ(1, scheduleRule2.startDate().get().monthOfYear().value());
  EXPECT_EQ(28, scheduleRule2.startDate().get().dayOfMonth());
  ASSERT_TRUE(scheduleRule2.endDate());
  EXPECT_EQ(2, scheduleRule2.endDate().get().monthOfYear().value());
  EXPECT_EQ(3, scheduleRule2.endDate().get().dayOfMonth());

  boost::optional<ScheduleRule> optscheduleRule3 = model.getConcreteModelObjectByName<ScheduleRule>("occupants schedule Week Rule - Jan28-Feb3 1");
  ASSERT_TRUE(optscheduleRule3);
  ScheduleRule scheduleRule3 = optscheduleRule3.get();
  EXPECT_EQ(scheduleRule3.scheduleRuleset().handle(), scheduleRuleset.handle());
  EXPECT_EQ(0, scheduleRule3.ruleIndex());
  ScheduleDay daySchedule3 = scheduleRule3.daySchedule();
  EXPECT_EQ(daySchedule3.nameString(), "occupants schedule allday2 1");
  EXPECT_EQ("No", daySchedule3.interpolatetoTimestep());
  EXPECT_EQ(8u, daySchedule3.values().size());
  EXPECT_FALSE(scheduleRule3.applySunday());
  EXPECT_FALSE(scheduleRule3.applyMonday());
  EXPECT_FALSE(scheduleRule3.applyTuesday());
  EXPECT_FALSE(scheduleRule3.applyWednesday());
  EXPECT_TRUE(scheduleRule3.applyThursday());
  EXPECT_TRUE(scheduleRule3.applyFriday());
  EXPECT_TRUE(scheduleRule3.applySaturday());
  EXPECT_EQ("DateRange", scheduleRule3.dateSpecificationType());
  ASSERT_TRUE(scheduleRule3.startDate());
  EXPECT_EQ(1, scheduleRule3.startDate().get().monthOfYear().value());
  EXPECT_EQ(28, scheduleRule3.startDate().get().dayOfMonth());
  ASSERT_TRUE(scheduleRule3.endDate());
  EXPECT_EQ(2, scheduleRule3.endDate().get().monthOfYear().value());
  EXPECT_EQ(3, scheduleRule3.endDate().get().dayOfMonth());

  std::vector<ScheduleDay> scheduleDays = model.getConcreteModelObjects<ScheduleDay>();
  // 1: "occupants schedule allday1"
  // 1: "occupants schedule allday2"
  // 1: "Schedule Day 1" (created by ScheduleRuleset ctor)
  // 2: clones of "occupants schedule allday1" created by ScheduleRule ctor (orphaned?)
  // 1: clone of "occupants schedule allday2" created by ScheduleRule ctor (orphaned?)
  // 5: clones of "Schedule Day 5" (created by set methods for holiday, design day, custom day schedule setters)
  ASSERT_EQ(11u, scheduleDays.size());  // 3 from the idf, 1 from ScheduleRuleset ctor, and 5 cloned

  std::vector<ScheduleYear> scheduleYears = model.getConcreteModelObjects<ScheduleYear>();
  ASSERT_EQ(0u, scheduleYears.size());

  std::vector<ScheduleWeek> scheduleWeeks = model.getConcreteModelObjects<ScheduleWeek>();
  ASSERT_EQ(2u, scheduleWeeks.size());  // RT of Schedule:Week:Daily called from ReverseTranslator.cpp directly
}

TEST_F(EnergyPlusFixture, ReverseTranslator_ScheduleYearWeekDailyToYearComplex) {
  openstudio::Workspace workspace(openstudio::StrictnessLevel::Minimal, openstudio::IddFileType::EnergyPlus);

  openstudio::IdfObject idfObject0(openstudio::IddObjectType::ScheduleTypeLimits);
  idfObject0.setString(0, "Fractional");
  idfObject0.setString(1, "0");
  idfObject0.setString(2, "1");
  idfObject0.setString(3, "Continuous");

  WorkspaceObject epScheduleTypeLimits = workspace.addObject(idfObject0).get();

  openstudio::IdfObject idfObject1(openstudio::IddObjectType::Schedule_Day_Interval);
  idfObject1.setString(0, "occupants schedule allday1");
  idfObject1.setString(1, "Fractional");
  idfObject1.setString(2, "No");
  idfObject1.setString(3, "07:00");
  idfObject1.setString(4, "1");
  idfObject1.setString(5, "08:00");
  idfObject1.setString(6, "0.868852459016393");
  idfObject1.setString(7, "09:00");
  idfObject1.setString(8, "0.409836065573771");
  idfObject1.setString(9, "16:00");
  idfObject1.setString(10, "0.245901639344262");
  idfObject1.setString(11, "17:00");
  idfObject1.setString(12, "0.295081967213115");
  idfObject1.setString(13, "18:00");
  idfObject1.setString(14, "0.540983606557377");
  idfObject1.setString(15, "21:00");
  idfObject1.setString(16, "0.885245901639344");
  idfObject1.setString(17, "24:00");
  idfObject1.setString(18, "1");

  WorkspaceObject epScheduleDayInterval1 = workspace.addObject(idfObject1).get();

  openstudio::IdfObject idfObject2(openstudio::IddObjectType::Schedule_Day_Interval);
  idfObject2.setString(0, "occupants schedule allday2");
  idfObject2.setString(1, "Fractional");
  idfObject2.setString(2, "No");
  idfObject2.setString(3, "07:00");
  idfObject2.setString(4, "1");
  idfObject2.setString(5, "08:00");
  idfObject2.setString(6, "0.868852459016393");
  idfObject2.setString(7, "09:00");
  idfObject2.setString(8, "0.409836065573771");
  idfObject2.setString(9, "16:00");
  idfObject2.setString(10, "0.245901639344262");
  idfObject2.setString(11, "17:00");
  idfObject2.setString(12, "0.295081967213115");
  idfObject2.setString(13, "18:00");
  idfObject2.setString(14, "0.540983606557377");
  idfObject2.setString(15, "21:00");
  idfObject2.setString(16, "0.885245901639344");
  idfObject2.setString(17, "24:00");
  idfObject2.setString(18, "1");

  WorkspaceObject epScheduleDayInterval2 = workspace.addObject(idfObject2).get();

  openstudio::IdfObject idfObject3(openstudio::IddObjectType::Schedule_Week_Daily);
  idfObject3.setString(0, "occupants schedule Week Rule - Jan1-Jan27");
  idfObject3.setString(1, "occupants schedule allday1");
  idfObject3.setString(2, "occupants schedule allday1");
  idfObject3.setString(3, "occupants schedule allday1");
  idfObject3.setString(4, "occupants schedule allday1");
  idfObject3.setString(5, "occupants schedule allday1");
  idfObject3.setString(6, "occupants schedule allday1");
  idfObject3.setString(7, "occupants schedule allday1");
  idfObject3.setString(8, "occupants schedule allday2");
  idfObject3.setString(9, "occupants schedule allday1");
  idfObject3.setString(10, "occupants schedule allday2");
  idfObject3.setString(11, "occupants schedule allday1");
  idfObject3.setString(12, "occupants schedule allday2");

  WorkspaceObject epScheduleWeekDaily1 = workspace.addObject(idfObject3).get();

  openstudio::IdfObject idfObject4(openstudio::IddObjectType::Schedule_Week_Daily);
  idfObject4.setString(0, "occupants schedule Week Rule - Jan28-Dec31");
  idfObject4.setString(1, "occupants schedule allday1");
  idfObject4.setString(2, "occupants schedule allday1");
  idfObject4.setString(3, "occupants schedule allday1");
  idfObject4.setString(4, "occupants schedule allday1");
  idfObject4.setString(5, "occupants schedule allday2");
  idfObject4.setString(6, "occupants schedule allday2");
  idfObject4.setString(7, "occupants schedule allday2");
  idfObject4.setString(8, "occupants schedule allday1");  // this differs from the first week
  idfObject4.setString(9, "occupants schedule allday1");
  idfObject4.setString(10, "occupants schedule allday2");
  idfObject4.setString(11, "occupants schedule allday1");
  idfObject4.setString(12, "occupants schedule allday2");

  WorkspaceObject epScheduleWeekDaily2 = workspace.addObject(idfObject4).get();

  openstudio::IdfObject idfObject5(openstudio::IddObjectType::Schedule_Year);
  idfObject5.setString(0, "occupants schedule");
  idfObject5.setString(1, "Fractional");
  idfObject5.setString(2, "occupants schedule Week Rule - Jan1-Jan27");
  idfObject5.setString(3, "1");
  idfObject5.setString(4, "1");
  idfObject5.setString(5, "1");
  idfObject5.setString(6, "27");
  idfObject5.setString(7, "occupants schedule Week Rule - Jan28-Dec31");
  idfObject5.setString(8, "1");
  idfObject5.setString(9, "28");
  idfObject5.setString(10, "12");
  idfObject5.setString(11, "31");

  WorkspaceObject epScheduleYear = workspace.addObject(idfObject5).get();

  ReverseTranslator trans;
  ASSERT_NO_THROW(trans.translateWorkspace(workspace));
  Model model = trans.translateWorkspace(workspace);

  std::vector<ScheduleYear> scheduleYears = model.getConcreteModelObjects<ScheduleYear>();
  ASSERT_EQ(1u, scheduleYears.size());
  ScheduleYear scheduleYear = scheduleYears[0];
  EXPECT_EQ(2u, scheduleYear.scheduleWeeks().size());
  EXPECT_EQ(2u, scheduleYear.dates().size());

  std::vector<ScheduleDay> scheduleDays = model.getConcreteModelObjects<ScheduleDay>();
  // 1: "occupants schedule allday1"
  // 1: "occupants schedule allday2"
  ASSERT_EQ(2u, scheduleDays.size());

  std::vector<ScheduleRuleset> scheduleRulesets = model.getConcreteModelObjects<ScheduleRuleset>();
  ASSERT_EQ(0u, scheduleRulesets.size());

  std::vector<ScheduleWeek> scheduleWeeks = model.getConcreteModelObjects<ScheduleWeek>();
  ASSERT_EQ(2u, scheduleWeeks.size());  // RT of Schedule:Week:Daily called from ReverseTranslator.cpp directly
}

TEST_F(EnergyPlusFixture, ReverseTranslator_ScheduleYearWeekCompactToRulesetComplex) {
  openstudio::Workspace workspace(openstudio::StrictnessLevel::Minimal, openstudio::IddFileType::EnergyPlus);

  openstudio::IdfObject idfObject0(openstudio::IddObjectType::ScheduleTypeLimits);
  idfObject0.setString(0, "Fractional");
  idfObject0.setString(1, "0");
  idfObject0.setString(2, "1");
  idfObject0.setString(3, "Continuous");

  WorkspaceObject epScheduleTypeLimits = workspace.addObject(idfObject0).get();

  openstudio::IdfObject idfObject1(openstudio::IddObjectType::Schedule_Day_Interval);
  idfObject1.setString(0, "occupants schedule allday1");
  idfObject1.setString(1, "Fractional");
  idfObject1.setString(2, "No");
  idfObject1.setString(3, "07:00");
  idfObject1.setString(4, "1");
  idfObject1.setString(5, "08:00");
  idfObject1.setString(6, "0.868852459016393");
  idfObject1.setString(7, "09:00");
  idfObject1.setString(8, "0.409836065573771");
  idfObject1.setString(9, "16:00");
  idfObject1.setString(10, "0.245901639344262");
  idfObject1.setString(11, "17:00");
  idfObject1.setString(12, "0.295081967213115");
  idfObject1.setString(13, "18:00");
  idfObject1.setString(14, "0.540983606557377");
  idfObject1.setString(15, "21:00");
  idfObject1.setString(16, "0.885245901639344");
  idfObject1.setString(17, "24:00");
  idfObject1.setString(18, "1");

  WorkspaceObject epScheduleDayInterval1 = workspace.addObject(idfObject1).get();

  openstudio::IdfObject idfObject2(openstudio::IddObjectType::Schedule_Day_Interval);
  idfObject2.setString(0, "occupants schedule allday2");
  idfObject2.setString(1, "Fractional");
  idfObject2.setString(2, "No");
  idfObject2.setString(3, "07:00");
  idfObject2.setString(4, "1");
  idfObject2.setString(5, "08:00");
  idfObject2.setString(6, "0.868852459016393");
  idfObject2.setString(7, "09:00");
  idfObject2.setString(8, "0.409836065573771");
  idfObject2.setString(9, "16:00");
  idfObject2.setString(10, "0.245901639344262");
  idfObject2.setString(11, "17:00");
  idfObject2.setString(12, "0.295081967213115");
  idfObject2.setString(13, "18:00");
  idfObject2.setString(14, "0.540983606557377");
  idfObject2.setString(15, "21:00");
  idfObject2.setString(16, "0.885245901639344");
  idfObject2.setString(17, "24:00");
  idfObject2.setString(18, "1");

  WorkspaceObject epScheduleDayInterval2 = workspace.addObject(idfObject2).get();

  openstudio::IdfObject idfObject3(openstudio::IddObjectType::Schedule_Week_Compact);
  idfObject3.setString(0, "occupants schedule Week Rule - Jan1-Jan27");
  idfObject3.setString(1, "AllDays");
  idfObject3.setString(2, "occupants schedule allday1");
  idfObject3.setString(3, "Holiday");
  idfObject3.setString(4, "occupants schedule allday2");
  idfObject3.setString(5, "SummerDesignDay");
  idfObject3.setString(6, "occupants schedule allday1");
  idfObject3.setString(7, "WinterDesignDay");
  idfObject3.setString(8, "occupants schedule allday2");
  idfObject3.setString(9, "CustomDay1");
  idfObject3.setString(10, "occupants schedule allday1");
  idfObject3.setString(11, "CustomDay2");
  idfObject3.setString(12, "occupants schedule allday2");

  WorkspaceObject epScheduleWeekDaily1 = workspace.addObject(idfObject3).get();

  openstudio::IdfObject idfObject4(openstudio::IddObjectType::Schedule_Week_Compact);
  idfObject4.setString(0, "occupants schedule Week Rule - Jan28-Feb3");
  idfObject4.setString(1, "Sunday");
  idfObject4.setString(2, "occupants schedule allday1");
  idfObject4.setString(3, "Monday");
  idfObject4.setString(4, "occupants schedule allday1");
  idfObject4.setString(5, "Tuesday");
  idfObject4.setString(6, "occupants schedule allday1");
  idfObject4.setString(7, "Wednesday");
  idfObject4.setString(8, "occupants schedule allday1");
  idfObject4.setString(9, "Thursday");
  idfObject4.setString(10, "occupants schedule allday2");
  idfObject4.setString(11, "Friday");
  idfObject4.setString(12, "occupants schedule allday2");
  idfObject4.setString(13, "Saturday");
  idfObject4.setString(14, "occupants schedule allday2");
  idfObject4.setString(15, "Holiday");
  idfObject4.setString(16, "occupants schedule allday2");
  idfObject4.setString(17, "SummerDesignDay");
  idfObject4.setString(18, "occupants schedule allday1");
  idfObject4.setString(19, "WinterDesignDay");
  idfObject4.setString(20, "occupants schedule allday2");
  idfObject4.setString(21, "CustomDay1");
  idfObject4.setString(22, "occupants schedule allday1");
  idfObject4.setString(23, "CustomDay2");
  idfObject4.setString(24, "occupants schedule allday2");

  WorkspaceObject epScheduleWeekDaily2 = workspace.addObject(idfObject4).get();

  openstudio::IdfObject idfObject5(openstudio::IddObjectType::Schedule_Year);
  idfObject5.setString(0, "occupants schedule");
  idfObject5.setString(1, "Fractional");
  idfObject5.setString(2, "occupants schedule Week Rule - Jan1-Jan27");
  idfObject5.setString(3, "1");
  idfObject5.setString(4, "1");
  idfObject5.setString(5, "1");
  idfObject5.setString(6, "27");
  idfObject5.setString(7, "occupants schedule Week Rule - Jan28-Feb3");
  idfObject5.setString(8, "1");
  idfObject5.setString(9, "28");
  idfObject5.setString(10, "2");
  idfObject5.setString(11, "3");

  WorkspaceObject epScheduleYear = workspace.addObject(idfObject5).get();

  ReverseTranslator trans;
  ASSERT_NO_THROW(trans.translateWorkspace(workspace));
  Model model = trans.translateWorkspace(workspace);

  std::vector<ScheduleRuleset> scheduleRulesets = model.getConcreteModelObjects<ScheduleRuleset>();
  ASSERT_EQ(1u, scheduleRulesets.size());
  ScheduleRuleset scheduleRuleset = scheduleRulesets[0];
  EXPECT_EQ("occupants schedule", scheduleRuleset.nameString());
  EXPECT_TRUE(scheduleRuleset.scheduleTypeLimits());
  EXPECT_EQ(3, scheduleRuleset.scheduleRules().size());
  EXPECT_EQ("Schedule Day 1", scheduleRuleset.defaultDaySchedule().nameString());
  EXPECT_EQ("occupants schedule allday2 2", scheduleRuleset.holidaySchedule().nameString());
  EXPECT_EQ("occupants schedule allday1 3", scheduleRuleset.summerDesignDaySchedule().nameString());
  EXPECT_EQ("occupants schedule allday2 3", scheduleRuleset.winterDesignDaySchedule().nameString());
  EXPECT_EQ("occupants schedule allday1 4", scheduleRuleset.customDay1Schedule().nameString());
  EXPECT_EQ("occupants schedule allday2 4", scheduleRuleset.customDay2Schedule().nameString());

  std::vector<ScheduleRule> scheduleRules = model.getConcreteModelObjects<ScheduleRule>();
  ASSERT_EQ(3u, scheduleRules.size());

  boost::optional<ScheduleRule> optscheduleRule1 = model.getConcreteModelObjectByName<ScheduleRule>("occupants schedule Week Rule - Jan1-Jan27");
  ASSERT_TRUE(optscheduleRule1);
  ScheduleRule scheduleRule1 = optscheduleRule1.get();
  EXPECT_EQ(scheduleRule1.scheduleRuleset().handle(), scheduleRuleset.handle());
  EXPECT_EQ(2, scheduleRule1.ruleIndex());
  ScheduleDay daySchedule1 = scheduleRule1.daySchedule();
  EXPECT_EQ(daySchedule1.nameString(), "occupants schedule allday1 1");
  EXPECT_EQ("No", daySchedule1.interpolatetoTimestep());
  EXPECT_EQ(8u, daySchedule1.values().size());
  EXPECT_TRUE(scheduleRule1.applySunday());
  EXPECT_TRUE(scheduleRule1.applyMonday());
  EXPECT_TRUE(scheduleRule1.applyTuesday());
  EXPECT_TRUE(scheduleRule1.applyWednesday());
  EXPECT_TRUE(scheduleRule1.applyThursday());
  EXPECT_TRUE(scheduleRule1.applyFriday());
  EXPECT_TRUE(scheduleRule1.applySaturday());
  EXPECT_EQ("DateRange", scheduleRule1.dateSpecificationType());
  ASSERT_TRUE(scheduleRule1.startDate());
  EXPECT_EQ(1, scheduleRule1.startDate().get().monthOfYear().value());
  EXPECT_EQ(1, scheduleRule1.startDate().get().dayOfMonth());
  ASSERT_TRUE(scheduleRule1.endDate());
  EXPECT_EQ(1, scheduleRule1.endDate().get().monthOfYear().value());
  EXPECT_EQ(27, scheduleRule1.endDate().get().dayOfMonth());

  boost::optional<ScheduleRule> optscheduleRule2 = model.getConcreteModelObjectByName<ScheduleRule>("occupants schedule Week Rule - Jan28-Feb3");
  ASSERT_TRUE(optscheduleRule2);
  ScheduleRule scheduleRule2 = optscheduleRule2.get();
  EXPECT_EQ(scheduleRule2.scheduleRuleset().handle(), scheduleRuleset.handle());
  EXPECT_EQ(1, scheduleRule2.ruleIndex());
  ScheduleDay daySchedule2 = scheduleRule2.daySchedule();
  EXPECT_EQ(daySchedule2.nameString(), "occupants schedule allday1 2");
  EXPECT_EQ("No", daySchedule2.interpolatetoTimestep());
  EXPECT_EQ(8u, daySchedule2.values().size());
  EXPECT_TRUE(scheduleRule2.applySunday());
  EXPECT_TRUE(scheduleRule2.applyMonday());
  EXPECT_TRUE(scheduleRule2.applyTuesday());
  EXPECT_TRUE(scheduleRule2.applyWednesday());
  EXPECT_FALSE(scheduleRule2.applyThursday());
  EXPECT_FALSE(scheduleRule2.applyFriday());
  EXPECT_FALSE(scheduleRule2.applySaturday());
  EXPECT_EQ("DateRange", scheduleRule2.dateSpecificationType());
  ASSERT_TRUE(scheduleRule2.startDate());
  EXPECT_EQ(1, scheduleRule2.startDate().get().monthOfYear().value());
  EXPECT_EQ(28, scheduleRule2.startDate().get().dayOfMonth());
  ASSERT_TRUE(scheduleRule2.endDate());
  EXPECT_EQ(2, scheduleRule2.endDate().get().monthOfYear().value());
  EXPECT_EQ(3, scheduleRule2.endDate().get().dayOfMonth());

  boost::optional<ScheduleRule> optscheduleRule3 = model.getConcreteModelObjectByName<ScheduleRule>("occupants schedule Week Rule - Jan28-Feb3 1");
  ASSERT_TRUE(optscheduleRule3);
  ScheduleRule scheduleRule3 = optscheduleRule3.get();
  EXPECT_EQ(scheduleRule3.scheduleRuleset().handle(), scheduleRuleset.handle());
  EXPECT_EQ(0, scheduleRule3.ruleIndex());
  ScheduleDay daySchedule3 = scheduleRule3.daySchedule();
  EXPECT_EQ(daySchedule3.nameString(), "occupants schedule allday2 1");
  EXPECT_EQ("No", daySchedule3.interpolatetoTimestep());
  EXPECT_EQ(8u, daySchedule3.values().size());
  EXPECT_FALSE(scheduleRule3.applySunday());
  EXPECT_FALSE(scheduleRule3.applyMonday());
  EXPECT_FALSE(scheduleRule3.applyTuesday());
  EXPECT_FALSE(scheduleRule3.applyWednesday());
  EXPECT_TRUE(scheduleRule3.applyThursday());
  EXPECT_TRUE(scheduleRule3.applyFriday());
  EXPECT_TRUE(scheduleRule3.applySaturday());
  EXPECT_EQ("DateRange", scheduleRule3.dateSpecificationType());
  ASSERT_TRUE(scheduleRule3.startDate());
  EXPECT_EQ(1, scheduleRule3.startDate().get().monthOfYear().value());
  EXPECT_EQ(28, scheduleRule3.startDate().get().dayOfMonth());
  ASSERT_TRUE(scheduleRule3.endDate());
  EXPECT_EQ(2, scheduleRule3.endDate().get().monthOfYear().value());
  EXPECT_EQ(3, scheduleRule3.endDate().get().dayOfMonth());

  std::vector<ScheduleDay> scheduleDays = model.getConcreteModelObjects<ScheduleDay>();
  // 1: "occupants schedule allday1"
  // 1: "occupants schedule allday2"
  // 1: "Schedule Day 1" (created by ScheduleRuleset ctor)
  // 2: clones of "occupants schedule allday1" created by ScheduleRule ctor (orphaned?)
  // 1: clone of "occupants schedule allday2" created by ScheduleRule ctor (orphaned?)
  // 5: clones of "Schedule Day 5" (created by set methods for holiday, design day, custom day schedule setters)
  ASSERT_EQ(11u, scheduleDays.size());  // 3 from the idf, 1 from ScheduleRuleset ctor, and 5 cloned

  std::vector<ScheduleYear> scheduleYears = model.getConcreteModelObjects<ScheduleYear>();
  ASSERT_EQ(0u, scheduleYears.size());

  std::vector<ScheduleWeek> scheduleWeeks = model.getConcreteModelObjects<ScheduleWeek>();
  ASSERT_EQ(0u, scheduleWeeks.size());
}

TEST_F(EnergyPlusFixture, ForwardTranslator_ScheduleRuleset_Bug5113) {
  Model model;
  ScheduleRuleset scheduleRuleset(model);

  ScheduleRule rule1(scheduleRuleset);
  rule1.setName("Annual Rule");
  rule1.setApplySunday(true);
  rule1.setApplyMonday(true);
  rule1.setApplyTuesday(true);
  rule1.setApplyWednesday(true);
  rule1.setApplyThursday(true);
  rule1.setApplyFriday(true);
  rule1.setApplySaturday(true);
  rule1.setStartDate(Date(1, 1));
  rule1.setEndDate(Date(12, 31));

  ScheduleRule rule2(scheduleRuleset);
  rule2.setName("Dec Rule");
  rule2.setApplySunday(true);
  rule2.setApplyMonday(true);
  rule2.setApplyTuesday(true);
  rule2.setApplyWednesday(true);
  rule2.setApplyThursday(true);
  rule2.setApplyFriday(true);
  rule2.setApplySaturday(true);
  rule2.setStartDate(Date(12, 15));
  rule2.setEndDate(Date(12, 21));

  {  // year ends on Thursday
    model::YearDescription yd = model.getUniqueModelObject<model::YearDescription>();
    EXPECT_EQ(2009, yd.assumedYear());
    EXPECT_FALSE(yd.calendarYear());
    EXPECT_TRUE(yd.setCalendarYear(2009));
    ASSERT_TRUE(yd.calendarYear());
    EXPECT_EQ(2009, yd.calendarYear().get());
    EXPECT_EQ(2009, yd.assumedYear());

    ForwardTranslator ft;
    Workspace workspace = ft.translateModel(model);

    std::vector<WorkspaceObject> scheduleYears = workspace.getObjectsByType(IddObjectType::Schedule_Year);
    ASSERT_EQ(1u, scheduleYears.size());
    std::vector<IdfExtensibleGroup> extensibleGroups = scheduleYears[0].extensibleGroups();
    ASSERT_EQ(4u, extensibleGroups.size());

    // 1/1-12/12
    ASSERT_TRUE(extensibleGroups[0].getInt(Schedule_YearExtensibleFields::StartMonth));
    EXPECT_EQ(1, extensibleGroups[0].getInt(Schedule_YearExtensibleFields::StartMonth).get());
    ASSERT_TRUE(extensibleGroups[0].getInt(Schedule_YearExtensibleFields::StartDay));
    EXPECT_EQ(1, extensibleGroups[0].getInt(Schedule_YearExtensibleFields::StartDay).get());
    ASSERT_TRUE(extensibleGroups[0].getInt(Schedule_YearExtensibleFields::EndMonth));
    EXPECT_EQ(12, extensibleGroups[0].getInt(Schedule_YearExtensibleFields::EndMonth).get());
    ASSERT_TRUE(extensibleGroups[0].getInt(Schedule_YearExtensibleFields::EndDay));
    EXPECT_EQ(12, extensibleGroups[0].getInt(Schedule_YearExtensibleFields::EndDay).get());

    // 12/13-12/19
    ASSERT_TRUE(extensibleGroups[1].getInt(Schedule_YearExtensibleFields::StartMonth));
    EXPECT_EQ(12, extensibleGroups[1].getInt(Schedule_YearExtensibleFields::StartMonth).get());
    ASSERT_TRUE(extensibleGroups[1].getInt(Schedule_YearExtensibleFields::StartDay));
    EXPECT_EQ(13, extensibleGroups[1].getInt(Schedule_YearExtensibleFields::StartDay).get());
    ASSERT_TRUE(extensibleGroups[1].getInt(Schedule_YearExtensibleFields::EndMonth));
    EXPECT_EQ(12, extensibleGroups[1].getInt(Schedule_YearExtensibleFields::EndMonth).get());
    ASSERT_TRUE(extensibleGroups[1].getInt(Schedule_YearExtensibleFields::EndDay));
    EXPECT_EQ(19, extensibleGroups[1].getInt(Schedule_YearExtensibleFields::EndDay).get());

    // 12/20-12/26
    ASSERT_TRUE(extensibleGroups[2].getInt(Schedule_YearExtensibleFields::StartMonth));
    EXPECT_EQ(12, extensibleGroups[2].getInt(Schedule_YearExtensibleFields::StartMonth).get());
    ASSERT_TRUE(extensibleGroups[2].getInt(Schedule_YearExtensibleFields::StartDay));
    EXPECT_EQ(20, extensibleGroups[2].getInt(Schedule_YearExtensibleFields::StartDay).get());
    ASSERT_TRUE(extensibleGroups[2].getInt(Schedule_YearExtensibleFields::EndMonth));
    EXPECT_EQ(12, extensibleGroups[2].getInt(Schedule_YearExtensibleFields::EndMonth).get());
    ASSERT_TRUE(extensibleGroups[2].getInt(Schedule_YearExtensibleFields::EndDay));
    EXPECT_EQ(26, extensibleGroups[2].getInt(Schedule_YearExtensibleFields::EndDay).get());

    // 12/27-12/31
    ASSERT_TRUE(extensibleGroups[3].getInt(Schedule_YearExtensibleFields::StartMonth));
    EXPECT_EQ(12, extensibleGroups[3].getInt(Schedule_YearExtensibleFields::StartMonth).get());
    ASSERT_TRUE(extensibleGroups[3].getInt(Schedule_YearExtensibleFields::StartDay));
    EXPECT_EQ(27, extensibleGroups[3].getInt(Schedule_YearExtensibleFields::StartDay).get());
    ASSERT_TRUE(extensibleGroups[3].getInt(Schedule_YearExtensibleFields::EndMonth));
    EXPECT_EQ(12, extensibleGroups[3].getInt(Schedule_YearExtensibleFields::EndMonth).get());
    ASSERT_TRUE(extensibleGroups[3].getInt(Schedule_YearExtensibleFields::EndDay));
    EXPECT_EQ(31, extensibleGroups[3].getInt(Schedule_YearExtensibleFields::EndDay).get());
  }

  {  // year ends on Saturday
    model::YearDescription yd = model.getUniqueModelObject<model::YearDescription>();
    EXPECT_TRUE(yd.setCalendarYear(2011));
    ASSERT_TRUE(yd.calendarYear());
    EXPECT_EQ(2011, yd.calendarYear().get());
    EXPECT_EQ(2011, yd.assumedYear());

    ForwardTranslator ft;
    Workspace workspace = ft.translateModel(model);

    std::vector<WorkspaceObject> scheduleYears = workspace.getObjectsByType(IddObjectType::Schedule_Year);
    ASSERT_EQ(1u, scheduleYears.size());
    std::vector<IdfExtensibleGroup> extensibleGroups = scheduleYears[0].extensibleGroups();
    ASSERT_EQ(4u, extensibleGroups.size());

    // 1/1-12/10
    ASSERT_TRUE(extensibleGroups[0].getInt(Schedule_YearExtensibleFields::StartMonth));
    EXPECT_EQ(1, extensibleGroups[0].getInt(Schedule_YearExtensibleFields::StartMonth).get());
    ASSERT_TRUE(extensibleGroups[0].getInt(Schedule_YearExtensibleFields::StartDay));
    EXPECT_EQ(1, extensibleGroups[0].getInt(Schedule_YearExtensibleFields::StartDay).get());
    ASSERT_TRUE(extensibleGroups[0].getInt(Schedule_YearExtensibleFields::EndMonth));
    EXPECT_EQ(12, extensibleGroups[0].getInt(Schedule_YearExtensibleFields::EndMonth).get());
    ASSERT_TRUE(extensibleGroups[0].getInt(Schedule_YearExtensibleFields::EndDay));
    EXPECT_EQ(10, extensibleGroups[0].getInt(Schedule_YearExtensibleFields::EndDay).get());

    // 12/11-12/17
    ASSERT_TRUE(extensibleGroups[1].getInt(Schedule_YearExtensibleFields::StartMonth));
    EXPECT_EQ(12, extensibleGroups[1].getInt(Schedule_YearExtensibleFields::StartMonth).get());
    ASSERT_TRUE(extensibleGroups[1].getInt(Schedule_YearExtensibleFields::StartDay));
    EXPECT_EQ(11, extensibleGroups[1].getInt(Schedule_YearExtensibleFields::StartDay).get());
    ASSERT_TRUE(extensibleGroups[1].getInt(Schedule_YearExtensibleFields::EndMonth));
    EXPECT_EQ(12, extensibleGroups[1].getInt(Schedule_YearExtensibleFields::EndMonth).get());
    ASSERT_TRUE(extensibleGroups[1].getInt(Schedule_YearExtensibleFields::EndDay));
    EXPECT_EQ(17, extensibleGroups[1].getInt(Schedule_YearExtensibleFields::EndDay).get());

    // 12/18-12/24
    ASSERT_TRUE(extensibleGroups[2].getInt(Schedule_YearExtensibleFields::StartMonth));
    EXPECT_EQ(12, extensibleGroups[2].getInt(Schedule_YearExtensibleFields::StartMonth).get());
    ASSERT_TRUE(extensibleGroups[2].getInt(Schedule_YearExtensibleFields::StartDay));
    EXPECT_EQ(18, extensibleGroups[2].getInt(Schedule_YearExtensibleFields::StartDay).get());
    ASSERT_TRUE(extensibleGroups[2].getInt(Schedule_YearExtensibleFields::EndMonth));
    EXPECT_EQ(12, extensibleGroups[2].getInt(Schedule_YearExtensibleFields::EndMonth).get());
    ASSERT_TRUE(extensibleGroups[2].getInt(Schedule_YearExtensibleFields::EndDay));
    EXPECT_EQ(24, extensibleGroups[2].getInt(Schedule_YearExtensibleFields::EndDay).get());

    // 12/25-12/31
    ASSERT_TRUE(extensibleGroups[3].getInt(Schedule_YearExtensibleFields::StartMonth));
    EXPECT_EQ(12, extensibleGroups[3].getInt(Schedule_YearExtensibleFields::StartMonth).get());
    ASSERT_TRUE(extensibleGroups[3].getInt(Schedule_YearExtensibleFields::StartDay));
    EXPECT_EQ(25, extensibleGroups[3].getInt(Schedule_YearExtensibleFields::StartDay).get());
    ASSERT_TRUE(extensibleGroups[3].getInt(Schedule_YearExtensibleFields::EndMonth));
    EXPECT_EQ(12, extensibleGroups[3].getInt(Schedule_YearExtensibleFields::EndMonth).get());
    ASSERT_TRUE(extensibleGroups[3].getInt(Schedule_YearExtensibleFields::EndDay));
    EXPECT_EQ(31, extensibleGroups[3].getInt(Schedule_YearExtensibleFields::EndDay).get());
  }

  {  // year ends on Sunday
    model::YearDescription yd = model.getUniqueModelObject<model::YearDescription>();
    EXPECT_TRUE(yd.setCalendarYear(2023));
    ASSERT_TRUE(yd.calendarYear());
    EXPECT_EQ(2023, yd.calendarYear().get());
    EXPECT_EQ(2023, yd.assumedYear());

    ForwardTranslator ft;
    Workspace workspace = ft.translateModel(model);

    std::vector<WorkspaceObject> scheduleYears = workspace.getObjectsByType(IddObjectType::Schedule_Year);
    ASSERT_EQ(1u, scheduleYears.size());
    std::vector<IdfExtensibleGroup> extensibleGroups = scheduleYears[0].extensibleGroups();
    ASSERT_EQ(4u, extensibleGroups.size());

    // 1/1-12/9
    ASSERT_TRUE(extensibleGroups[0].getInt(Schedule_YearExtensibleFields::StartMonth));
    EXPECT_EQ(1, extensibleGroups[0].getInt(Schedule_YearExtensibleFields::StartMonth).get());
    ASSERT_TRUE(extensibleGroups[0].getInt(Schedule_YearExtensibleFields::StartDay));
    EXPECT_EQ(1, extensibleGroups[0].getInt(Schedule_YearExtensibleFields::StartDay).get());
    ASSERT_TRUE(extensibleGroups[0].getInt(Schedule_YearExtensibleFields::EndMonth));
    EXPECT_EQ(12, extensibleGroups[0].getInt(Schedule_YearExtensibleFields::EndMonth).get());
    ASSERT_TRUE(extensibleGroups[0].getInt(Schedule_YearExtensibleFields::EndDay));
    EXPECT_EQ(9, extensibleGroups[0].getInt(Schedule_YearExtensibleFields::EndDay).get());

    // 12/10-12/16
    ASSERT_TRUE(extensibleGroups[1].getInt(Schedule_YearExtensibleFields::StartMonth));
    EXPECT_EQ(12, extensibleGroups[1].getInt(Schedule_YearExtensibleFields::StartMonth).get());
    ASSERT_TRUE(extensibleGroups[1].getInt(Schedule_YearExtensibleFields::StartDay));
    EXPECT_EQ(10, extensibleGroups[1].getInt(Schedule_YearExtensibleFields::StartDay).get());
    ASSERT_TRUE(extensibleGroups[1].getInt(Schedule_YearExtensibleFields::EndMonth));
    EXPECT_EQ(12, extensibleGroups[1].getInt(Schedule_YearExtensibleFields::EndMonth).get());
    ASSERT_TRUE(extensibleGroups[1].getInt(Schedule_YearExtensibleFields::EndDay));
    EXPECT_EQ(16, extensibleGroups[1].getInt(Schedule_YearExtensibleFields::EndDay).get());

    // 12/17-12/23
    ASSERT_TRUE(extensibleGroups[2].getInt(Schedule_YearExtensibleFields::StartMonth));
    EXPECT_EQ(12, extensibleGroups[2].getInt(Schedule_YearExtensibleFields::StartMonth).get());
    ASSERT_TRUE(extensibleGroups[2].getInt(Schedule_YearExtensibleFields::StartDay));
    EXPECT_EQ(17, extensibleGroups[2].getInt(Schedule_YearExtensibleFields::StartDay).get());
    ASSERT_TRUE(extensibleGroups[2].getInt(Schedule_YearExtensibleFields::EndMonth));
    EXPECT_EQ(12, extensibleGroups[2].getInt(Schedule_YearExtensibleFields::EndMonth).get());
    ASSERT_TRUE(extensibleGroups[2].getInt(Schedule_YearExtensibleFields::EndDay));
    EXPECT_EQ(23, extensibleGroups[2].getInt(Schedule_YearExtensibleFields::EndDay).get());

    // 12/24-12/31
    ASSERT_TRUE(extensibleGroups[3].getInt(Schedule_YearExtensibleFields::StartMonth));
    EXPECT_EQ(12, extensibleGroups[3].getInt(Schedule_YearExtensibleFields::StartMonth).get());
    ASSERT_TRUE(extensibleGroups[3].getInt(Schedule_YearExtensibleFields::StartDay));
    EXPECT_EQ(24, extensibleGroups[3].getInt(Schedule_YearExtensibleFields::StartDay).get());
    ASSERT_TRUE(extensibleGroups[3].getInt(Schedule_YearExtensibleFields::EndMonth));
    EXPECT_EQ(12, extensibleGroups[3].getInt(Schedule_YearExtensibleFields::EndMonth).get());
    ASSERT_TRUE(extensibleGroups[3].getInt(Schedule_YearExtensibleFields::EndDay));
    EXPECT_EQ(31, extensibleGroups[3].getInt(Schedule_YearExtensibleFields::EndDay).get());
  }

  {  // (leap) year ends on Monday
    model::YearDescription yd = model.getUniqueModelObject<model::YearDescription>();
    EXPECT_TRUE(yd.setCalendarYear(2012));
    ASSERT_TRUE(yd.calendarYear());
    EXPECT_EQ(2012, yd.calendarYear().get());
    EXPECT_EQ(2012, yd.assumedYear());

    ForwardTranslator ft;
    Workspace workspace = ft.translateModel(model);

    std::vector<WorkspaceObject> scheduleYears = workspace.getObjectsByType(IddObjectType::Schedule_Year);
    ASSERT_EQ(1u, scheduleYears.size());
    std::vector<IdfExtensibleGroup> extensibleGroups = scheduleYears[0].extensibleGroups();
    ASSERT_EQ(4u, extensibleGroups.size());

    // 1/1-12/8
    ASSERT_TRUE(extensibleGroups[0].getInt(Schedule_YearExtensibleFields::StartMonth));
    EXPECT_EQ(1, extensibleGroups[0].getInt(Schedule_YearExtensibleFields::StartMonth).get());
    ASSERT_TRUE(extensibleGroups[0].getInt(Schedule_YearExtensibleFields::StartDay));
    EXPECT_EQ(1, extensibleGroups[0].getInt(Schedule_YearExtensibleFields::StartDay).get());
    ASSERT_TRUE(extensibleGroups[0].getInt(Schedule_YearExtensibleFields::EndMonth));
    EXPECT_EQ(12, extensibleGroups[0].getInt(Schedule_YearExtensibleFields::EndMonth).get());
    ASSERT_TRUE(extensibleGroups[0].getInt(Schedule_YearExtensibleFields::EndDay));
    EXPECT_EQ(8, extensibleGroups[0].getInt(Schedule_YearExtensibleFields::EndDay).get());

    // 12/9-12/15
    ASSERT_TRUE(extensibleGroups[1].getInt(Schedule_YearExtensibleFields::StartMonth));
    EXPECT_EQ(12, extensibleGroups[1].getInt(Schedule_YearExtensibleFields::StartMonth).get());
    ASSERT_TRUE(extensibleGroups[1].getInt(Schedule_YearExtensibleFields::StartDay));
    EXPECT_EQ(9, extensibleGroups[1].getInt(Schedule_YearExtensibleFields::StartDay).get());
    ASSERT_TRUE(extensibleGroups[1].getInt(Schedule_YearExtensibleFields::EndMonth));
    EXPECT_EQ(12, extensibleGroups[1].getInt(Schedule_YearExtensibleFields::EndMonth).get());
    ASSERT_TRUE(extensibleGroups[1].getInt(Schedule_YearExtensibleFields::EndDay));
    EXPECT_EQ(15, extensibleGroups[1].getInt(Schedule_YearExtensibleFields::EndDay).get());

    // 12/16-12/22
    ASSERT_TRUE(extensibleGroups[2].getInt(Schedule_YearExtensibleFields::StartMonth));
    EXPECT_EQ(12, extensibleGroups[2].getInt(Schedule_YearExtensibleFields::StartMonth).get());
    ASSERT_TRUE(extensibleGroups[2].getInt(Schedule_YearExtensibleFields::StartDay));
    EXPECT_EQ(16, extensibleGroups[2].getInt(Schedule_YearExtensibleFields::StartDay).get());
    ASSERT_TRUE(extensibleGroups[2].getInt(Schedule_YearExtensibleFields::EndMonth));
    EXPECT_EQ(12, extensibleGroups[2].getInt(Schedule_YearExtensibleFields::EndMonth).get());
    ASSERT_TRUE(extensibleGroups[2].getInt(Schedule_YearExtensibleFields::EndDay));
    EXPECT_EQ(22, extensibleGroups[2].getInt(Schedule_YearExtensibleFields::EndDay).get());

    // 12/23-12/31
    ASSERT_TRUE(extensibleGroups[3].getInt(Schedule_YearExtensibleFields::StartMonth));
    EXPECT_EQ(12, extensibleGroups[3].getInt(Schedule_YearExtensibleFields::StartMonth).get());
    ASSERT_TRUE(extensibleGroups[3].getInt(Schedule_YearExtensibleFields::StartDay));
    EXPECT_EQ(23, extensibleGroups[3].getInt(Schedule_YearExtensibleFields::StartDay).get());
    ASSERT_TRUE(extensibleGroups[3].getInt(Schedule_YearExtensibleFields::EndMonth));
    EXPECT_EQ(12, extensibleGroups[3].getInt(Schedule_YearExtensibleFields::EndMonth).get());
    ASSERT_TRUE(extensibleGroups[3].getInt(Schedule_YearExtensibleFields::EndDay));
    EXPECT_EQ(31, extensibleGroups[3].getInt(Schedule_YearExtensibleFields::EndDay).get());
  }
}

TEST_F(EnergyPlusFixture, ForwardTranslator_ScheduleRuleset_Bug5113_2) {
  Model model;
  ScheduleRuleset scheduleRuleset(model);

  ScheduleRule rule1(scheduleRuleset);
  rule1.setName("Annual Rule");
  rule1.setApplySunday(true);
  rule1.setApplyMonday(true);
  rule1.setApplyTuesday(true);
  rule1.setApplyWednesday(true);
  rule1.setApplyThursday(true);
  rule1.setApplyFriday(true);
  rule1.setApplySaturday(true);
  rule1.setStartDate(Date(1, 1));
  rule1.setEndDate(Date(12, 31));

  ScheduleRule rule2(scheduleRuleset);
  rule2.setName("Dec Rule1");
  rule2.setApplySunday(true);
  rule2.setApplyMonday(true);
  rule2.setApplyTuesday(true);
  rule2.setApplyWednesday(true);
  rule2.setApplyThursday(true);
  rule2.setApplyFriday(true);
  rule2.setApplySaturday(true);
  rule2.setStartDate(Date(12, 10));
  rule2.setEndDate(Date(12, 16));

  ScheduleRule rule3(scheduleRuleset);
  rule3.setName("Dec Rule2");
  rule3.setApplySunday(true);
  rule3.setApplyMonday(true);
  rule3.setApplyTuesday(true);
  rule3.setApplyWednesday(true);
  rule3.setApplyThursday(true);
  rule3.setApplyFriday(true);
  rule3.setApplySaturday(true);
  rule3.setStartDate(Date(12, 31));
  rule3.setEndDate(Date(12, 31));

  {  // year ends on Thursday
    model::YearDescription yd = model.getUniqueModelObject<model::YearDescription>();
    EXPECT_EQ(2009, yd.assumedYear());
    EXPECT_FALSE(yd.calendarYear());
    EXPECT_TRUE(yd.setCalendarYear(2009));
    ASSERT_TRUE(yd.calendarYear());
    EXPECT_EQ(2009, yd.calendarYear().get());
    EXPECT_EQ(2009, yd.assumedYear());

    ForwardTranslator ft;
    Workspace workspace = ft.translateModel(model);

    std::vector<WorkspaceObject> scheduleYears = workspace.getObjectsByType(IddObjectType::Schedule_Year);
    ASSERT_EQ(1u, scheduleYears.size());
    std::vector<IdfExtensibleGroup> extensibleGroups = scheduleYears[0].extensibleGroups();
    ASSERT_EQ(5u, extensibleGroups.size());

    // 1/1-12/5
    ASSERT_TRUE(extensibleGroups[0].getInt(Schedule_YearExtensibleFields::StartMonth));
    EXPECT_EQ(1, extensibleGroups[0].getInt(Schedule_YearExtensibleFields::StartMonth).get());
    ASSERT_TRUE(extensibleGroups[0].getInt(Schedule_YearExtensibleFields::StartDay));
    EXPECT_EQ(1, extensibleGroups[0].getInt(Schedule_YearExtensibleFields::StartDay).get());
    ASSERT_TRUE(extensibleGroups[0].getInt(Schedule_YearExtensibleFields::EndMonth));
    EXPECT_EQ(12, extensibleGroups[0].getInt(Schedule_YearExtensibleFields::EndMonth).get());
    ASSERT_TRUE(extensibleGroups[0].getInt(Schedule_YearExtensibleFields::EndDay));
    EXPECT_EQ(5, extensibleGroups[0].getInt(Schedule_YearExtensibleFields::EndDay).get());

    // 12/6-12/12
    ASSERT_TRUE(extensibleGroups[1].getInt(Schedule_YearExtensibleFields::StartMonth));
    EXPECT_EQ(12, extensibleGroups[1].getInt(Schedule_YearExtensibleFields::StartMonth).get());
    ASSERT_TRUE(extensibleGroups[1].getInt(Schedule_YearExtensibleFields::StartDay));
    EXPECT_EQ(6, extensibleGroups[1].getInt(Schedule_YearExtensibleFields::StartDay).get());
    ASSERT_TRUE(extensibleGroups[1].getInt(Schedule_YearExtensibleFields::EndMonth));
    EXPECT_EQ(12, extensibleGroups[1].getInt(Schedule_YearExtensibleFields::EndMonth).get());
    ASSERT_TRUE(extensibleGroups[1].getInt(Schedule_YearExtensibleFields::EndDay));
    EXPECT_EQ(12, extensibleGroups[1].getInt(Schedule_YearExtensibleFields::EndDay).get());

    // 12/13-12/19
    ASSERT_TRUE(extensibleGroups[2].getInt(Schedule_YearExtensibleFields::StartMonth));
    EXPECT_EQ(12, extensibleGroups[2].getInt(Schedule_YearExtensibleFields::StartMonth).get());
    ASSERT_TRUE(extensibleGroups[2].getInt(Schedule_YearExtensibleFields::StartDay));
    EXPECT_EQ(13, extensibleGroups[2].getInt(Schedule_YearExtensibleFields::StartDay).get());
    ASSERT_TRUE(extensibleGroups[2].getInt(Schedule_YearExtensibleFields::EndMonth));
    EXPECT_EQ(12, extensibleGroups[2].getInt(Schedule_YearExtensibleFields::EndMonth).get());
    ASSERT_TRUE(extensibleGroups[2].getInt(Schedule_YearExtensibleFields::EndDay));
    EXPECT_EQ(19, extensibleGroups[2].getInt(Schedule_YearExtensibleFields::EndDay).get());

    // 12/20-12/26
    ASSERT_TRUE(extensibleGroups[3].getInt(Schedule_YearExtensibleFields::StartMonth));
    EXPECT_EQ(12, extensibleGroups[3].getInt(Schedule_YearExtensibleFields::StartMonth).get());
    ASSERT_TRUE(extensibleGroups[3].getInt(Schedule_YearExtensibleFields::StartDay));
    EXPECT_EQ(20, extensibleGroups[3].getInt(Schedule_YearExtensibleFields::StartDay).get());
    ASSERT_TRUE(extensibleGroups[3].getInt(Schedule_YearExtensibleFields::EndMonth));
    EXPECT_EQ(12, extensibleGroups[3].getInt(Schedule_YearExtensibleFields::EndMonth).get());
    ASSERT_TRUE(extensibleGroups[3].getInt(Schedule_YearExtensibleFields::EndDay));
    EXPECT_EQ(26, extensibleGroups[3].getInt(Schedule_YearExtensibleFields::EndDay).get());

    // 12/27-12/31
    ASSERT_TRUE(extensibleGroups[4].getInt(Schedule_YearExtensibleFields::StartMonth));
    EXPECT_EQ(12, extensibleGroups[4].getInt(Schedule_YearExtensibleFields::StartMonth).get());
    ASSERT_TRUE(extensibleGroups[4].getInt(Schedule_YearExtensibleFields::StartDay));
    EXPECT_EQ(27, extensibleGroups[4].getInt(Schedule_YearExtensibleFields::StartDay).get());
    ASSERT_TRUE(extensibleGroups[4].getInt(Schedule_YearExtensibleFields::EndMonth));
    EXPECT_EQ(12, extensibleGroups[4].getInt(Schedule_YearExtensibleFields::EndMonth).get());
    ASSERT_TRUE(extensibleGroups[4].getInt(Schedule_YearExtensibleFields::EndDay));
    EXPECT_EQ(31, extensibleGroups[4].getInt(Schedule_YearExtensibleFields::EndDay).get());
  }

  {  // year ends on Saturday
    model::YearDescription yd = model.getUniqueModelObject<model::YearDescription>();
    EXPECT_TRUE(yd.setCalendarYear(2011));
    ASSERT_TRUE(yd.calendarYear());
    EXPECT_EQ(2011, yd.calendarYear().get());
    EXPECT_EQ(2011, yd.assumedYear());

    ForwardTranslator ft;
    Workspace workspace = ft.translateModel(model);

    std::vector<WorkspaceObject> scheduleYears = workspace.getObjectsByType(IddObjectType::Schedule_Year);
    ASSERT_EQ(1u, scheduleYears.size());
    std::vector<IdfExtensibleGroup> extensibleGroups = scheduleYears[0].extensibleGroups();
    ASSERT_EQ(5u, extensibleGroups.size());

    // 1/1-12/3
    ASSERT_TRUE(extensibleGroups[0].getInt(Schedule_YearExtensibleFields::StartMonth));
    EXPECT_EQ(1, extensibleGroups[0].getInt(Schedule_YearExtensibleFields::StartMonth).get());
    ASSERT_TRUE(extensibleGroups[0].getInt(Schedule_YearExtensibleFields::StartDay));
    EXPECT_EQ(1, extensibleGroups[0].getInt(Schedule_YearExtensibleFields::StartDay).get());
    ASSERT_TRUE(extensibleGroups[0].getInt(Schedule_YearExtensibleFields::EndMonth));
    EXPECT_EQ(12, extensibleGroups[0].getInt(Schedule_YearExtensibleFields::EndMonth).get());
    ASSERT_TRUE(extensibleGroups[0].getInt(Schedule_YearExtensibleFields::EndDay));
    EXPECT_EQ(3, extensibleGroups[0].getInt(Schedule_YearExtensibleFields::EndDay).get());

    // 12/4-12/10
    ASSERT_TRUE(extensibleGroups[1].getInt(Schedule_YearExtensibleFields::StartMonth));
    EXPECT_EQ(12, extensibleGroups[1].getInt(Schedule_YearExtensibleFields::StartMonth).get());
    ASSERT_TRUE(extensibleGroups[1].getInt(Schedule_YearExtensibleFields::StartDay));
    EXPECT_EQ(4, extensibleGroups[1].getInt(Schedule_YearExtensibleFields::StartDay).get());
    ASSERT_TRUE(extensibleGroups[1].getInt(Schedule_YearExtensibleFields::EndMonth));
    EXPECT_EQ(12, extensibleGroups[1].getInt(Schedule_YearExtensibleFields::EndMonth).get());
    ASSERT_TRUE(extensibleGroups[1].getInt(Schedule_YearExtensibleFields::EndDay));
    EXPECT_EQ(10, extensibleGroups[1].getInt(Schedule_YearExtensibleFields::EndDay).get());

    // 12/11-12/17
    ASSERT_TRUE(extensibleGroups[2].getInt(Schedule_YearExtensibleFields::StartMonth));
    EXPECT_EQ(12, extensibleGroups[2].getInt(Schedule_YearExtensibleFields::StartMonth).get());
    ASSERT_TRUE(extensibleGroups[2].getInt(Schedule_YearExtensibleFields::StartDay));
    EXPECT_EQ(11, extensibleGroups[2].getInt(Schedule_YearExtensibleFields::StartDay).get());
    ASSERT_TRUE(extensibleGroups[2].getInt(Schedule_YearExtensibleFields::EndMonth));
    EXPECT_EQ(12, extensibleGroups[2].getInt(Schedule_YearExtensibleFields::EndMonth).get());
    ASSERT_TRUE(extensibleGroups[2].getInt(Schedule_YearExtensibleFields::EndDay));
    EXPECT_EQ(17, extensibleGroups[2].getInt(Schedule_YearExtensibleFields::EndDay).get());

    // 12/18-12/24
    ASSERT_TRUE(extensibleGroups[3].getInt(Schedule_YearExtensibleFields::StartMonth));
    EXPECT_EQ(12, extensibleGroups[3].getInt(Schedule_YearExtensibleFields::StartMonth).get());
    ASSERT_TRUE(extensibleGroups[3].getInt(Schedule_YearExtensibleFields::StartDay));
    EXPECT_EQ(18, extensibleGroups[3].getInt(Schedule_YearExtensibleFields::StartDay).get());
    ASSERT_TRUE(extensibleGroups[3].getInt(Schedule_YearExtensibleFields::EndMonth));
    EXPECT_EQ(12, extensibleGroups[3].getInt(Schedule_YearExtensibleFields::EndMonth).get());
    ASSERT_TRUE(extensibleGroups[3].getInt(Schedule_YearExtensibleFields::EndDay));
    EXPECT_EQ(24, extensibleGroups[3].getInt(Schedule_YearExtensibleFields::EndDay).get());

    // 12/25-12/31
    ASSERT_TRUE(extensibleGroups[4].getInt(Schedule_YearExtensibleFields::StartMonth));
    EXPECT_EQ(12, extensibleGroups[4].getInt(Schedule_YearExtensibleFields::StartMonth).get());
    ASSERT_TRUE(extensibleGroups[4].getInt(Schedule_YearExtensibleFields::StartDay));
    EXPECT_EQ(25, extensibleGroups[4].getInt(Schedule_YearExtensibleFields::StartDay).get());
    ASSERT_TRUE(extensibleGroups[4].getInt(Schedule_YearExtensibleFields::EndMonth));
    EXPECT_EQ(12, extensibleGroups[4].getInt(Schedule_YearExtensibleFields::EndMonth).get());
    ASSERT_TRUE(extensibleGroups[4].getInt(Schedule_YearExtensibleFields::EndDay));
    EXPECT_EQ(31, extensibleGroups[4].getInt(Schedule_YearExtensibleFields::EndDay).get());
  }

  {  // year ends on Sunday
    model::YearDescription yd = model.getUniqueModelObject<model::YearDescription>();
    EXPECT_TRUE(yd.setCalendarYear(2023));
    ASSERT_TRUE(yd.calendarYear());
    EXPECT_EQ(2023, yd.calendarYear().get());
    EXPECT_EQ(2023, yd.assumedYear());

    ForwardTranslator ft;
    Workspace workspace = ft.translateModel(model);

    std::vector<WorkspaceObject> scheduleYears = workspace.getObjectsByType(IddObjectType::Schedule_Year);
    ASSERT_EQ(1u, scheduleYears.size());
    std::vector<IdfExtensibleGroup> extensibleGroups = scheduleYears[0].extensibleGroups();
    ASSERT_EQ(4u, extensibleGroups.size());

    // 1/1-12/9
    ASSERT_TRUE(extensibleGroups[0].getInt(Schedule_YearExtensibleFields::StartMonth));
    EXPECT_EQ(1, extensibleGroups[0].getInt(Schedule_YearExtensibleFields::StartMonth).get());
    ASSERT_TRUE(extensibleGroups[0].getInt(Schedule_YearExtensibleFields::StartDay));
    EXPECT_EQ(1, extensibleGroups[0].getInt(Schedule_YearExtensibleFields::StartDay).get());
    ASSERT_TRUE(extensibleGroups[0].getInt(Schedule_YearExtensibleFields::EndMonth));
    EXPECT_EQ(12, extensibleGroups[0].getInt(Schedule_YearExtensibleFields::EndMonth).get());
    ASSERT_TRUE(extensibleGroups[0].getInt(Schedule_YearExtensibleFields::EndDay));
    EXPECT_EQ(9, extensibleGroups[0].getInt(Schedule_YearExtensibleFields::EndDay).get());

    // 12/10-12/16
    ASSERT_TRUE(extensibleGroups[1].getInt(Schedule_YearExtensibleFields::StartMonth));
    EXPECT_EQ(12, extensibleGroups[1].getInt(Schedule_YearExtensibleFields::StartMonth).get());
    ASSERT_TRUE(extensibleGroups[1].getInt(Schedule_YearExtensibleFields::StartDay));
    EXPECT_EQ(10, extensibleGroups[1].getInt(Schedule_YearExtensibleFields::StartDay).get());
    ASSERT_TRUE(extensibleGroups[1].getInt(Schedule_YearExtensibleFields::EndMonth));
    EXPECT_EQ(12, extensibleGroups[1].getInt(Schedule_YearExtensibleFields::EndMonth).get());
    ASSERT_TRUE(extensibleGroups[1].getInt(Schedule_YearExtensibleFields::EndDay));
    EXPECT_EQ(16, extensibleGroups[1].getInt(Schedule_YearExtensibleFields::EndDay).get());

    // 12/17-12/30
    ASSERT_TRUE(extensibleGroups[2].getInt(Schedule_YearExtensibleFields::StartMonth));
    EXPECT_EQ(12, extensibleGroups[2].getInt(Schedule_YearExtensibleFields::StartMonth).get());
    ASSERT_TRUE(extensibleGroups[2].getInt(Schedule_YearExtensibleFields::StartDay));
    EXPECT_EQ(17, extensibleGroups[2].getInt(Schedule_YearExtensibleFields::StartDay).get());
    ASSERT_TRUE(extensibleGroups[2].getInt(Schedule_YearExtensibleFields::EndMonth));
    EXPECT_EQ(12, extensibleGroups[2].getInt(Schedule_YearExtensibleFields::EndMonth).get());
    ASSERT_TRUE(extensibleGroups[2].getInt(Schedule_YearExtensibleFields::EndDay));
    EXPECT_EQ(30, extensibleGroups[2].getInt(Schedule_YearExtensibleFields::EndDay).get());

    // 12/31-12/31
    ASSERT_TRUE(extensibleGroups[3].getInt(Schedule_YearExtensibleFields::StartMonth));
    EXPECT_EQ(12, extensibleGroups[3].getInt(Schedule_YearExtensibleFields::StartMonth).get());
    ASSERT_TRUE(extensibleGroups[3].getInt(Schedule_YearExtensibleFields::StartDay));
    EXPECT_EQ(31, extensibleGroups[3].getInt(Schedule_YearExtensibleFields::StartDay).get());
    ASSERT_TRUE(extensibleGroups[3].getInt(Schedule_YearExtensibleFields::EndMonth));
    EXPECT_EQ(12, extensibleGroups[3].getInt(Schedule_YearExtensibleFields::EndMonth).get());
    ASSERT_TRUE(extensibleGroups[3].getInt(Schedule_YearExtensibleFields::EndDay));
    EXPECT_EQ(31, extensibleGroups[3].getInt(Schedule_YearExtensibleFields::EndDay).get());
  }

  {  // (leap) year ends on Monday
    model::YearDescription yd = model.getUniqueModelObject<model::YearDescription>();
    EXPECT_TRUE(yd.setCalendarYear(2012));
    ASSERT_TRUE(yd.calendarYear());
    EXPECT_EQ(2012, yd.calendarYear().get());
    EXPECT_EQ(2012, yd.assumedYear());

    ForwardTranslator ft;
    Workspace workspace = ft.translateModel(model);

    std::vector<WorkspaceObject> scheduleYears = workspace.getObjectsByType(IddObjectType::Schedule_Year);
    ASSERT_EQ(1u, scheduleYears.size());
    std::vector<IdfExtensibleGroup> extensibleGroups = scheduleYears[0].extensibleGroups();
    ASSERT_EQ(5u, extensibleGroups.size());

    // 1/1-12/8
    ASSERT_TRUE(extensibleGroups[0].getInt(Schedule_YearExtensibleFields::StartMonth));
    EXPECT_EQ(1, extensibleGroups[0].getInt(Schedule_YearExtensibleFields::StartMonth).get());
    ASSERT_TRUE(extensibleGroups[0].getInt(Schedule_YearExtensibleFields::StartDay));
    EXPECT_EQ(1, extensibleGroups[0].getInt(Schedule_YearExtensibleFields::StartDay).get());
    ASSERT_TRUE(extensibleGroups[0].getInt(Schedule_YearExtensibleFields::EndMonth));
    EXPECT_EQ(12, extensibleGroups[0].getInt(Schedule_YearExtensibleFields::EndMonth).get());
    ASSERT_TRUE(extensibleGroups[0].getInt(Schedule_YearExtensibleFields::EndDay));
    EXPECT_EQ(8, extensibleGroups[0].getInt(Schedule_YearExtensibleFields::EndDay).get());

    // 12/9-12/15
    ASSERT_TRUE(extensibleGroups[1].getInt(Schedule_YearExtensibleFields::StartMonth));
    EXPECT_EQ(12, extensibleGroups[1].getInt(Schedule_YearExtensibleFields::StartMonth).get());
    ASSERT_TRUE(extensibleGroups[1].getInt(Schedule_YearExtensibleFields::StartDay));
    EXPECT_EQ(9, extensibleGroups[1].getInt(Schedule_YearExtensibleFields::StartDay).get());
    ASSERT_TRUE(extensibleGroups[1].getInt(Schedule_YearExtensibleFields::EndMonth));
    EXPECT_EQ(12, extensibleGroups[1].getInt(Schedule_YearExtensibleFields::EndMonth).get());
    ASSERT_TRUE(extensibleGroups[1].getInt(Schedule_YearExtensibleFields::EndDay));
    EXPECT_EQ(15, extensibleGroups[1].getInt(Schedule_YearExtensibleFields::EndDay).get());

    // 12/16-12/22
    ASSERT_TRUE(extensibleGroups[2].getInt(Schedule_YearExtensibleFields::StartMonth));
    EXPECT_EQ(12, extensibleGroups[2].getInt(Schedule_YearExtensibleFields::StartMonth).get());
    ASSERT_TRUE(extensibleGroups[2].getInt(Schedule_YearExtensibleFields::StartDay));
    EXPECT_EQ(16, extensibleGroups[2].getInt(Schedule_YearExtensibleFields::StartDay).get());
    ASSERT_TRUE(extensibleGroups[2].getInt(Schedule_YearExtensibleFields::EndMonth));
    EXPECT_EQ(12, extensibleGroups[2].getInt(Schedule_YearExtensibleFields::EndMonth).get());
    ASSERT_TRUE(extensibleGroups[2].getInt(Schedule_YearExtensibleFields::EndDay));
    EXPECT_EQ(22, extensibleGroups[2].getInt(Schedule_YearExtensibleFields::EndDay).get());

    // 12/23-12/29
    ASSERT_TRUE(extensibleGroups[3].getInt(Schedule_YearExtensibleFields::StartMonth));
    EXPECT_EQ(12, extensibleGroups[3].getInt(Schedule_YearExtensibleFields::StartMonth).get());
    ASSERT_TRUE(extensibleGroups[3].getInt(Schedule_YearExtensibleFields::StartDay));
    EXPECT_EQ(23, extensibleGroups[3].getInt(Schedule_YearExtensibleFields::StartDay).get());
    ASSERT_TRUE(extensibleGroups[3].getInt(Schedule_YearExtensibleFields::EndMonth));
    EXPECT_EQ(12, extensibleGroups[3].getInt(Schedule_YearExtensibleFields::EndMonth).get());
    ASSERT_TRUE(extensibleGroups[3].getInt(Schedule_YearExtensibleFields::EndDay));
    EXPECT_EQ(29, extensibleGroups[3].getInt(Schedule_YearExtensibleFields::EndDay).get());

    // 12/30-12/31
    ASSERT_TRUE(extensibleGroups[4].getInt(Schedule_YearExtensibleFields::StartMonth));
    EXPECT_EQ(12, extensibleGroups[4].getInt(Schedule_YearExtensibleFields::StartMonth).get());
    ASSERT_TRUE(extensibleGroups[4].getInt(Schedule_YearExtensibleFields::StartDay));
    EXPECT_EQ(30, extensibleGroups[4].getInt(Schedule_YearExtensibleFields::StartDay).get());
    ASSERT_TRUE(extensibleGroups[4].getInt(Schedule_YearExtensibleFields::EndMonth));
    EXPECT_EQ(12, extensibleGroups[4].getInt(Schedule_YearExtensibleFields::EndMonth).get());
    ASSERT_TRUE(extensibleGroups[4].getInt(Schedule_YearExtensibleFields::EndDay));
    EXPECT_EQ(31, extensibleGroups[4].getInt(Schedule_YearExtensibleFields::EndDay).get());
  }
}
