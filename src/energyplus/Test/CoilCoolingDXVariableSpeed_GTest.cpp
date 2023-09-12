/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>
#include "EnergyPlusFixture.hpp"

#include "../ForwardTranslator.hpp"

#include "../../model/CoilCoolingDXVariableSpeed.hpp"
#include "../../model/CoilCoolingDXVariableSpeed_Impl.hpp"
#include "../../model/CoilCoolingDXVariableSpeedSpeedData.hpp"

#include "../../model/Model.hpp"
#include "../../model/AirLoopHVAC.hpp"
#include "../../model/AirLoopHVACUnitarySystem.hpp"
#include "../../model/CurveLinear.hpp"
#include "../../model/Node.hpp"
#include "../../model/ScheduleConstant.hpp"
#include "../../model/UnitarySystemPerformanceMultispeed.hpp"

#include "../../utilities/idf/Workspace.hpp"
#include "../../utilities/idf/IdfObject.hpp"
#include "../../utilities/idf/IdfExtensibleGroup.hpp"
#include "../../utilities/idf/WorkspaceObject.hpp"
#include "../../utilities/idf/WorkspaceObject_Impl.hpp"
#include "../../utilities/idf/WorkspaceExtensibleGroup.hpp"

#include <utilities/idd/Coil_Cooling_DX_VariableSpeed_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::energyplus;
using namespace openstudio::model;
using namespace openstudio;

TEST_F(EnergyPlusFixture, ForwardTranslator_CoilCoolingDXVariableSpeed) {

  Model m;
  CoilCoolingDXVariableSpeed coil(m);

  {
    CoilCoolingDXVariableSpeedSpeedData speed(m);
    speed.setName("DX VSD Speed 1");
    EXPECT_TRUE(speed.setName("DX VSD Speed 1"));
    EXPECT_TRUE(speed.setReferenceUnitGrossRatedTotalCoolingCapacity(4000.0));
    EXPECT_TRUE(speed.setReferenceUnitGrossRatedSensibleHeatRatio(0.75));
    EXPECT_TRUE(speed.setReferenceUnitGrossRatedCoolingCOP(4.0));
    EXPECT_TRUE(speed.setReferenceUnitRatedAirFlowRate(0.5));
    EXPECT_TRUE(speed.setReferenceUnitRatedCondenserAirFlowRate(0.8));
    EXPECT_TRUE(speed.setReferenceUnitRatedPadEffectivenessofEvapPrecooling(0.3));
    EXPECT_TRUE(speed.setRatedEvaporatorFanPowerPerVolumeFlowRate2017(817.0));
    EXPECT_TRUE(speed.setRatedEvaporatorFanPowerPerVolumeFlowRate2023(823.0));
    speed.totalCoolingCapacityFunctionofTemperatureCurve().setName("Speed1 CoolCapFT");
    speed.totalCoolingCapacityFunctionofAirFlowFractionCurve().setName("Speed1 TotCoolCapfFlow");
    speed.energyInputRatioFunctionofTemperatureCurve().setName("Speed1 EIRFT");
    speed.energyInputRatioFunctionofAirFlowFractionCurve().setName("Speed1 EIRfFlow");

    EXPECT_TRUE(coil.addSpeed(speed));
  }

  {
    CoilCoolingDXVariableSpeedSpeedData speed(m);
    speed.setName("DX VSD Speed 2");
    EXPECT_TRUE(speed.setName("DX VSD Speed 2"));
    EXPECT_TRUE(speed.setReferenceUnitGrossRatedTotalCoolingCapacity(8000.0));
    EXPECT_TRUE(speed.setReferenceUnitGrossRatedSensibleHeatRatio(0.79));
    EXPECT_TRUE(speed.setReferenceUnitGrossRatedCoolingCOP(3.5));
    EXPECT_TRUE(speed.setReferenceUnitRatedAirFlowRate(1.0));
    EXPECT_TRUE(speed.setReferenceUnitRatedCondenserAirFlowRate(1.2));
    EXPECT_TRUE(speed.setReferenceUnitRatedPadEffectivenessofEvapPrecooling(0.4));
    EXPECT_TRUE(speed.setRatedEvaporatorFanPowerPerVolumeFlowRate2017(927.0));
    EXPECT_TRUE(speed.setRatedEvaporatorFanPowerPerVolumeFlowRate2023(923.0));
    speed.totalCoolingCapacityFunctionofTemperatureCurve().setName("Speed2 CoolCapFT");
    speed.totalCoolingCapacityFunctionofAirFlowFractionCurve().setName("Speed2 TotCoolCapfFlow");
    speed.energyInputRatioFunctionofTemperatureCurve().setName("Speed2 EIRFT");
    speed.energyInputRatioFunctionofAirFlowFractionCurve().setName("Speed2 EIRfFlow");

    EXPECT_TRUE(coil.addSpeed(speed));
  }

  EXPECT_TRUE(coil.setName("Coil Cooling DX VSD"));

  EXPECT_TRUE(coil.setNominalSpeedLevel(2));
  EXPECT_TRUE(coil.setGrossRatedTotalCoolingCapacityAtSelectedNominalSpeedLevel(1000.6));
  EXPECT_TRUE(coil.setRatedAirFlowRateAtSelectedNominalSpeedLevel(0.7));
  EXPECT_TRUE(coil.setNominalTimeforCondensatetoBeginLeavingtheCoil(1.9));
  EXPECT_TRUE(coil.setInitialMoistureEvaporationRateDividedbySteadyStateACLatentCapacity(0.08));
  EXPECT_TRUE(coil.setMaximumCyclingRate(3.5));
  EXPECT_TRUE(coil.setLatentCapacityTimeConstant(65.0));
  EXPECT_TRUE(coil.setFanDelayTime(90.0));

  coil.energyPartLoadFractionCurve().setName("CoilPLFCurve");

  EXPECT_TRUE(coil.setCondenserType("EvaporativelyCooled"));

  EXPECT_TRUE(coil.setEvaporativeCondenserPumpRatedPowerConsumption(180.0));

  EXPECT_TRUE(coil.setCrankcaseHeaterCapacity(105.0));
  CurveLinear crankCurve(m);
  crankCurve.setName("CrankCoolCapFT");
  EXPECT_TRUE(coil.setCrankcaseHeaterCapacityFunctionofTemperatureCurve(crankCurve));
  EXPECT_TRUE(coil.setMaximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation(9.0));
  EXPECT_TRUE(coil.setMinimumOutdoorDryBulbTemperatureforCompressorOperation(-21.0));

  EXPECT_TRUE(coil.setBasinHeaterCapacity(560.0));
  EXPECT_TRUE(coil.setBasinHeaterSetpointTemperature(4.0));

  ScheduleConstant basinSch(m);
  basinSch.setName("Basin Heater Sch");
  EXPECT_TRUE(coil.setBasinHeaterOperatingSchedule(basinSch));

  // Need to be used to be translated
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
  const Workspace workspace = ft.translateModel(m);

  auto idfs_coils = workspace.getObjectsByType(IddObjectType::Coil_Cooling_DX_VariableSpeed);
  ASSERT_EQ(1, idfs_coils.size());
  const WorkspaceObject& idf_coil = idfs_coils[0];

  EXPECT_EQ("Coil Cooling DX VSD", idf_coil.getString(Coil_Cooling_DX_VariableSpeedFields::Name).get());
  EXPECT_EQ("Inlet Node", idf_coil.getString(Coil_Cooling_DX_VariableSpeedFields::IndoorAirInletNodeName).get());
  EXPECT_EQ("Outlet Node", idf_coil.getString(Coil_Cooling_DX_VariableSpeedFields::IndoorAirOutletNodeName).get());

  EXPECT_EQ(2, idf_coil.getInt(Coil_Cooling_DX_VariableSpeedFields::NominalSpeedLevel).get());
  EXPECT_EQ(1000.6, idf_coil.getDouble(Coil_Cooling_DX_VariableSpeedFields::GrossRatedTotalCoolingCapacityAtSelectedNominalSpeedLevel).get());
  EXPECT_EQ(0.7, idf_coil.getDouble(Coil_Cooling_DX_VariableSpeedFields::RatedAirFlowRateAtSelectedNominalSpeedLevel).get());
  EXPECT_EQ(1.9, idf_coil.getDouble(Coil_Cooling_DX_VariableSpeedFields::NominalTimeforCondensatetoBeginLeavingtheCoil).get());
  EXPECT_EQ(0.08, idf_coil.getDouble(Coil_Cooling_DX_VariableSpeedFields::InitialMoistureEvaporationRateDividedbySteadyStateACLatentCapacity).get());
  EXPECT_EQ(3.5, idf_coil.getDouble(Coil_Cooling_DX_VariableSpeedFields::MaximumCyclingRate).get());
  EXPECT_EQ(65.0, idf_coil.getDouble(Coil_Cooling_DX_VariableSpeedFields::LatentCapacityTimeConstant).get());
  EXPECT_EQ(90.0, idf_coil.getDouble(Coil_Cooling_DX_VariableSpeedFields::FanDelayTime).get());
  EXPECT_EQ("CoilPLFCurve", idf_coil.getString(Coil_Cooling_DX_VariableSpeedFields::EnergyPartLoadFractionCurveName).get());
  EXPECT_TRUE(idf_coil.isEmpty(Coil_Cooling_DX_VariableSpeedFields::CondenserAirInletNodeName));
  EXPECT_EQ("EvaporativelyCooled", idf_coil.getString(Coil_Cooling_DX_VariableSpeedFields::CondenserType).get());
  EXPECT_EQ(180.0, idf_coil.getDouble(Coil_Cooling_DX_VariableSpeedFields::EvaporativeCondenserPumpRatedPowerConsumption).get());
  EXPECT_EQ(105.0, idf_coil.getDouble(Coil_Cooling_DX_VariableSpeedFields::CrankcaseHeaterCapacity).get());
  EXPECT_EQ("CrankCoolCapFT", idf_coil.getString(Coil_Cooling_DX_VariableSpeedFields::CrankcaseHeaterCapacityFunctionofTemperatureCurveName).get());
  EXPECT_EQ(9.0, idf_coil.getDouble(Coil_Cooling_DX_VariableSpeedFields::MaximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation).get());
  EXPECT_EQ(-21.0, idf_coil.getDouble(Coil_Cooling_DX_VariableSpeedFields::MinimumOutdoorDryBulbTemperatureforCompressorOperation).get());
  EXPECT_TRUE(idf_coil.isEmpty(Coil_Cooling_DX_VariableSpeedFields::SupplyWaterStorageTankName));
  EXPECT_TRUE(idf_coil.isEmpty(Coil_Cooling_DX_VariableSpeedFields::CondensateCollectionWaterStorageTankName));
  EXPECT_EQ(560.0, idf_coil.getDouble(Coil_Cooling_DX_VariableSpeedFields::BasinHeaterCapacity).get());
  EXPECT_EQ(4.0, idf_coil.getDouble(Coil_Cooling_DX_VariableSpeedFields::BasinHeaterSetpointTemperature).get());
  EXPECT_EQ("Basin Heater Sch", idf_coil.getString(Coil_Cooling_DX_VariableSpeedFields::BasinHeaterOperatingScheduleName).get());

  EXPECT_EQ(2, idf_coil.getInt(Coil_Cooling_DX_VariableSpeedFields::NumberofSpeeds).get());
  ASSERT_EQ(2, idf_coil.numExtensibleGroups());
  auto egs = idf_coil.extensibleGroups();
  auto speeds = coil.speeds();
  for (size_t i = 0; i < egs.size(); ++i) {
    auto eg = egs[i].cast<WorkspaceExtensibleGroup>();  // Casting for getTarget
    const auto speed = speeds[i];

    EXPECT_EQ(speed.referenceUnitGrossRatedTotalCoolingCapacity(),
              eg.getDouble(Coil_Cooling_DX_VariableSpeedExtensibleFields::SpeedReferenceUnitGrossRatedTotalCoolingCapacity).get());
    EXPECT_EQ(speed.referenceUnitGrossRatedSensibleHeatRatio(),
              eg.getDouble(Coil_Cooling_DX_VariableSpeedExtensibleFields::SpeedReferenceUnitGrossRatedSensibleHeatRatio).get());
    EXPECT_EQ(speed.referenceUnitGrossRatedCoolingCOP(),
              eg.getDouble(Coil_Cooling_DX_VariableSpeedExtensibleFields::SpeedReferenceUnitGrossRatedCoolingCOP).get());
    EXPECT_EQ(speed.referenceUnitRatedAirFlowRate(),
              eg.getDouble(Coil_Cooling_DX_VariableSpeedExtensibleFields::SpeedReferenceUnitRatedAirFlowRate).get());
    EXPECT_EQ(speed.ratedEvaporatorFanPowerPerVolumeFlowRate2017(),
              eg.getDouble(Coil_Cooling_DX_VariableSpeedExtensibleFields::SpeedRatedEvaporatorFanPowerPerVolumeFlowRate2017).get());
    EXPECT_EQ(speed.ratedEvaporatorFanPowerPerVolumeFlowRate2023(),
              eg.getDouble(Coil_Cooling_DX_VariableSpeedExtensibleFields::SpeedRatedEvaporatorFanPowerPerVolumeFlowRate2023).get());
    EXPECT_EQ(speed.referenceUnitRatedCondenserAirFlowRate().get(),
              eg.getDouble(Coil_Cooling_DX_VariableSpeedExtensibleFields::SpeedReferenceUnitRatedCondenserAirFlowRate).get());
    EXPECT_EQ(speed.referenceUnitRatedPadEffectivenessofEvapPrecooling().get(),
              eg.getDouble(Coil_Cooling_DX_VariableSpeedExtensibleFields::SpeedReferenceUnitRatedPadEffectivenessofEvapPrecooling).get());
    EXPECT_EQ(speed.totalCoolingCapacityFunctionofTemperatureCurve().nameString(),
              eg.getTarget(Coil_Cooling_DX_VariableSpeedExtensibleFields::SpeedTotalCoolingCapacityFunctionofTemperatureCurveName)->nameString());
    EXPECT_EQ(speed.totalCoolingCapacityFunctionofAirFlowFractionCurve().nameString(),
              eg.getTarget(Coil_Cooling_DX_VariableSpeedExtensibleFields::SpeedTotalCoolingCapacityFunctionofAirFlowFractionCurveName)->nameString());
    EXPECT_EQ(speed.energyInputRatioFunctionofTemperatureCurve().nameString(),
              eg.getTarget(Coil_Cooling_DX_VariableSpeedExtensibleFields::SpeedEnergyInputRatioFunctionofTemperatureCurveName)->nameString());
    EXPECT_EQ(speed.energyInputRatioFunctionofAirFlowFractionCurve().nameString(),
              eg.getTarget(Coil_Cooling_DX_VariableSpeedExtensibleFields::SpeedEnergyInputRatioFunctionofAirFlowFractionCurveName)->nameString());
  }
}
