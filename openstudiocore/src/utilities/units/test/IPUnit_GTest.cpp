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

#include "../IPUnit.hpp"
#include "../IPUnit_Impl.hpp"
#include "../Scale.hpp"

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

using openstudio::createIPForce;
using openstudio::createIPEnergy;
using openstudio::createIPPower;

TEST_F(UnitsFixture,IPUnit_Constructors)
{
  LOG(Debug,"IPUnit_Constructors");

  IPUnit u1;
  ASSERT_THROW(u1.setBaseUnitExponent("m",1),Exception);
  ASSERT_NO_THROW(u1.setBaseUnitExponent("ft",2));
  testStreamOutput("ft^2",u1);

  IPUnit u2(IPExpnt(1,1,-2));
  testStreamOutput("lb_m*ft/s^2",u2);
  u2.lbmToLbf();
  testStreamOutput("lb_f",u2);
  ASSERT_TRUE(u2.baseUnitExponent("lb_f") == 1);
  ASSERT_TRUE(u2.baseUnitExponent("lb_m") == 0);
  ASSERT_TRUE(u2.baseUnitExponent("m") == 0);

  u2.lbfToLbm();
  testStreamOutput("lb_m*ft/s^2",u2);
}

TEST_F(UnitsFixture,IPUnit_LogicalOperators)
{
  LOG(Debug,"IPUnit_LogicalOperators");

  IPUnit u1 = createIPForce();
  IPUnit u2(IPExpnt(1,1,-2));

  ASSERT_TRUE(u1 == u2);
  u2.setBaseUnitExponent("s",-3);
  ASSERT_TRUE(u1 != u2);
}

TEST_F(UnitsFixture,IPUnit_ArithmeticOperators)
{
  LOG(Debug,"IPUnit_ArithmeticOperators");

  IPUnit u1;
  IPUnit u2 = createIPForce();
  IPUnit u3(IPExpnt(1));

  u1 *= u3;
  testStreamOutput("lb_m",u1);
  Unit u4 = u2*u3;
  ASSERT_TRUE(u4.system() == UnitSystem::IP);
  testStreamOutput("lb_m*lb_f",u4);
  u4.cast<IPUnit>().lbfToLbm();
  testStreamOutput("lb_m^2*ft/s^2",u4);

  Unit u5 = u4/u3;
  EXPECT_TRUE(u5.system() == UnitSystem::IP);
  EXPECT_TRUE(u5 == u2);

  Unit u6 = pow(u5,-6);
  EXPECT_TRUE(u6.system() == UnitSystem::IP);
  testStreamOutput("s^12/lb_m^6*ft^6",u6);
  u6.pow(1,3);
  testStreamOutput("s^4/lb_m^2*ft^2",u6);
}

TEST_F(UnitsFixture,IPUnit_createFunctions)
{
  IPUnit u;

  u = createIPMass();
  EXPECT_EQ(1,u.baseUnitExponent("lb_m"));
  EXPECT_EQ(0,u.scale().exponent);
  EXPECT_EQ("lb_m",u.standardString());
  EXPECT_EQ("",u.prettyString());

  u = createIPLength();
  EXPECT_EQ(1,u.baseUnitExponent("ft"));
  EXPECT_EQ(0,u.scale().exponent);
  EXPECT_EQ("ft",u.standardString());
  EXPECT_EQ("",u.prettyString());

  u = createIPTime();
  EXPECT_EQ(1,u.baseUnitExponent("s"));
  EXPECT_EQ(0,u.scale().exponent);
  EXPECT_EQ("s",u.standardString());
  EXPECT_EQ("",u.prettyString());

  u = createIPTemperature();
  EXPECT_EQ(1,u.baseUnitExponent("R"));
  EXPECT_EQ(0,u.scale().exponent);
  EXPECT_EQ("R",u.standardString());
  EXPECT_EQ("",u.prettyString());

  u = createIPPeople();
  EXPECT_EQ(1,u.baseUnitExponent("people"));
  EXPECT_EQ(0,u.scale().exponent);
  EXPECT_EQ("people",u.standardString());
  EXPECT_EQ("",u.prettyString());
  u.pow(-1);
  EXPECT_EQ("1/person",u.standardString());

  u = createIPForce();
  EXPECT_EQ(0,u.scale().exponent);
  EXPECT_EQ("lb_f",u.standardString());
  EXPECT_EQ("",u.prettyString());

  u = createIPEnergy();
  EXPECT_EQ(0,u.scale().exponent);
  EXPECT_EQ("ft*lb_f",u.standardString());
  EXPECT_EQ("",u.prettyString());

  u = createIPPower();
  EXPECT_EQ(0,u.scale().exponent);
  EXPECT_EQ("ft*lb_f/s",u.standardString());
  EXPECT_EQ("",u.prettyString());

}
