/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2017, Alliance for Sustainable Energy, LLC. All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 *  following conditions are met:
 *
 *  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
 *  disclaimer.
 *
 *  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *  following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 *  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote
 *  products derived from this software without specific prior written permission from the respective party.
 *
 *  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative
 *  works may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without
 *  specific prior written permission from Alliance for Sustainable Energy, LLC.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER, THE UNITED STATES GOVERNMENT, OR ANY CONTRIBUTORS BE LIABLE FOR
 *  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **********************************************************************************************************************/

#include <gtest/gtest.h>
#include "ModelFixture.hpp"
#include "../GroundHeatExchangerVertical.hpp"
#include "../GroundHeatExchangerVertical_Impl.hpp"
#include "../AirLoopHVAC.hpp"
#include "../PlantLoop.hpp"
#include "../Node.hpp"
#include "../Node_Impl.hpp"
#include "../AirLoopHVACZoneSplitter.hpp"

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
  Model m;
  GroundHeatExchangerVertical testObject(m);

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

  GroundHeatExchangerVertical testObjectClone = testObject.clone(m).cast<GroundHeatExchangerVertical>();
  supplyOutletNode = plantLoop.supplyOutletNode();

  EXPECT_TRUE(testObjectClone.addToNode(supplyOutletNode));
  EXPECT_EQ( (unsigned)9, plantLoop.supplyComponents().size() );
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

  std::vector< GFunction > gFunctions = testObject.gFunctions();
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

  EXPECT_DOUBLE_EQ(2.0, gFunctions[0].lnValue());
  EXPECT_DOUBLE_EQ(2.5, gFunctions[0].gValue());

  testObject.removeAllGFunctions();
  for (int i=0; i<100; i++) {
    testObject.addGFunction(i, i + 0.5);
  }
  gFunctions = testObject.gFunctions();
  EXPECT_EQ(100, gFunctions.size());
  EXPECT_THROW(testObject.addGFunction(1.0, 1.5), openstudio::Exception);
}
