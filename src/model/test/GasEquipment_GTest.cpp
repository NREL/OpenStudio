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
#include "../GasEquipment.hpp"
#include "../GasEquipment_Impl.hpp"
#include "../GasEquipmentDefinition.hpp"
#include "../GasEquipmentDefinition_Impl.hpp"
#include "../LifeCycleCost.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, GasEquipment) {
  Model model;

  GasEquipmentDefinition definition(model);
  GasEquipment gasEquipment(definition);
  EXPECT_EQ(2u, model.numObjects());
}

TEST_F(ModelFixture, GasEquipment_Cost) {
  Model model;

  GasEquipmentDefinition definition(model);
  GasEquipment gasEquipment(definition);
  EXPECT_EQ(2u, model.numObjects());

  boost::optional<LifeCycleCost> cost = LifeCycleCost::createLifeCycleCost("Gas Hookup", definition, 10.0, "CostPerEach", "Construction");
  ASSERT_TRUE(cost);

  EXPECT_DOUBLE_EQ(0, cost->totalCost());

  Space space(model);
  gasEquipment.setSpace(space);

  EXPECT_DOUBLE_EQ(10.0, cost->totalCost());

  ThermalZone thermalZone(model);
  space.setThermalZone(thermalZone);
  thermalZone.setMultiplier(4);

  EXPECT_DOUBLE_EQ(40.0, cost->totalCost());

  SpaceType spaceType(model);
  gasEquipment.setSpaceType(spaceType);

  EXPECT_DOUBLE_EQ(0, cost->totalCost());

  space.setSpaceType(spaceType);

  EXPECT_DOUBLE_EQ(40.0, cost->totalCost());

  GasEquipment gasEquipment2(definition);
  gasEquipment2.setSpace(space);

  EXPECT_DOUBLE_EQ(80.0, cost->totalCost());
}

/* Tests that you cannot set Fractions that sum to greater than 1 */
TEST_F(ModelFixture, GasEquipment_FractionsLatentRadiantLost) {
  Model m;
  GasEquipmentDefinition definition(m);

  ASSERT_TRUE(definition.setFractionLatent(0.5));
  ASSERT_TRUE(definition.setFractionRadiant(0.5));
  ASSERT_FALSE(definition.setFractionLost(0.75));
  ASSERT_FALSE(definition.setFractionLatent(0.75));
  ASSERT_FALSE(definition.setFractionRadiant(0.75));
}
