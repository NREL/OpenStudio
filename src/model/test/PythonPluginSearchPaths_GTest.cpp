/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>

#include "ModelFixture.hpp"

#include "../PythonPluginSearchPaths.hpp"
#include "../PythonPluginSearchPaths_Impl.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, PythonPluginSearchPaths_GettersSetters) {
  Model m;
  PythonPluginSearchPaths pythonPluginSearchPaths = m.getUniqueModelObject<PythonPluginSearchPaths>();

  // Add Current Working Directory to Search Path: Required Boolean
  EXPECT_TRUE(pythonPluginSearchPaths.setAddCurrentWorkingDirectorytoSearchPath(true));
  EXPECT_TRUE(pythonPluginSearchPaths.addCurrentWorkingDirectorytoSearchPath());
  EXPECT_TRUE(pythonPluginSearchPaths.setAddCurrentWorkingDirectorytoSearchPath(false));
  EXPECT_FALSE(pythonPluginSearchPaths.addCurrentWorkingDirectorytoSearchPath());

  // Add Input File Directory to Search Path: Required Boolean
  EXPECT_TRUE(pythonPluginSearchPaths.setAddInputFileDirectorytoSearchPath(true));
  EXPECT_TRUE(pythonPluginSearchPaths.addInputFileDirectorytoSearchPath());
  EXPECT_TRUE(pythonPluginSearchPaths.setAddInputFileDirectorytoSearchPath(false));
  EXPECT_FALSE(pythonPluginSearchPaths.addInputFileDirectorytoSearchPath());

  // Add epin Environment Variable to Search Path: Required Boolean
  EXPECT_TRUE(pythonPluginSearchPaths.setAddepinEnvironmentVariabletoSearchPath(true));
  EXPECT_TRUE(pythonPluginSearchPaths.addepinEnvironmentVariabletoSearchPath());
  EXPECT_TRUE(pythonPluginSearchPaths.setAddepinEnvironmentVariabletoSearchPath(false));
  EXPECT_FALSE(pythonPluginSearchPaths.addepinEnvironmentVariabletoSearchPath());

  // Search Paths
  EXPECT_TRUE(pythonPluginSearchPaths.searchPaths().empty());

  EXPECT_TRUE(pythonPluginSearchPaths.addSearchPath("/example/search/path/1"));
  ASSERT_EQ(1u, pythonPluginSearchPaths.searchPaths().size());
  EXPECT_EQ("/example/search/path/1", pythonPluginSearchPaths.searchPaths()[0]);

  EXPECT_TRUE(pythonPluginSearchPaths.addSearchPath("/example/search/path/1"));
  ASSERT_EQ(1u, pythonPluginSearchPaths.searchPaths().size());
  EXPECT_TRUE(pythonPluginSearchPaths.addSearchPath("/example/search/path/2"));
  ASSERT_EQ(2u, pythonPluginSearchPaths.searchPaths().size());
  EXPECT_EQ("/example/search/path/1", pythonPluginSearchPaths.searchPaths()[0]);
  EXPECT_EQ("/example/search/path/2", pythonPluginSearchPaths.searchPaths()[1]);

  std::vector<std::string> searchPaths({"path/3", "path/4", "path/5", "path/5"});
  EXPECT_TRUE(pythonPluginSearchPaths.setSearchPaths(searchPaths));
  ASSERT_EQ(3u, pythonPluginSearchPaths.searchPaths().size());
  EXPECT_EQ("path/3", pythonPluginSearchPaths.searchPaths()[0]);
  EXPECT_EQ("path/4", pythonPluginSearchPaths.searchPaths()[1]);
  EXPECT_EQ("path/5", pythonPluginSearchPaths.searchPaths()[2]);

  pythonPluginSearchPaths.clearSearchPaths();
  EXPECT_EQ(0u, pythonPluginSearchPaths.searchPaths().size());
}
