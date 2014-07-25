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
#include "../Space.hpp"
#include "../WaterUseEquipment.hpp"
#include "../WaterUseEquipment_Impl.hpp"
#include "../WaterUseEquipmentDefinition.hpp"
#include "../WaterUseEquipmentDefinition_Impl.hpp"
#include "../../utilities/units/Quantity.hpp"
#include "../../utilities/units/Unit.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture,WaterUseEquipment_WaterUseEquipment)
{
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  ASSERT_EXIT ( 
  {  
    //create a model to use in testing this code.
    model::Model m; 

    model::WaterUseEquipmentDefinition definition(m);

    model::WaterUseEquipment waterUseEquipment(definition);

    exit(0); 
  } ,
  ::testing::ExitedWithCode(0), "" );
}

TEST_F(ModelFixture,WaterUseEquipment_Space)
{
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
