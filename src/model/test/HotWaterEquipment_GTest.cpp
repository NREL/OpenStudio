/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>

#include "ModelFixture.hpp"
#include "../Model_Impl.hpp"
#include "../Space.hpp"
#include "../HotWaterEquipment.hpp"
#include "../HotWaterEquipment_Impl.hpp"
#include "../HotWaterEquipmentDefinition.hpp"
#include "../HotWaterEquipmentDefinition_Impl.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, HotWaterEquipment) {
  Model model;

  HotWaterEquipmentDefinition definition(model);
  HotWaterEquipment hotWaterEquipment(definition);
  EXPECT_EQ(2u, model.numObjects());
}

/* Tests that you cannot set Fractions that sum to greater than 1 */
TEST_F(ModelFixture, HotWaterEquipment_FractionsLatentRadiantLost) {
  Model m;
  HotWaterEquipmentDefinition definition(m);

  ASSERT_TRUE(definition.setFractionLatent(0.5));
  ASSERT_TRUE(definition.setFractionRadiant(0.5));
  ASSERT_FALSE(definition.setFractionLost(0.75));
  ASSERT_FALSE(definition.setFractionLatent(0.75));
  ASSERT_FALSE(definition.setFractionRadiant(0.75));
}
