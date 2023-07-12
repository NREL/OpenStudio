/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>
#include "EnergyPlusFixture.hpp"

#include "../ForwardTranslator.hpp"
#include "../ReverseTranslator.hpp"

#include "../../model/Model.hpp"
#include "../../model/ExternalFile.hpp"
#include "../../model/ExternalFile_Impl.hpp"
#include "../../model/PythonPluginInstance.hpp"
#include "../../model/PythonPluginInstance_Impl.hpp"

#include "../../utilities/idf/Workspace.hpp"
#include "../../utilities/idf/IdfObject.hpp"
#include "../../utilities/idf/WorkspaceObject.hpp"
#include "../../utilities/idf/IdfExtensibleGroup.hpp"

#include "../../utilities/core/PathHelpers.hpp"

#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/PythonPlugin_Instance_FieldEnums.hxx>
#include <utilities/idd/PythonPlugin_SearchPaths_FieldEnums.hxx>
#include <utilities/idd/OS_PythonPlugin_Instance_FieldEnums.hxx>

#include <resources.hxx>

#include <sstream>

using namespace openstudio::energyplus;
using namespace openstudio::model;
using namespace openstudio;

TEST_F(EnergyPlusFixture, ForwardTranslator_PythonPluginInstance) {
  Model model;

  openstudio::path p = resourcesPath() / toPath("model/PythonPluginThermochromicWindow.py");
  ASSERT_TRUE(exists(p));

  // This is to avoid issues where if you change the original OpenStudio/resources/model/PythonPluginThermochromicWindow.py it will throw in
  // getExternalFile because the file exists and it doesn't have the same content
  path expectedDestDir;
  std::vector<path> absoluteFilePaths = model.workflowJSON().absoluteFilePaths();
  if (absoluteFilePaths.empty()) {
    expectedDestDir = model.workflowJSON().absoluteRootDir();
  } else {
    expectedDestDir = absoluteFilePaths[0];
  }

  if (exists(expectedDestDir)) {
    removeDirectory(expectedDestDir);
  }
  ASSERT_FALSE(exists(expectedDestDir));

  boost::optional<ExternalFile> externalfile = ExternalFile::getExternalFile(model, openstudio::toString(p));
  ASSERT_TRUE(externalfile) << "Path doesn't exist: '" << p << "'";

  PythonPluginInstance pythonPluginInstance1(*externalfile, "ZN_1_wall_south_Window_1_Control");
  pythonPluginInstance1.setName("PythonPlugin1");
  PythonPluginInstance pythonPluginInstance2(*externalfile, "AnotherDummyEnergyPlusPluginClass");
  pythonPluginInstance2.setName("PythonPlugin2");

  pythonPluginInstance2.setRunDuringWarmupDays(true);

  // Mimic the case where a user originally had set a correct plugin class name, but then went and modified the python script: we still want to catch
  // this during FT
  PythonPluginInstance ppInstanceOriginallyOK(*externalfile, "ZN_1_wall_south_Window_1_Control");
  ppInstanceOriginallyOK.setName("PythonPlugin_Outdated");
  EXPECT_TRUE(ppInstanceOriginallyOK.setString(OS_PythonPlugin_InstanceFields::PluginClassName, "SomethingOutdated"));

  ForwardTranslator ft;
  Workspace workspace = ft.translateModel(model);

  std::string errors;
  for (const auto& error : ft.errors()) {
    errors += error.logMessage() + "\n";
  }
  EXPECT_EQ(0, ft.errors().size()) << errors;

  std::string warnings;
  for (const auto& warning : ft.warnings()) {
    warnings += warning.logMessage() + "\n";
  }
  ASSERT_EQ(1, ft.warnings().size()) << warnings;
  EXPECT_EQ("PythonPluginInstance PythonPlugin_Outdated does not have plugin class name 'SomethingOutdated' contained in referenced external file",
            ft.warnings().front().logMessage());

  std::vector<WorkspaceObject> instanceObjects = workspace.getObjectsByType(IddObjectType::PythonPlugin_Instance);
  // The ppInstanceOriginallyOK never got translated
  ASSERT_EQ(2, instanceObjects.size());
  std::sort(instanceObjects.begin(), instanceObjects.end(), IdfObjectNameLess());
  WorkspaceObject woInstance1(instanceObjects[0]);
  WorkspaceObject woInstance2(instanceObjects[1]);

  ASSERT_EQ(pythonPluginInstance1.name().get(), woInstance1.getString(PythonPlugin_InstanceFields::Name, false).get());
  EXPECT_EQ("No", woInstance1.getString(PythonPlugin_InstanceFields::RunDuringWarmupDays, false).get());
  EXPECT_EQ("PythonPluginThermochromicWindow", woInstance1.getString(PythonPlugin_InstanceFields::PythonModuleName, false).get());
  EXPECT_EQ("ZN_1_wall_south_Window_1_Control", woInstance1.getString(PythonPlugin_InstanceFields::PluginClassName, false).get());

  ASSERT_EQ(pythonPluginInstance2.name().get(), woInstance2.getString(PythonPlugin_InstanceFields::Name, false).get());
  EXPECT_EQ("Yes", woInstance2.getString(PythonPlugin_InstanceFields::RunDuringWarmupDays, false).get());
  EXPECT_EQ("PythonPluginThermochromicWindow", woInstance2.getString(PythonPlugin_InstanceFields::PythonModuleName, false).get());
  EXPECT_EQ("AnotherDummyEnergyPlusPluginClass", woInstance2.getString(PythonPlugin_InstanceFields::PluginClassName, false).get());

  std::vector<WorkspaceObject> searchPathObjects = workspace.getObjectsByType(IddObjectType::PythonPlugin_SearchPaths);
  ASSERT_EQ(1, searchPathObjects.size());
  WorkspaceObject woSearchPaths(searchPathObjects[0]);
  EXPECT_EQ("Python Plugin Search Paths", woSearchPaths.nameString());
  EXPECT_EQ("Yes", woSearchPaths.getString(PythonPlugin_SearchPathsFields::AddCurrentWorkingDirectorytoSearchPath).get());
  EXPECT_EQ("Yes", woSearchPaths.getString(PythonPlugin_SearchPathsFields::AddInputFileDirectorytoSearchPath).get());
  EXPECT_EQ("Yes", woSearchPaths.getString(PythonPlugin_SearchPathsFields::AddepinEnvironmentVariabletoSearchPath).get());
  ASSERT_EQ(1, woSearchPaths.numExtensibleGroups());
  EXPECT_EQ(openstudio::toString(externalfile->filePath().parent_path()),
            woSearchPaths.extensibleGroups()[0].getString(PythonPlugin_SearchPathsExtensibleFields::SearchPath).get());
}
