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
#include "DataFixture.hpp"

#include "../Vector.hpp"

using namespace std;
using namespace boost;
using namespace openstudio;

TEST_F(DataFixture, Vector_Constructor) {
  Vector v0(0);
  ASSERT_EQ(static_cast<unsigned int>(0), v0.size());

  Vector v1(1);
  ASSERT_EQ(static_cast<unsigned int>(1), v1.size());

  Vector v3(3);
  ASSERT_EQ(static_cast<unsigned int>(3), v3.size());
}

TEST_F(DataFixture, Vector_Interp) {
  Vector x(2);
  Vector y(2);

  x(0) = 0.0;
  x(1) = 1.0;
  y(0) = 1.0;
  y(1) = 2.0;

  EXPECT_DOUBLE_EQ(1.0, interp(x, y, 0.0, LinearInterp, NoneExtrap));
  EXPECT_DOUBLE_EQ(2.0, interp(x, y, 1.0, LinearInterp, NoneExtrap));
  EXPECT_DOUBLE_EQ(1.5, interp(x, y, 0.5, LinearInterp, NoneExtrap));
  EXPECT_DOUBLE_EQ(1.25, interp(x, y, 0.25, LinearInterp, NoneExtrap));
  EXPECT_DOUBLE_EQ(1.75, interp(x, y, 0.75, LinearInterp, NoneExtrap));
  EXPECT_DOUBLE_EQ(0.0, interp(x, y, -1.0, LinearInterp, NoneExtrap));
  EXPECT_DOUBLE_EQ(0.0, interp(x, y, -2.0, LinearInterp, NoneExtrap));

  EXPECT_DOUBLE_EQ(1.0, interp(x, y, 0.0, NearestInterp, NoneExtrap));
  EXPECT_DOUBLE_EQ(2.0, interp(x, y, 1.0, NearestInterp, NoneExtrap));
  EXPECT_DOUBLE_EQ(2.0, interp(x, y, 0.5, NearestInterp, NoneExtrap));
  EXPECT_DOUBLE_EQ(1.0, interp(x, y, 0.25, NearestInterp, NoneExtrap));
  EXPECT_DOUBLE_EQ(2.0, interp(x, y, 0.75, NearestInterp, NoneExtrap));
  EXPECT_DOUBLE_EQ(0.0, interp(x, y, -1.0, NearestInterp, NoneExtrap));
  EXPECT_DOUBLE_EQ(0.0, interp(x, y, -2.0, NearestInterp, NoneExtrap));

  EXPECT_DOUBLE_EQ(1.0, interp(x, y, 0.0, HoldLastInterp, NoneExtrap));
  EXPECT_DOUBLE_EQ(2.0, interp(x, y, 1.0, HoldLastInterp, NoneExtrap));
  EXPECT_DOUBLE_EQ(1.0, interp(x, y, 0.5, HoldLastInterp, NoneExtrap));
  EXPECT_DOUBLE_EQ(1.0, interp(x, y, 0.25, HoldLastInterp, NoneExtrap));
  EXPECT_DOUBLE_EQ(1.0, interp(x, y, 0.75, HoldLastInterp, NoneExtrap));
  EXPECT_DOUBLE_EQ(0.0, interp(x, y, -1.0, HoldLastInterp, NoneExtrap));
  EXPECT_DOUBLE_EQ(0.0, interp(x, y, -2.0, HoldLastInterp, NoneExtrap));

  EXPECT_DOUBLE_EQ(1.0, interp(x, y, 0.0, LinearInterp, NearestExtrap));
  EXPECT_DOUBLE_EQ(2.0, interp(x, y, 1.0, LinearInterp, NearestExtrap));
  EXPECT_DOUBLE_EQ(1.5, interp(x, y, 0.5, LinearInterp, NearestExtrap));
  EXPECT_DOUBLE_EQ(1.25, interp(x, y, 0.25, LinearInterp, NearestExtrap));
  EXPECT_DOUBLE_EQ(1.75, interp(x, y, 0.75, LinearInterp, NearestExtrap));
  EXPECT_DOUBLE_EQ(1.0, interp(x, y, -1.0, LinearInterp, NearestExtrap));
  EXPECT_DOUBLE_EQ(2.0, interp(x, y, 2.0, LinearInterp, NearestExtrap));
}

TEST_F(DataFixture, Vector_Linspace) {
  Vector vector = linspace(1, 3, 3);

  ASSERT_EQ(static_cast<unsigned int>(3), vector.size());
  EXPECT_DOUBLE_EQ(1, vector(0));
  EXPECT_DOUBLE_EQ(2, vector(1));
  EXPECT_DOUBLE_EQ(3, vector(2));
}

TEST(Vector, Logspace) {
  Vector vector = logspace(0, 2, 11);

  ASSERT_EQ(static_cast<unsigned int>(11), vector.size());
  EXPECT_NEAR(pow(10.0, 0.0), vector(0), 1e-6);
  EXPECT_NEAR(pow(10.0, 0.2), vector(1), 1e-6);
  EXPECT_NEAR(pow(10.0, 0.4), vector(2), 1e-6);
  EXPECT_NEAR(pow(10.0, 0.6), vector(3), 1e-6);
  EXPECT_NEAR(pow(10.0, 0.8), vector(4), 1e-6);
  EXPECT_NEAR(pow(10.0, 1.0), vector(5), 1e-6);
  EXPECT_NEAR(pow(10.0, 1.2), vector(6), 1e-6);
  EXPECT_NEAR(pow(10.0, 1.4), vector(7), 1e-6);
  EXPECT_NEAR(pow(10.0, 1.6), vector(8), 1e-6);
  EXPECT_NEAR(pow(10.0, 1.8), vector(9), 1e-6);
  EXPECT_NEAR(pow(10.0, 2.0), vector(10), 1e-6);
}

TEST_F(DataFixture, Vector_Log) {
  Vector vector(3);
  vector(0) = 1;
  vector(1) = 10;
  vector(2) = 100;

  Vector logVector = openstudio::log(vector, 10.0);

  ASSERT_EQ(vector.size(), logVector.size());
  EXPECT_DOUBLE_EQ(0, logVector(0));
  EXPECT_DOUBLE_EQ(1, logVector(1));
  EXPECT_DOUBLE_EQ(2, logVector(2));
}

TEST_F(DataFixture, Vector_Statistics_AllPositive) {
  Vector vector(8);

  // [2, 4, 4, 4, 5, 5, 7, 9]
  vector(0) = 2;
  vector(1) = 4;
  vector(2) = 4;
  vector(3) = 4;
  vector(4) = 5;
  vector(5) = 5;
  vector(6) = 7;
  vector(7) = 9;

  EXPECT_DOUBLE_EQ(40.0, sum(vector));
  EXPECT_DOUBLE_EQ(9.0, maximum(vector));
  EXPECT_DOUBLE_EQ(2.0, minimum(vector));
  EXPECT_DOUBLE_EQ(5.0, mean(vector));
  EXPECT_DOUBLE_EQ(4.0, variance(vector));
  EXPECT_DOUBLE_EQ(2.0, stdDev(vector));
}

TEST_F(DataFixture, Vector_Statistics_AllNegative) {
  Vector vector(8);

  // [-2, -4, -4, -4, -5, -5, -7, -9]
  vector(0) = -2;
  vector(1) = -4;
  vector(2) = -4;
  vector(3) = -4;
  vector(4) = -5;
  vector(5) = -5;
  vector(6) = -7;
  vector(7) = -9;

  EXPECT_DOUBLE_EQ(-40.0, sum(vector));
  EXPECT_DOUBLE_EQ(-2.0, maximum(vector));
  EXPECT_DOUBLE_EQ(-9.0, minimum(vector));
  EXPECT_DOUBLE_EQ(-5.0, mean(vector));
  EXPECT_DOUBLE_EQ(4.0, variance(vector));
  EXPECT_DOUBLE_EQ(2.0, stdDev(vector));
}

TEST_F(DataFixture, Vector_Statistics_MixedPositive) {
  Vector vector(8);

  // [-2, 0, 4, 4, 5, 5, 7, 9]
  vector(0) = -2;
  vector(1) = 0;
  vector(2) = 4;
  vector(3) = 4;
  vector(4) = 5;
  vector(5) = 5;
  vector(6) = 7;
  vector(7) = 9;

  EXPECT_DOUBLE_EQ(32.0, sum(vector));
  EXPECT_DOUBLE_EQ(9.0, maximum(vector));
  EXPECT_DOUBLE_EQ(-2.0, minimum(vector));
  EXPECT_DOUBLE_EQ(4.0, mean(vector));
  EXPECT_DOUBLE_EQ(11.0, variance(vector));
  EXPECT_DOUBLE_EQ(sqrt(11.0), stdDev(vector));
}
