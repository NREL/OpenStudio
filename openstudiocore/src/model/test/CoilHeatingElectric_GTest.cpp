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
#include "../AirLoopHVACOutdoorAirSystem.hpp"
#include "../ControllerOutdoorAir.hpp"
#include "../PlantLoop.hpp"
#include "../Model.hpp"
#include "../Node.hpp"
#include "../Node_Impl.hpp"
#include "../CoilHeatingElectric.hpp"
#include "../CoilHeatingElectric_Impl.hpp"
#include "../CoilCoolingWater.hpp"
#include "../Schedule.hpp"
#include "../AirLoopHVACZoneSplitter.hpp"

using namespace openstudio::model;

TEST_F(ModelFixture,CoilHeatingElectric_CoilHeatingElectric)
{
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  ASSERT_EXIT ( 
  {  
     Model m; 
     Schedule s = m.alwaysOnDiscreteSchedule();

     CoilHeatingElectric coil(m,s); 

     exit(0); 
  } ,
    ::testing::ExitedWithCode(0), "" );
}

TEST_F(ModelFixture,CoilHeatingElectric_addToNode) {
  Model m;
  Schedule s = m.alwaysOnDiscreteSchedule();

  CoilHeatingElectric testObject(m, s);

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

  CoilHeatingElectric testObject2(m, s);
  CoilHeatingElectric testObject3(m, s);

  if( boost::optional<Node> OANode = outdoorAirSystem.outboardOANode() ) {
    EXPECT_TRUE(testObject2.addToNode(*OANode));
    EXPECT_EQ( (unsigned)5, airLoop.supplyComponents().size() );
    EXPECT_EQ( (unsigned)3, outdoorAirSystem.oaComponents().size() );
  }

  if( boost::optional<Node> reliefNode = outdoorAirSystem.outboardReliefNode() ) {
    EXPECT_FALSE(testObject3.addToNode(*reliefNode));
    EXPECT_EQ( (unsigned)5, airLoop.supplyComponents().size() );
    EXPECT_EQ( (unsigned)1, outdoorAirSystem.reliefComponents().size() );
  }

  CoilHeatingElectric testObjectClone = testObject.clone(m).cast<CoilHeatingElectric>();
  supplyOutletNode = airLoop.supplyOutletNode();

  EXPECT_TRUE(testObjectClone.addToNode(supplyOutletNode));
  EXPECT_EQ( (unsigned)7, airLoop.supplyComponents().size() );
}
