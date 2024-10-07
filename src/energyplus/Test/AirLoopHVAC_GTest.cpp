/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>
#include "EnergyPlusFixture.hpp"

#include "../ForwardTranslator.hpp"

#include "../../model/AirLoopHVAC.hpp"
#include "../../model/AirLoopHVAC_Impl.hpp"

#include "../../model/Model.hpp"
#include "../../model/AvailabilityManagerAssignmentList.hpp"
#include "../../model/AvailabilityManagerAssignmentList_Impl.hpp"
#include "../../model/AvailabilityManagerScheduledOn.hpp"
#include "../../model/Node.hpp"
#include "../../model/Schedule.hpp"
#include "../../model/ScheduleCompact.hpp"
#include "../../model/FanVariableVolume.hpp"
#include "../../model/FanConstantVolume.hpp"
#include "../../model/AirTerminalSingleDuctSeriesPIUReheat.hpp"
#include "../../model/AirTerminalSingleDuctConstantVolumeNoReheat.hpp"
#include "../../model/CoilHeatingElectric.hpp"
#include "../../model/DesignSpecificationOutdoorAir.hpp"
#include "../../model/Space.hpp"
#include "../../model/ThermalZone.hpp"
#include "../../model/ThermalZone_Impl.hpp"
#include "../../model/AirLoopHVACOutdoorAirSystem.hpp"
#include "../../model/ControllerOutdoorAir.hpp"
#include "../../model/ControllerMechanicalVentilation.hpp"
#include "../../model/DesignDay.hpp"
#include "../../model/SetpointManagerSingleZoneHumidityMinimum.hpp"
#include "../../model/SetpointManagerScheduled.hpp"
#include "../../model/HumidifierSteamElectric.hpp"
#include "../../model/ScheduleConstant.hpp"

#include "../../utilities/core/Compare.hpp"

#include <utilities/idd/AirLoopHVAC_FieldEnums.hxx>
#include <utilities/idd/AvailabilityManagerAssignmentList_FieldEnums.hxx>
#include <utilities/idd/Fan_ConstantVolume_FieldEnums.hxx>
#include <utilities/idd/Fan_VariableVolume_FieldEnums.hxx>
#include <utilities/idd/AirTerminal_SingleDuct_SeriesPIU_Reheat_FieldEnums.hxx>
#include <utilities/idd/Controller_MechanicalVentilation_FieldEnums.hxx>
#include <utilities/idd/SetpointManager_Scheduled_FieldEnums.hxx>
#include <utilities/idd/SetpointManager_MixedAir_FieldEnums.hxx>
#include <utilities/idd/SetpointManager_SingleZone_Humidity_Minimum_FieldEnums.hxx>

#include <utilities/idd/IddEnums.hxx>
#include "../../utilities/idf/IdfExtensibleGroup.hpp"

using namespace openstudio::energyplus;
using namespace openstudio::model;
using namespace openstudio;

Model createModelWithDummyAirLoop() {

  // Generate the example Model
  Model m = openstudio::model::exampleModel();

  // Remove the existing loop
  AirLoopHVAC example_loop = m.getConcreteModelObjects<AirLoopHVAC>()[0];
  example_loop.remove();

  AirLoopHVAC a(m);

  FanVariableVolume fan(m);
  Node outletNode = a.supplyOutletNode();
  fan.addToNode(outletNode);
  fan.setName("AirLoopHVAC Supply Fan");

  ScheduleCompact hvac_op_sch(m);
  hvac_op_sch.setName("HVAC Operation Schedule");
  a.setAvailabilitySchedule(hvac_op_sch);

  FanConstantVolume piu_fan(m);
  piu_fan.setName("ATU PIU Fan");

  CoilHeatingElectric reheatC(m);
  reheatC.setName("ATU PIU Reheat Coil");

  AirTerminalSingleDuctSeriesPIUReheat piu(m, piu_fan, reheatC);

  // Get the single thermal Zone in the model
  ThermalZone z = m.getConcreteModelObjects<ThermalZone>()[0];

  a.addBranchForZone(z, piu);

  return m;
}

bool checkAvailabilityManagerList(Workspace w, std::vector<std::string> avm_types) {

  // Get AVM list (only one should be present)
  WorkspaceObjectVector idfObjs(w.getObjectsByType(IddObjectType::AvailabilityManagerAssignmentList));
  if (idfObjs.size() != 1u) {
    std::cout << "Expected one AVMList, got " << idfObjs.size() << '\n';
    return false;
  }
  WorkspaceObject idf_avm(idfObjs[0]);

  // The avm_types should have the same size as the extensible groups to begin with
  if (avm_types.size() != idf_avm.extensibleGroups().size()) {
    return false;
  }

  for (std::size_t i = 0; i != avm_types.size(); ++i) {
    IdfExtensibleGroup eg = idf_avm.extensibleGroups()[i];
    std::string s = eg.getString(AvailabilityManagerAssignmentListExtensibleFields::AvailabilityManagerObjectType).get();
    if (avm_types[i] != s) {
      std::cout << "AVM type not matching: expected '" << avm_types[i] << "' and got '" << s << '\n';
      return false;
    }
  }

  return true;
}

bool CheckFanSchedules(Workspace w) {

  boost::optional<WorkspaceObject> _wo;

  // Loop availabilitySchedule ends up on the Fan
  _wo = w.getObjectByTypeAndName(IddObjectType::Fan_VariableVolume, "AirLoopHVAC Supply Fan");
  if (!_wo.is_initialized()) {
    std::cout << "Cannot locate a Fan:VariableVolume named 'AirLoopHVAC Supply Fan'" << '\n';
    return false;
  }

  WorkspaceObject idf_a_fan = _wo.get();
  std::string idf_a_fan_avail = idf_a_fan.getString(Fan_VariableVolumeFields::AvailabilityScheduleName).get();
  if ("HVAC Operation Schedule" != idf_a_fan_avail) {
    std::cout << "AirLoop Fan Availability Schedule Name was expected to be 'HVAC Operation Schedule', instead it is " << idf_a_fan_avail << '\n';
    return false;
  }

  // And on the PIU Fan too
  _wo = w.getObjectByTypeAndName(IddObjectType::Fan_ConstantVolume, "ATU PIU Fan");
  std::string idf__fan_name = idf_a_fan.getString(Fan_ConstantVolumeFields::AvailabilityScheduleName).get();

  if (!_wo.is_initialized()) {
    std::cout << "Cannot locate a Fan:ConstantVolume named 'ATU PIU Fan'" << '\n';
    return false;
  }

  WorkspaceObject idf_piu_fan = _wo.get();
  std::string idf_piu_fan_avail = idf_piu_fan.getString(Fan_ConstantVolumeFields::AvailabilityScheduleName).get();
  if ("HVAC Operation Schedule" != idf_piu_fan_avail) {
    std::cout << "PIU Fan Availability Schedule Name was expected to be 'HVAC Operation Schedule', instead it is " << idf_piu_fan_avail << '\n';
    return false;
  }

  return true;
}

TEST_F(EnergyPlusFixture, ForwardTranslator_AirLoopHVAC_AvailabilityManagers_None) {

  Model m = createModelWithDummyAirLoop();

  // Add new AVMs

  // Not for this case

  // ForwardTranslate
  ForwardTranslator forwardTranslator;
  Workspace w = forwardTranslator.translateModel(m);

  // There should only be an AVM:Scheduled (auto created)
  std::vector<std::string> avm_types;
  avm_types.push_back("AvailabilityManager:Scheduled");

  ASSERT_TRUE(checkAvailabilityManagerList(w, avm_types));

  // Check that the loop availability schedule ended up in the Fan Schedules
  ASSERT_TRUE(CheckFanSchedules(w));

  // m.save(toPath("./AirLoopHVAC_AVM_None.osm"), true);
  // w.save(toPath("./AirLoopHVAC_AVM_None.idf"), true);
}

TEST_F(EnergyPlusFixture, ForwardTranslator_AirLoopHVAC_AvailabilityManagers_NightCycle) {

  Model m = createModelWithDummyAirLoop();

  // Add new AVMs
  AirLoopHVAC a = m.getConcreteModelObjects<AirLoopHVAC>()[0];
  // This triggers creation of an AVM:NightCycle
  a.setNightCycleControlType("CycleOnAny");

  // ForwardTranslate
  ForwardTranslator forwardTranslator;
  Workspace w = forwardTranslator.translateModel(m);

  // There should only be an AVM:Scheduled (auto created)
  std::vector<std::string> avm_types;
  avm_types.push_back("AvailabilityManager:NightCycle");

  ASSERT_TRUE(checkAvailabilityManagerList(w, avm_types));

  // Check that the loop availability schedule ended up in the Fan Schedules
  ASSERT_TRUE(CheckFanSchedules(w));

  // m.save(toPath("./AirLoopHVAC_AVM_NightCycle.osm"), true);
  // w.save(toPath("./AirLoopHVAC_AVM_NightCycle.idf"), true);
}

TEST_F(EnergyPlusFixture, ForwardTranslator_AirLoopHVAC_AvailabilityManagers_ScheduledOn) {

  Model m = createModelWithDummyAirLoop();

  // Add new AVMs
  AirLoopHVAC a = m.getConcreteModelObjects<AirLoopHVAC>()[0];

  AvailabilityManagerScheduledOn avm_schOn(m);
  a.addAvailabilityManager(avm_schOn);

  // ForwardTranslate
  ForwardTranslator forwardTranslator;
  Workspace w = forwardTranslator.translateModel(m);

  // There should only be an AVM:Scheduled (auto created)
  std::vector<std::string> avm_types;
  avm_types.push_back("AvailabilityManager:ScheduledOn");

  ASSERT_TRUE(checkAvailabilityManagerList(w, avm_types));

  // Check that the loop availability schedule ended up in the Fan Schedules
  ASSERT_TRUE(CheckFanSchedules(w));

  // m.save(toPath("./AirLoopHVAC_AVM_ScheduledOn.osm"), true);
  // w.save(toPath("./AirLoopHVAC_AVM_ScheduledOn.idf"), true);
}

TEST_F(EnergyPlusFixture, ForwardTranslator_AirLoopHVAC_AvailabilityManagers_NightCycle_ScheduledOn) {

  Model m = createModelWithDummyAirLoop();

  // Add new AVMs
  AirLoopHVAC a = m.getConcreteModelObjects<AirLoopHVAC>()[0];
  // This triggers creation of an AVM:NightCycle
  a.setNightCycleControlType("CycleOnAny");

  // Add an AVM:ScheduledOn
  AvailabilityManagerScheduledOn avm_schOn(m);
  a.addAvailabilityManager(avm_schOn);

  // Not for this case

  // ForwardTranslate
  ForwardTranslator forwardTranslator;
  Workspace w = forwardTranslator.translateModel(m);

  // There should only be an AVM:Scheduled (auto created)
  std::vector<std::string> avm_types;
  avm_types.push_back("AvailabilityManager:NightCycle");
  avm_types.push_back("AvailabilityManager:ScheduledOn");

  ASSERT_TRUE(checkAvailabilityManagerList(w, avm_types));

  // Check that the loop availability schedule ended up in the Fan Schedules
  ASSERT_TRUE(CheckFanSchedules(w));

  // m.save(toPath("./AirLoopHVAC_AVM_NightCycle_ScheduledOn.osm"), true);
  // w.save(toPath("./AirLoopHVAC_AVM_NightCycle_ScheduledOn.idf"), true);
}

TEST_F(EnergyPlusFixture, ForwardTranslator_AirLoopHVAC_MultiLoop_ControllerMV) {
  // Test for #3926

  Model m;

  ThermalZone z(m);
  Space s(m);
  DesignSpecificationOutdoorAir dsoa(m);
  EXPECT_TRUE(dsoa.setOutdoorAirMethod("Sum"));
  EXPECT_TRUE(dsoa.setOutdoorAirFlowAirChangesperHour(0.5));
  EXPECT_TRUE(s.setThermalZone(z));
  EXPECT_TRUE(s.setDesignSpecificationOutdoorAir(dsoa));

  // Absolutely need a sizing period for the Sizing:Zone to be translated
  DesignDay d(m);

  Schedule alwaysOn = m.alwaysOnDiscreteSchedule();

  AirLoopHVAC a1(m);
  ControllerOutdoorAir controller_oa1(m);
  AirLoopHVACOutdoorAirSystem oa_sys1(m, controller_oa1);
  Node supplyInletNode1 = a1.supplyInletNode();
  EXPECT_TRUE(oa_sys1.addToNode(supplyInletNode1));
  AirTerminalSingleDuctConstantVolumeNoReheat atu1(m, alwaysOn);
  EXPECT_TRUE(a1.multiAddBranchForZone(z, atu1));

  AirLoopHVAC a2(m);
  ControllerOutdoorAir controller_oa2(m);
  AirLoopHVACOutdoorAirSystem oa_sys2(m, controller_oa2);
  Node supplyInletNode2 = a2.supplyInletNode();
  EXPECT_TRUE(oa_sys2.addToNode(supplyInletNode2));
  AirTerminalSingleDuctConstantVolumeNoReheat atu2(m, alwaysOn);
  EXPECT_TRUE(a2.multiAddBranchForZone(z, atu2));

  EXPECT_EQ(2u, z.airLoopHVACs().size());

  ForwardTranslator ft;
  Workspace w = ft.translateModel(m);

  WorkspaceObjectVector idf_controller_mvs(w.getObjectsByType(IddObjectType::Controller_MechanicalVentilation));
  ASSERT_EQ(2u, idf_controller_mvs.size());

  int i = 1;
  for (const auto& idf_controller_mv : idf_controller_mvs) {
    // This construct is weird but I want to showcase that it works fine for the first ControllerMV but not the second one
    // so I don't want to use ASSERT_EQ
    if (idf_controller_mv.numExtensibleGroups() == 1u) {
      IdfExtensibleGroup w_eg = idf_controller_mv.extensibleGroups()[0];
      EXPECT_EQ(z.nameString(), w_eg.getString(Controller_MechanicalVentilationExtensibleFields::ZoneorZoneListName).get());
      EXPECT_EQ(dsoa.nameString(), w_eg.getString(Controller_MechanicalVentilationExtensibleFields::DesignSpecificationOutdoorAirObjectName).get());
    } else {
      EXPECT_EQ(1u, idf_controller_mv.numExtensibleGroups()) << "Failed for " << idf_controller_mv.nameString() << "(i = " << i << ")";
    }
    ++i;
  }
}

TEST_F(EnergyPlusFixture, ForwardTranslator_AirLoopHVAC_designReturnAirFlowFractionofSupplyAirFlow) {

  ForwardTranslator ft;

  Model m;
  AirLoopHVAC a(m);
  // Test Ctor value (E+ IDD default)
  EXPECT_TRUE(a.setDesignReturnAirFlowFractionofSupplyAirFlow(0.5));
  EXPECT_EQ(0.5, a.designReturnAirFlowFractionofSupplyAirFlow());

  Workspace w = ft.translateModel(m);

  // Get AVM list (only one should be present)
  WorkspaceObjectVector idfObjs(w.getObjectsByType(IddObjectType::AirLoopHVAC));
  ASSERT_EQ(1u, idfObjs.size());
  WorkspaceObject idf_loop(idfObjs[0]);
  EXPECT_EQ(0.5, idf_loop.getDouble(AirLoopHVACFields::DesignReturnAirFlowFractionofSupplyAirFlow).get());
}

TEST_F(EnergyPlusFixture, ForwardTranslator_AirLoopHVAC_Automatic_SPMs) {

  Model m;

  // Makes an AirLoopHVAC with:
  //       o   o
  //       |   |
  // ---- OA SYS ----o---HC----o----Fan----o-----Humidifier------
  // |                                                          |
  // o                                                          o < SPM:Scheduled, and SPM:SingleZone:HumidityMinimum
  AirLoopHVAC a(m);

  ThermalZone z(m);
  auto alwaysOn = m.alwaysOnDiscreteSchedule();
  AirTerminalSingleDuctConstantVolumeNoReheat atu(m, alwaysOn);
  a.addBranchForZone(z, atu);

  Node supplyOutletNode = a.supplyOutletNode();

  ControllerOutdoorAir oaController(m);
  AirLoopHVACOutdoorAirSystem oaSystem(m, oaController);
  oaSystem.addToNode(supplyOutletNode);

  CoilHeatingElectric hc(m);
  hc.addToNode(supplyOutletNode);
  hc.setName("Heating Coil");

  FanVariableVolume fan(m);
  fan.setName("AirLoopHVAC Supply Fan");
  EXPECT_TRUE(fan.addToNode(supplyOutletNode));

  HumidifierSteamElectric humidifier(m);
  humidifier.setName("Humidifier");
  EXPECT_TRUE(humidifier.addToNode(supplyOutletNode));

  ScheduleConstant sch(m);
  sch.setName("SPM schedule");
  sch.setValue(15);
  SetpointManagerScheduled spm_scheduled(m, sch);
  spm_scheduled.setName("SPM Scheduled");
  EXPECT_TRUE(spm_scheduled.addToNode(supplyOutletNode));

  SetpointManagerSingleZoneHumidityMinimum spm_hum_min(m);
  spm_hum_min.setName("SPM SZ Min Hum");
  EXPECT_TRUE(spm_hum_min.addToNode(supplyOutletNode));
  // This should be done automatically in addToNode since the zone was hooked already
  EXPECT_TRUE(spm_hum_min.setControlZone(z));

  a.supplyInletNode().setName("Supply Inlet Node");
  a.mixedAirNode()->setName("Mixed Air Node");
  hc.outletModelObject()->setName("Coil to Fan Node");
  fan.outletModelObject()->setName("Fan to Humidifier Node");
  supplyOutletNode.setName("Supply Outlet Node");

  z.zoneAirNode().setName("Zone Air Node");

  // Add new AVMs

  // Not for this case

  // ForwardTranslate
  ForwardTranslator ft;
  Workspace w = ft.translateModel(m);

  // What I expect in terms of SPMs
  //
  //       o   o     SPM:MixedAir      SPM Scheduled (cloned)
  //       |   |     v         v           v
  // ---- OA SYS ----o---HC----o----Fan----o-----Humidifier------
  // |                                                          |
  // o                                                          o < SPM:Scheduled, and SPM:SingleZone:HumidityMinimum

  {
    auto idf_spm_scheduleds = w.getObjectsByType(IddObjectType::SetpointManager_Scheduled);
    ASSERT_EQ(2u, idf_spm_scheduleds.size());
    bool foundSupplyOutlet = false;
    bool foundFanToHumidifierNode = false;

    for (const auto& spm : idf_spm_scheduleds) {

      EXPECT_EQ("Temperature", spm.getString(SetpointManager_ScheduledFields::ControlVariable).get());
      EXPECT_EQ("SPM schedule", spm.getString(SetpointManager_ScheduledFields::ScheduleName).get());

      auto spm_node = spm.getString(SetpointManager_ScheduledFields::SetpointNodeorNodeListName).get();
      if (openstudio::istringEqual("Supply Outlet Node", spm_node)) {
        foundSupplyOutlet = true;
        EXPECT_EQ("SPM Scheduled", spm.nameString());
      } else if (openstudio::istringEqual("Fan to Humidifier Node", spm_node)) {
        foundFanToHumidifierNode = true;
        EXPECT_NE(std::string::npos, spm.nameString().find("OS Default SPM"));
      }
    }
    EXPECT_TRUE(foundSupplyOutlet);
    EXPECT_TRUE(foundFanToHumidifierNode);
  }
  {
    auto idf_spm_mixed_airs = w.getObjectsByType(IddObjectType::SetpointManager_MixedAir);
    ASSERT_EQ(2u, idf_spm_mixed_airs.size());
    bool foundMixedAirNode = false;
    bool foundCoilToFanNode = false;
    for (const auto& spm : idf_spm_mixed_airs) {
      EXPECT_NE(std::string::npos, spm.nameString().find("OS Default SPM"));
      EXPECT_EQ("Temperature", spm.getString(SetpointManager_MixedAirFields::ControlVariable).get());
      EXPECT_EQ("Supply Outlet Node", spm.getString(SetpointManager_MixedAirFields::ReferenceSetpointNodeName).get());
      EXPECT_EQ("Coil to Fan Node", spm.getString(SetpointManager_MixedAirFields::FanInletNodeName).get());
      EXPECT_EQ("Fan to Humidifier Node", spm.getString(SetpointManager_MixedAirFields::FanOutletNodeName).get());

      auto spm_node = spm.getString(SetpointManager_MixedAirFields::SetpointNodeorNodeListName).get();
      if (openstudio::istringEqual("Mixed Air Node", spm_node)) {
        foundMixedAirNode = true;
      } else if (openstudio::istringEqual("Coil to Fan Node", spm_node)) {
        foundCoilToFanNode = true;
      }
    }
    EXPECT_TRUE(foundMixedAirNode);
    EXPECT_TRUE(foundCoilToFanNode);
  }
  {
    auto idf_spm_hum_mins = w.getObjectsByType(IddObjectType::SetpointManager_SingleZone_Humidity_Minimum);
    ASSERT_EQ(1u, idf_spm_hum_mins.size());
    EXPECT_EQ("SPM SZ Min Hum", idf_spm_hum_mins[0].nameString());
    EXPECT_EQ("Supply Outlet Node",
              idf_spm_hum_mins[0].getString(SetpointManager_SingleZone_Humidity_MinimumFields::SetpointNodeorNodeListName).get());
    EXPECT_EQ("Zone Air Node", idf_spm_hum_mins[0].getString(SetpointManager_SingleZone_Humidity_MinimumFields::ControlZoneAirNodeName).get());
  }
}
