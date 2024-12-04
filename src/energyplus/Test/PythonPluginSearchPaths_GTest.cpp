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
#include "../ReverseTranslator.hpp"

#include "../../model/PythonPluginSearchPaths.hpp"
#include "../../model/PythonPluginSearchPaths_Impl.hpp"

#include "../../utilities/idf/Workspace.hpp"
#include "../../utilities/idf/IdfObject.hpp"
#include "../../utilities/idf/WorkspaceObject.hpp"
// E+ FieldEnums
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/IddFactory.hxx>
#include <utilities/idd/PythonPlugin_SearchPaths_FieldEnums.hxx>
using namespace openstudio::energyplus;
using namespace openstudio::model;
using namespace openstudio;

TEST_F(EnergyPlusFixture, ForwardTranslator_PythonPluginSearchPaths) {

  ForwardTranslator ft;

  Model m;
  // TODO: Check regular Ctor arguments
  PythonPluginSearchPaths pythonPluginSearchPaths(m);
  // TODO: Or if a UniqueModelObject (and make sure _Impl is included)
  // PythonPluginSearchPaths pythonPluginSearchPaths = m.getUniqueModelObject<PythonPluginSearchPaths>();

  pythonPluginSearchPaths.setName("My PythonPluginSearchPaths");
  EXPECT_TRUE(pythonPluginSearchPaths.setAddCurrentWorkingDirectorytoSearchPath(false));  // Opposite from IDD default
  EXPECT_TRUE(pythonPluginSearchPaths.setAddInputFileDirectorytoSearchPath(false));  // Opposite from IDD default
  EXPECT_TRUE(pythonPluginSearchPaths.setAddepinEnvironmentVariabletoSearchPath(false));  // Opposite from IDD default


  // TODO: you're responsible for creating all other objects needed so this object actually gets ForwardTranslated

  const Workspace w = ft.translateModel(m);
  const auto idfObjs = w.getObjectsByType(IddObjectType::PythonPlugin_SearchPaths);
  ASSERT_EQ(1u, idfObjs.size());

  const auto& idfObject = idfObjs.front();
  EXPECT_EQ("No", idfObject.getString(PythonPlugin_SearchPathsFields::AddCurrentWorkingDirectorytoSearchPath).get());  EXPECT_EQ("No", idfObject.getString(PythonPlugin_SearchPathsFields::AddInputFileDirectorytoSearchPath).get());  EXPECT_EQ("No", idfObject.getString(PythonPlugin_SearchPathsFields::AddepinEnvironmentVariabletoSearchPath).get());
}
TEST_F(EnergyPlusFixture, ReverseTranslator_PythonPluginSearchPaths) {

  ReverseTranslator rt;

  Workspace w(StrictnessLevel::Minimal, IddFileType::EnergyPlus);

  auto woPPSP = w.addObject(IdfObject(IddObjectType::PythonPlugin_SearchPaths)).get();

  woPPSP.setName("My PythonPluginSearchPaths");
  EXPECT_TRUE(woPPSP.setString(PythonPlugin_SearchPathsFields::AddCurrentWorkingDirectorytoSearchPath, "No"));  // Opposite from IDD default
  EXPECT_TRUE(woPPSP.setString(PythonPlugin_SearchPathsFields::AddInputFileDirectorytoSearchPath, "No"));  // Opposite from IDD default
  EXPECT_TRUE(woPPSP.setString(PythonPlugin_SearchPathsFields::AddepinEnvironmentVariabletoSearchPath, "No"));  // Opposite from IDD default


  const Model m = rt.translateWorkspace(w);
  const auto modelObjects = m.getConcreteModelObjects<PythonPluginSearchPaths>();
  ASSERT_EQ(1u, modelObjects.size());

  const auto& modelObject = modelObjects.front();
  EXPECT_FALSE(modelObject.addCurrentWorkingDirectorytoSearchPath());  EXPECT_FALSE(modelObject.addInputFileDirectorytoSearchPath());  EXPECT_FALSE(modelObject.addepinEnvironmentVariabletoSearchPath());
}
