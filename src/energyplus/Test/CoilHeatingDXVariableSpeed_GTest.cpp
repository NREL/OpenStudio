/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>
#include "EnergyPlusFixture.hpp"

#include "../ForwardTranslator.hpp"
// #include "../ReverseTranslator.hpp"

#include "../../model/CoilHeatingDXVariableSpeed.hpp"
#include "../../model/CoilHeatingDXVariableSpeedSpeedData.hpp"

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
#include "../../utilities/idf/WorkspaceObject_Impl.hpp"
#include "../../utilities/idf/WorkspaceExtensibleGroup.hpp"
#include "../../utilities/core/Compare.hpp"

#include <utilities/idd/Coil_Heating_DX_VariableSpeed_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include <resources.hxx>
#include <sstream>

using namespace openstudio::energyplus;
using namespace openstudio::model;
using namespace openstudio;

TEST_F(EnergyPlusFixture, ForwardTranslator_CoilHeatingDXVariableSpeed) {

  Model m;
  CoilHeatingDXVariableSpeed coil(m);

  {
    CoilHeatingDXVariableSpeedSpeedData speed(m);
    speed.setName("DX VSD Speed 1");
    EXPECT_TRUE(speed.setName("DX VSD Speed 1"));
    EXPECT_TRUE(speed.setReferenceUnitGrossRatedHeatingCapacity(4000.0));
    EXPECT_TRUE(speed.setReferenceUnitGrossRatedHeatingCOP(4.0));
    EXPECT_TRUE(speed.setReferenceUnitRatedAirFlowRate(0.5));
    EXPECT_TRUE(speed.setRatedSupplyFanPowerPerVolumeFlowRate2017(817.0));
    EXPECT_TRUE(speed.setRatedSupplyFanPowerPerVolumeFlowRate2023(823.0));
    speed.heatingCapacityFunctionofTemperatureCurve().setName("Speed1 HeatCapFT");
    speed.totalHeatingCapacityFunctionofAirFlowFractionCurve().setName("Speed1 TotHeatCapfFlow");
    speed.energyInputRatioFunctionofTemperatureCurve().setName("Speed1 EIRFT");
    speed.energyInputRatioFunctionofAirFlowFractionCurve().setName("Speed1 EIRfFlow");

    EXPECT_TRUE(coil.addSpeed(speed));
  }

  {
    CoilHeatingDXVariableSpeedSpeedData speed(m);
    speed.setName("DX VSD Speed 2");
    EXPECT_TRUE(speed.setName("DX VSD Speed 2"));
    EXPECT_TRUE(speed.setReferenceUnitGrossRatedHeatingCapacity(8000.));
    EXPECT_TRUE(speed.setReferenceUnitGrossRatedHeatingCOP(3.5));
    EXPECT_TRUE(speed.setReferenceUnitRatedAirFlowRate(1.0));
    EXPECT_TRUE(speed.setRatedSupplyFanPowerPerVolumeFlowRate2017(927.0));
    EXPECT_TRUE(speed.setRatedSupplyFanPowerPerVolumeFlowRate2023(923.0));
    speed.heatingCapacityFunctionofTemperatureCurve().setName("Speed2 HeatCapFT");
    speed.totalHeatingCapacityFunctionofAirFlowFractionCurve().setName("Speed2 TotHeatCapfFlow");
    speed.energyInputRatioFunctionofTemperatureCurve().setName("Speed2 EIRFT");
    speed.energyInputRatioFunctionofAirFlowFractionCurve().setName("Speed2 EIRfFlow");

    EXPECT_TRUE(coil.addSpeed(speed));
  }

  EXPECT_TRUE(coil.setName("Coil Heating DX VSD"));

  EXPECT_TRUE(coil.setNominalSpeedLevel(2));
  EXPECT_TRUE(coil.setRatedHeatingCapacityAtSelectedNominalSpeedLevel(1000.6));
  EXPECT_TRUE(coil.setRatedAirFlowRateAtSelectedNominalSpeedLevel(0.7));

  coil.energyPartLoadFractionCurve().setName("CoilPLFCurve");

  CurveBiquadratic defrostEIRFT(m);
  defrostEIRFT.setName("DefrostEIRFT");
  EXPECT_TRUE(coil.setDefrostEnergyInputRatioFunctionofTemperatureCurve(defrostEIRFT));

  EXPECT_TRUE(coil.setMinimumOutdoorDryBulbTemperatureforCompressorOperation(-7.0));
  EXPECT_TRUE(coil.setOutdoorDryBulbTemperaturetoTurnOnCompressor(-5.0));
  EXPECT_TRUE(coil.setCrankcaseHeaterCapacity(105.0));
  CurveLinear crankCurve(m);
  crankCurve.setName("CrankHeatCapFT");
  EXPECT_TRUE(coil.setCrankcaseHeaterCapacityFunctionofTemperatureCurve(crankCurve));
  EXPECT_TRUE(coil.setMaximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation(9.0));

  EXPECT_TRUE(coil.setDefrostStrategy("ReverseCycle"));
  EXPECT_TRUE(coil.setDefrostControl("Timed"));
  EXPECT_TRUE(coil.setDefrostTimePeriodFraction(0.08));
  EXPECT_TRUE(coil.setResistiveDefrostHeaterCapacity(20.5));

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

  auto idfs_coils = workspace.getObjectsByType(IddObjectType::Coil_Heating_DX_VariableSpeed);
  ASSERT_EQ(1, idfs_coils.size());
  const WorkspaceObject& idf_coil = idfs_coils[0];

  EXPECT_EQ("Coil Heating DX VSD", idf_coil.getString(Coil_Heating_DX_VariableSpeedFields::Name).get());
  EXPECT_EQ("Inlet Node", idf_coil.getString(Coil_Heating_DX_VariableSpeedFields::IndoorAirInletNodeName).get());
  EXPECT_EQ("Outlet Node", idf_coil.getString(Coil_Heating_DX_VariableSpeedFields::IndoorAirOutletNodeName).get());

  EXPECT_EQ(2, idf_coil.getInt(Coil_Heating_DX_VariableSpeedFields::NominalSpeedLevel).get());
  EXPECT_EQ(1000.6, idf_coil.getDouble(Coil_Heating_DX_VariableSpeedFields::RatedHeatingCapacityAtSelectedNominalSpeedLevel).get());
  EXPECT_EQ(0.7, idf_coil.getDouble(Coil_Heating_DX_VariableSpeedFields::RatedAirFlowRateAtSelectedNominalSpeedLevel).get());
  EXPECT_EQ("CoilPLFCurve", idf_coil.getString(Coil_Heating_DX_VariableSpeedFields::EnergyPartLoadFractionCurveName).get());
  EXPECT_EQ("DefrostEIRFT", idf_coil.getString(Coil_Heating_DX_VariableSpeedFields::DefrostEnergyInputRatioFunctionofTemperatureCurveName).get());
  EXPECT_EQ(-7.0, idf_coil.getDouble(Coil_Heating_DX_VariableSpeedFields::MinimumOutdoorDryBulbTemperatureforCompressorOperation).get());
  EXPECT_EQ(-5.0, idf_coil.getDouble(Coil_Heating_DX_VariableSpeedFields::OutdoorDryBulbTemperaturetoTurnOnCompressor).get());
  EXPECT_EQ(5.0, idf_coil.getDouble(Coil_Heating_DX_VariableSpeedFields::MaximumOutdoorDryBulbTemperatureforDefrostOperation).get());
  EXPECT_EQ(105.0, idf_coil.getDouble(Coil_Heating_DX_VariableSpeedFields::CrankcaseHeaterCapacity).get());
  EXPECT_EQ("CrankHeatCapFT", idf_coil.getString(Coil_Heating_DX_VariableSpeedFields::CrankcaseHeaterCapacityFunctionofTemperatureCurveName).get());
  EXPECT_EQ(9.0, idf_coil.getDouble(Coil_Heating_DX_VariableSpeedFields::MaximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation).get());
  EXPECT_EQ("ReverseCycle", idf_coil.getString(Coil_Heating_DX_VariableSpeedFields::DefrostStrategy).get());
  EXPECT_EQ("Timed", idf_coil.getString(Coil_Heating_DX_VariableSpeedFields::DefrostControl).get());
  EXPECT_EQ(0.08, idf_coil.getDouble(Coil_Heating_DX_VariableSpeedFields::DefrostTimePeriodFraction).get());
  EXPECT_EQ(20.5, idf_coil.getDouble(Coil_Heating_DX_VariableSpeedFields::ResistiveDefrostHeaterCapacity).get());

  EXPECT_EQ(2, idf_coil.getInt(Coil_Heating_DX_VariableSpeedFields::NumberofSpeeds).get());
  ASSERT_EQ(2, idf_coil.numExtensibleGroups());
  auto egs = idf_coil.extensibleGroups();
  auto speeds = coil.speeds();
  for (size_t i = 0; i < egs.size(); ++i) {
    auto eg = egs[i].cast<WorkspaceExtensibleGroup>();  // Casting for getTarget
    const auto speed = speeds[i];

    EXPECT_EQ(speed.referenceUnitGrossRatedHeatingCapacity(),
              eg.getDouble(Coil_Heating_DX_VariableSpeedExtensibleFields::SpeedReferenceUnitGrossRatedHeatingCapacity).get());
    EXPECT_EQ(speed.referenceUnitGrossRatedHeatingCOP(),
              eg.getDouble(Coil_Heating_DX_VariableSpeedExtensibleFields::SpeedReferenceUnitGrossRatedHeatingCOP).get());
    EXPECT_EQ(speed.referenceUnitRatedAirFlowRate(),
              eg.getDouble(Coil_Heating_DX_VariableSpeedExtensibleFields::SpeedReferenceUnitRatedAirFlowRate).get());
    EXPECT_EQ(speed.ratedSupplyFanPowerPerVolumeFlowRate2017(),
              eg.getDouble(Coil_Heating_DX_VariableSpeedExtensibleFields::SpeedRatedSupplyAirFanPowerPerVolumeFlowRate2017).get());
    EXPECT_EQ(speed.ratedSupplyFanPowerPerVolumeFlowRate2023(),
              eg.getDouble(Coil_Heating_DX_VariableSpeedExtensibleFields::SpeedRatedSupplyAirFanPowerPerVolumeFlowRate2023).get());
    EXPECT_EQ(speed.heatingCapacityFunctionofTemperatureCurve().nameString(),
              eg.getTarget(Coil_Heating_DX_VariableSpeedExtensibleFields::SpeedHeatingCapacityFunctionofTemperatureCurveName)->nameString());
    EXPECT_EQ(speed.totalHeatingCapacityFunctionofAirFlowFractionCurve().nameString(),
              eg.getTarget(Coil_Heating_DX_VariableSpeedExtensibleFields::SpeedTotalHeatingCapacityFunctionofAirFlowFractionCurveName)->nameString());
    EXPECT_EQ(speed.energyInputRatioFunctionofTemperatureCurve().nameString(),
              eg.getTarget(Coil_Heating_DX_VariableSpeedExtensibleFields::SpeedEnergyInputRatioFunctionofTemperatureCurveName)->nameString());
    EXPECT_EQ(speed.energyInputRatioFunctionofAirFlowFractionCurve().nameString(),
              eg.getTarget(Coil_Heating_DX_VariableSpeedExtensibleFields::SpeedEnergyInputRatioFunctionofAirFlowFractionCurveName)->nameString());
  }
}
