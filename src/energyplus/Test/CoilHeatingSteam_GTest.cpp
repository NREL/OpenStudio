/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>
#include "EnergyPlusFixture.hpp"

#include "../ForwardTranslator.hpp"

#include "../../model/Model.hpp"
#include "../../model/CoilHeatingSteam.hpp"
#include "../../model/CoilHeatingDXSingleSpeed.hpp"
#include "../../model/CoilCoolingDXSingleSpeed.hpp"
#include "../../model/CoilCoolingDXMultiSpeed.hpp"
#include "../../model/CoilHeatingGasMultiStage.hpp"
#include "../../model/CoilHeatingWaterToAirHeatPumpEquationFit.hpp"
#include "../../model/CoilCoolingWaterToAirHeatPumpEquationFit.hpp"
#include "../../model/CoilCoolingDXVariableRefrigerantFlow.hpp"
#include "../../model/CoilHeatingDXVariableRefrigerantFlow.hpp"
#include "../../model/Schedule.hpp"
#include "../../model/ThermalZone.hpp"
#include "../../model/Space.hpp"
#include "../../model/AirLoopHVAC.hpp"
#include "../../model/Node.hpp"
#include "../../model/PlantLoop.hpp"
#include "../../model/FanConstantVolume.hpp"
#include "../../model/FanSystemModel.hpp"
#include "../../model/FanOnOff.hpp"
#include "../../model/AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed.hpp"
#include "../../model/AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass.hpp"
#include "../../model/AirLoopHVACUnitarySystem.hpp"
#include "../../model/AirTerminalSingleDuctVAVReheat.hpp"
#include "../../model/AirTerminalSingleDuctVAVHeatAndCoolReheat.hpp"
#include "../../model/AirTerminalSingleDuctConstantVolumeReheat.hpp"
#include "../../model/AirTerminalSingleDuctSeriesPIUReheat.hpp"
#include "../../model/AirTerminalSingleDuctParallelPIUReheat.hpp"
#include "../../model/AirTerminalSingleDuctConstantVolumeFourPipeInduction.hpp"
#include "../../model/ZoneHVACPackagedTerminalAirConditioner.hpp"
#include "../../model/ZoneHVACPackagedTerminalHeatPump.hpp"
#include "../../model/ZoneHVACTerminalUnitVariableRefrigerantFlow.hpp"
#include "../../model/ZoneHVACWaterToAirHeatPump.hpp"
#include "../../model/ZoneHVACUnitHeater.hpp"
#include "../../model/ZoneHVACUnitVentilator.hpp"

#include <utilities/idd/Coil_Heating_Steam_FieldEnums.hxx>
#include <utilities/idd/AirLoopHVAC_UnitaryHeatPump_AirToAir_MultiSpeed_FieldEnums.hxx>
#include <utilities/idd/AirLoopHVAC_UnitaryHeatCool_VAVChangeoverBypass_FieldEnums.hxx>
#include <utilities/idd/AirLoopHVAC_UnitarySystem_FieldEnums.hxx>
#include <utilities/idd/AirTerminal_SingleDuct_VAV_Reheat_FieldEnums.hxx>
#include <utilities/idd/AirTerminal_SingleDuct_VAV_HeatAndCool_Reheat_FieldEnums.hxx>
#include <utilities/idd/AirTerminal_SingleDuct_ConstantVolume_Reheat_FieldEnums.hxx>
#include <utilities/idd/AirTerminal_SingleDuct_SeriesPIU_Reheat_FieldEnums.hxx>
#include <utilities/idd/AirTerminal_SingleDuct_ParallelPIU_Reheat_FieldEnums.hxx>
#include <utilities/idd/AirTerminal_SingleDuct_ConstantVolume_FourPipeInduction_FieldEnums.hxx>
#include <utilities/idd/ZoneHVAC_PackagedTerminalAirConditioner_FieldEnums.hxx>
#include <utilities/idd/ZoneHVAC_PackagedTerminalHeatPump_FieldEnums.hxx>
#include <utilities/idd/ZoneHVAC_TerminalUnit_VariableRefrigerantFlow_FieldEnums.hxx>
#include <utilities/idd/ZoneHVAC_WaterToAirHeatPump_FieldEnums.hxx>
#include <utilities/idd/ZoneHVAC_UnitHeater_FieldEnums.hxx>
#include <utilities/idd/ZoneHVAC_UnitVentilator_FieldEnums.hxx>

#include <utilities/idd/IddEnums.hxx>
#include "../../utilities/idf/IdfObject.hpp"
#include "../../utilities/idf/IdfObject_Impl.hpp"

#include "../../utilities/idf/WorkspaceObject.hpp"
#include "../../utilities/idf/WorkspaceObject_Impl.hpp"

using namespace openstudio::energyplus;
using namespace openstudio::model;
using namespace openstudio;

TEST_F(EnergyPlusFixture, ForwardTranslator_CoilHeatingSteam) {
  Model m;

  ThermalZone z(m);
  Space s(m);
  s.setThermalZone(z);
  AirLoopHVAC a(m);
  PlantLoop p(m);

  CoilHeatingSteam coil = CoilHeatingSteam(m);

  Schedule availabilitySchedule = m.alwaysOnDiscreteSchedule();
  EXPECT_TRUE(coil.setAvailabilitySchedule(availabilitySchedule));
  EXPECT_TRUE(coil.setMaximumSteamFlowRate(1.1));
  EXPECT_TRUE(coil.setDegreeofSubCooling(2.5));
  EXPECT_TRUE(coil.setDegreeofLoopSubCooling(15.0));
  EXPECT_TRUE(coil.setCoilControlType("TemperatureSetpointControl"));
  Node outletNode = a.supplyOutletNode();
  EXPECT_TRUE(coil.setTemperatureSetpointNode(outletNode));

  Schedule sch = m.alwaysOnDiscreteSchedule();
  AirTerminalSingleDuctVAVReheat atu(m, sch, coil);

  p.addDemandBranchForComponent(coil);
  a.addBranchForZone(z, atu);

  ForwardTranslator ft;
  Workspace w = ft.translateModel(m);

  ASSERT_EQ(1u, w.getObjectsByType(IddObjectType::Coil_Heating_Steam).size());
  IdfObject idf_coil = w.getObjectsByType(IddObjectType::Coil_Heating_Steam)[0];

  EXPECT_EQ(coil.nameString(), idf_coil.getString(Coil_Heating_SteamFields::Name, false).get());
  EXPECT_EQ(sch.nameString(), idf_coil.getString(Coil_Heating_SteamFields::AvailabilityScheduleName, false).get());
  EXPECT_EQ(1.1, idf_coil.getDouble(Coil_Heating_SteamFields::MaximumSteamFlowRate, false).get());
  EXPECT_EQ(2.5, idf_coil.getDouble(Coil_Heating_SteamFields::DegreeofSubCooling, false).get());
  EXPECT_EQ(15.0, idf_coil.getDouble(Coil_Heating_SteamFields::DegreeofLoopSubCooling, false).get());
  EXPECT_EQ(coil.waterInletModelObject()->nameString(), idf_coil.getString(Coil_Heating_SteamFields::WaterInletNodeName, false).get());
  EXPECT_EQ(coil.waterOutletModelObject()->nameString(), idf_coil.getString(Coil_Heating_SteamFields::WaterOutletNodeName, false).get());
  EXPECT_EQ(atu.nameString() + " Damper Outlet", idf_coil.getString(Coil_Heating_SteamFields::AirInletNodeName, false).get());
  EXPECT_EQ(atu.outletModelObject()->nameString(), idf_coil.getString(Coil_Heating_SteamFields::AirOutletNodeName, false).get());
  EXPECT_EQ("TemperatureSetpointControl", idf_coil.getString(Coil_Heating_SteamFields::CoilControlType, false).get());
  EXPECT_EQ(outletNode.nameString(), idf_coil.getString(Coil_Heating_SteamFields::TemperatureSetpointNodeName, false).get());
}

TEST_F(EnergyPlusFixture, ForwardTranslator_CoilHeatingSteam_Equipment) {
  // AirLoopHVAC:UnitaryHeatPump:AirToAir:MultiSpeed
  {
    Model m;

    FanConstantVolume f(m);
    CoilHeatingGasMultiStage h(m);
    CoilHeatingSteam s = CoilHeatingSteam(m);
    CoilCoolingDXMultiSpeed c(m);

    AirLoopHVAC a(m);
    AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed u(m, f, h, c, s);

    Node supplyOutletNode = a.supplyOutletNode();
    u.addToNode(supplyOutletNode);

    ForwardTranslator ft;
    Workspace w = ft.translateModel(m);

    EXPECT_EQ(1u, w.getObjectsByType(IddObjectType::Coil_Heating_Steam).size());
    WorkspaceObjectVector idf_us(w.getObjectsByType(IddObjectType::AirLoopHVAC_UnitaryHeatPump_AirToAir_MultiSpeed));
    ASSERT_EQ(1u, idf_us.size());
    WorkspaceObject idf_u(idf_us[0]);

    EXPECT_EQ("Coil:Heating:Steam", idf_u.getString(AirLoopHVAC_UnitaryHeatPump_AirToAir_MultiSpeedFields::SupplementalHeatingCoilObjectType).get());
    boost::optional<WorkspaceObject> woSuppHeatCoil(
      idf_u.getTarget(AirLoopHVAC_UnitaryHeatPump_AirToAir_MultiSpeedFields::SupplementalHeatingCoilName));
    EXPECT_TRUE(woSuppHeatCoil);
    EXPECT_EQ(woSuppHeatCoil->iddObject().type(), IddObjectType::Coil_Heating_Steam);
    EXPECT_EQ("Coil Heating Steam 1", woSuppHeatCoil->nameString());
    EXPECT_EQ(u.nameString() + " Fan Outlet", woSuppHeatCoil->getString(Coil_Heating_SteamFields::AirInletNodeName).get());
    EXPECT_EQ(u.outletModelObject()->nameString(), woSuppHeatCoil->getString(Coil_Heating_SteamFields::AirOutletNodeName).get());
  }

  // AirLoopHVAC:UnitaryHeatCool:VAVChangeoverBypass
  {
    Model m;

    FanConstantVolume f(m);
    CoilHeatingSteam h = CoilHeatingSteam(m);
    CoilCoolingDXSingleSpeed c(m);

    AirLoopHVAC a(m);
    AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass u(m, f, c, h);

    Node supplyOutletNode = a.supplyOutletNode();
    u.addToNode(supplyOutletNode);

    ForwardTranslator ft;
    Workspace w = ft.translateModel(m);

    EXPECT_EQ(1u, w.getObjectsByType(IddObjectType::Coil_Heating_Steam).size());
    WorkspaceObjectVector idf_us(w.getObjectsByType(IddObjectType::AirLoopHVAC_UnitaryHeatCool_VAVChangeoverBypass));
    ASSERT_EQ(1u, idf_us.size());
    WorkspaceObject idf_u(idf_us[0]);

    EXPECT_EQ("Coil:Heating:Steam", idf_u.getString(AirLoopHVAC_UnitaryHeatCool_VAVChangeoverBypassFields::HeatingCoilObjectType).get());
    boost::optional<WorkspaceObject> woHeatCoil(idf_u.getTarget(AirLoopHVAC_UnitaryHeatCool_VAVChangeoverBypassFields::HeatingCoilName));
    EXPECT_TRUE(woHeatCoil);
    EXPECT_EQ(woHeatCoil->iddObject().type(), IddObjectType::Coil_Heating_Steam);
    EXPECT_EQ("Coil Heating Steam 1", woHeatCoil->nameString());
    EXPECT_EQ(u.nameString() + " Cooling Coil Outlet Node", woHeatCoil->getString(Coil_Heating_SteamFields::AirInletNodeName).get());
    EXPECT_EQ(u.nameString() + " Heating Coil Outlet Node", woHeatCoil->getString(Coil_Heating_SteamFields::AirOutletNodeName).get());
  }

  // AirLoopHVAC:UnitarySystem
  {
    Model m;

    CoilHeatingSteam h = CoilHeatingSteam(m);
    CoilHeatingSteam s = CoilHeatingSteam(m);

    AirLoopHVAC a(m);
    AirLoopHVACUnitarySystem u(m);

    u.setHeatingCoil(h);
    u.setSupplementalHeatingCoil(s);

    Node supplyOutletNode = a.supplyOutletNode();
    u.addToNode(supplyOutletNode);

    ForwardTranslator ft;
    Workspace w = ft.translateModel(m);

    EXPECT_EQ(2u, w.getObjectsByType(IddObjectType::Coil_Heating_Steam).size());
    WorkspaceObjectVector idf_us(w.getObjectsByType(IddObjectType::AirLoopHVAC_UnitarySystem));
    ASSERT_EQ(1u, idf_us.size());
    WorkspaceObject idf_u(idf_us[0]);

    EXPECT_EQ("Coil:Heating:Steam", idf_u.getString(AirLoopHVAC_UnitarySystemFields::HeatingCoilObjectType).get());
    boost::optional<WorkspaceObject> woHeatCoil(idf_u.getTarget(AirLoopHVAC_UnitarySystemFields::HeatingCoilName));
    EXPECT_TRUE(woHeatCoil);
    EXPECT_EQ(woHeatCoil->iddObject().type(), IddObjectType::Coil_Heating_Steam);
    EXPECT_EQ("Coil Heating Steam 1", woHeatCoil->nameString());
    EXPECT_EQ(u.inletNode()->nameString(), woHeatCoil->getString(Coil_Heating_SteamFields::AirInletNodeName).get());
    EXPECT_EQ(u.nameString() + " Heating Coil - Supplemental Coil Node", woHeatCoil->getString(Coil_Heating_SteamFields::AirOutletNodeName).get());

    EXPECT_EQ("Coil:Heating:Steam", idf_u.getString(AirLoopHVAC_UnitarySystemFields::SupplementalHeatingCoilObjectType).get());
    boost::optional<WorkspaceObject> woSuppHeatCoil(idf_u.getTarget(AirLoopHVAC_UnitarySystemFields::SupplementalHeatingCoilName));
    EXPECT_TRUE(woSuppHeatCoil);
    EXPECT_EQ(woSuppHeatCoil->iddObject().type(), IddObjectType::Coil_Heating_Steam);
    EXPECT_EQ("Coil Heating Steam 2", woSuppHeatCoil->nameString());
    EXPECT_EQ(u.nameString() + " Heating Coil - Supplemental Coil Node", woSuppHeatCoil->getString(Coil_Heating_SteamFields::AirInletNodeName).get());
    EXPECT_EQ(u.outletNode()->nameString(), woSuppHeatCoil->getString(Coil_Heating_SteamFields::AirOutletNodeName).get());
  }

  // AirTerminalSingleDuctVAVReheat
  {
    Model m;

    ThermalZone z(m);
    Space s(m);
    s.setThermalZone(z);

    Schedule sch = m.alwaysOnDiscreteSchedule();
    CoilHeatingSteam coil = CoilHeatingSteam(m, sch);
    AirTerminalSingleDuctVAVReheat atu(m, sch, coil);

    AirLoopHVAC a(m);
    a.addBranchForZone(z, atu);

    ForwardTranslator ft;
    Workspace w = ft.translateModel(m);

    EXPECT_EQ(1u, w.getObjectsByType(IddObjectType::Coil_Heating_Steam).size());
    WorkspaceObjectVector idf_atus(w.getObjectsByType(IddObjectType::AirTerminal_SingleDuct_VAV_Reheat));
    ASSERT_EQ(1u, idf_atus.size());
    WorkspaceObject idf_atu(idf_atus[0]);

    EXPECT_EQ("Coil:Heating:Steam", idf_atu.getString(AirTerminal_SingleDuct_VAV_ReheatFields::ReheatCoilObjectType).get());
    boost::optional<WorkspaceObject> woReheatCoil(idf_atu.getTarget(AirTerminal_SingleDuct_VAV_ReheatFields::ReheatCoilName));
    EXPECT_TRUE(woReheatCoil);
    EXPECT_EQ(woReheatCoil->iddObject().type(), IddObjectType::Coil_Heating_Steam);
    EXPECT_EQ("Coil Heating Steam 1", woReheatCoil->nameString());
    EXPECT_EQ(atu.nameString() + " Damper Outlet", woReheatCoil->getString(Coil_Heating_SteamFields::AirInletNodeName).get());
    EXPECT_EQ(atu.outletModelObject()->nameString(), woReheatCoil->getString(Coil_Heating_SteamFields::AirOutletNodeName).get());
  }

  // AirTerminal:SingleDuct:VAV:HeatAndCool:Reheat
  {
    Model m;

    ThermalZone z(m);
    Space s(m);
    s.setThermalZone(z);

    Schedule sch = m.alwaysOnDiscreteSchedule();
    CoilHeatingSteam coil = CoilHeatingSteam(m, sch);
    AirTerminalSingleDuctVAVHeatAndCoolReheat atu(m, coil);

    AirLoopHVAC a(m);
    a.addBranchForZone(z, atu);

    ForwardTranslator ft;
    Workspace w = ft.translateModel(m);

    EXPECT_EQ(1u, w.getObjectsByType(IddObjectType::Coil_Heating_Steam).size());
    WorkspaceObjectVector idf_atus(w.getObjectsByType(IddObjectType::AirTerminal_SingleDuct_VAV_HeatAndCool_Reheat));
    ASSERT_EQ(1u, idf_atus.size());
    WorkspaceObject idf_atu(idf_atus[0]);

    EXPECT_EQ("Coil:Heating:Steam", idf_atu.getString(AirTerminal_SingleDuct_VAV_HeatAndCool_ReheatFields::ReheatCoilObjectType).get());
    boost::optional<WorkspaceObject> woReheatCoil(idf_atu.getTarget(AirTerminal_SingleDuct_VAV_HeatAndCool_ReheatFields::ReheatCoilName));
    EXPECT_TRUE(woReheatCoil);
    EXPECT_EQ(woReheatCoil->iddObject().type(), IddObjectType::Coil_Heating_Steam);
    EXPECT_EQ("Coil Heating Steam 1", woReheatCoil->nameString());
    EXPECT_EQ(atu.nameString() + " Damper Outlet Node", woReheatCoil->getString(Coil_Heating_SteamFields::AirInletNodeName).get());
    EXPECT_EQ(atu.outletModelObject()->nameString(), woReheatCoil->getString(Coil_Heating_SteamFields::AirOutletNodeName).get());
  }

  // AirTerminal:SingleDuct:ConstantVolume:Reheat
  {
    Model m;

    ThermalZone z(m);
    Space s(m);
    s.setThermalZone(z);

    Schedule sch = m.alwaysOnDiscreteSchedule();
    CoilHeatingSteam coil = CoilHeatingSteam(m, sch);
    AirTerminalSingleDuctConstantVolumeReheat atu(m, sch, coil);

    AirLoopHVAC a(m);
    a.addBranchForZone(z, atu);

    ForwardTranslator ft;
    Workspace w = ft.translateModel(m);

    EXPECT_EQ(1u, w.getObjectsByType(IddObjectType::Coil_Heating_Steam).size());
    WorkspaceObjectVector idf_atus(w.getObjectsByType(IddObjectType::AirTerminal_SingleDuct_ConstantVolume_Reheat));
    ASSERT_EQ(1u, idf_atus.size());
    WorkspaceObject idf_atu(idf_atus[0]);

    EXPECT_EQ("Coil:Heating:Steam", idf_atu.getString(AirTerminal_SingleDuct_ConstantVolume_ReheatFields::ReheatCoilObjectType).get());
    boost::optional<WorkspaceObject> woReheatCoil(idf_atu.getTarget(AirTerminal_SingleDuct_ConstantVolume_ReheatFields::ReheatCoilName));
    EXPECT_TRUE(woReheatCoil);
    EXPECT_EQ(woReheatCoil->iddObject().type(), IddObjectType::Coil_Heating_Steam);
    EXPECT_EQ("Coil Heating Steam 1", woReheatCoil->nameString());
    EXPECT_EQ(atu.inletModelObject()->nameString(), woReheatCoil->getString(Coil_Heating_SteamFields::AirInletNodeName).get());
    EXPECT_EQ(atu.outletModelObject()->nameString(), woReheatCoil->getString(Coil_Heating_SteamFields::AirOutletNodeName).get());
  }

  // AirTerminal:SingleDuct:SeriesPIU:Reheat
  {
    Model m;

    ThermalZone z(m);
    Space s(m);
    s.setThermalZone(z);

    Schedule sch = m.alwaysOnDiscreteSchedule();
    FanSystemModel fan(m);
    CoilHeatingSteam coil = CoilHeatingSteam(m, sch);
    AirTerminalSingleDuctSeriesPIUReheat atu(m, fan, coil);

    AirLoopHVAC a(m);
    a.addBranchForZone(z, atu);

    ForwardTranslator ft;
    Workspace w = ft.translateModel(m);

    EXPECT_EQ(1u, w.getObjectsByType(IddObjectType::Coil_Heating_Steam).size());
    WorkspaceObjectVector idf_atus(w.getObjectsByType(IddObjectType::AirTerminal_SingleDuct_SeriesPIU_Reheat));
    ASSERT_EQ(1u, idf_atus.size());
    WorkspaceObject idf_atu(idf_atus[0]);

    EXPECT_EQ("Coil:Heating:Steam", idf_atu.getString(AirTerminal_SingleDuct_SeriesPIU_ReheatFields::ReheatCoilObjectType).get());
    boost::optional<WorkspaceObject> woReheatCoil(idf_atu.getTarget(AirTerminal_SingleDuct_SeriesPIU_ReheatFields::ReheatCoilName));
    EXPECT_TRUE(woReheatCoil);
    EXPECT_EQ(woReheatCoil->iddObject().type(), IddObjectType::Coil_Heating_Steam);
    EXPECT_EQ("Coil Heating Steam 1", woReheatCoil->nameString());
    EXPECT_EQ(atu.nameString() + " Fan Outlet", woReheatCoil->getString(Coil_Heating_SteamFields::AirInletNodeName).get());
    EXPECT_EQ(atu.outletModelObject()->nameString(), woReheatCoil->getString(Coil_Heating_SteamFields::AirOutletNodeName).get());
  }

  // AirTerminal:SingleDuct:ParallelPIU:Reheat
  {
    Model m;

    ThermalZone z(m);
    Space s(m);
    s.setThermalZone(z);

    Schedule sch = m.alwaysOnDiscreteSchedule();
    FanSystemModel fan(m);
    CoilHeatingSteam coil = CoilHeatingSteam(m, sch);
    AirTerminalSingleDuctParallelPIUReheat atu(m, sch, fan, coil);

    AirLoopHVAC a(m);
    a.addBranchForZone(z, atu);

    ForwardTranslator ft;
    Workspace w = ft.translateModel(m);

    EXPECT_EQ(1u, w.getObjectsByType(IddObjectType::Coil_Heating_Steam).size());
    WorkspaceObjectVector idf_atus(w.getObjectsByType(IddObjectType::AirTerminal_SingleDuct_ParallelPIU_Reheat));
    ASSERT_EQ(1u, idf_atus.size());
    WorkspaceObject idf_atu(idf_atus[0]);

    EXPECT_EQ("Coil:Heating:Steam", idf_atu.getString(AirTerminal_SingleDuct_ParallelPIU_ReheatFields::ReheatCoilObjectType).get());
    boost::optional<WorkspaceObject> woReheatCoil(idf_atu.getTarget(AirTerminal_SingleDuct_ParallelPIU_ReheatFields::ReheatCoilName));
    EXPECT_TRUE(woReheatCoil);
    EXPECT_EQ(woReheatCoil->iddObject().type(), IddObjectType::Coil_Heating_Steam);
    EXPECT_EQ("Coil Heating Steam 1", woReheatCoil->nameString());
    EXPECT_EQ(atu.nameString() + " Mixer Outlet", woReheatCoil->getString(Coil_Heating_SteamFields::AirInletNodeName).get());
    EXPECT_EQ(atu.outletModelObject()->nameString(), woReheatCoil->getString(Coil_Heating_SteamFields::AirOutletNodeName).get());
  }

  // AirTerminal:SingleDuct:ConstantVolume:FourPipeInduction
  {
    Model m;

    ThermalZone z(m);
    Space s(m);
    s.setThermalZone(z);

    Schedule sch = m.alwaysOnDiscreteSchedule();
    CoilHeatingSteam coil = CoilHeatingSteam(m, sch);
    AirTerminalSingleDuctConstantVolumeFourPipeInduction atu(m, coil);

    AirLoopHVAC a(m);
    a.addBranchForZone(z, atu);

    ForwardTranslator ft;
    Workspace w = ft.translateModel(m);

    EXPECT_EQ(1u, w.getObjectsByType(IddObjectType::Coil_Heating_Steam).size());
    WorkspaceObjectVector idf_atus(w.getObjectsByType(IddObjectType::AirTerminal_SingleDuct_ConstantVolume_FourPipeInduction));
    ASSERT_EQ(1u, idf_atus.size());
    WorkspaceObject idf_atu(idf_atus[0]);

    EXPECT_EQ("Coil:Heating:Steam", idf_atu.getString(AirTerminal_SingleDuct_ConstantVolume_FourPipeInductionFields::HeatingCoilObjectType).get());
    boost::optional<WorkspaceObject> woHeatCoil(idf_atu.getTarget(AirTerminal_SingleDuct_ConstantVolume_FourPipeInductionFields::HeatingCoilName));
    EXPECT_TRUE(woHeatCoil);
    EXPECT_EQ(woHeatCoil->iddObject().type(), IddObjectType::Coil_Heating_Steam);
    EXPECT_EQ("Coil Heating Steam 1", woHeatCoil->nameString());
    EXPECT_EQ(atu.inducedAirInletNode()->nameString(), woHeatCoil->getString(Coil_Heating_SteamFields::AirInletNodeName).get());
    EXPECT_EQ(atu.nameString() + " Heating Coil Outlet", woHeatCoil->getString(Coil_Heating_SteamFields::AirOutletNodeName).get());
  }

  // ZoneHVAC:PackagedTerminalHeatPump
  {
    Model m;

    ThermalZone z(m);
    Space s(m);
    s.setThermalZone(z);

    Schedule sch = m.alwaysOnDiscreteSchedule();
    FanConstantVolume fan(m);
    CoilHeatingSteam sh = CoilHeatingSteam(m, sch);
    CoilHeatingDXSingleSpeed h(m);
    CoilCoolingDXSingleSpeed c(m);
    ZoneHVACPackagedTerminalHeatPump zh(m, sch, fan, h, c, sh);

    zh.addToThermalZone(z);

    ForwardTranslator ft;
    Workspace w = ft.translateModel(m);

    EXPECT_EQ(1u, w.getObjectsByType(IddObjectType::Coil_Heating_Steam).size());
    WorkspaceObjectVector idf_zhs(w.getObjectsByType(IddObjectType::ZoneHVAC_PackagedTerminalHeatPump));
    ASSERT_EQ(1u, idf_zhs.size());
    WorkspaceObject idf_zh(idf_zhs[0]);

    EXPECT_EQ("Coil:Heating:Steam", idf_zh.getString(ZoneHVAC_PackagedTerminalHeatPumpFields::SupplementalHeatingCoilObjectType).get());
    boost::optional<WorkspaceObject> woSuppHeatCoil(idf_zh.getTarget(ZoneHVAC_PackagedTerminalHeatPumpFields::SupplementalHeatingCoilName));
    EXPECT_TRUE(woSuppHeatCoil);
    EXPECT_EQ(woSuppHeatCoil->iddObject().type(), IddObjectType::Coil_Heating_Steam);
    EXPECT_EQ("Coil Heating Steam 1", woSuppHeatCoil->nameString());
    EXPECT_EQ(zh.nameString() + " Fan Outlet Node", woSuppHeatCoil->getString(Coil_Heating_SteamFields::AirInletNodeName).get());
    EXPECT_EQ(zh.outletNode()->nameString(), woSuppHeatCoil->getString(Coil_Heating_SteamFields::AirOutletNodeName).get());
  }

  // ZoneHVAC:PackagedTerminalAirConditioner
  {
    Model m;

    ThermalZone z(m);
    Space s(m);
    s.setThermalZone(z);

    Schedule sch = m.alwaysOnDiscreteSchedule();
    FanConstantVolume fan(m);
    CoilHeatingSteam h = CoilHeatingSteam(m, sch);
    CoilCoolingDXSingleSpeed c(m);
    ZoneHVACPackagedTerminalAirConditioner zh(m, sch, fan, h, c);

    zh.addToThermalZone(z);

    ForwardTranslator ft;
    Workspace w = ft.translateModel(m);

    EXPECT_EQ(1u, w.getObjectsByType(IddObjectType::Coil_Heating_Steam).size());
    WorkspaceObjectVector idf_zhs(w.getObjectsByType(IddObjectType::ZoneHVAC_PackagedTerminalAirConditioner));
    ASSERT_EQ(1u, idf_zhs.size());
    WorkspaceObject idf_zh(idf_zhs[0]);

    EXPECT_EQ("Coil:Heating:Steam", idf_zh.getString(ZoneHVAC_PackagedTerminalAirConditionerFields::HeatingCoilObjectType).get());
    boost::optional<WorkspaceObject> woHeatCoil(idf_zh.getTarget(ZoneHVAC_PackagedTerminalAirConditionerFields::HeatingCoilName));
    EXPECT_TRUE(woHeatCoil);
    EXPECT_EQ(woHeatCoil->iddObject().type(), IddObjectType::Coil_Heating_Steam);
    EXPECT_EQ("Coil Heating Steam 1", woHeatCoil->nameString());
    EXPECT_EQ(zh.nameString() + " Cooling Coil Outlet Node", woHeatCoil->getString(Coil_Heating_SteamFields::AirInletNodeName).get());
    EXPECT_EQ(zh.nameString() + " Heating Coil Outlet Node", woHeatCoil->getString(Coil_Heating_SteamFields::AirOutletNodeName).get());
  }

  // ZoneHVAC:TerminalUnit:VariableRefrigerantFlow
  {
    Model m;

    ThermalZone z(m);
    Space s(m);
    s.setThermalZone(z);

    Schedule sch = m.alwaysOnDiscreteSchedule();
    FanOnOff fan(m);
    CoilCoolingDXVariableRefrigerantFlow c(m);
    CoilHeatingDXVariableRefrigerantFlow h(m);
    CoilHeatingSteam sh = CoilHeatingSteam(m, sch);
    ZoneHVACTerminalUnitVariableRefrigerantFlow zh(m, c, h, fan);
    EXPECT_TRUE(zh.setSupplementalHeatingCoil(sh));

    zh.addToThermalZone(z);

    ForwardTranslator ft;
    Workspace w = ft.translateModel(m);

    EXPECT_EQ(1u, w.getObjectsByType(IddObjectType::Coil_Heating_Steam).size());
    WorkspaceObjectVector idf_zhs(w.getObjectsByType(IddObjectType::ZoneHVAC_TerminalUnit_VariableRefrigerantFlow));
    ASSERT_EQ(1u, idf_zhs.size());
    WorkspaceObject idf_zh(idf_zhs[0]);

    EXPECT_EQ("Coil:Heating:Steam", idf_zh.getString(ZoneHVAC_TerminalUnit_VariableRefrigerantFlowFields::SupplementalHeatingCoilObjectType).get());
    boost::optional<WorkspaceObject> woSuppHeatCoil(
      idf_zh.getTarget(ZoneHVAC_TerminalUnit_VariableRefrigerantFlowFields::SupplementalHeatingCoilName));
    EXPECT_TRUE(woSuppHeatCoil);
    EXPECT_EQ(woSuppHeatCoil->iddObject().type(), IddObjectType::Coil_Heating_Steam);
    EXPECT_EQ("Coil Heating Steam 1", woSuppHeatCoil->nameString());
    EXPECT_EQ(zh.nameString() + " Fan Outlet Node", woSuppHeatCoil->getString(Coil_Heating_SteamFields::AirInletNodeName).get());
    EXPECT_EQ(zh.outletNode()->nameString(), woSuppHeatCoil->getString(Coil_Heating_SteamFields::AirOutletNodeName).get());
  }

  // ZoneHVAC:WaterToAirHeatPump
  {
    Model m;

    ThermalZone z(m);
    Space s(m);
    s.setThermalZone(z);

    Schedule sch = m.alwaysOnDiscreteSchedule();
    FanOnOff fan(m);
    CoilHeatingWaterToAirHeatPumpEquationFit h(m);
    CoilCoolingWaterToAirHeatPumpEquationFit c(m);
    CoilHeatingSteam sh = CoilHeatingSteam(m, sch);
    ZoneHVACWaterToAirHeatPump zh(m, sch, fan, h, c, sh);

    zh.addToThermalZone(z);

    ForwardTranslator ft;
    Workspace w = ft.translateModel(m);

    EXPECT_EQ(1u, w.getObjectsByType(IddObjectType::Coil_Heating_Steam).size());
    WorkspaceObjectVector idf_zhs(w.getObjectsByType(IddObjectType::ZoneHVAC_WaterToAirHeatPump));
    ASSERT_EQ(1u, idf_zhs.size());
    WorkspaceObject idf_zh(idf_zhs[0]);

    EXPECT_EQ("Coil:Heating:Steam", idf_zh.getString(ZoneHVAC_WaterToAirHeatPumpFields::SupplementalHeatingCoilObjectType).get());
    boost::optional<WorkspaceObject> woSuppHeatCoil(idf_zh.getTarget(ZoneHVAC_WaterToAirHeatPumpFields::SupplementalHeatingCoilName));
    EXPECT_TRUE(woSuppHeatCoil);
    EXPECT_EQ(woSuppHeatCoil->iddObject().type(), IddObjectType::Coil_Heating_Steam);
    EXPECT_EQ("Coil Heating Steam 1", woSuppHeatCoil->nameString());
    EXPECT_EQ(zh.nameString() + " Heating Coil Outlet Node", woSuppHeatCoil->getString(Coil_Heating_SteamFields::AirInletNodeName).get());
    EXPECT_EQ(zh.outletNode()->nameString(), woSuppHeatCoil->getString(Coil_Heating_SteamFields::AirOutletNodeName).get());
  }

  // ZoneHVAC:UnitHeater
  {
    Model m;

    ThermalZone z(m);
    Space s(m);
    s.setThermalZone(z);

    Schedule sch = m.alwaysOnDiscreteSchedule();
    FanConstantVolume fan(m);
    CoilHeatingSteam h = CoilHeatingSteam(m, sch);
    ZoneHVACUnitHeater zh(m, sch, fan, h);

    zh.addToThermalZone(z);

    ForwardTranslator ft;
    Workspace w = ft.translateModel(m);

    EXPECT_EQ(1u, w.getObjectsByType(IddObjectType::Coil_Heating_Steam).size());
    WorkspaceObjectVector idf_zhs(w.getObjectsByType(IddObjectType::ZoneHVAC_UnitHeater));
    ASSERT_EQ(1u, idf_zhs.size());
    WorkspaceObject idf_zh(idf_zhs[0]);

    EXPECT_EQ("Coil:Heating:Steam", idf_zh.getString(ZoneHVAC_UnitHeaterFields::HeatingCoilObjectType).get());
    boost::optional<WorkspaceObject> woHeatCoil(idf_zh.getTarget(ZoneHVAC_UnitHeaterFields::HeatingCoilName));
    EXPECT_TRUE(woHeatCoil);
    EXPECT_EQ(woHeatCoil->iddObject().type(), IddObjectType::Coil_Heating_Steam);
    EXPECT_EQ("Coil Heating Steam 1", woHeatCoil->nameString());
    EXPECT_EQ(zh.nameString() + " Fan Outlet Node", woHeatCoil->getString(Coil_Heating_SteamFields::AirInletNodeName).get());
    EXPECT_EQ(zh.outletNode()->nameString(), woHeatCoil->getString(Coil_Heating_SteamFields::AirOutletNodeName).get());
  }

  // ZoneHVAC:UnitVentilator
  {
    Model m;

    ThermalZone z(m);
    Space s(m);
    s.setThermalZone(z);

    Schedule sch = m.alwaysOnDiscreteSchedule();
    FanConstantVolume fan(m);
    CoilHeatingSteam h = CoilHeatingSteam(m, sch);
    ZoneHVACUnitVentilator zh(m, fan);
    EXPECT_TRUE(zh.setHeatingCoil(h));

    zh.addToThermalZone(z);

    ForwardTranslator ft;
    Workspace w = ft.translateModel(m);

    EXPECT_EQ(1u, w.getObjectsByType(IddObjectType::Coil_Heating_Steam).size());
    WorkspaceObjectVector idf_zhs(w.getObjectsByType(IddObjectType::ZoneHVAC_UnitVentilator));
    ASSERT_EQ(1u, idf_zhs.size());
    WorkspaceObject idf_zh(idf_zhs[0]);

    EXPECT_EQ("Coil:Heating:Steam", idf_zh.getString(ZoneHVAC_UnitVentilatorFields::HeatingCoilObjectType).get());
    boost::optional<WorkspaceObject> woHeatCoil(idf_zh.getTarget(ZoneHVAC_UnitVentilatorFields::HeatingCoilName));
    EXPECT_TRUE(woHeatCoil);
    EXPECT_EQ(woHeatCoil->iddObject().type(), IddObjectType::Coil_Heating_Steam);
    EXPECT_EQ("Coil Heating Steam 1", woHeatCoil->nameString());
    EXPECT_EQ(zh.nameString() + " Fan Outlet Node", woHeatCoil->getString(Coil_Heating_SteamFields::AirInletNodeName).get());
    EXPECT_EQ(zh.outletNode()->nameString(), woHeatCoil->getString(Coil_Heating_SteamFields::AirOutletNodeName).get());
  }
}
