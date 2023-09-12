/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>
#include "EnergyPlusFixture.hpp"

#include "../ForwardTranslator.hpp"

#include "../../model/Model.hpp"
#include "../../model/CoilWaterHeatingAirToWaterHeatPumpWrapped.hpp"
#include "../../model/CoilWaterHeatingAirToWaterHeatPumpWrapped_Impl.hpp"
#include "../../model/Schedule.hpp"
#include "../../model/Curve.hpp"
#include "../../model/CurveLinear.hpp"
#include "../../model/WaterHeaterHeatPumpWrappedCondenser.hpp"
#include "../../model/ThermalZone.hpp"
#include "../../model/Space.hpp"

#include <utilities/idd/Coil_WaterHeating_AirToWaterHeatPump_Wrapped_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include "../../utilities/idf/IdfExtensibleGroup.hpp"
#include "../../utilities/idf/WorkspaceExtensibleGroup.hpp"

using namespace openstudio::energyplus;
using namespace openstudio::model;
using namespace openstudio;

TEST_F(EnergyPlusFixture, ForwardTranslator_CoilWaterHeatingAirToWaterHeatPumpWrapped) {
  Model m;

  CoilWaterHeatingAirToWaterHeatPumpWrapped coil(m);
  coil.setName("My CoilWaterHeatingAirToWaterHeatPumpWrapped");

  EXPECT_TRUE(coil.setRatedHeatingCapacity(4000.0));
  EXPECT_TRUE(coil.setRatedCOP(3.2));
  EXPECT_TRUE(coil.setRatedSensibleHeatRatio(0.6956));
  EXPECT_TRUE(coil.setRatedEvaporatorInletAirDryBulbTemperature(28.0));
  EXPECT_TRUE(coil.setRatedEvaporatorInletAirWetBulbTemperature(21.00));
  EXPECT_TRUE(coil.setRatedCondenserWaterTemperature(52.00));
  EXPECT_TRUE(coil.setRatedEvaporatorAirFlowRate(1.0));
  EXPECT_TRUE(coil.setEvaporatorFanPowerIncludedinRatedCOP(false));

  EXPECT_TRUE(coil.setCrankcaseHeaterCapacity(105.0));
  CurveLinear crankCurve(m);
  crankCurve.setName("CrankCapFT");
  EXPECT_TRUE(coil.setCrankcaseHeaterCapacityFunctionofTemperatureCurve(crankCurve));
  EXPECT_TRUE(coil.setMaximumAmbientTemperatureforCrankcaseHeaterOperation(9.0));

  EXPECT_TRUE(coil.setEvaporatorAirTemperatureTypeforCurveObjects("DryBulbTemperature"));
  coil.heatingCapacityFunctionofTemperatureCurve().setName("HeatCapFT");
  coil.heatingCapacityFunctionofAirFlowFractionCurve().setName("HeatCapfAirFlow");

  coil.heatingCOPFunctionofTemperatureCurve().setName("HeatCOPFT");
  coil.heatingCOPFunctionofAirFlowFractionCurve().setName("HeatCOPfAirFlow");

  coil.partLoadFractionCorrelationCurve().setName("CoilPLFCurve");

  WaterHeaterHeatPumpWrappedCondenser hpwh(m);
  ThermalZone tz(m);
  Space space(m);
  space.setThermalZone(tz);

  hpwh.setDXCoil(coil);
  hpwh.addToThermalZone(tz);

  ForwardTranslator ft;
  const Workspace w = ft.translateModel(m);

  EXPECT_EQ(1, w.getObjectsByType(IddObjectType::WaterHeater_HeatPump_WrappedCondenser).size());
  EXPECT_EQ(7, w.getObjectsByType(IddObjectType::Curve_Cubic).size());
  EXPECT_EQ(4, w.getObjectsByType(IddObjectType::Curve_Biquadratic).size());
  EXPECT_EQ(1, w.getObjectsByType(IddObjectType::Curve_Exponent).size());
  EXPECT_EQ(1, w.getObjectsByType(IddObjectType::Curve_Linear).size());

  auto idfs_coils = w.getObjectsByType(IddObjectType::Coil_WaterHeating_AirToWaterHeatPump_Wrapped);
  ASSERT_EQ(1, idfs_coils.size());
  const WorkspaceObject& idf_coil = idfs_coils[0];

  EXPECT_EQ("My CoilWaterHeatingAirToWaterHeatPumpWrapped", idf_coil.getString(Coil_WaterHeating_AirToWaterHeatPump_WrappedFields::Name).get());
  EXPECT_EQ(4000.0, idf_coil.getDouble(Coil_WaterHeating_AirToWaterHeatPump_WrappedFields::RatedHeatingCapacity).get());
  EXPECT_EQ(3.2, idf_coil.getDouble(Coil_WaterHeating_AirToWaterHeatPump_WrappedFields::RatedCOP).get());
  EXPECT_EQ(0.6956, idf_coil.getDouble(Coil_WaterHeating_AirToWaterHeatPump_WrappedFields::RatedSensibleHeatRatio).get());
  EXPECT_EQ(28.0, idf_coil.getDouble(Coil_WaterHeating_AirToWaterHeatPump_WrappedFields::RatedEvaporatorInletAirDryBulbTemperature).get());
  EXPECT_EQ(21.0, idf_coil.getDouble(Coil_WaterHeating_AirToWaterHeatPump_WrappedFields::RatedEvaporatorInletAirWetBulbTemperature).get());
  EXPECT_EQ(52.0, idf_coil.getDouble(Coil_WaterHeating_AirToWaterHeatPump_WrappedFields::RatedCondenserWaterTemperature).get());
  EXPECT_EQ(1.0, idf_coil.getDouble(Coil_WaterHeating_AirToWaterHeatPump_WrappedFields::RatedEvaporatorAirFlowRate).get());
  EXPECT_EQ("No", idf_coil.getString(Coil_WaterHeating_AirToWaterHeatPump_WrappedFields::EvaporatorFanPowerIncludedinRatedCOP).get());
  EXPECT_FALSE(idf_coil.isEmpty(Coil_WaterHeating_AirToWaterHeatPump_WrappedFields::EvaporatorAirInletNodeName));
  EXPECT_FALSE(idf_coil.isEmpty(Coil_WaterHeating_AirToWaterHeatPump_WrappedFields::EvaporatorAirOutletNodeName));
  EXPECT_EQ(105.0, idf_coil.getDouble(Coil_WaterHeating_AirToWaterHeatPump_WrappedFields::CrankcaseHeaterCapacity).get());
  EXPECT_EQ("CrankCapFT",
            idf_coil.getString(Coil_WaterHeating_AirToWaterHeatPump_WrappedFields::CrankcaseHeaterCapacityFunctionofTemperatureCurveName).get());
  EXPECT_EQ(9.0, idf_coil.getDouble(Coil_WaterHeating_AirToWaterHeatPump_WrappedFields::MaximumAmbientTemperatureforCrankcaseHeaterOperation).get());
  EXPECT_EQ("DryBulbTemperature",
            idf_coil.getString(Coil_WaterHeating_AirToWaterHeatPump_WrappedFields::EvaporatorAirTemperatureTypeforCurveObjects).get());
  EXPECT_EQ("HeatCapFT", idf_coil.getString(Coil_WaterHeating_AirToWaterHeatPump_WrappedFields::HeatingCapacityFunctionofTemperatureCurveName).get());
  EXPECT_EQ("HeatCapfAirFlow",
            idf_coil.getString(Coil_WaterHeating_AirToWaterHeatPump_WrappedFields::HeatingCapacityFunctionofAirFlowFractionCurveName).get());
  EXPECT_EQ("HeatCOPFT", idf_coil.getString(Coil_WaterHeating_AirToWaterHeatPump_WrappedFields::HeatingCOPFunctionofTemperatureCurveName).get());
  EXPECT_EQ("HeatCOPfAirFlow",
            idf_coil.getString(Coil_WaterHeating_AirToWaterHeatPump_WrappedFields::HeatingCOPFunctionofAirFlowFractionCurveName).get());
  EXPECT_EQ("CoilPLFCurve", idf_coil.getString(Coil_WaterHeating_AirToWaterHeatPump_WrappedFields::PartLoadFractionCorrelationCurveName).get());
}
