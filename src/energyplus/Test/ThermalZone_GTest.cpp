/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>
#include "EnergyPlusFixture.hpp"

#include "../ForwardTranslator.hpp"
#include "../ReverseTranslator.hpp"

#include "../../model/Model.hpp"
#include "../../model/SpaceType.hpp"
#include "../../model/SpaceType_Impl.hpp"
#include "../../model/Space.hpp"
#include "../../model/Space_Impl.hpp"
#include "../../model/Lights.hpp"
#include "../../model/Lights_Impl.hpp"
#include "../../model/LightsDefinition.hpp"
#include "../../model/LightsDefinition_Impl.hpp"
#include "../../model/Surface.hpp"
#include "../../model/Surface_Impl.hpp"
#include "../../model/Building.hpp"
#include "../../model/Building_Impl.hpp"
#include "../../model/ThermalZone.hpp"
#include "../../model/ThermalZone_Impl.hpp"
#include "../../model/ScheduleConstant.hpp"
#include "../../model/DefaultScheduleSet.hpp"
#include "../../model/DaylightingControl.hpp"
#include "../../model/FanZoneExhaust.hpp"

#include <utilities/idd/Lights_FieldEnums.hxx>
#include <utilities/idd/Daylighting_Controls_FieldEnums.hxx>
#include <utilities/idd/Zone_FieldEnums.hxx>
#include <utilities/idd/ZoneHVAC_EquipmentList_FieldEnums.hxx>
#include <utilities/idd/Schedule_Constant_FieldEnums.hxx>
#include <utilities/idd/ThermostatSetpoint_DualSetpoint_FieldEnums.hxx>
#include <utilities/idd/ZoneControl_Thermostat_FieldEnums.hxx>

#include <utilities/idd/IddEnums.hxx>

#include "../../utilities/idf/IdfExtensibleGroup.hpp"
#include "../../utilities/idf/WorkspaceExtensibleGroup.hpp"

#include <resources.hxx>

#include <sstream>

using namespace openstudio::energyplus;
using namespace openstudio::model;
using namespace openstudio;

TEST_F(EnergyPlusFixture, ForwardTranslator_ThermalZone_OneZone_TwoSpaces_SameSpaceType) {
  Model model;

  Point3dVector points{
    {0, 0, 0},
    {0, 1, 0},
    {1, 1, 0},
    {1, 0, 0},
  };

  SpaceType spaceType(model);
  spaceType.setLightingPowerPerFloorArea(1);

  ThermalZone zone(model);

  Space space1(model);
  space1.setThermalZone(zone);
  space1.setSpaceType(spaceType);

  Space space2(model);
  space2.setThermalZone(zone);
  space2.setSpaceType(spaceType);

  Surface surface1(points, model);
  surface1.setSpace(space1);

  Surface surface2(points, model);
  surface2.setSpace(space2);

  EXPECT_EQ(1, space1.floorArea());
  EXPECT_EQ(1, space2.floorArea());
  EXPECT_EQ(1, space1.lightingPowerPerFloorArea());
  EXPECT_EQ(1, space2.lightingPowerPerFloorArea());

  ForwardTranslator forwardTranslator;

  // When excluding space translation (historical behavior)
  {
    forwardTranslator.setExcludeSpaceTranslation(true);
    Workspace workspace = forwardTranslator.translateModel(model);

    EXPECT_EQ(1u, workspace.getObjectsByType(IddObjectType::Zone).size());
    EXPECT_EQ(1u, workspace.getObjectsByType(IddObjectType::ZoneList).size());
    EXPECT_EQ(1u, workspace.getObjectsByType(IddObjectType::Lights).size());
    EXPECT_EQ(0u, workspace.getObjectsByType(IddObjectType::Space).size());
  }

  // When including Space translation (new E+ 9.6.0)
  {
    forwardTranslator.setExcludeSpaceTranslation(false);
    Workspace workspace = forwardTranslator.translateModel(model);

    EXPECT_EQ(1u, workspace.getObjectsByType(IddObjectType::Zone).size());
    EXPECT_EQ(0u, workspace.getObjectsByType(IddObjectType::ZoneList).size());
    EXPECT_EQ(1u, workspace.getObjectsByType(IddObjectType::Lights).size());
    EXPECT_EQ(2u, workspace.getObjectsByType(IddObjectType::Space).size());
  }
}

TEST_F(EnergyPlusFixture, ForwardTranslator_ThermalZone_OneZone_TwoSpaces_Building_SameSpaceType) {
  Model model;

  Point3dVector points{
    {0, 0, 0},
    {0, 1, 0},
    {1, 1, 0},
    {1, 0, 0},
  };

  SpaceType spaceType(model);
  spaceType.setLightingPowerPerFloorArea(1);

  Building building = model.getUniqueModelObject<Building>();
  building.setSpaceType(spaceType);

  ThermalZone zone(model);

  Space space1(model);
  space1.setThermalZone(zone);
  space1.setSpaceType(spaceType);

  Space space2(model);
  space2.setThermalZone(zone);

  Surface surface1(points, model);
  surface1.setSpace(space1);

  Surface surface2(points, model);
  surface2.setSpace(space2);

  EXPECT_EQ(1, space1.floorArea());
  EXPECT_EQ(1, space2.floorArea());
  EXPECT_EQ(1, space1.lightingPowerPerFloorArea());
  EXPECT_EQ(1, space2.lightingPowerPerFloorArea());

  ForwardTranslator forwardTranslator;

  // When excluding space translation (historical behavior)
  {
    forwardTranslator.setExcludeSpaceTranslation(true);
    Workspace workspace = forwardTranslator.translateModel(model);

    EXPECT_EQ(1u, workspace.getObjectsByType(IddObjectType::Zone).size());
    EXPECT_EQ(1u, workspace.getObjectsByType(IddObjectType::ZoneList).size());
    EXPECT_EQ(1u, workspace.getObjectsByType(IddObjectType::Lights).size());
    EXPECT_EQ(0u, workspace.getObjectsByType(IddObjectType::Space).size());
  }

  // When including Space translation (new E+ 9.6.0)
  {
    forwardTranslator.setExcludeSpaceTranslation(false);
    Workspace workspace = forwardTranslator.translateModel(model);

    EXPECT_EQ(1u, workspace.getObjectsByType(IddObjectType::Zone).size());
    EXPECT_EQ(0u, workspace.getObjectsByType(IddObjectType::ZoneList).size());
    EXPECT_EQ(1u, workspace.getObjectsByType(IddObjectType::Lights).size());
    EXPECT_EQ(2u, workspace.getObjectsByType(IddObjectType::Space).size());
  }
}

TEST_F(EnergyPlusFixture, ForwardTranslator_ThermalZone_TwoZone_TwoSpaces_SameSpaceType) {
  Model model;

  Point3dVector points{
    {0, 0, 0},
    {0, 1, 0},
    {1, 1, 0},
    {1, 0, 0},
  };

  SpaceType spaceType(model);
  spaceType.setLightingPowerPerFloorArea(1);

  ThermalZone zone1(model);
  ThermalZone zone2(model);

  Space space1(model);
  space1.setThermalZone(zone1);
  space1.setSpaceType(spaceType);

  Space space2(model);
  space2.setThermalZone(zone2);
  space2.setSpaceType(spaceType);

  Surface surface1(points, model);
  surface1.setSpace(space1);

  Surface surface2(points, model);
  surface2.setSpace(space2);

  EXPECT_EQ(1, space1.floorArea());
  EXPECT_EQ(1, space2.floorArea());
  EXPECT_EQ(1, space1.lightingPowerPerFloorArea());
  EXPECT_EQ(1, space2.lightingPowerPerFloorArea());

  ForwardTranslator forwardTranslator;

  // When excluding space translation (historical behavior)
  {
    forwardTranslator.setExcludeSpaceTranslation(true);
    Workspace workspace = forwardTranslator.translateModel(model);

    EXPECT_EQ(2u, workspace.getObjectsByType(IddObjectType::Zone).size());
    EXPECT_EQ(1u, workspace.getObjectsByType(IddObjectType::ZoneList).size());
    EXPECT_EQ(1u, workspace.getObjectsByType(IddObjectType::Lights).size());
    EXPECT_EQ(0u, workspace.getObjectsByType(IddObjectType::Space).size());
  }

  // When including Space translation (new E+ 9.6.0)
  {
    forwardTranslator.setExcludeSpaceTranslation(false);
    Workspace workspace = forwardTranslator.translateModel(model);

    EXPECT_EQ(2u, workspace.getObjectsByType(IddObjectType::Zone).size());
    EXPECT_EQ(0u, workspace.getObjectsByType(IddObjectType::ZoneList).size());
    EXPECT_EQ(1u, workspace.getObjectsByType(IddObjectType::Lights).size());
    EXPECT_EQ(2u, workspace.getObjectsByType(IddObjectType::Space).size());
  }
}

TEST_F(EnergyPlusFixture, ForwardTranslator_ThermalZone_TwoZone_TwoSpaces_Building_SameSpaceType) {
  Model model;

  Point3dVector points{
    {0, 0, 0},
    {0, 1, 0},
    {1, 1, 0},
    {1, 0, 0},
  };

  SpaceType spaceType(model);
  spaceType.setLightingPowerPerFloorArea(1);

  Building building = model.getUniqueModelObject<Building>();
  building.setSpaceType(spaceType);

  ThermalZone zone1(model);
  ThermalZone zone2(model);

  Space space1(model);
  space1.setThermalZone(zone1);
  space1.setSpaceType(spaceType);

  Space space2(model);
  space2.setThermalZone(zone2);

  Surface surface1(points, model);
  surface1.setSpace(space1);

  Surface surface2(points, model);
  surface2.setSpace(space2);

  EXPECT_EQ(1, space1.floorArea());
  EXPECT_EQ(1, space2.floorArea());
  EXPECT_EQ(1, space1.lightingPowerPerFloorArea());
  EXPECT_EQ(1, space2.lightingPowerPerFloorArea());

  ForwardTranslator forwardTranslator;

  // When excluding space translation (historical behavior)
  {
    forwardTranslator.setExcludeSpaceTranslation(true);
    Workspace workspace = forwardTranslator.translateModel(model);

    EXPECT_EQ(2u, workspace.getObjectsByType(IddObjectType::Zone).size());
    EXPECT_EQ(1u, workspace.getObjectsByType(IddObjectType::ZoneList).size());
    EXPECT_EQ(1u, workspace.getObjectsByType(IddObjectType::Lights).size());
    EXPECT_EQ(0u, workspace.getObjectsByType(IddObjectType::Space).size());
  }

  // When including Space translation (new E+ 9.6.0)
  {
    forwardTranslator.setExcludeSpaceTranslation(false);
    Workspace workspace = forwardTranslator.translateModel(model);

    EXPECT_EQ(2u, workspace.getObjectsByType(IddObjectType::Zone).size());
    EXPECT_EQ(0u, workspace.getObjectsByType(IddObjectType::ZoneList).size());
    EXPECT_EQ(1u, workspace.getObjectsByType(IddObjectType::Lights).size());
    EXPECT_EQ(2u, workspace.getObjectsByType(IddObjectType::Space).size());
  }
}

TEST_F(EnergyPlusFixture, ForwardTranslator_ThermalZone_OneZone_TwoSpaces_DifferentSpaceType) {
  Model model;

  Point3dVector points{
    {0, 0, 0},
    {0, 1, 0},
    {1, 1, 0},
    {1, 0, 0},
  };

  SpaceType spaceType1(model);
  spaceType1.setLightingPowerPerFloorArea(1);

  SpaceType spaceType2(model);
  spaceType2.setLightingPowerPerFloorArea(2);

  ThermalZone zone(model);

  Space space1(model);
  space1.setThermalZone(zone);
  space1.setSpaceType(spaceType1);

  Space space2(model);
  space2.setThermalZone(zone);
  space2.setSpaceType(spaceType2);

  Surface surface1(points, model);
  surface1.setSpace(space1);

  Surface surface2(points, model);
  surface2.setSpace(space2);

  EXPECT_EQ(1, space1.floorArea());
  EXPECT_EQ(1, space2.floorArea());
  EXPECT_EQ(1, space1.lightingPowerPerFloorArea());
  EXPECT_EQ(2, space2.lightingPowerPerFloorArea());

  ForwardTranslator forwardTranslator;

  // When excluding space translation (historical behavior)
  {
    forwardTranslator.setExcludeSpaceTranslation(true);
    Workspace workspace = forwardTranslator.translateModel(model);

    EXPECT_EQ(1u, workspace.getObjectsByType(IddObjectType::Zone).size());
    EXPECT_EQ(0u, workspace.getObjectsByType(IddObjectType::ZoneList).size());
    EXPECT_EQ(2u, workspace.getObjectsByType(IddObjectType::Lights).size());
    EXPECT_EQ(0u, workspace.getObjectsByType(IddObjectType::Space).size());
  }

  // When including Space translation (new E+ 9.6.0)
  {
    forwardTranslator.setExcludeSpaceTranslation(false);
    Workspace workspace = forwardTranslator.translateModel(model);

    EXPECT_EQ(1u, workspace.getObjectsByType(IddObjectType::Zone).size());
    EXPECT_EQ(0u, workspace.getObjectsByType(IddObjectType::ZoneList).size());
    EXPECT_EQ(2u, workspace.getObjectsByType(IddObjectType::Lights).size());
    EXPECT_EQ(2u, workspace.getObjectsByType(IddObjectType::Space).size());
  }
}

TEST_F(EnergyPlusFixture, ForwardTranslator_ThermalZone_OneZone_TwoSpaces_Building_DifferentSpaceType) {
  Model model;

  Point3dVector points{
    {0, 0, 0},
    {0, 1, 0},
    {1, 1, 0},
    {1, 0, 0},
  };

  SpaceType spaceType1(model);
  spaceType1.setLightingPowerPerFloorArea(1);

  SpaceType spaceType2(model);
  spaceType2.setLightingPowerPerFloorArea(2);

  SpaceType spaceType3(model);
  spaceType3.setLightingPowerPerFloorArea(3);

  Building building = model.getUniqueModelObject<Building>();
  building.setSpaceType(spaceType3);

  ThermalZone zone(model);

  Space space1(model);
  space1.setThermalZone(zone);
  space1.setSpaceType(spaceType1);

  Space space2(model);
  space2.setThermalZone(zone);
  space2.setSpaceType(spaceType2);

  Surface surface1(points, model);
  surface1.setSpace(space1);

  Surface surface2(points, model);
  surface2.setSpace(space2);

  EXPECT_EQ(1, space1.floorArea());
  EXPECT_EQ(1, space2.floorArea());
  EXPECT_EQ(1, space1.lightingPowerPerFloorArea());
  EXPECT_EQ(2, space2.lightingPowerPerFloorArea());

  ForwardTranslator forwardTranslator;

  // When excluding space translation (historical behavior)
  {
    forwardTranslator.setExcludeSpaceTranslation(true);
    Workspace workspace = forwardTranslator.translateModel(model);

    EXPECT_EQ(1u, workspace.getObjectsByType(IddObjectType::Zone).size());
    EXPECT_EQ(0u, workspace.getObjectsByType(IddObjectType::ZoneList).size());
    EXPECT_EQ(2u, workspace.getObjectsByType(IddObjectType::Lights).size());
    EXPECT_EQ(0u, workspace.getObjectsByType(IddObjectType::Space).size());
  }

  // When including Space translation (new E+ 9.6.0)
  {
    forwardTranslator.setExcludeSpaceTranslation(false);
    Workspace workspace = forwardTranslator.translateModel(model);

    EXPECT_EQ(1u, workspace.getObjectsByType(IddObjectType::Zone).size());
    EXPECT_EQ(0u, workspace.getObjectsByType(IddObjectType::ZoneList).size());
    EXPECT_EQ(2u, workspace.getObjectsByType(IddObjectType::Lights).size());
    EXPECT_EQ(2u, workspace.getObjectsByType(IddObjectType::Space).size());
  }
}

TEST_F(EnergyPlusFixture, ForwardTranslator_ThermalZone_OneZone_TwoSpaces_Building_DifferentSpaceType_2) {
  Model model;

  Point3dVector points{
    {0, 0, 0},
    {0, 1, 0},
    {1, 1, 0},
    {1, 0, 0},
  };

  SpaceType spaceType1(model);
  spaceType1.setLightingPowerPerFloorArea(1);

  SpaceType spaceType2(model);
  spaceType2.setLightingPowerPerFloorArea(2);

  Building building = model.getUniqueModelObject<Building>();
  building.setSpaceType(spaceType2);

  ThermalZone zone(model);

  Space space1(model);
  space1.setThermalZone(zone);
  space1.setSpaceType(spaceType1);

  Space space2(model);
  space2.setThermalZone(zone);

  Surface surface1(points, model);
  surface1.setSpace(space1);

  Surface surface2(points, model);
  surface2.setSpace(space2);

  EXPECT_EQ(1, space1.floorArea());
  EXPECT_EQ(1, space2.floorArea());
  EXPECT_EQ(1, space1.lightingPowerPerFloorArea());
  EXPECT_EQ(2, space2.lightingPowerPerFloorArea());

  ForwardTranslator forwardTranslator;

  // When excluding space translation (historical behavior)
  {
    forwardTranslator.setExcludeSpaceTranslation(true);
    Workspace workspace = forwardTranslator.translateModel(model);

    EXPECT_EQ(1u, workspace.getObjectsByType(IddObjectType::Zone).size());
    EXPECT_EQ(0u, workspace.getObjectsByType(IddObjectType::ZoneList).size());
    EXPECT_EQ(2u, workspace.getObjectsByType(IddObjectType::Lights).size());
    EXPECT_EQ(0u, workspace.getObjectsByType(IddObjectType::Space).size());
  }

  // When including Space translation (new E+ 9.6.0)
  {
    forwardTranslator.setExcludeSpaceTranslation(false);
    Workspace workspace = forwardTranslator.translateModel(model);

    EXPECT_EQ(1u, workspace.getObjectsByType(IddObjectType::Zone).size());
    EXPECT_EQ(0u, workspace.getObjectsByType(IddObjectType::ZoneList).size());
    EXPECT_EQ(2u, workspace.getObjectsByType(IddObjectType::Lights).size());
    EXPECT_EQ(2u, workspace.getObjectsByType(IddObjectType::Space).size());
  }
}

TEST_F(EnergyPlusFixture, ForwardTranslator_ThermalZone_OneZone_OneSpace_Building_DifferentSpaceType) {
  Model model;

  Point3dVector points{
    {0, 0, 0},
    {0, 1, 0},
    {1, 1, 0},
    {1, 0, 0},
  };

  SpaceType spaceType1(model);
  spaceType1.setLightingPowerPerFloorArea(1);

  SpaceType spaceType2(model);
  spaceType2.setLightingPowerPerFloorArea(2);

  Building building = model.getUniqueModelObject<Building>();
  building.setSpaceType(spaceType1);

  ThermalZone zone(model);

  Space space(model);
  space.setThermalZone(zone);
  space.setSpaceType(spaceType2);

  Surface surface(points, model);
  surface.setSpace(space);

  EXPECT_EQ(1, space.floorArea());
  EXPECT_EQ(2, space.lightingPowerPerFloorArea());

  ForwardTranslator forwardTranslator;

  // When excluding space translation (historical behavior)
  {
    forwardTranslator.setExcludeSpaceTranslation(true);
    Workspace workspace = forwardTranslator.translateModel(model);

    EXPECT_EQ(1u, workspace.getObjectsByType(IddObjectType::Zone).size());
    EXPECT_EQ(1u, workspace.getObjectsByType(IddObjectType::ZoneList).size());
    EXPECT_EQ(1u, workspace.getObjectsByType(IddObjectType::Lights).size());
    EXPECT_EQ(0u, workspace.getObjectsByType(IddObjectType::Space).size());
  }

  // When including Space translation (new E+ 9.6.0)
  {
    forwardTranslator.setExcludeSpaceTranslation(false);
    Workspace workspace = forwardTranslator.translateModel(model);

    EXPECT_EQ(1u, workspace.getObjectsByType(IddObjectType::Zone).size());
    EXPECT_EQ(0u, workspace.getObjectsByType(IddObjectType::ZoneList).size());
    EXPECT_EQ(1u, workspace.getObjectsByType(IddObjectType::Lights).size());
    EXPECT_EQ(1u, workspace.getObjectsByType(IddObjectType::Space).size());
  }
}

TEST_F(EnergyPlusFixture, ForwardTranslator_ThermalZone_OneZone_OneSpace_Building_SameSpaceType) {
  Model model;

  Point3dVector points{
    {0, 0, 0},
    {0, 1, 0},
    {1, 1, 0},
    {1, 0, 0},
  };

  SpaceType spaceType(model);
  spaceType.setLightingPowerPerFloorArea(1);

  Building building = model.getUniqueModelObject<Building>();
  building.setSpaceType(spaceType);

  ThermalZone zone(model);

  Space space(model);
  space.setThermalZone(zone);
  space.setSpaceType(spaceType);

  Surface surface(points, model);
  surface.setSpace(space);

  EXPECT_EQ(1, space.floorArea());
  EXPECT_EQ(1, space.lightingPowerPerFloorArea());

  ForwardTranslator forwardTranslator;

  // When excluding space translation (historical behavior)
  {
    forwardTranslator.setExcludeSpaceTranslation(true);
    Workspace workspace = forwardTranslator.translateModel(model);

    EXPECT_EQ(1u, workspace.getObjectsByType(IddObjectType::Zone).size());
    EXPECT_EQ(1u, workspace.getObjectsByType(IddObjectType::ZoneList).size());
    EXPECT_EQ(1u, workspace.getObjectsByType(IddObjectType::Lights).size());
    EXPECT_EQ(0u, workspace.getObjectsByType(IddObjectType::Space).size());
  }

  // When including Space translation (new E+ 9.6.0)
  {
    forwardTranslator.setExcludeSpaceTranslation(false);
    Workspace workspace = forwardTranslator.translateModel(model);

    EXPECT_EQ(1u, workspace.getObjectsByType(IddObjectType::Zone).size());
    EXPECT_EQ(0u, workspace.getObjectsByType(IddObjectType::ZoneList).size());
    EXPECT_EQ(1u, workspace.getObjectsByType(IddObjectType::Lights).size());
    EXPECT_EQ(1u, workspace.getObjectsByType(IddObjectType::Space).size());
  }
}

TEST_F(EnergyPlusFixture, ForwardTranslator_ThermalZone_OneZone_OneSpace_Building) {
  Model model;

  Point3dVector points{
    {0, 0, 0},
    {0, 1, 0},
    {1, 1, 0},
    {1, 0, 0},
  };

  SpaceType spaceType(model);
  spaceType.setLightingPowerPerFloorArea(1);

  Building building = model.getUniqueModelObject<Building>();
  building.setSpaceType(spaceType);

  ThermalZone zone(model);

  Space space(model);
  space.setThermalZone(zone);

  Surface surface(points, model);
  surface.setSpace(space);

  EXPECT_EQ(1, space.floorArea());
  EXPECT_EQ(1, space.lightingPowerPerFloorArea());

  ForwardTranslator forwardTranslator;

  // When excluding space translation (historical behavior)
  {
    forwardTranslator.setExcludeSpaceTranslation(true);
    Workspace workspace = forwardTranslator.translateModel(model);

    EXPECT_EQ(1u, workspace.getObjectsByType(IddObjectType::Zone).size());
    EXPECT_EQ(1u, workspace.getObjectsByType(IddObjectType::ZoneList).size());
    EXPECT_EQ(1u, workspace.getObjectsByType(IddObjectType::Lights).size());
    EXPECT_EQ(0u, workspace.getObjectsByType(IddObjectType::Space).size());
  }

  // When including Space translation (new E+ 9.6.0)
  {
    forwardTranslator.setExcludeSpaceTranslation(false);
    Workspace workspace = forwardTranslator.translateModel(model);

    EXPECT_EQ(1u, workspace.getObjectsByType(IddObjectType::Zone).size());
    EXPECT_EQ(0u, workspace.getObjectsByType(IddObjectType::ZoneList).size());
    EXPECT_EQ(1u, workspace.getObjectsByType(IddObjectType::Lights).size());
    EXPECT_EQ(1u, workspace.getObjectsByType(IddObjectType::Space).size());
  }
}

TEST_F(EnergyPlusFixture, ForwardTranslator_ThermalZone_3Zone_2Spaces) {
  Model model;

  Point3dVector points{
    {0, 0, 0},
    {0, 1, 0},
    {1, 1, 0},
    {1, 0, 0},
  };

  SpaceType spaceType1(model);
  spaceType1.setLightingPowerPerFloorArea(1);

  SpaceType spaceType2(model);
  spaceType2.setLightingPowerPerFloorArea(2);

  SpaceType spaceType3(model);
  spaceType3.setLightingPowerPerFloorArea(3);

  Building building = model.getUniqueModelObject<Building>();
  building.setSpaceType(spaceType1);

  ThermalZone zone1(model);
  ThermalZone zone2(model);

  Space space1(model);
  space1.setThermalZone(zone1);

  Surface surface1(points, model);
  surface1.setSpace(space1);

  Space space2(model);
  space2.setThermalZone(zone2);
  space2.setSpaceType(spaceType2);

  Surface surface2(points, model);
  surface2.setSpace(space2);

  Space space3(model);
  space3.setThermalZone(zone2);
  space3.setSpaceType(spaceType3);

  Surface surface3(points, model);
  surface3.setSpace(space3);

  EXPECT_EQ(1, space1.floorArea());
  EXPECT_EQ(1, space1.lightingPowerPerFloorArea());
  EXPECT_EQ(1, space2.floorArea());
  EXPECT_EQ(2, space2.lightingPowerPerFloorArea());
  EXPECT_EQ(1, space3.floorArea());
  EXPECT_EQ(3, space3.lightingPowerPerFloorArea());

  ForwardTranslator forwardTranslator;

  // When excluding space translation (historical behavior)
  {
    forwardTranslator.setExcludeSpaceTranslation(true);
    Workspace workspace = forwardTranslator.translateModel(model);

    EXPECT_EQ(2u, workspace.getObjectsByType(IddObjectType::Zone).size());
    EXPECT_EQ(1u, workspace.getObjectsByType(IddObjectType::ZoneList).size());
    EXPECT_EQ(3u, workspace.getObjectsByType(IddObjectType::Lights).size());
    EXPECT_EQ(0u, workspace.getObjectsByType(IddObjectType::Space).size());
  }

  // When including Space translation (new E+ 9.6.0)
  {
    forwardTranslator.setExcludeSpaceTranslation(false);
    Workspace workspace = forwardTranslator.translateModel(model);

    EXPECT_EQ(2u, workspace.getObjectsByType(IddObjectType::Zone).size());
    EXPECT_EQ(0u, workspace.getObjectsByType(IddObjectType::ZoneList).size());
    EXPECT_EQ(3u, workspace.getObjectsByType(IddObjectType::Lights).size());
    EXPECT_EQ(3u, workspace.getObjectsByType(IddObjectType::Space).size());
  }
}

TEST_F(EnergyPlusFixture, ForwardTranslator_ThermalZone_1Zone_2Spaces_HardSchedules) {
  Model model;

  Point3dVector points{
    {0, 0, 0},
    {0, 1, 0},
    {1, 1, 0},
    {1, 0, 0},
  };

  LightsDefinition lightsDef1(model);
  lightsDef1.setLightingLevel(1.0);

  LightsDefinition lightsDef2(model);
  lightsDef2.setLightingLevel(2.0);

  LightsDefinition lightsDef3(model);
  lightsDef3.setLightingLevel(3.0);

  LightsDefinition lightsDef4(model);
  lightsDef4.setLightingLevel(4.0);

  ScheduleConstant sch1(model);
  ScheduleConstant sch2(model);
  ScheduleConstant sch3(model);
  ScheduleConstant sch4(model);

  SpaceType spaceType1(model);
  Lights lights1(lightsDef1);
  lights1.setSpaceType(spaceType1);
  lights1.setSchedule(sch1);

  SpaceType spaceType2(model);
  Lights lights2(lightsDef2);
  lights2.setSpaceType(spaceType2);
  lights2.setSchedule(sch2);

  ThermalZone zone(model);

  Space space1(model);
  space1.setThermalZone(zone);
  space1.setSpaceType(spaceType1);

  Lights lights3(lightsDef3);
  lights3.setSpace(space1);
  lights3.setSchedule(sch3);

  Surface surface1(points, model);
  surface1.setSpace(space1);

  Space space2(model);
  space2.setThermalZone(zone);
  space2.setSpaceType(spaceType2);

  Lights lights4(lightsDef4);
  lights4.setSpace(space2);
  lights4.setSchedule(sch4);

  Surface surface2(points, model);
  surface2.setSpace(space2);

  EXPECT_EQ(4u, model.getConcreteModelObjects<Lights>().size());

  for (const Lights& light : model.getConcreteModelObjects<Lights>()) {
    EXPECT_TRUE(light.schedule());
  }

  ForwardTranslator forwardTranslator;

  // When excluding space translation (historical behavior)
  {
    forwardTranslator.setExcludeSpaceTranslation(true);
    Workspace workspace = forwardTranslator.translateModel(model);

    EXPECT_EQ(1u, workspace.getObjectsByType(IddObjectType::Zone).size());
    EXPECT_EQ(0u, workspace.getObjectsByType(IddObjectType::ZoneList).size());
    EXPECT_EQ(4u, workspace.getObjectsByType(IddObjectType::Lights).size());
    EXPECT_EQ(0u, workspace.getObjectsByType(IddObjectType::Space).size());

    for (const WorkspaceObject& workspaceObject : workspace.getObjectsByType(IddObjectType::Lights)) {
      EXPECT_TRUE(workspaceObject.getTarget(LightsFields::ScheduleName));
    }
  }

  // When including Space translation (new E+ 9.6.0)
  {
    forwardTranslator.setExcludeSpaceTranslation(false);
    Workspace workspace = forwardTranslator.translateModel(model);

    EXPECT_EQ(1u, workspace.getObjectsByType(IddObjectType::Zone).size());
    EXPECT_EQ(0u, workspace.getObjectsByType(IddObjectType::ZoneList).size());
    EXPECT_EQ(4u, workspace.getObjectsByType(IddObjectType::Lights).size());
    EXPECT_EQ(2u, workspace.getObjectsByType(IddObjectType::Space).size());

    for (const WorkspaceObject& workspaceObject : workspace.getObjectsByType(IddObjectType::Lights)) {
      EXPECT_TRUE(workspaceObject.getTarget(LightsFields::ScheduleName));
    }
  }
}

TEST_F(EnergyPlusFixture, ForwardTranslator_ThermalZone_1Zone_2Spaces_InheritSchedules) {
  Model model;

  Point3dVector points{
    {0, 0, 0},
    {0, 1, 0},
    {1, 1, 0},
    {1, 0, 0},
  };

  LightsDefinition lightsDef1(model);
  lightsDef1.setLightingLevel(1.0);

  LightsDefinition lightsDef2(model);
  lightsDef2.setLightingLevel(2.0);

  LightsDefinition lightsDef3(model);
  lightsDef3.setLightingLevel(3.0);

  LightsDefinition lightsDef4(model);
  lightsDef4.setLightingLevel(4.0);

  ScheduleConstant sch1(model);
  DefaultScheduleSet scheduleSet(model);
  scheduleSet.setLightingSchedule(sch1);

  SpaceType spaceType1(model);
  spaceType1.setDefaultScheduleSet(scheduleSet);
  Lights lights1(lightsDef1);
  lights1.setSpaceType(spaceType1);

  SpaceType spaceType2(model);
  spaceType2.setDefaultScheduleSet(scheduleSet);
  Lights lights2(lightsDef2);
  lights2.setSpaceType(spaceType2);

  ThermalZone zone(model);

  Space space1(model);
  space1.setThermalZone(zone);
  space1.setSpaceType(spaceType1);

  Lights lights3(lightsDef3);
  lights3.setSpace(space1);

  Surface surface1(points, model);
  surface1.setSpace(space1);

  Space space2(model);
  space2.setThermalZone(zone);
  space2.setSpaceType(spaceType2);

  Lights lights4(lightsDef4);
  lights4.setSpace(space2);

  Surface surface2(points, model);
  surface2.setSpace(space2);

  EXPECT_EQ(4u, model.getConcreteModelObjects<Lights>().size());

  for (const Lights& light : model.getConcreteModelObjects<Lights>()) {
    EXPECT_TRUE(light.schedule());
  }

  ForwardTranslator forwardTranslator;

  // When excluding space translation (historical behavior)
  {
    forwardTranslator.setExcludeSpaceTranslation(true);
    Workspace workspace = forwardTranslator.translateModel(model);

    EXPECT_EQ(1u, workspace.getObjectsByType(IddObjectType::Zone).size());
    EXPECT_EQ(0u, workspace.getObjectsByType(IddObjectType::ZoneList).size());
    EXPECT_EQ(4u, workspace.getObjectsByType(IddObjectType::Lights).size());
    EXPECT_EQ(0u, workspace.getObjectsByType(IddObjectType::Space).size());

    for (const WorkspaceObject& workspaceObject : workspace.getObjectsByType(IddObjectType::Lights)) {
      EXPECT_TRUE(workspaceObject.getTarget(LightsFields::ScheduleName)) << workspaceObject;
    }
  }

  // When including Space translation (new E+ 9.6.0)
  {
    forwardTranslator.setExcludeSpaceTranslation(false);
    Workspace workspace = forwardTranslator.translateModel(model);

    EXPECT_EQ(1u, workspace.getObjectsByType(IddObjectType::Zone).size());
    EXPECT_EQ(0u, workspace.getObjectsByType(IddObjectType::ZoneList).size());
    EXPECT_EQ(4u, workspace.getObjectsByType(IddObjectType::Lights).size());
    EXPECT_EQ(2u, workspace.getObjectsByType(IddObjectType::Space).size());

    for (const WorkspaceObject& workspaceObject : workspace.getObjectsByType(IddObjectType::Lights)) {
      EXPECT_TRUE(workspaceObject.getTarget(LightsFields::ScheduleName)) << workspaceObject;
    }
  }
}

/* Tests that the illuminance setpoint is set even if it is defaulted (ref #2849) */

TEST_F(EnergyPlusFixture, ForwardTranslator_ThermalZone_Daylighting) {
  Model m;

  ThermalZone z(m);
  Space space1(m);
  space1.setThermalZone(z);

  Point3dVector points{
    {0, 0, 0},
    {0, 1, 0},
    {1, 1, 0},
    {1, 0, 0},
  };

  Surface surface1(points, m);
  surface1.setSpace(space1);

  LightsDefinition lightsDef1(m);
  lightsDef1.setLightingLevel(1.0);

  Lights lights1(lightsDef1);
  lights1.setSpace(space1);

  DaylightingControl d_pri(m);
  ASSERT_TRUE(d_pri.isIlluminanceSetpointDefaulted());
  z.setPrimaryDaylightingControl(d_pri);

  DaylightingControl d_sec(m);
  ASSERT_TRUE(d_sec.isIlluminanceSetpointDefaulted());
  z.setSecondaryDaylightingControl(d_sec);

  ForwardTranslator forwardTranslator;

  // KSB: This function will be used to run translation tests in both,
  // "historical" and current space translation modes. I don't expect
  // to see differences in how daylighting controls are translated,
  // however this assumption should be vetted.
  const auto translateAndAssert = [&]() {
    Workspace workspace = forwardTranslator.translateModel(m);

    WorkspaceObjectVector idfObjs = workspace.getObjectsByType(IddObjectType::Daylighting_Controls);
    ASSERT_EQ(1u, idfObjs.size());
    WorkspaceObject idf_d(idfObjs[0]);

    // Should have two extensible groups
    boost::optional<double> sp;

    ASSERT_EQ(2u, idf_d.extensibleGroups().size());

    // Check that there is a value for primary, and that it's right
    auto w_eg = idf_d.extensibleGroups()[0].cast<WorkspaceExtensibleGroup>();
    sp = w_eg.getDouble(Daylighting_ControlsExtensibleFields::IlluminanceSetpointatReferencePoint);
    ASSERT_TRUE(sp);
    ASSERT_EQ(d_pri.illuminanceSetpoint(), sp.get());

    // same for secondary
    w_eg = idf_d.extensibleGroups()[1].cast<WorkspaceExtensibleGroup>();
    sp = w_eg.getDouble(Daylighting_ControlsExtensibleFields::IlluminanceSetpointatReferencePoint);
    ASSERT_TRUE(sp);
    ASSERT_EQ(d_sec.illuminanceSetpoint(), sp.get());
  };

  // When excluding space translation (historical behavior)
  {
    forwardTranslator.setExcludeSpaceTranslation(true);
    translateAndAssert();
  }

  // When including Space translation (new E+ 9.6.0)
  {
    forwardTranslator.setExcludeSpaceTranslation(false);
    translateAndAssert();
  }
}

TEST_F(EnergyPlusFixture, ForwardTranslator_LoadDistributionScheme) {
  // Create a model with a space and thermalZone
  Model model;

  Point3dVector points{
    {0, 0, 0},
    {0, 1, 0},
    {1, 1, 0},
    {1, 0, 0},
  };

  SpaceType spaceType(model);
  spaceType.setLightingPowerPerFloorArea(1);

  ThermalZone zone(model);

  Space space1(model);
  space1.setThermalZone(zone);
  space1.setSpaceType(spaceType);

  Surface surface1(points, model);
  surface1.setSpace(space1);

  // For the ZoneHVAC:EquipmentList to be created, we need to add at least one equipment
  FanZoneExhaust fan(model);
  fan.addToThermalZone(zone);

  // The field we're actually testing. Set it to the non-default value (which is "SequentialLoad")
  zone.setLoadDistributionScheme("UniformLoad");

  ForwardTranslator forwardTranslator;

  // KSB: Again, we expect no difference between translating
  // using the "historical" method and the current space translation method,
  // however this function is used to check both
  const auto translateAndAssert = [&]() {
    Workspace workspace = forwardTranslator.translateModel(model);

    // We verify that we end up with the right field in the IDF
    EXPECT_EQ(1u, workspace.getObjectsByType(IddObjectType::Zone).size());
    WorkspaceObjectVector idfObjs = workspace.getObjectsByType(IddObjectType::ZoneHVAC_EquipmentList);
    EXPECT_EQ(1u, idfObjs.size());
    WorkspaceObject idf_eqlist(idfObjs[0]);

    EXPECT_EQ("UniformLoad", idf_eqlist.getString(ZoneHVAC_EquipmentListFields::LoadDistributionScheme).get());
  };

  // When excluding space translation (historical behavior)
  {
    forwardTranslator.setExcludeSpaceTranslation(true);
    translateAndAssert();
  }

  // When including Space translation (new E+ 9.6.0)
  {
    forwardTranslator.setExcludeSpaceTranslation(false);
    translateAndAssert();
  }
}

TEST_F(EnergyPlusFixture, ReverseTranslator_ZoneControlThermostat) {
  ReverseTranslator rt;

  Workspace w(StrictnessLevel::Minimal, IddFileType::EnergyPlus);

  OptionalWorkspaceObject _i_zone = w.addObject(IdfObject(IddObjectType::Zone));
  ASSERT_TRUE(_i_zone);
  EXPECT_TRUE(_i_zone->setName("Zone1"));

  auto _i_tstat = w.addObject(IdfObject(IddObjectType::ThermostatSetpoint_DualSetpoint));
  ASSERT_TRUE(_i_tstat);
  EXPECT_TRUE(_i_tstat->setName("Thermostat Setpoint Dual Setpoint"));

  auto _i_heating_sch = w.addObject(IdfObject(openstudio::IddObjectType::Schedule_Constant));
  ASSERT_TRUE(_i_heating_sch);
  EXPECT_TRUE(_i_heating_sch->setName("Heating Schedule"));
  EXPECT_TRUE(_i_heating_sch->setDouble(Schedule_ConstantFields::HourlyValue, 19.0));
  EXPECT_TRUE(_i_tstat->setPointer(ThermostatSetpoint_DualSetpointFields::HeatingSetpointTemperatureScheduleName, _i_heating_sch->handle()));

  auto _i_cooling_sch = w.addObject(IdfObject(openstudio::IddObjectType::Schedule_Constant));
  ASSERT_TRUE(_i_cooling_sch);
  EXPECT_TRUE(_i_cooling_sch->setName("Cooling Schedule"));
  EXPECT_TRUE(_i_cooling_sch->setDouble(Schedule_ConstantFields::HourlyValue, 26.0));
  EXPECT_TRUE(_i_tstat->setPointer(ThermostatSetpoint_DualSetpointFields::CoolingSetpointTemperatureScheduleName, _i_cooling_sch->handle()));

  auto _i_zc = w.addObject(IdfObject(IddObjectType::ZoneControl_Thermostat));
  ASSERT_TRUE(_i_zc);
  EXPECT_TRUE(_i_zc->setName("Zone Control Thermostat"));
  EXPECT_TRUE(_i_zc->setPointer(ZoneControl_ThermostatFields::ZoneorZoneListName, _i_zone->handle()));

  auto _i_control_sch = w.addObject(IdfObject(openstudio::IddObjectType::Schedule_Constant));
  ASSERT_TRUE(_i_control_sch);
  EXPECT_TRUE(_i_control_sch->setName("Control Type Schedule"));
  EXPECT_TRUE(_i_control_sch->setDouble(Schedule_ConstantFields::HourlyValue, 4.0));

  // Make sure like in #4096, we end up with blank "Control Object Type / Name" groups
  EXPECT_TRUE(_i_zc->setString(ZoneControl_ThermostatFields::Control1ObjectType, _i_tstat->iddObject().name()));
  EXPECT_TRUE(_i_zc->setString(ZoneControl_ThermostatFields::Control1Name, _i_tstat->name().get()));
  EXPECT_TRUE(_i_zc->setDouble(ZoneControl_ThermostatFields::TemperatureDifferenceBetweenCutoutAndSetpoint, 0.0));

  ASSERT_NO_THROW(rt.translateWorkspace(w));
  Model m = rt.translateWorkspace(w);
}
