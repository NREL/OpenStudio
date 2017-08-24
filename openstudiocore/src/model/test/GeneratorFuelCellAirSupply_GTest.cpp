/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2016, Alliance for Sustainable Energy, LLC. All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 *  following conditions are met:
 *
 *  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
 *  disclaimer.
 *
 *  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *  following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 *  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote
 *  products derived from this software without specific prior written permission from the respective party.
 *
 *  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative
 *  works may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without
 *  specific prior written permission from Alliance for Sustainable Energy, LLC.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER, THE UNITED STATES GOVERNMENT, OR ANY CONTRIBUTORS BE LIABLE FOR
 *  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **********************************************************************************************************************/

#include <gtest/gtest.h>

#include <model/test/ModelFixture.hpp>

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
  EXPECT_EQ(0 ,blowerCurve.get().coefficient1Constant());
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
  //should fail since name is wrong
  ASSERT_FALSE(airSupply.addConstituent("MadeUp", 0.0092));
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
  EXPECT_EQ(airInletNode, airSupply.airInletNode());
  airSupply.resetAirInletNode();
  ASSERT_FALSE(airSupply.airInletNode());
  ASSERT_TRUE(airSupply.setAirInletNode(airInletNode));
  EXPECT_EQ(airInletNode, airSupply.airInletNode());
  CurveQuadratic curveQ1(model);
  ASSERT_TRUE(airSupply.setAirRateFunctionofElectricPowerCurve(curveQ1));
  EXPECT_EQ(curveQ1, airSupply.airRateFunctionofElectricPowerCurve());
  CurveQuadratic curveQ2(model);
  ASSERT_TRUE(airSupply.setAirRateFunctionofFuelRateCurve(curveQ2));
  EXPECT_EQ(curveQ2, airSupply.airRateFunctionofFuelRateCurve());
  CurveCubic blowerCurve(model);
  ASSERT_TRUE(airSupply.setBlowerPowerCurve(blowerCurve));
  airSupply.resetBlowerPowerCurve();
  ASSERT_FALSE(airSupply.blowerPowerCurve());
  airSupply.resetAirRateFunctionofElectricPowerCurve();
  ASSERT_FALSE(airSupply.airRateFunctionofElectricPowerCurve());
  airSupply.resetAirRateFunctionofFuelRateCurve();
  ASSERT_FALSE(airSupply.airRateFunctionofFuelRateCurve());
}

