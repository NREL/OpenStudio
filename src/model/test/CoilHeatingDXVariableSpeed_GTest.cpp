/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>
#include "ModelFixture.hpp"
#include "../CoilHeatingDXVariableSpeed.hpp"
#include "../CoilHeatingDXVariableSpeed_Impl.hpp"
#include "../Curve.hpp"
#include "../Curve_Impl.hpp"
#include "../CurveLinear.hpp"
#include "../CurveBiquadratic.hpp"
#include "../CoilHeatingDXVariableSpeedSpeedData.hpp"
#include "../CoilHeatingDXVariableSpeedSpeedData_Impl.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, CoilHeatingDXVariableSpeed) {

  Model m;
  CoilHeatingDXVariableSpeed coil(m);

  // Indoor Air Inlet Node Name: Required Object
  // Indoor Air Outlet Node Name: Required Object

  // Nominal Speed Level: Required Integer
  EXPECT_EQ(1, coil.nominalSpeedLevel());
  EXPECT_TRUE(coil.setNominalSpeedLevel(5));
  EXPECT_EQ(5, coil.nominalSpeedLevel());

  // Rated Heating Capacity At Selected Nominal Speed Level: Required Double
  EXPECT_TRUE(coil.isRatedHeatingCapacityAtSelectedNominalSpeedLevelAutosized());
  // Set
  EXPECT_TRUE(coil.setRatedHeatingCapacityAtSelectedNominalSpeedLevel(1000.6));
  ASSERT_TRUE(coil.ratedHeatingCapacityAtSelectedNominalSpeedLevel());
  EXPECT_EQ(1000.6, coil.ratedHeatingCapacityAtSelectedNominalSpeedLevel().get());
  EXPECT_FALSE(coil.isRatedHeatingCapacityAtSelectedNominalSpeedLevelAutosized());
  // Autosize
  coil.autosizeRatedHeatingCapacityAtSelectedNominalSpeedLevel();
  EXPECT_TRUE(coil.isRatedHeatingCapacityAtSelectedNominalSpeedLevelAutosized());

  // Rated Air Flow Rate At Selected Nominal Speed Level: Required Double
  // Set
  EXPECT_TRUE(coil.setRatedAirFlowRateAtSelectedNominalSpeedLevel(0.7));
  ASSERT_TRUE(coil.ratedAirFlowRateAtSelectedNominalSpeedLevel());
  EXPECT_EQ(0.7, coil.ratedAirFlowRateAtSelectedNominalSpeedLevel().get());
  EXPECT_FALSE(coil.isRatedAirFlowRateAtSelectedNominalSpeedLevelAutosized());
  // Autosize
  coil.autosizeRatedAirFlowRateAtSelectedNominalSpeedLevel();
  EXPECT_TRUE(coil.isRatedAirFlowRateAtSelectedNominalSpeedLevelAutosized());

  // Energy Part Load Fraction Curve Name: Required Object
  EXPECT_NO_THROW(coil.energyPartLoadFractionCurve());
  CurveLinear energyPartLoadFractionCurve(m);
  EXPECT_TRUE(coil.setEnergyPartLoadFractionCurve(energyPartLoadFractionCurve));
  EXPECT_EQ(energyPartLoadFractionCurve, coil.energyPartLoadFractionCurve());

  // Defrost Energy Input Ratio Function of Temperature Curve Name: Optional Object
  EXPECT_FALSE(coil.defrostEnergyInputRatioFunctionofTemperatureCurve());
  CurveBiquadratic defrostEnergyInputRatioFunctionofTemperatureCurve(m);
  EXPECT_TRUE(coil.setDefrostEnergyInputRatioFunctionofTemperatureCurve(defrostEnergyInputRatioFunctionofTemperatureCurve));
  ASSERT_TRUE(coil.defrostEnergyInputRatioFunctionofTemperatureCurve());
  EXPECT_EQ(defrostEnergyInputRatioFunctionofTemperatureCurve, coil.defrostEnergyInputRatioFunctionofTemperatureCurve().get());

  // Minimum Outdoor Dry-Bulb Temperature for Compressor Operation: Required Double
  EXPECT_EQ(-5.0, coil.minimumOutdoorDryBulbTemperatureforCompressorOperation());
  EXPECT_TRUE(coil.setMinimumOutdoorDryBulbTemperatureforCompressorOperation(-49.0));
  EXPECT_EQ(-49.0, coil.minimumOutdoorDryBulbTemperatureforCompressorOperation());
  // Bad Value
  EXPECT_FALSE(coil.setMinimumOutdoorDryBulbTemperatureforCompressorOperation(-1000.0));
  EXPECT_EQ(-49.0, coil.minimumOutdoorDryBulbTemperatureforCompressorOperation());

  // Outdoor Dry-Bulb Temperature to Turn On Compressor: Optional Double
  EXPECT_FALSE(coil.outdoorDryBulbTemperaturetoTurnOnCompressor());
  EXPECT_TRUE(coil.setOutdoorDryBulbTemperaturetoTurnOnCompressor(1.1));
  ASSERT_TRUE(coil.outdoorDryBulbTemperaturetoTurnOnCompressor());
  EXPECT_EQ(1.1, coil.outdoorDryBulbTemperaturetoTurnOnCompressor().get());
  coil.resetOutdoorDryBulbTemperaturetoTurnOnCompressor();
  EXPECT_FALSE(coil.outdoorDryBulbTemperaturetoTurnOnCompressor());

  // Maximum Outdoor Dry-Bulb Temperature for Defrost Operation: Required Double
  EXPECT_EQ(5.0, coil.maximumOutdoorDryBulbTemperatureforDefrostOperation());
  EXPECT_TRUE(coil.setMaximumOutdoorDryBulbTemperatureforDefrostOperation(6.665));
  EXPECT_EQ(6.665, coil.maximumOutdoorDryBulbTemperatureforDefrostOperation());
  // Bad Value
  EXPECT_FALSE(coil.setMaximumOutdoorDryBulbTemperatureforDefrostOperation(-100.0));
  EXPECT_EQ(6.665, coil.maximumOutdoorDryBulbTemperatureforDefrostOperation());

  // Crankcase Heater Capacity: Required Double
  EXPECT_EQ(200.0, coil.crankcaseHeaterCapacity());
  EXPECT_TRUE(coil.setCrankcaseHeaterCapacity(1.3));
  EXPECT_EQ(1.3, coil.crankcaseHeaterCapacity());
  // Bad Value
  EXPECT_FALSE(coil.setCrankcaseHeaterCapacity(-10.0));
  EXPECT_EQ(1.3, coil.crankcaseHeaterCapacity());

  // Crankcase Heater Capacity Function of Temperature Curve Name: Optional Object
  EXPECT_FALSE(coil.crankcaseHeaterCapacityFunctionofTemperatureCurve());
  CurveLinear crankCurve(m);
  EXPECT_TRUE(coil.setCrankcaseHeaterCapacityFunctionofTemperatureCurve(crankCurve));
  ASSERT_TRUE(coil.crankcaseHeaterCapacityFunctionofTemperatureCurve());
  EXPECT_EQ(crankCurve, coil.crankcaseHeaterCapacityFunctionofTemperatureCurve().get());
  coil.resetCrankcaseHeaterCapacityFunctionofTemperatureCurve();
  EXPECT_FALSE(coil.crankcaseHeaterCapacityFunctionofTemperatureCurve());

  // Maximum Outdoor Dry-Bulb Temperature for Crankcase Heater Operation: Required Double
  EXPECT_EQ(10.0, coil.maximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation());
  EXPECT_TRUE(coil.setMaximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation(1.5));
  EXPECT_EQ(1.5, coil.maximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation());
  // Bad Value
  EXPECT_FALSE(coil.setMaximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation(-10.0));
  EXPECT_EQ(1.5, coil.maximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation());

  // Defrost Strategy: Required String
  EXPECT_EQ("Resistive", coil.defrostStrategy());
  EXPECT_TRUE(coil.setDefrostStrategy("ReverseCycle"));
  EXPECT_EQ("ReverseCycle", coil.defrostStrategy());
  // Bad Value
  EXPECT_FALSE(coil.setDefrostStrategy("BADENUM"));
  EXPECT_EQ("ReverseCycle", coil.defrostStrategy());

  // Defrost Control: Required String
  EXPECT_EQ("OnDemand", coil.defrostControl());
  EXPECT_TRUE(coil.setDefrostControl("Timed"));
  EXPECT_EQ("Timed", coil.defrostControl());
  // Bad Value
  EXPECT_FALSE(coil.setDefrostControl("BADENUM"));
  EXPECT_EQ("Timed", coil.defrostControl());

  // Defrost Time Period Fraction: Required Double
  EXPECT_EQ(0.166667, coil.defrostTimePeriodFraction());
  EXPECT_TRUE(coil.setDefrostTimePeriodFraction(0.8));
  EXPECT_EQ(0.8, coil.defrostTimePeriodFraction());
  // Bad Value
  EXPECT_FALSE(coil.setDefrostTimePeriodFraction(-10.0));
  EXPECT_EQ(0.8, coil.defrostTimePeriodFraction());

  // Resistive Defrost Heater Capacity: Required Double
  EXPECT_TRUE(coil.isResistiveDefrostHeaterCapacityAutosized());
  // Set
  EXPECT_TRUE(coil.setResistiveDefrostHeaterCapacity(1.9));
  ASSERT_TRUE(coil.resistiveDefrostHeaterCapacity());
  EXPECT_EQ(1.9, coil.resistiveDefrostHeaterCapacity().get());
  // Bad Value
  EXPECT_FALSE(coil.setResistiveDefrostHeaterCapacity(-10.0));
  ASSERT_TRUE(coil.resistiveDefrostHeaterCapacity());
  EXPECT_EQ(1.9, coil.resistiveDefrostHeaterCapacity().get());
  EXPECT_FALSE(coil.isResistiveDefrostHeaterCapacityAutosized());
  // Autosize
  coil.autosizeResistiveDefrostHeaterCapacity();
  EXPECT_TRUE(coil.isResistiveDefrostHeaterCapacityAutosized());
}

TEST_F(ModelFixture, CoilHeatingDXVariableSpeed_Speeds) {
  Model m;
  CoilHeatingDXVariableSpeed coil(m);

  EXPECT_EQ(0, coil.speeds().size());

  CoilHeatingDXVariableSpeedSpeedData speed1(m);
  EXPECT_TRUE(coil.addSpeed(speed1));
  EXPECT_EQ(1, coil.speeds().size());
  EXPECT_EQ(speed1, coil.speeds().front());

  CoilHeatingDXVariableSpeedSpeedData speed2(m);
  coil.addSpeed(speed2);
  EXPECT_EQ(2, coil.speeds().size());
  EXPECT_EQ(speed1, coil.speeds().front());
  EXPECT_EQ(speed2, coil.speeds().back());
}

TEST_F(ModelFixture, CoilHeatingDXVariableSpeed_Remove) {
  Model m;
  auto count = m.modelObjects().size();

  CoilHeatingDXVariableSpeed coil(m);
  CoilHeatingDXVariableSpeedSpeedData speed1(m);
  EXPECT_TRUE(coil.addSpeed(speed1));
  coil.remove();

  auto curves = m.getModelObjects<model::Curve>();

  EXPECT_EQ(count, m.modelObjects().size() - curves.size());
}

TEST_F(ModelFixture, CoilHeatingDXVariableSpeed_HeatCoolFuelTypes) {
  Model m;
  CoilHeatingDXVariableSpeed coil(m);

  EXPECT_EQ(ComponentType(ComponentType::Heating), coil.componentType());
  EXPECT_EQ(0, coil.coolingFuelTypes().size());
  testFuelTypeEquality({FuelType::Electricity}, coil.heatingFuelTypes());
  testAppGFuelTypeEquality({AppGFuelType::HeatPump}, coil.appGHeatingFuelTypes());
}
