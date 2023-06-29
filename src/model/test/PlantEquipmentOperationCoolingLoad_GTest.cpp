/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>
#include "ModelFixture.hpp"
#include "../PlantEquipmentOperationCoolingLoad.hpp"
#include "../PlantEquipmentOperationCoolingLoad_Impl.hpp"
#include "../HVACComponent.hpp"
#include "../HVACComponent_Impl.hpp"
#include "../BoilerHotWater.hpp"
#include "../BoilerHotWater_Impl.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, PlantEquipmentOperationCoolingLoad) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  ASSERT_EXIT(
    {
      model::Model m;
      model::PlantEquipmentOperationCoolingLoad plantOperationScheme(m);
      exit(0);
    },
    ::testing::ExitedWithCode(0), "");

  {
    model::Model m;
    model::PlantEquipmentOperationCoolingLoad plantOperationScheme(m);
    BoilerHotWater boiler(m);

    auto upperLimits = plantOperationScheme.loadRangeUpperLimits();
    EXPECT_EQ(1u, upperLimits.size());
    EXPECT_TRUE(plantOperationScheme.equipment(upperLimits.back()).empty());

    EXPECT_TRUE(plantOperationScheme.addEquipment(boiler));
    EXPECT_EQ(1u, plantOperationScheme.equipment(upperLimits.back()).size());
    EXPECT_FALSE(plantOperationScheme.addEquipment(boiler));

    std::vector<model::HVACComponent> equipment = {boiler};
    EXPECT_TRUE(plantOperationScheme.addLoadRange(1000.0, equipment));
    upperLimits = plantOperationScheme.loadRangeUpperLimits();
    EXPECT_EQ(2u, upperLimits.size());
    EXPECT_DOUBLE_EQ(1000.0, upperLimits[0]);
    EXPECT_EQ(1u, plantOperationScheme.equipment(upperLimits[0]).size());
    EXPECT_EQ(1u, plantOperationScheme.equipment(upperLimits[1]).size());

    auto result = plantOperationScheme.removeLoadRange(1000.0);
    EXPECT_EQ(equipment, result);
    upperLimits = plantOperationScheme.loadRangeUpperLimits();
    EXPECT_EQ(1u, upperLimits.size());

    BoilerHotWater boiler2(m);
    EXPECT_TRUE(plantOperationScheme.addEquipment(boiler2));
    EXPECT_EQ(2u, plantOperationScheme.equipment(upperLimits.back()).size());

    equipment = {boiler2};
    EXPECT_TRUE(plantOperationScheme.addLoadRange(1000.0, equipment));
    upperLimits = plantOperationScheme.loadRangeUpperLimits();
    EXPECT_EQ(2u, upperLimits.size());
    EXPECT_DOUBLE_EQ(1000.0, upperLimits[0]);
    EXPECT_EQ(1u, plantOperationScheme.equipment(upperLimits[0]).size());
    EXPECT_EQ(2u, plantOperationScheme.equipment(upperLimits[1]).size());

    EXPECT_FALSE(plantOperationScheme.removeEquipment(1000.0, boiler));
    EXPECT_TRUE(plantOperationScheme.removeEquipment(1000.0, boiler2));
    EXPECT_EQ(0u, plantOperationScheme.equipment(upperLimits[0]).size());
  }
}
