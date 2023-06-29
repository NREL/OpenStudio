/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>

#include "UnitsFixture.hpp"
#include "../Scale.hpp"

using openstudio::notDefined;
using openstudio::yotta;
using openstudio::zetta;
using openstudio::exa;
using openstudio::peta;
using openstudio::tera;
using openstudio::giga;
using openstudio::mega;
using openstudio::kilo;
using openstudio::one;
using openstudio::centi;
using openstudio::milli;
using openstudio::micro;
using openstudio::nano;
using openstudio::pico;
using openstudio::femto;
using openstudio::atto;
using openstudio::zepto;
using openstudio::yocto;

TEST_F(UnitsFixture, Scale_DefinedConstants) {

  LOG(Debug, "Scale_DefinedConstants");

  // tests << via the log macro
  LOG(Info, notDefined());
  LOG(Info, yotta());
  LOG(Info, zetta());
  LOG(Info, exa());
  LOG(Info, peta());
  LOG(Info, tera());
  LOG(Info, giga());
  LOG(Info, mega());
  LOG(Info, kilo());
  LOG(Info, one());
  LOG(Info, centi());
  LOG(Info, milli());
  LOG(Info, micro());
  LOG(Info, nano());
  LOG(Info, pico());
  LOG(Info, femto());
  LOG(Info, atto());
  LOG(Info, zepto());
  LOG(Info, yocto());
}

TEST_F(UnitsFixture, Scale_LogicalOperators) {

  LOG(Debug, "Scale_LogicalOperators");

  // == and !=
  ASSERT_TRUE(one() == one());
  ASSERT_FALSE(one() == notDefined());
  ASSERT_TRUE(kilo() != micro());
  // <, <=, >, >=
  ASSERT_TRUE(centi() < one());
  ASSERT_TRUE(mega() > kilo());
  ASSERT_TRUE(one() <= giga());
  ASSERT_TRUE(milli() >= micro());
}
