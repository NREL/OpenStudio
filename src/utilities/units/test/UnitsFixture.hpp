/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2023, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

#ifndef UTILITIES_UNITS_TEST_UNITSFIXTURE_HPP
#define UTILITIES_UNITS_TEST_UNITSFIXTURE_HPP

#include <gtest/gtest.h>

#include "../../core/String.hpp"
#include "../../core/Logger.hpp"
#include "../../core/FileLogSink.hpp"

#include "../Quantity.hpp"
#include "../OSQuantityVector.hpp"

#include <iomanip>

class UnitsFixture : public ::testing::Test
{
 protected:
  // initialize for each test
  virtual void SetUp() override;

  // tear down after for each test
  virtual void TearDown() override;

  // initialize static members
  static void SetUpTestSuite();

  // tear down static members
  static void TearDownTestSuite();

  // set up logging
  REGISTER_LOGGER("UnitsFixture");

  static boost::optional<openstudio::FileLogSink> logFile;
  static double tol;

  static std::vector<openstudio::Quantity> testQuantityVector;
  static openstudio::OSQuantityVector testOSQuantityVector;
};

// compare floating point numbers. absolute error is compared against tol^2.5
// if the magnitude of expected is less than tol^2. otherwise relative error is
// compared against tol.
template <class T>
void testNumbersEqual(T expected, T actual, T tol = 1E-5) {

  T absoluteError = std::abs(expected - actual);
  T magnitudeOfExpected = std::abs(expected);

  if (magnitudeOfExpected < std::pow(tol, 2)) {
    EXPECT_TRUE(absoluteError < std::pow(tol, 2.5));
    LOG_FREE(Debug, "UnitsFixture",
             "Expected: " << expected << " Actual: " << actual << " Absolute error: " << absoluteError << " Tolerance: " << std::pow(tol, 2.5)
                          << " Test result: " << std::boolalpha << (absoluteError < std::pow(tol, 2.5)));
  } else {
    T relativeError = absoluteError / magnitudeOfExpected;
    EXPECT_TRUE(relativeError < tol);
    LOG_FREE(Debug, "UnitsFixture",
             "Expected: " << expected << " Actual: " << actual << " Relative error: " << relativeError << " Tolerance: " << tol
                          << " Test result: " << std::boolalpha << (relativeError < tol));
  }
}

template <class T>
void testStreamOutput(const std::string& expected, const T& outObject, int precision = -1) {
  std::stringstream testStream;
  if (precision < 0) {
    testStream << outObject;
  } else {
    testStream << std::fixed << std::setprecision(precision) << outObject;
  }
  EXPECT_EQ(expected, testStream.str());
}

#endif  // UTILITIES_UNITS_TEST_UNITSFIXTURE_HPP
