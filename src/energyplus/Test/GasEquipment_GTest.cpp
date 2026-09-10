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
#include "../../model/GasEquipment.hpp"
#include "../../model/GasEquipment_Impl.hpp"
#include "../../model/GasEquipmentDefinition.hpp"
#include "../../model/GasEquipmentDefinition_Impl.hpp"
#include "../../model/ScheduleConstant.hpp"
#include "../../model/ScheduleConstant_Impl.hpp"

#include "../../utilities/geometry/Point3d.hpp"

#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/GasEquipment_FieldEnums.hxx>
#include <utilities/idd/GasEquipment_Instance_FieldEnums.hxx>
#include <utilities/idd/GasEquipment_Definition_FieldEnums.hxx>

#include <resources.hxx>

#include <sstream>

using namespace openstudio::energyplus;
using namespace openstudio::model;
using namespace openstudio;

TEST_F(EnergyPlusFixture, ForwardTranslator_GasEquipment) {
  Model model;
}

TEST_F(EnergyPlusFixture, ForwardTranslator_GasEquipment_Legacy) {
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

  GasEquipmentDefinition definition(model);
  definition.setDesignLevel(100.0);

  GasEquipment gasEquipment(definition);
  gasEquipment.setSpace(*space);
  gasEquipment.setMultiplier(2.0);

  ForwardTranslator forwardTranslator;
  // Opt into the legacy (historical) behavior; new default is to use Instance/Definition objects
  forwardTranslator.setExcludeSpaceLoadInstances(true);

  Workspace workspace = forwardTranslator.translateModel(model);
  EXPECT_EQ(0, forwardTranslator.errors().size());

  WorkspaceObjectVector instances = workspace.getObjectsByType(IddObjectType::GasEquipment_Instance);
  ASSERT_EQ(0, instances.size());

  WorkspaceObjectVector equips = workspace.getObjectsByType(IddObjectType::GasEquipment);
  ASSERT_EQ(1, equips.size());
  const WorkspaceObject& equip = equips[0];
  EXPECT_EQ(space->nameString(), equip.getString(GasEquipmentFields::ZoneorZoneListorSpaceorSpaceListName, true).get());
  EXPECT_EQ("EquipmentLevel", equip.getString(GasEquipmentFields::DesignLevelCalculationMethod, true).get());
  // Legacy: multiplier is baked into the DesignLevel
  EXPECT_EQ(200.0, equip.getDouble(GasEquipmentFields::DesignLevel, true).get());
}

TEST_F(EnergyPlusFixture, ForwardTranslator_GasEquipment_Instance_Basic) {
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

  GasEquipmentDefinition definition(model);
  definition.setDesignLevel(100.0);
  definition.setFractionLatent(0.1);
  definition.setFractionRadiant(0.2);
  definition.setFractionLost(0.3);
  definition.setCarbonDioxideGenerationRate(1.0e-7);

  GasEquipment gasEquipment(definition);
  gasEquipment.setSpace(*space);
  gasEquipment.setMultiplier(2.0);
  gasEquipment.setEndUseSubcategory("My End Use");

  ScheduleConstant schedule(model);
  schedule.setValue(0.5);
  EXPECT_TRUE(gasEquipment.setSchedule(schedule));

  ForwardTranslator forwardTranslator;
  forwardTranslator.setExcludeSpaceLoadInstances(false);

  Workspace workspace = forwardTranslator.translateModel(model);
  EXPECT_EQ(0, forwardTranslator.errors().size());

  // Legacy GasEquipment object should not be translated at all
  WorkspaceObjectVector legacyEquips = workspace.getObjectsByType(IddObjectType::GasEquipment);
  ASSERT_EQ(0, legacyEquips.size());

  WorkspaceObjectVector definitions = workspace.getObjectsByType(IddObjectType::GasEquipment_Definition);
  ASSERT_EQ(1, definitions.size());
  const WorkspaceObject& definitionObject = definitions[0];
  EXPECT_EQ(definition.nameString(), definitionObject.nameString());
  EXPECT_EQ("EquipmentLevel", definitionObject.getString(GasEquipment_DefinitionFields::DesignLevelCalculationMethod, true).get());
  // Definition values are NOT multiplied
  EXPECT_EQ(100.0, definitionObject.getDouble(GasEquipment_DefinitionFields::DesignLevel, true).get());
  EXPECT_EQ(0.1, definitionObject.getDouble(GasEquipment_DefinitionFields::FractionLatent, true).get());
  EXPECT_EQ(0.2, definitionObject.getDouble(GasEquipment_DefinitionFields::FractionRadiant, true).get());
  EXPECT_EQ(0.3, definitionObject.getDouble(GasEquipment_DefinitionFields::FractionLost, true).get());
  EXPECT_EQ(1.0e-7, definitionObject.getDouble(GasEquipment_DefinitionFields::CarbonDioxideGenerationRate, true).get());

  WorkspaceObjectVector instances = workspace.getObjectsByType(IddObjectType::GasEquipment_Instance);
  ASSERT_EQ(1, instances.size());
  const WorkspaceObject& instanceObject = instances[0];
  EXPECT_EQ(gasEquipment.nameString(), instanceObject.nameString());

  boost::optional<WorkspaceObject> definitionTarget_ = instanceObject.getTarget(GasEquipment_InstanceFields::GasEquipmentDefinitionName);
  ASSERT_TRUE(definitionTarget_);
  EXPECT_EQ(definitionObject.handle(), definitionTarget_->handle());

  EXPECT_EQ(space->nameString(), instanceObject.getString(GasEquipment_InstanceFields::ZoneorZoneListorSpaceorSpaceListName, true).get());
  EXPECT_EQ(schedule.nameString(), instanceObject.getString(GasEquipment_InstanceFields::ScheduleName, true).get());
  EXPECT_EQ(2.0, instanceObject.getDouble(GasEquipment_InstanceFields::Multiplier, true).get());
  EXPECT_EQ("My End Use", instanceObject.getString(GasEquipment_InstanceFields::EndUseSubcategory, true).get());
}

TEST_F(EnergyPlusFixture, ForwardTranslator_GasEquipment_Instance_SharedDefinition) {
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

  GasEquipmentDefinition definition(model);
  definition.setDesignLevel(100.0);

  GasEquipment gasEquipment1(definition);
  gasEquipment1.setSpace(*space1);

  GasEquipment gasEquipment2(definition);
  gasEquipment2.setSpace(*space2);
  gasEquipment2.setMultiplier(3.0);

  ForwardTranslator forwardTranslator;
  forwardTranslator.setExcludeSpaceLoadInstances(false);

  Workspace workspace = forwardTranslator.translateModel(model);
  EXPECT_EQ(0, forwardTranslator.errors().size());

  // The shared Definition is only translated (and shared) once
  WorkspaceObjectVector definitions = workspace.getObjectsByType(IddObjectType::GasEquipment_Definition);
  ASSERT_EQ(1, definitions.size());
  const WorkspaceObject& definitionObject = definitions[0];
  EXPECT_EQ(100.0, definitionObject.getDouble(GasEquipment_DefinitionFields::DesignLevel, true).get());

  WorkspaceObjectVector instances = workspace.getObjectsByType(IddObjectType::GasEquipment_Instance);
  ASSERT_EQ(2, instances.size());
  for (const auto& instance : instances) {
    boost::optional<WorkspaceObject> definitionTarget_ = instance.getTarget(GasEquipment_InstanceFields::GasEquipmentDefinitionName);
    ASSERT_TRUE(definitionTarget_);
    EXPECT_EQ(definitionObject.handle(), definitionTarget_->handle());

    if (instance.nameString() == gasEquipment1.nameString()) {
      EXPECT_EQ(space1->nameString(), instance.getString(GasEquipment_InstanceFields::ZoneorZoneListorSpaceorSpaceListName, true).get());
      EXPECT_EQ(1.0, instance.getDouble(GasEquipment_InstanceFields::Multiplier, true).get());
    } else {
      EXPECT_EQ(space2->nameString(), instance.getString(GasEquipment_InstanceFields::ZoneorZoneListorSpaceorSpaceListName, true).get());
      EXPECT_EQ(3.0, instance.getDouble(GasEquipment_InstanceFields::Multiplier, true).get());
    }
  }
}
