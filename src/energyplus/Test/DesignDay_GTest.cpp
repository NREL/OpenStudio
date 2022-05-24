/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2022, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

#include "../../model/Model.hpp"
#include "../../model/DesignDay.hpp"
#include "../../model/DesignDay_Impl.hpp"
#include "../../model/ScheduleDay.hpp"
#include "../../model/ScheduleDay_Impl.hpp"

#include <utilities/idd/SizingPeriod_DesignDay_FieldEnums.hxx>

#include <utilities/idd/IddEnums.hxx>
#include "../../utilities/idf/IdfObject.hpp"
#include "../../utilities/idf/IdfObject_Impl.hpp"

#include "../../utilities/idf/WorkspaceObject.hpp"
#include "../../utilities/idf/WorkspaceObject_Impl.hpp"

using namespace openstudio::energyplus;
using namespace openstudio::model;
using namespace openstudio;

TEST_F(EnergyPlusFixture, ForwardTranslator_DesignDay) {
  Model m;
  DesignDay designDay(m);

  ScheduleDay humidityConditionDaySchedule(m);
  humidityConditionDaySchedule.setName("Humidity Condition Day Schedule");

  ScheduleDay dryBulbTemperatureRangeModifierDaySchedule(m);
  dryBulbTemperatureRangeModifierDaySchedule.setName("Dry Bulb Temperature Range Modifier Day Schedule");

  ScheduleDay beamSolarDaySchedule(m);
  beamSolarDaySchedule.setName("Beam Solar Day Schedule");

  ScheduleDay diffuseSolarDaySchedule(m);
  diffuseSolarDaySchedule.setName("Diffuse Solar Day Schedule");

  EXPECT_TRUE(designDay.setMaximumDryBulbTemperature(1));
  EXPECT_TRUE(designDay.setDailyDryBulbTemperatureRange(2));
  EXPECT_TRUE(designDay.setBarometricPressure(100000));
  EXPECT_TRUE(designDay.setWindSpeed(4));
  EXPECT_TRUE(designDay.setWindDirection(5));
  EXPECT_TRUE(designDay.setSkyClearness(0.5));
  EXPECT_TRUE(designDay.setRainIndicator(true));
  EXPECT_TRUE(designDay.setSnowIndicator(true));
  EXPECT_TRUE(designDay.setDayOfMonth(7));
  EXPECT_TRUE(designDay.setMonth(8));
  EXPECT_TRUE(designDay.setDayType("Sunday"));
  EXPECT_TRUE(designDay.setDaylightSavingTimeIndicator(true));
  EXPECT_TRUE(designDay.setHumidityConditionType("WetBulbProfileDifferenceSchedule"));
  EXPECT_TRUE(designDay.setHumidityConditionDaySchedule(humidityConditionDaySchedule));
  EXPECT_TRUE(designDay.setHumidityIndicatingConditionsAtMaximumDryBulb(9));
  EXPECT_TRUE(designDay.setWetBulbOrDewPointAtMaximumDryBulb(9));
  EXPECT_TRUE(designDay.setHumidityRatioAtMaximumDryBulb(10));
  EXPECT_TRUE(designDay.setEnthalpyAtMaximumDryBulb(11));
  EXPECT_TRUE(designDay.setDryBulbTemperatureRangeModifierType("MultiplierSchedule"));
  EXPECT_TRUE(designDay.setDryBulbTemperatureRangeModifierDaySchedule(dryBulbTemperatureRangeModifierDaySchedule));
  EXPECT_TRUE(designDay.setSolarModelIndicator("Schedule"));
  EXPECT_TRUE(designDay.setBeamSolarDaySchedule(beamSolarDaySchedule));
  EXPECT_TRUE(designDay.setDiffuseSolarDaySchedule(diffuseSolarDaySchedule));
  EXPECT_TRUE(designDay.setAshraeClearSkyOpticalDepthForBeamIrradiance(0.6));
  EXPECT_TRUE(designDay.setAshraeClearSkyOpticalDepthForDiffuseIrradiance(0.7));
  EXPECT_TRUE(designDay.setDailyWetBulbTemperatureRange(14));
  EXPECT_TRUE(designDay.setMaximumNumberWarmupDays(15));
  EXPECT_TRUE(designDay.setBeginEnvironmentResetMode("SuppressAllBeginEnvironmentResets"));

  openstudio::energyplus::ForwardTranslator ft;
  Workspace w = ft.translateModel(m);

  EXPECT_EQ(0u, ft.errors().size());

  WorkspaceObjectVector idf_dds(w.getObjectsByType(IddObjectType::SizingPeriod_DesignDay));
  EXPECT_EQ(1u, idf_dds.size());
  WorkspaceObject idf_dd(idf_dds[0]);

  EXPECT_EQ(8, idf_dd.getInt(SizingPeriod_DesignDayFields::Month, false).get());
  EXPECT_EQ(7, idf_dd.getInt(SizingPeriod_DesignDayFields::DayofMonth, false).get());
  EXPECT_EQ("Sunday", idf_dd.getString(SizingPeriod_DesignDayFields::DayType, false).get());
  EXPECT_EQ(1, idf_dd.getDouble(SizingPeriod_DesignDayFields::MaximumDryBulbTemperature, false).get());
  EXPECT_EQ(2, idf_dd.getDouble(SizingPeriod_DesignDayFields::DailyDryBulbTemperatureRange, false).get());
  EXPECT_EQ("MultiplierSchedule", idf_dd.getString(SizingPeriod_DesignDayFields::DryBulbTemperatureRangeModifierType, false).get());
  boost::optional<WorkspaceObject> woDaySchedule1(idf_dd.getTarget(SizingPeriod_DesignDayFields::DryBulbTemperatureRangeModifierDayScheduleName));
  EXPECT_TRUE(woDaySchedule1);
  EXPECT_EQ("Dry Bulb Temperature Range Modifier Day Schedule", woDaySchedule1->nameString());
  EXPECT_EQ("WetBulbProfileDifferenceSchedule", idf_dd.getString(SizingPeriod_DesignDayFields::HumidityConditionType, false).get());
  EXPECT_EQ(9, idf_dd.getDouble(SizingPeriod_DesignDayFields::WetbulborDewPointatMaximumDryBulb, false).get());
  boost::optional<WorkspaceObject> woDaySchedule2(idf_dd.getTarget(SizingPeriod_DesignDayFields::HumidityConditionDayScheduleName));
  EXPECT_TRUE(woDaySchedule2);
  EXPECT_EQ("Humidity Condition Day Schedule", woDaySchedule2->nameString());
  EXPECT_EQ("", idf_dd.getString(SizingPeriod_DesignDayFields::HumidityRatioatMaximumDryBulb, false).get());
  EXPECT_EQ("", idf_dd.getString(SizingPeriod_DesignDayFields::EnthalpyatMaximumDryBulb, false).get());
  EXPECT_EQ("", idf_dd.getString(SizingPeriod_DesignDayFields::DailyWetBulbTemperatureRange, false).get());
  EXPECT_EQ(100000, idf_dd.getDouble(SizingPeriod_DesignDayFields::BarometricPressure, false).get());
  EXPECT_EQ(4, idf_dd.getDouble(SizingPeriod_DesignDayFields::WindSpeed, false).get());
  EXPECT_EQ(5, idf_dd.getDouble(SizingPeriod_DesignDayFields::WindDirection, false).get());
  EXPECT_EQ("Yes", idf_dd.getString(SizingPeriod_DesignDayFields::RainIndicator, false).get());
  EXPECT_EQ("Yes", idf_dd.getString(SizingPeriod_DesignDayFields::SnowIndicator, false).get());
  EXPECT_EQ("Yes", idf_dd.getString(SizingPeriod_DesignDayFields::DaylightSavingTimeIndicator, false).get());
  EXPECT_EQ("Schedule", idf_dd.getString(SizingPeriod_DesignDayFields::SolarModelIndicator, false).get());
  boost::optional<WorkspaceObject> woDaySchedule3(idf_dd.getTarget(SizingPeriod_DesignDayFields::BeamSolarDayScheduleName));
  EXPECT_TRUE(woDaySchedule3);
  EXPECT_EQ("Beam Solar Day Schedule", woDaySchedule3->nameString());
  boost::optional<WorkspaceObject> woDaySchedule4(idf_dd.getTarget(SizingPeriod_DesignDayFields::DiffuseSolarDayScheduleName));
  EXPECT_TRUE(woDaySchedule4);
  EXPECT_EQ("Diffuse Solar Day Schedule", woDaySchedule4->nameString());
  EXPECT_EQ("", idf_dd.getString(SizingPeriod_DesignDayFields::ASHRAEClearSkyOpticalDepthforBeamIrradiance_taub_, false).get());
  EXPECT_EQ("", idf_dd.getString(SizingPeriod_DesignDayFields::ASHRAEClearSkyOpticalDepthforDiffuseIrradiance_taud_, false).get());
  EXPECT_EQ("", idf_dd.getString(SizingPeriod_DesignDayFields::SkyClearness, false).get());
  EXPECT_EQ(15, idf_dd.getInt(SizingPeriod_DesignDayFields::MaximumNumberWarmupDays, false).get());
  EXPECT_EQ("SuppressAllBeginEnvironmentResets", idf_dd.getString(SizingPeriod_DesignDayFields::BeginEnvironmentResetMode, false).get());
}

TEST_F(EnergyPlusFixture, ReverseTranslator_DesignDay) {
  openstudio::Workspace workspace(openstudio::StrictnessLevel::Minimal, openstudio::IddFileType::EnergyPlus);

  openstudio::IdfObject idfObject(openstudio::IddObjectType::SizingPeriod_DesignDay);
  idfObject.setString(SizingPeriod_DesignDayFields::Name, "Design Day");

  openstudio::WorkspaceObject epDD = workspace.addObject(idfObject).get();

  ReverseTranslator trans;
  ASSERT_NO_THROW(trans.translateWorkspace(workspace));
  Model model = trans.translateWorkspace(workspace);

  std::vector<DesignDay> designDays = model.getConcreteModelObjects<DesignDay>();
  ASSERT_EQ(1u, designDays.size());
  DesignDay designDay = designDays[0];
  EXPECT_EQ("Design Day", designDay.name().get());
  EXPECT_EQ(23, designDay.maximumDryBulbTemperature());
  EXPECT_EQ(0, designDay.dailyDryBulbTemperatureRange());
  EXPECT_EQ(31000, designDay.barometricPressure());
  EXPECT_EQ(0, designDay.windSpeed());
  EXPECT_EQ(0, designDay.windDirection());
  EXPECT_EQ(0, designDay.skyClearness());
  EXPECT_FALSE(designDay.rainIndicator());
  EXPECT_FALSE(designDay.snowIndicator());
  EXPECT_EQ(1, designDay.dayOfMonth());
  EXPECT_EQ(1, designDay.month());
  EXPECT_EQ("SummerDesignDay", designDay.dayType());
  EXPECT_FALSE(designDay.daylightSavingTimeIndicator());
  EXPECT_EQ("WetBulb", designDay.humidityConditionType());
  EXPECT_FALSE(designDay.humidityConditionDaySchedule());
  EXPECT_TRUE(designDay.wetBulbOrDewPointAtMaximumDryBulb());
  EXPECT_EQ(23, designDay.wetBulbOrDewPointAtMaximumDryBulb().get());
  EXPECT_FALSE(designDay.humidityRatioAtMaximumDryBulb());
  EXPECT_FALSE(designDay.enthalpyAtMaximumDryBulb());
  EXPECT_FALSE(designDay.dryBulbTemperatureRangeModifierDaySchedule());
  EXPECT_EQ("ASHRAEClearSky", designDay.solarModelIndicator());
  EXPECT_FALSE(designDay.beamSolarDaySchedule());
  EXPECT_FALSE(designDay.diffuseSolarDaySchedule());
  EXPECT_EQ(0, designDay.ashraeClearSkyOpticalDepthForBeamIrradiance());
  EXPECT_EQ(0, designDay.ashraeClearSkyOpticalDepthForDiffuseIrradiance());
  EXPECT_FALSE(designDay.dailyWetBulbTemperatureRange());
  EXPECT_FALSE(designDay.maximumNumberWarmupDays());
  EXPECT_EQ("FullResetAtBeginEnvironment", designDay.beginEnvironmentResetMode());
}
