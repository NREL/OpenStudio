/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>
#include "EnergyPlusFixture.hpp"

#include "../ForwardTranslator.hpp"

#include "../../model/Model.hpp"
#include "../../model/CoilWaterHeatingAirToWaterHeatPumpVariableSpeed.hpp"
#include "../../model/CoilWaterHeatingAirToWaterHeatPumpVariableSpeed_Impl.hpp"
#include "../../model/CoilWaterHeatingAirToWaterHeatPumpVariableSpeedSpeedData.hpp"
#include "../../model/Schedule.hpp"
#include "../../model/CurveLinear.hpp"
#include "../../model/CurveQuadratic.hpp"
#include "../../model/CurveBiquadratic.hpp"
#include "../../model/WaterHeaterHeatPump.hpp"
#include "../../model/ThermalZone.hpp"
#include "../../model/Space.hpp"

#include <utilities/idd/Coil_WaterHeating_AirToWaterHeatPump_VariableSpeed_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include "../../utilities/idf/IdfExtensibleGroup.hpp"
#include "../../utilities/idf/WorkspaceExtensibleGroup.hpp"

using namespace openstudio::energyplus;
using namespace openstudio::model;
using namespace openstudio;

TEST_F(EnergyPlusFixture, ForwardTranslator_CoilWaterHeatingAirToWaterHeatPumpVariableSpeed) {
  Model m;

  CoilWaterHeatingAirToWaterHeatPumpVariableSpeed coil(m);

  EXPECT_TRUE(coil.setCrankcaseHeaterCapacity(105.0));
  CurveLinear crankCurve(m);
  crankCurve.setName("CrankCapFT");
  EXPECT_TRUE(coil.setCrankcaseHeaterCapacityFunctionofTemperatureCurve(crankCurve));
  EXPECT_TRUE(coil.setMaximumAmbientTemperatureforCrankcaseHeaterOperation(9.0));

  WaterHeaterHeatPump hpwh(m);
  CoilWaterHeatingAirToWaterHeatPumpVariableSpeedSpeedData speed(m);
  ThermalZone tz(m);
  Space space(m);
  space.setThermalZone(tz);

  hpwh.setDXCoil(coil);
  hpwh.addToThermalZone(tz);
  coil.addSpeed(speed);

  ForwardTranslator ft;
  Workspace w = ft.translateModel(m);

  EXPECT_EQ(1u, w.getObjectsByType(IddObjectType::WaterHeater_HeatPump_PumpedCondenser).size());
  EXPECT_EQ(6u, w.getObjectsByType(IddObjectType::Curve_Quadratic).size());
  EXPECT_EQ(4u, w.getObjectsByType(IddObjectType::Curve_Biquadratic).size());

  WorkspaceObjectVector idf_coils(w.getObjectsByType(IddObjectType::Coil_WaterHeating_AirToWaterHeatPump_VariableSpeed));
  EXPECT_EQ(1u, idf_coils.size());
  WorkspaceObject idf_coil(idf_coils[0]);

  EXPECT_EQ(1, idf_coil.getInt(Coil_WaterHeating_AirToWaterHeatPump_VariableSpeedFields::NominalSpeedLevel, false).get());
  EXPECT_EQ(4000.0, idf_coil.getDouble(Coil_WaterHeating_AirToWaterHeatPump_VariableSpeedFields::RatedWaterHeatingCapacity, false).get());
  EXPECT_EQ(29.44,
            idf_coil.getDouble(Coil_WaterHeating_AirToWaterHeatPump_VariableSpeedFields::RatedEvaporatorInletAirDryBulbTemperature, false).get());
  EXPECT_EQ(22.22,
            idf_coil.getDouble(Coil_WaterHeating_AirToWaterHeatPump_VariableSpeedFields::RatedEvaporatorInletAirWetBulbTemperature, false).get());
  EXPECT_EQ(55.72, idf_coil.getDouble(Coil_WaterHeating_AirToWaterHeatPump_VariableSpeedFields::RatedCondenserInletWaterTemperature, false).get());
  EXPECT_EQ("autocalculate", idf_coil.getString(Coil_WaterHeating_AirToWaterHeatPump_VariableSpeedFields::RatedEvaporatorAirFlowRate, false).get());
  EXPECT_EQ("autocalculate", idf_coil.getString(Coil_WaterHeating_AirToWaterHeatPump_VariableSpeedFields::RatedCondenserWaterFlowRate, false).get());
  EXPECT_EQ("Yes", idf_coil.getString(Coil_WaterHeating_AirToWaterHeatPump_VariableSpeedFields::EvaporatorFanPowerIncludedinRatedCOP, false).get());
  EXPECT_EQ("No", idf_coil.getString(Coil_WaterHeating_AirToWaterHeatPump_VariableSpeedFields::CondenserPumpPowerIncludedinRatedCOP, false).get());
  EXPECT_EQ(
    "No",
    idf_coil.getString(Coil_WaterHeating_AirToWaterHeatPump_VariableSpeedFields::CondenserPumpHeatIncludedinRatedHeatingCapacityandRatedCOP, false)
      .get());
  EXPECT_EQ(0.2, idf_coil.getDouble(Coil_WaterHeating_AirToWaterHeatPump_VariableSpeedFields::FractionofCondenserPumpHeattoWater, false).get());
  EXPECT_NE("", idf_coil.getString(Coil_WaterHeating_AirToWaterHeatPump_VariableSpeedFields::EvaporatorAirInletNodeName, false).get());
  EXPECT_NE("", idf_coil.getString(Coil_WaterHeating_AirToWaterHeatPump_VariableSpeedFields::EvaporatorAirOutletNodeName, false).get());
  EXPECT_NE("", idf_coil.getString(Coil_WaterHeating_AirToWaterHeatPump_VariableSpeedFields::CondenserWaterInletNodeName, false).get());
  EXPECT_NE("", idf_coil.getString(Coil_WaterHeating_AirToWaterHeatPump_VariableSpeedFields::CondenserWaterOutletNodeName, false).get());
  EXPECT_EQ(105.0, idf_coil.getDouble(Coil_WaterHeating_AirToWaterHeatPump_VariableSpeedFields::CrankcaseHeaterCapacity).get());
  EXPECT_EQ(
    "CrankCapFT",
    idf_coil.getString(Coil_WaterHeating_AirToWaterHeatPump_VariableSpeedFields::CrankcaseHeaterCapacityFunctionofTemperatureCurveName).get());
  EXPECT_EQ(9.0,
            idf_coil.getDouble(Coil_WaterHeating_AirToWaterHeatPump_VariableSpeedFields::MaximumAmbientTemperatureforCrankcaseHeaterOperation).get());
  EXPECT_EQ("WetBulbTemperature",
            idf_coil.getString(Coil_WaterHeating_AirToWaterHeatPump_VariableSpeedFields::EvaporatorAirTemperatureTypeforCurveObjects, false).get());
  boost::optional<WorkspaceObject> idf_curve(
    idf_coil.getTarget(Coil_WaterHeating_AirToWaterHeatPump_VariableSpeedFields::PartLoadFractionCorrelationCurveName));
  ASSERT_TRUE(idf_curve);
  EXPECT_EQ(idf_curve->iddObject().type(), IddObjectType::Curve_Quadratic);
  EXPECT_EQ(1, idf_coil.getInt(Coil_WaterHeating_AirToWaterHeatPump_VariableSpeedFields::NumberofSpeeds, false).get());

  EXPECT_EQ(1u, idf_coil.numExtensibleGroups());

  auto w_eg = idf_coil.extensibleGroups()[0].cast<WorkspaceExtensibleGroup>();
  EXPECT_EQ(400.0, w_eg.getDouble(Coil_WaterHeating_AirToWaterHeatPump_VariableSpeedExtensibleFields::RatedWaterHeatingCapacityatSpeed, false).get());
  EXPECT_EQ(5.0, w_eg.getDouble(Coil_WaterHeating_AirToWaterHeatPump_VariableSpeedExtensibleFields::RatedWaterHeatingCOPatSpeed, false).get());
  EXPECT_EQ(0.8, w_eg.getDouble(Coil_WaterHeating_AirToWaterHeatPump_VariableSpeedExtensibleFields::RatedSensibleHeatRatioatSpeed, false).get());
  EXPECT_EQ(0.02014,
            w_eg.getDouble(Coil_WaterHeating_AirToWaterHeatPump_VariableSpeedExtensibleFields::SpeedReferenceUnitRatedAirFlowRate, false).get());
  EXPECT_EQ(0.000018,
            w_eg.getDouble(Coil_WaterHeating_AirToWaterHeatPump_VariableSpeedExtensibleFields::SpeedReferenceUnitRatedWaterFlowRate, false).get());
  EXPECT_EQ(
    10.0,
    w_eg.getDouble(Coil_WaterHeating_AirToWaterHeatPump_VariableSpeedExtensibleFields::SpeedReferenceUnitWaterPumpInputPowerAtRatedConditions, false)
      .get());
  boost::optional<WorkspaceObject> idf_curve1(
    w_eg.getTarget(Coil_WaterHeating_AirToWaterHeatPump_VariableSpeedExtensibleFields::SpeedTotalWHCapacityFunctionofTemperatureCurveName));
  ASSERT_TRUE(idf_curve1);
  EXPECT_EQ(idf_curve1->iddObject().type(), IddObjectType::Curve_Biquadratic);
  boost::optional<WorkspaceObject> idf_curve2(
    w_eg.getTarget(Coil_WaterHeating_AirToWaterHeatPump_VariableSpeedExtensibleFields::SpeedTotalWHCapacityFunctionofAirFlowFractionCurveName));
  ASSERT_TRUE(idf_curve2);
  EXPECT_EQ(idf_curve2->iddObject().type(), IddObjectType::Curve_Cubic);
  boost::optional<WorkspaceObject> idf_curve3(
    w_eg.getTarget(Coil_WaterHeating_AirToWaterHeatPump_VariableSpeedExtensibleFields::SpeedTotalWHCapacityFunctionofWaterFlowFractionCurveName));
  ASSERT_TRUE(idf_curve3);
  EXPECT_EQ(idf_curve3->iddObject().type(), IddObjectType::Curve_Cubic);
  boost::optional<WorkspaceObject> idf_curve4(
    w_eg.getTarget(Coil_WaterHeating_AirToWaterHeatPump_VariableSpeedExtensibleFields::SpeedCOPFunctionofTemperatureCurveName));
  ASSERT_TRUE(idf_curve4);
  EXPECT_EQ(idf_curve4->iddObject().type(), IddObjectType::Curve_Biquadratic);
  boost::optional<WorkspaceObject> idf_curve5(
    w_eg.getTarget(Coil_WaterHeating_AirToWaterHeatPump_VariableSpeedExtensibleFields::SpeedCOPFunctionofAirFlowFractionCurveName));
  ASSERT_TRUE(idf_curve5);
  EXPECT_EQ(idf_curve5->iddObject().type(), IddObjectType::Curve_Cubic);
  boost::optional<WorkspaceObject> idf_curve6(
    w_eg.getTarget(Coil_WaterHeating_AirToWaterHeatPump_VariableSpeedExtensibleFields::SpeedCOPFunctionofWaterFlowFractionCurveName));
  ASSERT_TRUE(idf_curve6);
  EXPECT_EQ(idf_curve6->iddObject().type(), IddObjectType::Curve_Cubic);
}
