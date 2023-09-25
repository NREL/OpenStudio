/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>

#include "ModelFixture.hpp"
#include "../Model_Impl.hpp"
#include "../Space.hpp"
#include "../SpaceType.hpp"
#include "../ThermalZone.hpp"
#include "../ElectricEquipment.hpp"
#include "../ElectricEquipment_Impl.hpp"
#include "../ElectricEquipmentDefinition.hpp"
#include "../ElectricEquipmentDefinition_Impl.hpp"
#include "../Building.hpp"
#include "../Building_Impl.hpp"
#include "../LifeCycleCost.hpp"

#include "../../utilities/geometry/Point3d.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, ElectricEquipment) {
  Model model;

  ElectricEquipmentDefinition definition(model);
  ElectricEquipment electricEquipment(definition);
  EXPECT_EQ(2u, model.numObjects());
}

TEST_F(ModelFixture, ElectricEquipment_Cost) {
  Model model;
  Building building = model.getUniqueModelObject<Building>();
  SpaceType spaceType(model);

  Point3dVector floorPrint{
    {0, 10, 0},
    {10, 10, 0},
    {10, 0, 0},
    {0, 0, 0},
  };

  boost::optional<Space> space1 = Space::fromFloorPrint(floorPrint, 3, model);
  ASSERT_TRUE(space1);
  boost::optional<Space> space2 = Space::fromFloorPrint(floorPrint, 3, model);
  ASSERT_TRUE(space2);

  EXPECT_DOUBLE_EQ(200, building.floorArea());

  ElectricEquipmentDefinition definition(model);
  ElectricEquipment electricEquipment1(definition);
  ElectricEquipment electricEquipment2(definition);

  boost::optional<LifeCycleCost> cost = LifeCycleCost::createLifeCycleCost("Dusting", definition, 0.5, "CostPerArea", "Maintenance", 1);
  ASSERT_TRUE(cost);
  EXPECT_EQ(1, cost->repeatPeriodYears());

  EXPECT_DOUBLE_EQ(0, cost->totalCost());

  electricEquipment1.setSpace(*space1);

  EXPECT_DOUBLE_EQ(50, cost->totalCost());

  electricEquipment2.setSpace(*space2);

  EXPECT_DOUBLE_EQ(100, cost->totalCost());

  ThermalZone thermalZone(model);
  space1->setThermalZone(thermalZone);
  thermalZone.setMultiplier(4);

  EXPECT_DOUBLE_EQ(250.0, cost->totalCost());

  building.setSpaceType(spaceType);
  electricEquipment1.setSpaceType(spaceType);

  EXPECT_DOUBLE_EQ(300.0, cost->totalCost());
}

/* Tests that you cannot set Fractions that sum to greater than 1 */
TEST_F(ModelFixture, ElectricEquipment_FractionsLatentRadiantLost) {
  Model m;
  ElectricEquipmentDefinition definition(m);

  ASSERT_TRUE(definition.setFractionLatent(0.5));
  ASSERT_TRUE(definition.setFractionRadiant(0.5));
  ASSERT_FALSE(definition.setFractionLost(0.75));
  ASSERT_FALSE(definition.setFractionLatent(0.75));
  ASSERT_FALSE(definition.setFractionRadiant(0.75));
}
