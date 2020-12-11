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
#include "../CurveTriquadratic.hpp"

#include <cmath>

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, CurveTriquadratic_DefaultConstructors) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  ASSERT_EXIT(
    {
      Model m;
      CurveTriquadratic curve(m);

      exit(0);
    },
    ::testing::ExitedWithCode(0), "");
}

TEST_F(ModelFixture, CurveTriquadratic_GetterSetters_evaluate) {

  Model m;
  CurveTriquadratic curve(m);

  EXPECT_FALSE(curve.coefficient1Constant());
  EXPECT_FALSE(curve.coefficient2xPOW2());
  EXPECT_FALSE(curve.coefficient3x());
  EXPECT_FALSE(curve.coefficient4yPOW2());
  EXPECT_FALSE(curve.coefficient5y());
  EXPECT_FALSE(curve.coefficient6zPOW2());
  EXPECT_FALSE(curve.coefficient7z());
  EXPECT_FALSE(curve.coefficient8xPOW2TIMESYPOW2());
  EXPECT_FALSE(curve.coefficient9xTIMESY());
  EXPECT_FALSE(curve.coefficient10xTIMESYPOW2());
  EXPECT_FALSE(curve.coefficient11xPOW2TIMESY());
  EXPECT_FALSE(curve.coefficient12xPOW2TIMESZPOW2());
  EXPECT_FALSE(curve.coefficient13xTIMESZ());
  EXPECT_FALSE(curve.coefficient14xTIMESZPOW2());
  EXPECT_FALSE(curve.coefficient15xPOW2TIMESZ());
  EXPECT_FALSE(curve.coefficient16yPOW2TIMESZPOW2());
  EXPECT_FALSE(curve.coefficient17yTIMESZ());
  EXPECT_FALSE(curve.coefficient18yTIMESZPOW2());
  EXPECT_FALSE(curve.coefficient19yPOW2TIMESZ());
  EXPECT_FALSE(curve.coefficient20xPOW2TIMESYPOW2TIMESZPOW2());
  EXPECT_FALSE(curve.coefficient21xPOW2TIMESYPOW2TIMESZ());
  EXPECT_FALSE(curve.coefficient22xPOW2TIMESYTIMESZPOW2());
  EXPECT_FALSE(curve.coefficient23xTIMESYPOW2TIMESZPOW2());
  EXPECT_FALSE(curve.coefficient24xPOW2TIMESYTIMESZ());
  EXPECT_FALSE(curve.coefficient25xTIMESYPOW2TIMESZ());
  EXPECT_FALSE(curve.coefficient26xTIMESYTIMESZPOW2());
  EXPECT_FALSE(curve.coefficient27xTIMESYTIMESZ());

  EXPECT_FALSE(curve.minimumValueofx());
  EXPECT_FALSE(curve.maximumValueofx());
  EXPECT_FALSE(curve.minimumValueofy());
  EXPECT_FALSE(curve.maximumValueofy());
  EXPECT_FALSE(curve.minimumValueofz());
  EXPECT_FALSE(curve.maximumValueofz());

  double c1 = 1.0;
  double c2 = 2.0;
  double c3 = 3.0;
  double c4 = 4.0;
  double c5 = 5.0;
  double c6 = 6.0;
  double c7 = 7.0;
  double c8 = 8.0;
  double c9 = 9.0;
  double c10 = 10.0;
  double c11 = 11.0;
  double c12 = 12.0;
  double c13 = 13.0;
  double c14 = 14.0;
  double c15 = 15.0;
  double c16 = 16.0;
  double c17 = 17.0;
  double c18 = 18.0;
  double c19 = 19.0;
  double c20 = 20.0;
  double c21 = 21.0;
  double c22 = 22.0;
  double c23 = 23.0;
  double c24 = 24.0;
  double c25 = 25.0;
  double c26 = 26.0;
  double c27 = 27.0;

  double min_x = 0.1;
  double max_x = 3.0;

  double min_y = 4.0;
  double max_y = 6.0;

  double min_z = 8.0;
  double max_z = 10.0;

  auto calc = [=](double x, double y, double z) {
    return c1 + c2 * std::pow(x, 2) + c3 * x + c4 * std::pow(y, 2) + c5 * y + c6 * std::pow(z, 2) + c7 * z + c8 * std::pow(x, 2) * std::pow(y, 2)
           + c9 * x * y + c10 * x * std::pow(y, 2) + c11 * std::pow(x, 2) * y + c12 * std::pow(x, 2) * std::pow(z, 2) + c13 * x * z
           + c14 * x * std::pow(z, 2) + c15 * std::pow(x, 2) * z + c16 * std::pow(y, 2) * std::pow(z, 2) + c17 * y * z + c18 * y * std::pow(z, 2)
           + c19 * std::pow(y, 2) * z + c20 * std::pow(x, 2) * std::pow(y, 2) * std::pow(z, 2) + c21 * std::pow(x, 2) * std::pow(y, 2) * z
           + c22 * std::pow(x, 2) * y * std::pow(z, 2) + c23 * x * std::pow(y, 2) * std::pow(z, 2) + c24 * std::pow(x, 2) * y * z
           + c25 * x * std::pow(y, 2) * z + c26 * x * y * std::pow(z, 2) + c27 * x * y * z;
  };

  EXPECT_TRUE(curve.setCoefficient1Constant(c1));
  EXPECT_TRUE(curve.setCoefficient2xPOW2(c2));
  EXPECT_TRUE(curve.setCoefficient3x(c3));
  EXPECT_TRUE(curve.setCoefficient4yPOW2(c4));
  EXPECT_TRUE(curve.setCoefficient5y(c5));
  EXPECT_TRUE(curve.setCoefficient6zPOW2(c6));
  EXPECT_TRUE(curve.setCoefficient7z(c7));
  EXPECT_TRUE(curve.setCoefficient8xPOW2TIMESYPOW2(c8));
  EXPECT_TRUE(curve.setCoefficient9xTIMESY(c9));
  EXPECT_TRUE(curve.setCoefficient10xTIMESYPOW2(c10));
  EXPECT_TRUE(curve.setCoefficient11xPOW2TIMESY(c11));
  EXPECT_TRUE(curve.setCoefficient12xPOW2TIMESZPOW2(c12));
  EXPECT_TRUE(curve.setCoefficient13xTIMESZ(c13));
  EXPECT_TRUE(curve.setCoefficient14xTIMESZPOW2(c14));
  EXPECT_TRUE(curve.setCoefficient15xPOW2TIMESZ(c15));
  EXPECT_TRUE(curve.setCoefficient16yPOW2TIMESZPOW2(c16));
  EXPECT_TRUE(curve.setCoefficient17yTIMESZ(c17));
  EXPECT_TRUE(curve.setCoefficient18yTIMESZPOW2(c18));
  EXPECT_TRUE(curve.setCoefficient19yPOW2TIMESZ(c19));
  EXPECT_TRUE(curve.setCoefficient20xPOW2TIMESYPOW2TIMESZPOW2(c20));
  EXPECT_TRUE(curve.setCoefficient21xPOW2TIMESYPOW2TIMESZ(c21));
  EXPECT_TRUE(curve.setCoefficient22xPOW2TIMESYTIMESZPOW2(c22));
  EXPECT_TRUE(curve.setCoefficient23xTIMESYPOW2TIMESZPOW2(c23));
  EXPECT_TRUE(curve.setCoefficient24xPOW2TIMESYTIMESZ(c24));
  EXPECT_TRUE(curve.setCoefficient25xTIMESYPOW2TIMESZ(c25));
  EXPECT_TRUE(curve.setCoefficient26xTIMESYTIMESZPOW2(c26));
  EXPECT_TRUE(curve.setCoefficient27xTIMESYTIMESZ(c27));

  ASSERT_TRUE(curve.coefficient1Constant());
  ASSERT_TRUE(curve.coefficient2xPOW2());
  ASSERT_TRUE(curve.coefficient3x());
  ASSERT_TRUE(curve.coefficient4yPOW2());
  ASSERT_TRUE(curve.coefficient5y());
  ASSERT_TRUE(curve.coefficient6zPOW2());
  ASSERT_TRUE(curve.coefficient7z());
  ASSERT_TRUE(curve.coefficient8xPOW2TIMESYPOW2());
  ASSERT_TRUE(curve.coefficient9xTIMESY());
  ASSERT_TRUE(curve.coefficient10xTIMESYPOW2());
  ASSERT_TRUE(curve.coefficient11xPOW2TIMESY());
  ASSERT_TRUE(curve.coefficient12xPOW2TIMESZPOW2());
  ASSERT_TRUE(curve.coefficient13xTIMESZ());
  ASSERT_TRUE(curve.coefficient14xTIMESZPOW2());
  ASSERT_TRUE(curve.coefficient15xPOW2TIMESZ());
  ASSERT_TRUE(curve.coefficient16yPOW2TIMESZPOW2());
  ASSERT_TRUE(curve.coefficient17yTIMESZ());
  ASSERT_TRUE(curve.coefficient18yTIMESZPOW2());
  ASSERT_TRUE(curve.coefficient19yPOW2TIMESZ());
  ASSERT_TRUE(curve.coefficient20xPOW2TIMESYPOW2TIMESZPOW2());
  ASSERT_TRUE(curve.coefficient21xPOW2TIMESYPOW2TIMESZ());
  ASSERT_TRUE(curve.coefficient22xPOW2TIMESYTIMESZPOW2());
  ASSERT_TRUE(curve.coefficient23xTIMESYPOW2TIMESZPOW2());
  ASSERT_TRUE(curve.coefficient24xPOW2TIMESYTIMESZ());
  ASSERT_TRUE(curve.coefficient25xTIMESYPOW2TIMESZ());
  ASSERT_TRUE(curve.coefficient26xTIMESYTIMESZPOW2());
  ASSERT_TRUE(curve.coefficient27xTIMESYTIMESZ());

  EXPECT_EQ(c1, curve.coefficient1Constant().get());
  EXPECT_EQ(c2, curve.coefficient2xPOW2().get());
  EXPECT_EQ(c3, curve.coefficient3x().get());
  EXPECT_EQ(c4, curve.coefficient4yPOW2().get());
  EXPECT_EQ(c5, curve.coefficient5y().get());
  EXPECT_EQ(c6, curve.coefficient6zPOW2().get());
  EXPECT_EQ(c7, curve.coefficient7z().get());
  EXPECT_EQ(c8, curve.coefficient8xPOW2TIMESYPOW2().get());
  EXPECT_EQ(c9, curve.coefficient9xTIMESY().get());
  EXPECT_EQ(c10, curve.coefficient10xTIMESYPOW2().get());
  EXPECT_EQ(c11, curve.coefficient11xPOW2TIMESY().get());
  EXPECT_EQ(c12, curve.coefficient12xPOW2TIMESZPOW2().get());
  EXPECT_EQ(c13, curve.coefficient13xTIMESZ().get());
  EXPECT_EQ(c14, curve.coefficient14xTIMESZPOW2().get());
  EXPECT_EQ(c15, curve.coefficient15xPOW2TIMESZ().get());
  EXPECT_EQ(c16, curve.coefficient16yPOW2TIMESZPOW2().get());
  EXPECT_EQ(c17, curve.coefficient17yTIMESZ().get());
  EXPECT_EQ(c18, curve.coefficient18yTIMESZPOW2().get());
  EXPECT_EQ(c19, curve.coefficient19yPOW2TIMESZ().get());
  EXPECT_EQ(c20, curve.coefficient20xPOW2TIMESYPOW2TIMESZPOW2().get());
  EXPECT_EQ(c21, curve.coefficient21xPOW2TIMESYPOW2TIMESZ().get());
  EXPECT_EQ(c22, curve.coefficient22xPOW2TIMESYTIMESZPOW2().get());
  EXPECT_EQ(c23, curve.coefficient23xTIMESYPOW2TIMESZPOW2().get());
  EXPECT_EQ(c24, curve.coefficient24xPOW2TIMESYTIMESZ().get());
  EXPECT_EQ(c25, curve.coefficient25xTIMESYPOW2TIMESZ().get());
  EXPECT_EQ(c26, curve.coefficient26xTIMESYTIMESZPOW2().get());
  EXPECT_EQ(c27, curve.coefficient27xTIMESYTIMESZ().get());

  // Lims
  EXPECT_TRUE(curve.setMinimumValueofx(min_x));
  EXPECT_TRUE(curve.setMaximumValueofx(max_x));
  ASSERT_TRUE(curve.minimumValueofx());
  ASSERT_TRUE(curve.maximumValueofx());
  EXPECT_EQ(min_x, curve.minimumValueofx().get());
  EXPECT_EQ(max_x, curve.maximumValueofx().get());

  EXPECT_TRUE(curve.setMinimumValueofy(min_y));
  EXPECT_TRUE(curve.setMaximumValueofy(max_y));
  ASSERT_TRUE(curve.minimumValueofy());
  ASSERT_TRUE(curve.maximumValueofy());
  EXPECT_EQ(min_y, curve.minimumValueofy().get());
  EXPECT_EQ(max_y, curve.maximumValueofy().get());

  EXPECT_TRUE(curve.setMinimumValueofz(min_z));
  EXPECT_TRUE(curve.setMaximumValueofz(max_z));
  ASSERT_TRUE(curve.minimumValueofz());
  ASSERT_TRUE(curve.maximumValueofz());
  EXPECT_EQ(min_z, curve.minimumValueofz().get());
  EXPECT_EQ(max_z, curve.maximumValueofz().get());

  EXPECT_FALSE(curve.minimumCurveOutput());
  EXPECT_FALSE(curve.maximumCurveOutput());

  // x, y and z in range, no output limit
  double x = 0.5;
  double y = 5.0;
  double z = 9.0;
  EXPECT_DOUBLE_EQ(calc(x, y, z), curve.evaluate(x, y, z));
  EXPECT_DOUBLE_EQ(92296.75, curve.evaluate(x, y, z));

  // x < min_x
  x = 0.05;
  EXPECT_DOUBLE_EQ(calc(min_x, y, z), curve.evaluate(x, y, z));
  EXPECT_DOUBLE_EQ(52520.19, curve.evaluate(x, y, z));

  // x > max_x
  x = 20.0;
  EXPECT_DOUBLE_EQ(calc(max_x, y, z), curve.evaluate(x, y, z));
  EXPECT_DOUBLE_EQ(751098.0, curve.evaluate(x, y, z));

  // y < min_y
  x = 0.5;
  y = 3.5;
  EXPECT_DOUBLE_EQ(calc(x, min_y, z), curve.evaluate(x, y, z));
  EXPECT_DOUBLE_EQ(62231.25, curve.evaluate(x, y, z));

  // y > max_y
  y = 40.0;
  EXPECT_DOUBLE_EQ(calc(x, max_y, z), curve.evaluate(x, y, z));
  EXPECT_DOUBLE_EQ(128310.75, curve.evaluate(x, y, z));

  // z < min_z
  x = 0.5;
  y = 5.0;
  z = 3.0;
  EXPECT_DOUBLE_EQ(calc(x, y, min_z), curve.evaluate(x, y, z));
  EXPECT_DOUBLE_EQ(73991.25, curve.evaluate(x, y, z));

  // z > max_z
  z = 40.0;
  EXPECT_DOUBLE_EQ(calc(x, y, max_z), curve.evaluate(x, y, z));
  EXPECT_DOUBLE_EQ(112624.25, curve.evaluate(x, y, z));

  // x < min_x, y < min_y, z < min_z
  x = -5.0;
  y = -5.0;
  z = -5.0;
  EXPECT_DOUBLE_EQ(calc(min_x, min_y, min_z), curve.evaluate(x, y, z));
  EXPECT_DOUBLE_EQ(28346.4, curve.evaluate(x, y, z));

  // x > max_x, y, z > max_y
  x = 10.0;
  y = 10.0;
  z = 10.0;
  EXPECT_DOUBLE_EQ(calc(max_x, max_y, max_z), curve.evaluate(x, y, z));
  EXPECT_DOUBLE_EQ(1273160.0, curve.evaluate(x, y, z));

  // Set output limits
  double min_output = 40000.0;
  double max_output = 100000.0;
  EXPECT_TRUE(curve.setMinimumCurveOutput(min_output));
  EXPECT_TRUE(curve.setMaximumCurveOutput(max_output));

  ASSERT_TRUE(curve.minimumCurveOutput());
  ASSERT_TRUE(curve.maximumCurveOutput());
  EXPECT_EQ(min_output, curve.minimumCurveOutput().get());
  EXPECT_EQ(max_output, curve.maximumCurveOutput().get());

  // out < min output
  EXPECT_DOUBLE_EQ(min_output, curve.evaluate(min_x, min_y, min_z));
  // out > max output
  EXPECT_DOUBLE_EQ(max_output, curve.evaluate(max_x, max_y, max_z));

  // Wrong number of arguments
  // EXPECT_THROW(curve.evaluate(1.0), openstudio::Exception);
  // EXPECT_THROW(curve.evaluate(1.0, 2.0), openstudio::Exception);
}
