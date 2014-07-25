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

#include "../SetpointManagerOutdoorAirReset.hpp"
#include "../SetpointManagerOutdoorAirReset_Impl.hpp"
#include "../Model.hpp"
#include "../Node.hpp"
#include "../AirLoopHVAC.hpp"
#include "../PlantLoop.hpp"
#include "../Schedule.hpp"

#include "../../utilities/units/Quantity.hpp"
#include "../../utilities/units/Unit.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, SetpointManagerOutdoorAirReset_DefaultConstructor)
{
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  ASSERT_EXIT ( 
  {  
    Model m;
    SetpointManagerOutdoorAirReset testObject(m);

    exit(0); 
  } ,
    ::testing::ExitedWithCode(0), "" );
}

TEST_F(ModelFixture, SetpointManagerOutdoorAirReset_addToNode)
{
  Model m;
  AirLoopHVAC airloop(m);
  PlantLoop plantLoop(m);
  Node testObject = airloop.supplyOutletNode();

  SetpointManagerOutdoorAirReset spm_1(m);
  spm_1.setControlVariable("Temperature");

  SetpointManagerOutdoorAirReset spm_2(m);
  spm_2.setControlVariable("Temperature");

  SetpointManagerOutdoorAirReset spm_3(m);
  spm_3.setControlVariable("Temperature");

  EXPECT_TRUE(spm_1.addToNode(testObject));

  Node plantNode = plantLoop.supplyOutletNode();
  EXPECT_TRUE(spm_3.addToNode(plantNode));

  std::vector<SetpointManager> _setpointManagers = testObject.setpointManagers();
  EXPECT_EQ(1, _setpointManagers.size());
  std::vector<SetpointManagerOutdoorAirReset> setpointManagerOutdoorAirResets = m.getModelObjects<SetpointManagerOutdoorAirReset>();
  EXPECT_EQ(3, setpointManagerOutdoorAirResets.size());

  EXPECT_EQ(testObject, spm_1.setpointNode());
  EXPECT_TRUE(spm_2.addToNode(testObject));
  EXPECT_EQ(testObject, spm_2.setpointNode());

  _setpointManagers = testObject.setpointManagers();
  EXPECT_TRUE(std::find(_setpointManagers.begin(), _setpointManagers.end(), spm_1) == _setpointManagers.end());
  EXPECT_EQ(1, _setpointManagers.size());
  setpointManagerOutdoorAirResets = m.getModelObjects<SetpointManagerOutdoorAirReset>();
  EXPECT_EQ(2, setpointManagerOutdoorAirResets.size());
}

TEST_F(ModelFixture, SetpointManagerOutdoorAirReset_remove)
{
  Model m;
  AirLoopHVAC airloop(m);
  Node testObject = airloop.supplyOutletNode();

  SetpointManagerOutdoorAirReset spm(m);
  spm.setControlVariable("Temperature");

  EXPECT_TRUE(spm.addToNode(testObject));

  std::vector<SetpointManager> _setpointManagers = testObject.setpointManagers();
  EXPECT_EQ(1, _setpointManagers.size());
  std::vector<SetpointManagerOutdoorAirReset> setpointManagerOutdoorAirResets = m.getModelObjects<SetpointManagerOutdoorAirReset>();
  EXPECT_EQ(1, setpointManagerOutdoorAirResets.size());

  spm.remove();

  _setpointManagers = testObject.setpointManagers();
  EXPECT_EQ(0, _setpointManagers.size());
  setpointManagerOutdoorAirResets = m.getModelObjects<SetpointManagerOutdoorAirReset>();
  EXPECT_EQ(0, setpointManagerOutdoorAirResets.size());
}

TEST_F(ModelFixture, SetpointManagerOutdoorAirReset_clone)
{
  Model m;
  AirLoopHVAC airloop(m);
  Node outletNode = airloop.supplyOutletNode();

  SetpointManagerOutdoorAirReset testObject(m);
  testObject.setControlVariable("Temperature");
  testObject.addToNode(outletNode);
  ASSERT_TRUE(testObject.setpointNode());
  EXPECT_EQ(outletNode, testObject.setpointNode().get());

  SetpointManagerOutdoorAirReset testObjectClone = testObject.clone(m).cast<SetpointManagerOutdoorAirReset>();
  EXPECT_FALSE(testObjectClone.setpointNode());

  EXPECT_NE(testObject, testObjectClone);
  EXPECT_EQ(testObject.controlVariable(), testObjectClone.controlVariable());
  EXPECT_EQ(testObject.setpointatOutdoorLowTemperature(), testObjectClone.setpointatOutdoorLowTemperature());
  EXPECT_EQ(testObject.outdoorLowTemperature(), testObjectClone.outdoorLowTemperature());
  EXPECT_EQ(testObject.setpointatOutdoorHighTemperature(), testObjectClone.setpointatOutdoorHighTemperature());
  EXPECT_EQ(testObject.outdoorHighTemperature(), testObjectClone.outdoorHighTemperature());
  EXPECT_EQ("Temperature", testObjectClone.controlVariable());
  EXPECT_EQ(22.0, testObjectClone.setpointatOutdoorLowTemperature());
  EXPECT_EQ(10.0, testObjectClone.outdoorLowTemperature());
  EXPECT_EQ(10.0, testObjectClone.setpointatOutdoorHighTemperature());
  EXPECT_EQ(24.0, testObjectClone.outdoorHighTemperature());
}

TEST_F(ModelFixture, SetpointManagerOutdoorAirReset_customDataClone)
{
  Model m;
  AirLoopHVAC airloop(m);
  Node outletNode = airloop.supplyOutletNode();

  Schedule s(m.alwaysOnDiscreteSchedule());

  SetpointManagerOutdoorAirReset testObject(m);
  testObject.setControlVariable("Temperature");
  testObject.addToNode(outletNode);
  testObject.setSetpointatOutdoorLowTemperature(999.9);
  testObject.setOutdoorLowTemperature(999.9);
  testObject.setSetpointatOutdoorHighTemperature(999.9);
  testObject.setOutdoorHighTemperature(999.9);
  testObject.setSchedule(s);
  testObject.setSetpointatOutdoorLowTemperature2(999.9);
  ASSERT_TRUE(testObject.setpointNode());
  EXPECT_EQ(outletNode, testObject.setpointNode().get());

  SetpointManagerOutdoorAirReset testObjectClone = testObject.clone(m).cast<SetpointManagerOutdoorAirReset>();
  EXPECT_FALSE(testObjectClone.setpointNode());

  EXPECT_NE(testObject, testObjectClone);
  EXPECT_EQ(testObject.controlVariable(), testObjectClone.controlVariable());
  EXPECT_EQ(testObject.setpointatOutdoorLowTemperature(), testObjectClone.setpointatOutdoorLowTemperature());
  EXPECT_EQ(testObject.outdoorLowTemperature(), testObjectClone.outdoorLowTemperature());
  EXPECT_EQ(testObject.setpointatOutdoorHighTemperature(), testObjectClone.setpointatOutdoorHighTemperature());
  EXPECT_EQ(testObject.outdoorHighTemperature(), testObjectClone.outdoorHighTemperature());
  EXPECT_EQ(testObject.schedule(), testObjectClone.schedule());
  EXPECT_EQ(testObject.setpointatOutdoorLowTemperature2(), testObjectClone.setpointatOutdoorLowTemperature2());
  EXPECT_EQ("Temperature", testObjectClone.controlVariable());
  EXPECT_EQ(999.9, testObjectClone.setpointatOutdoorLowTemperature());
  EXPECT_EQ(999.9, testObjectClone.outdoorLowTemperature());
  EXPECT_EQ(999.9, testObjectClone.setpointatOutdoorHighTemperature());
  EXPECT_EQ(999.9, testObjectClone.outdoorHighTemperature());
  EXPECT_EQ(s, testObjectClone.schedule());
  EXPECT_EQ(999.9, testObjectClone.setpointatOutdoorLowTemperature2().get());
}

TEST_F(ModelFixture,SetpointManagerOutdoorAirReset_Fields_Quantity) {
  Model model;
  // TODO: Check constructor.
  SetpointManagerOutdoorAirReset setpointManagerOutdoorAirReset(model);

  Unit units = setpointManagerOutdoorAirReset.getSetpointatOutdoorLowTemperature(true).units(); // Get IP units.
  // TODO: Check that value is appropriate (within bounds)
  double value(1.0);
  Quantity testQ(value,units);
  EXPECT_TRUE(setpointManagerOutdoorAirReset.setSetpointatOutdoorLowTemperature(testQ));
  Quantity q = setpointManagerOutdoorAirReset.getSetpointatOutdoorLowTemperature(true);
  EXPECT_NEAR(value,q.value(),1.0E-8);
  EXPECT_EQ(units.standardString(),q.units().standardString());

  units = setpointManagerOutdoorAirReset.getOutdoorLowTemperature(true).units(); // Get IP units.
  // TODO: Check that value is appropriate (within bounds)
  value = 1.0;
  testQ = Quantity(value,units);
  EXPECT_TRUE(setpointManagerOutdoorAirReset.setOutdoorLowTemperature(testQ));
  q = setpointManagerOutdoorAirReset.getOutdoorLowTemperature(true);
  EXPECT_NEAR(value,q.value(),1.0E-8);
  EXPECT_EQ(units.standardString(),q.units().standardString());

  units = setpointManagerOutdoorAirReset.getSetpointatOutdoorHighTemperature(true).units(); // Get IP units.
  // TODO: Check that value is appropriate (within bounds)
  value = 1.0;
  testQ = Quantity(value,units);
  EXPECT_TRUE(setpointManagerOutdoorAirReset.setSetpointatOutdoorHighTemperature(testQ));
  q = setpointManagerOutdoorAirReset.getSetpointatOutdoorHighTemperature(true);
  EXPECT_NEAR(value,q.value(),1.0E-8);
  EXPECT_EQ(units.standardString(),q.units().standardString());

  units = setpointManagerOutdoorAirReset.getOutdoorHighTemperature(true).units(); // Get IP units.
  // TODO: Check that value is appropriate (within bounds)
  value = 1.0;
  testQ = Quantity(value,units);
  EXPECT_TRUE(setpointManagerOutdoorAirReset.setOutdoorHighTemperature(testQ));
  q = setpointManagerOutdoorAirReset.getOutdoorHighTemperature(true);
  EXPECT_NEAR(value,q.value(),1.0E-8);
  EXPECT_EQ(units.standardString(),q.units().standardString());

  units = setpointManagerOutdoorAirReset.getSetpointatOutdoorLowTemperature2(true).units(); // Get IP units.
  // TODO: Check that value is appropriate (within bounds)
  value = 1.0;
  testQ = Quantity(value,units);
  EXPECT_TRUE(setpointManagerOutdoorAirReset.setSetpointatOutdoorLowTemperature2(testQ));
  OSOptionalQuantity optq = setpointManagerOutdoorAirReset.getSetpointatOutdoorLowTemperature2(true);
  ASSERT_TRUE(optq.isSet());
  EXPECT_NEAR(value,optq.get().value(),1.0E-8);
  EXPECT_EQ(units.standardString(),optq.units().standardString());

  units = setpointManagerOutdoorAirReset.getOutdoorLowTemperature2(true).units(); // Get IP units.
  // TODO: Check that value is appropriate (within bounds)
  value = 1.0;
  testQ = Quantity(value,units);
  EXPECT_TRUE(setpointManagerOutdoorAirReset.setOutdoorLowTemperature2(testQ));
  optq = setpointManagerOutdoorAirReset.getOutdoorLowTemperature2(true);
  ASSERT_TRUE(optq.isSet());
  EXPECT_NEAR(value,optq.get().value(),1.0E-8);
  EXPECT_EQ(units.standardString(),optq.units().standardString());

  units = setpointManagerOutdoorAirReset.getSetpointatOutdoorHighTemperature2(true).units(); // Get IP units.
  // TODO: Check that value is appropriate (within bounds)
  value = 1.0;
  testQ = Quantity(value,units);
  EXPECT_TRUE(setpointManagerOutdoorAirReset.setSetpointatOutdoorHighTemperature2(testQ));
  optq = setpointManagerOutdoorAirReset.getSetpointatOutdoorHighTemperature2(true);
  ASSERT_TRUE(optq.isSet());
  EXPECT_NEAR(value,optq.get().value(),1.0E-8);
  EXPECT_EQ(units.standardString(),optq.units().standardString());

  units = setpointManagerOutdoorAirReset.getOutdoorHighTemperature2(true).units(); // Get IP units.
  // TODO: Check that value is appropriate (within bounds)
  value = 1.0;
  testQ = Quantity(value,units);
  EXPECT_TRUE(setpointManagerOutdoorAirReset.setOutdoorHighTemperature2(testQ));
  optq = setpointManagerOutdoorAirReset.getOutdoorHighTemperature2(true);
  ASSERT_TRUE(optq.isSet());
  EXPECT_NEAR(value,optq.get().value(),1.0E-8);
  EXPECT_EQ(units.standardString(),optq.units().standardString());
}
