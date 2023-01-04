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

#include "../AirConditionerVariableRefrigerantFlowFluidTemperatureControl.hpp"
#include "../AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl.hpp"

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

TEST_F(ModelFixture, AirConditionerVariableRefrigerantFlowFluidTemperatureControl_GettersSetters) {
  Model m;
  // TODO: Check regular Ctor arguments
  AirConditionerVariableRefrigerantFlowFluidTemperatureControl airConditionerVariableRefrigerantFlowFluidTemperatureControl(m);
  // TODO: Or if a UniqueModelObject (and make sure _Impl is included)
  // AirConditionerVariableRefrigerantFlowFluidTemperatureControl airConditionerVariableRefrigerantFlowFluidTemperatureControl = m.getUniqueModelObject<AirConditionerVariableRefrigerantFlowFluidTemperatureControl>();

  airConditionerVariableRefrigerantFlowFluidTemperatureControl.setName("My AirConditionerVariableRefrigerantFlowFluidTemperatureControl");

  // Availability Schedule: Optional Object
  boost::optional<Schedule> obj(m);
  EXPECT_TRUE(airConditionerVariableRefrigerantFlowFluidTemperatureControl.setAvailabilitySchedule(obj));
  ASSERT_TRUE(airConditionerVariableRefrigerantFlowFluidTemperatureControl.availabilitySchedule());
  EXPECT_EQ(obj, airConditionerVariableRefrigerantFlowFluidTemperatureControl.availabilitySchedule().get());

  // Zone Terminal Unit List: Required Object
  ModelObjectLists obj(m);
  EXPECT_TRUE(airConditionerVariableRefrigerantFlowFluidTemperatureControl.setZoneTerminalUnitList(obj));
EXPECT_EQ(obj, airConditionerVariableRefrigerantFlowFluidTemperatureControl.zoneTerminalUnitList());

  // Refrigerant Type: Required String
  EXPECT_TRUE(airConditionerVariableRefrigerantFlowFluidTemperatureControl.setRefrigerantType("R11"));
  EXPECT_EQ("R11", airConditionerVariableRefrigerantFlowFluidTemperatureControl.refrigerantType());
  // Bad Value
  EXPECT_FALSE(airConditionerVariableRefrigerantFlowFluidTemperatureControl.setRefrigerantType("BADENUM"));
  EXPECT_EQ("R11", airConditionerVariableRefrigerantFlowFluidTemperatureControl.refrigerantType());

  // Rated Evaporative Capacity: Required Double
  // Autosize
  airConditionerVariableRefrigerantFlowFluidTemperatureControl.autosizeRatedEvaporativeCapacity();
  EXPECT_TRUE(airConditionerVariableRefrigerantFlowFluidTemperatureControl.isRatedEvaporativeCapacityAutosized());
  // Set
  EXPECT_TRUE(airConditionerVariableRefrigerantFlowFluidTemperatureControl.setRatedEvaporativeCapacity(0.6));
  ASSERT_TRUE(airConditionerVariableRefrigerantFlowFluidTemperatureControl.ratedEvaporativeCapacity());
  EXPECT_EQ(0.6, airConditionerVariableRefrigerantFlowFluidTemperatureControl.ratedEvaporativeCapacity().get());
  // Bad Value
  EXPECT_FALSE(airConditionerVariableRefrigerantFlowFluidTemperatureControl.setRatedEvaporativeCapacity(-10.0));
  ASSERT_TRUE(airConditionerVariableRefrigerantFlowFluidTemperatureControl.ratedEvaporativeCapacity());
  EXPECT_EQ(0.6, airConditionerVariableRefrigerantFlowFluidTemperatureControl.ratedEvaporativeCapacity().get());
  EXPECT_FALSE(airConditionerVariableRefrigerantFlowFluidTemperatureControl.isRatedEvaporativeCapacityAutosized());

  // Rated Compressor Power Per Unit of Rated Evaporative Capacity: Required Double
  EXPECT_TRUE(airConditionerVariableRefrigerantFlowFluidTemperatureControl.setRatedCompressorPowerPerUnitofRatedEvaporativeCapacity(0.7));
  EXPECT_EQ(0.7, airConditionerVariableRefrigerantFlowFluidTemperatureControl.ratedCompressorPowerPerUnitofRatedEvaporativeCapacity());
  // Bad Value
  EXPECT_FALSE(airConditionerVariableRefrigerantFlowFluidTemperatureControl.setRatedCompressorPowerPerUnitofRatedEvaporativeCapacity(-10.0));
  EXPECT_EQ(0.7, airConditionerVariableRefrigerantFlowFluidTemperatureControl.ratedCompressorPowerPerUnitofRatedEvaporativeCapacity());

  // Minimum Outdoor Air Temperature in Cooling Mode: Required Double
  EXPECT_TRUE(airConditionerVariableRefrigerantFlowFluidTemperatureControl.setMinimumOutdoorAirTemperatureinCoolingMode(0.8));
  EXPECT_EQ(0.8, airConditionerVariableRefrigerantFlowFluidTemperatureControl.minimumOutdoorAirTemperatureinCoolingMode());

  // Maximum Outdoor Air Temperature in Cooling Mode: Required Double
  EXPECT_TRUE(airConditionerVariableRefrigerantFlowFluidTemperatureControl.setMaximumOutdoorAirTemperatureinCoolingMode(0.9));
  EXPECT_EQ(0.9, airConditionerVariableRefrigerantFlowFluidTemperatureControl.maximumOutdoorAirTemperatureinCoolingMode());

  // Minimum Outdoor Air Temperature in Heating Mode: Required Double
  EXPECT_TRUE(airConditionerVariableRefrigerantFlowFluidTemperatureControl.setMinimumOutdoorAirTemperatureinHeatingMode(1.0));
  EXPECT_EQ(1.0, airConditionerVariableRefrigerantFlowFluidTemperatureControl.minimumOutdoorAirTemperatureinHeatingMode());

  // Maximum Outdoor Air Temperature in Heating Mode: Required Double
  EXPECT_TRUE(airConditionerVariableRefrigerantFlowFluidTemperatureControl.setMaximumOutdoorAirTemperatureinHeatingMode(1.1));
  EXPECT_EQ(1.1, airConditionerVariableRefrigerantFlowFluidTemperatureControl.maximumOutdoorAirTemperatureinHeatingMode());

  // Reference Outdoor Unit Superheating: Required Double
  EXPECT_TRUE(airConditionerVariableRefrigerantFlowFluidTemperatureControl.setReferenceOutdoorUnitSuperheating(1.2));
  EXPECT_EQ(1.2, airConditionerVariableRefrigerantFlowFluidTemperatureControl.referenceOutdoorUnitSuperheating());

  // Reference Outdoor Unit Subcooling: Required Double
  EXPECT_TRUE(airConditionerVariableRefrigerantFlowFluidTemperatureControl.setReferenceOutdoorUnitSubcooling(1.3));
  EXPECT_EQ(1.3, airConditionerVariableRefrigerantFlowFluidTemperatureControl.referenceOutdoorUnitSubcooling());

  // Refrigerant Temperature Control Algorithm for Indoor Unit: Required String
  EXPECT_TRUE(airConditionerVariableRefrigerantFlowFluidTemperatureControl.setRefrigerantTemperatureControlAlgorithmforIndoorUnit("ConstantTemp"));
  EXPECT_EQ("ConstantTemp", airConditionerVariableRefrigerantFlowFluidTemperatureControl.refrigerantTemperatureControlAlgorithmforIndoorUnit());
  // Bad Value
  EXPECT_FALSE(airConditionerVariableRefrigerantFlowFluidTemperatureControl.setRefrigerantTemperatureControlAlgorithmforIndoorUnit("BADENUM"));
  EXPECT_EQ("ConstantTemp", airConditionerVariableRefrigerantFlowFluidTemperatureControl.refrigerantTemperatureControlAlgorithmforIndoorUnit());

  // Reference Evaporating Temperature for Indoor Unit: Required Double
  EXPECT_TRUE(airConditionerVariableRefrigerantFlowFluidTemperatureControl.setReferenceEvaporatingTemperatureforIndoorUnit(1.5));
  EXPECT_EQ(1.5, airConditionerVariableRefrigerantFlowFluidTemperatureControl.referenceEvaporatingTemperatureforIndoorUnit());

  // Reference Condensing Temperature for Indoor Unit: Required Double
  EXPECT_TRUE(airConditionerVariableRefrigerantFlowFluidTemperatureControl.setReferenceCondensingTemperatureforIndoorUnit(1.6));
  EXPECT_EQ(1.6, airConditionerVariableRefrigerantFlowFluidTemperatureControl.referenceCondensingTemperatureforIndoorUnit());

  // Variable Evaporating Temperature Minimum for Indoor Unit: Required Double
  EXPECT_TRUE(airConditionerVariableRefrigerantFlowFluidTemperatureControl.setVariableEvaporatingTemperatureMinimumforIndoorUnit(1.7));
  EXPECT_EQ(1.7, airConditionerVariableRefrigerantFlowFluidTemperatureControl.variableEvaporatingTemperatureMinimumforIndoorUnit());

  // Variable Evaporating Temperature Maximum for Indoor Unit: Required Double
  EXPECT_TRUE(airConditionerVariableRefrigerantFlowFluidTemperatureControl.setVariableEvaporatingTemperatureMaximumforIndoorUnit(1.8));
  EXPECT_EQ(1.8, airConditionerVariableRefrigerantFlowFluidTemperatureControl.variableEvaporatingTemperatureMaximumforIndoorUnit());

  // Variable Condensing Temperature Minimum for Indoor Unit: Required Double
  EXPECT_TRUE(airConditionerVariableRefrigerantFlowFluidTemperatureControl.setVariableCondensingTemperatureMinimumforIndoorUnit(1.9));
  EXPECT_EQ(1.9, airConditionerVariableRefrigerantFlowFluidTemperatureControl.variableCondensingTemperatureMinimumforIndoorUnit());

  // Variable Condensing Temperature Maximum for Indoor Unit: Required Double
  EXPECT_TRUE(airConditionerVariableRefrigerantFlowFluidTemperatureControl.setVariableCondensingTemperatureMaximumforIndoorUnit(2.0));
  EXPECT_EQ(2.0, airConditionerVariableRefrigerantFlowFluidTemperatureControl.variableCondensingTemperatureMaximumforIndoorUnit());

  // Outdoor Unit Fan Power Per Unit of Rated Evaporative Capacity: Required Double
  EXPECT_TRUE(airConditionerVariableRefrigerantFlowFluidTemperatureControl.setOutdoorUnitFanPowerPerUnitofRatedEvaporativeCapacity(2.1));
  EXPECT_EQ(2.1, airConditionerVariableRefrigerantFlowFluidTemperatureControl.outdoorUnitFanPowerPerUnitofRatedEvaporativeCapacity());
  // Bad Value
  EXPECT_FALSE(airConditionerVariableRefrigerantFlowFluidTemperatureControl.setOutdoorUnitFanPowerPerUnitofRatedEvaporativeCapacity(-10.0));
  EXPECT_EQ(2.1, airConditionerVariableRefrigerantFlowFluidTemperatureControl.outdoorUnitFanPowerPerUnitofRatedEvaporativeCapacity());

  // Outdoor Unit Fan Flow Rate Per Unit of Rated Evaporative Capacity: Required Double
  EXPECT_TRUE(airConditionerVariableRefrigerantFlowFluidTemperatureControl.setOutdoorUnitFanFlowRatePerUnitofRatedEvaporativeCapacity(2.2));
  EXPECT_EQ(2.2, airConditionerVariableRefrigerantFlowFluidTemperatureControl.outdoorUnitFanFlowRatePerUnitofRatedEvaporativeCapacity());
  // Bad Value
  EXPECT_FALSE(airConditionerVariableRefrigerantFlowFluidTemperatureControl.setOutdoorUnitFanFlowRatePerUnitofRatedEvaporativeCapacity(-10.0));
  EXPECT_EQ(2.2, airConditionerVariableRefrigerantFlowFluidTemperatureControl.outdoorUnitFanFlowRatePerUnitofRatedEvaporativeCapacity());

  // Outdoor Unit Evaporating Temperature Function of Superheating Curve Name: Required Object
  UnivariateFunctions obj(m);
  EXPECT_TRUE(airConditionerVariableRefrigerantFlowFluidTemperatureControl.setOutdoorUnitEvaporatingTemperatureFunctionofSuperheatingCurve(obj));
EXPECT_EQ(obj, airConditionerVariableRefrigerantFlowFluidTemperatureControl.outdoorUnitEvaporatingTemperatureFunctionofSuperheatingCurve());

  // Outdoor Unit Condensing Temperature Function of Subcooling Curve Name: Required Object
  UnivariateFunctions obj(m);
  EXPECT_TRUE(airConditionerVariableRefrigerantFlowFluidTemperatureControl.setOutdoorUnitCondensingTemperatureFunctionofSubcoolingCurve(obj));
EXPECT_EQ(obj, airConditionerVariableRefrigerantFlowFluidTemperatureControl.outdoorUnitCondensingTemperatureFunctionofSubcoolingCurve());

  // Diameter of Main Pipe Connecting Outdoor Unit to the First Branch Joint: Required Double
  EXPECT_TRUE(airConditionerVariableRefrigerantFlowFluidTemperatureControl.setDiameterofMainPipeConnectingOutdoorUnittotheFirstBranchJoint(2.5));
  EXPECT_EQ(2.5, airConditionerVariableRefrigerantFlowFluidTemperatureControl.diameterofMainPipeConnectingOutdoorUnittotheFirstBranchJoint());
  // Bad Value
  EXPECT_FALSE(airConditionerVariableRefrigerantFlowFluidTemperatureControl.setDiameterofMainPipeConnectingOutdoorUnittotheFirstBranchJoint(-10.0));
  EXPECT_EQ(2.5, airConditionerVariableRefrigerantFlowFluidTemperatureControl.diameterofMainPipeConnectingOutdoorUnittotheFirstBranchJoint());

  // Length of Main Pipe Connecting Outdoor Unit to the First Branch Joint: Required Double
  EXPECT_TRUE(airConditionerVariableRefrigerantFlowFluidTemperatureControl.setLengthofMainPipeConnectingOutdoorUnittotheFirstBranchJoint(2.6));
  EXPECT_EQ(2.6, airConditionerVariableRefrigerantFlowFluidTemperatureControl.lengthofMainPipeConnectingOutdoorUnittotheFirstBranchJoint());
  // Bad Value
  EXPECT_FALSE(airConditionerVariableRefrigerantFlowFluidTemperatureControl.setLengthofMainPipeConnectingOutdoorUnittotheFirstBranchJoint(-10.0));
  EXPECT_EQ(2.6, airConditionerVariableRefrigerantFlowFluidTemperatureControl.lengthofMainPipeConnectingOutdoorUnittotheFirstBranchJoint());

  // Equivalent Length of Main Pipe Connecting Outdoor Unit to the First Branch Joint: Required Double
  EXPECT_TRUE(airConditionerVariableRefrigerantFlowFluidTemperatureControl.setEquivalentLengthofMainPipeConnectingOutdoorUnittotheFirstBranchJoint(2.7));
  EXPECT_EQ(2.7, airConditionerVariableRefrigerantFlowFluidTemperatureControl.equivalentLengthofMainPipeConnectingOutdoorUnittotheFirstBranchJoint());
  // Bad Value
  EXPECT_FALSE(airConditionerVariableRefrigerantFlowFluidTemperatureControl.setEquivalentLengthofMainPipeConnectingOutdoorUnittotheFirstBranchJoint(-10.0));
  EXPECT_EQ(2.7, airConditionerVariableRefrigerantFlowFluidTemperatureControl.equivalentLengthofMainPipeConnectingOutdoorUnittotheFirstBranchJoint());

  // Height Difference Between Outdoor Unit and Indoor Units: Required Double
  EXPECT_TRUE(airConditionerVariableRefrigerantFlowFluidTemperatureControl.setHeightDifferenceBetweenOutdoorUnitandIndoorUnits(2.8));
  EXPECT_EQ(2.8, airConditionerVariableRefrigerantFlowFluidTemperatureControl.heightDifferenceBetweenOutdoorUnitandIndoorUnits());

  // Main Pipe Insulation Thickness: Required Double
  EXPECT_TRUE(airConditionerVariableRefrigerantFlowFluidTemperatureControl.setMainPipeInsulationThickness(2.9));
  EXPECT_EQ(2.9, airConditionerVariableRefrigerantFlowFluidTemperatureControl.mainPipeInsulationThickness());
  // Bad Value
  EXPECT_FALSE(airConditionerVariableRefrigerantFlowFluidTemperatureControl.setMainPipeInsulationThickness(-10.0));
  EXPECT_EQ(2.9, airConditionerVariableRefrigerantFlowFluidTemperatureControl.mainPipeInsulationThickness());

  // Main Pipe Insulation Thermal Conductivity: Required Double
  EXPECT_TRUE(airConditionerVariableRefrigerantFlowFluidTemperatureControl.setMainPipeInsulationThermalConductivity(3.0));
  EXPECT_EQ(3.0, airConditionerVariableRefrigerantFlowFluidTemperatureControl.mainPipeInsulationThermalConductivity());
  // Bad Value
  EXPECT_FALSE(airConditionerVariableRefrigerantFlowFluidTemperatureControl.setMainPipeInsulationThermalConductivity(-10.0));
  EXPECT_EQ(3.0, airConditionerVariableRefrigerantFlowFluidTemperatureControl.mainPipeInsulationThermalConductivity());

  // Crankcase Heater Power per Compressor: Required Double
  EXPECT_TRUE(airConditionerVariableRefrigerantFlowFluidTemperatureControl.setCrankcaseHeaterPowerperCompressor(3.1));
  EXPECT_EQ(3.1, airConditionerVariableRefrigerantFlowFluidTemperatureControl.crankcaseHeaterPowerperCompressor());

  // Number of Compressors: Optional Integer
  // Default value from IDD
  EXPECT_TRUE(airConditionerVariableRefrigerantFlowFluidTemperatureControl.isNumberofCompressorsDefaulted());
  // Set
  EXPECT_TRUE(airConditionerVariableRefrigerantFlowFluidTemperatureControl.setNumberofCompressors(32));
  EXPECT_EQ(32, airConditionerVariableRefrigerantFlowFluidTemperatureControl.numberofCompressors());
  EXPECT_FALSE(airConditionerVariableRefrigerantFlowFluidTemperatureControl.isNumberofCompressorsDefaulted());
  // Reset
  airConditionerVariableRefrigerantFlowFluidTemperatureControl.resetNumberofCompressors();
  EXPECT_TRUE(airConditionerVariableRefrigerantFlowFluidTemperatureControl.isNumberofCompressorsDefaulted());

  // Ratio of Compressor Size to Total Compressor Capacity: Required Double
  EXPECT_TRUE(airConditionerVariableRefrigerantFlowFluidTemperatureControl.setRatioofCompressorSizetoTotalCompressorCapacity(3.3));
  EXPECT_EQ(3.3, airConditionerVariableRefrigerantFlowFluidTemperatureControl.ratioofCompressorSizetoTotalCompressorCapacity());

  // Maximum Outdoor Dry-Bulb Temperature for Crankcase Heater: Required Double
  EXPECT_TRUE(airConditionerVariableRefrigerantFlowFluidTemperatureControl.setMaximumOutdoorDryBulbTemperatureforCrankcaseHeater(3.4));
  EXPECT_EQ(3.4, airConditionerVariableRefrigerantFlowFluidTemperatureControl.maximumOutdoorDryBulbTemperatureforCrankcaseHeater());

  // Defrost Strategy: Required String
  EXPECT_TRUE(airConditionerVariableRefrigerantFlowFluidTemperatureControl.setDefrostStrategy("ReverseCycle"));
  EXPECT_EQ("ReverseCycle", airConditionerVariableRefrigerantFlowFluidTemperatureControl.defrostStrategy());
  // Bad Value
  EXPECT_FALSE(airConditionerVariableRefrigerantFlowFluidTemperatureControl.setDefrostStrategy("BADENUM"));
  EXPECT_EQ("ReverseCycle", airConditionerVariableRefrigerantFlowFluidTemperatureControl.defrostStrategy());

  // Defrost Control: Required String
  EXPECT_TRUE(airConditionerVariableRefrigerantFlowFluidTemperatureControl.setDefrostControl("Timed"));
  EXPECT_EQ("Timed", airConditionerVariableRefrigerantFlowFluidTemperatureControl.defrostControl());
  // Bad Value
  EXPECT_FALSE(airConditionerVariableRefrigerantFlowFluidTemperatureControl.setDefrostControl("BADENUM"));
  EXPECT_EQ("Timed", airConditionerVariableRefrigerantFlowFluidTemperatureControl.defrostControl());

  // Defrost Energy Input Ratio Modifier Function of Temperature Curve Name: Optional Object
  boost::optional<BivariateFunctions> obj(m);
  EXPECT_TRUE(airConditionerVariableRefrigerantFlowFluidTemperatureControl.setDefrostEnergyInputRatioModifierFunctionofTemperatureCurve(obj));
  ASSERT_TRUE(airConditionerVariableRefrigerantFlowFluidTemperatureControl.defrostEnergyInputRatioModifierFunctionofTemperatureCurve());
  EXPECT_EQ(obj, airConditionerVariableRefrigerantFlowFluidTemperatureControl.defrostEnergyInputRatioModifierFunctionofTemperatureCurve().get());

  // Defrost Time Period Fraction: Required Double
  EXPECT_TRUE(airConditionerVariableRefrigerantFlowFluidTemperatureControl.setDefrostTimePeriodFraction(3.8));
  EXPECT_EQ(3.8, airConditionerVariableRefrigerantFlowFluidTemperatureControl.defrostTimePeriodFraction());
  // Bad Value
  EXPECT_FALSE(airConditionerVariableRefrigerantFlowFluidTemperatureControl.setDefrostTimePeriodFraction(-10.0));
  EXPECT_EQ(3.8, airConditionerVariableRefrigerantFlowFluidTemperatureControl.defrostTimePeriodFraction());

  // Resistive Defrost Heater Capacity: Required Double
  // Autosize
  airConditionerVariableRefrigerantFlowFluidTemperatureControl.autosizeResistiveDefrostHeaterCapacity();
  EXPECT_TRUE(airConditionerVariableRefrigerantFlowFluidTemperatureControl.isResistiveDefrostHeaterCapacityAutosized());
  // Set
  EXPECT_TRUE(airConditionerVariableRefrigerantFlowFluidTemperatureControl.setResistiveDefrostHeaterCapacity(3.9));
  ASSERT_TRUE(airConditionerVariableRefrigerantFlowFluidTemperatureControl.resistiveDefrostHeaterCapacity());
  EXPECT_EQ(3.9, airConditionerVariableRefrigerantFlowFluidTemperatureControl.resistiveDefrostHeaterCapacity().get());
  // Bad Value
  EXPECT_FALSE(airConditionerVariableRefrigerantFlowFluidTemperatureControl.setResistiveDefrostHeaterCapacity(-10.0));
  ASSERT_TRUE(airConditionerVariableRefrigerantFlowFluidTemperatureControl.resistiveDefrostHeaterCapacity());
  EXPECT_EQ(3.9, airConditionerVariableRefrigerantFlowFluidTemperatureControl.resistiveDefrostHeaterCapacity().get());
  EXPECT_FALSE(airConditionerVariableRefrigerantFlowFluidTemperatureControl.isResistiveDefrostHeaterCapacityAutosized());

  // Maximum Outdoor Dry-bulb Temperature for Defrost Operation: Required Double
  EXPECT_TRUE(airConditionerVariableRefrigerantFlowFluidTemperatureControl.setMaximumOutdoorDrybulbTemperatureforDefrostOperation(4.0));
  EXPECT_EQ(4.0, airConditionerVariableRefrigerantFlowFluidTemperatureControl.maximumOutdoorDrybulbTemperatureforDefrostOperation());

  // Compressor maximum delta Pressure: Required Double
  EXPECT_TRUE(airConditionerVariableRefrigerantFlowFluidTemperatureControl.setCompressormaximumdeltaPressure(48809523.81));
  EXPECT_EQ(48809523.81, airConditionerVariableRefrigerantFlowFluidTemperatureControl.compressormaximumdeltaPressure());
  // Bad Value
  EXPECT_FALSE(airConditionerVariableRefrigerantFlowFluidTemperatureControl.setCompressormaximumdeltaPressure(-10.0));
  EXPECT_EQ(48809523.81, airConditionerVariableRefrigerantFlowFluidTemperatureControl.compressormaximumdeltaPressure());

  // Number of Compressor Loading Index Entries: Required Integer
  EXPECT_TRUE(airConditionerVariableRefrigerantFlowFluidTemperatureControl.setNumberofCompressorLoadingIndexEntries(8));
  EXPECT_EQ(8, airConditionerVariableRefrigerantFlowFluidTemperatureControl.numberofCompressorLoadingIndexEntries());
  // Bad Value
  EXPECT_FALSE(airConditionerVariableRefrigerantFlowFluidTemperatureControl.setNumberofCompressorLoadingIndexEntries(-8));
  EXPECT_EQ(8, airConditionerVariableRefrigerantFlowFluidTemperatureControl.numberofCompressorLoadingIndexEntries());

}
