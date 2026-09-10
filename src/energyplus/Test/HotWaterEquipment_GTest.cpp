/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Energy Innovation, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>
#include "EnergyPlusFixture.hpp"

#include "../ForwardTranslator.hpp"
#include "../ReverseTranslator.hpp"

#include "../../model/Model.hpp"
#include "../../model/ThermalZone.hpp"
#include "../../model/Space.hpp"
#include "../../model/HotWaterEquipment.hpp"
#include "../../model/HotWaterEquipment_Impl.hpp"
#include "../../model/HotWaterEquipmentDefinition.hpp"
#include "../../model/HotWaterEquipmentDefinition_Impl.hpp"
#include "../../model/ScheduleConstant.hpp"
#include "../../model/ScheduleConstant_Impl.hpp"

#include "../../utilities/geometry/Point3d.hpp"

#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/HotWaterEquipment_FieldEnums.hxx>
#include <utilities/idd/HotWaterEquipment_Instance_FieldEnums.hxx>
#include <utilities/idd/HotWaterEquipment_Definition_FieldEnums.hxx>

#include <resources.hxx>

#include <sstream>

using namespace openstudio::energyplus;
using namespace openstudio::model;
using namespace openstudio;

TEST_F(EnergyPlusFixture, ForwardTranslator_HotWater) {
  Model model;
}

TEST_F(EnergyPlusFixture, ForwardTranslator_HotWaterEquipment_Legacy) {
  Model model;
  ThermalZone zone(model);

  Point3dVector floorPrint{
    {0, 10, 0},
    {10, 10, 0},
    {10, 0, 0},
    {0, 0, 0},
  };

  boost::optional<Space> space = Space::fromFloorPrint(floorPrint, 3, model);
  ASSERT_TRUE(space);
  space->setThermalZone(zone);

  HotWaterEquipmentDefinition definition(model);
  definition.setDesignLevel(100.0);

  HotWaterEquipment hotWaterEquipment(definition);
  hotWaterEquipment.setSpace(*space);
  hotWaterEquipment.setMultiplier(2.0);

  ForwardTranslator forwardTranslator;
  // Opt into the legacy (historical) behavior; new default is to use Instance/Definition objects
  forwardTranslator.setExcludeSpaceLoadInstances(true);

  Workspace workspace = forwardTranslator.translateModel(model);
  EXPECT_EQ(0, forwardTranslator.errors().size());

  WorkspaceObjectVector instances = workspace.getObjectsByType(IddObjectType::HotWaterEquipment_Instance);
  ASSERT_EQ(0, instances.size());

  WorkspaceObjectVector equips = workspace.getObjectsByType(IddObjectType::HotWaterEquipment);
  ASSERT_EQ(1, equips.size());
  const WorkspaceObject& equip = equips[0];
  EXPECT_EQ(space->nameString(), equip.getString(HotWaterEquipmentFields::ZoneorZoneListorSpaceorSpaceListName, true).get());
  EXPECT_EQ("EquipmentLevel", equip.getString(HotWaterEquipmentFields::DesignLevelCalculationMethod, true).get());
  // Legacy: multiplier is baked into the DesignLevel
  EXPECT_EQ(200.0, equip.getDouble(HotWaterEquipmentFields::DesignLevel, true).get());
}

TEST_F(EnergyPlusFixture, ForwardTranslator_HotWaterEquipment_Instance_Basic) {
  Model model;
  ThermalZone zone(model);

  Point3dVector floorPrint{
    {0, 10, 0},
    {10, 10, 0},
    {10, 0, 0},
    {0, 0, 0},
  };

  boost::optional<Space> space = Space::fromFloorPrint(floorPrint, 3, model);
  ASSERT_TRUE(space);
  space->setThermalZone(zone);

  HotWaterEquipmentDefinition definition(model);
  definition.setDesignLevel(100.0);
  definition.setFractionLatent(0.1);
  definition.setFractionRadiant(0.2);
  definition.setFractionLost(0.3);

  HotWaterEquipment hotWaterEquipment(definition);
  hotWaterEquipment.setSpace(*space);
  hotWaterEquipment.setMultiplier(2.0);
  hotWaterEquipment.setEndUseSubcategory("My End Use");

  ScheduleConstant schedule(model);
  schedule.setValue(0.5);
  EXPECT_TRUE(hotWaterEquipment.setSchedule(schedule));

  ForwardTranslator forwardTranslator;
  forwardTranslator.setExcludeSpaceLoadInstances(false);

  Workspace workspace = forwardTranslator.translateModel(model);
  EXPECT_EQ(0, forwardTranslator.errors().size());

  // Legacy HotWaterEquipment object should not be translated at all
  WorkspaceObjectVector legacyEquips = workspace.getObjectsByType(IddObjectType::HotWaterEquipment);
  ASSERT_EQ(0, legacyEquips.size());

  WorkspaceObjectVector definitions = workspace.getObjectsByType(IddObjectType::HotWaterEquipment_Definition);
  ASSERT_EQ(1, definitions.size());
  const WorkspaceObject& definitionObject = definitions[0];
  EXPECT_EQ(definition.nameString(), definitionObject.nameString());
  EXPECT_EQ("EquipmentLevel", definitionObject.getString(HotWaterEquipment_DefinitionFields::DesignLevelCalculationMethod, true).get());
  // Definition values are NOT multiplied
  EXPECT_EQ(100.0, definitionObject.getDouble(HotWaterEquipment_DefinitionFields::DesignLevel, true).get());
  EXPECT_EQ(0.1, definitionObject.getDouble(HotWaterEquipment_DefinitionFields::FractionLatent, true).get());
  EXPECT_EQ(0.2, definitionObject.getDouble(HotWaterEquipment_DefinitionFields::FractionRadiant, true).get());
  EXPECT_EQ(0.3, definitionObject.getDouble(HotWaterEquipment_DefinitionFields::FractionLost, true).get());

  WorkspaceObjectVector instances = workspace.getObjectsByType(IddObjectType::HotWaterEquipment_Instance);
  ASSERT_EQ(1, instances.size());
  const WorkspaceObject& instanceObject = instances[0];
  EXPECT_EQ(hotWaterEquipment.nameString(), instanceObject.nameString());

  boost::optional<WorkspaceObject> definitionTarget_ = instanceObject.getTarget(HotWaterEquipment_InstanceFields::HotWaterEquipmentDefinitionName);
  ASSERT_TRUE(definitionTarget_);
  EXPECT_EQ(definitionObject.handle(), definitionTarget_->handle());

  EXPECT_EQ(space->nameString(), instanceObject.getString(HotWaterEquipment_InstanceFields::ZoneorZoneListorSpaceorSpaceListName, true).get());
  EXPECT_EQ(schedule.nameString(), instanceObject.getString(HotWaterEquipment_InstanceFields::ScheduleName, true).get());
  EXPECT_EQ(2.0, instanceObject.getDouble(HotWaterEquipment_InstanceFields::Multiplier, true).get());
  EXPECT_EQ("My End Use", instanceObject.getString(HotWaterEquipment_InstanceFields::EndUseSubcategory, true).get());
}

TEST_F(EnergyPlusFixture, ForwardTranslator_HotWaterEquipment_Instance_SharedDefinition) {
  Model model;
  ThermalZone zone(model);

  Point3dVector floorPrint{
    {0, 10, 0},
    {10, 10, 0},
    {10, 0, 0},
    {0, 0, 0},
  };

  boost::optional<Space> space1 = Space::fromFloorPrint(floorPrint, 3, model);
  ASSERT_TRUE(space1);
  space1->setThermalZone(zone);

  boost::optional<Space> space2 = Space::fromFloorPrint(floorPrint, 3, model);
  ASSERT_TRUE(space2);
  space2->setThermalZone(zone);

  HotWaterEquipmentDefinition definition(model);
  definition.setDesignLevel(100.0);

  HotWaterEquipment hotWaterEquipment1(definition);
  hotWaterEquipment1.setSpace(*space1);

  HotWaterEquipment hotWaterEquipment2(definition);
  hotWaterEquipment2.setSpace(*space2);
  hotWaterEquipment2.setMultiplier(3.0);

  ForwardTranslator forwardTranslator;
  forwardTranslator.setExcludeSpaceLoadInstances(false);

  Workspace workspace = forwardTranslator.translateModel(model);
  EXPECT_EQ(0, forwardTranslator.errors().size());

  // The shared Definition is only translated (and shared) once
  WorkspaceObjectVector definitions = workspace.getObjectsByType(IddObjectType::HotWaterEquipment_Definition);
  ASSERT_EQ(1, definitions.size());
  const WorkspaceObject& definitionObject = definitions[0];
  EXPECT_EQ(100.0, definitionObject.getDouble(HotWaterEquipment_DefinitionFields::DesignLevel, true).get());

  WorkspaceObjectVector instances = workspace.getObjectsByType(IddObjectType::HotWaterEquipment_Instance);
  ASSERT_EQ(2, instances.size());
  for (const auto& instance : instances) {
    boost::optional<WorkspaceObject> definitionTarget_ = instance.getTarget(HotWaterEquipment_InstanceFields::HotWaterEquipmentDefinitionName);
    ASSERT_TRUE(definitionTarget_);
    EXPECT_EQ(definitionObject.handle(), definitionTarget_->handle());

    if (instance.nameString() == hotWaterEquipment1.nameString()) {
      EXPECT_EQ(space1->nameString(), instance.getString(HotWaterEquipment_InstanceFields::ZoneorZoneListorSpaceorSpaceListName, true).get());
      EXPECT_EQ(1.0, instance.getDouble(HotWaterEquipment_InstanceFields::Multiplier, true).get());
    } else {
      EXPECT_EQ(space2->nameString(), instance.getString(HotWaterEquipment_InstanceFields::ZoneorZoneListorSpaceorSpaceListName, true).get());
      EXPECT_EQ(3.0, instance.getDouble(HotWaterEquipment_InstanceFields::Multiplier, true).get());
    }
  }
}
