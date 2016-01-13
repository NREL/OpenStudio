/**********************************************************************
 *  Copyright (c) 2008-2016, Alliance for Sustainable Energy.
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
#include "../PlantEquipmentOperationCoolingLoad.hpp"
#include "../PlantEquipmentOperationCoolingLoad_Impl.hpp"
#include "../HVACComponent.hpp"
#include "../HVACComponent_Impl.hpp"
#include "../BoilerHotWater.hpp"
#include "../BoilerHotWater_Impl.hpp"
#include "../../utilities/units/Quantity.hpp"
#include "../../utilities/units/Unit.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture,PlantEquipmentOperationCoolingLoad) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  ASSERT_EXIT (
    {
      model::Model m;
      model::PlantEquipmentOperationCoolingLoad plantOperationScheme(m);
      exit(0);
    } ,
    ::testing::ExitedWithCode(0), "" );

  {
    model::Model m;
    model::PlantEquipmentOperationCoolingLoad plantOperationScheme(m);
    BoilerHotWater boiler(m);

    auto upperLimits = plantOperationScheme.loadRangeUpperLimits();
    EXPECT_EQ(1u,upperLimits.size());
    EXPECT_TRUE(plantOperationScheme.equipment(upperLimits.back()).empty());

    EXPECT_TRUE(plantOperationScheme.addEquipment(boiler));
    EXPECT_EQ(1u,plantOperationScheme.equipment(upperLimits.back()).size());
    EXPECT_FALSE(plantOperationScheme.addEquipment(boiler));

    std::vector<model::HVACComponent> equipment = {boiler};
    EXPECT_TRUE(plantOperationScheme.addLoadRange(1000.0,equipment));
    upperLimits = plantOperationScheme.loadRangeUpperLimits();
    EXPECT_EQ(2u,upperLimits.size());
    EXPECT_FLOAT_EQ(1000.0,upperLimits[0]);
    EXPECT_EQ(1u,plantOperationScheme.equipment(upperLimits[0]).size());
    EXPECT_EQ(1u,plantOperationScheme.equipment(upperLimits[1]).size());

    auto result = plantOperationScheme.removeLoadRange(1000.0); 
    EXPECT_EQ(equipment,result);
    upperLimits = plantOperationScheme.loadRangeUpperLimits();
    EXPECT_EQ(1u,upperLimits.size());

    BoilerHotWater boiler2(m);
    EXPECT_TRUE(plantOperationScheme.addEquipment(boiler2));
    EXPECT_EQ(2u,plantOperationScheme.equipment(upperLimits.back()).size());

    equipment = {boiler2};
    EXPECT_TRUE(plantOperationScheme.addLoadRange(1000.0,equipment));
    upperLimits = plantOperationScheme.loadRangeUpperLimits();
    EXPECT_EQ(2u,upperLimits.size());
    EXPECT_FLOAT_EQ(1000.0,upperLimits[0]);
    EXPECT_EQ(1u,plantOperationScheme.equipment(upperLimits[0]).size());
    EXPECT_EQ(2u,plantOperationScheme.equipment(upperLimits[1]).size());

    EXPECT_FALSE(plantOperationScheme.removeEquipment(1000.0,boiler));
    EXPECT_TRUE(plantOperationScheme.removeEquipment(1000.0,boiler2));
    EXPECT_EQ(0u,plantOperationScheme.equipment(upperLimits[0]).size());
  }
}

