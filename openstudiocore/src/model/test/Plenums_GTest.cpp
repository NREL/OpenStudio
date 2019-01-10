/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2019, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
*  following conditions are met:
*
*  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
*  disclaimer.
*
*  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
*  disclaimer in the documentation and/or other materials provided with the distribution.
*
*  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote products
*  derived from this software without specific prior written permission from the respective party.
*
*  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative works
*  may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without specific prior
*  written permission from Alliance for Sustainable Energy, LLC.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER(S) AND ANY CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
*  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
*  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER(S), ANY CONTRIBUTORS, THE UNITED STATES GOVERNMENT, OR THE UNITED
*  STATES DEPARTMENT OF ENERGY, NOR ANY OF THEIR EMPLOYEES, BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
*  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
*  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
*  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
*  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
***********************************************************************************************************************/

#include <gtest/gtest.h>
#include "ModelFixture.hpp"
#include "../AirLoopHVACReturnPlenum.hpp"
#include "../AirLoopHVACReturnPlenum_Impl.hpp"
#include "../AirLoopHVACSupplyPlenum.hpp"
#include "../AirLoopHVACSupplyPlenum_Impl.hpp"
#include "../AirLoopHVACZoneSplitter.hpp"
#include "../AirLoopHVACZoneSplitter_Impl.hpp"
#include "../ThermalZone.hpp"
#include "../ThermalZone_Impl.hpp"
#include "../Node.hpp"
#include "../Node_Impl.hpp"
#include "../AirLoopHVAC.hpp"
#include "../AirLoopHVAC_Impl.hpp"
#include "../AirTerminalSingleDuctVAVReheat.hpp"
#include "../AirTerminalSingleDuctVAVReheat_Impl.hpp"
#include "../AirTerminalDualDuctVAV.hpp"
#include "../AirTerminalDualDuctVAV_Impl.hpp"
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

  // Plenum on dual duct
  {
    Model m;

    AirLoopHVAC airLoop(m,true); // <-- true makes it a dual duct

    ThermalZone supplyPlenumZone(m);
    ThermalZone returnPlenumZone(m);

    ThermalZone conditionedZone1(m);
    ThermalZone conditionedZone2(m);
    ThermalZone conditionedZone3(m);

    AirTerminalDualDuctVAV terminal(m);
    airLoop.addBranchForZone(conditionedZone1,terminal);
    EXPECT_EQ(12u,airLoop.demandComponents().size());


    {
      auto t_zoneSplitters = airLoop.zoneSplitters();
      EXPECT_EQ(2u,t_zoneSplitters.size());
      EXPECT_EQ(airLoop.zoneSplitter(),t_zoneSplitters.front());
      EXPECT_EQ(2u,airLoop.demandInletNodes().size());
    }

    ASSERT_EXIT (
    {
      conditionedZone1.removeSupplyPlenum();
      conditionedZone1.removeSupplyPlenum(0);
      conditionedZone1.removeSupplyPlenum(1);
      exit(0);
    } ,
      ::testing::ExitedWithCode(0), "" );

    EXPECT_TRUE(conditionedZone1.setSupplyPlenum(supplyPlenumZone));
    EXPECT_EQ(14u,airLoop.demandComponents().size());

    auto zoneSplitters = airLoop.zoneSplitters();
    EXPECT_EQ(2u,zoneSplitters.size());

    {
      auto demandComps = airLoop.demandComponents(zoneSplitters[0],conditionedZone1);
      auto plenums = subsetCastVector<AirLoopHVACSupplyPlenum>(demandComps);
      EXPECT_EQ(1u,plenums.size());
    }

    conditionedZone1.removeSupplyPlenum();
    EXPECT_EQ(12u,airLoop.demandComponents().size());

    {
      auto demandComps = airLoop.demandComponents(zoneSplitters[0],conditionedZone1);
      auto plenums = subsetCastVector<AirLoopHVACSupplyPlenum>(demandComps);
      EXPECT_EQ(0u,plenums.size());
    }

  }
}

