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

//#include <gtest/gtest.h>
//#include "ContamFixture.hpp"
#include "DemoModel.hpp"
//#include "../../model/Model.hpp"
#include "../../model/Building.hpp"
#include "../../model/Building_Impl.hpp"
#include "../../model/Space.hpp"
#include "../../model/Space_Impl.hpp"
#include "../../model/SpaceType.hpp"
#include "../../model/Surface.hpp"
#include "../../model/SubSurface.hpp"
#include "../../model/DesignSpecificationOutdoorAir.hpp"
#include "../../model/BuildingStory.hpp"
#include "../../model/ThermostatSetpointDualSetpoint.hpp"
#include "../../model/ThermostatSetpointDualSetpoint_Impl.hpp"
#include "../../model/ThermalZone.hpp"
#include "../../model/ThermalZone_Impl.hpp"
#include "../../model/SizingZone.hpp"
#include "../../model/AirLoopHVAC.hpp"
#include "../../model/AirLoopHVAC_Impl.hpp"
#include "../../model/HVACTemplates.hpp"
#include "../../model/SetpointManagerSingleZoneReheat.hpp"
#include "../../model/SetpointManagerSingleZoneReheat_Impl.hpp"
#include "../../osversion/VersionTranslator.hpp"
#include "../../utilities/idf/Handle.hpp"
#include "../../utilities/geometry/Point3d.hpp"

boost::optional<openstudio::model::Model> buildDemoModel2012(openstudio::model::Model model)
{
  // set outdoor air specifications
  openstudio::model::Building building = model.getUniqueModelObject<openstudio::model::Building>();
  boost::optional<openstudio::model::SpaceType> spaceType = building.spaceType();
  if(!spaceType) {
    return boost::none;
  }
  boost::optional<openstudio::model::DesignSpecificationOutdoorAir> oa = spaceType->designSpecificationOutdoorAir();
  if(!oa) {
    return boost::none;
  }

  if(!oa->setOutdoorAirMethod("Sum")) {
    return boost::none;
  }
  if(!oa->setOutdoorAirFlowperPerson(0.0)) {
    return boost::none;
  }
  if(!oa->setOutdoorAirFlowperFloorArea(0.00508)) { // 1 cfm/ft^2 = 0.00508 m/s
    return boost::none;
  }
  if(!oa->setOutdoorAirFlowRate(0.0)) {
    return boost::none;
  }
  if(!oa->setOutdoorAirFlowAirChangesperHour(0.0)) {
    return boost::none;
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
  if(!library) {
    return boost::none;
  }
  library->setName("Library");

  points.clear();
  points.push_back(openstudio::Point3d(8,10,0));
  points.push_back(openstudio::Point3d(8,17,0));
  points.push_back(openstudio::Point3d(18,17,0));
  points.push_back(openstudio::Point3d(18,10,0));
  points.push_back(openstudio::Point3d(11,10,0));

  boost::optional<openstudio::model::Space> office2 = openstudio::model::Space::fromFloorPrint(points, floorHeight, model);
  if(!office2) {
    return boost::none;
  }
  office2->setName("Office 2");

  points.clear();
  points.push_back(openstudio::Point3d(8,0,0));
  points.push_back(openstudio::Point3d(8,10,0));
  points.push_back(openstudio::Point3d(11,10,0));
  points.push_back(openstudio::Point3d(11,0,0));

  boost::optional<openstudio::model::Space> hallway = openstudio::model::Space::fromFloorPrint(points, floorHeight, model);
  if(!hallway) {
    return boost::none;
  }
  hallway->setName("Hallway");

  points.clear();
  points.push_back(openstudio::Point3d(11,0,0));
  points.push_back(openstudio::Point3d(11,10,0));
  points.push_back(openstudio::Point3d(18,10,0));
  points.push_back(openstudio::Point3d(18,0,0));

  boost::optional<openstudio::model::Space> office1 = openstudio::model::Space::fromFloorPrint(points, floorHeight, model);
  if(!office1) {
    return boost::none;
  }
  office1->setName("Office 1");

  library->matchSurfaces(*office2);
  library->matchSurfaces(*hallway);
  hallway->matchSurfaces(*office1);
  hallway->matchSurfaces(*office2);
  office1->matchSurfaces(*office2);

  // find thermostat
  boost::optional<openstudio::model::ThermostatSetpointDualSetpoint> thermostat;
  for (openstudio::model::ThermostatSetpointDualSetpoint t :
    model.getModelObjects<openstudio::model::ThermostatSetpointDualSetpoint>()) {
    thermostat = t;
    break;
  }
  if(!thermostat) {
    return boost::none;
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
  for (openstudio::model::SetpointManagerSingleZoneReheat t :
    model.getModelObjects<openstudio::model::SetpointManagerSingleZoneReheat>()) {
    setpointManager = t;
    break;
  }
  if(!setpointManager) {
    return boost::optional<openstudio::model::Model>();
  }
  setpointManager->setControlZone(libraryZone);

  return boost::optional<openstudio::model::Model>(model);
}

boost::optional<openstudio::model::Model> addDemoModelDoorsWindows(openstudio::model::Model model)
{
  boost::optional<openstudio::model::Space> hallway = model.getConcreteModelObjectByName<openstudio::model::Space>("Hallway");
  boost::optional<openstudio::model::Space> office1 = model.getConcreteModelObjectByName<openstudio::model::Space>("Office 1");
  boost::optional<openstudio::model::Space> office2 = model.getConcreteModelObjectByName<openstudio::model::Space>("Office 2");
  boost::optional<openstudio::model::Space> library = model.getConcreteModelObjectByName<openstudio::model::Space>("Library");

  if(!hallway || !office1 || !office2 || !library) {
    return boost::none;
  }

  // Doors
  // Hallway, Exterior Door
  std::vector<openstudio::Point3d> vertices;
  vertices.push_back(openstudio::Point3d(9.022, 0, 0));
  vertices.push_back(openstudio::Point3d(9.978, 0, 0));
  vertices.push_back(openstudio::Point3d(9.978, 0, 2.413));
  vertices.push_back(openstudio::Point3d(9.022, 0, 2.413));

  openstudio::model::SubSurface exteriorDoor(vertices, model);

  std::vector<openstudio::model::Surface> searchResults = hallway->findSurfaces(180.0, 180.0, 90.0, 90.0);
  if(searchResults.size() != 1) {
    return boost::none;
  }
  openstudio::model::Surface southWall = searchResults[0];
  exteriorDoor.setSurface(southWall);

  // Hallway to Office 2 Door
  vertices.clear();
  vertices.push_back(openstudio::Point3d(9.022, 10, 0));
  vertices.push_back(openstudio::Point3d(9.978, 10, 0));
  vertices.push_back(openstudio::Point3d(9.978, 10, 2.413));
  vertices.push_back(openstudio::Point3d(9.022, 10, 2.413));

  openstudio::model::SubSurface interiorDoor0(vertices, model);

  searchResults = office2->findSurfaces(180.0, 180.0, 90.0, 90.0);
  if(searchResults.size() != 2) {
    return boost::none;
  }
  int index = -1;
  for(unsigned i = 0; i < 2; i++) {
    for(openstudio::Point3d pt : searchResults[i].vertices()) {
      if(pt.x() < 8.1) {
        index = i;
        break;
      }
    }
  }
  if(index < 0) {
    return boost::none;
  }
  southWall = searchResults[index];
  interiorDoor0.setSurface(southWall);

  std::reverse(vertices.begin(), vertices.end());
  openstudio::model::SubSurface interiorDoor1 = openstudio::model::SubSurface(vertices, model);

  searchResults = hallway->findSurfaces(0.0, 0.0, 90.0, 90.0);
  if(searchResults.size() != 1) {
    return boost::none;
  }
  openstudio::model::Surface northWall = searchResults[0];
  interiorDoor1.setSurface(northWall);

  interiorDoor0.setAdjacentSubSurface(interiorDoor1);

  // Office 2 Exterior Door
  vertices.clear();
  vertices.push_back(openstudio::Point3d(9.022, 17, 0));
  vertices.push_back(openstudio::Point3d(9.022, 17, 2.413));
  vertices.push_back(openstudio::Point3d(9.978, 17, 2.413));
  vertices.push_back(openstudio::Point3d(9.978, 17, 0));

  exteriorDoor = openstudio::model::SubSurface(vertices, model);

  searchResults = office2->findSurfaces(0.0, 0.0, 90.0, 90.0);
  if(searchResults.size() != 1) {
    return boost::none;
  }

  northWall = searchResults[0];
  exteriorDoor.setSurface(northWall);

  // Hallway to Office 1 Door
  vertices.clear();
  vertices.push_back(openstudio::Point3d(11.0, 8.978, 0));
  vertices.push_back(openstudio::Point3d(11.0, 8.978, 2.413));
  vertices.push_back(openstudio::Point3d(11.0, 8.022, 2.413));
  vertices.push_back(openstudio::Point3d(11.0, 8.022, 0));

  interiorDoor0 = openstudio::model::SubSurface(vertices, model);

  searchResults = hallway->findSurfaces(90.0, 90.0, 90.0, 90.0);
  if(searchResults.size() != 1) {
    return boost::none;
  }
  openstudio::model::Surface eastWall = searchResults[0];

  interiorDoor0.setSurface(eastWall);

  std::reverse(vertices.begin(), vertices.end());
  interiorDoor1 = openstudio::model::SubSurface(vertices, model);

  searchResults = office1->findSurfaces(270.0, 270.0, 90.0, 90.0);
  if(searchResults.size() != 1) {
    return boost::none;
  }
  openstudio::model::Surface westWall = searchResults[0];

  interiorDoor1.setSurface(westWall);

  interiorDoor0.setAdjacentSubSurface(interiorDoor1);

  // Office 1 Exterior Door
  vertices.clear();
  vertices.push_back(openstudio::Point3d(18.0, 8.978, 0));
  vertices.push_back(openstudio::Point3d(18.0, 8.978, 2.413));
  vertices.push_back(openstudio::Point3d(18.0, 8.022, 2.413));
  vertices.push_back(openstudio::Point3d(18.0, 8.022, 0));

  exteriorDoor = openstudio::model::SubSurface(vertices, model);

  searchResults = office1->findSurfaces(90.0, 90.0, 90.0, 90.0);
  if(searchResults.size() != 1) {
    return boost::none;
  }
  eastWall = searchResults[0];

  exteriorDoor.setSurface(eastWall);

  // Hallway to Library Door
  vertices.clear();
  vertices.push_back(openstudio::Point3d(8.0, 4.522, 0));
  vertices.push_back(openstudio::Point3d(8.0, 4.522, 2.413));
  vertices.push_back(openstudio::Point3d(8.0, 5.478, 2.413));
  vertices.push_back(openstudio::Point3d(8.0, 5.478, 0));

  interiorDoor0 = openstudio::model::SubSurface(vertices, model);

  searchResults = hallway->findSurfaces(270.0, 270.0, 90.0, 90.0);
  if(searchResults.size() != 1) {
    return boost::none;
  }
  westWall = searchResults[0];

  interiorDoor0.setSurface(westWall);

  std::reverse(vertices.begin(), vertices.end());
  interiorDoor1 = openstudio::model::SubSurface(vertices, model);

  searchResults = library->findSurfaces(90.0, 90.0, 90.0, 90.0);
  index = -1;
  for(unsigned i = 0; i < 2; i++) {
    for(openstudio::Point3d pt : searchResults[i].vertices()) {
      if(pt.y() < 0.1) {
        index = i;
        break;
      }
    }
  }
  if(index < 0) {
    return boost::none;
  }
  eastWall = searchResults[index];

  interiorDoor1.setSurface(eastWall);

  interiorDoor0.setAdjacentSubSurface(interiorDoor1);

  // Library Exterior Door
  vertices.clear();
  vertices.push_back(openstudio::Point3d(6.022, 17, 0));
  vertices.push_back(openstudio::Point3d(6.022, 17, 2.413));
  vertices.push_back(openstudio::Point3d(6.978, 17, 2.413));
  vertices.push_back(openstudio::Point3d(6.978, 17, 0));

  exteriorDoor = openstudio::model::SubSurface(vertices, model);

  searchResults = library->findSurfaces(0.0, 0.0, 90.0, 90.0);
  if(searchResults.size() != 1) {
    return boost::none;
  }

  northWall = searchResults[0];
  exteriorDoor.setSurface(northWall);

  // Windows
  // Office 1, South Wall
  vertices.clear();
  vertices.push_back(openstudio::Point3d(12.0, 0, 0.806));
  vertices.push_back(openstudio::Point3d(12.895, 0, 0.806));
  vertices.push_back(openstudio::Point3d(12.895, 0, 2.413));
  vertices.push_back(openstudio::Point3d(12.0, 0, 2.413));

  openstudio::model::SubSurface exteriorWindow(vertices, model);

  searchResults = office1->findSurfaces(180.0, 180.0, 90.0, 90.0);
  if(searchResults.size() != 1) {
    return boost::none;
  }
  southWall = searchResults[0];

  exteriorWindow.setSurface(southWall);

  vertices.clear();
  vertices.push_back(openstudio::Point3d(13.2, 0, 0.806));
  vertices.push_back(openstudio::Point3d(14.095, 0, 0.806));
  vertices.push_back(openstudio::Point3d(14.095, 0, 2.413));
  vertices.push_back(openstudio::Point3d(13.2, 0, 2.413));

  exteriorWindow = openstudio::model::SubSurface(vertices, model);

  exteriorWindow.setSurface(southWall);

  vertices.clear();
  vertices.push_back(openstudio::Point3d(14.905, 0, 0.806));
  vertices.push_back(openstudio::Point3d(15.8, 0, 0.806));
  vertices.push_back(openstudio::Point3d(15.8, 0, 2.413));
  vertices.push_back(openstudio::Point3d(14.905, 0, 2.413));

  exteriorWindow = openstudio::model::SubSurface(vertices, model);

  exteriorWindow.setSurface(southWall);

  vertices.clear();
  vertices.push_back(openstudio::Point3d(16.105, 0, 0.806));
  vertices.push_back(openstudio::Point3d(17.0, 0, 0.806));
  vertices.push_back(openstudio::Point3d(17.0, 0, 2.413));
  vertices.push_back(openstudio::Point3d(16.105, 0, 2.413));

  exteriorWindow = openstudio::model::SubSurface(vertices, model);

  exteriorWindow.setSurface(southWall);

  // Office 1, East Wall
  searchResults = office1->findSurfaces(90.0, 90.0, 90.0, 90.0);
  if(searchResults.size() != 1) {
    return boost::none;
  }
  eastWall = searchResults[0];

  vertices.clear();
  vertices.push_back(openstudio::Point3d(18.0, 1.0, 0.806));
  vertices.push_back(openstudio::Point3d(18.0, 1.895, 0.806));
  vertices.push_back(openstudio::Point3d(18.0, 1.895, 2.413));
  vertices.push_back(openstudio::Point3d(18.0, 1.0, 2.413));

  exteriorWindow = openstudio::model::SubSurface(vertices, model);

  exteriorWindow.setSurface(eastWall);

  vertices.clear();
  vertices.push_back(openstudio::Point3d(18.0, 2.2, 0.806));
  vertices.push_back(openstudio::Point3d(18.0, 3.095, 0.806));
  vertices.push_back(openstudio::Point3d(18.0, 3.095, 2.413));
  vertices.push_back(openstudio::Point3d(18.0, 2.2, 2.413));

  exteriorWindow = openstudio::model::SubSurface(vertices, model);

  exteriorWindow.setSurface(eastWall);

  vertices.clear();
  vertices.push_back(openstudio::Point3d(18.0, 3.905, 0.806));
  vertices.push_back(openstudio::Point3d(18.0, 4.8, 0.806));
  vertices.push_back(openstudio::Point3d(18.0, 4.8, 2.413));
  vertices.push_back(openstudio::Point3d(18.0, 3.905, 2.413));

  exteriorWindow = openstudio::model::SubSurface(vertices, model);

  exteriorWindow.setSurface(eastWall);

  vertices.clear();
  vertices.push_back(openstudio::Point3d(18.0, 5.105, 0.806));
  vertices.push_back(openstudio::Point3d(18.0, 6.0, 0.806));
  vertices.push_back(openstudio::Point3d(18.0, 6.0, 2.413));
  vertices.push_back(openstudio::Point3d(18.0, 5.105, 2.413));

  exteriorWindow = openstudio::model::SubSurface(vertices, model);

  exteriorWindow.setSurface(eastWall);

  // Library, South Wall
  searchResults = library->findSurfaces(180.0, 180.0, 90.0, 90.0);
  if(searchResults.size() != 1) {
    return boost::none;
  }
  southWall = searchResults[0];

  vertices.clear();
  vertices.push_back(openstudio::Point3d(1.0, 0, 0.806));
  vertices.push_back(openstudio::Point3d(1.895, 0, 0.806));
  vertices.push_back(openstudio::Point3d(1.895, 0, 2.413));
  vertices.push_back(openstudio::Point3d(1.0, 0, 2.413));

  exteriorWindow = openstudio::model::SubSurface(vertices, model);

  exteriorWindow.setSurface(southWall);

  vertices.clear();
  vertices.push_back(openstudio::Point3d(2.2, 0, 0.806));
  vertices.push_back(openstudio::Point3d(3.095, 0, 0.806));
  vertices.push_back(openstudio::Point3d(3.095, 0, 2.413));
  vertices.push_back(openstudio::Point3d(2.2, 0, 2.413));

  exteriorWindow = openstudio::model::SubSurface(vertices, model);

  exteriorWindow.setSurface(southWall);

  vertices.clear();
  vertices.push_back(openstudio::Point3d(3.905, 0, 0.806));
  vertices.push_back(openstudio::Point3d(4.8, 0, 0.806));
  vertices.push_back(openstudio::Point3d(4.8, 0, 2.413));
  vertices.push_back(openstudio::Point3d(3.905, 0, 2.413));

  exteriorWindow = openstudio::model::SubSurface(vertices, model);

  exteriorWindow.setSurface(southWall);

  vertices.clear();
  vertices.push_back(openstudio::Point3d(5.105, 0, 0.806));
  vertices.push_back(openstudio::Point3d(6.0, 0, 0.806));
  vertices.push_back(openstudio::Point3d(6.0, 0, 2.413));
  vertices.push_back(openstudio::Point3d(5.105, 0, 2.413));

  exteriorWindow = openstudio::model::SubSurface(vertices, model);

  exteriorWindow.setSurface(southWall);

  // Library, West Wall
  searchResults = library->findSurfaces(270.0, 270.0, 90.0, 90.0);
  if(searchResults.size() != 1) {
    return boost::none;
  }
  westWall = searchResults[0];

  vertices.clear();
  vertices.push_back(openstudio::Point3d(0.0, 1.895, 0.806));
  vertices.push_back(openstudio::Point3d(0.0, 1.0, 0.806));
  vertices.push_back(openstudio::Point3d(0.0, 1.0, 2.413));
  vertices.push_back(openstudio::Point3d(0.0, 1.895, 2.413));

  exteriorWindow = openstudio::model::SubSurface(vertices, model);

  exteriorWindow.setSurface(westWall);

  vertices.clear();
  vertices.push_back(openstudio::Point3d(0.0, 3.095, 0.806));
  vertices.push_back(openstudio::Point3d(0.0, 2.2, 0.806));
  vertices.push_back(openstudio::Point3d(0.0, 2.2, 2.413));
  vertices.push_back(openstudio::Point3d(0.0, 3.095, 2.413));

  exteriorWindow = openstudio::model::SubSurface(vertices, model);

  exteriorWindow.setSurface(westWall);

  vertices.clear();
  vertices.push_back(openstudio::Point3d(0.0, 4.8, 0.806));
  vertices.push_back(openstudio::Point3d(0.0, 3.905, 0.806));
  vertices.push_back(openstudio::Point3d(0.0, 3.905, 2.413));
  vertices.push_back(openstudio::Point3d(0.0, 4.8, 2.413));

  exteriorWindow = openstudio::model::SubSurface(vertices, model);

  exteriorWindow.setSurface(westWall);

  vertices.clear();
  vertices.push_back(openstudio::Point3d(0.0, 6.0, 0.806));
  vertices.push_back(openstudio::Point3d(0.0, 5.105, 0.806));
  vertices.push_back(openstudio::Point3d(0.0, 5.105, 2.413));
  vertices.push_back(openstudio::Point3d(0.0, 6.0, 2.413));

  exteriorWindow = openstudio::model::SubSurface(vertices, model);

  exteriorWindow.setSurface(westWall);

  vertices.clear();
  vertices.push_back(openstudio::Point3d(0.0, 11.895, 0.806));
  vertices.push_back(openstudio::Point3d(0.0, 11.0, 0.806));
  vertices.push_back(openstudio::Point3d(0.0, 11.0, 2.413));
  vertices.push_back(openstudio::Point3d(0.0, 11.895, 2.413));

  exteriorWindow = openstudio::model::SubSurface(vertices, model);

  exteriorWindow.setSurface(westWall);

  vertices.clear();
  vertices.push_back(openstudio::Point3d(0.0, 13.095, 0.806));
  vertices.push_back(openstudio::Point3d(0.0, 12.2, 0.806));
  vertices.push_back(openstudio::Point3d(0.0, 12.2, 2.413));
  vertices.push_back(openstudio::Point3d(0.0, 13.095, 2.413));

  exteriorWindow = openstudio::model::SubSurface(vertices, model);

  exteriorWindow.setSurface(westWall);

  vertices.clear();
  vertices.push_back(openstudio::Point3d(0.0, 14.8, 0.806));
  vertices.push_back(openstudio::Point3d(0.0, 13.905, 0.806));
  vertices.push_back(openstudio::Point3d(0.0, 13.905, 2.413));
  vertices.push_back(openstudio::Point3d(0.0, 14.8, 2.413));

  exteriorWindow = openstudio::model::SubSurface(vertices, model);

  exteriorWindow.setSurface(westWall);

  vertices.clear();
  vertices.push_back(openstudio::Point3d(0.0, 16.0, 0.806));
  vertices.push_back(openstudio::Point3d(0.0, 15.105, 0.806));
  vertices.push_back(openstudio::Point3d(0.0, 15.105, 2.413));
  vertices.push_back(openstudio::Point3d(0.0, 16.0, 2.413));

  exteriorWindow = openstudio::model::SubSurface(vertices, model);

  exteriorWindow.setSurface(westWall);

  // Office 2, West Wall
  searchResults = office2->findSurfaces(90.0, 90.0, 90.0, 90.0);
  if(searchResults.size() != 1) {
    return boost::none;
  }
  westWall = searchResults[0];

  vertices.clear();
  vertices.push_back(openstudio::Point3d(18.0, 11.0, 0.806));
  vertices.push_back(openstudio::Point3d(18.0, 11.895, 0.806));
  vertices.push_back(openstudio::Point3d(18.0, 11.895, 2.413));
  vertices.push_back(openstudio::Point3d(18.0, 11.0, 2.413));

  exteriorWindow = openstudio::model::SubSurface(vertices, model);

  exteriorWindow.setSurface(westWall);

  vertices.clear();
  vertices.push_back(openstudio::Point3d(18.0, 12.2, 0.806));
  vertices.push_back(openstudio::Point3d(18.0, 13.095, 0.806));
  vertices.push_back(openstudio::Point3d(18.0, 13.095, 2.413));
  vertices.push_back(openstudio::Point3d(18.0, 12.2, 2.413));

  exteriorWindow = openstudio::model::SubSurface(vertices, model);

  exteriorWindow.setSurface(westWall);

  vertices.clear();
  vertices.push_back(openstudio::Point3d(18.0, 13.905, 0.806));
  vertices.push_back(openstudio::Point3d(18.0, 14.8, 0.806));
  vertices.push_back(openstudio::Point3d(18.0, 14.8, 2.413));
  vertices.push_back(openstudio::Point3d(18.0, 13.905, 2.413));

  exteriorWindow = openstudio::model::SubSurface(vertices, model);

  exteriorWindow.setSurface(westWall);

  vertices.clear();
  vertices.push_back(openstudio::Point3d(18.0, 15.105, 0.806));
  vertices.push_back(openstudio::Point3d(18.0, 16.0, 0.806));
  vertices.push_back(openstudio::Point3d(18.0, 16.0, 2.413));
  vertices.push_back(openstudio::Point3d(18.0, 15.105, 2.413));

  exteriorWindow = openstudio::model::SubSurface(vertices, model);

  exteriorWindow.setSurface(westWall);

  //return boost::none;
  return boost::optional<openstudio::model::Model>(model);
}

boost::optional<openstudio::model::Model> buildDemoModel2014(openstudio::model::Model model)
{
  boost::optional<openstudio::model::Model> optModel = buildDemoModel2012(model);
  if(optModel) {
    return addDemoModelDoorsWindows(optModel.get());
  }
  return boost::none;
}
