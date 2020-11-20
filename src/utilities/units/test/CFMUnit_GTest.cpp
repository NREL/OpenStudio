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

TEST_F(UnitsFixture, CFMUnit_Constructors) {}

TEST_F(UnitsFixture, CFMUnit_ArithmeticOperators) {
  // /=

  // *

  // /

  // pow
}

TEST_F(UnitsFixture, CFMUnit_CreateFunctions) {
  CFMUnit u;

  u = createCFMLength();
  EXPECT_EQ(1, u.baseUnitExponent("ft"));
  EXPECT_EQ(0, u.scale().exponent);
  EXPECT_EQ("ft", u.standardString());
  EXPECT_EQ("", u.prettyString());

  u = createCFMTime();
  EXPECT_EQ(1, u.baseUnitExponent("min"));
  EXPECT_EQ(0, u.scale().exponent);
  EXPECT_EQ("min", u.standardString());
  EXPECT_EQ("", u.prettyString());

  u = createCFMPower();
  EXPECT_EQ(1, u.baseUnitExponent("ton"));
  EXPECT_EQ(0, u.scale().exponent);
  EXPECT_EQ("ton", u.standardString());
  EXPECT_EQ("", u.prettyString());

  u = createCFMTemperature();
  EXPECT_EQ(1, u.baseUnitExponent("R"));
  EXPECT_EQ(0, u.scale().exponent);
  EXPECT_EQ("R", u.standardString());
  EXPECT_EQ("", u.prettyString());

  u = createCFMPeople();
  EXPECT_EQ(1, u.baseUnitExponent("people"));
  EXPECT_EQ(0, u.scale().exponent);
  EXPECT_EQ("people", u.standardString());
  EXPECT_EQ("", u.prettyString());

  u = createCFMVolumetricFlowrate();
  EXPECT_EQ(3, u.baseUnitExponent("ft"));
  EXPECT_EQ(-1, u.baseUnitExponent("min"));
  EXPECT_EQ(0, u.scale().exponent);
  EXPECT_EQ("ft^3/min", u.standardString());
  EXPECT_EQ("cfm", u.prettyString());
}
