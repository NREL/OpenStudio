/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
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

  std::vector<RefrigerationCondenserAirCooled> refrigerationCondenserAirCooleds = model.getConcreteModelObjects<RefrigerationCondenserAirCooled>();
  EXPECT_EQ(1, refrigerationCondenserAirCooleds.size());

  testObject.remove();

  refrigerationCondenserAirCooleds = model.getConcreteModelObjects<RefrigerationCondenserAirCooled>();
  EXPECT_EQ(0, refrigerationCondenserAirCooleds.size());
}

TEST_F(ModelFixture, RefrigerationCondenserAirCooled_CloneOneModelWithDefaultData) {
  Model model;
  RefrigerationCondenserAirCooled testObject = RefrigerationCondenserAirCooled(model);

  auto testObjectClone = testObject.clone(model).cast<RefrigerationCondenserAirCooled>();

  std::vector<CurveLinear> refrigerationCondenserCurves = model.getConcreteModelObjects<CurveLinear>();
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

  auto testObjectClone = testObject.clone(model).cast<RefrigerationCondenserAirCooled>();
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

  auto testObjectClone = testObject.clone(model).cast<RefrigerationCondenserAirCooled>();

  Model model2;

  std::vector<CurveLinear> refrigerationCondenserCurves2 = model2.getConcreteModelObjects<CurveLinear>();
  EXPECT_EQ(0, refrigerationCondenserCurves2.size());

  auto testObjectClone2 = testObject.clone(model2).cast<RefrigerationCondenserAirCooled>();

  std::vector<CurveLinear> refrigerationCondenserCurves = model.getConcreteModelObjects<CurveLinear>();
  EXPECT_EQ(1, refrigerationCondenserCurves.size());

  refrigerationCondenserCurves2 = model2.getConcreteModelObjects<CurveLinear>();
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
