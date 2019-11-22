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

#include "../ExternalInterfaceFunctionalMockupUnitImportFromVariable.hpp"
#include "../ExternalInterfaceFunctionalMockupUnitImportFromVariable_Impl.hpp"
#include "../ExternalInterfaceFunctionalMockupUnitImport.hpp"
#include "../ExternalInterfaceFunctionalMockupUnitImport_Impl.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, ExternalInterfaceFunctionalMockupUnitImportFromVariable) {
  Model model;
  // add FMU
  ExternalInterfaceFunctionalMockupUnitImport eifmui(model, "test name");

  std::string outputVariableIndexKeyName = "outputVariableIndexKeyName";
  std::string outputVariableName = "outputVariableName";
  std::string fMUVariableName = "fMUVariableName";
  std::string fMUInstanceName = "fMUInstanceName";

  ExternalInterfaceFunctionalMockupUnitImportFromVariable variable(model, outputVariableIndexKeyName, outputVariableName, eifmui, fMUInstanceName, fMUVariableName);
  EXPECT_EQ(outputVariableIndexKeyName, variable.outputVariableIndexKeyName());
  EXPECT_EQ(outputVariableName, variable.outputVariableName());
  EXPECT_EQ(fMUVariableName, variable.fMUVariableName());
  EXPECT_EQ(fMUInstanceName, variable.fMUInstanceName());
  EXPECT_EQ("External Interface Functional Mockup Unit Import From Variable 1", variable.nameString());
  variable.setOutputVariableIndexKeyName("outputVariableIndexKeyName2");
  EXPECT_TRUE(variable.setOutputVariableName("outputVariableName2"));
  variable.setFMUVariableName("fMUVariableName2");
  variable.setFMUInstanceName("fMUInstanceName2");
  EXPECT_EQ("outputVariableIndexKeyName2", variable.outputVariableIndexKeyName());
  EXPECT_EQ("outputVariableName2", variable.outputVariableName());
  EXPECT_EQ("fMUVariableName2", variable.fMUVariableName());
  EXPECT_EQ("fMUInstanceName2", variable.fMUInstanceName());
  EXPECT_EQ(eifmui, variable.fMUFile());

  ExternalInterfaceFunctionalMockupUnitImport eifmui2(model, "test name 2");
  EXPECT_TRUE(variable.setFMUFile(eifmui2));
  EXPECT_EQ(eifmui2, variable.fMUFile());
}

