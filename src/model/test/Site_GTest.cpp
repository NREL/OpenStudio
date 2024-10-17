/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>

#include "ModelFixture.hpp"
#include "../Site.hpp"
#include "../Site_Impl.hpp"

#include "../Model_Impl.hpp"
#include "../DesignDay.hpp"
#include "../DesignDay_Impl.hpp"
#include "../SkyTemperature.hpp"
#include "../SkyTemperature_Impl.hpp"
#include "../WeatherFile.hpp"
#include "../WeatherFile_Impl.hpp"
#include "../WeatherFileConditionType.hpp"
#include "../WeatherFileConditionType_Impl.hpp"
#include "../WeatherFileDays.hpp"
#include "../WeatherFileDays_Impl.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, Site_Outputvariables) {
  Model model;
  EXPECT_FALSE(model.getOptionalUniqueModelObject<Site>());
  Site site = model.getUniqueModelObject<Site>();
  EXPECT_TRUE(model.getOptionalUniqueModelObject<Site>());
  std::vector<std::string> variables = site.outputVariableNames();
  EXPECT_FALSE(variables.empty());
  std::vector<std::string> variables2 = site.outputVariableNames();
  ASSERT_EQ(variables.size(), variables2.size());
  for (size_t i = 0; i < variables.size(); ++i) {
    EXPECT_EQ(variables[i], variables2[i]);
  }
  EXPECT_EQ(site.outputVariableNames(), site.outputVariableNames());
  EXPECT_EQ(site.outputVariableNames().front(), site.outputVariableNames().front());
}

TEST_F(ModelFixture, Site_GettersSetters) {
  Model m;
  EXPECT_FALSE(m.getOptionalUniqueModelObject<Site>());
  Site site = m.getUniqueModelObject<Site>();
  EXPECT_TRUE(m.getOptionalUniqueModelObject<Site>());

  site.setName("My Site");

  // Latitude: Optional Double
  // Default value from IDD
  EXPECT_TRUE(site.isLatitudeDefaulted());
  // Set
  EXPECT_TRUE(site.setLatitude(45.0));
  EXPECT_EQ(45.0, site.latitude());
  EXPECT_FALSE(site.isLatitudeDefaulted());
  // Bad Value
  EXPECT_FALSE(site.setLatitude(-100.0));
  EXPECT_EQ(45.0, site.latitude());
  // Reset
  site.resetLatitude();
  EXPECT_TRUE(site.isLatitudeDefaulted());

  // Longitude: Optional Double
  // Default value from IDD
  EXPECT_TRUE(site.isLongitudeDefaulted());
  // Set
  EXPECT_TRUE(site.setLongitude(108.0));
  EXPECT_EQ(108.0, site.longitude());
  EXPECT_FALSE(site.isLongitudeDefaulted());
  // Bad Value
  EXPECT_FALSE(site.setLongitude(-190.0));
  EXPECT_EQ(108.0, site.longitude());
  // Reset
  site.resetLongitude();
  EXPECT_TRUE(site.isLongitudeDefaulted());

  // Time Zone: Optional Double
  // Default value from IDD
  EXPECT_TRUE(site.isTimeZoneDefaulted());
  // Set
  EXPECT_TRUE(site.setTimeZone(9.667));
  EXPECT_EQ(9.667, site.timeZone());
  EXPECT_FALSE(site.isTimeZoneDefaulted());
  // Bad Value
  EXPECT_FALSE(site.setTimeZone(-22.0));
  EXPECT_EQ(9.667, site.timeZone());
  // Reset
  site.resetTimeZone();
  EXPECT_TRUE(site.isTimeZoneDefaulted());

  // Elevation: Optional Double
  // Default value from IDD
  EXPECT_TRUE(site.isElevationDefaulted());
  // Set
  EXPECT_TRUE(site.setElevation(7585.714));
  EXPECT_EQ(7585.714, site.elevation());
  EXPECT_FALSE(site.isElevationDefaulted());
  // Bad Value
  EXPECT_FALSE(site.setElevation(-310.0));
  EXPECT_EQ(7585.714, site.elevation());
  // Reset
  site.resetElevation();
  EXPECT_TRUE(site.isElevationDefaulted());

  // Terrain: Optional String
  // Default value from IDD
  EXPECT_TRUE(site.isTerrainDefaulted());
  EXPECT_EQ("Suburbs", site.terrain());
  // Set
  EXPECT_TRUE(site.setTerrain("Country"));
  EXPECT_EQ("Country", site.terrain());
  EXPECT_FALSE(site.isTerrainDefaulted());
  // Bad Value
  EXPECT_FALSE(site.setTerrain("BADENUM"));
  EXPECT_EQ("Country", site.terrain());
  // Reset
  site.resetTerrain();
  EXPECT_TRUE(site.isTerrainDefaulted());

  // Keep Site Location Information: Optional Boolean
  // Default value from IDD
  EXPECT_TRUE(site.isKeepSiteLocationInformationDefaulted());
  EXPECT_FALSE(site.keepSiteLocationInformation());
  // Set
  EXPECT_TRUE(site.setKeepSiteLocationInformation(true));
  EXPECT_TRUE(site.keepSiteLocationInformation());
  EXPECT_TRUE(site.setKeepSiteLocationInformation(false));
  EXPECT_FALSE(site.keepSiteLocationInformation());
  // Reset
  site.resetKeepSiteLocationInformation();
  EXPECT_TRUE(site.isKeepSiteLocationInformationDefaulted());
  EXPECT_FALSE(site.keepSiteLocationInformation());
}
