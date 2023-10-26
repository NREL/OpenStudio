/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>

#include "ModelFixture.hpp"
#include "../Model_Impl.hpp"

#include "../ShadingSurfaceGroup.hpp"
#include "../ShadingSurfaceGroup_Impl.hpp"
#include "../ShadingSurface.hpp"
#include "../ShadingSurface_Impl.hpp"

using namespace openstudio::model;
using namespace openstudio;

TEST_F(ModelFixture, ShadingSurface) {
  Model model;

  Point3dVector points{
    {0, 1, 0},
    {0, 0, 0},
    {1, 0, 0},
    {1, 1, 0},
  };

  EXPECT_NO_THROW(ShadingSurface(points, model));

  ShadingSurface shadingSurface(points, model);
  EXPECT_FALSE(shadingSurface.shadingSurfaceGroup());
}

TEST_F(ModelFixture, ShadingSurface_isXXXAutocalculated_4399) {
  // Test for #4399
  Model m;
  std::vector<Point3d> points{
    Point3d{0, 1, 0},
    Point3d{0, 0, 0},
    Point3d{1, 0, 0},
    Point3d{1, 1, 0},
  };

  ShadingSurface shadingSurface(points, m);

  shadingSurface.autocalculateNumberofVertices();
  EXPECT_TRUE(shadingSurface.isNumberofVerticesAutocalculated());
}
