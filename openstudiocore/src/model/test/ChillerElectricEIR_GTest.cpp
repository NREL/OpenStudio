/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2018, Alliance for Sustainable Energy, LLC. All rights reserved.
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
#include "../PlantLoop.hpp"
#include "../Model.hpp"
#include "../ChillerElectricEIR.hpp"
#include "../CurveBiquadratic.hpp"
#include "../CurveQuadratic.hpp"
#include "../Mixer.hpp"
#include "../Mixer_Impl.hpp"
#include "../Splitter.hpp"
#include "../Splitter_Impl.hpp"
#include "../Node.hpp"
#include "../Node_Impl.hpp"

using namespace openstudio;

TEST(ChillerElectricEIR,ChillerElectricEIR_ChillerElectricEIR)
{
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  ASSERT_EXIT (
  {
     model::Model m;

     model::CurveBiquadratic ccFofT(m);
     model::CurveBiquadratic eirToCorfOfT(m);
     model::CurveQuadratic eiToCorfOfPlr(m);

     model::ChillerElectricEIR chiller(m,ccFofT,eirToCorfOfT,eiToCorfOfPlr);

     exit(0);
  } ,
    ::testing::ExitedWithCode(0), "" );
}

// Add to the end of an empty supply side and check that it is placed correctly.
TEST(ChillerElectricEIR,ChillerElectricEIR_addToNode1)
{
  model::Model m;

  model::PlantLoop plantLoop(m);

  model::CurveBiquadratic ccFofT(m);
  model::CurveBiquadratic eirToCorfOfT(m);
  model::CurveQuadratic eiToCorfOfPlr(m);

  model::ChillerElectricEIR chiller(m,ccFofT,eirToCorfOfT,eiToCorfOfPlr);

  model::Node supplyOutletNode = plantLoop.supplyOutletNode();
  model::Mixer supplyMixer = plantLoop.supplyMixer();

  EXPECT_TRUE(chiller.addToNode(supplyOutletNode));

  EXPECT_EQ(7u,plantLoop.supplyComponents().size());

  ASSERT_TRUE(chiller.supplyOutletModelObject());

  EXPECT_EQ(supplyOutletNode,chiller.supplyOutletModelObject().get());

  boost::optional<model::ModelObject> mo = chiller.supplyInletModelObject();

  ASSERT_TRUE(mo);

  boost::optional<model::Node> node = mo->optionalCast<model::Node>();

  ASSERT_TRUE(node);

  mo = node->inletModelObject();

  ASSERT_TRUE(mo);

  boost::optional<model::Mixer> mixer = mo->optionalCast<model::Mixer>();

  ASSERT_TRUE(mixer);
}

// Add to the front of an empty supply side and check that it is placed correctly.
TEST(ChillerElectricEIR,ChillerElectricEIR_addToNode2)
{
  model::Model m;

  model::PlantLoop plantLoop(m);

  model::CurveBiquadratic ccFofT(m);
  model::CurveBiquadratic eirToCorfOfT(m);
  model::CurveQuadratic eiToCorfOfPlr(m);

  model::ChillerElectricEIR chiller(m,ccFofT,eirToCorfOfT,eiToCorfOfPlr);

  model::Node supplyInletNode = plantLoop.supplyInletNode();

  EXPECT_TRUE(chiller.addToNode(supplyInletNode));

  EXPECT_EQ(7u,plantLoop.supplyComponents().size());

  boost::optional<model::ModelObject> mo = chiller.supplyInletModelObject();

  ASSERT_TRUE(mo);

  ASSERT_EQ(supplyInletNode,mo.get());

  mo = chiller.supplyOutletModelObject();

  ASSERT_TRUE(mo);

  boost::optional<model::Node> node = mo->optionalCast<model::Node>();

  ASSERT_TRUE(node);

  mo = node->outletModelObject();

  ASSERT_TRUE(mo);

  ASSERT_EQ(plantLoop.supplySplitter(),mo.get());
}

// Add to the middle of the existing branch.
TEST(ChillerElectricEIR,ChillerElectricEIR_addToNode3)
{
  model::Model m;

  model::PlantLoop plantLoop(m);

  model::CurveBiquadratic ccFofT(m);
  model::CurveBiquadratic eirToCorfOfT(m);
  model::CurveQuadratic eiToCorfOfPlr(m);

  model::ChillerElectricEIR chiller(m,ccFofT,eirToCorfOfT,eiToCorfOfPlr);

  model::Mixer supplyMixer = plantLoop.supplyMixer();
  model::Splitter supplySplitter = plantLoop.supplySplitter();

  boost::optional<model::ModelObject> mo = supplyMixer.inletModelObject(0);

  ASSERT_TRUE(mo);

  boost::optional<model::Node> node = mo->optionalCast<model::Node>();

  ASSERT_TRUE(node);

  EXPECT_TRUE(chiller.addToNode(node.get()));

  EXPECT_EQ(7u,plantLoop.supplyComponents().size());

  mo = chiller.supplyInletModelObject();

  ASSERT_TRUE(mo);

  ASSERT_EQ(node.get(),mo.get());

  mo = node->inletModelObject();

  EXPECT_TRUE(mo);

  EXPECT_EQ(mo.get(),supplySplitter);

  mo = chiller.supplyOutletModelObject();

  ASSERT_TRUE(mo);

  node = mo->optionalCast<model::Node>();

  ASSERT_TRUE(node);

  mo = node->outletModelObject();

  ASSERT_TRUE(mo);

  ASSERT_EQ(supplyMixer,mo.get());

  mo = supplySplitter.outletModelObject(0);

  boost::optional<model::ModelObject> mo2 = supplyMixer.inletModelObject(0);

  ASSERT_TRUE(mo);
  ASSERT_TRUE(mo2);

  boost::optional<model::HVACComponent> comp = mo->optionalCast<model::HVACComponent>();
  boost::optional<model::HVACComponent> comp2 = mo2->optionalCast<model::HVACComponent>();

  ASSERT_TRUE(comp);
  ASSERT_TRUE(comp2);

  std::vector<model::ModelObject> comps = plantLoop.supplyComponents(comp.get(),comp2.get());

  ASSERT_EQ(3u,comps.size());
}

// Add to new branch
TEST(ChillerElectricEIR,PlantLoop_addSupplyBranch)
{
  model::Model m;

  model::PlantLoop plantLoop(m);

  model::CurveBiquadratic ccFofT(m);
  model::CurveBiquadratic eirToCorfOfT(m);
  model::CurveQuadratic eiToCorfOfPlr(m);

  model::ChillerElectricEIR chiller(m,ccFofT,eirToCorfOfT,eiToCorfOfPlr);

  ASSERT_TRUE(plantLoop.addSupplyBranchForComponent(chiller));

  EXPECT_EQ(7u,plantLoop.supplyComponents().size());

  model::Mixer supplyMixer = plantLoop.supplyMixer();

  boost::optional<model::ModelObject> mo = supplyMixer.inletModelObject(0);

  ASSERT_TRUE(mo);

  boost::optional<model::Node> node = mo->optionalCast<model::Node>();

  ASSERT_TRUE(node);

  mo = chiller.supplyOutletModelObject();

  ASSERT_TRUE(mo);

  ASSERT_EQ(node.get(),mo.get());

  mo = chiller.supplyOutletModelObject();

  ASSERT_TRUE(mo);

  node = mo->optionalCast<model::Node>();

  ASSERT_TRUE(node);

  mo = node->outletModelObject();

  ASSERT_TRUE(mo);

  ASSERT_EQ(supplyMixer,mo.get());
}

// Add to the end of an empty demand side and check that it is placed correctly.
TEST(ChillerElectricEIR,ChillerElectricEIR_addToDemandNode1)
{
  model::Model m;

  model::PlantLoop plantLoop(m);

  model::CurveBiquadratic ccFofT(m);
  model::CurveBiquadratic eirToCorfOfT(m);
  model::CurveQuadratic eiToCorfOfPlr(m);

  model::ChillerElectricEIR chiller(m,ccFofT,eirToCorfOfT,eiToCorfOfPlr);

  model::Node demandOutletNode = plantLoop.demandOutletNode();
  model::Mixer demandMixer = plantLoop.demandMixer();

  EXPECT_TRUE(chiller.addToNode(demandOutletNode));

  EXPECT_EQ(7u,plantLoop.demandComponents().size());

  ASSERT_TRUE(chiller.demandOutletModelObject());

  EXPECT_EQ(demandOutletNode,chiller.demandOutletModelObject().get());

  boost::optional<model::ModelObject> mo = chiller.demandInletModelObject();

  ASSERT_TRUE(mo);

  boost::optional<model::Node> node = mo->optionalCast<model::Node>();

  ASSERT_TRUE(node);

  mo = node->inletModelObject();

  ASSERT_TRUE(mo);

  boost::optional<model::Mixer> mixer = mo->optionalCast<model::Mixer>();

  ASSERT_TRUE(mixer);

  ASSERT_TRUE(chiller.remove().size() > 0);

  EXPECT_EQ(5u,plantLoop.demandComponents().size());
}

// Add to the front of an empty demand side and check that it is placed correctly.
TEST(ChillerElectricEIR,ChillerElectricEIR_addToDemandNode2)
{
  model::Model m;

  model::PlantLoop plantLoop(m);

  model::CurveBiquadratic ccFofT(m);
  model::CurveBiquadratic eirToCorfOfT(m);
  model::CurveQuadratic eiToCorfOfPlr(m);

  model::ChillerElectricEIR chiller(m,ccFofT,eirToCorfOfT,eiToCorfOfPlr);

  model::Node demandInletNode = plantLoop.demandInletNode();

  EXPECT_TRUE(chiller.addToNode(demandInletNode));

  EXPECT_EQ(7u,plantLoop.demandComponents().size());

  boost::optional<model::ModelObject> mo = chiller.demandInletModelObject();

  ASSERT_TRUE(mo);

  ASSERT_EQ(demandInletNode,mo.get());

  mo = chiller.demandOutletModelObject();

  ASSERT_TRUE(mo);

  boost::optional<model::Node> node = mo->optionalCast<model::Node>();

  ASSERT_TRUE(node);

  mo = node->outletModelObject();

  ASSERT_TRUE(mo);

  ASSERT_EQ(plantLoop.demandSplitter(),mo.get());

  ASSERT_TRUE(chiller.remove().size() > 0);

  EXPECT_EQ(5u,plantLoop.demandComponents().size());
}

// Add to the middle of the existing branch.
TEST(ChillerElectricEIR,ChillerElectricEIR_addToDemandNode3)
{
  model::Model m;

  model::PlantLoop plantLoop(m);

  model::CurveBiquadratic ccFofT(m);
  model::CurveBiquadratic eirToCorfOfT(m);
  model::CurveQuadratic eiToCorfOfPlr(m);

  model::ChillerElectricEIR chiller(m,ccFofT,eirToCorfOfT,eiToCorfOfPlr);

  model::Mixer demandMixer = plantLoop.demandMixer();

  boost::optional<model::ModelObject> mo = demandMixer.inletModelObject(0);

  ASSERT_TRUE(mo);

  boost::optional<model::Node> node = mo->optionalCast<model::Node>();

  ASSERT_TRUE(node);

  EXPECT_TRUE(chiller.addToNode(node.get()));

  EXPECT_EQ(7u,plantLoop.demandComponents().size());

  mo = chiller.demandInletModelObject();

  ASSERT_TRUE(mo);

  ASSERT_EQ(node.get(),mo.get());

  mo = chiller.demandOutletModelObject();

  ASSERT_TRUE(mo);

  node = mo->optionalCast<model::Node>();

  ASSERT_TRUE(node);

  mo = node->outletModelObject();

  ASSERT_TRUE(mo);

  ASSERT_EQ(demandMixer,mo.get());

  ASSERT_TRUE(chiller.remove().size() > 0);

  EXPECT_EQ(5u,plantLoop.demandComponents().size());
}

// Add to new demand branch
TEST(ChillerElectricEIR,PlantLoop_addDemandBranch)
{
  model::Model m;

  model::PlantLoop plantLoop(m);

  model::CurveBiquadratic ccFofT(m);
  model::CurveBiquadratic eirToCorfOfT(m);
  model::CurveQuadratic eiToCorfOfPlr(m);

  model::ChillerElectricEIR chiller(m,ccFofT,eirToCorfOfT,eiToCorfOfPlr);

  ASSERT_TRUE(plantLoop.addDemandBranchForComponent(chiller));

  EXPECT_EQ(7u,plantLoop.demandComponents().size());

  model::Mixer demandMixer = plantLoop.demandMixer();

  boost::optional<model::ModelObject> mo = demandMixer.inletModelObject(0);

  ASSERT_TRUE(mo);

  boost::optional<model::Node> node = mo->optionalCast<model::Node>();

  ASSERT_TRUE(node);

  mo = chiller.demandOutletModelObject();

  ASSERT_TRUE(mo);

  ASSERT_EQ(node.get(),mo.get());

  mo = chiller.demandOutletModelObject();

  ASSERT_TRUE(mo);

  node = mo->optionalCast<model::Node>();

  ASSERT_TRUE(node);

  mo = node->outletModelObject();

  ASSERT_TRUE(mo);

  ASSERT_EQ(demandMixer,mo.get());

  ASSERT_TRUE(plantLoop.removeDemandBranchWithComponent(chiller));

  EXPECT_EQ(5u,plantLoop.demandComponents().size());
}

// Check condenser type setting/defaulting
TEST(ChillerElectricEIR, ChillerElectricEIR_CondenserType)
{
  model::Model m;

  model::PlantLoop pl1(m);
  model::PlantLoop pl2(m);

  model::ChillerElectricEIR ch(m);

  // By default, AirCooled (from IDD)
  EXPECT_EQ("AirCooled", ch.condenserType());

  // Not connected to a secondary plantLoop
  ASSERT_TRUE(ch.setCondenserType("EvaporativelyCooled"));
  ASSERT_EQ("EvaporativelyCooled", ch.condenserType());

  ASSERT_TRUE(ch.setCondenserType("AirCooled"));
  ASSERT_EQ("AirCooled", ch.condenserType());

  ASSERT_FALSE(ch.setCondenserType("WaterCooled"));

  // Add to primary plant loop (on supply), behavior should be the same
  ASSERT_TRUE(pl1.addSupplyBranchForComponent(ch));
  // Should have stayed
  ASSERT_EQ("AirCooled", ch.condenserType());
  ASSERT_TRUE(ch.setCondenserType("EvaporativelyCooled"));
  ASSERT_EQ("EvaporativelyCooled", ch.condenserType());

  ASSERT_TRUE(ch.setCondenserType("AirCooled"));
  ASSERT_EQ("AirCooled", ch.condenserType());

  ASSERT_FALSE(ch.setCondenserType("WaterCooled"));


  // Add to the Secondary plant loop (on demand), behavior should be reversed
  ASSERT_TRUE(pl2.addDemandBranchForComponent(ch));
  // Should have been automatically set to WaterCooled
  ASSERT_EQ("WaterCooled", ch.condenserType());

  ASSERT_FALSE(ch.setCondenserType("AirCooled"));
  ASSERT_FALSE(ch.setCondenserType("EvaporativelyCooled"));

  // Disconnect from the secondary plant Loop
  ASSERT_TRUE(ch.removeFromSecondaryPlantLoop());
  // Should have been automatically switched to AirCooled
  ASSERT_EQ("AirCooled", ch.condenserType());

  ASSERT_TRUE(ch.setCondenserType("EvaporativelyCooled"));
  ASSERT_EQ("EvaporativelyCooled", ch.condenserType());

  ASSERT_TRUE(ch.setCondenserType("AirCooled"));
  ASSERT_EQ("AirCooled", ch.condenserType());

  ASSERT_FALSE(ch.setCondenserType("WaterCooled"));

}
