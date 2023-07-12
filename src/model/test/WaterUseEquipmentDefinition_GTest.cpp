/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>
#include "ModelFixture.hpp"
#include "../WaterUseEquipmentDefinition.hpp"
#include "../WaterUseEquipmentDefinition_Impl.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, WaterUseEquipmentDefinition_WaterUseEquipmentDefinition) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  ASSERT_EXIT(
    {
      model::Model m;

      model::WaterUseEquipmentDefinition definition(m);

      exit(0);
    },
    ::testing::ExitedWithCode(0), "");
}
