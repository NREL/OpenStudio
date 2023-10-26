/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>

#include "ModelFixture.hpp"

#include "../ExternalInterfaceFunctionalMockupUnitExportFromVariable.hpp"
#include "../ExternalInterfaceFunctionalMockupUnitExportFromVariable_Impl.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, ExternalInterfaceFunctionalMockupUnitExportFromVariable) {
  Model model;

  std::string outputVariableIndexKeyName = "outputVariableIndexKeyName";
  std::string outputVariableName = "outputVariableName";
  std::string fMUVariableName = "fMUVariableName";

  ExternalInterfaceFunctionalMockupUnitExportFromVariable variable(model, outputVariableIndexKeyName, outputVariableName, fMUVariableName);
  EXPECT_EQ(outputVariableIndexKeyName, variable.outputVariableIndexKeyName());
  EXPECT_EQ(outputVariableName, variable.outputVariableName());
  EXPECT_EQ(fMUVariableName, variable.fMUVariableName());
  EXPECT_EQ("External Interface Functional Mockup Unit Export From Variable 1", variable.nameString());
  variable.setOutputVariableIndexKeyName("outputVariableIndexKeyName2");
  EXPECT_TRUE(variable.setOutputVariableName("outputVariableName2"));
  variable.setFMUVariableName("fMUVariableName2");
  EXPECT_EQ("outputVariableIndexKeyName2", variable.outputVariableIndexKeyName());
  EXPECT_EQ("outputVariableName2", variable.outputVariableName());
  EXPECT_EQ("fMUVariableName2", variable.fMUVariableName());
}
