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
#include "../AirConditionerVariableRefrigerantFlow.hpp"
#include "../AirConditionerVariableRefrigerantFlow_Impl.hpp"
#include "../ThermalZone.hpp"
#include "../ZoneHVACTerminalUnitVariableRefrigerantFlow.hpp"
#include "../ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl.hpp"
#include "../AirLoopHVAC.hpp"
#include "../PlantLoop.hpp"
#include "../Node.hpp"
#include "../Node_Impl.hpp"
#include "../Curve.hpp"
#include "../Curve_Impl.hpp"
#include "../AirLoopHVACZoneSplitter.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, AirConditionerVariableRefrigerantFlow) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  ASSERT_EXIT(
    {
      Model m;
      AirConditionerVariableRefrigerantFlow vrf(m);

      exit(0);
    },
    ::testing::ExitedWithCode(0), "");

  Model m;
  AirConditionerVariableRefrigerantFlow vrf(m);

  for (int i = 0; i != 5; i++) {
    ThermalZone zone(m);

    ZoneHVACTerminalUnitVariableRefrigerantFlow vrfTerminal(m);

    ASSERT_TRUE(vrfTerminal.addToThermalZone(zone));

    vrf.addTerminal(vrfTerminal);
  }

  ASSERT_EQ(5u, vrf.terminals().size());

  Model m2;

  boost::optional<AirConditionerVariableRefrigerantFlow> vrfClone = vrf.clone(m2).optionalCast<AirConditionerVariableRefrigerantFlow>();
  ASSERT_TRUE(vrfClone);

  ASSERT_TRUE(vrfClone->coolingCapacityRatioModifierFunctionofLowTemperatureCurve());
  ASSERT_TRUE(vrfClone->coolingCapacityRatioBoundaryCurve());
  ASSERT_TRUE(vrfClone->coolingCapacityRatioModifierFunctionofHighTemperatureCurve());
  ASSERT_TRUE(vrfClone->coolingEnergyInputRatioModifierFunctionofLowTemperatureCurve());
  ASSERT_TRUE(vrfClone->coolingEnergyInputRatioBoundaryCurve());
  ASSERT_TRUE(vrfClone->coolingEnergyInputRatioModifierFunctionofHighTemperatureCurve());
  ASSERT_TRUE(vrfClone->coolingEnergyInputRatioModifierFunctionofLowPartLoadRatioCurve());
  ASSERT_TRUE(vrfClone->coolingEnergyInputRatioModifierFunctionofHighPartLoadRatioCurve());
  ASSERT_TRUE(vrfClone->coolingCombinationRatioCorrectionFactorCurve());
  ASSERT_TRUE(vrfClone->coolingPartLoadFractionCorrelationCurve());
  ASSERT_TRUE(vrfClone->heatingCapacityRatioModifierFunctionofLowTemperatureCurve());
  ASSERT_TRUE(vrfClone->heatingCapacityRatioBoundaryCurve());
  ASSERT_TRUE(vrfClone->heatingCapacityRatioModifierFunctionofHighTemperatureCurve());
  ASSERT_TRUE(vrfClone->heatingEnergyInputRatioModifierFunctionofLowTemperatureCurve());
  ASSERT_TRUE(vrfClone->heatingEnergyInputRatioBoundaryCurve());
  ASSERT_TRUE(vrfClone->heatingEnergyInputRatioModifierFunctionofHighTemperatureCurve());
  ASSERT_TRUE(vrfClone->heatingEnergyInputRatioModifierFunctionofLowPartLoadRatioCurve());
  ASSERT_TRUE(vrfClone->heatingEnergyInputRatioModifierFunctionofHighPartLoadRatioCurve());
  ASSERT_TRUE(vrfClone->heatingCombinationRatioCorrectionFactorCurve());
  ASSERT_TRUE(vrfClone->heatingPartLoadFractionCorrelationCurve());
  ASSERT_TRUE(vrfClone->pipingCorrectionFactorforLengthinCoolingModeCurve());

  ASSERT_TRUE(vrfClone->terminals().empty());

  ASSERT_TRUE(!vrfClone->remove().empty());
}

TEST_F(ModelFixture, AirConditionerVariableRefrigerantFlow_addToNode) {
  Model m;
  AirConditionerVariableRefrigerantFlow vrf(m);

  // By default
  EXPECT_TRUE(vrf.isCondenserTypeDefaulted());
  EXPECT_EQ("AirCooled", vrf.condenserType());

  // Not connected to a PlantLoop, we still allow everything.
  EXPECT_TRUE(vrf.setCondenserType("EvaporativelyCooled"));
  EXPECT_FALSE(vrf.isCondenserTypeDefaulted());
  EXPECT_EQ("EvaporativelyCooled", vrf.condenserType());

  vrf.resetCondenserType();
  EXPECT_TRUE(vrf.setCondenserType("AirCooled"));
  EXPECT_FALSE(vrf.isCondenserTypeDefaulted());
  EXPECT_EQ("AirCooled", vrf.condenserType());

  vrf.resetCondenserType();
  EXPECT_TRUE(vrf.setCondenserType("WaterCooled"));
  EXPECT_FALSE(vrf.isCondenserTypeDefaulted());
  EXPECT_EQ("WaterCooled", vrf.condenserType());

  // Disallow connection to an AirLoopHVAC
  vrf.resetCondenserType();
  AirLoopHVAC airLoop(m);
  Node supplyOutletNode = airLoop.supplyOutletNode();
  EXPECT_FALSE(vrf.addToNode(supplyOutletNode));
  EXPECT_EQ((unsigned)2, airLoop.supplyComponents().size());
  EXPECT_TRUE(vrf.isCondenserTypeDefaulted());
  EXPECT_EQ("AirCooled", vrf.condenserType());

  Node inletNode = airLoop.zoneSplitter().lastOutletModelObject()->cast<Node>();
  EXPECT_FALSE(vrf.addToNode(inletNode));
  EXPECT_EQ((unsigned)5, airLoop.demandComponents().size());
  EXPECT_TRUE(vrf.isCondenserTypeDefaulted());
  EXPECT_EQ("AirCooled", vrf.condenserType());

  // Allow Connection to a PlantLoop, but only on Demand Side
  vrf.resetCondenserType();
  PlantLoop plantLoop(m);
  supplyOutletNode = plantLoop.supplyOutletNode();
  EXPECT_FALSE(vrf.addToNode(supplyOutletNode));
  EXPECT_EQ((unsigned)5, plantLoop.supplyComponents().size());
  EXPECT_TRUE(vrf.isCondenserTypeDefaulted());
  EXPECT_EQ("AirCooled", vrf.condenserType());

  // Demand side should be allowed
  EXPECT_TRUE(plantLoop.addDemandBranchForComponent(vrf));
  EXPECT_EQ((unsigned)7, plantLoop.demandComponents().size());
  // This should have switched the **default** Condenser Type to 'WaterCooled'
  EXPECT_TRUE(vrf.isCondenserTypeDefaulted());
  EXPECT_EQ("WaterCooled", vrf.condenserType());

  // Should still allow Evap or AirCooled (even though it's not correct...handled in FT)
  EXPECT_TRUE(vrf.setCondenserType("AirCooled"));
  EXPECT_FALSE(vrf.isCondenserTypeDefaulted());
  EXPECT_EQ("AirCooled", vrf.condenserType());

  EXPECT_TRUE(vrf.setCondenserType("EvaporativelyCooled"));
  EXPECT_FALSE(vrf.isCondenserTypeDefaulted());
  EXPECT_EQ("EvaporativelyCooled", vrf.condenserType());

  // Disconnect it, shouldn't affect anything
  // Note: do not use removeDemandBranchWithComponent here, it would delete the VRF itself
  vrf.disconnect();
  EXPECT_FALSE(vrf.plantLoop());
  EXPECT_EQ((unsigned)5, plantLoop.supplyComponents().size());
  EXPECT_FALSE(vrf.isCondenserTypeDefaulted());
  EXPECT_EQ("EvaporativelyCooled", vrf.condenserType());

  // Resetting should re-default to AirCooled
  vrf.resetCondenserType();
  EXPECT_TRUE(vrf.isCondenserTypeDefaulted());
  EXPECT_EQ("AirCooled", vrf.condenserType());
}
