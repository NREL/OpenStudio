/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>
#include "EnergyPlusFixture.hpp"

#include "../ForwardTranslator.hpp"
// #include "../ReverseTranslator.hpp"

#include "../../model/CoilHeatingDXSingleSpeed.hpp"

#include "../../model/Model.hpp"
#include "../../model/AirLoopHVAC.hpp"
#include "../../model/Node.hpp"
#include "../../model/Curve.hpp"
#include "../../model/CurveBiquadratic.hpp"
#include "../../model/CurveLinear.hpp"
#include "../../model/ScheduleConstant.hpp"

#include "../../utilities/idf/Workspace.hpp"
#include "../../utilities/idf/IdfObject.hpp"
#include "../../utilities/idf/WorkspaceObject.hpp"

#include <utilities/idd/Coil_Heating_DX_SingleSpeed_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include <resources.hxx>
#include <sstream>

using namespace openstudio::energyplus;
using namespace openstudio::model;
using namespace openstudio;

TEST_F(EnergyPlusFixture, ForwardTranslator_CoilHeatingDXSingleSpeed) {

  Model m;
  CoilHeatingDXSingleSpeed coil(m);
  EXPECT_TRUE(coil.setName("Coil Heating DX Single Speed"));

  ScheduleConstant availSch(m);
  availSch.setName("Coil Avail Sch");
  EXPECT_TRUE(coil.setAvailabilitySchedule(availSch));

  EXPECT_TRUE(coil.setRatedTotalHeatingCapacity(8000.0));
  EXPECT_TRUE(coil.setRatedCOP(4.5));
  EXPECT_TRUE(coil.setRatedAirFlowRate(1.2));
  EXPECT_TRUE(coil.setRatedSupplyFanPowerPerVolumeFlowRate2017(817.0));
  EXPECT_TRUE(coil.setRatedSupplyFanPowerPerVolumeFlowRate2023(823.0));

  AirLoopHVAC a(m);
  Node supplyOutletNode = a.supplyOutletNode();
  coil.addToNode(supplyOutletNode);
  a.supplyInletNode().setName("Supply Inlet Node");
  a.supplyOutletNode().setName("Supply Outlet Node");

  coil.totalHeatingCapacityFunctionofTemperatureCurve().setName("HeatCapFT");
  coil.totalHeatingCapacityFunctionofFlowFractionCurve().setName("TotHeatCapfFlow");
  coil.energyInputRatioFunctionofTemperatureCurve().setName("EIRFT");
  coil.energyInputRatioFunctionofFlowFractionCurve().setName("EIRfFlow");
  coil.partLoadFractionCorrelationCurve().setName("CoilPLFCurve");

  CurveBiquadratic defrostEIRFT(m);
  defrostEIRFT.setName("DefrostEIRFT");
  EXPECT_TRUE(coil.setDefrostEnergyInputRatioFunctionofTemperatureCurve(defrostEIRFT));

  EXPECT_TRUE(coil.setMinimumOutdoorDryBulbTemperatureforCompressorOperation(-7.0));
  EXPECT_TRUE(coil.setMaximumOutdoorDryBulbTemperatureforDefrostOperation(5.0));
  EXPECT_TRUE(coil.setCrankcaseHeaterCapacity(105.0));
  CurveLinear crankCurve(m);
  crankCurve.setName("CrankHeatCapFT");
  EXPECT_TRUE(coil.setCrankcaseHeaterCapacityFunctionofTemperatureCurve(crankCurve));
  EXPECT_TRUE(coil.setMaximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation(9.0));

  EXPECT_TRUE(coil.setDefrostStrategy("ReverseCycle"));
  EXPECT_TRUE(coil.setDefrostControl("Timed"));
  EXPECT_TRUE(coil.setDefrostTimePeriodFraction(0.08));
  EXPECT_TRUE(coil.setResistiveDefrostHeaterCapacity(20.5));

  //translate the model to EnergyPlus
  ForwardTranslator ft;
  const Workspace workspace = ft.translateModel(m);

  auto idfs_coils = workspace.getObjectsByType(IddObjectType::Coil_Heating_DX_SingleSpeed);
  ASSERT_EQ(1, idfs_coils.size());
  const WorkspaceObject& idf_coil = idfs_coils[0];

  EXPECT_EQ("Coil Heating DX Single Speed", idf_coil.getString(Coil_Heating_DX_SingleSpeedFields::Name).get());
  EXPECT_EQ("Coil Avail Sch", idf_coil.getString(Coil_Heating_DX_SingleSpeedFields::AvailabilityScheduleName).get());
  EXPECT_EQ(8000.0, idf_coil.getDouble(Coil_Heating_DX_SingleSpeedFields::GrossRatedHeatingCapacity).get());
  EXPECT_EQ(4.5, idf_coil.getDouble(Coil_Heating_DX_SingleSpeedFields::GrossRatedHeatingCOP).get());
  EXPECT_EQ(1.2, idf_coil.getDouble(Coil_Heating_DX_SingleSpeedFields::RatedAirFlowRate).get());
  EXPECT_EQ(817.0, idf_coil.getDouble(Coil_Heating_DX_SingleSpeedFields::RatedSupplyFanPowerPerVolumeFlowRate2017).get());
  EXPECT_EQ(823.0, idf_coil.getDouble(Coil_Heating_DX_SingleSpeedFields::RatedSupplyFanPowerPerVolumeFlowRate2023).get());
  EXPECT_EQ("Supply Inlet Node", idf_coil.getString(Coil_Heating_DX_SingleSpeedFields::AirInletNodeName).get());
  EXPECT_EQ("Supply Outlet Node", idf_coil.getString(Coil_Heating_DX_SingleSpeedFields::AirOutletNodeName).get());
  EXPECT_EQ("HeatCapFT", idf_coil.getString(Coil_Heating_DX_SingleSpeedFields::HeatingCapacityFunctionofTemperatureCurveName).get());
  EXPECT_EQ("TotHeatCapfFlow", idf_coil.getString(Coil_Heating_DX_SingleSpeedFields::HeatingCapacityFunctionofFlowFractionCurveName).get());
  EXPECT_EQ("EIRFT", idf_coil.getString(Coil_Heating_DX_SingleSpeedFields::EnergyInputRatioFunctionofTemperatureCurveName).get());
  EXPECT_EQ("EIRfFlow", idf_coil.getString(Coil_Heating_DX_SingleSpeedFields::EnergyInputRatioFunctionofFlowFractionCurveName).get());
  EXPECT_EQ("CoilPLFCurve", idf_coil.getString(Coil_Heating_DX_SingleSpeedFields::PartLoadFractionCorrelationCurveName).get());
  EXPECT_EQ("DefrostEIRFT", idf_coil.getString(Coil_Heating_DX_SingleSpeedFields::DefrostEnergyInputRatioFunctionofTemperatureCurveName).get());
  EXPECT_EQ(-7.0, idf_coil.getDouble(Coil_Heating_DX_SingleSpeedFields::MinimumOutdoorDryBulbTemperatureforCompressorOperation).get());
  EXPECT_EQ(5.0, idf_coil.getDouble(Coil_Heating_DX_SingleSpeedFields::MaximumOutdoorDryBulbTemperatureforDefrostOperation).get());
  EXPECT_EQ(105.0, idf_coil.getDouble(Coil_Heating_DX_SingleSpeedFields::CrankcaseHeaterCapacity).get());
  EXPECT_EQ("CrankHeatCapFT", idf_coil.getString(Coil_Heating_DX_SingleSpeedFields::CrankcaseHeaterCapacityFunctionofTemperatureCurveName).get());
  EXPECT_EQ(9.0, idf_coil.getDouble(Coil_Heating_DX_SingleSpeedFields::MaximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation).get());
  EXPECT_EQ("ReverseCycle", idf_coil.getString(Coil_Heating_DX_SingleSpeedFields::DefrostStrategy).get());
  EXPECT_EQ("Timed", idf_coil.getString(Coil_Heating_DX_SingleSpeedFields::DefrostControl).get());
  EXPECT_EQ(0.08, idf_coil.getDouble(Coil_Heating_DX_SingleSpeedFields::DefrostTimePeriodFraction).get());
  EXPECT_EQ(20.5, idf_coil.getDouble(Coil_Heating_DX_SingleSpeedFields::ResistiveDefrostHeaterCapacity).get());
}
