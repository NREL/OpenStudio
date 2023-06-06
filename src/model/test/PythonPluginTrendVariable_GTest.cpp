/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "ModelFixture.hpp"

#include "../PythonPluginTrendVariable.hpp"
#include "../PythonPluginVariable.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, PythonPluginTrendVariable_GettersSetters) {
  Model m;

  PythonPluginVariable pyVar(m);

  PythonPluginTrendVariable pythonPluginTrendVariable(pyVar);
  EXPECT_EQ(pyVar, pythonPluginTrendVariable.pythonPluginVariable());

  pythonPluginTrendVariable.setName("My PythonPluginTrendVariable");

  // Name of a Python Plugin Variable: Required Object
  PythonPluginVariable pyVar2(m);
  EXPECT_TRUE(pythonPluginTrendVariable.setPythonPluginVariable(pyVar2));
  EXPECT_EQ(pyVar2, pythonPluginTrendVariable.pythonPluginVariable());

  // Number of Timesteps to be Logged: Required Integer
  EXPECT_TRUE(pythonPluginTrendVariable.setNumberofTimestepstobeLogged(5));
  EXPECT_EQ(5, pythonPluginTrendVariable.numberofTimestepstobeLogged());
  // Bad Value
  EXPECT_FALSE(pythonPluginTrendVariable.setNumberofTimestepstobeLogged(-9));
  EXPECT_EQ(5, pythonPluginTrendVariable.numberofTimestepstobeLogged());
}
