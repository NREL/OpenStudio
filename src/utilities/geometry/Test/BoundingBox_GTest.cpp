/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2020, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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
#include "GeometryFixture.hpp"

#include "../BoundingBox.hpp"
#include "../Point3d.hpp"

using namespace openstudio;

TEST_F(GeometryFixture, BoundingBox) {
  BoundingBox b1;
  EXPECT_FALSE(b1.minX());
  EXPECT_FALSE(b1.minY());
  EXPECT_FALSE(b1.minZ());
  EXPECT_FALSE(b1.maxX());
  EXPECT_FALSE(b1.maxY());
  EXPECT_FALSE(b1.maxZ());

  b1.addPoint(Point3d(1, 2, 3));

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

  b1.addPoint(Point3d(2, 3, 4));

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

TEST_F(GeometryFixture, BoundingBox_Empty) {
  BoundingBox b1;
  BoundingBox b2;

  EXPECT_FALSE(b1.intersects(b1));
  EXPECT_FALSE(b2.intersects(b2));
  EXPECT_FALSE(b1.intersects(b2));
  EXPECT_FALSE(b2.intersects(b1));

  b1.addPoint(Point3d(0, 0, 0));

  EXPECT_TRUE(b1.intersects(b1));
  EXPECT_FALSE(b2.intersects(b2));
  EXPECT_FALSE(b1.intersects(b2));
  EXPECT_FALSE(b2.intersects(b1));

  b1.addPoint(Point3d(1, 1, 0));

  EXPECT_TRUE(b1.intersects(b1));
  EXPECT_FALSE(b2.intersects(b2));
  EXPECT_FALSE(b1.intersects(b2));
  EXPECT_FALSE(b2.intersects(b1));

  b2.addPoint(Point3d(0.5, 0.5, 0));

  EXPECT_TRUE(b1.intersects(b1));
  EXPECT_TRUE(b2.intersects(b2));
  EXPECT_TRUE(b1.intersects(b2));
  EXPECT_TRUE(b2.intersects(b1));

  b2.addPoint(Point3d(1, 1, 0));

  EXPECT_TRUE(b1.intersects(b1));
  EXPECT_TRUE(b2.intersects(b2));
  EXPECT_TRUE(b1.intersects(b2));
  EXPECT_TRUE(b2.intersects(b1));
}

TEST_F(GeometryFixture, BoundingBox_Touching) {
  BoundingBox b1;
  BoundingBox b2;

  b1.addPoint(Point3d(0, 0, 0));
  b1.addPoint(Point3d(1, 1, 1));

  b2.addPoint(Point3d(1, 1, 1));
  b2.addPoint(Point3d(2, 2, 2));

  EXPECT_TRUE(b1.intersects(b1));
  EXPECT_TRUE(b2.intersects(b2));
  EXPECT_TRUE(b1.intersects(b2));
  EXPECT_TRUE(b2.intersects(b1));
}

TEST_F(GeometryFixture, BoundingBox_NoIntersection) {
  BoundingBox b1;
  BoundingBox b2;

  b1.addPoint(Point3d(0, 0, 0));
  b1.addPoint(Point3d(1, 1, 0));

  b2.addPoint(Point3d(2, 2, 0));
  b2.addPoint(Point3d(3, 3, 0));

  EXPECT_TRUE(b1.intersects(b1));
  EXPECT_TRUE(b2.intersects(b2));
  EXPECT_FALSE(b1.intersects(b2));
  EXPECT_FALSE(b2.intersects(b1));
}
