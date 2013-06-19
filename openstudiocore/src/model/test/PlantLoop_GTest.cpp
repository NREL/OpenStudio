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
#include <model/PlantLoop.hpp>
#include <model/Model.hpp>
#include <model/Node.hpp>
#include <model/Node_Impl.hpp>
#include <model/Loop.hpp>
#include <model/Loop_Impl.hpp>
#include <model/ConnectorSplitter.hpp>
#include <model/ConnectorSplitter_Impl.hpp>
#include <model/ConnectorMixer.hpp>
#include <model/ConnectorMixer_Impl.hpp>
#include <model/ChillerElectricEIR.hpp>
#include <model/ChillerElectricEIR_Impl.hpp>
#include <model/CurveBiquadratic.hpp>
#include <model/CurveQuadratic.hpp>
#include <model/CoilHeatingWater.hpp>
#include <model/CoilCoolingWater.hpp>
#include <model/ScheduleCompact.hpp>
#include <model/LifeCycleCost.hpp>

//#include <utilities/idd/IddEnums.hxx>

using namespace openstudio;

TEST(PlantLoop,PlantLoop_PlantLoop)
{
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  ASSERT_EXIT ( 
  {  
     model::Model m; 

     model::PlantLoop plantLoop(m); 

     exit(0); 
  } ,
    ::testing::ExitedWithCode(0), "" );
}

TEST(PlantLoop,PlantLoop_supplyComponents)
{
  model::Model m; 

  // Empty Plant Loop
  
  model::PlantLoop plantLoop(m); 

  ASSERT_EQ( 5u,plantLoop.supplyComponents().size() );

  boost::optional<model::ModelObject> comp;

  comp = plantLoop.supplyComponents()[1];

  ASSERT_TRUE(comp);

  ASSERT_EQ(IddObjectType::OS_Connector_Splitter,comp->iddObjectType().value());

  model::ConnectorSplitter splitter = comp->cast<model::ConnectorSplitter>();

  comp = splitter.lastOutletModelObject();

  ASSERT_TRUE(comp);

  ASSERT_EQ(IddObjectType::OS_Node,comp->iddObjectType().value());

  model::Node connectorNode = comp->cast<model::Node>();

  comp = connectorNode.outletModelObject();

  ASSERT_TRUE(comp);

  ASSERT_EQ(IddObjectType::OS_Connector_Mixer,comp->iddObjectType().value());

  model::ConnectorMixer mixer = comp->cast<model::ConnectorMixer>();

  comp = mixer.outletModelObject();

  ASSERT_TRUE(comp);

  model::Node supplyOutletNode = plantLoop.supplyOutletNode();

  ASSERT_EQ(comp->handle(),supplyOutletNode.handle());

  // Add a new component

  model::CurveBiquadratic ccFofT(m);
  model::CurveBiquadratic eirToCorfOfT(m);
  model::CurveQuadratic eiToCorfOfPlr(m);
  
  model::ChillerElectricEIR chiller(m,ccFofT,eirToCorfOfT,eiToCorfOfPlr);

  ASSERT_TRUE(chiller.addToNode(supplyOutletNode));

  ASSERT_EQ( 7u,plantLoop.supplyComponents().size() );

  // Add a new supply branch

  model::ChillerElectricEIR chiller2 = chiller.clone(m).cast<model::ChillerElectricEIR>();

  ASSERT_EQ( 1u,splitter.nextBranchIndex() );
  ASSERT_EQ( 1u,mixer.nextBranchIndex() );

  ASSERT_TRUE(plantLoop.addSupplyBranchForComponent(chiller2));

  ASSERT_EQ( 1u,splitter.nextBranchIndex() );
  ASSERT_EQ( 1u,mixer.nextBranchIndex() );

  ASSERT_EQ( 1u,splitter.outletModelObjects().size() );

  ASSERT_EQ( 9u,plantLoop.supplyComponents().size() );

  // Remove the new supply branch

  ASSERT_TRUE(plantLoop.removeSupplyBranchWithComponent(chiller2));

  ASSERT_EQ( 7u,plantLoop.supplyComponents().size() );
}

TEST(PlantLoop,PlantLoop_demandComponents)
{
  model::Model m; 
  
  model::PlantLoop plantLoop(m); 

  ASSERT_EQ( 5u,plantLoop.demandComponents().size() );
}

TEST(PlantLoop,PlantLoop_addDemandBranchForComponent)
{
  model::Model m; 

  model::ScheduleCompact s(m);
  
  model::PlantLoop plantLoop(m); 

  model::CoilHeatingWater heatingCoil(m,s);

  model::CoilHeatingWater heatingCoil2(m,s);

  model::CoilCoolingWater coolingCoil(m,s);

  EXPECT_TRUE(plantLoop.addDemandBranchForComponent(heatingCoil));

  boost::optional<model::ModelObject> inletModelObject = heatingCoil.waterInletModelObject();

  boost::optional<model::ModelObject> outletModelObject = heatingCoil.waterOutletModelObject();

  ASSERT_TRUE( inletModelObject );

  ASSERT_TRUE( outletModelObject );

  boost::optional<model::Node> inletNode = inletModelObject->optionalCast<model::Node>();

  boost::optional<model::Node> outletNode = outletModelObject->optionalCast<model::Node>();

  ASSERT_TRUE( inletNode );

  ASSERT_TRUE( outletNode );

  boost::optional<model::ModelObject> inletModelObject2 = inletNode->inletModelObject();

  boost::optional<model::ModelObject> outletModelObject2 = outletNode->outletModelObject();

  ASSERT_TRUE( inletModelObject2 );

  ASSERT_TRUE( outletModelObject2 );

  ASSERT_EQ( (unsigned)7,plantLoop.demandComponents().size() );

  EXPECT_TRUE(plantLoop.addDemandBranchForComponent(heatingCoil2));

  ASSERT_EQ( (unsigned)10,plantLoop.demandComponents().size() );

  EXPECT_TRUE(plantLoop.addDemandBranchForComponent(coolingCoil));

  ASSERT_EQ( (unsigned)13,plantLoop.demandComponents().size() );
}

TEST(PlantLoop,PlantLoop_removeDemandBranchWithComponent)
{
  model::Model m; 
  
  model::PlantLoop plantLoop(m); 

  model::ScheduleCompact s(m);

  model::CoilHeatingWater heatingCoil(m,s);

  EXPECT_TRUE(plantLoop.addDemandBranchForComponent(heatingCoil));

  ASSERT_EQ( (unsigned)7,plantLoop.demandComponents().size() );

  model::CoilHeatingWater heatingCoil2(m,s);

  EXPECT_TRUE(plantLoop.addDemandBranchForComponent(heatingCoil2));

  ASSERT_EQ( (unsigned)10,plantLoop.demandComponents().size() );

  model::Splitter splitter = plantLoop.demandSplitter();

  ASSERT_EQ( (unsigned)2,splitter.nextBranchIndex() );

  std::vector<model::ModelObject> modelObjects = plantLoop.demandComponents(splitter,heatingCoil2);

  ASSERT_EQ( (unsigned)3,modelObjects.size() );

  EXPECT_TRUE(plantLoop.removeDemandBranchWithComponent(heatingCoil2));

  ASSERT_EQ( (unsigned)1,splitter.nextBranchIndex() );
}

TEST(PlantLoop,PlantLoop_Cost)
{
  model::Model m; 
  
  model::PlantLoop plantLoop(m); 

  boost::optional<model::LifeCycleCost> cost = model::LifeCycleCost::createLifeCycleCost("Install", plantLoop, 1000.0, "CostPerEach", "Construction");
  ASSERT_TRUE(cost);

  EXPECT_DOUBLE_EQ(1000.0, cost->totalCost());
}