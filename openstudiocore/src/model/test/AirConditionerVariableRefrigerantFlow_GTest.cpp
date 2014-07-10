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
#include "../AirConditionerVariableRefrigerantFlow.hpp"
#include "../AirConditionerVariableRefrigerantFlow_Impl.hpp"
#include "../ThermalZone.hpp"
#include "../ZoneHVACTerminalUnitVariableRefrigerantFlow.hpp"
#include "../ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl.hpp"
#include "../AirLoopHVAC.hpp"
#include "../PlantLoop.hpp"
#include "../Node.hpp"
#include "../Node_Impl.hpp"
#include "../AirLoopHVACZoneSplitter.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture,AirConditionerVariableRefrigerantFlow)
{
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  ASSERT_EXIT ( 
  {  
     Model m; 
     AirConditionerVariableRefrigerantFlow vrf(m); 

     exit(0); 
  } ,
    ::testing::ExitedWithCode(0), "" );

   Model m; 
   AirConditionerVariableRefrigerantFlow vrf(m); 

  for( int i = 0; i != 5; i++ )
  {
    ThermalZone zone(m);

    ZoneHVACTerminalUnitVariableRefrigerantFlow vrfTerminal(m);

    ASSERT_TRUE(vrfTerminal.addToThermalZone(zone));

    vrf.addTerminal(vrfTerminal);
  }

  ASSERT_EQ(5u,vrf.terminals().size());

  Model m2;

  boost::optional<AirConditionerVariableRefrigerantFlow> vrfClone = vrf.clone(m2).optionalCast<AirConditionerVariableRefrigerantFlow>(); 
  ASSERT_TRUE(vrfClone);

  ASSERT_TRUE(vrfClone->terminals().empty()); 

  ASSERT_TRUE(! vrfClone->remove().empty()); 
}

TEST_F(ModelFixture,AirConditionerVariableRefrigerantFlow_addToNode) {
  Model m;
  AirConditionerVariableRefrigerantFlow testObject(m);

  AirLoopHVAC airLoop(m);

  Node supplyOutletNode = airLoop.supplyOutletNode();

  EXPECT_FALSE(testObject.addToNode(supplyOutletNode));
  EXPECT_EQ( (unsigned)2, airLoop.supplyComponents().size() );

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
}
