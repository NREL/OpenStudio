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
