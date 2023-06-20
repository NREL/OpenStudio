/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>

#include "ModelFixture.hpp"

#include "../GeneratorFuelCellAirSupply.hpp"
#include "../GeneratorFuelCellAirSupply_Impl.hpp"
#include "../CurveCubic.hpp"
#include "../CurveCubic_Impl.hpp"
#include "../CurveQuadratic.hpp"
#include "../CurveQuadratic_Impl.hpp"
#include "../Node.hpp"
#include "../Node_Impl.hpp"

using namespace openstudio;
using namespace openstudio::model;
using std::string;

TEST_F(ModelFixture, FuelCellAirSupply) {
  Model model;

  // check default Airsupply
  GeneratorFuelCellAirSupply airSupply(model);
  boost::optional<CurveCubic> blowerCurve = airSupply.blowerPowerCurve();
  ASSERT_TRUE(blowerCurve);
  EXPECT_EQ(0, blowerCurve.get().coefficient1Constant());
  EXPECT_EQ(0, blowerCurve.get().coefficient2x());
  EXPECT_EQ(0, blowerCurve.get().coefficient3xPOW2());
  EXPECT_EQ(0, blowerCurve.get().coefficient4xPOW3());
  EXPECT_EQ(-1.0e10, blowerCurve.get().minimumValueofx());
  EXPECT_EQ(1.0e10, blowerCurve.get().maximumValueofx());
  EXPECT_EQ(1.0, airSupply.blowerHeatLossFactor());
  EXPECT_EQ("AirRatiobyStoics", airSupply.airSupplyRateCalculationMode());
  EXPECT_EQ(1.0, airSupply.stoichiometricRatio().get());
  EXPECT_EQ(0.00283, airSupply.airRateAirTemperatureCoefficient().get());
  EXPECT_EQ("NoRecovery", airSupply.airIntakeHeatRecoveryMode());
  EXPECT_EQ("AmbientAir", airSupply.airSupplyConstituentMode());
  boost::optional<Node> airInletNode = airSupply.airInletNode();
  ASSERT_FALSE(airInletNode);
  boost::optional<CurveQuadratic> electricCurve = airSupply.airRateFunctionofElectricPowerCurve();
  ASSERT_TRUE(electricCurve);
  boost::optional<CurveQuadratic> fuelCurve = airSupply.airRateFunctionofFuelRateCurve();
  ASSERT_TRUE(fuelCurve);
  EXPECT_EQ(0, airSupply.numberofUserDefinedConstituents().get());

  //should throw in Ctor of AirSupplyConstituent since name is wrong
  EXPECT_THROW(airSupply.addConstituent("MadeUp", 0.0092), openstudio::Exception);
  EXPECT_EQ(0, airSupply.numberofUserDefinedConstituents().get());

  ASSERT_TRUE(airSupply.addConstituent("CarbonDioxide", 0.0003));
  EXPECT_EQ(1, airSupply.numberofUserDefinedConstituents().get());
  ASSERT_TRUE(airSupply.addConstituent("Nitrogen", 0.7728));
  EXPECT_EQ(2, airSupply.numberofUserDefinedConstituents().get());
  ASSERT_TRUE(airSupply.addConstituent("Oxygen", 0.2073));
  EXPECT_EQ(3, airSupply.numberofUserDefinedConstituents().get());
  ASSERT_TRUE(airSupply.addConstituent("Water", 0.0104));
  EXPECT_EQ(4, airSupply.numberofUserDefinedConstituents().get());
  ASSERT_TRUE(airSupply.addConstituent("Argon", 0.0092));
  EXPECT_EQ(5, airSupply.numberofUserDefinedConstituents().get());
  //should fail since only 5 allowed
  ASSERT_FALSE(airSupply.addConstituent("Argon", 0.0092));
  airSupply.removeConstituent(3);
  ASSERT_TRUE(airSupply.addConstituent("Argon", 0.0092));
  EXPECT_EQ(5, airSupply.numberofUserDefinedConstituents().get());
  airSupply.removeAllConstituents();
  EXPECT_EQ(0, airSupply.numberofUserDefinedConstituents().get());
  airSupply.removeConstituent(0);
  EXPECT_EQ(0, airSupply.numberofUserDefinedConstituents().get());
}

TEST_F(ModelFixture, FuelCellAirSupply2) {
  Model model;

  Node airInletNode(model);
  GeneratorFuelCellAirSupply airSupply(model, airInletNode);
  EXPECT_EQ(airInletNode, airSupply.airInletNode().get());
  airSupply.resetAirInletNode();
  ASSERT_FALSE(airSupply.airInletNode());
  ASSERT_TRUE(airSupply.setAirInletNode(airInletNode));
  EXPECT_EQ(airInletNode, airSupply.airInletNode().get());
  CurveQuadratic curveQ1(model);
  ASSERT_TRUE(airSupply.setAirRateFunctionofElectricPowerCurve(curveQ1));
  EXPECT_EQ(curveQ1, airSupply.airRateFunctionofElectricPowerCurve().get());
  CurveQuadratic curveQ2(model);
  ASSERT_TRUE(airSupply.setAirRateFunctionofFuelRateCurve(curveQ2));
  EXPECT_EQ(curveQ2, airSupply.airRateFunctionofFuelRateCurve().get());
  CurveCubic blowerCurve(model);
  ASSERT_TRUE(airSupply.setBlowerPowerCurve(blowerCurve));
  airSupply.resetBlowerPowerCurve();
  ASSERT_FALSE(airSupply.blowerPowerCurve());
  airSupply.resetAirRateFunctionofElectricPowerCurve();
  ASSERT_FALSE(airSupply.airRateFunctionofElectricPowerCurve());
  airSupply.resetAirRateFunctionofFuelRateCurve();
  ASSERT_FALSE(airSupply.airRateFunctionofFuelRateCurve());
}
