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
#include "../AirLoopHVACReturnPlenum.hpp"
#include "../AirLoopHVACReturnPlenum_Impl.hpp"
#include "../AirLoopHVACSupplyPlenum.hpp"
#include "../AirLoopHVACSupplyPlenum_Impl.hpp"
#include "../ThermalZone.hpp"
#include "../ThermalZone_Impl.hpp"
#include "../Node.hpp"
#include "../Node_Impl.hpp"
#include "../AirLoopHVAC.hpp"
#include "../AirLoopHVAC_Impl.hpp"
#include "../AirTerminalSingleDuctVAVReheat.hpp"
#include "../AirTerminalSingleDuctVAVReheat_Impl.hpp"
#include "../HVACTemplates.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture,Plenums)
{
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  ASSERT_EXIT ( 
  {  
     model::Model m; 

     model::AirLoopHVACReturnPlenum returnPlenum(m); 
     model::AirLoopHVACSupplyPlenum supplyPlenum(m); 

     exit(0); 
  } ,
    ::testing::ExitedWithCode(0), "" );

  {
    Model m;

    AirLoopHVAC airLoop(m);

    ThermalZone supplyPlenumZone(m);
    ThermalZone returnPlenumZone(m);

    ThermalZone conditionedZone1(m);
    ThermalZone conditionedZone2(m);
    ThermalZone conditionedZone3(m);

    ASSERT_TRUE(airLoop.addBranchForZone(conditionedZone1));
    ASSERT_TRUE(airLoop.addBranchForZone(conditionedZone2));

    ASSERT_EQ(10u,airLoop.demandComponents().size());

    ASSERT_TRUE(supplyPlenumZone.canBePlenum());
    ASSERT_TRUE(conditionedZone1.setSupplyPlenum(supplyPlenumZone));

    ASSERT_TRUE(returnPlenumZone.canBePlenum());
    ASSERT_TRUE(conditionedZone1.setReturnPlenum(returnPlenumZone));

    ASSERT_EQ(14u,airLoop.demandComponents().size());

    ASSERT_TRUE(conditionedZone2.setSupplyPlenum(supplyPlenumZone));
    ASSERT_TRUE(conditionedZone2.setReturnPlenum(returnPlenumZone));

    ASSERT_EQ(14u,airLoop.demandComponents().size());
    ASSERT_EQ(1u,airLoop.demandComponents(AirLoopHVACSupplyPlenum::iddObjectType()).size());
    ASSERT_EQ(1u,airLoop.demandComponents(AirLoopHVACReturnPlenum::iddObjectType()).size());

    ASSERT_TRUE(airLoop.addBranchForZone(conditionedZone3));
    ASSERT_EQ(17u,airLoop.demandComponents().size());

    ThermalZone supplyPlenumZone2(m);
    ThermalZone returnPlenumZone2(m);

    ThermalZone conditionedZone4(m);
    ThermalZone conditionedZone5(m);

    ASSERT_TRUE(airLoop.addBranchForZone(conditionedZone4));
    ASSERT_TRUE(airLoop.addBranchForZone(conditionedZone5));
    ASSERT_EQ(23u,airLoop.demandComponents().size());

    ASSERT_TRUE(conditionedZone4.setSupplyPlenum(supplyPlenumZone2));
    ASSERT_TRUE(conditionedZone4.setReturnPlenum(returnPlenumZone2));

    ASSERT_TRUE(conditionedZone5.setSupplyPlenum(supplyPlenumZone2));
    ASSERT_TRUE(conditionedZone5.setReturnPlenum(returnPlenumZone2));

    ASSERT_EQ(27u,airLoop.demandComponents().size());
    ASSERT_EQ(2u,airLoop.demandComponents(AirLoopHVACSupplyPlenum::iddObjectType()).size());
    ASSERT_EQ(2u,airLoop.demandComponents(AirLoopHVACReturnPlenum::iddObjectType()).size());

    ASSERT_TRUE(conditionedZone4.setSupplyPlenum(supplyPlenumZone));
    ASSERT_TRUE(conditionedZone4.setReturnPlenum(returnPlenumZone));
    ASSERT_EQ(27u,airLoop.demandComponents().size());
    ASSERT_EQ(2u,airLoop.demandComponents(AirLoopHVACSupplyPlenum::iddObjectType()).size());
    ASSERT_EQ(2u,airLoop.demandComponents(AirLoopHVACReturnPlenum::iddObjectType()).size());

    ASSERT_TRUE(conditionedZone5.setSupplyPlenum(supplyPlenumZone));
    ASSERT_TRUE(conditionedZone5.setReturnPlenum(returnPlenumZone));
    ASSERT_EQ(23u,airLoop.demandComponents().size());
    ASSERT_EQ(1u,airLoop.demandComponents(AirLoopHVACSupplyPlenum::iddObjectType()).size());
    ASSERT_EQ(1u,airLoop.demandComponents(AirLoopHVACReturnPlenum::iddObjectType()).size());

    airLoop.demandComponents(AirLoopHVACReturnPlenum::iddObjectType()).front().remove();
    airLoop.demandComponents(AirLoopHVACSupplyPlenum::iddObjectType()).front().remove();

    ASSERT_TRUE(airLoop.demandComponents(AirLoopHVACSupplyPlenum::iddObjectType()).empty());
    ASSERT_TRUE(airLoop.demandComponents(AirLoopHVACReturnPlenum::iddObjectType()).empty());
    ASSERT_EQ(19u,airLoop.demandComponents().size());
  }
}

