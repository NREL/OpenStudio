/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2023, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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
