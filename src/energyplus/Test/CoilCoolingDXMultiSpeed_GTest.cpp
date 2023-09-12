/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>
#include "EnergyPlusFixture.hpp"

#include "../ForwardTranslator.hpp"
// #include "../ReverseTranslator.hpp"

#include "../../model/CoilCoolingDXMultiSpeed.hpp"
#include "../../model/CoilCoolingDXMultiSpeedStageData.hpp"

#include "../../model/Model.hpp"
#include "../../model/AirLoopHVAC.hpp"
#include "../../model/AirLoopHVACUnitarySystem.hpp"
#include "../../model/UnitarySystemPerformanceMultispeed.hpp"

#include "../../model/Node.hpp"
#include "../../model/Curve.hpp"
#include "../../model/CurveLinear.hpp"
#include "../../model/CurveBiquadratic.hpp"
#include "../../model/ScheduleConstant.hpp"

#include "../../utilities/idf/Workspace.hpp"
#include "../../utilities/idf/IdfObject.hpp"
#include "../../utilities/idf/IdfExtensibleGroup.hpp"
#include "../../utilities/idf/WorkspaceObject.hpp"
#include "../../utilities/idf/WorkspaceExtensibleGroup.hpp"
#include "../../utilities/core/Compare.hpp"

#include <utilities/idd/Coil_Cooling_DX_MultiSpeed_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include <resources.hxx>
#include <sstream>

using namespace openstudio::energyplus;
using namespace openstudio::model;
using namespace openstudio;

TEST_F(EnergyPlusFixture, ForwardTranslator_CoilCoolingDXMultiSpeed_Basic) {

  Model m;
  CoilCoolingDXMultiSpeed coil(m);

  CoilCoolingDXMultiSpeedStageData stage1(m);
  coil.addStage(stage1);

  EXPECT_TRUE(stage1.setGrossRatedTotalCoolingCapacity(10000.0));
  EXPECT_TRUE(stage1.setGrossRatedSensibleHeatRatio(0.85));
  EXPECT_TRUE(stage1.setGrossRatedCoolingCOP(3.1));
  EXPECT_TRUE(stage1.setRatedAirFlowRate(1.1));
  EXPECT_TRUE(stage1.setRatedEvaporatorFanPowerPerVolumeFlowRate(770.1));
  EXPECT_TRUE(stage1.setNominalTimeforCondensateRemovaltoBegin(60.0));
  EXPECT_TRUE(stage1.setRatioofInitialMoistureEvaporationRateandSteadyStateLatentCapacity(0.1));
  EXPECT_TRUE(stage1.setMaximumCyclingRate(4.0));
  EXPECT_TRUE(stage1.setLatentCapacityTimeConstant(0.1));
  EXPECT_TRUE(stage1.setRatedWasteHeatFractionofPowerInput(0.4));
  EXPECT_TRUE(stage1.setEvaporativeCondenserEffectiveness(0.85));
  EXPECT_TRUE(stage1.setEvaporativeCondenserAirFlowRate(3.0));
  EXPECT_TRUE(stage1.setRatedEvaporativeCondenserPumpPowerConsumption(100.1));

  // EXPECT_TRUE(stage1.setTotalCoolingCapacityFunctionofTemperatureCurve("Curve Biquadratic 1"));
  // EXPECT_TRUE(stage1.setTotalCoolingCapacityFunctionofFlowFractionCurve("Curve Quadratic 1"));
  // EXPECT_TRUE(stage1.setEnergyInputRatioFunctionofTemperatureCurve("Curve Biquadratic 2"));
  // EXPECT_TRUE(stage1.setEnergyInputRatioFunctionofFlowFractionCurve("Curve Quadratic 2"));
  // EXPECT_TRUE(stage1.setPartLoadFractionCorrelationCurve("Curve Quadratic 3"));
  // EXPECT_TRUE(stage1.setWasteHeatFunctionofTemperatureCurve("Curve Biquadratic 3"));

  CoilCoolingDXMultiSpeedStageData stage2(m);
  coil.addStage(stage2);

  EXPECT_TRUE(coil.setName("Coil Cooling DX Multi Speed"));

  ScheduleConstant availSch(m);
  availSch.setName("Coil Avail Sch");

  EXPECT_TRUE(coil.setAvailabilitySchedule(availSch));
  EXPECT_TRUE(coil.setCondenserType("EvaporativelyCooled"));
  EXPECT_TRUE(coil.setMinimumOutdoorDryBulbTemperatureforCompressorOperation(-7.5));
  // EXPECT_TRUE(coil.setSupplyWaterStorageTank(""));
  // EXPECT_TRUE(coil.setCondensateCollectionWaterStorageTank(""));
  EXPECT_TRUE(coil.setApplyPartLoadFractiontoSpeedsGreaterthan1(false));
  EXPECT_TRUE(coil.setApplyLatentDegradationtoSpeedsGreaterthan1(true));
  EXPECT_TRUE(coil.setCrankcaseHeaterCapacity(105.0));
  CurveLinear crankCurve(m);
  crankCurve.setName("CrankHeatCapFT");
  EXPECT_TRUE(coil.setCrankcaseHeaterCapacityFunctionofTemperatureCurve(crankCurve));
  EXPECT_TRUE(coil.setMaximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation(9.0));
  EXPECT_TRUE(coil.setBasinHeaterCapacity(0.0));
  EXPECT_TRUE(coil.setBasinHeaterSetpointTemperature(2.0));

  ScheduleConstant basinSch(m);
  basinSch.setName("Basin Heater Sch");
  EXPECT_TRUE(coil.setBasinHeaterOperatingSchedule(basinSch));

  EXPECT_TRUE(coil.setFuelType("NaturalGas"));

  // The coil can only be wrapped in E+ in AirLoopHVAC:UnitaryHeatPump:AirToAir:Multispeed
  // In OpenStudio, you can use UnitarySystem, the FT does the job
  AirLoopHVACUnitarySystem unitary(m);
  unitary.setCoolingCoil(coil);

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
  Workspace workspace = ft.translateModel(m);

  ASSERT_EQ(1, workspace.getObjectsByType(IddObjectType::Coil_Cooling_DX_MultiSpeed).size());
  WorkspaceObject idf_coil = workspace.getObjectsByType(IddObjectType::Coil_Cooling_DX_MultiSpeed)[0];

  EXPECT_EQ("Coil Cooling DX Multi Speed", idf_coil.getString(Coil_Cooling_DX_MultiSpeedFields::Name).get());
  EXPECT_EQ("Coil Avail Sch", idf_coil.getString(Coil_Cooling_DX_MultiSpeedFields::AvailabilityScheduleName).get());
  EXPECT_EQ("Inlet Node", idf_coil.getString(Coil_Cooling_DX_MultiSpeedFields::AirInletNodeName).get());
  EXPECT_EQ("Outlet Node", idf_coil.getString(Coil_Cooling_DX_MultiSpeedFields::AirOutletNodeName).get());
  EXPECT_EQ("", idf_coil.getString(Coil_Cooling_DX_MultiSpeedFields::CondenserAirInletNodeName).get());
  EXPECT_EQ("EvaporativelyCooled", idf_coil.getString(Coil_Cooling_DX_MultiSpeedFields::CondenserType).get());
  EXPECT_EQ(-7.5, idf_coil.getDouble(Coil_Cooling_DX_MultiSpeedFields::MinimumOutdoorDryBulbTemperatureforCompressorOperation).get());
  EXPECT_EQ("", idf_coil.getString(Coil_Cooling_DX_MultiSpeedFields::SupplyWaterStorageTankName).get());
  EXPECT_EQ("", idf_coil.getString(Coil_Cooling_DX_MultiSpeedFields::CondensateCollectionWaterStorageTankName).get());
  EXPECT_EQ("No", idf_coil.getString(Coil_Cooling_DX_MultiSpeedFields::ApplyPartLoadFractiontoSpeedsGreaterthan1).get());
  EXPECT_EQ("Yes", idf_coil.getString(Coil_Cooling_DX_MultiSpeedFields::ApplyLatentDegradationtoSpeedsGreaterthan1).get());
  EXPECT_EQ(105.0, idf_coil.getDouble(Coil_Cooling_DX_MultiSpeedFields::CrankcaseHeaterCapacity).get());
  EXPECT_EQ("CrankHeatCapFT", idf_coil.getString(Coil_Cooling_DX_MultiSpeedFields::CrankcaseHeaterCapacityFunctionofTemperatureCurveName).get());
  EXPECT_EQ(9.0, idf_coil.getDouble(Coil_Cooling_DX_MultiSpeedFields::MaximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation).get());
  EXPECT_EQ(0.0, idf_coil.getDouble(Coil_Cooling_DX_MultiSpeedFields::BasinHeaterCapacity).get());
  EXPECT_EQ(2.0, idf_coil.getDouble(Coil_Cooling_DX_MultiSpeedFields::BasinHeaterSetpointTemperature).get());
  EXPECT_EQ("Basin Heater Sch", idf_coil.getString(Coil_Cooling_DX_MultiSpeedFields::BasinHeaterOperatingScheduleName).get());
  EXPECT_EQ("NaturalGas", idf_coil.getString(Coil_Cooling_DX_MultiSpeedFields::FuelType).get());

  EXPECT_EQ(2, idf_coil.getInt(Coil_Cooling_DX_MultiSpeedFields::NumberofSpeeds).get());
  ASSERT_EQ(2, idf_coil.numExtensibleGroups());
  auto egs = idf_coil.extensibleGroups();
  auto stages = coil.stages();
  for (size_t i = 0; i < idf_coil.extensibleGroups().size(); ++i) {
    const IdfExtensibleGroup& eg = egs[i];
    const auto stage = stages[i];

    if (stage.isGrossRatedTotalCoolingCapacityAutosized()) {
      EXPECT_TRUE(
        openstudio::istringEqual("Autosize", eg.getString(Coil_Cooling_DX_MultiSpeedExtensibleFields::SpeedGrossRatedTotalCoolingCapacity).get()));
    } else {
      EXPECT_EQ(stage.grossRatedTotalCoolingCapacity().get(),
                eg.getDouble(Coil_Cooling_DX_MultiSpeedExtensibleFields::SpeedGrossRatedTotalCoolingCapacity).get());
    }

    if (stage.isGrossRatedSensibleHeatRatioAutosized()) {
      EXPECT_TRUE(
        openstudio::istringEqual("Autosize", eg.getString(Coil_Cooling_DX_MultiSpeedExtensibleFields::SpeedGrossRatedSensibleHeatRatio).get()));
    } else {
      EXPECT_EQ(stage.grossRatedSensibleHeatRatio().get(),
                eg.getDouble(Coil_Cooling_DX_MultiSpeedExtensibleFields::SpeedGrossRatedSensibleHeatRatio).get());
    }

    if (stage.isRatedAirFlowRateAutosized()) {
      EXPECT_TRUE(openstudio::istringEqual("Autosize", eg.getString(Coil_Cooling_DX_MultiSpeedExtensibleFields::SpeedRatedAirFlowRate).get()));
    } else {
      EXPECT_EQ(stage.ratedAirFlowRate().get(), eg.getDouble(Coil_Cooling_DX_MultiSpeedExtensibleFields::SpeedRatedAirFlowRate).get());
    }

    if (stage.isEvaporativeCondenserAirFlowRateAutosized()) {
      EXPECT_TRUE(
        openstudio::istringEqual("Autosize", eg.getString(Coil_Cooling_DX_MultiSpeedExtensibleFields::SpeedEvaporativeCondenserAirFlowRate).get()));
    } else {
      EXPECT_EQ(stage.evaporativeCondenserAirFlowRate().get(),
                eg.getDouble(Coil_Cooling_DX_MultiSpeedExtensibleFields::SpeedEvaporativeCondenserAirFlowRate).get());
    }

    if (stage.isRatedEvaporativeCondenserPumpPowerConsumptionAutosized()) {
      EXPECT_TRUE(openstudio::istringEqual(
        "Autosize", eg.getString(Coil_Cooling_DX_MultiSpeedExtensibleFields::SpeedRatedEvaporativeCondenserPumpPowerConsumption).get()));
    } else {
      EXPECT_EQ(stage.ratedEvaporativeCondenserPumpPowerConsumption().get(),
                eg.getDouble(Coil_Cooling_DX_MultiSpeedExtensibleFields::SpeedRatedEvaporativeCondenserPumpPowerConsumption).get());
    }

    EXPECT_EQ(stage.grossRatedCoolingCOP(), eg.getDouble(Coil_Cooling_DX_MultiSpeedExtensibleFields::SpeedGrossRatedCoolingCOP).get());
    EXPECT_EQ(stage.ratedEvaporatorFanPowerPerVolumeFlowRate2017(),
              eg.getDouble(Coil_Cooling_DX_MultiSpeedExtensibleFields::SpeedRatedEvaporatorFanPowerPerVolumeFlowRate2017).get());
    EXPECT_EQ(stage.ratedEvaporatorFanPowerPerVolumeFlowRate2023(),
              eg.getDouble(Coil_Cooling_DX_MultiSpeedExtensibleFields::SpeedRatedEvaporatorFanPowerPerVolumeFlowRate2023).get());
    EXPECT_EQ(stage.totalCoolingCapacityFunctionofTemperatureCurve().nameString(),
              eg.getString(Coil_Cooling_DX_MultiSpeedExtensibleFields::SpeedTotalCoolingCapacityFunctionofTemperatureCurveName).get());
    EXPECT_EQ(stage.totalCoolingCapacityFunctionofFlowFractionCurve().nameString(),
              eg.getString(Coil_Cooling_DX_MultiSpeedExtensibleFields::SpeedTotalCoolingCapacityFunctionofFlowFractionCurveName).get());
    EXPECT_EQ(stage.energyInputRatioFunctionofTemperatureCurve().nameString(),
              eg.getString(Coil_Cooling_DX_MultiSpeedExtensibleFields::SpeedEnergyInputRatioFunctionofTemperatureCurveName).get());
    EXPECT_EQ(stage.energyInputRatioFunctionofFlowFractionCurve().nameString(),
              eg.getString(Coil_Cooling_DX_MultiSpeedExtensibleFields::SpeedEnergyInputRatioFunctionofFlowFractionCurveName).get());
    EXPECT_EQ(stage.partLoadFractionCorrelationCurve().nameString(),
              eg.getString(Coil_Cooling_DX_MultiSpeedExtensibleFields::SpeedPartLoadFractionCorrelationCurveName).get());
    EXPECT_EQ(stage.nominalTimeforCondensateRemovaltoBegin(),
              eg.getDouble(Coil_Cooling_DX_MultiSpeedExtensibleFields::SpeedNominalTimeforCondensateRemovaltoBegin).get());
    EXPECT_EQ(stage.ratioofInitialMoistureEvaporationRateandSteadyStateLatentCapacity(),
              eg.getDouble(Coil_Cooling_DX_MultiSpeedExtensibleFields::SpeedRatioofInitialMoistureEvaporationRateandSteadyStateLatentCapacity).get());
    EXPECT_EQ(stage.maximumCyclingRate(), eg.getDouble(Coil_Cooling_DX_MultiSpeedExtensibleFields::SpeedMaximumCyclingRate).get());
    EXPECT_EQ(stage.latentCapacityTimeConstant(), eg.getDouble(Coil_Cooling_DX_MultiSpeedExtensibleFields::SpeedLatentCapacityTimeConstant).get());
    EXPECT_EQ(stage.ratedWasteHeatFractionofPowerInput(),
              eg.getDouble(Coil_Cooling_DX_MultiSpeedExtensibleFields::SpeedRatedWasteHeatFractionofPowerInput).get());
    EXPECT_EQ(stage.wasteHeatFunctionofTemperatureCurve().nameString(),
              eg.getString(Coil_Cooling_DX_MultiSpeedExtensibleFields::SpeedWasteHeatFunctionofTemperatureCurveName).get());
    EXPECT_EQ(stage.evaporativeCondenserEffectiveness(),
              eg.getDouble(Coil_Cooling_DX_MultiSpeedExtensibleFields::SpeedEvaporativeCondenserEffectiveness).get());
  }
}

/* It should allow for up to 4 stages. Tests that issue #2812 is fixed and doesn't come back */
TEST_F(EnergyPlusFixture, ForwardTranslator_CoilCoolingDXMultiSpeed_4Stages) {

  Model m;
  CoilCoolingDXMultiSpeed coil(m);

  CoilCoolingDXMultiSpeedStageData stage1(m);
  coil.addStage(stage1);

  CoilCoolingDXMultiSpeedStageData stage2(m);
  coil.addStage(stage2);

  CoilCoolingDXMultiSpeedStageData stage3(m);
  coil.addStage(stage3);

  CoilCoolingDXMultiSpeedStageData stage4(m);
  coil.addStage(stage4);

  ASSERT_EQ(4u, coil.stages().size());

  // The coil can only be wrapped in E+ in AirLoopHVAC:UnitaryHeatPump:AirToAir:Multispeed
  // In OpenStudio, you can use UnitarySystem, the FT does the job
  AirLoopHVACUnitarySystem unitary(m);
  unitary.setCoolingCoil(coil);

  UnitarySystemPerformanceMultispeed perf(m);
  unitary.setDesignSpecificationMultispeedObject(perf);

  AirLoopHVAC airLoop(m);
  Node supplyOutletNode = airLoop.supplyOutletNode();
  unitary.addToNode(supplyOutletNode);

  //translate the model to EnergyPlus
  ForwardTranslator ft;
  Workspace workspace = ft.translateModel(m);

  EXPECT_EQ(1u, workspace.getObjectsByType(IddObjectType::Coil_Cooling_DX_MultiSpeed).size());
  WorkspaceObject idf_coil = workspace.getObjectsByType(IddObjectType::Coil_Cooling_DX_MultiSpeed)[0];

  // Check that there are indeed 4 stages, both the NumberofSpeeds field (int) and the number of extensible groups
  ASSERT_EQ(4u, idf_coil.getInt(Coil_Cooling_DX_MultiSpeedFields::NumberofSpeeds).get());
  ASSERT_EQ(4u, idf_coil.extensibleGroups().size());

  // WorkspaceExtensibleGroup eg = idf_coil.extensibleGroups()[0] or getExtensibleGroup(0);

  ASSERT_EQ(idf_coil.getExtensibleGroup(0)
              .getString(Coil_Cooling_DX_MultiSpeedExtensibleFields::SpeedTotalCoolingCapacityFunctionofTemperatureCurveName)
              .get(),
            stage1.totalCoolingCapacityFunctionofTemperatureCurve().name().get());

  ASSERT_EQ(idf_coil.getExtensibleGroup(1)
              .getString(Coil_Cooling_DX_MultiSpeedExtensibleFields::SpeedTotalCoolingCapacityFunctionofTemperatureCurveName)
              .get(),
            stage2.totalCoolingCapacityFunctionofTemperatureCurve().name().get());

  ASSERT_EQ(idf_coil.getExtensibleGroup(2)
              .getString(Coil_Cooling_DX_MultiSpeedExtensibleFields::SpeedTotalCoolingCapacityFunctionofTemperatureCurveName)
              .get(),
            stage3.totalCoolingCapacityFunctionofTemperatureCurve().name().get());

  ASSERT_EQ(idf_coil.getExtensibleGroup(3)
              .getString(Coil_Cooling_DX_MultiSpeedExtensibleFields::SpeedTotalCoolingCapacityFunctionofTemperatureCurveName)
              .get(),
            stage4.totalCoolingCapacityFunctionofTemperatureCurve().name().get());
}

TEST_F(EnergyPlusFixture, ForwardTranslator_CoilCoolingDXMultiSpeed_MinOATCompressor) {
  Model m;

  CoilCoolingDXMultiSpeed coil(m);
  coil.setMinimumOutdoorDryBulbTemperatureforCompressorOperation(-7.5);

  // Won't be translated unless assigned
  AirLoopHVACUnitarySystem unitary(m);
  unitary.setCoolingCoil(coil);

  UnitarySystemPerformanceMultispeed perf(m);
  unitary.setDesignSpecificationMultispeedObject(perf);

  AirLoopHVAC airLoop(m);
  Node supplyOutletNode = airLoop.supplyOutletNode();
  unitary.addToNode(supplyOutletNode);

  ForwardTranslator ft;
  Workspace w = ft.translateModel(m);

  WorkspaceObjectVector idf_coils(w.getObjectsByType(IddObjectType::Coil_Cooling_DX_MultiSpeed));
  ASSERT_EQ(1u, idf_coils.size());
  WorkspaceObject idf_coil(idf_coils[0]);

  auto _d = idf_coil.getDouble(Coil_Cooling_DX_MultiSpeedFields::MinimumOutdoorDryBulbTemperatureforCompressorOperation);
  ASSERT_TRUE(_d);
  EXPECT_DOUBLE_EQ(-7.5, _d.get());
}
