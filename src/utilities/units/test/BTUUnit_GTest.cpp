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

#include "../BTUUnit.hpp"
#include "../Scale.hpp"

#include "../../core/Exception.hpp"

using openstudio::Unit;
using openstudio::UnitSystem;
using openstudio::BTUExpnt;
using openstudio::BTUUnit;
using openstudio::Exception;

using openstudio::createBTULength;
using openstudio::createBTUTime;
using openstudio::createBTUTemperature;
using openstudio::createBTUPeople;

using openstudio::createBTUEnergy;
using openstudio::createBTUPower;

TEST_F(UnitsFixture, BTUUnit_Constructors) {
  LOG(Debug, "BTUUnit_Constructors");

  BTUUnit l1(BTUExpnt(0, 1));
  BTUUnit P1(BTUExpnt(1, 0, -1, 0), -3);  // mBtu/h
  BTUUnit E1("k", BTUExpnt(1));           // kBtu

  EXPECT_EQ(0, l1.baseUnitExponent("Btu"));
  EXPECT_EQ(1, l1.baseUnitExponent("ft"));
  l1.setBaseUnitExponent("R", -1);
  EXPECT_EQ(-1, l1.baseUnitExponent("R"));
  EXPECT_THROW(l1.setBaseUnitExponent("m", 1), Exception);

  EXPECT_EQ("Btu/h", P1.standardString(false));
  EXPECT_EQ("mBtu/h", P1.standardString());
  EXPECT_EQ(UnitSystem::BTU, P1.system().value());
  EXPECT_EQ(0, P1.baseUnitExponent("R"));

  BTUUnit P2(P1);
  EXPECT_EQ(UnitSystem::BTU, P2.system().value());

  EXPECT_EQ("", E1.prettyString());
  EXPECT_EQ("kBtu", E1.standardString());
  EXPECT_EQ(0, E1.baseUnitExponent("ft"));
}

TEST_F(UnitsFixture, BTUUnit_ArithmeticOperators) {
  LOG(Debug, "BTUUnit_ArithmeticOperators");

  // /=
  BTUUnit P1(BTUExpnt(1));
  BTUUnit t1(BTUExpnt(0, 0, 1));
  P1 /= t1;
  EXPECT_EQ("Btu/h", P1.standardString(false));
  EXPECT_EQ(1, P1.baseUnitExponent("Btu"));
  EXPECT_EQ(-1, P1.baseUnitExponent("h"));
  EXPECT_EQ(0, P1.baseUnitExponent("ft"));
  EXPECT_EQ(0, P1.baseUnitExponent("m"));

  // *
  Unit E1 = P1 * t1;
  EXPECT_TRUE(E1.system() == UnitSystem::BTU);
  EXPECT_EQ("Btu", E1.standardString(false));
  EXPECT_EQ("", E1.prettyString());

  // /
  Unit u = P1 / E1;
  u /= t1;
  EXPECT_TRUE(u.system() == UnitSystem::BTU);
  EXPECT_EQ("1/h^2", u.standardString(false));
  EXPECT_EQ(-2, u.baseUnitExponent("h"));
  EXPECT_EQ(0, u.baseUnitExponent("Btu"));

  // pow
  u.pow(-1, 2);
  EXPECT_EQ("h", u.standardString(false));
  EXPECT_EQ(1, u.baseUnitExponent("h"));
}

TEST_F(UnitsFixture, BTUUnit_CreateFunctions) {
  BTUUnit u;

  u = createBTULength();
  EXPECT_EQ(1, u.baseUnitExponent("ft"));
  EXPECT_EQ(0, u.scale().exponent);
  EXPECT_EQ("ft", u.standardString(false));
  EXPECT_EQ("", u.prettyString());

  u = createBTUTime();
  EXPECT_EQ(1, u.baseUnitExponent("h"));
  EXPECT_EQ(0, u.scale().exponent);
  EXPECT_EQ("h", u.standardString(false));
  EXPECT_EQ("", u.prettyString());

  u = createBTUTemperature();
  EXPECT_EQ(1, u.baseUnitExponent("R"));
  EXPECT_EQ(0, u.scale().exponent);
  EXPECT_EQ("R", u.standardString(false));
  EXPECT_EQ("", u.prettyString());

  u = createBTUPeople();
  EXPECT_EQ(1, u.baseUnitExponent("people"));
  EXPECT_EQ(0, u.scale().exponent);
  EXPECT_EQ("people", u.standardString(false));
  EXPECT_EQ("", u.prettyString());
  u.pow(-1);
  EXPECT_EQ("1/person", u.standardString(false));

  u = createBTUEnergy();
  EXPECT_EQ(0, u.scale().exponent);
  EXPECT_EQ("Btu", u.standardString(false));
  EXPECT_EQ("", u.prettyString());

  u = createBTUPower();
  EXPECT_EQ(0, u.scale().exponent);
  EXPECT_EQ("Btu/h", u.standardString(false));
  EXPECT_EQ("", u.prettyString());
}
