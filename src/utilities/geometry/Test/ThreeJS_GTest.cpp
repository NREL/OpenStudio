/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>
#include "GeometryFixture.hpp"

#include "../ThreeJS.hpp"

#include <resources.hxx>

using namespace openstudio;

TEST_F(GeometryFixture, ThreeJS) {
  openstudio::path p = resourcesPath() / toPath("utilities/Geometry/threejs.json");
  ASSERT_TRUE(exists(p));

  std::ifstream ifs(toSystemFilename(p));
  EXPECT_TRUE(ifs.is_open());

  std::istreambuf_iterator<char> eos;
  std::string contents(std::istreambuf_iterator<char>(ifs), eos);
  ifs.close();
  EXPECT_FALSE(contents.empty());

  boost::optional<ThreeScene> scene = ThreeScene::load(contents);
  ASSERT_TRUE(scene);

  // checking user data
  std::vector<ThreeSceneChild> sceneChildren = scene->object().children();
  for (const ThreeSceneChild& sceneChild : sceneChildren) {
    EXPECT_EQ(1, sceneChild.userData().airLoopHVACNames().size());
  }

  // checking metadata
  std::vector<ThreeModelObjectMetadata> metadatas = scene->metadata().modelObjectMetadata();
  EXPECT_TRUE(std::any_of(metadatas.cbegin(), metadatas.cend(), [](const auto& metadata) {
    return istringEqual("OS:AirLoopHVAC", metadata.iddObjectType()) && istringEqual("Air Loop HVAC 1", metadata.name());
  }));

  scene = ThreeScene::load(toString(p));
  ASSERT_TRUE(scene);

  std::string json = scene->toJSON();
  EXPECT_FALSE(json.empty());

  scene = ThreeScene::load(toString(p));
  ASSERT_TRUE(scene);
}
