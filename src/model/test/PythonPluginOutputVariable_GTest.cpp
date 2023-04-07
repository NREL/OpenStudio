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

#include "../PythonPluginOutputVariable.hpp"
#include "../PythonPluginVariable.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, PythonPluginOutputVariable_GettersSetters) {
  Model m;

  PythonPluginVariable pyVar(m);
  PythonPluginOutputVariable pythonPluginOutputVariable(pyVar);
  EXPECT_EQ(pyVar, pythonPluginOutputVariable.pythonPluginVariable());
  EXPECT_EQ(1, pyVar.pythonPluginOutputVariables().size());

  pythonPluginOutputVariable.setName("My PythonPluginOutputVariable");

  // Python Plugin Variable Name: Required Object
  PythonPluginVariable pyVar2(m);
  EXPECT_TRUE(pythonPluginOutputVariable.setPythonPluginVariable(pyVar2));
  EXPECT_EQ(pyVar2, pythonPluginOutputVariable.pythonPluginVariable());

  // Type of Data in Variable: Required String
  EXPECT_TRUE(pythonPluginOutputVariable.setTypeofDatainVariable("Averaged"));
  EXPECT_EQ("Averaged", pythonPluginOutputVariable.typeofDatainVariable());
  // Bad Value
  EXPECT_FALSE(pythonPluginOutputVariable.setTypeofDatainVariable("BADENUM"));
  EXPECT_EQ("Averaged", pythonPluginOutputVariable.typeofDatainVariable());

  // Update Frequency: Required String
  EXPECT_TRUE(pythonPluginOutputVariable.setUpdateFrequency("ZoneTimestep"));
  EXPECT_EQ("ZoneTimestep", pythonPluginOutputVariable.updateFrequency());
  // Bad Value
  EXPECT_FALSE(pythonPluginOutputVariable.setUpdateFrequency("BADENUM"));
  EXPECT_EQ("ZoneTimestep", pythonPluginOutputVariable.updateFrequency());

  // Units: Optional String
  EXPECT_TRUE(pythonPluginOutputVariable.setUnits("C"));
  ASSERT_TRUE(pythonPluginOutputVariable.units());
  EXPECT_EQ("C", pythonPluginOutputVariable.units().get());

  // Resource Type: Optional String
  EXPECT_TRUE(pythonPluginOutputVariable.setResourceType("Electricity"));
  ASSERT_TRUE(pythonPluginOutputVariable.resourceType());
  EXPECT_EQ("Electricity", pythonPluginOutputVariable.resourceType().get());
  // Bad Value
  EXPECT_FALSE(pythonPluginOutputVariable.setResourceType("BADENUM"));
  ASSERT_TRUE(pythonPluginOutputVariable.resourceType());
  EXPECT_EQ("Electricity", pythonPluginOutputVariable.resourceType().get());

  // Group Type: Optional String
  EXPECT_TRUE(pythonPluginOutputVariable.setGroupType("Building"));
  ASSERT_TRUE(pythonPluginOutputVariable.groupType());
  EXPECT_EQ("Building", pythonPluginOutputVariable.groupType().get());
  // Bad Value
  EXPECT_FALSE(pythonPluginOutputVariable.setGroupType("BADENUM"));
  ASSERT_TRUE(pythonPluginOutputVariable.groupType());
  EXPECT_EQ("Building", pythonPluginOutputVariable.groupType().get());

  // End-Use Category: Optional String
  EXPECT_TRUE(pythonPluginOutputVariable.setEndUseCategory("Heating"));
  ASSERT_TRUE(pythonPluginOutputVariable.endUseCategory());
  EXPECT_EQ("Heating", pythonPluginOutputVariable.endUseCategory().get());
  // Bad Value
  EXPECT_FALSE(pythonPluginOutputVariable.setEndUseCategory("BADENUM"));
  ASSERT_TRUE(pythonPluginOutputVariable.endUseCategory());
  EXPECT_EQ("Heating", pythonPluginOutputVariable.endUseCategory().get());

  // End-Use Subcategory: Optional String
  EXPECT_TRUE(pythonPluginOutputVariable.setEndUseSubcategory("Space Heating"));
  ASSERT_TRUE(pythonPluginOutputVariable.endUseSubcategory());
  EXPECT_EQ("Space Heating", pythonPluginOutputVariable.endUseSubcategory().get());
}
