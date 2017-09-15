/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2017, Alliance for Sustainable Energy, LLC. All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 *  following conditions are met:
 *
 *  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
 *  disclaimer.
 *
 *  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *  following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 *  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote
 *  products derived from this software without specific prior written permission from the respective party.
 *
 *  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative
 *  works may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without
 *  specific prior written permission from Alliance for Sustainable Energy, LLC.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER, THE UNITED STATES GOVERNMENT, OR ANY CONTRIBUTORS BE LIABLE FOR
 *  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **********************************************************************************************************************/

#include <gtest/gtest.h>

#include "IdfFixture.hpp"

#include "../Workspace.hpp"
#include "../Workspace_Impl.hpp"
#include "../WorkspaceObject.hpp"
#include "../WorkspaceObjectOrder.hpp"
#include "../URLSearchPath.hpp"
#include "../ValidityReport.hpp"
#include "../IdfExtensibleGroup.hpp"
#include "../WorkspaceExtensibleGroup.hpp"

#include "../../idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/IddFactory.hxx>
#include <utilities/idd/Building_FieldEnums.hxx>
#include <utilities/idd/Construction_FieldEnums.hxx>
#include <utilities/idd/Zone_FieldEnums.hxx>
#include <utilities/idd/Lights_FieldEnums.hxx>
#include <utilities/idd/Output_Meter_FieldEnums.hxx>
#include <utilities/idd/Schedule_Compact_FieldEnums.hxx>
#include <utilities/idd/Wall_Exterior_FieldEnums.hxx>
#include <utilities/idd/Wall_Adiabatic_FieldEnums.hxx>
#include <utilities/idd/Window_FieldEnums.hxx>
#include <utilities/idd/BuildingSurface_Detailed_FieldEnums.hxx>
#include <utilities/idd/Sizing_Zone_FieldEnums.hxx>
#include <utilities/idd/OS_WeatherFile_FieldEnums.hxx>
#include "../WorkspaceWatcher.hpp"
#include "IdfTestQObjects.hpp"

#include "../../core/Application.hpp"
#include "../../core/Path.hpp"
#include "../../core/Optional.hpp"

#include "../../time/Time.hpp"

#include "../../core/Compare.hpp"
#include "../../core/StringStreamLogSink.hpp"

#include <resources.hxx>




using namespace openstudio;

#include <iostream>

TEST_F(IdfFixture, IdfFile_Workspace_DefaultConstructor)
{
  Workspace workspaceNone(StrictnessLevel::None);
  EXPECT_TRUE(workspaceNone.isValid());
  Workspace workspaceDraft(StrictnessLevel::Draft);
  EXPECT_TRUE(workspaceDraft.isValid());
  EXPECT_ANY_THROW(Workspace workspaceFinal(StrictnessLevel::Final));
}

TEST_F(IdfFixture, IdfFile_Workspace_Roundtrip)
{
  ASSERT_TRUE(epIdfFile.objects().size() > 0);
  Workspace workspace(epIdfFile,StrictnessLevel::None);
  IdfFile copyOfIdfFile = workspace.toIdfFile();
  // until == available, print out for diff
  openstudio::path outPath = outDir/toPath("passedThroughWorkspace.idf");
  openstudio::filesystem::ofstream outFile(outPath); ASSERT_TRUE(outFile?true:false);
  copyOfIdfFile.print(outFile); outFile.close();
}

TEST_F(IdfFixture, ObjectHasURL)
{
  Workspace workspace(epIdfFile,StrictnessLevel::None);
  workspace.addObject(IdfObject(IddObjectType::Schedule_File));
  workspace.addObject(IdfObject(IddObjectType::Zone));

  std::vector<WorkspaceObject> urls  = workspace.objectsWithURLFields();
  ASSERT_EQ(static_cast<unsigned>(1), urls.size());
  EXPECT_EQ(IddObjectType::Schedule_File, urls[0].iddObject().type().value());
}

TEST_F(IdfFixture, ObjectGettersAndSetters_StrictnessNone)
{
  Workspace workspace(epIdfFile,StrictnessLevel::None);
  workspace.setStrictnessLevel(StrictnessLevel::None);

  // getters before any modifications
  WorkspaceObjectVector result;
  WorkspaceObjectVector temp;
  // get all methods
  result = workspace.objects();
  LOG(Info,"There are " << result.size() << " objects in the Workspace.");
  // get matching methods
  result = workspace.getObjectsByName("C5-1");
  ASSERT_TRUE(result.size() > 0);
  EXPECT_EQ(static_cast<unsigned>(1),result.size());
  EXPECT_TRUE(result[0].iddObject().type() == IddObjectType::BuildingSurface_Detailed);
  result = workspace.getObjectsByType(IddObjectType::Schedule_Compact);
  EXPECT_EQ(static_cast<unsigned>(22),result.size());
  result = workspace.getObjectsByReference("CustomMeterNames");
  LOG(Info,"There are " << result.size() << " objects that can be accessed with \\object-list CustomMeterNames.");

}

TEST_F(IdfFixture, Workspace_StateCheckingAndRepair_ValidityCheckingAndReports)
{
  ASSERT_TRUE(epIdfFile.objects().size() > 0);
  Workspace workspace(epIdfFile, StrictnessLevel::None);

  ValidityReport report = workspace.validityReport(StrictnessLevel::None);
  EXPECT_EQ(static_cast<unsigned>(0),report.numErrors());
  EXPECT_TRUE(workspace.isValid(StrictnessLevel::None));
  EXPECT_TRUE(workspace.setStrictnessLevel(StrictnessLevel::None));
  EXPECT_EQ(StrictnessLevel::None, workspace.strictnessLevel().value());

  report = workspace.validityReport(StrictnessLevel::Draft);
  EXPECT_EQ(static_cast<unsigned>(0),report.numErrors());
  EXPECT_TRUE(workspace.isValid(StrictnessLevel::Draft));
  EXPECT_TRUE(workspace.setStrictnessLevel(StrictnessLevel::Draft));
  EXPECT_EQ(StrictnessLevel::Draft, workspace.strictnessLevel().value());

  report = workspace.validityReport(StrictnessLevel::Final);
  EXPECT_EQ(static_cast<unsigned>(0),report.numErrors());
  EXPECT_TRUE(workspace.isValid(StrictnessLevel::Final));
  EXPECT_TRUE(workspace.setStrictnessLevel(StrictnessLevel::Final));
  EXPECT_EQ(StrictnessLevel::Final, workspace.strictnessLevel().value());
}

TEST_F(IdfFixture, Workspace_StateCheckingAndRepair_ValidityCheckingAndReports_2)
{
  ASSERT_TRUE(epIdfFile.objects().size() > 0);
  Workspace workspace(epIdfFile, StrictnessLevel::None);
  ASSERT_TRUE(workspace.objects().size() > 0);

  ValidityReport report = workspace.validityReport(StrictnessLevel::Final);
  EXPECT_EQ(static_cast<unsigned>(0),report.numErrors());
  if (report.numErrors() > 0) {
    LOG(Error,"ValidityCheckingAndReports_2 invalid at StrictnessLevel Final. The ValidityReport "
        "follows." << std::endl << report);
  }
  EXPECT_TRUE(workspace.isValid(StrictnessLevel::Final));
  EXPECT_TRUE(workspace.setStrictnessLevel(StrictnessLevel::Final));
  EXPECT_EQ(StrictnessLevel::Final, workspace.strictnessLevel().value());

  report = workspace.validityReport(StrictnessLevel::Draft);
  EXPECT_EQ(static_cast<unsigned>(0),report.numErrors());
  if (report.numErrors() > 0) {
    LOG(Error,"ValidityCheckingAndReports_2 invalid at StrictnessLevel Draft. The ValidityReport "
        "follows." << std::endl << report);
  }
  EXPECT_TRUE(workspace.isValid(StrictnessLevel::Draft));
  EXPECT_TRUE(workspace.setStrictnessLevel(StrictnessLevel::Draft));
  EXPECT_EQ(StrictnessLevel::Draft, workspace.strictnessLevel().value());

  report = workspace.validityReport(StrictnessLevel::None);
  EXPECT_EQ(static_cast<unsigned>(0),report.numErrors());
  if (report.numErrors() > 0) {
    LOG(Error,"ValidityCheckingAndReports_2 invalid at StrictnessLevel None. The ValidityReport "
        "follows." << std::endl << report);
  }
  EXPECT_TRUE(workspace.isValid(StrictnessLevel::None));
  EXPECT_TRUE(workspace.setStrictnessLevel(StrictnessLevel::None));
  EXPECT_EQ(StrictnessLevel::None, workspace.strictnessLevel().value());

}

TEST_F(IdfFixture, Workspace_ValidityCheckingAndReports_3) {
  Workspace workspace(epIdfFile);

  // remove the building object
  WorkspaceObjectVector buildingObjects = workspace.getObjectsByType(IddObjectType::Building);
  ASSERT_TRUE(buildingObjects.size() > 0);
  EXPECT_EQ(static_cast<unsigned>(1),buildingObjects.size());
  workspace.removeObject(buildingObjects[0].handle());

  ValidityReport report = workspace.validityReport(StrictnessLevel::Final);
  EXPECT_TRUE(report.numErrors() > 0);
  bool errorFound = false;
  OptionalDataError oError = report.nextError();
  while (oError) {
    DataError error = *oError;
    if ((error.scope() == Scope::Collection) &&
        (error.type() == DataErrorType::NullAndRequired) &&
        (error.objectType()) &&
        (error.objectType().get() == IddObjectType::Building)) {
      errorFound = true;
      break;
    }
    oError = report.nextError();
  }
  EXPECT_TRUE(errorFound);
}

TEST_F(IdfFixture, Workspace_FollowingPointers) {

  ASSERT_TRUE(epIdfFile.objects().size() > 0);
  Workspace workspace(epIdfFile, StrictnessLevel::Draft);

  EXPECT_EQ(epIdfFile.objects().size(), workspace.objects().size());

  // there are 6 zones, 5 with lights

  // forward (e.g. lights -> zone)
  WorkspaceObjectVector lights = workspace.getObjectsByType(IddObjectType::Lights);
  EXPECT_EQ(static_cast<size_t>(5), lights.size());
  for (const WorkspaceObject& light : lights){

    // check the light
    EXPECT_TRUE(light.canBeSource());
    EXPECT_TRUE(light.isSource());

    // get the zone
    OptionalWorkspaceObject zone = light.getTarget(LightsFields::ZoneorZoneListName);
    ASSERT_TRUE(zone);
    //EXPECT_TRUE(zone->canBeTarget());
    EXPECT_TRUE(zone->isTarget());

    // check that this light is in the zone's sources
    WorkspaceObjectVector sources = zone->sources();
    WorkspaceObjectVector::const_iterator it = std::find(sources.begin(),sources.end(),light);
    EXPECT_TRUE(it != sources.end());
    if (it == sources.end()) {
      LOG(Warn,"Light object is not a source for its zone. Zone name = " << *(zone->name())
          << ", and Zone has " << sources.size() << " sources.");
    }
  }

  // backward (e.g. zone -> lights)
  WorkspaceObjectVector zones = workspace.getObjectsByType(IddObjectType::Zone);
  EXPECT_EQ(static_cast<size_t>(6), zones.size());
  for (const WorkspaceObject& zone : zones){

    // check the zone
    //EXPECT_TRUE(zone.canBeTarget());
    EXPECT_TRUE(zone.isTarget());

    // get lights for this zone
    WorkspaceObjectVector zoneLights = zone.getSources(IddObjectType(IddObjectType::Lights));

    // all zones except plenum should have one lights object
    OptionalString zoneName = zone.getString(ZoneFields::Name);
    ASSERT_TRUE(zoneName);
    if (istringEqual(*zoneName, "PLENUM-1")){
      EXPECT_EQ(static_cast<size_t>(0), zoneLights.size());
    }else{
      if (zoneLights.size() != 1) {
        LOG(Debug,"Unexpected number of Lights in Zone " << *zoneName << ". It has "
            << zoneLights.size() << " Lights objects pointing to it.");
      }
      EXPECT_EQ(static_cast<unsigned>(1), zoneLights.size());
      if (!zoneLights.empty()) {
        OptionalString lightsZoneName = zoneLights[0].getString(LightsFields::ZoneorZoneListName);
        ASSERT_TRUE(lightsZoneName);
        EXPECT_EQ(*zoneName, *lightsZoneName);
      }
    }
  }
}

// This test mimics the creation of a budget building. In particular, it blindly changes out
// all lights objects and their schedules.
//
// The test building has 6 Zone objects and 5 Lights objects. All zones are marked "Office"
// even though one is a plenum.
TEST_F(IdfFixture, Workspace_Alpha1) {

  // Construct workspace
  ASSERT_TRUE(epIdfFile.objects().size() > 0);
  Workspace workspace(epIdfFile, StrictnessLevel::Draft);
  ASSERT_TRUE(workspace.objects().size() > 0);

  // Add Lights schedule to workspace.
  std::stringstream ss;
  ss << "ScheduleTypeLimits, " << std::endl
     << "Any Number;              !- Name";
  IdfObject scheduleTypeLimits = IdfObject::load(ss.str(),
      *(IddFactory::instance().getObject(IddObjectType::ScheduleTypeLimits))).get();
  ss.str("");
  ss << "Schedule:Compact, " << std::endl
     << "LIT-SCHED-BldgAvg,       !- Name " << std::endl
     << "Any Number,              !- Schedule Type Limits Name " << std::endl
     << "Through: 12/31,          !- Field 1 " << std::endl
     << "For: Weekdays SummerDesignDay, !- Field 2 " << std::endl
     << "Until:  4:00,0.05,       !- Field 3 " << std::endl
     << "Until:  5:00,0.1,        !- Field 5 " << std::endl
     << "Until:  6:00,0.2,        !- Field 7 " << std::endl
     << "Until:  7:00,0.4,        !- Field 9 " << std::endl
     << "Until:  8:00,0.7,        !- Field 11 " << std::endl
     << "Until:  9:00,0.8,        !- Field 13 " << std::endl
     << "Until:  17:00,0.85,      !- Field 15 " << std::endl
     << "Until:  18:00,0.8,       !- Field 17 " << std::endl
     << "Until:  19:00,0.35,      !- Field 19 " << std::endl
     << "Until: 24:00,0.1,        !- Field 21 " << std::endl
     << "For: Saturday,           !- Field 23 " << std::endl
     << "Until:  5:00,0.05,       !- Field 24 " << std::endl
     << "Until:  6:00,0.1,        !- Field 26 " << std::endl
     << "Until:  7:00,0.15,       !- Field 28 " << std::endl
     << "Until:  9:00,0.21,       !- Field 30 " << std::endl
     << "Until: 14:00,0.25,       !- Field 32 " << std::endl
     << "Until: 17:00,0.2,        !- Field 34 " << std::endl
     << "Until: 18:00,0.15,       !- Field 36 " << std::endl
     << "Until: 24:00,0.1,        !- Field 38 " << std::endl
     << "For: Sunday Holidays WinterDesignDay AllOtherDays, !- Field 40 " << std::endl
     << "Until:  5:00,0.05,       !- Field 41 " << std::endl
     << "Until:  7:00,0.1,        !- Field 43 " << std::endl
     << "Until:  17:00,0.15,      !- Field 45 " << std::endl
     << "Until:  20:00,0.1,       !- Field 47 " << std::endl
     << "Until:  24:00,0.05;      !- Field 49";
  IdfObject schedule = IdfObject::load(ss.str(),
      *(IddFactory::instance().getObject(IddObjectType::Schedule_Compact))).get();
  ss.str("");
  IdfObjectVector idfObjectVector;
  if (!workspace.getObjectByTypeAndName(IddObjectType::ScheduleTypeLimits,*(scheduleTypeLimits.name())))
  {
    idfObjectVector.push_back(scheduleTypeLimits);
  }
  idfObjectVector.push_back(schedule);
  std::vector<WorkspaceObject> objects = workspace.addObjects(idfObjectVector);
  ASSERT_TRUE(objects.size() > 0);

  // get zones
  WorkspaceObjectVector zones = workspace.getObjectsByType(IddObjectType::Zone);
  EXPECT_EQ(static_cast<size_t>(6), zones.size());

  // for each zone
  for (const WorkspaceObject& zone : zones){

    // check zone name
    OptionalString zoneName = zone.getString(ZoneFields::Name);
    ASSERT_TRUE(zoneName);

    // remove the current lights for this zone
    WorkspaceObjectVector zoneLights = zone.getSources(IddObjectType(IddObjectType::Lights));
     if (istringEqual(*zoneName, "PLENUM-1")){
      EXPECT_EQ(static_cast<size_t>(0), zoneLights.size());
      EXPECT_TRUE(workspace.removeObjects(getHandles<WorkspaceObject>(zoneLights))); // trivially is able to remove the objects
      EXPECT_EQ(static_cast<size_t>(0), zone.getSources(IddObjectType(IddObjectType::Lights)).size());
    }else{
      EXPECT_EQ(static_cast<size_t>(1), zoneLights.size());
      EXPECT_TRUE(workspace.removeObjects(getHandles<WorkspaceObject>(zoneLights)));
      EXPECT_EQ(static_cast<size_t>(0), zone.getSources(IddObjectType(IddObjectType::Lights)).size());
    }

    // pretend we consult the rules here with space type and get a lpd and a schedule
    double lpd = 1.0;

    // make a new lights object with the correct lpd
    IdfObject newLights(IddObjectType::Lights);
    EXPECT_TRUE(newLights.setString(LightsFields::Name, *zoneName + "-Lights"));
    EXPECT_TRUE(newLights.setString(LightsFields::ZoneorZoneListName, *zoneName));
    EXPECT_TRUE(newLights.setString(LightsFields::ScheduleName, schedule.name().get()));
    EXPECT_TRUE(newLights.setString(LightsFields::DesignLevelCalculationMethod, "Watts/Area"));
    EXPECT_TRUE(newLights.setDouble(LightsFields::WattsperZoneFloorArea, lpd));
    // DLM@20100429: not handling the radiant fraction, etc

    // add lights object to the workspace
    EXPECT_TRUE(workspace.addObject(newLights));

    // get the lights again
    zoneLights = zone.getSources(IddObjectType(IddObjectType::Lights));
    ASSERT_EQ(static_cast<size_t>(1), zoneLights.size());

    OptionalString lightsName = zoneLights[0].getString(LightsFields::Name);
    ASSERT_TRUE(lightsName);
    EXPECT_TRUE(istringEqual(*zoneName + "-Lights", *lightsName));

    OptionalString lightsZoneName = zoneLights[0].getString(LightsFields::ZoneorZoneListName);
    ASSERT_TRUE(lightsZoneName);
    EXPECT_EQ(*zoneName, *lightsZoneName);

    OptionalString lightsScheduleName = zoneLights[0].getString(LightsFields::ScheduleName);
    ASSERT_TRUE(lightsScheduleName);
    EXPECT_EQ("LIT-SCHED-BldgAvg",*lightsScheduleName);

    OptionalString designLevelCalculationMethod = zoneLights[0].getString(LightsFields::DesignLevelCalculationMethod);
    ASSERT_TRUE(designLevelCalculationMethod);
    EXPECT_TRUE(istringEqual("Watts/Area", *designLevelCalculationMethod));

    EXPECT_FALSE(zoneLights[0].getDouble(LightsFields::LightingLevel));

    OptionalDouble wattsperZoneFloorArea = zoneLights[0].getDouble(LightsFields::WattsperZoneFloorArea);
    ASSERT_TRUE(wattsperZoneFloorArea);
    EXPECT_EQ(1.0, *wattsperZoneFloorArea);

    EXPECT_FALSE(zoneLights[0].getDouble(LightsFields::WattsperPerson));
  }

}

TEST_F(IdfFixture, Workspace_RemoveObject)
{
  Workspace workspace(epIdfFile, StrictnessLevel::Draft);

  ASSERT_TRUE(epIdfFile.objects().size() > 0);
  ASSERT_TRUE(workspace.objects().size() > 0);
  EXPECT_EQ(epIdfFile.objects().size(), workspace.objects().size());

  // delete object with only one reference
  WorkspaceObject object = workspace.objects()[0];
  Handle handle = object.handle();
  EXPECT_TRUE(workspace.removeObject(object.handle()));
  EXPECT_FALSE(workspace.getObject(handle));
  EXPECT_FALSE(object.isValid(workspace.strictnessLevel()));
}

TEST_F(IdfFixture, Workspace_RemoveObject2)
{
  Workspace workspace(epIdfFile, StrictnessLevel::Draft);

  ASSERT_TRUE(epIdfFile.objects().size() > 0);
  ASSERT_TRUE(workspace.objects().size() > 0);
  EXPECT_EQ(epIdfFile.objects().size(), workspace.objects().size());

  // delete object with two references

  WorkspaceObject object1 = workspace.objects()[0];
  WorkspaceObject object2 = object1;
  Handle handle = object1.handle();

  EXPECT_EQ(handle, object2.handle());

  EXPECT_TRUE(workspace.removeObject(object1.handle()));

  // removing disconnects the object, sets handle to Null.
  EXPECT_EQ(Handle(), object1.handle());
  EXPECT_EQ(Handle(), object2.handle());

  EXPECT_FALSE(workspace.getObject(handle));

  EXPECT_FALSE(object1.isValid(workspace.strictnessLevel()));
  EXPECT_FALSE(object2.isValid(workspace.strictnessLevel()));
}

// remove all zones and make sure no crash
TEST_F(IdfFixture, Workspace_RemoveAllZones)
{
  Workspace workspace(epIdfFile, StrictnessLevel::Draft);

  WorkspaceObjectVector zones = workspace.getObjectsByType(IddObjectType::Zone);
  EXPECT_EQ(static_cast<size_t>(6), zones.size());

  for (unsigned i = 0; i < zones.size(); ++i){
    EXPECT_TRUE(workspace.removeObject(zones[i].handle()));
  }

  zones = workspace.getObjectsByType(IddObjectType::Zone);
  EXPECT_EQ(static_cast<size_t>(0), zones.size());
}

TEST_F(IdfFixture, Workspace_SameNameNotReference)
{
  Workspace workspace(StrictnessLevel::Draft, IddFileType::EnergyPlus);

  OptionalWorkspaceObject w1 = workspace.addObject(IdfObject(IddObjectType::Output_Meter));
  ASSERT_TRUE(w1);
  OptionalWorkspaceObject object1 = workspace.getObject(w1->handle());
  ASSERT_TRUE(object1);

  OptionalWorkspaceObject w2 = workspace.addObject(IdfObject(IddObjectType::Output_Meter));
  ASSERT_TRUE(w2);
  OptionalWorkspaceObject object2 = workspace.getObject(w2->handle ());
  ASSERT_TRUE(object2);

  EXPECT_TRUE(w1->handle () != w2->handle ());

  ASSERT_TRUE(object1->getString(Output_MeterFields::KeyName));
  EXPECT_EQ("", object1->getString(Output_MeterFields::KeyName).get());

  ASSERT_TRUE(object2->getString(Output_MeterFields::KeyName));
  EXPECT_EQ("", object2->getString(Output_MeterFields::KeyName).get());

  EXPECT_TRUE(object1->setString(Output_MeterFields::KeyName, "Gas:Facility"));
  ASSERT_TRUE(object1->getString(Output_MeterFields::KeyName));
  EXPECT_EQ("Gas:Facility", object1->getString(Output_MeterFields::KeyName).get());

  EXPECT_TRUE(object2->setString(Output_MeterFields::KeyName, "Gas:Building"));
  ASSERT_TRUE(object2->getString(Output_MeterFields::KeyName));
  EXPECT_EQ("Gas:Building", object2->getString(Output_MeterFields::KeyName).get());

  EXPECT_TRUE(object2->setString(Output_MeterFields::KeyName, "Gas:Facility"));
  ASSERT_TRUE(object2->getString(Output_MeterFields::KeyName));
  EXPECT_EQ("Gas:Facility", object2->getString(Output_MeterFields::KeyName).get());

}

TEST_F(IdfFixture, Workspace_Clone) {
  Workspace workspace(epIdfFile,StrictnessLevel::None);
  Workspace clone = workspace.clone();
  WorkspaceObjectVector wsObjects, cloneObjects;

  wsObjects = workspace.getObjectsByType(IddObjectType::Building);
  ASSERT_TRUE(wsObjects.size() > static_cast<unsigned>(0));
  cloneObjects = clone.getObjectsByType(IddObjectType::Building);
  ASSERT_TRUE(cloneObjects.size() > static_cast<unsigned>(0));
  cloneObjects[0].setName("MyNewBuildingName");
  OptionalWorkspaceObject optObject = workspace.getObjectByTypeAndName(IddObjectType::Building,"MyNewBuildingName");
  EXPECT_FALSE(optObject);
  optObject = clone.getObjectByTypeAndName(IddObjectType::Building,"MyNewBuildingName");
  EXPECT_TRUE(optObject);

  HandleVector wsHandles, cloneHandles;
  wsHandles = workspace.handles();
  cloneHandles = clone.handles();
  EXPECT_EQ(wsHandles.size(),cloneHandles.size());
  EXPECT_FALSE(cloneHandles == wsHandles);
  HandleVector newOrder;
  for (int i = cloneHandles.size() - 1; i >= 0; --i) {
    newOrder.push_back(cloneHandles[i]);
  }
  clone.order().setDirectOrder(newOrder);
  cloneHandles = clone.handles();
  EXPECT_FALSE(cloneHandles == wsHandles);
}

TEST_F(IdfFixture,Workspace_Insert) {
  Workspace workspace(epIdfFile,StrictnessLevel::None);
  unsigned n = workspace.handles().size();

  // create IdfFile to insert
  IdfFile file;
  WorkspaceObjectVector wsObjects = workspace.getObjectsByType(IddObjectType::Schedule_Compact);
  ASSERT_TRUE(wsObjects.size() > static_cast<unsigned>(0));
  WorkspaceObjectVector wsTargets = wsObjects[0].targets();
  ASSERT_TRUE(wsTargets.size() > static_cast<unsigned>(0));
  ASSERT_TRUE(wsTargets[0].iddObject().type() == IddObjectType::ScheduleTypeLimits);
  Handle typeLimitsHandle = wsTargets[0].handle();
  IdfObject object = wsTargets[0].idfObject();
  file.addObject(object);
  EXPECT_EQ(1u,file.numObjects());
  object = wsObjects[0].idfObject();
  file.addObject(object);
  EXPECT_EQ(2u,file.numObjects());
  IdfObject clone = object.clone();
  clone.setName("SameScheduleNewName");
  file.addObject(clone);
  EXPECT_EQ(3u,file.numObjects());

  // insert. should have one new schedule.
  EXPECT_FALSE(workspace.insertObjects(file.objects()).empty());
  EXPECT_EQ(n+1,workspace.handles().size());
  OptionalWorkspaceObject clonedSchedule;
  clonedSchedule = workspace.getObjectByTypeAndName(IddObjectType::Schedule_Compact,"SameScheduleNewName");
  ASSERT_TRUE(clonedSchedule);
  WorkspaceObjectVector targets = clonedSchedule->targets();
  ASSERT_EQ(static_cast<unsigned>(1),targets.size());
  EXPECT_TRUE(targets[0].handle() == typeLimitsHandle);

}

#if 0 //Need to rewrite.
TEST_F(IdfFixture,Workspace_CloneSubset) {
  Workspace workspace(epIdfFile,StrictnessLevel::None);

  WorkspaceObjectVector wsObjects = workspace.getObjectsByType(IddObjectType::Schedule_Compact,true);
  unsigned n = wsObjects.size();
  ASSERT_TRUE(n > static_cast<unsigned>(0));
  n += workspace.getObjectsByType(IddObjectType::ScheduleTypeLimits).size();

  IddObjectTypeSet types;
  types.insert(IddObjectType::Schedule_Compact);
  types.insert(IddObjectType::ScheduleTypeLimits);

  Workspace compactSchedules = workspace.cloneSubset(types,StrictnessLevel::Final);
  EXPECT_EQ(n,compactSchedules.handles().size());
  WorkspaceObjectVector csObjects = compactSchedules.getObjectsByType(IddObjectType::Schedule_Compact,true);
  EXPECT_EQ(wsObjects.size(),csObjects.size());
  ASSERT_TRUE(csObjects.size() > static_cast<unsigned>(0));
  EXPECT_EQ(*wsObjects[0].name(),*csObjects[0].name());
  EXPECT_TRUE(csObjects[0].setName("NewName"));
  EXPECT_FALSE(*wsObjects[0].name() == *csObjects[0].name());
}
#endif

TEST_F(IdfFixture,Workspace_CloneAndRemove) {
  Workspace workspace(StrictnessLevel::None);
  IdfObject globalGeometryRules(IddObjectType::GlobalGeometryRules);
  workspace.addObject(globalGeometryRules);
  WorkspaceObjectVector objects = workspace.objects();
  ASSERT_EQ(static_cast<unsigned>(1), objects.size());
  EXPECT_EQ(IddObjectType::GlobalGeometryRules, workspace.objects()[0].iddObject().type().value());

  Workspace clone = workspace.clone();
  WorkspaceObjectVector cloneObjects = clone.objects();
  ASSERT_EQ(static_cast<unsigned>(1), cloneObjects.size());
  EXPECT_EQ(objects[0].iddObject().type(), cloneObjects[0].iddObject().type());
  EXPECT_NE(objects[0].handle(), cloneObjects[0].handle());

  HandleVector removeHandles;
  removeHandles.push_back(objects[0].handle());
  EXPECT_TRUE(workspace.removeObjects(removeHandles));
  EXPECT_EQ(static_cast<unsigned>(0), workspace.objects().size());

  HandleVector cloneRemoveHandles;
  cloneRemoveHandles.push_back(cloneObjects[0].handle());
  EXPECT_TRUE(clone.removeObjects(cloneRemoveHandles));
  EXPECT_EQ(static_cast<unsigned>(0), clone.objects().size());

}


TEST_F(IdfFixture,Workspace_BadObjects) {
  std::stringstream ss;

  ss << "SimulationControl, \n\
    No, !- Do Zone Sizing Calculation \n\
    No, !- Do System Sizing Calculation \n\
    No, !- Do Plant Sizing Calculation \n\
    No, !- Run Simulation for Sizing Periods \n\
    Yes; !- Run Simulation for Weather File Run Periods \n\
    \n\
  RunPeriod, \n\
    ,                        !- Name \n\
    1,                       !- Begin Month \n\
    1,                       !- Begin Day of Month \n\
    1,                       !- End Month \n\
    1,                       !- End Day of Month \n\
    Tuesday,                 !- Day of Week for Start Day \n\
    Yes,                     !- Use Weather File Holidays and Special Days \n\
    Yes,                     !- Use Weather File Daylight Saving Period \n\
    No,                      !- Apply Weekend Holiday Rule \n\
    Yes,                     !- Use Weather File Rain Indicators \n\
    Yes;                     !- Use Weather File Snow Indicators \n\
    \n\
  Building, \n\
    Building,                !- Name \n\
    30.,                     !- North Axis {deg} \n\
    City,                    !- Terrain \n\
    0.04,                    !- Loads Convergence Tolerance Value \n\
    0.4,                     !- Temperature Convergence Tolerance Value {deltaC} \n\
    FullExterior,            !- Solar Distribution \n\
    25;                      !- Maximum Number of Warmup Days \n\
    \n\
  GlobalGeometryRules, \n\
    UpperLeftCorner,         !- Starting Vertex Position \n\
    CounterClockWise,        !- Vertex Entry Direction \n\
    Relative, !- Coordinate System \n\
    Relative, !- Daylighting Coordinate System \n\
    Relative; !- Rectangular Surface Coordinate System\n\
    \n\
  Zone, \n\
    Zone,                    !- Name \n\
    30.,                     !- Direction of Relative North {deg} \n\
    10.,                     !- X Origin {m} \n\
    5.,                      !- Y Origin {m} \n\
    1.;                      !- Z Origin {m} \n\
    \n\
  Construction, !- bad object, not enough fields \n\
    Wall; \n\
    \n\
  BuildingSurface:Detailed, \n\
    Wall,                    !- Name \n\
    Wall,                    !- Surface Type \n\
    Wall,                    !- Construction Name \n\
    Zone,                    !- Zone Name \n\
    Outdoors,                !- Outside Boundary Condition \n\
    ,                        !- Outside Boundary Condition Object \n\
    SunExposed,              !- Sun Exposure \n\
    WindExposed,             !- Wind Exposure \n\
    0.50000,                 !- View Factor to Ground \n\
    4,                       !- Number of Vertices \n\
    0.0,0.0,1,  !- X,Y,Z ==> Vertex 1 {m} \n\
    0.0,0.0,0.0,  !- X,Y,Z ==> Vertex 2 {m} \n\
    10.0,0.0,0.0,  !- X,Y,Z ==> Vertex 3 {m} \n\
    10.0,0.0,1.;  !- X,Y,Z ==> Vertex 4 {m} \n";

  IdfFile idfFile;
  OptionalIdfFile oif = IdfFile::load(ss,IddFileType(IddFileType::EnergyPlus));
  ASSERT_TRUE(oif);
  idfFile = *oif;
  ASSERT_EQ(static_cast<unsigned>(7), idfFile.objects().size());

  Workspace workspace(idfFile);
  ASSERT_EQ(static_cast<unsigned>(7), workspace.objects().size());
}

TEST_F(IdfFixture,Workspace_SwapObjects) {
  std::stringstream original;
  original << "Zone, \n\
  Zone;                    !- Name \n\
Wall:Exterior, \n\
  Wall,                    !- Name \n\
  ,                        !- Construction Name \n\
  Zone,                    !- Zone Name \n\
  270,                     !- Azimuth Angle {deg} \n\
  90,                      !- Tilt Angle {deg} \n\
  0,                       !- Starting X Coordinate {m} \n\
  20,                      !- Starting Y Coordinate {m} \n\
  0,                       !- Starting Z Coordinate {m} \n\
  20,                      !- Length {m} \n\
  4;                       !- Height {m} \n\
Window, \n\
  Window,                  !- Name \n\
  ,                        !- Construction Name \n\
  Wall,                    !- Building Surface Name \n\
  ,                        !- Shading Control Name \n\
  ,                        !- Frame and Divider Name \n\
  ,                        !- Multiplier \n\
  1,                      !- Starting X Coordinate {m} \n\
  1,                      !- Starting Z Coordinate {m} \n\
  1,                      !- Length {m} \n\
  2;                      !- Height {m}";

  IdfFile idfFile = IdfFile::load(original,IddFileType(IddFileType::EnergyPlus)).get();
  Workspace workspace(idfFile);

  OptionalWorkspaceObject zone = workspace.getObjectByTypeAndName(IddObjectType::Zone, "Zone");
  OptionalWorkspaceObject wall = workspace.getObjectByTypeAndName(IddObjectType::Wall_Exterior, "Wall");
  OptionalWorkspaceObject window = workspace.getObjectByTypeAndName(IddObjectType::Window, "Window");

  ASSERT_TRUE(zone);
  ASSERT_TRUE(wall);
  ASSERT_TRUE(window);

  ASSERT_TRUE(zone->name());
  EXPECT_EQ("Zone", zone->name().get());
  ASSERT_EQ(static_cast<unsigned>(1), zone->sources().size());
  EXPECT_EQ(wall->handle(), zone->sources()[0].handle());

  ASSERT_TRUE(wall->name());
  EXPECT_EQ("Wall", wall->name().get());
  ASSERT_EQ(static_cast<unsigned>(1), wall->sources().size());
  EXPECT_EQ(window->handle(), wall->sources()[0].handle());
  ASSERT_TRUE(wall->getTarget(Wall_ExteriorFields::ZoneName));
  EXPECT_EQ(zone->handle(), wall->getTarget(Wall_ExteriorFields::ZoneName)->handle());

  ASSERT_TRUE(window->getString(WindowFields::BuildingSurfaceName));
  EXPECT_EQ("Wall", window->getString(WindowFields::BuildingSurfaceName).get());
  ASSERT_TRUE(window->getTarget(WindowFields::BuildingSurfaceName));
  ASSERT_TRUE(window->getTarget(WindowFields::BuildingSurfaceName)->name());
  EXPECT_EQ("Wall", window->getTarget(WindowFields::BuildingSurfaceName)->name().get());

  IdfObject newObject = IdfObject::load("BuildingSurface:Detailed, \n\
  Wall,        !- Name \n\
  Wall,        !- Surface Type \n\
  ,            !- Construction Name \n\
  Zone,        !- Zone Name \n\
  Outdoors,    !- Outside Boundary Condition \n\
  ,            !- Outside Boundary Condition Object \n\
  SunExposed,  !- Sun Exposure \n\
  WindExposed, !- Wind Exposure \n\
  ,            !- View Factor to Ground \n\
  4,           !- Number of Vertices \n\
  18.000000,   !- Vertex 1 X-coordinate {m} \n\
  56.000000,   !- Vertex 1 Y-coordinate {m} \n\
  4.000000,    !- Vertex 1 Z-coordinate {m} \n\
  18.000000,   !- Vertex 2 X-coordinate {m} \n\
  56.000000,   !- Vertex 2 Y-coordinate {m} \n\
  0.000000,    !- Vertex 2 Z-coordinate {m} \n\
  30.000000,   !- Vertex 3 X-coordinate {m} \n\
  40.000000,   !- Vertex 3 Y-coordinate {m} \n\
  0.000000,    !- Vertex 3 Z-coordinate {m} \n\
  30.000000,   !- Vertex 4 X-coordinate {m} \n\
  40.000000,   !- Vertex 4 Y-coordinate {m} \n\
  4.000000;    !- Vertex 4 Z-coordinate {m}").get();

  // should not be able to swap with window
  bool ok = workspace.swap(*window,newObject,true);
  EXPECT_FALSE(ok);
  ASSERT_TRUE(window);
  EXPECT_TRUE(newObject.iddObject().type() == IddObjectType::BuildingSurface_Detailed);
  EXPECT_TRUE(window->iddObject().type() == IddObjectType::Window);

  // should be able to swap with wall
  ok = workspace.swap(*wall,newObject,true);
  EXPECT_TRUE(ok);
  ASSERT_TRUE(wall);
  EXPECT_TRUE(wall->iddObject().type() == IddObjectType::BuildingSurface_Detailed);
  EXPECT_TRUE(newObject.iddObject().type() == IddObjectType::Wall_Exterior);

  // check that workspace is intact
  ASSERT_TRUE(zone->name());
  EXPECT_EQ("Zone", zone->name().get());
  ASSERT_EQ(static_cast<unsigned>(1), zone->sources().size());
  EXPECT_EQ(wall->handle(), zone->sources()[0].handle());

  ASSERT_TRUE(wall->name());
  EXPECT_EQ("Wall", wall->name().get());
  ASSERT_EQ(static_cast<unsigned>(1), wall->sources().size());
  EXPECT_EQ(window->handle(), wall->sources()[0].handle());
  ASSERT_TRUE(wall->getTarget(BuildingSurface_DetailedFields::ZoneName));
  EXPECT_EQ(zone->handle(), wall->getTarget(BuildingSurface_DetailedFields::ZoneName)->handle());

  ASSERT_TRUE(window->getString(WindowFields::BuildingSurfaceName));
  EXPECT_EQ("Wall", window->getString(WindowFields::BuildingSurfaceName).get());
  ASSERT_TRUE(window->getTarget(WindowFields::BuildingSurfaceName));
  ASSERT_TRUE(window->getTarget(WindowFields::BuildingSurfaceName)->name());
  EXPECT_EQ("Wall", window->getTarget(WindowFields::BuildingSurfaceName)->name().get());
}

// DLM@201008016: this test is the same as above but uses 'Wall:Adiabatic' instead of 'Wall:Exterior'
// 'Wall:Adiabatic' is not a member of the 'OutFaceEnvNames' reference group
// I have temporarily modified our EnergyPlus idd so this test will pass, however we need to reconcile
// with the real EnergyPlus idd at some point and remove 'OutFaceEnvNames' from 'Wall:Adiabatic' reference groups
TEST_F(IdfFixture,Workspace_SwapObjects2) {
  std::stringstream original;
  original << "Zone, \n\
  Zone;                    !- Name \n\
Wall:Adiabatic, \n\
  Wall,                    !- Name \n\
  ,                        !- Construction Name \n\
  Zone,                    !- Zone Name \n\
  270,                     !- Azimuth Angle {deg} \n\
  90,                      !- Tilt Angle {deg} \n\
  0,                       !- Starting X Coordinate {m} \n\
  20,                      !- Starting Y Coordinate {m} \n\
  0,                       !- Starting Z Coordinate {m} \n\
  20,                      !- Length {m} \n\
  4;                       !- Height {m} \n\
Window, \n\
  Window,                  !- Name \n\
  ,                        !- Construction Name \n\
  Wall,                    !- Building Surface Name \n\
  ,                        !- Shading Control Name \n\
  ,                        !- Frame and Divider Name \n\
  ,                        !- Multiplier \n\
  1,                      !- Starting X Coordinate {m} \n\
  1,                      !- Starting Z Coordinate {m} \n\
  1,                      !- Length {m} \n\
  2;                      !- Height {m}";

  IdfFile idfFile = IdfFile::load(original,IddFileType::EnergyPlus).get();
  Workspace workspace(idfFile);

  OptionalWorkspaceObject zone = workspace.getObjectByTypeAndName(IddObjectType::Zone, "Zone");
  OptionalWorkspaceObject wall = workspace.getObjectByTypeAndName(IddObjectType::Wall_Adiabatic, "Wall");
  OptionalWorkspaceObject window = workspace.getObjectByTypeAndName(IddObjectType::Window, "Window");

  ASSERT_TRUE(zone);
  ASSERT_TRUE(wall);
  ASSERT_TRUE(window);

  ASSERT_TRUE(zone->name());
  EXPECT_EQ("Zone", zone->name().get());
  ASSERT_EQ(static_cast<unsigned>(1), zone->sources().size());
  EXPECT_EQ(wall->handle(), zone->sources()[0].handle());

  ASSERT_TRUE(wall->name());
  EXPECT_EQ("Wall", wall->name().get());
  ASSERT_EQ(static_cast<unsigned>(1), wall->sources().size());
  EXPECT_EQ(window->handle(), wall->sources()[0].handle());
  ASSERT_TRUE(wall->getTarget(Wall_AdiabaticFields::ZoneName));
  EXPECT_EQ(zone->handle(), wall->getTarget(Wall_AdiabaticFields::ZoneName)->handle());

  ASSERT_TRUE(window->getString(WindowFields::BuildingSurfaceName));
  EXPECT_EQ("Wall", window->getString(WindowFields::BuildingSurfaceName).get());
  ASSERT_TRUE(window->getTarget(WindowFields::BuildingSurfaceName));
  ASSERT_TRUE(window->getTarget(WindowFields::BuildingSurfaceName)->name());
  EXPECT_EQ("Wall", window->getTarget(WindowFields::BuildingSurfaceName)->name().get());

  IdfObject newObject = IdfObject::load("BuildingSurface:Detailed, \n\
  Wall,        !- Name \n\
  Wall,        !- Surface Type \n\
  ,            !- Construction Name \n\
  Zone,        !- Zone Name \n\
  Adiabatic,   !- Outside Boundary Condition \n\
  ,            !- Outside Boundary Condition Object \n\
  NoSun,       !- Sun Exposure \n\
  NoWind,      !- Wind Exposure \n\
  ,            !- View Factor to Ground \n\
  4,           !- Number of Vertices \n\
  18.000000,   !- Vertex 1 X-coordinate {m} \n\
  56.000000,   !- Vertex 1 Y-coordinate {m} \n\
  4.000000,    !- Vertex 1 Z-coordinate {m} \n\
  18.000000,   !- Vertex 2 X-coordinate {m} \n\
  56.000000,   !- Vertex 2 Y-coordinate {m} \n\
  0.000000,    !- Vertex 2 Z-coordinate {m} \n\
  30.000000,   !- Vertex 3 X-coordinate {m} \n\
  40.000000,   !- Vertex 3 Y-coordinate {m} \n\
  0.000000,    !- Vertex 3 Z-coordinate {m} \n\
  30.000000,   !- Vertex 4 X-coordinate {m} \n\
  40.000000,   !- Vertex 4 Y-coordinate {m} \n\
  4.000000;    !- Vertex 4 Z-coordinate {m}").get();

  // should not be able to swap with window
  bool ok = workspace.swap(*window,newObject,true);
  EXPECT_FALSE(ok);
  ASSERT_TRUE(window);
  EXPECT_TRUE(newObject.iddObject().type() == IddObjectType::BuildingSurface_Detailed);
  EXPECT_TRUE(window->iddObject().type() == IddObjectType::Window);

  // should be able to swap with wall
  ok = workspace.swap(*wall,newObject,true);
  EXPECT_TRUE(ok);
  ASSERT_TRUE(wall);
  EXPECT_TRUE(wall->iddObject().type() == IddObjectType::BuildingSurface_Detailed);
  EXPECT_TRUE(newObject.iddObject().type() == IddObjectType::Wall_Adiabatic);

  // check that workspace is intact
  ASSERT_TRUE(zone->name());
  EXPECT_EQ("Zone", zone->name().get());
  ASSERT_EQ(static_cast<unsigned>(1), zone->sources().size());
  EXPECT_EQ(wall->handle(), zone->sources()[0].handle());

  ASSERT_TRUE(wall->name());
  EXPECT_EQ("Wall", wall->name().get());
  ASSERT_EQ(static_cast<unsigned>(1), wall->sources().size());
  EXPECT_EQ(window->handle(), wall->sources()[0].handle());
  ASSERT_TRUE(wall->getTarget(BuildingSurface_DetailedFields::ZoneName));
  EXPECT_EQ(zone->handle(), wall->getTarget(BuildingSurface_DetailedFields::ZoneName)->handle());

  ASSERT_TRUE(window->getString(WindowFields::BuildingSurfaceName));
  EXPECT_EQ("Wall", window->getString(WindowFields::BuildingSurfaceName).get());
  ASSERT_TRUE(window->getTarget(WindowFields::BuildingSurfaceName));
  ASSERT_TRUE(window->getTarget(WindowFields::BuildingSurfaceName)->name());
  EXPECT_EQ("Wall", window->getTarget(WindowFields::BuildingSurfaceName)->name().get());
}


// bad objects
TEST_F(IdfFixture,Workspace_ObjectsNotInIdd) {

  // bad object names, NotAWindow is not a class
  std::stringstream original;
  original << "Zone, \n\
  Zone;                    !- Name \n\
Wall:Adiabatic, \n\
  Wall,                    !- Name \n\
  ,                        !- Construction Name \n\
  Zone,                    !- Zone Name \n\
  270,                     !- Azimuth Angle {deg} \n\
  90,                      !- Tilt Angle {deg} \n\
  0,                       !- Starting X Coordinate {m} \n\
  20,                      !- Starting Y Coordinate {m} \n\
  0,                       !- Starting Z Coordinate {m} \n\
  20,                      !- Length {m} \n\
  4;                       !- Height {m} \n\
NotAWindow, \n\
  Window,                  !- Name \n\
  ,                        !- Construction Name \n\
  Wall,                    !- Building Surface Name \n\
  ,                        !- Shading Control Name \n\
  ,                        !- Frame and Divider Name \n\
  ,                        !- Multiplier \n\
  1,                      !- Starting X Coordinate {m} \n\
  1,                      !- Starting Z Coordinate {m} \n\
  1,                      !- Length {m} \n\
  2;                      !- Height {m}";

  // keeps bad object
  IdfFile idfFile = IdfFile::load(original, IddFileType::EnergyPlus).get();
  EXPECT_EQ(static_cast<unsigned>(3), idfFile.objects().size());
  EXPECT_TRUE(idfFile.isValid(StrictnessLevel::None));
  EXPECT_FALSE(idfFile.isValid(StrictnessLevel::Draft));
  EXPECT_FALSE(idfFile.isValid(StrictnessLevel::Final));

  Workspace workspace(idfFile, StrictnessLevel::None);
  EXPECT_EQ(static_cast<unsigned>(3), workspace.objects().size());
  EXPECT_TRUE(workspace.isValid(StrictnessLevel::None));
  EXPECT_FALSE(workspace.isValid(StrictnessLevel::Draft));
  EXPECT_FALSE(workspace.isValid(StrictnessLevel::Final));

  // example of how to fix the file
  ValidityReport report = idfFile.validityReport(StrictnessLevel::Draft);
  ASSERT_EQ(static_cast<unsigned>(1),report.numErrors());
  DataError error = report.nextError().get();
  ASSERT_TRUE(error.scope() == Scope::Object);
  ASSERT_TRUE(error.type() == DataErrorType::NoIdd);
  ASSERT_FALSE(error.objectIdentifier().isNull());
  OptionalIdfObject oObject = idfFile.getObject(error.objectIdentifier());
  IdfObject notAWindow = *oObject;
  EXPECT_TRUE(notAWindow.iddObject().type() == IddObjectType::Catchall);
  EXPECT_EQ("NotAWindow",notAWindow.getString(0).get()); // mistyped type saved in first field
  IdfObject isAWindow(IddObjectType::Window);
  for (unsigned i = 1, n = notAWindow.numFields(); i < n; ++i) {
    EXPECT_TRUE(isAWindow.setString(i-1,notAWindow.getString(i).get()));
  }
  /// \todo add idfFile.swap method
}

TEST_F(IdfFixture,HospitalBaseline) {
  openstudio::path p = resourcesPath()/toPath("energyplus/HospitalBaseline/in.idf");

  // open as IdfFile. time.
  openstudio::Time start = openstudio::Time::currentTime();
  OptionalIdfFile oIdfFile = IdfFile::load(p);
  openstudio::Time timingResult = openstudio::Time::currentTime() - start;

  // tests
  ASSERT_TRUE(oIdfFile);
  IdfFile idfFile = *oIdfFile;
  EXPECT_TRUE(idfFile.iddFileType() == IddFileType::EnergyPlus);

  // report on IdfFile
  LOG(Info,"Loaded HosptitalBaseline/in.idf into an IdfFile in " << timingResult
      << " s. The file has " << idfFile.objects().size() << " objects.");

  // construct workspace. time.
  start = openstudio::Time::currentTime();
  Workspace ws(idfFile);
  timingResult = openstudio::Time::currentTime() - start;

  // test draft validity
  ValidityReport report = ws.validityReport(StrictnessLevel::Draft);

  // report on file
  LOG(Info,"Created Workspace for HospitalBaseline/in.idf in " << timingResult << " s.");
  if (report.numErrors() > 0) {
    LOG(Info,"HospitalBaseline/in.idf is invalid at draft level. The ValidityReport follows."
        << std::endl << report);
  }
  else {
    LOG(Info,"HospitalBaseline/in.idf is valid to draft level.");
  }

}

TEST_F(IdfFixture,Workspace_InsertDifferentObjectSameName) {
  Workspace ws(StrictnessLevel::Draft, IddFileType::EnergyPlus);

  // two constructions with same name
  IdfObject construction1(IddObjectType::Construction);
  construction1.setName("Construction 1");
  IdfObject construction2(IddObjectType::Construction);
  construction2.setName(construction1.name().get());

  // create data
  IdfObject material1(IddObjectType::Material);
  material1.setName("Material 1");
  IdfObject material2(IddObjectType::Material);
  material2.setName("Material 2");
  IdfObject material3(IddObjectType::Material);
  material3.setName("Material 3");

  IdfExtensibleGroup layer = construction1.pushExtensibleGroup();
  ASSERT_FALSE(layer.empty());
  ASSERT_TRUE(layer.setString(ConstructionExtensibleFields::Layer,material1.name().get()));
  layer = construction1.pushExtensibleGroup();
  ASSERT_FALSE(layer.empty());
  ASSERT_TRUE(layer.setString(ConstructionExtensibleFields::Layer,material2.name().get()));
  layer = construction1.pushExtensibleGroup();
  ASSERT_FALSE(layer.empty());
  ASSERT_TRUE(layer.setString(ConstructionExtensibleFields::Layer,material1.name().get()));
  ASSERT_EQ(static_cast<unsigned>(3),construction1.numExtensibleGroups());

  layer = construction2.pushExtensibleGroup();
  ASSERT_FALSE(layer.empty());
  ASSERT_TRUE(layer.setString(ConstructionExtensibleFields::Layer,material3.name().get()));
  ASSERT_EQ(static_cast<unsigned>(3),construction1.numExtensibleGroups());
  ASSERT_EQ(static_cast<unsigned>(1),construction2.numExtensibleGroups());

  ASSERT_FALSE(construction1 == construction2);

  // add first construction
  IdfObjectVector objects;
  objects.push_back(construction1);
  objects.push_back(material1);
  objects.push_back(material2);
  WorkspaceObjectVector firstConstructionAdded = ws.addObjects(objects);
  ASSERT_TRUE(firstConstructionAdded.size() == objects.size());
  ASSERT_TRUE(firstConstructionAdded[0].name().get() == construction1.name().get());

  // insert same should do nothing
  WorkspaceObjectVector firstConstructionInserted = ws.insertObjects(objects);
  firstConstructionAdded = ws.sort(firstConstructionAdded);
  firstConstructionInserted = ws.sort(firstConstructionInserted);
  EXPECT_TRUE(firstConstructionAdded == firstConstructionInserted);
  EXPECT_EQ(objects.size(),ws.numObjects());

  // insert second should either fail or rename construction2 to avoid name conflict
  objects.clear();
  objects.push_back(construction2);
  objects.push_back(material3);
  WorkspaceObjectVector secondConstructionAdded = ws.insertObjects(objects);
  EXPECT_TRUE(secondConstructionAdded.empty() ||
              (secondConstructionAdded[0].name().get() != construction1.name().get()));
}

TEST_F(IdfFixture,Workspace_DefaultNames) {
  Workspace ws(StrictnessLevel::Draft, IddFileType::EnergyPlus);

  OptionalWorkspaceObject oObject = ws.addObject(IdfObject(IddObjectType::Building));
  ASSERT_TRUE(oObject);
  WorkspaceObject building = *oObject;
  ASSERT_TRUE(building.name());
  EXPECT_EQ("",building.name().get());

  oObject = ws.addObject(IdfObject(IddObjectType::Zone));
  ASSERT_TRUE(oObject);
  WorkspaceObject zone = *oObject;
  ASSERT_TRUE(zone.name());
  EXPECT_EQ("Zone 1",zone.name().get());

  oObject = ws.addObject(IdfObject(IddObjectType::Zone));
  ASSERT_TRUE(oObject);
  zone = *oObject;
  ASSERT_TRUE(zone.name());
  EXPECT_EQ("Zone 2",zone.name().get());

  oObject = ws.addObject(IdfObject(IddObjectType::Zone));
  ASSERT_TRUE(oObject);
  zone = *oObject;
  ASSERT_TRUE(zone.name());
  EXPECT_EQ("Zone 3",zone.name().get());

  EXPECT_TRUE(zone.setName("My Room"));

  oObject = ws.addObject(IdfObject(IddObjectType::Zone));
  ASSERT_TRUE(oObject);
  zone = *oObject;
  ASSERT_TRUE(zone.name());
  EXPECT_EQ("Zone 3",zone.name().get());

  zone.setName("Zone 352");

  oObject = ws.addObject(IdfObject(IddObjectType::Zone));
  ASSERT_TRUE(oObject);
  zone = *oObject;
  ASSERT_TRUE(zone.name());
  EXPECT_EQ("Zone 3",zone.name().get());

  EXPECT_EQ(static_cast<unsigned>(5),ws.numObjectsOfType(IddObjectType::Zone));
}

TEST_F(IdfFixture,Workspace_ComplexNames) {
  Workspace ws(StrictnessLevel::Draft, IddFileType::EnergyPlus);

  OptionalWorkspaceObject oObject = ws.addObject(IdfObject(IddObjectType::Building));
  ASSERT_TRUE(oObject);
  WorkspaceObject building = *oObject;
  ASSERT_TRUE(building.name());
  EXPECT_EQ("",building.name().get());

  oObject = ws.addObject(IdfObject(IddObjectType::Zone));
  ASSERT_TRUE(oObject);
  WorkspaceObject zone = *oObject;
  EXPECT_TRUE(zone.setName("My (least) favorite zone"));
  EXPECT_EQ("My (least) favorite zone",zone.name().get());

  oObject = ws.addObject(IdfObject(IddObjectType::Zone));
  ASSERT_TRUE(oObject);
  zone = *oObject;
  EXPECT_TRUE(zone.setName("My (least) favorite zone"));
  EXPECT_EQ("My (least) favorite zone 1",zone.name().get());

  oObject = ws.addObject(IdfObject(IddObjectType::Zone));
  ASSERT_TRUE(oObject);
  zone = *oObject;
  EXPECT_TRUE(zone.setName("My (least) favorite zone"));
  EXPECT_EQ("My (least) favorite zone 2",zone.name().get());

  oObject = ws.addObject(IdfObject(IddObjectType::Zone));
  ASSERT_TRUE(oObject);
  zone = *oObject;
  EXPECT_TRUE(zone.setName("My (!@#$^&*()least)(*&^$#@!) favorite zone"));
  EXPECT_EQ("My (!@#$^&*()least)(*&^$#@!) favorite zone",zone.name().get());

  oObject = ws.addObject(IdfObject(IddObjectType::Zone));
  ASSERT_TRUE(oObject);
  zone = *oObject;
  EXPECT_TRUE(zone.setName("My (!@#$^&*()least)(*&^$#@!) favorite zone"));
  EXPECT_EQ("My (!@#$^&*()least)(*&^$#@!) favorite zone 1",zone.name().get());

  oObject = ws.addObject(IdfObject(IddObjectType::Zone));
  ASSERT_TRUE(oObject);
  zone = *oObject;
  EXPECT_TRUE(zone.setName("My (!@#$^&*()least)(*&^$#@!) favorite zone 1"));
  EXPECT_EQ("My (!@#$^&*()least)(*&^$#@!) favorite zone 2", zone.name().get());

  oObject = ws.addObject(IdfObject(IddObjectType::Zone));
  ASSERT_TRUE(oObject);
  zone = *oObject;
  EXPECT_TRUE(zone.setName("My (!@#$^&*()least)(*&^$#@!) favorite zone            "));
  EXPECT_EQ("My (!@#$^&*()least)(*&^$#@!) favorite zone            ",zone.name().get());

  oObject = ws.addObject(IdfObject(IddObjectType::Zone));
  ASSERT_TRUE(oObject);
  zone = *oObject;
  EXPECT_TRUE(zone.setName("My (!@#$^&*()least)(*&^$#@!) favorite zone            "));
  EXPECT_EQ("My (!@#$^&*()least)(*&^$#@!) favorite zone             1",zone.name().get());

  oObject = ws.addObject(IdfObject(IddObjectType::Zone));
  ASSERT_TRUE(oObject);
  zone = *oObject;
  EXPECT_TRUE(zone.setName("My (!@#$^&*()least)(*&^$#@!) favorite zone             1"));
  EXPECT_EQ("My (!@#$^&*()least)(*&^$#@!) favorite zone             2", zone.name().get());

  oObject = ws.addObject(IdfObject(IddObjectType::Zone));
  ASSERT_TRUE(oObject);
  zone = *oObject;
  EXPECT_TRUE(zone.setName("            My (!@#$^&*()least)(*&^$#@!) favorite zone            "));
  EXPECT_EQ("            My (!@#$^&*()least)(*&^$#@!) favorite zone            ",zone.name().get());

  oObject = ws.addObject(IdfObject(IddObjectType::Zone));
  ASSERT_TRUE(oObject);
  zone = *oObject;
  EXPECT_TRUE(zone.setName("            My (!@#$^&*()least)(*&^$#@!) favorite zone            "));
  EXPECT_EQ("            My (!@#$^&*()least)(*&^$#@!) favorite zone             1",zone.name().get());

  oObject = ws.addObject(IdfObject(IddObjectType::Zone));
  ASSERT_TRUE(oObject);
  zone = *oObject;
  EXPECT_TRUE(zone.setName("            My (!@#$^&*()least)(*&^$#@!) favorite zone            1"));
  EXPECT_EQ("            My (!@#$^&*()least)(*&^$#@!) favorite zone            1",zone.name().get());

  oObject = ws.addObject(IdfObject(IddObjectType::Zone));
  ASSERT_TRUE(oObject);
  zone = *oObject;
  EXPECT_TRUE(zone.setName("            My (!@#$^&*()least)(*&^$#@!) favorite zone            1"));
  EXPECT_EQ("            My (!@#$^&*()least)(*&^$#@!) favorite zone            2",zone.name().get());

  oObject = ws.addObject(IdfObject(IddObjectType::Zone));
  ASSERT_TRUE(oObject);
  zone = *oObject;
  EXPECT_TRUE(zone.setName("My (!@#$^&*()least)(*&^$#@!) favorite zone 1 "));
  EXPECT_EQ("My (!@#$^&*()least)(*&^$#@!) favorite zone 1 ",zone.name().get());

  oObject = ws.addObject(IdfObject(IddObjectType::Zone));
  ASSERT_TRUE(oObject);
  zone = *oObject;
  EXPECT_TRUE(zone.setName("My (!@#$^&*()least)(*&^$#@!) favorite zone 1 "));
  EXPECT_EQ("My (!@#$^&*()least)(*&^$#@!) favorite zone 1  1",zone.name().get());

  oObject = ws.addObject(IdfObject(IddObjectType::Zone));
  ASSERT_TRUE(oObject);
  zone = *oObject;
  EXPECT_TRUE(zone.setName("My (!@#$^&*()least)(*&^$#@!) favorite zone "));
  EXPECT_EQ("My (!@#$^&*()least)(*&^$#@!) favorite zone ",zone.name().get());

  oObject = ws.addObject(IdfObject(IddObjectType::Zone));
  ASSERT_TRUE(oObject);
  zone = *oObject;
  EXPECT_TRUE(zone.setName("My (!@#$^&*()least)(*&^$#@!) favorite zone "));
  EXPECT_EQ("My (!@#$^&*()least)(*&^$#@!) favorite zone  1",zone.name().get());

  EXPECT_EQ(static_cast<unsigned>(17),ws.numObjectsOfType(IddObjectType::Zone));
}

TEST_F(IdfFixture, Workspace_SpecialNames) {
  Workspace ws(StrictnessLevel::Draft, IddFileType::EnergyPlus);

  OptionalWorkspaceObject oObject = ws.addObject(IdfObject(IddObjectType::Zone));
  ASSERT_TRUE(oObject);
  WorkspaceObject zone = *oObject;
  EXPECT_TRUE(zone.setName("Office, Hallway, and Other Zone"));
  EXPECT_EQ("Office, Hallway, and Other Zone", zone.name().get());

  oObject = ws.addObject(IdfObject(IddObjectType::Lights));
  ASSERT_TRUE(oObject);
  WorkspaceObject lights1 = *oObject;
  EXPECT_TRUE(lights1.setName("Office, Hallway, and Other Lights"));
  EXPECT_EQ("Office, Hallway, and Other Lights", lights1.name().get());

  oObject = ws.addObject(IdfObject(IddObjectType::Lights));
  ASSERT_TRUE(oObject);
  WorkspaceObject lights2 = *oObject;
  EXPECT_TRUE(lights2.setName("Office, Hallway, and Other Lights"));
  EXPECT_EQ("Office, Hallway, and Other Lights 1", lights2.name().get());

  bool exactMatch = true;
  std::vector<WorkspaceObject> objects = ws.getObjectsByName("Office, Hallway, and Other Lights", exactMatch);
  ASSERT_EQ(1u, objects.size());
  EXPECT_EQ(lights1.handle(), objects[0].handle());

  exactMatch = false;
  objects = ws.getObjectsByName("Office, Hallway, and Other Lights", exactMatch);
  ASSERT_EQ(2u, objects.size());

  EXPECT_TRUE(lights1.setString(LightsFields::ZoneorZoneListName, "Office, Hallway, and Other Zone"));
  ASSERT_TRUE(lights1.getTarget(LightsFields::ZoneorZoneListName));
  EXPECT_EQ(zone.handle(), lights1.getTarget(LightsFields::ZoneorZoneListName).get().handle());

  EXPECT_TRUE(lights2.setString(LightsFields::ZoneorZoneListName, "Office, Hallway, and Other Zone"));
  ASSERT_TRUE(lights2.getTarget(LightsFields::ZoneorZoneListName));
  EXPECT_EQ(zone.handle(), lights2.getTarget(LightsFields::ZoneorZoneListName).get().handle());
}

TEST_F(IdfFixture,Workspace_AvoidingNameClashes_IdfObject) {
  // create workspace with one object
  Workspace ws(StrictnessLevel::Draft, IddFileType::EnergyPlus);

  OptionalWorkspaceObject oZone = ws.addObject(IdfObject(IddObjectType::Zone));
  ASSERT_TRUE(oZone);
  ASSERT_TRUE(oZone->name());
  EXPECT_EQ("Zone 1",oZone->name().get());

  // add IdfObject with name clash
  IdfObject newZone(IddObjectType::Zone);
  newZone.setName(oZone->name().get());
  oZone = ws.addObject(newZone);
  ASSERT_TRUE(oZone);
  ASSERT_TRUE(oZone->name());

  // observe name change
  EXPECT_EQ("Zone 2",oZone->name().get());
}

TEST_F(IdfFixture,Workspace_AvoidingNameClashes_IdfObjects) {
  // create workspace with one construction
  Workspace ws(StrictnessLevel::Draft, IddFileType::EnergyPlus);

  OptionalWorkspaceObject owo = ws.addObject(IdfObject(IddObjectType::Construction));
  ASSERT_TRUE(owo);
  WorkspaceObject construction = *owo;
  EXPECT_EQ("Construction 1",construction.name().get());
  StringVector layer(1);
  owo = ws.addObject(IdfObject(IddObjectType::Material));
  ASSERT_TRUE(owo);
  WorkspaceObject material_1 = *owo;
  EXPECT_EQ("Material 1",material_1.name().get());
  layer[0] = material_1.name().get();
  EXPECT_EQ(static_cast<unsigned>(0),construction.numExtensibleGroups());
  IdfExtensibleGroup eg = construction.pushExtensibleGroup(layer);
  ASSERT_FALSE(eg.empty());

  // create IdfObjectVector with one construction
  IdfObjectVector idfObjects;
  idfObjects.push_back(IdfObject(IddObjectType::Construction));
  idfObjects.push_back(IdfObject(IddObjectType::Material));
  idfObjects.push_back(IdfObject(IddObjectType::Material));
  EXPECT_TRUE(idfObjects[0].setName("Construction 1"));
  EXPECT_TRUE(idfObjects[1].setName("Material 1"));
  EXPECT_TRUE(idfObjects[2].setName("Material 2"));
  EXPECT_EQ(static_cast<unsigned>(0),idfObjects[0].numExtensibleGroups());
  eg = idfObjects[0].pushExtensibleGroup(layer);
  ASSERT_FALSE(eg.empty());
  layer[0] = "Material 2";
  eg = idfObjects[0].pushExtensibleGroup(layer);
  ASSERT_FALSE(eg.empty());

  // add objects
  WorkspaceObjectVector newObjects = ws.addObjects(idfObjects);
  ASSERT_TRUE(newObjects.size() == idfObjects.size());

  // observe name changes and preservation of relationships
  EXPECT_EQ("Construction 2",newObjects[0].name().get());
  EXPECT_EQ("Material 3",newObjects[1].name().get());
  EXPECT_EQ("Material 2",newObjects[2].name().get());
  EXPECT_EQ(static_cast<unsigned>(2),newObjects[0].numExtensibleGroups());
  eg = newObjects[0].getExtensibleGroup(0);
  ASSERT_FALSE(eg.empty());
  WorkspaceExtensibleGroup weg = eg.cast<WorkspaceExtensibleGroup>();
  owo = weg.getTarget(0);
  ASSERT_TRUE(owo);
  EXPECT_TRUE(*owo == newObjects[1]);
  eg = newObjects[0].getExtensibleGroup(1);
  ASSERT_FALSE(eg.empty());
  weg = eg.cast<WorkspaceExtensibleGroup>();
  owo = weg.getTarget(0);
  ASSERT_TRUE(owo);
  EXPECT_TRUE(*owo == newObjects[2]);

  // check original construction
  EXPECT_EQ(static_cast<unsigned>(1),construction.numExtensibleGroups());
  eg = construction.getExtensibleGroup(0);
  ASSERT_FALSE(eg.empty());
  weg = eg.cast<WorkspaceExtensibleGroup>();
  owo = weg.getTarget(0);
  ASSERT_TRUE(owo);
  EXPECT_TRUE(*owo == material_1);
}

TEST_F(IdfFixture,Workspace_AvoidingNameClashes_Insert) {
  // create workspace with one construction that has one layer
  Workspace ws(StrictnessLevel::Draft, IddFileType::EnergyPlus);

  ws.order().setDirectOrder(HandleVector());
  OptionalWorkspaceObject owo = ws.addObject(IdfObject(IddObjectType::Construction));
  ASSERT_TRUE(owo);
  WorkspaceObject construction = *owo;
  EXPECT_EQ("Construction 1",construction.name().get());
  StringVector layer(1);
  owo = ws.addObject(IdfObject(IddObjectType::Material));
  ASSERT_TRUE(owo);
  WorkspaceObject material_1 = *owo;
  EXPECT_EQ("Material 1",material_1.name().get());
  layer[0] = material_1.name().get();
  EXPECT_EQ(static_cast<unsigned>(0),construction.numExtensibleGroups());
  IdfExtensibleGroup eg = construction.pushExtensibleGroup(layer);
  ASSERT_FALSE(eg.empty());

  // create workspace with one construction that has two layers
  Workspace toInsert = ws.clone();
  WorkspaceObjectVector constructions = toInsert.getObjectsByType(IddObjectType::Construction);
  ASSERT_EQ(static_cast<unsigned>(1),constructions.size());
  owo = toInsert.addObject(IdfObject(IddObjectType::Material));
  ASSERT_TRUE(owo);
  EXPECT_EQ("Material 2",owo->name().get());
  layer[0] = owo->name().get();
  EXPECT_EQ(static_cast<unsigned>(1),constructions[0].numExtensibleGroups());
  eg = constructions[0].pushExtensibleGroup(layer);
  ASSERT_FALSE(eg.empty());

  // insert workspace. expect material 1 merged, material 2 added, construction renamed
  WorkspaceObjectVector insertedObjects = ws.insertObjects(toInsert.objects(true));
  ASSERT_EQ(static_cast<unsigned>(3),insertedObjects.size());
  EXPECT_EQ("Construction 2",insertedObjects[0].name().get());
  EXPECT_FALSE(insertedObjects[0] == construction);
  EXPECT_EQ("Material 1",insertedObjects[1].name().get());
  EXPECT_TRUE(insertedObjects[1] == material_1);
  EXPECT_EQ("Material 2",insertedObjects[2].name().get());
  ASSERT_EQ(static_cast<unsigned>(1),construction.numExtensibleGroups());
  ASSERT_EQ(static_cast<unsigned>(2),insertedObjects[0].numExtensibleGroups());
}

TEST_F(IdfFixture,Workspace_AddAndInsertIdfObjects) {
  // base workspace
  Workspace ws(StrictnessLevel::Draft, IddFileType::EnergyPlus);

  OptionalWorkspaceObject owo = ws.addObject(IdfObject(IddObjectType::Construction));
  ASSERT_TRUE(owo);
  EXPECT_EQ("Construction 1",owo->name().get());
  owo = ws.addObject(IdfObject(IddObjectType::Construction));
  ASSERT_TRUE(owo);
  EXPECT_EQ("Construction 2",owo->name().get());
  EXPECT_EQ(2u,ws.numObjects());

  // objects to add and insert
  IdfObjectVector toAdd;
  toAdd.push_back(IdfObject(IddObjectType::Construction));
  toAdd[0].setName("Construction 1");
  IdfObjectVector toInsert;
  toInsert.push_back(IdfObject(IddObjectType::Construction));
  toInsert[0].setName("Construction 2");

  WorkspaceObjectVector result = ws.addAndInsertObjects(toAdd,toInsert);
  EXPECT_EQ(3u,ws.numObjects());
  ASSERT_EQ(2u,result.size());
  EXPECT_EQ("Construction 3",result[0].name().get());
  EXPECT_EQ("Construction 2",result[1].name().get());
  EXPECT_TRUE(result[1] == *owo);
}

TEST_F(IdfFixture,Workspace_AddAndInsertWorkspaceObjects) {
  // base workspace
  Workspace ws(StrictnessLevel::Draft, IddFileType::EnergyPlus);

  OptionalWorkspaceObject owo = ws.addObject(IdfObject(IddObjectType::Lights));
  ASSERT_TRUE(owo);
  WorkspaceObject originalLights = *owo;
  owo = ws.addObject(IdfObject(IddObjectType::Schedule_Compact));
  ASSERT_TRUE(owo);
  WorkspaceObject originalSchedule = *owo;
  originalLights.setPointer(LightsFields::ScheduleName,originalSchedule.handle());

  // model object-style clone into base
  WorkspaceObjectVector toAdd(1u,originalLights);
  WorkspaceObjectVector toInsert(1u,originalSchedule);
  WorkspaceObjectVector result = ws.addAndInsertObjects(toAdd,toInsert);
  ASSERT_EQ(2u,result.size());
  EXPECT_FALSE(result[0] == originalLights);
  EXPECT_EQ("Lights 2",result[0].name().get());
  EXPECT_TRUE(result[1] == originalSchedule);
}

TEST_F(IdfFixture,Workspace_LocateURLs) {
  // DLM: replace with OS_WeatherFileFields

  // create workspace with single TDV object in it
  Workspace ws;
  OptionalWorkspaceObject owo = ws.addObject(IdfObject(IddObjectType::OS_WeatherFile));
  ASSERT_TRUE(owo);
  WorkspaceObject epw = *owo;

  // set the url to be the absolute path on disk (to resources in build directory)
  openstudio::path absoluteEpwFilePath = resourcesPath()/toPath("utilities/Filetypes/USA_CO_Golden-NREL.724666_TMY3.epw");
  EXPECT_TRUE(epw.setString(OS_WeatherFileFields::Url, toString(absoluteEpwFilePath)));
  // check that setter worked as expected, and demonstrate getting field back out
  ASSERT_TRUE(epw.getString(OS_WeatherFileFields::Url));
  EXPECT_EQ(toString(absoluteEpwFilePath), epw.getString(OS_WeatherFileFields::Url).get());

  // test making path relative
  //
  std::vector<std::pair<QUrl, openstudio::path> > located = ws.locateUrls(std::vector<URLSearchPath>(), true, openstudio::path());

  EXPECT_EQ(epw.getString(OS_WeatherFileFields::Url).get(), "file:USA_CO_Golden-NREL.724666_TMY3.epw");
  ASSERT_EQ(located.size(), 1u);
  std::string a = located[0].first.toString().toStdString();
  std::string b = QUrl::fromLocalFile(openstudio::toQString(absoluteEpwFilePath)).toString().toStdString();
  EXPECT_TRUE(located[0].first == QUrl::fromLocalFile(openstudio::toQString(absoluteEpwFilePath)));
  EXPECT_EQ(located[0].second, openstudio::toPath("USA_CO_Golden-NREL.724666_TMY3.epw"));


  // test making path absolute again, by searching with an absolute search path
  std::vector<URLSearchPath> searchpaths;
  searchpaths.push_back(URLSearchPath(QUrl::fromLocalFile(toQString(absoluteEpwFilePath.parent_path()))));
  located = ws.locateUrls(searchpaths, false, openstudio::path());

  EXPECT_EQ(epw.getString(OS_WeatherFileFields::Url).get(),
            toString(QUrl::fromLocalFile(openstudio::toQString(absoluteEpwFilePath)).toString()));
  ASSERT_EQ(located.size(), 1u);
  EXPECT_TRUE(located[0].first == QUrl::fromLocalFile(openstudio::toQString(absoluteEpwFilePath)));
  EXPECT_EQ(located[0].second, absoluteEpwFilePath);

  // Make the path relative again
  ws.locateUrls(std::vector<URLSearchPath>(), true, openstudio::path());
  EXPECT_EQ(epw.getString(OS_WeatherFileFields::Url).get(), "file:USA_CO_Golden-NREL.724666_TMY3.epw");

  // test making path absolute again, this time, let's find it relative to a made-up location of the osm
  searchpaths.clear();
  searchpaths.push_back(URLSearchPath(QUrl::fromLocalFile("utilities/Filetypes/"), URLSearchPath::ToInputFile));
  located = ws.locateUrls(searchpaths, false, resourcesPath() / toPath("madeuposm.osm")); // give the search algo a relative place to start from

  EXPECT_EQ(epw.getString(OS_WeatherFileFields::Url).get(),
            toString(QUrl::fromLocalFile(openstudio::toQString(absoluteEpwFilePath)).toString()));
  ASSERT_EQ(located.size(), 1u);
  EXPECT_TRUE(located[0].first == QUrl::fromLocalFile(openstudio::toQString(absoluteEpwFilePath)));
  EXPECT_EQ(located[0].second, absoluteEpwFilePath);

  // Make the path relative again
  ws.locateUrls(std::vector<URLSearchPath>(), true, openstudio::path());
  EXPECT_EQ(epw.getString(OS_WeatherFileFields::Url).get(), "file:USA_CO_Golden-NREL.724666_TMY3.epw");

  // This time we want to fail, by providing it with a search that should work, but we give it the wrong object type
  searchpaths.clear();
  searchpaths.push_back(URLSearchPath(QUrl::fromLocalFile("utilities/Filetypes"), URLSearchPath::ToInputFile,
        IddObjectType::OS_Version));
  located = ws.locateUrls(searchpaths, false, resourcesPath() / toPath("madeuposm.osm")); // give the search algo a relative place to start from
  EXPECT_EQ(epw.getString(OS_WeatherFileFields::Url).get(), "file:USA_CO_Golden-NREL.724666_TMY3.epw");
  ASSERT_EQ(located.size(), 0u);

  // And finally, provide the correct field type for the search path, find it
  searchpaths.clear();
  searchpaths.push_back(URLSearchPath(QUrl::fromLocalFile("utilities/Filetypes"), URLSearchPath::ToInputFile,
        IddObjectType::OS_WeatherFile));
  located = ws.locateUrls(searchpaths, false, resourcesPath() / toPath("madeuposm.osm")); // give the search algo a relative place to start from

  EXPECT_EQ(epw.getString(OS_WeatherFileFields::Url).get(),
            toString(QUrl::fromLocalFile(openstudio::toQString(absoluteEpwFilePath)).toString()));
  ASSERT_EQ(located.size(), 1u);
  EXPECT_TRUE(located[0].first == QUrl::fromLocalFile(openstudio::toQString(absoluteEpwFilePath)));
  EXPECT_EQ(located[0].second, absoluteEpwFilePath);

}


TEST_F(IdfFixture, Workspace_GiveNames1) {

  std::string text = "\
    ! Header comment \n\
    \n\
    \n\
    RunPeriod,\n\
     ,                        !- Name\n\
    7,                        !- Begin Month\n\
    7,                        !- Begin Day of Month\n\
    7,                        !- End Month\n\
    7,                        !- End Day of Month\n\
    Tuesday,                  !- Day of Week for Start Day\n\
    Yes,                      !- Use Weather File Holidays and Special Days\n\
    Yes,                      !- Use Weather File Daylight Saving Period\n\
    No,                       !- Apply Weekend Holiday Rule\n\
    Yes,                      !- Use Weather File Rain Indicators\n\
    Yes;                      !- Use Weather File Snow Indicators\n\
  \n\
  RunPeriod,\n\
    ,                         !- Name\n\
    1,                        !- Begin Month\n\
    14,                       !- Begin Day of Month\n\
    1,                        !- End Month\n\
    14,                       !- End Day of Month\n\
    Tuesday,                  !- Day of Week for Start Day\n\
    Yes,                      !- Use Weather File Holidays and Special Days\n\
    Yes,                      !- Use Weather File Daylight Saving Period\n\
    No,                       !- Apply Weekend Holiday Rule\n\
    Yes,                      !- Use Weather File Rain Indicators\n\
    Yes;                      !- Use Weather File Snow Indicators\n\
  ";

  std::stringstream ss;

  ss << text;

  boost::optional<IdfFile> idf = IdfFile::load(ss, IddFileType::EnergyPlus);
  ASSERT_TRUE(idf);
  EXPECT_EQ(2u, idf->objects().size());
  std::vector<IdfObject> idfObjects = idf->getObjectsByType(IddObjectType::RunPeriod);
  ASSERT_EQ(2u, idfObjects.size());
  ASSERT_TRUE(idfObjects[0].name());
  EXPECT_EQ("", idfObjects[0].name().get());
  ASSERT_TRUE(idfObjects[1].name());
  EXPECT_EQ("", idfObjects[1].name().get());
  EXPECT_EQ("! Header comment", idf->header());

  Workspace workspace(*idf);
  EXPECT_EQ(2u, workspace.objects().size());
  std::vector<WorkspaceObject> workspaceObjects = workspace.getObjectsByType(IddObjectType::RunPeriod);
  ASSERT_EQ(2u, workspaceObjects.size());
  ASSERT_TRUE(workspaceObjects[0].name());
  ASSERT_TRUE(workspaceObjects[1].name());
  EXPECT_NE(workspaceObjects[0].name().get(), workspaceObjects[1].name().get());

  IdfFile idf2 = workspace.toIdfFile();
  EXPECT_EQ(2u, idf2.objects().size());
  idfObjects = idf2.getObjectsByType(IddObjectType::RunPeriod);
  ASSERT_EQ(2u, idfObjects.size());
  ASSERT_TRUE(idfObjects[0].name());
  ASSERT_TRUE(idfObjects[1].name());
  EXPECT_NE(idfObjects[0].name().get(), idfObjects[1].name().get());
  EXPECT_EQ(idf->header(), idf2.header());

}

TEST_F(IdfFixture, Workspace_GiveNames2) {

  std::string text = "\
  WaterUse:Connections,\n\
    Core_ZN Water Equipment,  !- Name\n\
    Core_ZN Water Equipment Water Inlet Node,  !- Inlet Node Name\n\
    Core_ZN Water Equipment Water Outlet Node,  !- Outlet Node Name\n\
    ,                         !- Supply Water Storage Tank Name\n\
    ,                         !- Reclamation Water Storage Tank Name\n\
    ,                         !- Hot Water Supply Temperature Schedule Name\n\
    ,                         !- Cold Water Supply Temperature Schedule Name\n\
    ,                         !- Drain Water Heat Exchanger Type\n\
    ,                         !- Drain Water Heat Exchanger Destination\n\
    ,                         !- Drain Water Heat Exchanger U-Factor Times Area {W/K}\n\
    Core_ZN Water Equipment;  !- Water Use Equipment 1 Name\n\
  \n\
  WaterUse:Equipment,\n\
    Core_ZN Water Equipment,  !- Name\n\
    ,                         !- End-Use Subcategory\n\
    3.15e-006,                !- Peak Flow Rate {m3/s}\n\
    BLDG_SWH_SCH,             !- Flow Rate Fraction Schedule Name\n\
    Water Equipment Temp Sched,  !- Target Temperature Schedule Name\n\
    Water Equipment Hot Supply Temp Sched,  !- Hot Water Supply Temperature Schedule Name\n\
    ,                         !- Cold Water Supply Temperature Schedule Name\n\
    Core_ZN,                  !- Zone Name\n\
    Water Equipment Sensible fract sched,  !- Sensible Fraction Schedule Name\n\
    Water Equipment Latent fract sched;  !- Latent Fraction Schedule Name\n\
  ";

  std::stringstream ss;

  ss << text;

  boost::optional<IdfFile> idf = IdfFile::load(ss, IddFileType::EnergyPlus);
  ASSERT_TRUE(idf);
  EXPECT_EQ(2u, idf->objects().size());
  std::vector<IdfObject> idfConnections = idf->getObjectsByType(IddObjectType::WaterUse_Connections);
  std::vector<IdfObject> idfEquipment = idf->getObjectsByType(IddObjectType::WaterUse_Equipment);
  ASSERT_EQ(1u, idfConnections.size());
  ASSERT_EQ(1u, idfEquipment.size());
  ASSERT_TRUE(idfConnections[0].name());
  EXPECT_EQ("Core_ZN Water Equipment", idfConnections[0].name().get());
  ASSERT_TRUE(idfEquipment[0].name());
  EXPECT_EQ("Core_ZN Water Equipment", idfEquipment[0].name().get());

  Workspace workspace(*idf);
  EXPECT_EQ(2u, workspace.objects().size());
  std::vector<WorkspaceObject> workspaceConnections = workspace.getObjectsByType(IddObjectType::WaterUse_Connections);
  std::vector<WorkspaceObject> workspaceEquipment = workspace.getObjectsByType(IddObjectType::WaterUse_Equipment);
  ASSERT_EQ(1u, workspaceConnections.size());
  ASSERT_EQ(1u, workspaceEquipment.size());
  ASSERT_TRUE(workspaceConnections[0].name());
  EXPECT_EQ("Core_ZN Water Equipment", workspaceConnections[0].name().get());
  ASSERT_TRUE(workspaceEquipment[0].name());
  EXPECT_EQ("Core_ZN Water Equipment", workspaceEquipment[0].name().get());

  IdfFile idf2 = workspace.toIdfFile();
  EXPECT_EQ(2u, idf2.objects().size());
  idfConnections = idf->getObjectsByType(IddObjectType::WaterUse_Connections);
  idfEquipment = idf->getObjectsByType(IddObjectType::WaterUse_Equipment);
  ASSERT_EQ(1u, idfConnections.size());
  ASSERT_EQ(1u, idfEquipment.size());
  ASSERT_TRUE(idfConnections[0].name());
  EXPECT_EQ("Core_ZN Water Equipment", idfConnections[0].name().get());
  ASSERT_TRUE(idfEquipment[0].name());
  EXPECT_EQ("Core_ZN Water Equipment", idfEquipment[0].name().get());

}

TEST_F(IdfFixture, Workspace_GiveNames3) {

  // one object, no name
  Workspace workspace(StrictnessLevel::Draft, IddFileType::EnergyPlus);
  IdfObjectVector idfObjects;
  idfObjects.push_back(IdfObject(IddObjectType::Zone));

  ASSERT_TRUE(idfObjects[0].name());
  EXPECT_EQ("", idfObjects[0].name().get());

  workspace.addObjects(idfObjects);

  ASSERT_EQ(1u, workspace.objects().size());
  ASSERT_TRUE(workspace.objects()[0].name());
  EXPECT_EQ("Zone 1", workspace.objects()[0].name().get());

  // one object, with name
  workspace = Workspace(StrictnessLevel::Draft, IddFileType::EnergyPlus);
  idfObjects.clear();
  idfObjects.push_back(IdfObject(IddObjectType::Zone));

  EXPECT_TRUE(idfObjects[0].setName("My Zone"));

  workspace.addObjects(idfObjects);

  ASSERT_EQ(1u, workspace.objects().size());
  ASSERT_TRUE(workspace.objects()[0].name());
  EXPECT_EQ("My Zone", workspace.objects()[0].name().get());

  // now do two objects with no names
  workspace = Workspace(StrictnessLevel::Draft, IddFileType::EnergyPlus);
  idfObjects.clear();
  idfObjects.push_back(IdfObject(IddObjectType::Zone));
  idfObjects.push_back(IdfObject(IddObjectType::Zone));

  ASSERT_TRUE(idfObjects[0].name());
  EXPECT_EQ("", idfObjects[0].name().get());
  ASSERT_TRUE(idfObjects[1].name());
  EXPECT_EQ("", idfObjects[1].name().get());

  workspace.addObjects(idfObjects);

  EXPECT_EQ(0u, workspace.objects().size());

  // now do two objects with the same name
  workspace = Workspace(StrictnessLevel::Draft, IddFileType::EnergyPlus);
  idfObjects.clear();
  idfObjects.push_back(IdfObject(IddObjectType::Zone));
  idfObjects.push_back(IdfObject(IddObjectType::Zone));

  EXPECT_TRUE(idfObjects[0].setName("My Zone"));
  EXPECT_TRUE(idfObjects[1].setName("My Zone"));

  workspace.addObjects(idfObjects);

  EXPECT_EQ(0u, workspace.objects().size());

  // now do two objects, potential name collisions
  workspace = Workspace(StrictnessLevel::Draft, IddFileType::EnergyPlus);
  idfObjects.clear();
  idfObjects.push_back(IdfObject(IddObjectType::Zone));
  idfObjects.push_back(IdfObject(IddObjectType::Zone));

  EXPECT_TRUE(idfObjects[0].setName("Zone 1"));
  ASSERT_TRUE(idfObjects[1].name());
  EXPECT_EQ("", idfObjects[1].name().get());

  workspace.addObjects(idfObjects);

  EXPECT_EQ(0u, workspace.objects().size());

  // now do two objects, potential name collisions
  workspace = Workspace(StrictnessLevel::Draft, IddFileType::EnergyPlus);
  idfObjects.clear();
  idfObjects.push_back(IdfObject(IddObjectType::Zone));
  idfObjects.push_back(IdfObject(IddObjectType::Zone));

  ASSERT_TRUE(idfObjects[0].name());
  EXPECT_EQ("", idfObjects[0].name().get());
  EXPECT_TRUE(idfObjects[1].setName("Zone 1"));

  workspace.addObjects(idfObjects);

  EXPECT_EQ(0u, workspace.objects().size());

  // now do two objects with no names, and one with a name
  workspace = Workspace(StrictnessLevel::Draft, IddFileType::EnergyPlus);
  idfObjects.clear();
  idfObjects.push_back(IdfObject(IddObjectType::Zone));
  idfObjects.push_back(IdfObject(IddObjectType::Zone));
  idfObjects.push_back(IdfObject(IddObjectType::Zone));

  ASSERT_TRUE(idfObjects[0].name());
  EXPECT_EQ("", idfObjects[0].name().get());
  ASSERT_TRUE(idfObjects[1].name());
  EXPECT_EQ("", idfObjects[1].name().get());
  EXPECT_TRUE(idfObjects[2].setName("My Zone"));

  workspace.addObjects(idfObjects);

  EXPECT_EQ(0u, workspace.objects().size());

  // now do two objects with same names, and one with a different name
  workspace = Workspace(StrictnessLevel::Draft, IddFileType::EnergyPlus);
  idfObjects.clear();
  idfObjects.push_back(IdfObject(IddObjectType::Zone));
  idfObjects.push_back(IdfObject(IddObjectType::Zone));
  idfObjects.push_back(IdfObject(IddObjectType::Zone));

  EXPECT_TRUE(idfObjects[0].setName("My Zone"));
  EXPECT_TRUE(idfObjects[1].setName("My Zone"));
  EXPECT_TRUE(idfObjects[2].setName("Other Zone"));

  workspace.addObjects(idfObjects);

  EXPECT_EQ(0u, workspace.objects().size());
}


TEST_F(IdfFixture, Workspace_GiveNames4) {

  // two objects, same name
  Workspace workspace(StrictnessLevel::None, IddFileType::EnergyPlus);
  IdfObjectVector idfObjects;
  idfObjects.push_back(IdfObject(IddObjectType::Zone));
  idfObjects.push_back(IdfObject(IddObjectType::Zone));

  EXPECT_TRUE(idfObjects[0].setName("My Zone"));
  EXPECT_TRUE(idfObjects[1].setName("My Zone"));

  workspace.addObjects(idfObjects);

  // automagically fixed?
  ASSERT_EQ(2u, workspace.objects().size());
  ASSERT_TRUE(workspace.objects()[0].name());
  EXPECT_EQ("My Zone", workspace.objects()[0].name().get());
  ASSERT_TRUE(workspace.objects()[1].name());
  EXPECT_EQ("My Zone 1", workspace.objects()[1].name().get());

  EXPECT_TRUE(workspace.isValid(StrictnessLevel::None));
  EXPECT_TRUE(workspace.isValid(StrictnessLevel::Draft));
}

TEST_F(IdfFixture, Workspace_AddObjects) {

std::string text = "\
  Construction,\n\
    Window Non-res Fixed,     !- Name\n\
    NonRes Fixed Assembly Window;  !- Outside Layer\n\
\n\
  WindowMaterial:SimpleGlazingSystem,\n\
    NonRes Fixed Assembly Window,  !- Name\n\
    3.23646,                  !- U-Factor {W/m2-K}\n\
    0.39,                     !- Solar Heat Gain Coefficient\n\
    ;                         !- Visible Transmittance\n\
    ";

  std::stringstream ss;
  ss << text;

  OptionalIdfFile idfFile = IdfFile::load(ss, IddFileType::EnergyPlus);
  ASSERT_TRUE(idfFile);
  Workspace workspace1(*idfFile);
  EXPECT_EQ(2u, workspace1.objects().size());
  ASSERT_EQ(1u, workspace1.getObjectsByType(IddObjectType::Construction).size());
  ASSERT_EQ(1u, workspace1.getObjectsByType(IddObjectType::WindowMaterial_SimpleGlazingSystem).size());
  ASSERT_TRUE(workspace1.getObjectsByType(IddObjectType::Construction)[0].getTarget(1));
  EXPECT_EQ(workspace1.getObjectsByType(IddObjectType::WindowMaterial_SimpleGlazingSystem)[0].handle(),
            workspace1.getObjectsByType(IddObjectType::Construction)[0].getTarget(1)->handle());

  std::vector<IdfObject> idfObjects;
  for (WorkspaceObject workspaceObject : workspace1.objects()){
    idfObjects.push_back(workspaceObject.idfObject());
  }

  // add workspace objects to workspace of none strictness
  {
    Workspace workspace2(StrictnessLevel::None, IddFileType::EnergyPlus);
    WorkspaceObjectVector workspaceObjects = workspace1.objects();
    workspace2.addObjects(workspaceObjects);
    EXPECT_EQ(2u, workspace2.objects().size());
    ASSERT_EQ(1u, workspace2.getObjectsByType(IddObjectType::Construction).size());
    ASSERT_EQ(1u, workspace2.getObjectsByType(IddObjectType::WindowMaterial_SimpleGlazingSystem).size());
    ASSERT_TRUE(workspace2.getObjectsByType(IddObjectType::Construction)[0].getTarget(1));
    EXPECT_EQ(workspace2.getObjectsByType(IddObjectType::WindowMaterial_SimpleGlazingSystem)[0].handle(),
              workspace2.getObjectsByType(IddObjectType::Construction)[0].getTarget(1)->handle());
  }

  // add workspace objects to workspace of draft strictness
  {
    Workspace workspace2(StrictnessLevel::Draft, IddFileType::EnergyPlus);
    WorkspaceObjectVector workspaceObjects = workspace1.objects();
    workspace2.addObjects(workspaceObjects);
    EXPECT_EQ(2u, workspace2.objects().size());
    ASSERT_EQ(1u, workspace2.getObjectsByType(IddObjectType::Construction).size());
    ASSERT_EQ(1u, workspace2.getObjectsByType(IddObjectType::WindowMaterial_SimpleGlazingSystem).size());
    ASSERT_TRUE(workspace2.getObjectsByType(IddObjectType::Construction)[0].getTarget(1));
    EXPECT_EQ(workspace2.getObjectsByType(IddObjectType::WindowMaterial_SimpleGlazingSystem)[0].handle(),
              workspace2.getObjectsByType(IddObjectType::Construction)[0].getTarget(1)->handle());
  }

  // add idf objects to workspace of none strictness
  {
    Workspace workspace3(StrictnessLevel::None, IddFileType::EnergyPlus);
    workspace3.addObjects(idfObjects);
    EXPECT_EQ(2u, workspace3.objects().size());
    ASSERT_EQ(1u, workspace3.getObjectsByType(IddObjectType::Construction).size());
    ASSERT_EQ(1u, workspace3.getObjectsByType(IddObjectType::WindowMaterial_SimpleGlazingSystem).size());
    ASSERT_TRUE(workspace3.getObjectsByType(IddObjectType::Construction)[0].getTarget(1));
    EXPECT_EQ(workspace3.getObjectsByType(IddObjectType::WindowMaterial_SimpleGlazingSystem)[0].handle(),
              workspace3.getObjectsByType(IddObjectType::Construction)[0].getTarget(1)->handle());
  }

  // add idf objects to workspace of draft strictness
  {
    Workspace workspace3(StrictnessLevel::Draft, IddFileType::EnergyPlus);
    workspace3.addObjects(idfObjects);
    EXPECT_EQ(2u, workspace3.objects().size());
    ASSERT_EQ(1u, workspace3.getObjectsByType(IddObjectType::Construction).size());
    ASSERT_EQ(1u, workspace3.getObjectsByType(IddObjectType::WindowMaterial_SimpleGlazingSystem).size());
    ASSERT_TRUE(workspace3.getObjectsByType(IddObjectType::Construction)[0].getTarget(1));
    EXPECT_EQ(workspace3.getObjectsByType(IddObjectType::WindowMaterial_SimpleGlazingSystem)[0].handle(),
              workspace3.getObjectsByType(IddObjectType::Construction)[0].getTarget(1)->handle());
  }
}

TEST_F(IdfFixture, Workspace_AddObjects2) {
  // Test added to demonstrate false warning message.

  Workspace workspace(StrictnessLevel::Draft, IddFileType::EnergyPlus);
  IdfObjectVector idfObjects;
  idfObjects.push_back(IdfObject(IddObjectType::Zone));
  EXPECT_TRUE(idfObjects[0].setName("OS:ThermalZone (Kit and Bath)"));

  std::vector<WorkspaceObject> addedObjects = workspace.addObjects(idfObjects);
  ASSERT_EQ(1u, addedObjects.size());
  ASSERT_EQ(IddObjectType::Zone, addedObjects[0].iddObject().type().value());
  EXPECT_EQ("OS:ThermalZone (Kit and Bath)", addedObjects[0].name().get());
  WorkspaceObject zoneObject = addedObjects[0];

  idfObjects.clear();
  idfObjects.push_back(IdfObject(IddObjectType::Sizing_Zone));
  EXPECT_TRUE(idfObjects[0].setString(Sizing_ZoneFields::ZoneorZoneListName, "OS:ThermalZone (Kit and Bath)"));
  EXPECT_TRUE(idfObjects[0].setString(Sizing_ZoneFields::ZoneCoolingDesignSupplyAirTemperatureInputMethod, "SupplyAirTemperature"));
  EXPECT_TRUE(idfObjects[0].setDouble(Sizing_ZoneFields::ZoneCoolingDesignSupplyAirTemperature, 12.8));
  EXPECT_TRUE(idfObjects[0].setDouble(Sizing_ZoneFields::ZoneCoolingDesignSupplyAirTemperatureDifference, 11.1));
  EXPECT_TRUE(idfObjects[0].setString(Sizing_ZoneFields::ZoneHeatingDesignSupplyAirTemperatureInputMethod, "SupplyAirTemperature"));
  EXPECT_TRUE(idfObjects[0].setDouble(Sizing_ZoneFields::ZoneHeatingDesignSupplyAirTemperature, 50));
  EXPECT_TRUE(idfObjects[0].setDouble(Sizing_ZoneFields::ZoneCoolingDesignSupplyAirHumidityRatio, 0.008));
  EXPECT_TRUE(idfObjects[0].setDouble(Sizing_ZoneFields::ZoneHeatingDesignSupplyAirHumidityRatio, 0.008));

  StringStreamLogSink sink;
  sink.setLogLevel(Warn);

  addedObjects = workspace.addObjects(idfObjects);
  ASSERT_EQ(1u, addedObjects.size());
  ASSERT_EQ(IddObjectType::Sizing_Zone, addedObjects[0].iddObject().type().value());
  ASSERT_TRUE(addedObjects[0].getString(Sizing_ZoneFields::ZoneorZoneListName));
  EXPECT_EQ("OS:ThermalZone (Kit and Bath)", addedObjects[0].getString(Sizing_ZoneFields::ZoneorZoneListName).get());
  EXPECT_TRUE(addedObjects[0].getTarget(Sizing_ZoneFields::ZoneorZoneListName));
  if (OptionalWorkspaceObject target = addedObjects[0].getTarget(Sizing_ZoneFields::ZoneorZoneListName)) {
    EXPECT_TRUE(target.get() == zoneObject);
  }

  EXPECT_EQ(0u, sink.logMessages().size());
}

TEST_F(IdfFixture, Workspace_AddObjects3) {
  // Test added to demonstrate duplicate warning message.

  Workspace workspace(StrictnessLevel::Draft, IddFileType::EnergyPlus);
  IdfObjectVector idfObjects;
  idfObjects.push_back(IdfObject(IddObjectType::Zone));
  EXPECT_TRUE(idfObjects[0].setName("OS:ThermalZone (Kit and Bath)"));

  std::vector<WorkspaceObject> addedObjects = workspace.addObjects(idfObjects);
  ASSERT_EQ(1u, addedObjects.size());
  ASSERT_EQ(IddObjectType::Zone, addedObjects[0].iddObject().type().value());
  EXPECT_EQ("OS:ThermalZone (Kit and Bath)", addedObjects[0].name().get());

  idfObjects.clear();
  idfObjects.push_back(IdfObject(IddObjectType::Sizing_Zone));
  EXPECT_TRUE(idfObjects[0].setString(Sizing_ZoneFields::ZoneorZoneListName, "Some other zone"));
  EXPECT_TRUE(idfObjects[0].setString(Sizing_ZoneFields::ZoneCoolingDesignSupplyAirTemperatureInputMethod, "SupplyAirTemperature"));
  EXPECT_TRUE(idfObjects[0].setDouble(Sizing_ZoneFields::ZoneCoolingDesignSupplyAirTemperature, 12.8));
  EXPECT_TRUE(idfObjects[0].setDouble(Sizing_ZoneFields::ZoneCoolingDesignSupplyAirTemperatureDifference, 11.1));
  EXPECT_TRUE(idfObjects[0].setString(Sizing_ZoneFields::ZoneHeatingDesignSupplyAirTemperatureInputMethod, "SupplyAirTemperature"));
  EXPECT_TRUE(idfObjects[0].setDouble(Sizing_ZoneFields::ZoneHeatingDesignSupplyAirTemperature, 50));
  EXPECT_TRUE(idfObjects[0].setDouble(Sizing_ZoneFields::ZoneCoolingDesignSupplyAirHumidityRatio, 0.008));
  EXPECT_TRUE(idfObjects[0].setDouble(Sizing_ZoneFields::ZoneHeatingDesignSupplyAirHumidityRatio, 0.008));

  StringStreamLogSink sink;
  sink.setLogLevel(Warn);

  addedObjects = workspace.addObjects(idfObjects);
  ASSERT_EQ(1u, addedObjects.size());
  ASSERT_EQ(IddObjectType::Sizing_Zone, addedObjects[0].iddObject().type().value());
  ASSERT_TRUE(addedObjects[0].getString(Sizing_ZoneFields::ZoneorZoneListName));
  EXPECT_EQ("", addedObjects[0].getString(Sizing_ZoneFields::ZoneorZoneListName).get());
  EXPECT_EQ(1u, sink.logMessages().size());
}


TEST_F(IdfFixture, Workspace_Signals)
{
  openstudio::Application::instance().application(false);
  IdfFile idfFile(IddFileType::EnergyPlus);
  Workspace workspace(idfFile);

  WorkspaceReciever* reciever = nullptr;

  ASSERT_NO_THROW( reciever = new WorkspaceReciever(workspace) );

  EXPECT_FALSE(reciever->m_objectImpl);
  EXPECT_FALSE(reciever->m_iddObjectType);
  EXPECT_FALSE(reciever->m_handle);

  WorkspaceObject object = workspace.addObject(IdfObject(IddObjectType::Zone)).get();
  Handle handle = object.handle();

  ASSERT_TRUE(reciever->m_objectImpl.get());
  ASSERT_TRUE(reciever->m_iddObjectType);
  EXPECT_EQ(IddObjectType::Zone, reciever->m_iddObjectType->value());
  ASSERT_TRUE(reciever->m_handle);
  EXPECT_EQ(handle, reciever->m_handle.get());

  reciever->clear();

  EXPECT_FALSE(reciever->m_objectImpl);
  EXPECT_FALSE(reciever->m_iddObjectType);
  EXPECT_FALSE(reciever->m_handle);

  object.remove();

  ASSERT_TRUE(reciever->m_objectImpl.get());
  ASSERT_TRUE(reciever->m_iddObjectType);
  EXPECT_EQ(IddObjectType::Zone, reciever->m_iddObjectType->value());
  ASSERT_TRUE(reciever->m_handle);
  EXPECT_EQ(handle, reciever->m_handle.get());

  delete reciever;
}

TEST_F(IdfFixture,Workspace_Swap) {
  Workspace ws1, ws2;
  ws1.addObject(IdfObject(IddObjectType::OS_Building));
  ws2.addObject(IdfObject(IddObjectType::OS_Lights_Definition));

  ws1.swap(ws2);

  ASSERT_EQ(1u,ws1.objects().size());
  ASSERT_EQ(1u,ws2.objects().size());

  EXPECT_EQ(IddObjectType(IddObjectType::OS_Lights_Definition),ws1.objects()[0].iddObject().type());
  EXPECT_EQ(IddObjectType(IddObjectType::OS_Building),ws2.objects()[0].iddObject().type());
}

TEST_F(IdfFixture, Workspace_DaylightingControlsZoneName)
{
  // DaylightingControls is an odd object with field 0 not being a name field
  Workspace ws(StrictnessLevel::Draft, IddFileType::EnergyPlus);

  boost::optional<WorkspaceObject> zone = ws.addObject(IdfObject(IddObjectType::Zone));
  boost::optional<WorkspaceObject> daylightingControl = ws.addObject(IdfObject(IddObjectType::Daylighting_Controls));

  ASSERT_TRUE(zone);
  ASSERT_TRUE(daylightingControl);

  zone->setName("Zone 1");
  EXPECT_FALSE(daylightingControl->getString(1,false,true));
  EXPECT_TRUE(daylightingControl->setPointer(1, zone->handle()));
  ASSERT_TRUE(daylightingControl->getString(1,false,true));
  EXPECT_EQ("Zone 1", daylightingControl->getString(1,false,true).get());

  EXPECT_TRUE(daylightingControl->setString(1, ""));
  EXPECT_FALSE(daylightingControl->getString(1,false,true));

  EXPECT_TRUE(daylightingControl->setString(1, "Zone 1"));
  ASSERT_TRUE(daylightingControl->getString(1,false,true));
  EXPECT_EQ("Zone 1", daylightingControl->getString(1,false,true).get());

}

TEST_F(IdfFixture, Workspace_NextName)
{
  Workspace ws(StrictnessLevel::Draft, IddFileType::EnergyPlus);

  EXPECT_EQ("Zone 1", ws.nextName(IddObjectType::Zone, false));
  EXPECT_EQ("Zone 1", ws.nextName(IddObjectType::Zone, true));

  boost::optional<WorkspaceObject> zone = ws.addObject(IdfObject(IddObjectType::Zone));
  ASSERT_TRUE(zone);
  EXPECT_EQ("Zone 1", zone->name().get());
  EXPECT_EQ("Zone 2", ws.nextName(IddObjectType::Zone, false));
  EXPECT_EQ("Zone 2", ws.nextName(IddObjectType::Zone, true));

  zone->setName("Zone 2");
  EXPECT_EQ("Zone 2", zone->name().get());
  EXPECT_EQ("Zone 3", ws.nextName(IddObjectType::Zone, false));
  EXPECT_EQ("Zone 1", ws.nextName(IddObjectType::Zone, true));

  zone->setName("{af63d539-6e16-4fd1-a10e-dafe3793373b}");
  EXPECT_EQ("{af63d539-6e16-4fd1-a10e-dafe3793373b}", zone->name().get());
  EXPECT_EQ("Zone 1", ws.nextName(IddObjectType::Zone, false));
  EXPECT_EQ("Zone 1", ws.nextName(IddObjectType::Zone, true));

  zone->setName("Zone 1");

  zone = ws.addObject(IdfObject(IddObjectType::Zone));
  zone->setName("Zone 2");
  EXPECT_EQ("Zone 2", zone->name().get());
  EXPECT_EQ("Zone 3", ws.nextName(IddObjectType::Zone, false));
  EXPECT_EQ("Zone 3", ws.nextName(IddObjectType::Zone, true));

  zone->setName("{af63d539-6e16-4fd1-a10e-dafe3793373b}");
  EXPECT_EQ("{af63d539-6e16-4fd1-a10e-dafe3793373b}", zone->name().get());
  EXPECT_EQ("Zone 2", ws.nextName(IddObjectType::Zone, false));
  EXPECT_EQ("Zone 2", ws.nextName(IddObjectType::Zone, true));
}

TEST_F(IdfFixture, Workspace_GetObjectsByNameUUID)
{
  Workspace ws(StrictnessLevel::Draft, IddFileType::EnergyPlus);

  boost::optional<WorkspaceObject> zone = ws.addObject(IdfObject(IddObjectType::Zone));
  EXPECT_EQ("Zone 1", zone->name().get());
  EXPECT_EQ(0u, ws.getObjectsByName("Zone", true).size());
  EXPECT_EQ(1u, ws.getObjectsByName("Zone", false).size());
  EXPECT_EQ(1u, ws.getObjectsByName("Zone 1", true).size());
  EXPECT_EQ(1u, ws.getObjectsByName("Zone 1", false).size());
  EXPECT_EQ(0u, ws.getObjectsByName("Zone 2", true).size());
  EXPECT_EQ(1u, ws.getObjectsByName("Zone 2", false).size());
  EXPECT_EQ(0u, ws.getObjectsByName("{af63d539-6e16-4fd1-a10e-dafe3793373b}", true).size());
  EXPECT_EQ(0u, ws.getObjectsByName("{af63d539-6e16-4fd1-a10e-dafe3793373b}", false).size());

  zone->setName("Zone 2");
  EXPECT_EQ("Zone 2", zone->name().get());
  EXPECT_EQ(0u, ws.getObjectsByName("Zone", true).size());
  EXPECT_EQ(1u, ws.getObjectsByName("Zone", false).size());
  EXPECT_EQ(0u, ws.getObjectsByName("Zone 1", true).size());
  EXPECT_EQ(1u, ws.getObjectsByName("Zone 1", false).size());
  EXPECT_EQ(1u, ws.getObjectsByName("Zone 2", true).size());
  EXPECT_EQ(1u, ws.getObjectsByName("Zone 2", false).size());
  EXPECT_EQ(0u, ws.getObjectsByName("{af63d539-6e16-4fd1-a10e-dafe3793373b}", true).size());
  EXPECT_EQ(0u, ws.getObjectsByName("{af63d539-6e16-4fd1-a10e-dafe3793373b}", false).size());

  zone->setName("{af63d539-6e16-4fd1-a10e-dafe3793373b}");
  EXPECT_EQ("{af63d539-6e16-4fd1-a10e-dafe3793373b}", zone->name().get());
  EXPECT_EQ(0u, ws.getObjectsByName("Zone", true).size());
  EXPECT_EQ(0u, ws.getObjectsByName("Zone", false).size());
  EXPECT_EQ(0u, ws.getObjectsByName("Zone 1", true).size());
  EXPECT_EQ(0u, ws.getObjectsByName("Zone 1", false).size());
  EXPECT_EQ(0u, ws.getObjectsByName("Zone 2", true).size());
  EXPECT_EQ(0u, ws.getObjectsByName("Zone 2", false).size());
  EXPECT_EQ(1u, ws.getObjectsByName("{af63d539-6e16-4fd1-a10e-dafe3793373b}", true).size());
  EXPECT_EQ(1u, ws.getObjectsByName("{af63d539-6e16-4fd1-a10e-dafe3793373b}", false).size());
}
