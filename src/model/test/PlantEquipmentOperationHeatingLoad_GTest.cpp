/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>
#include "ModelFixture.hpp"
#include "../PlantEquipmentOperationHeatingLoad.hpp"
#include "../PlantEquipmentOperationHeatingLoad_Impl.hpp"
#include "../HVACComponent.hpp"
#include "../HVACComponent_Impl.hpp"
#include "../BoilerHotWater.hpp"
#include "../BoilerHotWater_Impl.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, PlantEquipmentOperationHeatingLoad) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  // Almost all of this object's behavior is defined in the base PlantEquipmentOperationLoadScheme
  // and tested in PlantEquipmentOperationCoolingLoad. Here we test simply construction.
  ASSERT_EXIT(
    {
      model::Model m;
      model::PlantEquipmentOperationHeatingLoad plantOperationScheme(m);
      exit(0);
    },
    ::testing::ExitedWithCode(0), "");
}
