/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "ModelFixture.hpp"

#include "../PythonPluginVariable.hpp"
#include "../PythonPluginVariable_Impl.hpp"

#include "../PythonPluginTrendVariable.hpp"
#include "../PythonPluginTrendVariable_Impl.hpp"
#include "../PythonPluginOutputVariable.hpp"
#include "../PythonPluginOutputVariable_Impl.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, PythonPluginVariable) {
  Model m;
  PythonPluginVariable var(m);
  var.setName("My PythonPluginVariable");

  EXPECT_EQ(0, var.pythonPluginTrendVariables().size());
  EXPECT_EQ(0, var.pythonPluginOutputVariables().size());

  PythonPluginTrendVariable pyTrendVar(var);
  EXPECT_EQ(var, pyTrendVar.pythonPluginVariable());
  EXPECT_EQ(1, var.pythonPluginTrendVariables().size());
  EXPECT_EQ(0, var.pythonPluginOutputVariables().size());

  PythonPluginOutputVariable pyOutVar(var);
  EXPECT_EQ(var, pyOutVar.pythonPluginVariable());
  EXPECT_EQ(1, var.pythonPluginTrendVariables().size());
  EXPECT_EQ(1, var.pythonPluginOutputVariables().size());

  PythonPluginOutputVariable pyOutVar2(var);
  EXPECT_EQ(var, pyOutVar2.pythonPluginVariable());
  EXPECT_EQ(1, var.pythonPluginTrendVariables().size());
  EXPECT_EQ(2, var.pythonPluginOutputVariables().size());

  EXPECT_EQ(1, m.getConcreteModelObjects<PythonPluginVariable>().size());
  EXPECT_EQ(1, m.getConcreteModelObjects<PythonPluginTrendVariable>().size());
  EXPECT_EQ(2, m.getConcreteModelObjects<PythonPluginOutputVariable>().size());

  // Clone
  auto varClone = var.clone(m).cast<PythonPluginVariable>();

  EXPECT_EQ(2, m.getConcreteModelObjects<PythonPluginVariable>().size());
  EXPECT_EQ(2, m.getConcreteModelObjects<PythonPluginTrendVariable>().size());
  EXPECT_EQ(4, m.getConcreteModelObjects<PythonPluginOutputVariable>().size());

  EXPECT_EQ(var, pyTrendVar.pythonPluginVariable());
  EXPECT_EQ(var, pyOutVar.pythonPluginVariable());
  EXPECT_EQ(var, pyOutVar2.pythonPluginVariable());
  EXPECT_EQ(1, var.pythonPluginTrendVariables().size());
  EXPECT_EQ(2, var.pythonPluginOutputVariables().size());

  ASSERT_EQ(1, varClone.pythonPluginTrendVariables().size());
  EXPECT_NE(pyTrendVar, varClone.pythonPluginTrendVariables().front());
  ASSERT_EQ(2, varClone.pythonPluginOutputVariables().size());
  EXPECT_NE(pyOutVar, varClone.pythonPluginOutputVariables().front());
  EXPECT_NE(pyOutVar2, varClone.pythonPluginOutputVariables().front());
  EXPECT_NE(pyOutVar, varClone.pythonPluginOutputVariables().back());
  EXPECT_NE(pyOutVar2, varClone.pythonPluginOutputVariables().back());

  pyOutVar.remove();
  EXPECT_EQ(2, m.getConcreteModelObjects<PythonPluginTrendVariable>().size());
  EXPECT_EQ(3, m.getConcreteModelObjects<PythonPluginOutputVariable>().size());
  EXPECT_EQ(var, pyTrendVar.pythonPluginVariable());
  EXPECT_EQ(var, pyOutVar2.pythonPluginVariable());
  ASSERT_EQ(1, var.pythonPluginTrendVariables().size());
  EXPECT_EQ(pyTrendVar, var.pythonPluginTrendVariables().front());
  ASSERT_EQ(1, var.pythonPluginOutputVariables().size());
  EXPECT_EQ(pyOutVar2, var.pythonPluginOutputVariables().front());

  // remove
  varClone.remove();

  EXPECT_EQ(1, m.getConcreteModelObjects<PythonPluginVariable>().size());
  EXPECT_EQ(1, m.getConcreteModelObjects<PythonPluginTrendVariable>().size());
  EXPECT_EQ(1, m.getConcreteModelObjects<PythonPluginOutputVariable>().size());
  ASSERT_EQ(1, var.pythonPluginTrendVariables().size());
  EXPECT_EQ(pyTrendVar, var.pythonPluginTrendVariables().front());
  ASSERT_EQ(1, var.pythonPluginOutputVariables().size());
  EXPECT_EQ(pyOutVar2, var.pythonPluginOutputVariables().front());

  // Clone other model
  Model m2;
  varClone = var.clone(m2).cast<PythonPluginVariable>();
  EXPECT_EQ(1, m.getConcreteModelObjects<PythonPluginTrendVariable>().size());
  EXPECT_EQ(1, m.getConcreteModelObjects<PythonPluginOutputVariable>().size());
  ASSERT_EQ(1, var.pythonPluginTrendVariables().size());
  EXPECT_EQ(pyTrendVar, var.pythonPluginTrendVariables().front());
  ASSERT_EQ(1, var.pythonPluginOutputVariables().size());
  EXPECT_EQ(pyOutVar2, var.pythonPluginOutputVariables().front());

  EXPECT_EQ(1, m2.getConcreteModelObjects<PythonPluginVariable>().size());
  EXPECT_EQ(1, m2.getConcreteModelObjects<PythonPluginTrendVariable>().size());
  EXPECT_EQ(1, m2.getConcreteModelObjects<PythonPluginOutputVariable>().size());
  ASSERT_EQ(1, varClone.pythonPluginTrendVariables().size());
  EXPECT_NE(pyTrendVar, varClone.pythonPluginTrendVariables().front());
  ASSERT_EQ(1, varClone.pythonPluginOutputVariables().size());
  EXPECT_NE(pyOutVar2, varClone.pythonPluginOutputVariables().front());
}
