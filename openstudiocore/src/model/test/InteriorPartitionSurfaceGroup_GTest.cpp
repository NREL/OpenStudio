/**********************************************************************
*  Copyright (c) 2008-2014, Alliance for Sustainable Energy.
*  All rights reserved.
*
*  This library is free software; you can redistribute it and/or
*  modify it under the terms of the GNU Lesser General Public
*  License as published by the Free Software Foundation; either
*  version 2.1 of the License, or (at your option) any later version.
*
*  This library is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
*  Lesser General Public License for more details.
*
*  You should have received a copy of the GNU Lesser General Public
*  License along with this library; if not, write to the Free Software
*  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
**********************************************************************/

#include <gtest/gtest.h>

#include "ModelFixture.hpp"
#include "../Model_Impl.hpp"
#include "../Space.hpp"
#include "../Space_Impl.hpp"
#include "../InteriorPartitionSurface.hpp"
#include "../InteriorPartitionSurface_Impl.hpp"
#include "../InteriorPartitionSurfaceGroup.hpp"
#include "../InteriorPartitionSurfaceGroup_Impl.hpp"
#include "../DefaultConstructionSet.hpp"
#include "../DefaultConstructionSet_Impl.hpp"
#include "../Construction.hpp"
#include "../Construction_Impl.hpp"

using namespace openstudio::model;
using namespace openstudio;

TEST_F(ModelFixture, InteriorPartitionSurface_Hierarchy)
{
  Model model;

  Space space(model);

  // add a interior partition surface group
  InteriorPartitionSurfaceGroup group(model);
  EXPECT_FALSE(group.space());
  EXPECT_TRUE(group.setSpace(space));
  ASSERT_TRUE(group.space());
  EXPECT_EQ(space.handle(), group.space()->handle());

  // make a new interior partition surface
  Point3dVector points;
  points.push_back(Point3d(0, 2, 0));
  points.push_back(Point3d(0, 0, 0));
  points.push_back(Point3d(1, 0, 0));
  InteriorPartitionSurface surface(points, model);
  EXPECT_FALSE(surface.interiorPartitionSurfaceGroup());
  EXPECT_TRUE(surface.setInteriorPartitionSurfaceGroup(group));
  ASSERT_TRUE(surface.interiorPartitionSurfaceGroup());
  EXPECT_EQ(group.handle(), surface.interiorPartitionSurfaceGroup()->handle());

  // check children from group
  ASSERT_EQ(static_cast<unsigned>(1), group.children().size());
  EXPECT_EQ(surface.handle(), group.children()[0].handle());

  // now check children from space
  ASSERT_EQ(static_cast<unsigned>(1), space.children().size());
  EXPECT_EQ(group.handle(), space.children()[0].handle());

  // check parent from group
  ASSERT_TRUE(group.parent());
  EXPECT_EQ(space.handle(), group.parent()->handle());

  // check parent from surface
  ASSERT_TRUE(surface.parent());
  EXPECT_EQ(group.handle(), surface.parent()->handle());

  // clone the space
  ModelObject clone = space.clone(model);
  OptionalSpace spaceClone = clone.optionalCast<Space>();
  ASSERT_TRUE(spaceClone);
  ASSERT_EQ(static_cast<unsigned>(1), spaceClone->children().size());
  EXPECT_NE(group.handle(), spaceClone->children()[0].handle());
}

TEST_F(ModelFixture, InteriorPartitionSurface_DefaultConstruction)
{
  Model model;

  Space space(model);

  Construction construction(model);

  DefaultConstructionSet defaultConstructionSet(model);
  defaultConstructionSet.setInteriorPartitionConstruction(construction);
  space.setDefaultConstructionSet(defaultConstructionSet);

  InteriorPartitionSurfaceGroup group(model);
  group.setSpace(space);

  Point3dVector points;
  points.push_back(Point3d(0, 2, 0));
  points.push_back(Point3d(0, 0, 0));
  points.push_back(Point3d(1, 0, 0));
  InteriorPartitionSurface surface(points, model);
  
  EXPECT_FALSE(surface.construction());
  
  surface.setInteriorPartitionSurfaceGroup(group);

  ASSERT_TRUE(surface.construction());
  EXPECT_EQ(construction.handle(), surface.construction()->handle());
}
