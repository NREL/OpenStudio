/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>

#include "ModelFixture.hpp"

#include "../ZoneVentilationWindandStackOpenArea.hpp"
#include "../ZoneVentilationWindandStackOpenArea_Impl.hpp"
#include "../ScheduleConstant.hpp"
#include "../Schedule.hpp"
#include "../ThermalZone.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, ZoneVentilationWindandStackOpenArea) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  ASSERT_EXIT(
    {
      Model m;
      ZoneVentilationWindandStackOpenArea zv(m);

      exit(0);
    },
    ::testing::ExitedWithCode(0), "");
}

TEST_F(ModelFixture, ZoneVentilationWindandStackOpenArea_GettersSetters) {

  Model m;
  ZoneVentilationWindandStackOpenArea zv(m);

  zv.setName("My ZoneVentilationWindandStackOpenArea");

  // Opening Area: Required Double
  EXPECT_TRUE(zv.setOpeningArea(15));
  EXPECT_EQ(15, zv.openingArea());
  EXPECT_FALSE(zv.setOpeningArea(-0.1));
  EXPECT_EQ(15, zv.openingArea());

  // Opening Area Fraction Schedule Name: Required
  ScheduleConstant openingAreaSch(m);
  openingAreaSch.setValue(1.0);
  EXPECT_TRUE(zv.setOpeningAreaFractionSchedule(openingAreaSch));
  EXPECT_EQ(openingAreaSch, zv.openingAreaFractionSchedule());

  // Opening Effectiveness: Required Double
  zv.autocalculateOpeningEffectiveness();
  EXPECT_TRUE(zv.isOpeningEffectivenessAutocalculated());
  EXPECT_TRUE(zv.setOpeningEffectiveness(0.5));
  ASSERT_TRUE(zv.openingEffectiveness());
  EXPECT_EQ(0.5, zv.openingEffectiveness().get());
  EXPECT_FALSE(zv.isOpeningEffectivenessAutocalculated());
  EXPECT_FALSE(zv.setOpeningEffectiveness(1.5));
  EXPECT_EQ(0.5, zv.openingEffectiveness().get());
  EXPECT_FALSE(zv.setOpeningEffectiveness(-0.1));
  EXPECT_EQ(0.5, zv.openingEffectiveness().get());

  // Effective Angle: Required Double
  EXPECT_TRUE(zv.setEffectiveAngle(180.0));
  EXPECT_EQ(180.0, zv.effectiveAngle());
  EXPECT_FALSE(zv.setEffectiveAngle(365));
  EXPECT_EQ(180.0, zv.effectiveAngle());

  // Height Difference: Required Double
  EXPECT_TRUE(zv.setHeightDifference(0.1));
  EXPECT_EQ(0.1, zv.heightDifference());
  EXPECT_FALSE(zv.setHeightDifference(-0.5));
  EXPECT_EQ(0.1, zv.heightDifference());

  // Discharge Coefficient for Opening: Required Double
  zv.autocalculateDischargeCoefficientforOpening();
  EXPECT_TRUE(zv.isDischargeCoefficientforOpeningAutocalculated());
  EXPECT_TRUE(zv.setDischargeCoefficientforOpening(0.5));
  ASSERT_TRUE(zv.dischargeCoefficientforOpening());
  EXPECT_EQ(0.5, zv.dischargeCoefficientforOpening().get());
  EXPECT_FALSE(zv.isDischargeCoefficientforOpeningAutocalculated());
  EXPECT_FALSE(zv.setDischargeCoefficientforOpening(1.5));
  EXPECT_EQ(0.5, zv.dischargeCoefficientforOpening().get());
  EXPECT_FALSE(zv.setDischargeCoefficientforOpening(-0.1));
  EXPECT_EQ(0.5, zv.dischargeCoefficientforOpening().get());

  // Minimum Indoor Temperature: Required Double
  EXPECT_TRUE(zv.setMinimumIndoorTemperature(0.0));
  EXPECT_EQ(0.0, zv.minimumIndoorTemperature());
  EXPECT_FALSE(zv.setMinimumIndoorTemperature(-105.0));
  EXPECT_FALSE(zv.setMinimumIndoorTemperature(105.0));
  EXPECT_EQ(0.0, zv.minimumIndoorTemperature());

  // Minimum Indoor Temperature Schedule Name: Optional Object
  ScheduleConstant minIndoorTempSch(m);
  minIndoorTempSch.setValue(10.0);
  EXPECT_TRUE(zv.setMinimumIndoorTemperatureSchedule(minIndoorTempSch));
  ASSERT_TRUE(zv.minimumIndoorTemperatureSchedule());
  EXPECT_EQ(minIndoorTempSch, zv.minimumIndoorTemperatureSchedule().get());
  zv.resetMinimumIndoorTemperatureSchedule();
  EXPECT_FALSE(zv.minimumIndoorTemperatureSchedule());

  // Maximum Indoor Temperature: Required Double
  EXPECT_TRUE(zv.setMaximumIndoorTemperature(40.0));
  EXPECT_EQ(40.0, zv.maximumIndoorTemperature());
  EXPECT_FALSE(zv.setMaximumIndoorTemperature(-105.0));
  EXPECT_FALSE(zv.setMaximumIndoorTemperature(105.0));
  EXPECT_EQ(40.0, zv.maximumIndoorTemperature());

  // Maximum Indoor Temperature Schedule Name: Optional Object
  ScheduleConstant maxIndoorTempSch(m);
  maxIndoorTempSch.setValue(30.0);
  EXPECT_TRUE(zv.setMaximumIndoorTemperatureSchedule(maxIndoorTempSch));
  ASSERT_TRUE(zv.maximumIndoorTemperatureSchedule());
  EXPECT_EQ(maxIndoorTempSch, zv.maximumIndoorTemperatureSchedule().get());
  zv.resetMaximumIndoorTemperatureSchedule();
  EXPECT_FALSE(zv.maximumIndoorTemperatureSchedule());

  // Delta Temperature: Required Double
  EXPECT_TRUE(zv.setDeltaTemperature(-2.0));
  EXPECT_EQ(-2.0, zv.deltaTemperature());
  EXPECT_FALSE(zv.setDeltaTemperature(-105));

  // Delta Temperature Schedule Name: Optional Object
  ScheduleConstant deltaTempSch(m);
  deltaTempSch.setValue(3.0);
  EXPECT_TRUE(zv.setDeltaTemperatureSchedule(deltaTempSch));
  ASSERT_TRUE(zv.deltaTemperatureSchedule());
  EXPECT_EQ(deltaTempSch, zv.deltaTemperatureSchedule().get());
  zv.resetDeltaTemperatureSchedule();
  EXPECT_FALSE(zv.deltaTemperatureSchedule());

  // Minimum Outdoor Temperature: Required Double
  EXPECT_TRUE(zv.setMinimumOutdoorTemperature(-20.0));
  EXPECT_EQ(-20.0, zv.minimumOutdoorTemperature());
  EXPECT_FALSE(zv.setMinimumOutdoorTemperature(-105.0));
  EXPECT_FALSE(zv.setMinimumOutdoorTemperature(105.0));
  EXPECT_EQ(-20.0, zv.minimumOutdoorTemperature());

  // Minimum Outdoor Temperature Schedule Name: Optional Object
  ScheduleConstant minOutdoorTempSch(m);
  minOutdoorTempSch.setValue(-10.0);
  EXPECT_TRUE(zv.setMinimumOutdoorTemperatureSchedule(minOutdoorTempSch));
  ASSERT_TRUE(zv.minimumOutdoorTemperatureSchedule());
  EXPECT_EQ(minOutdoorTempSch, zv.minimumOutdoorTemperatureSchedule().get());
  zv.resetMinimumOutdoorTemperatureSchedule();
  EXPECT_FALSE(zv.minimumOutdoorTemperatureSchedule());

  // Maximum Outdoor Temperature: Required Double
  EXPECT_TRUE(zv.setMaximumOutdoorTemperature(50.0));
  EXPECT_EQ(50.0, zv.maximumOutdoorTemperature());
  EXPECT_FALSE(zv.setMaximumOutdoorTemperature(-105.0));
  EXPECT_FALSE(zv.setMaximumOutdoorTemperature(105.0));
  EXPECT_EQ(50.0, zv.maximumOutdoorTemperature());

  // Maximum Outdoor Temperature Schedule Name: Optional Object
  ScheduleConstant maxOutdoorTempSch(m);
  maxOutdoorTempSch.setValue(20.0);
  EXPECT_TRUE(zv.setMaximumOutdoorTemperatureSchedule(maxOutdoorTempSch));
  ASSERT_TRUE(zv.maximumOutdoorTemperatureSchedule());
  EXPECT_EQ(maxOutdoorTempSch, zv.maximumOutdoorTemperatureSchedule().get());
  zv.resetMaximumOutdoorTemperatureSchedule();
  EXPECT_FALSE(zv.maximumOutdoorTemperatureSchedule());

  // Maximum Wind Speed: Required Double
  EXPECT_TRUE(zv.setMaximumWindSpeed(20.0));
  EXPECT_EQ(20.0, zv.maximumWindSpeed());
  EXPECT_FALSE(zv.setMaximumWindSpeed(-2.0));
  EXPECT_FALSE(zv.setMaximumWindSpeed(42.0));
  EXPECT_EQ(20.0, zv.maximumWindSpeed());

}  // End of Getter_Setters test

TEST_F(ModelFixture, ZoneVentilationWindandStackOpenArea_AddToThermalZone) {

  Model m;
  ThermalZone z(m);
  ZoneVentilationWindandStackOpenArea zv(m);
  EXPECT_TRUE(zv.addToThermalZone(z));
  ASSERT_TRUE(zv.thermalZone());
  EXPECT_EQ(z, zv.thermalZone().get());

  zv.removeFromThermalZone();
  EXPECT_FALSE(zv.thermalZone());
}
