/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
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

  ExternalInterfaceFunctionalMockupUnitImportFromVariable variable(model, outputVariableIndexKeyName, outputVariableName, eifmui, fMUInstanceName,
                                                                   fMUVariableName);
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
