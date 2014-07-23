/**********************************************************************
*  Copyright (c) 2008-2014, Alliance for Sustainable Energy.
*  All rights reserved.
*
*  This library is free software; you can redistribute it and/or
*  modify it under the terms of the GNU Lesser General Public
*  License as published by the Free Software Foundation; either
*  version 2.1 of the License, or (at your option) any later version.
*
*  This library is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
*  Lesser General Public License for more details.
*
*  You should have received a copy of the GNU Lesser General Public
*  License along with this library; if not, write to the Free Software
*  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
**********************************************************************/

#ifndef UTILITIES_UNITS_TEST_UNITSFIXTURE_HPP
#define UTILITIES_UNITS_TEST_UNITSFIXTURE_HPP

#include <gtest/gtest.h>

#include "../../core/String.hpp"
#include "../../core/Logger.hpp"
#include "../../core/FileLogSink.hpp"

#include "../Quantity.hpp"
#include "../OSQuantityVector.hpp"

#include <iomanip>

class UnitsFixture : public ::testing::Test {
protected:

  // initialize for each test
  virtual void SetUp();

  // tear down after for each test
  virtual void TearDown();

  // initialize static members
  static void SetUpTestCase();

  // tear down static members
  static void TearDownTestCase();

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
template<class T>
void testNumbersEqual(T expected,T actual,T tol=1E-5) {

  T absoluteError = std::abs(expected - actual);
  T magnitudeOfExpected = std::abs(expected);

  if (magnitudeOfExpected < std::pow(tol,2)) {
    EXPECT_TRUE(absoluteError < std::pow(tol,2.5));
    LOG_FREE(Debug,"UnitsFixture","Expected: " << expected << " Actual: "
      << actual << " Absolute error: " << absoluteError << " Tolerance: "
      << std::pow(tol,2.5) << " Test result: " << std::boolalpha
      << (absoluteError < std::pow(tol,2.5)));
  }
  else {
    T relativeError = absoluteError/magnitudeOfExpected;
    EXPECT_TRUE(relativeError < tol);
    LOG_FREE(Debug,"UnitsFixture","Expected: " << expected << " Actual: "
      << actual << " Relative error: " << relativeError<< " Tolerance: " << tol
      << " Test result: " << std::boolalpha << (relativeError < tol));
  }

}

template<class T>
void testStreamOutput(const std::string& expected,const T& outObject,int precision=-1) {
  std::stringstream testStream;
  if (precision < 0) {
    testStream << outObject;
  }
  else {
    testStream << std::fixed << std::setprecision(precision) << outObject;
  }
  EXPECT_EQ(expected,testStream.str());
}

#endif // UTILITIES_UNITS_TEST_UNITSFIXTURE_HPP
