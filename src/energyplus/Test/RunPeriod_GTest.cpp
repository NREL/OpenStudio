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