/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
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
