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

#include "../AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR.hpp"
#include "../AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl.hpp"

// TODO: Check the following class names against object getters and setters.
#include "../Schedule.hpp"
#include "../Schedule_Impl.hpp"

#include "../ModelObjectLists.hpp"
#include "../ModelObjectLists_Impl.hpp"

#include "../UnivariateFunctions.hpp"
#include "../UnivariateFunctions_Impl.hpp"

#include "../UnivariateFunctions.hpp"
#include "../UnivariateFunctions_Impl.hpp"

#include "../BivariateFunctions.hpp"
#include "../BivariateFunctions_Impl.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_GettersSetters) {
  Model m;
  // TODO: Check regular Ctor arguments
  AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR airConditionerVariableRefrigerantFlowFluidTemperatureControlHR(m);
  // TODO: Or if a UniqueModelObject (and make sure _Impl is included)
  // AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR airConditionerVariableRefrigerantFlowFluidTemperatureControlHR = m.getUniqueModelObject<AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR>();

  airConditionerVariableRefrigerantFlowFluidTemperatureControlHR.setName("My AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR");

  // Availability Schedule: Optional Object
  boost::optional<Schedule> obj(m);
  EXPECT_TRUE(airConditionerVariableRefrigerantFlowFluidTemperatureControlHR.setAvailabilitySchedule(obj));
  ASSERT_TRUE(airConditionerVariableRefrigerantFlowFluidTemperatureControlHR.availabilitySchedule());
  EXPECT_EQ(obj, airConditionerVariableRefrigerantFlowFluidTemperatureControlHR.availabilitySchedule().get());

  // Zone Terminal Unit List: Required Object
  ModelObjectLists obj(m);
  EXPECT_TRUE(airConditionerVariableRefrigerantFlowFluidTemperatureControlHR.setZoneTerminalUnitList(obj));
  EXPECT_EQ(obj, airConditionerVariableRefrigerantFlowFluidTemperatureControlHR.zoneTerminalUnitList());

  // Refrigerant Type: Required String
  EXPECT_TRUE(airConditionerVariableRefrigerantFlowFluidTemperatureControlHR.setRefrigerantType("R11"));
  EXPECT_EQ("R11", airConditionerVariableRefrigerantFlowFluidTemperatureControlHR.refrigerantType());
  // Bad Value
  EXPECT_FALSE(airConditionerVariableRefrigerantFlowFluidTemperatureControlHR.setRefrigerantType("BADENUM"));
  EXPECT_EQ("R11", airConditionerVariableRefrigerantFlowFluidTemperatureControlHR.refrigerantType());

  // Rated Evaporative Capacity: Required Double
  // Autosize
  airConditionerVariableRefrigerantFlowFluidTemperatureControlHR.autosizeRatedEvaporativeCapacity();
  EXPECT_TRUE(airConditionerVariableRefrigerantFlowFluidTemperatureControlHR.isRatedEvaporativeCapacityAutosized());
  // Set
  EXPECT_TRUE(airConditionerVariableRefrigerantFlowFluidTemperatureControlHR.setRatedEvaporativeCapacity(0.6));
  ASSERT_TRUE(airConditionerVariableRefrigerantFlowFluidTemperatureControlHR.ratedEvaporativeCapacity());
  EXPECT_EQ(0.6, airConditionerVariableRefrigerantFlowFluidTemperatureControlHR.ratedEvaporativeCapacity().get());
  // Bad Value
  EXPECT_FALSE(airConditionerVariableRefrigerantFlowFluidTemperatureControlHR.setRatedEvaporativeCapacity(-10.0));
  ASSERT_TRUE(airConditionerVariableRefrigerantFlowFluidTemperatureControlHR.ratedEvaporativeCapacity());
  EXPECT_EQ(0.6, airConditionerVariableRefrigerantFlowFluidTemperatureControlHR.ratedEvaporativeCapacity().get());
  EXPECT_FALSE(airConditionerVariableRefrigerantFlowFluidTemperatureControlHR.isRatedEvaporativeCapacityAutosized());

  // Rated Compressor Power Per Unit of Rated Evaporative Capacity: Required Double
  EXPECT_TRUE(airConditionerVariableRefrigerantFlowFluidTemperatureControlHR.setRatedCompressorPowerPerUnitofRatedEvaporativeCapacity(0.7));
  EXPECT_EQ(0.7, airConditionerVariableRefrigerantFlowFluidTemperatureControlHR.ratedCompressorPowerPerUnitofRatedEvaporativeCapacity());
  // Bad Value
  EXPECT_FALSE(airConditionerVariableRefrigerantFlowFluidTemperatureControlHR.setRatedCompressorPowerPerUnitofRatedEvaporativeCapacity(-10.0));
  EXPECT_EQ(0.7, airConditionerVariableRefrigerantFlowFluidTemperatureControlHR.ratedCompressorPowerPerUnitofRatedEvaporativeCapacity());

  // Minimum Outdoor Air Temperature in Cooling Only Mode: Required Double
  EXPECT_TRUE(airConditionerVariableRefrigerantFlowFluidTemperatureControlHR.setMinimumOutdoorAirTemperatureinCoolingOnlyMode(0.8));
  EXPECT_EQ(0.8, airConditionerVariableRefrigerantFlowFluidTemperatureControlHR.minimumOutdoorAirTemperatureinCoolingOnlyMode());

  // Maximum Outdoor Air Temperature in Cooling Only Mode: Required Double
  EXPECT_TRUE(airConditionerVariableRefrigerantFlowFluidTemperatureControlHR.setMaximumOutdoorAirTemperatureinCoolingOnlyMode(0.9));
  EXPECT_EQ(0.9, airConditionerVariableRefrigerantFlowFluidTemperatureControlHR.maximumOutdoorAirTemperatureinCoolingOnlyMode());

  // Minimum Outdoor Air Temperature in Heating Only Mode: Required Double
  EXPECT_TRUE(airConditionerVariableRefrigerantFlowFluidTemperatureControlHR.setMinimumOutdoorAirTemperatureinHeatingOnlyMode(1.0));
  EXPECT_EQ(1.0, airConditionerVariableRefrigerantFlowFluidTemperatureControlHR.minimumOutdoorAirTemperatureinHeatingOnlyMode());

  // Maximum Outdoor Air Temperature in Heating Only Mode: Required Double
  EXPECT_TRUE(airConditionerVariableRefrigerantFlowFluidTemperatureControlHR.setMaximumOutdoorAirTemperatureinHeatingOnlyMode(1.1));
  EXPECT_EQ(1.1, airConditionerVariableRefrigerantFlowFluidTemperatureControlHR.maximumOutdoorAirTemperatureinHeatingOnlyMode());

  // Minimum Outdoor Temperature in Heat Recovery Mode: Required Double
  EXPECT_TRUE(airConditionerVariableRefrigerantFlowFluidTemperatureControlHR.setMinimumOutdoorTemperatureinHeatRecoveryMode(1.2));
  EXPECT_EQ(1.2, airConditionerVariableRefrigerantFlowFluidTemperatureControlHR.minimumOutdoorTemperatureinHeatRecoveryMode());

  // Maximum Outdoor Temperature in Heat Recovery Mode: Required Double
  EXPECT_TRUE(airConditionerVariableRefrigerantFlowFluidTemperatureControlHR.setMaximumOutdoorTemperatureinHeatRecoveryMode(1.3));
  EXPECT_EQ(1.3, airConditionerVariableRefrigerantFlowFluidTemperatureControlHR.maximumOutdoorTemperatureinHeatRecoveryMode());

  // Refrigerant Temperature Control Algorithm for Indoor Unit: Required String
  EXPECT_TRUE(airConditionerVariableRefrigerantFlowFluidTemperatureControlHR.setRefrigerantTemperatureControlAlgorithmforIndoorUnit("ConstantTemp"));
  EXPECT_EQ("ConstantTemp", airConditionerVariableRefrigerantFlowFluidTemperatureControlHR.refrigerantTemperatureControlAlgorithmforIndoorUnit());
  // Bad Value
  EXPECT_FALSE(airConditionerVariableRefrigerantFlowFluidTemperatureControlHR.setRefrigerantTemperatureControlAlgorithmforIndoorUnit("BADENUM"));
  EXPECT_EQ("ConstantTemp", airConditionerVariableRefrigerantFlowFluidTemperatureControlHR.refrigerantTemperatureControlAlgorithmforIndoorUnit());

  // Reference Evaporating Temperature for Indoor Unit: Required Double
  EXPECT_TRUE(airConditionerVariableRefrigerantFlowFluidTemperatureControlHR.setReferenceEvaporatingTemperatureforIndoorUnit(1.5));
  EXPECT_EQ(1.5, airConditionerVariableRefrigerantFlowFluidTemperatureControlHR.referenceEvaporatingTemperatureforIndoorUnit());

  // Reference Condensing Temperature for Indoor Unit: Required Double
  EXPECT_TRUE(airConditionerVariableRefrigerantFlowFluidTemperatureControlHR.setReferenceCondensingTemperatureforIndoorUnit(1.6));
  EXPECT_EQ(1.6, airConditionerVariableRefrigerantFlowFluidTemperatureControlHR.referenceCondensingTemperatureforIndoorUnit());

  // Variable Evaporating Temperature Minimum for Indoor Unit: Required Double
  EXPECT_TRUE(airConditionerVariableRefrigerantFlowFluidTemperatureControlHR.setVariableEvaporatingTemperatureMinimumforIndoorUnit(1.7));
  EXPECT_EQ(1.7, airConditionerVariableRefrigerantFlowFluidTemperatureControlHR.variableEvaporatingTemperatureMinimumforIndoorUnit());

  // Variable Evaporating Temperature Maximum for Indoor Unit: Required Double
  EXPECT_TRUE(airConditionerVariableRefrigerantFlowFluidTemperatureControlHR.setVariableEvaporatingTemperatureMaximumforIndoorUnit(1.8));
  EXPECT_EQ(1.8, airConditionerVariableRefrigerantFlowFluidTemperatureControlHR.variableEvaporatingTemperatureMaximumforIndoorUnit());

  // Variable Condensing Temperature Minimum for Indoor Unit: Required Double
  EXPECT_TRUE(airConditionerVariableRefrigerantFlowFluidTemperatureControlHR.setVariableCondensingTemperatureMinimumforIndoorUnit(1.9));
  EXPECT_EQ(1.9, airConditionerVariableRefrigerantFlowFluidTemperatureControlHR.variableCondensingTemperatureMinimumforIndoorUnit());

  // Variable Condensing Temperature Maximum for Indoor Unit: Required Double
  EXPECT_TRUE(airConditionerVariableRefrigerantFlowFluidTemperatureControlHR.setVariableCondensingTemperatureMaximumforIndoorUnit(2.0));
  EXPECT_EQ(2.0, airConditionerVariableRefrigerantFlowFluidTemperatureControlHR.variableCondensingTemperatureMaximumforIndoorUnit());

  // Outdoor Unit Evaporator Reference Superheating: Required Double
  EXPECT_TRUE(airConditionerVariableRefrigerantFlowFluidTemperatureControlHR.setOutdoorUnitEvaporatorReferenceSuperheating(2.1));
  EXPECT_EQ(2.1, airConditionerVariableRefrigerantFlowFluidTemperatureControlHR.outdoorUnitEvaporatorReferenceSuperheating());

  // Outdoor Unit Condenser Reference Subcooling: Required Double
  EXPECT_TRUE(airConditionerVariableRefrigerantFlowFluidTemperatureControlHR.setOutdoorUnitCondenserReferenceSubcooling(2.2));
  EXPECT_EQ(2.2, airConditionerVariableRefrigerantFlowFluidTemperatureControlHR.outdoorUnitCondenserReferenceSubcooling());

  // Outdoor Unit Evaporator Rated Bypass Factor: Required Double
  EXPECT_TRUE(airConditionerVariableRefrigerantFlowFluidTemperatureControlHR.setOutdoorUnitEvaporatorRatedBypassFactor(2.3));
  EXPECT_EQ(2.3, airConditionerVariableRefrigerantFlowFluidTemperatureControlHR.outdoorUnitEvaporatorRatedBypassFactor());
  // Bad Value
  EXPECT_FALSE(airConditionerVariableRefrigerantFlowFluidTemperatureControlHR.setOutdoorUnitEvaporatorRatedBypassFactor(-10.0));
  EXPECT_EQ(2.3, airConditionerVariableRefrigerantFlowFluidTemperatureControlHR.outdoorUnitEvaporatorRatedBypassFactor());

  // Outdoor Unit Condenser Rated Bypass Factor: Required Double
  EXPECT_TRUE(airConditionerVariableRefrigerantFlowFluidTemperatureControlHR.setOutdoorUnitCondenserRatedBypassFactor(2.4));
  EXPECT_EQ(2.4, airConditionerVariableRefrigerantFlowFluidTemperatureControlHR.outdoorUnitCondenserRatedBypassFactor());
  // Bad Value
  EXPECT_FALSE(airConditionerVariableRefrigerantFlowFluidTemperatureControlHR.setOutdoorUnitCondenserRatedBypassFactor(-10.0));
  EXPECT_EQ(2.4, airConditionerVariableRefrigerantFlowFluidTemperatureControlHR.outdoorUnitCondenserRatedBypassFactor());

  // Difference between Outdoor Unit Evaporating Temperature and Outdoor Air Temperature in Heat Recovery Mode: Required Double
  EXPECT_TRUE(airConditionerVariableRefrigerantFlowFluidTemperatureControlHR
                .setDifferencebetweenOutdoorUnitEvaporatingTemperatureandOutdoorAirTemperatureinHeatRecoveryMode(2.5));
  EXPECT_EQ(2.5, airConditionerVariableRefrigerantFlowFluidTemperatureControlHR
                   .differencebetweenOutdoorUnitEvaporatingTemperatureandOutdoorAirTemperatureinHeatRecoveryMode());

  // Outdoor Unit Heat Exchanger Capacity Ratio: Required Double
  EXPECT_TRUE(airConditionerVariableRefrigerantFlowFluidTemperatureControlHR.setOutdoorUnitHeatExchangerCapacityRatio(2.6));
  EXPECT_EQ(2.6, airConditionerVariableRefrigerantFlowFluidTemperatureControlHR.outdoorUnitHeatExchangerCapacityRatio());
  // Bad Value
  EXPECT_FALSE(airConditionerVariableRefrigerantFlowFluidTemperatureControlHR.setOutdoorUnitHeatExchangerCapacityRatio(-10.0));
  EXPECT_EQ(2.6, airConditionerVariableRefrigerantFlowFluidTemperatureControlHR.outdoorUnitHeatExchangerCapacityRatio());

  // Outdoor Unit Fan Power Per Unit of Rated Evaporative Capacity: Required Double
  EXPECT_TRUE(airConditionerVariableRefrigerantFlowFluidTemperatureControlHR.setOutdoorUnitFanPowerPerUnitofRatedEvaporativeCapacity(2.7));
  EXPECT_EQ(2.7, airConditionerVariableRefrigerantFlowFluidTemperatureControlHR.outdoorUnitFanPowerPerUnitofRatedEvaporativeCapacity());
  // Bad Value
  EXPECT_FALSE(airConditionerVariableRefrigerantFlowFluidTemperatureControlHR.setOutdoorUnitFanPowerPerUnitofRatedEvaporativeCapacity(-10.0));
  EXPECT_EQ(2.7, airConditionerVariableRefrigerantFlowFluidTemperatureControlHR.outdoorUnitFanPowerPerUnitofRatedEvaporativeCapacity());

  // Outdoor Unit Fan Flow Rate Per Unit of Rated Evaporative Capacity: Required Double
  EXPECT_TRUE(airConditionerVariableRefrigerantFlowFluidTemperatureControlHR.setOutdoorUnitFanFlowRatePerUnitofRatedEvaporativeCapacity(2.8));
  EXPECT_EQ(2.8, airConditionerVariableRefrigerantFlowFluidTemperatureControlHR.outdoorUnitFanFlowRatePerUnitofRatedEvaporativeCapacity());
  // Bad Value
  EXPECT_FALSE(airConditionerVariableRefrigerantFlowFluidTemperatureControlHR.setOutdoorUnitFanFlowRatePerUnitofRatedEvaporativeCapacity(-10.0));
  EXPECT_EQ(2.8, airConditionerVariableRefrigerantFlowFluidTemperatureControlHR.outdoorUnitFanFlowRatePerUnitofRatedEvaporativeCapacity());

  // Outdoor Unit Evaporating Temperature Function of Superheating Curve Name: Required Object
  UnivariateFunctions obj(m);
  EXPECT_TRUE(airConditionerVariableRefrigerantFlowFluidTemperatureControlHR.setOutdoorUnitEvaporatingTemperatureFunctionofSuperheatingCurve(obj));
  EXPECT_EQ(obj, airConditionerVariableRefrigerantFlowFluidTemperatureControlHR.outdoorUnitEvaporatingTemperatureFunctionofSuperheatingCurve());

  // Outdoor Unit Condensing Temperature Function of Subcooling Curve Name: Required Object
  UnivariateFunctions obj(m);
  EXPECT_TRUE(airConditionerVariableRefrigerantFlowFluidTemperatureControlHR.setOutdoorUnitCondensingTemperatureFunctionofSubcoolingCurve(obj));
  EXPECT_EQ(obj, airConditionerVariableRefrigerantFlowFluidTemperatureControlHR.outdoorUnitCondensingTemperatureFunctionofSubcoolingCurve());

  // Diameter of Main Pipe for Suction Gas: Required Double
  EXPECT_TRUE(airConditionerVariableRefrigerantFlowFluidTemperatureControlHR.setDiameterofMainPipeforSuctionGas(3.1));
  EXPECT_EQ(3.1, airConditionerVariableRefrigerantFlowFluidTemperatureControlHR.diameterofMainPipeforSuctionGas());
  // Bad Value
  EXPECT_FALSE(airConditionerVariableRefrigerantFlowFluidTemperatureControlHR.setDiameterofMainPipeforSuctionGas(-10.0));
  EXPECT_EQ(3.1, airConditionerVariableRefrigerantFlowFluidTemperatureControlHR.diameterofMainPipeforSuctionGas());

  // Diameter of Main Pipe for Discharge Gas: Required Double
  EXPECT_TRUE(airConditionerVariableRefrigerantFlowFluidTemperatureControlHR.setDiameterofMainPipeforDischargeGas(3.2));
  EXPECT_EQ(3.2, airConditionerVariableRefrigerantFlowFluidTemperatureControlHR.diameterofMainPipeforDischargeGas());
  // Bad Value
  EXPECT_FALSE(airConditionerVariableRefrigerantFlowFluidTemperatureControlHR.setDiameterofMainPipeforDischargeGas(-10.0));
  EXPECT_EQ(3.2, airConditionerVariableRefrigerantFlowFluidTemperatureControlHR.diameterofMainPipeforDischargeGas());

  // Length of Main Pipe Connecting Outdoor Unit to the First Branch Joint: Required Double
  EXPECT_TRUE(airConditionerVariableRefrigerantFlowFluidTemperatureControlHR.setLengthofMainPipeConnectingOutdoorUnittotheFirstBranchJoint(3.3));
  EXPECT_EQ(3.3, airConditionerVariableRefrigerantFlowFluidTemperatureControlHR.lengthofMainPipeConnectingOutdoorUnittotheFirstBranchJoint());
  // Bad Value
  EXPECT_FALSE(airConditionerVariableRefrigerantFlowFluidTemperatureControlHR.setLengthofMainPipeConnectingOutdoorUnittotheFirstBranchJoint(-10.0));
  EXPECT_EQ(3.3, airConditionerVariableRefrigerantFlowFluidTemperatureControlHR.lengthofMainPipeConnectingOutdoorUnittotheFirstBranchJoint());

  // Equivalent Length of Main Pipe Connecting Outdoor Unit to the First Branch Joint: Required Double
  EXPECT_TRUE(
    airConditionerVariableRefrigerantFlowFluidTemperatureControlHR.setEquivalentLengthofMainPipeConnectingOutdoorUnittotheFirstBranchJoint(3.4));
  EXPECT_EQ(3.4,
            airConditionerVariableRefrigerantFlowFluidTemperatureControlHR.equivalentLengthofMainPipeConnectingOutdoorUnittotheFirstBranchJoint());
  // Bad Value
  EXPECT_FALSE(
    airConditionerVariableRefrigerantFlowFluidTemperatureControlHR.setEquivalentLengthofMainPipeConnectingOutdoorUnittotheFirstBranchJoint(-10.0));
  EXPECT_EQ(3.4,
            airConditionerVariableRefrigerantFlowFluidTemperatureControlHR.equivalentLengthofMainPipeConnectingOutdoorUnittotheFirstBranchJoint());

  // Height Difference Between Outdoor Unit and Indoor Units: Required Double
  EXPECT_TRUE(airConditionerVariableRefrigerantFlowFluidTemperatureControlHR.setHeightDifferenceBetweenOutdoorUnitandIndoorUnits(3.5));
  EXPECT_EQ(3.5, airConditionerVariableRefrigerantFlowFluidTemperatureControlHR.heightDifferenceBetweenOutdoorUnitandIndoorUnits());

  // Main Pipe Insulation Thickness: Required Double
  EXPECT_TRUE(airConditionerVariableRefrigerantFlowFluidTemperatureControlHR.setMainPipeInsulationThickness(3.6));
  EXPECT_EQ(3.6, airConditionerVariableRefrigerantFlowFluidTemperatureControlHR.mainPipeInsulationThickness());
  // Bad Value
  EXPECT_FALSE(airConditionerVariableRefrigerantFlowFluidTemperatureControlHR.setMainPipeInsulationThickness(-10.0));
  EXPECT_EQ(3.6, airConditionerVariableRefrigerantFlowFluidTemperatureControlHR.mainPipeInsulationThickness());

  // Main Pipe Insulation Thermal Conductivity: Required Double
  EXPECT_TRUE(airConditionerVariableRefrigerantFlowFluidTemperatureControlHR.setMainPipeInsulationThermalConductivity(3.7));
  EXPECT_EQ(3.7, airConditionerVariableRefrigerantFlowFluidTemperatureControlHR.mainPipeInsulationThermalConductivity());
  // Bad Value
  EXPECT_FALSE(airConditionerVariableRefrigerantFlowFluidTemperatureControlHR.setMainPipeInsulationThermalConductivity(-10.0));
  EXPECT_EQ(3.7, airConditionerVariableRefrigerantFlowFluidTemperatureControlHR.mainPipeInsulationThermalConductivity());

  // Crankcase Heater Power per Compressor: Required Double
  EXPECT_TRUE(airConditionerVariableRefrigerantFlowFluidTemperatureControlHR.setCrankcaseHeaterPowerperCompressor(3.8));
  EXPECT_EQ(3.8, airConditionerVariableRefrigerantFlowFluidTemperatureControlHR.crankcaseHeaterPowerperCompressor());

  // Number of Compressors: Required Integer
  EXPECT_TRUE(airConditionerVariableRefrigerantFlowFluidTemperatureControlHR.setNumberofCompressors(39));
  EXPECT_EQ(39, airConditionerVariableRefrigerantFlowFluidTemperatureControlHR.numberofCompressors());

  // Ratio of Compressor Size to Total Compressor Capacity: Required Double
  EXPECT_TRUE(airConditionerVariableRefrigerantFlowFluidTemperatureControlHR.setRatioofCompressorSizetoTotalCompressorCapacity(4.0));
  EXPECT_EQ(4.0, airConditionerVariableRefrigerantFlowFluidTemperatureControlHR.ratioofCompressorSizetoTotalCompressorCapacity());

  // Maximum Outdoor Dry-Bulb Temperature for Crankcase Heater: Required Double
  EXPECT_TRUE(airConditionerVariableRefrigerantFlowFluidTemperatureControlHR.setMaximumOutdoorDryBulbTemperatureforCrankcaseHeater(4.1));
  EXPECT_EQ(4.1, airConditionerVariableRefrigerantFlowFluidTemperatureControlHR.maximumOutdoorDryBulbTemperatureforCrankcaseHeater());

  // Defrost Strategy: Required String
  EXPECT_TRUE(airConditionerVariableRefrigerantFlowFluidTemperatureControlHR.setDefrostStrategy("ReverseCycle"));
  EXPECT_EQ("ReverseCycle", airConditionerVariableRefrigerantFlowFluidTemperatureControlHR.defrostStrategy());
  // Bad Value
  EXPECT_FALSE(airConditionerVariableRefrigerantFlowFluidTemperatureControlHR.setDefrostStrategy("BADENUM"));
  EXPECT_EQ("ReverseCycle", airConditionerVariableRefrigerantFlowFluidTemperatureControlHR.defrostStrategy());

  // Defrost Control: Required String
  EXPECT_TRUE(airConditionerVariableRefrigerantFlowFluidTemperatureControlHR.setDefrostControl("Timed"));
  EXPECT_EQ("Timed", airConditionerVariableRefrigerantFlowFluidTemperatureControlHR.defrostControl());
  // Bad Value
  EXPECT_FALSE(airConditionerVariableRefrigerantFlowFluidTemperatureControlHR.setDefrostControl("BADENUM"));
  EXPECT_EQ("Timed", airConditionerVariableRefrigerantFlowFluidTemperatureControlHR.defrostControl());

  // Defrost Energy Input Ratio Modifier Function of Temperature Curve Name: Optional Object
  boost::optional<BivariateFunctions> obj(m);
  EXPECT_TRUE(airConditionerVariableRefrigerantFlowFluidTemperatureControlHR.setDefrostEnergyInputRatioModifierFunctionofTemperatureCurve(obj));
  ASSERT_TRUE(airConditionerVariableRefrigerantFlowFluidTemperatureControlHR.defrostEnergyInputRatioModifierFunctionofTemperatureCurve());
  EXPECT_EQ(obj, airConditionerVariableRefrigerantFlowFluidTemperatureControlHR.defrostEnergyInputRatioModifierFunctionofTemperatureCurve().get());

  // Defrost Time Period Fraction: Required Double
  EXPECT_TRUE(airConditionerVariableRefrigerantFlowFluidTemperatureControlHR.setDefrostTimePeriodFraction(4.5));
  EXPECT_EQ(4.5, airConditionerVariableRefrigerantFlowFluidTemperatureControlHR.defrostTimePeriodFraction());
  // Bad Value
  EXPECT_FALSE(airConditionerVariableRefrigerantFlowFluidTemperatureControlHR.setDefrostTimePeriodFraction(-10.0));
  EXPECT_EQ(4.5, airConditionerVariableRefrigerantFlowFluidTemperatureControlHR.defrostTimePeriodFraction());

  // Resistive Defrost Heater Capacity: Required Double
  // Autosize
  airConditionerVariableRefrigerantFlowFluidTemperatureControlHR.autosizeResistiveDefrostHeaterCapacity();
  EXPECT_TRUE(airConditionerVariableRefrigerantFlowFluidTemperatureControlHR.isResistiveDefrostHeaterCapacityAutosized());
  // Set
  EXPECT_TRUE(airConditionerVariableRefrigerantFlowFluidTemperatureControlHR.setResistiveDefrostHeaterCapacity(4.6));
  ASSERT_TRUE(airConditionerVariableRefrigerantFlowFluidTemperatureControlHR.resistiveDefrostHeaterCapacity());
  EXPECT_EQ(4.6, airConditionerVariableRefrigerantFlowFluidTemperatureControlHR.resistiveDefrostHeaterCapacity().get());
  // Bad Value
  EXPECT_FALSE(airConditionerVariableRefrigerantFlowFluidTemperatureControlHR.setResistiveDefrostHeaterCapacity(-10.0));
  ASSERT_TRUE(airConditionerVariableRefrigerantFlowFluidTemperatureControlHR.resistiveDefrostHeaterCapacity());
  EXPECT_EQ(4.6, airConditionerVariableRefrigerantFlowFluidTemperatureControlHR.resistiveDefrostHeaterCapacity().get());
  EXPECT_FALSE(airConditionerVariableRefrigerantFlowFluidTemperatureControlHR.isResistiveDefrostHeaterCapacityAutosized());

  // Maximum Outdoor Dry-bulb Temperature for Defrost Operation: Required Double
  EXPECT_TRUE(airConditionerVariableRefrigerantFlowFluidTemperatureControlHR.setMaximumOutdoorDrybulbTemperatureforDefrostOperation(4.7));
  EXPECT_EQ(4.7, airConditionerVariableRefrigerantFlowFluidTemperatureControlHR.maximumOutdoorDrybulbTemperatureforDefrostOperation());

  // Initial Heat Recovery Cooling Capacity Fraction: Required Double
  EXPECT_TRUE(airConditionerVariableRefrigerantFlowFluidTemperatureControlHR.setInitialHeatRecoveryCoolingCapacityFraction(4.8));
  EXPECT_EQ(4.8, airConditionerVariableRefrigerantFlowFluidTemperatureControlHR.initialHeatRecoveryCoolingCapacityFraction());

  // Heat Recovery Cooling Capacity Time Constant: Required Double
  EXPECT_TRUE(airConditionerVariableRefrigerantFlowFluidTemperatureControlHR.setHeatRecoveryCoolingCapacityTimeConstant(4.9));
  EXPECT_EQ(4.9, airConditionerVariableRefrigerantFlowFluidTemperatureControlHR.heatRecoveryCoolingCapacityTimeConstant());

  // Initial Heat Recovery Cooling Energy Fraction: Required Double
  EXPECT_TRUE(airConditionerVariableRefrigerantFlowFluidTemperatureControlHR.setInitialHeatRecoveryCoolingEnergyFraction(5.0));
  EXPECT_EQ(5.0, airConditionerVariableRefrigerantFlowFluidTemperatureControlHR.initialHeatRecoveryCoolingEnergyFraction());

  // Heat Recovery Cooling Energy Time Constant: Required Double
  EXPECT_TRUE(airConditionerVariableRefrigerantFlowFluidTemperatureControlHR.setHeatRecoveryCoolingEnergyTimeConstant(5.1));
  EXPECT_EQ(5.1, airConditionerVariableRefrigerantFlowFluidTemperatureControlHR.heatRecoveryCoolingEnergyTimeConstant());

  // Initial Heat Recovery Heating Capacity Fraction: Required Double
  EXPECT_TRUE(airConditionerVariableRefrigerantFlowFluidTemperatureControlHR.setInitialHeatRecoveryHeatingCapacityFraction(5.2));
  EXPECT_EQ(5.2, airConditionerVariableRefrigerantFlowFluidTemperatureControlHR.initialHeatRecoveryHeatingCapacityFraction());

  // Heat Recovery Heating Capacity Time Constant: Required Double
  EXPECT_TRUE(airConditionerVariableRefrigerantFlowFluidTemperatureControlHR.setHeatRecoveryHeatingCapacityTimeConstant(5.3));
  EXPECT_EQ(5.3, airConditionerVariableRefrigerantFlowFluidTemperatureControlHR.heatRecoveryHeatingCapacityTimeConstant());

  // Initial Heat Recovery Heating Energy Fraction: Required Double
  EXPECT_TRUE(airConditionerVariableRefrigerantFlowFluidTemperatureControlHR.setInitialHeatRecoveryHeatingEnergyFraction(5.4));
  EXPECT_EQ(5.4, airConditionerVariableRefrigerantFlowFluidTemperatureControlHR.initialHeatRecoveryHeatingEnergyFraction());

  // Heat Recovery Heating Energy Time Constant: Required Double
  EXPECT_TRUE(airConditionerVariableRefrigerantFlowFluidTemperatureControlHR.setHeatRecoveryHeatingEnergyTimeConstant(5.5));
  EXPECT_EQ(5.5, airConditionerVariableRefrigerantFlowFluidTemperatureControlHR.heatRecoveryHeatingEnergyTimeConstant());

  // Compressor maximum delta Pressure: Required Double
  EXPECT_TRUE(airConditionerVariableRefrigerantFlowFluidTemperatureControlHR.setCompressormaximumdeltaPressure(49122807.018));
  EXPECT_EQ(49122807.018, airConditionerVariableRefrigerantFlowFluidTemperatureControlHR.compressormaximumdeltaPressure());
  // Bad Value
  EXPECT_FALSE(airConditionerVariableRefrigerantFlowFluidTemperatureControlHR.setCompressormaximumdeltaPressure(-10.0));
  EXPECT_EQ(49122807.018, airConditionerVariableRefrigerantFlowFluidTemperatureControlHR.compressormaximumdeltaPressure());

  // Compressor Inverter Efficiency: Required Double
  EXPECT_TRUE(airConditionerVariableRefrigerantFlowFluidTemperatureControlHR.setCompressorInverterEfficiency(0.983));
  EXPECT_EQ(0.983, airConditionerVariableRefrigerantFlowFluidTemperatureControlHR.compressorInverterEfficiency());
  // Bad Value
  EXPECT_FALSE(airConditionerVariableRefrigerantFlowFluidTemperatureControlHR.setCompressorInverterEfficiency(-10.0));
  EXPECT_EQ(0.983, airConditionerVariableRefrigerantFlowFluidTemperatureControlHR.compressorInverterEfficiency());

  // Compressor Evaporative Capacity Correction Factor: Required Double
  EXPECT_TRUE(airConditionerVariableRefrigerantFlowFluidTemperatureControlHR.setCompressorEvaporativeCapacityCorrectionFactor(5.8));
  EXPECT_EQ(5.8, airConditionerVariableRefrigerantFlowFluidTemperatureControlHR.compressorEvaporativeCapacityCorrectionFactor());
  // Bad Value
  EXPECT_FALSE(airConditionerVariableRefrigerantFlowFluidTemperatureControlHR.setCompressorEvaporativeCapacityCorrectionFactor(-10.0));
  EXPECT_EQ(5.8, airConditionerVariableRefrigerantFlowFluidTemperatureControlHR.compressorEvaporativeCapacityCorrectionFactor());

  // Number of Compressor Loading Index Entries: Required Integer
  EXPECT_TRUE(airConditionerVariableRefrigerantFlowFluidTemperatureControlHR.setNumberofCompressorLoadingIndexEntries(61));
  EXPECT_EQ(61, airConditionerVariableRefrigerantFlowFluidTemperatureControlHR.numberofCompressorLoadingIndexEntries());
  // Bad Value
  EXPECT_FALSE(airConditionerVariableRefrigerantFlowFluidTemperatureControlHR.setNumberofCompressorLoadingIndexEntries(-8));
  EXPECT_EQ(61, airConditionerVariableRefrigerantFlowFluidTemperatureControlHR.numberofCompressorLoadingIndexEntries());
}
