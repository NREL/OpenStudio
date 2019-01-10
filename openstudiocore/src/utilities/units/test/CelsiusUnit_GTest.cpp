/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2019, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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
