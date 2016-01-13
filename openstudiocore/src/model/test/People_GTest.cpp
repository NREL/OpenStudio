/**********************************************************************
*  Copyright (c) 2008-2016, Alliance for Sustainable Energy.
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

