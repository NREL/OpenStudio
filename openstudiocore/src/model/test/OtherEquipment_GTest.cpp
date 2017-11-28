/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2017, Alliance for Sustainable Energy, LLC. All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 *  following conditions are met:
 *
 *  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
 *  disclaimer.
 *
 *  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *  following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 *  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote
 *  products derived from this software without specific prior written permission from the respective party.
 *
 *  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative
 *  works may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without
 *  specific prior written permission from Alliance for Sustainable Energy, LLC.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER, THE UNITED STATES GOVERNMENT, OR ANY CONTRIBUTORS BE LIABLE FOR
 *  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **********************************************************************************************************************/

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


TEST_F(ModelFixture, OtherEquipmentDefinition_SetDesignLevel)
{
  Model model;

  OtherEquipmentDefinition definition(model);
  // Default constructor
  EXPECT_EQ("EquipmentLevel",definition.designLevelCalculationMethod());
  ASSERT_TRUE(definition.designLevel());
  EXPECT_EQ(0.0,definition.designLevel().get());
  EXPECT_FALSE(definition.wattsperSpaceFloorArea());
  EXPECT_FALSE(definition.wattsperPerson());

  // Make sure hard-coded design level calculation method strings are still valid

  EXPECT_NO_THROW(definition.setWattsperSpaceFloorArea(5.0));
  EXPECT_EQ("Watts/Area",definition.designLevelCalculationMethod());
  EXPECT_FALSE(definition.designLevel());
  ASSERT_TRUE(definition.wattsperSpaceFloorArea());
  EXPECT_EQ(5.0,definition.wattsperSpaceFloorArea().get());
  EXPECT_FALSE(definition.wattsperPerson());

  EXPECT_NO_THROW(definition.setWattsperPerson(10.0));
  EXPECT_EQ("Watts/Person",definition.designLevelCalculationMethod());
  EXPECT_FALSE(definition.designLevel());
  EXPECT_FALSE(definition.wattsperSpaceFloorArea());
  ASSERT_TRUE(definition.wattsperPerson());
  EXPECT_EQ(10.0,definition.wattsperPerson().get());

  EXPECT_NO_THROW(definition.setDesignLevel(100.0));
  EXPECT_EQ("EquipmentLevel",definition.designLevelCalculationMethod());
  ASSERT_TRUE(definition.designLevel());
  EXPECT_EQ(100.0,definition.designLevel().get());
  EXPECT_FALSE(definition.wattsperSpaceFloorArea());
  EXPECT_FALSE(definition.wattsperPerson());
}

TEST_F(ModelFixture, OtherEquipment_Instance)
{
  Model model;
  OtherEquipmentDefinition definition(model);
  OtherEquipment instance(definition);
  EXPECT_TRUE(instance.definition().optionalCast<OtherEquipmentDefinition>());
}

TEST_F(ModelFixture, OtherEquipment_FuelType)
{
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

TEST_F(ModelFixture, OtherEquipment_EndUseSubcategory)
{
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
