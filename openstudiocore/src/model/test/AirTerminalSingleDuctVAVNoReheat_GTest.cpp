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
#include <model/test/ModelFixture.hpp>
#include <model/AirTerminalSingleDuctVAVNoReheat.hpp>
#include <model/AirTerminalSingleDuctVAVNoReheat_Impl.hpp>
#include <model/AirLoopHVAC.hpp>
#include <model/Model.hpp>
#include <model/Node.hpp>
#include <model/Node_Impl.hpp>
#include <model/ScheduleCompact.hpp>

using namespace openstudio;
using namespace openstudio::model;


TEST_F(ModelFixture,AirTerminalSingleDuctVAVNoReheat_isMaximumAirFlowRateAutosized) {
  Model model;

  AirTerminalSingleDuctVAVNoReheat airTerminalSingleDuctVAVNoReheat(model);

  airTerminalSingleDuctVAVNoReheat.autosizeMaximumAirFlowRate();
  EXPECT_TRUE(airTerminalSingleDuctVAVNoReheat.isMaximumAirFlowRateAutosized());
}

TEST_F(ModelFixture,AirTerminalSingleDuctVAVNoReheat_ConstantMinimumAirFlowFraction) {
  Model model;
  
  AirTerminalSingleDuctVAVNoReheat airTerminalSingleDuctVAVNoReheat(model);

  airTerminalSingleDuctVAVNoReheat.setConstantMinimumAirFlowFraction(0.3);
  boost::optional<double> value = airTerminalSingleDuctVAVNoReheat.constantMinimumAirFlowFraction();
  if (value) {
	EXPECT_NEAR(value.get(),0.3,1.0E-8);
  }
}

//Two important tests to do
TEST(AirTerminalSingleDuctVAVNoReheat,AirTerminalSingleDuctVAVNoReheat_Clone){

  openstudio::model::Model m;

  //make an object to clone, and edit some property to make sure the clone worked

  openstudio::model::AirTerminalSingleDuctVAVNoReheat AirTerminalSingleDuctVAVNoReheat(m);

  AirTerminalSingleDuctVAVNoReheat.setConstantMinimumAirFlowFraction(0.12345);

  //clone into the same model
  
  openstudio::model::AirTerminalSingleDuctVAVNoReheat AirTerminalSingleDuctVAVNoReheatClone = AirTerminalSingleDuctVAVNoReheat.clone(m).cast<openstudio::model::AirTerminalSingleDuctVAVNoReheat>();

  boost::optional<double> value = AirTerminalSingleDuctVAVNoReheatClone.constantMinimumAirFlowFraction();
  if (value) {
	EXPECT_NEAR(value.get(),0.12345,1.0E-8);
  }

  //clone into another model

  openstudio::model::Model m2;

  openstudio::model::AirTerminalSingleDuctVAVNoReheat AirTerminalSingleDuctVAVNoReheatClone2 = AirTerminalSingleDuctVAVNoReheat.clone(m2).cast<openstudio::model::AirTerminalSingleDuctVAVNoReheat>();

  value = AirTerminalSingleDuctVAVNoReheatClone2.constantMinimumAirFlowFraction();
  if (value) EXPECT_NEAR(value.get(),0.12345,1.0E-8);
}

TEST(AirTerminalSingleDuctVAVNoReheat,AirTerminalSingleDuctVAVNoReheat_addToNode)
{
  model::Model m; 

//  model::ScheduleCompact s(m);
  
  model::AirTerminalSingleDuctVAVNoReheat atuVAVNoReheat(m); 

  model::AirLoopHVAC airLoop(m);

  airLoop.addBranchForHVACComponent(atuVAVNoReheat);

//  model::Node supplyOutletNode = airLoop.supplyOutletNode();

//  atuVAVNoReheat.addToNode(supplyOutletNode);

  ASSERT_EQ((unsigned)7, airLoop.demandComponents().size());
}