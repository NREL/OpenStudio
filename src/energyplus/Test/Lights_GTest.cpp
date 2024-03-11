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
#include "../../model/Lights.hpp"
#include "../../model/Lights_Impl.hpp"
#include "../../model/LightsDefinition.hpp"
#include "../../model/LightsDefinition_Impl.hpp"
#include "../../model/Space.hpp"
#include "../../model/Space_Impl.hpp"
#include "../../model/Surface.hpp"
#include "../../model/Surface_Impl.hpp"
#include "../../model/SpaceType.hpp"
#include "../../model/SpaceType_Impl.hpp"
#include "../../model/ThermalZone.hpp"
#include "../../model/ThermalZone_Impl.hpp"
#include "../../model/ScheduleCompact.hpp"
#include "../../model/ScheduleCompact_Impl.hpp"
#include "../../model/DefaultScheduleSet.hpp"
#include "../../model/DefaultScheduleSet_Impl.hpp"
#include "../../model/LifeCycleCost.hpp"

#include "../../utilities/idf/WorkspaceExtensibleGroup.hpp"
#include "../../utilities/geometry/Point3d.hpp"

#include <utilities/idd/Lights_FieldEnums.hxx>
#include <utilities/idd/Zone_FieldEnums.hxx>
#include <utilities/idd/ZoneList_FieldEnums.hxx>
#include <utilities/idd/SpaceList_FieldEnums.hxx>
#include <utilities/idd/ComponentCost_LineItem_FieldEnums.hxx>
#include <utilities/idd/LifeCycleCost_NonrecurringCost_FieldEnums.hxx>
#include <utilities/idd/LifeCycleCost_RecurringCosts_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include <resources.hxx>

#include <sstream>

using namespace openstudio::energyplus;
using namespace openstudio::model;
using namespace openstudio;

TEST_F(EnergyPlusFixture, ForwardTranslator_LightsDefinition) {
  Model model;
  LightsDefinition definition(model);

  ForwardTranslator forwardTranslator;
  Workspace workspace = forwardTranslator.translateModel(model);

  std::string errors;
  for (const LogMessage& error : forwardTranslator.errors()) {
    errors += error.logMessage() + "\n";
  }
  EXPECT_EQ(0u, forwardTranslator.errors().size()) << errors;

  std::string warnings;
  for (const LogMessage& warning : forwardTranslator.warnings()) {
    warnings += warning.logMessage() + "\n";
  }
  EXPECT_EQ(0u, forwardTranslator.warnings().size()) << warnings;

  EXPECT_EQ(0u, workspace.getObjectsByType(IddObjectType::Lights).size());
}

TEST_F(EnergyPlusFixture, ForwardTranslator_Lights_NoSpace) {
  Model model;
  LightsDefinition definition(model);
  Lights lights(definition);

  ForwardTranslator forwardTranslator;
  Workspace workspace = forwardTranslator.translateModel(model);

  EXPECT_EQ(0u, workspace.getObjectsByType(IddObjectType::Lights).size());
}

TEST_F(EnergyPlusFixture, ForwardTranslator_Lights_Space) {
  Model model;

  ThermalZone thermalZone(model);
  Space space(model);
  space.setThermalZone(thermalZone);

  LightsDefinition definition(model);
  Lights lights(definition);
  lights.setSpace(space);

  ForwardTranslator forwardTranslator;

  // When excluding space translation (historical behavior)
  {
    forwardTranslator.setExcludeSpaceTranslation(true);

    Workspace workspace = forwardTranslator.translateModel(model);

    ASSERT_EQ(1, workspace.getObjectsByType(IddObjectType::Lights).size());
    ASSERT_EQ(1, workspace.getObjectsByType(IddObjectType::Zone).size());

    WorkspaceObject lightsObject = workspace.getObjectsByType(IddObjectType::Lights)[0];
    WorkspaceObject zoneObject = workspace.getObjectsByType(IddObjectType::Zone)[0];

    ASSERT_TRUE(lightsObject.getTarget(LightsFields::ZoneorZoneListorSpaceorSpaceListName));
    EXPECT_EQ(zoneObject.handle(), lightsObject.getTarget(LightsFields::ZoneorZoneListorSpaceorSpaceListName)->handle());
  }

  // When including Space translation (new E+ 9.6.0)
  {
    forwardTranslator.setExcludeSpaceTranslation(false);

    Workspace workspace = forwardTranslator.translateModel(model);

    ASSERT_EQ(1, workspace.getObjectsByType(IddObjectType::Lights).size());
    ASSERT_EQ(1, workspace.getObjectsByType(IddObjectType::Space).size());

    WorkspaceObject lightsObject = workspace.getObjectsByType(IddObjectType::Lights)[0];
    WorkspaceObject spaceObject = workspace.getObjectsByType(IddObjectType::Space)[0];

    ASSERT_TRUE(lightsObject.getTarget(LightsFields::ZoneorZoneListorSpaceorSpaceListName));
    EXPECT_EQ(spaceObject, lightsObject.getTarget(LightsFields::ZoneorZoneListorSpaceorSpaceListName).get());
  }
}

TEST_F(EnergyPlusFixture, ForwardTranslator_Lights_SpaceType) {
  Model model;

  SpaceType spaceType(model);

  ThermalZone thermalZone(model);
  Space space(model);
  space.setSpaceType(spaceType);
  space.setThermalZone(thermalZone);

  LightsDefinition definition(model);
  Lights lights(definition);
  lights.setSpaceType(spaceType);

  ForwardTranslator forwardTranslator;

  // When excluding space translation (historical behavior)
  {
    forwardTranslator.setExcludeSpaceTranslation(true);
    Workspace workspace = forwardTranslator.translateModel(model);

    ASSERT_EQ(1, workspace.getObjectsByType(IddObjectType::Lights).size());
    ASSERT_EQ(1, workspace.getObjectsByType(IddObjectType::Zone).size());
    ASSERT_EQ(1, workspace.getObjectsByType(IddObjectType::ZoneList).size());
    ASSERT_EQ(0, workspace.getObjectsByType(IddObjectType::SpaceList).size());
    ASSERT_EQ(0, workspace.getObjectsByType(IddObjectType::Space).size());

    WorkspaceObject lightsObject = workspace.getObjectsByType(IddObjectType::Lights)[0];
    WorkspaceObject zoneObject = workspace.getObjectsByType(IddObjectType::Zone)[0];
    WorkspaceObject zoneListObject = workspace.getObjectsByType(IddObjectType::ZoneList)[0];

    ASSERT_TRUE(lightsObject.getTarget(LightsFields::ZoneorZoneListorSpaceorSpaceListName));
    EXPECT_EQ(zoneListObject, lightsObject.getTarget(LightsFields::ZoneorZoneListorSpaceorSpaceListName).get());

    ASSERT_EQ(1, zoneListObject.extensibleGroups().size());
    ASSERT_TRUE(zoneListObject.extensibleGroups()[0].cast<WorkspaceExtensibleGroup>().getTarget(0));
    EXPECT_EQ(zoneObject, zoneListObject.extensibleGroups()[0].cast<WorkspaceExtensibleGroup>().getTarget(0).get());
  }

  // When including Space translation (new E+ 9.6.0)
  {
    forwardTranslator.setExcludeSpaceTranslation(false);

    Workspace workspace = forwardTranslator.translateModel(model);

    ASSERT_EQ(1, workspace.getObjectsByType(IddObjectType::Lights).size());
    ASSERT_EQ(1, workspace.getObjectsByType(IddObjectType::Zone).size());
    ASSERT_EQ(0, workspace.getObjectsByType(IddObjectType::ZoneList).size());
    ASSERT_EQ(1, workspace.getObjectsByType(IddObjectType::SpaceList).size());
    ASSERT_EQ(1, workspace.getObjectsByType(IddObjectType::Space).size());

    WorkspaceObject lightsObject = workspace.getObjectsByType(IddObjectType::Lights)[0];
    WorkspaceObject spaceObject = workspace.getObjectsByType(IddObjectType::Space)[0];
    WorkspaceObject spaceListObject = workspace.getObjectsByType(IddObjectType::SpaceList)[0];

    ASSERT_TRUE(lightsObject.getTarget(LightsFields::ZoneorZoneListorSpaceorSpaceListName));
    EXPECT_EQ(spaceListObject, lightsObject.getTarget(LightsFields::ZoneorZoneListorSpaceorSpaceListName).get());

    ASSERT_EQ(1, spaceListObject.extensibleGroups().size());
    ASSERT_TRUE(spaceListObject.extensibleGroups()[0].cast<WorkspaceExtensibleGroup>().getTarget(0));
    EXPECT_EQ(spaceObject, spaceListObject.extensibleGroups()[0].cast<WorkspaceExtensibleGroup>().getTarget(0).get());
  }
}

TEST_F(EnergyPlusFixture, ForwardTranslator_Lights_OneSpaceType_OneThermalZone) {
  Model model;

  // 2 W/m^2
  SpaceType spaceType(model);
  LightsDefinition definition(model);
  definition.setWattsperSpaceFloorArea(2.0);
  Lights lights(definition);
  lights.setSpaceType(spaceType);

  ThermalZone thermalZone(model);

  // 100 m^2
  std::vector<Point3d> points{{0, 10, 0}, {10, 10, 0}, {10, 0, 0}, {0, 0, 0}};
  boost::optional<Space> space1 = Space::fromFloorPrint(points, 3, model);
  ASSERT_TRUE(space1);
  space1->setSpaceType(spaceType);
  space1->setThermalZone(thermalZone);
  EXPECT_EQ(100.0, space1->floorArea());
  EXPECT_EQ(2.0, space1->lightingPowerPerFloorArea());
  EXPECT_EQ(200.0, space1->lightingPower());

  boost::optional<Space> space2 = Space::fromFloorPrint(points, 3, model);
  ASSERT_TRUE(space2);
  space2->setSpaceType(spaceType);
  space2->setThermalZone(thermalZone);
  EXPECT_EQ(100.0, space2->floorArea());
  EXPECT_EQ(2.0, space2->lightingPowerPerFloorArea());
  EXPECT_EQ(200.0, space2->lightingPower());

  ForwardTranslator forwardTranslator;

  // When excluding space translation (historical behavior)
  {
    forwardTranslator.setExcludeSpaceTranslation(true);

    Workspace workspace = forwardTranslator.translateModel(model);

    ASSERT_EQ(1, workspace.getObjectsByType(IddObjectType::Lights).size());
    ASSERT_EQ(1, workspace.getObjectsByType(IddObjectType::Zone).size());
    ASSERT_EQ(1, workspace.getObjectsByType(IddObjectType::ZoneList).size());
    ASSERT_EQ(0, workspace.getObjectsByType(IddObjectType::SpaceList).size());
    ASSERT_EQ(0, workspace.getObjectsByType(IddObjectType::Space).size());

    WorkspaceObject lightsObject = workspace.getObjectsByType(IddObjectType::Lights)[0];
    WorkspaceObject zoneObject = workspace.getObjectsByType(IddObjectType::Zone)[0];
    WorkspaceObject zoneListObject = workspace.getObjectsByType(IddObjectType::ZoneList)[0];

    ASSERT_TRUE(lightsObject.getString(LightsFields::DesignLevelCalculationMethod, false));
    EXPECT_EQ("Watts/Area", lightsObject.getString(LightsFields::DesignLevelCalculationMethod, false).get());

    ASSERT_TRUE(lightsObject.getDouble(LightsFields::WattsperFloorArea, false));
    EXPECT_EQ(2.0, lightsObject.getDouble(LightsFields::WattsperFloorArea, false).get());

    ASSERT_TRUE(lightsObject.getTarget(LightsFields::ZoneorZoneListorSpaceorSpaceListName));
    EXPECT_EQ(zoneListObject, lightsObject.getTarget(LightsFields::ZoneorZoneListorSpaceorSpaceListName).get());

    ASSERT_EQ(1, zoneListObject.extensibleGroups().size());
    ASSERT_TRUE(zoneListObject.extensibleGroups()[0].cast<WorkspaceExtensibleGroup>().getTarget(0));
    EXPECT_EQ(zoneObject, zoneListObject.extensibleGroups()[0].cast<WorkspaceExtensibleGroup>().getTarget(0).get());
  }

  // When including Space translation (new E+ 9.6.0)
  {
    forwardTranslator.setExcludeSpaceTranslation(false);

    Workspace workspace = forwardTranslator.translateModel(model);

    ASSERT_EQ(1, workspace.getObjectsByType(IddObjectType::Lights).size());
    ASSERT_EQ(1, workspace.getObjectsByType(IddObjectType::Zone).size());
    ASSERT_EQ(0, workspace.getObjectsByType(IddObjectType::ZoneList).size());
    ASSERT_EQ(1, workspace.getObjectsByType(IddObjectType::SpaceList).size());
    ASSERT_EQ(2, workspace.getObjectsByType(IddObjectType::Space).size());

    WorkspaceObject lightsObject = workspace.getObjectsByType(IddObjectType::Lights)[0];
    WorkspaceObject spaceListObject = workspace.getObjectsByType(IddObjectType::SpaceList)[0];

    auto idf_space1_ = workspace.getObjectByTypeAndName(IddObjectType::Space, space1->nameString());
    ASSERT_TRUE(idf_space1_);
    auto idf_space2_ = workspace.getObjectByTypeAndName(IddObjectType::Space, space2->nameString());
    ASSERT_TRUE(idf_space2_);

    ASSERT_TRUE(lightsObject.getString(LightsFields::DesignLevelCalculationMethod, false));
    EXPECT_EQ("Watts/Area", lightsObject.getString(LightsFields::DesignLevelCalculationMethod, false).get());

    ASSERT_TRUE(lightsObject.getDouble(LightsFields::WattsperFloorArea, false));
    EXPECT_EQ(2.0, lightsObject.getDouble(LightsFields::WattsperFloorArea, false).get());

    ASSERT_TRUE(lightsObject.getTarget(LightsFields::ZoneorZoneListorSpaceorSpaceListName));
    EXPECT_EQ(spaceListObject, lightsObject.getTarget(LightsFields::ZoneorZoneListorSpaceorSpaceListName).get());

    ASSERT_EQ(2, spaceListObject.extensibleGroups().size());
    bool foundSpace1 = false;
    bool foundSpace2 = false;
    for (const auto& idf_eg : spaceListObject.extensibleGroups()) {
      auto weg = idf_eg.cast<WorkspaceExtensibleGroup>();
      ASSERT_TRUE(weg.getTarget(0));
      if (idf_space1_.get() == weg.getTarget(0).get()) {
        foundSpace1 = true;
      } else if (idf_space2_.get() == weg.getTarget(0).get()) {
        foundSpace2 = true;
      }
    }
    ASSERT_TRUE(foundSpace1);
    ASSERT_TRUE(foundSpace2);
  }
}

TEST_F(EnergyPlusFixture, ForwardTranslator_Lights_TwoSpaceTypes_OneThermalZone) {
  Model model;

  // 1 W/m^2
  SpaceType spaceType1(model);
  LightsDefinition definition1(model);
  definition1.setWattsperSpaceFloorArea(1.0);
  Lights lights1(definition1);
  lights1.setSpaceType(spaceType1);

  // 2 W/m^2
  SpaceType spaceType2(model);
  LightsDefinition definition2(model);
  definition2.setWattsperSpaceFloorArea(2.0);
  Lights lights2(definition2);
  lights2.setSpaceType(spaceType2);

  ThermalZone thermalZone(model);

  // 100 m^2
  std::vector<Point3d> points{{0, 10, 0}, {10, 10, 0}, {10, 0, 0}, {0, 0, 0}};

  boost::optional<Space> space1 = Space::fromFloorPrint(points, 3, model);
  ASSERT_TRUE(space1);
  space1->setSpaceType(spaceType1);
  space1->setThermalZone(thermalZone);
  EXPECT_EQ(100.0, space1->floorArea());
  EXPECT_EQ(1.0, space1->lightingPowerPerFloorArea());
  EXPECT_EQ(100.0, space1->lightingPower());

  boost::optional<Space> space2 = Space::fromFloorPrint(points, 3, model);
  ASSERT_TRUE(space2);
  space2->setSpaceType(spaceType2);
  space2->setThermalZone(thermalZone);
  EXPECT_EQ(100.0, space2->floorArea());
  EXPECT_EQ(2.0, space2->lightingPowerPerFloorArea());
  EXPECT_EQ(200.0, space2->lightingPower());

  ForwardTranslator forwardTranslator;

  // When excluding space translation (historical behavior)
  {
    forwardTranslator.setExcludeSpaceTranslation(true);
    Workspace workspace = forwardTranslator.translateModel(model);

    ASSERT_EQ(2, workspace.getObjectsByType(IddObjectType::Lights).size());
    ASSERT_EQ(1, workspace.getObjectsByType(IddObjectType::Zone).size());
    EXPECT_EQ(0, workspace.getObjectsByType(IddObjectType::ZoneList).size());
    EXPECT_EQ(0, workspace.getObjectsByType(IddObjectType::SpaceList).size());
    EXPECT_EQ(0, workspace.getObjectsByType(IddObjectType::Space).size());

    bool foundLightingPower100 = false;
    bool foundLightingPower200 = false;
    for (int i = 0; i < 2; ++i) {
      ASSERT_TRUE(workspace.getObjectsByType(IddObjectType::Lights)[i].getString(LightsFields::DesignLevelCalculationMethod, false));
      EXPECT_EQ("LightingLevel",
                workspace.getObjectsByType(IddObjectType::Lights)[i].getString(LightsFields::DesignLevelCalculationMethod, false).get());

      ASSERT_TRUE(workspace.getObjectsByType(IddObjectType::Lights)[i].getDouble(LightsFields::LightingLevel, false));
      double lightingLevel = workspace.getObjectsByType(IddObjectType::Lights)[i].getDouble(LightsFields::LightingLevel, false).get();

      if (lightingLevel == 100.0) {
        foundLightingPower100 = true;
      } else if (lightingLevel == 200.0) {
        foundLightingPower200 = true;
      }
    }

    EXPECT_TRUE(foundLightingPower100);
    EXPECT_TRUE(foundLightingPower200);
  }

  // When including Space translation (new E+ 9.6.0)
  {
    forwardTranslator.setExcludeSpaceTranslation(false);

    Workspace workspace = forwardTranslator.translateModel(model);

    ASSERT_EQ(2, workspace.getObjectsByType(IddObjectType::Lights).size());
    ASSERT_EQ(1, workspace.getObjectsByType(IddObjectType::Zone).size());
    EXPECT_EQ(0, workspace.getObjectsByType(IddObjectType::ZoneList).size());
    EXPECT_EQ(2, workspace.getObjectsByType(IddObjectType::SpaceList).size());  // One per space type
    EXPECT_EQ(2, workspace.getObjectsByType(IddObjectType::Space).size());

    bool foundLightingPower1 = false;
    bool foundLightingPower2 = false;
    for (int i = 0; i < 2; ++i) {
      auto lights = workspace.getObjectsByType(IddObjectType::Lights)[i];
      // There's no need to force convert both to a DesignLevel, instead we keep a Watts/area here
      ASSERT_TRUE(lights.getString(LightsFields::DesignLevelCalculationMethod, false));
      EXPECT_EQ("Watts/Area", lights.getString(LightsFields::DesignLevelCalculationMethod, false).get());

      EXPECT_TRUE(lights.isEmpty(LightsFields::LightingLevel));

      ASSERT_TRUE(workspace.getObjectsByType(IddObjectType::Lights)[i].getDouble(LightsFields::WattsperFloorArea, false));
      double wattsperZoneFloorArea = workspace.getObjectsByType(IddObjectType::Lights)[i].getDouble(LightsFields::WattsperFloorArea, false).get();

      if (wattsperZoneFloorArea == 1.0) {
        foundLightingPower1 = true;
      } else if (wattsperZoneFloorArea == 2.0) {
        foundLightingPower2 = true;
      }
    }

    EXPECT_TRUE(foundLightingPower1);
    EXPECT_TRUE(foundLightingPower2);
  }
}

TEST_F(EnergyPlusFixture, ForwardTranslator_Lights_TwoSpaceTypes_OneThermalZone_Building) {
  Model model;

  // 1 W/m^2
  SpaceType spaceType1(model);
  LightsDefinition definition1(model);
  definition1.setWattsperSpaceFloorArea(1.0);
  Lights lights1(definition1);
  lights1.setSpaceType(spaceType1);

  // 2 W/m^2
  SpaceType spaceType2(model);
  LightsDefinition definition2(model);
  definition2.setWattsperSpaceFloorArea(2.0);
  Lights lights2(definition2);
  lights2.setSpaceType(spaceType2);

  Building building = model.getUniqueModelObject<Building>();
  building.setSpaceType(spaceType1);

  ThermalZone thermalZone(model);

  // 100 m^2
  std::vector<Point3d> points{{0, 10, 0}, {10, 10, 0}, {10, 0, 0}, {0, 0, 0}};

  boost::optional<Space> space1 = Space::fromFloorPrint(points, 3, model);
  ASSERT_TRUE(space1);
  space1->setSpaceType(spaceType1);
  space1->setThermalZone(thermalZone);
  EXPECT_EQ(100.0, space1->floorArea());
  EXPECT_EQ(1.0, space1->lightingPowerPerFloorArea());
  EXPECT_EQ(100.0, space1->lightingPower());

  boost::optional<Space> space2 = Space::fromFloorPrint(points, 3, model);
  ASSERT_TRUE(space2);
  space2->setSpaceType(spaceType2);
  space2->setThermalZone(thermalZone);
  EXPECT_EQ(100.0, space2->floorArea());
  EXPECT_EQ(2.0, space2->lightingPowerPerFloorArea());
  EXPECT_EQ(200.0, space2->lightingPower());

  ForwardTranslator forwardTranslator;

  // When excluding space translation (historical behavior)
  {
    forwardTranslator.setExcludeSpaceTranslation(true);
    Workspace workspace = forwardTranslator.translateModel(model);

    ASSERT_EQ(2, workspace.getObjectsByType(IddObjectType::Lights).size());
    ASSERT_EQ(1, workspace.getObjectsByType(IddObjectType::Zone).size());
    EXPECT_EQ(0, workspace.getObjectsByType(IddObjectType::ZoneList).size());
    EXPECT_EQ(0, workspace.getObjectsByType(IddObjectType::SpaceList).size());
    EXPECT_EQ(0, workspace.getObjectsByType(IddObjectType::Space).size());

    bool foundLightingPower100 = false;
    bool foundLightingPower200 = false;
    for (int i = 0; i < 2; ++i) {
      ASSERT_TRUE(workspace.getObjectsByType(IddObjectType::Lights)[i].getString(LightsFields::DesignLevelCalculationMethod, false));
      EXPECT_EQ("LightingLevel",
                workspace.getObjectsByType(IddObjectType::Lights)[i].getString(LightsFields::DesignLevelCalculationMethod, false).get());

      ASSERT_TRUE(workspace.getObjectsByType(IddObjectType::Lights)[i].getDouble(LightsFields::LightingLevel, false));
      double lightingLevel = workspace.getObjectsByType(IddObjectType::Lights)[i].getDouble(LightsFields::LightingLevel, false).get();

      if (lightingLevel == 100.0) {
        foundLightingPower100 = true;
      } else if (lightingLevel == 200.0) {
        foundLightingPower200 = true;
      }
    }

    EXPECT_TRUE(foundLightingPower100);
    EXPECT_TRUE(foundLightingPower200);
  }

  // When including Space translation (new E+ 9.6.0)
  {
    forwardTranslator.setExcludeSpaceTranslation(false);

    Workspace workspace = forwardTranslator.translateModel(model);

    ASSERT_EQ(2, workspace.getObjectsByType(IddObjectType::Lights).size());
    ASSERT_EQ(1, workspace.getObjectsByType(IddObjectType::Zone).size());
    EXPECT_EQ(0, workspace.getObjectsByType(IddObjectType::ZoneList).size());
    EXPECT_EQ(2, workspace.getObjectsByType(IddObjectType::SpaceList).size());  // One per space type
    EXPECT_EQ(2, workspace.getObjectsByType(IddObjectType::Space).size());

    std::vector<WorkspaceObject> lightss = workspace.getObjectsByType(IddObjectType::Lights);
    ASSERT_EQ(2, lightss.size());
    for (const auto& lights : lightss) {

      boost::optional<WorkspaceObject> target_ = lights.getTarget(LightsFields::ZoneorZoneListorSpaceorSpaceListName);
      ASSERT_TRUE(target_);
      ASSERT_EQ(target_->iddObject().type(), IddObjectType::SpaceList);
      EXPECT_EQ(1, target_->numExtensibleGroups());
      auto eg = target_->extensibleGroups()[0].cast<WorkspaceExtensibleGroup>();
      auto spaceTarget_ = eg.getTarget(SpaceListExtensibleFields::SpaceName);
      ASSERT_TRUE(spaceTarget_);

      // Ok so here it differs from the historical behavior, where it would (have to) combine the densities to a design level

      if (lights.nameString() == lights1.nameString()) {
        // electricEquipment1 is in SpaceType1 which has Space1, density of 1.0 W/m2
        EXPECT_EQ(space1->nameString(), spaceTarget_->nameString());
        EXPECT_EQ(1.0, lights.getDouble(LightsFields::WattsperFloorArea, true).get());

      } else {
        // electricEquipment2 is in SpaceType2 which has Space2, density of 1.0 W/m2 but with a multiplier of 2
        EXPECT_EQ(space2->nameString(), spaceTarget_->nameString());
        EXPECT_EQ(2.0, lights.getDouble(LightsFields::WattsperFloorArea, true).get());
      }

      EXPECT_TRUE(lights.isEmpty(LightsFields::LightingLevel));
      EXPECT_EQ("Watts/Area", lights.getString(LightsFields::DesignLevelCalculationMethod, true).get());
    }
  }
}

TEST_F(EnergyPlusFixture, ForwardTranslator_ExampleModel_Lights) {
  Model model = exampleModel();

  ASSERT_EQ(1u, model.getConcreteModelObjects<SpaceType>().size());
  SpaceType spaceType = model.getConcreteModelObjects<SpaceType>()[0];

  ASSERT_EQ(1u, model.getConcreteModelObjects<ThermalZone>().size());
  ThermalZone thermalZone = model.getConcreteModelObjects<ThermalZone>()[0];

  EXPECT_EQ(4u, model.getConcreteModelObjects<Space>().size());
  for (const Space& space : model.getConcreteModelObjects<Space>()) {
    ASSERT_TRUE(space.spaceType());
    EXPECT_EQ(spaceType.handle(), space.spaceType()->handle());
    ASSERT_TRUE(space.thermalZone());
    EXPECT_EQ(thermalZone.handle(), space.thermalZone()->handle());
  }

  ASSERT_EQ(1u, model.getConcreteModelObjects<Lights>().size());
  Lights lights = model.getConcreteModelObjects<Lights>()[0];
  EXPECT_FALSE(lights.space());
  ASSERT_TRUE(lights.spaceType());
  EXPECT_EQ(spaceType.handle(), lights.spaceType()->handle());

  ForwardTranslator forwardTranslator;
  Workspace workspace = forwardTranslator.translateModel(model);
  EXPECT_EQ(0u, forwardTranslator.errors().size());

  ASSERT_EQ(1u, workspace.getObjectsByType(IddObjectType::Lights).size());
  ASSERT_EQ(1u, workspace.getObjectsByType(IddObjectType::Zone).size());
}

TEST_F(EnergyPlusFixture, ForwardTranslator_Lights_Building_Schedule) {
  Model model;

  ScheduleCompact schedule1(model);
  ScheduleCompact schedule2(model);

  DefaultScheduleSet defaultScheduleSet(model);
  defaultScheduleSet.setLightingSchedule(schedule1);

  Building building = model.getUniqueModelObject<Building>();
  building.setDefaultScheduleSet(defaultScheduleSet);

  ThermalZone thermalZone(model);

  Space space(model);
  space.setThermalZone(thermalZone);

  LightsDefinition definition(model);
  definition.setLightingLevel(1.0);

  Lights lights(definition);
  lights.setSpace(space);
  ASSERT_TRUE(lights.schedule());
  EXPECT_EQ(schedule1.handle(), lights.schedule()->handle());

  lights.setSchedule(schedule2);
  ASSERT_TRUE(lights.schedule());
  EXPECT_EQ(schedule2.handle(), lights.schedule()->handle());

  ForwardTranslator forwardTranslator;
  Workspace workspace = forwardTranslator.translateModel(model);

  ASSERT_EQ(1u, workspace.getObjectsByType(IddObjectType::Lights).size());
  ASSERT_TRUE(workspace.getObjectsByType(IddObjectType::Lights)[0].getString(LightsFields::ScheduleName, false));
  EXPECT_EQ(schedule2.name().get(), workspace.getObjectsByType(IddObjectType::Lights)[0].getString(LightsFields::ScheduleName, false).get());
}

TEST_F(EnergyPlusFixture, ForwardTranslator_Lights_Bug983) {
  Model model;

  std::vector<Point3d> vertices{{0, 10, 0}, {10, 10, 0}, {10, 0, 0}, {0, 0, 0}};
  SpaceType spaceType(model);

  LightsDefinition lightsDefinition(model);
  lightsDefinition.setWattsperSpaceFloorArea(10.00);
  EXPECT_FALSE(lightsDefinition.lightingLevel());
  ASSERT_TRUE(lightsDefinition.wattsperSpaceFloorArea());
  EXPECT_DOUBLE_EQ(10.0, lightsDefinition.wattsperSpaceFloorArea().get());
  EXPECT_FALSE(lightsDefinition.wattsperPerson());

  Building building = model.getUniqueModelObject<Building>();
  building.setSpaceType(spaceType);

  ThermalZone zone(model);

  Space space1(model);
  space1.setThermalZone(zone);

  Surface floor1(vertices, model);
  floor1.setSpace(space1);

  Lights lights1(lightsDefinition);
  lights1.setSpace(space1);

  EXPECT_DOUBLE_EQ(100.00, space1.floorArea());
  EXPECT_DOUBLE_EQ(10.00, space1.lightingPowerPerFloorArea());
  EXPECT_DOUBLE_EQ(1000.00, space1.lightingPower());

  Space space2(model);
  space2.setThermalZone(zone);

  Surface floor2(vertices, model);
  floor2.setSpace(space2);

  Lights lights2(lightsDefinition);
  lights2.setSpace(space2);

  EXPECT_DOUBLE_EQ(100.00, space2.floorArea());
  EXPECT_DOUBLE_EQ(10.00, space2.lightingPowerPerFloorArea());
  EXPECT_DOUBLE_EQ(1000.00, space2.lightingPower());

  ForwardTranslator forwardTranslator;

  // When excluding space translation (historical behavior)
  {
    forwardTranslator.setExcludeSpaceTranslation(true);

    Workspace workspace = forwardTranslator.translateModel(model);
    EXPECT_EQ(0, forwardTranslator.errors().size());

    std::vector<WorkspaceObject> idfLights = workspace.getObjectsByType(IddObjectType::Lights);
    ASSERT_EQ(2, idfLights.size());

    ASSERT_TRUE(idfLights[0].getString(LightsFields::DesignLevelCalculationMethod));
    EXPECT_EQ("LightingLevel", idfLights[0].getString(LightsFields::DesignLevelCalculationMethod).get());
    ASSERT_TRUE(idfLights[0].getDouble(LightsFields::LightingLevel));
    EXPECT_DOUBLE_EQ(1000.0, idfLights[0].getDouble(LightsFields::LightingLevel).get());

    ASSERT_TRUE(idfLights[1].getString(LightsFields::DesignLevelCalculationMethod));
    EXPECT_EQ("LightingLevel", idfLights[1].getString(LightsFields::DesignLevelCalculationMethod).get());
    ASSERT_TRUE(idfLights[1].getDouble(LightsFields::LightingLevel));
    EXPECT_DOUBLE_EQ(1000.0, idfLights[1].getDouble(LightsFields::LightingLevel).get());
  }

  // When including Space translation (new E+ 9.6.0)
  {
    forwardTranslator.setExcludeSpaceTranslation(false);

    Workspace workspace = forwardTranslator.translateModel(model);
    EXPECT_EQ(0, forwardTranslator.errors().size());

    std::vector<WorkspaceObject> idfLights = workspace.getObjectsByType(IddObjectType::Lights);
    ASSERT_EQ(2, idfLights.size());

    ASSERT_TRUE(idfLights[0].getString(LightsFields::DesignLevelCalculationMethod));
    EXPECT_EQ("Watts/Area", idfLights[0].getString(LightsFields::DesignLevelCalculationMethod).get());
    ASSERT_TRUE(idfLights[0].getDouble(LightsFields::WattsperFloorArea));
    EXPECT_DOUBLE_EQ(10.0, idfLights[0].getDouble(LightsFields::WattsperFloorArea).get());

    ASSERT_TRUE(idfLights[1].getString(LightsFields::DesignLevelCalculationMethod));
    EXPECT_EQ("Watts/Area", idfLights[1].getString(LightsFields::DesignLevelCalculationMethod).get());
    ASSERT_TRUE(idfLights[1].getDouble(LightsFields::WattsperFloorArea));
    EXPECT_DOUBLE_EQ(10.0, idfLights[1].getDouble(LightsFields::WattsperFloorArea).get());
  }
}

TEST_F(EnergyPlusFixture, ForwardTranslator_Lights_Costs) {
  Model model;

  ThermalZone zone1(model);
  ThermalZone zone2(model);

  std::vector<Point3d> floorPrint{
    {0, 1, 0},
    {1, 1, 0},
    {1, 0, 0},
    {0, 0, 0},
  };
  boost::optional<Space> space1 = Space::fromFloorPrint(floorPrint, 3, model);
  ASSERT_TRUE(space1);
  space1->setThermalZone(zone1);
  boost::optional<Space> space2 = Space::fromFloorPrint(floorPrint, 3, model);
  ASSERT_TRUE(space2);
  space2->setThermalZone(zone2);

  LightsDefinition definition(model);

  boost::optional<LifeCycleCost> cost1 = LifeCycleCost::createLifeCycleCost("Installation", definition, 3.0, "CostPerArea", "Construction");
  ASSERT_TRUE(cost1);
  boost::optional<LifeCycleCost> cost2 = LifeCycleCost::createLifeCycleCost("Light Bulbs", definition, 5.0, "CostPerArea", "Maintenance", 1);
  ASSERT_TRUE(cost2);

  EXPECT_DOUBLE_EQ(0, definition.floorArea());

  Lights lights1(definition);
  lights1.setName("Lights 1");
  lights1.setSpace(*space1);

  EXPECT_DOUBLE_EQ(1, definition.floorArea());

  Lights lights2(definition);
  lights2.setName("Lights 2");
  lights2.setSpace(*space2);

  EXPECT_DOUBLE_EQ(2, definition.floorArea());

  ForwardTranslator forwardTranslator;
  Workspace workspace = forwardTranslator.translateModel(model);
  EXPECT_EQ(0u, forwardTranslator.errors().size());

  std::vector<WorkspaceObject> idfLights = workspace.getObjectsByType(IddObjectType::Lights);
  ASSERT_EQ(2u, idfLights.size());

  WorkspaceObjectVector idfObjects = workspace.getObjectsByType(IddObjectType::LifeCycleCost_NonrecurringCost);
  ASSERT_EQ(1u, idfObjects.size());
  ASSERT_TRUE(idfObjects[0].getString(LifeCycleCost_NonrecurringCostFields::Name));
  EXPECT_EQ("Installation", idfObjects[0].getString(LifeCycleCost_NonrecurringCostFields::Name).get());
  ASSERT_TRUE(idfObjects[0].getString(LifeCycleCost_NonrecurringCostFields::Category));
  EXPECT_EQ("Construction", idfObjects[0].getString(LifeCycleCost_NonrecurringCostFields::Category).get());
  ASSERT_TRUE(idfObjects[0].getDouble(LifeCycleCost_NonrecurringCostFields::Cost));
  EXPECT_EQ(6.0, idfObjects[0].getDouble(LifeCycleCost_NonrecurringCostFields::Cost).get());
  ASSERT_TRUE(idfObjects[0].getString(LifeCycleCost_NonrecurringCostFields::StartofCosts));
  EXPECT_EQ("ServicePeriod", idfObjects[0].getString(LifeCycleCost_NonrecurringCostFields::StartofCosts).get());
  EXPECT_FALSE(idfObjects[0].getInt(LifeCycleCost_NonrecurringCostFields::YearsfromStart));
  EXPECT_FALSE(idfObjects[0].getInt(LifeCycleCost_NonrecurringCostFields::MonthsfromStart));

  idfObjects = workspace.getObjectsByType(IddObjectType::LifeCycleCost_RecurringCosts);
  ASSERT_EQ(1u, idfObjects.size());
  ASSERT_TRUE(idfObjects[0].getString(LifeCycleCost_RecurringCostsFields::Name));
  EXPECT_EQ("Light Bulbs", idfObjects[0].getString(LifeCycleCost_RecurringCostsFields::Name).get());
  ASSERT_TRUE(idfObjects[0].getString(LifeCycleCost_RecurringCostsFields::Category));
  EXPECT_EQ("Maintenance", idfObjects[0].getString(LifeCycleCost_RecurringCostsFields::Category).get());
  ASSERT_TRUE(idfObjects[0].getDouble(LifeCycleCost_RecurringCostsFields::Cost));
  EXPECT_EQ(10.0, idfObjects[0].getDouble(LifeCycleCost_RecurringCostsFields::Cost).get());
  ASSERT_TRUE(idfObjects[0].getString(LifeCycleCost_RecurringCostsFields::StartofCosts));
  EXPECT_EQ("ServicePeriod", idfObjects[0].getString(LifeCycleCost_RecurringCostsFields::StartofCosts).get());
  EXPECT_FALSE(idfObjects[0].getInt(LifeCycleCost_RecurringCostsFields::YearsfromStart));
  EXPECT_FALSE(idfObjects[0].getInt(LifeCycleCost_RecurringCostsFields::MonthsfromStart));
  ASSERT_TRUE(idfObjects[0].getInt(LifeCycleCost_RecurringCostsFields::RepeatPeriodYears));
  EXPECT_EQ(1, idfObjects[0].getInt(LifeCycleCost_RecurringCostsFields::RepeatPeriodYears).get());
  EXPECT_FALSE(idfObjects[0].getInt(LifeCycleCost_RecurringCostsFields::RepeatPeriodMonths));
  EXPECT_FALSE(idfObjects[0].getDouble(LifeCycleCost_RecurringCostsFields::Annualescalationrate));
}
