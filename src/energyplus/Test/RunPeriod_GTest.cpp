/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>
#include "EnergyPlusFixture.hpp"

#include "../ErrorFile.hpp"
#include "../ForwardTranslator.hpp"
#include "../ReverseTranslator.hpp"

#include "../../model/Model.hpp"
#include "../../model/RunPeriod.hpp"
#include "../../model/RunPeriod_Impl.hpp"

#include "../../utilities/time/Date.hpp"

#include <utilities/idd/OS_RunPeriod_FieldEnums.hxx>
#include <utilities/idd/RunPeriod_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include <resources.hxx>

#include <sstream>

using namespace openstudio;
using namespace openstudio::model;
using openstudio::energyplus::ForwardTranslator;
using openstudio::energyplus::ReverseTranslator;

TEST_F(EnergyPlusFixture, ForwardTranslator_NoRunPeriod) {
  Model model;

  ForwardTranslator forwardTranslator;
  Workspace workspace = forwardTranslator.translateModel(model);

  ASSERT_EQ(1u, workspace.getObjectsByType(IddObjectType::RunPeriod).size());
  WorkspaceObject idfObject = workspace.getObjectsByType(IddObjectType::RunPeriod)[0];
  ASSERT_TRUE(idfObject.getString(RunPeriodFields::Name, false));
  EXPECT_EQ("Run Period 1", idfObject.getString(RunPeriodFields::Name, false).get());
  ASSERT_TRUE(idfObject.getInt(RunPeriodFields::BeginMonth, false));
  EXPECT_EQ(1, idfObject.getInt(RunPeriodFields::BeginMonth, false).get());
  ASSERT_TRUE(idfObject.getInt(RunPeriodFields::BeginDayofMonth, false));
  EXPECT_EQ(1, idfObject.getInt(RunPeriodFields::BeginDayofMonth, false).get());
  ASSERT_TRUE(idfObject.getInt(RunPeriodFields::BeginYear, false));
  EXPECT_EQ(2009, idfObject.getInt(RunPeriodFields::BeginYear, false).get());
  ASSERT_TRUE(idfObject.getInt(RunPeriodFields::EndMonth, false));
  EXPECT_EQ(12, idfObject.getInt(RunPeriodFields::EndMonth, false).get());
  ASSERT_TRUE(idfObject.getInt(RunPeriodFields::EndDayofMonth, false));
  EXPECT_EQ(31, idfObject.getInt(RunPeriodFields::EndDayofMonth, false).get());
  ASSERT_TRUE(idfObject.getInt(RunPeriodFields::EndYear, false));
  EXPECT_EQ(2009, idfObject.getInt(RunPeriodFields::EndYear, false).get());
  ASSERT_TRUE(idfObject.getString(RunPeriodFields::DayofWeekforStartDay, false));
  EXPECT_EQ("Thursday", idfObject.getString(RunPeriodFields::DayofWeekforStartDay, false).get());
  ASSERT_TRUE(idfObject.getString(RunPeriodFields::UseWeatherFileHolidaysandSpecialDays, false));
  EXPECT_EQ("No", idfObject.getString(RunPeriodFields::UseWeatherFileHolidaysandSpecialDays, false).get());
  ASSERT_TRUE(idfObject.getString(RunPeriodFields::UseWeatherFileDaylightSavingPeriod, false));
  EXPECT_EQ("No", idfObject.getString(RunPeriodFields::UseWeatherFileDaylightSavingPeriod, false).get());
  ASSERT_TRUE(idfObject.getString(RunPeriodFields::ApplyWeekendHolidayRule, false));
  EXPECT_EQ("No", idfObject.getString(RunPeriodFields::ApplyWeekendHolidayRule, false).get());
  ASSERT_TRUE(idfObject.getString(RunPeriodFields::UseWeatherFileRainIndicators, false));
  EXPECT_EQ("Yes", idfObject.getString(RunPeriodFields::UseWeatherFileRainIndicators, false).get());
  ASSERT_TRUE(idfObject.getString(RunPeriodFields::UseWeatherFileSnowIndicators, false));
  EXPECT_EQ("Yes", idfObject.getString(RunPeriodFields::UseWeatherFileSnowIndicators, false).get());
}

TEST_F(EnergyPlusFixture, ForwardTranslator_DefaultRunPeriod) {
  Model model;
  RunPeriod runPeriod = model.getUniqueModelObject<RunPeriod>();

  ForwardTranslator forwardTranslator;
  Workspace workspace = forwardTranslator.translateModel(model);

  ASSERT_EQ(1u, workspace.getObjectsByType(IddObjectType::RunPeriod).size());
  WorkspaceObject idfObject = workspace.getObjectsByType(IddObjectType::RunPeriod)[0];
  ASSERT_TRUE(idfObject.getString(RunPeriodFields::Name, false));
  EXPECT_EQ("Run Period 1", idfObject.getString(RunPeriodFields::Name, false).get());
  ASSERT_TRUE(idfObject.getInt(RunPeriodFields::BeginMonth, false));
  EXPECT_EQ(1, idfObject.getInt(RunPeriodFields::BeginMonth, false).get());
  ASSERT_TRUE(idfObject.getInt(RunPeriodFields::BeginDayofMonth, false));
  EXPECT_EQ(1, idfObject.getInt(RunPeriodFields::BeginDayofMonth, false).get());
  ASSERT_TRUE(idfObject.getInt(RunPeriodFields::BeginYear, false));
  EXPECT_EQ(2009, idfObject.getInt(RunPeriodFields::BeginYear, false).get());
  ASSERT_TRUE(idfObject.getInt(RunPeriodFields::EndMonth, false));
  EXPECT_EQ(12, idfObject.getInt(RunPeriodFields::EndMonth, false).get());
  ASSERT_TRUE(idfObject.getInt(RunPeriodFields::EndDayofMonth, false));
  EXPECT_EQ(31, idfObject.getInt(RunPeriodFields::EndDayofMonth, false).get());
  ASSERT_TRUE(idfObject.getInt(RunPeriodFields::EndYear, false));
  EXPECT_EQ(2009, idfObject.getInt(RunPeriodFields::EndYear, false).get());
  ASSERT_TRUE(idfObject.getString(RunPeriodFields::DayofWeekforStartDay, false));
  EXPECT_EQ("Thursday", idfObject.getString(RunPeriodFields::DayofWeekforStartDay, false).get());
  ASSERT_TRUE(idfObject.getString(RunPeriodFields::UseWeatherFileHolidaysandSpecialDays, false));
  EXPECT_EQ("No", idfObject.getString(RunPeriodFields::UseWeatherFileHolidaysandSpecialDays, false).get());
  ASSERT_TRUE(idfObject.getString(RunPeriodFields::UseWeatherFileDaylightSavingPeriod, false));
  EXPECT_EQ("No", idfObject.getString(RunPeriodFields::UseWeatherFileDaylightSavingPeriod, false).get());
  ASSERT_TRUE(idfObject.getString(RunPeriodFields::ApplyWeekendHolidayRule, false));
  EXPECT_EQ("No", idfObject.getString(RunPeriodFields::ApplyWeekendHolidayRule, false).get());
  ASSERT_TRUE(idfObject.getString(RunPeriodFields::UseWeatherFileRainIndicators, false));
  EXPECT_EQ("Yes", idfObject.getString(RunPeriodFields::UseWeatherFileRainIndicators, false).get());
  ASSERT_TRUE(idfObject.getString(RunPeriodFields::UseWeatherFileSnowIndicators, false));
  EXPECT_EQ("Yes", idfObject.getString(RunPeriodFields::UseWeatherFileSnowIndicators, false).get());
}

TEST_F(EnergyPlusFixture, ForwardTranslator_RunPeriod1) {
  Model model;
  RunPeriod runPeriod = model.getUniqueModelObject<RunPeriod>();
  runPeriod.setBeginMonth(2);
  runPeriod.setBeginDayOfMonth(4);

  ForwardTranslator forwardTranslator;
  Workspace workspace = forwardTranslator.translateModel(model);

  ASSERT_EQ(1u, workspace.getObjectsByType(IddObjectType::RunPeriod).size());
  WorkspaceObject idfObject = workspace.getObjectsByType(IddObjectType::RunPeriod)[0];
  ASSERT_TRUE(idfObject.getString(RunPeriodFields::Name, false));
  EXPECT_EQ("Run Period 1", idfObject.getString(RunPeriodFields::Name, false).get());
  ASSERT_TRUE(idfObject.getInt(RunPeriodFields::BeginMonth, false));
  EXPECT_EQ(2, idfObject.getInt(RunPeriodFields::BeginMonth, false).get());
  ASSERT_TRUE(idfObject.getInt(RunPeriodFields::BeginDayofMonth, false));
  EXPECT_EQ(4, idfObject.getInt(RunPeriodFields::BeginDayofMonth, false).get());
  ASSERT_TRUE(idfObject.getInt(RunPeriodFields::BeginYear, false));
  EXPECT_EQ(2009, idfObject.getInt(RunPeriodFields::BeginYear, false).get());
  ASSERT_TRUE(idfObject.getInt(RunPeriodFields::EndMonth, false));
  EXPECT_EQ(12, idfObject.getInt(RunPeriodFields::EndMonth, false).get());
  ASSERT_TRUE(idfObject.getInt(RunPeriodFields::EndDayofMonth, false));
  EXPECT_EQ(31, idfObject.getInt(RunPeriodFields::EndDayofMonth, false).get());
  ASSERT_TRUE(idfObject.getInt(RunPeriodFields::EndYear, false));
  EXPECT_EQ(2009, idfObject.getInt(RunPeriodFields::EndYear, false).get());
  ASSERT_TRUE(idfObject.getString(RunPeriodFields::DayofWeekforStartDay, false));
  EXPECT_EQ("Wednesday", idfObject.getString(RunPeriodFields::DayofWeekforStartDay, false).get());
  ASSERT_TRUE(idfObject.getString(RunPeriodFields::UseWeatherFileHolidaysandSpecialDays, false));
  EXPECT_EQ("No", idfObject.getString(RunPeriodFields::UseWeatherFileHolidaysandSpecialDays, false).get());
  ASSERT_TRUE(idfObject.getString(RunPeriodFields::UseWeatherFileDaylightSavingPeriod, false));
  EXPECT_EQ("No", idfObject.getString(RunPeriodFields::UseWeatherFileDaylightSavingPeriod, false).get());
  ASSERT_TRUE(idfObject.getString(RunPeriodFields::ApplyWeekendHolidayRule, false));
  EXPECT_EQ("No", idfObject.getString(RunPeriodFields::ApplyWeekendHolidayRule, false).get());
  ASSERT_TRUE(idfObject.getString(RunPeriodFields::UseWeatherFileRainIndicators, false));
  EXPECT_EQ("Yes", idfObject.getString(RunPeriodFields::UseWeatherFileRainIndicators, false).get());
  ASSERT_TRUE(idfObject.getString(RunPeriodFields::UseWeatherFileSnowIndicators, false));
  EXPECT_EQ("Yes", idfObject.getString(RunPeriodFields::UseWeatherFileSnowIndicators, false).get());
}

TEST_F(EnergyPlusFixture, ForwardTranslator_RunPeriod2) {
  Model model;
  RunPeriod runPeriod = model.getUniqueModelObject<RunPeriod>();
  runPeriod.setEndMonth(2);
  runPeriod.setEndDayOfMonth(4);

  ForwardTranslator forwardTranslator;
  Workspace workspace = forwardTranslator.translateModel(model);

  ASSERT_EQ(1u, workspace.getObjectsByType(IddObjectType::RunPeriod).size());
  WorkspaceObject idfObject = workspace.getObjectsByType(IddObjectType::RunPeriod)[0];
  ASSERT_TRUE(idfObject.getString(RunPeriodFields::Name, false));
  EXPECT_EQ("Run Period 1", idfObject.getString(RunPeriodFields::Name, false).get());
  ASSERT_TRUE(idfObject.getInt(RunPeriodFields::BeginMonth, false));
  EXPECT_EQ(1, idfObject.getInt(RunPeriodFields::BeginMonth, false).get());
  ASSERT_TRUE(idfObject.getInt(RunPeriodFields::BeginDayofMonth, false));
  EXPECT_EQ(1, idfObject.getInt(RunPeriodFields::BeginDayofMonth, false).get());
  ASSERT_TRUE(idfObject.getInt(RunPeriodFields::BeginYear, false));
  EXPECT_EQ(2009, idfObject.getInt(RunPeriodFields::BeginYear, false).get());
  ASSERT_TRUE(idfObject.getInt(RunPeriodFields::EndMonth, false));
  EXPECT_EQ(2, idfObject.getInt(RunPeriodFields::EndMonth, false).get());
  ASSERT_TRUE(idfObject.getInt(RunPeriodFields::EndDayofMonth, false));
  EXPECT_EQ(4, idfObject.getInt(RunPeriodFields::EndDayofMonth, false).get());
  ASSERT_TRUE(idfObject.getInt(RunPeriodFields::EndYear, false));
  EXPECT_EQ(2009, idfObject.getInt(RunPeriodFields::EndYear, false).get());
  ASSERT_TRUE(idfObject.getString(RunPeriodFields::DayofWeekforStartDay, false));
  EXPECT_EQ("Thursday", idfObject.getString(RunPeriodFields::DayofWeekforStartDay, false).get());
  ASSERT_TRUE(idfObject.getString(RunPeriodFields::UseWeatherFileHolidaysandSpecialDays, false));
  EXPECT_EQ("No", idfObject.getString(RunPeriodFields::UseWeatherFileHolidaysandSpecialDays, false).get());
  ASSERT_TRUE(idfObject.getString(RunPeriodFields::UseWeatherFileDaylightSavingPeriod, false));
  EXPECT_EQ("No", idfObject.getString(RunPeriodFields::UseWeatherFileDaylightSavingPeriod, false).get());
  ASSERT_TRUE(idfObject.getString(RunPeriodFields::ApplyWeekendHolidayRule, false));
  EXPECT_EQ("No", idfObject.getString(RunPeriodFields::ApplyWeekendHolidayRule, false).get());
  ASSERT_TRUE(idfObject.getString(RunPeriodFields::UseWeatherFileRainIndicators, false));
  EXPECT_EQ("Yes", idfObject.getString(RunPeriodFields::UseWeatherFileRainIndicators, false).get());
  ASSERT_TRUE(idfObject.getString(RunPeriodFields::UseWeatherFileSnowIndicators, false));
  EXPECT_EQ("Yes", idfObject.getString(RunPeriodFields::UseWeatherFileSnowIndicators, false).get());
}

TEST_F(EnergyPlusFixture, ForwardTranslator_RunPeriod3) {
  Model model;
  RunPeriod runPeriod = model.getUniqueModelObject<RunPeriod>();
  runPeriod.setBeginMonth(2);
  runPeriod.setBeginDayOfMonth(4);
  runPeriod.setEndMonth(2);
  runPeriod.setEndDayOfMonth(4);

  ForwardTranslator forwardTranslator;
  Workspace workspace = forwardTranslator.translateModel(model);

  ASSERT_EQ(1u, workspace.getObjectsByType(IddObjectType::RunPeriod).size());
  WorkspaceObject idfObject = workspace.getObjectsByType(IddObjectType::RunPeriod)[0];
  ASSERT_TRUE(idfObject.getString(RunPeriodFields::Name, false));
  EXPECT_EQ("Run Period 1", idfObject.getString(RunPeriodFields::Name, false).get());
  ASSERT_TRUE(idfObject.getInt(RunPeriodFields::BeginMonth, false));
  EXPECT_EQ(2, idfObject.getInt(RunPeriodFields::BeginMonth, false).get());
  ASSERT_TRUE(idfObject.getInt(RunPeriodFields::BeginDayofMonth, false));
  EXPECT_EQ(4, idfObject.getInt(RunPeriodFields::BeginDayofMonth, false).get());
  ASSERT_TRUE(idfObject.getInt(RunPeriodFields::BeginYear, false));
  EXPECT_EQ(2009, idfObject.getInt(RunPeriodFields::BeginYear, false).get());
  ASSERT_TRUE(idfObject.getInt(RunPeriodFields::EndMonth, false));
  EXPECT_EQ(2, idfObject.getInt(RunPeriodFields::EndMonth, false).get());
  ASSERT_TRUE(idfObject.getInt(RunPeriodFields::EndDayofMonth, false));
  EXPECT_EQ(4, idfObject.getInt(RunPeriodFields::EndDayofMonth, false).get());
  ASSERT_TRUE(idfObject.getInt(RunPeriodFields::EndYear, false));
  EXPECT_EQ(2009, idfObject.getInt(RunPeriodFields::EndYear, false).get());
  ASSERT_TRUE(idfObject.getString(RunPeriodFields::DayofWeekforStartDay, false));
  EXPECT_EQ("Wednesday", idfObject.getString(RunPeriodFields::DayofWeekforStartDay, false).get());
  ASSERT_TRUE(idfObject.getString(RunPeriodFields::UseWeatherFileHolidaysandSpecialDays, false));
  EXPECT_EQ("No", idfObject.getString(RunPeriodFields::UseWeatherFileHolidaysandSpecialDays, false).get());
  ASSERT_TRUE(idfObject.getString(RunPeriodFields::UseWeatherFileDaylightSavingPeriod, false));
  EXPECT_EQ("No", idfObject.getString(RunPeriodFields::UseWeatherFileDaylightSavingPeriod, false).get());
  ASSERT_TRUE(idfObject.getString(RunPeriodFields::ApplyWeekendHolidayRule, false));
  EXPECT_EQ("No", idfObject.getString(RunPeriodFields::ApplyWeekendHolidayRule, false).get());
  ASSERT_TRUE(idfObject.getString(RunPeriodFields::UseWeatherFileRainIndicators, false));
  EXPECT_EQ("Yes", idfObject.getString(RunPeriodFields::UseWeatherFileRainIndicators, false).get());
  ASSERT_TRUE(idfObject.getString(RunPeriodFields::UseWeatherFileSnowIndicators, false));
  EXPECT_EQ("Yes", idfObject.getString(RunPeriodFields::UseWeatherFileSnowIndicators, false).get());
}

TEST_F(EnergyPlusFixture, ForwardTranslator_RunPeriod4) {
  Model model;
  RunPeriod runPeriod = model.getUniqueModelObject<RunPeriod>();
  runPeriod.setBeginMonth(2);
  runPeriod.setBeginDayOfMonth(4);
  runPeriod.setEndMonth(2);
  runPeriod.setEndDayOfMonth(11);

  ForwardTranslator forwardTranslator;
  Workspace workspace = forwardTranslator.translateModel(model);

  ASSERT_EQ(1u, workspace.getObjectsByType(IddObjectType::RunPeriod).size());
  WorkspaceObject idfObject = workspace.getObjectsByType(IddObjectType::RunPeriod)[0];
  ASSERT_TRUE(idfObject.getString(RunPeriodFields::Name, false));
  EXPECT_EQ("Run Period 1", idfObject.getString(RunPeriodFields::Name, false).get());
  ASSERT_TRUE(idfObject.getInt(RunPeriodFields::BeginMonth, false));
  EXPECT_EQ(2, idfObject.getInt(RunPeriodFields::BeginMonth, false).get());
  ASSERT_TRUE(idfObject.getInt(RunPeriodFields::BeginDayofMonth, false));
  EXPECT_EQ(4, idfObject.getInt(RunPeriodFields::BeginDayofMonth, false).get());
  ASSERT_TRUE(idfObject.getInt(RunPeriodFields::BeginYear, false));
  EXPECT_EQ(2009, idfObject.getInt(RunPeriodFields::BeginYear, false).get());
  ASSERT_TRUE(idfObject.getInt(RunPeriodFields::EndMonth, false));
  EXPECT_EQ(2, idfObject.getInt(RunPeriodFields::EndMonth, false).get());
  ASSERT_TRUE(idfObject.getInt(RunPeriodFields::EndDayofMonth, false));
  EXPECT_EQ(11, idfObject.getInt(RunPeriodFields::EndDayofMonth, false).get());
  ASSERT_TRUE(idfObject.getInt(RunPeriodFields::EndYear, false));
  EXPECT_EQ(2009, idfObject.getInt(RunPeriodFields::EndYear, false).get());
  ASSERT_TRUE(idfObject.getString(RunPeriodFields::DayofWeekforStartDay, false));
  EXPECT_EQ("Wednesday", idfObject.getString(RunPeriodFields::DayofWeekforStartDay, false).get());
  ASSERT_TRUE(idfObject.getString(RunPeriodFields::UseWeatherFileHolidaysandSpecialDays, false));
  EXPECT_EQ("No", idfObject.getString(RunPeriodFields::UseWeatherFileHolidaysandSpecialDays, false).get());
  ASSERT_TRUE(idfObject.getString(RunPeriodFields::UseWeatherFileDaylightSavingPeriod, false));
  EXPECT_EQ("No", idfObject.getString(RunPeriodFields::UseWeatherFileDaylightSavingPeriod, false).get());
  ASSERT_TRUE(idfObject.getString(RunPeriodFields::ApplyWeekendHolidayRule, false));
  EXPECT_EQ("No", idfObject.getString(RunPeriodFields::ApplyWeekendHolidayRule, false).get());
  ASSERT_TRUE(idfObject.getString(RunPeriodFields::UseWeatherFileRainIndicators, false));
  EXPECT_EQ("Yes", idfObject.getString(RunPeriodFields::UseWeatherFileRainIndicators, false).get());
  ASSERT_TRUE(idfObject.getString(RunPeriodFields::UseWeatherFileSnowIndicators, false));
  EXPECT_EQ("Yes", idfObject.getString(RunPeriodFields::UseWeatherFileSnowIndicators, false).get());
}

TEST_F(EnergyPlusFixture, ForwardTranslator_RunPeriod5) {
  Model model;
  RunPeriod runPeriod = model.getUniqueModelObject<RunPeriod>();
  runPeriod.setBeginMonth(2);
  runPeriod.setBeginDayOfMonth(4);
  runPeriod.setEndMonth(2);
  runPeriod.setEndDayOfMonth(1);

  ForwardTranslator forwardTranslator;
  Workspace workspace = forwardTranslator.translateModel(model);

  ASSERT_EQ(1u, workspace.getObjectsByType(IddObjectType::RunPeriod).size());
  WorkspaceObject idfObject = workspace.getObjectsByType(IddObjectType::RunPeriod)[0];
  ASSERT_TRUE(idfObject.getString(RunPeriodFields::Name, false));
  EXPECT_EQ("Run Period 1", idfObject.getString(RunPeriodFields::Name, false).get());
  ASSERT_TRUE(idfObject.getInt(RunPeriodFields::BeginMonth, false));
  EXPECT_EQ(2, idfObject.getInt(RunPeriodFields::BeginMonth, false).get());
  ASSERT_TRUE(idfObject.getInt(RunPeriodFields::BeginDayofMonth, false));
  EXPECT_EQ(4, idfObject.getInt(RunPeriodFields::BeginDayofMonth, false).get());
  ASSERT_TRUE(idfObject.getInt(RunPeriodFields::BeginYear, false));
  EXPECT_EQ(2009, idfObject.getInt(RunPeriodFields::BeginYear, false).get());
  ASSERT_TRUE(idfObject.getInt(RunPeriodFields::EndMonth, false));
  EXPECT_EQ(2, idfObject.getInt(RunPeriodFields::EndMonth, false).get());
  ASSERT_TRUE(idfObject.getInt(RunPeriodFields::EndDayofMonth, false));
  EXPECT_EQ(1, idfObject.getInt(RunPeriodFields::EndDayofMonth, false).get());
  ASSERT_TRUE(idfObject.getInt(RunPeriodFields::EndYear, false));
  EXPECT_EQ(2010, idfObject.getInt(RunPeriodFields::EndYear, false).get());
  ASSERT_TRUE(idfObject.getString(RunPeriodFields::DayofWeekforStartDay, false));
  EXPECT_EQ("Wednesday", idfObject.getString(RunPeriodFields::DayofWeekforStartDay, false).get());
  ASSERT_TRUE(idfObject.getString(RunPeriodFields::UseWeatherFileHolidaysandSpecialDays, false));
  EXPECT_EQ("No", idfObject.getString(RunPeriodFields::UseWeatherFileHolidaysandSpecialDays, false).get());
  ASSERT_TRUE(idfObject.getString(RunPeriodFields::UseWeatherFileDaylightSavingPeriod, false));
  EXPECT_EQ("No", idfObject.getString(RunPeriodFields::UseWeatherFileDaylightSavingPeriod, false).get());
  ASSERT_TRUE(idfObject.getString(RunPeriodFields::ApplyWeekendHolidayRule, false));
  EXPECT_EQ("No", idfObject.getString(RunPeriodFields::ApplyWeekendHolidayRule, false).get());
  ASSERT_TRUE(idfObject.getString(RunPeriodFields::UseWeatherFileRainIndicators, false));
  EXPECT_EQ("Yes", idfObject.getString(RunPeriodFields::UseWeatherFileRainIndicators, false).get());
  ASSERT_TRUE(idfObject.getString(RunPeriodFields::UseWeatherFileSnowIndicators, false));
  EXPECT_EQ("Yes", idfObject.getString(RunPeriodFields::UseWeatherFileSnowIndicators, false).get());
}
