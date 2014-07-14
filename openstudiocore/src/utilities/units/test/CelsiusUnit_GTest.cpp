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

#include "../CelsiusUnit.hpp"
#include "../Scale.hpp"

#include "../../core/Exception.hpp"

using openstudio::Unit;
using openstudio::UnitSystem;
using openstudio::CelsiusUnit;
using openstudio::Exception;

using openstudio::createCelsiusTemperature;

TEST_F(UnitsFixture,CelsiusUnit_Constructors)
{
  LOG(Debug,"CelsiusUnit_Constructors");

  CelsiusUnit T1(1);

  EXPECT_EQ(1,T1.baseUnitExponent("C"));
  EXPECT_EQ(0,T1.baseUnitExponent("ft"));
  T1.setBaseUnitExponent("C",0);
  EXPECT_EQ(0,T1.baseUnitExponent("C"));

  CelsiusUnit T2(T1);
  EXPECT_EQ(UnitSystem::Celsius,T2.system().value());
}

TEST_F(UnitsFixture,CelsiusUnit_ArithmeticOperators)
{
  LOG(Debug,"CelsiusUnit_ArithmeticOperators");

  Unit T1 = CelsiusUnit(1);
  Unit T2 = CelsiusUnit(1);

  // /=
  T2 /= T1;
  EXPECT_EQ(0,T2.baseUnitExponent("C"));

  // *
  T2 = T1 * T1;
  EXPECT_EQ(2,T2.baseUnitExponent("C"));

  // /
  T2 = T2 / T1;
  EXPECT_EQ(1,T2.baseUnitExponent("C"));

  // pow
  T2 = pow(T1,3);
  EXPECT_EQ(3,T2.baseUnitExponent("C"));

}

TEST_F(UnitsFixture,CelsiusUnit_CreateFunctions)
{
  CelsiusUnit u;

  u = createCelsiusTemperature();
  EXPECT_EQ(1,u.baseUnitExponent("C"));
  EXPECT_EQ(0,u.scale().exponent);
  EXPECT_EQ("C",u.standardString());
  EXPECT_EQ("",u.prettyString());

}

TEST_F(UnitsFixture,CelsiusUnit_AbsoluteRelative) {
  CelsiusUnit T(1);
  EXPECT_TRUE(T.isAbsolute());
  T.setAsRelative();
  EXPECT_FALSE(T.isAbsolute());
}
