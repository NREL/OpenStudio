/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2022, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

#include "ModelFixture.hpp"

#include "../HeatPumpAirToWaterFuelFiredHeating.hpp"
#include "../HeatPumpAirToWaterFuelFiredHeating_Impl.hpp"

// TODO: Check the following class names against object getters and setters.
#include "../Connection.hpp"
#include "../Connection_Impl.hpp"

#include "../Connection.hpp"
#include "../Connection_Impl.hpp"

#include "../OutdoorAirNode.hpp"
#include "../OutdoorAirNode_Impl.hpp"

#include "../HeatPumpAirToWaterFuelFiredCooling.hpp"
#include "../HeatPumpAirToWaterFuelFiredCooling_Impl.hpp"

#include "../BivariateFunctions.hpp"
#include "../BivariateFunctions_Impl.hpp"

#include "../BivariateFunctions.hpp"
#include "../BivariateFunctions_Impl.hpp"

#include "../UnivariateFunctions.hpp"
#include "../UnivariateFunctions_Impl.hpp"

#include "../UnivariateFunctions.hpp"
#include "../UnivariateFunctions_Impl.hpp"

#include "../UnivariateFunctions.hpp"
#include "../UnivariateFunctions_Impl.hpp"

#include "../BivariateFunctions.hpp"
#include "../BivariateFunctions_Impl.hpp"

#include "../UnivariateFunctions.hpp"
#include "../UnivariateFunctions_Impl.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, HeatPumpAirToWaterFuelFiredHeating_GettersSetters) {
  Model m;
  // TODO: Check regular Ctor arguments
  HeatPumpAirToWaterFuelFiredHeating heatPumpAirToWaterFuelFiredHeating(m);
  // TODO: Or if a UniqueModelObject (and make sure _Impl is included)
  // HeatPumpAirToWaterFuelFiredHeating heatPumpAirToWaterFuelFiredHeating = m.getUniqueModelObject<HeatPumpAirToWaterFuelFiredHeating>();

  heatPumpAirToWaterFuelFiredHeating.setName("My HeatPumpAirToWaterFuelFiredHeating");

  // Water Inlet Node Name: Required Object
  Connection obj(m);
  EXPECT_TRUE(heatPumpAirToWaterFuelFiredHeating.setWaterInletNode(obj));
  EXPECT_EQ(obj, heatPumpAirToWaterFuelFiredHeating.waterInletNode());

  // Water Outlet Node Name: Required Object
  Connection obj(m);
  EXPECT_TRUE(heatPumpAirToWaterFuelFiredHeating.setWaterOutletNode(obj));
  EXPECT_EQ(obj, heatPumpAirToWaterFuelFiredHeating.waterOutletNode());

  // Air Source Node Name: Optional Object
  boost::optional<OutdoorAirNode> obj(m);
  EXPECT_TRUE(heatPumpAirToWaterFuelFiredHeating.setAirSourceNode(obj));
  ASSERT_TRUE(heatPumpAirToWaterFuelFiredHeating.airSourceNode());
  EXPECT_EQ(obj, heatPumpAirToWaterFuelFiredHeating.airSourceNode().get());

  // Companion Cooling Heat Pump Name: Optional Object
  boost::optional<HeatPumpAirToWaterFuelFiredCooling> obj(m);
  EXPECT_TRUE(heatPumpAirToWaterFuelFiredHeating.setCompanionCoolingHeatPump(obj));
  ASSERT_TRUE(heatPumpAirToWaterFuelFiredHeating.companionCoolingHeatPump());
  EXPECT_EQ(obj, heatPumpAirToWaterFuelFiredHeating.companionCoolingHeatPump().get());

  // Fuel Type: Required String
  EXPECT_TRUE(heatPumpAirToWaterFuelFiredHeating.setFuelType("NaturalGas"));
  EXPECT_EQ("NaturalGas", heatPumpAirToWaterFuelFiredHeating.fuelType());
  // Bad Value
  EXPECT_FALSE(heatPumpAirToWaterFuelFiredHeating.setFuelType("BADENUM"));
  EXPECT_EQ("NaturalGas", heatPumpAirToWaterFuelFiredHeating.fuelType());

  // End-Use Subcategory: Optional String
  // Default value from IDD
  EXPECT_TRUE(heatPumpAirToWaterFuelFiredHeating.isEndUseSubcategoryDefaulted());
  EXPECT_EQ("General", heatPumpAirToWaterFuelFiredHeating.endUseSubcategory());
  // Set
  EXPECT_TRUE(heatPumpAirToWaterFuelFiredHeating.setEndUseSubcategory());
  EXPECT_EQ(, heatPumpAirToWaterFuelFiredHeating.endUseSubcategory());
  EXPECT_FALSE(heatPumpAirToWaterFuelFiredHeating.isEndUseSubcategoryDefaulted());
  // Reset
  heatPumpAirToWaterFuelFiredHeating.resetEndUseSubcategory();
  EXPECT_TRUE(heatPumpAirToWaterFuelFiredHeating.isEndUseSubcategoryDefaulted());

  // Nominal Heating Capacity: Optional Double
  // Autosize
  heatPumpAirToWaterFuelFiredHeating.autosizeNominalHeatingCapacity();
  EXPECT_TRUE(heatPumpAirToWaterFuelFiredHeating.isNominalHeatingCapacityAutosized());
  // Set
  EXPECT_TRUE(heatPumpAirToWaterFuelFiredHeating.setNominalHeatingCapacity(0.9));
  ASSERT_TRUE(heatPumpAirToWaterFuelFiredHeating.nominalHeatingCapacity());
  EXPECT_EQ(0.9, heatPumpAirToWaterFuelFiredHeating.nominalHeatingCapacity().get());
  // Bad Value
  EXPECT_FALSE(heatPumpAirToWaterFuelFiredHeating.setNominalHeatingCapacity(-10.0));
  ASSERT_TRUE(heatPumpAirToWaterFuelFiredHeating.nominalHeatingCapacity());
  EXPECT_EQ(0.9, heatPumpAirToWaterFuelFiredHeating.nominalHeatingCapacity().get());
  EXPECT_FALSE(heatPumpAirToWaterFuelFiredHeating.isNominalHeatingCapacityAutosized());

  // Nominal COP: Required Double
  EXPECT_TRUE(heatPumpAirToWaterFuelFiredHeating.setNominalCOP(1.0));
  EXPECT_EQ(1.0, heatPumpAirToWaterFuelFiredHeating.nominalCOP());
  // Bad Value
  EXPECT_FALSE(heatPumpAirToWaterFuelFiredHeating.setNominalCOP(-10.0));
  EXPECT_EQ(1.0, heatPumpAirToWaterFuelFiredHeating.nominalCOP());

  // Design Flow Rate: Optional Double
  // Autosize
  heatPumpAirToWaterFuelFiredHeating.autosizeDesignFlowRate();
  EXPECT_TRUE(heatPumpAirToWaterFuelFiredHeating.isDesignFlowRateAutosized());
  // Set
  EXPECT_TRUE(heatPumpAirToWaterFuelFiredHeating.setDesignFlowRate(1.1));
  ASSERT_TRUE(heatPumpAirToWaterFuelFiredHeating.designFlowRate());
  EXPECT_EQ(1.1, heatPumpAirToWaterFuelFiredHeating.designFlowRate().get());
  // Bad Value
  EXPECT_FALSE(heatPumpAirToWaterFuelFiredHeating.setDesignFlowRate(-10.0));
  ASSERT_TRUE(heatPumpAirToWaterFuelFiredHeating.designFlowRate());
  EXPECT_EQ(1.1, heatPumpAirToWaterFuelFiredHeating.designFlowRate().get());
  EXPECT_FALSE(heatPumpAirToWaterFuelFiredHeating.isDesignFlowRateAutosized());

  // Design Supply Temperature: Required Double
  EXPECT_TRUE(heatPumpAirToWaterFuelFiredHeating.setDesignSupplyTemperature(1.2));
  EXPECT_EQ(1.2, heatPumpAirToWaterFuelFiredHeating.designSupplyTemperature());

  // Design Temperature Lift: Required Double
  // Autosize
  heatPumpAirToWaterFuelFiredHeating.autosizeDesignTemperatureLift();
  EXPECT_TRUE(heatPumpAirToWaterFuelFiredHeating.isDesignTemperatureLiftAutosized());
  // Set
  EXPECT_TRUE(heatPumpAirToWaterFuelFiredHeating.setDesignTemperatureLift(1.3));
  ASSERT_TRUE(heatPumpAirToWaterFuelFiredHeating.designTemperatureLift());
  EXPECT_EQ(1.3, heatPumpAirToWaterFuelFiredHeating.designTemperatureLift().get());
  EXPECT_FALSE(heatPumpAirToWaterFuelFiredHeating.isDesignTemperatureLiftAutosized());

  // Sizing Factor: Required Double
  EXPECT_TRUE(heatPumpAirToWaterFuelFiredHeating.setSizingFactor(2.4));
  EXPECT_EQ(2.4, heatPumpAirToWaterFuelFiredHeating.sizingFactor());
  // Bad Value
  EXPECT_FALSE(heatPumpAirToWaterFuelFiredHeating.setSizingFactor(-9.0));
  EXPECT_EQ(2.4, heatPumpAirToWaterFuelFiredHeating.sizingFactor());

  // Flow Mode: Required String
  EXPECT_TRUE(heatPumpAirToWaterFuelFiredHeating.setFlowMode("NotModulated"));
  EXPECT_EQ("NotModulated", heatPumpAirToWaterFuelFiredHeating.flowMode());
  // Bad Value
  EXPECT_FALSE(heatPumpAirToWaterFuelFiredHeating.setFlowMode("BADENUM"));
  EXPECT_EQ("NotModulated", heatPumpAirToWaterFuelFiredHeating.flowMode());

  // Outdoor Air Temperature Curve Input Variable: Required String
  EXPECT_TRUE(heatPumpAirToWaterFuelFiredHeating.setOutdoorAirTemperatureCurveInputVariable("DryBulb"));
  EXPECT_EQ("DryBulb", heatPumpAirToWaterFuelFiredHeating.outdoorAirTemperatureCurveInputVariable());
  // Bad Value
  EXPECT_FALSE(heatPumpAirToWaterFuelFiredHeating.setOutdoorAirTemperatureCurveInputVariable("BADENUM"));
  EXPECT_EQ("DryBulb", heatPumpAirToWaterFuelFiredHeating.outdoorAirTemperatureCurveInputVariable());

  // Water Temperature Curve Input Variable: Required String
  EXPECT_TRUE(heatPumpAirToWaterFuelFiredHeating.setWaterTemperatureCurveInputVariable("EnteringCondenser"));
  EXPECT_EQ("EnteringCondenser", heatPumpAirToWaterFuelFiredHeating.waterTemperatureCurveInputVariable());
  // Bad Value
  EXPECT_FALSE(heatPumpAirToWaterFuelFiredHeating.setWaterTemperatureCurveInputVariable("BADENUM"));
  EXPECT_EQ("EnteringCondenser", heatPumpAirToWaterFuelFiredHeating.waterTemperatureCurveInputVariable());

  // Normalized Capacity Function of Temperature Curve Name: Required Object
  BivariateFunctions obj(m);
  EXPECT_TRUE(heatPumpAirToWaterFuelFiredHeating.setNormalizedCapacityFunctionofTemperatureCurve(obj));
  EXPECT_EQ(obj, heatPumpAirToWaterFuelFiredHeating.normalizedCapacityFunctionofTemperatureCurve());

  // Fuel Energy Input Ratio Function of Temperature Curve Name: Required Object
  BivariateFunctions obj(m);
  EXPECT_TRUE(heatPumpAirToWaterFuelFiredHeating.setFuelEnergyInputRatioFunctionofTemperatureCurve(obj));
  EXPECT_EQ(obj, heatPumpAirToWaterFuelFiredHeating.fuelEnergyInputRatioFunctionofTemperatureCurve());

  // Fuel Energy Input Ratio Function of PLR Curve Name: Required Object
  UnivariateFunctions obj(m);
  EXPECT_TRUE(heatPumpAirToWaterFuelFiredHeating.setFuelEnergyInputRatioFunctionofPLRCurve(obj));
  EXPECT_EQ(obj, heatPumpAirToWaterFuelFiredHeating.fuelEnergyInputRatioFunctionofPLRCurve());

  // Minimum Part Load Ratio: Required Double
  EXPECT_TRUE(heatPumpAirToWaterFuelFiredHeating.setMinimumPartLoadRatio(0.955));
  EXPECT_EQ(0.955, heatPumpAirToWaterFuelFiredHeating.minimumPartLoadRatio());
  // Bad Value
  EXPECT_FALSE(heatPumpAirToWaterFuelFiredHeating.setMinimumPartLoadRatio(-10.0));
  EXPECT_EQ(0.955, heatPumpAirToWaterFuelFiredHeating.minimumPartLoadRatio());

  // Maximum Part Load Ratio: Required Double
  EXPECT_TRUE(heatPumpAirToWaterFuelFiredHeating.setMaximumPartLoadRatio(0.957));
  EXPECT_EQ(0.957, heatPumpAirToWaterFuelFiredHeating.maximumPartLoadRatio());
  // Bad Value
  EXPECT_FALSE(heatPumpAirToWaterFuelFiredHeating.setMaximumPartLoadRatio(-10.0));
  EXPECT_EQ(0.957, heatPumpAirToWaterFuelFiredHeating.maximumPartLoadRatio());

  // Defrost Control Type: Required String
  EXPECT_TRUE(heatPumpAirToWaterFuelFiredHeating.setDefrostControlType("Timed"));
  EXPECT_EQ("Timed", heatPumpAirToWaterFuelFiredHeating.defrostControlType());
  // Bad Value
  EXPECT_FALSE(heatPumpAirToWaterFuelFiredHeating.setDefrostControlType("BADENUM"));
  EXPECT_EQ("Timed", heatPumpAirToWaterFuelFiredHeating.defrostControlType());

  // Defrost Operation Time Fraction: Required Double
  EXPECT_TRUE(heatPumpAirToWaterFuelFiredHeating.setDefrostOperationTimeFraction(0.96));
  EXPECT_EQ(0.96, heatPumpAirToWaterFuelFiredHeating.defrostOperationTimeFraction());
  // Bad Value
  EXPECT_FALSE(heatPumpAirToWaterFuelFiredHeating.setDefrostOperationTimeFraction(-10.0));
  EXPECT_EQ(0.96, heatPumpAirToWaterFuelFiredHeating.defrostOperationTimeFraction());

  // Fuel Energy Input Ratio Defrost Adjustment Curve Name: Optional Object
  boost::optional<UnivariateFunctions> obj(m);
  EXPECT_TRUE(heatPumpAirToWaterFuelFiredHeating.setFuelEnergyInputRatioDefrostAdjustmentCurve(obj));
  ASSERT_TRUE(heatPumpAirToWaterFuelFiredHeating.fuelEnergyInputRatioDefrostAdjustmentCurve());
  EXPECT_EQ(obj, heatPumpAirToWaterFuelFiredHeating.fuelEnergyInputRatioDefrostAdjustmentCurve().get());

  // Resistive Defrost Heater Capacity: Required Double
  EXPECT_TRUE(heatPumpAirToWaterFuelFiredHeating.setResistiveDefrostHeaterCapacity(2.6));
  EXPECT_EQ(2.6, heatPumpAirToWaterFuelFiredHeating.resistiveDefrostHeaterCapacity());
  // Bad Value
  EXPECT_FALSE(heatPumpAirToWaterFuelFiredHeating.setResistiveDefrostHeaterCapacity(-10.0));
  EXPECT_EQ(2.6, heatPumpAirToWaterFuelFiredHeating.resistiveDefrostHeaterCapacity());

  // Maximum Outdoor Dry-bulb Temperature for Defrost Operation: Required Double
  EXPECT_TRUE(heatPumpAirToWaterFuelFiredHeating.setMaximumOutdoorDrybulbTemperatureforDefrostOperation(9.643));
  EXPECT_EQ(9.643, heatPumpAirToWaterFuelFiredHeating.maximumOutdoorDrybulbTemperatureforDefrostOperation());
  // Bad Value
  EXPECT_FALSE(heatPumpAirToWaterFuelFiredHeating.setMaximumOutdoorDrybulbTemperatureforDefrostOperation(-10.0));
  EXPECT_EQ(9.643, heatPumpAirToWaterFuelFiredHeating.maximumOutdoorDrybulbTemperatureforDefrostOperation());

  // Cycling Ratio Factor Curve Name: Optional Object
  boost::optional<UnivariateFunctions> obj(m);
  EXPECT_TRUE(heatPumpAirToWaterFuelFiredHeating.setCyclingRatioFactorCurve(obj));
  ASSERT_TRUE(heatPumpAirToWaterFuelFiredHeating.cyclingRatioFactorCurve());
  EXPECT_EQ(obj, heatPumpAirToWaterFuelFiredHeating.cyclingRatioFactorCurve().get());

  // Nominal Auxiliary Electric Power: Optional Double
  EXPECT_TRUE(heatPumpAirToWaterFuelFiredHeating.setNominalAuxiliaryElectricPower(2.9));
  ASSERT_TRUE(heatPumpAirToWaterFuelFiredHeating.nominalAuxiliaryElectricPower());
  EXPECT_EQ(2.9, heatPumpAirToWaterFuelFiredHeating.nominalAuxiliaryElectricPower().get());
  // Bad Value
  EXPECT_FALSE(heatPumpAirToWaterFuelFiredHeating.setNominalAuxiliaryElectricPower(-10.0));
  ASSERT_TRUE(heatPumpAirToWaterFuelFiredHeating.nominalAuxiliaryElectricPower());
  EXPECT_EQ(2.9, heatPumpAirToWaterFuelFiredHeating.nominalAuxiliaryElectricPower().get());

  // Auxiliary Electric Energy Input Ratio Function of Temperature Curve Name: Optional Object
  boost::optional<BivariateFunctions> obj(m);
  EXPECT_TRUE(heatPumpAirToWaterFuelFiredHeating.setAuxiliaryElectricEnergyInputRatioFunctionofTemperatureCurve(obj));
  ASSERT_TRUE(heatPumpAirToWaterFuelFiredHeating.auxiliaryElectricEnergyInputRatioFunctionofTemperatureCurve());
  EXPECT_EQ(obj, heatPumpAirToWaterFuelFiredHeating.auxiliaryElectricEnergyInputRatioFunctionofTemperatureCurve().get());

  // Auxiliary Electric Energy Input Ratio Function of PLR Curve Name: Optional Object
  boost::optional<UnivariateFunctions> obj(m);
  EXPECT_TRUE(heatPumpAirToWaterFuelFiredHeating.setAuxiliaryElectricEnergyInputRatioFunctionofPLRCurve(obj));
  ASSERT_TRUE(heatPumpAirToWaterFuelFiredHeating.auxiliaryElectricEnergyInputRatioFunctionofPLRCurve());
  EXPECT_EQ(obj, heatPumpAirToWaterFuelFiredHeating.auxiliaryElectricEnergyInputRatioFunctionofPLRCurve().get());

  // Standby Electric Power: Required Double
  EXPECT_TRUE(heatPumpAirToWaterFuelFiredHeating.setStandbyElectricPower(3.2));
  EXPECT_EQ(3.2, heatPumpAirToWaterFuelFiredHeating.standbyElectricPower());
  // Bad Value
  EXPECT_FALSE(heatPumpAirToWaterFuelFiredHeating.setStandbyElectricPower(-10.0));
  EXPECT_EQ(3.2, heatPumpAirToWaterFuelFiredHeating.standbyElectricPower());
}
