/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>
#include "ModelFixture.hpp"
#include "../Space.hpp"
#include "../WaterUseEquipment.hpp"
#include "../WaterUseEquipmentDefinition.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, WaterUseEquipment_WaterUseEquipment) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  ASSERT_EXIT(
    {
      //create a model to use in testing this code.
      model::Model m;

      model::WaterUseEquipmentDefinition definition(m);

      model::WaterUseEquipment waterUseEquipment(definition);

      exit(0);
    },
    ::testing::ExitedWithCode(0), "");
}

TEST_F(ModelFixture, WaterUseEquipment_Space) {
  model::Model m;

  model::Space space(m);

  model::WaterUseEquipmentDefinition definition(m);

  model::WaterUseEquipment waterUseEquipment(definition);

  EXPECT_EQ(0, space.children().size());

  EXPECT_TRUE(waterUseEquipment.setSpace(space));

  ASSERT_EQ(1u, space.children().size());

  EXPECT_EQ(waterUseEquipment.handle(), space.children()[0].handle());

  ASSERT_TRUE(waterUseEquipment.parent());

  EXPECT_EQ(space.handle(), waterUseEquipment.parent()->handle());

  space.remove();

  EXPECT_TRUE(space.handle().isNull());

  EXPECT_TRUE(waterUseEquipment.handle().isNull());

  EXPECT_FALSE(definition.handle().isNull());
}
