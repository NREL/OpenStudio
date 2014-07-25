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

#include "../FahrenheitUnit.hpp"
#include "../Scale.hpp"

#include "../../core/Exception.hpp"

using openstudio::Unit;
using openstudio::UnitSystem;
using openstudio::FahrenheitUnit;
using openstudio::Exception;

using openstudio::createFahrenheitTemperature;

TEST_F(UnitsFixture,FahrenheitUnit_Constructors)
{
  LOG(Debug,"FahrenheitUnit_Constructors");

  FahrenheitUnit T1(1);

  EXPECT_EQ(1,T1.baseUnitExponent("F"));
  EXPECT_EQ(0,T1.baseUnitExponent("ft"));
  T1.setBaseUnitExponent("F",0);
  EXPECT_EQ(0,T1.baseUnitExponent("F"));

  FahrenheitUnit T2(T1);
  EXPECT_EQ(UnitSystem::Fahrenheit,T2.system().value());
}

TEST_F(UnitsFixture,FahrenheitUnit_ArithmeticOperators)
{
  LOG(Debug,"FahrenheitUnit_ArithmeticOperators");

  Unit T1 = FahrenheitUnit(1);
  Unit T2 = FahrenheitUnit(1);

  // /=
  T2 /= T1;
  EXPECT_EQ(0,T2.baseUnitExponent("F"));

  // *
  T2 = T1 * T1;
  EXPECT_EQ(2,T2.baseUnitExponent("F"));

  // /
  T2 = T2 / T1;
  EXPECT_EQ(1,T2.baseUnitExponent("F"));

  // pow
  T2 = pow(T1,3);
  EXPECT_EQ(3,T2.baseUnitExponent("F"));

}

TEST_F(UnitsFixture,FahrenheitUnit_CreateFunctions)
{
  FahrenheitUnit u;

  u = createFahrenheitTemperature();
  EXPECT_EQ(1,u.baseUnitExponent("F"));
  EXPECT_EQ(0,u.scale().exponent);
  EXPECT_EQ("F",u.standardString());
  EXPECT_EQ("",u.prettyString());

}

TEST_F(UnitsFixture,FahrenheitUnit_AbsoluteRelative) {
  FahrenheitUnit T(1);
  EXPECT_TRUE(T.isAbsolute());
  T.setAsRelative();
  EXPECT_FALSE(T.isAbsolute());
}
