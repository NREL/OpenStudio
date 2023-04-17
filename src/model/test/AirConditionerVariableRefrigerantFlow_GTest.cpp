/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2023, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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
#include "../AirLoopHVACZoneSplitter.hpp"
#include "../Curve.hpp"
#include "../Curve_Impl.hpp"
#include "../CurveBiquadratic.hpp"
#include "../CurveBiquadratic_Impl.hpp"
#include "../CurveCubic.hpp"
#include "../CurveCubic_Impl.hpp"
#include "../CurveQuadratic.hpp"
#include "../CurveQuadratic_Impl.hpp"
#include "../CurveExponent.hpp"
#include "../CurveExponent_Impl.hpp"
#include "../ModelObjectList.hpp"
#include "../ModelObjectList_Impl.hpp"
#include "../FanOnOff.hpp"
#include "../FanSystemModel.hpp"
#include "../CoilCoolingDXVariableRefrigerantFlowFluidTemperatureControl.hpp"
#include "../CoilHeatingDXVariableRefrigerantFlowFluidTemperatureControl.hpp"
#include "../CoilCoolingDXVariableRefrigerantFlow.hpp"
#include "../CoilHeatingDXVariableRefrigerantFlow.hpp"

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
    EXPECT_FALSE(vrfTerminal.vrfSystem());

    ASSERT_TRUE(vrfTerminal.addToThermalZone(zone));

    vrf.addTerminal(vrfTerminal);
    ASSERT_TRUE(vrfTerminal.vrfSystem());
    EXPECT_EQ(vrf, vrfTerminal.vrfSystem().get());
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

TEST_F(ModelFixture, AirConditionerVariableRefrigerantFlow_Clone) {
  Model model;

  EXPECT_EQ(0, model.getModelObjects<Curve>().size());
  EXPECT_EQ(0, model.getConcreteModelObjects<CurveQuadratic>().size());
  EXPECT_EQ(0, model.getConcreteModelObjects<CurveBiquadratic>().size());
  EXPECT_EQ(0, model.getConcreteModelObjects<CurveCubic>().size());
  EXPECT_EQ(0, model.getConcreteModelObjects<CurveExponent>().size());

  EXPECT_EQ(0, model.getConcreteModelObjects<AirConditionerVariableRefrigerantFlow>().size());
  EXPECT_EQ(0, model.getConcreteModelObjects<ZoneHVACTerminalUnitVariableRefrigerantFlow>().size());
  EXPECT_EQ(0, model.getConcreteModelObjects<ModelObjectList>().size());

  AirConditionerVariableRefrigerantFlow vrf(model);
  {

    EXPECT_EQ(20, model.getModelObjects<Curve>().size());
    EXPECT_EQ(0, model.getConcreteModelObjects<CurveQuadratic>().size());
    EXPECT_EQ(9, model.getConcreteModelObjects<CurveBiquadratic>().size());
    EXPECT_EQ(11, model.getConcreteModelObjects<CurveCubic>().size());
    EXPECT_EQ(0, model.getConcreteModelObjects<CurveExponent>().size());

    EXPECT_EQ(1, model.getConcreteModelObjects<AirConditionerVariableRefrigerantFlow>().size());
    EXPECT_EQ(1, model.getConcreteModelObjects<ModelObjectList>().size());
    EXPECT_EQ(0, model.getConcreteModelObjects<ZoneHVACTerminalUnitVariableRefrigerantFlow>().size());

    // VRFTU has no curves, but its Cooling Coil and Heating Coil each add a Quadratic and a Biquadratic, the FanOnOff add a Cubic and one Exponent
    ZoneHVACTerminalUnitVariableRefrigerantFlow term1(model);
    vrf.addTerminal(term1);
    EXPECT_EQ(26, model.getModelObjects<Curve>().size());
    EXPECT_EQ(2, model.getConcreteModelObjects<CurveQuadratic>().size());
    EXPECT_EQ(11, model.getConcreteModelObjects<CurveBiquadratic>().size());
    EXPECT_EQ(12, model.getConcreteModelObjects<CurveCubic>().size());
    EXPECT_EQ(1, model.getConcreteModelObjects<CurveExponent>().size());

    EXPECT_EQ(1, model.getConcreteModelObjects<AirConditionerVariableRefrigerantFlow>().size());
    EXPECT_EQ(1, model.getConcreteModelObjects<ModelObjectList>().size());
    EXPECT_EQ(1, model.getConcreteModelObjects<ZoneHVACTerminalUnitVariableRefrigerantFlow>().size());

    ZoneHVACTerminalUnitVariableRefrigerantFlow term2(model);
    vrf.addTerminal(term2);
    EXPECT_EQ(32, model.getModelObjects<Curve>().size());
    EXPECT_EQ(4, model.getConcreteModelObjects<CurveQuadratic>().size());
    EXPECT_EQ(13, model.getConcreteModelObjects<CurveBiquadratic>().size());
    EXPECT_EQ(13, model.getConcreteModelObjects<CurveCubic>().size());
    EXPECT_EQ(2, model.getConcreteModelObjects<CurveExponent>().size());

    EXPECT_EQ(1, model.getConcreteModelObjects<AirConditionerVariableRefrigerantFlow>().size());
    EXPECT_EQ(1, model.getConcreteModelObjects<ModelObjectList>().size());  // 1 terminals
    EXPECT_EQ(2, model.getConcreteModelObjects<ZoneHVACTerminalUnitVariableRefrigerantFlow>().size());

    EXPECT_EQ(2, vrf.terminals().size());
  }

  // Cloning the VRF will clone the Loading Indexes, but not the underlying Curves (these are ResourceObjects after all!)
  // The Terminals aren't cloned
  auto vrfClone = vrf.clone(model).cast<AirConditionerVariableRefrigerantFlow>();
  {
    EXPECT_EQ(0, vrfClone.terminals().size());

    EXPECT_EQ(32, model.getModelObjects<Curve>().size());  // TODO: this is amazing, but this throws not only with 32 + 20 = 51, but 52! the
                                                           // VRFCPLFFPLR is cloned **TWICE**
    EXPECT_EQ(4, model.getConcreteModelObjects<CurveQuadratic>().size());
    EXPECT_EQ(13, model.getConcreteModelObjects<CurveBiquadratic>().size());
    EXPECT_EQ(13, model.getConcreteModelObjects<CurveCubic>().size());
    EXPECT_EQ(2, model.getConcreteModelObjects<CurveExponent>().size());

    EXPECT_EQ(2, model.getConcreteModelObjects<AirConditionerVariableRefrigerantFlow>().size());
    EXPECT_EQ(2, model.getConcreteModelObjects<ModelObjectList>().size());  // 1 terminals times 2
    EXPECT_EQ(2, model.getConcreteModelObjects<ZoneHVACTerminalUnitVariableRefrigerantFlow>().size());
  }

  {
    Model model2;

    EXPECT_EQ(0, model2.getModelObjects<Curve>().size());
    EXPECT_EQ(0, model2.getConcreteModelObjects<CurveQuadratic>().size());
    EXPECT_EQ(0, model2.getConcreteModelObjects<CurveBiquadratic>().size());
    EXPECT_EQ(0, model2.getConcreteModelObjects<CurveCubic>().size());
    EXPECT_EQ(0, model2.getConcreteModelObjects<CurveExponent>().size());

    EXPECT_EQ(0, model2.getConcreteModelObjects<AirConditionerVariableRefrigerantFlow>().size());
    EXPECT_EQ(0, model2.getConcreteModelObjects<ModelObjectList>().size());
    EXPECT_EQ(0, model2.getConcreteModelObjects<ZoneHVACTerminalUnitVariableRefrigerantFlow>().size());

    auto vrfClone2 = vrf.clone(model2).cast<AirConditionerVariableRefrigerantFlow>();
    EXPECT_EQ(0, vrfClone2.terminals().size());

    EXPECT_EQ(20, model2.getModelObjects<Curve>().size());
    EXPECT_EQ(0, model2.getConcreteModelObjects<CurveQuadratic>().size());
    EXPECT_EQ(9, model2.getConcreteModelObjects<CurveBiquadratic>().size());
    EXPECT_EQ(11, model2.getConcreteModelObjects<CurveCubic>().size());
    EXPECT_EQ(0, model2.getConcreteModelObjects<CurveExponent>().size());

    EXPECT_EQ(1, model2.getConcreteModelObjects<AirConditionerVariableRefrigerantFlow>().size());
    EXPECT_EQ(1, model2.getConcreteModelObjects<ModelObjectList>().size());
    EXPECT_EQ(0, model2.getConcreteModelObjects<ZoneHVACTerminalUnitVariableRefrigerantFlow>().size());
  }

  {
    // VRF + its terminals are gone, but the cloned one should be left intact (its curves and co are still there)
    vrf.remove();
    EXPECT_EQ(20, model.getModelObjects<Curve>().size());
    EXPECT_EQ(0, model.getConcreteModelObjects<CurveQuadratic>().size());
    EXPECT_EQ(9, model.getConcreteModelObjects<CurveBiquadratic>().size());
    EXPECT_EQ(11, model.getConcreteModelObjects<CurveCubic>().size());
    EXPECT_EQ(0, model.getConcreteModelObjects<CurveExponent>().size());

    EXPECT_EQ(1, model.getConcreteModelObjects<AirConditionerVariableRefrigerantFlow>().size());
    EXPECT_EQ(1, model.getConcreteModelObjects<ModelObjectList>().size());
    EXPECT_EQ(0, model.getConcreteModelObjects<ZoneHVACTerminalUnitVariableRefrigerantFlow>().size());

    // Curves are no longer used anywhere, so should be removed too
    vrfClone.remove();
    EXPECT_EQ(0, model.getModelObjects<Curve>().size());
    EXPECT_EQ(0, model.getConcreteModelObjects<CurveQuadratic>().size());
    EXPECT_EQ(0, model.getConcreteModelObjects<CurveBiquadratic>().size());
    EXPECT_EQ(0, model.getConcreteModelObjects<CurveCubic>().size());
    EXPECT_EQ(0, model.getConcreteModelObjects<CurveExponent>().size());

    EXPECT_EQ(0, model.getConcreteModelObjects<AirConditionerVariableRefrigerantFlow>().size());
    EXPECT_EQ(0, model.getConcreteModelObjects<ModelObjectList>().size());
    EXPECT_EQ(0, model.getConcreteModelObjects<ZoneHVACTerminalUnitVariableRefrigerantFlow>().size());
  }
}

TEST_F(ModelFixture, AirConditionerVariableRefrigerantFlow_Remove) {
  Model model;

  EXPECT_EQ(0, model.getModelObjects<Curve>().size());
  EXPECT_EQ(0, model.getConcreteModelObjects<CurveQuadratic>().size());
  EXPECT_EQ(0, model.getConcreteModelObjects<CurveBiquadratic>().size());
  EXPECT_EQ(0, model.getConcreteModelObjects<CurveCubic>().size());
  EXPECT_EQ(0, model.getConcreteModelObjects<CurveExponent>().size());

  EXPECT_EQ(0, model.getConcreteModelObjects<AirConditionerVariableRefrigerantFlow>().size());
  EXPECT_EQ(0, model.getConcreteModelObjects<ZoneHVACTerminalUnitVariableRefrigerantFlow>().size());
  EXPECT_EQ(0, model.getConcreteModelObjects<ModelObjectList>().size());

  auto size = model.modelObjects().size();
  AirConditionerVariableRefrigerantFlow vrf(model);
  EXPECT_EQ(20, model.getModelObjects<Curve>().size());
  EXPECT_EQ(0, model.getConcreteModelObjects<CurveQuadratic>().size());
  EXPECT_EQ(9, model.getConcreteModelObjects<CurveBiquadratic>().size());
  EXPECT_EQ(11, model.getConcreteModelObjects<CurveCubic>().size());
  EXPECT_EQ(0, model.getConcreteModelObjects<CurveExponent>().size());

  EXPECT_EQ(1, model.getConcreteModelObjects<AirConditionerVariableRefrigerantFlow>().size());
  EXPECT_EQ(0, model.getConcreteModelObjects<ZoneHVACTerminalUnitVariableRefrigerantFlow>().size());
  EXPECT_EQ(1, model.getConcreteModelObjects<ModelObjectList>().size());

  // VRFTU has no curves, but its Cooling Coil and Heating Coil each add a Quadratic and a Biquadratic, the FanOnOff add a Cubic and one Exponent
  ZoneHVACTerminalUnitVariableRefrigerantFlow term(model);
  vrf.addTerminal(term);
  EXPECT_EQ(1, vrf.terminals().size());

  EXPECT_EQ(26, model.getModelObjects<Curve>().size());
  EXPECT_EQ(2, model.getConcreteModelObjects<CurveQuadratic>().size());
  EXPECT_EQ(11, model.getConcreteModelObjects<CurveBiquadratic>().size());
  EXPECT_EQ(12, model.getConcreteModelObjects<CurveCubic>().size());
  EXPECT_EQ(1, model.getConcreteModelObjects<CurveExponent>().size());

  EXPECT_EQ(1, model.getConcreteModelObjects<AirConditionerVariableRefrigerantFlow>().size());
  EXPECT_EQ(1, model.getConcreteModelObjects<ZoneHVACTerminalUnitVariableRefrigerantFlow>().size());
  EXPECT_EQ(1, model.getConcreteModelObjects<ModelObjectList>().size());

  EXPECT_FALSE(vrf.remove().empty());
  EXPECT_EQ(0, model.getConcreteModelObjects<CurveQuadratic>().size());
  EXPECT_EQ(0, model.getConcreteModelObjects<CurveBiquadratic>().size());
  EXPECT_EQ(0, model.getConcreteModelObjects<CurveCubic>().size());

  EXPECT_EQ(0, model.getConcreteModelObjects<AirConditionerVariableRefrigerantFlow>().size());
  EXPECT_EQ(0, model.getConcreteModelObjects<ZoneHVACTerminalUnitVariableRefrigerantFlow>().size());
  EXPECT_EQ(0, model.getConcreteModelObjects<ModelObjectList>().size());

  EXPECT_EQ(size + 2, model.modelObjects().size());  // Always On Discrete, OnOff
}

TEST_F(ModelFixture, AirConditionerVariableRefrigerantFlow_MatchingCoilTypes) {

  Model model;
  AirConditionerVariableRefrigerantFlow vrf(model);

  {
    FanOnOff fan(model);
    CoilCoolingDXVariableRefrigerantFlow cc(model);
    CoilHeatingDXVariableRefrigerantFlow hc(model);

    ZoneHVACTerminalUnitVariableRefrigerantFlow vrfTerminal(model, cc, hc, fan);
    EXPECT_FALSE(vrfTerminal.isFluidTemperatureControl());

    EXPECT_EQ(0, vrf.terminals().size());
    EXPECT_TRUE(vrf.addTerminal(vrfTerminal));
    EXPECT_EQ(1, vrf.terminals().size());
  }

  {
    FanSystemModel fan(model);
    CoilCoolingDXVariableRefrigerantFlowFluidTemperatureControl ccFluidCtrl(model);
    CoilHeatingDXVariableRefrigerantFlowFluidTemperatureControl hcFluidCtrl(model);

    ZoneHVACTerminalUnitVariableRefrigerantFlow vrfTerminal(model, ccFluidCtrl, hcFluidCtrl, fan);
    EXPECT_TRUE(vrfTerminal.isFluidTemperatureControl());

    EXPECT_EQ(1, vrf.terminals().size());
    EXPECT_FALSE(vrf.addTerminal(vrfTerminal));
    EXPECT_EQ(1, vrf.terminals().size());
  }
}
