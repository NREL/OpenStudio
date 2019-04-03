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

#include "ModelFixture.hpp"

#include "../ExternalInterfaceFunctionalMockupUnitImportToVariable.hpp"
#include "../ExternalInterfaceFunctionalMockupUnitImportToVariable_Impl.hpp"
#include "../ExternalInterfaceFunctionalMockupUnitImport.hpp"
#include "../ExternalInterfaceFunctionalMockupUnitImport_Impl.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, ExternalInterfaceFunctionalMockupUnitImportToVariable) {
  Model model;
  // add FMU
  ExternalInterfaceFunctionalMockupUnitImport eifmui(model, "test name");

  ExternalInterfaceFunctionalMockupUnitImportToVariable variable(model, eifmui, "FMU", "FMU name", 10);
  EXPECT_EQ(10.0, variable.initialValue());
  EXPECT_EQ("External Interface Functional Mockup Unit Import To Variable 1", variable.nameString());
  variable.setInitialValue(1);
  EXPECT_EQ(1.0, variable.initialValue());
  EXPECT_EQ("FMU", variable.fMUInstanceName());
  EXPECT_EQ("FMU name", variable.fMUVariableName());
  variable.setFMUInstanceName("test 1");
  variable.setFMUVariableName("test 2");
  EXPECT_EQ("test 1", variable.fMUInstanceName());
  EXPECT_EQ("test 2", variable.fMUVariableName());

  ExternalInterfaceFunctionalMockupUnitImport eifmui2(model, "test name 2");
  EXPECT_TRUE(variable.setFMUFile(eifmui2));
  EXPECT_EQ(eifmui2, variable.fMUFile());
}

