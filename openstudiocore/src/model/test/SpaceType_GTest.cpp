/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2017, Alliance for Sustainable Energy, LLC. All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 *  following conditions are met:
 *
 *  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
 *  disclaimer.
 *
 *  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *  following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 *  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote
 *  products derived from this software without specific prior written permission from the respective party.
 *
 *  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative
 *  works may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without
 *  specific prior written permission from Alliance for Sustainable Energy, LLC.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER, THE UNITED STATES GOVERNMENT, OR ANY CONTRIBUTORS BE LIABLE FOR
 *  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **********************************************************************************************************************/

#include <gtest/gtest.h>

#include "ModelFixture.hpp"

#include "../SpaceType.hpp"
#include "../SpaceType_Impl.hpp"
#include "../People.hpp"
#include "../People_Impl.hpp"
#include "../PeopleDefinition.hpp"
#include "../PeopleDefinition_Impl.hpp"
#include "../Building.hpp"
#include "../Building_Impl.hpp"
#include "../Space.hpp"
#include "../ThermalZone.hpp"
#include "../ScheduleRuleset.hpp"
#include "../ScheduleRuleset_Impl.hpp"

#include "../../utilities/data/Attribute.hpp"
#include "../../utilities/geometry/Point3d.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, SpaceType_InternalGainAttributes_PeoplePerFloorArea) {
  Model model;
  SpaceType spaceType(model);

  // Removed due to removal of attributes
  // OptionalAttribute peoplePerFloorArea = spaceType.getAttribute("peoplePerFloorArea");
  // ASSERT_TRUE(peoplePerFloorArea);
  // EXPECT_TRUE(peoplePerFloorArea->valueType() == AttributeValueType::Double);

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

TEST_F(ModelFixture, SpaceType_Clone) {
  Model library;
  Model model;

  SpaceType librarySpaceType(library);

  PeopleDefinition definition(library);
  definition.setNumberofPeople(100.0);

  People people(definition); // Not a ResourceObject
  ScheduleRuleset activityLevelSchedule(library); // ResourceObject
  people.setActivityLevelSchedule(activityLevelSchedule);
  people.setSpaceType(librarySpaceType);

  EXPECT_EQ(6u,library.modelObjects().size()); // SpaceType, PeopleDefinition, People, ScheduleRuleset, ScheduleDay, ScheduleTypeLimits

  // Clone into same model
  // Even though SpaceType is a resource object, because we call ::clone on it directly, we get a new one
  librarySpaceType.clone(library);
  EXPECT_EQ(8u,library.modelObjects().size()); // SpaceType * 2, PeopleDefinition, People * 2, ScheduleRuleset, ScheduleDay, ScheduleTypeLimits

  EXPECT_EQ(2u,library.getModelObjects<SpaceType>().size());
  EXPECT_EQ(2u,library.getModelObjects<People>().size());

  // The referenced ResourceObject instances are not duplicated
  EXPECT_EQ(1u,library.getModelObjects<PeopleDefinition>().size());
  EXPECT_EQ(1u,library.getModelObjects<ScheduleRuleset>().size());

  // Clone into a different model
  librarySpaceType.clone(model);
  EXPECT_EQ(6u,model.modelObjects().size());

  auto modelSpaceTypes = model.getModelObjects<SpaceType>();
  ASSERT_EQ(1u,modelSpaceTypes.size());

  // SpaceType gets a new handle
  EXPECT_NE(librarySpaceType.handle(),modelSpaceTypes.front().handle());

  // Clone into model again
  librarySpaceType.clone(model);
  EXPECT_EQ(8u,model.modelObjects().size());

  EXPECT_EQ(2u,model.getModelObjects<SpaceType>().size());
  EXPECT_EQ(2u,model.getModelObjects<People>().size());

  // The referenced ResourceObject instances are not duplicated
  EXPECT_EQ(1u,model.getModelObjects<PeopleDefinition>().size());
  EXPECT_EQ(1u,model.getModelObjects<ScheduleRuleset>().size());
}

TEST_F(ModelFixture, SpaceType_Name_Clone) {
  Model m;

  SpaceType st1(m);
  EXPECT_TRUE(st1.setName("My Space Type"));

  SpaceType st2 = st1.clone(m).cast<SpaceType>();
  EXPECT_NE(st1.handle(), st2.handle());
  EXPECT_NE(st1.nameString(), st2.nameString());

  Model m2;
  SpaceType st3 = st1.clone(m2).cast<SpaceType>();
  EXPECT_NE(st1.handle(), st3.handle());
  EXPECT_EQ(st1.nameString(), st3.nameString());
}

/* If the space type is the unique Plenum Space type, it should Log and Error and return itself */
TEST_F(ModelFixture, SpaceType_Clone_Plenum) {

  Model m;

  SpaceType m1_st = m.plenumSpaceType();

  // Does clone
  SpaceType m1_stClone = m1_st.clone(m).cast<SpaceType>();
  ASSERT_NE(m1_st.handle(), m1_stClone.handle());
  ASSERT_NE(m1_st.nameString(), m1_stClone.nameString()) << m;

  // Try in another model with no plenum space type, the clone should become the new plenum space type
  Model m2;
  SpaceType m2_stClone = m1_st.clone(m2).cast<SpaceType>();
  ASSERT_NE(m1_st.handle(), m2_stClone.handle());
  ASSERT_EQ(m1_st.nameString(), m2_stClone.nameString());
  ASSERT_EQ(m2.plenumSpaceType().handle(), m2_stClone.handle());

  // Try in another model with a plenum space type, the clone should not become the new plenum space type
  Model m3;
  m3.plenumSpaceType();
  SpaceType m3_stClone = m1_st.clone(m3).cast<SpaceType>();
  ASSERT_NE(m1_st.handle(), m3_stClone.handle());
  ASSERT_NE(m1_st.nameString(), m3_stClone.nameString());
  ASSERT_NE(m3.plenumSpaceType().handle(), m3_stClone.handle());
}

