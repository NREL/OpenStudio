/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>
#include "EnergyPlusFixture.hpp"

#include "../ForwardTranslator.hpp"
#include "../ReverseTranslator.hpp"

#include "../../model/Model.hpp"
#include "../../model/PythonPluginVariable.hpp"

#include "../../utilities/idf/Workspace.hpp"
#include "../../utilities/idf/IdfObject.hpp"
#include "../../utilities/idf/WorkspaceObject.hpp"
#include "../../utilities/idf/IdfExtensibleGroup.hpp"

#include <utilities/idd/PythonPlugin_Variables_FieldEnums.hxx>

#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::energyplus;
using namespace openstudio::model;
using namespace openstudio;

TEST_F(EnergyPlusFixture, ForwardTranslator_PythonPluginVariable) {

  ForwardTranslator ft;

  // Create a model
  Model m;

  PythonPluginVariable pyVar1(m);
  pyVar1.setName("PythonPluginVariable1");

  PythonPluginVariable pyVar2(m);
  pyVar1.setName("PythonPluginVariable2");

  Workspace w = ft.translateModel(m);
  WorkspaceObjectVector idfObjs = w.getObjectsByType(IddObjectType::PythonPlugin_Variables);
  ASSERT_EQ(1u, idfObjs.size());
  auto idfObj = idfObjs.front();
  auto egs = idfObj.extensibleGroups();
  ASSERT_EQ(2, egs.size());
  std::vector<std::string> writtenVarNames;
  bool pyVar1Found = false;
  bool pyVar2Found = false;

  for (const auto& eg : egs) {
    auto name = eg.getString(0).get();
    if (name == pyVar1.nameString()) {
      pyVar1Found = true;
    } else if (name == pyVar2.nameString()) {
      pyVar2Found = true;
    }
  }
  EXPECT_TRUE(pyVar1Found);
  EXPECT_TRUE(pyVar2Found);
}
