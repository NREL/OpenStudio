/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2023, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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
#include "BCLFixture.hpp"

#include "../../core/StringStreamLogSink.hpp"

#include "../BCLComponent.hpp"

using namespace openstudio;

TEST_F(BCLFixture, BCLComponent) {
  std::string dir = toString(resourcesPath()) + "/utilities/BCL/Components/philadelphia pa [724086 TMY2-13739]/";
  BCLComponent component(dir);

  // "_" has been replaced by " "
  EXPECT_EQ("philadelphia pa [724086 TMY2-13739]", component.name());
}

TEST_F(BCLFixture, BCLComponent_WrongType_float) {
  // Test for #323
  openstudio::path compDir = resourcesPath() / "utilities/BCL/Components/323_wrongXMLType_float";

  StringStreamLogSink sink;
  sink.setLogLevel(Error);

  EXPECT_THROW(BCLComponent{compDir}, openstudio::Exception);
  ASSERT_EQ(1, sink.logMessages().size());
  EXPECT_EQ("Please double check your XML, you have an attribute named 'Effective R-value' with a datatype of 'float' but the value isn't a double: "
            "'THIS IS A MISTAKE'.",
            sink.logMessages()[0].logMessage());
}

TEST_F(BCLFixture, BCLComponent_WrongType_int) {
  // Test for #323
  openstudio::path compDir = resourcesPath() / "utilities/BCL/Components/323_wrongXMLType_int";

  StringStreamLogSink sink;
  sink.setLogLevel(Error);

  EXPECT_THROW(BCLComponent{compDir}, openstudio::Exception);
  ASSERT_EQ(1, sink.logMessages().size());
  EXPECT_EQ("Please double check your XML, you have an attribute named 'Effective R-value' with a datatype of 'int' but the value isn't an integer: "
            "'THIS IS A MISTAKE'.",
            sink.logMessages()[0].logMessage());
}
