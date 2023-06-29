/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>

#include "ModelFixture.hpp"
#include "../CurveQuadraticLinear.hpp"

#include <cmath>

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, CurveQuadraticLinear_DefaultConstructors) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  ASSERT_EXIT(
    {
      Model m;
      CurveQuadraticLinear curve(m);

      exit(0);
    },
    ::testing::ExitedWithCode(0), "");
}

TEST_F(ModelFixture, CurveQuadraticLinear_GetterSetters_evaluate) {

  Model m;
  CurveQuadraticLinear curve(m);

  EXPECT_EQ(0.0, curve.coefficient1Constant());
  EXPECT_EQ(0.0, curve.coefficient2x());
  EXPECT_EQ(0.0, curve.coefficient3xPOW2());
  EXPECT_EQ(0.0, curve.coefficient4y());
  EXPECT_EQ(0.0, curve.coefficient5xTIMESY());
  EXPECT_EQ(1.0, curve.coefficient6xPOW2TIMESY());

  EXPECT_EQ(0.0, curve.minimumValueofx());
  EXPECT_EQ(1.0, curve.maximumValueofx());
  EXPECT_EQ(0.0, curve.minimumValueofy());
  EXPECT_EQ(1.0, curve.maximumValueofy());

  double c1 = 1.0;
  double c2 = 2.0;
  double c3 = 3.0;
  double c4 = 4.0;
  double c5 = 5.0;
  double c6 = 6.0;
  double min_x = 0.1;
  double max_x = 3.0;
  double min_y = 10;
  double max_y = 30;

  auto calc = [c1, c2, c3, c4, c5, c6](double x, double y) { return c1 + c2 * x + c3 * std::pow(x, 2) + (c4 + c5 * x + c6 * std::pow(x, 2)) * y; };

  EXPECT_TRUE(curve.setCoefficient1Constant(c1));
  EXPECT_TRUE(curve.setCoefficient2x(c2));
  EXPECT_TRUE(curve.setCoefficient3xPOW2(c3));
  EXPECT_TRUE(curve.setCoefficient4y(c4));
  EXPECT_TRUE(curve.setCoefficient5xTIMESY(c5));
  EXPECT_TRUE(curve.setCoefficient6xPOW2TIMESY(c6));

  EXPECT_EQ(c1, curve.coefficient1Constant());
  EXPECT_EQ(c2, curve.coefficient2x());
  EXPECT_EQ(c3, curve.coefficient3xPOW2());
  EXPECT_EQ(c4, curve.coefficient4y());
  EXPECT_EQ(c5, curve.coefficient5xTIMESY());
  EXPECT_EQ(c6, curve.coefficient6xPOW2TIMESY());

  EXPECT_TRUE(curve.setMinimumValueofx(min_x));
  EXPECT_TRUE(curve.setMaximumValueofx(max_x));
  EXPECT_EQ(min_x, curve.minimumValueofx());
  EXPECT_EQ(max_x, curve.maximumValueofx());

  EXPECT_TRUE(curve.setMinimumValueofy(min_y));
  EXPECT_TRUE(curve.setMaximumValueofy(max_y));
  EXPECT_EQ(min_y, curve.minimumValueofy());
  EXPECT_EQ(max_y, curve.maximumValueofy());

  EXPECT_FALSE(curve.minimumCurveOutput());
  EXPECT_FALSE(curve.maximumCurveOutput());

  // x and y in range, no output limit
  double x = 0.5;
  double y = 15;
  EXPECT_DOUBLE_EQ(calc(x, y), curve.evaluate(x, y));
  EXPECT_DOUBLE_EQ(122.75, curve.evaluate(x, y));

  // x < min_x
  x = 0.05;
  EXPECT_DOUBLE_EQ(calc(min_x, y), curve.evaluate(x, y));
  EXPECT_DOUBLE_EQ(69.63, curve.evaluate(x, y));

  // x > max_x
  x = 20.0;
  EXPECT_DOUBLE_EQ(calc(max_x, y), curve.evaluate(x, y));
  EXPECT_DOUBLE_EQ(1129.0, curve.evaluate(x, y));

  // y < min_y
  x = 0.5;
  y = 5.0;
  EXPECT_DOUBLE_EQ(calc(x, min_y), curve.evaluate(x, y));
  EXPECT_DOUBLE_EQ(82.75, curve.evaluate(x, y));

  // y > max_y
  y = 40.0;
  EXPECT_DOUBLE_EQ(calc(x, max_y), curve.evaluate(x, y));
  EXPECT_DOUBLE_EQ(242.75, curve.evaluate(x, y));

  // x < min_x, y < min_y
  x = 0.05;
  y = 5.0;
  EXPECT_DOUBLE_EQ(calc(min_x, min_y), curve.evaluate(x, y));
  EXPECT_DOUBLE_EQ(46.83, curve.evaluate(x, y));

  // x > max_x, y > max_y
  x = 20.0;
  y = 50.0;
  EXPECT_DOUBLE_EQ(calc(max_x, max_y), curve.evaluate(x, y));
  EXPECT_DOUBLE_EQ(2224.0, curve.evaluate(x, y));

  // Set output limits
  double min_output = 50.0;
  double max_output = 1000.0;
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
