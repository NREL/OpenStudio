/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>

#include "ModelFixture.hpp"
#include "../ClimateZones.hpp"
#include "../ClimateZones_Impl.hpp"

#include "../Model.hpp"
#include "../Model_Impl.hpp"
#include "../Site.hpp"
#include "../Site_Impl.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, ClimateZones) {
  // construct directly
  Model model;
  EXPECT_FALSE(model.getOptionalUniqueModelObject<ClimateZones>());
  auto czs = model.getUniqueModelObject<ClimateZones>();

  // default
  ASSERT_EQ(0u, czs.numClimateZones());
  ASSERT_EQ(0u, czs.climateZones().size());

  // after clear
  czs.clear();
  EXPECT_EQ(0u, czs.numClimateZones());
  EXPECT_EQ(0, czs.climateZones().size());

  // append a climate zone
  ClimateZone cz = czs.appendClimateZone(ClimateZones::cecInstitutionName(),
                                         ClimateZones::validClimateZoneValues(ClimateZones::cecInstitutionName(), ClimateZones::cecDefaultYear())[0]);
  ASSERT_FALSE(cz.empty());
  EXPECT_EQ(ClimateZones::cecInstitutionName(), cz.institution());
  EXPECT_EQ(ClimateZones::cecDocumentName(), cz.documentName());
  EXPECT_EQ(ClimateZones::cecDefaultYear(), cz.year());
  EXPECT_EQ("1", cz.value());
}

TEST_F(ModelFixture, ClimateZones_Site) {
  // construct Site
  Model model;
  Site site = model.getUniqueModelObject<Site>();
  EXPECT_FALSE(site.climateZones());

  // set value creates ClimateZones object
  StringVector validValues = ClimateZones::validClimateZoneValues(ClimateZones::ashraeInstitutionName(), ClimateZones::ashraeDefaultYear());
  ASSERT_EQ(17u, validValues.size());
}
