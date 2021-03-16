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

#include "../IPUnit.hpp"
#include "../IPUnit_Impl.hpp"
#include "../Scale.hpp"
#include "../QuantityConverter.hpp"

#include "../../core/Exception.hpp"

using openstudio::Unit;
using openstudio::UnitSystem;
using openstudio::IPExpnt;
using openstudio::IPUnit;
using openstudio::Exception;
using openstudio::kilo;
using openstudio::micro;

using openstudio::createIPMass;
using openstudio::createIPLength;
using openstudio::createIPTime;
using openstudio::createIPTemperature;
using openstudio::createIPPeople;
using openstudio::createIPCurrency;

using openstudio::createIPForce;
using openstudio::createIPEnergy;
using openstudio::createIPPower;

using openstudio::convert;

TEST_F(UnitsFixture, IPUnit_Constructors) {
  LOG(Debug, "IPUnit_Constructors");

  IPUnit u1;
  ASSERT_THROW(u1.setBaseUnitExponent("m", 1), Exception);
  ASSERT_NO_THROW(u1.setBaseUnitExponent("ft", 2));
  testStreamOutput("ft^2", u1);

  IPUnit u2(IPExpnt(1, 1, -2));
  testStreamOutput("lb_m*ft/s^2", u2);
  u2.lbmToLbf();
  testStreamOutput("lb_f", u2);
  ASSERT_TRUE(u2.baseUnitExponent("lb_f") == 1);
  ASSERT_TRUE(u2.baseUnitExponent("lb_m") == 0);
  ASSERT_TRUE(u2.baseUnitExponent("m") == 0);
  u2.lbfToLbm();
  testStreamOutput("lb_m*ft/s^2", u2);

  IPUnit u3(IPExpnt(0, -2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1));
  testStreamOutput("$/ft^2", u3);
  ASSERT_TRUE(u3.baseUnitExponent("$") == 1);
  ASSERT_TRUE(u3.baseUnitExponent("ft") == -2);
  ASSERT_TRUE(u3.baseUnitExponent("s") == 0);
  u3.lbmToLbf();
  testStreamOutput("$/ft^2", u3);
  ASSERT_TRUE(u3.baseUnitExponent("$") == 1);
  ASSERT_TRUE(u3.baseUnitExponent("ft") == -2);
  ASSERT_TRUE(u3.baseUnitExponent("s") == 0);
  u3.lbfToLbm();
  testStreamOutput("$/ft^2", u3);
}

TEST_F(UnitsFixture, IPUnit_convert) {
  boost::optional<double> value;

  value = convert(1.0, "ft", "m");
  ASSERT_TRUE(value);
  EXPECT_NEAR(0.3048, value.get(), 0.0001);

  value = convert(1.0, "m", "ft");
  ASSERT_TRUE(value);
  EXPECT_NEAR(3.28084, value.get(), 0.0001);

  value = convert(1.0, "ft^2", "m^2");
  ASSERT_TRUE(value);
  EXPECT_NEAR(0.092903, value.get(), 0.0001);

  value = convert(1.0, "m^2", "ft^2");
  ASSERT_TRUE(value);
  EXPECT_NEAR(10.7639, value.get(), 0.0001);

  value = convert(1.0, "1/ft^2", "1/m^2");
  ASSERT_TRUE(value);
  EXPECT_NEAR(10.7639, value.get(), 0.0001);

  value = convert(1.0, "$/ft^2", "$/m^2");
  ASSERT_TRUE(value);
  EXPECT_NEAR(10.7639, value.get(), 0.0001);
}

TEST_F(UnitsFixture, IPUnit_convert2) {
  boost::optional<double> value;

  value = convert(1.0, "in", "cm");
  ASSERT_TRUE(value);
  EXPECT_NEAR(2.54, value.get(), 0.0001);

  value = convert(1.0, "cm", "in");
  ASSERT_TRUE(value);
  EXPECT_NEAR(0.393701, value.get(), 0.0001);

  value = convert(1.0, "in^2", "cm^2");
  ASSERT_TRUE(value);
  EXPECT_NEAR(6.4516, value.get(), 0.0001);

  value = convert(1.0, "cm^2", "in^2");
  ASSERT_TRUE(value);
  EXPECT_NEAR(0.1550, value.get(), 0.0001);

  value = convert(1.0, "1/in^2", "1/cm^2");
  ASSERT_TRUE(value);
  EXPECT_NEAR(0.1550, value.get(), 0.0001);

  value = convert(1.0, "$/in^2", "$/cm^2");
  ASSERT_TRUE(value);
  EXPECT_NEAR(0.1550, value.get(), 0.0001);
}

TEST_F(UnitsFixture, IPUnit_convert3) {
  boost::optional<double> value;

  value = convert(1.0, "in", "ft");
  ASSERT_TRUE(value);
  EXPECT_NEAR(0.083333, value.get(), 0.0001);

  value = convert(1.0, "ft", "in");
  ASSERT_TRUE(value);
  EXPECT_NEAR(12.0, value.get(), 0.0001);

  value = convert(1.0, "in^2", "ft^2");
  ASSERT_TRUE(value);
  EXPECT_NEAR(0.006944, value.get(), 0.0001);

  value = convert(1.0, "ft^2", "in^2");
  ASSERT_TRUE(value);
  EXPECT_NEAR(144.0, value.get(), 0.0001);

  value = convert(1.0, "1/in^2", "1/ft^2");
  ASSERT_TRUE(value);
  EXPECT_NEAR(144.0, value.get(), 0.0001);

  value = convert(1.0, "$/in^2", "$/ft^2");
  ASSERT_TRUE(value);
  EXPECT_NEAR(144.0, value.get(), 0.0001);
}

TEST_F(UnitsFixture, IPUnit_LogicalOperators) {
  LOG(Debug, "IPUnit_LogicalOperators");

  IPUnit u1 = createIPForce();
  IPUnit u2(IPExpnt(1, 1, -2));

  ASSERT_TRUE(u1 == u2);
  u2.setBaseUnitExponent("s", -3);
  ASSERT_TRUE(u1 != u2);
}

TEST_F(UnitsFixture, IPUnit_ArithmeticOperators) {
  LOG(Debug, "IPUnit_ArithmeticOperators");

  IPUnit u1;
  IPUnit u2 = createIPForce();
  IPUnit u3(IPExpnt(1));

  u1 *= u3;
  testStreamOutput("lb_m", u1);
  Unit u4 = u2 * u3;
  ASSERT_TRUE(u4.system() == UnitSystem::IP);
  testStreamOutput("lb_m*lb_f", u4);
  u4.cast<IPUnit>().lbfToLbm();
  testStreamOutput("lb_m^2*ft/s^2", u4);

  Unit u5 = u4 / u3;
  EXPECT_TRUE(u5.system() == UnitSystem::IP);
  EXPECT_TRUE(u5 == u2);

  Unit u6 = pow(u5, -6);
  EXPECT_TRUE(u6.system() == UnitSystem::IP);
  testStreamOutput("s^12/lb_m^6*ft^6", u6);
  u6.pow(1, 3);
  testStreamOutput("s^4/lb_m^2*ft^2", u6);
}

TEST_F(UnitsFixture, IPUnit_createFunctions) {
  IPUnit u;

  u = createIPMass();
  EXPECT_EQ(1, u.baseUnitExponent("lb_m"));
  EXPECT_EQ(0, u.scale().exponent);
  EXPECT_EQ("lb_m", u.standardString());
  EXPECT_EQ("", u.prettyString());

  u = createIPLength();
  EXPECT_EQ(1, u.baseUnitExponent("ft"));
  EXPECT_EQ(0, u.scale().exponent);
  EXPECT_EQ("ft", u.standardString());
  EXPECT_EQ("", u.prettyString());

  u = createIPTime();
  EXPECT_EQ(1, u.baseUnitExponent("s"));
  EXPECT_EQ(0, u.scale().exponent);
  EXPECT_EQ("s", u.standardString());
  EXPECT_EQ("", u.prettyString());

  u = createIPTemperature();
  EXPECT_EQ(1, u.baseUnitExponent("R"));
  EXPECT_EQ(0, u.scale().exponent);
  EXPECT_EQ("R", u.standardString());
  EXPECT_EQ("", u.prettyString());

  u = createIPPeople();
  EXPECT_EQ(1, u.baseUnitExponent("people"));
  EXPECT_EQ(0, u.scale().exponent);
  EXPECT_EQ("people", u.standardString());
  EXPECT_EQ("", u.prettyString());
  u.pow(-1);
  EXPECT_EQ("1/person", u.standardString());

  u = createIPCurrency();
  EXPECT_EQ(1, u.baseUnitExponent("$"));
  EXPECT_EQ(0, u.scale().exponent);
  EXPECT_EQ("$", u.standardString());
  EXPECT_EQ("", u.prettyString());
  u.pow(-1);
  EXPECT_EQ("1/$", u.standardString());

  u = createIPForce();
  EXPECT_EQ(0, u.scale().exponent);
  EXPECT_EQ("lb_f", u.standardString());
  EXPECT_EQ("", u.prettyString());

  u = createIPEnergy();
  EXPECT_EQ(0, u.scale().exponent);
  EXPECT_EQ("ft*lb_f", u.standardString());
  EXPECT_EQ("", u.prettyString());

  u = createIPPower();
  EXPECT_EQ(0, u.scale().exponent);
  EXPECT_EQ("ft*lb_f/s", u.standardString());
  EXPECT_EQ("", u.prettyString());
}
