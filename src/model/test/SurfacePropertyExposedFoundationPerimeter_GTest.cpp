/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>

#include "ModelFixture.hpp"

#include "../Model.hpp"
#include "../Model_Impl.hpp"

#include "../SurfacePropertyExposedFoundationPerimeter.hpp"
#include "../SurfacePropertyExposedFoundationPerimeter_Impl.hpp"

#include "../Surface.hpp"
#include "../Surface_Impl.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, SurfacePropertyExposedFoundationPerimeter_SurfacePropertyExposedFoundationPerimeter) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  // create a surface object to use
  Point3dVector points{
    {0, 2, 0},
    {0, 0, 0},
    {1, 0, 0},
  };

  ASSERT_EXIT(
    {
      // create a model to use
      Model model;

      Surface surface(points, model);

      exit(0);
    },
    ::testing::ExitedWithCode(0), "");

  // create a model to use
  Model model;

  Surface surface(points, model);
  EXPECT_EQ(1, model.modelObjects().size());

  // new surface does not have surface property exposed foundation perimeter yet
  EXPECT_TRUE(!surface.surfacePropertyExposedFoundationPerimeter());

  // create a surface property exposed foundation perimeter object to use
  boost::optional<SurfacePropertyExposedFoundationPerimeter> optprop =
    surface.createSurfacePropertyExposedFoundationPerimeter("TotalExposedPerimeter", 100);
  ASSERT_TRUE(optprop);
  EXPECT_EQ(2, model.modelObjects().size());

  // now the surface has surface property exposed foundation perimeter
  EXPECT_TRUE(surface.surfacePropertyExposedFoundationPerimeter());

  // check perimeter properties were defaulted properly
  auto prop = optprop.get();
  EXPECT_EQ("TotalExposedPerimeter", prop.exposedPerimeterCalculationMethod());
  boost::optional<double> opttotalexposedperimeter = prop.totalExposedPerimeter();
  EXPECT_TRUE(opttotalexposedperimeter);
  EXPECT_TRUE(prop.isExposedPerimeterFractionDefaulted());
  EXPECT_EQ(1, prop.exposedPerimeterFraction());

  // check that creating the surface property exposed foundation perimeter when they already exists does nothing and returns nil
  boost::optional<SurfacePropertyExposedFoundationPerimeter> optprop2 =
    surface.createSurfacePropertyExposedFoundationPerimeter("TotalExposedPerimeter", 100);
  ASSERT_FALSE(optprop2);
  EXPECT_EQ(2, model.modelObjects().size());
}

// test setting and getting
TEST_F(ModelFixture, SurfacePropertyExposedFoundationPerimeter_SetGetFields) {
  Model model;
  Point3dVector points;
  points = {
    {0, 2, 0},
    {0, 0, 0},
    {1, 0, 0},
  };
  Surface surface(points, model);
  boost::optional<SurfacePropertyExposedFoundationPerimeter> optprop =
    surface.createSurfacePropertyExposedFoundationPerimeter("ExposedPerimeterFraction", 1);
  auto prop = optprop.get();
  EXPECT_FALSE(prop.totalExposedPerimeter());

  // now override the defaults with explicit values
  prop.setExposedPerimeterFraction(0.8);

  EXPECT_EQ("ExposedPerimeterFraction", prop.exposedPerimeterCalculationMethod());
  boost::optional<double> opttotalexposedperimeter = prop.totalExposedPerimeter();
  EXPECT_FALSE(opttotalexposedperimeter);
  ASSERT_FALSE(prop.isExposedPerimeterFractionDefaulted());
  EXPECT_EQ(0.8, prop.exposedPerimeterFraction());

  // test setting back to defaults
  prop.resetExposedPerimeterFraction();
  ASSERT_TRUE(prop.isExposedPerimeterFractionDefaulted());
  EXPECT_EQ(1, prop.exposedPerimeterFraction());
}

// check that parent reset works
TEST_F(ModelFixture, SurfacePropertyExposedFoundationPerimeter_ParentReset) {
  Model model;
  Point3dVector points;
  points = {
    {0, 2, 0},
    {0, 0, 0},
    {1, 0, 0},
  };
  Surface surface(points, model);
  auto size = model.modelObjects().size();
  boost::optional<SurfacePropertyExposedFoundationPerimeter> optprop =
    surface.createSurfacePropertyExposedFoundationPerimeter("TotalExposedPerimeter", 100);
  surface.resetSurfacePropertyExposedFoundationPerimeter();
  EXPECT_FALSE(surface.surfacePropertyExposedFoundationPerimeter());
  EXPECT_EQ(size, model.modelObjects().size());
}

// check that parent remove works
TEST_F(ModelFixture, SurfacePropertyExposedFoundationPerimeter_ParentRemove) {
  Model model;
  Point3dVector points;
  points = {
    {0, 2, 0},
    {0, 0, 0},
    {1, 0, 0},
  };
  auto size = model.modelObjects().size();
  Surface surface(points, model);
  boost::optional<SurfacePropertyExposedFoundationPerimeter> optprop =
    surface.createSurfacePropertyExposedFoundationPerimeter("TotalExposedPerimeter", 100);
  EXPECT_FALSE(surface.remove().empty());
  EXPECT_EQ(size, model.modelObjects().size());
}

// check that child remove works
TEST_F(ModelFixture, SurfacePropertyExposedFoundationPerimeter_ChildRemove) {
  Model model;
  Point3dVector points;
  points = {
    {0, 2, 0},
    {0, 0, 0},
    {1, 0, 0},
  };
  Surface surface(points, model);
  auto size = model.modelObjects().size();
  boost::optional<SurfacePropertyExposedFoundationPerimeter> optprop =
    surface.createSurfacePropertyExposedFoundationPerimeter("TotalExposedPerimeter", 100);
  auto prop = optprop.get();
  EXPECT_EQ(2, model.modelObjects().size());
  EXPECT_FALSE(prop.remove().empty());
  EXPECT_EQ(size, model.modelObjects().size());
}

// check that it points to the surface
TEST_F(ModelFixture, SurfacePropertyExposedFoundationPerimeter_SurfaceName) {
  Model model;
  Point3dVector points;
  points = {
    {0, 2, 0},
    {0, 0, 0},
    {1, 0, 0},
  };
  Surface surface(points, model);
  boost::optional<SurfacePropertyExposedFoundationPerimeter> optprop =
    surface.createSurfacePropertyExposedFoundationPerimeter("TotalExposedPerimeter", 100);
  auto prop = optprop.get();
  EXPECT_EQ("Surface 1", prop.surfaceName());
  EXPECT_EQ(surface.handle(), prop.surface().handle());
  Surface surface2(points, model);
  boost::optional<SurfacePropertyExposedFoundationPerimeter> optprop2 =
    surface2.createSurfacePropertyExposedFoundationPerimeter("TotalExposedPerimeter", 100);
  auto prop2 = optprop2.get();
  EXPECT_EQ("Surface 2", prop2.surfaceName());
  EXPECT_EQ(surface2.handle(), prop2.surface().handle());
}
