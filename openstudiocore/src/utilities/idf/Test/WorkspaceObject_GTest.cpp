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
#include "IdfFixture.hpp"
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/Building_FieldEnums.hxx>
#include <utilities/idd/Zone_FieldEnums.hxx>
#include <utilities/idd/Lights_FieldEnums.hxx>
#include <utilities/idd/Schedule_Compact_FieldEnums.hxx>
#include <utilities/idd/OS_DaylightingDevice_Shelf_FieldEnums.hxx>
#include <utilities/idd/OS_SetpointManager_MixedAir_FieldEnums.hxx>

#include "../WorkspaceExtensibleGroup.hpp"
#include "../IdfFile.hpp"
#include "../IdfObject.hpp"
#include "../Workspace.hpp"
#include "../WorkspaceObject.hpp"

#include "../../core/Optional.hpp"

using namespace openstudio;

TEST_F(IdfFixture,WorkspaceObject_Construction) {
  // get vector of IdfFile objects as fodder for tests
  IdfObjectVector idfObjects = epIdfFile.objects();
  OptionalWorkspaceObject w;
  OptionalHandleVector hv;

  // construct empty workspace, and set Idd to be EnergyPlusIdd
  StrictnessLevel level(StrictnessLevel::Draft);
  IddFileType fileType(IddFileType::EnergyPlus);
  Workspace ws(level,fileType);

  // add object with no pointers. should be successful.
  w = ws.addObject(idfObjects[0]); // Building
  ASSERT_TRUE(w); // should return OptionalHandle instead
  OptionalWorkspaceObject object = ws.getObject(w->handle());
  ASSERT_TRUE(object);
  EXPECT_TRUE(object->iddObject().type() == openstudio::IddObjectType::Building);
  
  // add object with pointer, and do not include pointed to object. should be successful, but
  // pointer should be null. object.canBeSource() should be true.

  // add object with pointer, and do include pointed to object. should be successful, and 
  // following pointer should yield target object. source should be source, target should be
  // target.

}

TEST_F(IdfFixture, WorkspaceObject_GetStringAfterSetStringAddsFields)
{
  // construct empty workspace, and set Idd to be EnergyPlusIdd
  StrictnessLevel level(StrictnessLevel::Draft);
  IddFileType fileType(IddFileType::EnergyPlus);
  Workspace ws(level,fileType);

  OptionalWorkspaceObject w;
  IdfObject idfObj(IddObjectType::SurfaceProperty_ConvectionCoefficients);
  w = ws.addObject(idfObj);
  
  EXPECT_TRUE(w->numNonextensibleFields()==3);
  EXPECT_TRUE(w->setString(w->iddObject().numFields()-1,""));
  EXPECT_TRUE(w->numNonextensibleFields()==11);
  OptionalString s4 = w->getString(4,true);
  EXPECT_TRUE(s4);
  OptionalString s10 = w->getString(10,true);
  EXPECT_TRUE(s10);
}

TEST_F(IdfFixture, WorkspaceObject_Building) {

  Workspace workspace(epIdfFile, StrictnessLevel::Draft);
  WorkspaceObjectVector buildings = workspace.getObjectsByType(IddObjectType::Building);
  ASSERT_EQ(static_cast<size_t>(1), buildings.size());
  WorkspaceObject building = buildings[0];

  /*
  Building,
    Building,                !- Name
    30.,                     !- North Axis {deg}
    City,                    !- Terrain
    0.04,                    !- Loads Convergence Tolerance Value
    0.4,                     !- Temperature Convergence Tolerance Value {deltaC}
    FullExterior,            !- Solar Distribution
    25;                      !- Maximum Number of Warmup Days
  */
  
  OptionalString buildingName = building.getString(BuildingFields::Name);
  ASSERT_TRUE(buildingName);
  EXPECT_EQ("Building", *buildingName);

  OptionalString northAxisString = building.getString(BuildingFields::NorthAxis);
  ASSERT_TRUE(northAxisString);
  EXPECT_EQ("30.", *northAxisString);

  OptionalDouble northAxis = building.getDouble(BuildingFields::NorthAxis);
  ASSERT_TRUE(northAxis);
  EXPECT_DOUBLE_EQ(30.0, *northAxis);

  OptionalString terrain = building.getString(BuildingFields::Terrain);
  ASSERT_TRUE(terrain);
  EXPECT_EQ("City", *terrain);

  OptionalUnsigned maximumNumberofWarmupDays = building.getUnsigned(BuildingFields::MaximumNumberofWarmupDays);
  ASSERT_TRUE(maximumNumberofWarmupDays);
  EXPECT_EQ(static_cast<unsigned>(25), *maximumNumberofWarmupDays);

}

TEST_F(IdfFixture, WorkspaceObject_Lights) {

  Workspace workspace(epIdfFile, StrictnessLevel::Draft);
  OptionalWorkspaceObject light = workspace.getObjectByTypeAndName(IddObjectType::Lights, "SPACE1-1 Lights 1");
  ASSERT_TRUE(light);

  OptionalString lightsZoneName = light->getString(LightsFields::ZoneorZoneListName);
  ASSERT_TRUE(lightsZoneName);

  OptionalString lightsScheduleName = light->getString(LightsFields::ScheduleName);
  ASSERT_TRUE(lightsScheduleName);

  OptionalWorkspaceObject zone = light->getTarget(LightsFields::ZoneorZoneListName);
  ASSERT_TRUE(zone);

  OptionalString zoneName = zone->getString(ZoneFields::Name);
  ASSERT_TRUE(zoneName);
  EXPECT_EQ(*lightsZoneName, *zoneName);

  OptionalWorkspaceObject schedule = light->getTarget(LightsFields::ScheduleName);
  ASSERT_TRUE(schedule);
  EXPECT_EQ(IddObjectType::Schedule_Compact, schedule->iddObject().type().value());

  OptionalString scheduleName = schedule->getString(Schedule_CompactFields::Name);
  ASSERT_TRUE(scheduleName);
  EXPECT_EQ(*lightsScheduleName, *scheduleName);

  // now change name of zone and schedule and make sure string reference in light change as well

  EXPECT_TRUE(zone->setString(ZoneFields::Name, "New Zone Name"));
  zoneName = zone->getString(ZoneFields::Name);
  ASSERT_TRUE(zoneName);
  EXPECT_EQ("New Zone Name", *zoneName);

  lightsZoneName = light->getString(LightsFields::ZoneorZoneListName);
  ASSERT_TRUE(lightsZoneName);
  EXPECT_EQ(*lightsZoneName, *zoneName);

  EXPECT_TRUE(zone->createName());
  zoneName = zone->getString(ZoneFields::Name);
  ASSERT_TRUE(zoneName);
  EXPECT_EQ("Zone 1",*zoneName);

  lightsZoneName = light->getString(LightsFields::ZoneorZoneListName);
  ASSERT_TRUE(lightsZoneName);
  EXPECT_EQ(*lightsZoneName, *zoneName);

  EXPECT_TRUE(schedule->setString(Schedule_CompactFields::Name, "New Schedule Name"));
  scheduleName = schedule->getString(Schedule_CompactFields::Name);
  ASSERT_TRUE(scheduleName);
  EXPECT_EQ("New Schedule Name", *scheduleName);

  lightsScheduleName = light->getString(LightsFields::ScheduleName);
  ASSERT_TRUE(lightsScheduleName);
  EXPECT_EQ(*lightsScheduleName, *scheduleName);

}

TEST_F(IdfFixture, WorkspaceObject_Lights_Strictness_None) {

  Workspace workspace(StrictnessLevel::None,IddFileType::EnergyPlus);
  EXPECT_TRUE(workspace.isValid());

  OptionalWorkspaceObject w = workspace.addObject(IdfObject(IddObjectType::Lights));
  ASSERT_TRUE(w);

  OptionalWorkspaceObject light = workspace.getObject(w->handle());
  ASSERT_TRUE(light);

  // can invalidate all we want
  EXPECT_TRUE(light->setString(LightsFields::Name, ""));
  EXPECT_TRUE(light->setDouble(LightsFields::Name, 0));

  EXPECT_TRUE(light->setString(LightsFields::ZoneorZoneListName, ""));
  EXPECT_TRUE(light->setPointer(LightsFields::ZoneorZoneListName, Handle()));

  EXPECT_TRUE(light->setString(LightsFields::ScheduleName, ""));
  EXPECT_TRUE(light->setPointer(LightsFields::ScheduleName, Handle()));

  EXPECT_TRUE(light->setString(LightsFields::DesignLevelCalculationMethod, ""));
  EXPECT_TRUE(light->setDouble(LightsFields::DesignLevelCalculationMethod, 0));

  EXPECT_TRUE(light->setString(LightsFields::LightingLevel, "Hi"));
  EXPECT_TRUE(light->setDouble(LightsFields::LightingLevel, -1));
  EXPECT_TRUE(light->setDouble(LightsFields::LightingLevel, 0));
  EXPECT_TRUE(light->setDouble(LightsFields::LightingLevel, 1));

  EXPECT_TRUE(workspace.isValid(StrictnessLevel::None));
  EXPECT_FALSE(workspace.isValid(StrictnessLevel::Draft));
  EXPECT_FALSE(workspace.isValid(StrictnessLevel::Final));
}

TEST_F(IdfFixture, WorkspaceObject_Lights_Strictness_Draft) {

  Workspace workspace(StrictnessLevel::Draft,IddFileType::EnergyPlus);
  EXPECT_TRUE(workspace.isValid());

  OptionalWorkspaceObject w = workspace.addObject(IdfObject(IddObjectType::Lights));
  ASSERT_TRUE(w);

  OptionalWorkspaceObject light = workspace.getObject(w->handle());
  ASSERT_TRUE(light);

  // certain things we can't invalidate
  EXPECT_TRUE(light->setString(LightsFields::Name, ""));
  EXPECT_TRUE(light->setDouble(LightsFields::Name, 0));

  EXPECT_TRUE(light->setString(LightsFields::ZoneorZoneListName, "")); // PointerType error
  EXPECT_TRUE(light->setPointer(LightsFields::ZoneorZoneListName, Handle())); // PointerType error

  EXPECT_TRUE(light->setString(LightsFields::ScheduleName, "")); // PointerType error
  EXPECT_TRUE(light->setPointer(LightsFields::ScheduleName, Handle())); // PointerType error

  EXPECT_TRUE(light->setString(LightsFields::DesignLevelCalculationMethod, "")); // this is ok because there is a default
  EXPECT_FALSE(light->setString(LightsFields::DesignLevelCalculationMethod, "Hi")); // DataType error
  EXPECT_FALSE(light->setDouble(LightsFields::DesignLevelCalculationMethod, 0)); // DataType error

  EXPECT_FALSE(light->setString(LightsFields::LightingLevel, "Hi")); // DataType error
  EXPECT_FALSE(light->setDouble(LightsFields::LightingLevel, -1));  // NumericBound error
  EXPECT_TRUE(light->setDouble(LightsFields::LightingLevel, 0));
  EXPECT_TRUE(light->setDouble(LightsFields::LightingLevel, 1));

  EXPECT_TRUE(workspace.isValid(StrictnessLevel::None));
  EXPECT_TRUE(workspace.isValid(StrictnessLevel::Draft));
  EXPECT_FALSE(workspace.isValid(StrictnessLevel::Final));
}

// ETH@20120827 Removed Lights_Strictness_Final test because it is no longer possible
// to construct an empy Workspace at StrictnessLevel::Final. (The call to addVersionObject
// results in an exception being thrown.) Almost all work should be done at
// StrictnessLevel::Draft; one should only check for StrictnessLevel::Final validity
// immediately prior to simulation.

TEST_F(IdfFixture, WorkspaceObject_FieldSettingWithHiddenPushes) {
  Workspace scratch(StrictnessLevel::None,IddFileType::EnergyPlus); // Strictness level None

  std::stringstream text;
  text << "ZoneHVAC:HighTemperatureRadiant," << std::endl
       << "  MyRadiantSystem," << std::endl
       << "  MyHVACSchedule," << std::endl
       << "  MyCoreZone," << std::endl
       << "  HeatingDesignCapacity," << std::endl
       << "  Autosize," << std::endl
       << "  ," << std::endl
       << "  ," << std::endl
       << "  Electricity;";
  OptionalIdfObject oObj = IdfObject::load(text.str());
  ASSERT_TRUE(oObj);
  IdfObject idfObject = *oObj;
  OptionalWorkspaceObject w1 = scratch.addObject(idfObject);
  ASSERT_TRUE(w1);
  OptionalWorkspaceObject tObject = scratch.getObject(w1->handle ());
  ASSERT_TRUE(tObject);
  WorkspaceObject object = *tObject;
  EXPECT_EQ(static_cast<unsigned>(8),object.numFields());

  // create schedule object to point to from non-extensible field
  text.str("");
  text << "Schedule:Compact," << std::endl
       << "  AlwaysOn," << std::endl
       << "  ," << std::endl
       << "  For: AllOtherDays," << std::endl
       << "  Until: 24:00," << std::endl
       << "  1.0;";
  oObj = IdfObject::load(text.str());
  ASSERT_TRUE(oObj);
  idfObject = *oObj;
  ASSERT_TRUE(idfObject.iddObject().type() == IddObjectType::Schedule_Compact);
  OptionalWorkspaceObject w2 = scratch.addObject(idfObject);
  ASSERT_TRUE(w2);
  EXPECT_TRUE(object.setPointer(14,w2->handle ()));
  EXPECT_EQ(15u,object.numFields());
  tObject = object.getTarget(14);
  ASSERT_TRUE(tObject);
  EXPECT_TRUE(tObject->handle() == w2->handle());

  // hidden pushing for setting extensible string pointer
  EXPECT_TRUE(object.setString(16,*(tObject->name()))); // should only work at strictness none
  tObject = object.getTarget(16);
  ASSERT_TRUE(tObject);
  EXPECT_TRUE(tObject->handle() == w2->handle());
  EXPECT_EQ(18u,object.numFields());

  // hidden pushing for setting extensible double
  EXPECT_TRUE(object.setDouble(19,0.5));
  EXPECT_EQ(20u,object.numFields());
  OptionalDouble dValue = object.getDouble(19);
  ASSERT_TRUE(dValue);
  EXPECT_NEAR(0.5,*dValue,tol);
  
  // SHOULD NOT BE VALID
  scratch = Workspace(StrictnessLevel::Draft, IddFileType::EnergyPlus); // Non-null data must be valid
  text.str("");
  text << "ZoneHVAC:HighTemperatureRadiant," << std::endl
       << "  MyRadiantSystem," << std::endl
       << "  MyHVACSchedule," << std::endl
       << "  MyCoreZone," << std::endl
       << "  HeatingDesignCapacity," << std::endl
       << "  Autosize," << std::endl
       << "  ," << std::endl
       << "  ," << std::endl
       << "  Electricity;";
  oObj = IdfObject::load(text.str());
  ASSERT_TRUE(oObj);
  idfObject = *oObj;
  w2 = scratch.addObject(idfObject);
  ASSERT_TRUE(w2);
  tObject = scratch.getObject(w2->handle());
  ASSERT_TRUE(tObject);
  object = *tObject;

  // hidden pushing for setting nonextensible double
  EXPECT_FALSE(object.setDouble(9,1.5));
  EXPECT_EQ(8u,object.numFields());
  EXPECT_TRUE(object.setDouble(9,0.6));
  EXPECT_EQ(10u,object.numFields());

  // hidden pushing for setting nonextensible string
  EXPECT_FALSE(object.setString(12,"bad key"));
  EXPECT_EQ(10u,object.numFields());
  EXPECT_TRUE(object.setString(12,"MeanAirTemperature"));
  EXPECT_EQ(13u,object.numFields());

  // hidden pushing for setting nonextensible pointer
  EXPECT_TRUE(object.setString(14,""));
  EXPECT_EQ(15u,object.numFields());

  // hidden pushing for setting extensible string pointer
  EXPECT_FALSE(object.setString(16,"MySurface"));
  EXPECT_EQ(15u,object.numFields());
  EXPECT_TRUE(object.setString(16,""));
  EXPECT_EQ(18u,object.numFields());

  // hidden pushing for setting extensible double
  EXPECT_FALSE(object.setDouble(21,-1.5));
  EXPECT_EQ(18u,object.numFields());
  EXPECT_TRUE(object.setDouble(19,0.5));
  EXPECT_EQ(20u,object.numFields());
}

TEST_F(IdfFixture,WorkspaceObject_ClearGroups) {
  // always works in None or Draft strictness
  Workspace ws(epIdfFile);
  EXPECT_TRUE(ws.strictnessLevel() == StrictnessLevel::None);
  WorkspaceObjectVector surfaces = ws.getObjectsByType(IddObjectType::BuildingSurface_Detailed);
  ASSERT_TRUE(surfaces.size() > 0);
  unsigned n = surfaces[0].numFields();
  surfaces[0].clearExtensibleGroups();
  EXPECT_TRUE(surfaces[0].numFields() < n);

  // doesn't allow drops below minFields() in Final strictness
  ws = Workspace(epIdfFile);
  ws.setStrictnessLevel(StrictnessLevel::Final);
  surfaces = ws.getObjectsByType(IddObjectType::BuildingSurface_Detailed);
  ASSERT_TRUE(surfaces.size() > 0);
  n = surfaces[0].numFields();
  surfaces[0].clearExtensibleGroups();
  EXPECT_EQ(n,surfaces[0].numFields());
}


TEST_F(IdfFixture, WorkspaceObject_OS_DaylightingDevice_Shelf) 
{
  // defaults to IddFileType::OpenStudio
  Workspace ws;
  OptionalWorkspaceObject w1 = ws.addObject(IdfObject(IddObjectType::OS_DaylightingDevice_Shelf));
  OptionalWorkspaceObject w2 = ws.addObject(IdfObject(IddObjectType::OS_InteriorPartitionSurface));
  ASSERT_TRUE(w1);
  ASSERT_TRUE(w2);
  OptionalWorkspaceObject obj1 = ws.getObject(w1->handle());
  ASSERT_TRUE(obj1);
  EXPECT_TRUE(obj1->setPointer(OS_DaylightingDevice_ShelfFields::InsideShelfName, w2->handle()));
}

//TEST_F(IdfFixture, WorkspaceObject_OS_AirLoopHVAC_ZoneSplitter) 
//{
//  //Workspace ws();
//  //OptionalHandle h1 = ws.addObject(IdfObject(IddObjectType::OS_AirLoopHVAC_ZoneSplitter));
//  //OptionalHandle h2 = ws.addObject(IdfObject(IddObjectType::OS_Connection));
//}

TEST_F(IdfFixture, WorkspaceObject_RestoreHandleInAddObjects) 
{
  // defaults to IddFileType::OpenStudio
  Workspace ws1;
  OptionalWorkspaceObject w1 = ws1.addObject(IdfObject(IddObjectType::OS_Surface));
  ASSERT_TRUE(w1);
  EXPECT_EQ(1u, ws1.objects().size());
  OptionalString h1String = w1->getString(0);
  ASSERT_TRUE(h1String);
  Handle h1(toQString(*h1String));
  EXPECT_FALSE(h1.isNull());
  EXPECT_EQ(h1, w1->handle());

  Workspace ws2;
  ws2.addObjects(ws1.objects());
  ASSERT_EQ(1u, ws2.objects().size());
  WorkspaceObject w2 = ws2.objects()[0];
  OptionalString h2String = w2.getString(0);
  ASSERT_TRUE(h2String);
  Handle h2(toQString(*h2String));
  EXPECT_FALSE(h2.isNull());
  EXPECT_EQ(h2, w2.handle());
}

TEST_F(IdfFixture, WorkspaceObject_RestoreHandleInAddObjects2) 
{
  // defaults to IddFileType::OpenStudio
  Workspace ws1;
  OptionalWorkspaceObject w1 = ws1.addObject(IdfObject(IddObjectType::OS_Surface));
  ASSERT_TRUE(w1);
  EXPECT_EQ(1u, ws1.objects().size());
  OptionalString h1String = w1->getString(0);
  ASSERT_TRUE(h1String);
  Handle h1(toQString(*h1String));
  EXPECT_FALSE(h1.isNull());
  EXPECT_EQ(h1, w1->handle());

  IdfFile idf1 = ws1.toIdfFile();
  ASSERT_EQ(1u,idf1.objects().size());
  IdfObject i1 = idf1.objects()[0];
  OptionalString i1hString = i1.getString(0);
  ASSERT_TRUE(i1hString);
  Handle ih(toQString(*i1hString));
  EXPECT_FALSE(ih.isNull());
  EXPECT_EQ(ih,i1.handle());

  Workspace ws2;
  ws2.addObjects(idf1.objects());
  ASSERT_EQ(1u, ws2.objects().size());
  WorkspaceObject w2 = ws2.objects()[0];
  OptionalString h2String = w2.getString(0);
  ASSERT_TRUE(h2String);
  Handle h2(toQString(*h2String));
  EXPECT_FALSE(h2.isNull());
  EXPECT_EQ(h2, w2.handle());
}

TEST_F(IdfFixture, WorkspaceObject_Filter_Sources)
{
  Workspace ws;
  OptionalWorkspaceObject node = ws.addObject(IdfObject(IddObjectType::OS_Node));
  OptionalWorkspaceObject node2 = ws.addObject(IdfObject(IddObjectType::OS_Node));
  OptionalWorkspaceObject node3 = ws.addObject(IdfObject(IddObjectType::OS_Node));
  OptionalWorkspaceObject spm = ws.addObject(IdfObject(IddObjectType::OS_SetpointManager_MixedAir));

  EXPECT_TRUE(spm->setPointer(OS_SetpointManager_MixedAirFields::SetpointNodeorNodeListName, node->handle()));
  EXPECT_TRUE(spm->setPointer(OS_SetpointManager_MixedAirFields::FanInletNodeName,node->handle()));
  EXPECT_TRUE(spm->setPointer(OS_SetpointManager_MixedAirFields::FanOutletNodeName,node2->handle()));
  EXPECT_TRUE(spm->setPointer(OS_SetpointManager_MixedAirFields::ReferenceSetpointNodeName,node3->handle()));

  WorkspaceObjectVector sourcesVector = node->sources();
  EXPECT_EQ(1, sourcesVector.size());
  sourcesVector = node->getSources(IddObjectType::OS_SetpointManager_MixedAir);
  EXPECT_EQ(1, sourcesVector.size());
}
