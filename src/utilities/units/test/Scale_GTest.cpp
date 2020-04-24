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
#include "../Scale.hpp"

TEST_F(UnitsFixture,Scale_DefinedConstants) {

  LOG(Debug,"Scale_DefinedConstants");

  // tests << via the log macro
  LOG(Info,openstudio::notDefined());
  LOG(Info,openstudio::yotta());
  LOG(Info,openstudio::zetta());
  LOG(Info,openstudio::exa());
  LOG(Info,openstudio::peta());
  LOG(Info,openstudio::tera());
  LOG(Info,openstudio::giga());
  LOG(Info,openstudio::mega());
  LOG(Info,openstudio::kilo());
  LOG(Info,openstudio::one());
  LOG(Info,openstudio::centi());
  LOG(Info,openstudio::milli());
  LOG(Info,openstudio::micro());
  LOG(Info,openstudio::nano());
  LOG(Info,openstudio::pico());
  LOG(Info,openstudio::femto());
  LOG(Info,openstudio::atto());
  LOG(Info,openstudio::zepto());
  LOG(Info,openstudio::yocto());

}

TEST_F(UnitsFixture,Scale_LogicalOperators) {

  LOG(Debug,"Scale_LogicalOperators");

  // == and !=
  ASSERT_TRUE(openstudio::one() == openstudio::one());
  ASSERT_FALSE(openstudio::one() == openstudio::notDefined());
  ASSERT_TRUE(openstudio::kilo() != openstudio::micro());
  // <, <=, >, >=
  ASSERT_TRUE(openstudio::centi() < openstudio::one());
  ASSERT_TRUE(openstudio::mega() > openstudio::kilo());
  ASSERT_TRUE(openstudio::one() <= openstudio::giga());
  ASSERT_TRUE(openstudio::milli() >= openstudio::micro());

}
