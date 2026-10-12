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
#include "../../model/DefaultConstructionSet_Impl.hpp"
#include "../../model/DefaultSurfaceConstructions.hpp"
#include "../../model/DefaultSubSurfaceConstructions.hpp"
#include "../../model/Construction.hpp"
#include "../../model/Space.hpp"
#include "../../model/Space_Impl.hpp"
#include "../../model/SpaceType.hpp"
#include "../../model/SpaceType_Impl.hpp"
#include "../../model/ThermalZone.hpp"
#include "../../model/ThermalZone_Impl.hpp"
#include "../../model/StandardOpaqueMaterial.hpp"
#include "../../model/StandardOpaqueMaterial_Impl.hpp"

#include "../../utilities/geometry/Point3d.hpp"

#include "../../utilities/idf/Workspace.hpp"
#include "../../utilities/idf/IdfObject.hpp"
#include "../../utilities/idf/WorkspaceObject.hpp"

// E+ FieldEnums
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/IddFactory.hxx>
#include <utilities/idd/ConstructionAssignmentSet_FieldEnums.hxx>
#include <utilities/idd/Building_FieldEnums.hxx>
#include <utilities/idd/Space_FieldEnums.hxx>
#include <utilities/idd/SurfaceConstructionAssignments_FieldEnums.hxx>

using namespace openstudio::energyplus;
using namespace openstudio::model;
using namespace openstudio;

TEST_F(EnergyPlusFixture, ForwardTranslator_DefaultConstructionSet_Building) {

  ForwardTranslator ft;

  Model m;

  DefaultConstructionSet defaultConstructionSet(m);

  defaultConstructionSet.setName("Building Default Construction Set");
  Building building = m.getUniqueModelObject<Building>();
  EXPECT_TRUE(building.setDefaultConstructionSet(defaultConstructionSet));

  DefaultSurfaceConstructions exteriorSurfaceConstructionAssignments(m);
  exteriorSurfaceConstructionAssignments.setName("Exterior Surface Constructions");
  EXPECT_TRUE(defaultConstructionSet.setDefaultExteriorSurfaceConstructions(exteriorSurfaceConstructionAssignments));

  DefaultSurfaceConstructions interiorSurfaceConstructionAssignments(m);
  interiorSurfaceConstructionAssignments.setName("Interior Surface Constructions");
  EXPECT_TRUE(defaultConstructionSet.setDefaultInteriorSurfaceConstructions(interiorSurfaceConstructionAssignments));

  DefaultSurfaceConstructions groundContactSurfaceConstructionAssignments(m);
  groundContactSurfaceConstructionAssignments.setName("Ground Contact Surface Constructions");
  EXPECT_TRUE(defaultConstructionSet.setDefaultGroundContactSurfaceConstructions(groundContactSurfaceConstructionAssignments));

  DefaultSubSurfaceConstructions exteriorSubSurfaceConstructionAssignments(m);
  exteriorSubSurfaceConstructionAssignments.setName("Exterior SubSurface Constructions");
  EXPECT_TRUE(defaultConstructionSet.setDefaultExteriorSubSurfaceConstructions(exteriorSubSurfaceConstructionAssignments));

  DefaultSubSurfaceConstructions interiorSubSurfaceConstructionAssignments(m);
  interiorSubSurfaceConstructionAssignments.setName("Interior SubSurface Constructions");
  EXPECT_TRUE(defaultConstructionSet.setDefaultInteriorSubSurfaceConstructions(interiorSubSurfaceConstructionAssignments));

  Construction interiorPartitionConstruction(m);
  interiorPartitionConstruction.setName("Interior Partition Construction");
  EXPECT_TRUE(defaultConstructionSet.setInteriorPartitionConstruction(interiorPartitionConstruction));

  Construction adiabaticSurfaceConstruction(m);
  adiabaticSurfaceConstruction.setName("Adiabatic Surface Construction");
  EXPECT_TRUE(defaultConstructionSet.setAdiabaticSurfaceConstruction(adiabaticSurfaceConstruction));

  const Workspace w = ft.translateModel(m);

  const auto buildingObjs = w.getObjectsByType(IddObjectType::Building);
  ASSERT_EQ(1, buildingObjs.size());
  EXPECT_EQ(1, w.getObjectsByType(IddObjectType::ConstructionAssignmentSet).size());

  const auto& buildingObj = buildingObjs.front();
  ASSERT_TRUE(buildingObj.getTarget(BuildingFields::ConstructionAssignmentSetName));
  WorkspaceObject idfObject = buildingObj.getTarget(BuildingFields::ConstructionAssignmentSetName).get();

  EXPECT_EQ(defaultConstructionSet.nameString(), idfObject.nameString());
  EXPECT_EQ(exteriorSurfaceConstructionAssignments.nameString(),
            idfObject.getString(ConstructionAssignmentSetFields::ExteriorSurfaceConstructionAssignmentsName).get());
  EXPECT_EQ(interiorSurfaceConstructionAssignments.nameString(),
            idfObject.getString(ConstructionAssignmentSetFields::InteriorSurfaceConstructionAssignmentsName).get());
  EXPECT_EQ(groundContactSurfaceConstructionAssignments.nameString(),
            idfObject.getString(ConstructionAssignmentSetFields::GroundContactSurfaceConstructionAssignmentsName).get());
  EXPECT_EQ(exteriorSubSurfaceConstructionAssignments.nameString(),
            idfObject.getString(ConstructionAssignmentSetFields::ExteriorSubSurfaceConstructionAssignmentsName).get());
  EXPECT_EQ(interiorSubSurfaceConstructionAssignments.nameString(),
            idfObject.getString(ConstructionAssignmentSetFields::InteriorSubSurfaceConstructionAssignmentsName).get());
  EXPECT_EQ(interiorPartitionConstruction.nameString(),
            idfObject.getString(ConstructionAssignmentSetFields::InteriorPartitionConstructionName).get());
  EXPECT_EQ(adiabaticSurfaceConstruction.nameString(), idfObject.getString(ConstructionAssignmentSetFields::AdiabaticSurfaceConstructionName).get());
}

TEST_F(EnergyPlusFixture, ForwardTranslator_DefaultConstructionSet_Space) {
  // Only checks the wiring: the collapse itself is tested in the model namespace (DefaultConstructionSet_collapseToSpaceAndBuilding_*)
  Model m;

  ThermalZone z(m);

  // The sets must fill at least one slot, otherwise collapseToSpaceAndBuilding resets them
  Construction c(m);
  DefaultSurfaceConstructions exteriorSurfaceConstructions(m);
  EXPECT_TRUE(exteriorSurfaceConstructions.setWallConstruction(c));

  DefaultConstructionSet spaceDCS(m);
  spaceDCS.setName("Space DCS");
  EXPECT_TRUE(spaceDCS.setDefaultExteriorSurfaceConstructions(exteriorSurfaceConstructions));
  Space spaceWithDCS(m);
  spaceWithDCS.setName("Space With DCS");
  EXPECT_TRUE(spaceWithDCS.setThermalZone(z));
  EXPECT_TRUE(spaceWithDCS.setDefaultConstructionSet(spaceDCS));

  // No DCS on the Space, only on its SpaceType: collapseToSpaceAndBuilding moves it down to the Space
  DefaultConstructionSet spaceTypeDCS(m);
  spaceTypeDCS.setName("SpaceType DCS");
  EXPECT_TRUE(spaceTypeDCS.setDefaultExteriorSurfaceConstructions(exteriorSurfaceConstructions));
  SpaceType spaceType(m);
  EXPECT_TRUE(spaceType.setDefaultConstructionSet(spaceTypeDCS));
  Space spaceFromSpaceType(m);
  spaceFromSpaceType.setName("Space From SpaceType");
  EXPECT_TRUE(spaceFromSpaceType.setThermalZone(z));
  EXPECT_TRUE(spaceFromSpaceType.setSpaceType(spaceType));
  EXPECT_FALSE(spaceFromSpaceType.defaultConstructionSet());

  ForwardTranslator ft;

  {
    const Workspace w = ft.translateModel(m);

    auto spaceWithDCS_ = w.getObjectByTypeAndName(IddObjectType::Space, spaceWithDCS.nameString());
    ASSERT_TRUE(spaceWithDCS_);
    ASSERT_TRUE(spaceWithDCS_->getString(SpaceFields::ConstructionAssignmentSetName));
    EXPECT_EQ(spaceDCS.nameString(), spaceWithDCS_->getString(SpaceFields::ConstructionAssignmentSetName).get());

    auto spaceFromSpaceType_ = w.getObjectByTypeAndName(IddObjectType::Space, spaceFromSpaceType.nameString());
    ASSERT_TRUE(spaceFromSpaceType_);
    ASSERT_TRUE(spaceFromSpaceType_->getString(SpaceFields::ConstructionAssignmentSetName));
    EXPECT_EQ(spaceTypeDCS.nameString(), spaceFromSpaceType_->getString(SpaceFields::ConstructionAssignmentSetName).get());
  }

  {
    ft.setExcludeConstructionAssignmentSets(true);
    const Workspace w = ft.translateModel(m);

    for (const auto& space : {spaceWithDCS, spaceFromSpaceType}) {
      auto space_ = w.getObjectByTypeAndName(IddObjectType::Space, space.nameString());
      ASSERT_TRUE(space_);
      EXPECT_TRUE(space_->isEmpty(SpaceFields::ConstructionAssignmentSetName));
    }
  }
}

TEST_F(EnergyPlusFixture, ForwardTranslator_DefaultConstructionSet_BuildingSpaceType) {
  // Only checks the wiring: the Building's SpaceType set is merged into the Building's set, which the E+ Building references
  Model m;

  Construction wallConstruction(m);
  DefaultSurfaceConstructions buildingExteriorSurfaceConstructions(m);
  EXPECT_TRUE(buildingExteriorSurfaceConstructions.setWallConstruction(wallConstruction));
  DefaultConstructionSet buildingDCS(m);
  buildingDCS.setName("Building DCS");
  EXPECT_TRUE(buildingDCS.setDefaultExteriorSurfaceConstructions(buildingExteriorSurfaceConstructions));

  Construction roofConstruction(m);
  DefaultSurfaceConstructions spaceTypeExteriorSurfaceConstructions(m);
  EXPECT_TRUE(spaceTypeExteriorSurfaceConstructions.setRoofCeilingConstruction(roofConstruction));
  DefaultConstructionSet spaceTypeDCS(m);
  spaceTypeDCS.setName("Building SpaceType DCS");
  EXPECT_TRUE(spaceTypeDCS.setDefaultExteriorSurfaceConstructions(spaceTypeExteriorSurfaceConstructions));
  SpaceType spaceType(m);
  EXPECT_TRUE(spaceType.setDefaultConstructionSet(spaceTypeDCS));

  Building building = m.getUniqueModelObject<Building>();
  EXPECT_TRUE(building.setDefaultConstructionSet(buildingDCS));
  EXPECT_TRUE(building.setSpaceType(spaceType));

  ForwardTranslator ft;
  const Workspace w = ft.translateModel(m);

  const auto buildingObjs = w.getObjectsByType(IddObjectType::Building);
  ASSERT_EQ(1, buildingObjs.size());
  auto cas_ = buildingObjs.front().getTarget(BuildingFields::ConstructionAssignmentSetName);
  ASSERT_TRUE(cas_);
  EXPECT_EQ(buildingDCS.nameString(), cas_->nameString());

  auto exteriorSCA_ = cas_->getTarget(ConstructionAssignmentSetFields::ExteriorSurfaceConstructionAssignmentsName);
  ASSERT_TRUE(exteriorSCA_);
  ASSERT_TRUE(exteriorSCA_->getString(SurfaceConstructionAssignmentsFields::WallConstructionName));
  EXPECT_EQ(wallConstruction.nameString(), exteriorSCA_->getString(SurfaceConstructionAssignmentsFields::WallConstructionName).get());
  ASSERT_TRUE(exteriorSCA_->getString(SurfaceConstructionAssignmentsFields::RoofCeilingConstructionName));
  EXPECT_EQ(roofConstruction.nameString(), exteriorSCA_->getString(SurfaceConstructionAssignmentsFields::RoofCeilingConstructionName).get());
}
