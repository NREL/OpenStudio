/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>
#include "EnergyPlusFixture.hpp"

#include "../ForwardTranslator.hpp"

#include "../../model/Model.hpp"
#include "../../model/ThermalZone.hpp"
#include "../../model/Space.hpp"
#include "../../model/AirLoopHVAC.hpp"
#include "../../model/AirLoopHVACUnitarySystem.hpp"
#include "../../model/ZoneHVACWaterToAirHeatPump.hpp"
#include "../../model/ZoneHVACTerminalUnitVariableRefrigerantFlow.hpp"
#include "../../model/CoilCoolingDXVariableSpeed.hpp"
#include "../../model/CoilCoolingDXVariableSpeedSpeedData.hpp"
#include "../../model/CoilHeatingWaterToAirHeatPumpEquationFit.hpp"
#include "../../model/CoilCoolingWaterToAirHeatPumpEquationFit.hpp"
#include "../../model/CoilCoolingDXVariableRefrigerantFlow.hpp"
#include "../../model/CoilHeatingDXVariableRefrigerantFlow.hpp"
#include "../../model/FanOnOff.hpp"
#include "../../model/CoilHeatingElectric.hpp"
#include "../../model/ScheduleConstant.hpp"
#include "../../model/Node.hpp"
#include "../../model/UnitarySystemPerformanceMultispeed.hpp"

#include <utilities/idd/AirLoopHVAC_UnitarySystem_FieldEnums.hxx>
#include <utilities/idd/ZoneHVAC_WaterToAirHeatPump_FieldEnums.hxx>
#include <utilities/idd/ZoneHVAC_TerminalUnit_VariableRefrigerantFlow_FieldEnums.hxx>
#include <utilities/idd/UnitarySystemPerformance_Multispeed_FieldEnums.hxx>

#include <utilities/idd/IddEnums.hxx>
#include "../../utilities/idf/IdfObject.hpp"
#include "../../utilities/idf/IdfObject_Impl.hpp"

#include "../../utilities/idf/WorkspaceObject.hpp"
#include "../../utilities/idf/WorkspaceObject_Impl.hpp"

using namespace openstudio::energyplus;
using namespace openstudio::model;
using namespace openstudio;

TEST_F(EnergyPlusFixture, ForwardTranslator_UnitarySystemPerformanceMultispeed_AirLoopHVACUnitarySystem) {
  Model m;

  CoilCoolingDXVariableSpeed coil(m);
  CoilCoolingDXVariableSpeedSpeedData speed1(m);
  EXPECT_TRUE(coil.addSpeed(speed1));
  CoilCoolingDXVariableSpeedSpeedData speed2(m);
  EXPECT_TRUE(coil.addSpeed(speed2));

  AirLoopHVACUnitarySystem unitary(m);
  unitary.setCoolingCoil(coil);

  AirLoopHVAC airLoop(m);
  Node supplyOutletNode = airLoop.supplyOutletNode();
  unitary.addToNode(supplyOutletNode);

  UnitarySystemPerformanceMultispeed perf(m);
  perf.setName("US Perf Multispeed");
  EXPECT_TRUE(perf.addSupplyAirflowRatioField(1.0, 2.0));
  EXPECT_TRUE(unitary.setDesignSpecificationMultispeedObject(perf));

  ForwardTranslator ft;
  Workspace w = ft.translateModel(m);

  WorkspaceObjectVector idf_unitarys(w.getObjectsByType(IddObjectType::AirLoopHVAC_UnitarySystem));
  EXPECT_EQ(1, idf_unitarys.size());
  WorkspaceObject& idf_unitary = idf_unitarys.front();

  EXPECT_EQ("UnitarySystemPerformance:Multispeed",
            idf_unitary.getString(AirLoopHVAC_UnitarySystemFields::DesignSpecificationMultispeedObjectType).get());
  EXPECT_EQ("US Perf Multispeed", idf_unitary.getString(AirLoopHVAC_UnitarySystemFields::DesignSpecificationMultispeedObjectName).get());

  WorkspaceObjectVector idf_perfs(w.getObjectsByType(IddObjectType::UnitarySystemPerformance_Multispeed));
  EXPECT_EQ(1, idf_perfs.size());
  WorkspaceObject& idf_perf = idf_perfs.front();

  EXPECT_EQ(0, idf_perf.getInt(UnitarySystemPerformance_MultispeedFields::NumberofSpeedsforHeating).get());
  EXPECT_EQ(2, idf_perf.getInt(UnitarySystemPerformance_MultispeedFields::NumberofSpeedsforCooling).get());
  EXPECT_EQ("No", idf_perf.getString(UnitarySystemPerformance_MultispeedFields::SingleModeOperation).get());
  EXPECT_EQ(1.0, idf_perf.getDouble(UnitarySystemPerformance_MultispeedFields::NoLoadSupplyAirFlowRateRatio).get());

  ASSERT_EQ(1u, idf_perf.numExtensibleGroups());
}

TEST_F(EnergyPlusFixture, ForwardTranslator_UnitarySystemPerformanceMultispeed_ZoneHVACWaterToAirHeatPump) {
  Model m;

  Schedule sch = m.alwaysOnDiscreteSchedule();
  FanOnOff fan(m);
  CoilHeatingWaterToAirHeatPumpEquationFit htg_coil(m);
  CoilCoolingWaterToAirHeatPumpEquationFit clg_coil(m);
  CoilHeatingElectric supp_htg_coil(m);

  ZoneHVACWaterToAirHeatPump hp(m, sch, fan, htg_coil, clg_coil, supp_htg_coil);

  UnitarySystemPerformanceMultispeed perf(m);
  perf.setName("US Perf Multispeed");
  EXPECT_TRUE(perf.addSupplyAirflowRatioField(1.0, 2.0));
  EXPECT_TRUE(perf.addSupplyAirflowRatioField(3.0, 4.0));
  EXPECT_TRUE(hp.setDesignSpecificationMultispeedObject(perf));

  ThermalZone z(m);
  Space s(m);
  s.setThermalZone(z);
  EXPECT_TRUE(hp.addToThermalZone(z));

  ForwardTranslator ft;
  Workspace w = ft.translateModel(m);

  WorkspaceObjectVector idf_hps(w.getObjectsByType(IddObjectType::ZoneHVAC_WaterToAirHeatPump));
  EXPECT_EQ(1, idf_hps.size());
  WorkspaceObject& idf_hp = idf_hps.front();

  EXPECT_EQ("UnitarySystemPerformance:Multispeed",
            idf_hp.getString(ZoneHVAC_WaterToAirHeatPumpFields::DesignSpecificationMultispeedObjectType).get());
  EXPECT_EQ("US Perf Multispeed", idf_hp.getString(ZoneHVAC_WaterToAirHeatPumpFields::DesignSpecificationMultispeedObjectName).get());

  WorkspaceObjectVector idf_perfs(w.getObjectsByType(IddObjectType::UnitarySystemPerformance_Multispeed));
  EXPECT_EQ(1, idf_perfs.size());
  WorkspaceObject& idf_perf = idf_perfs.front();

  EXPECT_EQ(1, idf_perf.getInt(UnitarySystemPerformance_MultispeedFields::NumberofSpeedsforHeating).get());
  EXPECT_EQ(1, idf_perf.getInt(UnitarySystemPerformance_MultispeedFields::NumberofSpeedsforCooling).get());
  EXPECT_EQ("No", idf_perf.getString(UnitarySystemPerformance_MultispeedFields::SingleModeOperation).get());
  EXPECT_EQ(1.0, idf_perf.getDouble(UnitarySystemPerformance_MultispeedFields::NoLoadSupplyAirFlowRateRatio).get());

  ASSERT_EQ(2u, idf_perf.numExtensibleGroups());
}

TEST_F(EnergyPlusFixture, ForwardTranslator_UnitarySystemPerformanceMultispeed_ZoneHVACTerminalUnitVariableRefrigerantFlow) {
  Model m;

  FanOnOff fan(m);
  CoilCoolingDXVariableRefrigerantFlow cc(m);
  CoilHeatingDXVariableRefrigerantFlow hc(m);

  ZoneHVACTerminalUnitVariableRefrigerantFlow vrf(m, cc, hc, fan);

  UnitarySystemPerformanceMultispeed perf(m);
  perf.setName("US Perf Multispeed");
  EXPECT_TRUE(perf.addSupplyAirflowRatioField(1.0, 2.0));
  EXPECT_TRUE(perf.addSupplyAirflowRatioField(3.0, 4.0));
  EXPECT_TRUE(perf.addSupplyAirflowRatioField(5.0, 6.0));
  EXPECT_TRUE(vrf.setDesignSpecificationMultispeedObject(perf));

  ThermalZone z(m);
  Space s(m);
  s.setThermalZone(z);
  EXPECT_TRUE(vrf.addToThermalZone(z));

  ForwardTranslator ft;
  Workspace w = ft.translateModel(m);

  WorkspaceObjectVector idf_vrfs(w.getObjectsByType(IddObjectType::ZoneHVAC_TerminalUnit_VariableRefrigerantFlow));
  EXPECT_EQ(1, idf_vrfs.size());
  WorkspaceObject& idf_vrf = idf_vrfs.front();

  EXPECT_EQ("UnitarySystemPerformance:Multispeed",
            idf_vrf.getString(ZoneHVAC_TerminalUnit_VariableRefrigerantFlowFields::DesignSpecificationMultispeedObjectType).get());
  EXPECT_EQ("US Perf Multispeed",
            idf_vrf.getString(ZoneHVAC_TerminalUnit_VariableRefrigerantFlowFields::DesignSpecificationMultispeedObjectName).get());

  WorkspaceObjectVector idf_perfs(w.getObjectsByType(IddObjectType::UnitarySystemPerformance_Multispeed));
  EXPECT_EQ(1, idf_perfs.size());
  WorkspaceObject& idf_perf = idf_perfs.front();

  EXPECT_EQ(1, idf_perf.getInt(UnitarySystemPerformance_MultispeedFields::NumberofSpeedsforHeating).get());
  EXPECT_EQ(1, idf_perf.getInt(UnitarySystemPerformance_MultispeedFields::NumberofSpeedsforCooling).get());
  EXPECT_EQ("No", idf_perf.getString(UnitarySystemPerformance_MultispeedFields::SingleModeOperation).get());
  EXPECT_EQ(1.0, idf_perf.getDouble(UnitarySystemPerformance_MultispeedFields::NoLoadSupplyAirFlowRateRatio).get());

  ASSERT_EQ(3u, idf_perf.numExtensibleGroups());
}

TEST_F(EnergyPlusFixture, ForwardTranslator_UnitarySystemPerformanceMultispeed_DoubleAssign) {
  Model m;

  CoilCoolingDXVariableSpeed coil(m);
  CoilCoolingDXVariableSpeedSpeedData speed(m);
  EXPECT_TRUE(coil.addSpeed(speed));

  AirLoopHVACUnitarySystem unitary(m);
  unitary.setCoolingCoil(coil);
  AirLoopHVACUnitarySystem unitary2(m);
  unitary2.setCoolingCoil(coil);

  UnitarySystemPerformanceMultispeed perf(m);
  EXPECT_TRUE(unitary.setDesignSpecificationMultispeedObject(perf));
  EXPECT_TRUE(unitary2.setDesignSpecificationMultispeedObject(perf));

  AirLoopHVAC airLoop(m);
  Node supplyOutletNode = airLoop.supplyOutletNode();
  unitary.addToNode(supplyOutletNode);
  unitary2.addToNode(supplyOutletNode);

  ForwardTranslator ft;
  Workspace w = ft.translateModel(m);

  EXPECT_EQ(1u, ft.errors().size());
}
