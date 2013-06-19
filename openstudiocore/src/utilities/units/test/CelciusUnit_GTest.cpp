/**********************************************************************
*  Copyright (c) 2008-2013, Alliance for Sustainable Energy.  
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
#include <utilities/units/test/UnitsFixture.hpp>

#include <utilities/units/CelciusUnit.hpp>
#include <utilities/units/Scale.hpp>

#include <utilities/core/Exception.hpp>

using openstudio::Unit;
using openstudio::UnitSystem;
using openstudio::CelciusUnit;
using openstudio::Exception;

using openstudio::createCelciusTemperature;

TEST_F(UnitsFixture,CelciusUnit_Constructors)
{
  LOG(Debug,"CelciusUnit_Constructors");

  CelciusUnit T1(1);

  EXPECT_EQ(1,T1.baseUnitExponent("C"));
  EXPECT_EQ(0,T1.baseUnitExponent("ft"));
  T1.setBaseUnitExponent("C",0);
  EXPECT_EQ(0,T1.baseUnitExponent("C"));

  CelciusUnit T2(T1);
  EXPECT_EQ(UnitSystem::Celcius,T2.system().value());
}

TEST_F(UnitsFixture,CelciusUnit_ArithmeticOperators)
{
  LOG(Debug,"CelciusUnit_ArithmeticOperators");

  Unit T1 = CelciusUnit(1);
  Unit T2 = CelciusUnit(1);

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

TEST_F(UnitsFixture,CelciusUnit_CreateFunctions)
{
  CelciusUnit u;

  u = createCelciusTemperature();
  EXPECT_EQ(1,u.baseUnitExponent("C"));
  EXPECT_EQ(0,u.scale().exponent);
  EXPECT_EQ("C",u.standardString());
  EXPECT_EQ("",u.prettyString());

}

TEST_F(UnitsFixture,CelciusUnit_AbsoluteRelative) {
  CelciusUnit T(1);
  EXPECT_TRUE(T.isAbsolute());
  T.setAsRelative();
  EXPECT_FALSE(T.isAbsolute());
}
