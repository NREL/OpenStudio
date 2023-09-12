/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>
#include "EnergyPlusFixture.hpp"

#include "../ForwardTranslator.hpp"

#include "../../model/Model.hpp"
#include "../../model/CoilWaterHeatingAirToWaterHeatPump.hpp"
#include "../../model/CoilWaterHeatingAirToWaterHeatPump_Impl.hpp"
#include "../../model/Schedule.hpp"
#include "../../model/Curve.hpp"
#include "../../model/CurveLinear.hpp"
#include "../../model/WaterHeaterHeatPump.hpp"
#include "../../model/ThermalZone.hpp"
#include "../../model/Space.hpp"

#include <utilities/idd/Coil_WaterHeating_AirToWaterHeatPump_Pumped_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include "../../utilities/idf/IdfExtensibleGroup.hpp"
#include "../../utilities/idf/WorkspaceExtensibleGroup.hpp"

using namespace openstudio::energyplus;
using namespace openstudio::model;
using namespace openstudio;

TEST_F(EnergyPlusFixture, ForwardTranslator_CoilWaterHeatingAirToWaterHeatPumpPumped) {
  Model m;

  CoilWaterHeatingAirToWaterHeatPump coil(m);
  coil.setName("My CoilWaterHeatingAirToWaterHeatPumpPumped");

  EXPECT_TRUE(coil.setRatedHeatingCapacity(4000.0));
  EXPECT_TRUE(coil.setRatedCOP(3.2));
  EXPECT_TRUE(coil.setRatedSensibleHeatRatio(0.6956));
  EXPECT_TRUE(coil.setRatedEvaporatorInletAirDryBulbTemperature(28.0));
  EXPECT_TRUE(coil.setRatedEvaporatorInletAirWetBulbTemperature(21.00));
  EXPECT_TRUE(coil.setRatedCondenserInletWaterTemperature(52.00));
  EXPECT_TRUE(coil.setRatedEvaporatorAirFlowRate(1.0));
  EXPECT_TRUE(coil.setRatedCondenserWaterFlowRate(1.2));
  EXPECT_TRUE(coil.setEvaporatorFanPowerIncludedinRatedCOP(true));
  EXPECT_TRUE(coil.setCondenserPumpPowerIncludedinRatedCOP(false));
  EXPECT_TRUE(coil.setCondenserPumpHeatIncludedinRatedHeatingCapacityandRatedCOP(true));
  EXPECT_TRUE(coil.setCondenserWaterPumpPower(100.0));
  EXPECT_TRUE(coil.setFractionofCondenserPumpHeattoWater(0.15));

  EXPECT_TRUE(coil.setCrankcaseHeaterCapacity(105.0));
  CurveLinear crankCurve(m);
  crankCurve.setName("CrankCapFT");
  EXPECT_TRUE(coil.setCrankcaseHeaterCapacityFunctionofTemperatureCurve(crankCurve));
  EXPECT_TRUE(coil.setMaximumAmbientTemperatureforCrankcaseHeaterOperation(9.0));

  EXPECT_TRUE(coil.setEvaporatorAirTemperatureTypeforCurveObjects("DryBulbTemperature"));
  coil.heatingCapacityFunctionofTemperatureCurve().setName("HeatCapFT");
  coil.heatingCapacityFunctionofAirFlowFractionCurve().setName("HeatCapfAirFlow");
  coil.heatingCapacityFunctionofWaterFlowFractionCurve().setName("HeatCapfWaterFlow");

  coil.heatingCOPFunctionofTemperatureCurve().setName("HeatCOPFT");
  coil.heatingCOPFunctionofAirFlowFractionCurve().setName("HeatCOPfAirFlow");
  coil.heatingCOPFunctionofWaterFlowFractionCurve().setName("HeatCOPfWaterFlow");

  coil.partLoadFractionCorrelationCurve().setName("CoilPLFCurve");

  WaterHeaterHeatPump hpwh(m);
  ThermalZone tz(m);
  Space space(m);
  space.setThermalZone(tz);

  hpwh.setDXCoil(coil);
  hpwh.addToThermalZone(tz);

  ForwardTranslator ft;
  const Workspace w = ft.translateModel(m);

  EXPECT_EQ(1u, w.getObjectsByType(IddObjectType::WaterHeater_HeatPump_PumpedCondenser).size());
  EXPECT_EQ(10u, w.getObjectsByType(IddObjectType::Curve_Quadratic).size());
  EXPECT_EQ(4u, w.getObjectsByType(IddObjectType::Curve_Biquadratic).size());
  EXPECT_EQ(1u, w.getObjectsByType(IddObjectType::Curve_Exponent).size());
  EXPECT_EQ(1u, w.getObjectsByType(IddObjectType::Curve_Linear).size());

  auto idfs_coils = w.getObjectsByType(IddObjectType::Coil_WaterHeating_AirToWaterHeatPump_Pumped);
  ASSERT_EQ(1, idfs_coils.size());
  const WorkspaceObject& idf_coil = idfs_coils[0];

  EXPECT_EQ("My CoilWaterHeatingAirToWaterHeatPumpPumped", idf_coil.getString(Coil_WaterHeating_AirToWaterHeatPump_PumpedFields::Name).get());
  EXPECT_EQ(4000.0, idf_coil.getDouble(Coil_WaterHeating_AirToWaterHeatPump_PumpedFields::RatedHeatingCapacity).get());
  EXPECT_EQ(3.2, idf_coil.getDouble(Coil_WaterHeating_AirToWaterHeatPump_PumpedFields::RatedCOP).get());
  EXPECT_EQ(0.6956, idf_coil.getDouble(Coil_WaterHeating_AirToWaterHeatPump_PumpedFields::RatedSensibleHeatRatio).get());
  EXPECT_EQ(28.0, idf_coil.getDouble(Coil_WaterHeating_AirToWaterHeatPump_PumpedFields::RatedEvaporatorInletAirDryBulbTemperature).get());
  EXPECT_EQ(21.0, idf_coil.getDouble(Coil_WaterHeating_AirToWaterHeatPump_PumpedFields::RatedEvaporatorInletAirWetBulbTemperature).get());
  EXPECT_EQ(52.0, idf_coil.getDouble(Coil_WaterHeating_AirToWaterHeatPump_PumpedFields::RatedCondenserInletWaterTemperature).get());
  EXPECT_EQ(1.0, idf_coil.getDouble(Coil_WaterHeating_AirToWaterHeatPump_PumpedFields::RatedEvaporatorAirFlowRate).get());
  EXPECT_EQ(1.2, idf_coil.getDouble(Coil_WaterHeating_AirToWaterHeatPump_PumpedFields::RatedCondenserWaterFlowRate).get());
  EXPECT_EQ("Yes", idf_coil.getString(Coil_WaterHeating_AirToWaterHeatPump_PumpedFields::EvaporatorFanPowerIncludedinRatedCOP).get());
  EXPECT_EQ("No", idf_coil.getString(Coil_WaterHeating_AirToWaterHeatPump_PumpedFields::CondenserPumpPowerIncludedinRatedCOP).get());
  EXPECT_EQ("Yes",
            idf_coil.getString(Coil_WaterHeating_AirToWaterHeatPump_PumpedFields::CondenserPumpHeatIncludedinRatedHeatingCapacityandRatedCOP).get());
  EXPECT_EQ(100.0, idf_coil.getDouble(Coil_WaterHeating_AirToWaterHeatPump_PumpedFields::CondenserWaterPumpPower).get());
  EXPECT_EQ(0.15, idf_coil.getDouble(Coil_WaterHeating_AirToWaterHeatPump_PumpedFields::FractionofCondenserPumpHeattoWater).get());
  EXPECT_FALSE(idf_coil.isEmpty(Coil_WaterHeating_AirToWaterHeatPump_PumpedFields::EvaporatorAirInletNodeName));
  EXPECT_FALSE(idf_coil.isEmpty(Coil_WaterHeating_AirToWaterHeatPump_PumpedFields::EvaporatorAirOutletNodeName));
  EXPECT_FALSE(idf_coil.isEmpty(Coil_WaterHeating_AirToWaterHeatPump_PumpedFields::CondenserWaterInletNodeName));
  EXPECT_FALSE(idf_coil.isEmpty(Coil_WaterHeating_AirToWaterHeatPump_PumpedFields::CondenserWaterOutletNodeName));
  EXPECT_EQ(105.0, idf_coil.getDouble(Coil_WaterHeating_AirToWaterHeatPump_PumpedFields::CrankcaseHeaterCapacity).get());
  EXPECT_EQ("CrankCapFT",
            idf_coil.getString(Coil_WaterHeating_AirToWaterHeatPump_PumpedFields::CrankcaseHeaterCapacityFunctionofTemperatureCurveName).get());
  EXPECT_EQ(9.0, idf_coil.getDouble(Coil_WaterHeating_AirToWaterHeatPump_PumpedFields::MaximumAmbientTemperatureforCrankcaseHeaterOperation).get());
  EXPECT_EQ("DryBulbTemperature",
            idf_coil.getString(Coil_WaterHeating_AirToWaterHeatPump_PumpedFields::EvaporatorAirTemperatureTypeforCurveObjects).get());
  EXPECT_EQ("HeatCapFT", idf_coil.getString(Coil_WaterHeating_AirToWaterHeatPump_PumpedFields::HeatingCapacityFunctionofTemperatureCurveName).get());
  EXPECT_EQ("HeatCapfAirFlow",
            idf_coil.getString(Coil_WaterHeating_AirToWaterHeatPump_PumpedFields::HeatingCapacityFunctionofAirFlowFractionCurveName).get());
  EXPECT_EQ("HeatCapfWaterFlow",
            idf_coil.getString(Coil_WaterHeating_AirToWaterHeatPump_PumpedFields::HeatingCapacityFunctionofWaterFlowFractionCurveName).get());
  EXPECT_EQ("HeatCOPFT", idf_coil.getString(Coil_WaterHeating_AirToWaterHeatPump_PumpedFields::HeatingCOPFunctionofTemperatureCurveName).get());
  EXPECT_EQ("HeatCOPfAirFlow",
            idf_coil.getString(Coil_WaterHeating_AirToWaterHeatPump_PumpedFields::HeatingCOPFunctionofAirFlowFractionCurveName).get());
  EXPECT_EQ("HeatCOPfWaterFlow",
            idf_coil.getString(Coil_WaterHeating_AirToWaterHeatPump_PumpedFields::HeatingCOPFunctionofWaterFlowFractionCurveName).get());
  EXPECT_EQ("CoilPLFCurve", idf_coil.getString(Coil_WaterHeating_AirToWaterHeatPump_PumpedFields::PartLoadFractionCorrelationCurveName).get());
}
