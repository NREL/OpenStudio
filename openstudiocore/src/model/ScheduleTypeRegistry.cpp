/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2017, Alliance for Sustainable Energy, LLC. All rights reserved.
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

#include "ScheduleTypeRegistry.hpp"
#include "ScheduleTypeLimits.hpp"
#include "ScheduleTypeLimits_Impl.hpp"
#include "Schedule.hpp"
#include "Schedule_Impl.hpp"
#include "Model.hpp"

#include "../utilities/units/Quantity.hpp"
#include "../utilities/units/OSOptionalQuantity.hpp"

#include "../utilities/core/Containers.hpp"

namespace openstudio {
namespace model {

OSOptionalQuantity ScheduleType::getLowerLimitValue(bool returnIP) const {
  if (OptionalUnit u = ScheduleTypeLimits::units(unitType,returnIP)) {
    if (lowerLimitValue) {
      return OSOptionalQuantity(Quantity(*lowerLimitValue,*u));
    }
    return OSOptionalQuantity(*u);
  }
  LOG_FREE_AND_THROW("openstudio.model.ScheduleTypeRegistry",
                     "Cannot return any units for unitType " << unitType << ".");
  return OSOptionalQuantity(UnitSystem(UnitSystem::SI));
}

OSOptionalQuantity ScheduleType::getUpperLimitValue(bool returnIP) const {
  if (OptionalUnit u = ScheduleTypeLimits::units(unitType,returnIP)) {
    if (upperLimitValue) {
      return OSOptionalQuantity(Quantity(*upperLimitValue,*u));
    }
    return OSOptionalQuantity(*u);
  }
  LOG_FREE_AND_THROW("openstudio.model.ScheduleTypeRegistry",
                     "Cannot return any units for unitType " << unitType << ".");
  return OSOptionalQuantity(UnitSystem(UnitSystem::SI));
}

std::vector<std::string> ScheduleTypeRegistrySingleton::classNames() const {
  StringVector result;
  for (const ClassNameToScheduleTypesMap::value_type& p : m_classNameToScheduleTypesMap) {
    result.push_back(p.first);
  }
  return result;
}

std::vector<ScheduleType> ScheduleTypeRegistrySingleton::getScheduleTypesByClassName(
    const std::string& className) const
{
  ScheduleTypeVector result;
  auto it = m_classNameToScheduleTypesMap.find(className);
  if (it != m_classNameToScheduleTypesMap.end()) {
    result = it->second;
  }
  return result;
}

ScheduleType ScheduleTypeRegistrySingleton::getScheduleType(const std::string &className,
                                                            const std::string &scheduleDisplayName) const
{
  ScheduleTypeVector scheduleTypes = getScheduleTypesByClassName(className);
  for (const ScheduleType& scheduleType : scheduleTypes) {
    if (scheduleType.scheduleDisplayName == scheduleDisplayName) {
      return scheduleType;
    }
  }
  LOG_AND_THROW("No '" << scheduleDisplayName << "' Schedule is registered for class "
                << className << ".");
  return ScheduleType();
}

ScheduleTypeLimits ScheduleTypeRegistrySingleton::getOrCreateScheduleTypeLimits(const ScheduleType& scheduleType,
                                                                                Model& model) const
{
  std::string defaultName = getDefaultName(scheduleType);

  // DLM: I do not understand why both upper and lower limit have to be set to reuse this?
  //// if fully specified, try to retrieve
  //if (scheduleType.lowerLimitValue && scheduleType.upperLimitValue) {
  ScheduleTypeLimitsVector candidates = model.getConcreteModelObjectsByName<ScheduleTypeLimits>(defaultName);
    for (const ScheduleTypeLimits& candidate : candidates) {
      if (isCompatible(scheduleType,candidate)) {
        return candidate;
      }
    }
  //}

  // otherwise, or if not there, create and return
  ScheduleTypeLimits scheduleTypeLimits(model);

  scheduleTypeLimits.setName(defaultName);

  if (scheduleType.lowerLimitValue) {
    scheduleTypeLimits.setLowerLimitValue(scheduleType.lowerLimitValue.get());
  }
  if (scheduleType.upperLimitValue) {
    scheduleTypeLimits.setUpperLimitValue(scheduleType.upperLimitValue.get());
  }
  if (scheduleType.isContinuous) {
    scheduleTypeLimits.setNumericType("Continuous");
  }
  else {
    scheduleTypeLimits.setNumericType("Discrete");
  }
  if (!scheduleType.unitType.empty()) {
    scheduleTypeLimits.setUnitType(scheduleType.unitType);
  }

  return scheduleTypeLimits;
}

ScheduleTypeRegistrySingleton::ScheduleTypeRegistrySingleton()
{
  // className, scheduleDisplayName, scheduleRelationshipName, isContinuous, unitType, lowerLimitValue, upperLimitValue;

  const ScheduleType scheduleTypes[] =
  {
    {"AirConditionerVariableRefrigerantFlow","Availability Schedule","availabilitySchedule",false,"Availability",0.0,1.0},
    {"AirConditionerVariableRefrigerantFlow","Thermostat Priority Schedule","thermostatPrioritySchedule",false,"ControlMode",0.0,1.0},
    {"AirConditionerVariableRefrigerantFlow","Basin Heater Operating Schedule","basinHeaterOperatingSchedule",false,"Availability",0.0,1.0},
    {"AirLoopHVAC","Return Air Bypass Flow Temperature Setpoint","returnAirBypassFlowTemperatureSetpointSchedule",true,"Temperature",OptionalDouble(),OptionalDouble()},
    {"AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass","Availability Schedule","availabilitySchedule",false,"Availability",0.0,1.0},
    {"AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass","Outdoor Air Flow Rate Multiplier Schedule","outdoorAirFlowRateMultiplierSchedule",true,"",0.0,1.0},
    {"AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass","Supply Air Fan Operating Mode Schedule","supplyAirFanOperatingModeSchedule",false,"ControlMode",0.0,1.0},
    {"AirLoopHVACUnitaryHeatPumpAirToAir","Availability","availabilitySchedule",false,"Availability",0.0,1.0},
    {"AirLoopHVACUnitaryHeatPumpAirToAir","Supply Air Fan Operating Mode","supplyAirFanOperatingModeSchedule",false,"ControlMode",0.0,1.0},
    {"AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed","Availability Schedule","availabilitySchedule",false,"Availability",0.0,1.0},
    {"AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed","Supply Air Fan Operating Mode Schedule","supplyAirFanOperatingModeSchedule",false,"ControlMode",0.0,1.0},
    {"AirLoopHVACUnitarySystem","Availability","availabilitySchedule",false,"Availability",0.0,1.0},
    {"AirLoopHVACUnitarySystem","Supply Air Fan Operating Mode","supplyAirFanOperatingModeSchedule",false,"ControlMode",0.0,1.0},
    {"AirTerminalDualDuctVAV","Availability Schedule","availabilitySchedule",false,"Availability",0.0,1.0},
    {"AirTerminalSingleDuctConstantVolumeCooledBeam","Availability","availabilitySchedule",false,"Availability",0.0,1.0},
    {"AirTerminalSingleDuctConstantVolumeFourPipeInduction","Availability","availabilitySchedule",false,"Availability",0.0,1.0},
    {"AirTerminalSingleDuctConstantVolumeReheat","Availability","availabilitySchedule",false,"Availability",0.0,1.0},
    {"AirTerminalSingleDuctParallelPIUReheat","Availability","availabilitySchedule",false,"Availability",0.0,1.0},
    {"AirTerminalSingleDuctSeriesPIUReheat","Availability","availabilitySchedule",false,"Availability",0.0,1.0},
    {"AirTerminalSingleDuctUncontrolled","Availability","availabilitySchedule",false,"Availability",0.0,1.0},
    {"AirTerminalSingleDuctVAVReheat","Availability","availabilitySchedule",false,"Availability",0.0,1.0},
    {"AirTerminalSingleDuctVAVReheat","Minimum Air Flow Fraction","minimumAirFlowFractionSchedule",true,"",0.0,1.0},
    {"AirTerminalSingleDuctVAVNoReheat","Availability","availabilitySchedule",false,"Availability",0.0,1.0},
    {"AirTerminalSingleDuctVAVNoReheat","Minimum Air Flow Fraction","minimumAirFlowFractionSchedule",true,"",0.0,1.0},
    {"AirTerminalSingleDuctVAVHeatAndCoolNoReheat","Availability Schedule","availabilitySchedule",false,"Availability",0.0,1.0},
    {"AirTerminalSingleDuctVAVHeatAndCoolReheat","Availability Schedule","availabilitySchedule",false,"Availability",0.0,1.0},
    {"AvailabilityManagerScheduled","Availability","schedule",false,"Availability",0.0,1.0},
    {"AvailabilityManagerHybridVentilation","Ventilation Control Mode Schedule","ventilationControlModeSchedule",false,"",0.0,4.0},
    {"AvailabilityManagerHybridVentilation","Minimum Outdoor Ventilation Air Schedule","minimumOutdoorVentilationAirSchedule",true,"Temperature",OptionalDouble(),OptionalDouble()},
    {"AvailabilityManagerHybridVentilation","AirflowNetwork Control Type Schedule","airflowNetworkControlTypeSchedule",false,"ControlMode",0.0,1.0},
    {"AvailabilityManagerHybridVentilation","Simple Airflow Control Type Schedule","simpleAirflowControlTypeSchedule",false,"ControlMode",0.0,1.0},
    {"AvailabilityManagerNightVentilation","Applicability Schedule","applicabilitySchedule",false,"Availability",0.0,1.0},
    {"AvailabilityManagerNightVentilation","Ventilation Temperature Schedule","ventilationTemperatureSchedule",true,"Temperature",OptionalDouble(),OptionalDouble()},
    {"AvailabilityManagerOptimumStart","Applicability Schedule","applicabilitySchedule",false,"Availability",0.0,1.0},
    {"CentralHeatPumpSystem","Ancillary Operation","ancillaryOperationSchedule",false,"Availability",0.0,1.0},
    {"CentralHeatPumpSystemModule","Chiller Heater Modules Control","chillerHeaterModulesControlSchedule",false,"Availability",0.0,1.0},
    {"ChillerElectricEIR","Basin Heater","basinHeaterSchedule",false,"Availability",0.0,1.0},
    {"CoilCoolingCooledBeam","Availability","availabilitySchedule",false,"Availability",0.0,1.0},
    {"CoilCoolingDXSingleSpeed","Availability","availabilitySchedule",false,"Availability",0.0,1.0},
    {"CoilCoolingDXSingleSpeed","Basin Heater Operation","basinHeaterOperatingSchedule",false,"Availability",0.0,1.0},
    {"CoilCoolingDXTwoSpeed","Availability","availabilitySchedule",false,"Availability",0.0,1.0},
    {"CoilCoolingDXTwoSpeed","Basin Heater Operation","basinHeaterOperatingSchedule",false,"Availability",0.0,1.0},
    {"CoilCoolingDXTwoStageWithHumidityControlMode","Availability Schedule","availabilitySchedule",false,"Availability",0.0,1.0},
    {"CoilCoolingDXTwoStageWithHumidityControlMode","Basin Heater Operating Schedule","basinHeaterOperatingSchedule",false,"Availability",0.0,1.0},
    {"CoilCoolingDXMultiSpeed","Availability Schedule","availabilitySchedule",false,"Availability",0.0,1.0},
    {"CoilCoolingDXMultiSpeed","Basin Heater Operating Schedule","basinHeaterOperatingSchedule",false,"Availability",0.0,1.0},
    {"CoilCoolingDXVariableRefrigerantFlow","Availability Schedule","availabilitySchedule",false,"Availability",0.0,1.0},
    {"CoilCoolingDXVariableSpeed","Basin Heater Operating","basinHeaterOperatingSchedule",false,"Availability",0.0,1.0},
    {"CoilCoolingLowTempRadiantConstFlow","Cooling High Water Temperature","coolingHighWaterTemperatureSchedule",true,"Temperature",OptionalDouble(),OptionalDouble()},
    {"CoilCoolingLowTempRadiantConstFlow","Cooling Low Water Temperature","coolingLowWaterTemperatureSchedule",true,"Temperature",OptionalDouble(),OptionalDouble()},
    {"CoilCoolingLowTempRadiantConstFlow","Cooling High Control Temperature","coolingHighControlTemperatureSchedule",true,"Temperature",OptionalDouble(),OptionalDouble()},
    {"CoilCoolingLowTempRadiantConstFlow","Cooling Low Control Temperature","coolingLowControlTemperatureSchedule",true,"Temperature",OptionalDouble(),OptionalDouble()},
    {"CoilCoolingLowTempRadiantVarFlow","Cooling Control Temperature Schedule","coolingControlTemperatureSchedule",true,"Temperature",OptionalDouble(),OptionalDouble()},
    {"CoilCoolingWater","Availability","availabilitySchedule",false,"Availability",0.0,1.0},
    {"CoilHeatingDesuperheater","Availability","availabilitySchedule",false,"Availability",0.0,1.0},
    {"CoilHeatingDXMultiSpeed","Availability","availabilitySchedule",false,"Availability",0.0,1.0},
    {"CoilHeatingDXSingleSpeed","Availability","availabilitySchedule",false,"Availability",0.0,1.0},
    {"CoilHeatingDXVariableRefrigerantFlow","Availability Schedule","availabilitySchedule",false,"Availability",0.0,1.0},
    {"CoilHeatingElectric","Availability","availabilitySchedule",false,"Availability",0.0,1.0},
    {"CoilHeatingGas","Availability","availabilitySchedule",false,"Availability",0.0,1.0},
    {"CoilHeatingGasMultiStage","Availability Schedule","availabilitySchedule",false,"Availability",0.0,1.0},
    {"CoilHeatingLowTempRadiantConstFlow","Heating High Water Temperature","heatingHighWaterTemperatureSchedule",true,"Temperature",OptionalDouble(),OptionalDouble()},
    {"CoilHeatingLowTempRadiantConstFlow","Heating Low Water Temperature","heatingLowWaterTemperatureSchedule",true,"Temperature",OptionalDouble(),OptionalDouble()},
    {"CoilHeatingLowTempRadiantConstFlow","Heating High Control Temperature","heatingHighControlTemperatureSchedule",true,"Temperature",OptionalDouble(),OptionalDouble()},
    {"CoilHeatingLowTempRadiantConstFlow","Heating Low Control Temperature","heatingLowControlTemperatureSchedule",true,"Temperature",OptionalDouble(),OptionalDouble()},
    {"CoilHeatingLowTempRadiantVarFlow","Heating Control Temperature","heatingControlTemperature",true,"Temperature",OptionalDouble(),OptionalDouble()},
    {"CoilHeatingWater","Availability","availabilitySchedule",false,"Availability",0.0,1.0},
    {"CoilHeatingWaterBaseboard","Availability","availabilitySchedule",false,"Availability",0.0,1.0},
    {"CoilWaterHeatingDesuperheater","Availability","availabilitySchedule",false,"Availability",0.0,1.0},
    {"CoilWaterHeatingDesuperheater","Setpoint Temperature","setpointTemperatureSchedule",true,"Temperature",OptionalDouble(),OptionalDouble()},
    {"ControllerOutdoorAir","Minimum Outdoor Air","minimumOutdoorAirSchedule",true,"",0.0,1.0},
    {"ControllerOutdoorAir","Minimum Fraction of Outdoor Air","minimumFractionofOutdoorAirSchedule",true,"",0.0,1.0},
    {"ControllerOutdoorAir","Maximum Fraction of Outdoor Air","maximumFractionofOutdoorAirSchedule",true,"",0.0,1.0},
    {"ControllerOutdoorAir","Time of Day Economizer Control","timeofDayEconomizerControlSchedule",false,"Availability",0.0,1.0},
    {"ControllerMechanicalVentilation","Availability Schedule","availabilitySchedule",false,"Availability",0.0,1.0},
    {"CoolingTowerSingleSpeed","Basin Heater Operating","basinHeaterOperatingSchedule",false,"Availability",0.0,1.0},
    {"CoolingTowerSingleSpeed","Blowdown Makeup Water Usage","blowdownMakeupWaterUsageSchedule",true,"VolumetricFlowRate",0.0,OptionalDouble()},
    {"CoolingTowerTwoSpeed","Basin Heater Operating","basinHeaterOperatingSchedule",false,"Availability",0.0,1.0},
    {"CoolingTowerTwoSpeed","Blowdown Makeup Water Usage","blowdownMakeupWaterUsageSchedule",true,"VolumetricFlowRate",0.0,OptionalDouble()},
    {"CoolingTowerVariableSpeed","Basin Heater Operating Schedule","basinHeaterOperatingSchedule",false,"Availability",0.0,1.0},
    {"CoolingTowerVariableSpeed","Blowdown Makeup Water Usage Schedule","blowdownMakeupWaterUsageSchedule",true,"VolumetricFlowRate",0.0,OptionalDouble()},
    {"DefaultScheduleSet","Hours of Operation","hoursofOperationSchedule",false,"Availability",0.0,1.0},
    {"DefaultScheduleSet","Number of People","numberofPeopleSchedule",true,"",0.0,1.0},
    {"DefaultScheduleSet","People Activity Level","peopleActivityLevelSchedule",true,"ActivityLevel",0.0,OptionalDouble()},
    {"DefaultScheduleSet","Lighting","lightingSchedule",true,"",0.0,1.0},
    {"DefaultScheduleSet","Electric Equipment","electricEquipmentSchedule",true,"",0.0,1.0},
    {"DefaultScheduleSet","Gas Equipment","gasEquipmentSchedule",true,"",0.0,1.0},
    {"DefaultScheduleSet","Hot Water Equipment","hotWaterEquipmentSchedule",true,"",0.0,1.0},
    {"DefaultScheduleSet","Infiltration","infiltrationSchedule",true,"",0.0,1.0},
    {"DefaultScheduleSet","Steam Equipment","steamEquipmentSchedule",true,"",0.0,1.0},
    {"DefaultScheduleSet","Other Equipment","otherEquipmentSchedule",true,"",OptionalDouble(),OptionalDouble()},
    {"DesignSpecificationOutdoorAir","Outdoor Air Flow Rate","outdoorAirFlowRateFractionSchedule",true,"",0.0,1.0},
    {"ElectricEquipment","Electric Equipment","schedule",true,"",0.0,1.0},
    {"ElectricLoadCenterDistribution", "Track  Scheme", "trackScheduleSchemeSchedule",true,"",0.0,OptionalDouble()},
    {"ElectricLoadCenterDistribution", "Storage Charge Power Fraction", "storageChargePowerFractionSchedule",true,"",0.0,1.0},
    {"ElectricLoadCenterDistribution", "Storage Discharge Power Fraction", "storageDischargePowerFractionSchedule",true,"",0.0,1.0},
    {"ElectricLoadCenterDistribution", "Storage Control Utility Demand Target Fraction", "storageControlUtilityDemandTargetFractionScheduleName",true,"",-1.0,1.0},
    {"ElectricLoadCenterInverterLookUpTable","Availability","availabilitySchedule",false,"Availability",0.0,1.0},
    {"ElectricLoadCenterInverterSimple","Availability","availabilitySchedule",false,"Availability",0.0,1.0},
    {"ElectricLoadCenterStorageSimple","Availability","availabilitySchedule",false,"Availability",0.0,1.0},
    {"ElectricLoadCenterStorageConverter","Availability","availabilitySchedule",false,"Availability",0.0,1.0},
    {"EvaporativeCoolerDirectResearchSpecial","Availability","availabilitySchedule",false,"Availability",0.0,1.0},
    {"EvaporativeCoolerIndirectResearchSpecial","Availability","availabilitySchedule",false,"Availability",0.0,1.0},
    {"EvaporativeFluidCoolerSingleSpeed","Blowdown Makeup Water Usage","blowdownMakeupWaterUsageSchedule",true,"VolumetricFlowRate",0.0,OptionalDouble()},
    {"EvaporativeFluidCoolerTwoSpeed","Blowdown Makeup Water Usage","blowdownMakeupWaterUsageSchedule",true,"VolumetricFlowRate",0.0,OptionalDouble()},
    {"ExteriorLights","Exterior Lights","schedule",true,"",0.0,1.0},
    {"FanConstantVolume","Availability","availabilitySchedule",false,"Availability",0.0,1.0},
    {"FanOnOff","Availability","availabilitySchedule",false,"Availability",0.0,1.0},
    {"FanVariableVolume","Availability","availabilitySchedule",false,"Availability",0.0,1.0},
    {"FanZoneExhaust","Availability","availabilitySchedule",false,"Availability",0.0,1.0},
    {"FanZoneExhaust","Flow Fraction","flowFractionSchedule",true,"Dimensionless",0.0,1.0},
    {"FanZoneExhaust","Minimum Zone Temperature Limit","minimumZoneTemperatureLimitSchedule",true,"Temperature",OptionalDouble(),OptionalDouble()},
    {"FanZoneExhaust","Balanced Exhaust Fraction","balancedExhaustFractionSchedule",true,"Dimensionless",0.0,1.0},    
    {"GasEquipment","Gas Equipment","schedule",true,"",0.0,1.0},
    {"GeneratorFuelSupply", "Fuel Temperature", "fuelTemperatureScheduleName", true,"Temperature", 0.0, 100.0},
    {"GeneratorFuelCellWaterSupply", "Water Temperature", "waterTemperatureScheduleName", true, "Temperature", 0.0, 100.0},
    {"GeneratorMicroTurbine","Availability","availabilitySchedule",false,"Availability",0.0,1.0},
    {"GeneratorPhotovoltaic","Availability","availabilitySchedule",false,"Availability",0.0,1.0},
    {"HeatExchangerAirToAirSensibleAndLatent","Availability","availabilitySchedule",false,"Availability",0.0,1.0},
    {"HeatExchangerFluidToFluid","Availability","availabilitySchedule",false,"Availability",0.0,1.0},
    {"HumidifierSteamElectric","Availability","availabilitySchedule",false,"Availability",0.0,1.0},
    {"HotWaterEquipment","Hot Water Equipment","schedule",true,"",0.0,1.0},
    {"LoadProfilePlant","Load","loadSchedule",true,"",OptionalDouble(),OptionalDouble()},
    {"LoadProfilePlant","Flow Rate Fraction","flowRateFractionSchedule",true,"",0.0,1.0},
    {"Lights","Lighting","schedule",true,"",0.0,1.0},
    {"Luminaire","Luminaire","schedule",true,"",0.0,1.0},
    {"OtherEquipment","Other Equipment","schedule",true,"",OptionalDouble(),OptionalDouble()},
    {"OutputVariable","Active Hours","schedule",false,"Availability",0.0,1.0},
    {"People","Number of People","numberofPeopleSchedule",true,"",0.0,1.0},
    {"People","Activity Level","activityLevelSchedule",true,"ActivityLevel",0.0,OptionalDouble()},
    {"People","Work Efficiency","workEfficiencySchedule",true,"",0.0,1.0},
    {"People","Clothing Insulation","clothingInsulationSchedule",true,"ClothingInsulation",0.0,OptionalDouble()},
    {"People","Air Velocity","airVelocitySchedule",true,"Velocity",0.0,OptionalDouble()},
    {"PhotovoltaicPerformanceSimple","Efficiency","efficiencySchedule",true,"",0.0,1.0},
    {"PlantComponentTemperatureSource","Source Temperature","sourceTemperatureSchedule",true,"Temperature",OptionalDouble(),OptionalDouble()},
    {"PipeIndoor", "Ambient Temperature", "ambientTemperatureSchedule", true, "Temperature", OptionalDouble(), OptionalDouble() },
    {"PipeIndoor", "Ambient Air Velocity", "ambientAirVelocitySchedule", true, "Velocity", 0.0, OptionalDouble() },
    {"PlantLoop","Plant Equipment Operation Heating Load Schedule","plantEquipmentOperationHeatingLoadSchedule",false,"Availability",0.0,1.0},
    {"PlantLoop","Plant Equipment Operation Cooling Load Schedule","plantEquipmentOperationCoolingLoadSchedule",false,"Availability",0.0,1.0},
    {"PlantLoop","Primary Plant Equipment Operation Scheme Schedule","primaryPlantEquipmentOperationSchemeSchedule",false,"Availability",0.0,1.0},
    {"PlantLoop","Component Setpoint Operation Scheme Schedule","componentSetpointOperationSchemeSchedule",false,"Availability",0.0,1.0},
    {"PumpConstantSpeed","Pump Flow Rate","pumpFlowRateSchedule",true,"",0.0,1.0},
    {"PumpVariableSpeed","Pump Flow Rate","pumpFlowRateSchedule",true,"",0.0,1.0},
    {"PumpVariableSpeed","Pump RPM","pumpRPMSchedule",true,"RotationsPerMinute",OptionalDouble(),OptionalDouble()},
    {"PumpVariableSpeed","Minimum Pressure","minimumPressureSchedule",true,"Pressure",OptionalDouble(),OptionalDouble()},
    {"PumpVariableSpeed","Maximum Pressure","maximumPressureSchedule",true,"Pressure",OptionalDouble(),OptionalDouble()},
    {"PumpVariableSpeed","Minimum RPM","minimumRPMSchedule",true,"RotationsPerMinute",OptionalDouble(),OptionalDouble()},
    {"PumpVariableSpeed","Maximum RPM","maximumRPMSchedule",true,"RotationsPerMinute",OptionalDouble(),OptionalDouble()},
    {"HeaderedPumpsConstantSpeed","Pump Flow Rate Schedule","pumpFlowRateSchedule",true,"",0.0,1.0},
    {"HeaderedPumpsVariableSpeed","Pump Flow Rate Schedule","pumpFlowRateSchedule",true,"",0.0,1.0},
    {"RefrigerationCase","Availability","availabilitySchedule",false,"Availability",0.0,1.0},
    {"RefrigerationCase","Case Lighting","caseLightingSchedule",true,"",0.0,1.0},
    {"RefrigerationCase","Case Defrost","caseDefrostSchedule",true,"",0.0,1.0},
    {"RefrigerationCase","Case Defrost Drip-Down","caseDefrostDripDownSchedule",true,"",0.0,1.0},
    {"RefrigerationCase","Refrigerated Case Restocking","refrigeratedCaseRestockingSchedule",true,"LinearPowerDensity",0.0,OptionalDouble()},
    {"RefrigerationCase","Case Credit Fraction","caseCreditFractionSchedule",true,"",0.0,1.0,},
    {"RefrigerationCondenserEvaporativeCooled","Evaporative Condenser Availability","evaporativeCondenserAvailabilitySchedule",false,"Availability",0.0,1.0},
    {"RefrigerationCondenserWaterCooled","Water Outlet Temperature","waterOutletTemperatureSchedule",true,"Temperature",OptionalDouble(),OptionalDouble()},
    {"RefrigerationWalkIn","Availability","availabilitySchedule",false,"Availability",0.0,1.0},
    {"RefrigerationWalkIn","Heating Power","heatingPowerSchedule",true,"",0.0,1.0},
    {"RefrigerationWalkIn","Lighting","lightingSchedule",true,"",0.0,1.0},
    {"RefrigerationWalkIn","Defrost","defrostSchedule",true,"",0.0,1.0},
    {"RefrigerationWalkIn","Defrost Drip-Down","defrostDripDownSchedule",true,"",0.0,1.0},
    {"RefrigerationWalkIn","Restocking","restockingSchedule",true,"Capacity",0.0,OptionalDouble()},
    {"RefrigerationWalkInZoneBoundary","Glass Reach In Door Opening  Facing Zone","glassReachInDoorOpeningScheduleFacingZone",true,"",0.0,1.0},
    {"RefrigerationWalkInZoneBoundary","Stocking Door Opening  Facing Zone","stockingDoorOpeningScheduleFacingZone",true,"",0.0,1.0},
    {"SetpointManagerScheduled","(Exact, Min, Max) Temperature","schedule",true,"Temperature",OptionalDouble(),OptionalDouble()},
    {"SetpointManagerScheduled","(Exact, Min, Max) Humidity Ratio","schedule",true,"",0.0,OptionalDouble()},
    {"SetpointManagerScheduled","(Exact, Min, Max) Mass Flow Rate","schedule",true,"MassFlowRate",OptionalDouble(),OptionalDouble()},
    {"SetpointManagerScheduledDualSetpoint","High Setpoint","highSetpointSchedule",true,"Temperature",OptionalDouble(),OptionalDouble()},
    {"SetpointManagerScheduledDualSetpoint","Low Setpoint","lowSetpointSchedule",true,"Temperature",OptionalDouble(),OptionalDouble()},
    {"SetpointManagerOutdoorAirReset","Setpoint Manager Outdoor Air Reset","schedule",true,"",0.0,OptionalDouble()},
    {"ShadingSurface","Transmittance","transmittanceSchedule",true,"",0.0,1.0},
    {"SiteWaterMainsTemperature","Temperature","temperatureSchedule",true,"Temperature",OptionalDouble(),OptionalDouble()},
    { "SolarCollectorPerformancePhotovoltaicThermalSimple", "Thermal Conversion Efficiency", "thermalConversionEfficiencySchedule", true, "", 0.0, 1.0 },
    {"SurfacePropertyConvectionCoefficients", "Convection Coefficient 1", "convectionCoefficient1Schedule", true, "", 0.1, 1000},
    {"SurfacePropertyConvectionCoefficients", "Convection Coefficient 2", "convectionCoefficient2Schedule", true, "", 0.1, 1000},
    {"SpaceInfiltrationDesignFlowRate","Infiltration","schedule",true,"",0.0,1.0},
    {"SpaceInfiltrationEffectiveLeakageArea","Infiltration","schedule",true,"",0.0,1.0},
    {"SteamEquipment","Steam Equipment","schedule",true,"",0.0,1.0},
    {"SurfacePropertyConvectionCoefficients","Convection Coefficient 1","convectionCoefficient1Schedule",true,"",OptionalDouble(),OptionalDouble() },
    {"SurfacePropertyConvectionCoefficients","Convection Coefficient 2","convectionCoefficient2Schedule",true,"",OptionalDouble(),OptionalDouble() },
    {"SurfacePropertyConvectionCoefficientsMultipleSurface","Convection Coefficient 1","convectionCoefficient1Schedule",true,"",OptionalDouble(),OptionalDouble() },
    {"SurfacePropertyConvectionCoefficientsMultipleSurface","Convection Coefficient 2","convectionCoefficient2Schedule",true,"",OptionalDouble(),OptionalDouble() },
    {"ThermalStorageIceDetailed","Availability Schedule","availabilitySchedule",false,"Availability",0.0,1.0},
    {"ThermalStorageChilledWaterStratified","Setpoint Temperature","setpointTemperatureSchedule",true,"Temperature",OptionalDouble(),OptionalDouble()},
    {"ThermalStorageChilledWaterStratified","Ambient Temperature","ambientTemperatureSchedule",true,"Temperature",OptionalDouble(),OptionalDouble()},
    {"ThermalStorageChilledWaterStratified","Use Side Availability","useSideAvailabilitySchedule",false,"Availability",0.0,1.0},
    {"ThermalStorageChilledWaterStratified","Source Side Availability","sourceSideAvailabilitySchedule",false,"Availability",0.0,1.0},
    {"ThermostatSetpointDualSetpoint","Heating Setpoint Temperature","heatingSetpointTemperatureSchedule",true,"Temperature",OptionalDouble(),OptionalDouble()},
    {"ThermostatSetpointDualSetpoint","Cooling Setpoint Temperature","coolingSetpointTemperatureSchedule",true,"Temperature",OptionalDouble(),OptionalDouble()},
    {"ZoneControlThermostatStagedDualSetpoint","Heating Temperature Setpoint Schedule","heatingTemperatureSetpointSchedule",true,"Temperature",OptionalDouble(),OptionalDouble()},
    {"ZoneControlThermostatStagedDualSetpoint","Cooling Temperature Setpoint Base Schedule","coolingTemperatureSetpointBaseSchedule",true,"Temperature",OptionalDouble(),OptionalDouble()},
    {"ZoneControlContaminantController","Carbon Dioxide Control Availability","carbonDioxideControlAvailabilitySchedule",false,"Availability",0.0,1.0},
    {"ZoneControlContaminantController","Carbon Dioxide Setpoint","carbonDioxideSetpointSchedule",true,"",0.0,OptionalDouble()},
    {"ZoneControlContaminantController","Minimum Carbon Dioxide Concentration","minimumCarbonDioxideConcentrationSchedule",true,"",0.0,OptionalDouble()},
    {"ZoneControlContaminantController","Generic Contaminant Control Availability","genericContaminantControlAvailabilitySchedule",false,"Availability",0.0,1.0},
    {"ZoneControlContaminantController","Generic Contaminant Setpoint","genericContaminantSetpointSchedule",true,"",0.0,OptionalDouble()},
    {"WaterHeaterMixed","Setpoint Temperature","setpointTemperatureSchedule",true,"Temperature",OptionalDouble(),OptionalDouble()},
    {"WaterHeaterMixed","Ambient Temperature","ambientTemperatureSchedule",true,"Temperature",OptionalDouble(),OptionalDouble()},
    {"WaterHeaterMixed","Use Flow Rate Fraction","useFlowRateFractionSchedule",true,"",0.0,1.0},
    {"WaterHeaterMixed","Cold Water Supply Temperature","coldWaterSupplyTemperatureSchedule",true,"Temperature",OptionalDouble(),OptionalDouble()},
    {"WaterHeaterHeatPump","Availability Schedule","availabilitySchedule",false,"Availability",0.0,1.0},
    {"WaterHeaterHeatPump","Compressor Setpoint Temperature Schedule","compressorSetpointTemperatureSchedule",true,"Temperature",OptionalDouble(),OptionalDouble()},
    {"WaterHeaterHeatPump","Inlet Air Temperature Schedule","inletAirTemperatureSchedule",true,"Temperature",OptionalDouble(),OptionalDouble()},
    {"WaterHeaterHeatPump","Inlet Air Humidity Schedule","inletAirHumiditySchedule",true,"",0.0,OptionalDouble()},
    {"WaterHeaterHeatPump","Compressor Ambient Temperature Schedule","compressorAmbientTemperatureSchedule",true,"Temperature",OptionalDouble(),OptionalDouble()},
    {"WaterHeaterHeatPump","Inlet Air Mixer Schedule","inletAirMixerSchedule",true,"",0.0,1.0},
    {"WaterHeaterHeatPumpWrappedCondenser","Availability","availabilitySchedule",false,"Availability",0.0,1.0},
    {"WaterHeaterHeatPumpWrappedCondenser","Compressor Setpoint Temperature","compressorSetpointTemperatureSchedule",true,"Temperature",OptionalDouble(),OptionalDouble()},
    {"WaterHeaterHeatPumpWrappedCondenser","Inlet Air Temperature","inletAirTemperatureSchedule",true,"Temperature",OptionalDouble(),OptionalDouble()},
    {"WaterHeaterHeatPumpWrappedCondenser","Inlet Air Humidity","inletAirHumiditySchedule",true,"",0.0,OptionalDouble()},
    {"WaterHeaterHeatPumpWrappedCondenser","Compressor Ambient Temperature","compressorAmbientTemperatureSchedule",true,"Temperature",OptionalDouble(),OptionalDouble()},
    {"WaterHeaterHeatPumpWrappedCondenser","Inlet Air Mixer","inletAirMixerSchedule",true,"",0.0,1.0},
    {"WaterHeaterStratified","Heater 1 Setpoint Temperature","heater1SetpointTemperatureSchedule",true,"Temperature",OptionalDouble(),OptionalDouble()},
    {"WaterHeaterStratified","Heater 2 Setpoint Temperature","heater2SetpointTemperatureSchedule",true,"Temperature",OptionalDouble(),OptionalDouble()},
    {"WaterHeaterStratified","Ambient Temperature","ambientTemperatureSchedule",true,"Temperature",OptionalDouble(),OptionalDouble()},
    {"WaterHeaterStratified","Use Flow Rate Fraction","useFlowRateFractionSchedule",true,"",0.0,1.0},
    {"WaterHeaterStratified","Cold Water Supply Temperature","coldWaterSupplyTemperatureSchedule",true,"Temperature",OptionalDouble(),OptionalDouble()},
    {"WaterHeaterStratified","Indirect Alternate Setpoint Temperature","indirectAlternateSetpointTemperatureSchedule",true,"Temperature",OptionalDouble(),OptionalDouble()},
    {"WaterUseConnections","Hot Water Supply Temperature","hotWaterSupplyTemperatureSchedule",true,"Temperature",OptionalDouble(),OptionalDouble()},
    {"WaterUseConnections","Cold Water Supply Temperature","coldWaterSupplyTemperatureSchedule",true,"Temperature",OptionalDouble(),OptionalDouble()},
    {"WaterUseEquipment","Flow Rate Fraction","flowRateFractionSchedule",true,"",0.0,1.0},
    {"WaterUseEquipment","Target Temperature","targetTemperatureSchedule",true,"Temperature",OptionalDouble(),OptionalDouble()},
    {"WaterUseEquipment","Hot Water Supply Temperature","hotWaterSupplyTemperatureSchedule",true,"Temperature",OptionalDouble(),OptionalDouble()},
    {"WaterUseEquipment","Cold Water Supply Temperature","coldWaterSupplyTemperatureSchedule",true,"Temperature",OptionalDouble(),OptionalDouble()},
    {"WaterUseEquipment","Sensible Fraction","sensibleFractionSchedule",true,"",0.0,1.0},
    {"WaterUseEquipment","Latent Fraction","latentFractionSchedule",true,"",0.0,1.0},
    {"ZoneAirContaminantBalance","Outdoor Carbon Dioxide","outdoorCarbonDioxideSchedule",true,"",0.0,OptionalDouble()},
    {"ZoneControlHumidistat","Humidifying Relative Humidity Setpoint","humidifyingRelativeHumiditySetpointSchedule",true,"Percent",0.0,100.0},
    {"ZoneControlHumidistat","Dehumidifying Relative Humidity Setpoint","dehumidifyingRelativeHumiditySetpointSchedule",true,"Percent",0.0,100.0},
    {"ZoneHVACBaseboardConvectiveElectric","Availability","availabilitySchedule",false,"Availability",0.0,1.0},
    {"ZoneHVACBaseboardConvectiveWater","Availability","availabilitySchedule",false,"Availability",0.0,1.0},
    {"ZoneHVACIdealLoadsAirSystem","Availability","availabilitySchedule",false,"Availability",0.0,1.0},
    {"ZoneHVACIdealLoadsAirSystem","Heating Availability","heatingAvailabilitySchedule",false,"Availability",0.0,1.0},
    {"ZoneHVACIdealLoadsAirSystem","Cooling Availability","coolingAvailabilitySchedule",false,"Availability",0.0,1.0},
    {"ZoneHVACFourPipeFanCoil","Availability","availabilitySchedule",false,"Availability",0.0,1.0},
    {"ZoneHVACFourPipeFanCoil","Outdoor Air","outdoorAirSchedule",true,"",0.0,1.0},
    {"ZoneHVACFourPipeFanCoil","Supply Air Fan Operating Mode","supplyAirFanOperatingModeSchedule",false,"ControlMode",0.0,1.0},
    {"ZoneHVACLowTemperatureRadiantElectric","Availability","availabilitySchedule",false,"Availability",0.0,1.0},
    {"ZoneHVACLowTemperatureRadiantElectric","Heating Setpoint Temperature","heatingSetpointTemperature",true,"Temperature",OptionalDouble(),OptionalDouble()},
    {"ZoneHVACLowTempRadiantConstFlow","Availability","availabilitySchedule",false,"Availability",0.0,1.0},
    {"ZoneHVACLowTempRadiantConstFlow","Pump Flow Rate","pumpFlowRateSchedule",false,"Availability",0.0,1.0},
    {"ZoneHVACLowTempRadiantVarFlow","Availability","availabilitySchedule",false,"Availability",0.0,1.0},
    {"ZoneHVACPackagedTerminalAirConditioner","Availability","availabilitySchedule",false,"Availability",0.0,1.0},
    {"ZoneHVACPackagedTerminalAirConditioner","Supply Air Fan Operating Mode","supplyAirFanOperatingModeSchedule",false,"ControlMode",0.0,1.0},
    {"ZoneHVACPackagedTerminalHeatPump","Availability","availabilitySchedule",false,"Availability",0.0,1.0},
    {"ZoneHVACPackagedTerminalHeatPump","Supply Air Fan Operating Mode","supplyAirFanOperatingModeSchedule",false,"ControlMode",0.0,1.0},
    {"ZoneVentilationDesignFlowRate","Zone Ventilation Design Flow Rate","schedule",true,"",0.0,1.0},
    {"ZoneVentilationDesignFlowRate","Minimum Indoor Temperature","minimumIndoorTemperatureSchedule",true,"Temperature",OptionalDouble(),OptionalDouble()},
    {"ZoneVentilationDesignFlowRate","Maximum Indoor Temperature","maximumIndoorTemperatureSchedule",true,"Temperature",OptionalDouble(),OptionalDouble()},
    {"ZoneVentilationDesignFlowRate","Delta Temperature","deltaTemperatureSchedule",true,"DeltaTemperature",OptionalDouble(),OptionalDouble()},
    {"ZoneVentilationDesignFlowRate","Minimum Outdoor Temperature","minimumOutdoorTemperatureSchedule",true,"Temperature",OptionalDouble(),OptionalDouble()},
    {"ZoneVentilationDesignFlowRate","Maximum Outdoor Temperature","maximumOutdoorTemperatureSchedule",true,"Temperature",OptionalDouble(),OptionalDouble()},
    {"RefrigerationAirChiller","Availability","availabilitySchedule",false,"Availability",0.0,1.0},
    {"RefrigerationAirChiller","Heating Power","heatingPowerSchedule",true,"",0.0,1.0},
    {"RefrigerationAirChiller","Defrost","defrostSchedule",true,"",0.0,1.0},
    {"RefrigerationAirChiller","Defrost Drip-Down","defrostDripDownSchedule",true,"",0.0,1.0},
    {"ZoneHVACBaseboardRadiantConvectiveElectric","Availability","availabilitySchedule",false,"Availability",0.0,1.0},
    {"ZoneHVACBaseboardRadiantConvectiveWater","Availability","availabilitySchedule",false,"Availability",0.0,1.0},
    {"ZoneHVACDehumidifierDX","Availability","availabilitySchedule",false,"Availability",0.0,1.0},
    {"ZoneHVACEnergyRecoveryVentilator","Availability","availabilitySchedule",false,"Availability",0.0,1.0},
    {"ZoneHVACEnergyRecoveryVentilatorController","Time of Day Economizer Flow Control","timeofDayEconomizerFlowControlSchedule",true,"",0.0,1.0},
    {"ZoneHVACTerminalUnitVariableRefrigerantFlow","Terminal Unit Availability schedule","terminalUnitAvailabilityschedule",false,"Availability",0.0,1.0},
    {"ZoneHVACTerminalUnitVariableRefrigerantFlow","Supply Air Fan Operating Mode Schedule","supplyAirFanOperatingModeSchedule",false,"ControlMode",0.0,1.0},
    {"ZoneHVACHighTemperatureRadiant","Availability","availabilitySchedule",false,"Availability",0.0,1.0},
    {"ZoneHVACHighTemperatureRadiant","Heating Setpoint Temperature","heatingSetpointTemperatureSchedule",true,"Temperature",OptionalDouble(),OptionalDouble()},
    {"ZoneHVACWaterToAirHeatPump","Availability","availabilitySchedule",false,"Availability",0.0,1.0},
    {"ZoneHVACWaterToAirHeatPump","Supply Air Fan Operating Mode","supplyAirFanOperatingModeSchedule",false,"ControlMode",0.0,1.0},
    {"ZoneHVACUnitHeater","Availability","availabilitySchedule",false,"Availability",0.0,1.0},
    {"ZoneHVACUnitVentilator","Availability","availabilitySchedule",false,"Availability",0.0,1.0},
    {"ZoneHVACUnitVentilator","Minimum Outdoor Air","minimumOutdoorAirSchedule",true,"",0.0,1.0},
    {"ZoneHVACUnitVentilator","Maximum Outdoor Air Fraction or Temperature","maximumOutdoorAirFractionorTemperatureSchedule",true,"",OptionalDouble(),OptionalDouble()},
    {"ZoneHVACUnitVentilator","Supply Air Fan Operating Mode","supplyAirFanOperatingModeSchedule",false,"ControlMode",0.0,1.0},
    {"ZoneMixing", "Zone Mixing", "schedule", true, "Dimensionless", 0.0, 1.0},
    {"ZoneMixing", "Delta Temperature", "deltaTemperatureSchedule", true, "DeltaTemperature", OptionalDouble(), OptionalDouble() },
    {"ZoneMixing", "Minimum Zone Temperature", "minimumZoneTemperatureSchedule", true, "Temperature", OptionalDouble(), OptionalDouble() },
    {"ZoneMixing", "Maximum Zone Temperature", "maximumZoneTemperatureSchedule", true, "Temperature", OptionalDouble(), OptionalDouble() },
    {"ZoneMixing", "Minimum Source Zone Temperature", "minimumSourceZoneTemperatureSchedule", true, "Temperature", OptionalDouble(), OptionalDouble() },
    {"ZoneMixing", "Maximum Source Zone Temperature", "maximumSourceZoneTemperatureSchedule", true, "Temperature", OptionalDouble(), OptionalDouble() },
    {"ZoneMixing", "Minimum Outdoor Temperature", "minimumOutdoorTemperatureSchedule", true, "Temperature", OptionalDouble(), OptionalDouble() },
    {"ZoneMixing", "Maximum Outdoor Temperature", "maximumOutdoorTemperatureSchedule", true, "Temperature", OptionalDouble(), OptionalDouble() },
    {"","","",true,"",OptionalDouble(),OptionalDouble()}
  };

  int i(0);
  while (!scheduleTypes[i].className.empty()) {
    auto it = m_classNameToScheduleTypesMap.find(scheduleTypes[i].className);
    if (it == m_classNameToScheduleTypesMap.end()) {
      m_classNameToScheduleTypesMap.insert(ClassNameToScheduleTypesMap::value_type(scheduleTypes[i].className,ScheduleTypeVector(1u,scheduleTypes[i])));
    }
    else {
      it->second.push_back(scheduleTypes[i]);
    }
    ++i;
  }
}

std::string ScheduleTypeRegistrySingleton::getDefaultName(const ScheduleType& scheduleType) const
{
  std::string result = scheduleType.unitType;
  if (result.empty()) {
    if (scheduleType.isContinuous) {
      if (scheduleType.lowerLimitValue && (scheduleType.lowerLimitValue == 0.0) &&
          scheduleType.upperLimitValue && (scheduleType.upperLimitValue == 1.0))
      {
        result = "Fractional";
      }
      else {
        result = "Dimensionless";
      }
    }
    else {
      if (scheduleType.lowerLimitValue && (scheduleType.lowerLimitValue == 0.0) &&
          scheduleType.upperLimitValue && (scheduleType.upperLimitValue == 1.0))
      {
        result = "Binary";
      }
      else {
        result = "Integer";
      }
    }
  }
  else if (result == "Availability") {
    result = "OnOff";
  }
  return result;
}

bool isCompatible(const std::string& className,
                  const std::string& scheduleDisplayName,
                  const ScheduleTypeLimits& candidate)
{
  ScheduleType scheduleType = ScheduleTypeRegistry::instance().getScheduleType(className,scheduleDisplayName);
  return isCompatible(scheduleType,candidate);
}

bool isCompatible(const ScheduleType& scheduleType,
                  const ScheduleTypeLimits& candidate)
{
  // do not check continuous/discrete

  // check unit type
  std::string unitType = candidate.unitType();
  if (ScheduleTypeLimits::units(scheduleType.unitType,false) != ScheduleTypeLimits::units(unitType,false)) {
    return false;
  }

  // check lower limit
  if (scheduleType.lowerLimitValue) {
    if (!candidate.lowerLimitValue() ||
        (candidate.lowerLimitValue().get() < scheduleType.lowerLimitValue.get()))
    {
      return false;
    }
  }

  // check upper limit
  if (scheduleType.upperLimitValue) {
    if (!candidate.upperLimitValue() ||
        (candidate.upperLimitValue().get() > scheduleType.upperLimitValue.get()))
    {
      return false;
    }
  }

  // passed all checks
  return true;
}

bool checkOrAssignScheduleTypeLimits(const std::string& className,
                                     const std::string& scheduleDisplayName,
                                     Schedule& schedule)
{
  ScheduleType scheduleType = ScheduleTypeRegistry::instance().getScheduleType(
      className,
      scheduleDisplayName);
  bool result(true);
  if (OptionalScheduleTypeLimits scheduleTypeLimits = schedule.scheduleTypeLimits()) {
    if (!isCompatible(scheduleType,*scheduleTypeLimits)) {
      result = false;
    }
  }
  else {
    Model model = schedule.model();
    ScheduleTypeLimits candidate = ScheduleTypeRegistry::instance().getOrCreateScheduleTypeLimits(
        scheduleType,
        model);
    result = schedule.setScheduleTypeLimits(candidate);
    if (!result && (candidate.directUseCount(false) == 0)) {
      candidate.remove();
    }
  }
  return result;
}

std::vector<ScheduleTypeLimits> getCompatibleScheduleTypeLimits(const Model& model,
                                                                const std::string& className,
                                                                const std::string& scheduleDisplayName)
{
  ScheduleTypeLimitsVector result;
  ScheduleTypeLimitsVector candidates = model.getConcreteModelObjects<ScheduleTypeLimits>();
  ScheduleType scheduleType = ScheduleTypeRegistry::instance().getScheduleType(className,scheduleDisplayName);
  for (const ScheduleTypeLimits& candidate : candidates) {
    if (isCompatible(scheduleType,candidate)) {
      result.push_back(candidate);
    }
  }
  return result;
}

std::vector<Schedule> getCompatibleSchedules(const Model& model,
                                             const std::string& className,
                                             const std::string& scheduleDisplayName)
{
  ScheduleVector result;
  ScheduleVector candidates = model.getModelObjects<Schedule>();
  ScheduleTypeLimitsVector okTypes = getCompatibleScheduleTypeLimits(model,className,scheduleDisplayName);
  for (const Schedule& candidate : candidates) {
    if (OptionalScheduleTypeLimits candidateType = candidate.scheduleTypeLimits()) {
      if (std::find(okTypes.begin(),okTypes.end(),*candidateType) != okTypes.end()) {
        result.push_back(candidate);
      }
    }
    else {
      // by default, keep all non-typed schedules
      result.push_back(candidate);
    }
  }
  return result;
}

} // model
} // openstudio
