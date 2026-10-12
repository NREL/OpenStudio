/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Energy Innovation, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>
#include "EnergyPlusFixture.hpp"

#include "../ForwardTranslator.hpp"

#include "../../model/Building.hpp"
#include "../../model/Building_Impl.hpp"
#include "../../model/DefaultConstructionSet.hpp"
#include "../../model/DefaultSurfaceConstructions.hpp"
#include "../../model/Construction.hpp"

#include "../../utilities/idf/Workspace.hpp"
#include "../../utilities/idf/IdfObject.hpp"
#include "../../utilities/idf/WorkspaceObject.hpp"

// E+ FieldEnums
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/IddFactory.hxx>
#include <utilities/idd/SurfaceConstructionAssignments_FieldEnums.hxx>

using namespace openstudio::energyplus;
using namespace openstudio::model;
using namespace openstudio;

TEST_F(EnergyPlusFixture, ForwardTranslator_DefaultSurfaceConstructions) {

  ForwardTranslator ft;

  Model m;

  DefaultConstructionSet defaultConstructionSet(m);

  defaultConstructionSet.setName("Building Default Construction Set");
  Building building = m.getUniqueModelObject<Building>();
  EXPECT_TRUE(building.setDefaultConstructionSet(defaultConstructionSet));

  DefaultSurfaceConstructions defaultSurfaceConstructions(m);
  defaultSurfaceConstructions.setName("Default Surface Constructions");
  EXPECT_TRUE(defaultConstructionSet.setDefaultExteriorSurfaceConstructions(defaultSurfaceConstructions));

  Construction floorConstruction(m);
  floorConstruction.setName("Floor Construction");
  EXPECT_TRUE(defaultSurfaceConstructions.setFloorConstruction(floorConstruction));

  Construction wallConstruction(m);
  wallConstruction.setName("Wall Construction");
  EXPECT_TRUE(defaultSurfaceConstructions.setWallConstruction(wallConstruction));

  Construction roofCeilingConstruction(m);
  roofCeilingConstruction.setName("Roof Ceiling Construction");
  EXPECT_TRUE(defaultSurfaceConstructions.setRoofCeilingConstruction(roofCeilingConstruction));

  const Workspace w = ft.translateModel(m);
  const auto idfObjs = w.getObjectsByType(IddObjectType::SurfaceConstructionAssignments);
  ASSERT_EQ(1u, idfObjs.size());

  const auto& idfObject = idfObjs.front();
  EXPECT_EQ(defaultSurfaceConstructions.nameString(), idfObject.nameString());
  EXPECT_EQ(floorConstruction.nameString(), idfObject.getString(SurfaceConstructionAssignmentsFields::FloorConstructionName).get());
  EXPECT_EQ(wallConstruction.nameString(), idfObject.getString(SurfaceConstructionAssignmentsFields::WallConstructionName).get());
  EXPECT_EQ(roofCeilingConstruction.nameString(), idfObject.getString(SurfaceConstructionAssignmentsFields::RoofCeilingConstructionName).get());
}
