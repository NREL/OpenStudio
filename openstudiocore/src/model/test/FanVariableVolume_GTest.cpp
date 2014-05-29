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
#include "../FanVariableVolume.hpp"
#include "../FanVariableVolume_Impl.hpp"
#include "../Schedule.hpp"
#include "../AirLoopHVACOutdoorAirSystem.hpp"
#include "../ControllerOutdoorAir.hpp"
#include "../AirLoopHVAC.hpp"
#include "../PlantLoop.hpp"
#include "../Node.hpp"
#include "../Node_Impl.hpp"
#include "../AirLoopHVACZoneSplitter.hpp"

using namespace openstudio::model;

TEST_F(ModelFixture,FanVariableVolume_FanVariableVolume)
{
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  ASSERT_EXIT ( 
  {  
    Model m;
    Schedule s = m.alwaysOnDiscreteSchedule();
    FanVariableVolume testObject(m,s);

    exit(0); 
  } ,
    ::testing::ExitedWithCode(0), "" );
}

TEST_F(ModelFixture,FanVariableVolume_addToNode) {
  Model m;
  Schedule s = m.alwaysOnDiscreteSchedule();
  FanVariableVolume testObject(m,s);

  AirLoopHVAC airLoop(m);
  ControllerOutdoorAir controllerOutdoorAir(m);
  AirLoopHVACOutdoorAirSystem outdoorAirSystem(m,controllerOutdoorAir);

  Node supplyOutletNode = airLoop.supplyOutletNode();
  outdoorAirSystem.addToNode(supplyOutletNode);

  EXPECT_TRUE(testObject.addToNode(supplyOutletNode));
  EXPECT_EQ( (unsigned)5, airLoop.supplyComponents().size() );

  Node inletNode = airLoop.zoneSplitter().lastOutletModelObject()->cast<Node>();

  EXPECT_FALSE(testObject.addToNode(inletNode));
  EXPECT_EQ((unsigned)5, airLoop.demandComponents().size());

  PlantLoop plantLoop(m);
  supplyOutletNode = plantLoop.supplyOutletNode();
  EXPECT_FALSE(testObject.addToNode(supplyOutletNode));
  EXPECT_EQ( (unsigned)5, plantLoop.supplyComponents().size() );

  Node demandOutletNode = plantLoop.demandOutletNode();
  EXPECT_FALSE(testObject.addToNode(demandOutletNode));
  EXPECT_EQ( (unsigned)5, plantLoop.demandComponents().size() );

  if( boost::optional<Node> OANode = outdoorAirSystem.outboardOANode() ) {
    EXPECT_FALSE(testObject.addToNode(*OANode));
    EXPECT_EQ( (unsigned)5, airLoop.supplyComponents().size() );
    EXPECT_EQ( (unsigned)1, outdoorAirSystem.oaComponents().size() );
  }

  if( boost::optional<Node> reliefNode = outdoorAirSystem.outboardReliefNode() ) {
    EXPECT_FALSE(testObject.addToNode(*reliefNode));
    EXPECT_EQ( (unsigned)5, airLoop.supplyComponents().size() );
    EXPECT_EQ( (unsigned)1, outdoorAirSystem.reliefComponents().size() );
  }

  FanVariableVolume testObjectClone = testObject.clone(m).cast<FanVariableVolume>();
  supplyOutletNode = airLoop.supplyOutletNode();

  EXPECT_TRUE(testObjectClone.addToNode(supplyOutletNode));
  EXPECT_EQ( (unsigned)7, airLoop.supplyComponents().size() );

  FanVariableVolume fan2(m,s);
  EXPECT_FALSE(fan2.addToNode(supplyOutletNode));
  EXPECT_EQ( (unsigned)7, airLoop.supplyComponents().size() );
}
