/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>
#include "EnergyPlusFixture.hpp"

#include "../ForwardTranslator.hpp"

#include "../../model/Model.hpp"

#include "../../model/AvailabilityManagerNightCycle.hpp"
#include "../../model/ThermalZone.hpp"
#include "../../model/Space.hpp"
#include "../../model/AirLoopHVAC.hpp"
#include "../../model/Schedule.hpp"
#include "../../model/ScheduleConstant.hpp"

#include <utilities/idd/AvailabilityManager_NightCycle_FieldEnums.hxx>
#include <utilities/idd/ZoneList_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>
#include "../../utilities/idf/IdfExtensibleGroup.hpp"

using namespace openstudio::energyplus;
using namespace openstudio::model;
using namespace openstudio;

TEST_F(EnergyPlusFixture, ForwardTranslator_AvailabilityManagerNightCycle) {

  Model m;

  AvailabilityManagerNightCycle avm(m);

  EXPECT_TRUE(avm.setCyclingRunTime(1800));
  EXPECT_EQ(1800, avm.cyclingRunTime());
  EXPECT_TRUE(avm.setCyclingRunTimeControlType("Thermostat"));
  EXPECT_EQ("Thermostat", avm.cyclingRunTimeControlType());

  // Assign it to a loop
  AirLoopHVAC a(m);
  EXPECT_TRUE(a.addAvailabilityManager(avm));

  // Assign Thermal Zones to the AirLoopHVAC
  ThermalZone z1(m);
  Space s1(m);
  s1.setThermalZone(z1);
  ThermalZone z2(m);
  Space s2(m);
  s2.setThermalZone(z2);
  ThermalZone z3(m);
  Space s3(m);
  s3.setThermalZone(z3);
  ThermalZone z4(m);
  Space s4(m);
  s4.setThermalZone(z4);

  EXPECT_TRUE(a.addBranchForZone(z1));
  EXPECT_TRUE(a.addBranchForZone(z2));
  EXPECT_TRUE(a.addBranchForZone(z3));
  EXPECT_TRUE(a.addBranchForZone(z4));

  // Cooling Zones is the only required, we don't set it
  EXPECT_TRUE(avm.setControlType("CycleOnAnyCoolingZone"));

  std::vector<ThermalZone> controlThermalZones = {z1};
  EXPECT_TRUE(avm.setControlThermalZones(controlThermalZones));

  // std::vector<ThermalZone> coolingControlThermalZones = {z1, z2};
  // EXPECT_TRUE(avm.setCoolingControlThermalZones(coolingControlThermalZones));

  std::vector<ThermalZone> heatingControlThermalZones = {z1, z2};
  EXPECT_TRUE(avm.setHeatingControlThermalZones(heatingControlThermalZones));

  std::vector<ThermalZone> heatingZoneFansOnlyThermalZones = {z1, z2, z3};
  EXPECT_TRUE(avm.setHeatingZoneFansOnlyThermalZones(heatingZoneFansOnlyThermalZones));

  // ForwardTranslate
  ForwardTranslator ft;
  Workspace w = ft.translateModel(m);

  // Should have 3 zonelists only (control zone is one zone = just name and no zonelist)
  EXPECT_EQ(3u, w.getObjectsByType(IddObjectType::ZoneList).size());

  WorkspaceObjectVector idfObjs(w.getObjectsByType(IddObjectType::AvailabilityManager_NightCycle));
  EXPECT_EQ(1u, idfObjs.size());
  WorkspaceObject idf_avm(idfObjs[0]);

  // Cycling Run Time
  ASSERT_EQ(avm.cyclingRunTimeControlType(), idf_avm.getString(AvailabilityManager_NightCycleFields::CyclingRunTimeControlType).get());
  ASSERT_EQ(avm.cyclingRunTime(), idf_avm.getDouble(AvailabilityManager_NightCycleFields::CyclingRunTime).get());

  // Check the Zone Lists

  // Control Zones: single zone means no ZoneList, just the name of the zone here
  ASSERT_TRUE(idf_avm.getString(AvailabilityManager_NightCycleFields::ControlZoneorZoneListName));
  EXPECT_EQ(z1.name().get(), idf_avm.getString(AvailabilityManager_NightCycleFields::ControlZoneorZoneListName).get());

  // Heating Control Zones
  boost::optional<WorkspaceObject> idf_heatingControlThermalZones =
    idf_avm.getTarget(AvailabilityManager_NightCycleFields::HeatingControlZoneorZoneListName);
  ASSERT_TRUE(idf_heatingControlThermalZones);
  EXPECT_EQ(heatingControlThermalZones.size(), idf_heatingControlThermalZones->extensibleGroups().size());

  // Heating Zone Fans Only Zones
  boost::optional<WorkspaceObject> idf_heatingZoneFansOnlyThermalZones =
    idf_avm.getTarget(AvailabilityManager_NightCycleFields::HeatingZoneFansOnlyZoneorZoneListName);
  ASSERT_TRUE(idf_heatingZoneFansOnlyThermalZones);
  EXPECT_EQ(heatingZoneFansOnlyThermalZones.size(), idf_heatingZoneFansOnlyThermalZones->extensibleGroups().size());

  // Cooling, here it should have been defaulted to all zones served by the AirLoopHVAC
  boost::optional<WorkspaceObject> idf_coolingControlThermalZones =
    idf_avm.getTarget(AvailabilityManager_NightCycleFields::CoolingControlZoneorZoneListName);
  ASSERT_TRUE(idf_coolingControlThermalZones);
  EXPECT_EQ(a.thermalZones().size(), idf_coolingControlThermalZones->extensibleGroups().size());

  // Second Check

  // If we set to CycleOnAny, the cooling zone list should be present because no defaulting needed
  EXPECT_TRUE(avm.setControlType("CycleOnAny"));

  // Re translate
  w = ft.translateModel(m);

  // Should have 2 zonelists only (control zone is one zone = just name and no zonelist, and no cooling one)
  EXPECT_EQ(2u, w.getObjectsByType(IddObjectType::ZoneList).size());

  WorkspaceObjectVector idfObjs2(w.getObjectsByType(IddObjectType::AvailabilityManager_NightCycle));
  EXPECT_EQ(1u, idfObjs2.size());
  WorkspaceObject idf_avm2(idfObjs2[0]);

  boost::optional<WorkspaceObject> idf_coolingControlThermalZones2 =
    idf_avm2.getTarget(AvailabilityManager_NightCycleFields::CoolingControlZoneorZoneListName);
  EXPECT_FALSE(idf_coolingControlThermalZones2);

  // Schedules

  // default
  EXPECT_EQ(avm.fanSchedule()->name().get(), idf_avm2.getString(AvailabilityManager_NightCycleFields::FanScheduleName).get());
  EXPECT_EQ(a.availabilitySchedule().name().get(), idf_avm2.getString(AvailabilityManager_NightCycleFields::FanScheduleName).get());
  EXPECT_EQ("Always On Discrete", idf_avm2.getString(AvailabilityManager_NightCycleFields::FanScheduleName).get());

  EXPECT_EQ(avm.applicabilitySchedule().name().get(), idf_avm2.getString(AvailabilityManager_NightCycleFields::ApplicabilityScheduleName).get());
  EXPECT_EQ("Always On Discrete", idf_avm2.getString(AvailabilityManager_NightCycleFields::ApplicabilityScheduleName).get());

  // hardset them
  ScheduleConstant sch_op(m);
  sch_op.setName("HVAC Operation");
  sch_op.setValue(1.0);
  EXPECT_TRUE(a.setAvailabilitySchedule(sch_op));

  ScheduleConstant sch_applicability(m);
  sch_applicability.setName("AVM NightCycle Applicability Schedule");
  sch_applicability.setValue(1.0);
  EXPECT_TRUE(avm.setApplicabilitySchedule(sch_applicability));

  // Re translate
  w = ft.translateModel(m);

  WorkspaceObjectVector idfObjs3(w.getObjectsByType(IddObjectType::AvailabilityManager_NightCycle));
  EXPECT_EQ(1u, idfObjs3.size());
  WorkspaceObject idf_avm3(idfObjs3[0]);

  EXPECT_EQ(avm.fanSchedule()->name().get(), idf_avm3.getString(AvailabilityManager_NightCycleFields::FanScheduleName).get());
  EXPECT_EQ(a.availabilitySchedule().name().get(), idf_avm3.getString(AvailabilityManager_NightCycleFields::FanScheduleName).get());
  EXPECT_EQ("HVAC Operation", idf_avm3.getString(AvailabilityManager_NightCycleFields::FanScheduleName).get());

  EXPECT_EQ(avm.applicabilitySchedule().name().get(), idf_avm3.getString(AvailabilityManager_NightCycleFields::ApplicabilityScheduleName).get());
  EXPECT_EQ("AVM NightCycle Applicability Schedule", idf_avm3.getString(AvailabilityManager_NightCycleFields::ApplicabilityScheduleName).get());
}
