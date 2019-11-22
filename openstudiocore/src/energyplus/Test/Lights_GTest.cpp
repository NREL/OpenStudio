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
#include <utilities/idd/ComponentCost_LineItem_FieldEnums.hxx>
#include <utilities/idd/LifeCycleCost_NonrecurringCost_FieldEnums.hxx>
#include <utilities/idd/LifeCycleCost_RecurringCosts_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include <resources.hxx>

#include <sstream>

using namespace openstudio::energyplus;
using namespace openstudio::model;
using namespace openstudio;


TEST_F(EnergyPlusFixture,ForwardTranslator_LightsDefinition)
{
  Model model;
  LightsDefinition definition(model);

  ForwardTranslator forwardTranslator;
  Workspace workspace = forwardTranslator.translateModel(model);

  std::string errors;
  for (const LogMessage& error : forwardTranslator.errors()){
    errors += error.logMessage() + "\n";
  }
  EXPECT_EQ(0u, forwardTranslator.errors().size()) << errors;

  std::string warnings;
  for (const LogMessage& warning : forwardTranslator.warnings()){
    warnings += warning.logMessage() + "\n";
  }
  EXPECT_EQ(0u, forwardTranslator.warnings().size()) << warnings;

  EXPECT_EQ(0u, workspace.getObjectsByType(IddObjectType::Lights).size());
}

TEST_F(EnergyPlusFixture,ForwardTranslator_Lights_NoSpace)
{
  Model model;
  LightsDefinition definition(model);
  Lights lights(definition);

  ForwardTranslator forwardTranslator;
  Workspace workspace = forwardTranslator.translateModel(model);

  EXPECT_EQ(0u, workspace.getObjectsByType(IddObjectType::Lights).size());
}

TEST_F(EnergyPlusFixture,ForwardTranslator_Lights_Space)
{
  Model model;

  ThermalZone thermalZone(model);
  Space space(model);
  space.setThermalZone(thermalZone);

  LightsDefinition definition(model);
  Lights lights(definition);
  lights.setSpace(space);

  ForwardTranslator forwardTranslator;
  Workspace workspace = forwardTranslator.translateModel(model);

  ASSERT_EQ(1u, workspace.getObjectsByType(IddObjectType::Lights).size());
  ASSERT_EQ(1u, workspace.getObjectsByType(IddObjectType::Zone).size());

  WorkspaceObject lightsObject = workspace.getObjectsByType(IddObjectType::Lights)[0];
  WorkspaceObject zoneObject = workspace.getObjectsByType(IddObjectType::Zone)[0];

  ASSERT_TRUE(lightsObject.getTarget(LightsFields::ZoneorZoneListName));
  EXPECT_EQ(zoneObject.handle(), lightsObject.getTarget(LightsFields::ZoneorZoneListName)->handle());
}

TEST_F(EnergyPlusFixture,ForwardTranslator_Lights_SpaceType)
{
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
  Workspace workspace = forwardTranslator.translateModel(model);

  ASSERT_EQ(1u, workspace.getObjectsByType(IddObjectType::Lights).size());
  ASSERT_EQ(1u, workspace.getObjectsByType(IddObjectType::Zone).size());
  ASSERT_EQ(1u, workspace.getObjectsByType(IddObjectType::ZoneList).size());

  WorkspaceObject lightsObject = workspace.getObjectsByType(IddObjectType::Lights)[0];
  WorkspaceObject zoneObject = workspace.getObjectsByType(IddObjectType::Zone)[0];
  WorkspaceObject zoneListObject = workspace.getObjectsByType(IddObjectType::ZoneList)[0];

  ASSERT_TRUE(lightsObject.getTarget(LightsFields::ZoneorZoneListName));
  EXPECT_EQ(zoneListObject.handle(), lightsObject.getTarget(LightsFields::ZoneorZoneListName)->handle());

  ASSERT_EQ(1u, zoneListObject.extensibleGroups().size());
  ASSERT_TRUE(zoneListObject.extensibleGroups()[0].cast<WorkspaceExtensibleGroup>().getTarget(0));
  EXPECT_EQ(zoneObject.handle(), zoneListObject.extensibleGroups()[0].cast<WorkspaceExtensibleGroup>().getTarget(0)->handle());
}

TEST_F(EnergyPlusFixture,ForwardTranslator_Lights_OneSpaceType_OneThermalZone)
{
  Model model;

  // 2 W/m^2
  SpaceType spaceType(model);
  LightsDefinition definition(model);
  definition.setWattsperSpaceFloorArea(2.0);
  Lights lights(definition);
  lights.setSpaceType(spaceType);

  ThermalZone thermalZone(model);

  // 100 m^2
  std::vector<Point3d> points;
  points.push_back(Point3d(0,10,0));
  points.push_back(Point3d(10,10,0));
  points.push_back(Point3d(10,0,0));
  points.push_back(Point3d(0,0,0));

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
  Workspace workspace = forwardTranslator.translateModel(model);

  ASSERT_EQ(1u, workspace.getObjectsByType(IddObjectType::Lights).size());
  ASSERT_EQ(1u, workspace.getObjectsByType(IddObjectType::Zone).size());
  ASSERT_EQ(1u, workspace.getObjectsByType(IddObjectType::ZoneList).size());

  WorkspaceObject lightsObject = workspace.getObjectsByType(IddObjectType::Lights)[0];
  WorkspaceObject zoneObject = workspace.getObjectsByType(IddObjectType::Zone)[0];
  WorkspaceObject zoneListObject = workspace.getObjectsByType(IddObjectType::ZoneList)[0];

  ASSERT_TRUE(lightsObject.getString(LightsFields::DesignLevelCalculationMethod, false));
  EXPECT_EQ("Watts/Area", lightsObject.getString(LightsFields::DesignLevelCalculationMethod, false).get());

  ASSERT_TRUE(lightsObject.getDouble(LightsFields::WattsperZoneFloorArea, false));
  EXPECT_EQ(2.0, lightsObject.getDouble(LightsFields::WattsperZoneFloorArea, false).get());

  ASSERT_TRUE(lightsObject.getTarget(LightsFields::ZoneorZoneListName));
  EXPECT_EQ(zoneListObject.handle(), lightsObject.getTarget(LightsFields::ZoneorZoneListName)->handle());

  ASSERT_EQ(1u, zoneListObject.extensibleGroups().size());
  ASSERT_TRUE(zoneListObject.extensibleGroups()[0].cast<WorkspaceExtensibleGroup>().getTarget(0));
  EXPECT_EQ(zoneObject.handle(), zoneListObject.extensibleGroups()[0].cast<WorkspaceExtensibleGroup>().getTarget(0)->handle());
}

TEST_F(EnergyPlusFixture,ForwardTranslator_Lights_TwoSpaceTypes_OneThermalZone)
{
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
  std::vector<Point3d> points;
  points.push_back(Point3d(0,10,0));
  points.push_back(Point3d(10,10,0));
  points.push_back(Point3d(10,0,0));
  points.push_back(Point3d(0,0,0));

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
  Workspace workspace = forwardTranslator.translateModel(model);

  ASSERT_EQ(2u, workspace.getObjectsByType(IddObjectType::Lights).size());
  ASSERT_EQ(1u, workspace.getObjectsByType(IddObjectType::Zone).size());
  EXPECT_EQ(0u, workspace.getObjectsByType(IddObjectType::ZoneList).size());

  bool foundLightingPower100 = false;
  bool foundLightingPower200 = false;
  for (int i = 0; i < 2; ++i){
    ASSERT_TRUE(workspace.getObjectsByType(IddObjectType::Lights)[i].getString(LightsFields::DesignLevelCalculationMethod, false));
    EXPECT_EQ("LightingLevel", workspace.getObjectsByType(IddObjectType::Lights)[i].getString(LightsFields::DesignLevelCalculationMethod, false).get());

    ASSERT_TRUE(workspace.getObjectsByType(IddObjectType::Lights)[i].getDouble(LightsFields::LightingLevel, false));
    double lightingLevel = workspace.getObjectsByType(IddObjectType::Lights)[i].getDouble(LightsFields::LightingLevel, false).get();

    if (lightingLevel == 100.0){
      foundLightingPower100 = true;
    }else if (lightingLevel == 200.0){
      foundLightingPower200 = true;
    }
  }

  EXPECT_TRUE(foundLightingPower100);
  EXPECT_TRUE(foundLightingPower200);
}

TEST_F(EnergyPlusFixture,ForwardTranslator_Lights_TwoSpaceTypes_OneThermalZone_Building)
{
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
  std::vector<Point3d> points;
  points.push_back(Point3d(0,10,0));
  points.push_back(Point3d(10,10,0));
  points.push_back(Point3d(10,0,0));
  points.push_back(Point3d(0,0,0));

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
  Workspace workspace = forwardTranslator.translateModel(model);

  ASSERT_EQ(2u, workspace.getObjectsByType(IddObjectType::Lights).size());
  ASSERT_EQ(1u, workspace.getObjectsByType(IddObjectType::Zone).size());
  EXPECT_EQ(0u, workspace.getObjectsByType(IddObjectType::ZoneList).size());

  bool foundLightingPower100 = false;
  bool foundLightingPower200 = false;
  for (int i = 0; i < 2; ++i){
    ASSERT_TRUE(workspace.getObjectsByType(IddObjectType::Lights)[i].getString(LightsFields::DesignLevelCalculationMethod, false));
    EXPECT_EQ("LightingLevel", workspace.getObjectsByType(IddObjectType::Lights)[i].getString(LightsFields::DesignLevelCalculationMethod, false).get());

    ASSERT_TRUE(workspace.getObjectsByType(IddObjectType::Lights)[i].getDouble(LightsFields::LightingLevel, false));
    double lightingLevel = workspace.getObjectsByType(IddObjectType::Lights)[i].getDouble(LightsFields::LightingLevel, false).get();

    if (lightingLevel == 100.0){
      foundLightingPower100 = true;
    }else if (lightingLevel == 200.0){
      foundLightingPower200 = true;
    }
  }

  EXPECT_TRUE(foundLightingPower100);
  EXPECT_TRUE(foundLightingPower200);
}

TEST_F(EnergyPlusFixture, ForwardTranslator_ExampleModel_Lights) {
  Model model = exampleModel();

  ASSERT_EQ(1u, model.getModelObjects<SpaceType>().size());
  SpaceType spaceType = model.getModelObjects<SpaceType>()[0];

  ASSERT_EQ(1u, model.getModelObjects<ThermalZone>().size());
  ThermalZone thermalZone = model.getModelObjects<ThermalZone>()[0];

  EXPECT_EQ(4u, model.getModelObjects<Space>().size());
  for (const Space& space : model.getModelObjects<Space>()){
    ASSERT_TRUE(space.spaceType());
    EXPECT_EQ(spaceType.handle(), space.spaceType()->handle());
    ASSERT_TRUE(space.thermalZone());
    EXPECT_EQ(thermalZone.handle(), space.thermalZone()->handle());
  }

  ASSERT_EQ(1u, model.getModelObjects<Lights>().size());
  Lights lights = model.getModelObjects<Lights>()[0];
  EXPECT_FALSE(lights.space());
  ASSERT_TRUE(lights.spaceType());
  EXPECT_EQ(spaceType.handle(), lights.spaceType()->handle());

  ForwardTranslator forwardTranslator;
  Workspace workspace = forwardTranslator.translateModel(model);
  EXPECT_EQ(0u, forwardTranslator.errors().size());

  ASSERT_EQ(1u, workspace.getObjectsByType(IddObjectType::Lights).size());
  ASSERT_EQ(1u, workspace.getObjectsByType(IddObjectType::Zone).size());

}

TEST_F(EnergyPlusFixture,ForwardTranslator_Lights_Building_Schedule)
{
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

TEST_F(EnergyPlusFixture,ForwardTranslator_Lights_Bug983)
{
  Model model;

  std::vector<Point3d> vertices;
  vertices.push_back(Point3d(0,  10, 0));
  vertices.push_back(Point3d(10, 10, 0));
  vertices.push_back(Point3d(10, 0,  0));
  vertices.push_back(Point3d(0,  0,  0));

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
  Workspace workspace = forwardTranslator.translateModel(model);
  EXPECT_EQ(0u, forwardTranslator.errors().size());

  std::vector<WorkspaceObject> idfLights = workspace.getObjectsByType(IddObjectType::Lights);
  ASSERT_EQ(2u, idfLights.size());

  ASSERT_TRUE(idfLights[0].getString(LightsFields::DesignLevelCalculationMethod));
  EXPECT_EQ("LightingLevel", idfLights[0].getString(LightsFields::DesignLevelCalculationMethod).get());
  ASSERT_TRUE(idfLights[0].getDouble(LightsFields::LightingLevel));
  EXPECT_DOUBLE_EQ(1000.0, idfLights[0].getDouble(LightsFields::LightingLevel).get());

  ASSERT_TRUE(idfLights[1].getString(LightsFields::DesignLevelCalculationMethod));
  EXPECT_EQ("LightingLevel", idfLights[1].getString(LightsFields::DesignLevelCalculationMethod).get());
  ASSERT_TRUE(idfLights[1].getDouble(LightsFields::LightingLevel));
  EXPECT_DOUBLE_EQ(1000.0, idfLights[1].getDouble(LightsFields::LightingLevel).get());

}

TEST_F(EnergyPlusFixture,ForwardTranslator_Lights_Costs)
{
  Model model;

  ThermalZone zone1(model);
  ThermalZone zone2(model);

  std::vector<Point3d> floorPrint;
  floorPrint.push_back(Point3d(0, 1, 0));
  floorPrint.push_back(Point3d(1, 1, 0));
  floorPrint.push_back(Point3d(1, 0, 0));
  floorPrint.push_back(Point3d(0, 0, 0));
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

