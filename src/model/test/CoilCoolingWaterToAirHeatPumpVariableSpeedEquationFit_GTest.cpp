/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>
#include "ModelFixture.hpp"
#include "../CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit.hpp"
#include "../CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit_Impl.hpp"
#include "../Curve.hpp"
#include "../Curve_Impl.hpp"
#include "../CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData.hpp"
#include "../CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  ASSERT_EXIT(
    {
      Model m;
      CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit coil(m);

      exit(0);
    },
    ::testing::ExitedWithCode(0), "");
}

TEST_F(ModelFixture, CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit_Remove) {
  Model m;
  auto count = m.modelObjects().size();

  CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit coil(m);
  CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData speed(m);
  coil.addSpeed(speed);
  coil.remove();

  auto curves = m.getModelObjects<model::Curve>();

  EXPECT_EQ(count, m.modelObjects().size() - curves.size());
}

TEST_F(ModelFixture, CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit_2320_NewFields) {
  Model m;

  CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit coil(m);

  // E+ 23.2.0 defaults Maximum Cycling Rate and Latent Capacity Time Constant to 0.0, we don't, cf #4999
  EXPECT_EQ(2.5, coil.maximumCyclingRate());
  EXPECT_EQ(60.0, coil.latentCapacityTimeConstant());
  // Test IDD defaults
  EXPECT_EQ(60.0, coil.fanDelayTime());

  EXPECT_TRUE(coil.setMaximumCyclingRate(3.5));
  EXPECT_EQ(3.5, coil.maximumCyclingRate());
  EXPECT_FALSE(coil.setMaximumCyclingRate(-12.5));
  EXPECT_EQ(3.5, coil.maximumCyclingRate());

  EXPECT_TRUE(coil.setLatentCapacityTimeConstant(90.0));
  EXPECT_EQ(90.0, coil.latentCapacityTimeConstant());
  EXPECT_FALSE(coil.setLatentCapacityTimeConstant(-12.5));
  EXPECT_EQ(90.0, coil.latentCapacityTimeConstant());

  EXPECT_TRUE(coil.setFanDelayTime(120.0));
  EXPECT_EQ(120.0, coil.fanDelayTime());
  EXPECT_FALSE(coil.setFanDelayTime(-12.5));
  EXPECT_EQ(120.0, coil.fanDelayTime());
}
