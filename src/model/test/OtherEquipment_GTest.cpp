/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <algorithm>

#include <gtest/gtest.h>

#include "ModelFixture.hpp"
#include "../Model_Impl.hpp"
#include "../Space.hpp"
#include "../OtherEquipment.hpp"
#include "../OtherEquipment_Impl.hpp"
#include "../OtherEquipmentDefinition.hpp"
#include "../OtherEquipmentDefinition_Impl.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, OtherEquipmentDefinition_SetDesignLevel) {
  Model model;

  OtherEquipmentDefinition definition(model);
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

TEST_F(ModelFixture, OtherEquipment_Instance) {
  Model model;
  OtherEquipmentDefinition definition(model);
  OtherEquipment instance(definition);
  EXPECT_TRUE(instance.definition().optionalCast<OtherEquipmentDefinition>());
}

TEST_F(ModelFixture, OtherEquipment_FuelType) {
  Model model;
  OtherEquipmentDefinition definition(model);
  OtherEquipment equipment(definition);

  EXPECT_EQ(equipment.fuelType(), "None");
  EXPECT_TRUE(equipment.isFuelTypeDefaulted());
  EXPECT_TRUE(equipment.setFuelType("NaturalGas"));
  EXPECT_EQ(equipment.fuelType(), "NaturalGas");
  equipment.resetFuelType();
  EXPECT_EQ(equipment.fuelType(), "None");
  EXPECT_TRUE(equipment.isFuelTypeDefaulted());
  std::vector<std::string> validFuelTypes(equipment.validFuelTypeValues());
  EXPECT_NE(std::find(validFuelTypes.begin(), validFuelTypes.end(), "Electricity"), validFuelTypes.end());
  EXPECT_NE(std::find(validFuelTypes.begin(), validFuelTypes.end(), "NaturalGas"), validFuelTypes.end());
  EXPECT_NE(std::find(validFuelTypes.begin(), validFuelTypes.end(), "Coal"), validFuelTypes.end());
  EXPECT_EQ(validFuelTypes.size(), 14);
}

TEST_F(ModelFixture, OtherEquipment_EndUseSubcategory) {
  Model model;
  OtherEquipmentDefinition definition(model);
  OtherEquipment equipment(definition);

  EXPECT_EQ(equipment.endUseSubcategory(), "General");
  EXPECT_FALSE(equipment.isEndUseSubcategoryDefaulted());
  EXPECT_TRUE(equipment.setEndUseSubcategory("Category A"));
  EXPECT_EQ(equipment.endUseSubcategory(), "Category A");
  EXPECT_TRUE(equipment.setEndUseSubcategory("Category B"));
  EXPECT_EQ(equipment.endUseSubcategory(), "Category B");
  equipment.resetEndUseSubcategory();
  EXPECT_EQ(equipment.endUseSubcategory(), "General");
  EXPECT_TRUE(equipment.isEndUseSubcategoryDefaulted());
}

/* Tests that you cannot set Fractions that sum to greater than 1 */
TEST_F(ModelFixture, OtherEquipment_FractionsLatentRadiantLost) {
  Model m;
  OtherEquipmentDefinition definition(m);

  ASSERT_TRUE(definition.setFractionLatent(0.5));
  ASSERT_TRUE(definition.setFractionRadiant(0.5));
  ASSERT_FALSE(definition.setFractionLost(0.75));
  ASSERT_FALSE(definition.setFractionLatent(0.75));
  ASSERT_FALSE(definition.setFractionRadiant(0.75));
}
