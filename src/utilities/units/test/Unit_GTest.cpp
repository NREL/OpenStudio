/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>
#include "UnitsFixture.hpp"

#include "../Unit.hpp"
#include "../Scale.hpp"

#include "../../core/Exception.hpp"
#include "../IddUnitString.hpp"
#include "../QuantityFactory.hpp"
#include "../QuantityConverter.hpp"

#include <vector>
#include <string>

using namespace openstudio;

TEST_F(UnitsFixture, Unit_Constructors) {
  LOG(Debug, "Unit_Constructors");

  // default
  Unit myUnit;
  myUnit.setBaseUnitExponent("ft", 1);
  myUnit.setBaseUnitExponent("s", -1);
  myUnit.setBaseUnitExponent("s", -2);
  testStreamOutput("ft/s^2", myUnit);

  // scale from exponent and prettyString
  Unit myScaledUnit(-6, "J");
  myScaledUnit.setBaseUnitExponent("kg", 1);
  myScaledUnit.setBaseUnitExponent("m", 2);
  myScaledUnit.setBaseUnitExponent("s", -2);
  testStreamOutput("\\muJ", myScaledUnit);

  // scale from abbreviaiton
  Unit u1("k");
  u1.setBaseUnitExponent("m", 1);
  testStreamOutput("km", u1);

  // bad scale specifications
  ASSERT_THROW(Unit(100), Exception);
  ASSERT_THROW(Unit("b"), Exception);
}

TEST_F(UnitsFixture, Unit_BaseUnits) {
  Unit u1;
  std::vector<std::string> baseUnits = u1.baseUnits();
  ASSERT_EQ((unsigned)0, baseUnits.size());

  u1.setBaseUnitExponent("Btu", 1);
  u1.setBaseUnitExponent("h", -1);
  baseUnits = u1.baseUnits();
  ASSERT_EQ(static_cast<unsigned>(2), baseUnits.size());
  EXPECT_EQ("Btu", baseUnits[0]);
  EXPECT_EQ("h", baseUnits[1]);
  EXPECT_EQ(1, u1.baseUnitExponent("Btu"));
  EXPECT_EQ(0, u1.baseUnitExponent("ft"));
  EXPECT_TRUE(u1.isBaseUnit("h"));
  EXPECT_FALSE(u1.isBaseUnit("ft"));
}

TEST_F(UnitsFixture, Unit_Scales) {
  // kJ
  Unit u1(3, "J");
  u1.setBaseUnitExponent("kg", 1);
  u1.setBaseUnitExponent("m", 2);
  u1.setBaseUnitExponent("s", -2);
  Scale s1 = u1.scale();
  EXPECT_EQ("kilo", s1.name);

  // change to centi
  EXPECT_TRUE(u1.setScale(-2));
  s1 = u1.scale();
  EXPECT_EQ("c", s1.abbr);

  // there is no scale for 10^20
  ASSERT_FALSE(u1.setScale(20));

  // change to peta
  ASSERT_TRUE(u1.setScale("P"));
  s1 = u1.scale();
  EXPECT_EQ(15, s1.exponent);
}

TEST_F(UnitsFixture, Unit_StringOutput) {
  Unit u1(3, "J");
  u1.setBaseUnitExponent("kg", 1);
  u1.setBaseUnitExponent("m", 2);
  u1.setBaseUnitExponent("s", -2);
  EXPECT_EQ("kg*m^2/s^2", u1.standardString(false));
  EXPECT_EQ("kkg*m^2/s^2", u1.standardString());
  EXPECT_EQ("J", u1.prettyString(false));
  testStreamOutput("kJ", u1);

  Unit u2("c");
  u2.setBaseUnitExponent("m", 1);
  EXPECT_EQ("m", u2.standardString(false));
  EXPECT_EQ("", u2.prettyString(false));
  testStreamOutput("cm", u2);

  // pull scale into exponent
  Unit u3("M");
  u3.setBaseUnitExponent("m", 2);
  testStreamOutput("km^2", u3);
  // can't pull scale into exponent
  u3.setScale(3);
  testStreamOutput("k(m^2)", u3);

  // pull scale into denominator
  Unit u4(3);
  u4.setBaseUnitExponent("s", -1);
  testStreamOutput("1/ms", u4);

  // pull scale into numerator for prettyString, and into denominator for standardString
  Unit u5(3, "Hz");
  u5.setBaseUnitExponent("s", -1);
  testStreamOutput("kHz", u5);
  EXPECT_EQ("1/ms", u5.standardString());
}

TEST_F(UnitsFixture, Unit_LogicalOperators) {
  LOG(Debug, "Unit_LogicalOperators");

  Unit u1;
  u1.setBaseUnitExponent("ft", 1);
  u1.setBaseUnitExponent("s", -1);
  testStreamOutput("ft/s", u1);

  Unit u2;
  u2.setBaseUnitExponent("s", -1);
  u2.setBaseUnitExponent("ft", 1);
  testStreamOutput("ft/s", u2);

  Unit u3;

  EXPECT_TRUE(u1 == u1);
  EXPECT_TRUE(u1 == u2);
  EXPECT_FALSE(u1 == u3);

  u1.setScale("k");
  EXPECT_TRUE(u1 == u2);  // still equal even though different scales
  EXPECT_TRUE(u1 != u3);
}

TEST_F(UnitsFixture, Unit_ArithmeticOperators) {
  LOG(Debug, "Unit_ArithmeticOperators");

  // MULTIPLICATION

  Unit u1;
  u1.setBaseUnitExponent("ft", 1);
  u1 *= u1;
  {
    SCOPED_TRACE("u1 *= u1");
    testStreamOutput("ft^2", u1);
  }

  Unit u2;
  u2.setBaseUnitExponent("s", -1);
  Unit u3 = u1 * u2;
  {
    SCOPED_TRACE("u1*u2");
    testStreamOutput("ft^2/s", u3);
  }

  Unit u4(3);
  u4.setBaseUnitExponent("kg", 1);
  u4.setBaseUnitExponent("m", 1);
  u4.setBaseUnitExponent("s", -2);
  {
    SCOPED_TRACE("u4");
    testStreamOutput("kkg*m/s^2", u4);
  }

  Unit u5;
  u5.setBaseUnitExponent("m", 1);

  Unit u6 = u4 * u5;
  {
    SCOPED_TRACE("u4*u5");
    testStreamOutput("kJ", u6);
  }
  Unit u7 = u5 * u4;
  {
    SCOPED_TRACE("u5*u4");
    testStreamOutput("k(m^2*kg/s^2)", u7);
  }
  // cppcheck-suppress knownConditionTrueFalse
  EXPECT_TRUE(u6 == u7);

  // DIVISION
  Unit u8 = u7 / u4;
  EXPECT_TRUE(u8 == u5);

  // POW
  Unit u9 = pow(u3, 3);
  {
    SCOPED_TRACE("pow(u3,3)");
    testStreamOutput("ft^6/s^3", u9);
  }
  u9.pow(-4, 3);
  {
    SCOPED_TRACE("u9.pow(-4,3)");
    testStreamOutput("s^4/ft^8", u9);
  }

  Unit u10 = pow(u4, 2);
  {
    SCOPED_TRACE("pow(u4,2)");
    testStreamOutput("kkg^2*m^2/s^4", u10);
  }

  ASSERT_THROW(u10.pow(1, 3), Exception);
}

TEST_F(UnitsFixture, UnitString_Tests) {
  IddUnitString s1(std::string("m3/s"));

  EXPECT_TRUE(s1.toStandardUnitString() == "m^3/s");
}
