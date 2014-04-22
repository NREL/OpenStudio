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
#include <model/test/ModelFixture.hpp>
#include <model/AirLoopHVAC.hpp>
#include <model/AirLoopHVAC_Impl.hpp>
#include <model/AirLoopHVACSupplyPlenum.hpp>
#include <model/AirLoopHVACSupplyPlenum_Impl.hpp>
#include <model/AirLoopHVACReturnPlenum.hpp>
#include <model/AirLoopHVACReturnPlenum_Impl.hpp>
#include <model/CoilHeatingWater.hpp>
#include <model/CoilHeatingWater_Impl.hpp>
#include <model/CoilCoolingWater.hpp>
#include <model/CoilCoolingWater_Impl.hpp>
#include <model/ControllerWaterCoil.hpp>
#include <model/ControllerWaterCoil_Impl.hpp>
#include <model/AirLoopHVACOutdoorAirSystem.hpp>
#include <model/AirLoopHVACOutdoorAirSystem_Impl.hpp>
#include <model/ControllerOutdoorAir.hpp>
#include <model/Node.hpp>
#include <model/Node_Impl.hpp>
#include <model/AirLoopHVACZoneMixer.hpp>
#include <model/AirLoopHVACZoneMixer_Impl.hpp>
#include <model/AirLoopHVACZoneSplitter.hpp>
#include <model/AirLoopHVACZoneSplitter_Impl.hpp>
#include <model/AirTerminalSingleDuctUncontrolled.hpp>
#include <model/AirTerminalSingleDuctUncontrolled_Impl.hpp>
#include <model/ThermalZone.hpp>
#include <model/ThermalZone_Impl.hpp>
#include <model/ScheduleCompact.hpp>
#include <model/ScheduleCompact_Impl.hpp>
#include <model/ScheduleTypeLimits.hpp>
#include <model/FanConstantVolume.hpp>
#include <model/FanConstantVolume_Impl.hpp>
#include <model/SizingSystem.hpp>
#include <model/SizingSystem_Impl.hpp>
#include <model/Model.hpp>
#include <model/Model_Impl.hpp>
#include <model/HVACTemplates.hpp>
#include <model/LifeCycleCost.hpp>

using namespace openstudio;

TEST_F(ModelFixture,AirLoopHVAC_AirLoopHVAC)
{
  openstudio::model::Model model = openstudio::model::Model();
  openstudio::model::OptionalModelObject modelObject;

  openstudio::model::AirLoopHVAC airLoopHVAC = openstudio::model::AirLoopHVAC(model);
  ASSERT_EQ(airLoopHVAC.iddObject().name(),"OS:AirLoopHVAC");

  openstudio::model::OptionalNode supplyInletNode = airLoopHVAC.supplyInletNode();
  openstudio::model::OptionalNode demandOutletNode = airLoopHVAC.demandOutletNode();
  std::vector<openstudio::model::Node> supplyOutletNodes = airLoopHVAC.supplyOutletNodes();
  std::vector<openstudio::model::Node> demandInletNodes = airLoopHVAC.demandInletNodes();

  ASSERT_TRUE( supplyInletNode );
  ASSERT_TRUE( demandOutletNode );
  ASSERT_EQ( unsigned(1), supplyOutletNodes.size() );
  ASSERT_EQ( unsigned(1), demandInletNodes.size() );

  openstudio::model::OptionalNode supplyOutletNode = openstudio::model::OptionalNode(supplyOutletNodes.front());
  openstudio::model::OptionalNode demandInletNode = openstudio::model::OptionalNode(demandInletNodes.front());

  modelObject = demandInletNode->outletModelObject();
  ASSERT_TRUE( modelObject );

  openstudio::model::OptionalAirLoopHVACZoneSplitter splitter;
  splitter = modelObject->optionalCast<openstudio::model::AirLoopHVACZoneSplitter>();
  ASSERT_TRUE( splitter );

  modelObject = splitter->lastOutletModelObject();
  ASSERT_TRUE( modelObject );

  openstudio::model::OptionalNode splitterNode;
  splitterNode = modelObject->optionalCast<openstudio::model::Node>();
  ASSERT_TRUE( splitterNode );

  modelObject = splitterNode->outletModelObject();
  ASSERT_TRUE(modelObject);

  openstudio::model::OptionalAirLoopHVACZoneMixer mixer;
  mixer = modelObject->optionalCast<openstudio::model::AirLoopHVACZoneMixer>();
  ASSERT_TRUE( mixer );

  modelObject = mixer->outletModelObject();
  ASSERT_TRUE( modelObject );

  openstudio::model::OptionalNode _demandOutletNode;
  _demandOutletNode = modelObject->optionalCast<openstudio::model::Node>();
  ASSERT_TRUE( _demandOutletNode );

  modelObject = _demandOutletNode->outletModelObject();
  ASSERT_TRUE( modelObject );

  openstudio::model::OptionalAirLoopHVAC _airLoopHVAC;
  _airLoopHVAC = modelObject->optionalCast<openstudio::model::AirLoopHVAC>();
  ASSERT_TRUE( _airLoopHVAC );

  ASSERT_EQ("StayOff",_airLoopHVAC->nightCycleControlType());

  ASSERT_TRUE(_airLoopHVAC->setNightCycleControlType("CycleOnAny"));

  ASSERT_TRUE(_airLoopHVAC->setNightCycleControlType("CycleOnAnyZoneFansOnly"));
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  ASSERT_EXIT (
    {
      model::Model m;

      model::AirLoopHVAC airLoopHVAC(m);

      airLoopHVAC.availabilitySchedule();

      exit(0);
    } ,
    ::testing::ExitedWithCode(0), "" );
}

TEST_F(ModelFixture,AirLoopHVAC_addBranchForZone)
{
  openstudio::model::Model model = openstudio::model::Model();
  openstudio::model::OptionalModelObject modelObject;

  openstudio::model::AirLoopHVAC airLoopHVAC = openstudio::model::AirLoopHVAC(model);
  openstudio::model::ThermalZone thermalZone = openstudio::model::ThermalZone(model);
  openstudio::model::ThermalZone thermalZone2 = openstudio::model::ThermalZone(model);
  openstudio::model::ScheduleCompact scheduleCompact = openstudio::model::ScheduleCompact(model);
  openstudio::model::AirTerminalSingleDuctUncontrolled singleDuctTerminal = 
                                                         openstudio::model::AirTerminalSingleDuctUncontrolled(model,scheduleCompact);
  EXPECT_TRUE(scheduleCompact.scheduleTypeLimits());

  ASSERT_TRUE(airLoopHVAC.addBranchForZone(thermalZone,singleDuctTerminal));

  ASSERT_TRUE(airLoopHVAC.addBranchForZone(thermalZone2,boost::optional<openstudio::model::StraightComponent>()));

}

TEST_F(ModelFixture,AirLoopHVAC_demandComponents)
{
  openstudio::model::Model model = openstudio::model::Model();
  openstudio::model::OptionalModelObject modelObject;

  openstudio::model::AirLoopHVAC airLoopHVAC = openstudio::model::AirLoopHVAC(model);
  openstudio::model::ThermalZone thermalZone = openstudio::model::ThermalZone(model);
  openstudio::model::ThermalZone thermalZone2 = openstudio::model::ThermalZone(model);
  openstudio::model::ScheduleCompact scheduleCompact = openstudio::model::ScheduleCompact(model);
  openstudio::model::AirTerminalSingleDuctUncontrolled singleDuctTerminal = 
                                                         openstudio::model::AirTerminalSingleDuctUncontrolled(model,scheduleCompact);

  ASSERT_EQ( unsigned(5),airLoopHVAC.demandComponents().size() );

  airLoopHVAC.addBranchForZone(thermalZone, singleDuctTerminal);

  ASSERT_EQ( unsigned(8),airLoopHVAC.demandComponents().size() );

  airLoopHVAC.addBranchForZone(thermalZone2, boost::optional<openstudio::model::StraightComponent>());

  ASSERT_EQ( unsigned(12),airLoopHVAC.demandComponents().size() );

  ASSERT_EQ( thermalZone,airLoopHVAC.demandComponents( airLoopHVAC.zoneSplitter().outletModelObject(0)->cast<openstudio::model::HVACComponent>(), 
                                                       airLoopHVAC.zoneMixer().inletModelObject(0)->cast<openstudio::model::HVACComponent>(),
                                                       thermalZone.iddObjectType() ).front() );
}

TEST_F(ModelFixture,AirLoopHVAC_demandComponents2)
{
  openstudio::model::Model model = openstudio::model::Model();
  openstudio::model::OptionalModelObject modelObject;

  openstudio::model::AirLoopHVAC airLoopHVAC = openstudio::model::AirLoopHVAC(model);
  openstudio::model::ThermalZone thermalZone = openstudio::model::ThermalZone(model);

  EXPECT_EQ( unsigned(5),airLoopHVAC.demandComponents().size() );

  EXPECT_TRUE(airLoopHVAC.addBranchForZone(thermalZone,boost::optional<openstudio::model::StraightComponent>()));

  EXPECT_EQ( unsigned(7),airLoopHVAC.demandComponents().size() );

  EXPECT_TRUE(airLoopHVAC.removeBranchForZone(thermalZone));

  EXPECT_EQ( unsigned(5),airLoopHVAC.demandComponents().size() );
}

TEST_F(ModelFixture,AirLoopHVAC_removeBranchForZone)
{
  openstudio::model::Model model = openstudio::model::Model();
  openstudio::model::OptionalModelObject modelObject;

  openstudio::model::AirLoopHVAC airLoopHVAC = openstudio::model::AirLoopHVAC(model);
  openstudio::model::ThermalZone thermalZone = openstudio::model::ThermalZone(model);
  openstudio::model::ThermalZone thermalZone2 = openstudio::model::ThermalZone(model);
  openstudio::model::ScheduleCompact scheduleCompact = openstudio::model::ScheduleCompact(model);
  openstudio::model::AirTerminalSingleDuctUncontrolled singleDuctTerminal = 
                                                         openstudio::model::AirTerminalSingleDuctUncontrolled(model,scheduleCompact);

  EXPECT_EQ( unsigned(5),airLoopHVAC.demandComponents().size() );

  EXPECT_TRUE(airLoopHVAC.addBranchForZone(thermalZone,singleDuctTerminal));

  EXPECT_EQ( unsigned(8),airLoopHVAC.demandComponents().size() );

  EXPECT_TRUE(airLoopHVAC.addBranchForZone(thermalZone2,boost::optional<openstudio::model::StraightComponent>()));

  EXPECT_EQ( unsigned(12),airLoopHVAC.demandComponents().size() );

  EXPECT_TRUE(airLoopHVAC.removeBranchForZone(thermalZone2));

  EXPECT_EQ( unsigned(8),airLoopHVAC.demandComponents().size() );

  EXPECT_TRUE(airLoopHVAC.removeBranchForZone(thermalZone));

  EXPECT_EQ( unsigned(5),airLoopHVAC.demandComponents().size() );
}

TEST_F(ModelFixture,ThermalZone_remove)
{
  openstudio::model::Model model = openstudio::model::Model();
  openstudio::model::OptionalModelObject modelObject;

  openstudio::model::AirLoopHVAC airLoopHVAC = openstudio::model::AirLoopHVAC(model);
  openstudio::model::ThermalZone thermalZone = openstudio::model::ThermalZone(model);
  openstudio::model::ThermalZone thermalZone2 = openstudio::model::ThermalZone(model);
  openstudio::model::ScheduleCompact scheduleCompact = openstudio::model::ScheduleCompact(model);
  openstudio::model::AirTerminalSingleDuctUncontrolled singleDuctTerminal = 
                                                         openstudio::model::AirTerminalSingleDuctUncontrolled(model,scheduleCompact);

  ASSERT_TRUE(airLoopHVAC.addBranchForZone(thermalZone,singleDuctTerminal));

  ASSERT_TRUE(airLoopHVAC.addBranchForZone(thermalZone2,boost::optional<openstudio::model::StraightComponent>()));

  airLoopHVAC.removeBranchForZone(thermalZone2);

  airLoopHVAC.removeBranchForZone(thermalZone);
}

TEST_F(ModelFixture,AirLoopHVAC_remove)
{
  openstudio::model::Model model = openstudio::model::Model();
  openstudio::model::OptionalModelObject modelObject;

  openstudio::model::AirLoopHVAC airLoopHVAC = openstudio::model::AirLoopHVAC(model);
  openstudio::model::ThermalZone thermalZone = openstudio::model::ThermalZone(model);
  openstudio::model::ThermalZone thermalZone2 = openstudio::model::ThermalZone(model);
  openstudio::model::ScheduleCompact scheduleCompact = openstudio::model::ScheduleCompact(model);
  openstudio::model::AirTerminalSingleDuctUncontrolled singleDuctTerminal = 
                                                         openstudio::model::AirTerminalSingleDuctUncontrolled(model,scheduleCompact);

  ASSERT_TRUE(airLoopHVAC.addBranchForZone(thermalZone,singleDuctTerminal));

  ASSERT_TRUE(airLoopHVAC.addBranchForZone(thermalZone2,boost::optional<openstudio::model::StraightComponent>()));

  ASSERT_NO_THROW(airLoopHVAC.remove());
}

TEST_F(ModelFixture,AirLoopHVACOutdoorAirSystem_addToNode)
{
  openstudio::model::Model model = openstudio::model::Model();
  openstudio::model::OptionalModelObject modelObject;

  openstudio::model::ScheduleCompact schedule(model);
  openstudio::model::AirLoopHVAC airLoopHVAC(model);
  openstudio::model::ControllerOutdoorAir controller(model);
  openstudio::model::AirLoopHVACOutdoorAirSystem oaSystem(model,controller);
  openstudio::model::FanConstantVolume fan(model,schedule);

  openstudio::model::Node supplyInletNode = airLoopHVAC.supplyInletNode();

  ASSERT_TRUE(oaSystem.addToNode(supplyInletNode));

  ASSERT_EQ(oaSystem,supplyInletNode.outletModelObject().get());

  ASSERT_EQ(supplyInletNode,oaSystem.returnAirModelObject().get());

  ASSERT_TRUE(fan.addToNode(supplyInletNode));

  ASSERT_EQ(fan,supplyInletNode.outletModelObject().get());

  ASSERT_TRUE(oaSystem.returnAirModelObject()->optionalCast<openstudio::model::Node>());

  ASSERT_EQ(fan,oaSystem.returnAirModelObject()->cast<openstudio::model::Node>().inletModelObject().get());
}

TEST_F(ModelFixture,AirLoopHVAC_supplyComponents)
{
  openstudio::model::Model model = openstudio::model::Model();
  openstudio::model::OptionalModelObject modelObject;

  openstudio::model::ScheduleCompact schedule(model);
  openstudio::model::AirLoopHVAC airLoopHVAC(model);
  openstudio::model::ControllerOutdoorAir controller(model);
  openstudio::model::AirLoopHVACOutdoorAirSystem oaSystem(model,controller);
  openstudio::model::FanConstantVolume fan(model,schedule);

  openstudio::model::Node supplyInletNode = airLoopHVAC.supplyInletNode();

  ASSERT_TRUE(oaSystem.addToNode(supplyInletNode));

  ASSERT_EQ(oaSystem,supplyInletNode.outletModelObject().get());

  ASSERT_EQ(supplyInletNode,oaSystem.returnAirModelObject().get());

  ASSERT_TRUE(fan.addToNode(supplyInletNode));

  ASSERT_EQ(fan,supplyInletNode.outletModelObject().get());

  ASSERT_TRUE(oaSystem.returnAirModelObject()->optionalCast<openstudio::model::Node>());

  ASSERT_EQ(fan,oaSystem.returnAirModelObject()->cast<openstudio::model::Node>().inletModelObject().get());

  std::vector<openstudio::model::ModelObject> returnComponents = 
    airLoopHVAC.supplyComponents(supplyInletNode, oaSystem.returnAirModelObject()->cast<openstudio::model::Node>());
  returnComponents.erase( returnComponents.begin() );

  ASSERT_EQ(unsigned(2),returnComponents.size());
}

TEST_F(ModelFixture,FanConstantVolume_remove)
{
  openstudio::model::Model model = openstudio::model::Model();
  openstudio::model::OptionalModelObject modelObject;

  openstudio::model::ScheduleCompact schedule(model);
  openstudio::model::AirLoopHVAC airLoopHVAC(model);
  openstudio::model::ControllerOutdoorAir controller(model);
  openstudio::model::AirLoopHVACOutdoorAirSystem oaSystem(model,controller);
  openstudio::model::FanConstantVolume fan(model,schedule);

  openstudio::model::Node supplyInletNode = airLoopHVAC.supplyInletNode();
  openstudio::model::Node supplyOutletNode = airLoopHVAC.supplyOutletNode();

  ASSERT_TRUE(oaSystem.addToNode(supplyInletNode));

  ASSERT_TRUE(fan.addToNode(supplyOutletNode));

  ASSERT_EQ(fan,supplyOutletNode.inletModelObject().get());

  ASSERT_TRUE(fan.inletModelObject()->optionalCast<openstudio::model::Node>());

  ASSERT_EQ(oaSystem,fan.inletModelObject()->cast<openstudio::model::Node>().inletModelObject().get());

  ASSERT_TRUE(fan.outletModelObject());

  ASSERT_TRUE(fan.outletModelObject()->optionalCast<openstudio::model::Node>());

  ASSERT_EQ(supplyOutletNode,fan.outletModelObject().get());

  fan.remove();

  ASSERT_EQ(oaSystem,supplyOutletNode.inletModelObject().get());
}

TEST_F(ModelFixture,AirLoopHVAC_remove2)
{
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  model::Model m; 

  model::ScheduleCompact s(m);
  
  model::PlantLoop plantLoop(m); 

  model::AirLoopHVAC airLoop(m);

  model::Node airSupplyOutletNode = airLoop.supplyOutletNode();

  model::CoilHeatingWater heatingCoil(m,s);

  heatingCoil.addToNode(airSupplyOutletNode);

  EXPECT_TRUE(plantLoop.addDemandBranchForComponent(heatingCoil));

  EXPECT_EQ( (unsigned)7,plantLoop.demandComponents().size() );

  model::CoilCoolingWater coolingCoil(m,s);

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
  model::Model m; 

  EXPECT_EQ(0u, m.getModelObjects<model::AirLoopHVAC>().size());
  EXPECT_EQ(0u, m.getModelObjects<model::SizingSystem>().size());

  model::Loop loop = addSystemType5(m);

  EXPECT_EQ(1u, m.getModelObjects<model::AirLoopHVAC>().size());
  EXPECT_EQ(1u, m.getModelObjects<model::SizingSystem>().size());

  loop.remove();

  EXPECT_EQ(0u, m.getModelObjects<model::AirLoopHVAC>().size());
  EXPECT_EQ(0u, m.getModelObjects<model::SizingSystem>().size());
}

TEST_F(ModelFixture, AirLoopHVAC_Cost)
{
  openstudio::model::Model model; 

  openstudio::model::AirLoopHVAC airLoopHVAC = openstudio::model::AirLoopHVAC(model);
  openstudio::model::ThermalZone thermalZone = openstudio::model::ThermalZone(model);
  openstudio::model::ThermalZone thermalZone2 = openstudio::model::ThermalZone(model);
  openstudio::model::ScheduleCompact scheduleCompact = openstudio::model::ScheduleCompact(model);
  openstudio::model::AirTerminalSingleDuctUncontrolled singleDuctTerminal = 
                                                         openstudio::model::AirTerminalSingleDuctUncontrolled(model,scheduleCompact);
  EXPECT_TRUE(scheduleCompact.scheduleTypeLimits());

  boost::optional<openstudio::model::LifeCycleCost> cost1 = openstudio::model::LifeCycleCost::createLifeCycleCost("Install", airLoopHVAC, 1000.0, "CostPerEach", "Construction");
  ASSERT_TRUE(cost1);
  boost::optional<openstudio::model::LifeCycleCost> cost2 = openstudio::model::LifeCycleCost::createLifeCycleCost("Ducting", airLoopHVAC, 200.0, "CostPerThermalZone", "Construction");
  ASSERT_TRUE(cost2);
  boost::optional<openstudio::model::LifeCycleCost> cost3 = openstudio::model::LifeCycleCost::createLifeCycleCost("Terminals", airLoopHVAC, 66.0, "CostPerArea", "Construction");
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
  openstudio::model::Model model; 

  openstudio::model::AirLoopHVAC airLoopHVAC = openstudio::model::AirLoopHVAC(model);
  openstudio::model::ThermalZone thermalZone = openstudio::model::ThermalZone(model);
  openstudio::model::ThermalZone thermalZone2 = openstudio::model::ThermalZone(model);
  openstudio::model::ScheduleCompact scheduleCompact = openstudio::model::ScheduleCompact(model);
  openstudio::model::AirTerminalSingleDuctUncontrolled singleDuctTerminal = 
                                                         openstudio::model::AirTerminalSingleDuctUncontrolled(model,scheduleCompact);
  EXPECT_TRUE(scheduleCompact.scheduleTypeLimits());


  ASSERT_TRUE(airLoopHVAC.addBranchForZone(thermalZone,singleDuctTerminal));

  EXPECT_EQ(1u, airLoopHVAC.thermalZones().size());

  EXPECT_FALSE(airLoopHVAC.addBranchForZone(thermalZone2,singleDuctTerminal));

  EXPECT_EQ(1u, airLoopHVAC.thermalZones().size());

  openstudio::model::AirTerminalSingleDuctUncontrolled term2 = 
    singleDuctTerminal.clone(model).cast<openstudio::model::AirTerminalSingleDuctUncontrolled>();

  EXPECT_TRUE(airLoopHVAC.addBranchForZone(thermalZone2,term2));

  EXPECT_EQ(2u, airLoopHVAC.thermalZones().size());
}
