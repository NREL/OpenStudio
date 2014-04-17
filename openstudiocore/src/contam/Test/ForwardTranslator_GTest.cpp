/**********************************************************************
*  Copyright (c) 2008-2014, Alliance for Sustainable Energy.
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
#include <contam/Test/ContamFixture.hpp>

#include <contam/ForwardTranslator.hpp>

#include <model/Model.hpp>
#include <model/Building.hpp>
#include <model/Building_Impl.hpp>
#include <model/Space.hpp>
#include <model/SpaceType.hpp>
#include <model/DesignSpecificationOutdoorAir.hpp>
#include <model/BuildingStory.hpp>
#include <utilities/geometry/Point3d.hpp>
#include <model/ThermostatSetpointDualSetpoint.hpp>
#include <model/ThermostatSetpointDualSetpoint_Impl.hpp>
#include <model/ThermalZone.hpp>
#include <model/ThermalZone_Impl.hpp>
#include <model/SizingZone.hpp>
#include <model/AirLoopHVAC.hpp>
#include <model/AirLoopHVAC_Impl.hpp>
#include <model/HVACTemplates.hpp>
#include <model/SetpointManagerSingleZoneReheat.hpp>
#include <model/SetpointManagerSingleZoneReheat_Impl.hpp>
#include <osversion/VersionTranslator.hpp>
#include <utilities/idf/Handle.hpp>

#include <resources.hxx>

#include <sstream>

using namespace openstudio;

boost::optional<openstudio::model::Model> buildDemoModel(openstudio::model::Model model)
{
  // set outdoor air specifications
  openstudio::model::Building building = model.getUniqueModelObject<openstudio::model::Building>();
  boost::optional<openstudio::model::SpaceType> spaceType = building.spaceType();
  if(!spaceType)
  {
    return boost::optional<openstudio::model::Model>();
  }
  boost::optional<openstudio::model::DesignSpecificationOutdoorAir> oa = spaceType->designSpecificationOutdoorAir();
  if(!oa)
  {
    return boost::optional<openstudio::model::Model>();
  }

  if(!oa->setOutdoorAirMethod("Sum"))
  {
    return boost::optional<openstudio::model::Model>();
  }
  if(!oa->setOutdoorAirFlowperPerson(0.0))
  {
    return boost::optional<openstudio::model::Model>();
  }
  if(!oa->setOutdoorAirFlowperFloorArea(0.00508)) // 1 cfm/ft^2 = 0.00508 m/s
  {
    return boost::optional<openstudio::model::Model>();
  }
  if(!oa->setOutdoorAirFlowRate(0.0))
  {
    return boost::optional<openstudio::model::Model>();
  }
  if(!oa->setOutdoorAirFlowAirChangesperHour(0.0))
  {
    return boost::optional<openstudio::model::Model>();
  }

  double floorHeight = 3.0;

  openstudio::model::BuildingStory story1(model);
  story1.setName("Story 1");
  story1.setNominalZCoordinate(0.0);
  story1.setNominalFloortoFloorHeight(floorHeight);

  std::vector<openstudio::Point3d> points;
  points.push_back(openstudio::Point3d(0,0,0));
  points.push_back(openstudio::Point3d(0,17,0));
  points.push_back(openstudio::Point3d(8,17,0));
  points.push_back(openstudio::Point3d(8,10,0));
  points.push_back(openstudio::Point3d(8,0,0));

  boost::optional<openstudio::model::Space> library = openstudio::model::Space::fromFloorPrint(points, floorHeight, model);
  if(!library)
  {
    return boost::optional<openstudio::model::Model>();
  }
  library->setName("Library");

  points.clear();
  points.push_back(openstudio::Point3d(8,10,0));
  points.push_back(openstudio::Point3d(8,17,0));
  points.push_back(openstudio::Point3d(18,17,0));
  points.push_back(openstudio::Point3d(18,10,0));
  points.push_back(openstudio::Point3d(11,10,0));

  boost::optional<openstudio::model::Space> office2 = openstudio::model::Space::fromFloorPrint(points, floorHeight, model);
  if(!office2)
  {
    return boost::optional<openstudio::model::Model>();
  }
  office2->setName("Office 2");

  points.clear();
  points.push_back(openstudio::Point3d(8,0,0));
  points.push_back(openstudio::Point3d(8,10,0));
  points.push_back(openstudio::Point3d(11,10,0));
  points.push_back(openstudio::Point3d(11,0,0));

  boost::optional<openstudio::model::Space> hallway = openstudio::model::Space::fromFloorPrint(points, floorHeight, model);
  if(!hallway)
  {
    return boost::optional<openstudio::model::Model>();
  }
  hallway->setName("Hallway");

  points.clear();
  points.push_back(openstudio::Point3d(11,0,0));
  points.push_back(openstudio::Point3d(11,10,0));
  points.push_back(openstudio::Point3d(18,10,0));
  points.push_back(openstudio::Point3d(18,0,0));

  boost::optional<openstudio::model::Space> office1 = openstudio::model::Space::fromFloorPrint(points, floorHeight, model);
  if(!office1)
  {
    return boost::optional<openstudio::model::Model>();
  }
  office1->setName("Office 1");

  library->matchSurfaces(*office2);
  library->matchSurfaces(*hallway);
  hallway->matchSurfaces(*office1);
  hallway->matchSurfaces(*office2);
  office1->matchSurfaces(*office2);

  // find thermostat
  boost::optional<openstudio::model::ThermostatSetpointDualSetpoint> thermostat;
  BOOST_FOREACH(openstudio::model::ThermostatSetpointDualSetpoint t,
    model.getModelObjects<openstudio::model::ThermostatSetpointDualSetpoint>())
  {
    thermostat = t;
    break;
  }
  if(!thermostat)
  {
    return boost::optional<openstudio::model::Model>();
  }
  
  // create  thermal zones
  openstudio::model::ThermalZone libraryZone(model);
  openstudio::model::SizingZone librarySizing(model, libraryZone);
  libraryZone.setName("Library Zone");
  libraryZone.setThermostatSetpointDualSetpoint(*thermostat);
  library->setThermalZone(libraryZone);
  library->setBuildingStory(story1);

  openstudio::model::ThermalZone hallwayZone(model);
  //model::SizingZone hallwaySizing(model, hallwayZone);
  hallwayZone.setName("Hallway Zone");
  //hallwayZone.setThermostatSetpointDualSetpoint(*thermostat);
  hallway->setThermalZone(hallwayZone);
  hallway->setBuildingStory(story1);

  openstudio::model::ThermalZone office1Zone(model);
  openstudio::model::SizingZone office1Sizing(model, office1Zone);
  office1Zone.setName("Office 1 Zone");
  office1Zone.setThermostatSetpointDualSetpoint(*thermostat);
  office1->setThermalZone(office1Zone);
  office1->setBuildingStory(story1);

  openstudio::model::ThermalZone office2Zone(model);
  openstudio::model::SizingZone office2Sizing(model, office2Zone);
  office2Zone.setName("Office 2 Zone");
  office2Zone.setThermostatSetpointDualSetpoint(*thermostat);
  office2->setThermalZone(office2Zone);
  office2->setBuildingStory(story1);

  // add the air system
  openstudio::model::Loop loop = openstudio::model::addSystemType3(model);
  openstudio::model::AirLoopHVAC airLoop = loop.cast<openstudio::model::AirLoopHVAC>();
  airLoop.addBranchForZone(libraryZone);
  airLoop.addBranchForZone(office1Zone);
  airLoop.addBranchForZone(office2Zone);

  boost::optional<openstudio::model::SetpointManagerSingleZoneReheat> setpointManager;
  BOOST_FOREACH(openstudio::model::SetpointManagerSingleZoneReheat t, 
    model.getModelObjects<openstudio::model::SetpointManagerSingleZoneReheat>())
  {
    setpointManager = t;
    break;
  }
  if(!setpointManager)
  {
    return boost::optional<openstudio::model::Model>();
  }
  setpointManager->setControlZone(libraryZone);

  return boost::optional<openstudio::model::Model>(model);
}

TEST_F(ContamFixture, ForwardTranslator_exampleModel)
{
  model::Model model = model::exampleModel();

  path p = toPath("exampleModel.prj");

  bool test = contam::ForwardTranslator::modelToPrj(model, p);

  //bool test = contam::ForwardTranslator::modelToContam(model, p, m);

  EXPECT_TRUE(test);
}

TEST_F(ContamFixture, ForwardTranslator_DemoModel_2012)
{
  openstudio::path modelPath = (resourcesPath() / openstudio::toPath("contam") / openstudio::toPath("CONTAMTemplate.osm"));
  osversion::VersionTranslator vt;
  boost::optional<model::Model> optionalModel = vt.loadModel(modelPath);
  ASSERT_TRUE(optionalModel);
  model::Model model = optionalModel.get();

  boost::optional<model::Model> demoModel = buildDemoModel(model);

  ASSERT_TRUE(demoModel);

  contam::ForwardTranslator translator;

  boost::optional<openstudio::contam::IndexModel> prjModel = translator.translateModel(demoModel.get());

  ASSERT_TRUE(prjModel);

  // 6 Zones
  EXPECT_EQ(6,prjModel->zones().size());
  int systemZoneCount=0;
  int interiorZoneCount=0;
  BOOST_FOREACH(const contam::Zone zone, prjModel->zones())
  {
    if(zone.system())
    {
      systemZoneCount++;
    }
    else if(zone.variablePressure() && zone.variableContaminants())
    {
      interiorZoneCount++;
    }
  }
  EXPECT_EQ(2,systemZoneCount);
  EXPECT_EQ(4,interiorZoneCount);
  // 26 Paths
  EXPECT_EQ(26,prjModel->airflowPaths().size());
  int exhaustPathCount=0;
  int systemPathCount=0;
  int windPressurePathCount=0;
  int outsideAirPathCount=0;
  int recirculationPathCount=0;
  int plainPathCount=0;
  BOOST_FOREACH(contam::AirflowPath afp, prjModel->airflowPaths())
  {
    if(afp.system())
    {
      systemPathCount++;
    }
    else if(afp.windPressure())
    {
      windPressurePathCount++;
    }
    else if(afp.exhaust())
    {
      exhaustPathCount++;
    }
    else if(afp.outsideAir())
    {
      outsideAirPathCount++;
    }
    else if(afp.recirculation())
    {
      recirculationPathCount++;
    }
    else
    {
      plainPathCount++;
    }
  }
  EXPECT_EQ(6,systemPathCount);
  EXPECT_EQ(12,windPressurePathCount);
  EXPECT_EQ(5,plainPathCount);
  EXPECT_EQ(1,recirculationPathCount);
  EXPECT_EQ(1,outsideAirPathCount);
  EXPECT_EQ(1,exhaustPathCount);

  // Check out the zones using the zone map
  QVector<std::string> zoneNames;
  zoneNames << "Library Zone" << "Office 1 Zone" << "Office 2 Zone" << "Hallway Zone";
  QMap<std::string,int> exteriorWallCount;
  exteriorWallCount["Library Zone"] = 4;
  exteriorWallCount["Office 1 Zone"] = 3;
  exteriorWallCount["Office 2 Zone"] = 3;
  exteriorWallCount["Hallway Zone"] = 2;

  std::map <Handle, int> zoneMap = translator.zoneMap();
  EXPECT_EQ(4,zoneMap.size());

  std::vector<std::vector<int> > exteriorFlowPaths = prjModel->zoneExteriorFlowPaths();
  EXPECT_EQ(6,exteriorFlowPaths.size());

  BOOST_FOREACH(model::ThermalZone thermalZone, model.getConcreteModelObjects<model::ThermalZone>())
  {
    ASSERT_TRUE(zoneNames.contains(thermalZone.name().get()));
    unsigned zoneNumber = zoneMap[thermalZone.handle()];
    ASSERT_TRUE(zoneNumber > 0);
    ASSERT_TRUE(zoneNumber <= prjModel->zones().size());
    int zoneExteriorWallCount = exteriorFlowPaths[zoneNumber-1].size();
    EXPECT_EQ(exteriorWallCount[thermalZone.name().get()],zoneExteriorWallCount);
  }

  // Try setting some values to make sure things work
  EXPECT_TRUE(prjModel->setDef_T(297.15));
  EXPECT_TRUE(prjModel->setDef_T("297.15"));
  EXPECT_FALSE(prjModel->setDef_T("twoninetysevenpointonefive"));
  EXPECT_EQ(297.15,prjModel->def_T());
}
