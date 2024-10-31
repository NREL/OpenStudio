/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>
#include "UnitsFixture.hpp"

#include "../OSQuantityVector.hpp"
#include "../Quantity.hpp"
#include "../SIUnit.hpp"
#include "../WhUnit.hpp"
#include "../CelsiusUnit.hpp"

#include "../../data/Vector.hpp"

#include "../../core/Containers.hpp"

using namespace openstudio;

TEST_F(UnitsFixture, OSQuantityVector_Constructors) {
  SIUnit u = createSILength();
  OSQuantityVector qvec(u);
  EXPECT_TRUE(qvec.empty());
  qvec.push_back(Quantity(1.0, u));
  qvec.push_back(Quantity(5.5, u));
  EXPECT_ANY_THROW(qvec.push_back(Quantity(1.0, createSIPower())));
  EXPECT_EQ(2u, qvec.size());
  EXPECT_FALSE(qvec.empty());
  qvec.resize(5u, 3.0);
  EXPECT_EQ(5u, qvec.size());
  QuantityVector qs = qvec.quantities();
  ASSERT_EQ(5u, qs.size());
  EXPECT_EQ(3.0, qs[3].value());
  EXPECT_EQ("m", qs[3].standardUnitsString());

  qvec = OSQuantityVector(createSIPower(), 50u, 100.0);
  EXPECT_EQ("W", qvec.units().prettyString());
  qs = qvec.quantities();
  ASSERT_EQ(50u, qs.size());
  EXPECT_DOUBLE_EQ(100.0, qs[31].value());
  EXPECT_EQ("W", qs[25].prettyUnitsString());
  EXPECT_EQ("kg*m^2/s^3", qs[2].standardUnitsString());

  OSQuantityVector qvec2(createSIPower(), DoubleVector(50u, 100.0));
  EXPECT_TRUE(qvec2 == qvec);
}

TEST_F(UnitsFixture, OSQuantityVector_MathematicalOperators) {
  // basic addition
  OSQuantityVector testVec1(createSIEnergyUseIntensity(), 2u, 3.5);
  OSQuantityVector testVec2(createSIEnergyUseIntensity(), 2u, 0.25);
  Quantity testQ1(3.5, createSIEnergyUseIntensity());
  Quantity testQ2(0.25, createSIEnergyUseIntensity());

  OSQuantityVector resultVec(testVec1);
  resultVec += resultVec;
  Quantity resultQ(testQ1);
  resultQ += resultQ;
  ASSERT_EQ(2u, resultVec.size());
  EXPECT_EQ(resultQ, resultVec.getQuantity(0));
  EXPECT_EQ(resultQ, resultVec.getQuantity(1));

  resultVec = testVec1 + testVec2;
  resultQ = testQ1 + testQ2;
  ASSERT_EQ(2u, resultVec.size());
  EXPECT_EQ(resultQ, resultVec.getQuantity(0));
  EXPECT_EQ(resultQ, resultVec.getQuantity(1));

  resultVec = testVec1 + testQ2;
  ASSERT_EQ(2u, resultVec.size());
  EXPECT_EQ(resultQ, resultVec.getQuantity(0));
  EXPECT_EQ(resultQ, resultVec.getQuantity(1));

  // basic subtraction
  resultVec = testVec1;
  resultVec -= 2 * resultVec;
  resultQ = testQ1;
  resultQ -= 2 * resultQ;
  ASSERT_EQ(2u, resultVec.size());
  EXPECT_EQ(resultQ, resultVec.getQuantity(0));
  EXPECT_EQ(resultQ, resultVec.getQuantity(1));
  EXPECT_EQ(-3.5, resultQ.value());

  resultVec = testVec1 - testVec2;
  resultQ = testQ1 - testQ2;
  ASSERT_EQ(2u, resultVec.size());
  EXPECT_EQ(resultQ, resultVec.getQuantity(0));
  EXPECT_EQ(resultQ, resultVec.getQuantity(1));

  resultVec = testVec1 - testQ2;
  ASSERT_EQ(2u, resultVec.size());
  EXPECT_EQ(resultQ, resultVec.getQuantity(0));
  EXPECT_EQ(resultQ, resultVec.getQuantity(1));

  // add temperatures
  testVec1 = OSQuantityVector(createCelsiusTemperature(), 2u, 20.0);
  testVec2 = OSQuantityVector(createCelsiusTemperature(), 2u, 5.0);
  EXPECT_TRUE(testVec1.isAbsolute());
  EXPECT_FALSE(testVec1.isRelative());
  testVec2.setAsRelative();
  EXPECT_FALSE(testVec2.isAbsolute());
  EXPECT_TRUE(testVec2.isRelative());
  testQ1 = Quantity(20.0, createCelsiusTemperature());
  testQ2 = Quantity(5.0, createCelsiusTemperature());
  testQ2.setAsRelative();

  resultVec = testVec1 + testVec2;
  resultQ = testQ1 + testQ2;
  ASSERT_EQ(2u, resultVec.size());
  EXPECT_EQ(resultQ, resultVec.getQuantity(0));
  EXPECT_EQ(resultQ, resultVec.getQuantity(1));
  EXPECT_TRUE(resultVec.isAbsolute());
  EXPECT_TRUE(resultQ.isAbsolute());

  resultVec = testVec2 + testVec1;
  resultQ = testQ2 + testQ1;
  ASSERT_EQ(2u, resultVec.size());
  EXPECT_EQ(resultQ, resultVec.getQuantity(0));
  EXPECT_EQ(resultQ, resultVec.getQuantity(1));
  EXPECT_TRUE(resultVec.isAbsolute());
  EXPECT_TRUE(resultQ.isAbsolute());

  resultVec = testVec2 + testVec2;
  resultQ = testQ2 + testQ2;
  ASSERT_EQ(2u, resultVec.size());
  EXPECT_EQ(resultQ, resultVec.getQuantity(0));
  EXPECT_EQ(resultQ, resultVec.getQuantity(1));
  EXPECT_TRUE(resultVec.isRelative());
  EXPECT_TRUE(resultQ.isRelative());

  // subtract temperatures
  resultVec = testVec1 - testVec2;
  resultQ = testQ1 - testQ2;
  ASSERT_EQ(2u, resultVec.size());
  EXPECT_EQ(resultQ, resultVec.getQuantity(0));
  EXPECT_EQ(resultQ, resultVec.getQuantity(1));
  EXPECT_TRUE(resultVec.isAbsolute());
  EXPECT_TRUE(resultQ.isAbsolute());

  resultVec = testVec2 - testVec1;
  resultQ = testQ2 - testQ1;
  ASSERT_EQ(2u, resultVec.size());
  EXPECT_EQ(resultQ, resultVec.getQuantity(0));
  EXPECT_EQ(resultQ, resultVec.getQuantity(1));
  EXPECT_TRUE(resultVec.isAbsolute());
  EXPECT_TRUE(resultQ.isAbsolute());

  resultVec = testVec1 - testVec1;
  resultQ = testQ1 - testQ1;
  ASSERT_EQ(2u, resultVec.size());
  EXPECT_EQ(resultQ, resultVec.getQuantity(0));
  EXPECT_EQ(resultQ, resultVec.getQuantity(1));
  EXPECT_TRUE(resultVec.isRelative());
  EXPECT_TRUE(resultQ.isRelative());

  // multiply
  testVec1 = OSQuantityVector(createSILength(), 2u, 7.0);
  testQ1 = Quantity(7.0, createSILength());
  testQ2 = Quantity(2.0, createSIForce());

  resultVec = testVec1 * testQ2;
  resultQ = testQ1 * testQ2;
  ASSERT_EQ(2u, resultVec.size());
  EXPECT_EQ(resultQ, resultVec.getQuantity(0));
  EXPECT_EQ(resultQ, resultVec.getQuantity(1));

  resultVec = testQ2.value() * testVec1;
  resultQ = testQ2.value() * testQ1;
  ASSERT_EQ(2u, resultVec.size());
  EXPECT_EQ(resultQ, resultVec.getQuantity(0));
  EXPECT_EQ(resultQ, resultVec.getQuantity(1));

  resultQ = dot(testVec1, testVec1);
  EXPECT_DOUBLE_EQ(98.0, resultQ.value());
  EXPECT_EQ("m^2", resultQ.standardUnitsString());

  // divide
  testQ2 = Quantity(2.0, createSITime());

  resultVec = testVec1 / testQ2;
  resultQ = testQ1 / testQ2;
  ASSERT_EQ(2u, resultVec.size());
  EXPECT_EQ(resultQ, resultVec.getQuantity(0));
  EXPECT_EQ(resultQ, resultVec.getQuantity(1));

  resultVec = testVec1 / testQ2.value();
  resultQ = testQ1 / testQ2.value();
  ASSERT_EQ(2u, resultVec.size());
  EXPECT_EQ(resultQ, resultVec.getQuantity(0));
  EXPECT_EQ(resultQ, resultVec.getQuantity(1));
}
/*
TEST_F(UnitsFixture,OSQuantityVector_Profiling_Construction_QuantityVectorBaseCase) {
  for (unsigned i = 0, n = 100; i < n; ++i) {
    DoubleVector vals = toStandardVector(randVector(0.0,1.0,8760u));
    Unit u = createSIForce();
    QuantityVector result;
    for (double val : vals) {
      result.push_back(Quantity(val,u));
    }
  }
}
*/
TEST_F(UnitsFixture, OSQuantityVector_Profiling_Construction_OSQuantityVector) {
  for (unsigned i = 0, n = 100; i < n; ++i) {
    DoubleVector vals = toStandardVector(randVector(0.0, 1.0, 8760u));
    Unit u = createSIForce();
    OSQuantityVector result(u, vals);
  }
}
/*
TEST_F(UnitsFixture,OSQuantityVector_Profiling_Addition_QuantityVectorBaseCase) {
  for (unsigned i = 0, n = 10; i < n; ++i) {
    QuantityVector result(testQuantityVector);
    Quantity other(10.0,createSIPower());
    for (auto & elem : result) {
      elem += other;
    }
  }
}
*/
TEST_F(UnitsFixture, OSQuantityVector_Profiling_Addition_OSQuantityVector) {
  for (unsigned i = 0, n = 10; i < n; ++i) {
    Quantity other(10.0, createSIPower());
    [[maybe_unused]] OSQuantityVector result = testOSQuantityVector + other;
  }
}
/*
TEST_F(UnitsFixture,OSQuantityVector_Profiling_Multiplication_QuantityVectorBaseCase) {
  for (unsigned i = 0, n = 10; i < n; ++i) {
    QuantityVector result(testQuantityVector);
    Quantity other(8.0,createWhTime());
    for (auto & elem : result) {
      elem *= other;
    }
  }
}
*/
TEST_F(UnitsFixture, OSQuantityVector_Profiling_Multiplication_OSQuantityVector) {
  for (unsigned i = 0, n = 10; i < n; ++i) {
    Quantity other(8.0, createWhTime());
    [[maybe_unused]] OSQuantityVector result = testOSQuantityVector * other;
  }
}
