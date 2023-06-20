/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>

#include "ModelFixture.hpp"
#include "../EnergyManagementSystemTrendVariable.hpp"
#include "../EnergyManagementSystemGlobalVariable.hpp"
#include "../Model_Impl.hpp"

#include "../../utilities/idd/IddEnums.hpp"
#include "../../utilities/idf/ValidityReport.hpp"
#include "../../utilities/idf/IdfObject.hpp"
#include "../../utilities/data/TimeSeries.hpp"
#include "../../utilities/core/Compare.hpp"
#include "../../utilities/core/Optional.hpp"

using namespace openstudio;
using namespace openstudio::model;
using std::string;

TEST_F(ModelFixture, EMSTrendVariable_EMSTrendVariable) {
  Model model;

  // add global variable
  EnergyManagementSystemGlobalVariable globvar(model, "glob_var");

  // add trend variable
  EnergyManagementSystemTrendVariable var(model, "glob_var");
  var.setName("TestName");
  EXPECT_EQ("TestName", var.name().get());
  //var.setEMSVariableName("glob_var");
  EXPECT_EQ("glob_var", var.emsVariableName());

  var.setNumberOfTimestepsToBeLogged(2);
  EXPECT_EQ(2, var.numberOfTimestepsToBeLogged());

  EXPECT_EQ(globvar, var.emsGlobalVariable().get());

  // add global variable
  EnergyManagementSystemGlobalVariable globvar2(model, "glob_var2");

  EnergyManagementSystemTrendVariable var2(model, globvar2);
  EXPECT_EQ("glob_var2", var2.emsVariableName());
  ASSERT_TRUE(var2.emsVariableObject());
  ASSERT_TRUE(var2.emsGlobalVariable());
  EXPECT_EQ(globvar2, var2.emsGlobalVariable().get());
  EXPECT_EQ(var2.emsVariableObject().get(), var2.emsGlobalVariable().get());
}
