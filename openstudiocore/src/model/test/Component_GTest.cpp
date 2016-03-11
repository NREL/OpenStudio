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

#include "ModelFixture.hpp"

#include "../Model.hpp"
#include "../Model_Impl.hpp"
#include "../Component.hpp"
#include "../Construction.hpp"
#include "../ComponentData.hpp"
#include "../ComponentData_Impl.hpp"
#include "../ScheduleTypeLimits.hpp"
#include "../ScheduleTypeLimits_Impl.hpp"
#include "../ScheduleCompact.hpp"
#include "../ScheduleCompact_Impl.hpp"
#include "../Lights.hpp"
#include "../Lights_Impl.hpp"
#include "../LightsDefinition.hpp"
#include "../LightsDefinition_Impl.hpp"
#include "../DesignDay.hpp"
#include "../DesignDay_Impl.hpp"
#include "../SimulationControl.hpp"
#include "../SimulationControl_Impl.hpp"
#include "../Version.hpp"
#include "../Version_Impl.hpp"

#include "../../utilities/idf/IdfFile.hpp"
#include "../../utilities/idf/Workspace.hpp"
#include "../../utilities/idf/IdfExtensibleGroup.hpp"

#include <utilities/idd/OS_Schedule_Compact_FieldEnums.hxx>
#include <utilities/idd/OS_Lights_FieldEnums.hxx>
#include <utilities/idd/OS_SizingPeriod_DesignDay_FieldEnums.hxx>
#include <utilities/idd/OS_ComponentData_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include "../../utilities/core/PathHelpers.hpp"

#include <resources.hxx>

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, Component_LightingSchedule_FromScratch)
{
  // create schedule component
  Model justASchedule;
  EXPECT_EQ(0u,justASchedule.numObjects()); // Version doesn't count in basic object list
  EXPECT_TRUE(justASchedule.getOptionalUniqueModelObject<Version>());
  ScheduleTypeLimits typeLimits(justASchedule);
  typeLimits.setName("Fraction");
  typeLimits.setLowerLimitValue(0.0);
  typeLimits.setUpperLimitValue(1.0);
  typeLimits.setNumericType("Continuous");
  ScheduleCompact schedule(justASchedule);
  EXPECT_TRUE(schedule.setPointer(OS_Schedule_CompactFields::ScheduleTypeLimitsName,
                                  typeLimits.handle()));
  Component scheduleComponent = schedule.createComponent();
  EXPECT_EQ(static_cast<unsigned>(2),scheduleComponent.componentData().numComponentObjects());
  EXPECT_EQ(static_cast<unsigned>(3),scheduleComponent.numObjects());
  EXPECT_TRUE(scheduleComponent.primaryObject().optionalCast<ScheduleCompact>());

  // create model with Lights objects and set schedules
  Model justLights;
  LightsDefinition lightsDefinition(justLights);
  Lights light1(lightsDefinition);
  EXPECT_EQ(static_cast<unsigned>(2),justLights.numObjects());
  OptionalComponentData ocd = justLights.insertComponent(scheduleComponent);
  EXPECT_EQ(static_cast<unsigned>(5),justLights.numObjects());
  ASSERT_TRUE(ocd);
  OptionalScheduleCompact oSchedule = ocd->primaryComponentObject().optionalCast<ScheduleCompact>();
  ASSERT_TRUE(oSchedule);
  schedule = *oSchedule;
  EXPECT_TRUE(light1.setSchedule(schedule));
  EXPECT_EQ(static_cast<unsigned>(5),justLights.numObjects());
  Lights light2(lightsDefinition);
  EXPECT_EQ(static_cast<unsigned>(6),justLights.numObjects());
  ocd = justLights.insertComponent(scheduleComponent);
  EXPECT_EQ(static_cast<unsigned>(6),justLights.numObjects());
  ASSERT_TRUE(ocd);
  ASSERT_TRUE(ocd->primaryComponentObject().optionalCast<ScheduleCompact>());
  ASSERT_TRUE(light1.schedule());
  EXPECT_EQ(ocd->primaryComponentObject().handle(), light1.schedule()->handle());
  Schedule lightingSchedule = ocd->primaryComponentObject().cast<ScheduleCompact>();
  EXPECT_TRUE(light2.setSchedule(lightingSchedule));

  // create Lights component
  Component lightsComponent = light1.createComponent();
  EXPECT_EQ(static_cast<unsigned>(4),lightsComponent.componentData().numComponentObjects());
  EXPECT_EQ(static_cast<unsigned>(5),lightsComponent.numObjects());
  EXPECT_EQ(static_cast<unsigned>(1),lightsComponent.getModelObjects<ComponentData>().size());
  EXPECT_EQ(static_cast<unsigned>(1),lightsComponent.getModelObjects<Lights>().size());
  EXPECT_EQ(static_cast<unsigned>(1),lightsComponent.getModelObjects<ScheduleTypeLimits>().size());
  EXPECT_EQ(static_cast<unsigned>(1),lightsComponent.getModelObjects<ScheduleCompact>().size());
}

TEST_F(ModelFixture, ComponentWatcher_FromScratch) {
  // create schedule component
  Model justASchedule;
  ScheduleTypeLimits typeLimits(justASchedule);
  typeLimits.setName("Fraction");
  typeLimits.setLowerLimitValue(0.0);
  typeLimits.setUpperLimitValue(1.0);
  typeLimits.setNumericType("Continuous");
  ScheduleCompact schedule(justASchedule);
  EXPECT_TRUE(schedule.setPointer(OS_Schedule_CompactFields::ScheduleTypeLimitsName,
                                  typeLimits.handle()));
  Component scheduleComponent = schedule.createComponent();

  // create model with Lights objects and insert schedule component
  Model justLights;
  LightsDefinition lightsDefinition(justLights);
  Lights light1(lightsDefinition);
  OptionalComponentData ocd = justLights.insertComponent(scheduleComponent);

  // get ComponentData object
  ASSERT_TRUE(ocd);
  ComponentData componentData = *ocd;
  UUID versionUUID = componentData.versionUUID();

  // setting lighting schedule does not invalidate schedule component, or change its version UUID
  OptionalScheduleCompact oSchedule = componentData.primaryComponentObject().optionalCast<ScheduleCompact>();
  ASSERT_TRUE(oSchedule);
  schedule = *oSchedule;
  EXPECT_TRUE(light1.setSchedule(schedule));
  ASSERT_TRUE(componentData.initialized());
  EXPECT_EQ(versionUUID,componentData.versionUUID());

  // changing data field in componentObject does not invalidate schedule component, but does change version UUID
  StringVector values;
  values.push_back("Through: 12/31");
  IdfExtensibleGroup eg = componentData.primaryComponentObject().pushExtensibleGroup(values);
  EXPECT_FALSE(eg.empty());
  ASSERT_TRUE(componentData.initialized());
  EXPECT_NE(versionUUID,componentData.versionUUID());
  versionUUID = componentData.versionUUID();

  // changing type limits used by schedule in component invalidates the component
  Handle h = componentData.handle();
  EXPECT_TRUE(justLights.isMember(h));
  ScheduleTypeLimits newTypeLimits = typeLimits.clone(justLights).cast<ScheduleTypeLimits>();
  EXPECT_FALSE(newTypeLimits.handle() == typeLimits.handle());
  EXPECT_TRUE(schedule.setPointer(OS_Schedule_CompactFields::ScheduleTypeLimitsName,newTypeLimits.handle()));
  EXPECT_FALSE(componentData.initialized());
  EXPECT_FALSE(justLights.isMember(h));
}

TEST_F(ModelFixture, ComponentWatcher_FromLoadedFile) {
  // create osm file that contains ComponentData
  Model model;
  DesignDay designDay(model);
  EXPECT_EQ(static_cast<unsigned>(1),model.numObjects());
  Component designDayComponent = designDay.createComponent();
  model = Model();
  EXPECT_EQ(static_cast<unsigned>(0),model.numObjects());
  OptionalComponentData ocd = model.insertComponent(designDayComponent);
  ASSERT_TRUE(ocd);
  EXPECT_EQ(static_cast<unsigned>(2),model.numObjects());

  // serialize
  openstudio::path p = resourcesPath()/toPath("model/scratch/DesignDay.osm");
  bool ok = model.save(p,true);
  ASSERT_TRUE(ok);

  // load
  OptionalIdfFile oIdfFile = IdfFile::load(p);
  ASSERT_TRUE(oIdfFile);
  model = Model(*oIdfFile);
  EXPECT_EQ(static_cast<unsigned>(2),model.numObjects());

  // get ComponentData object
  ComponentDataVector cdv = model.getModelObjects<ComponentData>();
  ASSERT_EQ(static_cast<unsigned>(1),cdv.size());
  ComponentData cd = cdv[0];
  UUID versionUUID = cd.versionUUID();

  // changing data field in componentObject does not invalidate component, but does change version UUID
  OptionalDesignDay odd = cd.primaryComponentObject().optionalCast<DesignDay>();
  ASSERT_TRUE(odd);
  designDay = *odd;
  EXPECT_TRUE(designDay.setInt(OS_SizingPeriod_DesignDayFields::DayofMonth,17));
  EXPECT_TRUE(cd.initialized());
  EXPECT_NE(versionUUID,cd.versionUUID());
  versionUUID = cd.versionUUID();
  EXPECT_EQ(static_cast<unsigned>(2),model.numObjects());

  // removing object in the component invalidates the component
  Handle h = cd.handle();
  EXPECT_TRUE(model.isMember(h));
  designDay.remove();
  EXPECT_FALSE(cd.initialized());
  EXPECT_FALSE(model.isMember(h));
  EXPECT_EQ(static_cast<unsigned>(0),model.numObjects());
}

TEST_F(ModelFixture, ComponentWatcher_ComponentData_Interactions) {

  // create osm file that contains ComponentData
  Model model;
  DesignDay designDay(model);
  EXPECT_EQ(1u,model.numObjects());
  Component designDayComponent = designDay.createComponent();
  ComponentData componentData = designDayComponent.componentData();
  UUID uuid = componentData.uuid();
  UUID versionUUID = componentData.versionUUID();
  EXPECT_FALSE(versionUUID.isNull());
  model = Model();
  EXPECT_EQ(0u,model.numObjects());
  OptionalComponentData ocd = model.insertComponent(designDayComponent);
  ASSERT_TRUE(ocd);
  EXPECT_EQ(2u,model.numObjects());

  // simple insert should not change component UUIDs.
  componentData = *ocd;
  EXPECT_EQ(uuid,componentData.uuid());
  EXPECT_EQ(versionUUID,componentData.versionUUID());

  // changing data fields of component objects causes version id to change
  componentData.primaryComponentObject().cast<DesignDay>().setMaximumDryBulbTemperature(50.0);
  EXPECT_EQ(uuid,componentData.uuid());
  EXPECT_NE(versionUUID,componentData.versionUUID());
  versionUUID = componentData.versionUUID();

  // trying to change contents field to an invalid value directly is not allowed
  IdfExtensibleGroup eg = componentData.getExtensibleGroup(0);
  ASSERT_FALSE(eg.empty());
  std::string originalValue = eg.getString(0).get();
  EXPECT_EQ("Sizing Period Design Day 1",originalValue);
  EXPECT_FALSE(eg.setString(0,"My Material"));
  EXPECT_TRUE(componentData.initialized());
  EXPECT_EQ(2u,model.numObjects());
  EXPECT_EQ(originalValue,eg.getString(0).get());

  // ComponentData.remove() is okay (should automatically delete the ComponentWatcher)
  ocd = model.insertComponent(designDayComponent);
  ASSERT_TRUE(ocd);
  // should find original DesignDay and reinstantiate ComponentData
  EXPECT_EQ(2u,model.numObjects());
  componentData = *ocd;
  ASSERT_TRUE(componentData.initialized());
  EXPECT_FALSE(componentData.remove().empty());
  EXPECT_EQ(static_cast<unsigned>(1),model.numObjects());
}

TEST_F(ModelFixture, ComponentWatcher_BadComponentDataFromWorkspace) {
  Workspace ws;
  OptionalWorkspaceObject owo = ws.addObject(IdfObject(IddObjectType::OS_ComponentData));
  ASSERT_TRUE(owo);
  // make component data ok except points to non-existent object
  WorkspaceObject cd = *owo;
  OptionalString oName = cd.name(); // should have been set by constructor
  ASSERT_TRUE(oName);
  EXPECT_FALSE(oName->empty());
  cd.setString(OS_ComponentDataFields::UUID,toString(createUUID()));
  cd.setString(OS_ComponentDataFields::VersionUUID,toString(createUUID()));
  StringVector values;
  values.push_back("My Material");
  IdfExtensibleGroup eg = cd.pushExtensibleGroup(values);
  EXPECT_TRUE(eg.empty()); // Cannot register a bad pointer.

  EXPECT_EQ(1u,ws.numObjects());
  Model model(ws);
  // expect ComponentWatcher creation to kick out ComponentData
  EXPECT_EQ(0u,model.numObjects());
}

TEST_F(ModelFixture, ComponentWatcher_InComponent) {
  // create Component. ComponentWatcher should work here too (since Component is Model)
  Model model;
  DesignDay designDay(model);
  EXPECT_EQ(1u,model.numObjects());
  Component designDayComponent = designDay.createComponent();
  EXPECT_EQ(2u,designDayComponent.numObjects());
  ComponentData cd = designDayComponent.componentData();
  UUID versionUUID = cd.versionUUID();
  designDay = designDayComponent.primaryObject().cast<DesignDay>();

  // data changes ok--changes version
  EXPECT_TRUE(designDay.setName("My Design Day"));
  EXPECT_NE(versionUUID,cd.versionUUID());
  versionUUID = cd.versionUUID();

  // trying to assign contents results in refresh of contents
  IdfExtensibleGroup eg = cd.getExtensibleGroup(0);
  ASSERT_FALSE(eg.empty());
  std::string originalValue = eg.getString(0).get();
  EXPECT_FALSE(eg.setString(0,"Material"));
  EXPECT_TRUE(cd.initialized());
  EXPECT_EQ(2u,designDayComponent.numObjects());
  EXPECT_EQ(originalValue,eg.getString(0).get());
}

// test similar to ruby test, but without saving to osp, to see if problem
// can be replicated in C++
TEST_F(ModelFixture, Component_CreateScheduleLibrary) {
  Model model = exampleModel();

  ScheduleVector schedules = model.getModelObjects<Schedule>();
  ComponentVector components;
  for (const Schedule& schedule : schedules) {
    Component newComponent = schedule.createComponent();
    bool ok = newComponent.componentData().setName(schedule.name().get());
    EXPECT_TRUE(ok);
    components.push_back(newComponent);
  }

  int index(1);
  for (Component& component : components) {
    std::stringstream ss;
    ss << "./component" << index;
    openstudio::path p = toPath(ss.str());
    if (boost::filesystem::exists(p)) {
      boost::filesystem::remove_all(p);
    }
    component.save(p / toPath("component.osc"));
    ++index;
  }
}

TEST_F(ModelFixture,Component_BadSwaps) {
  Workspace workspace;
  Model model;
  Model tempModel;
  LightsDefinition light(tempModel);
  Component component = light.createComponent();

  EXPECT_ANY_THROW(workspace.swap(component));
  EXPECT_ANY_THROW(model.swap(component));
  EXPECT_ANY_THROW(component.swap(workspace));
  EXPECT_ANY_THROW(component.swap(model));
}
