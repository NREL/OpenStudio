/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>
#include "EnergyPlusFixture.hpp"

#include "../ForwardTranslator.hpp"
#include "../ReverseTranslator.hpp"

#include "../../model/Model.hpp"
#include "../../model/PythonPluginOutputVariable.hpp"
#include "../../model/PythonPluginVariable.hpp"

#include "../../utilities/idf/Workspace.hpp"
#include "../../utilities/idf/IdfObject.hpp"
#include "../../utilities/idf/WorkspaceObject.hpp"
#include "../../utilities/idf/IdfExtensibleGroup.hpp"

#include <utilities/idd/PythonPlugin_Variables_FieldEnums.hxx>
#include <utilities/idd/PythonPlugin_OutputVariable_FieldEnums.hxx>

#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::energyplus;
using namespace openstudio::model;
using namespace openstudio;

TEST_F(EnergyPlusFixture, ForwardTranslator_PythonPluginOutputVariable) {

  ForwardTranslator ft;

  // Create a model
  Model m;

  PythonPluginVariable pyVar(m);
  pyVar.setName("PythonPluginVariable1");

  PythonPluginOutputVariable pyOutVar(pyVar);
  pyOutVar.setName("Chiller Electricity Consumption");
  EXPECT_TRUE(pyOutVar.setTypeofDatainVariable("Metered"));
  EXPECT_TRUE(pyOutVar.setUpdateFrequency("SystemTimestep"));
  EXPECT_TRUE(pyOutVar.setUnits("J"));
  EXPECT_TRUE(pyOutVar.setResourceType("Electricity"));
  EXPECT_TRUE(pyOutVar.setGroupType("HVAC"));
  EXPECT_TRUE(pyOutVar.setEndUseCategory("Cooling"));
  EXPECT_TRUE(pyOutVar.setEndUseSubcategory("Custom air cooled chiller"));

  Workspace w = ft.translateModel(m);
  EXPECT_EQ(1, w.getObjectsByType(IddObjectType::PythonPlugin_Variables).size());
  WorkspaceObjectVector idfObjs = w.getObjectsByType(IddObjectType::PythonPlugin_OutputVariable);
  ASSERT_EQ(1u, idfObjs.size());
  auto idfObj = idfObjs.front();

  EXPECT_EQ(pyOutVar.nameString(), idfObj.nameString());

  EXPECT_EQ("PythonPluginVariable1", idfObj.getString(PythonPlugin_OutputVariableFields::PythonPluginVariableName).get());
  EXPECT_EQ("Metered", idfObj.getString(PythonPlugin_OutputVariableFields::TypeofDatainVariable).get());
  EXPECT_EQ("SystemTimestep", idfObj.getString(PythonPlugin_OutputVariableFields::UpdateFrequency).get());
  EXPECT_EQ("J", idfObj.getString(PythonPlugin_OutputVariableFields::Units).get());
  EXPECT_EQ("Electricity", idfObj.getString(PythonPlugin_OutputVariableFields::ResourceType).get());
  EXPECT_EQ("HVAC", idfObj.getString(PythonPlugin_OutputVariableFields::GroupType).get());
  EXPECT_EQ("Cooling", idfObj.getString(PythonPlugin_OutputVariableFields::EndUseCategory).get());
  EXPECT_EQ("Custom air cooled chiller", idfObj.getString(PythonPlugin_OutputVariableFields::EndUseSubcategory).get());
}
