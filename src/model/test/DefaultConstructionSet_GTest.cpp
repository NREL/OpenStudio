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
#include "../Space.hpp"
#include "../SpaceType.hpp"
#include "../Building.hpp"

#include "../../utilities/geometry/Point3d.hpp"

#include <utilities/idd/IddEnums.hxx>

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

TEST_F(ModelFixture, DefaultConstructionSet_collapseToSpaceAndBuilding_BuildingGroup) {

  // Building group: Building DCS (search distance 4), then Building SpaceType DCS (5), collapsed onto the Building

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
    DefaultConstructionSet bDCS(m);
    Construction adiabatic(m);
    EXPECT_TRUE(bDCS.setAdiabaticSurfaceConstruction(adiabatic));
    EXPECT_TRUE(building.setDefaultConstructionSet(bDCS));

    EXPECT_TRUE(DefaultConstructionSet::collapseToSpaceAndBuilding(m).empty());
    ASSERT_TRUE(building.defaultConstructionSet());
    EXPECT_EQ(bDCS, building.defaultConstructionSet().get());
    EXPECT_EQ(1u, m.getConcreteModelObjects<DefaultConstructionSet>().size());
  }

  // Building has an empty DCS: it contributes nothing, so it's reset
  {
    Model m;
    auto building = m.getUniqueModelObject<Building>();
    DefaultConstructionSet bDCS(m);
    EXPECT_TRUE(building.setDefaultConstructionSet(bDCS));

    EXPECT_TRUE(DefaultConstructionSet::collapseToSpaceAndBuilding(m).empty());
    EXPECT_FALSE(building.defaultConstructionSet());
    EXPECT_EQ(1u, m.getConcreteModelObjects<DefaultConstructionSet>().size());  // not removed, just no longer referenced
  }

  // Building SpaceType DCS only: moved up to the Building, no clone. The SpaceType keeps it (no purge)
  {
    Model m;
    auto building = m.getUniqueModelObject<Building>();
    SpaceType spaceType(m);
    EXPECT_TRUE(building.setSpaceType(spaceType));
    DefaultConstructionSet stDCS(m);
    Construction adiabatic(m);
    EXPECT_TRUE(stDCS.setAdiabaticSurfaceConstruction(adiabatic));
    EXPECT_TRUE(spaceType.setDefaultConstructionSet(stDCS));

    EXPECT_TRUE(DefaultConstructionSet::collapseToSpaceAndBuilding(m).empty());
    ASSERT_TRUE(building.defaultConstructionSet());
    EXPECT_EQ(stDCS, building.defaultConstructionSet().get());
    ASSERT_TRUE(spaceType.defaultConstructionSet());
    EXPECT_EQ(stDCS, spaceType.defaultConstructionSet().get());
    EXPECT_EQ(1u, m.getConcreteModelObjects<DefaultConstructionSet>().size());
  }

  // Both point to the same DCS: kept as is
  {
    Model m;
    auto building = m.getUniqueModelObject<Building>();
    SpaceType spaceType(m);
    EXPECT_TRUE(building.setSpaceType(spaceType));
    DefaultConstructionSet dcs(m);
    Construction adiabatic(m);
    EXPECT_TRUE(dcs.setAdiabaticSurfaceConstruction(adiabatic));
    EXPECT_TRUE(building.setDefaultConstructionSet(dcs));
    EXPECT_TRUE(spaceType.setDefaultConstructionSet(dcs));

    EXPECT_TRUE(DefaultConstructionSet::collapseToSpaceAndBuilding(m).empty());
    ASSERT_TRUE(building.defaultConstructionSet());
    EXPECT_EQ(dcs, building.defaultConstructionSet().get());
    ASSERT_TRUE(spaceType.defaultConstructionSet());
    EXPECT_EQ(dcs, spaceType.defaultConstructionSet().get());
    EXPECT_EQ(1u, m.getConcreteModelObjects<DefaultConstructionSet>().size());
  }

  // Both, but the SpaceType DCS only fills slots the Building DCS already fills (with different constructions): it's
  // ignored, the Building keeps its own, no clone
  {
    Model m;
    auto building = m.getUniqueModelObject<Building>();
    SpaceType spaceType(m);
    EXPECT_TRUE(building.setSpaceType(spaceType));

    DefaultConstructionSet bDCS(m);
    DefaultSurfaceConstructions bExt(m);
    Construction bWall(m);
    EXPECT_TRUE(bExt.setWallConstruction(bWall));
    EXPECT_TRUE(bDCS.setDefaultExteriorSurfaceConstructions(bExt));
    EXPECT_TRUE(building.setDefaultConstructionSet(bDCS));

    DefaultConstructionSet stDCS(m);
    DefaultSurfaceConstructions stExt(m);
    Construction stWall(m);
    EXPECT_TRUE(stExt.setWallConstruction(stWall));
    EXPECT_TRUE(stDCS.setDefaultExteriorSurfaceConstructions(stExt));
    EXPECT_TRUE(spaceType.setDefaultConstructionSet(stDCS));

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

    // A DCS that was already unused before the call must survive a purge
    DefaultConstructionSet unusedDCS(m);
    DefaultSurfaceConstructions unusedExt(m);
    EXPECT_TRUE(unusedDCS.setDefaultExteriorSurfaceConstructions(unusedExt));
    EXPECT_EQ(0u, unusedDCS.directUseCount());

    // Collapsing again with purge: same result for the Building, then the SpaceType's DCS is reset and removed, along with
    // its now unused DefaultSurfaceConstructions
    EXPECT_TRUE(DefaultConstructionSet::collapseToSpaceAndBuilding(m, true).empty());
    ASSERT_TRUE(building.defaultConstructionSet());
    EXPECT_EQ(bDCS, building.defaultConstructionSet().get());
    EXPECT_FALSE(spaceType.defaultConstructionSet());
    EXPECT_TRUE(stDCS.handle().isNull());
    EXPECT_TRUE(stExt.handle().isNull());

    EXPECT_FALSE(unusedDCS.handle().isNull());
    EXPECT_FALSE(unusedExt.handle().isNull());
    EXPECT_EQ(2u, m.getConcreteModelObjects<DefaultConstructionSet>().size());       // bDCS, unusedDCS
    EXPECT_EQ(2u, m.getConcreteModelObjects<DefaultSurfaceConstructions>().size());  // bExt, unusedExt
  }

  // Both, and the SpaceType DCS fills slots the Building DCS doesn't: a merged clone is created, Building's slots win,
  // and the originals are untouched
  {
    Model m;
    auto building = m.getUniqueModelObject<Building>();
    building.setName("HQ");
    SpaceType spaceType(m);
    spaceType.setName("Office");
    EXPECT_TRUE(building.setSpaceType(spaceType));

    DefaultConstructionSet bDCS(m);
    DefaultSurfaceConstructions bExt(m);
    Construction bWall(m);
    EXPECT_TRUE(bExt.setWallConstruction(bWall));
    EXPECT_TRUE(bDCS.setDefaultExteriorSurfaceConstructions(bExt));
    EXPECT_TRUE(building.setDefaultConstructionSet(bDCS));

    DefaultConstructionSet stDCS(m);
    DefaultSurfaceConstructions stExt(m);
    Construction stWall(m);
    Construction stRoof(m);
    EXPECT_TRUE(stExt.setWallConstruction(stWall));
    EXPECT_TRUE(stExt.setRoofCeilingConstruction(stRoof));
    EXPECT_TRUE(stDCS.setDefaultExteriorSurfaceConstructions(stExt));
    Construction stAdiabatic(m);
    EXPECT_TRUE(stDCS.setAdiabaticSurfaceConstruction(stAdiabatic));
    EXPECT_TRUE(spaceType.setDefaultConstructionSet(stDCS));

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
}
