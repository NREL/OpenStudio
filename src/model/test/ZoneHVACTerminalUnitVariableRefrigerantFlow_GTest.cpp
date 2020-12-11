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
#include "../Model.hpp"

#include "../Node.hpp"

#include "../FanOnOff.hpp"
#include "../HVACComponent.hpp"
#include "../HVACComponent_Impl.hpp"

#include "../ZoneHVACTerminalUnitVariableRefrigerantFlow.hpp"
#include "../CoilCoolingDXVariableRefrigerantFlow.hpp"
#include "../CoilHeatingDXVariableRefrigerantFlow.hpp"

#include "../CoilHeatingElectric.hpp"

#include "../ScheduleConstant.hpp"
#include "../Schedule.hpp"
#include "../ThermalZone.hpp"

#include "../../utilities/core/Compare.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, ZoneHVACTerminalUnitVariableRefrigerantFlow_Default_Ctor) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  ASSERT_EXIT(
    {
      Model model;

      ZoneHVACTerminalUnitVariableRefrigerantFlow vrfTerminal(model);

      exit(0);
    },
    ::testing::ExitedWithCode(0), "");
}

TEST_F(ModelFixture, ZoneHVACTerminalUnitVariableRefrigerantFlow_Explicit_Ctor) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  ASSERT_EXIT(
    {
      Model model;

      FanOnOff fan(model);
      CoilCoolingDXVariableRefrigerantFlow cc(model);
      CoilHeatingDXVariableRefrigerantFlow hc(model);

      ZoneHVACTerminalUnitVariableRefrigerantFlow vrfTerminal(model, cc, hc, fan);

      exit(0);
    },
    ::testing::ExitedWithCode(0), "");
}

TEST_F(ModelFixture, ZoneHVACTerminalUnitVariableRefrigerantFlow_SupplementalHeating) {

  Model m;
  ZoneHVACTerminalUnitVariableRefrigerantFlow vrf(m);

  // test defaults
  EXPECT_FALSE(vrf.supplementalHeatingCoil());
  EXPECT_TRUE(vrf.isMaximumSupplyAirTemperaturefromSupplementalHeaterAutosized());
  EXPECT_FALSE(vrf.maximumSupplyAirTemperaturefromSupplementalHeater());
  EXPECT_EQ(21.0, vrf.maximumOutdoorDryBulbTemperatureforSupplementalHeaterOperation());

  // Supplemental Coil
  CoilHeatingElectric supplemental_hc(m);
  EXPECT_TRUE(vrf.setSupplementalHeatingCoil(supplemental_hc));
  ASSERT_TRUE(vrf.supplementalHeatingCoil());
  EXPECT_EQ(supplemental_hc, vrf.supplementalHeatingCoil().get());

  CoilHeatingDXVariableRefrigerantFlow hc_wrong_type(m);
  EXPECT_FALSE(vrf.setSupplementalHeatingCoil(hc_wrong_type));
  ASSERT_TRUE(vrf.supplementalHeatingCoil());
  EXPECT_EQ(supplemental_hc, vrf.supplementalHeatingCoil().get());

  vrf.resetSupplementalHeatingCoil();
  EXPECT_FALSE(vrf.supplementalHeatingCoil());

  // Max SAT
  vrf.setMaximumSupplyAirTemperaturefromSupplementalHeater(35.0);
  EXPECT_FALSE(vrf.isMaximumSupplyAirTemperaturefromSupplementalHeaterAutosized());
  ASSERT_TRUE(vrf.maximumSupplyAirTemperaturefromSupplementalHeater());
  EXPECT_EQ(35.0, vrf.maximumSupplyAirTemperaturefromSupplementalHeater().get());

  vrf.autosizeMaximumSupplyAirTemperaturefromSupplementalHeater();
  EXPECT_TRUE(vrf.isMaximumSupplyAirTemperaturefromSupplementalHeaterAutosized());
  EXPECT_FALSE(vrf.maximumSupplyAirTemperaturefromSupplementalHeater());

  // Max OATdb for supplemental heater
  EXPECT_TRUE(vrf.setMaximumOutdoorDryBulbTemperatureforSupplementalHeaterOperation(19.0));
  EXPECT_EQ(19.0, vrf.maximumOutdoorDryBulbTemperatureforSupplementalHeaterOperation());
  EXPECT_FALSE(vrf.setMaximumOutdoorDryBulbTemperatureforSupplementalHeaterOperation(45.0));  // > max
  EXPECT_EQ(19.0, vrf.maximumOutdoorDryBulbTemperatureforSupplementalHeaterOperation());
}
