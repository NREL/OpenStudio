/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>
#include "ModelFixture.hpp"
#include "../CoilCoolingDXVariableSpeed.hpp"
#include "../CoilCoolingDXVariableSpeed_Impl.hpp"
#include "../Curve.hpp"
#include "../Curve_Impl.hpp"
#include "../CurveLinear.hpp"
#include "../CoilCoolingDXVariableSpeedSpeedData.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, CoilCoolingDXVariableSpeed) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  ASSERT_EXIT(
    {
      Model m;
      CoilCoolingDXVariableSpeed coil(m);

      exit(0);
    },
    ::testing::ExitedWithCode(0), "");
}

TEST_F(ModelFixture, CoilCoolingDXVariableSpeed_Speeds) {
  Model m;
  CoilCoolingDXVariableSpeed coil(m);

  CoilCoolingDXVariableSpeedSpeedData speed1(m);
  coil.addSpeed(speed1);

  CoilCoolingDXVariableSpeedSpeedData speed2(m);
  coil.addSpeed(speed2);

  ASSERT_EQ(2u, coil.speeds().size());
}

TEST_F(ModelFixture, CoilCoolingDXVariableSpeed_Remove) {
  Model m;
  auto count = m.modelObjects().size();

  CoilCoolingDXVariableSpeed coil(m);
  CoilCoolingDXVariableSpeedSpeedData speed1(m);
  coil.addSpeed(speed1);
  coil.remove();

  auto curves = m.getModelObjects<model::Curve>();

  EXPECT_EQ(count, m.modelObjects().size() - curves.size());
}

TEST_F(ModelFixture, CoilCoolingDXVariableSpeed_MinOATCompressor) {
  Model m;

  CoilCoolingDXVariableSpeed coil(m);

  // #3976 - Minimum Outdoor Dry-Bulb Temperature for Compressor Operation
  // IDD Default
  EXPECT_EQ(-25.0, coil.minimumOutdoorDryBulbTemperatureforCompressorOperation());
  // There are no IDD limits, so everything should work
  EXPECT_TRUE(coil.setMinimumOutdoorDryBulbTemperatureforCompressorOperation(-5));
  EXPECT_EQ(-5, coil.minimumOutdoorDryBulbTemperatureforCompressorOperation());
}

TEST_F(ModelFixture, CoilCoolingDXVariableSpeed_2320_NewFields) {
  Model m;

  CoilCoolingDXVariableSpeed coil(m);

  // Test IDD defaults
  EXPECT_EQ(2.5, coil.maximumCyclingRate());
  EXPECT_EQ(60.0, coil.latentCapacityTimeConstant());
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

  EXPECT_FALSE(coil.crankcaseHeaterCapacityFunctionofTemperatureCurve());
  const CurveLinear crankCurve(m);
  EXPECT_TRUE(coil.setCrankcaseHeaterCapacityFunctionofTemperatureCurve(crankCurve));
  ASSERT_TRUE(coil.crankcaseHeaterCapacityFunctionofTemperatureCurve());
  EXPECT_EQ(crankCurve, coil.crankcaseHeaterCapacityFunctionofTemperatureCurve().get());
  coil.resetCrankcaseHeaterCapacityFunctionofTemperatureCurve();
  EXPECT_FALSE(coil.crankcaseHeaterCapacityFunctionofTemperatureCurve());
}
