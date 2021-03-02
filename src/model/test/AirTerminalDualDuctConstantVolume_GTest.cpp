/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2020, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

#include "../AirTerminalDualDuctConstantVolume.hpp"
#include "../AirTerminalDualDuctConstantVolume_Impl.hpp"

#include "../AirLoopHVAC.hpp"
#include "../AirLoopHVAC_Impl.hpp"
#include "../AirLoopHVACZoneSplitter.hpp"
#include "../AirLoopHVACZoneSplitter_Impl.hpp"
#include "../Node.hpp"
#include "../Node_Impl.hpp"
#include "../ThermalZone.hpp"
#include "../ThermalZone_Impl.hpp"
#include "../Schedule.hpp"
#include "../ScheduleCompact.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, AirTerminalDualDuctConstantVolume) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  ASSERT_EXIT(
    {
      Model m;
      AirTerminalDualDuctConstantVolume atu(m);

      exit(0);
    },
    ::testing::ExitedWithCode(0), "");
}

TEST_F(ModelFixture, AirTerminalDualDuctConstantVolume_DefaultCtor) {

  Model m;
  AirTerminalDualDuctConstantVolume atu(m);

  // Should be autosized, have alwaysOn avail sch, and not connected
  EXPECT_TRUE(atu.isMaximumAirFlowRateAutosized());
  EXPECT_FALSE(atu.hotAirInletNode());
  EXPECT_FALSE(atu.coldAirInletNode());
  EXPECT_EQ(m.alwaysOnDiscreteSchedule().handle(), atu.availabilitySchedule().handle());
}

TEST_F(ModelFixture, AirTerminalDualDuctConstantVolume_SettersGetters) {

  Model m;
  AirTerminalDualDuctConstantVolume atu(m);

  ScheduleCompact sch2(m);
  EXPECT_TRUE(atu.setAvailabilitySchedule(sch2));
  EXPECT_EQ(sch2.handle(), atu.availabilitySchedule().handle());

  EXPECT_TRUE(atu.setMaximumAirFlowRate(0.1));
  EXPECT_FALSE(atu.isMaximumAirFlowRateAutosized());
  ASSERT_TRUE(atu.maximumAirFlowRate());
  EXPECT_EQ(0.1, atu.maximumAirFlowRate().get());
  atu.autosizeMaximumAirFlowRate();
  EXPECT_TRUE(atu.isMaximumAirFlowRateAutosized());
}

TEST_F(ModelFixture, AirTerminalDualDuctConstantVolume_AddToAirLoopHVAC) {

  // Add terminal
  {
    Model m;
    AirTerminalDualDuctConstantVolume atu(m);

    AirLoopHVAC a(m);
    {
      auto t_zoneSplitters = a.zoneSplitters();
      EXPECT_EQ(1u, t_zoneSplitters.size());
      EXPECT_EQ(a.zoneSplitter(), t_zoneSplitters.front());
      EXPECT_EQ(1u, a.demandInletNodes().size());
    }

    EXPECT_TRUE(a.addBranchForHVACComponent(atu));
    EXPECT_EQ(2u, a.demandInletNodes().size());
    EXPECT_EQ(2u, a.zoneSplitters().size());

    EXPECT_EQ(10u, a.demandComponents().size());

    // Check that inletModelObject(0) is the hotAirInletNode
    ASSERT_TRUE(atu.hotAirInletNode());
    ASSERT_TRUE(atu.inletModelObject(0));
    EXPECT_EQ(atu.hotAirInletNode()->handle(), atu.inletModelObject(0)->handle());

    // Check that inletModelObject(1) is the coldAirInletNode
    ASSERT_TRUE(atu.coldAirInletNode());
    ASSERT_TRUE(atu.inletModelObject(1));
    EXPECT_EQ(atu.coldAirInletNode()->handle(), atu.inletModelObject(1)->handle());
  }

  {
    // Add terminal with zone
    Model m;
    AirTerminalDualDuctConstantVolume atu(m);

    AirLoopHVAC a(m);
    {
      auto t_zoneSplitters = a.zoneSplitters();
      EXPECT_EQ(1u, t_zoneSplitters.size());
      EXPECT_EQ(a.zoneSplitter(), t_zoneSplitters.front());
      EXPECT_EQ(1u, a.demandInletNodes().size());
    }

    ThermalZone zone(m);
    EXPECT_TRUE(a.addBranchForZone(zone, atu));
    {
      auto t_zoneSplitters = a.zoneSplitters();
      EXPECT_EQ(2u, t_zoneSplitters.size());
      EXPECT_EQ(a.zoneSplitter(), t_zoneSplitters.front());
      EXPECT_EQ(2u, a.demandInletNodes().size());
    }

    EXPECT_EQ(12u, a.demandComponents().size());

    // Remove the terminal
    atu.remove();
    EXPECT_EQ(7u, a.demandComponents().size());

    // Make sure we can't add the same zone again
    EXPECT_EQ(1u, zone.airLoopHVACs().size());
    EXPECT_FALSE(a.addBranchForZone(zone));

    // Remove the whole branch
    EXPECT_TRUE(a.removeBranchForZone(zone));
    EXPECT_EQ(5u, a.demandComponents().size());

    AirTerminalDualDuctConstantVolume atu2(m);
    EXPECT_TRUE(a.addBranchForZone(zone, atu2));
    EXPECT_EQ(12u, a.demandComponents().size());

    // Remove the whole branch again, this time there is a terminal to worry about
    EXPECT_TRUE(a.removeBranchForZone(zone));
    EXPECT_EQ(5u, a.demandComponents().size());

    // Do it again and then see that the terminal duplication is happening when we add additional branches
    AirTerminalDualDuctConstantVolume atu3(m);

    EXPECT_TRUE(a.addBranchForZone(zone, atu3));
    EXPECT_EQ(12u, a.demandComponents().size());

    ThermalZone zone2(m);
    EXPECT_TRUE(a.addBranchForZone(zone2));

    {
      auto terminals = subsetCastVector<AirTerminalDualDuctConstantVolume>(a.demandComponents());
      EXPECT_EQ(2u, terminals.size());
    }
  }
}
