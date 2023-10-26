/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>
#include "EnergyPlusFixture.hpp"

#include "../ForwardTranslator.hpp"
#include "../ReverseTranslator.hpp"

#include "../../model/Model.hpp"
#include "../../model/PythonPluginTrendVariable.hpp"
#include "../../model/PythonPluginVariable.hpp"

#include "../../utilities/idf/Workspace.hpp"
#include "../../utilities/idf/IdfObject.hpp"
#include "../../utilities/idf/WorkspaceObject.hpp"
#include "../../utilities/idf/IdfExtensibleGroup.hpp"

#include <utilities/idd/PythonPlugin_Variables_FieldEnums.hxx>
#include <utilities/idd/PythonPlugin_TrendVariable_FieldEnums.hxx>

#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::energyplus;
using namespace openstudio::model;
using namespace openstudio;

TEST_F(EnergyPlusFixture, ForwardTranslator_PythonPluginTrendVariable) {

  ForwardTranslator ft;

  // Create a model
  Model m;

  PythonPluginVariable pyVar(m);
  pyVar.setName("PythonPluginVariable1");

  PythonPluginTrendVariable pyTrendVar(pyVar);
  pyTrendVar.setName("PythonPluginTrendVariable");
  EXPECT_TRUE(pyTrendVar.setNumberofTimestepstobeLogged(10));

  Workspace w = ft.translateModel(m);
  EXPECT_EQ(1, w.getObjectsByType(IddObjectType::PythonPlugin_Variables).size());
  WorkspaceObjectVector idfObjs = w.getObjectsByType(IddObjectType::PythonPlugin_TrendVariable);
  ASSERT_EQ(1u, idfObjs.size());
  auto idfObj = idfObjs.front();

  EXPECT_EQ(pyTrendVar.nameString(), idfObj.getString(PythonPlugin_TrendVariableFields::Name).get());
  EXPECT_EQ(pyVar.nameString(), idfObj.getString(PythonPlugin_TrendVariableFields::NameofaPythonPluginVariable).get());
  EXPECT_EQ(10, idfObj.getInt(PythonPlugin_TrendVariableFields::NumberofTimestepstobeLogged).get());
}
