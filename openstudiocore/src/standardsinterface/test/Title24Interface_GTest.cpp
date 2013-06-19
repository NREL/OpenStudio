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
#include <standardsinterface/test/StandardsInterfaceFixture.hpp>
#include <standardsinterface/Title24Interface.hpp>

#include <model/Model_Impl.hpp>
#include <model/Site.hpp>
#include <model/Site_Impl.hpp>
#include <model/ClimateZones.hpp>

#include <utilities/data/Attribute.hpp>

using namespace openstudio::standardsinterface;
using namespace openstudio::model;
using namespace openstudio;

TEST_F(StandardsInterfaceFixture,Title24Interface_ClimateZone) {
  Title24Interface standardsView(exampleModel(false).clone().cast<Model>());

  // on construction, CEC is set as climate zone institution, but no value is set
  Model bareModel = standardsView.model();
  OptionalSite oSite = bareModel.getOptionalUniqueModelObject<Site>();
  ASSERT_TRUE(oSite);
  Site site = *oSite;
  // access through attributes
  boost::optional<Attribute> climateZoneInstitution = site.getAttribute("activeClimateZoneInstitution");
  ASSERT_TRUE(climateZoneInstitution);
  ASSERT_TRUE(climateZoneInstitution->valueType() == AttributeValueType::String);
  EXPECT_EQ("CEC",climateZoneInstitution->valueAsString());
  boost::optional<Attribute> climateZone = site.getAttribute("activeClimateZoneValue");
  ASSERT_TRUE(climateZone);
  ASSERT_TRUE(climateZone->valueType() == AttributeValueType::String);
  EXPECT_EQ("",climateZone->valueAsString());
  // access through ClimateZones ModelObject
  OptionalClimateZones oClimateZones = site.climateZones();
  ASSERT_TRUE(oClimateZones);
  ClimateZones climateZones = *oClimateZones;
  openstudio::model::ClimateZone activeClimateZone = climateZones.activeClimateZone();
  EXPECT_EQ("CEC",activeClimateZone.institution());
  EXPECT_EQ(ClimateZones::cecDocumentName(),activeClimateZone.documentName());
  EXPECT_EQ(ClimateZones::cecDefaultYear(),activeClimateZone.year());
  EXPECT_EQ("",activeClimateZone.value());

  // set climate zone value through interface
  ModelObjectVectorResult objectResult = standardsView.getObjects("Building");
  ASSERT_TRUE(objectResult);
  ModelObjectVector retrievedObjects = *objectResult;
  ASSERT_EQ(1u,retrievedObjects.size());
  ModelObject building = retrievedObjects[0];
  VoidResult ok = standardsView.setAttribute("building",
                                             building,
                                             "climateZone",
                                             "3");
  ASSERT_TRUE(ok);
  // get should be ok
  StringResult stringResult = standardsView.enumAttribute("building",
                                                          building,
                                                          "climateZone");
  ASSERT_TRUE(stringResult);
  std::string climateZoneValue = *stringResult;
  EXPECT_EQ("3",climateZoneValue);
  // should be vieweable through ClimateZones
  EXPECT_TRUE(activeClimateZone == climateZones.activeClimateZone());
  EXPECT_EQ("CEC",activeClimateZone.institution());
  EXPECT_EQ(ClimateZones::cecDocumentName(),activeClimateZone.documentName());
  EXPECT_EQ(ClimateZones::cecDefaultYear(),activeClimateZone.year());
  EXPECT_EQ("3",activeClimateZone.value());
}
