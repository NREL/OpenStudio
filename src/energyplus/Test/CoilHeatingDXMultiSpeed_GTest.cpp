/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>
#include "EnergyPlusFixture.hpp"

#include "../ForwardTranslator.hpp"
// #include "../ReverseTranslator.hpp"

#include "../../model/CoilHeatingDXMultiSpeed.hpp"
#include "../../model/CoilHeatingDXMultiSpeedStageData.hpp"

#include "../../model/Model.hpp"
#include "../../model/AirLoopHVAC.hpp"
#include "../../model/AirLoopHVACUnitarySystem.hpp"
#include "../../model/UnitarySystemPerformanceMultispeed.hpp"

#include "../../model/Node.hpp"
#include "../../model/Curve.hpp"
#include "../../model/CurveBiquadratic.hpp"
#include "../../model/CurveLinear.hpp"
#include "../../model/ScheduleConstant.hpp"

#include "../../utilities/idf/Workspace.hpp"
#include "../../utilities/idf/IdfObject.hpp"
#include "../../utilities/idf/IdfExtensibleGroup.hpp"
#include "../../utilities/idf/WorkspaceObject.hpp"
#include "../../utilities/idf/WorkspaceExtensibleGroup.hpp"
#include "../../utilities/core/Compare.hpp"

#include <utilities/idd/Coil_Heating_DX_MultiSpeed_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include <resources.hxx>
#include <sstream>

using namespace openstudio::energyplus;
using namespace openstudio::model;
using namespace openstudio;

TEST_F(EnergyPlusFixture, ForwardTranslator_CoilHeatingDXMultiSpeed_Basic) {

  Model m;
  CoilHeatingDXMultiSpeed coil(m);

  CoilHeatingDXMultiSpeedStageData stage1(m);
  coil.addStage(stage1);
  EXPECT_TRUE(stage1.setGrossRatedHeatingCapacity(10000));
  EXPECT_TRUE(stage1.setGrossRatedHeatingCOP(3.1));
  EXPECT_TRUE(stage1.setRatedAirFlowRate(1.1));
  EXPECT_TRUE(stage1.setRatedSupplyAirFanPowerPerVolumeFlowRate(770.1));
  EXPECT_TRUE(stage1.setRatedWasteHeatFractionofPowerInput(0.3));

  CoilHeatingDXMultiSpeedStageData stage2(m);
  coil.addStage(stage2);

  EXPECT_TRUE(coil.setName("Coil Heating DX Multi Speed"));

  ScheduleConstant availSch(m);
  availSch.setName("Coil Avail Sch");
  EXPECT_TRUE(coil.setAvailabilitySchedule(availSch));

  EXPECT_TRUE(coil.setMinimumOutdoorDryBulbTemperatureforCompressorOperation(-7.0));
  EXPECT_TRUE(coil.setOutdoorDryBulbTemperaturetoTurnOnCompressor(12.0));
  EXPECT_TRUE(coil.setCrankcaseHeaterCapacity(105.0));
  CurveLinear crankCurve(m);
  crankCurve.setName("CrankHeatCapFT");
  EXPECT_TRUE(coil.setCrankcaseHeaterCapacityFunctionofTemperatureCurve(crankCurve));
  EXPECT_TRUE(coil.setMaximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation(9.0));

  CurveBiquadratic defrostEIRFT(m);
  defrostEIRFT.setName("Defrost EIR FT");

  EXPECT_TRUE(coil.setDefrostEnergyInputRatioFunctionofTemperatureCurve(defrostEIRFT));
  EXPECT_TRUE(coil.setMaximumOutdoorDryBulbTemperatureforDefrostOperation(6.0));
  EXPECT_TRUE(coil.setDefrostStrategy("Resistive"));
  EXPECT_TRUE(coil.setDefrostControl("OnDemand"));
  EXPECT_TRUE(coil.setDefrostTimePeriodFraction(0.08));
  EXPECT_TRUE(coil.setResistiveDefrostHeaterCapacity(20.5));
  EXPECT_TRUE(coil.setApplyPartLoadFractiontoSpeedsGreaterthan1(false));
  EXPECT_TRUE(coil.setFuelType("NaturalGas"));
  EXPECT_TRUE(coil.setRegionnumberforCalculatingHSPF(4));

  // The coil can only be wrapped in E+ in AirLoopHVAC:UnitaryHeatPump:AirToAir:Multispeed
  // In OpenStudio, you can use UnitarySystem, the FT does the job
  AirLoopHVACUnitarySystem unitary(m);
  unitary.setHeatingCoil(coil);

  UnitarySystemPerformanceMultispeed perf(m);
  unitary.setDesignSpecificationMultispeedObject(perf);

  AirLoopHVAC airLoop(m);
  Node supplyOutletNode = airLoop.supplyOutletNode();
  unitary.addToNode(supplyOutletNode);

  ASSERT_TRUE(unitary.inletNode());
  EXPECT_TRUE(unitary.inletNode()->setName("Inlet Node"));
  ASSERT_TRUE(unitary.outletNode());
  EXPECT_TRUE(unitary.outletNode()->setName("Outlet Node"));

  //translate the model to EnergyPlus
  ForwardTranslator ft;
  const Workspace workspace = ft.translateModel(m);

  auto idfs_coils = workspace.getObjectsByType(IddObjectType::Coil_Heating_DX_MultiSpeed);
  ASSERT_EQ(1, idfs_coils.size());
  const WorkspaceObject& idf_coil = idfs_coils[0];

  EXPECT_EQ("Coil Heating DX Multi Speed", idf_coil.getString(Coil_Heating_DX_MultiSpeedFields::Name).get());
  EXPECT_EQ("Coil Avail Sch", idf_coil.getString(Coil_Heating_DX_MultiSpeedFields::AvailabilityScheduleName).get());
  EXPECT_EQ("Inlet Node", idf_coil.getString(Coil_Heating_DX_MultiSpeedFields::AirInletNodeName).get());
  EXPECT_EQ("Outlet Node", idf_coil.getString(Coil_Heating_DX_MultiSpeedFields::AirOutletNodeName).get());
  EXPECT_EQ(-7.0, idf_coil.getDouble(Coil_Heating_DX_MultiSpeedFields::MinimumOutdoorDryBulbTemperatureforCompressorOperation).get());
  EXPECT_EQ(12.0, idf_coil.getDouble(Coil_Heating_DX_MultiSpeedFields::OutdoorDryBulbTemperaturetoTurnOnCompressor).get());
  EXPECT_EQ(105.0, idf_coil.getDouble(Coil_Heating_DX_MultiSpeedFields::CrankcaseHeaterCapacity).get());
  EXPECT_EQ("CrankHeatCapFT", idf_coil.getString(Coil_Heating_DX_MultiSpeedFields::CrankcaseHeaterCapacityFunctionofTemperatureCurveName).get());
  EXPECT_EQ(9.0, idf_coil.getDouble(Coil_Heating_DX_MultiSpeedFields::MaximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation).get());
  EXPECT_EQ("Defrost EIR FT", idf_coil.getString(Coil_Heating_DX_MultiSpeedFields::DefrostEnergyInputRatioFunctionofTemperatureCurveName).get());
  EXPECT_EQ(6.0, idf_coil.getDouble(Coil_Heating_DX_MultiSpeedFields::MaximumOutdoorDryBulbTemperatureforDefrostOperation).get());
  EXPECT_EQ("Resistive", idf_coil.getString(Coil_Heating_DX_MultiSpeedFields::DefrostStrategy).get());
  EXPECT_EQ("OnDemand", idf_coil.getString(Coil_Heating_DX_MultiSpeedFields::DefrostControl).get());
  EXPECT_EQ(0.08, idf_coil.getDouble(Coil_Heating_DX_MultiSpeedFields::DefrostTimePeriodFraction).get());
  EXPECT_EQ(20.5, idf_coil.getDouble(Coil_Heating_DX_MultiSpeedFields::ResistiveDefrostHeaterCapacity).get());
  EXPECT_EQ("No", idf_coil.getString(Coil_Heating_DX_MultiSpeedFields::ApplyPartLoadFractiontoSpeedsGreaterthan1).get());
  EXPECT_EQ("NaturalGas", idf_coil.getString(Coil_Heating_DX_MultiSpeedFields::FuelType).get());
  EXPECT_EQ(4, idf_coil.getInt(Coil_Heating_DX_MultiSpeedFields::RegionnumberforCalculatingHSPF).get());

  EXPECT_EQ(2, idf_coil.getInt(Coil_Heating_DX_MultiSpeedFields::NumberofSpeeds).get());
  ASSERT_EQ(2, idf_coil.numExtensibleGroups());
  auto egs = idf_coil.extensibleGroups();
  auto stages = coil.stages();
  for (size_t i = 0; i < idf_coil.extensibleGroups().size(); ++i) {
    const IdfExtensibleGroup& eg = egs[i];
    const auto stage = stages[i];
    if (stage.isGrossRatedHeatingCapacityAutosized()) {
      EXPECT_TRUE(
        openstudio::istringEqual("Autosize", eg.getString(Coil_Heating_DX_MultiSpeedExtensibleFields::SpeedGrossRatedHeatingCapacity).get()));
    } else {
      EXPECT_EQ(stage.grossRatedHeatingCapacity().get(),
                eg.getDouble(Coil_Heating_DX_MultiSpeedExtensibleFields::SpeedGrossRatedHeatingCapacity).get());
    }

    EXPECT_EQ(stage.grossRatedHeatingCOP(), eg.getDouble(Coil_Heating_DX_MultiSpeedExtensibleFields::SpeedGrossRatedHeatingCOP).get());
    if (stage.isRatedAirFlowRateAutosized()) {
      EXPECT_TRUE(openstudio::istringEqual("Autosize", eg.getString(Coil_Heating_DX_MultiSpeedExtensibleFields::SpeedRatedAirFlowRate).get()));
    } else {
      EXPECT_EQ(stage.ratedAirFlowRate().get(), eg.getDouble(Coil_Heating_DX_MultiSpeedExtensibleFields::SpeedRatedAirFlowRate).get());
    }

    EXPECT_EQ(stage.ratedSupplyAirFanPowerPerVolumeFlowRate2017(),
              eg.getDouble(Coil_Heating_DX_MultiSpeedExtensibleFields::SpeedRatedSupplyAirFanPowerPerVolumeFlowRate2017).get());
    EXPECT_EQ(stage.ratedSupplyAirFanPowerPerVolumeFlowRate2023(),
              eg.getDouble(Coil_Heating_DX_MultiSpeedExtensibleFields::SpeedRatedSupplyAirFanPowerPerVolumeFlowRate2023).get());
    EXPECT_EQ(stage.heatingCapacityFunctionofTemperatureCurve().nameString(),
              eg.getString(Coil_Heating_DX_MultiSpeedExtensibleFields::SpeedHeatingCapacityFunctionofTemperatureCurveName).get());
    EXPECT_EQ(stage.heatingCapacityFunctionofFlowFractionCurve().nameString(),
              eg.getString(Coil_Heating_DX_MultiSpeedExtensibleFields::SpeedHeatingCapacityFunctionofFlowFractionCurveName).get());
    EXPECT_EQ(stage.energyInputRatioFunctionofTemperatureCurve().nameString(),
              eg.getString(Coil_Heating_DX_MultiSpeedExtensibleFields::SpeedEnergyInputRatioFunctionofTemperatureCurveName).get());
    EXPECT_EQ(stage.energyInputRatioFunctionofFlowFractionCurve().nameString(),
              eg.getString(Coil_Heating_DX_MultiSpeedExtensibleFields::SpeedEnergyInputRatioFunctionofFlowFractionCurveName).get());
    EXPECT_EQ(stage.partLoadFractionCorrelationCurve().nameString(),
              eg.getString(Coil_Heating_DX_MultiSpeedExtensibleFields::SpeedPartLoadFractionCorrelationCurveName).get());
    EXPECT_EQ(stage.ratedWasteHeatFractionofPowerInput(),
              eg.getDouble(Coil_Heating_DX_MultiSpeedExtensibleFields::SpeedRatedWasteHeatFractionofPowerInput).get());
    EXPECT_EQ(stage.wasteHeatFunctionofTemperatureCurve().nameString(),
              eg.getString(Coil_Heating_DX_MultiSpeedExtensibleFields::SpeedWasteHeatFunctionofTemperatureCurveName).get());
  }
}

/* It should allow for up to 4 stages. Tests that issue #2812 is fixed and doesn't come back */
TEST_F(EnergyPlusFixture, ForwardTranslator_CoilHeatingDXMultiSpeed_4Stages) {

  Model m;
  CoilHeatingDXMultiSpeed coil(m);

  CoilHeatingDXMultiSpeedStageData stage1(m);
  coil.addStage(stage1);

  CoilHeatingDXMultiSpeedStageData stage2(m);
  coil.addStage(stage2);

  CoilHeatingDXMultiSpeedStageData stage3(m);
  coil.addStage(stage3);

  CoilHeatingDXMultiSpeedStageData stage4(m);
  coil.addStage(stage4);

  ASSERT_EQ(4u, coil.stages().size());

  // The coil can only be wrapped in E+ in AirLoopHVAC:UnitaryHeatPump:AirToAir:Multispeed
  // In OpenStudio, you can use UnitarySystem, the FT does the job
  AirLoopHVACUnitarySystem unitary(m);
  unitary.setHeatingCoil(coil);

  UnitarySystemPerformanceMultispeed perf(m);
  unitary.setDesignSpecificationMultispeedObject(perf);

  AirLoopHVAC airLoop(m);
  Node supplyOutletNode = airLoop.supplyOutletNode();
  unitary.addToNode(supplyOutletNode);

  //translate the model to EnergyPlus
  ForwardTranslator ft;
  Workspace workspace = ft.translateModel(m);

  EXPECT_EQ(1u, workspace.getObjectsByType(IddObjectType::Coil_Heating_DX_MultiSpeed).size());
  WorkspaceObject idf_coil = workspace.getObjectsByType(IddObjectType::Coil_Heating_DX_MultiSpeed)[0];

  // Check that there are indeed 4 stages, both the NumberofSpeeds field (int) and the number of extensible groups
  ASSERT_EQ(4u, idf_coil.getInt(Coil_Heating_DX_MultiSpeedFields::NumberofSpeeds).get());
  ASSERT_EQ(4u, idf_coil.extensibleGroups().size());

  // WorkspaceExtensibleGroup eg = idf_coil.extensibleGroups()[0] or getExtensibleGroup(0);

  ASSERT_EQ(
    idf_coil.getExtensibleGroup(0).getString(Coil_Heating_DX_MultiSpeedExtensibleFields::SpeedHeatingCapacityFunctionofTemperatureCurveName).get(),
    stage1.heatingCapacityFunctionofTemperatureCurve().name().get());

  ASSERT_EQ(
    idf_coil.getExtensibleGroup(1).getString(Coil_Heating_DX_MultiSpeedExtensibleFields::SpeedHeatingCapacityFunctionofTemperatureCurveName).get(),
    stage2.heatingCapacityFunctionofTemperatureCurve().name().get());

  ASSERT_EQ(
    idf_coil.getExtensibleGroup(2).getString(Coil_Heating_DX_MultiSpeedExtensibleFields::SpeedHeatingCapacityFunctionofTemperatureCurveName).get(),
    stage3.heatingCapacityFunctionofTemperatureCurve().name().get());

  ASSERT_EQ(
    idf_coil.getExtensibleGroup(3).getString(Coil_Heating_DX_MultiSpeedExtensibleFields::SpeedHeatingCapacityFunctionofTemperatureCurveName).get(),
    stage4.heatingCapacityFunctionofTemperatureCurve().name().get());
}

TEST_F(EnergyPlusFixture, ForwardTranslator_CoilHeatingDXMultiSpeed_MinOATCompressor) {
  Model m;

  CoilHeatingDXMultiSpeed coil(m);
  coil.setMinimumOutdoorDryBulbTemperatureforCompressorOperation(-7.5);

  // Won't be translated unless assigned
  AirLoopHVACUnitarySystem unitary(m);
  unitary.setHeatingCoil(coil);

  UnitarySystemPerformanceMultispeed perf(m);
  unitary.setDesignSpecificationMultispeedObject(perf);

  AirLoopHVAC airLoop(m);
  Node supplyOutletNode = airLoop.supplyOutletNode();
  unitary.addToNode(supplyOutletNode);

  ForwardTranslator ft;
  Workspace w = ft.translateModel(m);

  WorkspaceObjectVector idf_coils(w.getObjectsByType(IddObjectType::Coil_Heating_DX_MultiSpeed));
  ASSERT_EQ(1u, idf_coils.size());
  WorkspaceObject idf_coil(idf_coils[0]);

  auto _d = idf_coil.getDouble(Coil_Heating_DX_MultiSpeedFields::MinimumOutdoorDryBulbTemperatureforCompressorOperation);
  ASSERT_TRUE(_d);
  EXPECT_DOUBLE_EQ(-7.5, _d.get());
}
