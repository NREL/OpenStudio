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

#include <model/WaterUseConnections.hpp>
#include <model/WaterUseConnections_Impl.hpp>
#include <model/WaterUseEquipment.hpp>
#include <model/WaterUseEquipment_Impl.hpp>
#include <model/WaterUseEquipmentDefinition.hpp>
#include <model/WaterUseEquipmentDefinition_Impl.hpp>

#include <utilities/units/Quantity.hpp>
#include <utilities/units/Unit.hpp>

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture,WaterUseConnections_WaterUseConnections)
{
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  ASSERT_EXIT ( 
  {  
    //create a model to use in testing this code.
    model::Model m; 

    model::WaterUseConnections waterUseConnections(m);

    exit(0); 
  } ,
  ::testing::ExitedWithCode(0), "" );

  model::Model m;

  model::WaterUseConnections waterUseConnections(m);

  model::WaterUseEquipmentDefinition definition(m);

  model::WaterUseEquipment e1(definition);
  model::WaterUseEquipment e2(definition);
  model::WaterUseEquipment e3(definition);

  waterUseConnections.addWaterUseEquipment(e1);
  waterUseConnections.addWaterUseEquipment(e2);
  waterUseConnections.addWaterUseEquipment(e3);

  EXPECT_EQ(3u,waterUseConnections.waterUseEquipment().size()); 

  waterUseConnections.removeWaterUseEquipment(e2);

  EXPECT_EQ(2u,waterUseConnections.waterUseEquipment().size()); 
}

