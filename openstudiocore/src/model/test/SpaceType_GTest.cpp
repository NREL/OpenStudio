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

#include "../SpaceType.hpp"
#include "../SpaceType_Impl.hpp"
#include "../People.hpp"
#include "../PeopleDefinition.hpp"
#include "../Building.hpp"
#include "../Building_Impl.hpp"
#include "../Space.hpp"
#include "../ThermalZone.hpp"

#include "../../utilities/data/Attribute.hpp"
#include "../../utilities/geometry/Point3d.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, SpaceType_InternalGainAttributes_PeoplePerFloorArea) {
  Model model;
  SpaceType spaceType(model);

  OptionalAttribute peoplePerFloorArea = spaceType.getAttribute("peoplePerFloorArea");
  ASSERT_TRUE(peoplePerFloorArea);
  EXPECT_TRUE(peoplePerFloorArea->valueType() == AttributeValueType::Double);

  PeopleDefinition defPerArea(model);
  People instPerArea(defPerArea);
  instPerArea.setSpaceType(spaceType);
}

TEST_F(ModelFixture, SpaceType_InternalGainAttributes_FloorAreaPerPerson) {

}

TEST_F(ModelFixture, SpaceType_InternalGainGetters_People) {
  Model model;
  SpaceType spaceType(model);

  PeopleDefinition defNumber(model);
  defNumber.setNumberofPeople(100.0);
  People instNumber(defNumber);
  instNumber.setMultiplier(2);
  instNumber.setSpaceType(spaceType);

  PeopleDefinition defPerArea(model);
  defPerArea.setPeopleperSpaceFloorArea(1.0); // 1 person / m^2
  People instPerArea(defPerArea);
  instPerArea.setSpaceType(spaceType);

  PeopleDefinition defAreaPer(model);
  defAreaPer.setSpaceFloorAreaperPerson(10.0); // 10.0 m^2 / person
  People instAreaPer(defAreaPer);
  instAreaPer.setMultiplier(3);
  instAreaPer.setSpaceType(spaceType);

  // in 100 m^2 have:
  // 100 * 2 + 1 * 100 + 0.1 * 3 * 100 = 330 people
  EXPECT_DOUBLE_EQ(330.0,spaceType.getNumberOfPeople(100.0));
  EXPECT_DOUBLE_EQ(3.3,spaceType.getPeoplePerFloorArea(100.0));
  EXPECT_DOUBLE_EQ(1.0/3.3,spaceType.getFloorAreaPerPerson(100.0));
}

TEST_F(ModelFixture, SpaceType_FloorArea) {
  Model model;
  Building building = model.getUniqueModelObject<Building>();
  SpaceType spaceType1(model);
  SpaceType spaceType2(model);
  EXPECT_DOUBLE_EQ(0, spaceType1.floorArea());
  EXPECT_DOUBLE_EQ(0, spaceType2.floorArea());

  Point3dVector floorPrint;
  floorPrint.push_back(Point3d(0, 10, 0));
  floorPrint.push_back(Point3d(10, 10, 0));
  floorPrint.push_back(Point3d(10, 0, 0));
  floorPrint.push_back(Point3d(0, 0, 0));

  boost::optional<Space> space1 = Space::fromFloorPrint(floorPrint, 3, model);
  ASSERT_TRUE(space1);
  boost::optional<Space> space2 = Space::fromFloorPrint(floorPrint, 3, model);
  ASSERT_TRUE(space2);

  EXPECT_DOUBLE_EQ(200, building.floorArea());

  building.setSpaceType(spaceType1);
  EXPECT_DOUBLE_EQ(200, spaceType1.floorArea());
  EXPECT_DOUBLE_EQ(0, spaceType2.floorArea());

  space2->setSpaceType(spaceType2);
  EXPECT_DOUBLE_EQ(100, spaceType1.floorArea());
  EXPECT_DOUBLE_EQ(100, spaceType2.floorArea());

  ThermalZone thermalZone1(model);
  space1->setThermalZone(thermalZone1);
  thermalZone1.setMultiplier(2);

  EXPECT_DOUBLE_EQ(300, building.floorArea());
  EXPECT_DOUBLE_EQ(200, spaceType1.floorArea());
  EXPECT_DOUBLE_EQ(100, spaceType2.floorArea());
}


TEST_F(ModelFixture, SpaceType_StandardsTypes) {
  Model model;
  SpaceType spaceType(model);

  EXPECT_FALSE(spaceType.standardsBuildingType());
  std::vector<std::string> suggestedStandardsBuildingTypes = spaceType.suggestedStandardsBuildingTypes();
  unsigned numBuildingTypes = suggestedStandardsBuildingTypes.size();
  ASSERT_GT(numBuildingTypes, 0u);
  EXPECT_FALSE(spaceType.standardsSpaceType());
  std::vector<std::string> suggestedStandardsSpaceTypes = spaceType.suggestedStandardsSpaceTypes();
  ASSERT_EQ(2u, suggestedStandardsSpaceTypes.size());
  EXPECT_EQ("Attic", suggestedStandardsSpaceTypes[0]);
  EXPECT_EQ("Plenum", suggestedStandardsSpaceTypes[1]);

  std::vector<std::string>::const_iterator it = std::find(suggestedStandardsBuildingTypes.begin(), suggestedStandardsBuildingTypes.end(), "SecondarySchool");
  EXPECT_NE(suggestedStandardsBuildingTypes.end(), it);

  EXPECT_TRUE(spaceType.setStandardsBuildingType("SecondarySchool"));
  ASSERT_TRUE(spaceType.standardsBuildingType());
  EXPECT_EQ("SecondarySchool", spaceType.standardsBuildingType().get());
  ASSERT_EQ(numBuildingTypes, spaceType.suggestedStandardsBuildingTypes().size());
  EXPECT_EQ("SecondarySchool", spaceType.suggestedStandardsBuildingTypes()[0]);
  EXPECT_FALSE(spaceType.standardsSpaceType());

  std::vector<std::string> secondarySchoolStandardsSpaceTypes = spaceType.suggestedStandardsSpaceTypes();
  unsigned numSpaceTypes = secondarySchoolStandardsSpaceTypes.size();

  ASSERT_NE(0, numSpaceTypes);
  EXPECT_TRUE(spaceType.setStandardsSpaceType(secondarySchoolStandardsSpaceTypes[0]));
  ASSERT_TRUE(spaceType.standardsSpaceType());
  EXPECT_EQ(secondarySchoolStandardsSpaceTypes[0], spaceType.standardsSpaceType().get());
  ASSERT_EQ(numSpaceTypes, spaceType.suggestedStandardsSpaceTypes().size());

  EXPECT_TRUE(spaceType.setStandardsSpaceType("Anything Goes"));
  ASSERT_TRUE(spaceType.standardsSpaceType());
  EXPECT_EQ("Anything Goes", spaceType.standardsSpaceType().get());
  ASSERT_EQ(numSpaceTypes + 1, spaceType.suggestedStandardsSpaceTypes().size());
  EXPECT_EQ("Anything Goes", spaceType.suggestedStandardsSpaceTypes()[0]);

  EXPECT_TRUE(spaceType.setStandardsSpaceType(""));
  EXPECT_FALSE(spaceType.standardsSpaceType());
  ASSERT_EQ(numSpaceTypes, spaceType.suggestedStandardsSpaceTypes().size());

  EXPECT_TRUE(spaceType.setStandardsBuildingType("Outpatient"));
  ASSERT_TRUE(spaceType.standardsBuildingType());
  EXPECT_EQ("Outpatient", spaceType.standardsBuildingType().get());
  ASSERT_EQ(numBuildingTypes, spaceType.suggestedStandardsBuildingTypes().size());
  EXPECT_EQ("Outpatient", spaceType.suggestedStandardsBuildingTypes()[0]);
  EXPECT_FALSE(spaceType.standardsSpaceType());
  EXPECT_FALSE(spaceType.suggestedStandardsSpaceTypes().empty());

  std::vector<std::string> outpatientStandardsSpaceTypes = spaceType.suggestedStandardsSpaceTypes();

  bool allSame = true;
  if (secondarySchoolStandardsSpaceTypes.size() != outpatientStandardsSpaceTypes.size()){
    allSame = false;
  }else{
    for (unsigned i = 0; i < secondarySchoolStandardsSpaceTypes.size(); ++i){
      if (secondarySchoolStandardsSpaceTypes[i] != outpatientStandardsSpaceTypes[i]){
        allSame = false;
      }
    }
  }
  EXPECT_FALSE(allSame);

  EXPECT_TRUE(spaceType.setStandardsBuildingType("Anything Goes"));
  ASSERT_TRUE(spaceType.standardsBuildingType());
  EXPECT_EQ("Anything Goes", spaceType.standardsBuildingType().get());
  ASSERT_EQ(numBuildingTypes + 1, spaceType.suggestedStandardsBuildingTypes().size());
  EXPECT_EQ("Anything Goes", spaceType.suggestedStandardsBuildingTypes()[0]);
  EXPECT_FALSE(spaceType.standardsSpaceType());
  suggestedStandardsSpaceTypes = spaceType.suggestedStandardsSpaceTypes();
  ASSERT_EQ(2u, suggestedStandardsSpaceTypes.size());
  EXPECT_EQ("Attic", suggestedStandardsSpaceTypes[0]);
  EXPECT_EQ("Plenum", suggestedStandardsSpaceTypes[1]);
} 
