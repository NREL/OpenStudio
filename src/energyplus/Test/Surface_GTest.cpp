/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Energy Innovation, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>
#include "EnergyPlusFixture.hpp"

#include "../ForwardTranslator.hpp"
#include "../ReverseTranslator.hpp"

#include "../../model/Model.hpp"
#include "../../model/Surface.hpp"
#include "../../model/Surface_Impl.hpp"
#include "../../model/Space.hpp"
#include "../../model/Space_Impl.hpp"
#include "../../model/ThermalZone.hpp"
#include "../../model/ThermalZone_Impl.hpp"
#include "../../model/SpaceType.hpp"
#include "../../model/SpaceType_Impl.hpp"
#include "../../model/DefaultSurfaceConstructions.hpp"
#include "../../model/DefaultSurfaceConstructions_Impl.hpp"
#include "../../model/DefaultConstructionSet.hpp"
#include "../../model/DefaultConstructionSet_Impl.hpp"
#include "../../model/Construction.hpp"
#include "../../model/Construction_Impl.hpp"
#include "../../model/Building.hpp"
#include "../../model/Building_Impl.hpp"
#include "../../model/StandardOpaqueMaterial.hpp"

#include <utilities/idd/BuildingSurface_Detailed_FieldEnums.hxx>
#include <utilities/idd/Building_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include <resources.hxx>

#include <set>
#include <sstream>

using namespace openstudio::energyplus;
using namespace openstudio::model;
using namespace openstudio;

TEST_F(EnergyPlusFixture, ForwardTranslator_Surface) {
  Model model;

  Point3dVector points{
    {0, 1, 0},
    {0, 0, 0},
    {1, 0, 0},
    {1, 1, 0},
  };
  Surface surface(points, model);

  ForwardTranslator forwardTranslator;
  Workspace workspace = forwardTranslator.translateModel(model);
  EXPECT_EQ(0u, forwardTranslator.errors().size());
  EXPECT_EQ(1u, forwardTranslator.warnings().size());

  EXPECT_EQ(0u, workspace.getObjectsByType(IddObjectType::BuildingSurface_Detailed).size());
}

TEST_F(EnergyPlusFixture, ForwardTranslator_Surface_Zone) {
  Model model;

  ThermalZone thermalZone(model);

  Space space(model);
  space.setThermalZone(thermalZone);

  Point3dVector points{
    {0, 1, 0},
    {0, 0, 0},
    {1, 0, 0},
    {1, 1, 0},
  };
  Surface surface(points, model);
  surface.setSpace(space);

  ForwardTranslator forwardTranslator;
  Workspace workspace = forwardTranslator.translateModel(model);
  EXPECT_EQ(0u, forwardTranslator.errors().size());
  EXPECT_EQ(0u, forwardTranslator.warnings().size());

  ASSERT_EQ(1u, workspace.getObjectsByType(IddObjectType::BuildingSurface_Detailed).size());
  ASSERT_EQ(1u, workspace.getObjectsByType(IddObjectType::Zone).size());

  WorkspaceObject surfaceObject = workspace.getObjectsByType(IddObjectType::BuildingSurface_Detailed)[0];
  WorkspaceObject zoneObject = workspace.getObjectsByType(IddObjectType::Zone)[0];

  ASSERT_TRUE(surfaceObject.getTarget(BuildingSurface_DetailedFields::ZoneName));
  EXPECT_EQ(zoneObject.handle(), surfaceObject.getTarget(BuildingSurface_DetailedFields::ZoneName)->handle());
}

TEST_F(EnergyPlusFixture, ForwardTranslator_Surface_DefaultConstruction) {
  Model model;

  Construction construction(model);

  DefaultSurfaceConstructions defaultSurfaceConstructions(model);
  defaultSurfaceConstructions.setRoofCeilingConstruction(construction);

  DefaultConstructionSet defaultConstructionSet(model);
  defaultConstructionSet.setDefaultExteriorSurfaceConstructions(defaultSurfaceConstructions);

  Building building = model.getUniqueModelObject<Building>();
  building.setDefaultConstructionSet(defaultConstructionSet);

  Space space(model);
  ThermalZone zone(model);
  EXPECT_TRUE(space.setThermalZone(zone));

  Point3dVector points{
    {0, 1, 0},
    {0, 0, 0},
    {1, 0, 0},
    {1, 1, 0},
  };
  Surface surface(points, model);
  surface.setSpace(space);

  EXPECT_EQ("RoofCeiling", surface.surfaceType());
  EXPECT_EQ("Outdoors", surface.outsideBoundaryCondition());
  EXPECT_FALSE(surface.adjacentSurface());
  ASSERT_TRUE(surface.construction());
  EXPECT_TRUE(surface.isConstructionDefaulted());
  EXPECT_EQ(construction.handle(), surface.construction()->handle());

  ForwardTranslator forwardTranslator;
  {
    // Old behavior: construction assignment sets excluded, the defaulted construction is written
    forwardTranslator.setExcludeConstructionAssignmentSets(true);
    Workspace workspace = forwardTranslator.translateModel(model);
    EXPECT_EQ(0u, forwardTranslator.errors().size());
    EXPECT_EQ(0u, forwardTranslator.warnings().size());

    ASSERT_EQ(1u, workspace.getObjectsByType(IddObjectType::BuildingSurface_Detailed).size());
    ASSERT_EQ(1u, workspace.getObjectsByType(IddObjectType::Construction).size());

    WorkspaceObject surfaceObject = workspace.getObjectsByType(IddObjectType::BuildingSurface_Detailed)[0];
    WorkspaceObject constructionObject = workspace.getObjectsByType(IddObjectType::Construction)[0];

    ASSERT_TRUE(surfaceObject.getTarget(BuildingSurface_DetailedFields::ConstructionName));
    EXPECT_EQ(constructionObject.handle(), surfaceObject.getTarget(BuildingSurface_DetailedFields::ConstructionName)->handle());
  }

  {
    // Default: the construction is resolved by E+ via the Building's ConstructionAssignmentSet, so it isn't written
    forwardTranslator.setExcludeConstructionAssignmentSets(false);
    Workspace workspace = forwardTranslator.translateModel(model);
    EXPECT_EQ(0u, forwardTranslator.errors().size());
    EXPECT_EQ(0u, forwardTranslator.warnings().size());

    ASSERT_EQ(1u, workspace.getObjectsByType(IddObjectType::BuildingSurface_Detailed).size());
    ASSERT_EQ(1u, workspace.getObjectsByType(IddObjectType::ConstructionAssignmentSet).size());

    WorkspaceObject surfaceObject = workspace.getObjectsByType(IddObjectType::BuildingSurface_Detailed)[0];
    EXPECT_FALSE(surfaceObject.getTarget(BuildingSurface_DetailedFields::ConstructionName));

    WorkspaceObject buildingObject = workspace.getObjectsByType(IddObjectType::Building)[0];
    WorkspaceObject casObject = workspace.getObjectsByType(IddObjectType::ConstructionAssignmentSet)[0];
    ASSERT_TRUE(buildingObject.getTarget(BuildingFields::ConstructionAssignmentSetName));
    EXPECT_EQ(casObject.handle(), buildingObject.getTarget(BuildingFields::ConstructionAssignmentSetName)->handle());
  }
}

TEST_F(EnergyPlusFixture, ForwardTranslator_Surface_HardAssignedConstruction) {
  // A hard-assigned construction is still written when the construction assignment sets are enabled
  Model model;

  Construction defaultConstruction(model);
  DefaultSurfaceConstructions defaultSurfaceConstructions(model);
  EXPECT_TRUE(defaultSurfaceConstructions.setRoofCeilingConstruction(defaultConstruction));
  DefaultConstructionSet defaultConstructionSet(model);
  EXPECT_TRUE(defaultConstructionSet.setDefaultExteriorSurfaceConstructions(defaultSurfaceConstructions));
  EXPECT_TRUE(model.getUniqueModelObject<Building>().setDefaultConstructionSet(defaultConstructionSet));

  ThermalZone zone(model);
  Space space(model);
  EXPECT_TRUE(space.setThermalZone(zone));

  Surface surface({{0, 1, 0}, {0, 0, 0}, {1, 0, 0}, {1, 1, 0}}, model);
  EXPECT_TRUE(surface.setSpace(space));
  EXPECT_EQ("RoofCeiling", surface.surfaceType());
  EXPECT_EQ("Outdoors", surface.outsideBoundaryCondition());

  Construction hardAssignedConstruction(model);
  EXPECT_TRUE(surface.setConstruction(hardAssignedConstruction));
  EXPECT_FALSE(surface.isConstructionDefaulted());

  ForwardTranslator forwardTranslator;
  Workspace workspace = forwardTranslator.translateModel(model);

  auto surfaceObject_ = workspace.getObjectByTypeAndName(IddObjectType::BuildingSurface_Detailed, surface.nameString());
  ASSERT_TRUE(surfaceObject_);
  ASSERT_TRUE(surfaceObject_->getString(BuildingSurface_DetailedFields::ConstructionName));
  EXPECT_EQ(hardAssignedConstruction.nameString(), surfaceObject_->getString(BuildingSurface_DetailedFields::ConstructionName).get());
}

TEST_F(EnergyPlusFixture, ForwardTranslator_Surface_NoOSDefaultConstruction_Warning) {
  // OpenStudio doesn't resolve a default construction for GroundFCfactorMethod (unlike E+, which uses the Ground Contact slot): the FT warns
  Model model;

  Construction groundFloorConstruction(model);
  DefaultSurfaceConstructions groundContactSurfaceConstructions(model);
  EXPECT_TRUE(groundContactSurfaceConstructions.setFloorConstruction(groundFloorConstruction));
  DefaultConstructionSet defaultConstructionSet(model);
  EXPECT_TRUE(defaultConstructionSet.setDefaultGroundContactSurfaceConstructions(groundContactSurfaceConstructions));
  EXPECT_TRUE(model.getUniqueModelObject<Building>().setDefaultConstructionSet(defaultConstructionSet));

  ThermalZone zone(model);
  Space space(model);
  EXPECT_TRUE(space.setThermalZone(zone));

  auto makeFloor = [&model, &space](const std::string& name) {
    Surface floor({{0, 0, 0}, {0, 1, 0}, {1, 1, 0}, {1, 0, 0}}, model);
    floor.setName(name);
    EXPECT_TRUE(floor.setSpace(space));
    EXPECT_EQ("Floor", floor.surfaceType());
    EXPECT_EQ("Ground", floor.outsideBoundaryCondition());
    return floor;
  };

  Surface groundFloor = makeFloor("Ground Floor");
  ASSERT_TRUE(groundFloor.construction());
  EXPECT_EQ(groundFloorConstruction, groundFloor.construction().get());

  Surface fcFloor = makeFloor("FCfactor Floor");
  EXPECT_TRUE(fcFloor.setOutsideBoundaryCondition("GroundFCfactorMethod"));
  EXPECT_FALSE(fcFloor.construction());

  ForwardTranslator forwardTranslator;
  Workspace workspace = forwardTranslator.translateModel(model);

  std::vector<std::string> warnedSurfaces;
  for (const auto& logMessage : forwardTranslator.warnings()) {
    if (logMessage.logMessage().find("has no construction in OpenStudio") != std::string::npos) {
      warnedSurfaces.push_back(logMessage.logMessage());
    }
  }
  ASSERT_EQ(1, warnedSurfaces.size());
  EXPECT_NE(std::string::npos, warnedSurfaces.front().find(fcFloor.nameString()));

  for (const auto& floor : {groundFloor, fcFloor}) {
    auto surfaceObject_ = workspace.getObjectByTypeAndName(IddObjectType::BuildingSurface_Detailed, floor.nameString());
    ASSERT_TRUE(surfaceObject_);
    EXPECT_TRUE(surfaceObject_->isEmpty(BuildingSurface_DetailedFields::ConstructionName));
  }
}

TEST_F(EnergyPlusFixture, ForwardTranslator_Surface_MatchedPair) {
  // resolveMatchedSurfaceConstructionConflicts hard-assigns both sides of a matched pair with a non-symmetric construction (one gets the reversed
  // construction), so both are written even though they were defaulted
  Model model;

  StandardOpaqueMaterial outer(model);
  StandardOpaqueMaterial inner(model);
  inner.setThickness(outer.thickness() * 2);
  Construction interiorWallConstruction(std::vector<OpaqueMaterial>{outer, inner});
  ASSERT_FALSE(interiorWallConstruction.isSymmetric());

  DefaultSurfaceConstructions interiorSurfaceConstructions(model);
  EXPECT_TRUE(interiorSurfaceConstructions.setWallConstruction(interiorWallConstruction));
  DefaultConstructionSet defaultConstructionSet(model);
  EXPECT_TRUE(defaultConstructionSet.setDefaultInteriorSurfaceConstructions(interiorSurfaceConstructions));
  EXPECT_TRUE(model.getUniqueModelObject<Building>().setDefaultConstructionSet(defaultConstructionSet));

  ThermalZone zone(model);
  std::vector<Space> spaces;
  for (double x0 : {0.0, 10.0}) {
    auto space_ = Space::fromFloorPrint({{x0, 0, 0}, {x0, 10, 0}, {x0 + 10, 10, 0}, {x0 + 10, 0, 0}}, 3.0, model);
    ASSERT_TRUE(space_);
    EXPECT_TRUE(space_->setThermalZone(zone));
    spaces.push_back(*space_);
  }
  matchSurfaces(spaces);

  std::vector<std::string> matchedNames;
  for (const auto& surface : model.getConcreteModelObjects<Surface>()) {
    if (surface.adjacentSurface()) {
      EXPECT_TRUE(surface.isConstructionDefaulted());
      matchedNames.push_back(surface.nameString());
    }
  }
  ASSERT_EQ(2, matchedNames.size());

  ForwardTranslator forwardTranslator;
  Workspace workspace = forwardTranslator.translateModel(model);

  std::set<std::string> constructionNames;
  for (const auto& name : matchedNames) {
    SCOPED_TRACE(name);
    auto surfaceObject_ = workspace.getObjectByTypeAndName(IddObjectType::BuildingSurface_Detailed, name);
    ASSERT_TRUE(surfaceObject_);
    auto constructionName_ = surfaceObject_->getString(BuildingSurface_DetailedFields::ConstructionName, false, true);
    ASSERT_TRUE(constructionName_);
    EXPECT_FALSE(constructionName_->empty());
    constructionNames.insert(*constructionName_);
  }
  // One side has the construction, the other its reverse
  EXPECT_EQ(2, constructionNames.size());
  EXPECT_EQ(1, constructionNames.count(interiorWallConstruction.nameString()));
}

TEST_F(EnergyPlusFixture, ForwardTranslator_Surface_ExcludeSpaceTranslation) {
  // Without E+ Spaces there's no Space level ConstructionAssignmentSet: the FT hard-assigns the constructions resolved from the Space, SpaceType and
  // BuildingStory levels, while the Building level ones are still resolved by E+ via the Building's ConstructionAssignmentSet
  Model model;

  Construction roofConstruction(model);
  DefaultSurfaceConstructions spaceSurfaceConstructions(model);
  EXPECT_TRUE(spaceSurfaceConstructions.setRoofCeilingConstruction(roofConstruction));
  DefaultConstructionSet spaceConstructionSet(model);
  EXPECT_TRUE(spaceConstructionSet.setDefaultExteriorSurfaceConstructions(spaceSurfaceConstructions));

  Construction wallConstruction(model);
  DefaultSurfaceConstructions buildingSurfaceConstructions(model);
  EXPECT_TRUE(buildingSurfaceConstructions.setWallConstruction(wallConstruction));
  DefaultConstructionSet buildingConstructionSet(model);
  EXPECT_TRUE(buildingConstructionSet.setDefaultExteriorSurfaceConstructions(buildingSurfaceConstructions));
  EXPECT_TRUE(model.getUniqueModelObject<Building>().setDefaultConstructionSet(buildingConstructionSet));

  ThermalZone zone(model);
  Space space(model);
  EXPECT_TRUE(space.setThermalZone(zone));
  EXPECT_TRUE(space.setDefaultConstructionSet(spaceConstructionSet));

  Surface roof({{0, 1, 0}, {0, 0, 0}, {1, 0, 0}, {1, 1, 0}}, model);
  EXPECT_TRUE(roof.setSpace(space));
  EXPECT_EQ("RoofCeiling", roof.surfaceType());
  EXPECT_TRUE(roof.isConstructionDefaulted());

  Surface wall({{0, 0, 1}, {0, 0, 0}, {1, 0, 0}, {1, 0, 1}}, model);
  EXPECT_TRUE(wall.setSpace(space));
  EXPECT_EQ("Wall", wall.surfaceType());
  EXPECT_TRUE(wall.isConstructionDefaulted());

  ForwardTranslator forwardTranslator;
  forwardTranslator.setExcludeSpaceTranslation(true);
  EXPECT_FALSE(forwardTranslator.forwardTranslatorOptions().excludeConstructionAssignmentSets());

  Workspace workspace = forwardTranslator.translateModel(model);

  EXPECT_EQ(0u, forwardTranslator.errors().size());
  EXPECT_EQ(0u, forwardTranslator.warnings().size());
  EXPECT_EQ(0u, workspace.getObjectsByType(IddObjectType::Space).size());

  // The Space level construction is hard-assigned
  auto roofObject_ = workspace.getObjectByTypeAndName(IddObjectType::BuildingSurface_Detailed, roof.nameString());
  ASSERT_TRUE(roofObject_);
  ASSERT_TRUE(roofObject_->getString(BuildingSurface_DetailedFields::ConstructionName));
  EXPECT_EQ(roofConstruction.nameString(), roofObject_->getString(BuildingSurface_DetailedFields::ConstructionName).get());

  // The Building level construction is left for E+ to resolve via the Building's ConstructionAssignmentSet
  auto wallObject_ = workspace.getObjectByTypeAndName(IddObjectType::BuildingSurface_Detailed, wall.nameString());
  ASSERT_TRUE(wallObject_);
  EXPECT_TRUE(wallObject_->isEmpty(BuildingSurface_DetailedFields::ConstructionName));

  WorkspaceObject buildingObject = workspace.getObjectsByType(IddObjectType::Building)[0];
  ASSERT_TRUE(buildingObject.getTarget(BuildingFields::ConstructionAssignmentSetName));

  // The input model isn't touched
  EXPECT_TRUE(roof.isConstructionDefaulted());
}
