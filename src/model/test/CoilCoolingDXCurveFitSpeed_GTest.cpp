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
#include <string>
#include "ModelFixture.hpp"

#include "../CoilCoolingDXCurveFitSpeed.hpp"
#include "../CoilCoolingDXCurveFitSpeed_Impl.hpp"

#include "../CoilCoolingDXCurveFitOperatingMode.hpp"
#include "../CoilCoolingDXCurveFitOperatingMode_Impl.hpp"
#include "../CurveBiquadratic.hpp"
#include "../CurveBiquadratic_Impl.hpp"
#include "../CurveQuadratic.hpp"
#include "../CurveQuadratic_Impl.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, CoilCoolingDXCurveFitSpeed_CoilCoolingDXCurveFitSpeed) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  ASSERT_EXIT(
    {
      // create a model to use
      Model model;

      // create a coil cooling dx curve fit speed object to use
      CoilCoolingDXCurveFitSpeed speed(model);

      exit(0);
    },
    ::testing::ExitedWithCode(0),
    ""
  );

  // create a model to use
  Model model;

  // create a coil cooling dx curve fit speed object to use
  CoilCoolingDXCurveFitSpeed speed(model);

  EXPECT_EQ(1.0, speed.grossTotalCoolingCapacityFraction());
  EXPECT_EQ(1.0, speed.evaporatorAirFlowRateFraction());
  EXPECT_EQ(1.0, speed.condenserAirFlowRateFraction());
  ASSERT_FALSE(speed.grossSensibleHeatRatio());
  EXPECT_TRUE(speed.isGrossSensibleHeatRatioAutosized());
  EXPECT_EQ(3.0, speed.grossCoolingCOP());
  EXPECT_EQ(1.0, speed.activeFractionofCoilFaceArea());
  EXPECT_EQ(773.3, speed.ratedEvaporatorFanPowerPerVolumeFlowRate());
  EXPECT_EQ(1.0, speed.evaporativeCondenserPumpPowerFraction());
  EXPECT_EQ(0.9, speed.evaporativeCondenserEffectiveness());
  EXPECT_FALSE(speed.totalCoolingCapacityModifierFunctionofTemperatureCurve());
  EXPECT_FALSE(speed.totalCoolingCapacityModifierFunctionofAirFlowFractionCurve());
  EXPECT_FALSE(speed.energyInputRatioModifierFunctionofTemperatureCurve());
  EXPECT_FALSE(speed.energyInputRatioModifierFunctionofAirFlowFractionCurve());
  EXPECT_FALSE(speed.partLoadFractionCorrelationCurve());
  EXPECT_FALSE(speed.wasteHeatModifierFunctionofTemperatureCurve());
  ASSERT_FALSE(speed.sensibleHeatRatioModifierFunctionofTemperatureCurve());
  ASSERT_FALSE(speed.sensibleHeatRatioModifierFunctionofFlowFractionCurve());
  EXPECT_EQ(0, speed.coilCoolingDXCurveFitOperatingModes().size());
}

TEST_F(ModelFixture, CoilCoolingDXCurveFitSpeed_GettersSetters) {
  // create a model to use
  Model model;

  // create a coil cooling dx curve fit speed object to use
  CoilCoolingDXCurveFitSpeed speed(model);
}
