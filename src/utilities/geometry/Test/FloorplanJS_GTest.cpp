/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>
#include "GeometryFixture.hpp"

#include "../FloorplanJS.hpp"
#include "../ThreeJS.hpp"

#include <resources.hxx>

using namespace openstudio;

TEST_F(GeometryFixture, FloorplanJS) {
  openstudio::path p = resourcesPath() / toPath("utilities/Geometry/floorplan.json");
  ASSERT_TRUE(exists(p));

  std::ifstream ifs(toSystemFilename(p));
  EXPECT_TRUE(ifs.is_open());

  std::istreambuf_iterator<char> eos;
  std::string contents(std::istreambuf_iterator<char>(ifs), eos);
  ifs.close();
  EXPECT_FALSE(contents.empty());

  boost::optional<FloorplanJS> floorplan = FloorplanJS::load(contents);
  ASSERT_TRUE(floorplan);

  EXPECT_DOUBLE_EQ(floorplan->northAxis(), 30.0);
  EXPECT_DOUBLE_EQ(floorplan->latitude(), 40.0);
  EXPECT_DOUBLE_EQ(floorplan->longitude(), -104.0);
  EXPECT_DOUBLE_EQ(floorplan->elevation(), 5280.0);

  floorplan = FloorplanJS::load(toString(p));
  ASSERT_TRUE(floorplan);

  EXPECT_DOUBLE_EQ(floorplan->northAxis(), 30.0);
  EXPECT_DOUBLE_EQ(floorplan->latitude(), 40.0);
  EXPECT_DOUBLE_EQ(floorplan->longitude(), -104.0);
  EXPECT_DOUBLE_EQ(floorplan->elevation(), 5280.0);

  ThreeScene scene = floorplan->toThreeScene(true);
  std::string json = scene.toJSON(false);
  openstudio::path out = resourcesPath() / toPath("utilities/Geometry/floorplan_threejs_triangulated.json");
  {
    openstudio::filesystem::ofstream file(out);
    ASSERT_TRUE(file.is_open());
    file << json;
    file.close();
  }

  scene = floorplan->toThreeScene(false);
  json = scene.toJSON(false);
  out = resourcesPath() / toPath("utilities/Geometry/floorplan_threejs_nontriangulated.json");
  {
    openstudio::filesystem::ofstream file(out);
    ASSERT_TRUE(file.is_open());
    file << json;
    file.close();
  }
}
