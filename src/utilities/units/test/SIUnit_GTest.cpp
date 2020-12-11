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

#include "../SIUnit.hpp"
#include "../Scale.hpp"
#include "../QuantityConverter.hpp"
#include "../UnitFactory.hpp"

#include "../../core/Exception.hpp"

using openstudio::Unit;
using openstudio::UnitSystem;
using openstudio::UnitFactory;
using openstudio::SIExpnt;
using openstudio::SIUnit;
using openstudio::Exception;

using openstudio::createSIMass;
using openstudio::createSILength;
using openstudio::createSITime;
using openstudio::createSITemperature;
using openstudio::createSIPeople;

using openstudio::createSIForce;
using openstudio::createSIEnergy;
using openstudio::createSIPower;

using openstudio::convert;

TEST_F(UnitsFixture, SIUnit_Constructors) {
  LOG(Debug, "SIUnit_Constructors");

  // default
  SIUnit u1;
  ASSERT_NO_THROW(u1.setBaseUnitExponent("m", 1));
  ASSERT_THROW(u1.setBaseUnitExponent("ft", 2), Exception);
  testStreamOutput("m", u1);

  // default with pretty string
  SIUnit u2(SIExpnt(1, 1, -2, 0), 0, "N");
  testStreamOutput("N", u2);
  // scale exponent and pretty string
  SIUnit u3(SIExpnt(1, 1, -2, 0), 3, "N");
  testStreamOutput("kN", u3);
  EXPECT_TRUE(u3.baseUnitExponent("kg") == 1);
  EXPECT_TRUE(u3.baseUnitExponent("K") == 0);
  EXPECT_TRUE(u3.baseUnitExponent("ft") == 0);

  u2.setScale("\\mu");
  testStreamOutput("\\muN", u2);

  // scale abbreviation
  SIUnit u4("c", SIExpnt(0, 0, 0, 1));
  testStreamOutput("cK", u4);
  EXPECT_EQ(1, u4.baseUnitExponent("K"));
}

TEST_F(UnitsFixture, SIUnit_BaseUnits) {
  SIUnit u1;
  EXPECT_TRUE(u1.isBaseUnit("s"));
  EXPECT_TRUE(u1.isBaseUnit("cd"));
  EXPECT_FALSE(u1.isBaseUnit("ft"));
  ASSERT_THROW(u1.setBaseUnitExponent("Z", 1), Exception);
}

TEST_F(UnitsFixture, SIUnit_LogicalOperators) {
  LOG(Debug, "SIUnit_LogicalOperators");

  SIUnit u1;
  SIUnit u2(SIExpnt(1, 1, -2));
  SIUnit u3(SIExpnt(1, 1, -2, 0), 3);

  EXPECT_TRUE(u2 == u3);
  EXPECT_FALSE(u1 == u3);
}

TEST_F(UnitsFixture, SIUnit_ArithmeticOperators) {
  LOG(Debug, "SIUnit_ArithmeticOperators");

  SIUnit u1;
  SIUnit u2(SIExpnt(1, 1, -2, 0), 0, "N");
  SIUnit u3(SIExpnt(1));               // kg
  SIUnit u4(SIExpnt(0, 1, 0, 0), -2);  // cm

  // multiplication
  u1 *= u3;  // kg
  testStreamOutput("kg", u1);
  Unit u5 = u2 * u3;  // N*kg
  EXPECT_TRUE(u5.system() == UnitSystem::SI);
  testStreamOutput("N*kg", u5);
  Unit u6 = u2 * u4;  // cJ
  EXPECT_TRUE(u6.system() == UnitSystem::SI);
  testStreamOutput("cJ", u6);

  // division
  u6 /= u4;
  EXPECT_TRUE(u6 == u2);

  // power
  Unit u7 = pow(u4, 6);  // (cm)^6
  EXPECT_TRUE(u7.system() == UnitSystem::SI);
  testStreamOutput("cm^6", u7);
  EXPECT_EQ("p", u7.scale().abbr);
  u7.pow(1, 3);  // (cm)^2
  testStreamOutput("cm^2", u7);
}

TEST_F(UnitsFixture, SIUnit_createFunctions) {
  SIUnit u;

  u = createSIMass();
  EXPECT_EQ(1, u.baseUnitExponent("kg"));
  EXPECT_EQ(0, u.scale().exponent);
  EXPECT_EQ("kg", u.standardString());
  EXPECT_EQ("", u.prettyString());

  u = createSILength();
  EXPECT_EQ(1, u.baseUnitExponent("m"));
  EXPECT_EQ(0, u.scale().exponent);
  EXPECT_EQ("m", u.standardString());
  EXPECT_EQ("", u.prettyString());

  u = createSITime();
  EXPECT_EQ(1, u.baseUnitExponent("s"));
  EXPECT_EQ(0, u.scale().exponent);
  EXPECT_EQ("s", u.standardString());
  EXPECT_EQ("", u.prettyString());

  u = createSITemperature();
  EXPECT_EQ(1, u.baseUnitExponent("K"));
  EXPECT_EQ(0, u.scale().exponent);
  EXPECT_EQ("K", u.standardString());
  EXPECT_EQ("", u.prettyString());

  u = createSIPeople();
  EXPECT_EQ(1, u.baseUnitExponent("people"));
  EXPECT_EQ(0, u.scale().exponent);
  EXPECT_EQ("people", u.standardString());
  EXPECT_EQ("", u.prettyString());
  u.pow(-1);
  EXPECT_EQ("1/person", u.standardString());

  u = createSIForce();
  EXPECT_EQ(0, u.scale().exponent);
  EXPECT_EQ("kg*m/s^2", u.standardString());
  EXPECT_EQ("N", u.prettyString());

  u = createSIEnergy();
  EXPECT_EQ(0, u.scale().exponent);
  EXPECT_EQ("kg*m^2/s^2", u.standardString());
  EXPECT_EQ("J", u.prettyString());

  u = createSIPower();
  EXPECT_EQ(0, u.scale().exponent);
  EXPECT_EQ("kg*m^2/s^3", u.standardString());
  EXPECT_EQ("W", u.prettyString());
}

TEST_F(UnitsFixture, SIUnit_convert) {
  boost::optional<double> value;

  boost::optional<Unit> mUnit = UnitFactory::instance().createUnit("m");
  boost::optional<Unit> cmUnit = UnitFactory::instance().createUnit("cm");
  boost::optional<Unit> m2Unit = UnitFactory::instance().createUnit("m^2");
  boost::optional<Unit> cm2Unit = UnitFactory::instance().createUnit("cm^2");
  ASSERT_TRUE(mUnit);
  ASSERT_TRUE(cmUnit);
  ASSERT_TRUE(m2Unit);
  ASSERT_TRUE(cm2Unit);

  EXPECT_TRUE(mUnit->isBaseUnit("m"));
  EXPECT_EQ(1, mUnit->baseUnitExponent("m"));
  EXPECT_EQ("", mUnit->scale().abbr);
  EXPECT_EQ("", mUnit->scale().name);
  EXPECT_EQ(0, mUnit->scale().exponent);
  EXPECT_EQ(1, mUnit->scale().value);

  EXPECT_TRUE(cmUnit->isBaseUnit("m"));
  EXPECT_EQ(1, cmUnit->baseUnitExponent("m"));
  EXPECT_EQ("c", cmUnit->scale().abbr);
  EXPECT_EQ("centi", cmUnit->scale().name);
  EXPECT_EQ(-2, cmUnit->scale().exponent);
  EXPECT_EQ(0.01, cmUnit->scale().value);

  EXPECT_TRUE(m2Unit->isBaseUnit("m"));
  EXPECT_EQ(2, m2Unit->baseUnitExponent("m"));
  EXPECT_EQ("", m2Unit->scale().abbr);
  EXPECT_EQ("", m2Unit->scale().name);
  EXPECT_EQ(0, m2Unit->scale().exponent);
  EXPECT_EQ(1, m2Unit->scale().value);

  EXPECT_TRUE(cm2Unit->isBaseUnit("m"));
  EXPECT_EQ(2, cm2Unit->baseUnitExponent("m"));
  EXPECT_EQ("_dm", cm2Unit->scale().abbr);
  EXPECT_EQ("decimilli", cm2Unit->scale().name);
  EXPECT_EQ(-4, cm2Unit->scale().exponent);
  EXPECT_EQ(0.0001, cm2Unit->scale().value);

  value = convert(1.0, "m", "cm");
  ASSERT_TRUE(value);
  EXPECT_NEAR(100.0, value.get(), 0.0001);

  value = convert(1.0, "cm", "m");
  ASSERT_TRUE(value);
  EXPECT_NEAR(0.01, value.get(), 0.0001);

  value = convert(1.0, "1/m", "1/cm");
  ASSERT_TRUE(value);
  EXPECT_NEAR(0.01, value.get(), 0.0001);

  value = convert(1.0, "$/m", "$/cm");
  ASSERT_TRUE(value);
  EXPECT_NEAR(0.01, value.get(), 0.0001);

  value = convert(1.0, "m^2", "cm^2");
  ASSERT_TRUE(value);
  EXPECT_NEAR(10000.0, value.get(), 0.0001);

  value = convert(1.0, "cm^2", "m^2");
  ASSERT_TRUE(value);
  EXPECT_NEAR(0.0001, value.get(), 0.0001);

  value = convert(1.0, "1/m^2", "1/cm^2");
  ASSERT_TRUE(value);
  EXPECT_NEAR(0.0001, value.get(), 0.0001);

  value = convert(1.0, "$/m^2", "$/cm^2");
  ASSERT_TRUE(value);
  EXPECT_NEAR(0.0001, value.get(), 0.0001);
}

TEST_F(UnitsFixture, SIUnit_convert2) {
  boost::optional<double> value;

  boost::optional<Unit> mUnit = UnitFactory::instance().createUnit("m");
  boost::optional<Unit> cmUnit = UnitFactory::instance().createUnit("cm");
  boost::optional<Unit> m3Unit = UnitFactory::instance().createUnit("m^3");
  boost::optional<Unit> cm3Unit = UnitFactory::instance().createUnit("cm^3");
  ASSERT_TRUE(mUnit);
  ASSERT_TRUE(cmUnit);
  ASSERT_TRUE(m3Unit);
  ASSERT_TRUE(cm3Unit);

  EXPECT_TRUE(mUnit->isBaseUnit("m"));
  EXPECT_EQ(1, mUnit->baseUnitExponent("m"));
  EXPECT_EQ("", mUnit->scale().abbr);
  EXPECT_EQ("", mUnit->scale().name);
  EXPECT_EQ(0, mUnit->scale().exponent);
  EXPECT_EQ(1, mUnit->scale().value);

  EXPECT_TRUE(cmUnit->isBaseUnit("m"));
  EXPECT_EQ(1, cmUnit->baseUnitExponent("m"));
  EXPECT_EQ("c", cmUnit->scale().abbr);
  EXPECT_EQ("centi", cmUnit->scale().name);
  EXPECT_EQ(-2, cmUnit->scale().exponent);
  EXPECT_EQ(0.01, cmUnit->scale().value);

  EXPECT_TRUE(m3Unit->isBaseUnit("m"));
  EXPECT_EQ(3, m3Unit->baseUnitExponent("m"));
  EXPECT_EQ("", m3Unit->scale().abbr);
  EXPECT_EQ("", m3Unit->scale().name);
  EXPECT_EQ(0, m3Unit->scale().exponent);
  EXPECT_EQ(1, m3Unit->scale().value);

  EXPECT_TRUE(cm3Unit->isBaseUnit("m"));
  EXPECT_EQ(3, cm3Unit->baseUnitExponent("m"));
  EXPECT_EQ("\\mu", cm3Unit->scale().abbr);
  EXPECT_EQ("micro", cm3Unit->scale().name);
  EXPECT_EQ(-6, cm3Unit->scale().exponent);
  EXPECT_EQ(0.000001, cm3Unit->scale().value);

  value = convert(1.0, "m", "cm");
  ASSERT_TRUE(value);
  EXPECT_NEAR(100.0, value.get(), 0.0001);

  value = convert(1.0, "cm", "m");
  ASSERT_TRUE(value);
  EXPECT_NEAR(0.01, value.get(), 0.0001);

  value = convert(1.0, "m^3", "cm^3");
  ASSERT_TRUE(value);
  EXPECT_NEAR(1000000.0, value.get(), 0.0001);

  value = convert(1.0, "cm^3", "m^3");
  ASSERT_TRUE(value);
  EXPECT_NEAR(0.000001, value.get(), 0.0000001);

  value = convert(1.0, "1/m^3", "1/cm^3");
  ASSERT_TRUE(value);
  EXPECT_NEAR(0.000001, value.get(), 0.0000001);

  value = convert(1.0, "$/m^3", "$/cm^3");
  ASSERT_TRUE(value);
  EXPECT_NEAR(0.000001, value.get(), 0.0000001);
}