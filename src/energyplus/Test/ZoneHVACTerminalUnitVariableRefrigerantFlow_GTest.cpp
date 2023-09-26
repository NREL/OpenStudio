/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>
#include "EnergyPlusFixture.hpp"

#include "../ForwardTranslator.hpp"
// #include "../ReverseTranslator.hpp"

#include "../../model/Model.hpp"

#include "../../model/ZoneHVACTerminalUnitVariableRefrigerantFlow.hpp"
#include "../../model/FanOnOff.hpp"
#include "../../model/CoilCoolingDXVariableRefrigerantFlow.hpp"
#include "../../model/CoilHeatingDXVariableRefrigerantFlow.hpp"

#include "../../model/CoilHeatingElectric.hpp"
#include "../../model/CoilHeatingGas.hpp"
#include "../../model/CoilHeatingWater.hpp"

#include "../../model/ThermalZone.hpp"
#include "../../model/Space.hpp"
#include "../../model/Node.hpp"
#include "../../model/Node_Impl.hpp"
#include "../../model/AirLoopHVAC.hpp"
#include "../../model/AirLoopHVACOutdoorAirSystem.hpp"
#include "../../model/ControllerOutdoorAir.hpp"
#include "../../model/AirTerminalSingleDuctConstantVolumeNoReheat.hpp"
#include "../../model/AirTerminalSingleDuctInletSideMixer.hpp"
#include "../../model/Schedule.hpp"
#include "../../model/ScheduleConstant.hpp"
#include "../../model/SetpointManagerScheduled.hpp"
#include "../../model/FanSystemModel.hpp"
#include "../../model/UnitarySystemPerformanceMultispeed.hpp"

#include "../../utilities/idf/IdfObject.hpp"
#include "../../utilities/idf/IdfObject_Impl.hpp"
#include "../../utilities/idf/WorkspaceObject.hpp"
#include "../../utilities/idf/WorkspaceObject_Impl.hpp"
#include "../../utilities/idf/IdfExtensibleGroup.hpp"
#include "../../utilities/idf/WorkspaceExtensibleGroup.hpp"

#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/ZoneHVAC_TerminalUnit_VariableRefrigerantFlow_FieldEnums.hxx>
#include <utilities/idd/Coil_Cooling_DX_VariableRefrigerantFlow_FieldEnums.hxx>
#include <utilities/idd/Coil_Heating_DX_VariableRefrigerantFlow_FieldEnums.hxx>
#include <utilities/idd/Fan_OnOff_FieldEnums.hxx>
#include <utilities/idd/Coil_Heating_Electric_FieldEnums.hxx>
#include <utilities/idd/Coil_Heating_Fuel_FieldEnums.hxx>
#include <utilities/idd/Coil_Heating_Water_FieldEnums.hxx>
// Not yet supported in OS
// #include <utilities/idd/Coil_Heating_Steam_FieldEnums.hxx>
#include <utilities/idd/AirLoopHVAC_FieldEnums.hxx>
#include <utilities/idd/BranchList_FieldEnums.hxx>
#include <utilities/idd/Branch_FieldEnums.hxx>
#include <utilities/idd/AirLoopHVAC_OutdoorAirSystem_FieldEnums.hxx>
#include <utilities/idd/AirLoopHVAC_OutdoorAirSystem_EquipmentList_FieldEnums.hxx>
#include <utilities/idd/OutdoorAir_Mixer_FieldEnums.hxx>
#include <utilities/idd/SetpointManager_Scheduled_FieldEnums.hxx>
#include <utilities/idd/SetpointManager_MixedAir_FieldEnums.hxx>
#include <utilities/idd/Fan_SystemModel_FieldEnums.hxx>
#include <utilities/idd/UnitarySystemPerformance_Multispeed_FieldEnums.hxx>

#include <resources.hxx>

#include <sstream>

using namespace openstudio::energyplus;
using namespace openstudio::model;
using namespace openstudio;

TEST_F(EnergyPlusFixture, ForwardTranslatorZoneHVACTerminalUnitVariableRefrigerantFlow_NodeConnections) {

  Model m;

  FanOnOff fan(m);
  CoilCoolingDXVariableRefrigerantFlow cc(m);
  CoilHeatingDXVariableRefrigerantFlow hc(m);

  ZoneHVACTerminalUnitVariableRefrigerantFlow vrf(m, cc, hc, fan);

  UnitarySystemPerformanceMultispeed perf(m);
  perf.setName("US Perf Multispeed");
  EXPECT_TRUE(vrf.setDesignSpecificationMultispeedObject(perf));

  ThermalZone z(m);
  Space s(m);
  s.setThermalZone(z);

  EXPECT_TRUE(vrf.addToThermalZone(z));

  ForwardTranslator ft;

  // VRF defaults to DrawThrough, so components are ordered like
  // InletNode ----- (Mixer) ---- CoolingCoil ----- HeatingCoil ----- Fan ---- (Supplemental Heating Coil) -------- OutletNode

  // No Supplemental HC
  {
    // Translate
    Workspace w = ft.translateModel(m);
    // No Errors
    EXPECT_EQ(0u, ft.errors().size());

    std::vector<WorkspaceObject> idf_vrfs = w.getObjectsByType(IddObjectType::ZoneHVAC_TerminalUnit_VariableRefrigerantFlow);
    std::vector<WorkspaceObject> idf_ccs = w.getObjectsByType(IddObjectType::Coil_Cooling_DX_VariableRefrigerantFlow);
    std::vector<WorkspaceObject> idf_hcs = w.getObjectsByType(IddObjectType::Coil_Heating_DX_VariableRefrigerantFlow);
    std::vector<WorkspaceObject> idf_fans = w.getObjectsByType(IddObjectType::Fan_OnOff);
    std::vector<WorkspaceObject> idf_supHCs = w.getObjectsByType(IddObjectType::Coil_Heating_Electric);

    EXPECT_EQ(1u, idf_vrfs.size());
    EXPECT_EQ(1u, idf_ccs.size());
    EXPECT_EQ(1u, idf_hcs.size());
    EXPECT_EQ(1u, idf_fans.size());
    EXPECT_EQ(0u, idf_supHCs.size());

    WorkspaceObject idf_vrf = idf_vrfs[0];
    WorkspaceObject idf_cc = idf_ccs[0];
    WorkspaceObject idf_hc = idf_hcs[0];
    WorkspaceObject idf_fan = idf_fans[0];

    // Check Node Connections
    // --- CC --- HC --- Fan
    EXPECT_EQ(idf_cc.getString(Coil_Cooling_DX_VariableRefrigerantFlowFields::CoilAirOutletNode).get(),
              idf_hc.getString(Coil_Heating_DX_VariableRefrigerantFlowFields::CoilAirInletNode).get());

    EXPECT_EQ(idf_hc.getString(Coil_Heating_DX_VariableRefrigerantFlowFields::CoilAirOutletNode).get(),
              idf_fan.getString(Fan_OnOffFields::AirInletNodeName).get());
  }

  // Supplemental HC = CoilHeatingElectric
  {
    CoilHeatingElectric supHC(m);
    EXPECT_TRUE(vrf.setSupplementalHeatingCoil(supHC));

    // Translate
    Workspace w = ft.translateModel(m);
    // No Errors
    EXPECT_EQ(0u, ft.errors().size());

    std::vector<WorkspaceObject> idf_vrfs = w.getObjectsByType(IddObjectType::ZoneHVAC_TerminalUnit_VariableRefrigerantFlow);
    std::vector<WorkspaceObject> idf_ccs = w.getObjectsByType(IddObjectType::Coil_Cooling_DX_VariableRefrigerantFlow);
    std::vector<WorkspaceObject> idf_hcs = w.getObjectsByType(IddObjectType::Coil_Heating_DX_VariableRefrigerantFlow);
    std::vector<WorkspaceObject> idf_fans = w.getObjectsByType(IddObjectType::Fan_OnOff);
    std::vector<WorkspaceObject> idf_supHCs = w.getObjectsByType(IddObjectType::Coil_Heating_Electric);

    EXPECT_EQ(1u, idf_vrfs.size());
    EXPECT_EQ(1u, idf_ccs.size());
    EXPECT_EQ(1u, idf_hcs.size());
    EXPECT_EQ(1u, idf_fans.size());
    EXPECT_EQ(1u, idf_supHCs.size());

    WorkspaceObject idf_vrf = idf_vrfs[0];
    WorkspaceObject idf_cc = idf_ccs[0];
    WorkspaceObject idf_hc = idf_hcs[0];
    WorkspaceObject idf_fan = idf_fans[0];
    WorkspaceObject idf_supHC = idf_supHCs[0];

    // Check Node Connections
    // --- CC --- HC --- Fan --- supHC
    EXPECT_EQ(idf_cc.getString(Coil_Cooling_DX_VariableRefrigerantFlowFields::CoilAirOutletNode).get(),
              idf_hc.getString(Coil_Heating_DX_VariableRefrigerantFlowFields::CoilAirInletNode).get());

    EXPECT_EQ(idf_hc.getString(Coil_Heating_DX_VariableRefrigerantFlowFields::CoilAirOutletNode).get(),
              idf_fan.getString(Fan_OnOffFields::AirInletNodeName).get());

    EXPECT_EQ(idf_fan.getString(Fan_OnOffFields::AirOutletNodeName).get(), idf_supHC.getString(Coil_Heating_ElectricFields::AirInletNodeName).get());
  }

  // Supplemental HC = CoilHeatingGas (translates to Coil_Heating_Fuel)
  {
    CoilHeatingGas supHC(m);
    EXPECT_TRUE(vrf.setSupplementalHeatingCoil(supHC));

    // Translate
    Workspace w = ft.translateModel(m);
    // No Errors
    EXPECT_EQ(0u, ft.errors().size());

    std::vector<WorkspaceObject> idf_vrfs = w.getObjectsByType(IddObjectType::ZoneHVAC_TerminalUnit_VariableRefrigerantFlow);
    std::vector<WorkspaceObject> idf_ccs = w.getObjectsByType(IddObjectType::Coil_Cooling_DX_VariableRefrigerantFlow);
    std::vector<WorkspaceObject> idf_hcs = w.getObjectsByType(IddObjectType::Coil_Heating_DX_VariableRefrigerantFlow);
    std::vector<WorkspaceObject> idf_fans = w.getObjectsByType(IddObjectType::Fan_OnOff);
    std::vector<WorkspaceObject> idf_supHCs = w.getObjectsByType(IddObjectType::Coil_Heating_Fuel);

    EXPECT_EQ(1u, idf_vrfs.size());
    EXPECT_EQ(1u, idf_ccs.size());
    EXPECT_EQ(1u, idf_hcs.size());
    EXPECT_EQ(1u, idf_fans.size());
    EXPECT_EQ(1u, idf_supHCs.size());

    WorkspaceObject idf_vrf = idf_vrfs[0];
    WorkspaceObject idf_cc = idf_ccs[0];
    WorkspaceObject idf_hc = idf_hcs[0];
    WorkspaceObject idf_fan = idf_fans[0];
    WorkspaceObject idf_supHC = idf_supHCs[0];

    // Check Node Connections
    // --- CC --- HC --- Fan --- supHC
    EXPECT_EQ(idf_cc.getString(Coil_Cooling_DX_VariableRefrigerantFlowFields::CoilAirOutletNode).get(),
              idf_hc.getString(Coil_Heating_DX_VariableRefrigerantFlowFields::CoilAirInletNode).get());

    EXPECT_EQ(idf_hc.getString(Coil_Heating_DX_VariableRefrigerantFlowFields::CoilAirOutletNode).get(),
              idf_fan.getString(Fan_OnOffFields::AirInletNodeName).get());

    EXPECT_EQ(idf_fan.getString(Fan_OnOffFields::AirOutletNodeName).get(), idf_supHC.getString(Coil_Heating_FuelFields::AirInletNodeName).get());
  }

  // Supplemental HC = CoilHeatingWater
  {
    CoilHeatingWater supHC(m);
    EXPECT_TRUE(vrf.setSupplementalHeatingCoil(supHC));

    // Translate
    Workspace w = ft.translateModel(m);
    // No Errors
    EXPECT_EQ(0u, ft.errors().size());

    std::vector<WorkspaceObject> idf_vrfs = w.getObjectsByType(IddObjectType::ZoneHVAC_TerminalUnit_VariableRefrigerantFlow);
    std::vector<WorkspaceObject> idf_ccs = w.getObjectsByType(IddObjectType::Coil_Cooling_DX_VariableRefrigerantFlow);
    std::vector<WorkspaceObject> idf_hcs = w.getObjectsByType(IddObjectType::Coil_Heating_DX_VariableRefrigerantFlow);
    std::vector<WorkspaceObject> idf_fans = w.getObjectsByType(IddObjectType::Fan_OnOff);
    std::vector<WorkspaceObject> idf_supHCs = w.getObjectsByType(IddObjectType::Coil_Heating_Water);

    EXPECT_EQ(1u, idf_vrfs.size());
    EXPECT_EQ(1u, idf_ccs.size());
    EXPECT_EQ(1u, idf_hcs.size());
    EXPECT_EQ(1u, idf_fans.size());
    EXPECT_EQ(1u, idf_supHCs.size());

    WorkspaceObject idf_vrf = idf_vrfs[0];
    WorkspaceObject idf_cc = idf_ccs[0];
    WorkspaceObject idf_hc = idf_hcs[0];
    WorkspaceObject idf_fan = idf_fans[0];
    WorkspaceObject idf_supHC = idf_supHCs[0];

    // Check Node Connections
    // --- CC --- HC --- Fan --- supHC
    EXPECT_EQ(idf_cc.getString(Coil_Cooling_DX_VariableRefrigerantFlowFields::CoilAirOutletNode).get(),
              idf_hc.getString(Coil_Heating_DX_VariableRefrigerantFlowFields::CoilAirInletNode).get());

    EXPECT_EQ(idf_hc.getString(Coil_Heating_DX_VariableRefrigerantFlowFields::CoilAirOutletNode).get(),
              idf_fan.getString(Fan_OnOffFields::AirInletNodeName).get());

    EXPECT_EQ(idf_fan.getString(Fan_OnOffFields::AirOutletNodeName).get(), idf_supHC.getString(Coil_Heating_WaterFields::AirInletNodeName).get());
  }

  // Supplemental HC = CoilHeatingWater, BlowThrough
  {
    EXPECT_TRUE(vrf.setSupplyAirFanPlacement("BlowThrough"));

    // Translate
    Workspace w = ft.translateModel(m);
    // No Errors
    EXPECT_EQ(0u, ft.errors().size());

    std::vector<WorkspaceObject> idf_vrfs = w.getObjectsByType(IddObjectType::ZoneHVAC_TerminalUnit_VariableRefrigerantFlow);
    std::vector<WorkspaceObject> idf_ccs = w.getObjectsByType(IddObjectType::Coil_Cooling_DX_VariableRefrigerantFlow);
    std::vector<WorkspaceObject> idf_hcs = w.getObjectsByType(IddObjectType::Coil_Heating_DX_VariableRefrigerantFlow);
    std::vector<WorkspaceObject> idf_fans = w.getObjectsByType(IddObjectType::Fan_OnOff);
    std::vector<WorkspaceObject> idf_supHCs = w.getObjectsByType(IddObjectType::Coil_Heating_Water);
    std::vector<WorkspaceObject> idf_perfs = w.getObjectsByType(IddObjectType::UnitarySystemPerformance_Multispeed);

    EXPECT_EQ(1u, idf_vrfs.size());
    EXPECT_EQ(1u, idf_ccs.size());
    EXPECT_EQ(1u, idf_hcs.size());
    EXPECT_EQ(1u, idf_fans.size());
    EXPECT_EQ(1u, idf_supHCs.size());
    EXPECT_EQ(1u, idf_perfs.size());

    WorkspaceObject idf_vrf = idf_vrfs[0];
    WorkspaceObject idf_cc = idf_ccs[0];
    WorkspaceObject idf_hc = idf_hcs[0];
    WorkspaceObject idf_fan = idf_fans[0];
    WorkspaceObject idf_supHC = idf_supHCs[0];

    // Check Node Connections
    // --- OA Mixer -- Fan -- CC --- HC --- Fan --- supHC

    EXPECT_EQ("BlowThrough", idf_vrf.getString(ZoneHVAC_TerminalUnit_VariableRefrigerantFlowFields::SupplyAirFanPlacement).get());

    auto oa_mixer_ = idf_vrf.getTarget(ZoneHVAC_TerminalUnit_VariableRefrigerantFlowFields::OutsideAirMixerObjectName);
    ASSERT_TRUE(oa_mixer_);
    EXPECT_EQ(idf_vrf.getString(ZoneHVAC_TerminalUnit_VariableRefrigerantFlowFields::TerminalUnitAirInletNodeName).get(),
              oa_mixer_->getString(OutdoorAir_MixerFields::ReturnAirStreamNodeName).get());

    EXPECT_EQ(oa_mixer_->getString(OutdoorAir_MixerFields::MixedAirNodeName).get(), idf_fan.getString(Fan_OnOffFields::AirInletNodeName).get());

    EXPECT_EQ(idf_fan.getString(Fan_OnOffFields::AirOutletNodeName).get(),
              idf_cc.getString(Coil_Cooling_DX_VariableRefrigerantFlowFields::CoilAirInletNode).get());

    EXPECT_EQ(idf_cc.getString(Coil_Cooling_DX_VariableRefrigerantFlowFields::CoilAirOutletNode).get(),
              idf_hc.getString(Coil_Heating_DX_VariableRefrigerantFlowFields::CoilAirInletNode).get());

    EXPECT_EQ(idf_hc.getString(Coil_Heating_DX_VariableRefrigerantFlowFields::CoilAirOutletNode).get(),
              idf_supHC.getString(Coil_Heating_WaterFields::AirInletNodeName).get());

    EXPECT_EQ(idf_supHC.getString(Coil_Heating_WaterFields::AirOutletNodeName).get(),
              idf_vrf.getString(ZoneHVAC_TerminalUnit_VariableRefrigerantFlowFields::TerminalUnitAirOutletNodeName).get());

    EXPECT_EQ("UnitarySystemPerformance:Multispeed",
              idf_vrf.getString(ZoneHVAC_TerminalUnit_VariableRefrigerantFlowFields::DesignSpecificationMultispeedObjectType).get());
    EXPECT_EQ("US Perf Multispeed",
              idf_vrf.getString(ZoneHVAC_TerminalUnit_VariableRefrigerantFlowFields::DesignSpecificationMultispeedObjectName).get());

    IdfObject idf_perf = w.getObjectsByType(IddObjectType::UnitarySystemPerformance_Multispeed)[0];

    EXPECT_EQ(1, idf_perf.getInt(UnitarySystemPerformance_MultispeedFields::NumberofSpeedsforHeating).get());
    EXPECT_EQ(1, idf_perf.getInt(UnitarySystemPerformance_MultispeedFields::NumberofSpeedsforCooling).get());
    EXPECT_EQ("No", idf_perf.getString(UnitarySystemPerformance_MultispeedFields::SingleModeOperation).get());
    EXPECT_EQ(1.0, idf_perf.getDouble(UnitarySystemPerformance_MultispeedFields::NoLoadSupplyAirFlowRateRatio).get());

    ASSERT_EQ(0u, idf_perf.numExtensibleGroups());
  }
}

TEST_F(EnergyPlusFixture, ForwardTranslatorZoneHVACTerminalUnitVariableRefrigerantFlow_SupplementalHeating) {

  Model m;

  FanOnOff fan(m);
  CoilCoolingDXVariableRefrigerantFlow cc(m);
  CoilHeatingDXVariableRefrigerantFlow hc(m);

  ZoneHVACTerminalUnitVariableRefrigerantFlow vrf(m, cc, hc, fan);

  ThermalZone z(m);
  Space s(m);
  s.setThermalZone(z);
  EXPECT_TRUE(vrf.addToThermalZone(z));

  ForwardTranslator ft;

  {
    vrf.autosizeMaximumSupplyAirTemperaturefromSupplementalHeater();
    vrf.setMaximumOutdoorDryBulbTemperatureforSupplementalHeaterOperation(19.5);

    // Translate
    Workspace w = ft.translateModel(m);
    // No Errors
    EXPECT_EQ(0u, ft.errors().size());

    std::vector<WorkspaceObject> idf_vrfs = w.getObjectsByType(IddObjectType::ZoneHVAC_TerminalUnit_VariableRefrigerantFlow);
    EXPECT_EQ(1u, idf_vrfs.size());
    WorkspaceObject idf_vrf = idf_vrfs[0];

    EXPECT_TRUE(openstudio::istringEqual(
      "Autosize", idf_vrf.getString(ZoneHVAC_TerminalUnit_VariableRefrigerantFlowFields::MaximumSupplyAirTemperaturefromSupplementalHeater).get()));
    EXPECT_EQ(
      19.5,
      idf_vrf.getDouble(ZoneHVAC_TerminalUnit_VariableRefrigerantFlowFields::MaximumOutdoorDryBulbTemperatureforSupplementalHeaterOperation).get());
  }

  {
    vrf.setMaximumSupplyAirTemperaturefromSupplementalHeater(35.2);
    vrf.setMaximumOutdoorDryBulbTemperatureforSupplementalHeaterOperation(15.5);

    // Translate
    Workspace w = ft.translateModel(m);
    // No Errors
    EXPECT_EQ(0u, ft.errors().size());

    std::vector<WorkspaceObject> idf_vrfs = w.getObjectsByType(IddObjectType::ZoneHVAC_TerminalUnit_VariableRefrigerantFlow);
    EXPECT_EQ(1u, idf_vrfs.size());
    WorkspaceObject idf_vrf = idf_vrfs[0];

    EXPECT_EQ(35.2, idf_vrf.getDouble(ZoneHVAC_TerminalUnit_VariableRefrigerantFlowFields::MaximumSupplyAirTemperaturefromSupplementalHeater).get());
    EXPECT_EQ(
      15.5,
      idf_vrf.getDouble(ZoneHVAC_TerminalUnit_VariableRefrigerantFlowFields::MaximumOutdoorDryBulbTemperatureforSupplementalHeaterOperation).get());
  }
}

TEST_F(EnergyPlusFixture, ForwardTranslatorZoneHVACTerminalUnitVariableRefrigerantFlow_AirLoopHVAC) {

  Model m;

  FanOnOff fan(m);
  CoilCoolingDXVariableRefrigerantFlow cc(m);
  CoilHeatingDXVariableRefrigerantFlow hc(m);

  ZoneHVACTerminalUnitVariableRefrigerantFlow vrf(m, cc, hc, fan);

  AirLoopHVAC a(m);
  Node supplyOutletNode = a.supplyOutletNode();

  ControllerOutdoorAir controllerOutdoorAir(m);
  AirLoopHVACOutdoorAirSystem outdoorAirSystem(m, controllerOutdoorAir);
  outdoorAirSystem.addToNode(supplyOutletNode);

  EXPECT_TRUE(vrf.addToNode(supplyOutletNode));
  ScheduleConstant sch(m);
  SetpointManagerScheduled spm(m, sch);
  EXPECT_TRUE(spm.addToNode(supplyOutletNode));

  ThermalZone z(m);
  Space s(m);
  s.setThermalZone(z);
  auto alwaysOn = m.alwaysOnDiscreteSchedule();
  AirTerminalSingleDuctConstantVolumeNoReheat atu(m, alwaysOn);
  EXPECT_TRUE(a.addBranchForZone(z, atu));

  EXPECT_TRUE(vrf.setControllingZoneorThermostatLocation(z));

  ZoneHVACTerminalUnitVariableRefrigerantFlow vrf_oa(m);
  {
    boost::optional<Node> OANode = outdoorAirSystem.outboardOANode();
    ASSERT_TRUE(OANode);
    vrf_oa.setName("VRF TU on Outdoor Air Intake");
    vrf_oa.addToNode(OANode.get());
    vrf_oa.outletNode()->setName("VRF TU on Outdoor Air Outlet Node");
    OANode->setName("Outdoor Air Inlet Node Name");
  }

  ZoneHVACTerminalUnitVariableRefrigerantFlow vrf_relief_oa(m);
  {
    boost::optional<Node> reliefNode = outdoorAirSystem.outboardReliefNode();
    ASSERT_TRUE(reliefNode);
    vrf_relief_oa.setName("VRF TU on Relief Air");
    vrf_relief_oa.addToNode(reliefNode.get());
    reliefNode->setName("Relief Node Name");
    vrf_relief_oa.inletNode()->setName("VRF TU on Relief Air Inlet Node");
  }

  a.mixedAirNode()->setName("Mixed Air Node");
  a.supplyInletNode().setName("Supply Inlet Node");
  a.supplyOutletNode().setName("Supply Outlet Node");

  ForwardTranslator ft;

  // The main branch of the AirLoopHVAC has no fan
  {
    Workspace w = ft.translateModel(m);

    std::vector<WorkspaceObject> idf_vrfs = w.getObjectsByType(IddObjectType::ZoneHVAC_TerminalUnit_VariableRefrigerantFlow);
    std::vector<WorkspaceObject> idf_ccs = w.getObjectsByType(IddObjectType::Coil_Cooling_DX_VariableRefrigerantFlow);
    std::vector<WorkspaceObject> idf_hcs = w.getObjectsByType(IddObjectType::Coil_Heating_DX_VariableRefrigerantFlow);
    std::vector<WorkspaceObject> idf_fans = w.getObjectsByType(IddObjectType::Fan_OnOff);
    std::vector<WorkspaceObject> idf_supHCs = w.getObjectsByType(IddObjectType::Coil_Heating_Water);

    EXPECT_EQ(3u, idf_vrfs.size());
    EXPECT_EQ(3u, idf_ccs.size());
    EXPECT_EQ(3u, idf_hcs.size());
    EXPECT_EQ(1u, idf_fans.size());  // Only the one in the VRF TU that is on the main branch

    // Go from AirLoopHVAC to BranchList to Branch
    WorkspaceObjectVector idf_airloops(w.getObjectsByType(IddObjectType::AirLoopHVAC));
    ASSERT_EQ(1u, idf_airloops.size());
    WorkspaceObject idf_airLoopHVAC = idf_airloops[0];
    WorkspaceObject idf_brlist = idf_airLoopHVAC.getTarget(AirLoopHVACFields::BranchListName).get();

    // Should have one branch only
    ASSERT_EQ(1u, idf_brlist.extensibleGroups().size());
    auto w_eg = idf_brlist.extensibleGroups()[0].cast<WorkspaceExtensibleGroup>();
    WorkspaceObject idf_branch = w_eg.getTarget(BranchListExtensibleFields::BranchName).get();

    // There should be two pieces of equipment on the branch
    ASSERT_EQ(2u, idf_branch.extensibleGroups().size());

    // 1: the AirLoopHVACOutdoorAirSystem
    ASSERT_EQ(1u, w.getObjectsByType(IddObjectType::AirLoopHVAC_OutdoorAirSystem).size());
    {
      auto w_eg2 = idf_branch.extensibleGroups()[0].cast<WorkspaceExtensibleGroup>();

      EXPECT_EQ("AirLoopHVAC:OutdoorAirSystem", w_eg2.getString(BranchExtensibleFields::ComponentObjectType).get());
      EXPECT_EQ(w_eg2.getString(BranchExtensibleFields::ComponentName).get(), outdoorAirSystem.nameString());

      auto idf_oa_ = w_eg2.getTarget(BranchExtensibleFields::ComponentName);
      ASSERT_TRUE(idf_oa_);
      auto idf_oa_eq_list_ = idf_oa_->getTarget(AirLoopHVAC_OutdoorAirSystemFields::OutdoorAirEquipmentListName);
      ASSERT_TRUE(idf_oa_eq_list_);

      ASSERT_EQ(3u, idf_oa_eq_list_->extensibleGroups().size());

      {
        auto w_comp_eg = idf_oa_eq_list_->extensibleGroups()[0].cast<WorkspaceExtensibleGroup>();
        EXPECT_EQ("OutdoorAir:Mixer", w_comp_eg.getString(AirLoopHVAC_OutdoorAirSystem_EquipmentListExtensibleFields::ComponentObjectType).get());
      }

      {
        auto w_comp_eg = idf_oa_eq_list_->extensibleGroups()[1].cast<WorkspaceExtensibleGroup>();
        EXPECT_EQ("ZoneHVAC:TerminalUnit:VariableRefrigerantFlow",
                  w_comp_eg.getString(AirLoopHVAC_OutdoorAirSystem_EquipmentListExtensibleFields::ComponentObjectType).get());
        auto idf_vrf_ = w_comp_eg.getTarget(AirLoopHVAC_OutdoorAirSystem_EquipmentListExtensibleFields::ComponentName);
        ASSERT_TRUE(idf_vrf_);
        EXPECT_EQ(vrf_oa.nameString(), idf_vrf_->nameString());
        // No OutsideAirMixer, since it's on a AirLoopHVACOutdoorAirSystem
        EXPECT_FALSE(idf_vrf_->getTarget(ZoneHVAC_TerminalUnit_VariableRefrigerantFlowFields::OutsideAirMixerObjectName));

        // Note: should we *not* translate the fan? E+ doesn't complain if you do, but IDD is kinda clear that it should be empty
        // > Leave blank if terminal unit is used in AirLoopHVAC:OutdoorAirSystem:EquipmentList
        // Fan is NOT translated
        EXPECT_FALSE(idf_vrf_->getTarget(ZoneHVAC_TerminalUnit_VariableRefrigerantFlowFields::SupplyAirFanObjectName));
      }

      {
        auto w_comp_eg = idf_oa_eq_list_->extensibleGroups()[2].cast<WorkspaceExtensibleGroup>();
        EXPECT_EQ("ZoneHVAC:TerminalUnit:VariableRefrigerantFlow",
                  w_comp_eg.getString(AirLoopHVAC_OutdoorAirSystem_EquipmentListExtensibleFields::ComponentObjectType).get());
        auto idf_vrf_ = w_comp_eg.getTarget(AirLoopHVAC_OutdoorAirSystem_EquipmentListExtensibleFields::ComponentName);
        ASSERT_TRUE(idf_vrf_);
        EXPECT_EQ(vrf_relief_oa.nameString(), idf_vrf_->nameString());
        // No OutsideAirMixer, since it's on a AirLoopHVACOutdoorAirSystem
        EXPECT_FALSE(idf_vrf_->getTarget(ZoneHVAC_TerminalUnit_VariableRefrigerantFlowFields::OutsideAirMixerObjectName));

        // Fan is NOT translated
        EXPECT_FALSE(idf_vrf_->getTarget(ZoneHVAC_TerminalUnit_VariableRefrigerantFlowFields::SupplyAirFanObjectName));
      }
    }

    // 2: The VRF TU
    {
      auto w_eg2 = idf_branch.extensibleGroups()[1].cast<WorkspaceExtensibleGroup>();

      EXPECT_EQ("ZoneHVAC:TerminalUnit:VariableRefrigerantFlow", w_eg2.getString(BranchExtensibleFields::ComponentObjectType).get());
      EXPECT_EQ(w_eg2.getString(BranchExtensibleFields::ComponentName).get(), vrf.nameString());

      EXPECT_EQ(w_eg2.getString(BranchExtensibleFields::ComponentInletNodeName).get(), vrf.inletNode().get().nameString());

      EXPECT_EQ(w_eg2.getString(BranchExtensibleFields::ComponentOutletNodeName).get(), vrf.outletNode().get().nameString());

      auto idf_vrf_ = w_eg2.getTarget(BranchExtensibleFields::ComponentName);
      ASSERT_TRUE(idf_vrf_);
      EXPECT_EQ(vrf.nameString(), idf_vrf_->nameString());
      // No OutsideAirMixer, since it's on a AirLoopHVAC
      EXPECT_FALSE(idf_vrf_->getTarget(ZoneHVAC_TerminalUnit_VariableRefrigerantFlowFields::OutsideAirMixerObjectName));
      // Fan **is** translated
      EXPECT_TRUE(idf_vrf_->getTarget(ZoneHVAC_TerminalUnit_VariableRefrigerantFlowFields::SupplyAirFanObjectName));
    }

    // Let's see if we got the SPMs
    WorkspaceObjectVector spm_scheduleds(w.getObjectsByType(IddObjectType::SetpointManager_Scheduled));
    // There's the one I created for the supply outlet node
    ASSERT_EQ(1u, spm_scheduleds.size());
    {
      auto thisSPM = spm_scheduleds[0];
      EXPECT_EQ(spm.nameString(), thisSPM.nameString());
      EXPECT_EQ("Temperature", thisSPM.getString(SetpointManager_ScheduledFields::ControlVariable).get());
      EXPECT_EQ(sch.nameString(), thisSPM.getString(SetpointManager_ScheduledFields::ScheduleName).get());
      EXPECT_EQ("Supply Outlet Node", thisSPM.getString(SetpointManager_ScheduledFields::SetpointNodeorNodeListName).get());
    }

    WorkspaceObjectVector spm_mixedairs(w.getObjectsByType(IddObjectType::SetpointManager_MixedAir));
    EXPECT_EQ(2u, spm_mixedairs.size());
    for (const auto& spm_mixedair : spm_mixedairs) {
      EXPECT_EQ("Temperature", spm_mixedair.getString(SetpointManager_MixedAirFields::ControlVariable).get());
      EXPECT_EQ("Supply Outlet Node", spm_mixedair.getString(SetpointManager_MixedAirFields::ReferenceSetpointNodeName).get());
      EXPECT_EQ(vrf.nameString() + " Heating Coil Outlet Node", spm_mixedair.getString(SetpointManager_MixedAirFields::FanInletNodeName).get());
      EXPECT_EQ("Supply Outlet Node", spm_mixedair.getString(SetpointManager_MixedAirFields::FanOutletNodeName).get());
      EXPECT_TRUE(spm_mixedair.nameString().find("OS Default SPM") != std::string::npos);
      if (spm_mixedair.nameString().find("Mixed Air") != std::string::npos) {
        EXPECT_EQ("Mixed Air Node", spm_mixedair.getString(SetpointManager_MixedAirFields::SetpointNodeorNodeListName).get());
      } else {
        EXPECT_EQ("VRF TU on Outdoor Air Outlet Node", spm_mixedair.getString(SetpointManager_MixedAirFields::SetpointNodeorNodeListName).get());
      }
    }
  }

  // The main branch of the AirLoopHVAC *HAS a fan*
  {
    FanSystemModel loopFan(m);
    supplyOutletNode = a.supplyOutletNode();
    EXPECT_TRUE(loopFan.addToNode(supplyOutletNode));
    loopFan.inletModelObject()->setName("VRF TU Outlet to Fan Inlet Node");

    Workspace w = ft.translateModel(m);

    std::vector<WorkspaceObject> idf_vrfs = w.getObjectsByType(IddObjectType::ZoneHVAC_TerminalUnit_VariableRefrigerantFlow);
    std::vector<WorkspaceObject> idf_ccs = w.getObjectsByType(IddObjectType::Coil_Cooling_DX_VariableRefrigerantFlow);
    std::vector<WorkspaceObject> idf_hcs = w.getObjectsByType(IddObjectType::Coil_Heating_DX_VariableRefrigerantFlow);
    std::vector<WorkspaceObject> idf_fans = w.getObjectsByType(IddObjectType::Fan_OnOff);
    std::vector<WorkspaceObject> idf_fanSM = w.getObjectsByType(IddObjectType::Fan_SystemModel);
    std::vector<WorkspaceObject> idf_supHCs = w.getObjectsByType(IddObjectType::Coil_Heating_Water);

    EXPECT_EQ(3u, idf_vrfs.size());
    EXPECT_EQ(3u, idf_ccs.size());
    EXPECT_EQ(3u, idf_hcs.size());
    // None of the fans should be translated. The two on the OA System should never be
    // and the one on main branch in this case either since the AirLoopHVAC has a supply fan
    EXPECT_EQ(0u, idf_fans.size());
    EXPECT_EQ(1u, idf_fanSM.size());

    // Go from AirLoopHVAC to BranchList to Branch
    WorkspaceObjectVector idf_airloops(w.getObjectsByType(IddObjectType::AirLoopHVAC));
    ASSERT_EQ(1u, idf_airloops.size());
    WorkspaceObject idf_airLoopHVAC = idf_airloops[0];
    WorkspaceObject idf_brlist = idf_airLoopHVAC.getTarget(AirLoopHVACFields::BranchListName).get();

    // Should have one branch only
    ASSERT_EQ(1u, idf_brlist.extensibleGroups().size());
    auto w_eg = idf_brlist.extensibleGroups()[0].cast<WorkspaceExtensibleGroup>();
    WorkspaceObject idf_branch = w_eg.getTarget(BranchListExtensibleFields::BranchName).get();

    // There should be: OA Sys, VRF TU, fan
    ASSERT_EQ(3u, idf_branch.extensibleGroups().size());

    // 1: the AirLoopHVACOutdoorAirSystem
    ASSERT_EQ(1u, w.getObjectsByType(IddObjectType::AirLoopHVAC_OutdoorAirSystem).size());
    {
      auto w_eg2 = idf_branch.extensibleGroups()[0].cast<WorkspaceExtensibleGroup>();

      EXPECT_EQ("AirLoopHVAC:OutdoorAirSystem", w_eg2.getString(BranchExtensibleFields::ComponentObjectType).get());
      EXPECT_EQ(w_eg2.getString(BranchExtensibleFields::ComponentName).get(), outdoorAirSystem.nameString());

      auto idf_oa_ = w_eg2.getTarget(BranchExtensibleFields::ComponentName);
      ASSERT_TRUE(idf_oa_);
      auto idf_oa_eq_list_ = idf_oa_->getTarget(AirLoopHVAC_OutdoorAirSystemFields::OutdoorAirEquipmentListName);
      ASSERT_TRUE(idf_oa_eq_list_);

      ASSERT_EQ(3u, idf_oa_eq_list_->extensibleGroups().size());

      {
        auto w_comp_eg = idf_oa_eq_list_->extensibleGroups()[0].cast<WorkspaceExtensibleGroup>();
        EXPECT_EQ("OutdoorAir:Mixer", w_comp_eg.getString(AirLoopHVAC_OutdoorAirSystem_EquipmentListExtensibleFields::ComponentObjectType).get());
      }

      {
        auto w_comp_eg = idf_oa_eq_list_->extensibleGroups()[1].cast<WorkspaceExtensibleGroup>();
        EXPECT_EQ("ZoneHVAC:TerminalUnit:VariableRefrigerantFlow",
                  w_comp_eg.getString(AirLoopHVAC_OutdoorAirSystem_EquipmentListExtensibleFields::ComponentObjectType).get());
        auto idf_vrf_ = w_comp_eg.getTarget(AirLoopHVAC_OutdoorAirSystem_EquipmentListExtensibleFields::ComponentName);
        ASSERT_TRUE(idf_vrf_);
        EXPECT_EQ(vrf_oa.nameString(), idf_vrf_->nameString());
        // No OutsideAirMixer, since it's on a AirLoopHVACOutdoorAirSystem
        EXPECT_FALSE(idf_vrf_->getTarget(ZoneHVAC_TerminalUnit_VariableRefrigerantFlowFields::OutsideAirMixerObjectName));

        // Fan is not translated (AirLoopHVACOutdoorAirSystem)
        EXPECT_FALSE(idf_vrf_->getTarget(ZoneHVAC_TerminalUnit_VariableRefrigerantFlowFields::SupplyAirFanObjectName));
      }

      {
        auto w_comp_eg = idf_oa_eq_list_->extensibleGroups()[2].cast<WorkspaceExtensibleGroup>();
        EXPECT_EQ("ZoneHVAC:TerminalUnit:VariableRefrigerantFlow",
                  w_comp_eg.getString(AirLoopHVAC_OutdoorAirSystem_EquipmentListExtensibleFields::ComponentObjectType).get());
        auto idf_vrf_ = w_comp_eg.getTarget(AirLoopHVAC_OutdoorAirSystem_EquipmentListExtensibleFields::ComponentName);
        ASSERT_TRUE(idf_vrf_);
        EXPECT_EQ(vrf_relief_oa.nameString(), idf_vrf_->nameString());
        // No OutsideAirMixer, since it's on a AirLoopHVACOutdoorAirSystem
        EXPECT_FALSE(idf_vrf_->getTarget(ZoneHVAC_TerminalUnit_VariableRefrigerantFlowFields::OutsideAirMixerObjectName));

        // Fan is not translated (AirLoopHVACOutdoorAirSystem)
        EXPECT_FALSE(idf_vrf_->getTarget(ZoneHVAC_TerminalUnit_VariableRefrigerantFlowFields::SupplyAirFanObjectName));
      }
    }

    // 2: The VRF TU
    {
      auto w_eg2 = idf_branch.extensibleGroups()[1].cast<WorkspaceExtensibleGroup>();

      EXPECT_EQ("ZoneHVAC:TerminalUnit:VariableRefrigerantFlow", w_eg2.getString(BranchExtensibleFields::ComponentObjectType).get());
      EXPECT_EQ(w_eg2.getString(BranchExtensibleFields::ComponentName).get(), vrf.nameString());

      EXPECT_EQ(w_eg2.getString(BranchExtensibleFields::ComponentInletNodeName).get(), vrf.inletNode().get().nameString());

      EXPECT_EQ(w_eg2.getString(BranchExtensibleFields::ComponentOutletNodeName).get(), vrf.outletNode().get().nameString());

      auto idf_vrf_ = w_eg2.getTarget(BranchExtensibleFields::ComponentName);
      ASSERT_TRUE(idf_vrf_);
      EXPECT_EQ(vrf.nameString(), idf_vrf_->nameString());
      // No OutsideAirMixer, since it's on a AirLoopHVAC
      EXPECT_FALSE(idf_vrf_->getTarget(ZoneHVAC_TerminalUnit_VariableRefrigerantFlowFields::OutsideAirMixerObjectName));
      // Fan is NOT translated (there is one on the main AirLoopHVAC branch)
      EXPECT_FALSE(idf_vrf_->getTarget(ZoneHVAC_TerminalUnit_VariableRefrigerantFlowFields::SupplyAirFanObjectName));
    }

    // Let's see if we got the SPMs
    WorkspaceObjectVector spm_scheduleds(w.getObjectsByType(IddObjectType::SetpointManager_Scheduled));
    // There's the one I created for the supply outlet node
    ASSERT_EQ(1u, spm_scheduleds.size());
    {
      auto thisSPM = spm_scheduleds[0];
      EXPECT_EQ(spm.nameString(), thisSPM.nameString());
      EXPECT_EQ("Temperature", thisSPM.getString(SetpointManager_ScheduledFields::ControlVariable).get());
      EXPECT_EQ(sch.nameString(), thisSPM.getString(SetpointManager_ScheduledFields::ScheduleName).get());
      EXPECT_EQ("Supply Outlet Node", thisSPM.getString(SetpointManager_ScheduledFields::SetpointNodeorNodeListName).get());
    }

    WorkspaceObjectVector spm_mixedairs(w.getObjectsByType(IddObjectType::SetpointManager_MixedAir));
    // Now we have three: one for the VRF TU outlet node that's on the OA System, one for the VRF TU outlet node on main branch
    // One for the FanSystemModel Outlet Node
    EXPECT_EQ(3u, spm_mixedairs.size());
    for (const auto& spm_mixedair : spm_mixedairs) {
      EXPECT_EQ("Temperature", spm_mixedair.getString(SetpointManager_MixedAirFields::ControlVariable).get());
      EXPECT_EQ("Supply Outlet Node", spm_mixedair.getString(SetpointManager_MixedAirFields::ReferenceSetpointNodeName).get());
      EXPECT_EQ("VRF TU Outlet to Fan Inlet Node", spm_mixedair.getString(SetpointManager_MixedAirFields::FanInletNodeName).get());
      EXPECT_EQ("Supply Outlet Node", spm_mixedair.getString(SetpointManager_MixedAirFields::FanOutletNodeName).get());
      EXPECT_TRUE(spm_mixedair.nameString().find("OS Default SPM") != std::string::npos);
      if (spm_mixedair.nameString().find("Mixed Air") != std::string::npos) {
        EXPECT_EQ("Mixed Air Node", spm_mixedair.getString(SetpointManager_MixedAirFields::SetpointNodeorNodeListName).get());
      } else if (spm_mixedair.nameString().find("VRF TU on Outdoor Air") != std::string::npos) {
        EXPECT_EQ("VRF TU on Outdoor Air Outlet Node", spm_mixedair.getString(SetpointManager_MixedAirFields::SetpointNodeorNodeListName).get());
      } else {
        EXPECT_EQ("VRF TU Outlet to Fan Inlet Node", spm_mixedair.getString(SetpointManager_MixedAirFields::SetpointNodeorNodeListName).get());
      }
    }
  }
}

TEST_F(EnergyPlusFixture, ForwardTranslatorZoneHVACTerminalUnitVariableRefrigerantFlow_AirLoopHVAC_DemandSide) {

  Model m;

  AirLoopHVAC a(m);
  Node supplyOutletNode = a.supplyOutletNode();

  FanSystemModel loopFan(m);
  supplyOutletNode = a.supplyOutletNode();
  EXPECT_TRUE(loopFan.addToNode(supplyOutletNode));

  ThermalZone z(m);
  Space s(m);
  s.setThermalZone(z);
  AirTerminalSingleDuctInletSideMixer atu(m);
  EXPECT_TRUE(a.addBranchForZone(z, atu));

  FanOnOff vrfFan(m);
  CoilCoolingDXVariableRefrigerantFlow cc(m);
  CoilHeatingDXVariableRefrigerantFlow hc(m);
  ZoneHVACTerminalUnitVariableRefrigerantFlow vrf(m, cc, hc, vrfFan);
  Node atuOutletNode = atu.outletModelObject()->cast<model::Node>();
  EXPECT_TRUE(vrf.addToNode(atuOutletNode));

  ForwardTranslator ft;
  Workspace w = ft.translateModel(m);

  // When the VRF is on the demand side,
  // then its fan should be translated into the resulting idf
  // As previous tests show, if the VRF is a supply component then the fan
  // should not be translated
  std::vector<WorkspaceObject> idf_onoff_fans = w.getObjectsByType(IddObjectType::Fan_OnOff);
  EXPECT_EQ(1u, idf_onoff_fans.size());

  std::vector<WorkspaceObject> idf_system_fans = w.getObjectsByType(IddObjectType::Fan_SystemModel);
  EXPECT_EQ(1u, idf_system_fans.size());
}
