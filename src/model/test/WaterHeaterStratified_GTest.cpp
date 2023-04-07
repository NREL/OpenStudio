/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2023, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
*  following conditions are met:
*
*  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
*  disclaimer.
*
*  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
*  disclaimer in the documentation and/or other materials provided with the distribution.
*
*  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote products
*  derived from this software without specific prior written permission from the respective party.
*
*  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative works
*  may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without specific prior
*  written permission from Alliance for Sustainable Energy, LLC.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER(S) AND ANY CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
*  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
*  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER(S), ANY CONTRIBUTORS, THE UNITED STATES GOVERNMENT, OR THE UNITED
*  STATES DEPARTMENT OF ENERGY, NOR ANY OF THEIR EMPLOYEES, BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
*  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
*  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
*  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
*  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
***********************************************************************************************************************/

#include <gtest/gtest.h>
#include "ModelFixture.hpp"
#include "../WaterHeaterStratified.hpp"
#include "../WaterHeaterStratified_Impl.hpp"
#include "../WaterUseConnections.hpp"
#include "../WaterUseEquipment.hpp"
#include "../WaterUseEquipmentDefinition.hpp"
#include "../PlantLoop.hpp"
#include "../PlantLoop_Impl.hpp"
#include "../PipeAdiabatic.hpp"
#include "../PipeAdiabatic_Impl.hpp"
#include "../Node.hpp"
#include "../Node_Impl.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, WaterHeaterStratified_WaterHeaterStratified) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  ASSERT_EXIT(
    {
      Model m;
      WaterHeaterStratified waterHeaterStratified(m);
      exit(0);
    },
    ::testing::ExitedWithCode(0), "");
}

TEST_F(ModelFixture, WaterHeaterStratified_addToPlantLoop) {
  Model m;
  WaterHeaterStratified waterHeaterStratified(m);

  PlantLoop plantLoop(m);
  ASSERT_EQ(5u, plantLoop.supplyComponents().size());
  ASSERT_EQ(5u, plantLoop.demandComponents().size());

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
  ASSERT_EQ(5u, plantLoop.supplyComponents().size());
  ASSERT_EQ(7u, plantLoop.demandComponents().size());
}

TEST_F(ModelFixture, WaterHeaterStratified_PlantLoopConnections) {
  Model m;

  WaterHeaterStratified wh(m);

  PlantLoop p1(m);
  PlantLoop p2(m);

  // plant loop #1
  auto node1 = p1.supplyInletNode();
  EXPECT_TRUE(wh.addToNode(node1));

  EXPECT_EQ(1u, p1.supplyComponents(WaterHeaterStratified::iddObjectType()).size());
  EXPECT_EQ(0u, p2.supplyComponents(WaterHeaterStratified::iddObjectType()).size());

  ASSERT_TRUE(wh.supplyInletModelObject());
  ASSERT_TRUE(wh.supplyOutletModelObject());
  EXPECT_FALSE(wh.demandInletModelObject());
  EXPECT_FALSE(wh.demandOutletModelObject());

  EXPECT_EQ(p1.supplyInletNode().nameString(), wh.supplyInletModelObject()->cast<Node>().nameString());
  std::string useSideOutletNodeName = wh.supplyOutletModelObject()->cast<Node>().nameString();
  EXPECT_NE("", useSideOutletNodeName);

  ASSERT_TRUE(wh.plantLoop());
  EXPECT_EQ(wh.plantLoop()->handle(), p1.handle());
  ASSERT_TRUE(wh.useSidePlantLoop());
  EXPECT_EQ(wh.useSidePlantLoop()->handle(), p1.handle());
  EXPECT_FALSE(wh.sourceSidePlantLoop());

  // plant loop #2
  PipeAdiabatic bypass_pipe(m);
  p2.addSupplyBranchForComponent(bypass_pipe);
  ASSERT_TRUE(bypass_pipe.inletModelObject());
  ASSERT_TRUE(bypass_pipe.inletModelObject()->optionalCast<Node>());
  auto node2 = bypass_pipe.inletModelObject()->cast<Node>();
  EXPECT_TRUE(wh.addToSourceSideNode(node2));
  bypass_pipe.remove();

  ASSERT_TRUE(wh.plantLoop());
  EXPECT_EQ(wh.plantLoop()->handle(), p1.handle());
  ASSERT_TRUE(wh.useSidePlantLoop());
  EXPECT_EQ(wh.useSidePlantLoop()->handle(), p1.handle());
  ASSERT_TRUE(wh.sourceSidePlantLoop());
  EXPECT_NE(wh.plantLoop()->handle(), wh.sourceSidePlantLoop()->handle());
  EXPECT_EQ(wh.sourceSidePlantLoop()->handle(), p2.handle());

  EXPECT_EQ(1u, p1.supplyComponents(WaterHeaterStratified::iddObjectType()).size());
  EXPECT_EQ(1u, p2.supplyComponents(WaterHeaterStratified::iddObjectType()).size());

  ASSERT_TRUE(wh.supplyInletModelObject());
  ASSERT_TRUE(wh.supplyOutletModelObject());
  ASSERT_TRUE(wh.demandInletModelObject());
  ASSERT_TRUE(wh.demandOutletModelObject());

  EXPECT_EQ(p1.supplyInletNode().nameString(), wh.supplyInletModelObject()->cast<Node>().nameString());  // doesn't change
  EXPECT_EQ(useSideOutletNodeName, wh.supplyOutletModelObject()->cast<Node>().nameString());             // doesn't change
  EXPECT_NE(p2.supplyInletNode().nameString(), wh.demandInletModelObject()->cast<Node>().nameString());
  EXPECT_NE("", wh.demandOutletModelObject()->cast<Node>().nameString());

  EXPECT_TRUE(wh.removeFromSourceSidePlantLoop());
  EXPECT_FALSE(wh.sourceSidePlantLoop());
}

TEST_F(ModelFixture, WaterHeaterStratified_PlantLoopConnections_addToNodeOverride) {
  Model m;

  WaterHeaterStratified wh(m);

  PlantLoop p1(m);
  PlantLoop p2(m);

  // plant loop #1
  auto node1 = p1.supplyInletNode();
  EXPECT_TRUE(wh.addToNode(node1));

  EXPECT_EQ(1u, p1.supplyComponents(WaterHeaterStratified::iddObjectType()).size());
  EXPECT_EQ(0u, p2.supplyComponents(WaterHeaterStratified::iddObjectType()).size());

  ASSERT_TRUE(wh.supplyInletModelObject());
  ASSERT_TRUE(wh.supplyOutletModelObject());
  EXPECT_FALSE(wh.demandInletModelObject());
  EXPECT_FALSE(wh.demandOutletModelObject());

  EXPECT_EQ(p1.supplyInletNode().nameString(), wh.supplyInletModelObject()->cast<Node>().nameString());
  std::string useSideOutletNodeName = wh.supplyOutletModelObject()->cast<Node>().nameString();
  EXPECT_NE("", useSideOutletNodeName);

  ASSERT_TRUE(wh.plantLoop());
  EXPECT_EQ(wh.plantLoop()->handle(), p1.handle());
  ASSERT_TRUE(wh.useSidePlantLoop());
  EXPECT_EQ(wh.useSidePlantLoop()->handle(), p1.handle());
  EXPECT_FALSE(wh.sourceSidePlantLoop());

  // plant loop #2
  PipeAdiabatic bypass_pipe(m);
  p2.addSupplyBranchForComponent(bypass_pipe);
  ASSERT_TRUE(bypass_pipe.inletModelObject());
  ASSERT_TRUE(bypass_pipe.inletModelObject()->optionalCast<Node>());
  auto node2 = bypass_pipe.inletModelObject()->cast<Node>();
  EXPECT_TRUE(wh.addToNode(node2));  // try with addToNode instead
  bypass_pipe.remove();

  ASSERT_TRUE(wh.plantLoop());
  EXPECT_EQ(wh.plantLoop()->handle(), p1.handle());
  ASSERT_TRUE(wh.useSidePlantLoop());
  EXPECT_EQ(wh.useSidePlantLoop()->handle(), p1.handle());
  ASSERT_TRUE(wh.sourceSidePlantLoop());
  EXPECT_NE(wh.plantLoop()->handle(), wh.sourceSidePlantLoop()->handle());
  EXPECT_EQ(wh.sourceSidePlantLoop()->handle(), p2.handle());

  EXPECT_EQ(1u, p1.supplyComponents(WaterHeaterStratified::iddObjectType()).size());
  EXPECT_EQ(1u, p2.supplyComponents(WaterHeaterStratified::iddObjectType()).size());

  ASSERT_TRUE(wh.supplyInletModelObject());
  ASSERT_TRUE(wh.supplyOutletModelObject());
  ASSERT_TRUE(wh.demandInletModelObject());
  ASSERT_TRUE(wh.demandOutletModelObject());

  EXPECT_EQ(p1.supplyInletNode().nameString(), wh.supplyInletModelObject()->cast<Node>().nameString());  // doesn't change
  EXPECT_EQ(useSideOutletNodeName, wh.supplyOutletModelObject()->cast<Node>().nameString());             // doesn't change
  EXPECT_NE(p2.supplyInletNode().nameString(), wh.demandInletModelObject()->cast<Node>().nameString());
  EXPECT_NE("", wh.demandOutletModelObject()->cast<Node>().nameString());

  EXPECT_TRUE(wh.removeFromSourceSidePlantLoop());
  EXPECT_FALSE(wh.sourceSidePlantLoop());
}

TEST_F(ModelFixture, WaterHeaterStratified_PlantLoopConnections_addToNodeOverride_addSupplyBranch) {

  // Since addSupplyBranchForComponent calls addToNode, we should be able to call it twice in a row with two supply side nodes,
  // and connect it to the Use Side PlantLoop first and Source Side PlantLoop second
  Model m;

  WaterHeaterStratified wh(m);

  PlantLoop p1(m);
  PlantLoop p2(m);

  // plant loop #1
  EXPECT_TRUE(p1.addSupplyBranchForComponent(wh));

  EXPECT_EQ(1, p1.supplyComponents(WaterHeaterStratified::iddObjectType()).size());
  EXPECT_EQ(0, p2.supplyComponents(WaterHeaterStratified::iddObjectType()).size());

  ASSERT_TRUE(wh.supplyInletModelObject());
  ASSERT_TRUE(wh.supplyOutletModelObject());
  EXPECT_FALSE(wh.demandInletModelObject());
  EXPECT_FALSE(wh.demandOutletModelObject());

  std::string useSideOutletNodeName = wh.supplyOutletModelObject()->cast<Node>().nameString();
  EXPECT_NE("", useSideOutletNodeName);

  ASSERT_TRUE(wh.plantLoop());
  EXPECT_EQ(wh.plantLoop()->handle(), p1.handle());
  ASSERT_TRUE(wh.useSidePlantLoop());
  EXPECT_EQ(wh.useSidePlantLoop()->handle(), p1.handle());
  EXPECT_FALSE(wh.sourceSidePlantLoop());

  // plant loop #2
  EXPECT_TRUE(p2.addSupplyBranchForComponent(wh));

  ASSERT_TRUE(wh.plantLoop());
  EXPECT_EQ(wh.plantLoop()->handle(), p1.handle());
  ASSERT_TRUE(wh.useSidePlantLoop());
  EXPECT_EQ(wh.useSidePlantLoop()->handle(), p1.handle());

  ASSERT_TRUE(wh.secondaryPlantLoop());
  EXPECT_NE(wh.plantLoop()->handle(), wh.secondaryPlantLoop()->handle());
  EXPECT_EQ(wh.secondaryPlantLoop()->handle(), p2.handle());
  ASSERT_TRUE(wh.sourceSidePlantLoop());
  EXPECT_NE(wh.plantLoop()->handle(), wh.sourceSidePlantLoop()->handle());
  EXPECT_EQ(wh.sourceSidePlantLoop()->handle(), p2.handle());

  EXPECT_EQ(1, p1.supplyComponents(WaterHeaterStratified::iddObjectType()).size());
  EXPECT_EQ(1, p2.supplyComponents(WaterHeaterStratified::iddObjectType()).size());

  ASSERT_TRUE(wh.supplyInletModelObject());
  ASSERT_TRUE(wh.supplyOutletModelObject());
  ASSERT_TRUE(wh.demandInletModelObject());
  ASSERT_TRUE(wh.demandOutletModelObject());

  EXPECT_EQ(useSideOutletNodeName, wh.supplyOutletModelObject()->cast<Node>().nameString());  // doesn't change
  EXPECT_NE("", wh.demandOutletModelObject()->cast<Node>().nameString());

  EXPECT_TRUE(wh.removeFromSourceSidePlantLoop());
  EXPECT_FALSE(wh.sourceSidePlantLoop());
}
