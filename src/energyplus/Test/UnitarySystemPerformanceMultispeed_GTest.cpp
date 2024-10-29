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
#include <utilities/idd/UnitarySystemPerformance_Multispeed_FieldEnums.hxx>

#include <utilities/idd/IddEnums.hxx>
#include "../../utilities/idf/IdfObject.hpp"
#include "../../utilities/idf/IdfObject_Impl.hpp"
#include "../../utilities/idf/IdfExtensibleGroup.hpp"

#include "../../utilities/idf/WorkspaceObject.hpp"
#include "../../utilities/idf/WorkspaceObject_Impl.hpp"

using namespace openstudio::energyplus;
using namespace openstudio::model;
using namespace openstudio;

TEST_F(EnergyPlusFixture, ForwardTranslator_UnitarySystemPerformanceMultispeed_Ctor) {
  {  // h == c
    Model m;

    CoilHeatingDXMultiSpeed htgcoil(m);
    CoilHeatingDXMultiSpeedStageData htgstage1(m);
    EXPECT_TRUE(htgcoil.addStage(htgstage1));
    CoilHeatingDXMultiSpeedStageData htgstage2(m);
    EXPECT_TRUE(htgcoil.addStage(htgstage2));

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
    unitary.addToNode(supplyOutletNode);

    UnitarySystemPerformanceMultispeed perf(m);
    perf.setName("US Perf Multispeed");
    EXPECT_TRUE(perf.addSupplyAirflowRatioField(0.1, 0.2));
    EXPECT_TRUE(perf.addSupplyAirflowRatioField(0.3, 0.4));
    EXPECT_TRUE(unitary.setDesignSpecificationMultispeedObject(perf));

    ForwardTranslator ft;
    Workspace w = ft.translateModel(m);

    const auto idf_unitarys = w.getObjectsByType(IddObjectType::AirLoopHVAC_UnitarySystem);
    EXPECT_EQ(1, idf_unitarys.size());
    const WorkspaceObject& idf_unitary = idf_unitarys.front();

    EXPECT_EQ("UnitarySystemPerformance:Multispeed",
              idf_unitary.getString(AirLoopHVAC_UnitarySystemFields::DesignSpecificationMultispeedObjectType).get());
    EXPECT_EQ("US Perf Multispeed", idf_unitary.getString(AirLoopHVAC_UnitarySystemFields::DesignSpecificationMultispeedObjectName).get());

    const auto idf_perfs = w.getObjectsByType(IddObjectType::UnitarySystemPerformance_Multispeed);
    EXPECT_EQ(1, idf_perfs.size());
    const WorkspaceObject& idf_perf = idf_perfs.front();

    EXPECT_EQ(2, idf_perf.getInt(UnitarySystemPerformance_MultispeedFields::NumberofSpeedsforHeating).get());
    EXPECT_EQ(2, idf_perf.getInt(UnitarySystemPerformance_MultispeedFields::NumberofSpeedsforCooling).get());
    EXPECT_EQ("No", idf_perf.getString(UnitarySystemPerformance_MultispeedFields::SingleModeOperation).get());
    EXPECT_EQ(1.0, idf_perf.getDouble(UnitarySystemPerformance_MultispeedFields::NoLoadSupplyAirFlowRateRatio).get());

    ASSERT_EQ(2u, idf_perf.numExtensibleGroups());

    auto egs = idf_perf.extensibleGroups();

    IdfExtensibleGroup eg1 = egs[0];
    ASSERT_TRUE(eg1.getDouble(UnitarySystemPerformance_MultispeedExtensibleFields::HeatingSpeedSupplyAirFlowRatio));
    EXPECT_EQ(0.1, eg1.getDouble(UnitarySystemPerformance_MultispeedExtensibleFields::HeatingSpeedSupplyAirFlowRatio).get());
    ASSERT_TRUE(eg1.getDouble(UnitarySystemPerformance_MultispeedExtensibleFields::CoolingSpeedSupplyAirFlowRatio));
    EXPECT_EQ(0.2, eg1.getDouble(UnitarySystemPerformance_MultispeedExtensibleFields::CoolingSpeedSupplyAirFlowRatio).get());

    IdfExtensibleGroup eg2 = egs[1];
    ASSERT_TRUE(eg2.getDouble(UnitarySystemPerformance_MultispeedExtensibleFields::HeatingSpeedSupplyAirFlowRatio));
    EXPECT_EQ(0.3, eg2.getDouble(UnitarySystemPerformance_MultispeedExtensibleFields::HeatingSpeedSupplyAirFlowRatio).get());
    ASSERT_TRUE(eg2.getDouble(UnitarySystemPerformance_MultispeedExtensibleFields::CoolingSpeedSupplyAirFlowRatio));
    EXPECT_EQ(0.4, eg2.getDouble(UnitarySystemPerformance_MultispeedExtensibleFields::CoolingSpeedSupplyAirFlowRatio).get());
  }

  {  // h > c
    Model m;

    CoilHeatingDXMultiSpeed htgcoil(m);
    CoilHeatingDXMultiSpeedStageData htgstage1(m);
    EXPECT_TRUE(htgcoil.addStage(htgstage1));
    CoilHeatingDXMultiSpeedStageData htgstage2(m);
    EXPECT_TRUE(htgcoil.addStage(htgstage2));

    CoilCoolingDXMultiSpeed clgcoil(m);
    CoilCoolingDXMultiSpeedStageData clgstage1(m);
    EXPECT_TRUE(clgcoil.addStage(clgstage1));

    AirLoopHVACUnitarySystem unitary(m);
    unitary.setHeatingCoil(htgcoil);
    unitary.setCoolingCoil(clgcoil);

    AirLoopHVAC airLoop(m);
    Node supplyOutletNode = airLoop.supplyOutletNode();
    unitary.addToNode(supplyOutletNode);

    UnitarySystemPerformanceMultispeed perf(m);
    perf.setName("US Perf Multispeed");
    EXPECT_TRUE(perf.addSupplyAirflowRatioField(0.1, 0.2));
    EXPECT_TRUE(perf.addSupplyAirflowRatioField(SupplyAirflowRatioField::fromHeatingRatio(0.3)));
    EXPECT_TRUE(unitary.setDesignSpecificationMultispeedObject(perf));

    ForwardTranslator ft;
    Workspace w = ft.translateModel(m);

    const auto idf_unitarys = w.getObjectsByType(IddObjectType::AirLoopHVAC_UnitarySystem);
    EXPECT_EQ(1, idf_unitarys.size());
    const WorkspaceObject& idf_unitary = idf_unitarys.front();

    EXPECT_EQ("UnitarySystemPerformance:Multispeed",
              idf_unitary.getString(AirLoopHVAC_UnitarySystemFields::DesignSpecificationMultispeedObjectType).get());
    EXPECT_EQ("US Perf Multispeed", idf_unitary.getString(AirLoopHVAC_UnitarySystemFields::DesignSpecificationMultispeedObjectName).get());

    const auto idf_perfs = w.getObjectsByType(IddObjectType::UnitarySystemPerformance_Multispeed);
    EXPECT_EQ(1, idf_perfs.size());
    const WorkspaceObject& idf_perf = idf_perfs.front();

    EXPECT_EQ(2, idf_perf.getInt(UnitarySystemPerformance_MultispeedFields::NumberofSpeedsforHeating).get());
    EXPECT_EQ(1, idf_perf.getInt(UnitarySystemPerformance_MultispeedFields::NumberofSpeedsforCooling).get());
    EXPECT_EQ("No", idf_perf.getString(UnitarySystemPerformance_MultispeedFields::SingleModeOperation).get());
    EXPECT_EQ(1.0, idf_perf.getDouble(UnitarySystemPerformance_MultispeedFields::NoLoadSupplyAirFlowRateRatio).get());

    ASSERT_EQ(2u, idf_perf.numExtensibleGroups());

    const auto egs = idf_perf.extensibleGroups();

    {
      const IdfExtensibleGroup& eg = egs[0];
      ASSERT_TRUE(eg.getDouble(UnitarySystemPerformance_MultispeedExtensibleFields::HeatingSpeedSupplyAirFlowRatio));
      EXPECT_EQ(0.1, eg.getDouble(UnitarySystemPerformance_MultispeedExtensibleFields::HeatingSpeedSupplyAirFlowRatio).get());
      ASSERT_TRUE(eg.getDouble(UnitarySystemPerformance_MultispeedExtensibleFields::CoolingSpeedSupplyAirFlowRatio));
      EXPECT_EQ(0.2, eg.getDouble(UnitarySystemPerformance_MultispeedExtensibleFields::CoolingSpeedSupplyAirFlowRatio).get());
    }
    {
      const IdfExtensibleGroup& eg = egs[1];
      ASSERT_TRUE(eg.getDouble(UnitarySystemPerformance_MultispeedExtensibleFields::HeatingSpeedSupplyAirFlowRatio));
      EXPECT_EQ(0.3, eg.getDouble(UnitarySystemPerformance_MultispeedExtensibleFields::HeatingSpeedSupplyAirFlowRatio).get());
      ASSERT_FALSE(eg.getDouble(UnitarySystemPerformance_MultispeedExtensibleFields::CoolingSpeedSupplyAirFlowRatio));
      EXPECT_EQ("autosize", eg.getString(UnitarySystemPerformance_MultispeedExtensibleFields::CoolingSpeedSupplyAirFlowRatio).get());
    }
  }

  {  // h < c
    Model m;

    CoilHeatingDXMultiSpeed htgcoil(m);
    CoilHeatingDXMultiSpeedStageData htgstage1(m);
    EXPECT_TRUE(htgcoil.addStage(htgstage1));

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
    unitary.addToNode(supplyOutletNode);

    UnitarySystemPerformanceMultispeed perf(m);
    perf.setName("US Perf Multispeed");
    EXPECT_TRUE(perf.addSupplyAirflowRatioField(0.1, 0.2));
    EXPECT_TRUE(perf.addSupplyAirflowRatioField(SupplyAirflowRatioField::fromCoolingRatio(0.3)));
    EXPECT_TRUE(unitary.setDesignSpecificationMultispeedObject(perf));

    ForwardTranslator ft;
    Workspace w = ft.translateModel(m);

    const auto idf_unitarys = w.getObjectsByType(IddObjectType::AirLoopHVAC_UnitarySystem);
    EXPECT_EQ(1, idf_unitarys.size());
    const WorkspaceObject& idf_unitary = idf_unitarys.front();

    EXPECT_EQ("UnitarySystemPerformance:Multispeed",
              idf_unitary.getString(AirLoopHVAC_UnitarySystemFields::DesignSpecificationMultispeedObjectType).get());
    EXPECT_EQ("US Perf Multispeed", idf_unitary.getString(AirLoopHVAC_UnitarySystemFields::DesignSpecificationMultispeedObjectName).get());

    const auto idf_perfs = w.getObjectsByType(IddObjectType::UnitarySystemPerformance_Multispeed);
    EXPECT_EQ(1, idf_perfs.size());
    const WorkspaceObject& idf_perf = idf_perfs.front();

    EXPECT_EQ(1, idf_perf.getInt(UnitarySystemPerformance_MultispeedFields::NumberofSpeedsforHeating).get());
    EXPECT_EQ(2, idf_perf.getInt(UnitarySystemPerformance_MultispeedFields::NumberofSpeedsforCooling).get());
    EXPECT_EQ("No", idf_perf.getString(UnitarySystemPerformance_MultispeedFields::SingleModeOperation).get());
    EXPECT_EQ(1.0, idf_perf.getDouble(UnitarySystemPerformance_MultispeedFields::NoLoadSupplyAirFlowRateRatio).get());

    ASSERT_EQ(2u, idf_perf.numExtensibleGroups());

    const auto egs = idf_perf.extensibleGroups();
    {
      const IdfExtensibleGroup& eg = egs[0];
      ASSERT_TRUE(eg.getDouble(UnitarySystemPerformance_MultispeedExtensibleFields::HeatingSpeedSupplyAirFlowRatio));
      EXPECT_EQ(0.1, eg.getDouble(UnitarySystemPerformance_MultispeedExtensibleFields::HeatingSpeedSupplyAirFlowRatio).get());
      ASSERT_TRUE(eg.getDouble(UnitarySystemPerformance_MultispeedExtensibleFields::CoolingSpeedSupplyAirFlowRatio));
      EXPECT_EQ(0.2, eg.getDouble(UnitarySystemPerformance_MultispeedExtensibleFields::CoolingSpeedSupplyAirFlowRatio).get());
    }
    {
      const IdfExtensibleGroup& eg = egs[1];
      ASSERT_FALSE(eg.getDouble(UnitarySystemPerformance_MultispeedExtensibleFields::HeatingSpeedSupplyAirFlowRatio));
      EXPECT_EQ("autosize", eg.getString(UnitarySystemPerformance_MultispeedExtensibleFields::HeatingSpeedSupplyAirFlowRatio).get());
      ASSERT_TRUE(eg.getDouble(UnitarySystemPerformance_MultispeedExtensibleFields::CoolingSpeedSupplyAirFlowRatio));
      EXPECT_EQ(0.3, eg.getDouble(UnitarySystemPerformance_MultispeedExtensibleFields::CoolingSpeedSupplyAirFlowRatio).get());
    }
  }
}

TEST_F(EnergyPlusFixture, ForwardTranslator_UnitarySystemPerformanceMultispeed_AirLoopHVACUnitarySystem_Defaulted) {
  // When the user didn't specifically instantiate and assign a UnitarySystemPerformanceMultispeed object, the FT creates one

  {  // h == c
    Model m;

    CoilHeatingDXMultiSpeed htgcoil(m);
    CoilHeatingDXMultiSpeedStageData htgstage1(m);
    EXPECT_TRUE(htgstage1.setRatedAirFlowRate(1));
    EXPECT_TRUE(htgcoil.addStage(htgstage1));
    CoilHeatingDXMultiSpeedStageData htgstage2(m);
    EXPECT_TRUE(htgstage2.setRatedAirFlowRate(2));
    EXPECT_TRUE(htgcoil.addStage(htgstage2));

    CoilCoolingDXMultiSpeed clgcoil(m);
    CoilCoolingDXMultiSpeedStageData clgstage1(m);
    EXPECT_TRUE(clgstage1.setRatedAirFlowRate(3));
    EXPECT_TRUE(clgcoil.addStage(clgstage1));
    CoilCoolingDXMultiSpeedStageData clgstage2(m);
    EXPECT_TRUE(clgstage2.setRatedAirFlowRate(4));
    EXPECT_TRUE(clgcoil.addStage(clgstage2));

    AirLoopHVACUnitarySystem unitary(m);
    unitary.setHeatingCoil(htgcoil);
    unitary.setCoolingCoil(clgcoil);
    unitary.setSupplyAirFlowRateDuringCoolingOperation(10);
    unitary.setSupplyAirFlowRateDuringHeatingOperation(10);

    AirLoopHVAC airLoop(m);
    Node supplyOutletNode = airLoop.supplyOutletNode();
    unitary.addToNode(supplyOutletNode);

    ForwardTranslator ft;
    Workspace w = ft.translateModel(m);

    const auto idf_unitarys = w.getObjectsByType(IddObjectType::AirLoopHVAC_UnitarySystem);
    EXPECT_EQ(1, idf_unitarys.size());
    const auto& idf_unitary = idf_unitarys.front();

    EXPECT_EQ("UnitarySystemPerformance:Multispeed",
              idf_unitary.getString(AirLoopHVAC_UnitarySystemFields::DesignSpecificationMultispeedObjectType).get());
    EXPECT_EQ(unitary.nameString() + " Unitary System Performance",
              idf_unitary.getString(AirLoopHVAC_UnitarySystemFields::DesignSpecificationMultispeedObjectName).get());

    const auto idf_perfs = w.getObjectsByType(IddObjectType::UnitarySystemPerformance_Multispeed);
    EXPECT_EQ(1, idf_perfs.size());
    const auto& idf_perf = idf_perfs.front();

    EXPECT_EQ(2, idf_perf.getInt(UnitarySystemPerformance_MultispeedFields::NumberofSpeedsforHeating).get());
    EXPECT_EQ(2, idf_perf.getInt(UnitarySystemPerformance_MultispeedFields::NumberofSpeedsforCooling).get());
    EXPECT_EQ("No", idf_perf.getString(UnitarySystemPerformance_MultispeedFields::SingleModeOperation).get());
    EXPECT_TRUE(idf_perf.isEmpty(UnitarySystemPerformance_MultispeedFields::NoLoadSupplyAirFlowRateRatio));

    ASSERT_EQ(2u, idf_perf.numExtensibleGroups());

    const auto egs = idf_perf.extensibleGroups();

    {
      const IdfExtensibleGroup& eg = egs[0];
      ASSERT_TRUE(eg.getDouble(UnitarySystemPerformance_MultispeedExtensibleFields::HeatingSpeedSupplyAirFlowRatio));
      EXPECT_EQ(0.1, eg.getDouble(UnitarySystemPerformance_MultispeedExtensibleFields::HeatingSpeedSupplyAirFlowRatio).get());
      ASSERT_TRUE(eg.getDouble(UnitarySystemPerformance_MultispeedExtensibleFields::CoolingSpeedSupplyAirFlowRatio));
      EXPECT_EQ(0.3, eg.getDouble(UnitarySystemPerformance_MultispeedExtensibleFields::CoolingSpeedSupplyAirFlowRatio).get());
    }

    {
      const IdfExtensibleGroup& eg = egs[1];
      ASSERT_TRUE(eg.getDouble(UnitarySystemPerformance_MultispeedExtensibleFields::HeatingSpeedSupplyAirFlowRatio));
      EXPECT_EQ(0.2, eg.getDouble(UnitarySystemPerformance_MultispeedExtensibleFields::HeatingSpeedSupplyAirFlowRatio).get());
      ASSERT_TRUE(eg.getDouble(UnitarySystemPerformance_MultispeedExtensibleFields::CoolingSpeedSupplyAirFlowRatio));
      EXPECT_EQ(0.4, eg.getDouble(UnitarySystemPerformance_MultispeedExtensibleFields::CoolingSpeedSupplyAirFlowRatio).get());
    }
  }

  {  // h > c
    // #5277: Forward Translator Issues for UnitarySystemPerformance:Multispeed
    Model m;

    CoilHeatingDXMultiSpeed htgcoil(m);
    CoilHeatingDXMultiSpeedStageData htgstage1(m);
    EXPECT_TRUE(htgstage1.setRatedAirFlowRate(1));
    EXPECT_TRUE(htgcoil.addStage(htgstage1));
    CoilHeatingDXMultiSpeedStageData htgstage2(m);
    EXPECT_TRUE(htgstage2.setRatedAirFlowRate(2));
    EXPECT_TRUE(htgcoil.addStage(htgstage2));

    CoilCoolingDXMultiSpeed clgcoil(m);
    CoilCoolingDXMultiSpeedStageData clgstage1(m);
    EXPECT_TRUE(clgstage1.setRatedAirFlowRate(3));
    EXPECT_TRUE(clgcoil.addStage(clgstage1));

    AirLoopHVACUnitarySystem unitary(m);
    unitary.setHeatingCoil(htgcoil);
    unitary.setCoolingCoil(clgcoil);
    unitary.setSupplyAirFlowRateDuringCoolingOperation(10);
    unitary.setSupplyAirFlowRateDuringHeatingOperation(10);

    AirLoopHVAC airLoop(m);
    Node supplyOutletNode = airLoop.supplyOutletNode();
    unitary.addToNode(supplyOutletNode);

    ForwardTranslator ft;
    Workspace w = ft.translateModel(m);

    const auto& idf_unitarys = w.getObjectsByType(IddObjectType::AirLoopHVAC_UnitarySystem);
    EXPECT_EQ(1, idf_unitarys.size());
    const WorkspaceObject& idf_unitary = idf_unitarys.front();

    EXPECT_EQ("UnitarySystemPerformance:Multispeed",
              idf_unitary.getString(AirLoopHVAC_UnitarySystemFields::DesignSpecificationMultispeedObjectType).get());
    EXPECT_EQ(unitary.nameString() + " Unitary System Performance",
              idf_unitary.getString(AirLoopHVAC_UnitarySystemFields::DesignSpecificationMultispeedObjectName).get());

    const auto idf_perfs = w.getObjectsByType(IddObjectType::UnitarySystemPerformance_Multispeed);
    EXPECT_EQ(1, idf_perfs.size());
    const WorkspaceObject& idf_perf = idf_perfs.front();

    EXPECT_EQ(2, idf_perf.getInt(UnitarySystemPerformance_MultispeedFields::NumberofSpeedsforHeating).get());
    EXPECT_EQ(1, idf_perf.getInt(UnitarySystemPerformance_MultispeedFields::NumberofSpeedsforCooling).get());
    EXPECT_EQ("No", idf_perf.getString(UnitarySystemPerformance_MultispeedFields::SingleModeOperation).get());
    EXPECT_TRUE(idf_perf.isEmpty(UnitarySystemPerformance_MultispeedFields::NoLoadSupplyAirFlowRateRatio));

    ASSERT_EQ(2u, idf_perf.numExtensibleGroups());

    const auto egs = idf_perf.extensibleGroups();
    {
      const IdfExtensibleGroup& eg = egs[0];
      ASSERT_TRUE(eg.getDouble(UnitarySystemPerformance_MultispeedExtensibleFields::HeatingSpeedSupplyAirFlowRatio));
      EXPECT_EQ(0.1, eg.getDouble(UnitarySystemPerformance_MultispeedExtensibleFields::HeatingSpeedSupplyAirFlowRatio).get());
      ASSERT_TRUE(eg.getDouble(UnitarySystemPerformance_MultispeedExtensibleFields::CoolingSpeedSupplyAirFlowRatio));
      EXPECT_EQ(0.3, eg.getDouble(UnitarySystemPerformance_MultispeedExtensibleFields::CoolingSpeedSupplyAirFlowRatio).get());
    }
    {
      const IdfExtensibleGroup& eg = egs[1];
      ASSERT_TRUE(eg.getDouble(UnitarySystemPerformance_MultispeedExtensibleFields::HeatingSpeedSupplyAirFlowRatio));
      EXPECT_EQ(0.2, eg.getDouble(UnitarySystemPerformance_MultispeedExtensibleFields::HeatingSpeedSupplyAirFlowRatio).get());
      ASSERT_TRUE(eg.getDouble(UnitarySystemPerformance_MultispeedExtensibleFields::CoolingSpeedSupplyAirFlowRatio));
      EXPECT_EQ(1, eg.getDouble(UnitarySystemPerformance_MultispeedExtensibleFields::CoolingSpeedSupplyAirFlowRatio).get());
    }
  }

  {  // h < c
    Model m;

    const int maxHeatingStages = 2;
    const int maxCoolingStages = 4;
    const int maxStages = std::max(maxHeatingStages, maxCoolingStages);

    CoilHeatingDXMultiSpeed htgcoil(m);
    for (int i = 1; i <= maxHeatingStages; ++i) {
      CoilHeatingDXMultiSpeedStageData htgstage(m);
      EXPECT_TRUE(htgstage.setRatedAirFlowRate(i));
      EXPECT_TRUE(htgcoil.addStage(htgstage));
    }

    CoilCoolingDXMultiSpeed clgcoil(m);
    for (int i = 1; i <= maxCoolingStages; ++i) {
      CoilCoolingDXMultiSpeedStageData clgstage(m);
      EXPECT_TRUE(clgstage.setRatedAirFlowRate(i + maxHeatingStages));
      EXPECT_TRUE(clgcoil.addStage(clgstage));
    }

    AirLoopHVACUnitarySystem unitary(m);
    unitary.setHeatingCoil(htgcoil);
    unitary.setCoolingCoil(clgcoil);
    unitary.setSupplyAirFlowRateDuringCoolingOperation(10);
    unitary.setSupplyAirFlowRateDuringHeatingOperation(10);

    AirLoopHVAC airLoop(m);
    Node supplyOutletNode = airLoop.supplyOutletNode();
    unitary.addToNode(supplyOutletNode);

    ForwardTranslator ft;
    Workspace w = ft.translateModel(m);

    const auto idf_unitarys = w.getObjectsByType(IddObjectType::AirLoopHVAC_UnitarySystem);
    EXPECT_EQ(1, idf_unitarys.size());
    const auto& idf_unitary = idf_unitarys.front();

    EXPECT_EQ("UnitarySystemPerformance:Multispeed",
              idf_unitary.getString(AirLoopHVAC_UnitarySystemFields::DesignSpecificationMultispeedObjectType).get());
    EXPECT_EQ(unitary.nameString() + " Unitary System Performance",
              idf_unitary.getString(AirLoopHVAC_UnitarySystemFields::DesignSpecificationMultispeedObjectName).get());

    const auto idf_perfs = w.getObjectsByType(IddObjectType::UnitarySystemPerformance_Multispeed);
    EXPECT_EQ(1, idf_perfs.size());
    const WorkspaceObject& idf_perf = idf_perfs.front();

    EXPECT_EQ(maxHeatingStages, idf_perf.getInt(UnitarySystemPerformance_MultispeedFields::NumberofSpeedsforHeating).get());
    EXPECT_EQ(maxCoolingStages, idf_perf.getInt(UnitarySystemPerformance_MultispeedFields::NumberofSpeedsforCooling).get());
    EXPECT_EQ("No", idf_perf.getString(UnitarySystemPerformance_MultispeedFields::SingleModeOperation).get());
    EXPECT_TRUE(idf_perf.isEmpty(UnitarySystemPerformance_MultispeedFields::NoLoadSupplyAirFlowRateRatio));

    ASSERT_EQ(maxStages, idf_perf.numExtensibleGroups());

    const auto egs = idf_perf.extensibleGroups();

    {
      const IdfExtensibleGroup& eg = egs[0];
      ASSERT_TRUE(eg.getDouble(UnitarySystemPerformance_MultispeedExtensibleFields::HeatingSpeedSupplyAirFlowRatio));
      EXPECT_EQ(0.1, eg.getDouble(UnitarySystemPerformance_MultispeedExtensibleFields::HeatingSpeedSupplyAirFlowRatio).get());
      ASSERT_TRUE(eg.getDouble(UnitarySystemPerformance_MultispeedExtensibleFields::CoolingSpeedSupplyAirFlowRatio));
      EXPECT_EQ(0.3, eg.getDouble(UnitarySystemPerformance_MultispeedExtensibleFields::CoolingSpeedSupplyAirFlowRatio).get());
    }

    {
      const IdfExtensibleGroup& eg = egs[1];
      ASSERT_TRUE(eg.getDouble(UnitarySystemPerformance_MultispeedExtensibleFields::HeatingSpeedSupplyAirFlowRatio));
      EXPECT_EQ(0.2, eg.getDouble(UnitarySystemPerformance_MultispeedExtensibleFields::HeatingSpeedSupplyAirFlowRatio).get());
      ASSERT_TRUE(eg.getDouble(UnitarySystemPerformance_MultispeedExtensibleFields::CoolingSpeedSupplyAirFlowRatio));
      EXPECT_EQ(0.4, eg.getDouble(UnitarySystemPerformance_MultispeedExtensibleFields::CoolingSpeedSupplyAirFlowRatio).get());
    }
    {
      const IdfExtensibleGroup& eg = egs[2];
      ASSERT_TRUE(eg.getDouble(UnitarySystemPerformance_MultispeedExtensibleFields::HeatingSpeedSupplyAirFlowRatio));
      EXPECT_EQ(1.0, eg.getDouble(UnitarySystemPerformance_MultispeedExtensibleFields::HeatingSpeedSupplyAirFlowRatio).get());
      ASSERT_TRUE(eg.getDouble(UnitarySystemPerformance_MultispeedExtensibleFields::CoolingSpeedSupplyAirFlowRatio));
      EXPECT_EQ(0.5, eg.getDouble(UnitarySystemPerformance_MultispeedExtensibleFields::CoolingSpeedSupplyAirFlowRatio).get());
    }
    {
      const IdfExtensibleGroup& eg = egs[3];
      ASSERT_TRUE(eg.getDouble(UnitarySystemPerformance_MultispeedExtensibleFields::HeatingSpeedSupplyAirFlowRatio));
      EXPECT_EQ(1.0, eg.getDouble(UnitarySystemPerformance_MultispeedExtensibleFields::HeatingSpeedSupplyAirFlowRatio).get());
      ASSERT_TRUE(eg.getDouble(UnitarySystemPerformance_MultispeedExtensibleFields::CoolingSpeedSupplyAirFlowRatio));
      EXPECT_EQ(0.6, eg.getDouble(UnitarySystemPerformance_MultispeedExtensibleFields::CoolingSpeedSupplyAirFlowRatio).get());
    }
  }
}
