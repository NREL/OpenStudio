/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>
#include "EnergyPlusFixture.hpp"

#include "../ForwardTranslator.hpp"

#include "../../model/Model.hpp"
#include "../../model/PythonPluginSearchPaths.hpp"
#include "../../model/PythonPluginSearchPaths_Impl.hpp"
#include "../../model/ExternalFile.hpp"
#include "../../model/ExternalFile_Impl.hpp"
#include "../../model/PythonPluginInstance.hpp"
#include "../../model/PythonPluginInstance_Impl.hpp"

#include "../../utilities/idf/Workspace.hpp"
#include "../../utilities/idf/IdfObject.hpp"
#include "../../utilities/idf/WorkspaceObject.hpp"
#include "../../utilities/idf/IdfExtensibleGroup.hpp"
#include "../../utilities/idf/WorkspaceExtensibleGroup.hpp"
#include "../../utilities/core/PathHelpers.hpp"
// E+ FieldEnums
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/IddFactory.hxx>
#include <utilities/idd/PythonPlugin_SearchPaths_FieldEnums.hxx>

#include <resources.hxx>

using namespace openstudio::energyplus;
using namespace openstudio::model;
using namespace openstudio;

TEST_F(EnergyPlusFixture, ForwardTranslator_PythonPluginSearchPaths) {

  ForwardTranslator ft;

  Model m;
  PythonPluginSearchPaths pythonPluginSearchPaths = m.getUniqueModelObject<PythonPluginSearchPaths>();

  pythonPluginSearchPaths.setName("My PythonPluginSearchPaths");

  EXPECT_TRUE(pythonPluginSearchPaths.searchPaths().empty());
  // No search paths; not translated
  {
    EXPECT_TRUE(pythonPluginSearchPaths.setAddCurrentWorkingDirectorytoSearchPath(false));  // Opposite from IDD default
    EXPECT_TRUE(pythonPluginSearchPaths.setAddInputFileDirectorytoSearchPath(false));       // Opposite from IDD default
    EXPECT_TRUE(pythonPluginSearchPaths.setAddepinEnvironmentVariabletoSearchPath(false));  // Opposite from IDD default

    const Workspace w = ft.translateModel(m);

    const auto idfObjs = w.getObjectsByType(IddObjectType::PythonPlugin_SearchPaths);
    ASSERT_EQ(0u, idfObjs.size());
  }

  {
    EXPECT_TRUE(pythonPluginSearchPaths.setAddCurrentWorkingDirectorytoSearchPath(true));
    EXPECT_TRUE(pythonPluginSearchPaths.setAddInputFileDirectorytoSearchPath(true));
    EXPECT_TRUE(pythonPluginSearchPaths.setAddepinEnvironmentVariabletoSearchPath(true));

    std::vector<std::string> searchPaths({"/path/to/lib1", "/path/to/lib2"});
    EXPECT_TRUE(pythonPluginSearchPaths.setSearchPaths(searchPaths));
    EXPECT_EQ(2u, pythonPluginSearchPaths.searchPaths().size());

    const Workspace w = ft.translateModel(m);

    const auto idfObjs = w.getObjectsByType(IddObjectType::PythonPlugin_SearchPaths);
    ASSERT_EQ(1u, idfObjs.size());

    const auto& idfObject = idfObjs.front();
    EXPECT_EQ("Yes", idfObject.getString(PythonPlugin_SearchPathsFields::AddCurrentWorkingDirectorytoSearchPath).get());
    EXPECT_EQ("Yes", idfObject.getString(PythonPlugin_SearchPathsFields::AddInputFileDirectorytoSearchPath).get());
    EXPECT_EQ("Yes", idfObject.getString(PythonPlugin_SearchPathsFields::AddepinEnvironmentVariabletoSearchPath).get());

    ASSERT_EQ(2u, idfObject.extensibleGroups().size());
    for (int i = 0; i < 2; ++i) {
      EXPECT_EQ(searchPaths[i], idfObject.extensibleGroups()[i].getString(PythonPlugin_SearchPathsExtensibleFields::SearchPath).get());
    }
  }

  {
    EXPECT_TRUE(pythonPluginSearchPaths.setAddCurrentWorkingDirectorytoSearchPath(false));  // Opposite from IDD default
    EXPECT_TRUE(pythonPluginSearchPaths.setAddInputFileDirectorytoSearchPath(false));       // Opposite from IDD default
    EXPECT_TRUE(pythonPluginSearchPaths.setAddepinEnvironmentVariabletoSearchPath(false));  // Opposite from IDD default

    std::vector<std::string> searchPaths({"/path/to/lib1", "/path/to/lib2"});
    EXPECT_TRUE(pythonPluginSearchPaths.setSearchPaths(searchPaths));
    EXPECT_EQ(2u, pythonPluginSearchPaths.searchPaths().size());

    const Workspace w = ft.translateModel(m);

    const auto idfObjs = w.getObjectsByType(IddObjectType::PythonPlugin_SearchPaths);
    ASSERT_EQ(1u, idfObjs.size());

    const auto& idfObject = idfObjs.front();
    EXPECT_EQ("No", idfObject.getString(PythonPlugin_SearchPathsFields::AddCurrentWorkingDirectorytoSearchPath).get());
    EXPECT_EQ("No", idfObject.getString(PythonPlugin_SearchPathsFields::AddInputFileDirectorytoSearchPath).get());
    EXPECT_EQ("No", idfObject.getString(PythonPlugin_SearchPathsFields::AddepinEnvironmentVariabletoSearchPath).get());

    ASSERT_EQ(2u, idfObject.extensibleGroups().size());
    for (int i = 0; i < 2; ++i) {
      EXPECT_EQ(searchPaths[i], idfObject.extensibleGroups()[i].getString(PythonPlugin_SearchPathsExtensibleFields::SearchPath).get());
    }
  }

  {
    EXPECT_TRUE(pythonPluginSearchPaths.setAddCurrentWorkingDirectorytoSearchPath(false));  // Opposite from IDD default
    EXPECT_TRUE(pythonPluginSearchPaths.setAddInputFileDirectorytoSearchPath(false));       // Opposite from IDD default
    EXPECT_TRUE(pythonPluginSearchPaths.setAddepinEnvironmentVariabletoSearchPath(false));  // Opposite from IDD default

    openstudio::path p = resourcesPath() / toPath("model/PythonPluginThermochromicWindow.py");
    ASSERT_TRUE(exists(p));

    boost::optional<ExternalFile> externalfile = ExternalFile::getExternalFile(m, openstudio::toString(p));
    ASSERT_TRUE(externalfile) << "Path doesn't exist: '" << p << "'";

    PythonPluginInstance pythonPluginInstance(*externalfile, "ZN_1_wall_south_Window_1_Control");

    std::vector<std::string> searchPaths({"/path/to/lib1", "/path/to/lib2"});
    EXPECT_TRUE(pythonPluginSearchPaths.setSearchPaths(searchPaths));
    EXPECT_EQ(2u, pythonPluginSearchPaths.searchPaths().size());

    const Workspace w = ft.translateModel(m);

    const auto idfObjs = w.getObjectsByType(IddObjectType::PythonPlugin_SearchPaths);
    ASSERT_EQ(1u, idfObjs.size());

    const auto& idfObject = idfObjs.front();
    EXPECT_EQ("No", idfObject.getString(PythonPlugin_SearchPathsFields::AddCurrentWorkingDirectorytoSearchPath).get());
    EXPECT_EQ("No", idfObject.getString(PythonPlugin_SearchPathsFields::AddInputFileDirectorytoSearchPath).get());
    EXPECT_EQ("No", idfObject.getString(PythonPlugin_SearchPathsFields::AddepinEnvironmentVariabletoSearchPath).get());

    ASSERT_EQ(3u, idfObject.extensibleGroups().size());
    for (int i = 0; i < 2; ++i) {
      EXPECT_EQ(searchPaths[i], idfObject.extensibleGroups()[i].getString(PythonPlugin_SearchPathsExtensibleFields::SearchPath).get());
    }
    // since PythonPlugin_Instance is translated after PythonPlugin_SearchPaths, this search path is appended to the existing search paths
    EXPECT_EQ(openstudio::toString(externalfile->filePath().parent_path()),
              idfObject.extensibleGroups()[2].getString(PythonPlugin_SearchPathsExtensibleFields::SearchPath).get());
  }
}
