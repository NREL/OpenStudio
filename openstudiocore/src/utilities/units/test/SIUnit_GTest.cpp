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

#include "../SIUnit.hpp"
#include "../Scale.hpp"

#include "../../core/Exception.hpp"

using openstudio::Unit;
using openstudio::UnitSystem;
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

TEST_F(UnitsFixture,SIUnit_Constructors)
{
  LOG(Debug,"SIUnit_Constructors");

  // default
  SIUnit u1;
  ASSERT_NO_THROW(u1.setBaseUnitExponent("m",1));
  ASSERT_THROW(u1.setBaseUnitExponent("ft",2),Exception);
  testStreamOutput("m",u1);

  // default with pretty string
  SIUnit u2(SIExpnt(1,1,-2,0),0,"N");
  testStreamOutput("N",u2);
  // scale exponent and pretty string
  SIUnit u3(SIExpnt(1,1,-2,0),3,"N");
  testStreamOutput("kN",u3);
  EXPECT_TRUE(u3.baseUnitExponent("kg") == 1);
  EXPECT_TRUE(u3.baseUnitExponent("K") == 0);
  EXPECT_TRUE(u3.baseUnitExponent("ft") == 0);

  u2.setScale("\\mu");
  testStreamOutput("\\muN",u2);

  // scale abbreviation
  SIUnit u4("c",SIExpnt(0,0,0,1));
  testStreamOutput("cK",u4);
  EXPECT_EQ(1,u4.baseUnitExponent("K"));
}

TEST_F(UnitsFixture,SIUnit_BaseUnits)
{
  SIUnit u1;
  EXPECT_TRUE(u1.isBaseUnit("s"));
  EXPECT_TRUE(u1.isBaseUnit("cd"));
  EXPECT_FALSE(u1.isBaseUnit("ft"));
  ASSERT_THROW(u1.setBaseUnitExponent("Z",1),Exception);
}

TEST_F(UnitsFixture,SIUnit_LogicalOperators)
{
  LOG(Debug,"SIUnit_LogicalOperators");

  SIUnit u1;
  SIUnit u2(SIExpnt(1,1,-2));
  SIUnit u3(SIExpnt(1,1,-2,0),3);
  
  EXPECT_TRUE(u2 == u3);
  EXPECT_FALSE(u1 == u3);
}

TEST_F(UnitsFixture,SIUnit_ArithmeticOperators)
{
  LOG(Debug,"SIUnit_ArithmeticOperators");

  SIUnit u1;
  SIUnit u2(SIExpnt(1,1,-2,0),0,"N");
  SIUnit u3(SIExpnt(1)); // kg
  SIUnit u4(SIExpnt(0,1,0,0),-2); // cm

  // multiplication
  u1 *= u3; // kg
  testStreamOutput("kg",u1);
  Unit u5 = u2*u3; // N*kg
  EXPECT_TRUE(u5.system() == UnitSystem::SI);
  testStreamOutput("N*kg",u5);
  Unit u6 = u2*u4; // cJ
  EXPECT_TRUE(u6.system() == UnitSystem::SI);
  testStreamOutput("cJ",u6);

  // division
  u6 /= u4;
  EXPECT_TRUE(u6 == u2);  

  // power
  Unit u7 = pow(u4,6); // (cm)^6
  EXPECT_TRUE(u7.system() == UnitSystem::SI);
  testStreamOutput("cm^6",u7);
  EXPECT_EQ("p",u7.scale().abbr);
  u7.pow(1,3); // (cm)^2
  testStreamOutput("m(m^2)",u7); // no 10^-4 scale--moves to 10^-3
}

TEST_F(UnitsFixture,SIUnit_createFunctions)
{
  SIUnit u;
  
  u = createSIMass();
  EXPECT_EQ(1,u.baseUnitExponent("kg"));
  EXPECT_EQ(0,u.scale().exponent);
  EXPECT_EQ("kg",u.standardString());
  EXPECT_EQ("",u.prettyString());

  u = createSILength();
  EXPECT_EQ(1,u.baseUnitExponent("m"));
  EXPECT_EQ(0,u.scale().exponent);
  EXPECT_EQ("m",u.standardString());
  EXPECT_EQ("",u.prettyString());

  u = createSITime();
  EXPECT_EQ(1,u.baseUnitExponent("s"));
  EXPECT_EQ(0,u.scale().exponent);
  EXPECT_EQ("s",u.standardString());
  EXPECT_EQ("",u.prettyString());

  u = createSITemperature();
  EXPECT_EQ(1,u.baseUnitExponent("K"));
  EXPECT_EQ(0,u.scale().exponent);
  EXPECT_EQ("K",u.standardString());
  EXPECT_EQ("",u.prettyString());

  u = createSIPeople();
  EXPECT_EQ(1,u.baseUnitExponent("people"));
  EXPECT_EQ(0,u.scale().exponent);
  EXPECT_EQ("people",u.standardString());
  EXPECT_EQ("",u.prettyString());
  u.pow(-1);
  EXPECT_EQ("1/person",u.standardString());

  u = createSIForce();
  EXPECT_EQ(0,u.scale().exponent);
  EXPECT_EQ("kg*m/s^2",u.standardString());
  EXPECT_EQ("N",u.prettyString());

  u = createSIEnergy();
  EXPECT_EQ(0,u.scale().exponent);
  EXPECT_EQ("kg*m^2/s^2",u.standardString());
  EXPECT_EQ("J",u.prettyString());

  u = createSIPower();
  EXPECT_EQ(0,u.scale().exponent);
  EXPECT_EQ("kg*m^2/s^3",u.standardString());
  EXPECT_EQ("W",u.prettyString());

}
