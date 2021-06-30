/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2021, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

#include "../HeatPumpPlantLoopEIRHeating.hpp"
#include "../HeatPumpPlantLoopEIRHeating_Impl.hpp"
#include "../HeatPumpPlantLoopEIRCooling.hpp"
#include "../HeatPumpPlantLoopEIRCooling_Impl.hpp"
#include "../CurveBiquadratic.hpp"
#include "../CurveBiquadratic_Impl.hpp"
#include "../CurveQuadratic.hpp"
#include "../CurveQuadratic_Impl.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, HeatPumpPlantLoopEIRHeating_HeatPumpPlantLoopEIRHeating) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  ASSERT_EXIT(
    {
      Model m;
      HeatPumpPlantLoopEIRHeating hp(m);

      exit(0);
    },
    ::testing::ExitedWithCode(0), "");

  Model m;
  HeatPumpPlantLoopEIRHeating hp(m);

  EXPECT_EQ("WaterSource", hp.condenserType());
  EXPECT_FALSE(hp.companionCoolingHeatPump());
  EXPECT_FALSE(hp.referenceLoadSideFlowRate());
  EXPECT_TRUE(hp.isReferenceLoadSideFlowRateAutosized());
  EXPECT_FALSE(hp.referenceSourceSideFlowRate());
  EXPECT_TRUE(hp.isReferenceSourceSideFlowRateAutosized());
  EXPECT_FALSE(hp.referenceCapacity());
  EXPECT_TRUE(hp.isReferenceCapacityAutosized());
  EXPECT_EQ(7.5, hp.referenceCoefficientofPerformance());
  EXPECT_EQ(1.0, hp.sizingFactor());
  boost::optional<CurveBiquadratic> capacityModifierFunctionofTemperatureCurve = hp.capacityModifierFunctionofTemperatureCurve();
  EXPECT_TRUE(capacityModifierFunctionofTemperatureCurve);
  boost::optional<CurveBiquadratic> electricInputtoOutputRatioModifierFunctionofTemperatureCurve =
    hp.electricInputtoOutputRatioModifierFunctionofTemperatureCurve();
  EXPECT_TRUE(electricInputtoOutputRatioModifierFunctionofTemperatureCurve);
  boost::optional<CurveQuadratic> electricInputtoOutputRatioModifierFunctionofPartLoadRatioCurve =
    hp.electricInputtoOutputRatioModifierFunctionofPartLoadRatioCurve();
  EXPECT_TRUE(electricInputtoOutputRatioModifierFunctionofPartLoadRatioCurve);
}

TEST_F(ModelFixture, HeatPumpPlantLoopEIRHeating_GettersSetters) {
  Model m;
  HeatPumpPlantLoopEIRHeating hp(m);

  EXPECT_TRUE(hp.setCondenserType("AirSource"));
  HeatPumpPlantLoopEIRCooling companionHP(m);
  EXPECT_TRUE(hp.setCompanionCoolingHeatPump(companionHP));
  EXPECT_TRUE(hp.setReferenceLoadSideFlowRate(1.0));
  EXPECT_TRUE(hp.setReferenceSourceSideFlowRate(2.0));
  EXPECT_TRUE(hp.setReferenceCapacity(3.0));
  EXPECT_TRUE(hp.setReferenceCoefficientofPerformance(4.0));
  EXPECT_TRUE(hp.setSizingFactor(5.0));
  CurveBiquadratic curve1(m);
  EXPECT_TRUE(hp.setCapacityModifierFunctionofTemperatureCurve(curve1));
  CurveBiquadratic curve2(m);
  EXPECT_TRUE(hp.setElectricInputtoOutputRatioModifierFunctionofTemperatureCurve(curve2));
  CurveQuadratic curve3(m);
  EXPECT_TRUE(hp.setElectricInputtoOutputRatioModifierFunctionofPartLoadRatioCurve(curve3));

  EXPECT_EQ("AirSource", hp.condenserType());
  ASSERT_TRUE(hp.companionCoolingHeatPump());
  EXPECT_EQ(companionHP.handle(), hp.companionCoolingHeatPump().get().handle());
  ASSERT_TRUE(hp.referenceLoadSideFlowRate());
  EXPECT_EQ(1.0, hp.referenceLoadSideFlowRate().get());
  EXPECT_FALSE(hp.isReferenceLoadSideFlowRateAutosized());
  ASSERT_TRUE(hp.referenceSourceSideFlowRate());
  EXPECT_EQ(2.0, hp.referenceSourceSideFlowRate());
  EXPECT_FALSE(hp.isReferenceSourceSideFlowRateAutosized());
  ASSERT_TRUE(hp.referenceCapacity());
  EXPECT_EQ(3.0, hp.referenceCapacity().get());
  EXPECT_FALSE(hp.isReferenceCapacityAutosized());
  EXPECT_EQ(4.0, hp.referenceCoefficientofPerformance());
  EXPECT_EQ(5.0, hp.sizingFactor());
  EXPECT_EQ(curve1.handle(), hp.capacityModifierFunctionofTemperatureCurve().handle());
  EXPECT_EQ(curve2.handle(), hp.electricInputtoOutputRatioModifierFunctionofTemperatureCurve().handle());
  EXPECT_EQ(curve3.handle(), hp.electricInputtoOutputRatioModifierFunctionofPartLoadRatioCurve().handle());

  hp.autosizeReferenceLoadSideFlowRate();
  hp.autosizeReferenceSourceSideFlowRate();
  hp.autosizeReferenceCapacity();

  EXPECT_TRUE(hp.isReferenceLoadSideFlowRateAutosized());
  EXPECT_TRUE(hp.isReferenceSourceSideFlowRateAutosized());
  EXPECT_TRUE(hp.isReferenceCapacityAutosized());
}

TEST_F(ModelFixture, HeatPumpPlantLoopEIRHeating_remove) {
  Model m;
  HeatPumpPlantLoopEIRHeating hp(m);
  auto size = m.modelObjects().size();
  EXPECT_FALSE(hp.remove().empty());
  EXPECT_EQ(size - 1, m.modelObjects().size());
  EXPECT_EQ(0u, m.getConcreteModelObjects<HeatPumpPlantLoopEIRHeating>().size());
}

TEST_F(ModelFixture, HeatPumpPlantLoopEIRHeating_clone) {
  Model m;
  CurveBiquadratic curve1(m);
  CurveBiquadratic curve2(m);
  CurveQuadratic curve3(m);
  HeatPumpPlantLoopEIRHeating hp(m, curve1, curve2, curve3);

  {
    HeatPumpPlantLoopEIRHeating hpClone = hp.clone(m).cast<HeatPumpPlantLoopEIRHeating>();
    EXPECT_EQ(curve1.handle(), hp.capacityModifierFunctionofTemperatureCurve().handle());
    EXPECT_EQ(curve2.handle(), hp.electricInputtoOutputRatioModifierFunctionofTemperatureCurve().handle());
    EXPECT_EQ(curve3.handle(), hp.electricInputtoOutputRatioModifierFunctionofPartLoadRatioCurve().handle());
  }

  {
    Model m2;
    HeatPumpPlantLoopEIRHeating hpClone2 = hp.clone(m2).cast<HeatPumpPlantLoopEIRHeating>();
    EXPECT_EQ(curve1.handle(), hp.capacityModifierFunctionofTemperatureCurve().handle());
    EXPECT_EQ(curve2.handle(), hp.electricInputtoOutputRatioModifierFunctionofTemperatureCurve().handle());
    EXPECT_EQ(curve3.handle(), hp.electricInputtoOutputRatioModifierFunctionofPartLoadRatioCurve().handle());
  }
}
