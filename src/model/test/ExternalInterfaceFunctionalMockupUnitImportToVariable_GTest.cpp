/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
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
