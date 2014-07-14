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
#include "../SpaceType.hpp"
#include "../ThermalZone.hpp"
#include "../GasEquipment.hpp"
#include "../GasEquipment_Impl.hpp"
#include "../GasEquipmentDefinition.hpp"
#include "../GasEquipmentDefinition_Impl.hpp"
#include "../LifeCycleCost.hpp"

using namespace openstudio;
using namespace openstudio::model;


TEST_F(ModelFixture, GasEquipment)
{
  Model model;

  GasEquipmentDefinition definition(model);
  GasEquipment gasEquipment(definition);
  EXPECT_EQ(2u, model.numObjects());
}

TEST_F(ModelFixture, GasEquipment_Cost)
{
  Model model;

  GasEquipmentDefinition definition(model);
  GasEquipment gasEquipment(definition);
  EXPECT_EQ(2u, model.numObjects());

  boost::optional<LifeCycleCost> cost = LifeCycleCost::createLifeCycleCost("Gas Hookup", definition, 10.0, "CostPerEach", "Construction");
  ASSERT_TRUE(cost);
  
  EXPECT_DOUBLE_EQ(0, cost->totalCost());

  Space space(model);
  gasEquipment.setSpace(space);

  EXPECT_DOUBLE_EQ(10.0, cost->totalCost());

  ThermalZone thermalZone(model);
  space.setThermalZone(thermalZone);
  thermalZone.setMultiplier(4);

  EXPECT_DOUBLE_EQ(40.0, cost->totalCost());

  SpaceType spaceType(model);
  gasEquipment.setSpaceType(spaceType);

  EXPECT_DOUBLE_EQ(0, cost->totalCost());

  space.setSpaceType(spaceType);

  EXPECT_DOUBLE_EQ(40.0, cost->totalCost());

  GasEquipment gasEquipment2(definition);
  gasEquipment2.setSpace(space);

  EXPECT_DOUBLE_EQ(80.0, cost->totalCost());
}
