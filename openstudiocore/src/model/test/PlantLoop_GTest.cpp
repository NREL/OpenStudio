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
#include "../PlantLoop.hpp"
#include "../PlantLoop_Impl.hpp"
#include "../Node.hpp"
#include "../Node_Impl.hpp"
#include "../Loop.hpp"
#include "../ConnectorSplitter.hpp"
#include "../ConnectorSplitter_Impl.hpp"
#include "../ConnectorMixer.hpp"
#include "../ConnectorMixer_Impl.hpp"
#include "../ChillerElectricEIR.hpp"
#include "../ChillerElectricEIR_Impl.hpp"
#include "../PumpVariableSpeed.hpp"
#include "../PipeAdiabatic.hpp"
#include "../CurveBiquadratic.hpp"
#include "../CurveQuadratic.hpp"
#include "../CoilHeatingWater.hpp"
#include "../CoilHeatingWater_Impl.hpp"
#include "../CoilCoolingWater.hpp"
#include "../ScheduleCompact.hpp"
#include "../LifeCycleCost.hpp"
#include "../HVACTemplates.hpp"
#include "../AirLoopHVAC.hpp"
#include "../AirLoopHVAC_Impl.hpp"
#include "../PlantEquipmentOperationCoolingLoad.hpp"
#include "../PlantEquipmentOperationCoolingLoad_Impl.hpp"
#include "../PlantEquipmentOperationHeatingLoad.hpp"
#include "../PlantEquipmentOperationHeatingLoad_Impl.hpp"
#include "../PlantEquipmentOperationOutdoorDryBulb.hpp"
#include "../PlantEquipmentOperationOutdoorDryBulb_Impl.hpp"
#include "../AvailabilityManagerDifferentialThermostat.hpp"
#include "../AvailabilityManagerDifferentialThermostat_Impl.hpp"

#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

TEST_F(ModelFixture,PlantLoop_PlantLoop)
{
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  ASSERT_EXIT ( 
  {  
     Model m; 
     PlantLoop plantLoop(m); 

     exit(0); 
  } ,
    ::testing::ExitedWithCode(0), "" );
}

TEST_F(ModelFixture,PlantLoop_Remove)
{
  Model m; 
  auto size = m.modelObjects().size();
  PlantLoop plantLoop(m); 

  EXPECT_FALSE(plantLoop.remove().empty());

  EXPECT_EQ(size,m.modelObjects().size());
}

TEST_F(ModelFixture,PlantLoop_supplyComponents)
{
  Model m; 

  // Empty Plant Loop
  
  PlantLoop plantLoop(m); 
  ASSERT_EQ( 5u,plantLoop.supplyComponents().size() );

  EXPECT_EQ("Optimal",plantLoop.loadDistributionScheme());

  boost::optional<ModelObject> comp;
  comp = plantLoop.supplyComponents()[1];
  ASSERT_TRUE(comp);
  ASSERT_EQ(openstudio::IddObjectType::OS_Connector_Splitter,comp->iddObjectType().value());

  ConnectorSplitter splitter = comp->cast<ConnectorSplitter>();
  comp = splitter.lastOutletModelObject();
  ASSERT_TRUE(comp);
  ASSERT_EQ(openstudio::IddObjectType::OS_Node,comp->iddObjectType().value());

  Node connectorNode = comp->cast<Node>();
  comp = connectorNode.outletModelObject();
  ASSERT_TRUE(comp);
  ASSERT_EQ(openstudio::IddObjectType::OS_Connector_Mixer,comp->iddObjectType().value());

  ConnectorMixer mixer = comp->cast<ConnectorMixer>();
  comp = mixer.outletModelObject();
  ASSERT_TRUE(comp);
  Node supplyOutletNode = plantLoop.supplyOutletNode();
  ASSERT_EQ(comp->handle(),supplyOutletNode.handle());

  // Add a new component

  CurveBiquadratic ccFofT(m);
  CurveBiquadratic eirToCorfOfT(m);
  CurveQuadratic eiToCorfOfPlr(m);
  
  ChillerElectricEIR chiller(m,ccFofT,eirToCorfOfT,eiToCorfOfPlr);
  ASSERT_TRUE(chiller.addToNode(supplyOutletNode));
  ASSERT_EQ( 7u,plantLoop.supplyComponents().size() );

  // Add a new supply branch

  ChillerElectricEIR chiller2 = chiller.clone(m).cast<ChillerElectricEIR>();

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

TEST_F(ModelFixture,PlantLoop_demandComponent)
{
  Model m; 
  PlantLoop plantLoop(m); 

  ASSERT_EQ( 1u,plantLoop.demandInletNodes().size() );

  auto demandInletNode = plantLoop.demandInletNode();
  auto mo = plantLoop.demandComponent(demandInletNode.handle());
  ASSERT_TRUE(mo);
  EXPECT_EQ( demandInletNode,mo.get() );
}

TEST_F(ModelFixture,PlantLoop_demandComponents)
{
  Model m; 
  PlantLoop plantLoop(m); 
  ASSERT_EQ( 5u,plantLoop.demandComponents().size() );

  Schedule s = m.alwaysOnDiscreteSchedule();

  CoilHeatingWater coil(m,s);
  plantLoop.addDemandBranchForComponent(coil);
  ASSERT_EQ( 7u,plantLoop.demandComponents().size() );

  CoilHeatingWater coil2(m,s);
  plantLoop.addDemandBranchForComponent(coil2);
  ASSERT_EQ( 10u,plantLoop.demandComponents().size() );

  Splitter splitter = plantLoop.demandSplitter();
  ASSERT_EQ( 3u,plantLoop.demandComponents(splitter,coil).size() );
  ASSERT_EQ( 3u,plantLoop.demandComponents(splitter,coil2).size() );

  Mixer mixer = plantLoop.demandMixer();
  ASSERT_EQ( 3u,plantLoop.demandComponents(coil,mixer).size() );
  ASSERT_EQ( 3u,plantLoop.demandComponents(coil2,mixer).size() );
}

TEST_F(ModelFixture,PlantLoop_addDemandBranchForComponent)
{
  Model m; 
  ScheduleCompact s(m);
  PlantLoop plantLoop(m); 
  CoilHeatingWater heatingCoil(m,s);
  CoilHeatingWater heatingCoil2(m,s);
  CoilCoolingWater coolingCoil(m,s);

  EXPECT_TRUE(plantLoop.addDemandBranchForComponent(heatingCoil));

  boost::optional<ModelObject> inletModelObject = heatingCoil.waterInletModelObject();
  boost::optional<ModelObject> outletModelObject = heatingCoil.waterOutletModelObject();
  ASSERT_TRUE( inletModelObject );
  ASSERT_TRUE( outletModelObject );

  boost::optional<Node> inletNode = inletModelObject->optionalCast<Node>();
  boost::optional<Node> outletNode = outletModelObject->optionalCast<Node>();
  ASSERT_TRUE( inletNode );
  ASSERT_TRUE( outletNode );

  boost::optional<ModelObject> inletModelObject2 = inletNode->inletModelObject();
  boost::optional<ModelObject> outletModelObject2 = outletNode->outletModelObject();
  ASSERT_TRUE( inletModelObject2 );
  ASSERT_TRUE( outletModelObject2 );

  ASSERT_EQ( (unsigned)7,plantLoop.demandComponents().size() );

  EXPECT_TRUE(plantLoop.addDemandBranchForComponent(heatingCoil2));
  ASSERT_EQ( (unsigned)10,plantLoop.demandComponents().size() );

  EXPECT_TRUE(plantLoop.addDemandBranchForComponent(coolingCoil));
  ASSERT_EQ( (unsigned)13,plantLoop.demandComponents().size() );
}

TEST_F(ModelFixture,PlantLoop_removeDemandBranchWithComponent)
{
  Model m; 
  PlantLoop plantLoop(m); 
  ScheduleCompact s(m);
  CoilHeatingWater heatingCoil(m,s);

  EXPECT_TRUE(plantLoop.addDemandBranchForComponent(heatingCoil));
  ASSERT_EQ( (unsigned)7,plantLoop.demandComponents().size() );

  CoilHeatingWater heatingCoil2(m,s);

  EXPECT_TRUE(plantLoop.addDemandBranchForComponent(heatingCoil2));
  ASSERT_EQ( (unsigned)10,plantLoop.demandComponents().size() );

  Splitter splitter = plantLoop.demandSplitter();

  ASSERT_EQ( (unsigned)2,splitter.nextBranchIndex() );
  std::vector<ModelObject> modelObjects = plantLoop.demandComponents(splitter,heatingCoil2);
  ASSERT_EQ( (unsigned)3,modelObjects.size() );

  EXPECT_TRUE(plantLoop.removeDemandBranchWithComponent(heatingCoil2));
  ASSERT_EQ( (unsigned)1,splitter.nextBranchIndex() );
}

TEST_F(ModelFixture,PlantLoop_Cost)
{
  Model m; 
  PlantLoop plantLoop(m); 

  boost::optional<LifeCycleCost> cost = LifeCycleCost::createLifeCycleCost("Install", plantLoop, 1000.0, "CostPerEach", "Construction");
  ASSERT_TRUE(cost);

  EXPECT_DOUBLE_EQ(1000.0, cost->totalCost());
}

TEST_F(ModelFixture, PlantLoop_edges)
{
  Model m;

  PlantLoop plantLoop(m);
  PlantLoop plantLoop2(m);
  ScheduleCompact s(m);

  // supply components
  CurveBiquadratic ccFofT(m);
  CurveBiquadratic eirToCorfOfT(m);
  CurveQuadratic eiToCorfOfPlr(m);

  PumpVariableSpeed pump(m);
  ChillerElectricEIR chiller(m, ccFofT, eirToCorfOfT, eiToCorfOfPlr);
  PipeAdiabatic pipe1(m);
  PipeAdiabatic pipe2(m);

  Node plantOutletNode = plantLoop.supplyOutletNode();
  Node plantInletNode = plantLoop.supplyInletNode();

  Splitter supplySplitter = plantLoop.supplySplitter();
  Mixer supplyMixer = plantLoop.supplyMixer();

  pump.addToNode(plantInletNode);
  plantLoop.addSupplyBranchForComponent(chiller);
  plantLoop.addSupplyBranchForComponent(pipe1);
  pipe2.addToNode(plantOutletNode);

  // demand components
  CoilCoolingWater coil1(m, s);
  CoilCoolingWater coil2(m, s);
  plantLoop.addDemandBranchForComponent(coil1);
  plantLoop.addDemandBranchForComponent(coil2);

  Splitter demandSplitter = plantLoop.demandSplitter();
  Splitter demandSplitter2 = plantLoop2.demandSplitter();

  plantLoop2.addDemandBranchForComponent(chiller);

  boost::optional<ModelObject> splitter_demand_obj = plantLoop.demandComponent(demandSplitter.handle());
  ASSERT_TRUE(splitter_demand_obj);
  EXPECT_EQ(demandSplitter, *splitter_demand_obj);
  //std::vector<HVACComponent> edges = demandSplitter.getImpl<detail::HVACComponent_Impl>()->edges(true); // should be nodes
  //EXPECT_EQ(2, edges.size());
  //bool found_coil_1 = false;
  //bool found_coil_2 = false;
  //for( std::vector<HVACComponent>::iterator it = edges.begin(); it != edges.end(); ++it )
  //{
  //  std::vector<HVACComponent> splitter_edges = (*it).getImpl<detail::HVACComponent_Impl>()->edges(false); // should be a coil
  //  ASSERT_EQ(1, splitter_edges.size());
  //  if( coil1 == splitter_edges[0] ) {
  //    found_coil_1 = true;
  //  }
  //  else if( coil2 == splitter_edges[0] ) {
  //    found_coil_2 = true;
  //  }
  //}
  //EXPECT_TRUE(found_coil_1);
  //EXPECT_TRUE(found_coil_2);

  //boost::optional<ModelObject> pump_obj = plantLoop.supplyComponent(pump.handle());
  //ASSERT_TRUE(pump_obj);
  //EXPECT_EQ(pump, *pump_obj);
  //edges = pump.getImpl<detail::HVACComponent_Impl>()->edges(false); // should be Node
  //ASSERT_EQ(1, edges.size());
  //edges = edges[0].getImpl<detail::HVACComponent_Impl>()->edges(false); // should be Splitter
  //ASSERT_EQ(1, edges.size());
  //EXPECT_EQ(supplySplitter, edges[0]);

  //boost::optional<ModelObject> splitter_supply_obj = plantLoop.supplyComponent(supplySplitter.handle());
  //ASSERT_TRUE(splitter_supply_obj);
  //EXPECT_EQ(supplySplitter, *splitter_supply_obj);
  //edges = supplySplitter.getImpl<detail::HVACComponent_Impl>()->edges(false); // should be nodes
  //EXPECT_EQ(2, edges.size());
  //bool found_chiller = false;
  //bool found_pipe = false;
  //for( std::vector<HVACComponent>::iterator it = edges.begin(); it != edges.end(); ++it )
  //{
  //  std::vector<HVACComponent> splitter_edges = (*it).getImpl<detail::HVACComponent_Impl>()->edges(false); // should be chiller or pipe
  //  ASSERT_EQ(1, splitter_edges.size());
  //  if( chiller == splitter_edges[0] ) {
  //    found_chiller = true;
  //  }
  //  else if( pipe1 == splitter_edges[0] ) {
  //    found_pipe = true;
  //  }
  //}
  //EXPECT_TRUE(found_chiller);
  //EXPECT_TRUE(found_pipe);

  //boost::optional<ModelObject> supply_mixer_obj = plantLoop.supplyComponent(supplyMixer.handle());
  //ASSERT_TRUE(supply_mixer_obj);
  //EXPECT_EQ(supplyMixer, *supply_mixer_obj);
  //edges = supplyMixer.getImpl<detail::HVACComponent_Impl>()->edges(false); // should be Node
  //ASSERT_EQ(1, edges.size());
  //edges = edges[0].getImpl<detail::HVACComponent_Impl>()->edges(false); // should be Pipe
  //ASSERT_EQ(1, edges.size());
  //EXPECT_EQ(pipe2, edges[0]);

  //boost::optional<ModelObject> splitter2_demand_obj = plantLoop2.demandComponent(demandSplitter2.handle());
  //ASSERT_TRUE(splitter2_demand_obj);
  //EXPECT_EQ(demandSplitter2, *splitter2_demand_obj);
  //edges = demandSplitter2.getImpl<detail::HVACComponent_Impl>()->edges(true); // should be node
  //EXPECT_EQ(1, edges.size());
  //bool found_demand_chiller = false;
  //for( std::vector<HVACComponent>::iterator it = edges.begin(); it != edges.end(); ++it )
  //{
  //  std::vector<HVACComponent> splitter_edges = (*it).getImpl<detail::HVACComponent_Impl>()->edges(true); // should be chiller
  //  ASSERT_EQ(1, splitter_edges.size());
  //  if( chiller == splitter_edges[0] ) {
  //    found_demand_chiller = true;
  //  }
  //}
  //EXPECT_TRUE(found_demand_chiller);
}

TEST_F(ModelFixture, PlantLoop_removeBranchWithComponent)
{
  Model m;
  auto airSystem = addSystemType5(m).cast<AirLoopHVAC>();

  auto coil = airSystem.supplyComponents(CoilHeatingWater::iddObjectType()).front().cast<CoilHeatingWater>();
  auto plant = coil.plantLoop().get();

  EXPECT_TRUE(plant.removeDemandBranchWithComponent(coil));

  auto coilFromAirSystem = airSystem.supplyComponent(coil.handle());
  EXPECT_TRUE(coilFromAirSystem);

  auto coilFromPlant = plant.demandComponent(coil.handle());
  EXPECT_FALSE(coilFromPlant);

  auto plantDemandComps = plant.demandComponents();
  EXPECT_EQ(7u,plantDemandComps.size());

  auto splitter = plant.demandSplitter();
  auto mixer = plant.demandMixer();

  EXPECT_EQ(1u,splitter.outletModelObjects().size());
  EXPECT_EQ(1u,mixer.inletModelObjects().size());

  plantDemandComps = plant.demandComponents(splitter,mixer);
  EXPECT_EQ(5u,plantDemandComps.size());
}

TEST_F(ModelFixture, PlantLoop_OperationSchemes)
{
  Model m;
  PlantLoop plant(m);

  PlantEquipmentOperationCoolingLoad plantEquipmentOperationCoolingLoad(m);
  EXPECT_TRUE(plant.setPlantEquipmentOperationCoolingLoad(plantEquipmentOperationCoolingLoad));
  auto coolingLoad = plant.plantEquipmentOperationCoolingLoad();
  EXPECT_TRUE(coolingLoad);
  if( coolingLoad ) {
    EXPECT_EQ(plantEquipmentOperationCoolingLoad,coolingLoad.get());
  }

  PlantEquipmentOperationHeatingLoad plantEquipmentOperationHeatingLoad(m);
  EXPECT_TRUE(plant.setPlantEquipmentOperationHeatingLoad(plantEquipmentOperationHeatingLoad));
  auto heatingLoad = plant.plantEquipmentOperationHeatingLoad();
  EXPECT_TRUE(heatingLoad);
  if( heatingLoad ) {
    EXPECT_EQ(plantEquipmentOperationHeatingLoad,heatingLoad.get());
  }

  PlantEquipmentOperationOutdoorDryBulb plantEquipmentOperationOutdoorDryBulb(m);
  EXPECT_TRUE(plant.setPrimaryPlantEquipmentOperationScheme(plantEquipmentOperationOutdoorDryBulb));
  auto dryBulb = plant.primaryPlantEquipmentOperationScheme();
  EXPECT_TRUE(dryBulb);
  if( dryBulb ) {
    EXPECT_EQ(plantEquipmentOperationOutdoorDryBulb,dryBulb.get());
  }
  
}

TEST_F(ModelFixture, PlantLoop_GlycolConcentration) {
  Model m;
  PlantLoop plant(m);

  EXPECT_TRUE(plant.setFluidType("PropyleneGlycol"));
  EXPECT_EQ(plant.fluidType(), "PropyleneGlycol");
  plant.setGlycolConcentration(50);
  EXPECT_EQ(plant.glycolConcentration(), 50);
}

TEST_F(ModelFixture, PlantLoop_AvailabilityManager) {
  Model m;
  PlantLoop plant(m);
  AvailabilityManagerDifferentialThermostat availMgr(m);

  EXPECT_FALSE(plant.availabilityManager());
  EXPECT_TRUE(plant.setAvailabilityManager(availMgr));
  OptionalAvailabilityManager availMgr2 = plant.availabilityManager();
  EXPECT_TRUE(availMgr2);
  if (availMgr2) {
    EXPECT_EQ(availMgr2.get(), availMgr);
  }
  PlantLoop plant2 = plant.clone(m).cast<PlantLoop>();
  EXPECT_TRUE(plant2.availabilityManager());
  plant.resetAvailabilityManager();
  EXPECT_FALSE(plant.availabilityManager());
}
