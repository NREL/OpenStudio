/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>

#include "UnitsFixture.hpp"

#include "../ScaleFactory.hpp"

using openstudio::Scale;
using openstudio::ScaleFactory;
using openstudio::ScaleConstant;
using openstudio::ScaleOpReturnType;

using openstudio::peta;
using openstudio::tera;
using openstudio::giga;
using openstudio::mega;
using openstudio::kilo;
using openstudio::deka;
using openstudio::one;
using openstudio::deci;
using openstudio::centi;
using openstudio::milli;
using openstudio::micro;
using openstudio::nano;

TEST_F(UnitsFixture, ScaleFactory_RegisteredScales) {

  LOG(Debug, "ScaleFactory_RegisteredScales");

  // ETH@20100909 Using ScaleFactory instead of ScaleFactory in an attempt
  // to get ScaleFactory.hpp to show up in coverage analysis.
  std::vector<Scale> scales = ScaleFactory::instance().registeredScales();

  std::stringstream vectorOut;
  std::stringstream factoryOut;

  std::vector<Scale>::const_iterator scaleIter;
  auto scaleEnd = scales.end();
  for (scaleIter = scales.begin(); scaleIter != scaleEnd; ++scaleIter) {
    vectorOut << *scaleIter << '\n';
  }

  factoryOut << ScaleFactory::instance();
  std::string factoryOutFromInstance = factoryOut.str();

  EXPECT_FALSE(factoryOutFromInstance.empty());
  EXPECT_TRUE(factoryOutFromInstance == vectorOut.str());

  // print to log so can add formatting if desired
  LOG(Info, factoryOutFromInstance);

  factoryOut.str("");
  factoryOut << openstudio::printScales();
  EXPECT_EQ(factoryOutFromInstance, factoryOut.str());
}

// Create from abbreviaitons--needed for parsing quantities.
TEST_F(UnitsFixture, ScaleFactory_CreateFromAbbreviation) {

  LOG(Debug, "ScaleFactory_CreateFromAbbreviation");

  // in factory
  ScaleConstant s = ScaleFactory::instance().createScale("P");
  ASSERT_EQ(15, s().exponent);
  ASSERT_EQ("peta", s().name);

  // not in factory
  s = ScaleFactory::instance().createScale("A");
  testNumbersEqual(0.0, s().value);
}

// Tests arithmetic operations, which relies on the factory method for creating
// scales from exponents.
TEST_F(UnitsFixture, ScaleFactory_ScaleOperations) {

  LOG(Debug, "ScaleFactory_ScaleOperations");

  // MULTIPLICATION
  // exact match in factory
  ScaleOpReturnType scaleResult = kilo() * milli();
  ASSERT_EQ(scaleResult.first().exponent, 0);
  testNumbersEqual(1.0, scaleResult.second);
  // exact match not in factory
  scaleResult = deci() * micro();
  ASSERT_EQ(scaleResult.first().exponent, -6);
  testNumbersEqual(0.1, scaleResult.second);

  // DIVISION
  // exact match in factory
  scaleResult = giga() / mega();
  ASSERT_EQ(scaleResult.first().exponent, 3);
  testNumbersEqual(1.0, scaleResult.second);
  // exact match not in factory
  scaleResult = micro() / deka();
  ASSERT_EQ(scaleResult.first().exponent, -6);
  testNumbersEqual(0.1, scaleResult.second);

  // POWER
  // exact match in factory
  scaleResult = pow(kilo(), 3);
  ASSERT_EQ(scaleResult.first().exponent, 9);
  testNumbersEqual(1.0, scaleResult.second);
  scaleResult = pow(one(), 12);
  ASSERT_EQ(scaleResult.first().exponent, 0);
  testNumbersEqual(1.0, scaleResult.second);
  // exact match not in factory
  scaleResult = pow(centi(), -5);
  ASSERT_EQ(9, scaleResult.first().exponent);
  testNumbersEqual(10.0, scaleResult.second);
}
