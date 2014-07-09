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
#include "../AirLoopHVAC.hpp"
#include "../AirLoopHVAC_Impl.hpp"
#include "../AirLoopHVACSupplyPlenum.hpp"
#include "../AirLoopHVACReturnPlenum.hpp"
#include "../CoilHeatingWater.hpp"
#include "../CoilCoolingWater.hpp"
#include "../ControllerWaterCoil.hpp"
#include "../AirLoopHVACOutdoorAirSystem.hpp"
#include "../ControllerOutdoorAir.hpp"
#include "../Node.hpp"
#include "../Node_Impl.hpp"
#include "../AirLoopHVACZoneMixer.hpp"
#include "../AirLoopHVACZoneMixer_Impl.hpp"
#include "../AirLoopHVACZoneSplitter.hpp"
#include "../AirLoopHVACZoneSplitter_Impl.hpp"
#include "../AirTerminalSingleDuctUncontrolled.hpp"
#include "../AirTerminalSingleDuctUncontrolled_Impl.hpp"
#include "../ThermalZone.hpp"
#include "../ScheduleCompact.hpp"
#include "../ScheduleTypeLimits.hpp"
#include "../FanConstantVolume.hpp"
#include "../SizingSystem.hpp"
#include "../SizingSystem_Impl.hpp"
#include "../CoilHeatingElectric.hpp"
#include "../SetpointManagerSingleZoneReheat.hpp"
#include "../HVACComponent.hpp"
#include "../HVACComponent_Impl.hpp"
#include "../HVACTemplates.hpp"
#include "../LifeCycleCost.hpp"

using namespace openstudio::model;

TEST_F(ModelFixture,AirLoopHVAC_AirLoopHVAC)
{
  Model model = Model();
  OptionalModelObject modelObject;

  AirLoopHVAC airLoopHVAC = AirLoopHVAC(model);
  ASSERT_EQ(airLoopHVAC.iddObject().name(),"OS:AirLoopHVAC");

  OptionalNode supplyInletNode = airLoopHVAC.supplyInletNode();
  OptionalNode demandOutletNode = airLoopHVAC.demandOutletNode();
  std::vector<Node> supplyOutletNodes = airLoopHVAC.supplyOutletNodes();
  std::vector<Node> demandInletNodes = airLoopHVAC.demandInletNodes();

  ASSERT_TRUE( supplyInletNode );
  ASSERT_TRUE( demandOutletNode );
  ASSERT_EQ( unsigned(1), supplyOutletNodes.size() );
  ASSERT_EQ( unsigned(1), demandInletNodes.size() );

  OptionalNode supplyOutletNode = OptionalNode(supplyOutletNodes.front());
  OptionalNode demandInletNode = OptionalNode(demandInletNodes.front());

  modelObject = demandInletNode->outletModelObject();
  ASSERT_TRUE( modelObject );

  OptionalAirLoopHVACZoneSplitter splitter;
  splitter = modelObject->optionalCast<AirLoopHVACZoneSplitter>();
  ASSERT_TRUE( splitter );

  modelObject = splitter->lastOutletModelObject();
  ASSERT_TRUE( modelObject );

  OptionalNode splitterNode;
  splitterNode = modelObject->optionalCast<Node>();
  ASSERT_TRUE( splitterNode );

  modelObject = splitterNode->outletModelObject();
  ASSERT_TRUE(modelObject);

  OptionalAirLoopHVACZoneMixer mixer;
  mixer = modelObject->optionalCast<AirLoopHVACZoneMixer>();
  ASSERT_TRUE( mixer );

  modelObject = mixer->outletModelObject();
  ASSERT_TRUE( modelObject );

  OptionalNode _demandOutletNode;
  _demandOutletNode = modelObject->optionalCast<Node>();
  ASSERT_TRUE( _demandOutletNode );

  modelObject = _demandOutletNode->outletModelObject();
  ASSERT_TRUE( modelObject );

  OptionalAirLoopHVAC _airLoopHVAC;
  _airLoopHVAC = modelObject->optionalCast<AirLoopHVAC>();
  ASSERT_TRUE( _airLoopHVAC );

  ASSERT_EQ("StayOff",_airLoopHVAC->nightCycleControlType());

  ASSERT_TRUE(_airLoopHVAC->setNightCycleControlType("CycleOnAny"));

  ASSERT_TRUE(_airLoopHVAC->setNightCycleControlType("CycleOnAnyZoneFansOnly"));
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  ASSERT_EXIT (
    {
      Model m;

      AirLoopHVAC airLoopHVAC(m);

      airLoopHVAC.availabilitySchedule();

      exit(0);
    } ,
    ::testing::ExitedWithCode(0), "" );
}

TEST_F(ModelFixture,AirLoopHVAC_addBranchForZone)
{
  Model model = Model();
  OptionalModelObject modelObject;

  AirLoopHVAC airLoopHVAC = AirLoopHVAC(model);
  ThermalZone thermalZone = ThermalZone(model);
  ThermalZone thermalZone2 = ThermalZone(model);
  ScheduleCompact scheduleCompact = ScheduleCompact(model);
  AirTerminalSingleDuctUncontrolled singleDuctTerminal = 
                                                         AirTerminalSingleDuctUncontrolled(model,scheduleCompact);
  EXPECT_TRUE(scheduleCompact.scheduleTypeLimits());

  ASSERT_TRUE(airLoopHVAC.addBranchForZone(thermalZone,singleDuctTerminal));

  ASSERT_TRUE(airLoopHVAC.addBranchForZone(thermalZone2,boost::optional<StraightComponent>()));

}

TEST_F(ModelFixture,AirLoopHVAC_demandComponents)
{
  Model model = Model();
  OptionalModelObject modelObject;

  AirLoopHVAC airLoopHVAC = AirLoopHVAC(model);
  ThermalZone thermalZone = ThermalZone(model);
  ThermalZone thermalZone2 = ThermalZone(model);
  ScheduleCompact scheduleCompact = ScheduleCompact(model);
  AirTerminalSingleDuctUncontrolled singleDuctTerminal = 
                                                         AirTerminalSingleDuctUncontrolled(model,scheduleCompact);

  ASSERT_EQ( unsigned(5),airLoopHVAC.demandComponents().size() );

  airLoopHVAC.addBranchForZone(thermalZone, singleDuctTerminal);

  ASSERT_EQ( unsigned(8),airLoopHVAC.demandComponents().size() );

  airLoopHVAC.addBranchForZone(thermalZone2, boost::optional<StraightComponent>());

  ASSERT_EQ( unsigned(12),airLoopHVAC.demandComponents().size() );

  ASSERT_EQ( thermalZone,airLoopHVAC.demandComponents( airLoopHVAC.zoneSplitter().outletModelObject(0)->cast<HVACComponent>(), 
                                                       airLoopHVAC.zoneMixer().inletModelObject(0)->cast<HVACComponent>(),
                                                       thermalZone.iddObjectType() ).front() );
}

TEST_F(ModelFixture,AirLoopHVAC_demandComponents2)
{
  Model model = Model();
  OptionalModelObject modelObject;

  AirLoopHVAC airLoopHVAC = AirLoopHVAC(model);
  ThermalZone thermalZone = ThermalZone(model);

  EXPECT_EQ( unsigned(5),airLoopHVAC.demandComponents().size() );

  EXPECT_TRUE(airLoopHVAC.addBranchForZone(thermalZone,boost::optional<StraightComponent>()));

  EXPECT_EQ( unsigned(7),airLoopHVAC.demandComponents().size() );

  EXPECT_TRUE(airLoopHVAC.removeBranchForZone(thermalZone));

  EXPECT_EQ( unsigned(5),airLoopHVAC.demandComponents().size() );
}

TEST_F(ModelFixture,AirLoopHVAC_removeBranchForZone)
{
  Model model = Model();

  AirLoopHVAC airLoopHVAC = AirLoopHVAC(model);
  ThermalZone thermalZone = ThermalZone(model);
  ThermalZone thermalZone2 = ThermalZone(model);
  ScheduleCompact scheduleCompact = ScheduleCompact(model);
  AirTerminalSingleDuctUncontrolled singleDuctTerminal(model,scheduleCompact);
  SetpointManagerSingleZoneReheat spm(model);

  Node outletNode = airLoopHVAC.supplyOutletNode();

  EXPECT_EQ( unsigned(5),airLoopHVAC.demandComponents().size() );

  EXPECT_TRUE(airLoopHVAC.addBranchForZone(thermalZone,singleDuctTerminal));

  spm.addToNode(outletNode);

  EXPECT_EQ(thermalZone, spm.controlZone());

  EXPECT_EQ( unsigned(8),airLoopHVAC.demandComponents().size() );

  EXPECT_TRUE(airLoopHVAC.addBranchForZone(thermalZone2,boost::optional<StraightComponent>()));

  EXPECT_EQ( unsigned(12),airLoopHVAC.demandComponents().size() );

  EXPECT_TRUE(airLoopHVAC.removeBranchForZone(thermalZone2));

  EXPECT_EQ( unsigned(8),airLoopHVAC.demandComponents().size() );

  EXPECT_TRUE(airLoopHVAC.removeBranchForZone(thermalZone));

  EXPECT_EQ( unsigned(5),airLoopHVAC.demandComponents().size() );

  EXPECT_NE(thermalZone, spm.controlZone());
  EXPECT_FALSE(spm.controlZone());
}

TEST_F(ModelFixture,ThermalZone_remove)
{
  Model model = Model();
  OptionalModelObject modelObject;

  AirLoopHVAC airLoopHVAC = AirLoopHVAC(model);
  ThermalZone thermalZone = ThermalZone(model);
  ThermalZone thermalZone2 = ThermalZone(model);
  ScheduleCompact scheduleCompact = ScheduleCompact(model);
  AirTerminalSingleDuctUncontrolled singleDuctTerminal = 
                                                         AirTerminalSingleDuctUncontrolled(model,scheduleCompact);

  ASSERT_TRUE(airLoopHVAC.addBranchForZone(thermalZone,singleDuctTerminal));

  ASSERT_TRUE(airLoopHVAC.addBranchForZone(thermalZone2,boost::optional<StraightComponent>()));

  airLoopHVAC.removeBranchForZone(thermalZone2);

  airLoopHVAC.removeBranchForZone(thermalZone);
}

TEST_F(ModelFixture,AirLoopHVAC_remove)
{
  Model model = Model();
  OptionalModelObject modelObject;

  AirLoopHVAC airLoopHVAC = AirLoopHVAC(model);
  ThermalZone thermalZone = ThermalZone(model);
  ThermalZone thermalZone2 = ThermalZone(model);
  ScheduleCompact scheduleCompact = ScheduleCompact(model);
  AirTerminalSingleDuctUncontrolled singleDuctTerminal = 
                                                         AirTerminalSingleDuctUncontrolled(model,scheduleCompact);

  ASSERT_TRUE(airLoopHVAC.addBranchForZone(thermalZone,singleDuctTerminal));

  ASSERT_TRUE(airLoopHVAC.addBranchForZone(thermalZone2,boost::optional<StraightComponent>()));

  ASSERT_NO_THROW(airLoopHVAC.remove());
}

TEST_F(ModelFixture,AirLoopHVACOutdoorAirSystem_addToNode)
{
  Model model = Model();
  OptionalModelObject modelObject;

  ScheduleCompact schedule(model);
  AirLoopHVAC airLoopHVAC(model);
  ControllerOutdoorAir controller(model);
  AirLoopHVACOutdoorAirSystem oaSystem(model,controller);
  FanConstantVolume fan(model,schedule);

  Node supplyInletNode = airLoopHVAC.supplyInletNode();

  ASSERT_TRUE(oaSystem.addToNode(supplyInletNode));

  ASSERT_EQ(oaSystem,supplyInletNode.outletModelObject().get());

  ASSERT_EQ(supplyInletNode,oaSystem.returnAirModelObject().get());

  ASSERT_TRUE(fan.addToNode(supplyInletNode));

  ASSERT_EQ(fan,supplyInletNode.outletModelObject().get());

  ASSERT_TRUE(oaSystem.returnAirModelObject()->optionalCast<Node>());

  ASSERT_EQ(fan,oaSystem.returnAirModelObject()->cast<Node>().inletModelObject().get());
}

TEST_F(ModelFixture,AirLoopHVAC_supplyComponents)
{
  Model model = Model();
  OptionalModelObject modelObject;

  ScheduleCompact schedule(model);
  AirLoopHVAC airLoopHVAC(model);
  ControllerOutdoorAir controller(model);
  AirLoopHVACOutdoorAirSystem oaSystem(model,controller);
  FanConstantVolume fan(model,schedule);

  Node supplyInletNode = airLoopHVAC.supplyInletNode();

  ASSERT_TRUE(oaSystem.addToNode(supplyInletNode));

  ASSERT_EQ(oaSystem,supplyInletNode.outletModelObject().get());

  ASSERT_EQ(supplyInletNode,oaSystem.returnAirModelObject().get());

  ASSERT_TRUE(fan.addToNode(supplyInletNode));

  ASSERT_EQ(fan,supplyInletNode.outletModelObject().get());

  ASSERT_TRUE(oaSystem.returnAirModelObject()->optionalCast<Node>());

  ASSERT_EQ(fan,oaSystem.returnAirModelObject()->cast<Node>().inletModelObject().get());

  std::vector<ModelObject> returnComponents = 
    airLoopHVAC.supplyComponents(supplyInletNode, oaSystem.returnAirModelObject()->cast<Node>());
  returnComponents.erase( returnComponents.begin() );

  ASSERT_EQ(unsigned(2),returnComponents.size());
}

TEST_F(ModelFixture,FanConstantVolume_remove)
{
  Model model = Model();
  OptionalModelObject modelObject;

  ScheduleCompact schedule(model);
  AirLoopHVAC airLoopHVAC(model);
  ControllerOutdoorAir controller(model);
  AirLoopHVACOutdoorAirSystem oaSystem(model,controller);
  FanConstantVolume fan(model,schedule);

  Node supplyInletNode = airLoopHVAC.supplyInletNode();
  Node supplyOutletNode = airLoopHVAC.supplyOutletNode();

  ASSERT_TRUE(oaSystem.addToNode(supplyInletNode));

  ASSERT_TRUE(fan.addToNode(supplyOutletNode));

  ASSERT_EQ(fan,supplyOutletNode.inletModelObject().get());

  ASSERT_TRUE(fan.inletModelObject()->optionalCast<Node>());

  ASSERT_EQ(oaSystem,fan.inletModelObject()->cast<Node>().inletModelObject().get());

  ASSERT_TRUE(fan.outletModelObject());

  ASSERT_TRUE(fan.outletModelObject()->optionalCast<Node>());

  ASSERT_EQ(supplyOutletNode,fan.outletModelObject().get());

  fan.remove();

  ASSERT_EQ(oaSystem,supplyOutletNode.inletModelObject().get());
}

TEST_F(ModelFixture,AirLoopHVAC_remove2)
{
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  Model m; 

  ScheduleCompact s(m);
  
  PlantLoop plantLoop(m); 

  AirLoopHVAC airLoop(m);

  Node airSupplyOutletNode = airLoop.supplyOutletNode();

  CoilHeatingWater heatingCoil(m,s);

  heatingCoil.addToNode(airSupplyOutletNode);

  EXPECT_TRUE(plantLoop.addDemandBranchForComponent(heatingCoil));

  EXPECT_EQ( (unsigned)7,plantLoop.demandComponents().size() );

  CoilCoolingWater coolingCoil(m,s);

  coolingCoil.addToNode(airSupplyOutletNode);

  EXPECT_TRUE(plantLoop.addDemandBranchForComponent(coolingCoil));

  EXPECT_EQ( (unsigned)10,plantLoop.demandComponents().size() );

  EXPECT_TRUE( coolingCoil.controllerWaterCoil() );

  EXPECT_TRUE( heatingCoil.controllerWaterCoil() );

  airLoop.remove();

  EXPECT_EQ( 10u,plantLoop.demandComponents().size() );

  coolingCoil.remove();
  heatingCoil.remove();

  EXPECT_EQ( 5u,plantLoop.demandComponents().size() );

  EXPECT_EXIT ( 
  {  
    plantLoop.demandInletNode();

    plantLoop.demandOutletNode();

     exit(0); 
  } ,
    ::testing::ExitedWithCode(0), "" );
}


TEST_F(ModelFixture, AirLoopHVAC_remove3)
{
  Model m; 

  EXPECT_EQ(0u, m.getModelObjects<AirLoopHVAC>().size());
  EXPECT_EQ(0u, m.getModelObjects<SizingSystem>().size());

  Loop loop = addSystemType5(m);

  EXPECT_EQ(1u, m.getModelObjects<AirLoopHVAC>().size());
  EXPECT_EQ(1u, m.getModelObjects<SizingSystem>().size());

  loop.remove();

  EXPECT_EQ(0u, m.getModelObjects<AirLoopHVAC>().size());
  EXPECT_EQ(0u, m.getModelObjects<SizingSystem>().size());
}

TEST_F(ModelFixture, AirLoopHVAC_Cost)
{
  Model model; 

  AirLoopHVAC airLoopHVAC = AirLoopHVAC(model);
  ThermalZone thermalZone = ThermalZone(model);
  ThermalZone thermalZone2 = ThermalZone(model);
  ScheduleCompact scheduleCompact = ScheduleCompact(model);
  AirTerminalSingleDuctUncontrolled singleDuctTerminal = 
                                                         AirTerminalSingleDuctUncontrolled(model,scheduleCompact);
  EXPECT_TRUE(scheduleCompact.scheduleTypeLimits());

  boost::optional<LifeCycleCost> cost1 = LifeCycleCost::createLifeCycleCost("Install", airLoopHVAC, 1000.0, "CostPerEach", "Construction");
  ASSERT_TRUE(cost1);
  boost::optional<LifeCycleCost> cost2 = LifeCycleCost::createLifeCycleCost("Ducting", airLoopHVAC, 200.0, "CostPerThermalZone", "Construction");
  ASSERT_TRUE(cost2);
  boost::optional<LifeCycleCost> cost3 = LifeCycleCost::createLifeCycleCost("Terminals", airLoopHVAC, 66.0, "CostPerArea", "Construction");
  EXPECT_FALSE(cost3);

  EXPECT_DOUBLE_EQ(1000.0, cost1->totalCost());
  EXPECT_DOUBLE_EQ(0.0, cost2->totalCost());

  ASSERT_TRUE(airLoopHVAC.addBranchForZone(thermalZone,singleDuctTerminal));

  EXPECT_EQ(1u, airLoopHVAC.thermalZones().size());

  EXPECT_DOUBLE_EQ(1000.0, cost1->totalCost());
  EXPECT_DOUBLE_EQ(200.0, cost2->totalCost());

  ASSERT_TRUE(airLoopHVAC.addBranchForZone(thermalZone2,boost::none));

  EXPECT_EQ(2u, airLoopHVAC.thermalZones().size());

  EXPECT_DOUBLE_EQ(1000.0, cost1->totalCost());
  EXPECT_DOUBLE_EQ(400.0, cost2->totalCost());

  thermalZone.setMultiplier(2);

  EXPECT_EQ(2u, airLoopHVAC.thermalZones().size());

  EXPECT_DOUBLE_EQ(1000.0, cost1->totalCost());
  EXPECT_DOUBLE_EQ(600.0, cost2->totalCost());

}

TEST_F(ModelFixture, AirLoopHVAC_AddBranchForZone_ReuseTerminal)
{
  Model model; 

  AirLoopHVAC airLoopHVAC = AirLoopHVAC(model);
  ThermalZone thermalZone = ThermalZone(model);
  ThermalZone thermalZone2 = ThermalZone(model);
  ScheduleCompact scheduleCompact = ScheduleCompact(model);
  AirTerminalSingleDuctUncontrolled singleDuctTerminal = 
                                                         AirTerminalSingleDuctUncontrolled(model,scheduleCompact);
  EXPECT_TRUE(scheduleCompact.scheduleTypeLimits());


  ASSERT_TRUE(airLoopHVAC.addBranchForZone(thermalZone,singleDuctTerminal));

  EXPECT_EQ(1u, airLoopHVAC.thermalZones().size());

  EXPECT_FALSE(airLoopHVAC.addBranchForZone(thermalZone2,singleDuctTerminal));

  EXPECT_EQ(1u, airLoopHVAC.thermalZones().size());

  AirTerminalSingleDuctUncontrolled term2 = 
    singleDuctTerminal.clone(model).cast<AirTerminalSingleDuctUncontrolled>();

  EXPECT_TRUE(airLoopHVAC.addBranchForZone(thermalZone2,term2));

  EXPECT_EQ(2u, airLoopHVAC.thermalZones().size());
}

TEST_F(ModelFixture, AirLoopHVAC_edges)
{
  Model m;

  AirLoopHVAC airLoopHVAC(m);
  PlantLoop plantLoop(m);
  // demand components
  ThermalZone thermalZone(m);
  ThermalZone thermalZone2(m);
  ThermalZone thermalZone3(m);
  ThermalZone thermalZone4(m);
  ScheduleCompact s(m);
  AirTerminalSingleDuctUncontrolled singleDuctTerminal(m, s);
  AirTerminalSingleDuctUncontrolled singleDuctTerminal2(m, s);
  AirTerminalSingleDuctUncontrolled singleDuctTerminal3(m, s);
  AirTerminalSingleDuctUncontrolled singleDuctTerminal4(m, s);

  AirLoopHVACZoneSplitter splitter = airLoopHVAC.zoneSplitter();
  AirLoopHVACZoneMixer mixer = airLoopHVAC.zoneMixer();

  Mixer plantDemandMixer = plantLoop.demandMixer();

  // supply components
  ControllerOutdoorAir controllerOutdoorAir(m);
  AirLoopHVACOutdoorAirSystem outdoorAirSystem(m, controllerOutdoorAir);
  CoilHeatingElectric coil(m, s);
  CoilHeatingElectric coil2(m, s);
  CoilHeatingWater coil3(m, s);
  CoilCoolingWater coil4(m, s);
  FanConstantVolume fan(m, s);

  airLoopHVAC.addBranchForZone(thermalZone,singleDuctTerminal);
  airLoopHVAC.addBranchForZone(thermalZone2,singleDuctTerminal2);
  airLoopHVAC.addBranchForZone(thermalZone3,singleDuctTerminal3);
  airLoopHVAC.addBranchForZone(thermalZone4,singleDuctTerminal4);

  Node supplyOutletNode = airLoopHVAC.supplyOutletNode();
  outdoorAirSystem.addToNode(supplyOutletNode);
  coil4.addToNode(supplyOutletNode);
  coil3.addToNode(supplyOutletNode);
  coil.addToNode(supplyOutletNode);
  fan.addToNode(supplyOutletNode);

  plantLoop.addDemandBranchForComponent(coil3);

  boost::optional<Node> OANode = outdoorAirSystem.outboardOANode();
  ASSERT_TRUE(OANode);
  coil2.addToNode(*OANode);

  boost::optional<ModelObject> oaSystem = airLoopHVAC.supplyComponent(outdoorAirSystem.handle());
  ASSERT_TRUE(oaSystem);
  EXPECT_EQ(outdoorAirSystem, *oaSystem);
  std::vector<HVACComponent> edges = outdoorAirSystem.getImpl<detail::HVACComponent_Impl>()->edges(false); // should be Node
  ASSERT_EQ(1, edges.size());
  edges = edges[0].getImpl<detail::HVACComponent_Impl>()->edges(false); // should be CoilHeatingElectric
  ASSERT_EQ(1, edges.size());
  EXPECT_EQ(coil4, edges[0]);

  boost::optional<ModelObject> splitter_obj = airLoopHVAC.demandComponent(splitter.handle());
  ASSERT_TRUE(splitter_obj);
  EXPECT_EQ(splitter, *splitter_obj);
  edges = splitter.getImpl<detail::HVACComponent_Impl>()->edges(true); // should be all air terminals
  EXPECT_EQ(4, edges.size());
  bool found_terminal_1 = false;
  bool found_terminal_2 = false;
  bool found_terminal_3 = false;
  bool found_terminal_4 = false;
  for( std::vector<HVACComponent>::iterator it = edges.begin(); it != edges.end(); ++it )
  {
    if( singleDuctTerminal == *it ) {
      found_terminal_1 = true;
    }
    else if( singleDuctTerminal2 == *it ) {
      found_terminal_2 = true;
    }
    else if( singleDuctTerminal3 == *it ) {
      found_terminal_3 = true;
    }
    else if( singleDuctTerminal4 == *it ) {
      found_terminal_4 = true;
    }
  }
  EXPECT_TRUE(found_terminal_1);
  EXPECT_TRUE(found_terminal_2);
  EXPECT_TRUE(found_terminal_3);
  EXPECT_TRUE(found_terminal_4);

  boost::optional<ModelObject> thermal_zone = airLoopHVAC.demandComponent(thermalZone.handle());
  ASSERT_TRUE(thermal_zone);
  EXPECT_EQ(thermalZone, *thermal_zone);
  edges = thermalZone.getImpl<detail::HVACComponent_Impl>()->edges(true); // should be Node
  ASSERT_EQ(1, edges.size());
  edges = edges[0].getImpl<detail::HVACComponent_Impl>()->edges(true); // should be Mixer
  ASSERT_EQ(1, edges.size());
  EXPECT_EQ(mixer, edges[0]);

  boost::optional<ModelObject> terminal = airLoopHVAC.demandComponent(singleDuctTerminal.handle());
  ASSERT_TRUE(terminal);
  EXPECT_EQ(singleDuctTerminal, *terminal);
  edges = singleDuctTerminal.getImpl<detail::HVACComponent_Impl>()->edges(true); // should be Node
  ASSERT_EQ(1, edges.size());
  edges = edges[0].getImpl<detail::HVACComponent_Impl>()->edges(true); // should be ThermalZone
  ASSERT_EQ(1, edges.size());
  EXPECT_EQ(thermalZone, edges[0]);

  boost::optional<ModelObject> heatingElecCoil = airLoopHVAC.supplyComponent(coil.handle());
  ASSERT_TRUE(heatingElecCoil);
  EXPECT_EQ(coil, *heatingElecCoil);
  edges = coil.getImpl<detail::HVACComponent_Impl>()->edges(false); // should be Node
  ASSERT_EQ(1, edges.size());
  edges = edges[0].getImpl<detail::HVACComponent_Impl>()->edges(false); // should be ConstantVolumeFan
  ASSERT_EQ(1, edges.size());
  EXPECT_EQ(fan, edges[0]);

  Node supplyInletNode = airLoopHVAC.supplyInletNode();
  boost::optional<ModelObject> inletNode = airLoopHVAC.supplyComponent(supplyInletNode.handle());
  ASSERT_TRUE(inletNode);
  EXPECT_EQ(supplyInletNode, *inletNode);
  edges = supplyInletNode.getImpl<detail::HVACComponent_Impl>()->edges(false); // should be OASystem

  boost::optional<ModelObject> heatingWaterCoil_air = airLoopHVAC.supplyComponent(coil3.handle());
  ASSERT_TRUE(heatingWaterCoil_air);
  EXPECT_EQ(coil3, *heatingWaterCoil_air);
  edges = coil3.getImpl<detail::HVACComponent_Impl>()->edges(false); // should be Node
  ASSERT_EQ(1, edges.size());
  edges = edges[0].getImpl<detail::HVACComponent_Impl>()->edges(false); // should be CoilHeatingElectric
  ASSERT_EQ(1, edges.size());
  EXPECT_EQ(coil, edges[0]);

  boost::optional<ModelObject> heatingWaterCoil_plant = plantLoop.demandComponent(coil3.handle());
  ASSERT_TRUE(heatingWaterCoil_plant);
  EXPECT_EQ(coil3, *heatingWaterCoil_plant);
  edges = coil3.getImpl<detail::HVACComponent_Impl>()->edges(true); // should be Node
  ASSERT_EQ(1, edges.size());
  edges = edges[0].getImpl<detail::HVACComponent_Impl>()->edges(true); // should be ConnectorMixer
  ASSERT_EQ(1, edges.size());
  EXPECT_EQ(plantDemandMixer, edges[0]);

  // does not search OA system
  boost::optional<ModelObject> oaInletNode = airLoopHVAC.supplyComponent((*OANode).handle());
  ASSERT_FALSE(oaInletNode);
  boost::optional<ModelObject> heatingElecCoil2 = airLoopHVAC.supplyComponent(coil2.handle());
  ASSERT_FALSE(heatingElecCoil2);
}
