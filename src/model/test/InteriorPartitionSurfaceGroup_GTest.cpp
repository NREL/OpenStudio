/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2019, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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
