/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>

#include "ModelFixture.hpp"
#include "../Model_Impl.hpp"
#include "../Space.hpp"
#include "../SteamEquipment.hpp"
#include "../SteamEquipment_Impl.hpp"
#include "../SteamEquipmentDefinition.hpp"
#include "../SteamEquipmentDefinition_Impl.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, SteamEquipmentDefinition_SetDesignLevel) {
  Model model;

  SteamEquipmentDefinition definition(model);
  // Default constructor
  EXPECT_EQ("EquipmentLevel", definition.designLevelCalculationMethod());
  ASSERT_TRUE(definition.designLevel());
  EXPECT_EQ(0.0, definition.designLevel().get());
  EXPECT_FALSE(definition.wattsperSpaceFloorArea());
  EXPECT_FALSE(definition.wattsperPerson());

  // Make sure hard-coded design level calculation method strings are still valid

  EXPECT_NO_THROW(definition.setWattsperSpaceFloorArea(5.0));
  EXPECT_EQ("Watts/Area", definition.designLevelCalculationMethod());
  EXPECT_FALSE(definition.designLevel());
  ASSERT_TRUE(definition.wattsperSpaceFloorArea());
  EXPECT_EQ(5.0, definition.wattsperSpaceFloorArea().get());
  EXPECT_FALSE(definition.wattsperPerson());

  EXPECT_NO_THROW(definition.setWattsperPerson(10.0));
  EXPECT_EQ("Watts/Person", definition.designLevelCalculationMethod());
  EXPECT_FALSE(definition.designLevel());
  EXPECT_FALSE(definition.wattsperSpaceFloorArea());
  ASSERT_TRUE(definition.wattsperPerson());
  EXPECT_EQ(10.0, definition.wattsperPerson().get());

  EXPECT_NO_THROW(definition.setDesignLevel(100.0));
  EXPECT_EQ("EquipmentLevel", definition.designLevelCalculationMethod());
  ASSERT_TRUE(definition.designLevel());
  EXPECT_EQ(100.0, definition.designLevel().get());
  EXPECT_FALSE(definition.wattsperSpaceFloorArea());
  EXPECT_FALSE(definition.wattsperPerson());
}

TEST_F(ModelFixture, SteamEquipment_Instance) {
  Model model;
  SteamEquipmentDefinition definition(model);
  SteamEquipment instance(definition);
  EXPECT_TRUE(instance.definition().optionalCast<SteamEquipmentDefinition>());
}

/* Tests that you cannot set Fractions that sum to greater than 1 */
TEST_F(ModelFixture, SteamEquipment_FractionsLatentRadiantLost) {
  Model m;
  SteamEquipmentDefinition definition(m);

  ASSERT_TRUE(definition.setFractionLatent(0.5));
  ASSERT_TRUE(definition.setFractionRadiant(0.5));
  ASSERT_FALSE(definition.setFractionLost(0.75));
  ASSERT_FALSE(definition.setFractionLatent(0.75));
  ASSERT_FALSE(definition.setFractionRadiant(0.75));
}
