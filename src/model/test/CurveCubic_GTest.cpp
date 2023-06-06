/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>

#include "ModelFixture.hpp"
#include "../CurveCubic.hpp"

#include <cmath>

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, CurveCubic_DefaultConstructors) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  ASSERT_EXIT(
    {
      Model m;
      CurveCubic curve(m);

      exit(0);
    },
    ::testing::ExitedWithCode(0), "");
}

TEST_F(ModelFixture, CurveCubic_GetterSetters_evaluate) {

  Model m;
  CurveCubic curve(m);

  EXPECT_EQ(0.0, curve.coefficient1Constant());
  EXPECT_EQ(0.0, curve.coefficient2x());
  EXPECT_EQ(0.0, curve.coefficient3xPOW2());
  EXPECT_EQ(1.0, curve.coefficient4xPOW3());

  EXPECT_EQ(0.0, curve.minimumValueofx());
  EXPECT_EQ(1.0, curve.maximumValueofx());

  double c1 = 1.0;
  double c2 = 2.0;
  double c3 = 3.0;
  double c4 = 4.0;
  double min_x = 0.1;
  double max_x = 3.0;

  auto calc = [c1, c2, c3, c4](double x) { return c1 + c2 * x + c3 * std::pow(x, 2) + c4 * std::pow(x, 3); };

  EXPECT_TRUE(curve.setCoefficient1Constant(c1));
  EXPECT_TRUE(curve.setCoefficient2x(c2));
  EXPECT_TRUE(curve.setCoefficient3xPOW2(c3));
  EXPECT_TRUE(curve.setCoefficient4xPOW3(c4));
  EXPECT_EQ(c1, curve.coefficient1Constant());
  EXPECT_EQ(c2, curve.coefficient2x());
  EXPECT_EQ(c3, curve.coefficient3xPOW2());
  EXPECT_EQ(c4, curve.coefficient4xPOW3());

  EXPECT_TRUE(curve.setMinimumValueofx(min_x));
  EXPECT_TRUE(curve.setMaximumValueofx(max_x));
  EXPECT_EQ(min_x, curve.minimumValueofx());
  EXPECT_EQ(max_x, curve.maximumValueofx());

  EXPECT_FALSE(curve.minimumCurveOutput());
  EXPECT_FALSE(curve.maximumCurveOutput());

  // X in range, no output limit
  double x = 0.5;
  EXPECT_DOUBLE_EQ(calc(x), curve.evaluate(x));
  EXPECT_DOUBLE_EQ(3.25, curve.evaluate(x));

  // x < min_x
  x = 0.05;
  EXPECT_DOUBLE_EQ(calc(min_x), curve.evaluate(x));
  EXPECT_DOUBLE_EQ(1.234, curve.evaluate(x));

  // x > max_x
  x = 20.0;
  EXPECT_DOUBLE_EQ(calc(max_x), curve.evaluate(x));
  EXPECT_DOUBLE_EQ(142, curve.evaluate(x));

  // Set output limits
  double min_output = 2.5;
  double max_output = 4.0;
  EXPECT_TRUE(curve.setMinimumCurveOutput(min_output));
  EXPECT_TRUE(curve.setMaximumCurveOutput(max_output));

  ASSERT_TRUE(curve.minimumCurveOutput());
  ASSERT_TRUE(curve.maximumCurveOutput());
  EXPECT_EQ(min_output, curve.minimumCurveOutput().get());
  EXPECT_EQ(max_output, curve.maximumCurveOutput().get());

  // 1.234 < min output
  EXPECT_DOUBLE_EQ(min_output, curve.evaluate(min_x));
  // 142 > max output
  EXPECT_DOUBLE_EQ(max_output, curve.evaluate(max_x));

  // Wrong number of arguments
  // EXPECT_THROW(curve.evaluate(1.0, 2.0), openstudio::Exception);
  // EXPECT_THROW(curve.evaluate(1.0, 2.0, 3.0), openstudio::Exception);
}
