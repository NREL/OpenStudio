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
#include "../PumpVariableSpeed.hpp"
#include "../PumpVariableSpeed_Impl.hpp"
#include "../AirLoopHVAC.hpp"
#include "../PlantLoop.hpp"
#include "../Node.hpp"
#include "../Node_Impl.hpp"
#include "../AirLoopHVACZoneSplitter.hpp"

using namespace openstudio::model;

TEST_F(ModelFixture,PumpVariableSpeed_PumpVariableSpeed)
{
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  ASSERT_EXIT ( 
  {  
     Model m; 
     PumpVariableSpeed pump(m); 

     exit(0); 
  } ,
    ::testing::ExitedWithCode(0), "" );
}

TEST_F(ModelFixture,PumpVariableSpeed_connections)
{
  Model m; 
  PumpVariableSpeed pump(m); 

  Node inletNode(m);
  Node outletNode(m);

  m.connect(inletNode,inletNode.outletPort(),pump,pump.inletPort());
  m.connect(pump,pump.outletPort(),outletNode,outletNode.inletPort());

  ASSERT_TRUE( pump.inletModelObject() );
  ASSERT_TRUE( pump.outletModelObject() );
 
  EXPECT_EQ( inletNode.handle(), pump.inletModelObject()->handle() );
  EXPECT_EQ( outletNode.handle(), pump.outletModelObject()->handle() );
}

TEST_F(ModelFixture,PumpVariableSpeed_addToNode) {
  Model m;
  PumpVariableSpeed testObject(m);

  AirLoopHVAC airLoop(m);

  Node supplyOutletNode = airLoop.supplyOutletNode();

  EXPECT_FALSE(testObject.addToNode(supplyOutletNode));
  EXPECT_EQ( (unsigned)2, airLoop.supplyComponents().size() );

  Node inletNode = airLoop.zoneSplitter().lastOutletModelObject()->cast<Node>();

  EXPECT_FALSE(testObject.addToNode(inletNode));
  EXPECT_EQ((unsigned)5, airLoop.demandComponents().size());

  PlantLoop plantLoop(m);
  supplyOutletNode = plantLoop.supplyOutletNode();
  EXPECT_TRUE(testObject.addToNode(supplyOutletNode));
  EXPECT_EQ( (unsigned)7, plantLoop.supplyComponents().size() );

  Node demandOutletNode = plantLoop.demandOutletNode();
  EXPECT_FALSE(testObject.addToNode(demandOutletNode));
  EXPECT_EQ( (unsigned)5, plantLoop.demandComponents().size() );

  PumpVariableSpeed testObject2(m);

  EXPECT_TRUE(testObject2.addToNode(demandOutletNode));
  EXPECT_EQ( (unsigned)7, plantLoop.demandComponents().size() );

  PlantLoop plantLoop2(m);
  demandOutletNode = plantLoop2.demandOutletNode();
  EXPECT_FALSE(testObject.addToNode(demandOutletNode));
  EXPECT_EQ( (unsigned)5, plantLoop2.demandComponents().size() );

  PumpVariableSpeed testObjectClone = testObject.clone(m).cast<PumpVariableSpeed>();
  supplyOutletNode = plantLoop.supplyOutletNode();

  EXPECT_TRUE(testObjectClone.addToNode(supplyOutletNode));
  EXPECT_EQ( (unsigned)9, plantLoop.supplyComponents().size() );
}
