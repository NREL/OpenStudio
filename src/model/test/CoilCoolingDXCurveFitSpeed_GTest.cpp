/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2020, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

#include "../CoilCoolingDXCurveFitSpeed.hpp"
#include "../CoilCoolingDXCurveFitSpeed_Impl.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, CoilCoolingDXCurveFitSpeed_GettersSetters) {
  Model m;
  // TODO: Check regular Ctor arguments
  CoilCoolingDXCurveFitSpeed coilCoolingDXCurveFitSpeed(m);
  // TODO: Or if a UniqueModelObject (and make sure _Impl is included)
  // CoilCoolingDXCurveFitSpeed coilCoolingDXCurveFitSpeed = m.getUniqueModelObject<CoilCoolingDXCurveFitSpeed>();

  coilCoolingDXCurveFitSpeed.setName("My CoilCoolingDXCurveFitSpeed");

  // Gross Total Cooling Capacity Fraction: Required Double
  EXPECT_TRUE(coilCoolingDXCurveFitSpeed.setGrossTotalCoolingCapacityFraction(0.1));
  EXPECT_EQ(0.1, coilCoolingDXCurveFitSpeed.grossTotalCoolingCapacityFraction());
   Bad Value
  EXPECT_FALSE(coilCoolingDXCurveFitSpeed.setGrossTotalCoolingCapacityFraction(-10.0));
  EXPECT_EQ(0.1, coilCoolingDXCurveFitSpeed.grossTotalCoolingCapacityFraction());

  // Evaporator Air Flow Rate Fraction: Required Double
  EXPECT_TRUE(coilCoolingDXCurveFitSpeed.setEvaporatorAirFlowRateFraction(0.1));
  EXPECT_EQ(0.1, coilCoolingDXCurveFitSpeed.evaporatorAirFlowRateFraction());
   Bad Value
  EXPECT_FALSE(coilCoolingDXCurveFitSpeed.setEvaporatorAirFlowRateFraction(-10.0));
  EXPECT_EQ(0.1, coilCoolingDXCurveFitSpeed.evaporatorAirFlowRateFraction());

  // Condenser Air Flow Rate Fraction: Optional Double
  EXPECT_TRUE(coilCoolingDXCurveFitSpeed.setCondenserAirFlowRateFraction(0.1));
  ASSERT_TRUE(coilCoolingDXCurveFitSpeed.condenserAirFlowRateFraction());
  EXPECT_EQ(0.1, coilCoolingDXCurveFitSpeed.condenserAirFlowRateFraction().get());
   Bad Value
  EXPECT_FALSE(coilCoolingDXCurveFitSpeed.setCondenserAirFlowRateFraction(-10.0));
  ASSERT_TRUE(coilCoolingDXCurveFitSpeed.condenserAirFlowRateFraction());
  EXPECT_EQ(0.1, coilCoolingDXCurveFitSpeed.condenserAirFlowRateFraction().get());

  // Gross Sensible Heat Ratio: Required Double
  coilCoolingDXCurveFitSpeed.autosizeGrossSensibleHeatRatio();
  EXPECT_TRUE(coilCoolingDXCurveFitSpeed.isGrossSensibleHeatRatioAutosized());
  EXPECT_TRUE(coilCoolingDXCurveFitSpeed.setGrossSensibleHeatRatio(0.55));
  ASSERT_TRUE(coilCoolingDXCurveFitSpeed.grossSensibleHeatRatio());
  EXPECT_EQ(0.55, coilCoolingDXCurveFitSpeed.grossSensibleHeatRatio().get());
   Bad Value
  EXPECT_FALSE(coilCoolingDXCurveFitSpeed.setGrossSensibleHeatRatio(-9.9));
  ASSERT_TRUE(coilCoolingDXCurveFitSpeed.grossSensibleHeatRatio());
  EXPECT_EQ(0.55, coilCoolingDXCurveFitSpeed.grossSensibleHeatRatio().get());
  EXPECT_FALSE(coilCoolingDXCurveFitSpeed.isGrossSensibleHeatRatioAutosized());

  // Gross Cooling COP: Required Double
  EXPECT_TRUE(coilCoolingDXCurveFitSpeed.setGrossCoolingCOP(0.1));
  EXPECT_EQ(0.1, coilCoolingDXCurveFitSpeed.grossCoolingCOP());
   Bad Value
  EXPECT_FALSE(coilCoolingDXCurveFitSpeed.setGrossCoolingCOP(-10.0));
  EXPECT_EQ(0.1, coilCoolingDXCurveFitSpeed.grossCoolingCOP());

  // Active Fraction of Coil Face Area: Optional Double
  EXPECT_TRUE(coilCoolingDXCurveFitSpeed.setActiveFractionofCoilFaceArea(0.5));
  EXPECT_EQ(0.5, coilCoolingDXCurveFitSpeed.activeFractionofCoilFaceArea());
   Bad Value
  EXPECT_FALSE(coilCoolingDXCurveFitSpeed.setActiveFractionofCoilFaceArea(-10.0));
  EXPECT_EQ(0.5, coilCoolingDXCurveFitSpeed.activeFractionofCoilFaceArea());

  // Rated Evaporator Fan Power Per Volume Flow Rate: Required Double
  EXPECT_TRUE(coilCoolingDXCurveFitSpeed.setRatedEvaporatorFanPowerPerVolumeFlowRate(625.0));
  EXPECT_EQ(625.0, coilCoolingDXCurveFitSpeed.ratedEvaporatorFanPowerPerVolumeFlowRate());
   Bad Value
  EXPECT_FALSE(coilCoolingDXCurveFitSpeed.setRatedEvaporatorFanPowerPerVolumeFlowRate(-10.0));
  EXPECT_EQ(625.0, coilCoolingDXCurveFitSpeed.ratedEvaporatorFanPowerPerVolumeFlowRate());

  // Evaporative Condenser Pump Power Fraction: Optional Double
  EXPECT_TRUE(coilCoolingDXCurveFitSpeed.setEvaporativeCondenserPumpPowerFraction(0.1));
  EXPECT_EQ(0.1, coilCoolingDXCurveFitSpeed.evaporativeCondenserPumpPowerFraction());
   Bad Value
  EXPECT_FALSE(coilCoolingDXCurveFitSpeed.setEvaporativeCondenserPumpPowerFraction(-10.0));
  EXPECT_EQ(0.1, coilCoolingDXCurveFitSpeed.evaporativeCondenserPumpPowerFraction());

  // Evaporative Condenser Effectiveness: Required Double
  EXPECT_TRUE(coilCoolingDXCurveFitSpeed.setEvaporativeCondenserEffectiveness(0.5));
  EXPECT_EQ(0.5, coilCoolingDXCurveFitSpeed.evaporativeCondenserEffectiveness());
   Bad Value
  EXPECT_FALSE(coilCoolingDXCurveFitSpeed.setEvaporativeCondenserEffectiveness(-10.0));
  EXPECT_EQ(0.5, coilCoolingDXCurveFitSpeed.evaporativeCondenserEffectiveness());

  // Total Cooling Capacity Modifier Function of Temperature Curve: Optional Object
  boost::optional<UnivariateFunctions> obj(m);
  EXPECT_TRUE(coilCoolingDXCurveFitSpeed.setTotalCoolingCapacityModifierFunctionofTemperatureCurve(obj));
  ASSERT_TRUE(coilCoolingDXCurveFitSpeed.totalCoolingCapacityModifierFunctionofTemperatureCurve());
  EXPECT_EQ(obj, coilCoolingDXCurveFitSpeed.totalCoolingCapacityModifierFunctionofTemperatureCurve().get());

  // Total Cooling Capacity Modifier Function of Air Flow Fraction Curve: Optional Object
  boost::optional<UnivariateFunctions> obj(m);
  EXPECT_TRUE(coilCoolingDXCurveFitSpeed.setTotalCoolingCapacityModifierFunctionofAirFlowFractionCurve(obj));
  ASSERT_TRUE(coilCoolingDXCurveFitSpeed.totalCoolingCapacityModifierFunctionofAirFlowFractionCurve());
  EXPECT_EQ(obj, coilCoolingDXCurveFitSpeed.totalCoolingCapacityModifierFunctionofAirFlowFractionCurve().get());

  // Energy Input Ratio Modifier Function of Temperature Curve: Optional Object
  boost::optional<UnivariateFunctions> obj(m);
  EXPECT_TRUE(coilCoolingDXCurveFitSpeed.setEnergyInputRatioModifierFunctionofTemperatureCurve(obj));
  ASSERT_TRUE(coilCoolingDXCurveFitSpeed.energyInputRatioModifierFunctionofTemperatureCurve());
  EXPECT_EQ(obj, coilCoolingDXCurveFitSpeed.energyInputRatioModifierFunctionofTemperatureCurve().get());

  // Energy Input Ratio Modifier Function of Air Flow Fraction Curve: Optional Object
  boost::optional<UnivariateFunctions> obj(m);
  EXPECT_TRUE(coilCoolingDXCurveFitSpeed.setEnergyInputRatioModifierFunctionofAirFlowFractionCurve(obj));
  ASSERT_TRUE(coilCoolingDXCurveFitSpeed.energyInputRatioModifierFunctionofAirFlowFractionCurve());
  EXPECT_EQ(obj, coilCoolingDXCurveFitSpeed.energyInputRatioModifierFunctionofAirFlowFractionCurve().get());

  // Part Load Fraction Correlation Curve: Optional Object
  boost::optional<UnivariateFunctions> obj(m);
  EXPECT_TRUE(coilCoolingDXCurveFitSpeed.setPartLoadFractionCorrelationCurve(obj));
  ASSERT_TRUE(coilCoolingDXCurveFitSpeed.partLoadFractionCorrelationCurve());
  EXPECT_EQ(obj, coilCoolingDXCurveFitSpeed.partLoadFractionCorrelationCurve().get());

  // Rated Waste Heat Fraction of Power Input: Required Double
  EXPECT_TRUE(coilCoolingDXCurveFitSpeed.setRatedWasteHeatFractionofPowerInput(0.5));
  EXPECT_EQ(0.5, coilCoolingDXCurveFitSpeed.ratedWasteHeatFractionofPowerInput());
   Bad Value
  EXPECT_FALSE(coilCoolingDXCurveFitSpeed.setRatedWasteHeatFractionofPowerInput(-10.0));
  EXPECT_EQ(0.5, coilCoolingDXCurveFitSpeed.ratedWasteHeatFractionofPowerInput());

  // Waste Heat Modifier Function of Temperature Curve: Optional Object
  boost::optional<BivariateFunctions> obj(m);
  EXPECT_TRUE(coilCoolingDXCurveFitSpeed.setWasteHeatModifierFunctionofTemperatureCurve(obj));
  ASSERT_TRUE(coilCoolingDXCurveFitSpeed.wasteHeatModifierFunctionofTemperatureCurve());
  EXPECT_EQ(obj, coilCoolingDXCurveFitSpeed.wasteHeatModifierFunctionofTemperatureCurve().get());

  // Sensible Heat Ratio Modifier Function of Temperature Curve: Optional Object
  boost::optional<BivariateFunctions> obj(m);
  EXPECT_TRUE(coilCoolingDXCurveFitSpeed.setSensibleHeatRatioModifierFunctionofTemperatureCurve(obj));
  ASSERT_TRUE(coilCoolingDXCurveFitSpeed.sensibleHeatRatioModifierFunctionofTemperatureCurve());
  EXPECT_EQ(obj, coilCoolingDXCurveFitSpeed.sensibleHeatRatioModifierFunctionofTemperatureCurve().get());

  // Sensible Heat Ratio Modifier Function of Flow Fraction Curve: Optional Object
  boost::optional<UnivariateFunctions> obj(m);
  EXPECT_TRUE(coilCoolingDXCurveFitSpeed.setSensibleHeatRatioModifierFunctionofFlowFractionCurve(obj));
  ASSERT_TRUE(coilCoolingDXCurveFitSpeed.sensibleHeatRatioModifierFunctionofFlowFractionCurve());
  EXPECT_EQ(obj, coilCoolingDXCurveFitSpeed.sensibleHeatRatioModifierFunctionofFlowFractionCurve().get());

