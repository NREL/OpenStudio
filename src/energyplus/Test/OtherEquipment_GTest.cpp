/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Energy Innovation, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>
#include "EnergyPlusFixture.hpp"

#include "../ForwardTranslator.hpp"

#include "../../model/Model.hpp"
#include "../../model/ThermalZone.hpp"
#include "../../model/Space.hpp"
#include "../../model/OtherEquipment.hpp"
#include "../../model/OtherEquipment_Impl.hpp"
#include "../../model/OtherEquipmentDefinition.hpp"
#include "../../model/OtherEquipmentDefinition_Impl.hpp"
#include "../../model/ScheduleConstant.hpp"
#include "../../model/ScheduleConstant_Impl.hpp"

#include "../../utilities/geometry/Point3d.hpp"

#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/OtherEquipment_FieldEnums.hxx>
#include <utilities/idd/OtherEquipment_Instance_FieldEnums.hxx>
#include <utilities/idd/OtherEquipment_Definition_FieldEnums.hxx>

#include <resources.hxx>

using namespace openstudio::energyplus;
using namespace openstudio::model;
using namespace openstudio;

TEST_F(EnergyPlusFixture, ForwardTranslator_OtherEquipment_Legacy) {
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

  OtherEquipmentDefinition definition(model);
  definition.setDesignLevel(100.0);

  OtherEquipment otherEquipment(definition);
  otherEquipment.setSpace(*space);
  otherEquipment.setMultiplier(2.0);
  EXPECT_TRUE(otherEquipment.setFuelType("NaturalGas"));

  ForwardTranslator forwardTranslator;
  // Default: preserves historical behavior
  ASSERT_TRUE(forwardTranslator.forwardTranslatorOptions().isExcludeSpaceLoadInstancesDefaulted());
  ASSERT_TRUE(forwardTranslator.forwardTranslatorOptions().excludeSpaceLoadInstances());

  Workspace workspace = forwardTranslator.translateModel(model);
  EXPECT_EQ(0, forwardTranslator.errors().size());

  WorkspaceObjectVector instances = workspace.getObjectsByType(IddObjectType::OtherEquipment_Instance);
  ASSERT_EQ(0, instances.size());

  WorkspaceObjectVector equips = workspace.getObjectsByType(IddObjectType::OtherEquipment);
  ASSERT_EQ(1, equips.size());
  const WorkspaceObject& equip = equips[0];
  EXPECT_EQ(space->nameString(), equip.getString(OtherEquipmentFields::ZoneorZoneListorSpaceorSpaceListName, true).get());
  EXPECT_EQ("EquipmentLevel", equip.getString(OtherEquipmentFields::DesignLevelCalculationMethod, true).get());
  EXPECT_EQ("NaturalGas", equip.getString(OtherEquipmentFields::FuelType, true).get());
  // Legacy: multiplier is baked into the DesignLevel
  EXPECT_EQ(200.0, equip.getDouble(OtherEquipmentFields::DesignLevel, true).get());
}

TEST_F(EnergyPlusFixture, ForwardTranslator_OtherEquipment_Instance_Basic) {
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

  OtherEquipmentDefinition definition(model);
  definition.setDesignLevel(100.0);
  definition.setFractionLatent(0.1);
  definition.setFractionRadiant(0.2);
  definition.setFractionLost(0.3);

  OtherEquipment otherEquipment(definition);
  otherEquipment.setSpace(*space);
  otherEquipment.setMultiplier(2.0);
  otherEquipment.setEndUseSubcategory("My End Use");
  EXPECT_TRUE(otherEquipment.setFuelType("NaturalGas"));

  ScheduleConstant schedule(model);
  schedule.setValue(0.5);
  EXPECT_TRUE(otherEquipment.setSchedule(schedule));

  ForwardTranslator forwardTranslator;
  forwardTranslator.setExcludeSpaceLoadInstances(false);

  Workspace workspace = forwardTranslator.translateModel(model);
  EXPECT_EQ(0, forwardTranslator.errors().size());

  // Legacy OtherEquipment object should not be translated at all
  WorkspaceObjectVector legacyEquips = workspace.getObjectsByType(IddObjectType::OtherEquipment);
  ASSERT_EQ(0, legacyEquips.size());

  WorkspaceObjectVector definitions = workspace.getObjectsByType(IddObjectType::OtherEquipment_Definition);
  ASSERT_EQ(1, definitions.size());
  const WorkspaceObject& definitionObject = definitions[0];
  EXPECT_EQ(definition.nameString(), definitionObject.nameString());
  EXPECT_EQ("EquipmentLevel", definitionObject.getString(OtherEquipment_DefinitionFields::DesignLevelCalculationMethod, true).get());
  // Definition values are NOT multiplied
  EXPECT_EQ(100.0, definitionObject.getDouble(OtherEquipment_DefinitionFields::DesignLevel, true).get());
  EXPECT_EQ(0.1, definitionObject.getDouble(OtherEquipment_DefinitionFields::FractionLatent, true).get());
  EXPECT_EQ(0.2, definitionObject.getDouble(OtherEquipment_DefinitionFields::FractionRadiant, true).get());
  EXPECT_EQ(0.3, definitionObject.getDouble(OtherEquipment_DefinitionFields::FractionLost, true).get());

  WorkspaceObjectVector instances = workspace.getObjectsByType(IddObjectType::OtherEquipment_Instance);
  ASSERT_EQ(1, instances.size());
  const WorkspaceObject& instanceObject = instances[0];
  EXPECT_EQ(otherEquipment.nameString(), instanceObject.nameString());

  boost::optional<WorkspaceObject> definitionTarget_ = instanceObject.getTarget(OtherEquipment_InstanceFields::OtherEquipmentDefinitionName);
  ASSERT_TRUE(definitionTarget_);
  EXPECT_EQ(definitionObject.handle(), definitionTarget_->handle());

  // FuelType lives on the Instance, not the Definition
  EXPECT_EQ("NaturalGas", instanceObject.getString(OtherEquipment_InstanceFields::FuelType, true).get());
  EXPECT_EQ(space->nameString(), instanceObject.getString(OtherEquipment_InstanceFields::ZoneorZoneListorSpaceorSpaceListName, true).get());
  EXPECT_EQ(schedule.nameString(), instanceObject.getString(OtherEquipment_InstanceFields::ScheduleName, true).get());
  EXPECT_EQ(2.0, instanceObject.getDouble(OtherEquipment_InstanceFields::Multiplier, true).get());
  EXPECT_EQ("My End Use", instanceObject.getString(OtherEquipment_InstanceFields::EndUseSubcategory, true).get());
}

TEST_F(EnergyPlusFixture, ForwardTranslator_OtherEquipment_Instance_SharedDefinition) {
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

  OtherEquipmentDefinition definition(model);
  definition.setDesignLevel(100.0);

  OtherEquipment otherEquipment1(definition);
  otherEquipment1.setSpace(*space1);

  OtherEquipment otherEquipment2(definition);
  otherEquipment2.setSpace(*space2);
  otherEquipment2.setMultiplier(3.0);

  ForwardTranslator forwardTranslator;
  forwardTranslator.setExcludeSpaceLoadInstances(false);

  Workspace workspace = forwardTranslator.translateModel(model);
  EXPECT_EQ(0, forwardTranslator.errors().size());

  // The shared Definition is only translated (and shared) once
  WorkspaceObjectVector definitions = workspace.getObjectsByType(IddObjectType::OtherEquipment_Definition);
  ASSERT_EQ(1, definitions.size());
  const WorkspaceObject& definitionObject = definitions[0];
  EXPECT_EQ(100.0, definitionObject.getDouble(OtherEquipment_DefinitionFields::DesignLevel, true).get());

  WorkspaceObjectVector instances = workspace.getObjectsByType(IddObjectType::OtherEquipment_Instance);
  ASSERT_EQ(2, instances.size());
  for (const auto& instance : instances) {
    boost::optional<WorkspaceObject> definitionTarget_ = instance.getTarget(OtherEquipment_InstanceFields::OtherEquipmentDefinitionName);
    ASSERT_TRUE(definitionTarget_);
    EXPECT_EQ(definitionObject.handle(), definitionTarget_->handle());

    if (instance.nameString() == otherEquipment1.nameString()) {
      EXPECT_EQ(space1->nameString(), instance.getString(OtherEquipment_InstanceFields::ZoneorZoneListorSpaceorSpaceListName, true).get());
      EXPECT_EQ(1.0, instance.getDouble(OtherEquipment_InstanceFields::Multiplier, true).get());
    } else {
      EXPECT_EQ(space2->nameString(), instance.getString(OtherEquipment_InstanceFields::ZoneorZoneListorSpaceorSpaceListName, true).get());
      EXPECT_EQ(3.0, instance.getDouble(OtherEquipment_InstanceFields::Multiplier, true).get());
    }
  }
}
