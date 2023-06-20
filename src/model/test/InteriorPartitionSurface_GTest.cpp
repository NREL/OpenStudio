/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>

#include "ModelFixture.hpp"

#include "../InteriorPartitionSurface.hpp"
#include "../InteriorPartitionSurface_Impl.hpp"
#include "../InteriorPartitionSurfaceGroup.hpp"
#include "../InteriorPartitionSurfaceGroup_Impl.hpp"

using namespace openstudio::model;
using namespace openstudio;

TEST_F(ModelFixture, InteriorPartitionSurface) {
  Model model;

  Point3dVector points{
    {0, 1, 0},
    {0, 0, 0},
    {1, 0, 0},
    {1, 1, 0},
  };

  EXPECT_NO_THROW(InteriorPartitionSurface(points, model));

  InteriorPartitionSurface interiorPartitionSurface(points, model);
  EXPECT_FALSE(interiorPartitionSurface.interiorPartitionSurfaceGroup());
}

TEST_F(ModelFixture, InteriorPartitionSurface_isXXXAutocalculated_4399) {
  // Test for #4399
  Model m;
  std::vector<Point3d> points{
    Point3d{0, 1, 0},
    Point3d{0, 0, 0},
    Point3d{1, 0, 0},
    Point3d{1, 1, 0},
  };

  InteriorPartitionSurface interiorPartitionSurface(points, m);

  interiorPartitionSurface.autocalculateNumberofVertices();
  EXPECT_TRUE(interiorPartitionSurface.isNumberofVerticesAutocalculated());
}
