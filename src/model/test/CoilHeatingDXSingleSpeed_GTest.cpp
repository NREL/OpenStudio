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
#include "../AirLoopHVAC.hpp"
#include "../AirLoopHVACOutdoorAirSystem.hpp"
#include "../ControllerOutdoorAir.hpp"
#include "../PlantLoop.hpp"
#include "../Model.hpp"
#include "../Node.hpp"
#include "../Node_Impl.hpp"
#include "../CoilHeatingDXSingleSpeed.hpp"
#include "../CoilHeatingDXSingleSpeed_Impl.hpp"
#include "../Schedule.hpp"
#include "../CurveBiquadratic.hpp"
#include "../CurveQuadratic.hpp"
#include "../AirLoopHVACZoneSplitter.hpp"

using namespace openstudio::model;

TEST_F(ModelFixture, CoilHeatingDXSingleSpeed_CoilHeatingDXSingleSpeed) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  ASSERT_EXIT(
    {
      Model m;
      Schedule s = m.alwaysOnDiscreteSchedule();
      CurveBiquadratic totalHeatingCapacityFunctionofTemperatureCurve(m);
      CurveQuadratic totalHeatingCapacityFunctionofFlowFractionCurve(m);
      CurveBiquadratic energyInputRatioFunctionofTemperatureCurve(m);
      CurveQuadratic energyInputRatioFunctionofFlowFractionCurve(m);
      CurveQuadratic partLoadFractionCorrelationCurve(m);

      CoilHeatingDXSingleSpeed coil(m, s, totalHeatingCapacityFunctionofTemperatureCurve, totalHeatingCapacityFunctionofFlowFractionCurve,
                                    energyInputRatioFunctionofTemperatureCurve, energyInputRatioFunctionofFlowFractionCurve,
                                    partLoadFractionCorrelationCurve);

      exit(0);
    },
    ::testing::ExitedWithCode(0), "");
}

TEST_F(ModelFixture, CoilHeatingDXSingleSpeed_addToNode) {
  Model m;
  Schedule s = m.alwaysOnDiscreteSchedule();
  CurveBiquadratic totalHeatingCapacityFunctionofTemperatureCurve(m);
  CurveQuadratic totalHeatingCapacityFunctionofFlowFractionCurve(m);
  CurveBiquadratic energyInputRatioFunctionofTemperatureCurve(m);
  CurveQuadratic energyInputRatioFunctionofFlowFractionCurve(m);
  CurveQuadratic partLoadFractionCorrelationCurve(m);

  CoilHeatingDXSingleSpeed testObject(m, s, totalHeatingCapacityFunctionofTemperatureCurve, totalHeatingCapacityFunctionofFlowFractionCurve,
                                      energyInputRatioFunctionofTemperatureCurve, energyInputRatioFunctionofFlowFractionCurve,
                                      partLoadFractionCorrelationCurve);

  AirLoopHVAC airLoop(m);
  ControllerOutdoorAir controllerOutdoorAir(m);
  AirLoopHVACOutdoorAirSystem outdoorAirSystem(m, controllerOutdoorAir);

  Node supplyOutletNode = airLoop.supplyOutletNode();
  outdoorAirSystem.addToNode(supplyOutletNode);

  EXPECT_TRUE(testObject.addToNode(supplyOutletNode));
  EXPECT_EQ((unsigned)5, airLoop.supplyComponents().size());

  Node inletNode = airLoop.zoneSplitter().lastOutletModelObject()->cast<Node>();

  EXPECT_FALSE(testObject.addToNode(inletNode));
  EXPECT_EQ((unsigned)5, airLoop.demandComponents().size());

  PlantLoop plantLoop(m);
  supplyOutletNode = plantLoop.supplyOutletNode();
  EXPECT_FALSE(testObject.addToNode(supplyOutletNode));
  EXPECT_EQ((unsigned)5, plantLoop.supplyComponents().size());

  Node demandOutletNode = plantLoop.demandOutletNode();
  EXPECT_FALSE(testObject.addToNode(demandOutletNode));
  EXPECT_EQ((unsigned)5, plantLoop.demandComponents().size());

  CurveBiquadratic c1(m);
  CurveQuadratic c2(m);
  CurveBiquadratic c3(m);
  CurveQuadratic c4(m);
  CurveQuadratic c5(m);

  CoilHeatingDXSingleSpeed testObject2(m, s, c1, c2, c3, c4, c5);

  if (boost::optional<Node> OANode = outdoorAirSystem.outboardOANode()) {
    EXPECT_TRUE(testObject2.addToNode(*OANode));
    EXPECT_EQ((unsigned)5, airLoop.supplyComponents().size());
    EXPECT_EQ((unsigned)3, outdoorAirSystem.oaComponents().size());
  }

  CurveBiquadratic c1_2(m);
  CurveQuadratic c2_2(m);
  CurveBiquadratic c3_2(m);
  CurveQuadratic c4_2(m);
  CurveQuadratic c5_2(m);

  CoilHeatingDXSingleSpeed testObject3(m, s, c1_2, c2_2, c3_2, c4_2, c5_2);

  if (boost::optional<Node> reliefNode = outdoorAirSystem.outboardReliefNode()) {
    EXPECT_TRUE(testObject3.addToNode(*reliefNode));
    EXPECT_EQ((unsigned)5, airLoop.supplyComponents().size());
    EXPECT_EQ((unsigned)3, outdoorAirSystem.reliefComponents().size());
  }

  CoilHeatingDXSingleSpeed testObjectClone = testObject.clone(m).cast<CoilHeatingDXSingleSpeed>();
  supplyOutletNode = airLoop.supplyOutletNode();

  EXPECT_TRUE(testObjectClone.addToNode(supplyOutletNode));
  EXPECT_EQ((unsigned)7, airLoop.supplyComponents().size());
}
