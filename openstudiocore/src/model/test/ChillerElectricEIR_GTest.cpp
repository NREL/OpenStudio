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


