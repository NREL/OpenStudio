/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2019, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
*  following conditions are met:
*
*  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
*  disclaimer.
*
*  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
*  disclaimer in the documentation and/or other materials provided with the distribution.
*
*  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote products
*  derived from this software without specific prior written permission from the respective party.
*
*  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative works
*  may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without specific prior
*  written permission from Alliance for Sustainable Energy, LLC.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER(S) AND ANY CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
*  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
*  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER(S), ANY CONTRIBUTORS, THE UNITED STATES GOVERNMENT, OR THE UNITED
*  STATES DEPARTMENT OF ENERGY, NOR ANY OF THEIR EMPLOYEES, BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
*  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
*  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
*  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
*  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
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
  EXPECT_EQ("Normalized",powerModule.efficiencyCurveMode());
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
  EXPECT_EQ(tz, powerModule.zone());
  EXPECT_EQ(inletNode, powerModule.dilutionInletAirNode());
  EXPECT_EQ(outletNode, powerModule.dilutionOutletAirNode());
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
  EXPECT_EQ(tz, powerModule.zone());
  EXPECT_EQ(inletNode, powerModule.dilutionInletAirNode());
  EXPECT_EQ(outletNode, powerModule.dilutionOutletAirNode());
  EXPECT_EQ(skinlossCurve, powerModule.skinLossQuadraticCurve());
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
  EXPECT_EQ(tz, powerModule.zone());
  EXPECT_EQ(inletNode, powerModule.dilutionInletAirNode());
  EXPECT_EQ(outletNode, powerModule.dilutionOutletAirNode());

}
