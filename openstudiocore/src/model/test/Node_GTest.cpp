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
#include "../Model.hpp"
#include "../Node.hpp"
#include "../Node_Impl.hpp"
#include "../Building.hpp"
#include "../Building_Impl.hpp"
#include "../ModelObject_Impl.hpp"
#include "../StraightComponent_Impl.hpp"
#include "../AirLoopHVAC.hpp"
#include "../ThermalZone.hpp"
#include "../ScheduleConstant.hpp"
#include "../Schedule.hpp"
#include "../FanVariableVolume.hpp"
#include "../CoilHeatingElectric.hpp"
#include "../SetpointManagerFollowOutdoorAirTemperature.hpp"
#include "../SetpointManagerFollowOutdoorAirTemperature_Impl.hpp"
#include "../SetpointManagerMixedAir.hpp"
#include "../SetpointManagerMixedAir_Impl.hpp"
#include "../SetpointManagerOutdoorAirReset.hpp"
#include "../SetpointManagerOutdoorAirReset_Impl.hpp"
#include "../SetpointManagerScheduled.hpp"
#include "../SetpointManagerScheduled_Impl.hpp"
#include "../SetpointManagerSingleZoneReheat.hpp"
#include "../SetpointManagerSingleZoneReheat_Impl.hpp"
#include "../SetpointManagerWarmest.hpp"
#include "../SetpointManagerWarmest_Impl.hpp"
#include <utilities/idd/OS_SetpointManager_Scheduled_FieldEnums.hxx>

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, Node_Casting)
{
  Model model;

  Node node = Node(model);
  EXPECT_NO_THROW(node.cast<Node>());
  EXPECT_NO_THROW(node.cast<StraightComponent>());
  EXPECT_NO_THROW(node.cast<ModelObject>());
  EXPECT_THROW(node.cast<Building>(), std::bad_cast);

  EXPECT_TRUE(node.optionalCast<Node>());
  EXPECT_TRUE(node.optionalCast<StraightComponent>());
  EXPECT_TRUE(node.optionalCast<ModelObject>());
  EXPECT_FALSE(node.optionalCast<Building>());

  ModelObject obj = Node(model);
  EXPECT_NO_THROW(obj.cast<Node>());
  EXPECT_NO_THROW(obj.cast<StraightComponent>());
  EXPECT_NO_THROW(obj.cast<ModelObject>());
  EXPECT_THROW(obj.cast<Building>(), std::bad_cast);

  EXPECT_TRUE(obj.optionalCast<Node>());
  EXPECT_TRUE(obj.optionalCast<StraightComponent>());
  EXPECT_TRUE(obj.optionalCast<ModelObject>());
  EXPECT_FALSE(obj.optionalCast<Building>());
}

TEST_F(ModelFixture, Node_SetpointManagers)
{
  Model m;
  AirLoopHVAC airloop(m);
  Node _node = airloop.supplyOutletNode();
  ScheduleConstant tempSch(m);
  tempSch.setValue(50);

  SetpointManagerScheduled spm_1(m,tempSch);
  spm_1.setControlVariable("MaximumTemperature");

  SetpointManagerScheduled spm_2(m,tempSch);
  spm_2.setControlVariable("MinimumTemperature");

  SetpointManagerScheduled spm_3(m,tempSch);
  spm_3.setControlVariable("Temperature");

  spm_1.addToNode(_node);
  spm_2.addToNode(_node);
  spm_3.addToNode(_node);

  std::vector<SetpointManager> _setpointManagers = _node.setpointManagers();
  EXPECT_EQ(3, _setpointManagers.size());

  _node.getImpl<openstudio::model::detail::Node_Impl>()->removeSetpointManagers();

  _setpointManagers = _node.setpointManagers();
  EXPECT_EQ(0, _setpointManagers.size());
}

TEST_F(ModelFixture, Node_Remove)
{
  Model m;
  Node _node(m);
  ScheduleConstant tempSch(m);
  tempSch.setValue(50);

  SetpointManagerScheduled spm_1(m,tempSch);
  spm_1.setControlVariable("MaximumTemperature");

  SetpointManagerScheduled spm_2(m,tempSch);
  spm_2.setControlVariable("MinimumTemperature");

  SetpointManagerScheduled spm_3(m,tempSch);
  spm_3.setControlVariable("Temperature");

  EXPECT_TRUE(spm_1.setPointer(OS_SetpointManager_ScheduledFields::SetpointNodeorNodeListName,_node.handle()));
  EXPECT_TRUE(spm_2.setPointer(OS_SetpointManager_ScheduledFields::SetpointNodeorNodeListName,_node.handle()));
  EXPECT_TRUE(spm_3.setPointer(OS_SetpointManager_ScheduledFields::SetpointNodeorNodeListName,_node.handle()));

  std::vector<SetpointManager> _setpointManagers = _node.setpointManagers();
  EXPECT_EQ(3, _setpointManagers.size());
  std::vector<SetpointManager> setpointManagers = m.getModelObjects<SetpointManager>();
  EXPECT_EQ(3, setpointManagers.size());
  std::vector<Node> nodes = m.getModelObjects<Node>();
  EXPECT_EQ(1, nodes.size());

  EXPECT_TRUE(_node.isRemovable());
  _node.remove();

  _setpointManagers = _node.setpointManagers();
  EXPECT_EQ(0, _setpointManagers.size());
  setpointManagers = m.getModelObjects<SetpointManager>();
  EXPECT_EQ(0, setpointManagers.size());
  nodes = m.getModelObjects<Node>();
  EXPECT_EQ(0, nodes.size());
}

TEST_F(ModelFixture, Node_SetpointManagerSingleZoneReheat)
{
  Model m;
  AirLoopHVAC airloop(m);
  ThermalZone thermalZone(m);
  airloop.addBranchForZone(thermalZone);
  Node _node = airloop.supplyOutletNode();

  std::vector<SetpointManager> _setpointManagers = _node.setpointManagers();
  EXPECT_EQ(0, _setpointManagers.size());
  std::vector<SetpointManagerSingleZoneReheat> setpointManagerSingleZoneReheats = m.getModelObjects<SetpointManagerSingleZoneReheat>();
  EXPECT_EQ(0, setpointManagerSingleZoneReheats.size());

  SetpointManagerSingleZoneReheat spm(m);
  spm.setControlVariable("Temperature");

  _node.addSetpointManager(spm);

  ASSERT_TRUE(spm.setpointNode());
  ASSERT_TRUE(spm.controlZone());
  EXPECT_EQ(_node, spm.setpointNode().get());
  EXPECT_EQ(thermalZone, spm.controlZone().get());

  _setpointManagers = _node.setpointManagers();
  EXPECT_EQ(1, _setpointManagers.size());
  setpointManagerSingleZoneReheats = m.getModelObjects<SetpointManagerSingleZoneReheat>();
  EXPECT_EQ(1, setpointManagerSingleZoneReheats.size());

  OptionalSetpointManagerSingleZoneReheat nodeSPM = _node.getSetpointManagerSingleZoneReheat();
  ASSERT_TRUE(nodeSPM);
  EXPECT_EQ(spm, *nodeSPM);

  _node.removeSetpointManagerSingleZoneReheat();

  _setpointManagers = _node.setpointManagers();
  EXPECT_EQ(0, _setpointManagers.size());
  setpointManagerSingleZoneReheats = m.getModelObjects<SetpointManagerSingleZoneReheat>();
  EXPECT_EQ(0, setpointManagerSingleZoneReheats.size());
}

TEST_F(ModelFixture, Node_SetpointManagerMixedAir)
{
  Model m;
  AirLoopHVAC airloop(m);
  Node _node = airloop.supplyOutletNode();
  Schedule s = m.alwaysOnDiscreteSchedule();
  FanVariableVolume fan(m,s);
  CoilHeatingElectric coil(m,s);

  coil.addToNode(_node);
  fan.addToNode(_node);
  Node node = coil.outletModelObject()->cast<Node>();

  std::vector<SetpointManager> _setpointManagers = node.setpointManagers();
  EXPECT_EQ(0, _setpointManagers.size());
  std::vector<SetpointManagerMixedAir> setpointManagerMixedAirs = m.getModelObjects<SetpointManagerMixedAir>();
  EXPECT_EQ(0, setpointManagerMixedAirs.size());

  SetpointManagerMixedAir spm(m);
  spm.setControlVariable("Temperature");

  node.addSetpointManager(spm);

  ASSERT_TRUE(spm.setpointNode());
  ASSERT_TRUE(spm.fanInletNode());
  ASSERT_TRUE(spm.fanOutletNode());
  ASSERT_TRUE(spm.referenceSetpointNode());
  EXPECT_EQ(node, spm.setpointNode().get());
  EXPECT_EQ(_node, spm.referenceSetpointNode().get());
  EXPECT_EQ(fan.inletModelObject()->cast<Node>(), spm.fanInletNode().get());
  EXPECT_EQ(fan.outletModelObject()->cast<Node>(), spm.fanOutletNode().get());

  _setpointManagers = node.setpointManagers();
  EXPECT_EQ(1, _setpointManagers.size());
  setpointManagerMixedAirs = m.getModelObjects<SetpointManagerMixedAir>();
  EXPECT_EQ(1, setpointManagerMixedAirs.size());

  OptionalSetpointManagerMixedAir nodeSPM = node.getSetpointManagerMixedAir();
  ASSERT_TRUE(nodeSPM);
  EXPECT_EQ(spm, *nodeSPM);

  node.removeSetpointManagerMixedAir();

  _setpointManagers = node.setpointManagers();
  EXPECT_EQ(0, _setpointManagers.size());
  setpointManagerMixedAirs = m.getModelObjects<SetpointManagerMixedAir>();
  EXPECT_EQ(0, setpointManagerMixedAirs.size());
}

TEST_F(ModelFixture, Node_SetpointManagerScheduled)
{
  Model m;
  AirLoopHVAC airloop(m);
  Node _node = airloop.supplyOutletNode();
  ScheduleConstant tempSch(m);
  tempSch.setValue(50);

  std::vector<SetpointManager> _setpointManagers = _node.setpointManagers();
  EXPECT_EQ(0, _setpointManagers.size());
  std::vector<SetpointManagerScheduled> setpointManagerScheduleds = m.getModelObjects<SetpointManagerScheduled>();
  EXPECT_EQ(0, setpointManagerScheduleds.size());

  SetpointManagerScheduled spm(m,tempSch);
  spm.setControlVariable("Temperature");

  _node.addSetpointManager(spm);

  _setpointManagers = _node.setpointManagers();
  EXPECT_EQ(1, _setpointManagers.size());
  setpointManagerScheduleds = m.getModelObjects<SetpointManagerScheduled>();
  EXPECT_EQ(1, setpointManagerScheduleds.size());

  boost::optional<SetpointManagerScheduled> nodeSPM = _node.setpointManagerScheduled();
  ASSERT_TRUE(nodeSPM);
  EXPECT_EQ(spm, *nodeSPM);

  _node.removeSetpointManagerScheduled();

  _setpointManagers = _node.setpointManagers();
  EXPECT_EQ(0, _setpointManagers.size());
  setpointManagerScheduleds = m.getModelObjects<SetpointManagerScheduled>();
  EXPECT_EQ(0, setpointManagerScheduleds.size());
}

TEST_F(ModelFixture, Node_SetpointManagerFollowOutdoorAirTemperature)
{
  Model m;
  AirLoopHVAC airloop(m);
  Node _node = airloop.supplyOutletNode();

  std::vector<SetpointManager> _setpointManagers = _node.setpointManagers();
  EXPECT_EQ(0, _setpointManagers.size());
  std::vector<SetpointManagerFollowOutdoorAirTemperature> setpointManagerFollowOutdoorAirTemperatures = m.getModelObjects<SetpointManagerFollowOutdoorAirTemperature>();
  EXPECT_EQ(0, setpointManagerFollowOutdoorAirTemperatures.size());

  SetpointManagerFollowOutdoorAirTemperature spm(m);
  spm.setControlVariable("Temperature");

  _node.addSetpointManager(spm);

  _setpointManagers = _node.setpointManagers();
  EXPECT_EQ(1, _setpointManagers.size());
  setpointManagerFollowOutdoorAirTemperatures = m.getModelObjects<SetpointManagerFollowOutdoorAirTemperature>();
  EXPECT_EQ(1, setpointManagerFollowOutdoorAirTemperatures.size());

  boost::optional<SetpointManagerFollowOutdoorAirTemperature> nodeSPM = _node.setpointManagerFollowOutdoorAirTemperature();
  ASSERT_TRUE(nodeSPM);
  EXPECT_EQ(spm, *nodeSPM);

  _node.removeSetpointManagerFollowOutdoorAirTemperature();

  _setpointManagers = _node.setpointManagers();
  EXPECT_EQ(0, _setpointManagers.size());
  setpointManagerFollowOutdoorAirTemperatures = m.getModelObjects<SetpointManagerFollowOutdoorAirTemperature>();
  EXPECT_EQ(0, setpointManagerFollowOutdoorAirTemperatures.size());
}

TEST_F(ModelFixture, Node_SetpointManagerOutdoorAirReset)
{
  Model m;
  AirLoopHVAC airloop(m);
  Node _node = airloop.supplyOutletNode();

  std::vector<SetpointManager> _setpointManagers = _node.setpointManagers();
  EXPECT_EQ(0, _setpointManagers.size());
  std::vector<SetpointManagerOutdoorAirReset> setpointManagerOutdoorAirResets = m.getModelObjects<SetpointManagerOutdoorAirReset>();
  EXPECT_EQ(0, setpointManagerOutdoorAirResets.size());

  SetpointManagerOutdoorAirReset spm(m);
  spm.setControlVariable("Temperature");

  _node.addSetpointManager(spm);

  _setpointManagers = _node.setpointManagers();
  EXPECT_EQ(1, _setpointManagers.size());
  setpointManagerOutdoorAirResets = m.getModelObjects<SetpointManagerOutdoorAirReset>();
  EXPECT_EQ(1, setpointManagerOutdoorAirResets.size());

  OptionalSetpointManagerOutdoorAirReset nodeSPM = _node.setpointManagerOutdoorAirReset();
  ASSERT_TRUE(nodeSPM);
  EXPECT_EQ(spm, *nodeSPM);

  _node.removeSetpointManagerOutdoorAirReset();

  _setpointManagers = _node.setpointManagers();
  EXPECT_EQ(0, _setpointManagers.size());
  setpointManagerOutdoorAirResets = m.getModelObjects<SetpointManagerOutdoorAirReset>();
  EXPECT_EQ(0, setpointManagerOutdoorAirResets.size());
}

TEST_F(ModelFixture, Node_SetpointManagerWarmest)
{
  Model m;
  AirLoopHVAC airloop(m);
  Node _node = airloop.supplyOutletNode();

  std::vector<SetpointManager> _setpointManagers = _node.setpointManagers();
  EXPECT_EQ(0, _setpointManagers.size());
  std::vector<SetpointManagerWarmest> setpointManagerWarmests = m.getModelObjects<SetpointManagerWarmest>();
  EXPECT_EQ(0, setpointManagerWarmests.size());

  SetpointManagerWarmest spm(m);
  spm.setControlVariable("Temperature");

  _node.addSetpointManagerWarmest(spm);

  _setpointManagers = _node.setpointManagers();
  EXPECT_EQ(1, _setpointManagers.size());
  setpointManagerWarmests = m.getModelObjects<SetpointManagerWarmest>();
  EXPECT_EQ(1, setpointManagerWarmests.size());

  OptionalSetpointManagerWarmest nodeSPM = _node.setpointManagerWarmest();
  ASSERT_TRUE(nodeSPM);
  EXPECT_EQ(spm, *nodeSPM);

  _node.removeSetpointManagerWarmest();

  _setpointManagers = _node.setpointManagers();
  EXPECT_EQ(0, _setpointManagers.size());
  setpointManagerWarmests = m.getModelObjects<SetpointManagerWarmest>();
  EXPECT_EQ(0, setpointManagerWarmests.size());
}

TEST_F(ModelFixture, Node_Children)
{
  Model m;
  AirLoopHVAC airloop(m);
  Node _node = airloop.supplyOutletNode();
  ScheduleConstant tempSch(m);
  tempSch.setValue(50);

  std::vector<SetpointManager> _setpointManagers = _node.setpointManagers();
  EXPECT_EQ(0, _setpointManagers.size());
  std::vector<SetpointManager> setpointManagers = m.getModelObjects<SetpointManager>();
  EXPECT_EQ(0, setpointManagers.size());

  SetpointManagerSingleZoneReheat spm_1(m);
  spm_1.setControlVariable("Temperature");

  SetpointManagerFollowOutdoorAirTemperature spm_2(m);
  spm_2.setControlVariable("MaximumTemperature");

  SetpointManagerScheduled spm_3(m,tempSch);
  spm_3.setControlVariable("MinimumTemperature");

  spm_1.addToNode(_node);
  spm_2.addToNode(_node);
  spm_3.addToNode(_node);

  _setpointManagers = _node.setpointManagers();
  EXPECT_EQ(3, _setpointManagers.size());
  setpointManagers = m.getModelObjects<SetpointManager>();
  EXPECT_EQ(3, setpointManagers.size());

  std::vector<ModelObject> _children = _node.getImpl<openstudio::model::detail::Node_Impl>()->children();
  EXPECT_EQ(3, _children.size());
}
