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

#include "../Model.hpp"
#include "../Model_Impl.hpp"
#include "../DefaultConstructionSet.hpp"
#include "../DefaultConstructionSet_Impl.hpp"
#include "../DefaultSurfaceConstructions.hpp"
#include "../DefaultSurfaceConstructions_Impl.hpp"
#include "../DefaultSubSurfaceConstructions.hpp"
#include "../DefaultSubSurfaceConstructions_Impl.hpp"
#include "../StandardOpaqueMaterial.hpp"
#include "../Construction.hpp"
#include "../Construction_Impl.hpp"
#include "../Surface.hpp"
#include "../SubSurface.hpp"
#include "../InteriorPartitionSurface.hpp"
#include "../InteriorPartitionSurfaceGroup.hpp"
#include "../ShadingSurface.hpp"
#include "../ShadingSurfaceGroup.hpp"
#include "../Space.hpp"

#include "../../utilities/geometry/Point3d.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, DefaultConstructionSet)
{
  Model model;

  DefaultConstructionSet defaultConstructionSet(model);
  DefaultSurfaceConstructions defaultSurfaceConstructions(model);
  DefaultSubSurfaceConstructions defaultSubSurfaceConstructions(model);
  Construction construction(model);

  EXPECT_FALSE(defaultConstructionSet.defaultExteriorSurfaceConstructions());
  EXPECT_TRUE(defaultConstructionSet.setDefaultExteriorSurfaceConstructions(defaultSurfaceConstructions));
  ASSERT_TRUE(defaultConstructionSet.defaultExteriorSurfaceConstructions());
  EXPECT_EQ(defaultSurfaceConstructions.handle(), defaultConstructionSet.defaultExteriorSurfaceConstructions()->handle());
  defaultConstructionSet.resetDefaultExteriorSurfaceConstructions();
  EXPECT_FALSE(defaultConstructionSet.defaultExteriorSurfaceConstructions());

  EXPECT_FALSE(defaultConstructionSet.defaultInteriorSurfaceConstructions());
  EXPECT_TRUE(defaultConstructionSet.setDefaultInteriorSurfaceConstructions(defaultSurfaceConstructions));
  ASSERT_TRUE(defaultConstructionSet.defaultInteriorSurfaceConstructions());
  EXPECT_EQ(defaultSurfaceConstructions.handle(), defaultConstructionSet.defaultInteriorSurfaceConstructions()->handle());
  defaultConstructionSet.resetDefaultInteriorSurfaceConstructions();
  EXPECT_FALSE(defaultConstructionSet.defaultInteriorSurfaceConstructions());

  EXPECT_FALSE(defaultConstructionSet.defaultGroundContactSurfaceConstructions());
  EXPECT_TRUE(defaultConstructionSet.setDefaultGroundContactSurfaceConstructions(defaultSurfaceConstructions));
  ASSERT_TRUE(defaultConstructionSet.defaultGroundContactSurfaceConstructions());
  EXPECT_EQ(defaultSurfaceConstructions.handle(), defaultConstructionSet.defaultGroundContactSurfaceConstructions()->handle());
  defaultConstructionSet.resetDefaultGroundContactSurfaceConstructions();
  EXPECT_FALSE(defaultConstructionSet.defaultGroundContactSurfaceConstructions());

  EXPECT_FALSE(defaultConstructionSet.defaultExteriorSubSurfaceConstructions());
  EXPECT_TRUE(defaultConstructionSet.setDefaultExteriorSubSurfaceConstructions(defaultSubSurfaceConstructions));
  ASSERT_TRUE(defaultConstructionSet.defaultExteriorSubSurfaceConstructions());
  EXPECT_EQ(defaultSubSurfaceConstructions.handle(), defaultConstructionSet.defaultExteriorSubSurfaceConstructions()->handle());
  defaultConstructionSet.resetDefaultExteriorSubSurfaceConstructions();
  EXPECT_FALSE(defaultConstructionSet.defaultExteriorSubSurfaceConstructions());

  EXPECT_FALSE(defaultConstructionSet.defaultInteriorSubSurfaceConstructions());
  EXPECT_TRUE(defaultConstructionSet.setDefaultInteriorSubSurfaceConstructions(defaultSubSurfaceConstructions));
  ASSERT_TRUE(defaultConstructionSet.defaultInteriorSubSurfaceConstructions());
  EXPECT_EQ(defaultSubSurfaceConstructions.handle(), defaultConstructionSet.defaultInteriorSubSurfaceConstructions()->handle());
  defaultConstructionSet.resetDefaultInteriorSubSurfaceConstructions();
  EXPECT_FALSE(defaultConstructionSet.defaultInteriorSubSurfaceConstructions());

  EXPECT_FALSE(defaultConstructionSet.interiorPartitionConstruction());
  EXPECT_TRUE(defaultConstructionSet.setInteriorPartitionConstruction(construction));
  ASSERT_TRUE(defaultConstructionSet.interiorPartitionConstruction());
  EXPECT_EQ(construction.handle(), defaultConstructionSet.interiorPartitionConstruction()->handle());
  defaultConstructionSet.resetInteriorPartitionConstruction();
  EXPECT_FALSE(defaultConstructionSet.interiorPartitionConstruction());

  EXPECT_FALSE(defaultConstructionSet.spaceShadingConstruction());
  EXPECT_TRUE(defaultConstructionSet.setSpaceShadingConstruction(construction));
  ASSERT_TRUE(defaultConstructionSet.spaceShadingConstruction());
  EXPECT_EQ(construction.handle(), defaultConstructionSet.spaceShadingConstruction()->handle());
  defaultConstructionSet.resetSpaceShadingConstruction();
  EXPECT_FALSE(defaultConstructionSet.spaceShadingConstruction());

  EXPECT_FALSE(defaultConstructionSet.buildingShadingConstruction());
  EXPECT_TRUE(defaultConstructionSet.setBuildingShadingConstruction(construction));
  ASSERT_TRUE(defaultConstructionSet.buildingShadingConstruction());
  EXPECT_EQ(construction.handle(), defaultConstructionSet.buildingShadingConstruction()->handle());
  defaultConstructionSet.resetBuildingShadingConstruction();
  EXPECT_FALSE(defaultConstructionSet.buildingShadingConstruction());

  EXPECT_FALSE(defaultConstructionSet.siteShadingConstruction());
  EXPECT_TRUE(defaultConstructionSet.setSiteShadingConstruction(construction));
  ASSERT_TRUE(defaultConstructionSet.siteShadingConstruction());
  EXPECT_EQ(construction.handle(), defaultConstructionSet.siteShadingConstruction()->handle());
  defaultConstructionSet.resetSiteShadingConstruction();
  EXPECT_FALSE(defaultConstructionSet.siteShadingConstruction());
}

TEST_F(ModelFixture, DefaultConstructionSet_ExteriorSurfaces)
{
  Model model;

  Point3dVector points;
  points.push_back(Point3d(0,1,0));
  points.push_back(Point3d(0,0,0));
  points.push_back(Point3d(1,0,0));
  Surface surface(points, model);
  EXPECT_TRUE(surface.setOutsideBoundaryCondition("Outdoors"));

  DefaultConstructionSet defaultConstructionSet(model);
  DefaultSurfaceConstructions defaultSurfaceConstructions(model);
  Construction construction(model);

  EXPECT_FALSE(defaultConstructionSet.defaultExteriorSurfaceConstructions());
  EXPECT_TRUE(defaultConstructionSet.setDefaultExteriorSurfaceConstructions(defaultSurfaceConstructions));
  ASSERT_TRUE(defaultConstructionSet.defaultExteriorSurfaceConstructions());
  EXPECT_EQ(defaultSurfaceConstructions.handle(), defaultConstructionSet.defaultExteriorSurfaceConstructions()->handle());

  EXPECT_TRUE(surface.setSurfaceType("Floor"));
  EXPECT_FALSE(defaultConstructionSet.getDefaultConstruction(surface));
  EXPECT_TRUE(defaultSurfaceConstructions.setFloorConstruction(construction));
  ASSERT_TRUE(defaultConstructionSet.getDefaultConstruction(surface));
  EXPECT_EQ(construction.handle(), defaultConstructionSet.getDefaultConstruction(surface)->handle());
  defaultSurfaceConstructions.resetFloorConstruction();
  EXPECT_FALSE(defaultConstructionSet.getDefaultConstruction(surface));

  EXPECT_TRUE(surface.setSurfaceType("Wall"));
  EXPECT_FALSE(defaultConstructionSet.getDefaultConstruction(surface));
  EXPECT_TRUE(defaultSurfaceConstructions.setWallConstruction(construction));
  ASSERT_TRUE(defaultConstructionSet.getDefaultConstruction(surface));
  EXPECT_EQ(construction.handle(), defaultConstructionSet.getDefaultConstruction(surface)->handle());
  defaultSurfaceConstructions.resetWallConstruction();
  EXPECT_FALSE(defaultConstructionSet.getDefaultConstruction(surface));

  EXPECT_TRUE(surface.setSurfaceType("RoofCeiling"));
  EXPECT_FALSE(defaultConstructionSet.getDefaultConstruction(surface));
  EXPECT_TRUE(defaultSurfaceConstructions.setRoofCeilingConstruction(construction));
  ASSERT_TRUE(defaultConstructionSet.getDefaultConstruction(surface));
  EXPECT_EQ(construction.handle(), defaultConstructionSet.getDefaultConstruction(surface)->handle());
  defaultSurfaceConstructions.resetRoofCeilingConstruction();
  EXPECT_FALSE(defaultConstructionSet.getDefaultConstruction(surface));
}

TEST_F(ModelFixture, DefaultConstructionSet_InteriorSurfaces)
{
  Model model;

  Point3dVector points;
  points.push_back(Point3d(0,1,0));
  points.push_back(Point3d(0,0,0));
  points.push_back(Point3d(1,0,0));
  Space space(model);
  Surface surface(points, model);
  surface.setSpace(space);
  EXPECT_FALSE(surface.setOutsideBoundaryCondition("Surface"));

  points.clear();
  points.push_back(Point3d(1,0,0));
  points.push_back(Point3d(0,0,0));
  points.push_back(Point3d(0,1,0));
  Space space2(model);
  Surface surface2(points, model);
  surface2.setSpace(space2);
  EXPECT_TRUE(surface.setAdjacentSurface(surface2));
  EXPECT_EQ("Surface", surface.outsideBoundaryCondition());
  EXPECT_EQ("Surface", surface2.outsideBoundaryCondition());
  ASSERT_TRUE(surface.adjacentSurface());
  ASSERT_TRUE(surface2.adjacentSurface());
  EXPECT_EQ(surface2.handle(), surface.adjacentSurface()->handle());
  EXPECT_EQ(surface.handle(), surface2.adjacentSurface()->handle());

  DefaultConstructionSet defaultConstructionSet(model);
  DefaultSurfaceConstructions defaultSurfaceConstructions(model);
  Construction construction(model);

  EXPECT_FALSE(defaultConstructionSet.defaultInteriorSurfaceConstructions());
  EXPECT_TRUE(defaultConstructionSet.setDefaultInteriorSurfaceConstructions(defaultSurfaceConstructions));
  ASSERT_TRUE(defaultConstructionSet.defaultInteriorSurfaceConstructions());
  EXPECT_EQ(defaultSurfaceConstructions.handle(), defaultConstructionSet.defaultInteriorSurfaceConstructions()->handle());

  EXPECT_TRUE(surface.setSurfaceType("Floor"));
  EXPECT_FALSE(defaultConstructionSet.getDefaultConstruction(surface));
  EXPECT_TRUE(defaultSurfaceConstructions.setFloorConstruction(construction));
  ASSERT_TRUE(defaultConstructionSet.getDefaultConstruction(surface));
  EXPECT_EQ(construction.handle(), defaultConstructionSet.getDefaultConstruction(surface)->handle());
  defaultSurfaceConstructions.resetFloorConstruction();
  EXPECT_FALSE(defaultConstructionSet.getDefaultConstruction(surface));

  EXPECT_TRUE(surface.setSurfaceType("Wall"));
  EXPECT_FALSE(defaultConstructionSet.getDefaultConstruction(surface));
  EXPECT_TRUE(defaultSurfaceConstructions.setWallConstruction(construction));
  ASSERT_TRUE(defaultConstructionSet.getDefaultConstruction(surface));
  EXPECT_EQ(construction.handle(), defaultConstructionSet.getDefaultConstruction(surface)->handle());
  defaultSurfaceConstructions.resetWallConstruction();
  EXPECT_FALSE(defaultConstructionSet.getDefaultConstruction(surface));

  EXPECT_TRUE(surface.setSurfaceType("RoofCeiling"));
  EXPECT_FALSE(defaultConstructionSet.getDefaultConstruction(surface));
  EXPECT_TRUE(defaultSurfaceConstructions.setRoofCeilingConstruction(construction));
  ASSERT_TRUE(defaultConstructionSet.getDefaultConstruction(surface));
  EXPECT_EQ(construction.handle(), defaultConstructionSet.getDefaultConstruction(surface)->handle());
  defaultSurfaceConstructions.resetRoofCeilingConstruction();
  EXPECT_FALSE(defaultConstructionSet.getDefaultConstruction(surface));
}


TEST_F(ModelFixture, DefaultConstructionSet_GroundContactSurfaces)
{
  Model model;

  Point3dVector points;
  points.push_back(Point3d(0,1,0));
  points.push_back(Point3d(0,0,0));
  points.push_back(Point3d(1,0,0));
  Surface surface(points, model);
  EXPECT_TRUE(surface.setOutsideBoundaryCondition("Ground"));

  DefaultConstructionSet defaultConstructionSet(model);
  DefaultSurfaceConstructions defaultSurfaceConstructions(model);
  Construction construction(model);

  EXPECT_FALSE(defaultConstructionSet.defaultGroundContactSurfaceConstructions());
  EXPECT_TRUE(defaultConstructionSet.setDefaultGroundContactSurfaceConstructions(defaultSurfaceConstructions));
  ASSERT_TRUE(defaultConstructionSet.defaultGroundContactSurfaceConstructions());
  EXPECT_EQ(defaultSurfaceConstructions.handle(), defaultConstructionSet.defaultGroundContactSurfaceConstructions()->handle());

  EXPECT_TRUE(surface.setSurfaceType("Floor"));
  EXPECT_FALSE(defaultConstructionSet.getDefaultConstruction(surface));
  EXPECT_TRUE(defaultSurfaceConstructions.setFloorConstruction(construction));
  ASSERT_TRUE(defaultConstructionSet.getDefaultConstruction(surface));
  EXPECT_EQ(construction.handle(), defaultConstructionSet.getDefaultConstruction(surface)->handle());
  defaultSurfaceConstructions.resetFloorConstruction();
  EXPECT_FALSE(defaultConstructionSet.getDefaultConstruction(surface));

  EXPECT_TRUE(surface.setSurfaceType("Wall"));
  EXPECT_FALSE(defaultConstructionSet.getDefaultConstruction(surface));
  EXPECT_TRUE(defaultSurfaceConstructions.setWallConstruction(construction));
  ASSERT_TRUE(defaultConstructionSet.getDefaultConstruction(surface));
  EXPECT_EQ(construction.handle(), defaultConstructionSet.getDefaultConstruction(surface)->handle());
  defaultSurfaceConstructions.resetWallConstruction();
  EXPECT_FALSE(defaultConstructionSet.getDefaultConstruction(surface));

  EXPECT_TRUE(surface.setSurfaceType("RoofCeiling"));
  EXPECT_FALSE(defaultConstructionSet.getDefaultConstruction(surface));
  EXPECT_TRUE(defaultSurfaceConstructions.setRoofCeilingConstruction(construction));
  ASSERT_TRUE(defaultConstructionSet.getDefaultConstruction(surface));
  EXPECT_EQ(construction.handle(), defaultConstructionSet.getDefaultConstruction(surface)->handle());
  defaultSurfaceConstructions.resetRoofCeilingConstruction();
  EXPECT_FALSE(defaultConstructionSet.getDefaultConstruction(surface));
}

TEST_F(ModelFixture, DefaultConstructionSet_ExteriorSubSurfaces)
{
  Model model;

  Point3dVector points;
  points.push_back(Point3d(0,1,0));
  points.push_back(Point3d(0,0,0));
  points.push_back(Point3d(1,0,0));
  Surface surface(points, model);
  EXPECT_TRUE(surface.setOutsideBoundaryCondition("Outdoors"));

  SubSurface subSurface(points, model);
  EXPECT_TRUE(subSurface.setSurface(surface));

  DefaultConstructionSet defaultConstructionSet(model);
  DefaultSubSurfaceConstructions defaultSubSurfaceConstructions(model);
  Construction construction(model);

  EXPECT_FALSE(defaultConstructionSet.defaultExteriorSubSurfaceConstructions());
  EXPECT_TRUE(defaultConstructionSet.setDefaultExteriorSubSurfaceConstructions(defaultSubSurfaceConstructions));
  ASSERT_TRUE(defaultConstructionSet.defaultExteriorSubSurfaceConstructions());
  EXPECT_EQ(defaultSubSurfaceConstructions.handle(), defaultConstructionSet.defaultExteriorSubSurfaceConstructions()->handle());

  EXPECT_TRUE(subSurface.setSubSurfaceType("FixedWindow"));
  EXPECT_FALSE(defaultConstructionSet.getDefaultConstruction(subSurface));
  EXPECT_TRUE(defaultSubSurfaceConstructions.setFixedWindowConstruction(construction));
  ASSERT_TRUE(defaultConstructionSet.getDefaultConstruction(subSurface));
  EXPECT_EQ(construction.handle(), defaultConstructionSet.getDefaultConstruction(subSurface)->handle());
  defaultSubSurfaceConstructions.resetFixedWindowConstruction();
  EXPECT_FALSE(defaultConstructionSet.getDefaultConstruction(subSurface));

  EXPECT_TRUE(subSurface.setSubSurfaceType("Door"));
  EXPECT_FALSE(defaultConstructionSet.getDefaultConstruction(subSurface));
  EXPECT_TRUE(defaultSubSurfaceConstructions.setDoorConstruction(construction));
  ASSERT_TRUE(defaultConstructionSet.getDefaultConstruction(subSurface));
  EXPECT_EQ(construction.handle(), defaultConstructionSet.getDefaultConstruction(subSurface)->handle());
  defaultSubSurfaceConstructions.resetDoorConstruction();
  EXPECT_FALSE(defaultConstructionSet.getDefaultConstruction(subSurface));

  EXPECT_TRUE(subSurface.setSubSurfaceType("Skylight"));
  EXPECT_FALSE(defaultConstructionSet.getDefaultConstruction(subSurface));
  EXPECT_TRUE(defaultSubSurfaceConstructions.setSkylightConstruction(construction));
  ASSERT_TRUE(defaultConstructionSet.getDefaultConstruction(subSurface));
  EXPECT_EQ(construction.handle(), defaultConstructionSet.getDefaultConstruction(subSurface)->handle());
  defaultSubSurfaceConstructions.resetSkylightConstruction();
  EXPECT_FALSE(defaultConstructionSet.getDefaultConstruction(subSurface));
}

TEST_F(ModelFixture, DefaultConstructionSet_InteriorSubSurfaces)
{
  Model model;

  Point3dVector points;
  points.push_back(Point3d(0,1,0));
  points.push_back(Point3d(0,0,0));
  points.push_back(Point3d(1,0,0));
  Space space(model);
  Surface surface(points, model);
  surface.setSpace(space);
  EXPECT_FALSE(surface.setOutsideBoundaryCondition("Surface"));

  points.clear();
  points.push_back(Point3d(1,0,0));
  points.push_back(Point3d(0,0,0));
  points.push_back(Point3d(0,1,0));
  Space space2(model);
  Surface surface2(points, model);
  surface2.setSpace(space2);
  EXPECT_TRUE(surface.setAdjacentSurface(surface2));
  EXPECT_EQ("Surface", surface.outsideBoundaryCondition());
  EXPECT_EQ("Surface", surface2.outsideBoundaryCondition());
  ASSERT_TRUE(surface.adjacentSurface());
  ASSERT_TRUE(surface2.adjacentSurface());
  EXPECT_EQ(surface2.handle(), surface.adjacentSurface()->handle());
  EXPECT_EQ(surface.handle(), surface2.adjacentSurface()->handle());

  SubSurface subSurface(points, model);
  EXPECT_TRUE(subSurface.setSurface(surface));

  DefaultConstructionSet defaultConstructionSet(model);
  DefaultSubSurfaceConstructions defaultSubSurfaceConstructions(model);
  Construction construction(model);

  EXPECT_FALSE(defaultConstructionSet.defaultInteriorSubSurfaceConstructions());
  EXPECT_TRUE(defaultConstructionSet.setDefaultInteriorSubSurfaceConstructions(defaultSubSurfaceConstructions));
  ASSERT_TRUE(defaultConstructionSet.defaultInteriorSubSurfaceConstructions());
  EXPECT_EQ(defaultSubSurfaceConstructions.handle(), defaultConstructionSet.defaultInteriorSubSurfaceConstructions()->handle());

  EXPECT_TRUE(subSurface.setSubSurfaceType("FixedWindow"));
  EXPECT_FALSE(defaultConstructionSet.getDefaultConstruction(subSurface));
  EXPECT_TRUE(defaultSubSurfaceConstructions.setFixedWindowConstruction(construction));
  ASSERT_TRUE(defaultConstructionSet.getDefaultConstruction(subSurface));
  EXPECT_EQ(construction.handle(), defaultConstructionSet.getDefaultConstruction(subSurface)->handle());
  defaultSubSurfaceConstructions.resetFixedWindowConstruction();
  EXPECT_FALSE(defaultConstructionSet.getDefaultConstruction(subSurface));

  EXPECT_TRUE(subSurface.setSubSurfaceType("Door"));
  EXPECT_FALSE(defaultConstructionSet.getDefaultConstruction(subSurface));
  EXPECT_TRUE(defaultSubSurfaceConstructions.setDoorConstruction(construction));
  ASSERT_TRUE(defaultConstructionSet.getDefaultConstruction(subSurface));
  EXPECT_EQ(construction.handle(), defaultConstructionSet.getDefaultConstruction(subSurface)->handle());
  defaultSubSurfaceConstructions.resetDoorConstruction();
  EXPECT_FALSE(defaultConstructionSet.getDefaultConstruction(subSurface));

  EXPECT_TRUE(subSurface.setSubSurfaceType("Skylight"));
  EXPECT_FALSE(defaultConstructionSet.getDefaultConstruction(subSurface));
  EXPECT_TRUE(defaultSubSurfaceConstructions.setSkylightConstruction(construction));
  ASSERT_TRUE(defaultConstructionSet.getDefaultConstruction(subSurface));
  EXPECT_EQ(construction.handle(), defaultConstructionSet.getDefaultConstruction(subSurface)->handle());
  defaultSubSurfaceConstructions.resetSkylightConstruction();
  EXPECT_FALSE(defaultConstructionSet.getDefaultConstruction(subSurface));
}

TEST_F(ModelFixture, DefaultConstructionSet_InteriorPartitionSurface)
{
  Model model;

  Point3dVector points;
  points.push_back(Point3d(0,1,0));
  points.push_back(Point3d(0,0,0));
  points.push_back(Point3d(1,0,0));
  InteriorPartitionSurface surface(points, model);

  DefaultConstructionSet defaultConstructionSet(model);
  Construction construction(model);

  EXPECT_FALSE(defaultConstructionSet.interiorPartitionConstruction());
  EXPECT_FALSE(defaultConstructionSet.getDefaultConstruction(surface));
  EXPECT_TRUE(defaultConstructionSet.setInteriorPartitionConstruction(construction));
  ASSERT_TRUE(defaultConstructionSet.interiorPartitionConstruction());
  EXPECT_EQ(construction.handle(), defaultConstructionSet.interiorPartitionConstruction()->handle());
  ASSERT_TRUE(defaultConstructionSet.getDefaultConstruction(surface));
  EXPECT_EQ(construction.handle(), defaultConstructionSet.getDefaultConstruction(surface)->handle());
  defaultConstructionSet.resetInteriorPartitionConstruction();
  EXPECT_FALSE(defaultConstructionSet.getDefaultConstruction(surface));

}

TEST_F(ModelFixture, DefaultConstructionSet_ShadingSurface)
{
  Model model;

  Point3dVector points;
  points.push_back(Point3d(0,1,0));
  points.push_back(Point3d(0,0,0));
  points.push_back(Point3d(1,0,0));
  ShadingSurface surface(points, model);

  ShadingSurfaceGroup shadingSurfaceGroup(model);
  surface.setShadingSurfaceGroup(shadingSurfaceGroup);

  DefaultConstructionSet defaultConstructionSet(model);
  DefaultSurfaceConstructions defaultSurfaceConstructions(model);
  Construction construction(model);

  EXPECT_TRUE(shadingSurfaceGroup.setShadingSurfaceType("Site"));
  EXPECT_EQ("Site", shadingSurfaceGroup.shadingSurfaceType());

  EXPECT_FALSE(defaultConstructionSet.siteShadingConstruction());
  EXPECT_FALSE(defaultConstructionSet.getDefaultConstruction(surface));
  EXPECT_TRUE(defaultConstructionSet.setSiteShadingConstruction(construction));
  ASSERT_TRUE(defaultConstructionSet.siteShadingConstruction());
  EXPECT_EQ(construction.handle(), defaultConstructionSet.siteShadingConstruction()->handle());
  ASSERT_TRUE(defaultConstructionSet.getDefaultConstruction(surface));
  EXPECT_EQ(construction.handle(), defaultConstructionSet.getDefaultConstruction(surface)->handle());
  defaultConstructionSet.resetSiteShadingConstruction();
  EXPECT_FALSE(defaultConstructionSet.getDefaultConstruction(surface));

  EXPECT_TRUE(shadingSurfaceGroup.setShadingSurfaceType("Building"));
  EXPECT_EQ("Building", shadingSurfaceGroup.shadingSurfaceType());

  EXPECT_FALSE(defaultConstructionSet.buildingShadingConstruction());
  EXPECT_FALSE(defaultConstructionSet.getDefaultConstruction(surface));
  EXPECT_TRUE(defaultConstructionSet.setBuildingShadingConstruction(construction));
  ASSERT_TRUE(defaultConstructionSet.buildingShadingConstruction());
  EXPECT_EQ(construction.handle(), defaultConstructionSet.buildingShadingConstruction()->handle());
  ASSERT_TRUE(defaultConstructionSet.getDefaultConstruction(surface));
  EXPECT_EQ(construction.handle(), defaultConstructionSet.getDefaultConstruction(surface)->handle());
  defaultConstructionSet.resetBuildingShadingConstruction();
  EXPECT_FALSE(defaultConstructionSet.getDefaultConstruction(surface));

  EXPECT_FALSE(shadingSurfaceGroup.setShadingSurfaceType("Space"));
  Space space(model);
  EXPECT_TRUE(shadingSurfaceGroup.setSpace(space));
  EXPECT_EQ("Space", shadingSurfaceGroup.shadingSurfaceType());

  EXPECT_FALSE(defaultConstructionSet.spaceShadingConstruction());
  EXPECT_FALSE(defaultConstructionSet.getDefaultConstruction(surface));
  EXPECT_TRUE(defaultConstructionSet.setSpaceShadingConstruction(construction));
  ASSERT_TRUE(defaultConstructionSet.spaceShadingConstruction());
  EXPECT_EQ(construction.handle(), defaultConstructionSet.spaceShadingConstruction()->handle());
  ASSERT_TRUE(defaultConstructionSet.getDefaultConstruction(surface));
  EXPECT_EQ(construction.handle(), defaultConstructionSet.getDefaultConstruction(surface)->handle());
  defaultConstructionSet.resetSpaceShadingConstruction();
  EXPECT_FALSE(defaultConstructionSet.getDefaultConstruction(surface));
}


TEST_F(ModelFixture, DefaultConstructionSet_Clone)
{
  Model model;

  StandardOpaqueMaterial m1(model);
  StandardOpaqueMaterial m2(model);

  Construction construction(model);
  EXPECT_TRUE(construction.insertLayer(0, m1));
  EXPECT_TRUE(construction.insertLayer(1, m2));

  DefaultSurfaceConstructions defaultSurfaceConstructions(model);
  // this test was failing due to characters in the object's name that were not handled correctly in clone
  EXPECT_TRUE(defaultSurfaceConstructions.setName("Attic_ClimateZone 1-8 (generic)_Exterior_DefSurfCons"));
  EXPECT_TRUE(defaultSurfaceConstructions.setFloorConstruction(construction));
  EXPECT_TRUE(defaultSurfaceConstructions.setWallConstruction(construction));
  EXPECT_TRUE(defaultSurfaceConstructions.setRoofCeilingConstruction(construction));

  DefaultConstructionSet defaultConstructionSet1(model);
  defaultConstructionSet1.setDefaultExteriorSurfaceConstructions(defaultSurfaceConstructions);
  EXPECT_EQ(1u, defaultSurfaceConstructions.getModelObjectSources<model::DefaultConstructionSet>().size());

  DefaultConstructionSet defaultConstructionSet2(model);
  defaultConstructionSet2.setDefaultExteriorSurfaceConstructions(defaultSurfaceConstructions);
  EXPECT_EQ(2u, defaultSurfaceConstructions.getModelObjectSources<model::DefaultConstructionSet>().size());

  DefaultConstructionSet defaultConstructionSet3(model);
  defaultConstructionSet3.setDefaultExteriorSurfaceConstructions(defaultSurfaceConstructions);
  EXPECT_EQ(3u, defaultSurfaceConstructions.getModelObjectSources<model::DefaultConstructionSet>().size());
 
  boost::optional<model::DefaultSurfaceConstructions> defExtSurfCons = defaultConstructionSet1.defaultExteriorSurfaceConstructions();
  ASSERT_TRUE(defExtSurfCons);
  EXPECT_EQ(3u, defExtSurfCons->getModelObjectSources<model::DefaultConstructionSet>().size());
  model::ModelObject clone = defExtSurfCons->clone(model);
  defExtSurfCons = clone.cast<model::DefaultSurfaceConstructions>();
  ASSERT_TRUE(defExtSurfCons);
  defaultConstructionSet1.setDefaultExteriorSurfaceConstructions(*defExtSurfCons);
  EXPECT_EQ(2u, defaultSurfaceConstructions.getModelObjectSources<model::DefaultConstructionSet>().size());
  EXPECT_EQ(1u, defExtSurfCons->getModelObjectSources<model::DefaultConstructionSet>().size());

  defExtSurfCons = defaultConstructionSet2.defaultExteriorSurfaceConstructions();
  ASSERT_TRUE(defExtSurfCons);
  EXPECT_EQ(2u, defExtSurfCons->getModelObjectSources<model::DefaultConstructionSet>().size());
  clone = defExtSurfCons->clone(model);
  defExtSurfCons = clone.cast<model::DefaultSurfaceConstructions>();
  ASSERT_TRUE(defExtSurfCons);
  defaultConstructionSet2.setDefaultExteriorSurfaceConstructions(*defExtSurfCons);
  EXPECT_EQ(1u, defaultSurfaceConstructions.getModelObjectSources<model::DefaultConstructionSet>().size());
  EXPECT_EQ(1u, defExtSurfCons->getModelObjectSources<model::DefaultConstructionSet>().size());

}


TEST_F(ModelFixture, DefaultConstructionSet_CloneBadName)
{
  Model model;

  DefaultSurfaceConstructions defaultSurfaceConstructions(model);
  defaultSurfaceConstructions.setName("Easy name");
  ModelObject clone = defaultSurfaceConstructions.clone(model);
  EXPECT_EQ("Easy name 1", clone.name().get());
  clone = defaultSurfaceConstructions.clone(model);
  EXPECT_EQ("Easy name 2", clone.name().get());

  defaultSurfaceConstructions = DefaultSurfaceConstructions(model);
  defaultSurfaceConstructions.setName("Harder 1-8_(name)");
  clone = defaultSurfaceConstructions.clone(model);
  EXPECT_EQ("Harder 1-8_(name) 1", clone.name().get());
  clone = defaultSurfaceConstructions.clone(model);
  EXPECT_EQ("Harder 1-8_(name) 2", clone.name().get());

  defaultSurfaceConstructions = DefaultSurfaceConstructions(model);
  defaultSurfaceConstructions.setName("*H.a.r.d.e.s.t*^\\1|-|8/_#($name$)?#");
  clone = defaultSurfaceConstructions.clone(model);
  EXPECT_EQ("*H.a.r.d.e.s.t*^\\1|-|8/_#($name$)?# 1", clone.name().get());
  clone = defaultSurfaceConstructions.clone(model);
  EXPECT_EQ("*H.a.r.d.e.s.t*^\\1|-|8/_#($name$)?# 2", clone.name().get());
}
