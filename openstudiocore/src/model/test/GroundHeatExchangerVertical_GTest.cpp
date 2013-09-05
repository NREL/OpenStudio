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

#include <model/GroundHeatExchangerVertical.hpp>
#include <model/GroundHeatExchangerVertical_Impl.hpp>
#include <model/Node.hpp>
#include <model/Node_Impl.hpp>

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, GroundHeatExchangerVertical_DefaultConstructor)
{
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  ASSERT_EXIT ( 
  {  
    Model model;
    GroundHeatExchangerVertical testObject = GroundHeatExchangerVertical(model);

    exit(0); 
  } ,
    ::testing::ExitedWithCode(0), "" );
}

TEST_F(ModelFixture, GroundHeatExchangerVertical_Connections)
{
  Model m; 
  GroundHeatExchangerVertical testObject(m); 

  Node inletNode(m);
  Node outletNode(m);

  m.connect(inletNode,inletNode.outletPort(),testObject,testObject.inletPort());
  m.connect(testObject,testObject.outletPort(),outletNode,outletNode.inletPort());

  ASSERT_TRUE( testObject.inletModelObject() );
  ASSERT_TRUE( testObject.outletModelObject() );
 
  EXPECT_EQ( inletNode.handle(), testObject.inletModelObject()->handle() );
  EXPECT_EQ( outletNode.handle(), testObject.outletModelObject()->handle() );
}

TEST_F(ModelFixture, GroundHeatExchangerVertical_addToNode)
{
  Model model; 
  GroundHeatExchangerVertical testObject(model);

  PlantLoop plantLoop(model);

  Node supplyOutletNode = plantLoop.supplyOutletNode();

  EXPECT_TRUE(testObject.addToNode(supplyOutletNode));

  EXPECT_EQ( (unsigned)7, plantLoop.supplyComponents().size() );
  EXPECT_EQ( (unsigned)5, plantLoop.demandComponents().size() );
				
  // inlet and outlet ports
  EXPECT_TRUE(testObject.inletPort());
  EXPECT_TRUE(testObject.outletPort());

}

TEST_F(ModelFixture, GroundHeatExchangerVertical_addToNodeDemandSide)
{
  Model model; 
  GroundHeatExchangerVertical testObject(model);

  PlantLoop plantLoop(model);

  Node demandInletNode = plantLoop.demandInletNode();

  EXPECT_FALSE(testObject.addToNode(demandInletNode));

  EXPECT_EQ( (unsigned)5, plantLoop.supplyComponents().size() );
  EXPECT_EQ( (unsigned)5, plantLoop.demandComponents().size() );
}

TEST_F(ModelFixture, GroundHeatExchangerVertical_AddToNodeAirLoop)
{
  Model model; 
  GroundHeatExchangerVertical testObject(model);

  AirLoopHVAC airLoop(model);

  Node supplyOutletNode = airLoop.supplyOutletNode();
  Node demandInletNode = airLoop.demandInletNode();

  EXPECT_FALSE(testObject.addToNode(supplyOutletNode));
  EXPECT_FALSE(testObject.addToNode(demandInletNode));

  EXPECT_EQ( (unsigned)2, airLoop.supplyComponents().size() );
  EXPECT_EQ( (unsigned)5, airLoop.demandComponents().size() );
}

TEST_F(ModelFixture, GroundHeatExchangerVertical_AddRemoveSupplyBranchForComponent)
{
  Model model; 
  GroundHeatExchangerVertical testObject(model);

  PlantLoop plantLoop(model);

  EXPECT_TRUE(plantLoop.addSupplyBranchForComponent(testObject));
  EXPECT_EQ((unsigned)7, plantLoop.supplyComponents().size());
  EXPECT_NE((unsigned)9, plantLoop.supplyComponents().size());
  
  EXPECT_TRUE(testObject.inletPort());
  EXPECT_TRUE(testObject.outletPort());

  EXPECT_TRUE(plantLoop.removeSupplyBranchWithComponent(testObject));
  EXPECT_EQ((unsigned)5, plantLoop.supplyComponents().size());
  EXPECT_NE((unsigned)7, plantLoop.supplyComponents().size());
}

TEST_F(ModelFixture, GroundHeatExchangerVertical_AddDemandBranchForComponent)
{
  Model model; 
  GroundHeatExchangerVertical testObject(model);

  PlantLoop plantLoop(model);

  EXPECT_FALSE(plantLoop.addDemandBranchForComponent(testObject));
  EXPECT_EQ((unsigned)5, plantLoop.demandComponents().size());
  EXPECT_NE((unsigned)7, plantLoop.demandComponents().size());
}

TEST_F(ModelFixture, GroundHeatExchangerVertical_AddToNodeTwoSameObjects)
{
  Model model; 
  GroundHeatExchangerVertical testObject(model);

  PlantLoop plantLoop(model);

  Node supplyOutletNode = plantLoop.supplyOutletNode();
  testObject.addToNode(supplyOutletNode);
  supplyOutletNode = plantLoop.supplyOutletNode();
  EXPECT_FALSE(testObject.addToNode(supplyOutletNode)); 
}

TEST_F(ModelFixture, GroundHeatExchangerVertical_Remove)
{
  Model model; 
  GroundHeatExchangerVertical testObject(model);

  PlantLoop plantLoop(model);

  Node supplyOutletNode = plantLoop.supplyOutletNode();
  testObject.addToNode(supplyOutletNode);

  EXPECT_EQ((unsigned)7, plantLoop.supplyComponents().size());

  testObject.remove();
  EXPECT_EQ((unsigned)5, plantLoop.supplyComponents().size());
}

//test cloning the object
TEST_F(ModelFixture, GroundHeatExchangerVertical_Clone){
  Model m;
  //make an object to clone, and edit some property to make sure the clone worked
  GroundHeatExchangerVertical testObject(m);
  testObject.setMaximumFlowRate(3.14);

  //clone into the same model
  GroundHeatExchangerVertical testObjectClone = testObject.clone(m).cast<GroundHeatExchangerVertical>();
  EXPECT_EQ(3.14,testObjectClone.maximumFlowRate());

  //clone into another model
  Model m2;
  GroundHeatExchangerVertical testObjectClone2 = testObject.clone(m2).cast<GroundHeatExchangerVertical>();
  EXPECT_EQ(3.14,testObjectClone2.maximumFlowRate());

  EXPECT_NE(testObjectClone2, testObjectClone);
  EXPECT_NE(testObjectClone2.handle(), testObjectClone.handle());
}

TEST_F(ModelFixture, GroundHeatExchangerVertical_GFunctions)
{
  Model model;
  GroundHeatExchangerVertical testObject(model);

  std::vector< std::pair<double,double> > gFunctions = testObject.gFunctions();
  EXPECT_EQ(35, gFunctions.size());

  testObject.removeAllGFunctions();

  gFunctions = testObject.gFunctions();
  EXPECT_EQ(0, gFunctions.size());

  EXPECT_TRUE(testObject.addGFunction(1.0, 1.5));
  gFunctions = testObject.gFunctions();
  EXPECT_EQ(1, gFunctions.size());

  testObject.addGFunction(2.0, 2.5);

  testObject.removeGFunction(0);
  gFunctions = testObject.gFunctions();
  EXPECT_EQ(1, gFunctions.size());

  EXPECT_DOUBLE_EQ(2.0, gFunctions[0].first);
  EXPECT_DOUBLE_EQ(2.5, gFunctions[0].second);

  testObject.removeAllGFunctions();
  for (int i=0; i<100; i++) {
    testObject.addGFunction(i, i + 0.5);
  }
  gFunctions = testObject.gFunctions();
  EXPECT_EQ(100, gFunctions.size());
  EXPECT_THROW(testObject.addGFunction(1.0, 1.5), openstudio::Exception);
}
