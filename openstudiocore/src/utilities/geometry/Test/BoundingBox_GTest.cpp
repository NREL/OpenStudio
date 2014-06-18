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
#include "GeometryFixture.hpp"

#include "../BoundingBox.hpp"
#include "../Point3d.hpp"

using namespace openstudio;

TEST_F(GeometryFixture, BoundingBox)
{
  BoundingBox b1;
  EXPECT_FALSE(b1.minX());
  EXPECT_FALSE(b1.minY());
  EXPECT_FALSE(b1.minZ());
  EXPECT_FALSE(b1.maxX());
  EXPECT_FALSE(b1.maxY());
  EXPECT_FALSE(b1.maxZ());

  b1.addPoint(Point3d(1,2,3));

  ASSERT_TRUE(b1.minX());
  EXPECT_EQ(1, b1.minX().get());
  ASSERT_TRUE(b1.minY());
  EXPECT_EQ(2, b1.minY().get());
  ASSERT_TRUE(b1.minZ());
  EXPECT_EQ(3, b1.minZ().get());
  ASSERT_TRUE(b1.maxX());
  EXPECT_EQ(1, b1.maxX().get());
  ASSERT_TRUE(b1.maxY());
  EXPECT_EQ(2, b1.maxY().get());
  ASSERT_TRUE(b1.maxZ());
  EXPECT_EQ(3, b1.maxZ().get());

  b1.addPoint(Point3d(2,3,4));

  ASSERT_TRUE(b1.minX());
  EXPECT_EQ(1, b1.minX().get());
  ASSERT_TRUE(b1.minY());
  EXPECT_EQ(2, b1.minY().get());
  ASSERT_TRUE(b1.minZ());
  EXPECT_EQ(3, b1.minZ().get());
  ASSERT_TRUE(b1.maxX());
  EXPECT_EQ(2, b1.maxX().get());
  ASSERT_TRUE(b1.maxY());
  EXPECT_EQ(3, b1.maxY().get());
  ASSERT_TRUE(b1.maxZ());
  EXPECT_EQ(4, b1.maxZ().get());
}

TEST_F(GeometryFixture, BoundingBox_Empty)
{
  BoundingBox b1;
  BoundingBox b2;

  EXPECT_FALSE(b1.intersects(b1));
  EXPECT_FALSE(b2.intersects(b2));
  EXPECT_FALSE(b1.intersects(b2));
  EXPECT_FALSE(b2.intersects(b1));

  b1.addPoint(Point3d(0,0,0));

  EXPECT_TRUE(b1.intersects(b1));
  EXPECT_FALSE(b2.intersects(b2));
  EXPECT_FALSE(b1.intersects(b2));
  EXPECT_FALSE(b2.intersects(b1));

  b1.addPoint(Point3d(1,1,0));

  EXPECT_TRUE(b1.intersects(b1));
  EXPECT_FALSE(b2.intersects(b2));
  EXPECT_FALSE(b1.intersects(b2));
  EXPECT_FALSE(b2.intersects(b1));

  b2.addPoint(Point3d(0.5,0.5,0));

  EXPECT_TRUE(b1.intersects(b1));
  EXPECT_TRUE(b2.intersects(b2));
  EXPECT_TRUE(b1.intersects(b2));
  EXPECT_TRUE(b2.intersects(b1));

  b2.addPoint(Point3d(1,1,0));

  EXPECT_TRUE(b1.intersects(b1));
  EXPECT_TRUE(b2.intersects(b2));
  EXPECT_TRUE(b1.intersects(b2));
  EXPECT_TRUE(b2.intersects(b1));
}


TEST_F(GeometryFixture, BoundingBox_Touching)
{
  BoundingBox b1;
  BoundingBox b2;

  b1.addPoint(Point3d(0,0,0));
  b1.addPoint(Point3d(1,1,1));

  b2.addPoint(Point3d(1,1,1));
  b2.addPoint(Point3d(2,2,2));

  EXPECT_TRUE(b1.intersects(b1));
  EXPECT_TRUE(b2.intersects(b2));
  EXPECT_TRUE(b1.intersects(b2));
  EXPECT_TRUE(b2.intersects(b1));
}

TEST_F(GeometryFixture, BoundingBox_NoIntersection)
{
  BoundingBox b1;
  BoundingBox b2;

  b1.addPoint(Point3d(0,0,0));
  b1.addPoint(Point3d(1,1,0));

  b2.addPoint(Point3d(2,2,0));
  b2.addPoint(Point3d(3,3,0));

  EXPECT_TRUE(b1.intersects(b1));
  EXPECT_TRUE(b2.intersects(b2));
  EXPECT_FALSE(b1.intersects(b2));
  EXPECT_FALSE(b2.intersects(b1));
}
