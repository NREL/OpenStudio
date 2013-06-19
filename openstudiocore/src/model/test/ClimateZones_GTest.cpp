/**********************************************************************
*  Copyright (c) 2008-2013, Alliance for Sustainable Energy.
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

#include <model/test/ModelFixture.hpp>
#include <model/ClimateZones.hpp>
#include <model/ClimateZones_Impl.hpp>

#include <model/Model.hpp>
#include <model/Model_Impl.hpp>
#include <model/Site.hpp>
#include <model/Site_Impl.hpp>

#include <utilities/data/Attribute.hpp>

#include <boost/foreach.hpp>

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, ClimateZones)
{
  // construct directly
  Model model;
  EXPECT_FALSE(model.getOptionalUniqueModelObject<ClimateZones>());
  ClimateZones czs = model.getUniqueModelObject<ClimateZones>();

  // default
  EXPECT_EQ(1u,czs.numClimateZones());
  ClimateZone acz = czs.activeClimateZone();
  ASSERT_FALSE(acz.empty());
  EXPECT_EQ(ClimateZones::ashraeInstitutionName(),acz.institution());
  EXPECT_EQ(ClimateZones::ashraeDocumentName(),acz.documentName());
  EXPECT_EQ(ClimateZones::ashraeDefaultYear(),acz.year());
  EXPECT_EQ("",acz.value());

  // after clear
  czs.clear();
  EXPECT_EQ(0u,czs.numClimateZones());
  acz = czs.activeClimateZone();
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

  // acz still empty
  acz = czs.activeClimateZone();
  EXPECT_TRUE(acz.empty());

  // set acz
  acz = czs.setActiveClimateZone(ClimateZones::cecInstitutionName(),
                                 ClimateZones::cecDefaultYear());
  ASSERT_FALSE(acz.empty());
  EXPECT_TRUE(acz == cz);
}

TEST_F(ModelFixture, ClimateZones_AsAttributesOfSite)
{
  // construct Site
  Model model;
  Site site = model.getUniqueModelObject<Site>();
  EXPECT_FALSE(site.climateZones());

  // climate zone attribute before creation of ClimateZones object
  ASSERT_TRUE(site.getAttribute("activeClimateZoneValue"));
  Attribute climateZoneAttribute = site.getAttribute("activeClimateZoneValue").get();
  EXPECT_TRUE(climateZoneAttribute.valueType() == AttributeValueType::String);
  EXPECT_TRUE(climateZoneAttribute.valueAsString().empty()); // ... so is empty
  EXPECT_FALSE(site.climateZones());

  // climate zone institution attribute before creation of ClimateZones object
  ASSERT_TRUE(site.getAttribute("activeClimateZoneInstitution"));
  Attribute climateZoneInstitutionAttribute = site.getAttribute("activeClimateZoneInstitution").get();
  EXPECT_TRUE(climateZoneInstitutionAttribute.valueType() == AttributeValueType::String);
  EXPECT_TRUE(climateZoneInstitutionAttribute.valueAsString().empty());
  EXPECT_FALSE(site.climateZones());

  // set value creates ClimateZones object
  StringVector validValues = ClimateZones::validClimateZoneValues(
      ClimateZones::ashraeInstitutionName(),ClimateZones::ashraeDefaultYear());
  ASSERT_EQ(17u,validValues.size());

  EXPECT_TRUE(site.setAttribute("activeClimateZoneValue", validValues[7]));

  ASSERT_TRUE(site.getAttribute("activeClimateZoneValue"));
  climateZoneAttribute = site.getAttribute("activeClimateZoneValue").get();
  EXPECT_TRUE(climateZoneAttribute.valueType() == AttributeValueType::String);
  EXPECT_EQ("4A",climateZoneAttribute.valueAsString());
  EXPECT_TRUE(site.climateZones());

  ASSERT_TRUE(site.getAttribute("activeClimateZoneInstitution"));
  climateZoneInstitutionAttribute = site.getAttribute("activeClimateZoneInstitution").get();
  EXPECT_EQ("ASHRAE",climateZoneInstitutionAttribute.valueAsString());

  // set institution to ASHRAE changes nothing
  EXPECT_TRUE(site.setAttribute("activeClimateZoneInstitution", "ASHRAE"));

  ASSERT_TRUE(site.getAttribute("activeClimateZoneInstitution"));
  climateZoneInstitutionAttribute = site.getAttribute("activeClimateZoneInstitution").get();
  EXPECT_EQ("ASHRAE",climateZoneInstitutionAttribute.valueAsString());
  EXPECT_EQ(1u,site.climateZones()->numClimateZones());

  // set institution to CEC creates new climate zone with empty value
  EXPECT_TRUE(site.setAttribute("activeClimateZoneInstitution", "CEC"));

  ASSERT_TRUE(site.getAttribute("activeClimateZoneInstitution"));
  climateZoneInstitutionAttribute = site.getAttribute("activeClimateZoneInstitution").get();
  EXPECT_EQ("CEC",climateZoneInstitutionAttribute.valueAsString());
  EXPECT_EQ(2u,site.climateZones()->numClimateZones());

  EXPECT_TRUE(site.setAttribute("activeClimateZoneValue", "14"));

  ASSERT_TRUE(site.getAttribute("activeClimateZoneValue"));
  climateZoneAttribute = site.getAttribute("activeClimateZoneValue").get();
  EXPECT_TRUE(climateZoneAttribute.valueType() == AttributeValueType::String);
  EXPECT_EQ("14",climateZoneAttribute.valueAsString());
}
