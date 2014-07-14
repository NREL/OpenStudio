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

TEST_F(UnitsFixture,BTUUnit_Constructors)
{
  LOG(Debug,"BTUUnit_Constructors");

  BTUUnit l1(BTUExpnt(0,1));
  BTUUnit P1(BTUExpnt(1,0,-1,0),-3); // mBtu/h
  BTUUnit E1("k",BTUExpnt(1)); // kBtu

  EXPECT_EQ(0,l1.baseUnitExponent("Btu"));
  EXPECT_EQ(1,l1.baseUnitExponent("ft"));
  l1.setBaseUnitExponent("R",-1);
  EXPECT_EQ(-1,l1.baseUnitExponent("R"));
  EXPECT_THROW(l1.setBaseUnitExponent("m",1),Exception);

  EXPECT_EQ("Btu/h",P1.standardString(false));
  EXPECT_EQ("mBtu/h",P1.standardString());
  EXPECT_EQ(UnitSystem::BTU,P1.system().value());
  EXPECT_EQ(0,P1.baseUnitExponent("R"));

  BTUUnit P2(P1);
  EXPECT_EQ(UnitSystem::BTU,P2.system().value());

  EXPECT_EQ("",E1.prettyString());
  EXPECT_EQ("kBtu",E1.standardString());
  EXPECT_EQ(0,E1.baseUnitExponent("ft"));
}

TEST_F(UnitsFixture,BTUUnit_ArithmeticOperators)
{
  LOG(Debug,"BTUUnit_ArithmeticOperators");

  // /=
  BTUUnit P1(BTUExpnt(1));
  BTUUnit t1(BTUExpnt(0,0,1));
  P1 /= t1;
  EXPECT_EQ("Btu/h",P1.standardString(false));
  EXPECT_EQ(1,P1.baseUnitExponent("Btu"));
  EXPECT_EQ(-1,P1.baseUnitExponent("h"));
  EXPECT_EQ(0,P1.baseUnitExponent("ft"));
  EXPECT_EQ(0,P1.baseUnitExponent("m"));

  // *
  Unit E1 = P1 * t1;
  EXPECT_TRUE(E1.system() == UnitSystem::BTU);
  EXPECT_EQ("Btu",E1.standardString(false));
  EXPECT_EQ("",E1.prettyString());

  // /
  Unit u = P1/E1;
  u /= t1;
  EXPECT_TRUE(u.system() == UnitSystem::BTU);
  EXPECT_EQ("1/h^2",u.standardString(false));
  EXPECT_EQ(-2,u.baseUnitExponent("h"));
  EXPECT_EQ(0,u.baseUnitExponent("Btu"));

  // pow
  u.pow(-1,2);
  EXPECT_EQ("h",u.standardString(false));
  EXPECT_EQ(1,u.baseUnitExponent("h"));

}

TEST_F(UnitsFixture,BTUUnit_CreateFunctions)
{
  BTUUnit u;

  u = createBTULength();
  EXPECT_EQ(1,u.baseUnitExponent("ft"));
  EXPECT_EQ(0,u.scale().exponent);
  EXPECT_EQ("ft",u.standardString(false));
  EXPECT_EQ("",u.prettyString());

  u = createBTUTime();
  EXPECT_EQ(1,u.baseUnitExponent("h"));
  EXPECT_EQ(0,u.scale().exponent);
  EXPECT_EQ("h",u.standardString(false));
  EXPECT_EQ("",u.prettyString());

  u = createBTUTemperature();
  EXPECT_EQ(1,u.baseUnitExponent("R"));
  EXPECT_EQ(0,u.scale().exponent);
  EXPECT_EQ("R",u.standardString(false));
  EXPECT_EQ("",u.prettyString());

  u = createBTUPeople();
  EXPECT_EQ(1,u.baseUnitExponent("people"));
  EXPECT_EQ(0,u.scale().exponent);
  EXPECT_EQ("people",u.standardString(false));
  EXPECT_EQ("",u.prettyString());
  u.pow(-1);
  EXPECT_EQ("1/person",u.standardString(false));

  u = createBTUEnergy();
  EXPECT_EQ(0,u.scale().exponent);
  EXPECT_EQ("Btu",u.standardString(false));
  EXPECT_EQ("",u.prettyString());

  u = createBTUPower();
  EXPECT_EQ(0,u.scale().exponent);
  EXPECT_EQ("Btu/h",u.standardString(false));
  EXPECT_EQ("",u.prettyString());

}
