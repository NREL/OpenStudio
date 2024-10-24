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
#include "../../model/CoilHeatingDXMultiSpeed.hpp"
#include "../../model/CoilHeatingDXMultiSpeedStageData.hpp"
#include "../../model/CoilCoolingDXMultiSpeed.hpp"
#include "../../model/CoilCoolingDXMultiSpeedStageData.hpp"
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

  ASSERT_EQ(2u, idf_perf.numExtensibleGroups());
}

TEST_F(EnergyPlusFixture, ForwardTranslator_UnitarySystemPerformanceMultispeed_Speeds) {
  // #5277: Forward Translator Issues for UnitarySystemPerformance:Multispeed
  Model m;

  CoilHeatingDXMultiSpeed htgcoil(m);
  CoilHeatingDXMultiSpeedStageData htgstage1(m);
  EXPECT_TRUE(htgcoil.addStage(htgstage1));
  CoilHeatingDXMultiSpeedStageData htgstage2(m);
  EXPECT_TRUE(htgcoil.addStage(htgstage2));
  CoilHeatingDXMultiSpeedStageData htgstage3(m);
  EXPECT_TRUE(htgcoil.addStage(htgstage3));
  CoilHeatingDXMultiSpeedStageData htgstage4(m);
  EXPECT_TRUE(htgcoil.addStage(htgstage4));

  CoilCoolingDXMultiSpeed clgcoil(m);
  CoilCoolingDXMultiSpeedStageData clgstage1(m);
  EXPECT_TRUE(clgcoil.addStage(clgstage1));
  CoilCoolingDXMultiSpeedStageData clgstage2(m);
  EXPECT_TRUE(clgcoil.addStage(clgstage2));

  AirLoopHVACUnitarySystem unitary(m);
  unitary.setHeatingCoil(htgcoil);
  unitary.setCoolingCoil(clgcoil);

  AirLoopHVAC airLoop(m);
  Node supplyOutletNode = airLoop.supplyOutletNode();

  WorkspaceObjectVector idf_unitarys(w.getObjectsByType(IddObjectType::AirLoopHVAC_UnitarySystem));
  EXPECT_EQ(1, idf_unitarys.size());
  WorkspaceObject& idf_unitary = idf_unitarys.front();

  EXPECT_EQ("UnitarySystemPerformance:Multispeed",
            idf_unitary.getString(AirLoopHVAC_UnitarySystemFields::DesignSpecificationMultispeedObjectType).get());
  EXPECT_EQ(unitary.nameString() + " Unitary System Performance",
            idf_unitary.getString(AirLoopHVAC_UnitarySystemFields::DesignSpecificationMultispeedObjectName).get());

  WorkspaceObjectVector idf_perfs(w.getObjectsByType(IddObjectType::UnitarySystemPerformance_Multispeed));
  EXPECT_EQ(1, idf_perfs.size());
  WorkspaceObject& idf_perf = idf_perfs.front();

  EXPECT_EQ(4, idf_perf.getInt(UnitarySystemPerformance_MultispeedFields::NumberofSpeedsforHeating).get());
  EXPECT_EQ(2, idf_perf.getInt(UnitarySystemPerformance_MultispeedFields::NumberofSpeedsforCooling).get());
  EXPECT_EQ("No", idf_perf.getString(UnitarySystemPerformance_MultispeedFields::SingleModeOperation).get());
  EXPECT_EQ(1.0, idf_perf.getDouble(UnitarySystemPerformance_MultispeedFields::NoLoadSupplyAirFlowRateRatio).get());

  ASSERT_EQ(4u, idf_perf.numExtensibleGroups());
}
