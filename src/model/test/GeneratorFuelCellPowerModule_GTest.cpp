/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>

#include "ModelFixture.hpp"

#include "../GeneratorFuelCellPowerModule.hpp"
#include "../GeneratorFuelCellPowerModule_Impl.hpp"

#include "../CurveQuadratic.hpp"
#include "../CurveQuadratic_Impl.hpp"
#include "../Node.hpp"
#include "../Node_Impl.hpp"
#include "../ThermalZone.hpp"
#include "../ThermalZone_Impl.hpp"

using namespace openstudio;
using namespace openstudio::model;
using std::string;

TEST_F(ModelFixture, FuelCellPowerModule) {
  Model model;

  // check default PowerModule
  CurveQuadratic curve(model);
  GeneratorFuelCellPowerModule powerModule(model, curve);
  EXPECT_EQ(curve, powerModule.efficiencyCurve());
  EXPECT_EQ("Annex42", powerModule.efficiencyCurveMode());
  EXPECT_EQ(1.0, powerModule.nominalEfficiency());
  EXPECT_EQ(3400, powerModule.nominalElectricalPower());
  EXPECT_EQ(0, powerModule.numberofStopsatStartofSimulation());
  EXPECT_EQ(0.0, powerModule.cyclingPerformanceDegradationCoefficient());
  EXPECT_EQ(0, powerModule.numberofRunHoursatBeginningofSimulation());
  EXPECT_EQ(0.0, powerModule.accumulatedRunTimeDegradationCoefficient());
  EXPECT_EQ(10000, powerModule.runTimeDegradationInitiationTimeThreshold());
  EXPECT_EQ(1.4, powerModule.powerUpTransientLimit());
  EXPECT_EQ(0.2, powerModule.powerDownTransientLimit());
  EXPECT_EQ(0.0, powerModule.startUpTime());
  EXPECT_EQ(0.2, powerModule.startUpFuel());
  EXPECT_EQ(0, powerModule.startUpElectricityConsumption());
  EXPECT_EQ(0.0, powerModule.startUpElectricityProduced());
  EXPECT_EQ(0.0, powerModule.shutDownTime());
  EXPECT_EQ(0.2, powerModule.shutDownFuel());
  EXPECT_EQ(0.0, powerModule.shutDownElectricityConsumption());
  EXPECT_EQ(0.0, powerModule.ancillaryElectricityConstantTerm());
  EXPECT_EQ(0.0, powerModule.ancillaryElectricityLinearTerm());
  EXPECT_EQ("ConstantRate", powerModule.skinLossCalculationMode());
  EXPECT_EQ(0.6392, powerModule.skinLossRadiativeFraction());
  EXPECT_EQ(729, powerModule.constantSkinLossRate());
  EXPECT_EQ(0.0, powerModule.skinLossUFactorTimesAreaTerm());
  EXPECT_EQ(0.006156, powerModule.dilutionAirFlowRate());
  EXPECT_EQ(2307, powerModule.stackHeatlosstoDilutionAir());
  EXPECT_EQ(3010, powerModule.minimumOperatingPoint());
  EXPECT_EQ(3728, powerModule.maximumOperatingPoint());
  //change efficiency curve mode
  ASSERT_TRUE(powerModule.setEfficiencyCurveMode("Normalized"));
  EXPECT_EQ("Normalized", powerModule.efficiencyCurveMode());
  ASSERT_FALSE(powerModule.setEfficiencyCurveMode("MAdeup"));
  powerModule.resetEfficiencyCurveMode();
  EXPECT_EQ("Annex42", powerModule.efficiencyCurveMode());
}

TEST_F(ModelFixture, FuelCellPowerModule2) {
  Model model;

  // check default PowerModule
  CurveQuadratic curve(model);
  Node inletNode(model);
  Node outletNode(model);
  ThermalZone tz(model);
  GeneratorFuelCellPowerModule powerModule(model, curve, tz, inletNode, outletNode);
  EXPECT_EQ(tz, powerModule.zone().get());
  EXPECT_EQ(inletNode, powerModule.dilutionInletAirNode().get());
  EXPECT_EQ(outletNode, powerModule.dilutionOutletAirNode().get());
  //reset nodes
  powerModule.resetDilutionInletAirNode();
  powerModule.resetDilutionOutletAirNode();
  ASSERT_FALSE(powerModule.dilutionInletAirNode());
  ASSERT_FALSE(powerModule.dilutionOutletAirNode());
}

TEST_F(ModelFixture, FuelCellPowerModule3) {
  Model model;

  // check default PowerModule
  CurveQuadratic curve(model);
  CurveQuadratic skinlossCurve(model);
  Node inletNode(model);
  Node outletNode(model);
  ThermalZone tz(model);
  GeneratorFuelCellPowerModule powerModule(model, curve, tz, inletNode, outletNode, skinlossCurve);
  EXPECT_EQ(tz, powerModule.zone().get());
  EXPECT_EQ(inletNode, powerModule.dilutionInletAirNode().get());
  EXPECT_EQ(outletNode, powerModule.dilutionOutletAirNode().get());
  EXPECT_EQ(skinlossCurve, powerModule.skinLossQuadraticCurve().get());
  //reset skinloss curve
  powerModule.resetSkinLossQuadraticCurve();
  ASSERT_FALSE(powerModule.skinLossQuadraticCurve());
}

TEST_F(ModelFixture, FuelCellPowerModule4) {
  Model model;

  // check default PowerModule
  Node inletNode(model);
  Node outletNode(model);
  ThermalZone tz(model);
  GeneratorFuelCellPowerModule powerModule(model, tz, inletNode, outletNode);
  EXPECT_EQ(tz, powerModule.zone().get());
  EXPECT_EQ(inletNode, powerModule.dilutionInletAirNode().get());
  EXPECT_EQ(outletNode, powerModule.dilutionOutletAirNode().get());
}
