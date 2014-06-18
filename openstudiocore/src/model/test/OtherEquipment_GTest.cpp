/**********************************************************************
*  Copyright (c) 2008-2014, Alliance for Sustainable Energy.
*  All rights reserved.
*
*  This library is free software; you can redistribute it and/or
*  modify it under the terms of the GNU Lesser General Public
*  License as published by the Free Software Foundation; either
*  version 2.1 of the License, or (at your option) any later version.
*
*  This library is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
*  Lesser General Public License for more details.
*
*  You should have received a copy of the GNU Lesser General Public
*  License along with this library; if not, write to the Free Software
*  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
**********************************************************************/

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
