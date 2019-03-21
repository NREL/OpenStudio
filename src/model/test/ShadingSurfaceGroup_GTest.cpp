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
#include "../Site.hpp"
#include "../Site_Impl.hpp"
#include "../Building.hpp"
#include "../Building_Impl.hpp"
#include "../Space.hpp"
#include "../Space_Impl.hpp"
#include "../ShadingSurface.hpp"
#include "../ShadingSurface_Impl.hpp"
#include "../ShadingSurfaceGroup.hpp"
#include "../ShadingSurfaceGroup_Impl.hpp"

using namespace openstudio::model;
using namespace openstudio;

TEST_F(ModelFixture, ShadingSurfaceGroup_Space_Hierarchy)
{
  Model model;

  // add a space
  Space space(model);

  // add a shading surface group
  ShadingSurfaceGroup group(model);
  EXPECT_EQ("Building", group.shadingSurfaceType());
  EXPECT_TRUE(group.setSpace(space));
  EXPECT_EQ("Space", group.shadingSurfaceType());
  ASSERT_TRUE(group.space());
  EXPECT_EQ(space.handle(), group.space()->handle());

  // make a new shading surface
  Point3dVector points;
  points.push_back(Point3d(0, 2, 0));
  points.push_back(Point3d(0, 0, 0));
  points.push_back(Point3d(1, 0, 0));
  ShadingSurface surface(points, model);
  EXPECT_TRUE(surface.setShadingSurfaceGroup(group));
  ASSERT_TRUE(surface.shadingSurfaceGroup());
  EXPECT_EQ(group.handle(), surface.shadingSurfaceGroup()->handle());

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
  OptionalShadingSurfaceGroup cloneChild = spaceClone->children()[0].optionalCast<ShadingSurfaceGroup>();
  ASSERT_TRUE(cloneChild);
  ASSERT_EQ(static_cast<unsigned>(1), cloneChild->children().size());
  EXPECT_NE(surface.handle(), cloneChild->children()[0].handle());
}

TEST_F(ModelFixture, ShadingSurfaceGroup_Building_Hierarchy)
{
  Model model;

  // make a building
  Building building = model.getUniqueModelObject<Building>();

  // add a shading surface group
  ShadingSurfaceGroup group(model);
  EXPECT_TRUE(group.setShadingSurfaceType("Building"));

  // make a new shading surface
  Point3dVector points;
  points.push_back(Point3d(0, 2, 0));
  points.push_back(Point3d(0, 0, 0));
  points.push_back(Point3d(1, 0, 0));
  ShadingSurface surface(points, model);
  EXPECT_TRUE(surface.setShadingSurfaceGroup(group));
  ASSERT_TRUE(surface.shadingSurfaceGroup());
  EXPECT_EQ(group.handle(), surface.shadingSurfaceGroup()->handle());

  // check children from group
  ASSERT_EQ(static_cast<unsigned>(1), group.children().size());
  EXPECT_EQ(surface.handle(), group.children()[0].handle());

  // now check children from building
  ASSERT_EQ(static_cast<unsigned>(1), building.children().size());
  EXPECT_EQ(group.handle(), building.children()[0].handle());

  // check parent from group
  ASSERT_TRUE(group.parent());
  EXPECT_EQ(building.handle(), group.parent()->handle());

  // check parent from surface
  ASSERT_TRUE(surface.parent());
  EXPECT_EQ(group.handle(), surface.parent()->handle());
}

TEST_F(ModelFixture, ShadingSurfaceGroup_Site_Hierarchy)
{
  Model model;

  // make a site
  Site site = model.getUniqueModelObject<Site>();

  // add a shading surface group
  ShadingSurfaceGroup group(model);
  EXPECT_TRUE(group.setShadingSurfaceType("Site"));

  // make a new shading surface
  Point3dVector points;
  points.push_back(Point3d(0, 2, 0));
  points.push_back(Point3d(0, 0, 0));
  points.push_back(Point3d(1, 0, 0));
  ShadingSurface surface(points, model);
  EXPECT_TRUE(surface.setShadingSurfaceGroup(group));
  ASSERT_TRUE(surface.shadingSurfaceGroup());
  EXPECT_EQ(group.handle(), surface.shadingSurfaceGroup()->handle());

  // check children from group
  ASSERT_EQ(static_cast<unsigned>(1), group.children().size());
  EXPECT_EQ(surface.handle(), group.children()[0].handle());

  // now check children from site
  ASSERT_EQ(static_cast<unsigned>(1), site.children().size());
  EXPECT_EQ(group.handle(), site.children()[0].handle());

  // check parent from group
  ASSERT_TRUE(group.parent());
  EXPECT_EQ(site.handle(), group.parent()->handle());

  // check parent from surface
  ASSERT_TRUE(surface.parent());
  EXPECT_EQ(group.handle(), surface.parent()->handle());
}

TEST_F(ModelFixture, ShadingSurfaceGroup_SetParent)
{
  Model model;

  Site site = model.getUniqueModelObject<Site>();
  Building building = model.getUniqueModelObject<Building>();
  Space space(model);

  ShadingSurfaceGroup group(model);
  EXPECT_EQ("Building", group.shadingSurfaceType());
  ASSERT_TRUE(group.parent());
  EXPECT_EQ(building.handle(), group.parent()->handle());
  EXPECT_FALSE(group.space());

  EXPECT_TRUE(group.setParent(site));
  EXPECT_EQ("Site", group.shadingSurfaceType());
  ASSERT_TRUE(group.parent());
  EXPECT_EQ(site.handle(), group.parent()->handle());
  EXPECT_FALSE(group.space());

  EXPECT_TRUE(group.setParent(space));
  EXPECT_EQ("Space", group.shadingSurfaceType());
  ASSERT_TRUE(group.parent());
  EXPECT_EQ(space.handle(), group.parent()->handle());
  ASSERT_TRUE(group.space());
  EXPECT_EQ(space.handle(), group.space()->handle());

  EXPECT_TRUE(group.setParent(building));
  EXPECT_EQ("Building", group.shadingSurfaceType());
  ASSERT_TRUE(group.parent());
  EXPECT_EQ(building.handle(), group.parent()->handle());
  EXPECT_FALSE(group.space());
}

TEST_F(ModelFixture, ShadingSurfaceGroup_Hierarchy)
{
  Model model;
  Space space(model);

  ShadingSurfaceGroup siteGroup(model);
  EXPECT_TRUE(siteGroup.setShadingSurfaceType("Site"));

  ShadingSurfaceGroup buildingGroup(model);
  EXPECT_TRUE(buildingGroup.setShadingSurfaceType("Building"));

  ShadingSurfaceGroup spaceGroup(model);
  EXPECT_TRUE(spaceGroup.setSpace(space));

  Point3dVector points;
  points.push_back(Point3d(0, 2, 0));
  points.push_back(Point3d(0, 0, 0));
  points.push_back(Point3d(1, 0, 0));

  // add site shading surface
  ShadingSurface siteSurface(points, model);
  EXPECT_TRUE(siteSurface.setShadingSurfaceGroup(siteGroup));
  ASSERT_TRUE(siteSurface.shadingSurfaceGroup());
  EXPECT_EQ(siteGroup.handle(), siteSurface.shadingSurfaceGroup()->handle());

  EXPECT_EQ(1u, siteGroup.shadingSurfaces().size());
  EXPECT_EQ(0u, buildingGroup.shadingSurfaces().size());
  EXPECT_EQ(0u, spaceGroup.shadingSurfaces().size());

  // add building shading surface
  ShadingSurface buildingSurface(points, model);
  EXPECT_TRUE(buildingSurface.setShadingSurfaceGroup(buildingGroup));
  ASSERT_TRUE(buildingSurface.shadingSurfaceGroup());
  EXPECT_EQ(buildingGroup.handle(), buildingSurface.shadingSurfaceGroup()->handle());

  EXPECT_EQ(1u, siteGroup.shadingSurfaces().size());
  EXPECT_EQ(1u, buildingGroup.shadingSurfaces().size());
  EXPECT_EQ(0u, spaceGroup.shadingSurfaces().size());

   // add space shading surface
  ShadingSurface spaceSurface(points, model);
  EXPECT_TRUE(spaceSurface.setShadingSurfaceGroup(spaceGroup));
  ASSERT_TRUE(spaceSurface.shadingSurfaceGroup());
  EXPECT_EQ(spaceGroup.handle(), spaceSurface.shadingSurfaceGroup()->handle());

  EXPECT_EQ(1u, siteGroup.shadingSurfaces().size());
  EXPECT_EQ(1u, buildingGroup.shadingSurfaces().size());
  EXPECT_EQ(1u, spaceGroup.shadingSurfaces().size());
}
