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

#include <utilities/idd/Lights_FieldEnums.hxx>
#include <utilities/idd/Zone_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include <resources.hxx>

#include <sstream>

using namespace openstudio::energyplus;
using namespace openstudio::model;
using namespace openstudio;

TEST_F(EnergyPlusFixture,ForwardTranslator_ThermalZone_OneZone_TwoSpaces_SameSpaceType)
{
  Model model;

  Point3dVector points;
  points.push_back(Point3d(0, 0, 0));
  points.push_back(Point3d(0, 1, 0));
  points.push_back(Point3d(1, 1, 0));
  points.push_back(Point3d(1, 0, 0));

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

  ForwardTranslator trans;
  Workspace workspace = trans.translateModel(model);

  EXPECT_EQ(1u, workspace.getObjectsByType(IddObjectType::Zone).size());
  EXPECT_EQ(1u, workspace.getObjectsByType(IddObjectType::ZoneList).size());
  EXPECT_EQ(1u, workspace.getObjectsByType(IddObjectType::Lights).size());
}

TEST_F(EnergyPlusFixture,ForwardTranslator_ThermalZone_OneZone_TwoSpaces_Building_SameSpaceType)
{
  Model model;

  Point3dVector points;
  points.push_back(Point3d(0, 0, 0));
  points.push_back(Point3d(0, 1, 0));
  points.push_back(Point3d(1, 1, 0));
  points.push_back(Point3d(1, 0, 0));

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

  ForwardTranslator trans;
  Workspace workspace = trans.translateModel(model);

  EXPECT_EQ(1u, workspace.getObjectsByType(IddObjectType::Zone).size());
  EXPECT_EQ(1u, workspace.getObjectsByType(IddObjectType::ZoneList).size());
  EXPECT_EQ(1u, workspace.getObjectsByType(IddObjectType::Lights).size());
}

TEST_F(EnergyPlusFixture,ForwardTranslator_ThermalZone_TwoZone_TwoSpaces_SameSpaceType)
{
  Model model;

  Point3dVector points;
  points.push_back(Point3d(0, 0, 0));
  points.push_back(Point3d(0, 1, 0));
  points.push_back(Point3d(1, 1, 0));
  points.push_back(Point3d(1, 0, 0));

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

  ForwardTranslator trans;
  Workspace workspace = trans.translateModel(model);

  EXPECT_EQ(2u, workspace.getObjectsByType(IddObjectType::Zone).size());
  EXPECT_EQ(1u, workspace.getObjectsByType(IddObjectType::ZoneList).size());
  EXPECT_EQ(1u, workspace.getObjectsByType(IddObjectType::Lights).size());
}

TEST_F(EnergyPlusFixture,ForwardTranslator_ThermalZone_TwoZone_TwoSpaces_Building_SameSpaceType)
{
  Model model;

  Point3dVector points;
  points.push_back(Point3d(0, 0, 0));
  points.push_back(Point3d(0, 1, 0));
  points.push_back(Point3d(1, 1, 0));
  points.push_back(Point3d(1, 0, 0));

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

  ForwardTranslator trans;
  Workspace workspace = trans.translateModel(model);

  EXPECT_EQ(2u, workspace.getObjectsByType(IddObjectType::Zone).size());
  EXPECT_EQ(1u, workspace.getObjectsByType(IddObjectType::ZoneList).size());
  EXPECT_EQ(1u, workspace.getObjectsByType(IddObjectType::Lights).size());
}

TEST_F(EnergyPlusFixture,ForwardTranslator_ThermalZone_OneZone_TwoSpaces_DifferentSpaceType)
{
  Model model;

  Point3dVector points;
  points.push_back(Point3d(0, 0, 0));
  points.push_back(Point3d(0, 1, 0));
  points.push_back(Point3d(1, 1, 0));
  points.push_back(Point3d(1, 0, 0));

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

  ForwardTranslator trans;
  Workspace workspace = trans.translateModel(model);

  EXPECT_EQ(1u, workspace.getObjectsByType(IddObjectType::Zone).size());
  EXPECT_EQ(0u, workspace.getObjectsByType(IddObjectType::ZoneList).size());
  EXPECT_EQ(2u, workspace.getObjectsByType(IddObjectType::Lights).size());
}

TEST_F(EnergyPlusFixture,ForwardTranslator_ThermalZone_OneZone_TwoSpaces_Building_DifferentSpaceType)
{
  Model model;

  Point3dVector points;
  points.push_back(Point3d(0, 0, 0));
  points.push_back(Point3d(0, 1, 0));
  points.push_back(Point3d(1, 1, 0));
  points.push_back(Point3d(1, 0, 0));

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

  ForwardTranslator trans;
  Workspace workspace = trans.translateModel(model);

  EXPECT_EQ(1u, workspace.getObjectsByType(IddObjectType::Zone).size());
  EXPECT_EQ(0u, workspace.getObjectsByType(IddObjectType::ZoneList).size());
  EXPECT_EQ(2u, workspace.getObjectsByType(IddObjectType::Lights).size());
}

TEST_F(EnergyPlusFixture,ForwardTranslator_ThermalZone_OneZone_TwoSpaces_Building_DifferentSpaceType_2)
{
  Model model;

  Point3dVector points;
  points.push_back(Point3d(0, 0, 0));
  points.push_back(Point3d(0, 1, 0));
  points.push_back(Point3d(1, 1, 0));
  points.push_back(Point3d(1, 0, 0));

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

  ForwardTranslator trans;
  Workspace workspace = trans.translateModel(model);

  EXPECT_EQ(1u, workspace.getObjectsByType(IddObjectType::Zone).size());
  EXPECT_EQ(0u, workspace.getObjectsByType(IddObjectType::ZoneList).size());
  EXPECT_EQ(2u, workspace.getObjectsByType(IddObjectType::Lights).size());
}

TEST_F(EnergyPlusFixture,ForwardTranslator_ThermalZone_OneZone_OneSpace_Building_DifferentSpaceType)
{
  Model model;

  Point3dVector points;
  points.push_back(Point3d(0, 0, 0));
  points.push_back(Point3d(0, 1, 0));
  points.push_back(Point3d(1, 1, 0));
  points.push_back(Point3d(1, 0, 0));

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

  ForwardTranslator trans;
  Workspace workspace = trans.translateModel(model);

  EXPECT_EQ(1u, workspace.getObjectsByType(IddObjectType::Zone).size());
  EXPECT_EQ(1u, workspace.getObjectsByType(IddObjectType::ZoneList).size());
  EXPECT_EQ(1u, workspace.getObjectsByType(IddObjectType::Lights).size());
}


TEST_F(EnergyPlusFixture,ForwardTranslator_ThermalZone_OneZone_OneSpace_Building_SameSpaceType)
{
  Model model;

  Point3dVector points;
  points.push_back(Point3d(0, 0, 0));
  points.push_back(Point3d(0, 1, 0));
  points.push_back(Point3d(1, 1, 0));
  points.push_back(Point3d(1, 0, 0));

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

  ForwardTranslator trans;
  Workspace workspace = trans.translateModel(model);

  EXPECT_EQ(1u, workspace.getObjectsByType(IddObjectType::Zone).size());
  EXPECT_EQ(1u, workspace.getObjectsByType(IddObjectType::ZoneList).size());
  EXPECT_EQ(1u, workspace.getObjectsByType(IddObjectType::Lights).size());
}

TEST_F(EnergyPlusFixture,ForwardTranslator_ThermalZone_OneZone_OneSpace_Building)
{
  Model model;

  Point3dVector points;
  points.push_back(Point3d(0, 0, 0));
  points.push_back(Point3d(0, 1, 0));
  points.push_back(Point3d(1, 1, 0));
  points.push_back(Point3d(1, 0, 0));

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

  ForwardTranslator trans;
  Workspace workspace = trans.translateModel(model);

  EXPECT_EQ(1u, workspace.getObjectsByType(IddObjectType::Zone).size());
  EXPECT_EQ(1u, workspace.getObjectsByType(IddObjectType::ZoneList).size());
  EXPECT_EQ(1u, workspace.getObjectsByType(IddObjectType::Lights).size());
}

TEST_F(EnergyPlusFixture,ForwardTranslator_ThermalZone_3Zone_2Spaces)
{
  Model model;

  Point3dVector points;
  points.push_back(Point3d(0, 0, 0));
  points.push_back(Point3d(0, 1, 0));
  points.push_back(Point3d(1, 1, 0));
  points.push_back(Point3d(1, 0, 0));

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

  ForwardTranslator trans;
  Workspace workspace = trans.translateModel(model);

  EXPECT_EQ(2u, workspace.getObjectsByType(IddObjectType::Zone).size());
  EXPECT_EQ(1u, workspace.getObjectsByType(IddObjectType::ZoneList).size());
  EXPECT_EQ(3u, workspace.getObjectsByType(IddObjectType::Lights).size());
}


TEST_F(EnergyPlusFixture,ForwardTranslator_ThermalZone_1Zone_2Spaces_HardSchedules)
{
  Model model;

  Point3dVector points;
  points.push_back(Point3d(0, 0, 0));
  points.push_back(Point3d(0, 1, 0));
  points.push_back(Point3d(1, 1, 0));
  points.push_back(Point3d(1, 0, 0));

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

  EXPECT_EQ(4u, model.getModelObjects<Lights>().size());

  for (const Lights& light : model.getModelObjects<Lights>()){
    EXPECT_TRUE(light.schedule());
  }

  ForwardTranslator trans;
  Workspace workspace = trans.translateModel(model);

  EXPECT_EQ(1u, workspace.getObjectsByType(IddObjectType::Zone).size());
  EXPECT_EQ(0u, workspace.getObjectsByType(IddObjectType::ZoneList).size());
  EXPECT_EQ(4u, workspace.getObjectsByType(IddObjectType::Lights).size());

  for (const WorkspaceObject& workspaceObject : workspace.getObjectsByType(IddObjectType::Lights)){
    EXPECT_TRUE(workspaceObject.getTarget(LightsFields::ScheduleName));
  }
}

TEST_F(EnergyPlusFixture,ForwardTranslator_ThermalZone_1Zone_2Spaces_InheritSchedules)
{
  Model model;

  Point3dVector points;
  points.push_back(Point3d(0, 0, 0));
  points.push_back(Point3d(0, 1, 0));
  points.push_back(Point3d(1, 1, 0));
  points.push_back(Point3d(1, 0, 0));

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

  EXPECT_EQ(4u, model.getModelObjects<Lights>().size());

  for (const Lights& light : model.getModelObjects<Lights>()){
    EXPECT_TRUE(light.schedule());
  }

  ForwardTranslator trans;
  Workspace workspace = trans.translateModel(model);

  EXPECT_EQ(1u, workspace.getObjectsByType(IddObjectType::Zone).size());
  EXPECT_EQ(0u, workspace.getObjectsByType(IddObjectType::ZoneList).size());
  EXPECT_EQ(4u, workspace.getObjectsByType(IddObjectType::Lights).size());

  for (const WorkspaceObject& workspaceObject : workspace.getObjectsByType(IddObjectType::Lights)){
    EXPECT_TRUE(workspaceObject.getTarget(LightsFields::ScheduleName)) << workspaceObject;
  }
}
