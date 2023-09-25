/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
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
