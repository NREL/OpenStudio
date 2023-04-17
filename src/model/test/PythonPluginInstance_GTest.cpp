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

#include "ModelFixture.hpp"
#include "../ExternalFile.hpp"
#include "../ExternalFile_Impl.hpp"
#include "../PythonPluginInstance.hpp"
#include "../PythonPluginInstance_Impl.hpp"

#include "../../utilities/core/PathHelpers.hpp"

using namespace openstudio::model;
using namespace openstudio;

TEST_F(ModelFixture, PythonPluginInstance) {
  Model model;
  EXPECT_EQ(0u, model.getConcreteModelObjects<ExternalFile>().size());
  EXPECT_EQ(0u, model.getConcreteModelObjects<PythonPluginInstance>().size());

  path p = resourcesPath() / toPath("model/PythonPluginThermochromicWindow.py");
  EXPECT_TRUE(exists(p));

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
  ASSERT_TRUE(externalfile);
  EXPECT_EQ(1u, model.getConcreteModelObjects<ExternalFile>().size());
  EXPECT_EQ(0u, externalfile->pythonPluginInstances().size());
  EXPECT_EQ(openstudio::toString(p.filename()), externalfile->fileName());
  EXPECT_TRUE(equivalent(expectedDestDir / externalfile->fileName(), externalfile->filePath()));
  EXPECT_TRUE(exists(externalfile->filePath()));
  EXPECT_NE(p, externalfile->filePath());

  PythonPluginInstance plugin(*externalfile, "ZN_1_wall_south_Window_1_Control");
  EXPECT_EQ(1u, model.getConcreteModelObjects<PythonPluginInstance>().size());
  EXPECT_EQ(1u, externalfile->pythonPluginInstances().size());
  EXPECT_EQ(externalfile->handle(), plugin.externalFile().handle());
  EXPECT_EQ("ZN_1_wall_south_Window_1_Control", plugin.pluginClassName());

  EXPECT_FALSE(plugin.runDuringWarmupDays());
  EXPECT_TRUE(plugin.isRunDuringWarmupDaysDefaulted());
  plugin.setRunDuringWarmupDays(true);
  EXPECT_TRUE(plugin.runDuringWarmupDays());
  EXPECT_FALSE(plugin.isRunDuringWarmupDaysDefaulted());
  plugin.resetRunDuringWarmupDays();
  EXPECT_FALSE(plugin.runDuringWarmupDays());
  EXPECT_TRUE(plugin.isRunDuringWarmupDaysDefaulted());

  PythonPluginInstance plugin2(*externalfile, "ZN_1_wall_south_Window_1_Control");
  EXPECT_EQ(2u, model.getConcreteModelObjects<PythonPluginInstance>().size());
  EXPECT_EQ(2u, externalfile->pythonPluginInstances().size());
  EXPECT_EQ(externalfile->handle(), plugin2.externalFile().handle());

  plugin.remove();
  EXPECT_EQ(1u, model.getConcreteModelObjects<ExternalFile>().size());
  EXPECT_EQ(1u, model.getConcreteModelObjects<PythonPluginInstance>().size());
  EXPECT_EQ(1u, externalfile->pythonPluginInstances().size());

  path filePath = externalfile->filePath();
  EXPECT_TRUE(exists(p));
  EXPECT_TRUE(exists(filePath));

  externalfile->remove();
  EXPECT_EQ(0u, model.getConcreteModelObjects<ExternalFile>().size());
  EXPECT_EQ(0u, model.getConcreteModelObjects<PythonPluginInstance>().size());
  EXPECT_EQ(0u, externalfile->pythonPluginInstances().size());

  EXPECT_TRUE(exists(p));
  EXPECT_FALSE(exists(filePath));
}

TEST_F(ModelFixture, PythonPluginInstance_NotPYFile) {
  Model model;

  path p = resourcesPath() / toPath("model/7-7_Windows_Complete.osm");
  EXPECT_TRUE(exists(p));

  boost::optional<ExternalFile> externalfile = ExternalFile::getExternalFile(model, openstudio::toString(p));
  ASSERT_TRUE(externalfile);

  ASSERT_THROW(PythonPluginInstance plugin(*externalfile, "ZN_1_wall_south_Window_1_Control"), openstudio::Exception);
}

TEST_F(ModelFixture, PythonPluginInstance_ClassNameValidation) {
  Model model;

  path p = resourcesPath() / toPath("model/PythonPluginThermochromicWindow.py");
  EXPECT_TRUE(exists(p));

  boost::optional<ExternalFile> externalfile = ExternalFile::getExternalFile(model, openstudio::toString(p));
  ASSERT_TRUE(externalfile);

  // ctor
  EXPECT_ANY_THROW(PythonPluginInstance(*externalfile, "ZN_1_wall_east_Window_1_Control"));

  // Start with a valid one
  PythonPluginInstance plugin(*externalfile, "ZN_1_wall_south_Window_1_Control");
  // Then try to set an invalid one
  EXPECT_FALSE(plugin.setPluginClassName("ZN_1_wall_west_Window_1_Control"));
  EXPECT_EQ("ZN_1_wall_south_Window_1_Control", plugin.pluginClassName());

  // Try another valid one
  EXPECT_TRUE(plugin.setPluginClassName("AnotherDummyEnergyPlusPluginClass"));
  EXPECT_EQ("AnotherDummyEnergyPlusPluginClass", plugin.pluginClassName());

  // Try with a support class that doesn't derive EnergyPlusPlugin
  EXPECT_FALSE(plugin.setPluginClassName("MySupportClassThatDoesntDeriveEnergyPlusPlugin"));
  EXPECT_EQ("AnotherDummyEnergyPlusPluginClass", plugin.pluginClassName());

  auto validClassNames = plugin.validPluginClassNamesInFile();
  ASSERT_EQ(2, validClassNames.size());
  EXPECT_EQ("ZN_1_wall_south_Window_1_Control", validClassNames[0]);
  EXPECT_EQ("AnotherDummyEnergyPlusPluginClass", validClassNames[1]);
}
