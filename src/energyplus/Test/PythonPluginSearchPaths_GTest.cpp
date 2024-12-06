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
  EXPECT_TRUE(pythonPluginSearchPaths.setAddCurrentWorkingDirectorytoSearchPath(false));  // Opposite from IDD default
  EXPECT_TRUE(pythonPluginSearchPaths.setAddInputFileDirectorytoSearchPath(false));       // Opposite from IDD default
  EXPECT_TRUE(pythonPluginSearchPaths.setAddepinEnvironmentVariabletoSearchPath(false));  // Opposite from IDD default

  EXPECT_TRUE(pythonPluginSearchPaths.searchPaths().empty());
  // No search paths; not translated
  {
    const Workspace w = ft.translateModel(m);

    const auto idfObjs = w.getObjectsByType(IddObjectType::PythonPlugin_SearchPaths);
    ASSERT_EQ(0u, idfObjs.size());
  }

  {
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
