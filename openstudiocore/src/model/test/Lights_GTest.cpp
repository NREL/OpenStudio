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
#include "../SpaceType.hpp"
#include "../ThermalZone.hpp"
#include "../Lights.hpp"
#include "../Lights_Impl.hpp"
#include "../LightsDefinition.hpp"
#include "../LightsDefinition_Impl.hpp"
#include "../Schedule.hpp"
#include "../Schedule_Impl.hpp"
#include "../LifeCycleCost.hpp"
#include "../Building.hpp"
#include "../Building_Impl.hpp"

#include "../../utilities/geometry/Point3d.hpp"

#include <utilities/idd/OS_Lights_Definition_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio;
using namespace openstudio::model;


TEST_F(ModelFixture, Lights_DefaultConstructor)
{
  Model model;

  // make a new lights object
  LightsDefinition definition(model);
  Lights light(definition);
  EXPECT_EQ(2u, model.numObjects());

  EXPECT_EQ(definition.handle(), light.definition().handle());
  EXPECT_EQ(definition.handle(), light.lightsDefinition().handle());
  ASSERT_EQ(1u, definition.instances().size());
  EXPECT_EQ(light.handle(), definition.instances()[0].handle());

  EXPECT_EQ("LightingLevel", definition.designLevelCalculationMethod());
  ASSERT_TRUE(definition.lightingLevel());
  EXPECT_EQ(0, definition.lightingLevel().get());
  EXPECT_FALSE(definition.wattsperSpaceFloorArea());
  EXPECT_FALSE(definition.wattsperPerson());
}

TEST_F(ModelFixture, Lights_LightingLevels)
{
  Model model;

  // make a new lights object
  LightsDefinition definition(model);
  Lights light(definition);

  // set absolute lighting level
  definition.setLightingLevel(1.0);
  EXPECT_EQ("LightingLevel", definition.designLevelCalculationMethod());
  ASSERT_TRUE(definition.lightingLevel());
  EXPECT_EQ(1.0, definition.lightingLevel().get());
  EXPECT_FALSE(definition.wattsperSpaceFloorArea());
  EXPECT_FALSE(definition.wattsperPerson());
  EXPECT_EQ(1.0, definition.getLightingPower(0.0, 0.0));
  EXPECT_EQ(1.0, definition.getLightingPower(100.0, 0.0));
  EXPECT_EQ(1.0, definition.getLightingPower(100.0, 10.0));
  EXPECT_EQ(1.0, definition.getLightingPower(0.0, 10.0));
  EXPECT_THROW(definition.getPowerPerFloorArea(0.0, 0.0),std::exception); // division by 0
  EXPECT_EQ(0.01, definition.getPowerPerFloorArea(100.0, 0.0));
  EXPECT_EQ(0.01, definition.getPowerPerFloorArea(100.0, 10.0));
  EXPECT_THROW(definition.getPowerPerFloorArea(0.0, 10.0),std::exception);  // division by 0
  EXPECT_THROW(definition.getPowerPerPerson(0.0, 0.0),std::exception);  // division by 0
  EXPECT_THROW(definition.getPowerPerPerson(100.0, 0.0),std::exception);  // division by 0
  EXPECT_EQ(0.1, definition.getPowerPerPerson(100.0, 10.0));
  EXPECT_EQ(0.1, definition.getPowerPerPerson(0.0, 10.0));

  // set lighting power per floor area
  definition.setWattsperSpaceFloorArea(1.0);
  EXPECT_EQ("Watts/Area", definition.designLevelCalculationMethod());
  EXPECT_FALSE(definition.lightingLevel());
  ASSERT_TRUE(definition.wattsperSpaceFloorArea());
  EXPECT_EQ(1.0, definition.wattsperSpaceFloorArea().get());
  EXPECT_FALSE(definition.wattsperPerson());
  EXPECT_EQ(0.0, definition.getLightingPower(0.0, 0.0));
  EXPECT_EQ(100.0, definition.getLightingPower(100.0, 0.0));
  EXPECT_EQ(100.0, definition.getLightingPower(100.0, 10.0));
  EXPECT_EQ(0.0, definition.getLightingPower(0.0, 10.0));
  EXPECT_EQ(1.0, definition.getPowerPerFloorArea(0.0, 0.0));
  EXPECT_EQ(1.0, definition.getPowerPerFloorArea(100.0, 0.0));
  EXPECT_EQ(1.0, definition.getPowerPerFloorArea(100.0, 10.0));
  EXPECT_EQ(1.0, definition.getPowerPerFloorArea(0.0, 10.0));
  EXPECT_THROW(definition.getPowerPerPerson(0.0, 0.0),std::exception);  // 0.0/0.0
  EXPECT_THROW(definition.getPowerPerPerson(100.0, 0.0),std::exception);   // division by 0.0
  EXPECT_EQ(10.0, definition.getPowerPerPerson(100.0, 10.0));
  EXPECT_EQ(0.0, definition.getPowerPerPerson(0.0, 10.0));

  // set lighting power per person
  definition.setWattsperPerson(1.0);
  EXPECT_EQ("Watts/Person", definition.designLevelCalculationMethod());
  EXPECT_FALSE(definition.lightingLevel());
  EXPECT_FALSE(definition.wattsperSpaceFloorArea());
  ASSERT_TRUE(definition.wattsperPerson());
  EXPECT_EQ(1.0, definition.wattsperPerson().get());
  EXPECT_EQ(0.0, definition.getLightingPower(0.0, 0.0));
  EXPECT_EQ(0.0, definition.getLightingPower(100.0, 0.0));
  EXPECT_EQ(10.0, definition.getLightingPower(100.0, 10.0));
  EXPECT_EQ(10.0, definition.getLightingPower(0.0, 10.0));
  EXPECT_THROW(definition.getPowerPerFloorArea(0.0, 0.0),std::exception); // 0/0
  EXPECT_EQ(0.0, definition.getPowerPerFloorArea(100.0, 0.0));
  EXPECT_EQ(0.1, definition.getPowerPerFloorArea(100.0, 10.0));
  EXPECT_THROW(definition.getPowerPerFloorArea(0.0, 10.0),std::exception);
  EXPECT_EQ(1.0, definition.getPowerPerPerson(0.0, 0.0));
  EXPECT_EQ(1.0, definition.getPowerPerPerson(100.0, 0.0));
  EXPECT_EQ(1.0, definition.getPowerPerPerson(100.0, 10.0));
  EXPECT_EQ(1.0, definition.getPowerPerPerson(0.0, 10.0));
}

TEST_F(ModelFixture, Lights_Remove)
{
  Model model;

  LightsDefinition definition(model);
  Lights light(definition);

  EXPECT_EQ(2u, model.numObjects());
  EXPECT_EQ(static_cast<unsigned>(1), model.getModelObjects<Lights>().size());
  EXPECT_EQ(static_cast<unsigned>(1), model.getModelObjects<LightsDefinition>().size());
  std::vector<IdfObject> removed = light.remove();
  ASSERT_EQ(static_cast<unsigned>(1), removed.size());
  EXPECT_EQ(IddObjectType::OS_Lights, removed[0].iddObject().type().value());
  EXPECT_EQ(static_cast<unsigned>(0), model.getModelObjects<Lights>().size());
  EXPECT_EQ(static_cast<unsigned>(1), model.getModelObjects<LightsDefinition>().size());
  EXPECT_EQ(1u, model.numObjects());
}

TEST_F(ModelFixture, Lights_Remove2)
{
  Model model;

  LightsDefinition definition(model);
  Lights light1(definition);
  Lights light2(definition);

  EXPECT_EQ(3u, model.numObjects());
  EXPECT_EQ(static_cast<unsigned>(2), model.getModelObjects<Lights>().size());
  EXPECT_EQ(static_cast<unsigned>(1), model.getModelObjects<LightsDefinition>().size());
  std::vector<IdfObject> removed = light1.remove();
  ASSERT_EQ(static_cast<unsigned>(1), removed.size());
  EXPECT_EQ(IddObjectType::OS_Lights, removed[0].iddObject().type().value());
  EXPECT_EQ(static_cast<unsigned>(1), model.getModelObjects<Lights>().size());
  EXPECT_EQ(static_cast<unsigned>(1), model.getModelObjects<LightsDefinition>().size());
  EXPECT_EQ(2u, model.numObjects());
}

TEST_F(ModelFixture, Lights_RemoveDefinition)
{
  Model model;

  LightsDefinition definition(model);
  Lights light(definition);

  EXPECT_EQ(2u, model.numObjects());
  EXPECT_EQ(static_cast<unsigned>(1), model.getModelObjects<Lights>().size());
  EXPECT_EQ(static_cast<unsigned>(1), model.getModelObjects<LightsDefinition>().size());
  std::vector<IdfObject> removed = definition.remove();
  ASSERT_EQ(static_cast<unsigned>(2), removed.size());
  EXPECT_EQ(IddObjectType::OS_Lights_Definition, removed[0].iddObject().type().value());
  EXPECT_EQ(IddObjectType::OS_Lights, removed[1].iddObject().type().value());
  EXPECT_EQ(static_cast<unsigned>(0), model.getModelObjects<Lights>().size());
  EXPECT_EQ(static_cast<unsigned>(0), model.getModelObjects<LightsDefinition>().size());
  EXPECT_EQ(0u, model.numObjects());
}

TEST_F(ModelFixture, Lights_RemoveDefinition2)
{
  Model model;

  LightsDefinition definition(model);
  Lights light1(definition);
  Lights light2(definition);

  EXPECT_EQ(3u, model.numObjects());
  EXPECT_EQ(static_cast<unsigned>(2), model.getModelObjects<Lights>().size());
  EXPECT_EQ(static_cast<unsigned>(1), model.getModelObjects<LightsDefinition>().size());
  std::vector<IdfObject> removed = definition.remove();
  ASSERT_EQ(static_cast<unsigned>(3), removed.size());
  EXPECT_EQ(IddObjectType::OS_Lights_Definition, removed[0].iddObject().type().value());
  EXPECT_EQ(IddObjectType::OS_Lights, removed[1].iddObject().type().value());
  EXPECT_EQ(IddObjectType::OS_Lights, removed[2].iddObject().type().value());
  EXPECT_EQ(static_cast<unsigned>(0), model.getModelObjects<Lights>().size());
  EXPECT_EQ(static_cast<unsigned>(0), model.getModelObjects<LightsDefinition>().size());
  EXPECT_EQ(0u, model.numObjects());
}

TEST_F(ModelFixture, Lights_ExampleModel)
{
  Model model = exampleModel();
  
  std::vector<Lights> lights = model.getModelObjects<Lights>();
  EXPECT_FALSE(lights.empty());

  for (Lights light : lights){
    EXPECT_TRUE(light.schedule());
  } 

}

TEST_F(ModelFixture, Lights_Precision)
{
  Model model;

  LightsDefinition definition(model);

  double watts = 0.01;
  EXPECT_TRUE(definition.setLightingLevel(watts));
  ASSERT_TRUE(definition.lightingLevel());
  EXPECT_EQ(watts, definition.lightingLevel().get());
  EXPECT_TRUE(watts == definition.lightingLevel().get());
  ASSERT_TRUE(definition.getString(OS_Lights_DefinitionFields::LightingLevel));
  std::string testString = definition.getString(OS_Lights_DefinitionFields::LightingLevel).get();
  EXPECT_EQ("0.01", testString);

  for (unsigned i = 0; i < 100; i++){
    EXPECT_TRUE(definition.setLightingLevel(watts));
    ASSERT_TRUE(definition.lightingLevel());
    EXPECT_EQ(watts, definition.lightingLevel().get());
    EXPECT_TRUE(watts == definition.lightingLevel().get());
    ASSERT_TRUE(definition.getString(OS_Lights_DefinitionFields::LightingLevel));
    EXPECT_EQ(testString, definition.getString(OS_Lights_DefinitionFields::LightingLevel).get());
  }

  watts = 0.03;
  EXPECT_TRUE(definition.setLightingLevel(watts));
  ASSERT_TRUE(definition.lightingLevel());
  EXPECT_EQ(watts, definition.lightingLevel().get());
  EXPECT_TRUE(watts == definition.lightingLevel().get());
  ASSERT_TRUE(definition.getString(OS_Lights_DefinitionFields::LightingLevel));
  testString = definition.getString(OS_Lights_DefinitionFields::LightingLevel).get();
  EXPECT_EQ("0.03", testString);

  for (unsigned i = 0; i < 100; i++){
    EXPECT_TRUE(definition.setLightingLevel(watts));
    ASSERT_TRUE(definition.lightingLevel());
    EXPECT_EQ(watts, definition.lightingLevel().get());
    EXPECT_TRUE(watts == definition.lightingLevel().get());
    ASSERT_TRUE(definition.getString(OS_Lights_DefinitionFields::LightingLevel));
    EXPECT_EQ(testString, definition.getString(OS_Lights_DefinitionFields::LightingLevel).get());
  }

  watts = 0.05;
  EXPECT_TRUE(definition.setLightingLevel(watts));
  ASSERT_TRUE(definition.lightingLevel());
  EXPECT_EQ(watts, definition.lightingLevel().get());
  EXPECT_TRUE(watts == definition.lightingLevel().get());
  ASSERT_TRUE(definition.getString(OS_Lights_DefinitionFields::LightingLevel));
  testString = definition.getString(OS_Lights_DefinitionFields::LightingLevel).get();
  EXPECT_EQ("0.05", testString);

  for (unsigned i = 0; i < 100; i++){
    EXPECT_TRUE(definition.setLightingLevel(watts));
    ASSERT_TRUE(definition.lightingLevel());
    EXPECT_EQ(watts, definition.lightingLevel().get());
    EXPECT_TRUE(watts == definition.lightingLevel().get());
    ASSERT_TRUE(definition.getString(OS_Lights_DefinitionFields::LightingLevel));
    EXPECT_EQ(testString, definition.getString(OS_Lights_DefinitionFields::LightingLevel).get());
  }

  watts = 0.9;
  EXPECT_TRUE(definition.setLightingLevel(watts));
  ASSERT_TRUE(definition.lightingLevel());
  EXPECT_EQ(watts, definition.lightingLevel().get());
  EXPECT_TRUE(watts == definition.lightingLevel().get());
  ASSERT_TRUE(definition.getString(OS_Lights_DefinitionFields::LightingLevel));
  testString = definition.getString(OS_Lights_DefinitionFields::LightingLevel).get();
  EXPECT_EQ("0.9", testString);

  for (unsigned i = 0; i < 100; i++){
    EXPECT_TRUE(definition.setLightingLevel(watts));
    ASSERT_TRUE(definition.lightingLevel());
    EXPECT_EQ(watts, definition.lightingLevel().get());
    EXPECT_TRUE(watts == definition.lightingLevel().get());
    ASSERT_TRUE(definition.getString(OS_Lights_DefinitionFields::LightingLevel));
    EXPECT_EQ(testString, definition.getString(OS_Lights_DefinitionFields::LightingLevel).get());
  }

  double x1 = 0.03;
  double x2 = 0.029999999999999999;
  EXPECT_TRUE(x1 == x2);

  x1 = 0.05;
  x2 = 0.050000000000000003;
  EXPECT_TRUE(x1 == x2);
}

TEST_F(ModelFixture, Lights_Costs)
{
  Model model;

  // make a new lights object
  LightsDefinition definition(model);
  Lights light(definition);
  EXPECT_EQ(2u, model.numObjects());

  // add a cost to the definition
  LifeCycleCost definitionCost(definition);
  EXPECT_EQ(3u, model.numObjects());

  // clone definition into new model
  {
    Model model2;
    definition.clone(model2);
    EXPECT_EQ(2u, model2.numObjects());
  }

  // clone light into new model
  {
    Model model2;
    light.clone(model2);
    EXPECT_EQ(3u, model2.numObjects());
  }

  // clone this into same model
  ModelObject light2 = light.clone(model);
  EXPECT_EQ(4u, model.numObjects());
  EXPECT_EQ(0u, light.lifeCycleCosts().size());
  EXPECT_EQ(0u, light2.lifeCycleCosts().size());
  EXPECT_EQ(1u, definition.lifeCycleCosts().size());

  // remove one light
  std::vector<IdfObject> removed = light2.remove();
  EXPECT_EQ(1u, removed.size());
  EXPECT_EQ(3u, model.numObjects());

  // remove definition
  removed = definition.remove();
  EXPECT_EQ(3u, removed.size());
  EXPECT_EQ(0u, model.numObjects());
}

TEST_F(ModelFixture, Lights_Costs2)
{
  Model model;

  // make a new lights object
  LightsDefinition definition(model);
  Lights light(definition);
  EXPECT_EQ(2u, model.numObjects());

  // cannot add cost to light
  EXPECT_THROW(LifeCycleCost cost(light), openstudio::Exception);

  // add a cost to the definition
  LifeCycleCost definitionCost(definition);
  EXPECT_EQ(3u, model.numObjects());

  EXPECT_EQ("Lights", definitionCost.itemType());
  EXPECT_EQ(definition.handle(), definitionCost.item().handle());
  EXPECT_DOUBLE_EQ(0, definitionCost.cost());
  EXPECT_EQ("CostPerEach", definitionCost.costUnits());
  EXPECT_EQ(0, definitionCost.yearsFromStart());
  EXPECT_TRUE(definitionCost.isYearsFromStartDefaulted());
  EXPECT_EQ(0, definitionCost.monthsFromStart());
  EXPECT_TRUE(definitionCost.isMonthsFromStartDefaulted());
  EXPECT_EQ(0, definitionCost.repeatPeriodYears());
  EXPECT_TRUE(definitionCost.isRepeatPeriodYearsDefaulted());
  EXPECT_EQ(0, definitionCost.repeatPeriodMonths());
  EXPECT_TRUE(definitionCost.isRepeatPeriodMonthsDefaulted());

  EXPECT_TRUE(definitionCost.setCost(2.0));
  EXPECT_DOUBLE_EQ(2.0, definitionCost.cost());

  EXPECT_TRUE(definitionCost.setCostUnits("CostPerArea"));
  EXPECT_EQ("CostPerArea", definitionCost.costUnits());

  Point3dVector floorPrint;
  floorPrint.push_back(Point3d(0, 10, 0));
  floorPrint.push_back(Point3d(10, 10, 0));
  floorPrint.push_back(Point3d(10, 0, 0));
  floorPrint.push_back(Point3d(0, 0, 0));

  boost::optional<Space> space = Space::fromFloorPrint(floorPrint, 3, model);
  ASSERT_TRUE(space);

  ASSERT_TRUE(definitionCost.costedArea());
  EXPECT_DOUBLE_EQ(0.0, definitionCost.costedArea().get());
  EXPECT_FALSE(definitionCost.costedQuantity());
  EXPECT_FALSE(definitionCost.costedThermalZones());

  light.setSpace(*space);

  ASSERT_TRUE(definitionCost.costedArea());
  EXPECT_DOUBLE_EQ(100.0, definitionCost.costedArea().get());
  EXPECT_FALSE(definitionCost.costedQuantity());
  EXPECT_FALSE(definitionCost.costedThermalZones());

  // now make a space type and instance the light to the space through the space type
  SpaceType spaceType(model);
  Lights spaceTypeLight(definition);
  spaceTypeLight.setSpaceType(spaceType);
  
  ASSERT_TRUE(definitionCost.costedArea());
  EXPECT_DOUBLE_EQ(100.0, definitionCost.costedArea().get());
  EXPECT_FALSE(definitionCost.costedQuantity());
  EXPECT_FALSE(definitionCost.costedThermalZones());

  space->setSpaceType(spaceType);

  ASSERT_TRUE(definitionCost.costedArea());
  EXPECT_DOUBLE_EQ(200.0, definitionCost.costedArea().get());
  EXPECT_FALSE(definitionCost.costedQuantity());
  EXPECT_FALSE(definitionCost.costedThermalZones());
} 


TEST_F(ModelFixture, Lights_FloorArea) {
  Model model;
  Building building = model.getUniqueModelObject<Building>();
  SpaceType spaceType(model);
  building.setSpaceType(spaceType);

  Point3dVector floorPrint;
  floorPrint.push_back(Point3d(0, 10, 0));
  floorPrint.push_back(Point3d(10, 10, 0));
  floorPrint.push_back(Point3d(10, 0, 0));
  floorPrint.push_back(Point3d(0, 0, 0));

  boost::optional<Space> space1 = Space::fromFloorPrint(floorPrint, 3, model);
  ASSERT_TRUE(space1);
  boost::optional<Space> space2 = Space::fromFloorPrint(floorPrint, 3, model);
  ASSERT_TRUE(space2);

  LightsDefinition definition(model);
  Lights lights(definition);
  lights.setSpace(*space1);

  EXPECT_DOUBLE_EQ(200, building.floorArea());
  EXPECT_DOUBLE_EQ(100, lights.floorArea());
  EXPECT_DOUBLE_EQ(100, definition.floorArea());

  lights.setSpaceType(spaceType);
  EXPECT_DOUBLE_EQ(200, building.floorArea());
  EXPECT_DOUBLE_EQ(200, lights.floorArea());
  EXPECT_DOUBLE_EQ(200, definition.floorArea());

  ThermalZone thermalZone(model);
  space1->setThermalZone(thermalZone);
  thermalZone.setMultiplier(2);

  EXPECT_DOUBLE_EQ(300, building.floorArea());
  EXPECT_DOUBLE_EQ(300, lights.floorArea());
  EXPECT_DOUBLE_EQ(300, definition.floorArea());

  lights.setSpace(*space1);
  EXPECT_DOUBLE_EQ(300, building.floorArea());
  EXPECT_DOUBLE_EQ(200, lights.floorArea());
  EXPECT_DOUBLE_EQ(200, definition.floorArea());
}
