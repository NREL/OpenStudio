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
#include "../BoilerHotWater.hpp"
#include "../BoilerHotWater_Impl.hpp"
#include "../AirLoopHVAC.hpp"
#include "../PlantLoop.hpp"
#include "../Node.hpp"
#include "../Node_Impl.hpp"
#include "../AirLoopHVACZoneSplitter.hpp"

using namespace openstudio::model;

TEST_F(ModelFixture,BoilerHotWater_BoilerHotWater)
{
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  ASSERT_EXIT (
  {
     Model m;
     BoilerHotWater boiler(m);

     exit(0);
  } ,
    ::testing::ExitedWithCode(0), "" );
}

TEST_F(ModelFixture,BoilerHotWater_connections)
{
  Model m;
  BoilerHotWater boiler(m);

  Node inletNode(m);
  Node outletNode(m);

  m.connect(inletNode,inletNode.outletPort(),boiler,boiler.inletPort());
  m.connect(boiler,boiler.outletPort(),outletNode,outletNode.inletPort());

  ASSERT_TRUE( boiler.inletModelObject() );
  ASSERT_TRUE( boiler.outletModelObject() );

  EXPECT_EQ( inletNode.handle(), boiler.inletModelObject()->handle() );
  EXPECT_EQ( outletNode.handle(), boiler.outletModelObject()->handle() );
}

TEST_F(ModelFixture,BoilerHotWater_addToNode) {
  Model m;
  BoilerHotWater testObject(m);

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

  BoilerHotWater testObjectClone = testObject.clone(m).cast<BoilerHotWater>();
  supplyOutletNode = plantLoop.supplyOutletNode();

  EXPECT_TRUE(testObjectClone.addToNode(supplyOutletNode));
  EXPECT_EQ( (unsigned)9, plantLoop.supplyComponents().size() );
}

TEST_F(ModelFixture,BoilerHotWater_remove) {
  Model m;

  PlantLoop plant(m);
  BoilerHotWater b1(m);
  BoilerHotWater b2(m);
  EXPECT_FALSE(b1.loop());
  EXPECT_FALSE(b2.loop());

  EXPECT_EQ(plant.supplyComponents(Node::iddObjectType()).size(), 3u);

  EXPECT_TRUE(plant.addSupplyBranchForComponent(b1));
  EXPECT_GT(plant.supplyComponents(b1, plant.supplyOutletNode()).size(), 0u);
  EXPECT_TRUE(b1.loop());
  EXPECT_EQ(plant.supplyComponents(Node::iddObjectType()).size(), 4u);
  {
    auto node = b1.outletModelObject()->cast<Node>();
    EXPECT_TRUE(b2.addToNode(node));
    EXPECT_GT(plant.supplyComponents(b2, plant.supplyOutletNode()).size(), 0u);
    EXPECT_TRUE(b2.loop());
  }

  EXPECT_EQ(plant.supplyComponents(Node::iddObjectType()).size(), 5u);

  EXPECT_GT(plant.supplyComponents(b1, plant.supplyOutletNode()).size(), 0u);
  EXPECT_GT(plant.supplyComponents(b2, plant.supplyOutletNode()).size(), 0u);
  EXPECT_EQ(plant.demandComponents(b1, plant.demandOutletNode()).size(), 0u);
  EXPECT_EQ(plant.demandComponents(b2, plant.demandOutletNode()).size(), 0u);

  auto n1 = b1.outletModelObject().get();
  auto n2 = b2.outletModelObject().get();

  b1.remove();
  EXPECT_EQ(plant.supplyComponents(b1, plant.supplyOutletNode()).size(), 0u);
  EXPECT_GT(plant.supplyComponents(b2, plant.supplyOutletNode()).size(), 0u);
  EXPECT_EQ(plant.supplyComponents(Node::iddObjectType()).size(), 4u);
  b2.remove();
  EXPECT_EQ(plant.supplyComponents(b2, plant.supplyOutletNode()).size(), 0u);
  EXPECT_EQ(plant.supplyComponents(Node::iddObjectType()).size(), 3u);

  EXPECT_TRUE(n1.handle().isNull());
  EXPECT_TRUE(n2.handle().isNull());
}
