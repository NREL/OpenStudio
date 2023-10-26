/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>

#include "ModelFixture.hpp"

#include "../ExternalInterfaceFunctionalMockupUnitExportToVariable.hpp"
#include "../ExternalInterfaceFunctionalMockupUnitExportToVariable_Impl.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, ExternalInterfaceFunctionalMockupUnitExportToVariable) {
  Model model;

  ExternalInterfaceFunctionalMockupUnitExportToVariable variable(model, "FMU name", 10);
  EXPECT_EQ(10.0, variable.initialValue());
  EXPECT_EQ("External Interface Functional Mockup Unit Export To Variable 1", variable.nameString());
  variable.setInitialValue(1);
  EXPECT_EQ(1.0, variable.initialValue());
  EXPECT_EQ("FMU name", variable.fMUVariableName());
  variable.setFMUVariableName("test 2");
  EXPECT_EQ("test 2", variable.fMUVariableName());
}
