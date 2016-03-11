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
#include "../WaterHeaterStratified.hpp"
#include "../WaterHeaterStratified_Impl.hpp"
#include "../WaterUseConnections.hpp"
#include "../WaterUseEquipment.hpp"
#include "../WaterUseEquipmentDefinition.hpp"
#include "../PlantLoop.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, WaterHeaterStratified_WaterHeaterStratified)
{
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  ASSERT_EXIT ( 
  {  
     Model m;
     WaterHeaterStratified waterHeaterStratified(m);
     exit(0);
  } ,
    ::testing::ExitedWithCode(0), "" );
}

TEST_F(ModelFixture, WaterHeaterStratified_addToPlantLoop)
{
    Model m;
    WaterHeaterStratified waterHeaterStratified(m);

    PlantLoop plantLoop(m);
    ASSERT_EQ( 5u, plantLoop.supplyComponents().size() );
    ASSERT_EQ( 5u, plantLoop.demandComponents().size() );

    WaterUseConnections waterUseConnections(m);

    WaterUseEquipmentDefinition definition(m);

    WaterUseEquipment e1(definition);
    WaterUseEquipment e2(definition);
    WaterUseEquipment e3(definition);

    waterUseConnections.addWaterUseEquipment(e1);
    waterUseConnections.addWaterUseEquipment(e2);
    waterUseConnections.addWaterUseEquipment(e3);

    EXPECT_EQ(3u, waterUseConnections.waterUseEquipment().size());

    plantLoop.addDemandBranchForComponent(waterUseConnections);
    ASSERT_EQ( 5u, plantLoop.supplyComponents().size() );
    ASSERT_EQ( 7u, plantLoop.demandComponents().size() );
}
