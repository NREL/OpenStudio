/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Energy Innovation, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

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
#include "../PlanarSurface.hpp"
#include "../PlanarSurface_Impl.hpp"
#include "../Space.hpp"
#include "../Space_Impl.hpp"
#include "../SpaceType.hpp"
#include "../SpaceType_Impl.hpp"
#include "../Building.hpp"
#include "../Building_Impl.hpp"
#include "../BuildingStory.hpp"
#include "../BuildingStory_Impl.hpp"

#include "../../utilities/core/Compare.hpp"
#include "../../utilities/geometry/Point3d.hpp"
#include "../../utilities/geometry/Vector3d.hpp"

#include <utilities/idd/IddEnums.hxx>

#include <fmt/format.h>

#include <map>
#include <set>

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, DefaultConstructionSet) {
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

  EXPECT_FALSE(defaultConstructionSet.adiabaticSurfaceConstruction());
  EXPECT_TRUE(defaultConstructionSet.setAdiabaticSurfaceConstruction(construction));
  ASSERT_TRUE(defaultConstructionSet.adiabaticSurfaceConstruction());
  EXPECT_EQ(construction.handle(), defaultConstructionSet.adiabaticSurfaceConstruction()->handle());
  defaultConstructionSet.resetAdiabaticSurfaceConstruction();
  EXPECT_FALSE(defaultConstructionSet.adiabaticSurfaceConstruction());
}

TEST_F(ModelFixture, DefaultConstructionSet_ExteriorSurfaces) {
  Model model;

  Point3dVector points{
    {0, 1, 0},
    {0, 0, 0},
    {1, 0, 0},
  };
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

TEST_F(ModelFixture, DefaultConstructionSet_InteriorSurfaces) {
  Model model;

  Point3dVector points{
    {0, 1, 0},
    {0, 0, 0},
    {1, 0, 0},
  };
  Space space(model);
  Surface surface(points, model);
  surface.setSpace(space);
  EXPECT_FALSE(surface.setOutsideBoundaryCondition("Surface"));

  points = {
    {1, 0, 0},
    {0, 0, 0},
    {0, 1, 0},
  };
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

TEST_F(ModelFixture, DefaultConstructionSet_GroundContactSurfaces) {
  Model model;

  Point3dVector points{
    {0, 1, 0},
    {0, 0, 0},
    {1, 0, 0},
  };
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

TEST_F(ModelFixture, DefaultConstructionSet_ExteriorSubSurfaces) {
  Model model;

  Point3dVector points{
    {0, 1, 0},
    {0, 0, 0},
    {1, 0, 0},
  };
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

TEST_F(ModelFixture, DefaultConstructionSet_InteriorSubSurfaces) {
  Model model;

  Point3dVector points{
    {0, 1, 0},
    {0, 0, 0},
    {1, 0, 0},
  };
  Space space(model);
  Surface surface(points, model);
  surface.setSpace(space);
  EXPECT_FALSE(surface.setOutsideBoundaryCondition("Surface"));

  points = {
    {1, 0, 0},
    {0, 0, 0},
    {0, 1, 0},
  };
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

TEST_F(ModelFixture, DefaultConstructionSet_InteriorPartitionSurface) {
  Model model;

  Point3dVector points{
    {0, 1, 0},
    {0, 0, 0},
    {1, 0, 0},
  };
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

TEST_F(ModelFixture, DefaultConstructionSet_ShadingSurface) {
  Model model;

  Point3dVector points{
    {0, 1, 0},
    {0, 0, 0},
    {1, 0, 0},
  };
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

TEST_F(ModelFixture, DefaultConstructionSet_AdiabaticSurface) {
  Model model;

  Point3dVector points{
    {0, 1, 0},
    {0, 0, 0},
    {1, 0, 0},
  };
  Space space(model);
  Surface surface(points, model);
  surface.setSpace(space);
  EXPECT_TRUE(surface.setOutsideBoundaryCondition("Adiabatic"));

  DefaultConstructionSet defaultConstructionSet(model);
  Construction construction(model);

  EXPECT_FALSE(defaultConstructionSet.adiabaticSurfaceConstruction());
  EXPECT_FALSE(defaultConstructionSet.getDefaultConstruction(surface));
  EXPECT_TRUE(defaultConstructionSet.setAdiabaticSurfaceConstruction(construction));
  ASSERT_TRUE(defaultConstructionSet.adiabaticSurfaceConstruction());
  EXPECT_EQ(construction.handle(), defaultConstructionSet.adiabaticSurfaceConstruction()->handle());
  ASSERT_TRUE(defaultConstructionSet.getDefaultConstruction(surface));
  EXPECT_EQ(construction.handle(), defaultConstructionSet.getDefaultConstruction(surface)->handle());
  defaultConstructionSet.resetAdiabaticSurfaceConstruction();
  EXPECT_FALSE(defaultConstructionSet.getDefaultConstruction(surface));
}

TEST_F(ModelFixture, DefaultConstructionSet_Clone) {
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

TEST_F(ModelFixture, DefaultConstructionSet_CloneBadName) {
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

TEST_F(ModelFixture, DefaultConstructionSet_Merge) {
  Model model;

  Construction c1(model);
  Construction c2(model);

  DefaultConstructionSet dcs1(model);
  DefaultConstructionSet dcs2(model);

  // dcs1 has some direct constructions set, dcs2 has all of them
  EXPECT_TRUE(dcs1.setInteriorPartitionConstruction(c1));
  EXPECT_TRUE(dcs1.setAdiabaticSurfaceConstruction(c1));

  EXPECT_TRUE(dcs2.setInteriorPartitionConstruction(c2));
  EXPECT_TRUE(dcs2.setSpaceShadingConstruction(c2));
  EXPECT_TRUE(dcs2.setBuildingShadingConstruction(c2));
  EXPECT_TRUE(dcs2.setSiteShadingConstruction(c2));
  EXPECT_TRUE(dcs2.setAdiabaticSurfaceConstruction(c2));

  // dcs1 has no exterior surface constructions, both have (different) interior ones
  DefaultSurfaceConstructions ext2(model);
  EXPECT_TRUE(ext2.setWallConstruction(c2));
  EXPECT_TRUE(dcs2.setDefaultExteriorSurfaceConstructions(ext2));

  DefaultSurfaceConstructions int1(model);
  EXPECT_TRUE(int1.setFloorConstruction(c1));
  EXPECT_TRUE(dcs1.setDefaultInteriorSurfaceConstructions(int1));
  DefaultSurfaceConstructions int2(model);
  EXPECT_TRUE(int2.setFloorConstruction(c2));
  EXPECT_TRUE(int2.setWallConstruction(c2));
  EXPECT_TRUE(dcs2.setDefaultInteriorSurfaceConstructions(int2));

  dcs1.merge(dcs2);

  // Existing fields of this win, missing ones are taken from other
  ASSERT_TRUE(dcs1.interiorPartitionConstruction());
  EXPECT_EQ(c1, dcs1.interiorPartitionConstruction().get());
  ASSERT_TRUE(dcs1.adiabaticSurfaceConstruction());
  EXPECT_EQ(c1, dcs1.adiabaticSurfaceConstruction().get());
  ASSERT_TRUE(dcs1.spaceShadingConstruction());
  EXPECT_EQ(c2, dcs1.spaceShadingConstruction().get());
  ASSERT_TRUE(dcs1.buildingShadingConstruction());
  EXPECT_EQ(c2, dcs1.buildingShadingConstruction().get());
  ASSERT_TRUE(dcs1.siteShadingConstruction());
  EXPECT_EQ(c2, dcs1.siteShadingConstruction().get());

  // Missing sub-object: points to other's, no clone
  ASSERT_TRUE(dcs1.defaultExteriorSurfaceConstructions());
  EXPECT_EQ(ext2, dcs1.defaultExteriorSurfaceConstructions().get());

  // Both have one, not shared: merged in place
  ASSERT_TRUE(dcs1.defaultInteriorSurfaceConstructions());
  EXPECT_EQ(int1, dcs1.defaultInteriorSurfaceConstructions().get());
  ASSERT_TRUE(int1.floorConstruction());
  EXPECT_EQ(c1, int1.floorConstruction().get());
  ASSERT_TRUE(int1.wallConstruction());
  EXPECT_EQ(c2, int1.wallConstruction().get());

  // Adiabatic is taken from other when missing
  DefaultConstructionSet dcs3(model);
  dcs3.merge(dcs2);
  ASSERT_TRUE(dcs3.adiabaticSurfaceConstruction());
  EXPECT_EQ(c2, dcs3.adiabaticSurfaceConstruction().get());

  EXPECT_EQ(3u, model.getConcreteModelObjects<DefaultSurfaceConstructions>().size());
}

// Helpers for the DefaultConstructionSet::collapseToSpaceAndBuilding tests
namespace {

// Only the slots the collapse tests need; extend as needed. Designated initializers must follow declaration order
struct DCSSpec
{
  boost::optional<Construction> extFloor;
  boost::optional<Construction> extWall;
  boost::optional<Construction> extRoof;
  boost::optional<Construction> intWall;
  boost::optional<Construction> groundFloor;
  boost::optional<Construction> extFixedWindow;
  boost::optional<Construction> interiorPartition;
  boost::optional<Construction> spaceShading;
  boost::optional<Construction> adiabatic;
};

// Creates a DefaultConstructionSet that fills exactly the slots set in spec, with new Default(Sub)SurfaceConstructions as needed
DefaultConstructionSet makeDCS(Model& m, const DCSSpec& spec) {
  DefaultConstructionSet dcs(m);

  if (spec.extFloor || spec.extWall || spec.extRoof) {
    DefaultSurfaceConstructions ext(m);
    if (spec.extFloor) {
      EXPECT_TRUE(ext.setFloorConstruction(*spec.extFloor));
    }
    if (spec.extWall) {
      EXPECT_TRUE(ext.setWallConstruction(*spec.extWall));
    }
    if (spec.extRoof) {
      EXPECT_TRUE(ext.setRoofCeilingConstruction(*spec.extRoof));
    }
    EXPECT_TRUE(dcs.setDefaultExteriorSurfaceConstructions(ext));
  }

  if (spec.intWall) {
    DefaultSurfaceConstructions interior(m);
    EXPECT_TRUE(interior.setWallConstruction(*spec.intWall));
    EXPECT_TRUE(dcs.setDefaultInteriorSurfaceConstructions(interior));
  }

  if (spec.groundFloor) {
    DefaultSurfaceConstructions ground(m);
    EXPECT_TRUE(ground.setFloorConstruction(*spec.groundFloor));
    EXPECT_TRUE(dcs.setDefaultGroundContactSurfaceConstructions(ground));
  }

  if (spec.extFixedWindow) {
    DefaultSubSurfaceConstructions extSub(m);
    EXPECT_TRUE(extSub.setFixedWindowConstruction(*spec.extFixedWindow));
    EXPECT_TRUE(dcs.setDefaultExteriorSubSurfaceConstructions(extSub));
  }

  if (spec.interiorPartition) {
    EXPECT_TRUE(dcs.setInteriorPartitionConstruction(*spec.interiorPartition));
  }
  if (spec.spaceShading) {
    EXPECT_TRUE(dcs.setSpaceShadingConstruction(*spec.spaceShading));
  }
  if (spec.adiabatic) {
    EXPECT_TRUE(dcs.setAdiabaticSurfaceConstruction(*spec.adiabatic));
  }

  return dcs;
}

// The construction of one slot of the DCS's exterior surface constructions, e.g. extSlot(dcs, &DefaultSurfaceConstructions::wallConstruction)
boost::optional<ConstructionBase> extSlot(const DefaultConstructionSet& dcs,
                                          boost::optional<ConstructionBase> (DefaultSurfaceConstructions::*slot)() const) {
  if (auto ext_ = dcs.defaultExteriorSurfaceConstructions()) {
    return ((*ext_).*slot)();
  }
  return boost::none;
}

}  // namespace

// Building group: Building DCS (search distance 4), then Building SpaceType DCS (5), collapsed onto the Building

TEST_F(ModelFixture, DefaultConstructionSet_collapseToSpaceAndBuilding_BuildingGroup_Noop) {

  // No Building: nothing to do, and no Building gets instantiated
  {
    Model m;
    SpaceType spaceType(m);
    DefaultConstructionSet stDCS(m);
    EXPECT_TRUE(spaceType.setDefaultConstructionSet(stDCS));

    EXPECT_TRUE(DefaultConstructionSet::collapseToSpaceAndBuilding(m).empty());
    EXPECT_FALSE(m.building());
    EXPECT_EQ(1u, m.getConcreteModelObjects<DefaultConstructionSet>().size());
  }

  // Building without any DCS, and no SpaceType
  {
    Model m;
    auto building = m.getUniqueModelObject<Building>();

    EXPECT_TRUE(DefaultConstructionSet::collapseToSpaceAndBuilding(m).empty());
    EXPECT_FALSE(building.defaultConstructionSet());
    EXPECT_EQ(0u, m.getConcreteModelObjects<DefaultConstructionSet>().size());
  }

  // Building DCS only: kept as is
  {
    Model m;
    auto building = m.getUniqueModelObject<Building>();
    Construction adiabatic(m);
    auto bDCS = makeDCS(m, {.adiabatic = adiabatic});
    EXPECT_TRUE(building.setDefaultConstructionSet(bDCS));

    EXPECT_TRUE(DefaultConstructionSet::collapseToSpaceAndBuilding(m).empty());
    ASSERT_TRUE(building.defaultConstructionSet());
    EXPECT_EQ(bDCS, building.defaultConstructionSet().get());
    EXPECT_EQ(1u, m.getConcreteModelObjects<DefaultConstructionSet>().size());
  }

  // Building and its SpaceType point to the same DCS: kept as is
  {
    Model m;
    auto building = m.getUniqueModelObject<Building>();
    SpaceType spaceType(m);
    EXPECT_TRUE(building.setSpaceType(spaceType));
    Construction adiabatic(m);
    auto dcs = makeDCS(m, {.adiabatic = adiabatic});
    EXPECT_TRUE(building.setDefaultConstructionSet(dcs));
    EXPECT_TRUE(spaceType.setDefaultConstructionSet(dcs));

    EXPECT_TRUE(DefaultConstructionSet::collapseToSpaceAndBuilding(m).empty());
    ASSERT_TRUE(building.defaultConstructionSet());
    EXPECT_EQ(dcs, building.defaultConstructionSet().get());
    ASSERT_TRUE(spaceType.defaultConstructionSet());
    EXPECT_EQ(dcs, spaceType.defaultConstructionSet().get());
    EXPECT_EQ(1u, m.getConcreteModelObjects<DefaultConstructionSet>().size());
  }
}

TEST_F(ModelFixture, DefaultConstructionSet_collapseToSpaceAndBuilding_BuildingGroup_EmptySetIsReset) {

  // Building has an empty DCS: it contributes nothing, so it's reset
  Model m;
  auto building = m.getUniqueModelObject<Building>();
  DefaultConstructionSet bDCS(m);
  EXPECT_TRUE(building.setDefaultConstructionSet(bDCS));

  EXPECT_TRUE(DefaultConstructionSet::collapseToSpaceAndBuilding(m).empty());
  EXPECT_FALSE(building.defaultConstructionSet());
  EXPECT_EQ(1u, m.getConcreteModelObjects<DefaultConstructionSet>().size());  // not removed, just no longer referenced
}

TEST_F(ModelFixture, DefaultConstructionSet_collapseToSpaceAndBuilding_BuildingGroup_SpaceTypeSetMovedUp) {

  // Building SpaceType DCS only: moved up to the Building, no clone. The SpaceType keeps it (no purge)
  Model m;
  auto building = m.getUniqueModelObject<Building>();
  SpaceType spaceType(m);
  EXPECT_TRUE(building.setSpaceType(spaceType));
  Construction adiabatic(m);
  auto stDCS = makeDCS(m, {.adiabatic = adiabatic});
  EXPECT_TRUE(spaceType.setDefaultConstructionSet(stDCS));

  EXPECT_TRUE(DefaultConstructionSet::collapseToSpaceAndBuilding(m).empty());
  ASSERT_TRUE(building.defaultConstructionSet());
  EXPECT_EQ(stDCS, building.defaultConstructionSet().get());
  ASSERT_TRUE(spaceType.defaultConstructionSet());
  EXPECT_EQ(stDCS, spaceType.defaultConstructionSet().get());
  EXPECT_EQ(1u, m.getConcreteModelObjects<DefaultConstructionSet>().size());
}

TEST_F(ModelFixture, DefaultConstructionSet_collapseToSpaceAndBuilding_BuildingGroup_RedundantSetIgnored) {

  // The SpaceType DCS only fills slots the Building DCS already fills (with different constructions): it's ignored, the
  // Building keeps its own, no clone
  Model m;
  auto building = m.getUniqueModelObject<Building>();
  SpaceType spaceType(m);
  EXPECT_TRUE(building.setSpaceType(spaceType));

  Construction bWall(m);
  auto bDCS = makeDCS(m, {.extWall = bWall});
  EXPECT_TRUE(building.setDefaultConstructionSet(bDCS));

  Construction stWall(m);
  auto stDCS = makeDCS(m, {.extWall = stWall});
  EXPECT_TRUE(spaceType.setDefaultConstructionSet(stDCS));
  auto stExt = stDCS.defaultExteriorSurfaceConstructions().get();

  EXPECT_TRUE(DefaultConstructionSet::collapseToSpaceAndBuilding(m).empty());
  ASSERT_TRUE(building.defaultConstructionSet());
  EXPECT_EQ(bDCS, building.defaultConstructionSet().get());
  EXPECT_EQ(2u, m.getConcreteModelObjects<DefaultConstructionSet>().size());
  EXPECT_EQ(2u, m.getConcreteModelObjects<DefaultSurfaceConstructions>().size());

  // No purge: the SpaceType keeps its DCS
  ASSERT_TRUE(spaceType.defaultConstructionSet());
  EXPECT_EQ(stDCS, spaceType.defaultConstructionSet().get());
  EXPECT_EQ(1u, stDCS.directUseCount());
  EXPECT_EQ(1u, stExt.directUseCount());
  EXPECT_EQ(1u, bDCS.directUseCount());
}

TEST_F(ModelFixture, DefaultConstructionSet_collapseToSpaceAndBuilding_BuildingGroup_MergedInPlace) {

  // The SpaceType DCS fills slots the Building DCS doesn't, and the Building DCS is used by the Building only: the SpaceType's
  // slots are merged into the Building DCS itself, Building's slots win, nothing is created and the SpaceType DCS is untouched
  Model m;
  auto building = m.getUniqueModelObject<Building>();
  SpaceType spaceType(m);
  EXPECT_TRUE(building.setSpaceType(spaceType));

  Construction bWall(m);
  auto bDCS = makeDCS(m, {.extWall = bWall});
  bDCS.setName("Building DCS");
  EXPECT_TRUE(building.setDefaultConstructionSet(bDCS));

  Construction stWall(m);
  Construction stRoof(m);
  Construction stAdiabatic(m);
  auto stDCS = makeDCS(m, {.extWall = stWall, .extRoof = stRoof, .adiabatic = stAdiabatic});
  EXPECT_TRUE(spaceType.setDefaultConstructionSet(stDCS));
  auto stExt = stDCS.defaultExteriorSurfaceConstructions().get();

  EXPECT_TRUE(DefaultConstructionSet::collapseToSpaceAndBuilding(m).empty());

  ASSERT_TRUE(building.defaultConstructionSet());
  EXPECT_EQ(bDCS, building.defaultConstructionSet().get());
  EXPECT_EQ("Building DCS", bDCS.nameString());

  // Merged content: Building's wall wins, roof and adiabatic come from the SpaceType
  ASSERT_TRUE(extSlot(bDCS, &DefaultSurfaceConstructions::wallConstruction));
  EXPECT_EQ(bWall, extSlot(bDCS, &DefaultSurfaceConstructions::wallConstruction).get());
  ASSERT_TRUE(extSlot(bDCS, &DefaultSurfaceConstructions::roofCeilingConstruction));
  EXPECT_EQ(stRoof, extSlot(bDCS, &DefaultSurfaceConstructions::roofCeilingConstruction).get());
  ASSERT_TRUE(bDCS.adiabaticSurfaceConstruction());
  EXPECT_EQ(stAdiabatic, bDCS.adiabaticSurfaceConstruction().get());

  // SpaceType DCS untouched
  ASSERT_TRUE(stDCS.defaultExteriorSurfaceConstructions());
  EXPECT_EQ(stExt, stDCS.defaultExteriorSurfaceConstructions().get());
  ASSERT_TRUE(stExt.wallConstruction());
  EXPECT_EQ(stWall, stExt.wallConstruction().get());
  EXPECT_EQ(2u, m.getConcreteModelObjects<DefaultConstructionSet>().size());
}

TEST_F(ModelFixture, DefaultConstructionSet_collapseToSpaceAndBuilding_BuildingGroup_MergedCloned_UsedBySpace) {

  // Same, but a Space also uses the Building DCS directly, so it can't be modified: a merged clone is created, Building's
  // slots win, and the originals are untouched
  Model m;
  auto building = m.getUniqueModelObject<Building>();
  building.setName("HQ");
  SpaceType spaceType(m);
  spaceType.setName("Office");
  EXPECT_TRUE(building.setSpaceType(spaceType));

  Construction bWall(m);
  auto bDCS = makeDCS(m, {.extWall = bWall});
  EXPECT_TRUE(building.setDefaultConstructionSet(bDCS));
  Space space(m);
  EXPECT_TRUE(space.setDefaultConstructionSet(bDCS));
  auto bExt = bDCS.defaultExteriorSurfaceConstructions().get();

  Construction stWall(m);
  Construction stRoof(m);
  Construction stAdiabatic(m);
  auto stDCS = makeDCS(m, {.extWall = stWall, .extRoof = stRoof, .adiabatic = stAdiabatic});
  EXPECT_TRUE(spaceType.setDefaultConstructionSet(stDCS));
  auto stExt = stDCS.defaultExteriorSurfaceConstructions().get();

  auto created = DefaultConstructionSet::collapseToSpaceAndBuilding(m);
  ASSERT_EQ(1u, created.size());
  const auto& merged = created.front();
  EXPECT_EQ("Building HQ @ SpaceType Office", merged.nameString());
  EXPECT_NE(bDCS, merged);
  EXPECT_NE(stDCS, merged);

  ASSERT_TRUE(building.defaultConstructionSet());
  EXPECT_EQ(merged, building.defaultConstructionSet().get());
  ASSERT_TRUE(spaceType.defaultConstructionSet());
  EXPECT_EQ(stDCS, spaceType.defaultConstructionSet().get());

  // Merged content: Building's wall wins, roof and adiabatic come from the SpaceType
  ASSERT_TRUE(merged.defaultExteriorSurfaceConstructions());
  auto mergedExt = merged.defaultExteriorSurfaceConstructions().get();
  ASSERT_TRUE(mergedExt.wallConstruction());
  EXPECT_EQ(bWall, mergedExt.wallConstruction().get());
  ASSERT_TRUE(mergedExt.roofCeilingConstruction());
  EXPECT_EQ(stRoof, mergedExt.roofCeilingConstruction().get());
  EXPECT_FALSE(mergedExt.floorConstruction());
  ASSERT_TRUE(merged.adiabaticSurfaceConstruction());
  EXPECT_EQ(stAdiabatic, merged.adiabaticSurfaceConstruction().get());

  // Originals untouched: the shared exterior surface constructions got cloned before being merged into
  EXPECT_NE(bExt, mergedExt);
  EXPECT_NE(stExt, mergedExt);
  ASSERT_TRUE(bDCS.defaultExteriorSurfaceConstructions());
  EXPECT_EQ(bExt, bDCS.defaultExteriorSurfaceConstructions().get());
  EXPECT_FALSE(bExt.roofCeilingConstruction());
  EXPECT_FALSE(bDCS.adiabaticSurfaceConstruction());
  EXPECT_EQ(3u, m.getConcreteModelObjects<DefaultConstructionSet>().size());
  EXPECT_EQ(3u, m.getConcreteModelObjects<DefaultSurfaceConstructions>().size());
}

TEST_F(ModelFixture, DefaultConstructionSet_collapseToSpaceAndBuilding_Purge) {

  // Same setup as BuildingGroup_RedundantSetIgnored, plus a DefaultSubSurfaceConstructions shared by both sets, and a DCS that
  // is already unused before the call
  Model m;
  auto building = m.getUniqueModelObject<Building>();
  SpaceType spaceType(m);
  EXPECT_TRUE(building.setSpaceType(spaceType));

  DefaultSubSurfaceConstructions sharedExtSub(m);
  Construction window(m);
  EXPECT_TRUE(sharedExtSub.setFixedWindowConstruction(window));

  Construction bWall(m);
  auto bDCS = makeDCS(m, {.extWall = bWall});
  EXPECT_TRUE(bDCS.setDefaultExteriorSubSurfaceConstructions(sharedExtSub));
  EXPECT_TRUE(building.setDefaultConstructionSet(bDCS));

  Construction stWall(m);
  auto stDCS = makeDCS(m, {.extWall = stWall});
  EXPECT_TRUE(stDCS.setDefaultExteriorSubSurfaceConstructions(sharedExtSub));
  EXPECT_TRUE(spaceType.setDefaultConstructionSet(stDCS));
  auto stExt = stDCS.defaultExteriorSurfaceConstructions().get();

  Construction unusedWall(m);
  auto unusedDCS = makeDCS(m, {.extWall = unusedWall});
  auto unusedExt = unusedDCS.defaultExteriorSurfaceConstructions().get();
  EXPECT_EQ(0u, unusedDCS.directUseCount());

  EXPECT_TRUE(DefaultConstructionSet::collapseToSpaceAndBuilding(m, true).empty());

  // Same result for the Building as without purge
  ASSERT_TRUE(building.defaultConstructionSet());
  EXPECT_EQ(bDCS, building.defaultConstructionSet().get());

  // The SpaceType's DCS is reset and removed, along with its now unused DefaultSurfaceConstructions
  EXPECT_FALSE(spaceType.defaultConstructionSet());
  EXPECT_TRUE(stDCS.handle().isNull());
  EXPECT_TRUE(stExt.handle().isNull());

  // The DefaultSubSurfaceConstructions it shared with the Building's DCS is still in use, so it stays
  EXPECT_FALSE(sharedExtSub.handle().isNull());
  ASSERT_TRUE(bDCS.defaultExteriorSubSurfaceConstructions());
  EXPECT_EQ(sharedExtSub, bDCS.defaultExteriorSubSurfaceConstructions().get());

  // A DCS that was already unused before the call survives, along with its sub-object
  EXPECT_FALSE(unusedDCS.handle().isNull());
  EXPECT_FALSE(unusedExt.handle().isNull());

  EXPECT_EQ(2u, m.getConcreteModelObjects<DefaultConstructionSet>().size());          // bDCS, unusedDCS
  EXPECT_EQ(2u, m.getConcreteModelObjects<DefaultSurfaceConstructions>().size());     // bExt, unusedExt
  EXPECT_EQ(1u, m.getConcreteModelObjects<DefaultSubSurfaceConstructions>().size());  // sharedExtSub
}

// Space group: Space DCS (search distance 1), then its SpaceType DCS if explicitly set (2), then its BuildingStory DCS (3),
// collapsed onto the Space

TEST_F(ModelFixture, DefaultConstructionSet_collapseToSpaceAndBuilding_SpaceGroup_Noop) {

  // A SpaceType and a BuildingStory, but no DCS anywhere: nothing to do
  {
    Model m;
    Space space(m);
    SpaceType spaceType(m);
    EXPECT_TRUE(space.setSpaceType(spaceType));
    BuildingStory story(m);
    EXPECT_TRUE(space.setBuildingStory(story));

    EXPECT_TRUE(DefaultConstructionSet::collapseToSpaceAndBuilding(m).empty());
    EXPECT_FALSE(space.defaultConstructionSet());
    EXPECT_EQ(0u, m.getConcreteModelObjects<DefaultConstructionSet>().size());
  }

  // Space DCS only: kept as is
  {
    Model m;
    Space space(m);
    Construction adiabatic(m);
    auto sDCS = makeDCS(m, {.adiabatic = adiabatic});
    EXPECT_TRUE(space.setDefaultConstructionSet(sDCS));

    EXPECT_TRUE(DefaultConstructionSet::collapseToSpaceAndBuilding(m).empty());
    ASSERT_TRUE(space.defaultConstructionSet());
    EXPECT_EQ(sDCS, space.defaultConstructionSet().get());
    EXPECT_EQ(1u, m.getConcreteModelObjects<DefaultConstructionSet>().size());
  }

  // Space, its SpaceType and its BuildingStory all point to the same DCS: kept as is
  {
    Model m;
    Space space(m);
    SpaceType spaceType(m);
    EXPECT_TRUE(space.setSpaceType(spaceType));
    BuildingStory story(m);
    EXPECT_TRUE(space.setBuildingStory(story));
    Construction adiabatic(m);
    auto dcs = makeDCS(m, {.adiabatic = adiabatic});
    EXPECT_TRUE(space.setDefaultConstructionSet(dcs));
    EXPECT_TRUE(spaceType.setDefaultConstructionSet(dcs));
    EXPECT_TRUE(story.setDefaultConstructionSet(dcs));

    EXPECT_TRUE(DefaultConstructionSet::collapseToSpaceAndBuilding(m).empty());
    ASSERT_TRUE(space.defaultConstructionSet());
    EXPECT_EQ(dcs, space.defaultConstructionSet().get());
    ASSERT_TRUE(spaceType.defaultConstructionSet());
    EXPECT_EQ(dcs, spaceType.defaultConstructionSet().get());
    ASSERT_TRUE(story.defaultConstructionSet());
    EXPECT_EQ(dcs, story.defaultConstructionSet().get());
    EXPECT_EQ(1u, m.getConcreteModelObjects<DefaultConstructionSet>().size());
  }

  // Several Spaces sharing the same Space DCS, nothing below: all kept as is
  {
    Model m;
    Space space1(m);
    Space space2(m);
    Construction adiabatic(m);
    auto sDCS = makeDCS(m, {.adiabatic = adiabatic});
    EXPECT_TRUE(space1.setDefaultConstructionSet(sDCS));
    EXPECT_TRUE(space2.setDefaultConstructionSet(sDCS));

    EXPECT_TRUE(DefaultConstructionSet::collapseToSpaceAndBuilding(m).empty());
    ASSERT_TRUE(space1.defaultConstructionSet());
    EXPECT_EQ(sDCS, space1.defaultConstructionSet().get());
    ASSERT_TRUE(space2.defaultConstructionSet());
    EXPECT_EQ(sDCS, space2.defaultConstructionSet().get());
    EXPECT_EQ(1u, m.getConcreteModelObjects<DefaultConstructionSet>().size());
  }
}

TEST_F(ModelFixture, DefaultConstructionSet_collapseToSpaceAndBuilding_SpaceGroup_LowerSetMovedDown) {

  // Only one level below the Space has a DCS: it's assigned to the Space as is, no clone. The lower level keeps it (no purge)

  // SpaceType DCS only
  {
    Model m;
    Space space(m);
    SpaceType spaceType(m);
    EXPECT_TRUE(space.setSpaceType(spaceType));
    Construction adiabatic(m);
    auto stDCS = makeDCS(m, {.adiabatic = adiabatic});
    EXPECT_TRUE(spaceType.setDefaultConstructionSet(stDCS));

    EXPECT_TRUE(DefaultConstructionSet::collapseToSpaceAndBuilding(m).empty());
    ASSERT_TRUE(space.defaultConstructionSet());
    EXPECT_EQ(stDCS, space.defaultConstructionSet().get());
    ASSERT_TRUE(spaceType.defaultConstructionSet());
    EXPECT_EQ(stDCS, spaceType.defaultConstructionSet().get());
    EXPECT_EQ(1u, m.getConcreteModelObjects<DefaultConstructionSet>().size());
  }

  // BuildingStory DCS only
  {
    Model m;
    Space space(m);
    BuildingStory story(m);
    EXPECT_TRUE(space.setBuildingStory(story));
    Construction adiabatic(m);
    auto bsDCS = makeDCS(m, {.adiabatic = adiabatic});
    EXPECT_TRUE(story.setDefaultConstructionSet(bsDCS));

    EXPECT_TRUE(DefaultConstructionSet::collapseToSpaceAndBuilding(m).empty());
    ASSERT_TRUE(space.defaultConstructionSet());
    EXPECT_EQ(bsDCS, space.defaultConstructionSet().get());
    ASSERT_TRUE(story.defaultConstructionSet());
    EXPECT_EQ(bsDCS, story.defaultConstructionSet().get());
    EXPECT_EQ(1u, m.getConcreteModelObjects<DefaultConstructionSet>().size());
  }

  // Several Spaces sharing a SpaceType with a DCS: they all get that same DCS
  {
    Model m;
    Space space1(m);
    Space space2(m);
    SpaceType spaceType(m);
    EXPECT_TRUE(space1.setSpaceType(spaceType));
    EXPECT_TRUE(space2.setSpaceType(spaceType));
    Construction adiabatic(m);
    auto stDCS = makeDCS(m, {.adiabatic = adiabatic});
    EXPECT_TRUE(spaceType.setDefaultConstructionSet(stDCS));

    EXPECT_TRUE(DefaultConstructionSet::collapseToSpaceAndBuilding(m).empty());
    ASSERT_TRUE(space1.defaultConstructionSet());
    EXPECT_EQ(stDCS, space1.defaultConstructionSet().get());
    ASSERT_TRUE(space2.defaultConstructionSet());
    EXPECT_EQ(stDCS, space2.defaultConstructionSet().get());
    EXPECT_EQ(1u, m.getConcreteModelObjects<DefaultConstructionSet>().size());
  }
}

TEST_F(ModelFixture, DefaultConstructionSet_collapseToSpaceAndBuilding_SpaceGroup_DefaultedSpaceTypeIgnored) {

  // A Space without an explicit SpaceType inherits the Building's SpaceType, whose DCS is at search distance 5: it belongs to
  // the Building group, never to the Space group

  // Space without a DCS: it stays without one, the Building picks up its SpaceType's DCS
  {
    Model m;
    auto building = m.getUniqueModelObject<Building>();
    SpaceType buildingSpaceType(m);
    EXPECT_TRUE(building.setSpaceType(buildingSpaceType));
    Construction adiabatic(m);
    auto bstDCS = makeDCS(m, {.adiabatic = adiabatic});
    EXPECT_TRUE(buildingSpaceType.setDefaultConstructionSet(bstDCS));

    Space space(m);
    EXPECT_TRUE(space.isSpaceTypeDefaulted());
    ASSERT_TRUE(space.spaceType());
    EXPECT_EQ(buildingSpaceType, space.spaceType().get());

    EXPECT_TRUE(DefaultConstructionSet::collapseToSpaceAndBuilding(m).empty());
    EXPECT_FALSE(space.defaultConstructionSet());
    ASSERT_TRUE(building.defaultConstructionSet());
    EXPECT_EQ(bstDCS, building.defaultConstructionSet().get());
    EXPECT_EQ(1u, m.getConcreteModelObjects<DefaultConstructionSet>().size());
  }

  // Space with its own DCS, and the Building's SpaceType DCS fills other slots: no merge into the Space's DCS, the Building
  // picks up its SpaceType's DCS
  {
    Model m;
    auto building = m.getUniqueModelObject<Building>();
    SpaceType buildingSpaceType(m);
    EXPECT_TRUE(building.setSpaceType(buildingSpaceType));
    Construction bstWall(m);
    auto bstDCS = makeDCS(m, {.extWall = bstWall});
    EXPECT_TRUE(buildingSpaceType.setDefaultConstructionSet(bstDCS));

    Space space(m);
    EXPECT_TRUE(space.isSpaceTypeDefaulted());
    Construction sAdiabatic(m);
    auto sDCS = makeDCS(m, {.adiabatic = sAdiabatic});
    EXPECT_TRUE(space.setDefaultConstructionSet(sDCS));

    EXPECT_TRUE(DefaultConstructionSet::collapseToSpaceAndBuilding(m).empty());
    ASSERT_TRUE(space.defaultConstructionSet());
    EXPECT_EQ(sDCS, space.defaultConstructionSet().get());
    EXPECT_FALSE(sDCS.defaultExteriorSurfaceConstructions());
    ASSERT_TRUE(building.defaultConstructionSet());
    EXPECT_EQ(bstDCS, building.defaultConstructionSet().get());
    EXPECT_EQ(2u, m.getConcreteModelObjects<DefaultConstructionSet>().size());
  }
}

TEST_F(ModelFixture, DefaultConstructionSet_collapseToSpaceAndBuilding_SpaceGroup_RedundantSetIgnored) {

  // A lower level DCS that only fills slots already filled higher up (with different constructions) is ignored: no clone

  // Space DCS and SpaceType DCS both fill the exterior wall: the Space keeps its own
  {
    Model m;
    Space space(m);
    SpaceType spaceType(m);
    EXPECT_TRUE(space.setSpaceType(spaceType));

    Construction sWall(m);
    auto sDCS = makeDCS(m, {.extWall = sWall});
    EXPECT_TRUE(space.setDefaultConstructionSet(sDCS));

    Construction stWall(m);
    auto stDCS = makeDCS(m, {.extWall = stWall});
    EXPECT_TRUE(spaceType.setDefaultConstructionSet(stDCS));

    EXPECT_TRUE(DefaultConstructionSet::collapseToSpaceAndBuilding(m).empty());
    ASSERT_TRUE(space.defaultConstructionSet());
    EXPECT_EQ(sDCS, space.defaultConstructionSet().get());
    ASSERT_TRUE(spaceType.defaultConstructionSet());
    EXPECT_EQ(stDCS, spaceType.defaultConstructionSet().get());
    EXPECT_EQ(2u, m.getConcreteModelObjects<DefaultConstructionSet>().size());
    EXPECT_EQ(2u, m.getConcreteModelObjects<DefaultSurfaceConstructions>().size());
  }

  // No Space DCS, SpaceType DCS and BuildingStory DCS both fill the exterior wall: the Space gets the SpaceType's
  {
    Model m;
    Space space(m);
    SpaceType spaceType(m);
    EXPECT_TRUE(space.setSpaceType(spaceType));
    BuildingStory story(m);
    EXPECT_TRUE(space.setBuildingStory(story));

    Construction stWall(m);
    auto stDCS = makeDCS(m, {.extWall = stWall});
    EXPECT_TRUE(spaceType.setDefaultConstructionSet(stDCS));

    Construction bsWall(m);
    auto bsDCS = makeDCS(m, {.extWall = bsWall});
    EXPECT_TRUE(story.setDefaultConstructionSet(bsDCS));

    EXPECT_TRUE(DefaultConstructionSet::collapseToSpaceAndBuilding(m).empty());
    ASSERT_TRUE(space.defaultConstructionSet());
    EXPECT_EQ(stDCS, space.defaultConstructionSet().get());
    ASSERT_TRUE(story.defaultConstructionSet());
    EXPECT_EQ(bsDCS, story.defaultConstructionSet().get());
    EXPECT_EQ(2u, m.getConcreteModelObjects<DefaultConstructionSet>().size());
  }

  // Space DCS fills the exterior wall and roof, the SpaceType DCS the wall, the BuildingStory DCS the roof: each lower set is
  // redundant on its own slots, the Space keeps its own
  {
    Model m;
    Space space(m);
    SpaceType spaceType(m);
    EXPECT_TRUE(space.setSpaceType(spaceType));
    BuildingStory story(m);
    EXPECT_TRUE(space.setBuildingStory(story));

    Construction sWall(m);
    Construction sRoof(m);
    auto sDCS = makeDCS(m, {.extWall = sWall, .extRoof = sRoof});
    EXPECT_TRUE(space.setDefaultConstructionSet(sDCS));

    Construction stWall(m);
    auto stDCS = makeDCS(m, {.extWall = stWall});
    EXPECT_TRUE(spaceType.setDefaultConstructionSet(stDCS));

    Construction bsRoof(m);
    auto bsDCS = makeDCS(m, {.extRoof = bsRoof});
    EXPECT_TRUE(story.setDefaultConstructionSet(bsDCS));

    EXPECT_TRUE(DefaultConstructionSet::collapseToSpaceAndBuilding(m).empty());
    ASSERT_TRUE(space.defaultConstructionSet());
    EXPECT_EQ(sDCS, space.defaultConstructionSet().get());
    EXPECT_EQ(3u, m.getConcreteModelObjects<DefaultConstructionSet>().size());
  }
}

TEST_F(ModelFixture, DefaultConstructionSet_collapseToSpaceAndBuilding_SpaceGroup_EmptySetIsReset) {

  // An empty DCS fills no slot, so it's ignored like any redundant set. Empty sets are not removed (no purge), just no longer
  // referenced by the Space

  // Space has an empty DCS, nothing below: the Space is reset
  {
    Model m;
    Space space(m);
    DefaultConstructionSet sDCS(m);
    EXPECT_TRUE(space.setDefaultConstructionSet(sDCS));

    EXPECT_TRUE(DefaultConstructionSet::collapseToSpaceAndBuilding(m).empty());
    EXPECT_FALSE(space.defaultConstructionSet());
    EXPECT_EQ(1u, m.getConcreteModelObjects<DefaultConstructionSet>().size());
  }

  // Space has an empty DCS, its SpaceType a non-empty one: the Space gets the SpaceType's
  {
    Model m;
    Space space(m);
    SpaceType spaceType(m);
    EXPECT_TRUE(space.setSpaceType(spaceType));
    DefaultConstructionSet sDCS(m);
    EXPECT_TRUE(space.setDefaultConstructionSet(sDCS));
    Construction adiabatic(m);
    auto stDCS = makeDCS(m, {.adiabatic = adiabatic});
    EXPECT_TRUE(spaceType.setDefaultConstructionSet(stDCS));

    EXPECT_TRUE(DefaultConstructionSet::collapseToSpaceAndBuilding(m).empty());
    ASSERT_TRUE(space.defaultConstructionSet());
    EXPECT_EQ(stDCS, space.defaultConstructionSet().get());
    EXPECT_EQ(2u, m.getConcreteModelObjects<DefaultConstructionSet>().size());
  }

  // SpaceType has an empty DCS, BuildingStory a non-empty one: the Space gets the BuildingStory's
  {
    Model m;
    Space space(m);
    SpaceType spaceType(m);
    EXPECT_TRUE(space.setSpaceType(spaceType));
    BuildingStory story(m);
    EXPECT_TRUE(space.setBuildingStory(story));
    DefaultConstructionSet stDCS(m);
    EXPECT_TRUE(spaceType.setDefaultConstructionSet(stDCS));
    Construction adiabatic(m);
    auto bsDCS = makeDCS(m, {.adiabatic = adiabatic});
    EXPECT_TRUE(story.setDefaultConstructionSet(bsDCS));

    EXPECT_TRUE(DefaultConstructionSet::collapseToSpaceAndBuilding(m).empty());
    ASSERT_TRUE(space.defaultConstructionSet());
    EXPECT_EQ(bsDCS, space.defaultConstructionSet().get());
    EXPECT_EQ(2u, m.getConcreteModelObjects<DefaultConstructionSet>().size());
  }

  // Space, SpaceType and BuildingStory all have an empty DCS: the Space stays without one
  {
    Model m;
    Space space(m);
    SpaceType spaceType(m);
    EXPECT_TRUE(space.setSpaceType(spaceType));
    BuildingStory story(m);
    EXPECT_TRUE(space.setBuildingStory(story));
    DefaultConstructionSet sDCS(m);
    EXPECT_TRUE(space.setDefaultConstructionSet(sDCS));
    DefaultConstructionSet stDCS(m);
    EXPECT_TRUE(spaceType.setDefaultConstructionSet(stDCS));
    DefaultConstructionSet bsDCS(m);
    EXPECT_TRUE(story.setDefaultConstructionSet(bsDCS));

    EXPECT_TRUE(DefaultConstructionSet::collapseToSpaceAndBuilding(m).empty());
    EXPECT_FALSE(space.defaultConstructionSet());
    EXPECT_EQ(3u, m.getConcreteModelObjects<DefaultConstructionSet>().size());
  }
}

// Space group, merged from a lower level (no Space DCS): the merge starts from the SpaceType DCS, which serves every Space of
// that SpaceType, so it's always cloned. One merged set per combination of owners, shared by every Space with that combination

TEST_F(ModelFixture, DefaultConstructionSet_collapseToSpaceAndBuilding_SpaceGroup_MergedShared_SameSpaceTypeAndStory) {

  // Several Spaces with the same SpaceType and BuildingStory: a single merged set, shared by all
  Model m;
  SpaceType spaceType(m);
  spaceType.setName("Office");
  BuildingStory story(m);
  story.setName("L1");

  Construction stWall(m);
  auto stDCS = makeDCS(m, {.extWall = stWall});
  EXPECT_TRUE(spaceType.setDefaultConstructionSet(stDCS));
  auto stExt = stDCS.defaultExteriorSurfaceConstructions().get();

  Construction bsWall(m);
  Construction bsRoof(m);
  auto bsDCS = makeDCS(m, {.extWall = bsWall, .extRoof = bsRoof});
  EXPECT_TRUE(story.setDefaultConstructionSet(bsDCS));
  auto bsExt = bsDCS.defaultExteriorSurfaceConstructions().get();

  Space space1(m);
  Space space2(m);
  EXPECT_TRUE(space1.setSpaceType(spaceType));
  EXPECT_TRUE(space1.setBuildingStory(story));
  EXPECT_TRUE(space2.setSpaceType(spaceType));
  EXPECT_TRUE(space2.setBuildingStory(story));

  auto created = DefaultConstructionSet::collapseToSpaceAndBuilding(m);
  ASSERT_EQ(1u, created.size());
  const auto& merged = created.front();
  EXPECT_EQ("SpaceType Office @ BuildingStory L1", merged.nameString());

  ASSERT_TRUE(space1.defaultConstructionSet());
  EXPECT_EQ(merged, space1.defaultConstructionSet().get());
  ASSERT_TRUE(space2.defaultConstructionSet());
  EXPECT_EQ(merged, space2.defaultConstructionSet().get());

  // Merged content: SpaceType's wall wins, roof comes from the BuildingStory
  ASSERT_TRUE(merged.defaultExteriorSurfaceConstructions());
  auto mergedExt = merged.defaultExteriorSurfaceConstructions().get();
  ASSERT_TRUE(mergedExt.wallConstruction());
  EXPECT_EQ(stWall, mergedExt.wallConstruction().get());
  ASSERT_TRUE(mergedExt.roofCeilingConstruction());
  EXPECT_EQ(bsRoof, mergedExt.roofCeilingConstruction().get());

  // Originals untouched, and still referenced by their owners (no purge)
  EXPECT_NE(stExt, mergedExt);
  EXPECT_NE(bsExt, mergedExt);
  EXPECT_FALSE(stExt.roofCeilingConstruction());
  ASSERT_TRUE(spaceType.defaultConstructionSet());
  EXPECT_EQ(stDCS, spaceType.defaultConstructionSet().get());
  ASSERT_TRUE(story.defaultConstructionSet());
  EXPECT_EQ(bsDCS, story.defaultConstructionSet().get());
  EXPECT_EQ(3u, m.getConcreteModelObjects<DefaultConstructionSet>().size());
}

TEST_F(ModelFixture, DefaultConstructionSet_collapseToSpaceAndBuilding_SpaceGroup_MergedShared_OnePerStory) {

  // Same SpaceType, different BuildingStories: one merged set per BuildingStory
  Model m;
  SpaceType spaceType(m);
  spaceType.setName("Office");
  Construction stWall(m);
  auto stDCS = makeDCS(m, {.extWall = stWall});
  EXPECT_TRUE(spaceType.setDefaultConstructionSet(stDCS));

  BuildingStory story1(m);
  story1.setName("L1");
  Construction bs1Roof(m);
  auto bs1DCS = makeDCS(m, {.extRoof = bs1Roof});
  EXPECT_TRUE(story1.setDefaultConstructionSet(bs1DCS));

  BuildingStory story2(m);
  story2.setName("L2");
  Construction bs2Roof(m);
  auto bs2DCS = makeDCS(m, {.extRoof = bs2Roof});
  EXPECT_TRUE(story2.setDefaultConstructionSet(bs2DCS));

  Space space1(m);
  EXPECT_TRUE(space1.setSpaceType(spaceType));
  EXPECT_TRUE(space1.setBuildingStory(story1));
  Space space2(m);
  EXPECT_TRUE(space2.setSpaceType(spaceType));
  EXPECT_TRUE(space2.setBuildingStory(story2));

  auto created = DefaultConstructionSet::collapseToSpaceAndBuilding(m);
  EXPECT_EQ(2u, created.size());

  ASSERT_TRUE(space1.defaultConstructionSet());
  auto merged1 = space1.defaultConstructionSet().get();
  EXPECT_EQ("SpaceType Office @ BuildingStory L1", merged1.nameString());
  ASSERT_TRUE(merged1.defaultExteriorSurfaceConstructions());
  ASSERT_TRUE(merged1.defaultExteriorSurfaceConstructions()->roofCeilingConstruction());
  EXPECT_EQ(bs1Roof, merged1.defaultExteriorSurfaceConstructions()->roofCeilingConstruction().get());

  ASSERT_TRUE(space2.defaultConstructionSet());
  auto merged2 = space2.defaultConstructionSet().get();
  EXPECT_EQ("SpaceType Office @ BuildingStory L2", merged2.nameString());
  ASSERT_TRUE(merged2.defaultExteriorSurfaceConstructions());
  ASSERT_TRUE(merged2.defaultExteriorSurfaceConstructions()->roofCeilingConstruction());
  EXPECT_EQ(bs2Roof, merged2.defaultExteriorSurfaceConstructions()->roofCeilingConstruction().get());

  EXPECT_NE(merged1, merged2);
  EXPECT_EQ(5u, m.getConcreteModelObjects<DefaultConstructionSet>().size());
}

TEST_F(ModelFixture, DefaultConstructionSet_collapseToSpaceAndBuilding_SpaceGroup_MergedShared_KeyAfterPruning) {

  // The key is built after pruning: a Space with an empty DCS of its own shares the merged set of a Space without one
  Model m;
  SpaceType spaceType(m);
  spaceType.setName("Office");
  Construction stWall(m);
  auto stDCS = makeDCS(m, {.extWall = stWall});
  EXPECT_TRUE(spaceType.setDefaultConstructionSet(stDCS));

  BuildingStory story(m);
  story.setName("L1");
  Construction bsRoof(m);
  auto bsDCS = makeDCS(m, {.extRoof = bsRoof});
  EXPECT_TRUE(story.setDefaultConstructionSet(bsDCS));

  Space space1(m);
  Space space2(m);
  EXPECT_TRUE(space1.setSpaceType(spaceType));
  EXPECT_TRUE(space1.setBuildingStory(story));
  EXPECT_TRUE(space2.setSpaceType(spaceType));
  EXPECT_TRUE(space2.setBuildingStory(story));
  DefaultConstructionSet emptyDCS(m);
  EXPECT_TRUE(space1.setDefaultConstructionSet(emptyDCS));

  auto created = DefaultConstructionSet::collapseToSpaceAndBuilding(m);
  ASSERT_EQ(1u, created.size());
  const auto& merged = created.front();
  EXPECT_EQ("SpaceType Office @ BuildingStory L1", merged.nameString());
  ASSERT_TRUE(space1.defaultConstructionSet());
  EXPECT_EQ(merged, space1.defaultConstructionSet().get());
  ASSERT_TRUE(space2.defaultConstructionSet());
  EXPECT_EQ(merged, space2.defaultConstructionSet().get());
}

// Space group, pruning is by slot: a lower level DCS that fills the same surface type in another category, or a slot of
// another kind, is not redundant and gets merged in. Each test has the SpaceType (2) and the BuildingStory (3) fill
// different slots, with no Space DCS, so the merge always starts from the SpaceType DCS

namespace {

// A Space with an explicit SpaceType "Office" and a BuildingStory "L1", each with the given DCS
Space makeSpaceWithSpaceTypeAndStory(Model& m, const DefaultConstructionSet& stDCS, const DefaultConstructionSet& bsDCS) {
  SpaceType spaceType(m);
  spaceType.setName("Office");
  EXPECT_TRUE(spaceType.setDefaultConstructionSet(stDCS));

  BuildingStory story(m);
  story.setName("L1");
  EXPECT_TRUE(story.setDefaultConstructionSet(bsDCS));

  Space space(m);
  EXPECT_TRUE(space.setSpaceType(spaceType));
  EXPECT_TRUE(space.setBuildingStory(story));
  return space;
}

}  // namespace

TEST_F(ModelFixture, DefaultConstructionSet_collapseToSpaceAndBuilding_SpaceGroup_NotPruned_SameTypeOtherCategory) {

  // SpaceType fills the exterior wall, BuildingStory the interior wall
  Model m;
  Construction stExtWall(m);
  Construction bsIntWall(m);
  auto space = makeSpaceWithSpaceTypeAndStory(m, makeDCS(m, {.extWall = stExtWall}), makeDCS(m, {.intWall = bsIntWall}));

  auto created = DefaultConstructionSet::collapseToSpaceAndBuilding(m);
  ASSERT_EQ(1u, created.size());
  const auto& merged = created.front();
  EXPECT_EQ("SpaceType Office @ BuildingStory L1", merged.nameString());
  ASSERT_TRUE(space.defaultConstructionSet());
  EXPECT_EQ(merged, space.defaultConstructionSet().get());

  ASSERT_TRUE(merged.defaultExteriorSurfaceConstructions());
  ASSERT_TRUE(merged.defaultExteriorSurfaceConstructions()->wallConstruction());
  EXPECT_EQ(stExtWall, merged.defaultExteriorSurfaceConstructions()->wallConstruction().get());
  ASSERT_TRUE(merged.defaultInteriorSurfaceConstructions());
  ASSERT_TRUE(merged.defaultInteriorSurfaceConstructions()->wallConstruction());
  EXPECT_EQ(bsIntWall, merged.defaultInteriorSurfaceConstructions()->wallConstruction().get());
}

TEST_F(ModelFixture, DefaultConstructionSet_collapseToSpaceAndBuilding_SpaceGroup_NotPruned_GroundVsExteriorFloor) {

  // SpaceType fills the exterior floor, BuildingStory the ground contact floor
  Model m;
  Construction stExtFloor(m);
  Construction bsGroundFloor(m);
  auto space = makeSpaceWithSpaceTypeAndStory(m, makeDCS(m, {.extFloor = stExtFloor}), makeDCS(m, {.groundFloor = bsGroundFloor}));

  auto created = DefaultConstructionSet::collapseToSpaceAndBuilding(m);
  ASSERT_EQ(1u, created.size());
  const auto& merged = created.front();
  ASSERT_TRUE(space.defaultConstructionSet());
  EXPECT_EQ(merged, space.defaultConstructionSet().get());

  ASSERT_TRUE(merged.defaultExteriorSurfaceConstructions());
  ASSERT_TRUE(merged.defaultExteriorSurfaceConstructions()->floorConstruction());
  EXPECT_EQ(stExtFloor, merged.defaultExteriorSurfaceConstructions()->floorConstruction().get());
  ASSERT_TRUE(merged.defaultGroundContactSurfaceConstructions());
  ASSERT_TRUE(merged.defaultGroundContactSurfaceConstructions()->floorConstruction());
  EXPECT_EQ(bsGroundFloor, merged.defaultGroundContactSurfaceConstructions()->floorConstruction().get());
}

TEST_F(ModelFixture, DefaultConstructionSet_collapseToSpaceAndBuilding_SpaceGroup_NotPruned_SubSurfaceSlot) {

  // SpaceType fills the exterior wall, BuildingStory only the exterior fixed window
  Model m;
  Construction stExtWall(m);
  Construction bsFixedWindow(m);
  auto space = makeSpaceWithSpaceTypeAndStory(m, makeDCS(m, {.extWall = stExtWall}), makeDCS(m, {.extFixedWindow = bsFixedWindow}));

  auto created = DefaultConstructionSet::collapseToSpaceAndBuilding(m);
  ASSERT_EQ(1u, created.size());
  const auto& merged = created.front();
  ASSERT_TRUE(space.defaultConstructionSet());
  EXPECT_EQ(merged, space.defaultConstructionSet().get());

  ASSERT_TRUE(merged.defaultExteriorSurfaceConstructions());
  ASSERT_TRUE(merged.defaultExteriorSurfaceConstructions()->wallConstruction());
  EXPECT_EQ(stExtWall, merged.defaultExteriorSurfaceConstructions()->wallConstruction().get());
  ASSERT_TRUE(merged.defaultExteriorSubSurfaceConstructions());
  ASSERT_TRUE(merged.defaultExteriorSubSurfaceConstructions()->fixedWindowConstruction());
  EXPECT_EQ(bsFixedWindow, merged.defaultExteriorSubSurfaceConstructions()->fixedWindowConstruction().get());
}

TEST_F(ModelFixture, DefaultConstructionSet_collapseToSpaceAndBuilding_SpaceGroup_NotPruned_InteriorPartitionSlot) {

  // SpaceType fills the exterior wall, BuildingStory only the interior partition
  Model m;
  Construction stExtWall(m);
  Construction bsPartition(m);
  auto space = makeSpaceWithSpaceTypeAndStory(m, makeDCS(m, {.extWall = stExtWall}), makeDCS(m, {.interiorPartition = bsPartition}));

  auto created = DefaultConstructionSet::collapseToSpaceAndBuilding(m);
  ASSERT_EQ(1u, created.size());
  const auto& merged = created.front();
  ASSERT_TRUE(space.defaultConstructionSet());
  EXPECT_EQ(merged, space.defaultConstructionSet().get());

  ASSERT_TRUE(merged.defaultExteriorSurfaceConstructions());
  ASSERT_TRUE(merged.defaultExteriorSurfaceConstructions()->wallConstruction());
  EXPECT_EQ(stExtWall, merged.defaultExteriorSurfaceConstructions()->wallConstruction().get());
  ASSERT_TRUE(merged.interiorPartitionConstruction());
  EXPECT_EQ(bsPartition, merged.interiorPartitionConstruction().get());
}

// Space group, merged from the Space DCS: these only check what the Space resolves to, slot by slot. Whether the Space DCS is
// merged into in place or cloned first is tested separately. The lower level DCSs are never modified either way

TEST_F(ModelFixture, DefaultConstructionSet_collapseToSpaceAndBuilding_SpaceGroup_MergedContent_SpaceAndSpaceType) {

  // Space fills the exterior wall, SpaceType the exterior wall and roof: Space's wall wins, roof comes from the SpaceType
  Model m;
  Space space(m);
  SpaceType spaceType(m);
  EXPECT_TRUE(space.setSpaceType(spaceType));

  Construction sWall(m);
  EXPECT_TRUE(space.setDefaultConstructionSet(makeDCS(m, {.extWall = sWall})));
  Construction stWall(m);
  Construction stRoof(m);
  auto stDCS = makeDCS(m, {.extWall = stWall, .extRoof = stRoof});
  EXPECT_TRUE(spaceType.setDefaultConstructionSet(stDCS));

  DefaultConstructionSet::collapseToSpaceAndBuilding(m);

  ASSERT_TRUE(space.defaultConstructionSet());
  auto result = space.defaultConstructionSet().get();
  ASSERT_TRUE(extSlot(result, &DefaultSurfaceConstructions::wallConstruction));
  EXPECT_EQ(sWall, extSlot(result, &DefaultSurfaceConstructions::wallConstruction).get());
  ASSERT_TRUE(extSlot(result, &DefaultSurfaceConstructions::roofCeilingConstruction));
  EXPECT_EQ(stRoof, extSlot(result, &DefaultSurfaceConstructions::roofCeilingConstruction).get());

  // SpaceType DCS untouched
  ASSERT_TRUE(extSlot(stDCS, &DefaultSurfaceConstructions::wallConstruction));
  EXPECT_EQ(stWall, extSlot(stDCS, &DefaultSurfaceConstructions::wallConstruction).get());
  ASSERT_TRUE(extSlot(stDCS, &DefaultSurfaceConstructions::roofCeilingConstruction));
  EXPECT_EQ(stRoof, extSlot(stDCS, &DefaultSurfaceConstructions::roofCeilingConstruction).get());
}

TEST_F(ModelFixture, DefaultConstructionSet_collapseToSpaceAndBuilding_SpaceGroup_MergedContent_SpaceAndStory) {

  // Space fills the exterior wall, BuildingStory the exterior wall and roof: Space's wall wins, roof comes from the BuildingStory
  Model m;
  Space space(m);
  BuildingStory story(m);
  EXPECT_TRUE(space.setBuildingStory(story));

  Construction sWall(m);
  EXPECT_TRUE(space.setDefaultConstructionSet(makeDCS(m, {.extWall = sWall})));
  Construction bsWall(m);
  Construction bsRoof(m);
  auto bsDCS = makeDCS(m, {.extWall = bsWall, .extRoof = bsRoof});
  EXPECT_TRUE(story.setDefaultConstructionSet(bsDCS));

  DefaultConstructionSet::collapseToSpaceAndBuilding(m);

  ASSERT_TRUE(space.defaultConstructionSet());
  auto result = space.defaultConstructionSet().get();
  ASSERT_TRUE(extSlot(result, &DefaultSurfaceConstructions::wallConstruction));
  EXPECT_EQ(sWall, extSlot(result, &DefaultSurfaceConstructions::wallConstruction).get());
  ASSERT_TRUE(extSlot(result, &DefaultSurfaceConstructions::roofCeilingConstruction));
  EXPECT_EQ(bsRoof, extSlot(result, &DefaultSurfaceConstructions::roofCeilingConstruction).get());

  // BuildingStory DCS untouched
  ASSERT_TRUE(extSlot(bsDCS, &DefaultSurfaceConstructions::wallConstruction));
  EXPECT_EQ(bsWall, extSlot(bsDCS, &DefaultSurfaceConstructions::wallConstruction).get());
  ASSERT_TRUE(extSlot(bsDCS, &DefaultSurfaceConstructions::roofCeilingConstruction));
  EXPECT_EQ(bsRoof, extSlot(bsDCS, &DefaultSurfaceConstructions::roofCeilingConstruction).get());
}

TEST_F(ModelFixture, DefaultConstructionSet_collapseToSpaceAndBuilding_SpaceGroup_MergedContent_AllThree) {

  // Priority Space > SpaceType > BuildingStory, slot by slot:
  // | Slot           | Space | SpaceType | BuildingStory | Result  |
  // |----------------|-------|-----------|---------------|---------|
  // | exterior wall  | sWall | stWall    | bsWall        | sWall   |
  // | exterior roof  |       | stRoof    | bsRoof        | stRoof  |
  // | exterior floor |       |           | bsFloor       | bsFloor |
  Model m;
  Space space(m);
  SpaceType spaceType(m);
  EXPECT_TRUE(space.setSpaceType(spaceType));
  BuildingStory story(m);
  EXPECT_TRUE(space.setBuildingStory(story));

  Construction sWall(m);
  EXPECT_TRUE(space.setDefaultConstructionSet(makeDCS(m, {.extWall = sWall})));
  Construction stWall(m);
  Construction stRoof(m);
  auto stDCS = makeDCS(m, {.extWall = stWall, .extRoof = stRoof});
  EXPECT_TRUE(spaceType.setDefaultConstructionSet(stDCS));
  Construction bsFloor(m);
  Construction bsWall(m);
  Construction bsRoof(m);
  auto bsDCS = makeDCS(m, {.extFloor = bsFloor, .extWall = bsWall, .extRoof = bsRoof});
  EXPECT_TRUE(story.setDefaultConstructionSet(bsDCS));

  DefaultConstructionSet::collapseToSpaceAndBuilding(m);

  ASSERT_TRUE(space.defaultConstructionSet());
  auto result = space.defaultConstructionSet().get();
  ASSERT_TRUE(extSlot(result, &DefaultSurfaceConstructions::wallConstruction));
  EXPECT_EQ(sWall, extSlot(result, &DefaultSurfaceConstructions::wallConstruction).get());
  ASSERT_TRUE(extSlot(result, &DefaultSurfaceConstructions::roofCeilingConstruction));
  EXPECT_EQ(stRoof, extSlot(result, &DefaultSurfaceConstructions::roofCeilingConstruction).get());
  ASSERT_TRUE(extSlot(result, &DefaultSurfaceConstructions::floorConstruction));
  EXPECT_EQ(bsFloor, extSlot(result, &DefaultSurfaceConstructions::floorConstruction).get());

  // Lower level DCSs untouched
  EXPECT_EQ(stWall, extSlot(stDCS, &DefaultSurfaceConstructions::wallConstruction).get());
  EXPECT_EQ(stRoof, extSlot(stDCS, &DefaultSurfaceConstructions::roofCeilingConstruction).get());
  EXPECT_FALSE(extSlot(stDCS, &DefaultSurfaceConstructions::floorConstruction));
  EXPECT_EQ(bsFloor, extSlot(bsDCS, &DefaultSurfaceConstructions::floorConstruction).get());
  EXPECT_EQ(bsWall, extSlot(bsDCS, &DefaultSurfaceConstructions::wallConstruction).get());
  EXPECT_EQ(bsRoof, extSlot(bsDCS, &DefaultSurfaceConstructions::roofCeilingConstruction).get());
}

TEST_F(ModelFixture, DefaultConstructionSet_collapseToSpaceAndBuilding_SpaceGroup_MergedContent_RedundantSpaceTypeSkipped) {

  // Space fills the exterior wall, SpaceType only the exterior wall too (redundant), BuildingStory the exterior roof: the
  // SpaceType is left out of the merge, so it doesn't show up in the name of the resulting set either (whether that's the
  // Space's own set, or a merged clone named after its owners)
  Model m;
  Space space(m);
  space.setName("A");
  SpaceType spaceType(m);
  spaceType.setName("Office");
  EXPECT_TRUE(space.setSpaceType(spaceType));
  BuildingStory story(m);
  story.setName("L1");
  EXPECT_TRUE(space.setBuildingStory(story));

  Construction sWall(m);
  auto sDCS = makeDCS(m, {.extWall = sWall});
  sDCS.setName("Space DCS");
  EXPECT_TRUE(space.setDefaultConstructionSet(sDCS));
  Construction stWall(m);
  EXPECT_TRUE(spaceType.setDefaultConstructionSet(makeDCS(m, {.extWall = stWall})));
  Construction bsRoof(m);
  EXPECT_TRUE(story.setDefaultConstructionSet(makeDCS(m, {.extRoof = bsRoof})));

  DefaultConstructionSet::collapseToSpaceAndBuilding(m);

  ASSERT_TRUE(space.defaultConstructionSet());
  auto result = space.defaultConstructionSet().get();
  ASSERT_TRUE(extSlot(result, &DefaultSurfaceConstructions::wallConstruction));
  EXPECT_EQ(sWall, extSlot(result, &DefaultSurfaceConstructions::wallConstruction).get());
  ASSERT_TRUE(extSlot(result, &DefaultSurfaceConstructions::roofCeilingConstruction));
  EXPECT_EQ(bsRoof, extSlot(result, &DefaultSurfaceConstructions::roofCeilingConstruction).get());
  EXPECT_EQ(std::string::npos, result.nameString().find("Office")) << result.nameString();
}

TEST_F(ModelFixture, DefaultConstructionSet_collapseToSpaceAndBuilding_SpaceGroup_BuildingGroupUntouched) {

  // The Space's own DCS is also the Building's, and its SpaceType contributes a slot: the Space resolves the merged content, but
  // the Building's DCS must not pick up the SpaceType's slot, or Spaces without a DCS of their own would resolve it
  Model m;
  auto building = m.getUniqueModelObject<Building>();
  Space space(m);
  SpaceType spaceType(m);
  EXPECT_TRUE(space.setSpaceType(spaceType));

  Construction wall(m);
  auto sharedDCS = makeDCS(m, {.extWall = wall});
  EXPECT_TRUE(space.setDefaultConstructionSet(sharedDCS));
  EXPECT_TRUE(building.setDefaultConstructionSet(sharedDCS));

  Construction stRoof(m);
  EXPECT_TRUE(spaceType.setDefaultConstructionSet(makeDCS(m, {.extRoof = stRoof})));

  // A dumb other Space without a DCS, explicit SpaceType or BuildingStory: it falls through to the Building's DCS, so that DCS
  // is used by more than the Space, even if the Space is the only one pointing at it directly
  Space otherSpace(m);

  DefaultConstructionSet::collapseToSpaceAndBuilding(m);

  // The Space resolves the merged content
  ASSERT_TRUE(space.defaultConstructionSet());
  auto result = space.defaultConstructionSet().get();
  ASSERT_TRUE(extSlot(result, &DefaultSurfaceConstructions::wallConstruction));
  EXPECT_EQ(wall, extSlot(result, &DefaultSurfaceConstructions::wallConstruction).get());
  ASSERT_TRUE(extSlot(result, &DefaultSurfaceConstructions::roofCeilingConstruction));
  EXPECT_EQ(stRoof, extSlot(result, &DefaultSurfaceConstructions::roofCeilingConstruction).get());

  // The Building group has a single set, so the Building keeps it, and its content is unchanged: no roof
  ASSERT_TRUE(building.defaultConstructionSet());
  EXPECT_EQ(sharedDCS, building.defaultConstructionSet().get());
  ASSERT_TRUE(extSlot(sharedDCS, &DefaultSurfaceConstructions::wallConstruction));
  EXPECT_EQ(wall, extSlot(sharedDCS, &DefaultSurfaceConstructions::wallConstruction).get());
  EXPECT_FALSE(extSlot(sharedDCS, &DefaultSurfaceConstructions::roofCeilingConstruction));

  // The other Space has nothing in its Space group, so it stays without a DCS and keeps falling through to the Building's
  EXPECT_FALSE(otherSpace.defaultConstructionSet());
}

// Space group, merged from the Space DCS: merged into in place when that's safe, cloned otherwise

TEST_F(ModelFixture, DefaultConstructionSet_collapseToSpaceAndBuilding_SpaceGroup_MergedInPlace_SoleUser) {

  // The Space DCS is used by this Space only, and its SpaceType contributes a slot: the SpaceType's slot is merged into the
  // Space DCS itself, nothing is created
  Model m;
  Space space(m);
  SpaceType spaceType(m);
  EXPECT_TRUE(space.setSpaceType(spaceType));

  Construction sWall(m);
  auto sDCS = makeDCS(m, {.extWall = sWall});
  sDCS.setName("Space DCS");
  EXPECT_TRUE(space.setDefaultConstructionSet(sDCS));

  Construction stRoof(m);
  EXPECT_TRUE(spaceType.setDefaultConstructionSet(makeDCS(m, {.extRoof = stRoof})));

  EXPECT_TRUE(DefaultConstructionSet::collapseToSpaceAndBuilding(m).empty());

  ASSERT_TRUE(space.defaultConstructionSet());
  EXPECT_EQ(sDCS, space.defaultConstructionSet().get());
  EXPECT_EQ("Space DCS", sDCS.nameString());
  ASSERT_TRUE(extSlot(sDCS, &DefaultSurfaceConstructions::wallConstruction));
  EXPECT_EQ(sWall, extSlot(sDCS, &DefaultSurfaceConstructions::wallConstruction).get());
  ASSERT_TRUE(extSlot(sDCS, &DefaultSurfaceConstructions::roofCeilingConstruction));
  EXPECT_EQ(stRoof, extSlot(sDCS, &DefaultSurfaceConstructions::roofCeilingConstruction).get());
  EXPECT_EQ(2u, m.getConcreteModelObjects<DefaultConstructionSet>().size());
}

// Space group, purge: SpaceType and BuildingStory sets are reset, then the sets that this left unused are removed, along with
// their Default(Sub)SurfaceConstructions if nothing else uses them anymore

TEST_F(ModelFixture, DefaultConstructionSet_collapseToSpaceAndBuilding_SpaceGroup_Purge_MovedDownSetKept) {

  // The SpaceType DCS is moved down to the Space: the SpaceType is reset, but its DCS is still used by the Space so it stays
  Model m;
  Space space(m);
  SpaceType spaceType(m);
  EXPECT_TRUE(space.setSpaceType(spaceType));
  Construction stWall(m);
  auto stDCS = makeDCS(m, {.extWall = stWall});
  EXPECT_TRUE(spaceType.setDefaultConstructionSet(stDCS));
  auto stExt = stDCS.defaultExteriorSurfaceConstructions().get();

  EXPECT_TRUE(DefaultConstructionSet::collapseToSpaceAndBuilding(m, true).empty());

  EXPECT_FALSE(spaceType.defaultConstructionSet());
  ASSERT_TRUE(space.defaultConstructionSet());
  EXPECT_EQ(stDCS, space.defaultConstructionSet().get());
  EXPECT_FALSE(stDCS.handle().isNull());
  EXPECT_FALSE(stExt.handle().isNull());
  EXPECT_EQ(1u, m.getConcreteModelObjects<DefaultConstructionSet>().size());
  EXPECT_EQ(1u, m.getConcreteModelObjects<DefaultSurfaceConstructions>().size());
}

TEST_F(ModelFixture, DefaultConstructionSet_collapseToSpaceAndBuilding_SpaceGroup_Purge_MergedOriginalsRemoved) {

  // SpaceType fills the exterior wall, BuildingStory the exterior roof and the interior wall: they get merged, then both
  // originals are removed. Their sub-objects go too, except the ones the merged set still uses:
  // - The exterior ones were both filled, so the merged set got its own (a clone of the SpaceType's): both originals are removed
  // - The interior one was only filled by the BuildingStory, so the merged set points to it as is: it stays
  Model m;
  Construction stWall(m);
  Construction bsRoof(m);
  Construction bsIntWall(m);
  auto stDCS = makeDCS(m, {.extWall = stWall});
  auto bsDCS = makeDCS(m, {.extRoof = bsRoof, .intWall = bsIntWall});
  auto stExt = stDCS.defaultExteriorSurfaceConstructions().get();
  auto bsExt = bsDCS.defaultExteriorSurfaceConstructions().get();
  auto bsInt = bsDCS.defaultInteriorSurfaceConstructions().get();
  auto space = makeSpaceWithSpaceTypeAndStory(m, stDCS, bsDCS);
  auto spaceType = space.spaceType().get();
  auto story = space.buildingStory().get();

  auto created = DefaultConstructionSet::collapseToSpaceAndBuilding(m, true);
  ASSERT_EQ(1u, created.size());
  const auto& merged = created.front();
  ASSERT_TRUE(space.defaultConstructionSet());
  EXPECT_EQ(merged, space.defaultConstructionSet().get());

  EXPECT_FALSE(spaceType.defaultConstructionSet());
  EXPECT_FALSE(story.defaultConstructionSet());
  EXPECT_TRUE(stDCS.handle().isNull());
  EXPECT_TRUE(bsDCS.handle().isNull());
  EXPECT_TRUE(stExt.handle().isNull());
  EXPECT_TRUE(bsExt.handle().isNull());
  EXPECT_FALSE(bsInt.handle().isNull());

  // The merged set is intact
  ASSERT_TRUE(merged.defaultExteriorSurfaceConstructions());
  auto mergedExt = merged.defaultExteriorSurfaceConstructions().get();
  ASSERT_TRUE(mergedExt.wallConstruction());
  EXPECT_EQ(stWall, mergedExt.wallConstruction().get());
  ASSERT_TRUE(mergedExt.roofCeilingConstruction());
  EXPECT_EQ(bsRoof, mergedExt.roofCeilingConstruction().get());
  ASSERT_TRUE(merged.defaultInteriorSurfaceConstructions());
  EXPECT_EQ(bsInt, merged.defaultInteriorSurfaceConstructions().get());

  EXPECT_EQ(1u, m.getConcreteModelObjects<DefaultConstructionSet>().size());       // merged
  EXPECT_EQ(2u, m.getConcreteModelObjects<DefaultSurfaceConstructions>().size());  // mergedExt, bsInt
}

namespace {

// Four 10x10x3 Spaces in a row, every level of the inheritance chain winning at least one surface:
//
//            y (=North)
//   ▲
//   │                  building height = 3m
// 10├────────┼────────┼────────┼────────┤
//   │        ║        │        │        │
//   │   A    ║   B    │   C    │   D    │
//   │        ║        │        │        │
//   └────────┴────────┴────────┴────────┴──► x
//  0        10       20        30       40
//
// Only A and B are matched (║), and both resolve their interior wall from the same BuildingStory, so the matched pair resolution
// in Surface::construction doesn't come into play
//
// | Level                  | Owner of               | Fills                                                 |
// |------------------------|------------------------|-------------------------------------------------------|
// | 1 Space                | A, D (shared)          | ext wall                                              |
// | 2 SpaceType Office     | A, B (explicit)        | ext wall (hidden behind A's), ext fixed window        |
// | 3 BuildingStory L1     | A, B                   | ext roof, int wall                                    |
// | 4 Building             |                        | ground floor, interior partition                      |
// | 5 Building SpaceType   | C, D (defaulted)       | ext wall, ext roof, space shading, adiabatic          |
//
// Plus: a fixed window on an exterior wall of A and of B, an interior partition in A, a space shading surface on B, and one
// adiabatic wall in C
Model makeEquivalenceModel() {
  Model m;

  constexpr double width = 10.0;
  constexpr double height = 3.0;

  // Counterclockwise points
  std::vector<Point3d> floorPoints{{0.0, 0.0, 0.0}, {0.0, width, 0.0}, {width, width, 0.0}, {width, 0.0, 0.0}};

  auto spaceA = Space::fromFloorPrint(floorPoints, height, m).get();
  spaceA.setName("A");
  auto spaceB = Space::fromFloorPrint(floorPoints, height, m).get();
  spaceB.setName("B");
  spaceB.setXOrigin(width);
  auto spaceC = Space::fromFloorPrint(floorPoints, height, m).get();
  spaceC.setName("C");
  spaceC.setXOrigin(width * 2);
  auto spaceD = Space::fromFloorPrint(floorPoints, height, m).get();
  spaceD.setName("D");
  spaceD.setXOrigin(width * 3);

  spaceA.matchSurfaces(spaceB);

  // Exterior wall facing South (outward normal -y), on which we put a window or which we make adiabatic
  auto southWall = [](const Space& space) {
    boost::optional<Surface> result;
    for (const auto& surface : space.surfaces()) {
      if (istringEqual("Wall", surface.surfaceType()) && istringEqual("Outdoors", surface.outsideBoundaryCondition())
          && surface.outwardNormal().y() < -0.5) {
        result = surface;
      }
    }
    EXPECT_TRUE(result) << space.nameString() << " has no exterior wall facing South";
    return result.value();
  };

  EXPECT_TRUE(southWall(spaceA).setWindowToWallRatio(0.4));
  EXPECT_TRUE(southWall(spaceB).setWindowToWallRatio(0.4));
  EXPECT_TRUE(southWall(spaceC).setOutsideBoundaryCondition("Adiabatic"));

  InteriorPartitionSurfaceGroup partitionGroup(m);
  EXPECT_TRUE(partitionGroup.setSpace(spaceA));
  InteriorPartitionSurface partition({{5.0, 2.0, height}, {5.0, 2.0, 0.0}, {5.0, 8.0, 0.0}, {5.0, 8.0, height}}, m);
  EXPECT_TRUE(partition.setInteriorPartitionSurfaceGroup(partitionGroup));

  ShadingSurfaceGroup shadingGroup(m);
  EXPECT_TRUE(shadingGroup.setSpace(spaceB));
  ShadingSurface overhang({{0.0, 0.0, height}, {0.0, -1.0, height}, {width, -1.0, height}, {width, 0.0, height}}, m);
  EXPECT_TRUE(overhang.setShadingSurfaceGroup(shadingGroup));

  // Level 1: Space
  Construction sWall(m);
  auto sDCS = makeDCS(m, {.extWall = sWall});
  EXPECT_TRUE(spaceA.setDefaultConstructionSet(sDCS));
  EXPECT_TRUE(spaceD.setDefaultConstructionSet(sDCS));

  // Level 2: SpaceType
  SpaceType office(m);
  office.setName("Office");
  Construction stWall(m);
  Construction stWindow(m);
  EXPECT_TRUE(office.setDefaultConstructionSet(makeDCS(m, {.extWall = stWall, .extFixedWindow = stWindow})));
  EXPECT_TRUE(spaceA.setSpaceType(office));
  EXPECT_TRUE(spaceB.setSpaceType(office));

  // Level 3: BuildingStory
  BuildingStory story(m);
  story.setName("L1");
  Construction bsRoof(m);
  Construction bsIntWall(m);
  EXPECT_TRUE(story.setDefaultConstructionSet(makeDCS(m, {.extRoof = bsRoof, .intWall = bsIntWall})));
  EXPECT_TRUE(spaceA.setBuildingStory(story));
  EXPECT_TRUE(spaceB.setBuildingStory(story));

  // Level 4: Building
  auto building = m.getUniqueModelObject<Building>();
  Construction bGroundFloor(m);
  Construction bPartition(m);
  EXPECT_TRUE(building.setDefaultConstructionSet(makeDCS(m, {.groundFloor = bGroundFloor, .interiorPartition = bPartition})));

  // Level 5: Building SpaceType, inherited by C and D whose SpaceType is defaulted
  SpaceType buildingSpaceType(m);
  buildingSpaceType.setName("Building SpaceType");
  Construction bstWall(m);
  Construction bstRoof(m);
  Construction bstShading(m);
  Construction bstAdiabatic(m);
  EXPECT_TRUE(buildingSpaceType.setDefaultConstructionSet(
    makeDCS(m, {.extWall = bstWall, .extRoof = bstRoof, .spaceShading = bstShading, .adiabatic = bstAdiabatic})));
  EXPECT_TRUE(building.setSpaceType(buildingSpaceType));
  EXPECT_TRUE(spaceC.isSpaceTypeDefaulted());
  EXPECT_TRUE(spaceD.isSpaceTypeDefaulted());

  return m;
}

// PlanarSurface handle -> its resolved construction (null Handle if none) and search distance (-1 if none)
struct Resolved
{
  Handle construction;
  int searchDistance = -1;
};

std::map<Handle, Resolved> resolveAll(const Model& m) {
  std::map<Handle, Resolved> result;
  for (const auto& planarSurface : m.getModelObjects<PlanarSurface>()) {
    Resolved r;
    // Not redundant: Surface and SubSurface override `construction()` to resolve matched pairs (the side with the lower search
    // distance wins, so it can return the adjacent surface's construction). `construction()` is what OS and the FT use.
    // `constructionWithSearchDistance()` is the surface's own lookup, and tells which level it came from
    if (auto c_ = planarSurface.construction()) {
      r.construction = c_->handle();
    }
    if (auto cwsd_ = planarSurface.constructionWithSearchDistance()) {
      r.searchDistance = cwsd_->second;
    }
    result.emplace(planarSurface.handle(), r);
  }
  return result;
}

}  // namespace

TEST_F(ModelFixture, DefaultConstructionSet_collapseToSpaceAndBuilding_Equivalence) {

  // The contract: collapsing never changes what any surface resolves to, and afterwards every defaulted construction comes from
  // the Space (1) or the Building (4), the only two levels E+ has. Same with or without purge

  for (const bool purge : {false, true}) {
    SCOPED_TRACE(fmt::format("purge = {}", purge));

    Model m = makeEquivalenceModel();
    const auto before = resolveAll(m);

    // Sanity checks so the test can't pass vacuously: every surface resolves, and every level wins at least one
    std::set<int> distancesBefore;
    for (const auto& [handle, r] : before) {
      EXPECT_FALSE(r.construction.isNull()) << m.getModelObject<ModelObject>(handle)->briefDescription() << " has no construction";
      distancesBefore.insert(r.searchDistance);
    }
    EXPECT_EQ((std::set<int>{1, 2, 3, 4, 5}), distancesBefore);

    DefaultConstructionSet::collapseToSpaceAndBuilding(m, purge);

    const auto after = resolveAll(m);
    ASSERT_EQ(before.size(), after.size());
    for (const auto& [handle, rBefore] : before) {
      auto it = after.find(handle);
      ASSERT_NE(after.end(), it);
      const auto& rAfter = it->second;
      const auto description = m.getModelObject<ModelObject>(handle)->briefDescription();

      EXPECT_EQ(rBefore.construction, rAfter.construction) << description << " resolves to a different construction";
      EXPECT_TRUE(rAfter.searchDistance == 1 || rAfter.searchDistance == 4)
        << description << " resolves at search distance " << rAfter.searchDistance << " (from " << rBefore.searchDistance << ")";
    }
  }
}

TEST_F(ModelFixture, DefaultConstructionSet_collapseToSpaceAndBuilding_Idempotence) {

  // Collapsing an already collapsed model is a no-op: nothing created, no pointer changed, no object added or removed

  // Owner (Space, SpaceType, BuildingStory, Building) handle -> its DCS handle (null Handle if none)
  auto ownerSets = [](const Model& m) {
    std::map<Handle, Handle> result;
    auto record = [&result](const auto& owners) {
      for (const auto& owner : owners) {
        auto dcs_ = owner.defaultConstructionSet();
        result.emplace(owner.handle(), dcs_ ? dcs_->handle() : Handle{});
      }
    };
    record(m.getConcreteModelObjects<Space>());
    record(m.getConcreteModelObjects<SpaceType>());
    record(m.getConcreteModelObjects<BuildingStory>());
    if (auto building_ = m.building()) {
      record(std::vector<Building>{*building_});
    }
    return result;
  };

  for (const bool purge : {false, true}) {
    SCOPED_TRACE(fmt::format("purge = {}", purge));

    Model m = makeEquivalenceModel();
    EXPECT_FALSE(DefaultConstructionSet::collapseToSpaceAndBuilding(m, purge).empty());  // Sanity check: the first call does merge

    const auto setsBefore = ownerSets(m);
    const auto nDCS = m.getConcreteModelObjects<DefaultConstructionSet>().size();
    const auto nSurfaceConstructions = m.getConcreteModelObjects<DefaultSurfaceConstructions>().size();
    const auto nSubSurfaceConstructions = m.getConcreteModelObjects<DefaultSubSurfaceConstructions>().size();

    EXPECT_TRUE(DefaultConstructionSet::collapseToSpaceAndBuilding(m, purge).empty());

    EXPECT_EQ(setsBefore, ownerSets(m));
    EXPECT_EQ(nDCS, m.getConcreteModelObjects<DefaultConstructionSet>().size());
    EXPECT_EQ(nSurfaceConstructions, m.getConcreteModelObjects<DefaultSurfaceConstructions>().size());
    EXPECT_EQ(nSubSurfaceConstructions, m.getConcreteModelObjects<DefaultSubSurfaceConstructions>().size());
  }
}
