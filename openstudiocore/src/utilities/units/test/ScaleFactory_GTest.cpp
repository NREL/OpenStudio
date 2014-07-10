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
using openstudio::one;
using openstudio::centi;
using openstudio::milli;
using openstudio::micro;
using openstudio::nano;

TEST_F(UnitsFixture,ScaleFactory_RegisteredScales) {

  LOG(Debug,"ScaleFactory_RegisteredScales");

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
  LOG(Info,factoryOutFromInstance);

  factoryOut.str("");
  factoryOut << openstudio::printScales();
  EXPECT_EQ(factoryOutFromInstance,factoryOut.str());

}

// Create from abbreviaitons--needed for parsing quantities.
TEST_F(UnitsFixture,ScaleFactory_CreateFromAbbreviation) {

  LOG(Debug,"ScaleFactory_CreateFromAbbreviation");

  // in factory
  ScaleConstant s = ScaleFactory::instance().createScale("P");
  ASSERT_EQ(15,s().exponent);
  ASSERT_EQ("peta",s().name);

  // not in factory
  s = ScaleFactory::instance().createScale("A");
  testNumbersEqual(0.0,s().value);

}

// Tests arithmetic operations, which relies on the factory method for creating
// scales from exponents.
TEST_F(UnitsFixture,ScaleFactory_ScaleOperations) {

  LOG(Debug,"ScaleFactory_ScaleOperations");

  // MULTIPLICATION
  // exact match in factory
  ScaleOpReturnType scaleResult = kilo()*milli();
  ASSERT_EQ(scaleResult.first().exponent,0);
  testNumbersEqual(1.0,scaleResult.second);
  // exact match not in factory
  scaleResult = centi()*kilo();
  ASSERT_EQ(scaleResult.first().exponent,0);
  testNumbersEqual(10.0,scaleResult.second);

  // DIVISION
  // exact match in factory
  scaleResult = giga()/mega();
  ASSERT_EQ(scaleResult.first().exponent,3);
  testNumbersEqual(1.0,scaleResult.second);
  // exact match not in factory
  scaleResult = micro()/centi();
  ASSERT_EQ(scaleResult.first().exponent,-3);
  testNumbersEqual(0.1,scaleResult.second);

  // POWER
  // exact match in factory
  scaleResult = pow(kilo(),3);
  ASSERT_EQ(scaleResult.first().exponent,9);
  testNumbersEqual(1.0,scaleResult.second);
  scaleResult = pow(one(),12);
  ASSERT_EQ(scaleResult.first().exponent,0);
  testNumbersEqual(1.0,scaleResult.second);
  // exact match not in factory
  scaleResult = pow(centi(),-2);
  ASSERT_EQ(3,scaleResult.first().exponent);
  testNumbersEqual(10.0,scaleResult.second);

}
