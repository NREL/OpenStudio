/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>

#include "ModelFixture.hpp"

#include "../ExternalInterfaceVariable.hpp"
#include "../ExternalInterfaceVariable_Impl.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, ExternalInterfaceVariable) {
  Model model;
  ExternalInterfaceVariable variable(model, "test name", 10);
  EXPECT_EQ(10.0, variable.initialValue());
  EXPECT_EQ("test name", variable.nameString());
  variable.setInitialValue(1);
  EXPECT_EQ(1.0, variable.initialValue());
  //Check BCVTB
  EXPECT_TRUE(variable.exportToBCVTB());
  EXPECT_TRUE(variable.isExportToBCVTBDefaulted());
  variable.setExportToBCVTB(false);
  EXPECT_FALSE(variable.exportToBCVTB());
  variable.resetExportToBCVTB();
  EXPECT_TRUE(variable.exportToBCVTB());
}
