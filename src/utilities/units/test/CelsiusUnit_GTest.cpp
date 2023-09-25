/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>
#include "UnitsFixture.hpp"

#include "../CelsiusUnit.hpp"
#include "../Scale.hpp"

#include "../../core/Exception.hpp"

using openstudio::Unit;
using openstudio::UnitSystem;
using openstudio::CelsiusUnit;
using openstudio::Exception;

using openstudio::createCelsiusTemperature;

TEST_F(UnitsFixture, CelsiusUnit_Constructors) {
  LOG(Debug, "CelsiusUnit_Constructors");

  CelsiusUnit T1(1);

  EXPECT_EQ(1, T1.baseUnitExponent("C"));
  EXPECT_EQ(0, T1.baseUnitExponent("ft"));
  T1.setBaseUnitExponent("C", 0);
  EXPECT_EQ(0, T1.baseUnitExponent("C"));

  CelsiusUnit T2(T1);
  EXPECT_EQ(UnitSystem::Celsius, T2.system().value());
}

TEST_F(UnitsFixture, CelsiusUnit_ArithmeticOperators) {
  LOG(Debug, "CelsiusUnit_ArithmeticOperators");

  Unit T1 = CelsiusUnit(1);
  Unit T2 = CelsiusUnit(1);

  // /=
  T2 /= T1;
  EXPECT_EQ(0, T2.baseUnitExponent("C"));

  // *
  T2 = T1 * T1;
  EXPECT_EQ(2, T2.baseUnitExponent("C"));

  // /
  T2 = T2 / T1;
  EXPECT_EQ(1, T2.baseUnitExponent("C"));

  // pow
  T2 = pow(T1, 3);
  EXPECT_EQ(3, T2.baseUnitExponent("C"));
}

TEST_F(UnitsFixture, CelsiusUnit_CreateFunctions) {
  CelsiusUnit u;

  u = createCelsiusTemperature();
  EXPECT_EQ(1, u.baseUnitExponent("C"));
  EXPECT_EQ(0, u.scale().exponent);
  EXPECT_EQ("C", u.standardString());
  EXPECT_EQ("", u.prettyString());
}

TEST_F(UnitsFixture, CelsiusUnit_AbsoluteRelative) {
  CelsiusUnit T(1);
  EXPECT_TRUE(T.isAbsolute());
  T.setAsRelative();
  EXPECT_FALSE(T.isAbsolute());
}
