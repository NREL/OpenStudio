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
#include "../HeatExchangerAirToAirSensibleAndLatent.hpp"
#include "../HeatExchangerAirToAirSensibleAndLatent_Impl.hpp"
#include "../AirLoopHVAC.hpp"
#include "../AirLoopHVAC_Impl.hpp"
#include "../Schedule.hpp"
#include "../Schedule_Impl.hpp"
#include "../Node.hpp"
#include "../Node_Impl.hpp"
#include "../AirLoopHVACOutdoorAirSystem.hpp"
#include "../AirLoopHVACOutdoorAirSystem_Impl.hpp"
#include "../EvaporativeCoolerDirectResearchSpecial.hpp"
#include "../EvaporativeCoolerDirectResearchSpecial_Impl.hpp"
#include "../HVACTemplates.hpp"
#include "../ScheduleCompact.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, HeatExchangerAirToAirSensibleAndLatent_Ctor) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  ASSERT_EXIT(
    {
      Model m;
      HeatExchangerAirToAirSensibleAndLatent hx(m);

      exit(0);
    },
    ::testing::ExitedWithCode(0), "");
}

TEST_F(ModelFixture, HeatExchangerAirToAirSensibleAndLatent_GettersSetters) {
  Model m;
  HeatExchangerAirToAirSensibleAndLatent hx(m);

  // Availability Schedule:  Object
  // Default in Ctor is m.alwaysOnDiscreteSchedule
  EXPECT_EQ(m.alwaysOnDiscreteSchedule(), hx.availabilitySchedule());
  ScheduleCompact sch(m);
  EXPECT_TRUE(hx.setAvailabilitySchedule(sch));
  EXPECT_EQ(sch, hx.availabilitySchedule());

  // Nominal Supply Air Flow Rate: Optional Double
  // No Default
  EXPECT_TRUE(hx.setNominalSupplyAirFlowRate(1.0));
  ASSERT_TRUE(hx.nominalSupplyAirFlowRate());
  EXPECT_EQ(1.0, hx.nominalSupplyAirFlowRate().get());

  // Sensible Effectiveness at 100% Heating Air Flow:  Double
  // No Default
  EXPECT_TRUE(hx.setSensibleEffectivenessat100HeatingAirFlow(0.5));
  EXPECT_EQ(0.5, hx.sensibleEffectivenessat100HeatingAirFlow());

  // Latent Effectiveness at 100% Heating Air Flow:  Double
  // No Default
  EXPECT_TRUE(hx.setLatentEffectivenessat100HeatingAirFlow(0.5));
  EXPECT_EQ(0.5, hx.latentEffectivenessat100HeatingAirFlow());

  // Sensible Effectiveness at 75% Heating Air Flow:  Double
  // No Default
  EXPECT_TRUE(hx.setSensibleEffectivenessat75HeatingAirFlow(0.5));
  EXPECT_EQ(0.5, hx.sensibleEffectivenessat75HeatingAirFlow());

  // Latent Effectiveness at 75% Heating Air Flow:  Double
  // No Default
  EXPECT_TRUE(hx.setLatentEffectivenessat75HeatingAirFlow(0.5));
  EXPECT_EQ(0.5, hx.latentEffectivenessat75HeatingAirFlow());

  // Sensible Effectiveness at 100% Cooling Air Flow:  Double
  // No Default
  EXPECT_TRUE(hx.setSensibleEffectivenessat100CoolingAirFlow(0.5));
  EXPECT_EQ(0.5, hx.sensibleEffectivenessat100CoolingAirFlow());

  // Latent Effectiveness at 100% Cooling Air Flow:  Double
  // No Default
  EXPECT_TRUE(hx.setLatentEffectivenessat100CoolingAirFlow(0.5));
  EXPECT_EQ(0.5, hx.latentEffectivenessat100CoolingAirFlow());

  // Sensible Effectiveness at 75% Cooling Air Flow:  Double
  // No Default
  EXPECT_TRUE(hx.setSensibleEffectivenessat75CoolingAirFlow(0.5));
  EXPECT_EQ(0.5, hx.sensibleEffectivenessat75CoolingAirFlow());

  // Latent Effectiveness at 75% Cooling Air Flow:  Double
  // No Default
  EXPECT_TRUE(hx.setLatentEffectivenessat75CoolingAirFlow(0.5));
  EXPECT_EQ(0.5, hx.latentEffectivenessat75CoolingAirFlow());

  // Supply Air Inlet Node: Optional Object

  // Supply Air Outlet Node: Optional Object

  // Exhaust Air Inlet Node: Optional Object

  // Exhaust Air Outlet Node: Optional Object

  // Nominal Electric Power:  Double
  // No Default
  EXPECT_TRUE(hx.setNominalElectricPower(1.0));
  EXPECT_EQ(1.0, hx.nominalElectricPower());

  // Supply Air Outlet Temperature Control:  Boolean
  // No Default
  EXPECT_TRUE(hx.setSupplyAirOutletTemperatureControl(true));
  EXPECT_EQ(true, hx.supplyAirOutletTemperatureControl());
  EXPECT_TRUE(hx.setSupplyAirOutletTemperatureControl(false));
  EXPECT_EQ(false, hx.supplyAirOutletTemperatureControl());

  // Heat Exchanger Type:  String
  // No Default
  // Test a valid choice
  EXPECT_TRUE(hx.setHeatExchangerType("Plate"));
  EXPECT_EQ("Plate", hx.heatExchangerType());
  // Test an invalid choice
  EXPECT_FALSE(hx.setHeatExchangerType("BadChoice"));
  EXPECT_EQ("Plate", hx.heatExchangerType());

  // Frost Control Type:  String
  // No Default
  // Test a valid choice
  EXPECT_TRUE(hx.setFrostControlType("None"));
  EXPECT_EQ("None", hx.frostControlType());
  // Test an invalid choice
  EXPECT_FALSE(hx.setFrostControlType("BadChoice"));
  EXPECT_EQ("None", hx.frostControlType());

  // Threshold Temperature:  Double
  // Check Idd default: 1.7
  EXPECT_EQ(1.7, hx.thresholdTemperature());
  EXPECT_TRUE(hx.setThresholdTemperature(0.7));
  EXPECT_EQ(0.7, hx.thresholdTemperature());
  hx.resetThresholdTemperature();
  EXPECT_EQ(1.7, hx.thresholdTemperature());

  // Initial Defrost Time Fraction: Optional Double
  // No Default
  EXPECT_TRUE(hx.setInitialDefrostTimeFraction(0.5));
  ASSERT_TRUE(hx.initialDefrostTimeFraction());
  EXPECT_EQ(0.5, hx.initialDefrostTimeFraction().get());
  hx.resetInitialDefrostTimeFraction();
  EXPECT_FALSE(hx.initialDefrostTimeFraction());

  // Rate of Defrost Time Fraction Increase: Optional Double
  // No Default
  EXPECT_TRUE(hx.setRateofDefrostTimeFractionIncrease(1.0));
  ASSERT_TRUE(hx.rateofDefrostTimeFractionIncrease());
  EXPECT_EQ(1.0, hx.rateofDefrostTimeFractionIncrease().get());
  hx.resetRateofDefrostTimeFractionIncrease();
  EXPECT_FALSE(hx.rateofDefrostTimeFractionIncrease());

  // Economizer Lockout:  Boolean
  // No Default
  EXPECT_TRUE(hx.setEconomizerLockout(true));
  EXPECT_EQ(true, hx.economizerLockout());
  EXPECT_TRUE(hx.setEconomizerLockout(false));
  EXPECT_EQ(false, hx.economizerLockout());
}

TEST_F(ModelFixture, HeatExchangerAirToAirSensibleAndLatent_addToNode) {
  Model model;
  HeatExchangerAirToAirSensibleAndLatent heatExchangerAirToAirSensibleAndLatent(model);

  AirLoopHVAC loop = addSystemType3(model).cast<AirLoopHVAC>();

  AirLoopHVACOutdoorAirSystem oaSystem = loop.airLoopHVACOutdoorAirSystem().get();

  Node oaNode = oaSystem.outboardOANode().get();
  Node reliefNode = oaSystem.outboardReliefNode().get();

  HeatExchangerAirToAirSensibleAndLatent hx1(model);
  EXPECT_TRUE(hx1.addToNode(oaNode));

  HeatExchangerAirToAirSensibleAndLatent hx2(model);
  EXPECT_TRUE(hx2.addToNode(oaNode));

  HeatExchangerAirToAirSensibleAndLatent hx3(model);
  EXPECT_TRUE(hx3.addToNode(reliefNode));

  oaNode = oaSystem.outdoorAirModelObject()->cast<Node>();
  reliefNode = oaSystem.reliefAirModelObject()->cast<Node>();

  HeatExchangerAirToAirSensibleAndLatent hx4(model);
  EXPECT_TRUE(hx4.addToNode(reliefNode));

  HeatExchangerAirToAirSensibleAndLatent hx5(model);
  EXPECT_TRUE(hx5.addToNode(oaNode));

  Schedule schedule = model.alwaysOnDiscreteSchedule();
  EvaporativeCoolerDirectResearchSpecial evap(model, schedule);
  EXPECT_TRUE(evap.addToNode(oaNode));
  Node inletNode = evap.inletModelObject()->cast<Node>();
  Node outletNode = evap.outletModelObject()->cast<Node>();

  HeatExchangerAirToAirSensibleAndLatent hx6(model);
  EXPECT_TRUE(hx6.addToNode(inletNode));

  HeatExchangerAirToAirSensibleAndLatent hx7(model);
  EXPECT_TRUE(hx7.addToNode(outletNode));

  hx1.remove();
  hx2.remove();
  hx3.remove();
  hx4.remove();
  hx5.remove();
  hx6.remove();
  hx7.remove();
  evap.remove();

  EXPECT_EQ(1u, oaSystem.oaComponents().size());
  EXPECT_EQ(1u, oaSystem.reliefComponents().size());
}

TEST_F(ModelFixture, HeatExchangerAirToAirSensibleAndLatent_remove) {
  Model model;
  HeatExchangerAirToAirSensibleAndLatent heatExchangerAirToAirSensibleAndLatent(model);

  AirLoopHVAC loop = addSystemType3(model).cast<AirLoopHVAC>();

  AirLoopHVACOutdoorAirSystem oaSystem = loop.airLoopHVACOutdoorAirSystem().get();

  Node oaNode = oaSystem.outboardOANode().get();
  Node reliefNode = oaSystem.outboardReliefNode().get();

  HeatExchangerAirToAirSensibleAndLatent hx1(model);
  EXPECT_TRUE(hx1.addToNode(oaNode));

  HeatExchangerAirToAirSensibleAndLatent hx2(model);
  EXPECT_TRUE(hx2.addToNode(oaNode));

  HeatExchangerAirToAirSensibleAndLatent hx3(model);
  EXPECT_TRUE(hx3.addToNode(reliefNode));

  oaNode = oaSystem.outdoorAirModelObject()->cast<Node>();
  reliefNode = oaSystem.reliefAirModelObject()->cast<Node>();

  HeatExchangerAirToAirSensibleAndLatent hx4(model);
  EXPECT_TRUE(hx4.addToNode(reliefNode));

  HeatExchangerAirToAirSensibleAndLatent hx5(model);
  EXPECT_TRUE(hx5.addToNode(oaNode));

  Schedule schedule = model.alwaysOnDiscreteSchedule();
  EvaporativeCoolerDirectResearchSpecial evap(model, schedule);
  EXPECT_TRUE(evap.addToNode(oaNode));
  Node inletNode = evap.inletModelObject()->cast<Node>();
  Node outletNode = evap.outletModelObject()->cast<Node>();

  HeatExchangerAirToAirSensibleAndLatent hx6(model);
  EXPECT_TRUE(hx6.addToNode(inletNode));

  HeatExchangerAirToAirSensibleAndLatent hx7(model);
  EXPECT_TRUE(hx7.addToNode(outletNode));

  EXPECT_TRUE(loop.remove().size() > 0);
}
