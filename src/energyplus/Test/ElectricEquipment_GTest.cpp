/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>
#include "EnergyPlusFixture.hpp"

#include "../ForwardTranslator.hpp"
#include "../ReverseTranslator.hpp"

#include "../../model/Model.hpp"
#include "../../model/Building.hpp"
#include "../../model/Building_Impl.hpp"
#include "../../model/SpaceType.hpp"
#include "../../model/ThermalZone.hpp"
#include "../../model/Space.hpp"
#include "../../model/ElectricEquipment.hpp"
#include "../../model/ElectricEquipment_Impl.hpp"
#include "../../model/ElectricEquipmentDefinition.hpp"
#include "../../model/ElectricEquipmentDefinition_Impl.hpp"

#include "../../utilities/geometry/Point3d.hpp"
#include "../../utilities/idf/IdfExtensibleGroup.hpp"
#include "../../utilities/idf/WorkspaceExtensibleGroup.hpp"

#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/ElectricEquipment_FieldEnums.hxx>
#include <utilities/idd/SpaceList_FieldEnums.hxx>

#include <resources.hxx>

#include <sstream>

using namespace openstudio::energyplus;
using namespace openstudio::model;
using namespace openstudio;

TEST_F(EnergyPlusFixture, ForwardTranslator_ElectricEquipment_SameSpaceType_DesignLevel) {
  Model model;
  Building building = model.getUniqueModelObject<Building>();
  SpaceType spaceType(model);
  ThermalZone zone(model);

  Point3dVector floorPrint{
    {0, 10, 0},
    {10, 10, 0},
    {10, 0, 0},
    {0, 0, 0},
  };

  boost::optional<Space> space1 = Space::fromFloorPrint(floorPrint, 3, model);
  ASSERT_TRUE(space1);
  space1->setSpaceType(spaceType);
  space1->setThermalZone(zone);

  boost::optional<Space> space2 = Space::fromFloorPrint(floorPrint, 3, model);
  ASSERT_TRUE(space2);
  space2->setSpaceType(spaceType);
  space2->setThermalZone(zone);

  EXPECT_DOUBLE_EQ(200.0, building.floorArea());

  ElectricEquipmentDefinition definition(model);
  definition.setDesignLevel(100.0);

  ElectricEquipment electricEquipment(definition);
  electricEquipment.setSpaceType(spaceType);

  EXPECT_EQ(200.0, building.electricEquipmentPower());

  ForwardTranslator forwardTranslator;

  // When excluding space translation (historical behavior)
  {
    forwardTranslator.setExcludeSpaceTranslation(true);

    Workspace workspace = forwardTranslator.translateModel(model);
    EXPECT_EQ(0, forwardTranslator.errors().size());

    WorkspaceObjectVector zones = workspace.getObjectsByType(IddObjectType::Zone);
    ASSERT_EQ(1, zones.size());

    // does not translate absolute loads to a space type
    WorkspaceObjectVector zonelists = workspace.getObjectsByType(IddObjectType::ZoneList);
    ASSERT_EQ(0, zonelists.size());

    WorkspaceObjectVector spaceLists = workspace.getObjectsByType(IddObjectType::SpaceList);
    ASSERT_EQ(0, spaceLists.size());

    WorkspaceObjectVector equips = workspace.getObjectsByType(IddObjectType::ElectricEquipment);
    ASSERT_EQ(2, equips.size());

    for (const auto& equip : equips) {
      EXPECT_EQ(zones[0].name().get(), equip.getString(ElectricEquipmentFields::ZoneorZoneListorSpaceorSpaceListName, true).get());
      EXPECT_EQ("EquipmentLevel", equip.getString(ElectricEquipmentFields::DesignLevelCalculationMethod, true).get());
      EXPECT_EQ(100.0, equip.getDouble(ElectricEquipmentFields::DesignLevel, true).get());
    }
  }

  // When including Space translation (new E+ 9.6.0)
  {
    forwardTranslator.setExcludeSpaceTranslation(false);

    Workspace workspace = forwardTranslator.translateModel(model);
    EXPECT_EQ(0, forwardTranslator.errors().size());

    WorkspaceObjectVector zones = workspace.getObjectsByType(IddObjectType::Zone);
    ASSERT_EQ(1, zones.size());

    // does translate loads to a SpaceList
    WorkspaceObjectVector spaceLists = workspace.getObjectsByType(IddObjectType::SpaceList);
    ASSERT_EQ(1, spaceLists.size());
    ASSERT_EQ(2, spaceLists[0].numExtensibleGroups());

    WorkspaceObjectVector zonelists = workspace.getObjectsByType(IddObjectType::ZoneList);
    ASSERT_EQ(0, zonelists.size());

    WorkspaceObjectVector equips = workspace.getObjectsByType(IddObjectType::ElectricEquipment);
    ASSERT_EQ(1, equips.size());

    for (const auto& equip : equips) {
      EXPECT_EQ(spaceLists[0].name().get(), equip.getString(ElectricEquipmentFields::ZoneorZoneListorSpaceorSpaceListName, true).get());
      EXPECT_EQ("EquipmentLevel", equip.getString(ElectricEquipmentFields::DesignLevelCalculationMethod, true).get());
      EXPECT_EQ(100, equip.getDouble(ElectricEquipmentFields::DesignLevel, true).get());
    }
  }
}

TEST_F(EnergyPlusFixture, ForwardTranslator_ElectricEquipment_DiffSpaceType_DesignLevel) {
  Model model;
  Building building = model.getUniqueModelObject<Building>();
  SpaceType spaceType1(model);
  SpaceType spaceType2(model);
  ThermalZone zone(model);

  Point3dVector floorPrint{
    {0, 10, 0},
    {10, 10, 0},
    {10, 0, 0},
    {0, 0, 0},
  };

  boost::optional<Space> space1 = Space::fromFloorPrint(floorPrint, 3, model);
  ASSERT_TRUE(space1);
  space1->setSpaceType(spaceType1);
  space1->setThermalZone(zone);

  boost::optional<Space> space2 = Space::fromFloorPrint(floorPrint, 3, model);
  ASSERT_TRUE(space2);
  space2->setSpaceType(spaceType2);
  space2->setThermalZone(zone);

  EXPECT_DOUBLE_EQ(200.0, building.floorArea());

  ElectricEquipmentDefinition definition1(model);
  definition1.setDesignLevel(100.0);

  ElectricEquipment electricEquipment1(definition1);
  electricEquipment1.setSpaceType(spaceType1);

  ElectricEquipmentDefinition definition2(model);
  definition2.setDesignLevel(200.0);

  ElectricEquipment electricEquipment2(definition2);
  electricEquipment2.setSpaceType(spaceType2);

  EXPECT_EQ(300.0, building.electricEquipmentPower());

  ForwardTranslator forwardTranslator;

  // When excluding space translation (historical behavior)
  {
    forwardTranslator.setExcludeSpaceTranslation(true);

    Workspace workspace = forwardTranslator.translateModel(model);
    EXPECT_EQ(0, forwardTranslator.errors().size());

    WorkspaceObjectVector zones = workspace.getObjectsByType(IddObjectType::Zone);
    ASSERT_EQ(1, zones.size());

    WorkspaceObjectVector zonelists = workspace.getObjectsByType(IddObjectType::ZoneList);
    ASSERT_EQ(0, zonelists.size());

    WorkspaceObjectVector spaceLists = workspace.getObjectsByType(IddObjectType::SpaceList);
    ASSERT_EQ(0, spaceLists.size());

    WorkspaceObjectVector equips = workspace.getObjectsByType(IddObjectType::ElectricEquipment);
    ASSERT_EQ(2, equips.size());

    double designLevelTotal = 0;
    for (const auto& equip : equips) {
      EXPECT_EQ(zones[0].name().get(), equip.getString(ElectricEquipmentFields::ZoneorZoneListorSpaceorSpaceListName, true).get());
      EXPECT_EQ("EquipmentLevel", equip.getString(ElectricEquipmentFields::DesignLevelCalculationMethod, true).get());
      double thisLevel = equip.getDouble(ElectricEquipmentFields::DesignLevel, true).get();
      designLevelTotal += thisLevel;
    }
    EXPECT_EQ(300.0, designLevelTotal);
  }

  // When including Space translation (new E+ 9.6.0)
  {
    forwardTranslator.setExcludeSpaceTranslation(false);

    Workspace workspace = forwardTranslator.translateModel(model);
    EXPECT_EQ(0, forwardTranslator.errors().size());

    WorkspaceObjectVector zones = workspace.getObjectsByType(IddObjectType::Zone);
    ASSERT_EQ(1, zones.size());

    // does translate loads to a SpaceList
    WorkspaceObjectVector spaceLists = workspace.getObjectsByType(IddObjectType::SpaceList);
    ASSERT_EQ(2, spaceLists.size());

    WorkspaceObjectVector zonelists = workspace.getObjectsByType(IddObjectType::ZoneList);
    ASSERT_EQ(0, zonelists.size());

    WorkspaceObjectVector equips = workspace.getObjectsByType(IddObjectType::ElectricEquipment);
    ASSERT_EQ(2, equips.size());

    double designLevelTotal = 0;
    for (const auto& equip : equips) {
      boost::optional<WorkspaceObject> target_ = equip.getTarget(ElectricEquipmentFields::ZoneorZoneListorSpaceorSpaceListName);
      ASSERT_TRUE(target_);
      ASSERT_EQ(target_->iddObject().type(), IddObjectType::SpaceList);
      EXPECT_EQ(1, target_->numExtensibleGroups());
      auto eg = target_->extensibleGroups()[0].cast<WorkspaceExtensibleGroup>();
      auto spaceTarget_ = eg.getTarget(SpaceListExtensibleFields::SpaceName);
      ASSERT_TRUE(spaceTarget_);
      double thisLevel = equip.getDouble(ElectricEquipmentFields::DesignLevel, true).get();
      designLevelTotal += thisLevel;
      if (equip.nameString() == electricEquipment1.nameString()) {
        // electricEquipment1 is in SpaceType1 which has Space1
        EXPECT_EQ(space1->nameString(), spaceTarget_->nameString());
        EXPECT_EQ(100.0, equip.getDouble(ElectricEquipmentFields::DesignLevel, true).get());

      } else {
        // electricEquipment2 is in SpaceType2 which has Space2
        EXPECT_EQ(space2->nameString(), spaceTarget_->nameString());
        EXPECT_EQ(200.0, equip.getDouble(ElectricEquipmentFields::DesignLevel, true).get());
      }

      EXPECT_EQ("EquipmentLevel", equip.getString(ElectricEquipmentFields::DesignLevelCalculationMethod, true).get());
    }

    EXPECT_EQ(300.0, designLevelTotal);
  }
}

TEST_F(EnergyPlusFixture, ForwardTranslator_ElectricEquipment_DiffSpaceType_DesignLevel_Multiplier) {
  Model model;
  Building building = model.getUniqueModelObject<Building>();
  SpaceType spaceType1(model);
  SpaceType spaceType2(model);
  ThermalZone zone(model);

  Point3dVector floorPrint{
    {0, 10, 0},
    {10, 10, 0},
    {10, 0, 0},
    {0, 0, 0},
  };

  boost::optional<Space> space1 = Space::fromFloorPrint(floorPrint, 3, model);
  ASSERT_TRUE(space1);
  space1->setSpaceType(spaceType1);
  space1->setThermalZone(zone);

  boost::optional<Space> space2 = Space::fromFloorPrint(floorPrint, 3, model);
  ASSERT_TRUE(space2);
  space2->setSpaceType(spaceType2);
  space2->setThermalZone(zone);

  EXPECT_DOUBLE_EQ(200.0, building.floorArea());

  ElectricEquipmentDefinition definition1(model);
  definition1.setDesignLevel(100.0);

  ElectricEquipment electricEquipment1(definition1);
  electricEquipment1.setSpaceType(spaceType1);

  ElectricEquipmentDefinition definition2(model);
  definition2.setDesignLevel(100.0);

  ElectricEquipment electricEquipment2(definition2);
  electricEquipment2.setSpaceType(spaceType2);
  electricEquipment2.setMultiplier(2);

  EXPECT_EQ(300.0, building.electricEquipmentPower());

  ForwardTranslator forwardTranslator;

  // When excluding space translation (historical behavior)
  {
    forwardTranslator.setExcludeSpaceTranslation(true);

    Workspace workspace = forwardTranslator.translateModel(model);
    EXPECT_EQ(0, forwardTranslator.errors().size());

    WorkspaceObjectVector zones = workspace.getObjectsByType(IddObjectType::Zone);
    ASSERT_EQ(1, zones.size());

    WorkspaceObjectVector zonelists = workspace.getObjectsByType(IddObjectType::ZoneList);
    ASSERT_EQ(0, zonelists.size());

    WorkspaceObjectVector spaceLists = workspace.getObjectsByType(IddObjectType::SpaceList);
    ASSERT_EQ(0, spaceLists.size());

    WorkspaceObjectVector equips = workspace.getObjectsByType(IddObjectType::ElectricEquipment);
    ASSERT_EQ(2, equips.size());

    double designLevelTotal = 0;
    for (const auto& equip : equips) {
      EXPECT_EQ(zones[0].name().get(), equip.getString(ElectricEquipmentFields::ZoneorZoneListorSpaceorSpaceListName, true).get());
      EXPECT_EQ("EquipmentLevel", equip.getString(ElectricEquipmentFields::DesignLevelCalculationMethod, true).get());
      designLevelTotal += equip.getDouble(ElectricEquipmentFields::DesignLevel, true).get();
    }
    EXPECT_EQ(300.0, designLevelTotal);
  }

  // When including Space translation (new E+ 9.6.0)
  {
    forwardTranslator.setExcludeSpaceTranslation(false);

    Workspace workspace = forwardTranslator.translateModel(model);
    EXPECT_EQ(0, forwardTranslator.errors().size());

    WorkspaceObjectVector zones = workspace.getObjectsByType(IddObjectType::Zone);
    ASSERT_EQ(1, zones.size());

    WorkspaceObjectVector zonelists = workspace.getObjectsByType(IddObjectType::ZoneList);
    ASSERT_EQ(0, zonelists.size());

    WorkspaceObjectVector spaceLists = workspace.getObjectsByType(IddObjectType::SpaceList);
    ASSERT_EQ(2, spaceLists.size());

    WorkspaceObjectVector equips = workspace.getObjectsByType(IddObjectType::ElectricEquipment);
    ASSERT_EQ(2, equips.size());

    double designLevelTotal = 0;
    for (const auto& equip : equips) {
      boost::optional<WorkspaceObject> target_ = equip.getTarget(ElectricEquipmentFields::ZoneorZoneListorSpaceorSpaceListName);
      ASSERT_TRUE(target_);
      ASSERT_EQ(target_->iddObject().type(), IddObjectType::SpaceList);
      EXPECT_EQ(1, target_->numExtensibleGroups());
      auto eg = target_->extensibleGroups()[0].cast<WorkspaceExtensibleGroup>();
      auto spaceTarget_ = eg.getTarget(SpaceListExtensibleFields::SpaceName);
      ASSERT_TRUE(spaceTarget_);
      double thisLevel = equip.getDouble(ElectricEquipmentFields::DesignLevel, true).get();
      designLevelTotal += thisLevel;
      if (equip.nameString() == electricEquipment1.nameString()) {
        // electricEquipment1 is in SpaceType1 which has Space1
        EXPECT_EQ(space1->nameString(), spaceTarget_->nameString());
        EXPECT_EQ(100.0, equip.getDouble(ElectricEquipmentFields::DesignLevel, true).get());

      } else {
        // electricEquipment2 is in SpaceType2 which has Space2, and it has 100 W with a multiplier of 2!
        EXPECT_EQ(space2->nameString(), spaceTarget_->nameString());
        EXPECT_EQ(200.0, equip.getDouble(ElectricEquipmentFields::DesignLevel, true).get());
      }

      EXPECT_EQ("EquipmentLevel", equip.getString(ElectricEquipmentFields::DesignLevelCalculationMethod, true).get());
    }

    EXPECT_EQ(300.0, designLevelTotal);
  }
}

TEST_F(EnergyPlusFixture, ForwardTranslator_ElectricEquipment_SameSpaceType_Density) {
  Model model;
  Building building = model.getUniqueModelObject<Building>();
  SpaceType spaceType(model);
  ThermalZone zone(model);

  Point3dVector floorPrint{
    {0, 10, 0},
    {10, 10, 0},
    {10, 0, 0},
    {0, 0, 0},
  };

  boost::optional<Space> space1 = Space::fromFloorPrint(floorPrint, 3, model);
  ASSERT_TRUE(space1);
  space1->setSpaceType(spaceType);
  space1->setThermalZone(zone);

  boost::optional<Space> space2 = Space::fromFloorPrint(floorPrint, 3, model);
  ASSERT_TRUE(space2);
  space2->setSpaceType(spaceType);
  space2->setThermalZone(zone);

  EXPECT_DOUBLE_EQ(200.0, building.floorArea());

  ElectricEquipmentDefinition definition(model);
  definition.setWattsperSpaceFloorArea(1);

  ElectricEquipment electricEquipment(definition);
  electricEquipment.setSpaceType(spaceType);

  EXPECT_EQ(200.0, building.electricEquipmentPower());

  ForwardTranslator forwardTranslator;

  // When excluding space translation (historical behavior)
  {
    forwardTranslator.setExcludeSpaceTranslation(true);

    Workspace workspace = forwardTranslator.translateModel(model);
    EXPECT_EQ(0, forwardTranslator.errors().size());

    WorkspaceObjectVector zones = workspace.getObjectsByType(IddObjectType::Zone);
    ASSERT_EQ(1, zones.size());

    WorkspaceObjectVector zonelists = workspace.getObjectsByType(IddObjectType::ZoneList);
    ASSERT_EQ(1, zonelists.size());

    WorkspaceObjectVector spaceLists = workspace.getObjectsByType(IddObjectType::SpaceList);
    ASSERT_EQ(0, spaceLists.size());

    WorkspaceObjectVector equips = workspace.getObjectsByType(IddObjectType::ElectricEquipment);
    ASSERT_EQ(1, equips.size());

    for (const auto& equip : equips) {
      EXPECT_EQ(zonelists[0].name().get(), equip.getString(ElectricEquipmentFields::ZoneorZoneListorSpaceorSpaceListName, true).get());
      EXPECT_EQ("Watts/Area", equip.getString(ElectricEquipmentFields::DesignLevelCalculationMethod, true).get());
      EXPECT_EQ(1.0, equip.getDouble(ElectricEquipmentFields::WattsperFloorArea, true).get());
    }
  }

  // When including Space translation (new E+ 9.6.0)
  {
    forwardTranslator.setExcludeSpaceTranslation(false);

    Workspace workspace = forwardTranslator.translateModel(model);
    EXPECT_EQ(0, forwardTranslator.errors().size());

    WorkspaceObjectVector zones = workspace.getObjectsByType(IddObjectType::Zone);
    ASSERT_EQ(1, zones.size());

    // does translate loads to a SpaceList
    WorkspaceObjectVector spaceLists = workspace.getObjectsByType(IddObjectType::SpaceList);
    ASSERT_EQ(1, spaceLists.size());
    ASSERT_EQ(2, spaceLists[0].numExtensibleGroups());

    WorkspaceObjectVector zonelists = workspace.getObjectsByType(IddObjectType::ZoneList);
    ASSERT_EQ(0, zonelists.size());

    WorkspaceObjectVector equips = workspace.getObjectsByType(IddObjectType::ElectricEquipment);
    ASSERT_EQ(1, equips.size());

    for (const auto& equip : equips) {
      EXPECT_EQ(spaceLists[0].name().get(), equip.getString(ElectricEquipmentFields::ZoneorZoneListorSpaceorSpaceListName, true).get());
      EXPECT_EQ("Watts/Area", equip.getString(ElectricEquipmentFields::DesignLevelCalculationMethod, true).get());
      EXPECT_EQ(1.0, equip.getDouble(ElectricEquipmentFields::WattsperFloorArea, true).get());
    }
  }
}

TEST_F(EnergyPlusFixture, ForwardTranslator_ElectricEquipment_DiffSpaceType_Density) {
  Model model;
  Building building = model.getUniqueModelObject<Building>();
  SpaceType spaceType1(model);
  SpaceType spaceType2(model);
  ThermalZone zone(model);

  Point3dVector floorPrint{
    {0, 10, 0},
    {10, 10, 0},
    {10, 0, 0},
    {0, 0, 0},
  };

  boost::optional<Space> space1 = Space::fromFloorPrint(floorPrint, 3, model);
  ASSERT_TRUE(space1);
  space1->setSpaceType(spaceType1);
  space1->setThermalZone(zone);

  boost::optional<Space> space2 = Space::fromFloorPrint(floorPrint, 3, model);
  ASSERT_TRUE(space2);
  space2->setSpaceType(spaceType2);
  space2->setThermalZone(zone);

  EXPECT_DOUBLE_EQ(200.0, building.floorArea());

  ElectricEquipmentDefinition definition1(model);
  definition1.setWattsperSpaceFloorArea(1.0);

  ElectricEquipment electricEquipment1(definition1);
  electricEquipment1.setSpaceType(spaceType1);

  ElectricEquipmentDefinition definition2(model);
  definition2.setWattsperSpaceFloorArea(2.0);

  ElectricEquipment electricEquipment2(definition2);
  electricEquipment2.setSpaceType(spaceType2);

  EXPECT_EQ(300.0, building.electricEquipmentPower());

  ForwardTranslator forwardTranslator;
  // When excluding space translation (historical behavior)
  {
    forwardTranslator.setExcludeSpaceTranslation(true);

    Workspace workspace = forwardTranslator.translateModel(model);
    EXPECT_EQ(0, forwardTranslator.errors().size());

    WorkspaceObjectVector zones = workspace.getObjectsByType(IddObjectType::Zone);
    ASSERT_EQ(1, zones.size());

    WorkspaceObjectVector zonelists = workspace.getObjectsByType(IddObjectType::ZoneList);
    ASSERT_EQ(0, zonelists.size());

    WorkspaceObjectVector spaceLists = workspace.getObjectsByType(IddObjectType::SpaceList);
    ASSERT_EQ(0, spaceLists.size());

    WorkspaceObjectVector equips = workspace.getObjectsByType(IddObjectType::ElectricEquipment);
    ASSERT_EQ(2, equips.size());

    double designLevelTotal = 0;
    for (const auto& equip : equips) {
      EXPECT_EQ(zones[0].name().get(), equip.getString(ElectricEquipmentFields::ZoneorZoneListorSpaceorSpaceListName, true).get());
      EXPECT_EQ("EquipmentLevel", equip.getString(ElectricEquipmentFields::DesignLevelCalculationMethod, true).get());
      designLevelTotal += equip.getDouble(ElectricEquipmentFields::DesignLevel, true).get();
    }
    EXPECT_EQ(300.0, designLevelTotal);
  }

  // When including Space translation (new E+ 9.6.0)
  {
    forwardTranslator.setExcludeSpaceTranslation(false);

    Workspace workspace = forwardTranslator.translateModel(model);
    EXPECT_EQ(0, forwardTranslator.errors().size());

    WorkspaceObjectVector zones = workspace.getObjectsByType(IddObjectType::Zone);
    ASSERT_EQ(1, zones.size());

    WorkspaceObjectVector zonelists = workspace.getObjectsByType(IddObjectType::ZoneList);
    ASSERT_EQ(0, zonelists.size());

    WorkspaceObjectVector spaceLists = workspace.getObjectsByType(IddObjectType::SpaceList);
    ASSERT_EQ(2, spaceLists.size());

    WorkspaceObjectVector equips = workspace.getObjectsByType(IddObjectType::ElectricEquipment);
    ASSERT_EQ(2, equips.size());

    for (const auto& equip : equips) {
      boost::optional<WorkspaceObject> target_ = equip.getTarget(ElectricEquipmentFields::ZoneorZoneListorSpaceorSpaceListName);
      ASSERT_TRUE(target_);
      ASSERT_EQ(target_->iddObject().type(), IddObjectType::SpaceList);
      EXPECT_EQ(1, target_->numExtensibleGroups());
      auto eg = target_->extensibleGroups()[0].cast<WorkspaceExtensibleGroup>();
      auto spaceTarget_ = eg.getTarget(SpaceListExtensibleFields::SpaceName);
      ASSERT_TRUE(spaceTarget_);

      // Ok so here it differs from the historical behavior, where it would (have to) combine the densities to a design level

      if (equip.nameString() == electricEquipment1.nameString()) {
        // electricEquipment1 is in SpaceType1 which has Space1, density of 1.0 W/m2
        EXPECT_EQ(space1->nameString(), spaceTarget_->nameString());
        EXPECT_EQ(1.0, equip.getDouble(ElectricEquipmentFields::WattsperFloorArea, true).get());

      } else {
        // electricEquipment2 is in SpaceType2 which has Space2, density of 2.0 W/m2
        EXPECT_EQ(space2->nameString(), spaceTarget_->nameString());
        EXPECT_EQ(2.0, equip.getDouble(ElectricEquipmentFields::WattsperFloorArea, true).get());
      }

      EXPECT_TRUE(equip.isEmpty(ElectricEquipmentFields::DesignLevel));
      EXPECT_EQ("Watts/Area", equip.getString(ElectricEquipmentFields::DesignLevelCalculationMethod, true).get());
    }
  }
}

TEST_F(EnergyPlusFixture, ForwardTranslator_ElectricEquipment_DiffSpaceType_Density_Multiplier) {
  Model model;
  Building building = model.getUniqueModelObject<Building>();
  SpaceType spaceType1(model);
  SpaceType spaceType2(model);
  ThermalZone zone(model);

  Point3dVector floorPrint{
    {0, 10, 0},
    {10, 10, 0},
    {10, 0, 0},
    {0, 0, 0},
  };

  boost::optional<Space> space1 = Space::fromFloorPrint(floorPrint, 3, model);
  ASSERT_TRUE(space1);
  space1->setSpaceType(spaceType1);
  space1->setThermalZone(zone);

  boost::optional<Space> space2 = Space::fromFloorPrint(floorPrint, 3, model);
  ASSERT_TRUE(space2);
  space2->setSpaceType(spaceType2);
  space2->setThermalZone(zone);

  EXPECT_DOUBLE_EQ(200.0, building.floorArea());

  ElectricEquipmentDefinition definition1(model);
  definition1.setWattsperSpaceFloorArea(1);

  ElectricEquipment electricEquipment1(definition1);
  electricEquipment1.setSpaceType(spaceType1);

  ElectricEquipmentDefinition definition2(model);
  definition2.setWattsperSpaceFloorArea(1);

  ElectricEquipment electricEquipment2(definition2);
  electricEquipment2.setSpaceType(spaceType2);
  electricEquipment2.setMultiplier(2);

  EXPECT_EQ(300.0, building.electricEquipmentPower());

  ForwardTranslator forwardTranslator;
  // When excluding space translation (historical behavior)
  {
    forwardTranslator.setExcludeSpaceTranslation(true);

    Workspace workspace = forwardTranslator.translateModel(model);
    EXPECT_EQ(0, forwardTranslator.errors().size());

    WorkspaceObjectVector zones = workspace.getObjectsByType(IddObjectType::Zone);
    ASSERT_EQ(1, zones.size());

    WorkspaceObjectVector zonelists = workspace.getObjectsByType(IddObjectType::ZoneList);
    ASSERT_EQ(0, zonelists.size());

    WorkspaceObjectVector spaceLists = workspace.getObjectsByType(IddObjectType::SpaceList);
    ASSERT_EQ(0, spaceLists.size());

    WorkspaceObjectVector equips = workspace.getObjectsByType(IddObjectType::ElectricEquipment);
    ASSERT_EQ(2, equips.size());

    double designLevelTotal = 0;
    for (const auto& equip : equips) {
      EXPECT_EQ(zones[0].name().get(), equip.getString(ElectricEquipmentFields::ZoneorZoneListorSpaceorSpaceListName, true).get());
      EXPECT_EQ("EquipmentLevel", equip.getString(ElectricEquipmentFields::DesignLevelCalculationMethod, true).get());
      designLevelTotal += equip.getDouble(ElectricEquipmentFields::DesignLevel, true).get();
    }
    EXPECT_EQ(300.0, designLevelTotal);
  }

  // When including Space translation (new E+ 9.6.0)
  {
    forwardTranslator.setExcludeSpaceTranslation(false);

    Workspace workspace = forwardTranslator.translateModel(model);
    EXPECT_EQ(0, forwardTranslator.errors().size());

    WorkspaceObjectVector zones = workspace.getObjectsByType(IddObjectType::Zone);
    ASSERT_EQ(1, zones.size());

    WorkspaceObjectVector zonelists = workspace.getObjectsByType(IddObjectType::ZoneList);
    ASSERT_EQ(0, zonelists.size());

    WorkspaceObjectVector spaceLists = workspace.getObjectsByType(IddObjectType::SpaceList);
    ASSERT_EQ(2, spaceLists.size());

    WorkspaceObjectVector equips = workspace.getObjectsByType(IddObjectType::ElectricEquipment);
    ASSERT_EQ(2, equips.size());

    for (const auto& equip : equips) {
      boost::optional<WorkspaceObject> target_ = equip.getTarget(ElectricEquipmentFields::ZoneorZoneListorSpaceorSpaceListName);
      ASSERT_TRUE(target_);
      ASSERT_EQ(target_->iddObject().type(), IddObjectType::SpaceList);
      EXPECT_EQ(1, target_->numExtensibleGroups());
      auto eg = target_->extensibleGroups()[0].cast<WorkspaceExtensibleGroup>();
      auto spaceTarget_ = eg.getTarget(SpaceListExtensibleFields::SpaceName);
      ASSERT_TRUE(spaceTarget_);

      // Ok so here it differs from the historical behavior, where it would (have to) combine the densities to a design level

      if (equip.nameString() == electricEquipment1.nameString()) {
        // electricEquipment1 is in SpaceType1 which has Space1, density of 1.0 W/m2
        EXPECT_EQ(space1->nameString(), spaceTarget_->nameString());
        EXPECT_EQ(1.0, equip.getDouble(ElectricEquipmentFields::WattsperFloorArea, true).get());

      } else {
        // electricEquipment2 is in SpaceType2 which has Space2, density of 1.0 W/m2 but with a multiplier of 2
        EXPECT_EQ(space2->nameString(), spaceTarget_->nameString());
        EXPECT_EQ(2.0, equip.getDouble(ElectricEquipmentFields::WattsperFloorArea, true).get());
      }

      EXPECT_TRUE(equip.isEmpty(ElectricEquipmentFields::DesignLevel));
      EXPECT_EQ("Watts/Area", equip.getString(ElectricEquipmentFields::DesignLevelCalculationMethod, true).get());
    }
  }
}
