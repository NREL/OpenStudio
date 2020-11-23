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

#include "../RefrigerationCondenserAirCooled.hpp"
#include "../RefrigerationCondenserAirCooled_Impl.hpp"
#include "../CurveLinear.hpp"
#include "../CurveLinear_Impl.hpp"
#include "../ThermalZone.hpp"
#include "../ThermalZone_Impl.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, RefrigerationCondenserAirCooled_DefaultConstructor) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  ASSERT_EXIT(
    {
      Model model;
      RefrigerationCondenserAirCooled testObject = RefrigerationCondenserAirCooled(model);

      exit(0);
    },
    ::testing::ExitedWithCode(0), "");
}

TEST_F(ModelFixture, RefrigerationCondenserAirCooled_Remove) {
  Model model;
  RefrigerationCondenserAirCooled testObject = RefrigerationCondenserAirCooled(model);

  std::vector<RefrigerationCondenserAirCooled> refrigerationCondenserAirCooleds = model.getModelObjects<RefrigerationCondenserAirCooled>();
  EXPECT_EQ(1, refrigerationCondenserAirCooleds.size());

  testObject.remove();

  refrigerationCondenserAirCooleds = model.getModelObjects<RefrigerationCondenserAirCooled>();
  EXPECT_EQ(0, refrigerationCondenserAirCooleds.size());
}

TEST_F(ModelFixture, RefrigerationCondenserAirCooled_CloneOneModelWithDefaultData) {
  Model model;
  RefrigerationCondenserAirCooled testObject = RefrigerationCondenserAirCooled(model);

  RefrigerationCondenserAirCooled testObjectClone = testObject.clone(model).cast<RefrigerationCondenserAirCooled>();

  std::vector<CurveLinear> refrigerationCondenserCurves = model.getModelObjects<CurveLinear>();
  for (auto it = refrigerationCondenserCurves.begin(); it != refrigerationCondenserCurves.end(); ++it) {
    EXPECT_TRUE(it->parent());
  }

  EXPECT_DOUBLE_EQ(0.0, testObjectClone.ratedSubcoolingTemperatureDifference());
  EXPECT_DOUBLE_EQ(7000.0, testObjectClone.ratedFanPower());
  EXPECT_EQ("VariableSpeed", testObjectClone.condenserFanSpeedControlType());
  EXPECT_DOUBLE_EQ(0.25, testObjectClone.minimumFanAirFlowRatio());
  EXPECT_EQ(testObject.ratedEffectiveTotalHeatRejectionRateCurve().get().handle(),
            testObjectClone.ratedEffectiveTotalHeatRejectionRateCurve().get().handle());
}

TEST_F(ModelFixture, RefrigerationCondenserAirCooled_CloneOneModelWithCustomData) {
  Model model;
  ThermalZone thermalZone(model);
  RefrigerationCondenserAirCooled testObject = RefrigerationCondenserAirCooled(model);
  testObject.setRatedSubcoolingTemperatureDifference(999.0);
  testObject.setCondenserFanSpeedControlType("TwoSpeed");
  testObject.setRatedFanPower(999.0);
  testObject.setMinimumFanAirFlowRatio(999.0);
  testObject.setAirInletZone(thermalZone);

  CurveLinear heatRejectionCurve = CurveLinear(model);
  heatRejectionCurve.setName("Refrigeration Condenser Air Cooled HR Curve Test");
  heatRejectionCurve.setCoefficient1Constant(0.0);
  heatRejectionCurve.setCoefficient2x(22000.0);
  heatRejectionCurve.setMinimumValueofx(5.0);
  heatRejectionCurve.setMaximumValueofx(22.2);

  testObject.setRatedEffectiveTotalHeatRejectionRateCurve(heatRejectionCurve);

  RefrigerationCondenserAirCooled testObjectClone = testObject.clone(model).cast<RefrigerationCondenserAirCooled>();
  EXPECT_DOUBLE_EQ(999.0, testObjectClone.ratedSubcoolingTemperatureDifference());
  EXPECT_DOUBLE_EQ(999.0, testObjectClone.ratedFanPower());
  EXPECT_EQ("TwoSpeed", testObjectClone.condenserFanSpeedControlType());
  EXPECT_DOUBLE_EQ(999.0, testObjectClone.minimumFanAirFlowRatio());
  EXPECT_FALSE(testObjectClone.airInletZone());
  EXPECT_EQ(testObject.ratedEffectiveTotalHeatRejectionRateCurve().get().handle(),
            testObjectClone.ratedEffectiveTotalHeatRejectionRateCurve().get().handle());
  EXPECT_EQ(heatRejectionCurve.handle(), testObjectClone.ratedEffectiveTotalHeatRejectionRateCurve().get().handle());
}

TEST_F(ModelFixture, RefrigerationCondenserAirCooled_CloneTwoModelsWithDefaultData) {
  Model model;
  RefrigerationCondenserAirCooled testObject = RefrigerationCondenserAirCooled(model);

  RefrigerationCondenserAirCooled testObjectClone = testObject.clone(model).cast<RefrigerationCondenserAirCooled>();

  Model model2;

  std::vector<CurveLinear> refrigerationCondenserCurves2 = model2.getModelObjects<CurveLinear>();
  EXPECT_EQ(0, refrigerationCondenserCurves2.size());

  RefrigerationCondenserAirCooled testObjectClone2 = testObject.clone(model2).cast<RefrigerationCondenserAirCooled>();

  std::vector<CurveLinear> refrigerationCondenserCurves = model.getModelObjects<CurveLinear>();
  EXPECT_EQ(1, refrigerationCondenserCurves.size());

  refrigerationCondenserCurves2 = model2.getModelObjects<CurveLinear>();
  EXPECT_EQ(1, refrigerationCondenserCurves2.size());

  for (auto it = refrigerationCondenserCurves.begin(); it != refrigerationCondenserCurves.end(); ++it) {
    EXPECT_TRUE(it->parent());
  }

  for (auto it = refrigerationCondenserCurves2.begin(); it != refrigerationCondenserCurves2.end(); ++it) {
    EXPECT_TRUE(it->parent());
  }

  EXPECT_DOUBLE_EQ(0.0, testObjectClone2.ratedSubcoolingTemperatureDifference());
  EXPECT_DOUBLE_EQ(7000.0, testObjectClone2.ratedFanPower());
  EXPECT_EQ("VariableSpeed", testObjectClone2.condenserFanSpeedControlType());
  EXPECT_DOUBLE_EQ(0.25, testObjectClone2.minimumFanAirFlowRatio());
  EXPECT_NE(testObject.ratedEffectiveTotalHeatRejectionRateCurve().get().handle(),
            testObjectClone2.ratedEffectiveTotalHeatRejectionRateCurve().get().handle());
  EXPECT_NE(testObjectClone2, testObjectClone);
  EXPECT_NE(testObjectClone2.handle(), testObjectClone.handle());
}
