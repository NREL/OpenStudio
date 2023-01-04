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

#include "../ForwardTranslator.hpp"
#include "../../model/Model.hpp"

#include "../../model/AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR.hpp"

// TODO: Check the following class names against object getters and setters.
#include "../../model/Schedule.hpp"
#include "../../model/Schedule_Impl.hpp"

#include "../../model/ZoneTerminalUnitList.hpp"
#include "../../model/ZoneTerminalUnitList_Impl.hpp"

#include "../../model/Fluid.hpp"
#include "../../model/Fluid_Impl.hpp"

#include "../../model/UnivariateFunctions.hpp"
#include "../../model/UnivariateFunctions_Impl.hpp"

#include "../../model/BivariateFunctions.hpp"
#include "../../model/BivariateFunctions_Impl.hpp"

#include <utilities/idd/AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl_HR_FieldEnums.hxx>
// #include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

boost::optional<IdfObject> ForwardTranslator::translateAirConditionerVariableRefrigerantFlowFluidTemperatureControlHR( model::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR& modelObject )
{
  boost::optional<IdfObject> result;
  boost::optional<WorkspaceObject> _wo;
  boost::optional<ModelObject> _mo;

  // Instantiate an IdfObject of the class to store the values
  IdfObject idfObject = createRegisterAndNameIdfObject(openstudio::IddObjectType::AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl_HR, modelObject);
  // If it doesn't have a name, or if you aren't sure you are going to want to return it
  // IdfObject idfObject( openstudio::IddObjectType::AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl_HR );
  // m_idfObjects.push_back(idfObject);

  // TODO: Note JM 2018-10-17
  // You are responsible for implementing any additional logic based on choice fields, etc.
  // The ForwardTranslator generator script is meant to facilitate your work, not get you 100% of the way

  // TODO: If you keep createRegisterAndNameIdfObject above, you don't need this.
  // But in some cases, you'll want to handle failure without pushing to the map
  // Name
  if (boost::optional<std::string> moName = modelObject.name()) {
    idfObject.setName(*moName);
  }

  // Availability Schedule Name: Optional Object
  if (boost::optional<Schedule> _availabilitySchedule = modelObject.availabilitySchedule()) {
    if ( boost::optional<IdfObject> _owo = translateAndMapModelObject(_availabilitySchedule.get()) )  {
      idfObject.setString(AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl_HRFields::AvailabilityScheduleName, _owo->nameString());
    }
  }

  // Zone Terminal Unit List Name: Required Object
  ZoneTerminalUnitList zoneTerminalUnitList = modelObject.zoneTerminalUnitList();
  if ( boost::optional<IdfObject> _owo = translateAndMapModelObject(zoneTerminalUnitList) )  {
    idfObject.setString(AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl_HRFields::ZoneTerminalUnitListName, _owo->nameString());
  }

  // Refrigerant Type: Optional Object
  Fluid refrigerantType = modelObject.refrigerantType();
  if ( boost::optional<IdfObject> _owo = translateAndMapModelObject(refrigerantType) )  {
    idfObject.setString(AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl_HRFields::RefrigerantType, _owo->nameString());
  }

  if (modelObject.isRatedEvaporativeCapacityAutosized()) {
    idfObject.setString(AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl_HRFields::RatedEvaporativeCapacity, "Autosize");
  } else {
    // Rated Evaporative Capacity: boost::optional<double>
    if (boost::optional<double> _ratedEvaporativeCapacity = modelObject.ratedEvaporativeCapacity()) {
      idfObject.setDouble(AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl_HRFields::RatedEvaporativeCapacity, _ratedEvaporativeCapacity.get());
    }
  }

  // Rated Compressor Power Per Unit of Rated Evaporative Capacity: Optional Double
  double ratedCompressorPowerPerUnitofRatedEvaporativeCapacity = modelObject.ratedCompressorPowerPerUnitofRatedEvaporativeCapacity();
  idfObject.setDouble(AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl_HRFields::RatedCompressorPowerPerUnitofRatedEvaporativeCapacity, ratedCompressorPowerPerUnitofRatedEvaporativeCapacity);


  // Minimum Outdoor Air Temperature in Cooling Only Mode: Optional Double
  double minimumOutdoorAirTemperatureinCoolingOnlyMode = modelObject.minimumOutdoorAirTemperatureinCoolingOnlyMode();
  idfObject.setDouble(AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl_HRFields::MinimumOutdoorAirTemperatureinCoolingOnlyMode, minimumOutdoorAirTemperatureinCoolingOnlyMode);


  // Maximum Outdoor Air Temperature in Cooling Only Mode: Optional Double
  double maximumOutdoorAirTemperatureinCoolingOnlyMode = modelObject.maximumOutdoorAirTemperatureinCoolingOnlyMode();
  idfObject.setDouble(AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl_HRFields::MaximumOutdoorAirTemperatureinCoolingOnlyMode, maximumOutdoorAirTemperatureinCoolingOnlyMode);


  // Minimum Outdoor Air Temperature in Heating Only Mode: Optional Double
  double minimumOutdoorAirTemperatureinHeatingOnlyMode = modelObject.minimumOutdoorAirTemperatureinHeatingOnlyMode();
  idfObject.setDouble(AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl_HRFields::MinimumOutdoorAirTemperatureinHeatingOnlyMode, minimumOutdoorAirTemperatureinHeatingOnlyMode);


  // Maximum Outdoor Air Temperature in Heating Only Mode: Optional Double
  double maximumOutdoorAirTemperatureinHeatingOnlyMode = modelObject.maximumOutdoorAirTemperatureinHeatingOnlyMode();
  idfObject.setDouble(AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl_HRFields::MaximumOutdoorAirTemperatureinHeatingOnlyMode, maximumOutdoorAirTemperatureinHeatingOnlyMode);


  // Minimum Outdoor Temperature in Heat Recovery Mode: Optional Double
  double minimumOutdoorTemperatureinHeatRecoveryMode = modelObject.minimumOutdoorTemperatureinHeatRecoveryMode();
  idfObject.setDouble(AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl_HRFields::MinimumOutdoorTemperatureinHeatRecoveryMode, minimumOutdoorTemperatureinHeatRecoveryMode);


  // Maximum Outdoor Temperature in Heat Recovery Mode: Optional Double
  double maximumOutdoorTemperatureinHeatRecoveryMode = modelObject.maximumOutdoorTemperatureinHeatRecoveryMode();
  idfObject.setDouble(AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl_HRFields::MaximumOutdoorTemperatureinHeatRecoveryMode, maximumOutdoorTemperatureinHeatRecoveryMode);


  // Refrigerant Temperature Control Algorithm for Indoor Unit: Optional String
  std::string refrigerantTemperatureControlAlgorithmforIndoorUnit = modelObject.refrigerantTemperatureControlAlgorithmforIndoorUnit();
  idfObject.setString(AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl_HRFields::RefrigerantTemperatureControlAlgorithmforIndoorUnit, refrigerantTemperatureControlAlgorithmforIndoorUnit);


  // Reference Evaporating Temperature for Indoor Unit: Optional Double
  double referenceEvaporatingTemperatureforIndoorUnit = modelObject.referenceEvaporatingTemperatureforIndoorUnit();
  idfObject.setDouble(AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl_HRFields::ReferenceEvaporatingTemperatureforIndoorUnit, referenceEvaporatingTemperatureforIndoorUnit);


  // Reference Condensing Temperature for Indoor Unit: Optional Double
  double referenceCondensingTemperatureforIndoorUnit = modelObject.referenceCondensingTemperatureforIndoorUnit();
  idfObject.setDouble(AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl_HRFields::ReferenceCondensingTemperatureforIndoorUnit, referenceCondensingTemperatureforIndoorUnit);


  // Variable Evaporating Temperature Minimum for Indoor Unit: Optional Double
  double variableEvaporatingTemperatureMinimumforIndoorUnit = modelObject.variableEvaporatingTemperatureMinimumforIndoorUnit();
  idfObject.setDouble(AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl_HRFields::VariableEvaporatingTemperatureMinimumforIndoorUnit, variableEvaporatingTemperatureMinimumforIndoorUnit);


  // Variable Evaporating Temperature Maximum for Indoor Unit: Optional Double
  double variableEvaporatingTemperatureMaximumforIndoorUnit = modelObject.variableEvaporatingTemperatureMaximumforIndoorUnit();
  idfObject.setDouble(AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl_HRFields::VariableEvaporatingTemperatureMaximumforIndoorUnit, variableEvaporatingTemperatureMaximumforIndoorUnit);


  // Variable Condensing Temperature Minimum for Indoor Unit: Optional Double
  double variableCondensingTemperatureMinimumforIndoorUnit = modelObject.variableCondensingTemperatureMinimumforIndoorUnit();
  idfObject.setDouble(AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl_HRFields::VariableCondensingTemperatureMinimumforIndoorUnit, variableCondensingTemperatureMinimumforIndoorUnit);


  // Variable Condensing Temperature Maximum for Indoor Unit: Optional Double
  double variableCondensingTemperatureMaximumforIndoorUnit = modelObject.variableCondensingTemperatureMaximumforIndoorUnit();
  idfObject.setDouble(AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl_HRFields::VariableCondensingTemperatureMaximumforIndoorUnit, variableCondensingTemperatureMaximumforIndoorUnit);


  // Outdoor Unit Evaporator Reference Superheating: Optional Double
  double outdoorUnitEvaporatorReferenceSuperheating = modelObject.outdoorUnitEvaporatorReferenceSuperheating();
  idfObject.setDouble(AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl_HRFields::OutdoorUnitEvaporatorReferenceSuperheating, outdoorUnitEvaporatorReferenceSuperheating);


  // Outdoor Unit Condenser Reference Subcooling: Optional Double
  double outdoorUnitCondenserReferenceSubcooling = modelObject.outdoorUnitCondenserReferenceSubcooling();
  idfObject.setDouble(AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl_HRFields::OutdoorUnitCondenserReferenceSubcooling, outdoorUnitCondenserReferenceSubcooling);


  // Outdoor Unit Evaporator Rated Bypass Factor: Optional Double
  double outdoorUnitEvaporatorRatedBypassFactor = modelObject.outdoorUnitEvaporatorRatedBypassFactor();
  idfObject.setDouble(AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl_HRFields::OutdoorUnitEvaporatorRatedBypassFactor, outdoorUnitEvaporatorRatedBypassFactor);


  // Outdoor Unit Condenser Rated Bypass Factor: Optional Double
  double outdoorUnitCondenserRatedBypassFactor = modelObject.outdoorUnitCondenserRatedBypassFactor();
  idfObject.setDouble(AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl_HRFields::OutdoorUnitCondenserRatedBypassFactor, outdoorUnitCondenserRatedBypassFactor);


  // Difference between Outdoor Unit Evaporating Temperature and Outdoor Air Temperature in Heat Recovery Mode: Optional Double
  double differencebetweenOutdoorUnitEvaporatingTemperatureandOutdoorAirTemperatureinHeatRecoveryMode = modelObject.differencebetweenOutdoorUnitEvaporatingTemperatureandOutdoorAirTemperatureinHeatRecoveryMode();
  idfObject.setDouble(AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl_HRFields::DifferencebetweenOutdoorUnitEvaporatingTemperatureandOutdoorAirTemperatureinHeatRecoveryMode, differencebetweenOutdoorUnitEvaporatingTemperatureandOutdoorAirTemperatureinHeatRecoveryMode);


  // Outdoor Unit Heat Exchanger Capacity Ratio: Optional Double
  double outdoorUnitHeatExchangerCapacityRatio = modelObject.outdoorUnitHeatExchangerCapacityRatio();
  idfObject.setDouble(AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl_HRFields::OutdoorUnitHeatExchangerCapacityRatio, outdoorUnitHeatExchangerCapacityRatio);


  // Outdoor Unit Fan Power Per Unit of Rated Evaporative Capacity: Optional Double
  double outdoorUnitFanPowerPerUnitofRatedEvaporativeCapacity = modelObject.outdoorUnitFanPowerPerUnitofRatedEvaporativeCapacity();
  idfObject.setDouble(AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl_HRFields::OutdoorUnitFanPowerPerUnitofRatedEvaporativeCapacity, outdoorUnitFanPowerPerUnitofRatedEvaporativeCapacity);


  // Outdoor Unit Fan Flow Rate Per Unit of Rated Evaporative Capacity: Optional Double
  double outdoorUnitFanFlowRatePerUnitofRatedEvaporativeCapacity = modelObject.outdoorUnitFanFlowRatePerUnitofRatedEvaporativeCapacity();
  idfObject.setDouble(AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl_HRFields::OutdoorUnitFanFlowRatePerUnitofRatedEvaporativeCapacity, outdoorUnitFanFlowRatePerUnitofRatedEvaporativeCapacity);


  // Outdoor Unit Evaporating Temperature Function of Superheating Curve Name: Required Object
  UnivariateFunctions outdoorUnitEvaporatingTemperatureFunctionofSuperheatingCurve = modelObject.outdoorUnitEvaporatingTemperatureFunctionofSuperheatingCurve();
  if ( boost::optional<IdfObject> _owo = translateAndMapModelObject(outdoorUnitEvaporatingTemperatureFunctionofSuperheatingCurve) )  {
    idfObject.setString(AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl_HRFields::OutdoorUnitEvaporatingTemperatureFunctionofSuperheatingCurveName, _owo->nameString());
  }

  // Outdoor Unit Condensing Temperature Function of Subcooling Curve Name: Required Object
  UnivariateFunctions outdoorUnitCondensingTemperatureFunctionofSubcoolingCurve = modelObject.outdoorUnitCondensingTemperatureFunctionofSubcoolingCurve();
  if ( boost::optional<IdfObject> _owo = translateAndMapModelObject(outdoorUnitCondensingTemperatureFunctionofSubcoolingCurve) )  {
    idfObject.setString(AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl_HRFields::OutdoorUnitCondensingTemperatureFunctionofSubcoolingCurveName, _owo->nameString());
  }

  // Diameter of Main Pipe for Suction Gas: Optional Double
  double diameterofMainPipeforSuctionGas = modelObject.diameterofMainPipeforSuctionGas();
  idfObject.setDouble(AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl_HRFields::DiameterofMainPipeforSuctionGas, diameterofMainPipeforSuctionGas);


  // Diameter of Main Pipe for Discharge Gas: Optional Double
  double diameterofMainPipeforDischargeGas = modelObject.diameterofMainPipeforDischargeGas();
  idfObject.setDouble(AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl_HRFields::DiameterofMainPipeforDischargeGas, diameterofMainPipeforDischargeGas);


  // Length of Main Pipe Connecting Outdoor Unit to the First Branch Joint: Optional Double
  double lengthofMainPipeConnectingOutdoorUnittotheFirstBranchJoint = modelObject.lengthofMainPipeConnectingOutdoorUnittotheFirstBranchJoint();
  idfObject.setDouble(AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl_HRFields::LengthofMainPipeConnectingOutdoorUnittotheFirstBranchJoint, lengthofMainPipeConnectingOutdoorUnittotheFirstBranchJoint);


  // Equivalent Length of Main Pipe Connecting Outdoor Unit to the First Branch Joint: Optional Double
  double equivalentLengthofMainPipeConnectingOutdoorUnittotheFirstBranchJoint = modelObject.equivalentLengthofMainPipeConnectingOutdoorUnittotheFirstBranchJoint();
  idfObject.setDouble(AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl_HRFields::EquivalentLengthofMainPipeConnectingOutdoorUnittotheFirstBranchJoint, equivalentLengthofMainPipeConnectingOutdoorUnittotheFirstBranchJoint);


  // Height Difference Between Outdoor Unit and Indoor Units: Optional Double
  double heightDifferenceBetweenOutdoorUnitandIndoorUnits = modelObject.heightDifferenceBetweenOutdoorUnitandIndoorUnits();
  idfObject.setDouble(AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl_HRFields::HeightDifferenceBetweenOutdoorUnitandIndoorUnits, heightDifferenceBetweenOutdoorUnitandIndoorUnits);


  // Main Pipe Insulation Thickness: Optional Double
  double mainPipeInsulationThickness = modelObject.mainPipeInsulationThickness();
  idfObject.setDouble(AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl_HRFields::MainPipeInsulationThickness, mainPipeInsulationThickness);


  // Main Pipe Insulation Thermal Conductivity: Optional Double
  double mainPipeInsulationThermalConductivity = modelObject.mainPipeInsulationThermalConductivity();
  idfObject.setDouble(AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl_HRFields::MainPipeInsulationThermalConductivity, mainPipeInsulationThermalConductivity);


  // Crankcase Heater Power per Compressor: Optional Double
  double crankcaseHeaterPowerperCompressor = modelObject.crankcaseHeaterPowerperCompressor();
  idfObject.setDouble(AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl_HRFields::CrankcaseHeaterPowerperCompressor, crankcaseHeaterPowerperCompressor);


  // Number of Compressors: Optional Integer
  int numberofCompressors = modelObject.numberofCompressors();
  idfObject.setInt(AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl_HRFields::NumberofCompressors, numberofCompressors);


  // Ratio of Compressor Size to Total Compressor Capacity: Optional Double
  double ratioofCompressorSizetoTotalCompressorCapacity = modelObject.ratioofCompressorSizetoTotalCompressorCapacity();
  idfObject.setDouble(AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl_HRFields::RatioofCompressorSizetoTotalCompressorCapacity, ratioofCompressorSizetoTotalCompressorCapacity);


  // Maximum Outdoor Dry-Bulb Temperature for Crankcase Heater: Optional Double
  double maximumOutdoorDryBulbTemperatureforCrankcaseHeater = modelObject.maximumOutdoorDryBulbTemperatureforCrankcaseHeater();
  idfObject.setDouble(AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl_HRFields::MaximumOutdoorDryBulbTemperatureforCrankcaseHeater, maximumOutdoorDryBulbTemperatureforCrankcaseHeater);


  // Defrost Strategy: Optional String
  std::string defrostStrategy = modelObject.defrostStrategy();
  idfObject.setString(AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl_HRFields::DefrostStrategy, defrostStrategy);


  // Defrost Control: Optional String
  std::string defrostControl = modelObject.defrostControl();
  idfObject.setString(AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl_HRFields::DefrostControl, defrostControl);


  // Defrost Energy Input Ratio Modifier Function of Temperature Curve Name: Optional Object
  if (boost::optional<BivariateFunctions> _defrostEnergyInputRatioModifierFunctionofTemperatureCurve = modelObject.defrostEnergyInputRatioModifierFunctionofTemperatureCurve()) {
    if ( boost::optional<IdfObject> _owo = translateAndMapModelObject(_defrostEnergyInputRatioModifierFunctionofTemperatureCurve.get()) )  {
      idfObject.setString(AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl_HRFields::DefrostEnergyInputRatioModifierFunctionofTemperatureCurveName, _owo->nameString());
    }
  }

  // Defrost Time Period Fraction: Optional Double
  double defrostTimePeriodFraction = modelObject.defrostTimePeriodFraction();
  idfObject.setDouble(AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl_HRFields::DefrostTimePeriodFraction, defrostTimePeriodFraction);


  if (modelObject.isResistiveDefrostHeaterCapacityAutosized()) {
    idfObject.setString(AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl_HRFields::ResistiveDefrostHeaterCapacity, "Autosize");
  } else {
    // Resistive Defrost Heater Capacity: boost::optional<double>
    if (boost::optional<double> _resistiveDefrostHeaterCapacity = modelObject.resistiveDefrostHeaterCapacity()) {
      idfObject.setDouble(AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl_HRFields::ResistiveDefrostHeaterCapacity, _resistiveDefrostHeaterCapacity.get());
    }
  }

  // Maximum Outdoor Dry-bulb Temperature for Defrost Operation: Optional Double
  double maximumOutdoorDrybulbTemperatureforDefrostOperation = modelObject.maximumOutdoorDrybulbTemperatureforDefrostOperation();
  idfObject.setDouble(AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl_HRFields::MaximumOutdoorDrybulbTemperatureforDefrostOperation, maximumOutdoorDrybulbTemperatureforDefrostOperation);


  // Initial Heat Recovery Cooling Capacity Fraction: Optional Double
  double initialHeatRecoveryCoolingCapacityFraction = modelObject.initialHeatRecoveryCoolingCapacityFraction();
  idfObject.setDouble(AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl_HRFields::InitialHeatRecoveryCoolingCapacityFraction, initialHeatRecoveryCoolingCapacityFraction);


  // Heat Recovery Cooling Capacity Time Constant: Optional Double
  double heatRecoveryCoolingCapacityTimeConstant = modelObject.heatRecoveryCoolingCapacityTimeConstant();
  idfObject.setDouble(AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl_HRFields::HeatRecoveryCoolingCapacityTimeConstant, heatRecoveryCoolingCapacityTimeConstant);


  // Initial Heat Recovery Cooling Energy Fraction: Optional Double
  double initialHeatRecoveryCoolingEnergyFraction = modelObject.initialHeatRecoveryCoolingEnergyFraction();
  idfObject.setDouble(AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl_HRFields::InitialHeatRecoveryCoolingEnergyFraction, initialHeatRecoveryCoolingEnergyFraction);


  // Heat Recovery Cooling Energy Time Constant: Optional Double
  double heatRecoveryCoolingEnergyTimeConstant = modelObject.heatRecoveryCoolingEnergyTimeConstant();
  idfObject.setDouble(AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl_HRFields::HeatRecoveryCoolingEnergyTimeConstant, heatRecoveryCoolingEnergyTimeConstant);


  // Initial Heat Recovery Heating Capacity Fraction: Optional Double
  double initialHeatRecoveryHeatingCapacityFraction = modelObject.initialHeatRecoveryHeatingCapacityFraction();
  idfObject.setDouble(AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl_HRFields::InitialHeatRecoveryHeatingCapacityFraction, initialHeatRecoveryHeatingCapacityFraction);


  // Heat Recovery Heating Capacity Time Constant: Optional Double
  double heatRecoveryHeatingCapacityTimeConstant = modelObject.heatRecoveryHeatingCapacityTimeConstant();
  idfObject.setDouble(AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl_HRFields::HeatRecoveryHeatingCapacityTimeConstant, heatRecoveryHeatingCapacityTimeConstant);


  // Initial Heat Recovery Heating Energy Fraction: Optional Double
  double initialHeatRecoveryHeatingEnergyFraction = modelObject.initialHeatRecoveryHeatingEnergyFraction();
  idfObject.setDouble(AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl_HRFields::InitialHeatRecoveryHeatingEnergyFraction, initialHeatRecoveryHeatingEnergyFraction);


  // Heat Recovery Heating Energy Time Constant: Optional Double
  double heatRecoveryHeatingEnergyTimeConstant = modelObject.heatRecoveryHeatingEnergyTimeConstant();
  idfObject.setDouble(AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl_HRFields::HeatRecoveryHeatingEnergyTimeConstant, heatRecoveryHeatingEnergyTimeConstant);


  // Compressor maximum delta Pressure: Optional Double
  double compressormaximumdeltaPressure = modelObject.compressormaximumdeltaPressure();
  idfObject.setDouble(AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl_HRFields::CompressormaximumdeltaPressure, compressormaximumdeltaPressure);


  // Compressor Inverter Efficiency: Optional Double
  double compressorInverterEfficiency = modelObject.compressorInverterEfficiency();
  idfObject.setDouble(AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl_HRFields::CompressorInverterEfficiency, compressorInverterEfficiency);


  // Compressor Evaporative Capacity Correction Factor: Optional Double
  double compressorEvaporativeCapacityCorrectionFactor = modelObject.compressorEvaporativeCapacityCorrectionFactor();
  idfObject.setDouble(AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl_HRFields::CompressorEvaporativeCapacityCorrectionFactor, compressorEvaporativeCapacityCorrectionFactor);


  // Number of Compressor Loading Index Entries: Optional Integer
  int numberofCompressorLoadingIndexEntries = modelObject.numberofCompressorLoadingIndexEntries();
  idfObject.setInt(AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl_HRFields::NumberofCompressorLoadingIndexEntries, numberofCompressorLoadingIndexEntries);


  return idfObject;
} // End of translate function

} // end namespace energyplus
} // end namespace openstudio