/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>

#include "ModelFixture.hpp"
#include "../CurveFanPressureRise.hpp"

#include <cmath>

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, CurveFanPressureRise_DefaultConstructors) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  ASSERT_EXIT(
    {
      Model m;
      CurveFanPressureRise curve(m);

      exit(0);
    },
    ::testing::ExitedWithCode(0), "");
}

TEST_F(ModelFixture, CurveFanPressureRise_GetterSetters_evaluate) {

  Model m;
  CurveFanPressureRise curve(m);

  EXPECT_EQ(1.0, curve.coefficient1C1());
  EXPECT_EQ(1.0, curve.coefficient2C2());
  EXPECT_EQ(1.0, curve.coefficient3C3());
  EXPECT_EQ(1.0, curve.coefficient4C4());

  EXPECT_EQ(0.0, curve.minimumValueofQfan());
  EXPECT_EQ(10.0, curve.maximumValueofQfan());
  EXPECT_EQ(0.0, curve.minimumValueofPsm());
  EXPECT_EQ(500.0, curve.maximumValueofPsm());

  double c1 = 1.0;
  double c2 = 2.0;
  double c3 = 3.0;
  double c4 = 4.0;
  double min_x = 0.1;
  double max_x = 3.0;
  double min_y = 10;
  double max_y = 400;

  auto calc = [c1, c2, c3, c4](double x, double y) {
    // CurveValue = x * (c1 * x + c2 + c3 * std::sqrt(y)) + c4 * y;
    return c1 * std::pow(x, 2) + c2 * x + c3 * x * std::sqrt(y) + c4 * y;
  };

  EXPECT_TRUE(curve.setCoefficient1C1(c1));
  EXPECT_TRUE(curve.setCoefficient2C2(c2));
  EXPECT_TRUE(curve.setCoefficient3C3(c3));
  EXPECT_TRUE(curve.setCoefficient4C4(c4));

  EXPECT_EQ(c1, curve.coefficient1C1());
  EXPECT_EQ(c2, curve.coefficient2C2());
  EXPECT_EQ(c3, curve.coefficient3C3());
  EXPECT_EQ(c4, curve.coefficient4C4());

  EXPECT_TRUE(curve.setMinimumValueofQfan(min_x));
  EXPECT_TRUE(curve.setMaximumValueofQfan(max_x));
  EXPECT_EQ(min_x, curve.minimumValueofQfan());
  EXPECT_EQ(max_x, curve.maximumValueofQfan());

  EXPECT_TRUE(curve.setMinimumValueofPsm(min_y));
  EXPECT_TRUE(curve.setMaximumValueofPsm(max_y));
  EXPECT_EQ(min_y, curve.minimumValueofPsm());
  EXPECT_EQ(max_y, curve.maximumValueofPsm());

  EXPECT_FALSE(curve.minimumCurveOutput());
  EXPECT_FALSE(curve.maximumCurveOutput());

  // x and y in range, no output limit
  double x = 0.5;
  double y = 15;
  EXPECT_DOUBLE_EQ(calc(x, y), curve.evaluate(x, y));
  EXPECT_DOUBLE_EQ(67.05947501931112, curve.evaluate(x, y));

  // x < min_x
  x = 0.05;
  EXPECT_DOUBLE_EQ(calc(min_x, y), curve.evaluate(x, y));
  EXPECT_DOUBLE_EQ(61.37189500386223, curve.evaluate(x, y));

  // x > max_x
  x = 20.0;
  EXPECT_DOUBLE_EQ(calc(max_x, y), curve.evaluate(x, y));
  EXPECT_DOUBLE_EQ(109.85685011586676, curve.evaluate(x, y));

  // y < min_y
  x = 0.5;
  y = 5.0;
  EXPECT_DOUBLE_EQ(calc(x, min_y), curve.evaluate(x, y));
  EXPECT_DOUBLE_EQ(45.99341649025257, curve.evaluate(x, y));

  // y > max_y
  y = 1000.0;
  EXPECT_DOUBLE_EQ(calc(x, max_y), curve.evaluate(x, y));
  EXPECT_DOUBLE_EQ(1631.25, curve.evaluate(x, y));

  // x < min_x, y < min_y
  x = 0.05;
  y = 5.0;
  EXPECT_DOUBLE_EQ(calc(min_x, min_y), curve.evaluate(x, y));
  EXPECT_DOUBLE_EQ(41.15868329805051, curve.evaluate(x, y));

  // x > max_x, y > max_y
  x = 20.0;
  y = 1000.0;
  EXPECT_DOUBLE_EQ(calc(max_x, max_y), curve.evaluate(x, y));
  EXPECT_DOUBLE_EQ(1795.0, curve.evaluate(x, y));

  // Set output limits
  double min_output = 50.0;
  double max_output = 1500.0;
  EXPECT_TRUE(curve.setMinimumCurveOutput(min_output));
  EXPECT_TRUE(curve.setMaximumCurveOutput(max_output));

  ASSERT_TRUE(curve.minimumCurveOutput());
  ASSERT_TRUE(curve.maximumCurveOutput());
  EXPECT_EQ(min_output, curve.minimumCurveOutput().get());
  EXPECT_EQ(max_output, curve.maximumCurveOutput().get());

  // out < min output
  EXPECT_DOUBLE_EQ(min_output, curve.evaluate(min_x, min_y));
  // out > max output
  EXPECT_DOUBLE_EQ(max_output, curve.evaluate(max_x, max_y));

  // Wrong number of arguments
  // EXPECT_THROW(curve.evaluate(1.0), openstudio::Exception);
  // EXPECT_THROW(curve.evaluate(1.0, 2.0, 3.0), openstudio::Exception);
}
