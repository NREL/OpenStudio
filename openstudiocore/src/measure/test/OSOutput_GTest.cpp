/**********************************************************************
*  Copyright (c) 2008-2016, Alliance for Sustainable Energy.
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
#include "MeasureFixture.hpp"

#include "../OSOutput.hpp"

#include <vector>
#include <map>

using namespace openstudio;
using namespace openstudio::measure;


TEST_F(MeasureFixture, OSOutput) {

  OSOutput boolOutput = OSOutput::makeBoolOutput("bool");
  OSOutput doubleOutput = OSOutput::makeDoubleOutput("double");
  OSOutput integerOutput = OSOutput::makeIntegerOutput("integer");
  OSOutput stringOutput = OSOutput::makeStringOutput("string");

  EXPECT_EQ(OSOutputType::Boolean, boolOutput.type().value());
  EXPECT_EQ(OSOutputType::Integer, integerOutput.type().value());
  EXPECT_EQ(OSOutputType::Double, doubleOutput.type().value());
  EXPECT_EQ(OSOutputType::String, stringOutput.type().value());

  std::stringstream os;
  os << boolOutput;
  os << integerOutput;
  os << doubleOutput;
  os << stringOutput;
}