/**********************************************************************
*  Copyright (c) 2008-2016, Alliance for Sustainable Energy.
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

#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/ElectricEquipment_FieldEnums.hxx>

#include <resources.hxx>

#include <sstream>

using namespace openstudio::energyplus;
using namespace openstudio::model;
using namespace openstudio;

TEST_F(EnergyPlusFixture,ForwardTranslator_ElectricEquipment_SameSpaceType_DesignLevel)
{
  Model model;
  Building building = model.getUniqueModelObject<Building>();
  SpaceType spaceType(model);
  ThermalZone zone(model);

  Point3dVector floorPrint;
  floorPrint.push_back(Point3d(0, 10, 0));
  floorPrint.push_back(Point3d(10, 10, 0));
  floorPrint.push_back(Point3d(10, 0, 0));
  floorPrint.push_back(Point3d(0, 0, 0));

  boost::optional<Space> space1 = Space::fromFloorPrint(floorPrint, 3, model);
  ASSERT_TRUE(space1);
  space1->setSpaceType(spaceType);
  space1->setThermalZone(zone);

  boost::optional<Space> space2 = Space::fromFloorPrint(floorPrint, 3, model);
  ASSERT_TRUE(space2);
  space2->setSpaceType(spaceType);
  space2->setThermalZone(zone);

  EXPECT_DOUBLE_EQ(200, building.floorArea());

  ElectricEquipmentDefinition definition(model);
  definition.setDesignLevel(100);

  ElectricEquipment electricEquipment(definition);
  electricEquipment.setSpaceType(spaceType);

  EXPECT_EQ(200, building.electricEquipmentPower());

  ForwardTranslator forwardTranslator;
  Workspace workspace = forwardTranslator.translateModel(model);
  EXPECT_EQ(0u, forwardTranslator.errors().size());

  WorkspaceObjectVector zones = workspace.getObjectsByType(IddObjectType::Zone);
  ASSERT_EQ(1u, zones.size());

  // does not translate absolute loads to a space type
  WorkspaceObjectVector zonelists = workspace.getObjectsByType(IddObjectType::ZoneList);
  ASSERT_EQ(0u, zonelists.size());

  WorkspaceObjectVector equips = workspace.getObjectsByType(IddObjectType::ElectricEquipment);
  ASSERT_EQ(2u, equips.size());

  for (const auto& equip : equips){
    EXPECT_EQ(zones[0].name().get(), equip.getString(ElectricEquipmentFields::ZoneorZoneListName, true).get());
    EXPECT_EQ("EquipmentLevel", equip.getString(ElectricEquipmentFields::DesignLevelCalculationMethod, true).get());
    EXPECT_EQ(100, equip.getDouble(ElectricEquipmentFields::DesignLevel, true).get());
  }
}

TEST_F(EnergyPlusFixture, ForwardTranslator_ElectricEquipment_DiffSpaceType_DesignLevel)
{
  Model model;
  Building building = model.getUniqueModelObject<Building>();
  SpaceType spaceType1(model);
  SpaceType spaceType2(model);
  ThermalZone zone(model);

  Point3dVector floorPrint;
  floorPrint.push_back(Point3d(0, 10, 0));
  floorPrint.push_back(Point3d(10, 10, 0));
  floorPrint.push_back(Point3d(10, 0, 0));
  floorPrint.push_back(Point3d(0, 0, 0));

  boost::optional<Space> space1 = Space::fromFloorPrint(floorPrint, 3, model);
  ASSERT_TRUE(space1);
  space1->setSpaceType(spaceType1);
  space1->setThermalZone(zone);

  boost::optional<Space> space2 = Space::fromFloorPrint(floorPrint, 3, model);
  ASSERT_TRUE(space2);
  space2->setSpaceType(spaceType2);
  space2->setThermalZone(zone);

  EXPECT_DOUBLE_EQ(200, building.floorArea());

  ElectricEquipmentDefinition definition1(model);
  definition1.setDesignLevel(100);

  ElectricEquipment electricEquipment1(definition1);
  electricEquipment1.setSpaceType(spaceType1);

  ElectricEquipmentDefinition definition2(model);
  definition2.setDesignLevel(200);

  ElectricEquipment electricEquipment2(definition2);
  electricEquipment2.setSpaceType(spaceType2);

  EXPECT_EQ(300, building.electricEquipmentPower());

  ForwardTranslator forwardTranslator;
  Workspace workspace = forwardTranslator.translateModel(model);
  EXPECT_EQ(0u, forwardTranslator.errors().size());

  WorkspaceObjectVector zones = workspace.getObjectsByType(IddObjectType::Zone);
  ASSERT_EQ(1u, zones.size());

  WorkspaceObjectVector zonelists = workspace.getObjectsByType(IddObjectType::ZoneList);
  ASSERT_EQ(0u, zonelists.size());

  WorkspaceObjectVector equips = workspace.getObjectsByType(IddObjectType::ElectricEquipment);
  ASSERT_EQ(2u, equips.size());

  double designLevelTotal = 0;
  for (const auto& equip : equips){
    EXPECT_EQ(zones[0].name().get(), equip.getString(ElectricEquipmentFields::ZoneorZoneListName, true).get());
    EXPECT_EQ("EquipmentLevel", equip.getString(ElectricEquipmentFields::DesignLevelCalculationMethod, true).get());
    designLevelTotal += equip.getDouble(ElectricEquipmentFields::DesignLevel, true).get();
  }
  EXPECT_EQ(300, designLevelTotal);
}

TEST_F(EnergyPlusFixture, ForwardTranslator_ElectricEquipment_DiffSpaceType_DesignLevel_Multiplier)
{
  Model model;
  Building building = model.getUniqueModelObject<Building>();
  SpaceType spaceType1(model);
  SpaceType spaceType2(model);
  ThermalZone zone(model);

  Point3dVector floorPrint;
  floorPrint.push_back(Point3d(0, 10, 0));
  floorPrint.push_back(Point3d(10, 10, 0));
  floorPrint.push_back(Point3d(10, 0, 0));
  floorPrint.push_back(Point3d(0, 0, 0));

  boost::optional<Space> space1 = Space::fromFloorPrint(floorPrint, 3, model);
  ASSERT_TRUE(space1);
  space1->setSpaceType(spaceType1);
  space1->setThermalZone(zone);

  boost::optional<Space> space2 = Space::fromFloorPrint(floorPrint, 3, model);
  ASSERT_TRUE(space2);
  space2->setSpaceType(spaceType2);
  space2->setThermalZone(zone);

  EXPECT_DOUBLE_EQ(200, building.floorArea());

  ElectricEquipmentDefinition definition1(model);
  definition1.setDesignLevel(100);

  ElectricEquipment electricEquipment1(definition1);
  electricEquipment1.setSpaceType(spaceType1);

  ElectricEquipmentDefinition definition2(model);
  definition2.setDesignLevel(100);
  
  ElectricEquipment electricEquipment2(definition2);
  electricEquipment2.setSpaceType(spaceType2);
  electricEquipment2.setMultiplier(2);

  EXPECT_EQ(300, building.electricEquipmentPower());

  ForwardTranslator forwardTranslator;
  Workspace workspace = forwardTranslator.translateModel(model);
  EXPECT_EQ(0u, forwardTranslator.errors().size());

  WorkspaceObjectVector zones = workspace.getObjectsByType(IddObjectType::Zone);
  ASSERT_EQ(1u, zones.size());

  WorkspaceObjectVector zonelists = workspace.getObjectsByType(IddObjectType::ZoneList);
  ASSERT_EQ(0u, zonelists.size());

  WorkspaceObjectVector equips = workspace.getObjectsByType(IddObjectType::ElectricEquipment);
  ASSERT_EQ(2u, equips.size());

  double designLevelTotal = 0;
  for (const auto& equip : equips){
    EXPECT_EQ(zones[0].name().get(), equip.getString(ElectricEquipmentFields::ZoneorZoneListName, true).get());
    EXPECT_EQ("EquipmentLevel", equip.getString(ElectricEquipmentFields::DesignLevelCalculationMethod, true).get());
    designLevelTotal += equip.getDouble(ElectricEquipmentFields::DesignLevel, true).get();
  }
  EXPECT_EQ(300, designLevelTotal);
}

TEST_F(EnergyPlusFixture, ForwardTranslator_ElectricEquipment_SameSpaceType_Density)
{
  Model model;
  Building building = model.getUniqueModelObject<Building>();
  SpaceType spaceType(model);
  ThermalZone zone(model);

  Point3dVector floorPrint;
  floorPrint.push_back(Point3d(0, 10, 0));
  floorPrint.push_back(Point3d(10, 10, 0));
  floorPrint.push_back(Point3d(10, 0, 0));
  floorPrint.push_back(Point3d(0, 0, 0));

  boost::optional<Space> space1 = Space::fromFloorPrint(floorPrint, 3, model);
  ASSERT_TRUE(space1);
  space1->setSpaceType(spaceType);
  space1->setThermalZone(zone);

  boost::optional<Space> space2 = Space::fromFloorPrint(floorPrint, 3, model);
  ASSERT_TRUE(space2);
  space2->setSpaceType(spaceType);
  space2->setThermalZone(zone);

  EXPECT_DOUBLE_EQ(200, building.floorArea());

  ElectricEquipmentDefinition definition(model);
  definition.setWattsperSpaceFloorArea(1);

  ElectricEquipment electricEquipment(definition);
  electricEquipment.setSpaceType(spaceType);

  EXPECT_EQ(200, building.electricEquipmentPower());

  ForwardTranslator forwardTranslator;
  Workspace workspace = forwardTranslator.translateModel(model);
  EXPECT_EQ(0u, forwardTranslator.errors().size());

  WorkspaceObjectVector zones = workspace.getObjectsByType(IddObjectType::Zone);
  ASSERT_EQ(1u, zones.size());

  WorkspaceObjectVector zonelists = workspace.getObjectsByType(IddObjectType::ZoneList);
  ASSERT_EQ(1u, zonelists.size());

  WorkspaceObjectVector equips = workspace.getObjectsByType(IddObjectType::ElectricEquipment);
  ASSERT_EQ(1u, equips.size());

  for (const auto& equip : equips){
    EXPECT_EQ(zonelists[0].name().get(), equip.getString(ElectricEquipmentFields::ZoneorZoneListName, true).get());
    EXPECT_EQ("Watts/Area", equip.getString(ElectricEquipmentFields::DesignLevelCalculationMethod, true).get());
    EXPECT_EQ(1, equip.getDouble(ElectricEquipmentFields::WattsperZoneFloorArea, true).get());
  }
}

TEST_F(EnergyPlusFixture, ForwardTranslator_ElectricEquipment_DiffSpaceType_Density)
{
  Model model;
  Building building = model.getUniqueModelObject<Building>();
  SpaceType spaceType1(model);
  SpaceType spaceType2(model);
  ThermalZone zone(model);

  Point3dVector floorPrint;
  floorPrint.push_back(Point3d(0, 10, 0));
  floorPrint.push_back(Point3d(10, 10, 0));
  floorPrint.push_back(Point3d(10, 0, 0));
  floorPrint.push_back(Point3d(0, 0, 0));

  boost::optional<Space> space1 = Space::fromFloorPrint(floorPrint, 3, model);
  ASSERT_TRUE(space1);
  space1->setSpaceType(spaceType1);
  space1->setThermalZone(zone);

  boost::optional<Space> space2 = Space::fromFloorPrint(floorPrint, 3, model);
  ASSERT_TRUE(space2);
  space2->setSpaceType(spaceType2);
  space2->setThermalZone(zone);

  EXPECT_DOUBLE_EQ(200, building.floorArea());

  ElectricEquipmentDefinition definition1(model);
  definition1.setWattsperSpaceFloorArea(1);

  ElectricEquipment electricEquipment1(definition1);
  electricEquipment1.setSpaceType(spaceType1);

  ElectricEquipmentDefinition definition2(model);
  definition2.setWattsperSpaceFloorArea(2);

  ElectricEquipment electricEquipment2(definition2);
  electricEquipment2.setSpaceType(spaceType2);

  EXPECT_EQ(300, building.electricEquipmentPower());

  ForwardTranslator forwardTranslator;
  Workspace workspace = forwardTranslator.translateModel(model);
  EXPECT_EQ(0u, forwardTranslator.errors().size());

  WorkspaceObjectVector zones = workspace.getObjectsByType(IddObjectType::Zone);
  ASSERT_EQ(1u, zones.size());

  WorkspaceObjectVector zonelists = workspace.getObjectsByType(IddObjectType::ZoneList);
  ASSERT_EQ(0u, zonelists.size());

  WorkspaceObjectVector equips = workspace.getObjectsByType(IddObjectType::ElectricEquipment);
  ASSERT_EQ(2u, equips.size());

  double designLevelTotal = 0;
  for (const auto& equip : equips){
    EXPECT_EQ(zones[0].name().get(), equip.getString(ElectricEquipmentFields::ZoneorZoneListName, true).get());
    EXPECT_EQ("EquipmentLevel", equip.getString(ElectricEquipmentFields::DesignLevelCalculationMethod, true).get());
    designLevelTotal += equip.getDouble(ElectricEquipmentFields::DesignLevel, true).get();
  }
  EXPECT_EQ(300, designLevelTotal);
}

TEST_F(EnergyPlusFixture, ForwardTranslator_ElectricEquipment_DiffSpaceType_Density_Multiplier)
{
  Model model;
  Building building = model.getUniqueModelObject<Building>();
  SpaceType spaceType1(model);
  SpaceType spaceType2(model);
  ThermalZone zone(model);

  Point3dVector floorPrint;
  floorPrint.push_back(Point3d(0, 10, 0));
  floorPrint.push_back(Point3d(10, 10, 0));
  floorPrint.push_back(Point3d(10, 0, 0));
  floorPrint.push_back(Point3d(0, 0, 0));

  boost::optional<Space> space1 = Space::fromFloorPrint(floorPrint, 3, model);
  ASSERT_TRUE(space1);
  space1->setSpaceType(spaceType1);
  space1->setThermalZone(zone);

  boost::optional<Space> space2 = Space::fromFloorPrint(floorPrint, 3, model);
  ASSERT_TRUE(space2);
  space2->setSpaceType(spaceType2);
  space2->setThermalZone(zone);

  EXPECT_DOUBLE_EQ(200, building.floorArea());

  ElectricEquipmentDefinition definition1(model);
  definition1.setWattsperSpaceFloorArea(1);

  ElectricEquipment electricEquipment1(definition1);
  electricEquipment1.setSpaceType(spaceType1);

  ElectricEquipmentDefinition definition2(model);
  definition2.setWattsperSpaceFloorArea(1);

  ElectricEquipment electricEquipment2(definition2);
  electricEquipment2.setSpaceType(spaceType2);
  electricEquipment2.setMultiplier(2);

  EXPECT_EQ(300, building.electricEquipmentPower());

  ForwardTranslator forwardTranslator;
  Workspace workspace = forwardTranslator.translateModel(model);
  EXPECT_EQ(0u, forwardTranslator.errors().size());

  WorkspaceObjectVector zones = workspace.getObjectsByType(IddObjectType::Zone);
  ASSERT_EQ(1u, zones.size());

  WorkspaceObjectVector zonelists = workspace.getObjectsByType(IddObjectType::ZoneList);
  ASSERT_EQ(0u, zonelists.size());

  WorkspaceObjectVector equips = workspace.getObjectsByType(IddObjectType::ElectricEquipment);
  ASSERT_EQ(2u, equips.size());

  double designLevelTotal = 0;
  for (const auto& equip : equips){
    EXPECT_EQ(zones[0].name().get(), equip.getString(ElectricEquipmentFields::ZoneorZoneListName, true).get());
    EXPECT_EQ("EquipmentLevel", equip.getString(ElectricEquipmentFields::DesignLevelCalculationMethod, true).get());
    designLevelTotal += equip.getDouble(ElectricEquipmentFields::DesignLevel, true).get();
  }
  EXPECT_EQ(300, designLevelTotal);
}