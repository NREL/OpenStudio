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