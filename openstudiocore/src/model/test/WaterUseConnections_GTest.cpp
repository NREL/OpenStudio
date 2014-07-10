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
#include "../WaterUseConnections.hpp"
#include "../WaterUseConnections_Impl.hpp"
#include "../WaterUseEquipment.hpp"
#include "../WaterUseEquipmentDefinition.hpp"
#include "../AirLoopHVAC.hpp"
#include "../PlantLoop.hpp"
#include "../Node.hpp"
#include "../Node_Impl.hpp"
#include "../AirLoopHVACZoneSplitter.hpp"

using namespace openstudio::model;

TEST_F(ModelFixture,WaterUseConnections_WaterUseConnections)
{
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  ASSERT_EXIT ( 
  {  
    //create a model to use in testing this code.
    Model m; 

    WaterUseConnections waterUseConnections(m);

    exit(0); 
  } ,
  ::testing::ExitedWithCode(0), "" );
}

TEST_F(ModelFixture,WaterUseConnections_addRemoveEquipment) {
  Model m;
  WaterUseConnections waterUseConnections(m);

  WaterUseEquipmentDefinition definition(m);

  WaterUseEquipment e1(definition);
  WaterUseEquipment e2(definition);
  WaterUseEquipment e3(definition);

  waterUseConnections.addWaterUseEquipment(e1);
  waterUseConnections.addWaterUseEquipment(e2);
  waterUseConnections.addWaterUseEquipment(e3);

  EXPECT_EQ(3u,waterUseConnections.waterUseEquipment().size()); 

  waterUseConnections.removeWaterUseEquipment(e2);

  EXPECT_EQ(2u,waterUseConnections.waterUseEquipment().size()); 
}

TEST_F(ModelFixture,WaterUseConnections_addToNode) {
  Model m;
  WaterUseConnections testObject(m);

  AirLoopHVAC airLoop(m);

  Node supplyOutletNode = airLoop.supplyOutletNode();

  EXPECT_FALSE(testObject.addToNode(supplyOutletNode));
  EXPECT_EQ( (unsigned)2, airLoop.supplyComponents().size() );

  Node inletNode = airLoop.zoneSplitter().lastOutletModelObject()->cast<Node>();

  EXPECT_FALSE(testObject.addToNode(inletNode));
  EXPECT_EQ((unsigned)5, airLoop.demandComponents().size());

  PlantLoop plantLoop(m);
  supplyOutletNode = plantLoop.supplyOutletNode();
  EXPECT_FALSE(testObject.addToNode(supplyOutletNode));
  EXPECT_EQ( (unsigned)5, plantLoop.supplyComponents().size() );

  Node demandOutletNode = plantLoop.demandOutletNode();
  EXPECT_TRUE(testObject.addToNode(demandOutletNode));
  EXPECT_EQ( (unsigned)7, plantLoop.demandComponents().size() );

  WaterUseConnections testObjectClone = testObject.clone(m).cast<WaterUseConnections>();
  demandOutletNode = plantLoop.demandOutletNode();

  EXPECT_TRUE(testObjectClone.addToNode(demandOutletNode));
  EXPECT_EQ( (unsigned)9, plantLoop.demandComponents().size() );
}
