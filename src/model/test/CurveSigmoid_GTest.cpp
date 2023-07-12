/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>

#include "ModelFixture.hpp"
#include "../CurveSigmoid.hpp"

#include <cmath>

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, CurveSigmoid_DefaultConstructors) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  ASSERT_EXIT(
    {
      Model m;
      CurveSigmoid curve(m);

      exit(0);
    },
    ::testing::ExitedWithCode(0), "");
}

TEST_F(ModelFixture, CurveSigmoid_GetterSetters_evaluate) {

  Model m;
  CurveSigmoid curve(m);

  EXPECT_EQ(0.0, curve.coefficient1C1());
  EXPECT_EQ(1.0, curve.coefficient2C2());
  EXPECT_EQ(1.0, curve.coefficient3C3());
  EXPECT_EQ(1.0, curve.coefficient4C4());
  EXPECT_EQ(1.0, curve.coefficient5C5());

  EXPECT_EQ(0.0, curve.minimumValueofx());
  EXPECT_EQ(1.0, curve.maximumValueofx());

  double c1 = 1.0;
  double c2 = 2.0;
  double c3 = 3.0;
  double c4 = 4.0;
  double c5 = 5.0;
  double min_x = -10;
  double max_x = 10;

  auto calc = [c1, c2, c3, c4, c5](double x) {
    double curveValueExp = std::exp((c3 - x) / c4);
    return c1 + c2 / std::pow(1.0 + curveValueExp, c5);
  };

  EXPECT_TRUE(curve.setCoefficient1C1(c1));
  EXPECT_TRUE(curve.setCoefficient2C2(c2));
  EXPECT_TRUE(curve.setCoefficient3C3(c3));
  EXPECT_TRUE(curve.setCoefficient4C4(c4));
  EXPECT_TRUE(curve.setCoefficient5C5(c5));

  EXPECT_EQ(c1, curve.coefficient1C1());
  EXPECT_EQ(c2, curve.coefficient2C2());
  EXPECT_EQ(c3, curve.coefficient3C3());
  EXPECT_EQ(c4, curve.coefficient4C4());
  EXPECT_EQ(c5, curve.coefficient5C5());

  EXPECT_TRUE(curve.setMinimumValueofx(min_x));
  EXPECT_TRUE(curve.setMaximumValueofx(max_x));
  EXPECT_EQ(min_x, curve.minimumValueofx());
  EXPECT_EQ(max_x, curve.maximumValueofx());

  EXPECT_FALSE(curve.minimumCurveOutput());
  EXPECT_FALSE(curve.maximumCurveOutput());

  // X in range, no output limit
  double x = 5;
  EXPECT_DOUBLE_EQ(calc(x), curve.evaluate(x));
  EXPECT_DOUBLE_EQ(1.1868895010341447, curve.evaluate(x));

  // x < min_x
  x = -20.0;
  EXPECT_DOUBLE_EQ(calc(min_x), curve.evaluate(x));
  EXPECT_DOUBLE_EQ(1.0000001449235205, curve.evaluate(x));

  // x > max_x
  x = 20.0;
  EXPECT_DOUBLE_EQ(calc(max_x), curve.evaluate(x));
  EXPECT_DOUBLE_EQ(1.8976513195777158, curve.evaluate(x));

  // Set output limits
  double min_output = 1.1;
  double max_output = 1.5;
  EXPECT_TRUE(curve.setMinimumCurveOutput(min_output));
  EXPECT_TRUE(curve.setMaximumCurveOutput(max_output));

  ASSERT_TRUE(curve.minimumCurveOutput());
  ASSERT_TRUE(curve.maximumCurveOutput());
  EXPECT_EQ(min_output, curve.minimumCurveOutput().get());
  EXPECT_EQ(max_output, curve.maximumCurveOutput().get());

  // out < min output
  EXPECT_DOUBLE_EQ(min_output, curve.evaluate(min_x));
  // out > max output
  EXPECT_DOUBLE_EQ(max_output, curve.evaluate(max_x));

  // Wrong number of arguments
  // EXPECT_THROW(curve.evaluate(1.0, 2.0), openstudio::Exception);
  // EXPECT_THROW(curve.evaluate(1.0, 2.0, 3.0), openstudio::Exception);
}
