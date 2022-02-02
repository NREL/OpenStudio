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
#include <string>

#include "ModelFixture.hpp"

#include "../Model.hpp"
#include "../Model_Impl.hpp"

#include "../DesignDay.hpp"
#include "../DesignDay_Impl.hpp"
#include "../ScheduleDay.hpp"
#include "../ScheduleDay_Impl.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, DesignDay_DesignDay) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  ASSERT_EXIT(
    {
      Model m;
      DesignDay designDay(m);

      exit(0);
    },
    ::testing::ExitedWithCode(0), "");

  Model m;
  DesignDay designDay(m);

  EXPECT_EQ(23, designDay.maximumDryBulbTemperature());
  EXPECT_TRUE(designDay.isMaximumDryBulbTemperatureDefaulted());
  EXPECT_EQ(0, designDay.dailyDryBulbTemperatureRange());
  EXPECT_TRUE(designDay.isDailyDryBulbTemperatureRangeDefaulted());
  EXPECT_EQ(31000, designDay.barometricPressure());
  EXPECT_TRUE(designDay.isBarometricPressureDefaulted());
  EXPECT_EQ(0, designDay.windSpeed());
  EXPECT_TRUE(designDay.isWindSpeedDefaulted());
  EXPECT_EQ(0, designDay.windDirection());
  EXPECT_TRUE(designDay.isWindDirectionDefaulted());
  EXPECT_EQ(0, designDay.skyClearness());
  EXPECT_TRUE(designDay.isSkyClearnessDefaulted());
  EXPECT_FALSE(designDay.rainIndicator());
  EXPECT_TRUE(designDay.isRainIndicatorDefaulted());
  EXPECT_FALSE(designDay.snowIndicator());
  EXPECT_TRUE(designDay.isSnowIndicatorDefaulted());
  EXPECT_EQ(1, designDay.dayOfMonth());
  EXPECT_TRUE(designDay.isDayOfMonthDefaulted());
  EXPECT_EQ(1, designDay.month());
  EXPECT_TRUE(designDay.isMonthDefaulted());
  EXPECT_EQ("SummerDesignDay", designDay.dayType());
  EXPECT_TRUE(designDay.isDayTypeDefaulted());
  EXPECT_FALSE(designDay.daylightSavingTimeIndicator());
  EXPECT_TRUE(designDay.isDaylightSavingTimeIndicatorDefaulted());
  EXPECT_EQ("WetBulb", designDay.humidityConditionType());
  EXPECT_TRUE(designDay.isHumidityConditionTypeDefaulted());
  EXPECT_FALSE(designDay.humidityConditionDaySchedule());
  ASSERT_TRUE(designDay.wetBulbOrDewPointAtMaximumDryBulb());
  EXPECT_EQ(23, designDay.wetBulbOrDewPointAtMaximumDryBulb().get());
  EXPECT_FALSE(designDay.humidityRatioAtMaximumDryBulb());
  EXPECT_FALSE(designDay.enthalpyAtMaximumDryBulb());
  EXPECT_EQ("DefaultMultipliers", designDay.dryBulbTemperatureRangeModifierType());
  EXPECT_TRUE(designDay.isDryBulbTemperatureRangeModifierTypeDefaulted());
  EXPECT_FALSE(designDay.dryBulbTemperatureRangeModifierDaySchedule());
  EXPECT_EQ("ASHRAEClearSky", designDay.solarModelIndicator());
  EXPECT_TRUE(designDay.isSolarModelIndicatorDefaulted());
  EXPECT_FALSE(designDay.beamSolarDaySchedule());
  EXPECT_FALSE(designDay.diffuseSolarDaySchedule());
  EXPECT_EQ(0, designDay.ashraeClearSkyOpticalDepthForBeamIrradiance());
  EXPECT_TRUE(designDay.isAshraeClearSkyOpticalDepthForBeamIrradianceDefaulted());
  EXPECT_EQ(0, designDay.ashraeClearSkyOpticalDepthForDiffuseIrradiance());
  EXPECT_TRUE(designDay.isAshraeClearSkyOpticalDepthForDiffuseIrradianceDefaulted());
  EXPECT_FALSE(designDay.dailyWetBulbTemperatureRange());
  EXPECT_FALSE(designDay.maximumNumberWarmupDays());
  EXPECT_EQ("FullResetAtBeginEnvironment", designDay.beginEnvironmentResetMode());
  EXPECT_TRUE(designDay.isBeginEnvironmentResetModeDefaulted());

  // DEPRECATED
  EXPECT_EQ(23, designDay.humidityIndicatingConditionsAtMaximumDryBulb());
  EXPECT_FALSE(designDay.isHumidityIndicatingConditionsAtMaximumDryBulbDefaulted());
  EXPECT_EQ("WetBulb", designDay.humidityIndicatingType());
  EXPECT_TRUE(designDay.isHumidityIndicatingTypeDefaulted());
  EXPECT_FALSE(designDay.humidityIndicatingDaySchedule());
  EXPECT_FALSE(designDay.dryBulbTemperatureRangeModifierSchedule());
  EXPECT_EQ(0, designDay.ashraeTaub());
  EXPECT_TRUE(designDay.isAshraeTaubDefaulted());
  EXPECT_EQ(0, designDay.ashraeTaud());
  EXPECT_TRUE(designDay.isAshraeTaudDefaulted());
}

// test setting and getting
TEST_F(ModelFixture, DesignDay_SetGetFields) {
  Model m;
  DesignDay designDay(m);
  ScheduleDay daySchedule(m);

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
  EXPECT_TRUE(designDay.setHumidityConditionType("DewPoint"));
  EXPECT_TRUE(designDay.setHumidityConditionDaySchedule(daySchedule));
  EXPECT_TRUE(designDay.setWetBulbOrDewPointAtMaximumDryBulb(9));
  EXPECT_TRUE(designDay.setHumidityRatioAtMaximumDryBulb(10));
  EXPECT_TRUE(designDay.setEnthalpyAtMaximumDryBulb(11));
  EXPECT_TRUE(designDay.setDryBulbTemperatureRangeModifierType("MultiplierSchedule"));
  EXPECT_TRUE(designDay.setDryBulbTemperatureRangeModifierDaySchedule(daySchedule));
  EXPECT_TRUE(designDay.setSolarModelIndicator("ZhangHuang"));
  EXPECT_TRUE(designDay.setBeamSolarDaySchedule(daySchedule));
  EXPECT_TRUE(designDay.setDiffuseSolarDaySchedule(daySchedule));
  EXPECT_TRUE(designDay.setAshraeClearSkyOpticalDepthForBeamIrradiance(0.6));
  EXPECT_TRUE(designDay.setAshraeClearSkyOpticalDepthForDiffuseIrradiance(0.7));
  EXPECT_TRUE(designDay.setDailyWetBulbTemperatureRange(14));
  EXPECT_TRUE(designDay.setMaximumNumberWarmupDays(15));
  EXPECT_TRUE(designDay.setBeginEnvironmentResetMode("SuppressAllBeginEnvironmentResets"));

  EXPECT_EQ(1, designDay.maximumDryBulbTemperature());
  EXPECT_FALSE(designDay.isMaximumDryBulbTemperatureDefaulted());
  EXPECT_EQ(2, designDay.dailyDryBulbTemperatureRange());
  EXPECT_FALSE(designDay.isDailyDryBulbTemperatureRangeDefaulted());
  EXPECT_EQ(100000, designDay.barometricPressure());
  EXPECT_FALSE(designDay.isBarometricPressureDefaulted());
  EXPECT_EQ(4, designDay.windSpeed());
  EXPECT_FALSE(designDay.isWindSpeedDefaulted());
  EXPECT_EQ(5, designDay.windDirection());
  EXPECT_FALSE(designDay.isWindDirectionDefaulted());
  EXPECT_EQ(0.5, designDay.skyClearness());
  EXPECT_FALSE(designDay.isSkyClearnessDefaulted());
  EXPECT_TRUE(designDay.rainIndicator());
  EXPECT_FALSE(designDay.isRainIndicatorDefaulted());
  EXPECT_TRUE(designDay.snowIndicator());
  EXPECT_FALSE(designDay.isSnowIndicatorDefaulted());
  EXPECT_EQ(7, designDay.dayOfMonth());
  EXPECT_FALSE(designDay.isDayOfMonthDefaulted());
  EXPECT_EQ(8, designDay.month());
  EXPECT_FALSE(designDay.isMonthDefaulted());
  EXPECT_EQ("Sunday", designDay.dayType());
  EXPECT_FALSE(designDay.isDayTypeDefaulted());
  EXPECT_TRUE(designDay.daylightSavingTimeIndicator());
  EXPECT_FALSE(designDay.isDaylightSavingTimeIndicatorDefaulted());
  EXPECT_EQ("DewPoint", designDay.humidityIndicatingType());
  EXPECT_EQ("DewPoint", designDay.humidityConditionType());
  EXPECT_FALSE(designDay.isHumidityConditionTypeDefaulted());
  EXPECT_TRUE(designDay.humidityConditionDaySchedule());
  ASSERT_TRUE(designDay.wetBulbOrDewPointAtMaximumDryBulb());
  EXPECT_EQ(9, designDay.wetBulbOrDewPointAtMaximumDryBulb().get());
  ASSERT_TRUE(designDay.humidityRatioAtMaximumDryBulb());
  EXPECT_EQ(10, designDay.humidityRatioAtMaximumDryBulb().get());
  ASSERT_TRUE(designDay.enthalpyAtMaximumDryBulb());
  EXPECT_EQ(11, designDay.enthalpyAtMaximumDryBulb().get());
  EXPECT_EQ("MultiplierSchedule", designDay.dryBulbTemperatureRangeModifierType());
  EXPECT_FALSE(designDay.isDryBulbTemperatureRangeModifierTypeDefaulted());
  EXPECT_TRUE(designDay.dryBulbTemperatureRangeModifierDaySchedule());
  EXPECT_EQ("ZhangHuang", designDay.solarModelIndicator());
  EXPECT_FALSE(designDay.isSolarModelIndicatorDefaulted());
  EXPECT_TRUE(designDay.beamSolarDaySchedule());
  EXPECT_TRUE(designDay.diffuseSolarDaySchedule());
  EXPECT_EQ(0.6, designDay.ashraeClearSkyOpticalDepthForBeamIrradiance());
  EXPECT_FALSE(designDay.isAshraeClearSkyOpticalDepthForBeamIrradianceDefaulted());
  EXPECT_EQ(0.7, designDay.ashraeClearSkyOpticalDepthForDiffuseIrradiance());
  EXPECT_FALSE(designDay.isAshraeClearSkyOpticalDepthForDiffuseIrradianceDefaulted());
  ASSERT_TRUE(designDay.dailyWetBulbTemperatureRange());
  EXPECT_EQ(14, designDay.dailyWetBulbTemperatureRange().get());
  ASSERT_TRUE(designDay.maximumNumberWarmupDays());
  EXPECT_EQ(15, designDay.maximumNumberWarmupDays().get());
  EXPECT_EQ("SuppressAllBeginEnvironmentResets", designDay.beginEnvironmentResetMode());
  EXPECT_FALSE(designDay.isBeginEnvironmentResetModeDefaulted());

  designDay.resetMaximumDryBulbTemperature();
  designDay.resetDailyDryBulbTemperatureRange();
  designDay.resetBarometricPressure();
  designDay.resetWindSpeed();
  designDay.resetWindDirection();
  designDay.resetSkyClearness();
  designDay.resetRainIndicator();
  designDay.resetSnowIndicator();
  designDay.resetDayOfMonth();
  designDay.resetMonth();
  designDay.resetDayType();
  designDay.resetDaylightSavingTimeIndicator();
  designDay.resetHumidityConditionType();
  designDay.resetHumidityConditionDaySchedule();
  designDay.resetWetBulbOrDewPointAtMaximumDryBulb();
  designDay.resetHumidityRatioAtMaximumDryBulb();
  designDay.resetEnthalpyAtMaximumDryBulb();
  designDay.resetDryBulbTemperatureRangeModifierType();
  designDay.resetDryBulbTemperatureRangeModifierDaySchedule();
  designDay.resetSolarModelIndicator();
  designDay.resetBeamSolarDaySchedule();
  designDay.resetDiffuseSolarDaySchedule();
  designDay.resetAshraeClearSkyOpticalDepthForBeamIrradiance();
  designDay.resetAshraeClearSkyOpticalDepthForDiffuseIrradiance();
  designDay.resetDailyWetBulbTemperatureRange();
  designDay.resetMaximumNumberWarmupDays();
  designDay.resetBeginEnvironmentResetMode();

  EXPECT_EQ(23, designDay.maximumDryBulbTemperature());
  EXPECT_TRUE(designDay.isMaximumDryBulbTemperatureDefaulted());
  EXPECT_EQ(0, designDay.dailyDryBulbTemperatureRange());
  EXPECT_TRUE(designDay.isDailyDryBulbTemperatureRangeDefaulted());
  EXPECT_EQ(31000, designDay.barometricPressure());
  EXPECT_TRUE(designDay.isBarometricPressureDefaulted());
  EXPECT_EQ(0, designDay.windSpeed());
  EXPECT_TRUE(designDay.isWindSpeedDefaulted());
  EXPECT_EQ(0, designDay.windDirection());
  EXPECT_TRUE(designDay.isWindDirectionDefaulted());
  EXPECT_EQ(0, designDay.skyClearness());
  EXPECT_TRUE(designDay.isSkyClearnessDefaulted());
  EXPECT_FALSE(designDay.rainIndicator());
  EXPECT_TRUE(designDay.isRainIndicatorDefaulted());
  EXPECT_FALSE(designDay.snowIndicator());
  EXPECT_TRUE(designDay.isSnowIndicatorDefaulted());
  EXPECT_EQ(1, designDay.dayOfMonth());
  EXPECT_TRUE(designDay.isDayOfMonthDefaulted());
  EXPECT_EQ(1, designDay.month());
  EXPECT_TRUE(designDay.isMonthDefaulted());
  EXPECT_EQ("SummerDesignDay", designDay.dayType());
  EXPECT_TRUE(designDay.isDayTypeDefaulted());
  EXPECT_FALSE(designDay.daylightSavingTimeIndicator());
  EXPECT_TRUE(designDay.isDaylightSavingTimeIndicatorDefaulted());
  EXPECT_EQ("WetBulb", designDay.humidityConditionType());
  EXPECT_TRUE(designDay.isHumidityConditionTypeDefaulted());
  EXPECT_FALSE(designDay.humidityConditionDaySchedule());
  EXPECT_FALSE(designDay.wetBulbOrDewPointAtMaximumDryBulb());
  EXPECT_FALSE(designDay.humidityRatioAtMaximumDryBulb());
  EXPECT_FALSE(designDay.enthalpyAtMaximumDryBulb());
  EXPECT_EQ("DefaultMultipliers", designDay.dryBulbTemperatureRangeModifierType());
  EXPECT_TRUE(designDay.isDryBulbTemperatureRangeModifierTypeDefaulted());
  EXPECT_FALSE(designDay.dryBulbTemperatureRangeModifierDaySchedule());
  EXPECT_EQ("ASHRAEClearSky", designDay.solarModelIndicator());
  EXPECT_TRUE(designDay.isSolarModelIndicatorDefaulted());
  EXPECT_FALSE(designDay.beamSolarDaySchedule());
  EXPECT_FALSE(designDay.diffuseSolarDaySchedule());
  EXPECT_EQ(0, designDay.ashraeClearSkyOpticalDepthForBeamIrradiance());
  EXPECT_TRUE(designDay.isAshraeClearSkyOpticalDepthForBeamIrradianceDefaulted());
  EXPECT_EQ(0, designDay.ashraeClearSkyOpticalDepthForDiffuseIrradiance());
  EXPECT_TRUE(designDay.isAshraeClearSkyOpticalDepthForDiffuseIrradianceDefaulted());
  EXPECT_FALSE(designDay.dailyWetBulbTemperatureRange());
  EXPECT_FALSE(designDay.maximumNumberWarmupDays());
  EXPECT_EQ("FullResetAtBeginEnvironment", designDay.beginEnvironmentResetMode());
  EXPECT_TRUE(designDay.isBeginEnvironmentResetModeDefaulted());

  // Deprecated
  EXPECT_TRUE(designDay.setHumidityIndicatingType("DewPoint"));
  EXPECT_TRUE(designDay.setHumidityIndicatingDaySchedule(daySchedule));
  EXPECT_TRUE(designDay.setHumidityIndicatingConditionsAtMaximumDryBulb(9));
  EXPECT_TRUE(designDay.setDryBulbTemperatureRangeModifierSchedule(daySchedule));
  EXPECT_TRUE(designDay.setAshraeTaub(0.6));
  EXPECT_TRUE(designDay.setAshraeTaud(0.7));

  EXPECT_EQ(9, designDay.humidityIndicatingConditionsAtMaximumDryBulb());
  EXPECT_FALSE(designDay.isHumidityIndicatingConditionsAtMaximumDryBulbDefaulted());
  EXPECT_EQ("DewPoint", designDay.humidityIndicatingType());
  EXPECT_FALSE(designDay.isHumidityIndicatingTypeDefaulted());
  EXPECT_TRUE(designDay.humidityIndicatingDaySchedule());
  EXPECT_TRUE(designDay.dryBulbTemperatureRangeModifierSchedule());
  EXPECT_EQ(0.6, designDay.ashraeTaub());
  EXPECT_FALSE(designDay.isAshraeTaubDefaulted());
  EXPECT_EQ(0.7, designDay.ashraeTaud());
  EXPECT_FALSE(designDay.isAshraeTaudDefaulted());

  designDay.resetHumidityIndicatingType();
  designDay.resetHumidityIndicatingDaySchedule();
  designDay.resetHumidityIndicatingConditionsAtMaximumDryBulb();
  designDay.resetDryBulbTemperatureRangeModifierSchedule();
  designDay.resetAshraeTaub();
  designDay.resetAshraeTaud();

  /* EXPECT_EQ("", designDay.humidityIndicatingConditionsAtMaximumDryBulb()); */
  EXPECT_TRUE(designDay.isHumidityIndicatingConditionsAtMaximumDryBulbDefaulted());
  EXPECT_EQ("WetBulb", designDay.humidityIndicatingType());
  EXPECT_TRUE(designDay.isHumidityIndicatingTypeDefaulted());
  EXPECT_FALSE(designDay.humidityIndicatingDaySchedule());
  EXPECT_FALSE(designDay.dryBulbTemperatureRangeModifierSchedule());
  EXPECT_EQ(0, designDay.ashraeTaub());
  EXPECT_TRUE(designDay.isAshraeTaubDefaulted());
  EXPECT_EQ(0, designDay.ashraeTaud());
  EXPECT_TRUE(designDay.isAshraeTaudDefaulted());
}

// test cloning it
TEST_F(ModelFixture, DesignDay_Clone) {
  Model m;
  DesignDay designDay(m);

  designDay.setSnowIndicator(true);

  auto designDayClone = designDay.clone(m).cast<DesignDay>();
  EXPECT_TRUE(designDayClone.snowIndicator());

  Model m2;
  auto designDayClone2 = designDay.clone(m2).cast<DesignDay>();
  EXPECT_TRUE(designDay.snowIndicator());
}

// check that remove works
TEST_F(ModelFixture, DesignDay_Remove) {
  Model m;
  auto size = m.modelObjects().size();
  DesignDay designDay(m);
  EXPECT_EQ(size + 1, m.modelObjects().size());
  EXPECT_FALSE(designDay.remove().empty());
  EXPECT_EQ(size, m.modelObjects().size());
}
