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

#include "../CFMUnit.hpp"
#include "../Scale.hpp"

#include "../../core/Exception.hpp"

using openstudio::Exception;

using openstudio::CFMUnit;
using openstudio::CFMExpnt;

using openstudio::createCFMLength;
using openstudio::createCFMTime;
using openstudio::createCFMPower;
using openstudio::createCFMTemperature;
using openstudio::createCFMPeople;

using openstudio::createCFMVolumetricFlowrate;

TEST_F(UnitsFixture,CFMUnit_Constructors)
{
}

TEST_F(UnitsFixture,CFMUnit_ArithmeticOperators)
{
  // /=

  // *

  // /

  // pow

}

TEST_F(UnitsFixture,CFMUnit_CreateFunctions)
{
  CFMUnit u;

  u = createCFMLength();
  EXPECT_EQ(1,u.baseUnitExponent("ft"));
  EXPECT_EQ(0,u.scale().exponent);
  EXPECT_EQ("ft",u.standardString());
  EXPECT_EQ("",u.prettyString());

  u = createCFMTime();
  EXPECT_EQ(1,u.baseUnitExponent("min"));
  EXPECT_EQ(0,u.scale().exponent);
  EXPECT_EQ("min",u.standardString());
  EXPECT_EQ("",u.prettyString());

  u = createCFMPower();
  EXPECT_EQ(1,u.baseUnitExponent("ton"));
  EXPECT_EQ(0,u.scale().exponent);
  EXPECT_EQ("ton",u.standardString());
  EXPECT_EQ("",u.prettyString());

  u = createCFMTemperature();
  EXPECT_EQ(1,u.baseUnitExponent("R"));
  EXPECT_EQ(0,u.scale().exponent);
  EXPECT_EQ("R",u.standardString());
  EXPECT_EQ("",u.prettyString());

  u = createCFMPeople();
  EXPECT_EQ(1,u.baseUnitExponent("people"));
  EXPECT_EQ(0,u.scale().exponent);
  EXPECT_EQ("people",u.standardString());
  EXPECT_EQ("",u.prettyString());

  u = createCFMVolumetricFlowrate();
  EXPECT_EQ(3,u.baseUnitExponent("ft"));
  EXPECT_EQ(-1,u.baseUnitExponent("min"));
  EXPECT_EQ(0,u.scale().exponent);
  EXPECT_EQ("ft^3/min",u.standardString());
  EXPECT_EQ("cfm",u.prettyString());

}
