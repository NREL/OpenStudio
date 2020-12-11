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

#include "ModelFixture.hpp"

#include "../ExternalInterfaceFunctionalMockupUnitImport.hpp"
#include "../ExternalInterfaceFunctionalMockupUnitImport_Impl.hpp"

using namespace openstudio;
using namespace openstudio::model;
using std::string;

TEST_F(ModelFixture, ExternalInterfaceFunctionalMockupUnitImport) {
  Model model;

  ExternalInterfaceFunctionalMockupUnitImport eifmui(model, "test name");
  EXPECT_EQ("test name", eifmui.fMUFileName());
  EXPECT_TRUE(eifmui.isFMUTimeoutDefaulted());
  EXPECT_EQ(0.0, eifmui.fMUTimeout());
  EXPECT_TRUE(eifmui.isFMULoggingOnDefaulted());
  EXPECT_EQ(0, eifmui.fMULoggingOn());
  eifmui.setFMUTimeout(100);
  EXPECT_EQ(100.0, eifmui.fMUTimeout());
  eifmui.resetFMUTimeout();
  EXPECT_TRUE(eifmui.isFMUTimeoutDefaulted());
  eifmui.setFMULoggingOn(1);
  EXPECT_EQ(1, eifmui.fMULoggingOn());
  eifmui.setFMULoggingOn(2);
  EXPECT_EQ(2, eifmui.fMULoggingOn());
  eifmui.resetFMULoggingOn();
  EXPECT_TRUE(eifmui.isFMUTimeoutDefaulted());
  EXPECT_TRUE(eifmui.setFMUFileName("Test Name"));
  EXPECT_NE("test name", eifmui.fMUFileName());
  EXPECT_EQ("Test Name", eifmui.fMUFileName());
}

TEST_F(ModelFixture, ExternalInterfaceFunctionalMockupUnitImport2) {
  Model model;

  ExternalInterfaceFunctionalMockupUnitImport eifmui(model, "c:\\Program Files\\Test\\blah.fmu");
  EXPECT_EQ("c:\\Program Files\\Test\\blah.fmu", eifmui.fMUFileName());
  EXPECT_EQ("External Interface Functional Mockup Unit Import 1", eifmui.nameString());
  EXPECT_TRUE(eifmui.setFMUFileName("c:/Program Files/Test/blah.fmu"));
  EXPECT_EQ("c:/Program Files/Test/blah.fmu", eifmui.fMUFileName());
}
