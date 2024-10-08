/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>
#include "UnitsFixture.hpp"

#include "../ScaleFactory.hpp"
#include "../Quantity.hpp"
#include "../Unit.hpp"
#include "../BTUUnit.hpp"
#include "../CFMUnit.hpp"
#include "../IPUnit.hpp"
#include "../SIUnit.hpp"
#include "../CelsiusUnit.hpp"
#include "../FahrenheitUnit.hpp"
#include "../TemperatureUnit.hpp"

#include "../../core/Exception.hpp"

using namespace openstudio;

using std::stringstream;

TEST_F(UnitsFixture, Quantity_Constructors) {
  LOG(Debug, "Quantity_Constructors")

  Quantity q1;
  testStreamOutput("0", q1);
  Quantity q2(1.0);
  testStreamOutput("1", q2);

  Unit u3;
  u3.setBaseUnitExponent("ft", 1);
  Quantity q3(34.2, u3);
  testStreamOutput("34.2 ft", q3, 1);

  q3.setScale(3);
  testStreamOutput("0.0342 kft", q3, 4);
}

TEST_F(UnitsFixture, Quantity_ArithmeticOperators) {
  LOG(Debug, "Quantity_ArithmeticOperators");

  Quantity q1;
  Quantity q2(1.0);

  Unit u3;
  u3.setBaseUnitExponent("ft", 1);
  Quantity q3(34.2, u3);

  // addition
  Quantity q4 = q1 + q2;
  q4 += q4;
  testNumbersEqual(2.0, q4.value());
  testStreamOutput("2", q4);
  ASSERT_THROW(q3 + q1, Exception);

  // subtraction;
  ASSERT_THROW(q3 - q1, Exception);
  Quantity len(3.5, u3);
  len -= q3;
  testNumbersEqual(-30.7, len.value());
  testStreamOutput("-30.7 ft", len, 1);

  // multiplication
  q3 *= q4;
  testNumbersEqual(68.4, q3.value());
  testStreamOutput("68.4 ft", q3, 1);

  Unit u5;
  u5.setBaseUnitExponent("s", -2);
  u5.setBaseUnitExponent("ft", 1);
  Quantity q5(0.5, u5);
  Quantity q6 = q5 * q3;
  testNumbersEqual(34.2, q6.value());
  testStreamOutput("34.2 ft^2/s^2", q6, 1);

  // division
  len = Quantity(1.1, u3);
  Quantity a = q6 / len;
  testNumbersEqual(31.0909090909, a.value());
  EXPECT_EQ("ft/s^2", a.standardUnitsString());

  // pow
  // cppcheck-suppress ignoredReturnValue
  a.pow(6);
  EXPECT_EQ("ft^6/s^12", a.standardUnitsString());
  Quantity b = openstudio::pow(a, 1, 3);
  EXPECT_EQ("ft^2/s^4", b.standardUnitsString());
  testNumbersEqual(966.644628099, b.value());
  a = b;

  // multiplication and division with double
  a *= 1.21;
  testNumbersEqual(1169.64, a.value());
  b = a / 2;
  testNumbersEqual(584.82, b.value());
  EXPECT_EQ("ft^2/s^4", b.standardUnitsString());
  EXPECT_EQ("", b.prettyUnitsString());

  Quantity c = 32.0 / b;
  EXPECT_EQ("s^4/ft^2", c.standardUnitsString());
  testNumbersEqual(0.054717690913, c.value());

  // Operations on Quantities with units of temperature
  LOG(Debug, "Quantity_FahrenheitUnit_Arithmetic");

  FahrenheitUnit fu1(1, 0, "");
  Quantity T1(60.0, fu1);
  FahrenheitUnit fu2(1);
  Quantity T2(74.0, fu2);
  Quantity TT1 = pow(T1, 2);
  Quantity TT2 = pow(T2, 2);

  EXPECT_TRUE(TT1.isAbsolute());
  EXPECT_TRUE(TT2.isAbsolute());

  Quantity q;

  // -, unary
  q = -T1;
  testStreamOutput("-60 F", q);
  q.setAsRelative();
  q = -q;
  testStreamOutput("60 F", q);
  EXPECT_FALSE(q.isAbsolute());

  // +, good units, bad units, absolute + relative = absolute
  q = T1 + T2;
  EXPECT_TRUE(q.isAbsolute());
  testStreamOutput("134 F", q, 0);
  EXPECT_THROW(T1 + TT1, Exception);
  q.setAsRelative();
  q += T1;
  EXPECT_TRUE(q.isAbsolute());

  // -, binary, good units, bad units, absolute -> relative when exp == 1, ow same as +
  q = T2 - T1;
  testStreamOutput("14 F", q, 0);
  EXPECT_FALSE(q.isAbsolute());
  EXPECT_THROW(q - TT2, Exception);
  q -= T1;
  EXPECT_TRUE(q.isAbsolute());
  testStreamOutput("-46 F", q, 0);
  q = TT2 - TT1;
  EXPECT_TRUE(q.isAbsolute());

  // *, absolute*relative = absolute
  Quantity deltaT = T2 - T1;
  q = deltaT * TT1;
  EXPECT_TRUE(q.isAbsolute());
  testNumbersEqual(14.0 * 3600.0, q.value());
  q = TT1 * deltaT;
  EXPECT_TRUE(q.isAbsolute());
  testNumbersEqual(14.0 * 3600.0, q.value());
  EXPECT_EQ("F^3", q.standardUnitsString());

  // /, absolute/relative = absolute and vice-versa
  q = TT1 / deltaT;
  EXPECT_TRUE(q.isAbsolute());
  q = deltaT / TT2;
  EXPECT_TRUE(q.isAbsolute());
  testNumbersEqual(14.0 / TT2.value(), q.value());
  EXPECT_EQ("1/F", q.standardUnitsString());

  // * w/ double, retains absolute v. relative of quantity
  q = T1 * 3.0;
  EXPECT_TRUE(q.isAbsolute());
  testStreamOutput("180 F", q, 0);
  q = 2.0 * deltaT;
  EXPECT_FALSE(q.isAbsolute());
  testNumbersEqual(28.0, q.value());

  // / w/ double, retains absolute v. relative of quantity
  q = 20.0 / T1;
  EXPECT_TRUE(q.isAbsolute());
  testStreamOutput("0.333 1/F", q, 3);
  q = deltaT / 2.0;
  EXPECT_FALSE(q.isAbsolute());
  testStreamOutput("7 F", q, 0);
}

TEST_F(UnitsFixture, Quantity_MixedTypeOperations) {
  openstudio::SIUnit siu(SIExpnt(0, 1), 0, "");
  Quantity l1(3.0, siu);
  openstudio::IPUnit ipu(IPExpnt(0, 1), 0, "");
  Quantity l2(2.0, ipu);
  openstudio::BTUUnit btuu(BTUExpnt(0, 1), 0, "");
  Quantity l3(0.5, btuu);

  Quantity r;
  r = l1 * l2;
  EXPECT_EQ(UnitSystem(UnitSystem::Mixed), r.system());
  r = l1 / l2;
  EXPECT_EQ(UnitSystem(UnitSystem::Mixed), r.system());
  l3 *= l1;
  EXPECT_EQ(UnitSystem(UnitSystem::Mixed), l3.system());
  ASSERT_THROW(l1 + l2, Exception);
}
