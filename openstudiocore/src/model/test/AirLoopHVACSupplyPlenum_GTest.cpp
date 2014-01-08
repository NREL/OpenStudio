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
#include <model/AirLoopHVACSupplyPlenum.hpp>
#include <model/AirLoopHVACSupplyPlenum_Impl.hpp>
#include <model/ThermalZone.hpp>
#include <model/ThermalZone_Impl.hpp>
#include <model/AirLoopHVAC.hpp>
#include <model/AirLoopHVAC_Impl.hpp>
#include <model/Node.hpp>
#include <model/Node_Impl.hpp>
#include <model/HVACTemplates.hpp>
#include <model/AirTerminalSingleDuctUncontrolled.hpp>
#include <model/AirTerminalSingleDuctUncontrolled_Impl.hpp>
#include <model/AirTerminalSingleDuctVAVReheat.hpp>
#include <model/AirTerminalSingleDuctVAVReheat_Impl.hpp>

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture,AirLoopHVACSupplyPlenum)
{
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  ASSERT_EXIT ( 
  {  
     model::Model m; 

     model::AirLoopHVACSupplyPlenum plenum(m); 

     exit(0); 
  } ,
    ::testing::ExitedWithCode(0), "" );

  // Add to system 3, on node directly after terminal
  {
    Model m;

    AirLoopHVACSupplyPlenum plenum(m); 

    AirLoopHVAC system3 = addSystemType3(m).cast<AirLoopHVAC>(); 

    AirTerminalSingleDuctUncontrolled terminal = system3.demandComponents(AirTerminalSingleDuctUncontrolled::iddObjectType()).front().cast<AirTerminalSingleDuctUncontrolled>();

    Node node = terminal.outletModelObject()->cast<Node>();

    ASSERT_TRUE(plenum.addToNode(node));

    ASSERT_FALSE(plenum.addToNode(node));
  }

  // Add to system 7, on node directly before terminal
  {
    Model m;

    AirLoopHVACSupplyPlenum plenum(m); 

    AirLoopHVAC system7 = addSystemType7(m).cast<AirLoopHVAC>(); 

    AirTerminalSingleDuctVAVReheat terminal = system7.demandComponents(AirTerminalSingleDuctVAVReheat::iddObjectType()).front().cast<AirTerminalSingleDuctVAVReheat>();

    Node node = terminal.inletModelObject()->cast<Node>();

    ASSERT_TRUE(plenum.addToNode(node));

    ASSERT_FALSE(plenum.addToNode(node));
  }

  // Make sure that other nodes fail
  {
    Model m;

    AirLoopHVACSupplyPlenum plenum(m); 

    AirLoopHVAC system7 = addSystemType7(m).cast<AirLoopHVAC>(); 

    AirTerminalSingleDuctVAVReheat terminal = system7.demandComponents(AirTerminalSingleDuctVAVReheat::iddObjectType()).front().cast<AirTerminalSingleDuctVAVReheat>();

    Node node = terminal.outletModelObject()->cast<Node>();

    ASSERT_FALSE(plenum.addToNode(node));

    Node demandInletNode = system7.demandInletNode();

    ASSERT_FALSE(plenum.addToNode(demandInletNode));
  }

  // Make sure that system branches that already have a plenum wont accept another plenum
  {
    Model m;

    AirLoopHVACSupplyPlenum plenum(m); 

    AirLoopHVAC system5 = addSystemType5(m).cast<AirLoopHVAC>(); 

    AirTerminalSingleDuctVAVReheat terminal = system5.demandComponents(AirTerminalSingleDuctVAVReheat::iddObjectType()).front().cast<AirTerminalSingleDuctVAVReheat>();

    Node node = terminal.inletModelObject()->cast<Node>();

    ASSERT_TRUE(plenum.addToNode(node));

    AirLoopHVACSupplyPlenum plenum2(m); 

    ASSERT_FALSE(plenum2.addToNode(node));
  }
  
}

