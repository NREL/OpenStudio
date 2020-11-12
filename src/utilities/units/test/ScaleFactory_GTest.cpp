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
  std::vector<Scale>::const_iterator scaleEnd = scales.end();
  for (scaleIter = scales.begin(); scaleIter != scaleEnd; ++scaleIter) {
    vectorOut << *scaleIter << std::endl;
  }

  factoryOut << ScaleFactory::instance();
  std::string factoryOutFromInstance = factoryOut.str();

  EXPECT_FALSE(factoryOutFromInstance == "");
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
