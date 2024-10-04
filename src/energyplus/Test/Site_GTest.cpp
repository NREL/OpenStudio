/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>
#include "EnergyPlusFixture.hpp"

#include "../ForwardTranslator.hpp"

#include "../../model/Model.hpp"
#include "../../model/Site.hpp"

#include <utilities/idd/Site_Location_FieldEnums.hxx>

#include <utilities/idd/IddEnums.hxx>
#include "../../utilities/idf/IdfObject.hpp"
#include "../../utilities/idf/IdfObject_Impl.hpp"

#include "../../utilities/idf/WorkspaceObject.hpp"
#include "../../utilities/idf/WorkspaceObject_Impl.hpp"

using namespace openstudio::energyplus;
using namespace openstudio::model;
using namespace openstudio;

TEST_F(EnergyPlusFixture, ForwardTranslator_Site) {
  Model m;

  Site site = m.getUniqueModelObject<Site>();

  EXPECT_TRUE(site.setName("Site 1"));
  EXPECT_TRUE(site.setLatitude(0.0));
  EXPECT_TRUE(site.setLongitude(0.0));
  EXPECT_TRUE(site.setTimeZone(0.0));
  EXPECT_TRUE(site.setElevation(0.0));
  EXPECT_TRUE(site.setTerrain("Suburbs"));
  EXPECT_TRUE(site.setKeepSiteLocationInformation("Yes"));

  ForwardTranslator ft;
  Workspace w = ft.translateModel(m);

  EXPECT_EQ(1u, w.getObjectsByType(IddObjectType::Site_Location).size());

  IdfObject idf_site = w.getObjectsByType(IddObjectType::Site_Location)[0];

  EXPECT_EQ("Site 1", idf_site.getString(Site_LocationFields::Name).get());
  EXPECT_EQ(0.0, idf_site.getDouble(Site_LocationFields::Latitude).get());
  EXPECT_EQ(0.0, idf_site.getDouble(Site_LocationFields::Longitude).get());
  EXPECT_EQ(0.0, idf_site.getDouble(Site_LocationFields::TimeZone).get());
  EXPECT_EQ(0.0, idf_site.getDouble(Site_LocationFields::Elevation).get());
  // EXPECT_EQ("Suburbs", idf_site.getString(Site_LocationFields::Terrain).get()); this is FT'd on the Building object
  EXPECT_EQ("Yes", idf_site.getString(Site_LocationFields::KeepSiteLocationInformation).get());
}
