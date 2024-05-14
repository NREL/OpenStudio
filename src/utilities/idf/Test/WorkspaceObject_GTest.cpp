/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>
#include "IdfFixture.hpp"
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/Building_FieldEnums.hxx>
#include <utilities/idd/Zone_FieldEnums.hxx>
#include <utilities/idd/Lights_FieldEnums.hxx>
#include <utilities/idd/Schedule_Compact_FieldEnums.hxx>

#include <utilities/idd/OS_DaylightingDevice_Shelf_FieldEnums.hxx>
#include <utilities/idd/OS_SetpointManager_MixedAir_FieldEnums.hxx>
#include <utilities/idd/OS_ZoneHVAC_EquipmentList_FieldEnums.hxx>
#include <utilities/idd/OS_ZoneHVAC_Baseboard_Convective_Electric_FieldEnums.hxx>
#include <utilities/idd/OS_Schedule_Constant_FieldEnums.hxx>

#include <utilities/idd/IddFactory.hxx>

#include "../WorkspaceExtensibleGroup.hpp"
#include "../IdfFile.hpp"
#include "../IdfObject.hpp"
#include "../Workspace.hpp"
#include "../WorkspaceObject.hpp"
#include "../WorkspaceObject_Impl.hpp"

#include "../../core/Optional.hpp"

using namespace openstudio;

TEST_F(IdfFixture, WorkspaceObject_Construction) {
  // get vector of IdfFile objects as fodder for tests
  IdfObjectVector idfObjects = epIdfFile.objects();
  OptionalWorkspaceObject w;
  OptionalHandleVector hv;

  // construct empty workspace, and set Idd to be EnergyPlusIdd
  StrictnessLevel level(StrictnessLevel::Draft);
  IddFileType fileType(IddFileType::EnergyPlus);
  Workspace ws(level, fileType);

  // add object with no pointers. should be successful.
  w = ws.addObject(idfObjects[0]);  // Building
  ASSERT_TRUE(w);                   // should return OptionalHandle instead
  OptionalWorkspaceObject object = ws.getObject(w->handle());
  ASSERT_TRUE(object);
  EXPECT_TRUE(object->iddObject().type() == openstudio::IddObjectType::Building);

  // add object with pointer, and do not include pointed to object. should be successful, but
  // pointer should be null. object.canBeSource() should be true.

  // add object with pointer, and do include pointed to object. should be successful, and
  // following pointer should yield target object. source should be source, target should be
  // target.
}

TEST_F(IdfFixture, WorkspaceObject_GetStringAfterSetStringAddsFields) {
  // construct empty workspace, and set Idd to be EnergyPlusIdd
  StrictnessLevel level(StrictnessLevel::Draft);
  IddFileType fileType(IddFileType::EnergyPlus);
  Workspace ws(level, fileType);

  OptionalWorkspaceObject w;
  IdfObject idfObj(IddObjectType::SurfaceProperty_ConvectionCoefficients);
  w = ws.addObject(idfObj);

  EXPECT_TRUE(w->numNonextensibleFields() == 3);
  EXPECT_TRUE(w->setString(w->iddObject().numFields() - 1, ""));
  EXPECT_TRUE(w->numNonextensibleFields() == 11);
  OptionalString s4 = w->getString(4, true);
  EXPECT_TRUE(s4);
  OptionalString s10 = w->getString(10, true);
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

  OptionalString lightsZoneName = light->getString(LightsFields::ZoneorZoneListorSpaceorSpaceListName);
  ASSERT_TRUE(lightsZoneName);

  OptionalString lightsScheduleName = light->getString(LightsFields::ScheduleName);
  ASSERT_TRUE(lightsScheduleName);

  OptionalWorkspaceObject zone = light->getTarget(LightsFields::ZoneorZoneListorSpaceorSpaceListName);
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

  lightsZoneName = light->getString(LightsFields::ZoneorZoneListorSpaceorSpaceListName);
  ASSERT_TRUE(lightsZoneName);
  EXPECT_EQ(*lightsZoneName, *zoneName);

  EXPECT_TRUE(zone->createName());
  zoneName = zone->getString(ZoneFields::Name);
  ASSERT_TRUE(zoneName);
  EXPECT_EQ("Zone 1", *zoneName);

  lightsZoneName = light->getString(LightsFields::ZoneorZoneListorSpaceorSpaceListName);
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

TEST_F(IdfFixture, WorkspaceObject_Lights_Strictness_Minimal) {

  Workspace workspace(StrictnessLevel::Minimal, IddFileType::EnergyPlus);
  EXPECT_TRUE(workspace.isValid());

  OptionalWorkspaceObject w = workspace.addObject(IdfObject(IddObjectType::Lights));
  ASSERT_TRUE(w);

  OptionalWorkspaceObject light = workspace.getObject(w->handle());
  ASSERT_TRUE(light);

  // can invalidate all we want
  EXPECT_TRUE(light->setString(LightsFields::Name, ""));
  EXPECT_TRUE(light->setDouble(LightsFields::Name, 0));

  EXPECT_TRUE(light->setString(LightsFields::ZoneorZoneListorSpaceorSpaceListName, ""));
  EXPECT_TRUE(light->setPointer(LightsFields::ZoneorZoneListorSpaceorSpaceListName, Handle()));

  EXPECT_TRUE(light->setString(LightsFields::ScheduleName, ""));
  EXPECT_TRUE(light->setPointer(LightsFields::ScheduleName, Handle()));

  EXPECT_TRUE(light->setString(LightsFields::DesignLevelCalculationMethod, ""));
  EXPECT_TRUE(light->setDouble(LightsFields::DesignLevelCalculationMethod, 0));

  EXPECT_TRUE(light->setString(LightsFields::LightingLevel, "Hi"));
  EXPECT_TRUE(light->setDouble(LightsFields::LightingLevel, -1));
  EXPECT_TRUE(light->setDouble(LightsFields::LightingLevel, 0));
  EXPECT_TRUE(light->setDouble(LightsFields::LightingLevel, 1));

  EXPECT_TRUE(workspace.isValid(StrictnessLevel::Minimal));
  EXPECT_FALSE(workspace.isValid(StrictnessLevel::Draft));
  EXPECT_FALSE(workspace.isValid(StrictnessLevel::Final));
}

TEST_F(IdfFixture, WorkspaceObject_Lights_Strictness_Draft) {

  Workspace workspace(StrictnessLevel::Draft, IddFileType::EnergyPlus);
  EXPECT_TRUE(workspace.isValid());

  OptionalWorkspaceObject w = workspace.addObject(IdfObject(IddObjectType::Lights));
  ASSERT_TRUE(w);

  OptionalWorkspaceObject light = workspace.getObject(w->handle());
  ASSERT_TRUE(light);

  // certain things we can't invalidate
  EXPECT_FALSE(light->setString(LightsFields::Name, ""));
  EXPECT_TRUE(light->setDouble(LightsFields::Name, 0));

  EXPECT_TRUE(light->setString(LightsFields::ZoneorZoneListorSpaceorSpaceListName, ""));         // PointerType error
  EXPECT_TRUE(light->setPointer(LightsFields::ZoneorZoneListorSpaceorSpaceListName, Handle()));  // PointerType error

  EXPECT_TRUE(light->setString(LightsFields::ScheduleName, ""));         // PointerType error
  EXPECT_TRUE(light->setPointer(LightsFields::ScheduleName, Handle()));  // PointerType error

  EXPECT_TRUE(light->setString(LightsFields::DesignLevelCalculationMethod, ""));     // this is ok because there is a default
  EXPECT_FALSE(light->setString(LightsFields::DesignLevelCalculationMethod, "Hi"));  // DataType error
  EXPECT_FALSE(light->setDouble(LightsFields::DesignLevelCalculationMethod, 0));     // DataType error

  EXPECT_FALSE(light->setString(LightsFields::LightingLevel, "Hi"));  // DataType error
  EXPECT_FALSE(light->setDouble(LightsFields::LightingLevel, -1));    // NumericBound error
  EXPECT_TRUE(light->setDouble(LightsFields::LightingLevel, 0));
  EXPECT_TRUE(light->setDouble(LightsFields::LightingLevel, 1));

  EXPECT_TRUE(workspace.isValid(StrictnessLevel::Minimal));
  EXPECT_TRUE(workspace.isValid(StrictnessLevel::Draft));
  EXPECT_FALSE(workspace.isValid(StrictnessLevel::Final));
}

// ETH@20120827 Removed Lights_Strictness_Final test because it is no longer possible
// to construct an empy Workspace at StrictnessLevel::Final. (The call to addVersionObject
// results in an exception being thrown.) Almost all work should be done at
// StrictnessLevel::Draft; one should only check for StrictnessLevel::Final validity
// immediately prior to simulation.

TEST_F(IdfFixture, WorkspaceObject_FieldSettingWithHiddenPushes) {
  Workspace scratch(StrictnessLevel::Minimal, IddFileType::EnergyPlus);  // Strictness level None

  std::stringstream text;
  text << "ZoneHVAC:HighTemperatureRadiant," << '\n'
       << "  MyRadiantSystem," << '\n'
       << "  MyHVACSchedule," << '\n'
       << "  MyCoreZone," << '\n'
       << "  HeatingDesignCapacity," << '\n'
       << "  Autosize," << '\n'
       << "  ," << '\n'
       << "  ," << '\n'
       << "  Electricity;";
  OptionalIdfObject oObj = IdfObject::load(text.str());
  ASSERT_TRUE(oObj);
  IdfObject idfObject = *oObj;
  OptionalWorkspaceObject w1 = scratch.addObject(idfObject);
  ASSERT_TRUE(w1);
  OptionalWorkspaceObject tObject = scratch.getObject(w1->handle());
  ASSERT_TRUE(tObject);
  WorkspaceObject object = *tObject;
  EXPECT_EQ(static_cast<unsigned>(14), object.numFields());

  // create schedule object to point to from non-extensible field
  text.str("");
  text << "Schedule:Compact," << '\n'
       << "  AlwaysOn," << '\n'
       << "  ," << '\n'
       << "  For: AllOtherDays," << '\n'
       << "  Until: 24:00," << '\n'
       << "  1.0;";
  oObj = IdfObject::load(text.str());
  ASSERT_TRUE(oObj);
  idfObject = *oObj;
  ASSERT_TRUE(idfObject.iddObject().type() == IddObjectType::Schedule_Compact);
  OptionalWorkspaceObject w2 = scratch.addObject(idfObject);
  ASSERT_TRUE(w2);
  EXPECT_TRUE(object.setPointer(14, w2->handle()));
  EXPECT_EQ(15u, object.numFields());
  tObject = object.getTarget(14);
  ASSERT_TRUE(tObject);
  EXPECT_TRUE(tObject->handle() == w2->handle());

  // hidden pushing for setting extensible string pointer
  EXPECT_TRUE(object.setString(16, *(tObject->name())));  // should only work at strictness none
  tObject = object.getTarget(16);
  ASSERT_TRUE(tObject);
  EXPECT_TRUE(tObject->handle() == w2->handle());
  EXPECT_EQ(18u, object.numFields());

  // hidden pushing for setting extensible double
  EXPECT_TRUE(object.setDouble(19, 0.5));
  EXPECT_EQ(20u, object.numFields());
  OptionalDouble dValue = object.getDouble(19);
  ASSERT_TRUE(dValue);
  EXPECT_NEAR(0.5, *dValue, tol);

  // SHOULD NOT BE VALID
  scratch = Workspace(StrictnessLevel::Draft, IddFileType::EnergyPlus);  // Non-null data must be valid
  text.str("");
  text << "ZoneHVAC:HighTemperatureRadiant," << '\n'
       << "  MyRadiantSystem," << '\n'
       << "  MyHVACSchedule," << '\n'
       << "  MyCoreZone," << '\n'
       << "  HeatingDesignCapacity," << '\n'
       << "  Autosize," << '\n'
       << "  ," << '\n'
       << "  ," << '\n'
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
  EXPECT_FALSE(object.setDouble(9, 1.5));
  EXPECT_EQ(14u, object.numFields());
  EXPECT_TRUE(object.setDouble(9, 0.6));
  EXPECT_EQ(14u, object.numFields());

  // hidden pushing for setting nonextensible string
  EXPECT_FALSE(object.setString(12, "bad key"));
  EXPECT_EQ(14u, object.numFields());
  EXPECT_TRUE(object.setString(12, "MeanAirTemperature"));
  EXPECT_EQ(14u, object.numFields());

  // hidden pushing for setting nonextensible pointer
  EXPECT_TRUE(object.setString(14, ""));
  EXPECT_EQ(15u, object.numFields());

  // hidden pushing for setting extensible string pointer
  EXPECT_FALSE(object.setString(16, "MySurface"));
  EXPECT_EQ(15u, object.numFields());
  EXPECT_TRUE(object.setString(16, ""));
  EXPECT_EQ(18u, object.numFields());

  // hidden pushing for setting extensible double
  EXPECT_FALSE(object.setDouble(21, -1.5));
  EXPECT_EQ(18u, object.numFields());
  EXPECT_TRUE(object.setDouble(19, 0.5));
  EXPECT_EQ(20u, object.numFields());
}

TEST_F(IdfFixture, WorkspaceObject_ClearGroups) {
  // always works in None or Draft strictness
  Workspace ws(epIdfFile);
  EXPECT_TRUE(ws.strictnessLevel() == StrictnessLevel::Minimal);
  WorkspaceObjectVector surfaces = ws.getObjectsByType(IddObjectType::BuildingSurface_Detailed);
  ASSERT_TRUE(!surfaces.empty());
  unsigned n = surfaces[0].numFields();
  surfaces[0].clearExtensibleGroups();
  EXPECT_TRUE(surfaces[0].numFields() < n);

  // doesn't allow drops below minFields() in Final strictness
  ws = Workspace(epIdfFile);
  ws.setStrictnessLevel(StrictnessLevel::Final);
  surfaces = ws.getObjectsByType(IddObjectType::BuildingSurface_Detailed);
  ASSERT_TRUE(!surfaces.empty());
  n = surfaces[0].numFields();
  surfaces[0].clearExtensibleGroups();
  EXPECT_EQ(n, surfaces[0].numFields());
}

TEST_F(IdfFixture, WorkspaceObject_OS_DaylightingDevice_Shelf) {
  // defaults to IddFileType::EnergyPlus so need to specify IddFileType::OpenStudio
  Workspace ws(StrictnessLevel::Draft, IddFileType::OpenStudio);
  OptionalWorkspaceObject w1 = ws.addObject(IdfObject(IddObjectType::OS_DaylightingDevice_Shelf));
  OptionalWorkspaceObject w2 = ws.addObject(IdfObject(IddObjectType::OS_InteriorPartitionSurface));
  ASSERT_TRUE(w1);
  ASSERT_TRUE(w2);
  OptionalWorkspaceObject obj1 = ws.getObject(w1->handle());
  ASSERT_TRUE(obj1);
  EXPECT_TRUE(obj1->setPointer(OS_DaylightingDevice_ShelfFields::InsideShelfName, w2->handle()));
  ASSERT_TRUE(obj1->getString(OS_DaylightingDevice_ShelfFields::InsideShelfName));
  EXPECT_EQ(w2->nameString(), obj1->getString(OS_DaylightingDevice_ShelfFields::InsideShelfName).get());

  // check that you can set string with pointer
  EXPECT_TRUE(obj1->setString(OS_DaylightingDevice_ShelfFields::InsideShelfName, ""));
  ASSERT_TRUE(obj1->getString(OS_DaylightingDevice_ShelfFields::InsideShelfName));
  EXPECT_EQ("", obj1->getString(OS_DaylightingDevice_ShelfFields::InsideShelfName).get());

  // check that you can set string with pointer
  EXPECT_TRUE(obj1->setString(OS_DaylightingDevice_ShelfFields::InsideShelfName, toString(w2->handle())));
  ASSERT_TRUE(obj1->getString(OS_DaylightingDevice_ShelfFields::InsideShelfName));
  EXPECT_EQ(w2->nameString(), obj1->getString(OS_DaylightingDevice_ShelfFields::InsideShelfName).get());

  // check that you cannot set string with bad pointer
  EXPECT_FALSE(obj1->setString(OS_DaylightingDevice_ShelfFields::InsideShelfName, toString(w1->handle())));
  ASSERT_TRUE(obj1->getString(OS_DaylightingDevice_ShelfFields::InsideShelfName));
  EXPECT_EQ(w2->nameString(), obj1->getString(OS_DaylightingDevice_ShelfFields::InsideShelfName).get());

  // check that you cannot set string with bad pointer
  EXPECT_FALSE(obj1->setString(OS_DaylightingDevice_ShelfFields::InsideShelfName, toString(createUUID())));
  ASSERT_TRUE(obj1->getString(OS_DaylightingDevice_ShelfFields::InsideShelfName));
  EXPECT_EQ(w2->nameString(), obj1->getString(OS_DaylightingDevice_ShelfFields::InsideShelfName).get());
}

//TEST_F(IdfFixture, WorkspaceObject_OS_AirLoopHVAC_ZoneSplitter)
//{
//  //Workspace ws(StrictnessLevel::Draft, IddFileType::OpenStudio);
//  //OptionalHandle h1 = ws.addObject(IdfObject(IddObjectType::OS_AirLoopHVAC_ZoneSplitter));
//  //OptionalHandle h2 = ws.addObject(IdfObject(IddObjectType::OS_Connection));
//}

TEST_F(IdfFixture, WorkspaceObject_RestoreHandleInAddObjects) {
  // defaults to IddFileType::EnergyPlus so need to specify IddFileType::OpenStudio
  Workspace ws1(StrictnessLevel::Draft, IddFileType::OpenStudio);
  OptionalWorkspaceObject w1 = ws1.addObject(IdfObject(IddObjectType::OS_Surface));
  ASSERT_TRUE(w1);
  EXPECT_EQ(1u, ws1.objects().size());
  OptionalString h1String = w1->getString(0);
  ASSERT_TRUE(h1String);
  Handle h1(toUUID(*h1String));
  EXPECT_FALSE(h1.isNull());
  EXPECT_EQ(h1, w1->handle());

  Workspace ws2(StrictnessLevel::Draft, IddFileType::OpenStudio);
  ws2.addObjects(ws1.objects());
  ASSERT_EQ(1u, ws2.objects().size());
  WorkspaceObject w2 = ws2.objects()[0];
  OptionalString h2String = w2.getString(0);
  ASSERT_TRUE(h2String);
  Handle h2(toUUID(*h2String));
  EXPECT_FALSE(h2.isNull());
  EXPECT_EQ(h2, w2.handle());
}

TEST_F(IdfFixture, WorkspaceObject_RestoreHandleInAddObjects2) {
  // defaults to IddFileType::EnergyPlus so need to specify IddFileType::OpenStudio
  Workspace ws1(StrictnessLevel::Draft, IddFileType::OpenStudio);
  OptionalWorkspaceObject w1 = ws1.addObject(IdfObject(IddObjectType::OS_Surface));
  ASSERT_TRUE(w1);
  EXPECT_EQ(1u, ws1.objects().size());
  OptionalString h1String = w1->getString(0);
  ASSERT_TRUE(h1String);
  Handle h1(toUUID(*h1String));
  EXPECT_FALSE(h1.isNull());
  EXPECT_EQ(h1, w1->handle());

  IdfFile idf1 = ws1.toIdfFile();
  ASSERT_EQ(1u, idf1.objects().size());
  IdfObject i1 = idf1.objects()[0];
  OptionalString i1hString = i1.getString(0);
  ASSERT_TRUE(i1hString);
  Handle ih(toUUID(*i1hString));
  EXPECT_FALSE(ih.isNull());
  EXPECT_EQ(ih, i1.handle());

  Workspace ws2(StrictnessLevel::Draft, IddFileType::OpenStudio);
  ws2.addObjects(idf1.objects());
  ASSERT_EQ(1u, ws2.objects().size());
  WorkspaceObject w2 = ws2.objects()[0];
  OptionalString h2String = w2.getString(0);
  ASSERT_TRUE(h2String);
  Handle h2(toUUID(*h2String));
  EXPECT_FALSE(h2.isNull());
  EXPECT_EQ(h2, w2.handle());
}

TEST_F(IdfFixture, WorkspaceObject_Filter_Sources) {
  // defaults to IddFileType::EnergyPlus so need to specify IddFileType::OpenStudio
  Workspace ws(StrictnessLevel::Draft, IddFileType::OpenStudio);
  OptionalWorkspaceObject node = ws.addObject(IdfObject(IddObjectType::OS_Node));
  OptionalWorkspaceObject node2 = ws.addObject(IdfObject(IddObjectType::OS_Node));
  OptionalWorkspaceObject node3 = ws.addObject(IdfObject(IddObjectType::OS_Node));
  OptionalWorkspaceObject spm = ws.addObject(IdfObject(IddObjectType::OS_SetpointManager_MixedAir));

  EXPECT_TRUE(spm->setPointer(OS_SetpointManager_MixedAirFields::SetpointNodeorNodeListName, node->handle()));
  EXPECT_TRUE(spm->setPointer(OS_SetpointManager_MixedAirFields::FanInletNodeName, node->handle()));
  EXPECT_TRUE(spm->setPointer(OS_SetpointManager_MixedAirFields::FanOutletNodeName, node2->handle()));
  EXPECT_TRUE(spm->setPointer(OS_SetpointManager_MixedAirFields::ReferenceSetpointNodeName, node3->handle()));

  WorkspaceObjectVector sourcesVector = node->sources();
  EXPECT_EQ(1, sourcesVector.size());
  sourcesVector = node->getSources(IddObjectType::OS_SetpointManager_MixedAir);
  EXPECT_EQ(1, sourcesVector.size());
}

TEST_F(IdfFixture, WorkspaceObject_SetDouble_NaN_and_Inf) {

  // try with an WorkspaceObject
  // WorkspaceObject does prevent Infinity and NaN
  // defaults to IddFileType::EnergyPlus so need to specify IddFileType::OpenStudio
  Workspace ws(StrictnessLevel::Draft, IddFileType::OpenStudio);
  WorkspaceObject object = ws.addObject(IdfObject(IddObjectType::OS_People_Definition)).get();

  // Set Number of People
  // Check for nan
  EXPECT_FALSE(object.setDouble(3, std::numeric_limits<double>::quiet_NaN()));

  // Infinity
  EXPECT_FALSE(object.setDouble(3, std::numeric_limits<double>::infinity()));
  EXPECT_FALSE(object.setDouble(3, -std::numeric_limits<double>::infinity()));

  // try with an IdfExtensibleGroup (Hour, Minute, Value)
  WorkspaceObject object2 = ws.addObject(IdfObject(IddObjectType::OS_Schedule_Day)).get();
  IdfExtensibleGroup eg = object2.pushExtensibleGroup();
  // set the value field
  // Check for nan
  EXPECT_FALSE(eg.setDouble(2, std::numeric_limits<double>::quiet_NaN()));

  // Infinity
  EXPECT_FALSE(eg.setDouble(2, std::numeric_limits<double>::infinity()));
  EXPECT_FALSE(eg.setDouble(2, -std::numeric_limits<double>::infinity()));

  // new extensible group
  std::vector<std::string> group;
  group.push_back("1");
  group.push_back("2");
  group.push_back(toString(std::numeric_limits<double>::quiet_NaN()));
  EXPECT_EQ(1u, object2.numExtensibleGroups());
  EXPECT_TRUE(object2.pushExtensibleGroup(group).empty());
  EXPECT_EQ(1u, object2.numExtensibleGroups());

  group.clear();
  group.push_back("1");
  group.push_back("2");
  group.push_back(toString(std::numeric_limits<double>::infinity()));
  EXPECT_EQ(1u, object2.numExtensibleGroups());
  EXPECT_TRUE(object2.pushExtensibleGroup(group).empty());
  EXPECT_EQ(1u, object2.numExtensibleGroups());

  group.clear();
  group.push_back("1");
  group.push_back("2");
  group.push_back(toString(3.0));
  EXPECT_EQ(1u, object2.numExtensibleGroups());
  EXPECT_FALSE(object2.pushExtensibleGroup(group).empty());
  EXPECT_EQ(2u, object2.numExtensibleGroups());
}

TEST_F(IdfFixture, WorkspaceObject_setString) {

  // Test for #4205 - setString on a WorkspaceObject should prevent duplicate names
  Workspace ws(StrictnessLevel::Draft, IddFileType::OpenStudio);
  WorkspaceObject space1 = ws.addObject(IdfObject(IddObjectType::OS_Space)).get();

  WorkspaceObject space2 = ws.addObject(IdfObject(IddObjectType::OS_Space)).get();

  unsigned nameIndex = 1;
  EXPECT_TRUE(space1.setString(nameIndex, "Space 1"));
  ASSERT_TRUE(space1.getString(nameIndex));
  EXPECT_EQ("Space 1", space1.getString(nameIndex).get());

  EXPECT_TRUE(space2.setString(nameIndex, "SpaceA"));
  EXPECT_EQ("SpaceA", space2.getString(nameIndex).get());

  EXPECT_TRUE(space2.setString(nameIndex, "Space 1"));  // Setting works, but it should modify it
  ASSERT_TRUE(space2.getString(nameIndex));
  EXPECT_NE("Space 1", space2.getString(nameIndex).get());
  EXPECT_EQ("Space 2", space2.getString(nameIndex).get());

  // That portion is not accepted either, even at Draft level
  EXPECT_FALSE(space2.setString(nameIndex, ""));
  ASSERT_TRUE(space2.getString(nameIndex));
  EXPECT_EQ("Space 2", space2.getString(nameIndex).get());
}

TEST_F(IdfFixture, WorkspaceObject_setName_allObjects) {

  std::vector<openstudio::IddObjectType> exclusions{openstudio::IddObjectType::OS_Output_Meter};

  auto osIddFile = openstudio::IddFactory::instance().getIddFile(openstudio::IddFileType::OpenStudio);

  for (const IddObject& iddObject : osIddFile.objects()) {
    auto iddObjectType = iddObject.type();
    if (std::find(exclusions.cbegin(), exclusions.cend(), iddObjectType) == exclusions.cend()) {
      if (auto index_ = iddObject.nameFieldIndex()) {
        Workspace w(StrictnessLevel::Draft, IddFileType::OpenStudio);
        WorkspaceObjectVector objs;
        for (auto i = 0; i < 2; ++i) {
          WorkspaceObject obj = w.addObject(IdfObject(iddObjectType)).get();
          EXPECT_TRUE(obj.setString(index_.get(), "object name"));
          objs.emplace_back(obj);
        }
        EXPECT_NE(objs[0].nameString(), objs[1].nameString()) << "Name unicity isn't enforced for " << iddObject.name();
      }
    }
  }
}

TEST_F(IdfFixture, WorkspaceObject_eraseExtensibleGroups) {

  Workspace w(StrictnessLevel::Draft, IddFileType::OpenStudio);
  WorkspaceObject eqlist = w.addObject(IdfObject(IddObjectType::OS_ZoneHVAC_EquipmentList)).get();

  WorkspaceObject bb_delete = w.addObject(IdfObject(IddObjectType::OS_ZoneHVAC_Baseboard_Convective_Electric)).get();
  {
    auto weg = eqlist.pushExtensibleGroup().cast<WorkspaceExtensibleGroup>();
    EXPECT_TRUE(weg.setPointer(0, bb_delete.handle()));
    EXPECT_TRUE(weg.setUnsigned(1, 1));
    EXPECT_TRUE(weg.setUnsigned(2, 1));
  }

  IdfObject bb_idf(IddObjectType::OS_ZoneHVAC_Baseboard_Convective_Electric);
  bb_idf.setName("Baseboard");

  IdfObject bb_sch_idf(IddObjectType::OS_Schedule_Constant);
  bb_sch_idf.setName(bb_idf.nameString());

  WorkspaceObject bb = w.addObject(bb_idf).get();
  WorkspaceObject bb_sch = w.addObject(bb_sch_idf).get();
  // Because underneath in Workspace m_workspaceObjectMap is a std::unordered_map we have no guarantee of order, so in case it's not good (it IS on
  // Unix, but not on Windows), we redo the insertion in a different order
  {
    auto objects = w.getObjectsByName(bb.nameString());
    EXPECT_EQ(2, objects.size());
    if (objects.front().iddObject().type() != IddObjectType(IddObjectType::OS_Schedule_Constant)) {
      bb.remove();
      bb = w.addObject(bb_idf).get();
    }
  }

  // Assert this time the order is good
  auto objects = w.getObjectsByName(bb.nameString());
  EXPECT_EQ(2, objects.size());
  EXPECT_EQ(IddObjectType(IddObjectType::OS_Schedule_Constant), objects.front().iddObject().type());

  EXPECT_EQ("Baseboard", bb.nameString());
  EXPECT_EQ("Baseboard", bb_sch.nameString());
  EXPECT_EQ(bb.nameString(), bb_sch.nameString());

  EXPECT_TRUE(bb.setPointer(2, bb_sch.handle()));

  {
    auto weg = eqlist.pushExtensibleGroup().cast<WorkspaceExtensibleGroup>();
    EXPECT_TRUE(weg.setPointer(0, bb.handle()));
    EXPECT_TRUE(weg.setUnsigned(1, 1));
    EXPECT_TRUE(weg.setUnsigned(2, 1));
  }

  EXPECT_EQ(2, eqlist.numExtensibleGroups());
  for (const auto& eg : eqlist.extensibleGroups()) {
    const auto weg = eg.cast<WorkspaceExtensibleGroup>();
    auto target_ = weg.getTarget(OS_ZoneHVAC_EquipmentListExtensibleFields::ZoneEquipment);
    ASSERT_TRUE(target_);
    EXPECT_EQ(IddObjectType(IddObjectType::OS_ZoneHVAC_Baseboard_Convective_Electric), target_->iddObject().type());
  }

  eqlist.eraseExtensibleGroup(0);

  EXPECT_EQ(1, eqlist.numExtensibleGroups());
  for (const auto& eg : eqlist.extensibleGroups()) {
    const auto weg = eg.cast<WorkspaceExtensibleGroup>();
    auto target_ = weg.getTarget(OS_ZoneHVAC_EquipmentListExtensibleFields::ZoneEquipment);
    ASSERT_TRUE(target_);
    EXPECT_EQ(IddObjectType(IddObjectType::OS_ZoneHVAC_Baseboard_Convective_Electric), target_->iddObject().type());
  }
}

TEST_F(IdfFixture, WorkspaceObject_SpecialMembers) {

  static_assert(!std::is_trivial<WorkspaceObject>{});
  static_assert(!std::is_pod<WorkspaceObject>{});

  // checks if a type has a default constructor
  static_assert(!std::is_default_constructible<WorkspaceObject>{});
  static_assert(!std::is_trivially_default_constructible<WorkspaceObject>{});
  static_assert(!std::is_nothrow_default_constructible<WorkspaceObject>{});

  // checks if a type has a copy constructor
  static_assert(std::is_copy_constructible<WorkspaceObject>{});
  static_assert(!std::is_trivially_copy_constructible<WorkspaceObject>{});
  static_assert(!std::is_nothrow_copy_constructible<WorkspaceObject>{});

  // checks if a type can be constructed from an rvalue reference
  // Note: Types without a move constructor, but with a copy constructor that accepts const T& arguments, satisfy std::is_move_constructible.
  // Move constructors are usually noexcept, since otherwise they are unusable in any code that provides strong exception guarantee.
  static_assert(std::is_move_constructible<WorkspaceObject>{});
  static_assert(!std::is_trivially_move_constructible<std::shared_ptr<openstudio::detail::WorkspaceObject_Impl>>{});
  static_assert(!std::is_trivially_move_constructible<WorkspaceObject>{});
  static_assert(std::is_nothrow_move_constructible<WorkspaceObject>{});

  // checks if a type has a copy assignment operator
  static_assert(std::is_copy_assignable<WorkspaceObject>{});
  static_assert(!std::is_trivially_copy_assignable<WorkspaceObject>{});
  static_assert(!std::is_nothrow_copy_assignable<WorkspaceObject>{});  // We didn't specify noexcept on the user defined one

  // checks if a type has a move assignment operator
  static_assert(std::is_move_assignable<WorkspaceObject>{});
  static_assert(!std::is_trivially_move_assignable<std::shared_ptr<openstudio::detail::WorkspaceObject_Impl>>{});
  static_assert(!std::is_trivially_move_assignable<WorkspaceObject>{});
  static_assert(std::is_nothrow_move_assignable<WorkspaceObject>{});

  // checks if a type has a non-deleted destructor
  static_assert(std::is_destructible<WorkspaceObject>{});
  static_assert(!std::is_trivially_destructible<std::shared_ptr<openstudio::detail::WorkspaceObject_Impl>>{});
  static_assert(!std::is_trivially_destructible<WorkspaceObject>{});
  static_assert(std::is_nothrow_destructible<WorkspaceObject>{});

  // checks if a type has a virtual destructor
  static_assert(std::has_virtual_destructor<WorkspaceObject>{});

  // checks if objects of a type can be swapped with objects of same or different type
  static_assert(std::is_swappable<WorkspaceObject>{});
  static_assert(std::is_nothrow_swappable<WorkspaceObject>{});
}
