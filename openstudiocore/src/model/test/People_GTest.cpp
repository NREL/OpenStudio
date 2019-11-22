/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2019, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

#include "ModelFixture.hpp"
#include "../Model_Impl.hpp"
#include "../Space.hpp"
#include "../People.hpp"
#include "../People_Impl.hpp"
#include "../PeopleDefinition.hpp"
#include "../PeopleDefinition_Impl.hpp"

#include "../ScheduleRuleset.hpp"
#include "../ScheduleDay.hpp"
#include "../ScheduleTypeRegistry.hpp"

#include "../../utilities/units/QuantityConverter.hpp"
#include "../../utilities/units/Quantity.hpp"
#include "../../utilities/units/OSQuantityVector.hpp"
#include "../../utilities/units/SIUnit.hpp"
#include "../../utilities/units/BTUUnit.hpp"

#include <utilities/idd/IddEnums.hxx>

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, People_DefaultConstructor)
{
  Model model;

  // make a new People object
  PeopleDefinition definition(model);
  People person(definition);
  EXPECT_EQ(2u, model.numObjects());

  EXPECT_EQ(definition.handle(), person.definition().handle());
  EXPECT_EQ(definition.handle(), person.peopleDefinition().handle());
  ASSERT_EQ(1u, definition.instances().size());
  EXPECT_EQ(person.handle(), definition.instances()[0].handle());

  EXPECT_EQ("People", definition.numberofPeopleCalculationMethod());
  ASSERT_TRUE(definition.numberofPeople());
  EXPECT_EQ(0, definition.numberofPeople().get());
  EXPECT_FALSE(definition.peopleperSpaceFloorArea());
  EXPECT_FALSE(definition.spaceFloorAreaperPerson());
}

TEST_F(ModelFixture, People_DesignLevels)
{
  Model model;

  // make a new People object
  PeopleDefinition definition(model);
  People person(definition);

  // set number of people
  definition.setNumberofPeople(1.0);
  EXPECT_EQ("People", definition.numberofPeopleCalculationMethod());
  ASSERT_TRUE(definition.numberofPeople());
  EXPECT_EQ(1.0, definition.numberofPeople().get());
  EXPECT_FALSE(definition.peopleperSpaceFloorArea());
  EXPECT_FALSE(definition.spaceFloorAreaperPerson());
  EXPECT_EQ(1.0, definition.getNumberOfPeople(0.0));
  EXPECT_EQ(1.0, definition.getNumberOfPeople(100.0));
  EXPECT_THROW(definition.getPeoplePerFloorArea(0.0), std::exception);
  EXPECT_EQ(1.0/100.0, definition.getPeoplePerFloorArea(100.0));
  EXPECT_EQ(0.0, definition.getFloorAreaPerPerson(0.0));
  EXPECT_EQ(100.0, definition.getFloorAreaPerPerson(100.0));

  // set people per floor area
  definition.setPeopleperSpaceFloorArea(1.0);
  EXPECT_EQ("People/Area", definition.numberofPeopleCalculationMethod());
  EXPECT_FALSE(definition.numberofPeople());
  ASSERT_TRUE(definition.peopleperSpaceFloorArea());
  EXPECT_EQ(1.0, definition.peopleperSpaceFloorArea().get());
  EXPECT_FALSE(definition.spaceFloorAreaperPerson());
  EXPECT_EQ(0.0, definition.getNumberOfPeople(0.0));
  EXPECT_EQ(100.0, definition.getNumberOfPeople(100.0));
  EXPECT_EQ(1.0, definition.getPeoplePerFloorArea(0.0));
  EXPECT_EQ(1.0, definition.getPeoplePerFloorArea(100.0));
  EXPECT_EQ(1.0, definition.getFloorAreaPerPerson(0.0));
  EXPECT_EQ(1.0, definition.getFloorAreaPerPerson(100.0));

  // set floor area per person
  definition.setSpaceFloorAreaperPerson(1.0);
  EXPECT_EQ("Area/Person", definition.numberofPeopleCalculationMethod());
  EXPECT_FALSE(definition.numberofPeople());
  EXPECT_FALSE(definition.peopleperSpaceFloorArea());
  ASSERT_TRUE(definition.spaceFloorAreaperPerson());
  EXPECT_EQ(1.0, definition.spaceFloorAreaperPerson().get());
  EXPECT_EQ(0.0, definition.getNumberOfPeople(0.0));
  EXPECT_EQ(100.0, definition.getNumberOfPeople(100.0));
  EXPECT_EQ(1.0,definition.getPeoplePerFloorArea(0.0));
  EXPECT_EQ(1.0, definition.getPeoplePerFloorArea(100.0));
  EXPECT_EQ(1.0, definition.getFloorAreaPerPerson(0.0));
  EXPECT_EQ(1.0, definition.getFloorAreaPerPerson(100.0));
}

TEST_F(ModelFixture, People_Remove)
{
  Model model;

  PeopleDefinition definition(model);
  People person(definition);

  EXPECT_EQ(2u, model.numObjects());
  EXPECT_EQ(static_cast<unsigned>(1), model.getModelObjects<People>().size());
  EXPECT_EQ(static_cast<unsigned>(1), model.getModelObjects<PeopleDefinition>().size());
  std::vector<IdfObject> removed = person.remove();
  ASSERT_EQ(static_cast<unsigned>(1), removed.size());
  EXPECT_EQ(IddObjectType::OS_People, removed[0].iddObject().type().value());
  EXPECT_EQ(static_cast<unsigned>(0), model.getModelObjects<People>().size());
  EXPECT_EQ(static_cast<unsigned>(1), model.getModelObjects<PeopleDefinition>().size());
  EXPECT_EQ(1u, model.numObjects());
}

TEST_F(ModelFixture, People_Remove2)
{
  Model model;

  PeopleDefinition definition(model);
  People person1(definition);
  People person2(definition);

  EXPECT_EQ(3u, model.numObjects());
  EXPECT_EQ(static_cast<unsigned>(2), model.getModelObjects<People>().size());
  EXPECT_EQ(static_cast<unsigned>(1), model.getModelObjects<PeopleDefinition>().size());
  std::vector<IdfObject> removed = person1.remove();
  ASSERT_EQ(static_cast<unsigned>(1), removed.size());
  EXPECT_EQ(IddObjectType::OS_People, removed[0].iddObject().type().value());
  EXPECT_EQ(static_cast<unsigned>(1), model.getModelObjects<People>().size());
  EXPECT_EQ(static_cast<unsigned>(1), model.getModelObjects<PeopleDefinition>().size());
  EXPECT_EQ(2u, model.numObjects());
}

TEST_F(ModelFixture, People_RemoveDefinition)
{
  Model model;

  PeopleDefinition definition(model);
  People person(definition);

  EXPECT_EQ(2u, model.numObjects());
  EXPECT_EQ(static_cast<unsigned>(1), model.getModelObjects<People>().size());
  EXPECT_EQ(static_cast<unsigned>(1), model.getModelObjects<PeopleDefinition>().size());
  std::vector<IdfObject> removed = definition.remove();
  ASSERT_EQ(static_cast<unsigned>(2), removed.size());
  EXPECT_EQ(IddObjectType::OS_People_Definition, removed[0].iddObject().type().value());
  EXPECT_EQ(IddObjectType::OS_People, removed[1].iddObject().type().value());
  EXPECT_EQ(static_cast<unsigned>(0), model.getModelObjects<People>().size());
  EXPECT_EQ(static_cast<unsigned>(0), model.getModelObjects<PeopleDefinition>().size());
  EXPECT_EQ(0u, model.numObjects());
}

TEST_F(ModelFixture, People_RemoveDefinition2)
{
  Model model;

  PeopleDefinition definition(model);
  People person1(definition);
  People person2(definition);

  EXPECT_EQ(3u, model.numObjects());
  EXPECT_EQ(static_cast<unsigned>(2), model.getModelObjects<People>().size());
  EXPECT_EQ(static_cast<unsigned>(1), model.getModelObjects<PeopleDefinition>().size());
  std::vector<IdfObject> removed = definition.remove();
  ASSERT_EQ(static_cast<unsigned>(3), removed.size());
  EXPECT_EQ(IddObjectType::OS_People_Definition, removed[0].iddObject().type().value());
  EXPECT_EQ(IddObjectType::OS_People, removed[1].iddObject().type().value());
  EXPECT_EQ(IddObjectType::OS_People, removed[2].iddObject().type().value());
  EXPECT_EQ(static_cast<unsigned>(0), model.getModelObjects<People>().size());
  EXPECT_EQ(static_cast<unsigned>(0), model.getModelObjects<PeopleDefinition>().size());
  EXPECT_EQ(0u, model.numObjects());
}


TEST_F(ModelFixture,People_Schedule_Quantities) {
  Model model;
  PeopleDefinition definition(model);
  definition.setNumberofPeople(100.0);

  People people(definition);
  ScheduleRuleset activityLevelSchedule(model);
  EXPECT_TRUE(checkOrAssignScheduleTypeLimits("People","Activity Level",activityLevelSchedule));
  ScheduleDay defaultSchedule = activityLevelSchedule.defaultDaySchedule();
  defaultSchedule.addValue(Time(0,24,0,0),Quantity(150.0,createSIPower()/createSIPeople()));
  EXPECT_TRUE(people.setActivityLevelSchedule(activityLevelSchedule));
  OSQuantityVector ipValues = defaultSchedule.getValues(true);
  EXPECT_EQ("W/person",ipValues.units().prettyString());
  EXPECT_DOUBLE_EQ(150.0,ipValues.values()[0]);
}

TEST_F(ModelFixture,People_Clone) {
  Model library;
  Model model;

  PeopleDefinition definition(library); // ResourceObject
  definition.setNumberofPeople(100.0);

  People people(definition); // Not a ResourceObject
  ScheduleRuleset activityLevelSchedule(library); // ResourceObject
  people.setActivityLevelSchedule(activityLevelSchedule);

  EXPECT_EQ(5u,library.modelObjects().size()); // PeopleDefinition, People, ScheduleRuleset, ScheduleDay, ScheduleTypeLimits

  // Clone into the same Model
  people.clone(library);
  EXPECT_EQ(6u,library.modelObjects().size()); // PeopleDefinition, People * 2, ScheduleRuleset, ScheduleDay, ScheduleTypeLimits

  auto allPeople = library.getModelObjects<People>();
  EXPECT_EQ(2u,allPeople.size());

  // Note the PeopleDefinition is shared because it is a ResourceObject
  auto allPeopleDefinitions = library.getModelObjects<PeopleDefinition>();
  EXPECT_EQ(1u,allPeopleDefinitions.size());

  // Clone into a different Model
  people.clone(model);

  EXPECT_EQ(5u,model.modelObjects().size()); // PeopleDefinition, People, ScheduleRuleset, ScheduleDay, ScheduleTypeLimits

  // Clone from library into the model again
  people.clone(model);
  EXPECT_EQ(6u,model.modelObjects().size()); // PeopleDefinition, People * 2, ScheduleRuleset, ScheduleDay, ScheduleTypeLimits

  allPeople = model.getModelObjects<People>();
  EXPECT_EQ(2u,allPeople.size());

  // Note the PeopleDefinition is shared because it is a ResourceObject
  allPeopleDefinitions = model.getModelObjects<PeopleDefinition>();
  EXPECT_EQ(1u,allPeopleDefinitions.size());
}

