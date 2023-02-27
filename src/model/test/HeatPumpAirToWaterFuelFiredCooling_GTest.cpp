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

#include "../HeatPumpAirToWaterFuelFiredCooling.hpp"
#include "../HeatPumpAirToWaterFuelFiredCooling_Impl.hpp"

// TODO: Check the following class names against object getters and setters.
#include "../Connection.hpp"
#include "../Connection_Impl.hpp"

#include "../Connection.hpp"
#include "../Connection_Impl.hpp"

#include "../OutdoorAirNode.hpp"
#include "../OutdoorAirNode_Impl.hpp"

#include "../HeatPumpAirToWaterFuelFiredHeating.hpp"
#include "../HeatPumpAirToWaterFuelFiredHeating_Impl.hpp"

#include "../BivariateFunctions.hpp"
#include "../BivariateFunctions_Impl.hpp"

#include "../BivariateFunctions.hpp"
#include "../BivariateFunctions_Impl.hpp"

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

TEST_F(ModelFixture, HeatPumpAirToWaterFuelFiredCooling_GettersSetters) {
  Model m;
  // TODO: Check regular Ctor arguments
  HeatPumpAirToWaterFuelFiredCooling heatPumpAirToWaterFuelFiredCooling(m);
  // TODO: Or if a UniqueModelObject (and make sure _Impl is included)
  // HeatPumpAirToWaterFuelFiredCooling heatPumpAirToWaterFuelFiredCooling = m.getUniqueModelObject<HeatPumpAirToWaterFuelFiredCooling>();

  heatPumpAirToWaterFuelFiredCooling.setName("My HeatPumpAirToWaterFuelFiredCooling");

  // Water Inlet Node Name: Required Object
  Connection obj(m);
  EXPECT_TRUE(heatPumpAirToWaterFuelFiredCooling.setWaterInletNode(obj));
  EXPECT_EQ(obj, heatPumpAirToWaterFuelFiredCooling.waterInletNode());

  // Water Outlet Node Name: Required Object
  Connection obj(m);
  EXPECT_TRUE(heatPumpAirToWaterFuelFiredCooling.setWaterOutletNode(obj));
  EXPECT_EQ(obj, heatPumpAirToWaterFuelFiredCooling.waterOutletNode());

  // Air Source Node Name: Optional Object
  boost::optional<OutdoorAirNode> obj(m);
  EXPECT_TRUE(heatPumpAirToWaterFuelFiredCooling.setAirSourceNode(obj));
  ASSERT_TRUE(heatPumpAirToWaterFuelFiredCooling.airSourceNode());
  EXPECT_EQ(obj, heatPumpAirToWaterFuelFiredCooling.airSourceNode().get());

  // Companion Heating Heat Pump Name: Optional Object
  boost::optional<HeatPumpAirToWaterFuelFiredHeating> obj(m);
  EXPECT_TRUE(heatPumpAirToWaterFuelFiredCooling.setCompanionHeatingHeatPump(obj));
  ASSERT_TRUE(heatPumpAirToWaterFuelFiredCooling.companionHeatingHeatPump());
  EXPECT_EQ(obj, heatPumpAirToWaterFuelFiredCooling.companionHeatingHeatPump().get());

  // Fuel Type: Required String
  EXPECT_TRUE(heatPumpAirToWaterFuelFiredCooling.setFuelType("NaturalGas"));
  EXPECT_EQ("NaturalGas", heatPumpAirToWaterFuelFiredCooling.fuelType());
  // Bad Value
  EXPECT_FALSE(heatPumpAirToWaterFuelFiredCooling.setFuelType("BADENUM"));
  EXPECT_EQ("NaturalGas", heatPumpAirToWaterFuelFiredCooling.fuelType());

  // End-Use Subcategory: Optional String
  // Default value from IDD
  EXPECT_TRUE(heatPumpAirToWaterFuelFiredCooling.isEndUseSubcategoryDefaulted());
  EXPECT_EQ("General", heatPumpAirToWaterFuelFiredCooling.endUseSubcategory());
  // Set
  EXPECT_TRUE(heatPumpAirToWaterFuelFiredCooling.setEndUseSubcategory());
  EXPECT_EQ(, heatPumpAirToWaterFuelFiredCooling.endUseSubcategory());
  EXPECT_FALSE(heatPumpAirToWaterFuelFiredCooling.isEndUseSubcategoryDefaulted());
  // Reset
  heatPumpAirToWaterFuelFiredCooling.resetEndUseSubcategory();
  EXPECT_TRUE(heatPumpAirToWaterFuelFiredCooling.isEndUseSubcategoryDefaulted());

  // Nominal Cooling Capacity: Optional Double
  // Autosize
  heatPumpAirToWaterFuelFiredCooling.autosizeNominalCoolingCapacity();
  EXPECT_TRUE(heatPumpAirToWaterFuelFiredCooling.isNominalCoolingCapacityAutosized());
  // Set
  EXPECT_TRUE(heatPumpAirToWaterFuelFiredCooling.setNominalCoolingCapacity(0.9));
  ASSERT_TRUE(heatPumpAirToWaterFuelFiredCooling.nominalCoolingCapacity());
  EXPECT_EQ(0.9, heatPumpAirToWaterFuelFiredCooling.nominalCoolingCapacity().get());
  // Bad Value
  EXPECT_FALSE(heatPumpAirToWaterFuelFiredCooling.setNominalCoolingCapacity(-10.0));
  ASSERT_TRUE(heatPumpAirToWaterFuelFiredCooling.nominalCoolingCapacity());
  EXPECT_EQ(0.9, heatPumpAirToWaterFuelFiredCooling.nominalCoolingCapacity().get());
  EXPECT_FALSE(heatPumpAirToWaterFuelFiredCooling.isNominalCoolingCapacityAutosized());

  // Nominal COP: Required Double
  EXPECT_TRUE(heatPumpAirToWaterFuelFiredCooling.setNominalCOP(1.0));
  EXPECT_EQ(1.0, heatPumpAirToWaterFuelFiredCooling.nominalCOP());
  // Bad Value
  EXPECT_FALSE(heatPumpAirToWaterFuelFiredCooling.setNominalCOP(-10.0));
  EXPECT_EQ(1.0, heatPumpAirToWaterFuelFiredCooling.nominalCOP());

  // Design Flow Rate: Optional Double
  // Autosize
  heatPumpAirToWaterFuelFiredCooling.autosizeDesignFlowRate();
  EXPECT_TRUE(heatPumpAirToWaterFuelFiredCooling.isDesignFlowRateAutosized());
  // Set
  EXPECT_TRUE(heatPumpAirToWaterFuelFiredCooling.setDesignFlowRate(1.1));
  ASSERT_TRUE(heatPumpAirToWaterFuelFiredCooling.designFlowRate());
  EXPECT_EQ(1.1, heatPumpAirToWaterFuelFiredCooling.designFlowRate().get());
  // Bad Value
  EXPECT_FALSE(heatPumpAirToWaterFuelFiredCooling.setDesignFlowRate(-10.0));
  ASSERT_TRUE(heatPumpAirToWaterFuelFiredCooling.designFlowRate());
  EXPECT_EQ(1.1, heatPumpAirToWaterFuelFiredCooling.designFlowRate().get());
  EXPECT_FALSE(heatPumpAirToWaterFuelFiredCooling.isDesignFlowRateAutosized());

  // Design Supply Temperature: Required Double
  EXPECT_TRUE(heatPumpAirToWaterFuelFiredCooling.setDesignSupplyTemperature(1.2));
  EXPECT_EQ(1.2, heatPumpAirToWaterFuelFiredCooling.designSupplyTemperature());

  // Design Temperature Lift: Required Double
  // Autosize
  heatPumpAirToWaterFuelFiredCooling.autosizeDesignTemperatureLift();
  EXPECT_TRUE(heatPumpAirToWaterFuelFiredCooling.isDesignTemperatureLiftAutosized());
  // Set
  EXPECT_TRUE(heatPumpAirToWaterFuelFiredCooling.setDesignTemperatureLift(1.3));
  ASSERT_TRUE(heatPumpAirToWaterFuelFiredCooling.designTemperatureLift());
  EXPECT_EQ(1.3, heatPumpAirToWaterFuelFiredCooling.designTemperatureLift().get());
  EXPECT_FALSE(heatPumpAirToWaterFuelFiredCooling.isDesignTemperatureLiftAutosized());

  // Sizing Factor: Required Double
  EXPECT_TRUE(heatPumpAirToWaterFuelFiredCooling.setSizingFactor(2.4));
  EXPECT_EQ(2.4, heatPumpAirToWaterFuelFiredCooling.sizingFactor());
  // Bad Value
  EXPECT_FALSE(heatPumpAirToWaterFuelFiredCooling.setSizingFactor(-9.0));
  EXPECT_EQ(2.4, heatPumpAirToWaterFuelFiredCooling.sizingFactor());

  // Flow Mode: Required String
  EXPECT_TRUE(heatPumpAirToWaterFuelFiredCooling.setFlowMode("NotModulated"));
  EXPECT_EQ("NotModulated", heatPumpAirToWaterFuelFiredCooling.flowMode());
  // Bad Value
  EXPECT_FALSE(heatPumpAirToWaterFuelFiredCooling.setFlowMode("BADENUM"));
  EXPECT_EQ("NotModulated", heatPumpAirToWaterFuelFiredCooling.flowMode());

  // Outdoor Air Temperature Curve Input Variable: Required String
  EXPECT_TRUE(heatPumpAirToWaterFuelFiredCooling.setOutdoorAirTemperatureCurveInputVariable("DryBulb"));
  EXPECT_EQ("DryBulb", heatPumpAirToWaterFuelFiredCooling.outdoorAirTemperatureCurveInputVariable());
  // Bad Value
  EXPECT_FALSE(heatPumpAirToWaterFuelFiredCooling.setOutdoorAirTemperatureCurveInputVariable("BADENUM"));
  EXPECT_EQ("DryBulb", heatPumpAirToWaterFuelFiredCooling.outdoorAirTemperatureCurveInputVariable());

  // Water Temperature Curve Input Variable: Required String
  EXPECT_TRUE(heatPumpAirToWaterFuelFiredCooling.setWaterTemperatureCurveInputVariable("EnteringEvaporator"));
  EXPECT_EQ("EnteringEvaporator", heatPumpAirToWaterFuelFiredCooling.waterTemperatureCurveInputVariable());
  // Bad Value
  EXPECT_FALSE(heatPumpAirToWaterFuelFiredCooling.setWaterTemperatureCurveInputVariable("BADENUM"));
  EXPECT_EQ("EnteringEvaporator", heatPumpAirToWaterFuelFiredCooling.waterTemperatureCurveInputVariable());

  // Normalized Capacity Function of Temperature Curve Name: Required Object
  BivariateFunctions obj(m);
  EXPECT_TRUE(heatPumpAirToWaterFuelFiredCooling.setNormalizedCapacityFunctionofTemperatureCurve(obj));
  EXPECT_EQ(obj, heatPumpAirToWaterFuelFiredCooling.normalizedCapacityFunctionofTemperatureCurve());

  // Fuel Energy Input Ratio Function of Temperature Curve Name: Required Object
  BivariateFunctions obj(m);
  EXPECT_TRUE(heatPumpAirToWaterFuelFiredCooling.setFuelEnergyInputRatioFunctionofTemperatureCurve(obj));
  EXPECT_EQ(obj, heatPumpAirToWaterFuelFiredCooling.fuelEnergyInputRatioFunctionofTemperatureCurve());

  // Fuel Energy Input Ratio Function of PLR Curve Name: Required Object
  UnivariateFunctions obj(m);
  EXPECT_TRUE(heatPumpAirToWaterFuelFiredCooling.setFuelEnergyInputRatioFunctionofPLRCurve(obj));
  EXPECT_EQ(obj, heatPumpAirToWaterFuelFiredCooling.fuelEnergyInputRatioFunctionofPLRCurve());

  // Minimum Part Load Ratio: Required Double
  EXPECT_TRUE(heatPumpAirToWaterFuelFiredCooling.setMinimumPartLoadRatio(0.955));
  EXPECT_EQ(0.955, heatPumpAirToWaterFuelFiredCooling.minimumPartLoadRatio());
  // Bad Value
  EXPECT_FALSE(heatPumpAirToWaterFuelFiredCooling.setMinimumPartLoadRatio(-10.0));
  EXPECT_EQ(0.955, heatPumpAirToWaterFuelFiredCooling.minimumPartLoadRatio());

  // Maximum Part Load Ratio: Required Double
  EXPECT_TRUE(heatPumpAirToWaterFuelFiredCooling.setMaximumPartLoadRatio(0.957));
  EXPECT_EQ(0.957, heatPumpAirToWaterFuelFiredCooling.maximumPartLoadRatio());
  // Bad Value
  EXPECT_FALSE(heatPumpAirToWaterFuelFiredCooling.setMaximumPartLoadRatio(-10.0));
  EXPECT_EQ(0.957, heatPumpAirToWaterFuelFiredCooling.maximumPartLoadRatio());

  // Cycling Ratio Factor Curve Name: Optional Object
  boost::optional<UnivariateFunctions> obj(m);
  EXPECT_TRUE(heatPumpAirToWaterFuelFiredCooling.setCyclingRatioFactorCurve(obj));
  ASSERT_TRUE(heatPumpAirToWaterFuelFiredCooling.cyclingRatioFactorCurve());
  EXPECT_EQ(obj, heatPumpAirToWaterFuelFiredCooling.cyclingRatioFactorCurve().get());

  // Nominal Auxiliary Electric Power: Optional Double
  EXPECT_TRUE(heatPumpAirToWaterFuelFiredCooling.setNominalAuxiliaryElectricPower(2.4));
  ASSERT_TRUE(heatPumpAirToWaterFuelFiredCooling.nominalAuxiliaryElectricPower());
  EXPECT_EQ(2.4, heatPumpAirToWaterFuelFiredCooling.nominalAuxiliaryElectricPower().get());
  // Bad Value
  EXPECT_FALSE(heatPumpAirToWaterFuelFiredCooling.setNominalAuxiliaryElectricPower(-10.0));
  ASSERT_TRUE(heatPumpAirToWaterFuelFiredCooling.nominalAuxiliaryElectricPower());
  EXPECT_EQ(2.4, heatPumpAirToWaterFuelFiredCooling.nominalAuxiliaryElectricPower().get());

  // Auxiliary Electric Energy Input Ratio Function of Temperature Curve Name: Optional Object
  boost::optional<BivariateFunctions> obj(m);
  EXPECT_TRUE(heatPumpAirToWaterFuelFiredCooling.setAuxiliaryElectricEnergyInputRatioFunctionofTemperatureCurve(obj));
  ASSERT_TRUE(heatPumpAirToWaterFuelFiredCooling.auxiliaryElectricEnergyInputRatioFunctionofTemperatureCurve());
  EXPECT_EQ(obj, heatPumpAirToWaterFuelFiredCooling.auxiliaryElectricEnergyInputRatioFunctionofTemperatureCurve().get());

  // Auxiliary Electric Energy Input Ratio Function of PLR Curve Name: Optional Object
  boost::optional<UnivariateFunctions> obj(m);
  EXPECT_TRUE(heatPumpAirToWaterFuelFiredCooling.setAuxiliaryElectricEnergyInputRatioFunctionofPLRCurve(obj));
  ASSERT_TRUE(heatPumpAirToWaterFuelFiredCooling.auxiliaryElectricEnergyInputRatioFunctionofPLRCurve());
  EXPECT_EQ(obj, heatPumpAirToWaterFuelFiredCooling.auxiliaryElectricEnergyInputRatioFunctionofPLRCurve().get());

  // Standby Electric Power: Required Double
  EXPECT_TRUE(heatPumpAirToWaterFuelFiredCooling.setStandbyElectricPower(2.7));
  EXPECT_EQ(2.7, heatPumpAirToWaterFuelFiredCooling.standbyElectricPower());
  // Bad Value
  EXPECT_FALSE(heatPumpAirToWaterFuelFiredCooling.setStandbyElectricPower(-10.0));
  EXPECT_EQ(2.7, heatPumpAirToWaterFuelFiredCooling.standbyElectricPower());
}
