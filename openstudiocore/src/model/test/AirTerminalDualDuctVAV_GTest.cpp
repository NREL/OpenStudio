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
#include "../AirTerminalDualDuctVAV.hpp"
#include "../AirTerminalDualDuctVAV_Impl.hpp"
#include "../AirLoopHVAC.hpp"
#include "../AirLoopHVAC_Impl.hpp"
#include "../AirLoopHVACZoneSplitter.hpp"
#include "../AirLoopHVACZoneSplitter_Impl.hpp"
#include "../Node.hpp"
#include "../Node_Impl.hpp"
#include "../ThermalZone.hpp"
#include "../ThermalZone_Impl.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture,AirTerminalDualDuctVAV) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  ASSERT_EXIT (
  {
     Model m;
     AirTerminalDualDuctVAV terminal(m);

     exit(0);
  } ,
    ::testing::ExitedWithCode(0), "" );

  // Add terminal
  {
    Model m;
    AirTerminalDualDuctVAV terminal(m);

    AirLoopHVAC airLoopHVAC(m);
    {
      auto t_zoneSplitters = airLoopHVAC.zoneSplitters();
      EXPECT_EQ(1u,t_zoneSplitters.size());
      EXPECT_EQ(airLoopHVAC.zoneSplitter(),t_zoneSplitters.front());
      EXPECT_EQ(1u,airLoopHVAC.demandInletNodes().size());
    }

    EXPECT_TRUE(airLoopHVAC.addBranchForHVACComponent(terminal));
    EXPECT_EQ(2u,airLoopHVAC.demandInletNodes().size());
    EXPECT_EQ(2u,airLoopHVAC.zoneSplitters().size());

    EXPECT_EQ(10u,airLoopHVAC.demandComponents().size());
  }

  {
    // Add terminal with zone
    Model m;
    AirTerminalDualDuctVAV terminal(m);

    AirLoopHVAC airLoopHVAC(m);
    {
      auto t_zoneSplitters = airLoopHVAC.zoneSplitters();
      EXPECT_EQ(1u,t_zoneSplitters.size());
      EXPECT_EQ(airLoopHVAC.zoneSplitter(),t_zoneSplitters.front());
      EXPECT_EQ(1u,airLoopHVAC.demandInletNodes().size());
    }

    ThermalZone zone(m);
    EXPECT_TRUE(airLoopHVAC.addBranchForZone(zone,terminal));
    {
      auto t_zoneSplitters = airLoopHVAC.zoneSplitters();
      EXPECT_EQ(2u,t_zoneSplitters.size());
      EXPECT_EQ(airLoopHVAC.zoneSplitter(),t_zoneSplitters.front());
      EXPECT_EQ(2u,airLoopHVAC.demandInletNodes().size());
    }

    EXPECT_EQ(12u,airLoopHVAC.demandComponents().size());

    // Remove the terminal
    terminal.remove();
    EXPECT_EQ(7u,airLoopHVAC.demandComponents().size());

    // Make sure we cant add the same zone again
    EXPECT_FALSE(airLoopHVAC.addBranchForZone(zone));

    // Remove the whole branch
    EXPECT_TRUE(airLoopHVAC.removeBranchForZone(zone));
    EXPECT_EQ(5u,airLoopHVAC.demandComponents().size());

    AirTerminalDualDuctVAV terminal2(m);
    EXPECT_TRUE(airLoopHVAC.addBranchForZone(zone,terminal2));
    EXPECT_EQ(12u,airLoopHVAC.demandComponents().size());

    // Remove the whole branch again, this time there is a terminal to worry about
    EXPECT_TRUE(airLoopHVAC.removeBranchForZone(zone));
    EXPECT_EQ(5u,airLoopHVAC.demandComponents().size());

    // Do it again and then see that the terminal duplication is happening when we add additional branches
    AirTerminalDualDuctVAV terminal3(m);

    EXPECT_TRUE(airLoopHVAC.addBranchForZone(zone,terminal3));
    EXPECT_EQ(12u,airLoopHVAC.demandComponents().size());

    ThermalZone zone2(m);
    EXPECT_TRUE(airLoopHVAC.addBranchForZone(zone2));

    {
      auto terminals = subsetCastVector<AirTerminalDualDuctVAV>(airLoopHVAC.demandComponents());
      EXPECT_EQ(2u,terminals.size());
    }
  }

}
