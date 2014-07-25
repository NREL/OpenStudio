/**********************************************************************
*  Copyright (c) 2008-2014, Alliance for Sustainable Energy.
*  All rights reserved.
*
*  This library is free software; you can redistribute it and/or
*  modify it under the terms of the GNU Lesser General Public
*  License as published by the Free Software Foundation; either
*  version 2.1 of the License, or (at your option) any later version.
*
*  This library is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
*  Lesser General Public License for more details.
*
*  You should have received a copy of the GNU Lesser General Public
*  License along with this library; if not, write to the Free Software
*  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
**********************************************************************/

#include <gtest/gtest.h>

#include "ModelFixture.hpp"
#include "../ClimateZones.hpp"
#include "../ClimateZones_Impl.hpp"

#include "../Model.hpp"
#include "../Model_Impl.hpp"
#include "../Site.hpp"
#include "../Site_Impl.hpp"

#include "../../utilities/data/Attribute.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, ClimateZones)
{
  // construct directly
  Model model;
  EXPECT_FALSE(model.getOptionalUniqueModelObject<ClimateZones>());
  ClimateZones czs = model.getUniqueModelObject<ClimateZones>();

  // default
  ASSERT_EQ(1u,czs.numClimateZones());
  ASSERT_EQ(1u,czs.climateZones().size());
  ClimateZone acz = czs.climateZones()[0];
  ASSERT_FALSE(acz.empty());
  EXPECT_EQ(ClimateZones::ashraeInstitutionName(),acz.institution());
  EXPECT_EQ(ClimateZones::ashraeDocumentName(),acz.documentName());
  EXPECT_EQ(ClimateZones::ashraeDefaultYear(),acz.year());
  EXPECT_EQ("",acz.value());

  // after clear
  czs.clear();
  EXPECT_EQ(0u,czs.numClimateZones());
  EXPECT_EQ(0,czs.climateZones().size());
  EXPECT_TRUE(acz.empty());

  // append a climate zone
  ClimateZone cz = czs.appendClimateZone(ClimateZones::cecInstitutionName(),
                                         ClimateZones::validClimateZoneValues(
                                             ClimateZones::cecInstitutionName(),
                                             ClimateZones::cecDefaultYear())[0]);
  ASSERT_FALSE(cz.empty());
  EXPECT_EQ(ClimateZones::cecInstitutionName(),cz.institution());
  EXPECT_EQ(ClimateZones::cecDocumentName(),cz.documentName());
  EXPECT_EQ(ClimateZones::cecDefaultYear(),cz.year());
  EXPECT_EQ("1",cz.value());
}

TEST_F(ModelFixture, ClimateZones_Site)
{
  // construct Site
  Model model;
  Site site = model.getUniqueModelObject<Site>();
  EXPECT_FALSE(site.climateZones());

  // set value creates ClimateZones object
  StringVector validValues = ClimateZones::validClimateZoneValues(
      ClimateZones::ashraeInstitutionName(),ClimateZones::ashraeDefaultYear());
  ASSERT_EQ(17u,validValues.size());
}
