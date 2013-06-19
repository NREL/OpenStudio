/**********************************************************************
*  Copyright (c) 2008-2013, Alliance for Sustainable Energy.
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

#include <model/test/ModelFixture.hpp>
#include <model/Model_Impl.hpp>
#include <model/Space.hpp>
#include <model/SpaceType.hpp>
#include <model/ThermalZone.hpp>
#include <model/ElectricEquipment.hpp>
#include <model/ElectricEquipment_Impl.hpp>
#include <model/ElectricEquipmentDefinition.hpp>
#include <model/ElectricEquipmentDefinition_Impl.hpp>
#include <model/Building.hpp>
#include <model/Building_Impl.hpp>
#include <model/LifeCycleCost.hpp>

#include <utilities/geometry/Point3d.hpp>

#include <boost/foreach.hpp>

using namespace openstudio;
using namespace openstudio::model;


TEST_F(ModelFixture, ElectricEquipment)
{
  Model model;

  ElectricEquipmentDefinition definition(model);
  ElectricEquipment electricEquipment(definition);
  EXPECT_EQ(2u, model.numObjects());
}

TEST_F(ModelFixture, ElectricEquipment_Cost) {
  Model model;
  Building building = model.getUniqueModelObject<Building>();
  SpaceType spaceType(model);

  Point3dVector floorPrint;
  floorPrint.push_back(Point3d(0, 10, 0));
  floorPrint.push_back(Point3d(10, 10, 0));
  floorPrint.push_back(Point3d(10, 0, 0));
  floorPrint.push_back(Point3d(0, 0, 0));

  boost::optional<Space> space1 = Space::fromFloorPrint(floorPrint, 3, model);
  ASSERT_TRUE(space1);
  boost::optional<Space> space2 = Space::fromFloorPrint(floorPrint, 3, model);
  ASSERT_TRUE(space2);

  EXPECT_DOUBLE_EQ(200, building.floorArea());

  ElectricEquipmentDefinition definition(model);
  ElectricEquipment electricEquipment1(definition);
  ElectricEquipment electricEquipment2(definition);

  boost::optional<LifeCycleCost> cost = LifeCycleCost::createLifeCycleCost("Dusting", definition, 0.5, "CostPerArea", "Maintenance", 1);
  ASSERT_TRUE(cost);
  EXPECT_EQ(1, cost->repeatPeriodYears());

  EXPECT_DOUBLE_EQ(0, cost->totalCost());

  electricEquipment1.setSpace(*space1);
  
  EXPECT_DOUBLE_EQ(50, cost->totalCost());

  electricEquipment2.setSpace(*space2);

  EXPECT_DOUBLE_EQ(100, cost->totalCost());
  
  ThermalZone thermalZone(model);
  space1->setThermalZone(thermalZone);
  thermalZone.setMultiplier(4);

  EXPECT_DOUBLE_EQ(250.0, cost->totalCost());

  building.setSpaceType(spaceType);
  electricEquipment1.setSpaceType(spaceType);

  EXPECT_DOUBLE_EQ(300.0, cost->totalCost());
}