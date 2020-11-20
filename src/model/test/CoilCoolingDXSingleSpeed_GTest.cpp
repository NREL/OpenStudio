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
#include "../CoilCoolingDXSingleSpeed.hpp"
#include "../CoilCoolingDXSingleSpeed_Impl.hpp"
#include "../Schedule.hpp"
#include "../CurveBiquadratic.hpp"
#include "../CurveQuadratic.hpp"
#include "../Node.hpp"
#include "../Node_Impl.hpp"
#include "../AirLoopHVACZoneSplitter.hpp"
#include "../AirLoopHVACOutdoorAirSystem.hpp"
#include "../ControllerOutdoorAir.hpp"
#include "../AirLoopHVAC.hpp"
#include "../PlantLoop.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, CoilCoolingDXSingleSpeed_RatedTotalCoolingCapacity_GettersSetters) {
  Model m;
  Schedule s = m.alwaysOnDiscreteSchedule();
  CurveBiquadratic totalCoolingCapacityFunctionofTemperatureCurve(m);
  CurveQuadratic totalCoolingCapacityFunctionofFlowFractionCurve(m);
  CurveBiquadratic energyInputRatioFunctionofTemperatureCurve(m);
  CurveQuadratic energyInputRatioFunctionofFlowFractionCurve(m);
  CurveQuadratic partLoadFractionCorrelationCurve(m);

  CoilCoolingDXSingleSpeed coil(m, s, totalCoolingCapacityFunctionofTemperatureCurve, totalCoolingCapacityFunctionofFlowFractionCurve,
                                energyInputRatioFunctionofTemperatureCurve, energyInputRatioFunctionofFlowFractionCurve,
                                partLoadFractionCorrelationCurve);

  coil.setRatedTotalCoolingCapacity(1.0);
  coil.setRatedSensibleHeatRatio(0.6);
  coil.setRatedCOP(3.6);
  coil.setRatedAirFlowRate(1.1);
  coil.setRatedEvaporatorFanPowerPerVolumeFlowRate(600.0);
  coil.setNominalTimeForCondensateRemovalToBegin(50.0);
  coil.setRatioOfInitialMoistureEvaporationRateAndSteadyStateLatentCapacity(3.0);
  coil.setMaximumCyclingRate(3.2);
  coil.setLatentCapacityTimeConstant(499.8);
  coil.setEvaporativeCondenserEffectiveness(0.8);
  coil.setEvaporativeCondenserAirFlowRate(1.2);
  coil.setEvaporativeCondenserPumpRatedPowerConsumption(1.3);
  coil.setMaximumOutdoorDryBulbTemperatureForCrankcaseHeaterOperation(1.4);
  coil.setBasinHeaterCapacity(1.5);
  coil.setBasinHeaterSetpointTemperature(2.5);

  EXPECT_EQ(coil.ratedTotalCoolingCapacity(), 1.0);
  EXPECT_EQ(coil.ratedSensibleHeatRatio(), 0.6);
  EXPECT_EQ(coil.ratedCOP(), 3.6);
  EXPECT_EQ(coil.ratedAirFlowRate(), 1.1);
  EXPECT_EQ(coil.ratedEvaporatorFanPowerPerVolumeFlowRate(), 600.0);
  EXPECT_EQ(coil.nominalTimeForCondensateRemovalToBegin(), 50.0);
  EXPECT_EQ(coil.ratioOfInitialMoistureEvaporationRateAndSteadyStateLatentCapacity(), 3.0);
  EXPECT_EQ(coil.maximumCyclingRate(), 3.2);
  EXPECT_EQ(coil.latentCapacityTimeConstant(), 499.8);
  EXPECT_EQ(coil.evaporativeCondenserEffectiveness(), 0.8);
  EXPECT_EQ(coil.evaporativeCondenserAirFlowRate(), 1.2);
  EXPECT_EQ(coil.evaporativeCondenserPumpRatedPowerConsumption(), 1.3);
  EXPECT_EQ(coil.maximumOutdoorDryBulbTemperatureForCrankcaseHeaterOperation(), 1.4);
  EXPECT_EQ(coil.basinHeaterCapacity(), 1.5);
  EXPECT_EQ(coil.basinHeaterSetpointTemperature(), 2.5);

  // #3976 - Minimum Outdoor Dry-Bulb Temperature for Compressor Operation
  // IDD Default
  EXPECT_EQ(-25.0, coil.minimumOutdoorDryBulbTemperatureforCompressorOperation());
  // There are no IDD limits, so everything should work
  EXPECT_TRUE(coil.setMinimumOutdoorDryBulbTemperatureforCompressorOperation(-5));
  EXPECT_EQ(-5, coil.minimumOutdoorDryBulbTemperatureforCompressorOperation());
}

TEST_F(ModelFixture, CoilCoolingDXSingleSpeed_addToNode) {
  Model m;
  Schedule s = m.alwaysOnDiscreteSchedule();
  CurveBiquadratic totalCoolingCapacityFunctionofTemperatureCurve(m);
  CurveQuadratic totalCoolingCapacityFunctionofFlowFractionCurve(m);
  CurveBiquadratic energyInputRatioFunctionofTemperatureCurve(m);
  CurveQuadratic energyInputRatioFunctionofFlowFractionCurve(m);
  CurveQuadratic partLoadFractionCorrelationCurve(m);

  CoilCoolingDXSingleSpeed testObject(m, s, totalCoolingCapacityFunctionofTemperatureCurve, totalCoolingCapacityFunctionofFlowFractionCurve,
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

  CoilCoolingDXSingleSpeed testObject2(m, s, c1, c2, c3, c4, c5);

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

  CoilCoolingDXSingleSpeed testObject3(m, s, c1_2, c2_2, c3_2, c4_2, c5_2);

  if (boost::optional<Node> reliefNode = outdoorAirSystem.outboardReliefNode()) {
    EXPECT_TRUE(testObject3.addToNode(*reliefNode));
    EXPECT_EQ((unsigned)5, airLoop.supplyComponents().size());
    EXPECT_EQ((unsigned)3, outdoorAirSystem.reliefComponents().size());
  }

  // tests and checks to figure out clone bug
  // resolution: Due to using ModelObject::clone instead of StraightComponent::clone in reimplementation
  AirLoopHVAC airLoop2(m);
  EXPECT_EQ((unsigned)5, airLoop.supplyComponents().size());
  CoilCoolingDXSingleSpeed testObjectClone = testObject.clone(m).cast<CoilCoolingDXSingleSpeed>();
  CoilCoolingDXSingleSpeed testObjectClone2 = testObject.clone(m).cast<CoilCoolingDXSingleSpeed>();
  EXPECT_EQ((unsigned)5, airLoop.supplyComponents().size());
  Node supplyOutletNode2 = airLoop2.supplyOutletNode();
  supplyOutletNode = airLoop.supplyOutletNode();

  EXPECT_NE(testObject, testObjectClone);
  EXPECT_FALSE(testObjectClone.loop());
  EXPECT_FALSE(testObjectClone.airLoopHVAC());
  EXPECT_EQ(testObject.totalCoolingCapacityFunctionOfTemperatureCurve(), testObjectClone.totalCoolingCapacityFunctionOfTemperatureCurve());
  EXPECT_EQ(testObject.totalCoolingCapacityFunctionOfFlowFractionCurve(), testObjectClone.totalCoolingCapacityFunctionOfFlowFractionCurve());
  EXPECT_EQ(testObject.energyInputRatioFunctionOfTemperatureCurve(), testObjectClone.energyInputRatioFunctionOfTemperatureCurve());
  EXPECT_EQ(testObject.energyInputRatioFunctionOfFlowFractionCurve(), testObjectClone.energyInputRatioFunctionOfFlowFractionCurve());
  EXPECT_EQ(testObject.partLoadFractionCorrelationCurve(), testObjectClone.partLoadFractionCorrelationCurve());
  EXPECT_TRUE(testObject.inletModelObject());
  EXPECT_TRUE(testObject.outletModelObject());
  EXPECT_FALSE(testObjectClone.inletModelObject());
  EXPECT_FALSE(testObjectClone.outletModelObject());

  EXPECT_TRUE(testObjectClone.addToNode(supplyOutletNode2));
  EXPECT_EQ((unsigned)5, airLoop.supplyComponents().size());
  EXPECT_EQ((unsigned)3, airLoop2.supplyComponents().size());
  EXPECT_TRUE(testObjectClone2.addToNode(supplyOutletNode));
  EXPECT_EQ((unsigned)7, airLoop.supplyComponents().size());
}
