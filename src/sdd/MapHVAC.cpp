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

#include "ReverseTranslator.hpp"
#include "ForwardTranslator.hpp"
#include "Helpers.hpp"

#include "../model/AirLoopHVAC.hpp"
#include "../model/AirLoopHVAC_Impl.hpp"
#include "../model/AirLoopHVACUnitarySystem.hpp"
#include "../model/AirLoopHVACUnitarySystem_Impl.hpp"
#include "../model/ControllerMechanicalVentilation.hpp"
#include "../model/ControllerMechanicalVentilation_Impl.hpp"
#include "../model/AirLoopHVACZoneSplitter.hpp"
#include "../model/AirLoopHVACZoneSplitter_Impl.hpp"
#include "../model/AirLoopHVACOutdoorAirSystem.hpp"
#include "../model/AirLoopHVACOutdoorAirSystem_Impl.hpp"
#include "../model/ControllerOutdoorAir.hpp"
#include "../model/ConstructionWithInternalSource.hpp"
#include "../model/ConstructionWithInternalSource_Impl.hpp"
#include "../model/FanConstantVolume.hpp"
#include "../model/FanConstantVolume_Impl.hpp"
#include "../model/FanVariableVolume.hpp"
#include "../model/FanVariableVolume_Impl.hpp"
#include "../model/FanOnOff.hpp"
#include "../model/FanOnOff_Impl.hpp"
#include "../model/FanZoneExhaust.hpp"
#include "../model/FanZoneExhaust_Impl.hpp"
#include "../model/CoilHeatingWaterToAirHeatPumpEquationFit.hpp"
#include "../model/CoilHeatingWaterToAirHeatPumpEquationFit_Impl.hpp"
#include "../model/CoilCoolingWaterToAirHeatPumpEquationFit.hpp"
#include "../model/CoilCoolingWaterToAirHeatPumpEquationFit_Impl.hpp"
#include "../model/CoilCoolingDXSingleSpeed.hpp"
#include "../model/CoilCoolingDXSingleSpeed_Impl.hpp"
#include "../model/CoilCoolingDXTwoSpeed.hpp"
#include "../model/CoilCoolingDXMultiSpeed.hpp"
#include "../model/CoilCoolingLowTempRadiantVarFlow.hpp"
#include "../model/CoilCoolingLowTempRadiantVarFlow_Impl.hpp"
#include "../model/CoilHeatingGas.hpp"
#include "../model/CoilHeatingGas_Impl.hpp"
#include "../model/CoilHeatingElectric.hpp"
#include "../model/CoilHeatingElectric_Impl.hpp"
#include "../model/CoilHeatingDXSingleSpeed.hpp"
#include "../model/CoilHeatingLowTempRadiantVarFlow.hpp"
#include "../model/CoilHeatingLowTempRadiantVarFlow_Impl.hpp"
#include "../model/CoilWaterHeatingAirToWaterHeatPump.hpp"
#include "../model/CoilWaterHeatingAirToWaterHeatPump_Impl.hpp"
#include "../model/ControllerWaterCoil.hpp"
#include "../model/ControllerWaterCoil_Impl.hpp"
#include "../model/CurveBiquadratic.hpp"
#include "../model/CurveBiquadratic_Impl.hpp"
#include "../model/CurveLinear.hpp"
#include "../model/CurveLinear_Impl.hpp"
#include "../model/CurveCubic.hpp"
#include "../model/CurveCubic_Impl.hpp"
#include "../model/CurveQuadratic.hpp"
#include "../model/CurveQuadratic_Impl.hpp"
#include "../model/CurveQuadraticLinear.hpp"
#include "../model/CurveQuadraticLinear_Impl.hpp"
#include "../model/Curve.hpp"
#include "../model/Curve_Impl.hpp"
#include "../model/TableMultiVariableLookup.hpp"
#include "../model/TableMultiVariableLookup_Impl.hpp"
#include "../model/Duct.hpp"
#include "../model/Duct_Impl.hpp"
#include "../model/ScheduleConstant.hpp"
#include "../model/ScheduleConstant_Impl.hpp"
#include "../model/ScheduleRuleset.hpp"
#include "../model/ScheduleRuleset_Impl.hpp"
#include "../model/ScheduleRule.hpp"
#include "../model/ScheduleRule_Impl.hpp"
#include "../model/ScheduleDay.hpp"
#include "../model/ScheduleDay_Impl.hpp"
#include "../model/ScheduleYear.hpp"
#include "../model/ScheduleYear_Impl.hpp"
#include "../model/ScheduleWeek.hpp"
#include "../model/ScheduleWeek_Impl.hpp"
#include "../model/ScheduleTypeLimits.hpp"
#include "../model/ScheduleTypeLimits_Impl.hpp"
#include "../model/SizingZone.hpp"
#include "../model/SizingZone_Impl.hpp"
#include "../model/Node.hpp"
#include "../model/Node_Impl.hpp"
#include "../model/Space.hpp"
#include "../model/Space_Impl.hpp"
#include "../model/AirConditionerVariableRefrigerantFlow.hpp"
#include "../model/AirConditionerVariableRefrigerantFlow_Impl.hpp"
#include "../model/AvailabilityManagerLowTemperatureTurnOff.hpp"
#include "../model/AvailabilityManagerLowTemperatureTurnOff_Impl.hpp"
#include "../model/AvailabilityManagerOptimumStart.hpp"
#include "../model/AvailabilityManagerOptimumStart_Impl.hpp"
#include "../model/AvailabilityManagerNightCycle.hpp"
#include "../model/AvailabilityManagerNightCycle_Impl.hpp"
#include "../model/CoilCoolingDXVariableRefrigerantFlow.hpp"
#include "../model/CoilCoolingDXVariableRefrigerantFlow_Impl.hpp"
#include "../model/CoilHeatingDXVariableRefrigerantFlow.hpp"
#include "../model/CoilHeatingDXVariableRefrigerantFlow_Impl.hpp"
#include "../model/DesignSpecificationOutdoorAir.hpp"
#include "../model/DesignSpecificationOutdoorAir_Impl.hpp"
#include "../model/CoolingTowerSingleSpeed.hpp"
#include "../model/CoolingTowerSingleSpeed_Impl.hpp"
#include "../model/CoolingTowerTwoSpeed.hpp"
#include "../model/CoolingTowerTwoSpeed_Impl.hpp"
#include "../model/CoolingTowerVariableSpeed.hpp"
#include "../model/CoolingTowerVariableSpeed_Impl.hpp"
#include "../model/FluidCoolerSingleSpeed.hpp"
#include "../model/FluidCoolerSingleSpeed_Impl.hpp"
#include "../model/FluidCoolerTwoSpeed.hpp"
#include "../model/FluidCoolerTwoSpeed_Impl.hpp"
#include "../model/EvaporativeFluidCoolerSingleSpeed.hpp"
#include "../model/EvaporativeFluidCoolerSingleSpeed_Impl.hpp"
#include "../model/EvaporativeFluidCoolerTwoSpeed.hpp"
#include "../model/EvaporativeFluidCoolerTwoSpeed_Impl.hpp"
#include "../model/SetpointManagerFollowOutdoorAirTemperature.hpp"
#include "../model/SetpointManagerFollowOutdoorAirTemperature_Impl.hpp"
#include "../model/SetpointManagerMixedAir.hpp"
#include "../model/SetpointManagerMixedAir_Impl.hpp"
#include "../model/SetpointManagerSingleZoneReheat.hpp"
#include "../model/SetpointManagerSingleZoneReheat_Impl.hpp"
#include "../model/SetpointManagerScheduled.hpp"
#include "../model/SetpointManagerScheduled_Impl.hpp"
#include "../model/SetpointManagerScheduledDualSetpoint.hpp"
#include "../model/SetpointManagerScheduledDualSetpoint_Impl.hpp"
#include "../model/SetpointManagerWarmest.hpp"
#include "../model/SetpointManagerWarmest_Impl.hpp"
#include "../model/SetpointManagerWarmestTemperatureFlow.hpp"
#include "../model/SetpointManagerWarmestTemperatureFlow_Impl.hpp"
#include "../model/SetpointManagerOutdoorAirReset.hpp"
#include "../model/SetpointManagerOutdoorAirReset_Impl.hpp"
#include "../model/SetpointManagerScheduledDualSetpoint.hpp"
#include "../model/SetpointManagerScheduledDualSetpoint_Impl.hpp"
#include "../model/ThermalZone.hpp"
#include "../model/ThermalZone_Impl.hpp"
#include "../model/ThermalStorageIceDetailed.hpp"
#include "../model/ThermalStorageIceDetailed_Impl.hpp"
#include "../model/ThermalStorageChilledWaterStratified.hpp"
#include "../model/ThermalStorageChilledWaterStratified_Impl.hpp"
#include "../model/AirTerminalSingleDuctConstantVolumeCooledBeam.hpp"
#include "../model/AirTerminalSingleDuctConstantVolumeNoReheat.hpp"
#include "../model/AirTerminalSingleDuctVAVReheat.hpp"
#include "../model/AirTerminalSingleDuctVAVNoReheat.hpp"
#include "../model/AirTerminalSingleDuctParallelPIUReheat.hpp"
#include "../model/AirTerminalSingleDuctParallelPIUReheat_Impl.hpp"
#include "../model/AirTerminalSingleDuctSeriesPIUReheat.hpp"
#include "../model/AirTerminalSingleDuctSeriesPIUReheat_Impl.hpp"
#include "../model/AirTerminalSingleDuctInletSideMixer.hpp"
#include "../model/AirTerminalSingleDuctInletSideMixer_Impl.hpp"
#include "../model/ThermostatSetpointDualSetpoint.hpp"
#include "../model/ThermostatSetpointDualSetpoint_Impl.hpp"
#include "../model/Schedule.hpp"
#include "../model/Schedule_Impl.hpp"
#include "../model/PumpConstantSpeed.hpp"
#include "../model/PumpConstantSpeed_Impl.hpp"
#include "../model/PumpVariableSpeed.hpp"
#include "../model/PumpVariableSpeed_Impl.hpp"
#include "../model/PlantEquipmentOperationHeatingLoad.hpp"
#include "../model/PlantEquipmentOperationHeatingLoad_Impl.hpp"
#include "../model/PlantEquipmentOperationCoolingLoad.hpp"
#include "../model/PlantEquipmentOperationCoolingLoad_Impl.hpp"
#include "../model/BoilerHotWater.hpp"
#include "../model/BoilerHotWater_Impl.hpp"
#include "../model/ChillerElectricEIR.hpp"
#include "../model/ChillerElectricEIR_Impl.hpp"
#include "../model/ChillerAbsorptionIndirect.hpp"
#include "../model/ChillerAbsorptionIndirect_Impl.hpp"
#include "../model/CoilCoolingCooledBeam.hpp"
#include "../model/CoilCoolingCooledBeam_Impl.hpp"
#include "../model/CoilCoolingWater.hpp"
#include "../model/CoilCoolingWater_Impl.hpp"
#include "../model/CoilHeatingWater.hpp"
#include "../model/CoilHeatingWater_Impl.hpp"
#include "../model/WaterToAirComponent.hpp"
#include "../model/WaterToAirComponent_Impl.hpp"
#include "../model/HeatExchangerAirToAirSensibleAndLatent.hpp"
#include "../model/HeatExchangerAirToAirSensibleAndLatent_Impl.hpp"
#include "../model/LoadProfilePlant.hpp"
#include "../model/LoadProfilePlant_Impl.hpp"
#include "../model/PlantLoop.hpp"
#include "../model/PlantLoop_Impl.hpp"
#include "../model/ZoneHVACComponent.hpp"
#include "../model/ZoneHVACComponent_Impl.hpp"
#include "../model/ZoneHVACTerminalUnitVariableRefrigerantFlow.hpp"
#include "../model/ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl.hpp"
#include "../model/ZoneHVACPackagedTerminalAirConditioner.hpp"
#include "../model/ZoneHVACPackagedTerminalAirConditioner_Impl.hpp"
#include "../model/ZoneHVACPackagedTerminalHeatPump.hpp"
#include "../model/ZoneHVACPackagedTerminalHeatPump_Impl.hpp"
#include "../model/ZoneHVACWaterToAirHeatPump.hpp"
#include "../model/ZoneHVACWaterToAirHeatPump_Impl.hpp"
#include "../model/ZoneHVACFourPipeFanCoil.hpp"
#include "../model/ZoneHVACFourPipeFanCoil_Impl.hpp"
#include "../model/ZoneHVACBaseboardConvectiveElectric.hpp"
#include "../model/ZoneHVACBaseboardConvectiveElectric_Impl.hpp"
#include "../model/ZoneHVACBaseboardConvectiveWater.hpp"
#include "../model/ZoneHVACBaseboardConvectiveWater_Impl.hpp"
#include "../model/ZoneHVACLowTemperatureRadiantElectric.hpp"
#include "../model/ZoneHVACLowTemperatureRadiantElectric_Impl.hpp"
#include "../model/ZoneHVACLowTempRadiantVarFlow.hpp"
#include "../model/ZoneHVACLowTempRadiantVarFlow_Impl.hpp"
#include "../model/ZoneVentilationDesignFlowRate.hpp"
#include "../model/ZoneVentilationDesignFlowRate_Impl.hpp"
#include "../model/CoilHeatingWaterBaseboard.hpp"
#include "../model/CoilHeatingWaterBaseboard_Impl.hpp"
#include "../model/WaterHeaterMixed.hpp"
#include "../model/WaterHeaterMixed_Impl.hpp"
#include "../model/WaterHeaterHeatPump.hpp"
#include "../model/WaterHeaterHeatPump_Impl.hpp"
#include "../model/Model.hpp"
#include "../model/PortList.hpp"
#include "../model/SizingPlant.hpp"
#include "../model/SizingPlant_Impl.hpp"
#include "../model/SizingSystem.hpp"
#include "../model/SizingSystem_Impl.hpp"
#include "../model/AirTerminalSingleDuctVAVReheat_Impl.hpp"
#include "../model/PipeAdiabatic.hpp"
#include "../model/PipeAdiabatic_Impl.hpp"
#include "../model/Splitter.hpp"
#include "../model/Splitter_Impl.hpp"
#include "../model/Mixer.hpp"
#include "../model/Mixer_Impl.hpp"
#include "../model/OpaqueMaterial.hpp"
#include "../model/OpaqueMaterial_Impl.hpp"
#include "../model/EvaporativeCoolerDirectResearchSpecial.hpp"
#include "../model/EvaporativeCoolerDirectResearchSpecial_Impl.hpp"
#include "../model/EvaporativeCoolerIndirectResearchSpecial.hpp"
#include "../model/EvaporativeCoolerIndirectResearchSpecial_Impl.hpp"
#include "../model/HeatExchangerFluidToFluid.hpp"
#include "../model/HeatExchangerFluidToFluid_Impl.hpp"
#include "../model/DaylightingControl.hpp"
#include "../utilities/units/QuantityConverter.hpp"
#include "../utilities/units/Quantity.hpp"
#include "../utilities/units/IPUnit.hpp"
#include "../utilities/units/SIUnit.hpp"
#include "../utilities/units/BTUUnit.hpp"
#include "../utilities/units/CFMUnit.hpp"
#include "../utilities/units/FahrenheitUnit.hpp"
#include "../utilities/units/MPHUnit.hpp"
#include "../utilities/units/WhUnit.hpp"
#include "../utilities/core/Assert.hpp"
#include "../utilities/core/StringHelpers.hpp"
#include "../utilities/time/Time.hpp"
#include "../utilities/time/Date.hpp"
#include "../utilities/units/UnitFactory.hpp"
#include "../utilities/math/FloatCompare.hpp"
#include "../utilities/geometry/Geometry.hpp"
#include "../utilities/geometry/Point3d.hpp"
#include "../utilities/geometry/BoundingBox.hpp"

#include <cmath>
#include <functional>

#include <pugixml.hpp>

namespace openstudio {
namespace sdd {

constexpr double footToMeter =  0.3048;
constexpr double cpWater = 4180.0;
constexpr double densityWater = 1000.0;

// Adjust scheduleDay by delaying the start and stop by the respective offset
// This is used by the OA Controller ventilation schedule
void adjustScheduleDay(model::ScheduleDay & scheduleDay, int startOffset)
{
  std::vector<double> values = scheduleDay.values();
  std::vector<Time> times = scheduleDay.times();

  std::vector<double>::iterator valueIt;
  std::vector<Time>::iterator timeIt;

  timeIt = times.begin() + 1;
  for( valueIt = values.begin() + 1; valueIt < values.end(); ++valueIt )
  {
    if( equal<double>(*valueIt,1.0,0.01) && equal<double>(*(valueIt - 1),0.0,0.01) )
    {
      Time newStartTime;

      newStartTime = *(timeIt - 1) + Time(0,startOffset);

      scheduleDay.removeValue(*(timeIt - 1));

      scheduleDay.addValue(newStartTime,0.0);
    }

    ++timeIt;
  }
}

void adjustSchedule(model::ScheduleYear & scheduleYear, int startOffset)
{
  std::vector<model::ScheduleWeek> scheduleWeeks = scheduleYear.scheduleWeeks();

  for( const auto & scheduleWeek : scheduleWeeks)
  {
    std::vector<boost::optional<model::ScheduleDay> > scheduleDays;

    scheduleDays.push_back(scheduleWeek.sundaySchedule());

    scheduleDays.push_back(scheduleWeek.mondaySchedule());

    scheduleDays.push_back(scheduleWeek.tuesdaySchedule());

    scheduleDays.push_back(scheduleWeek.wednesdaySchedule());

    scheduleDays.push_back(scheduleWeek.thursdaySchedule());

    scheduleDays.push_back(scheduleWeek.fridaySchedule());

    scheduleDays.push_back(scheduleWeek.saturdaySchedule());

    scheduleDays.push_back(scheduleWeek.holidaySchedule());

    scheduleDays.push_back(scheduleWeek.summerDesignDaySchedule());

    scheduleDays.push_back(scheduleWeek.winterDesignDaySchedule());

    scheduleDays.push_back(scheduleWeek.customDay1Schedule());

    scheduleDays.push_back(scheduleWeek.customDay2Schedule());

    for( auto & scheduleDayIt : scheduleDays)
    {
      if( scheduleDayIt )
      {
        adjustScheduleDay( scheduleDayIt.get(), startOffset );
      }
    }
  }
}

// Make a deep copy into same model as scheduleYear
model::ScheduleYear deepScheduleYearClone(const model::ScheduleYear & scheduleYear, const std::string & name)
{
  model::Model model = scheduleYear.model();

  model::ScheduleYear scheduleYearClone = scheduleYear.clone(model).cast<model::ScheduleYear>();
  scheduleYearClone.setName(name);
  scheduleYearClone.clearScheduleWeeks();

  std::vector<model::ScheduleWeek> scheduleWeeks = scheduleYear.scheduleWeeks();
  std::vector<Date> dates = scheduleYear.dates();

  auto dateIt = dates.begin();

  int i = 1;

  for( const auto & scheduleWeek : scheduleWeeks)
  {
    model::ScheduleWeek scheduleWeekClone = scheduleWeek.clone(model).cast<model::ScheduleWeek>();
    scheduleWeekClone.setName(name + " Week " + openstudio::string_conversions::number(i));
    scheduleYearClone.addScheduleWeek(*dateIt,scheduleWeekClone);

    boost::optional<model::ScheduleDay> scheduleDay;

    if( (scheduleDay = scheduleWeek.sundaySchedule()) )
    {
      model::ScheduleDay s = scheduleDay->clone(model).cast<model::ScheduleDay>();
      s.setName(name + " Week " + openstudio::string_conversions::number(i) + " Sunday");
      scheduleWeekClone.setSundaySchedule(s);
    }

    if( (scheduleDay = scheduleWeek.mondaySchedule()) )
    {
      model::ScheduleDay s = scheduleDay->clone(model).cast<model::ScheduleDay>();
      s.setName(name + " Week " + openstudio::string_conversions::number(i) + " Monday");
      scheduleWeekClone.setMondaySchedule(s);
    }

    if( (scheduleDay = scheduleWeek.tuesdaySchedule()) )
    {
      model::ScheduleDay s = scheduleDay->clone(model).cast<model::ScheduleDay>();
      s.setName(name + " Week " + openstudio::string_conversions::number(i) + " Tuesday");
      scheduleWeekClone.setTuesdaySchedule(s);
    }

    if( (scheduleDay = scheduleWeek.wednesdaySchedule()) )
    {
      model::ScheduleDay s = scheduleDay->clone(model).cast<model::ScheduleDay>();
      s.setName(name + " Week " + openstudio::string_conversions::number(i) + " Wednesday");
      scheduleWeekClone.setWednesdaySchedule(s);
    }

    if( (scheduleDay = scheduleWeek.thursdaySchedule()) )
    {
      model::ScheduleDay s = scheduleDay->clone(model).cast<model::ScheduleDay>();
      s.setName(name + " Week " + openstudio::string_conversions::number(i) + " Thursday");
      scheduleWeekClone.setThursdaySchedule(s);
    }

    if( (scheduleDay = scheduleWeek.fridaySchedule()) )
    {
      model::ScheduleDay s = scheduleDay->clone(model).cast<model::ScheduleDay>();
      s.setName(name + " Week " + openstudio::string_conversions::number(i) + " Friday");
      scheduleWeekClone.setFridaySchedule(s);
    }

    if( (scheduleDay = scheduleWeek.saturdaySchedule()) )
    {
      model::ScheduleDay s = scheduleDay->clone(model).cast<model::ScheduleDay>();
      s.setName(name + " Week " + openstudio::string_conversions::number(i) + " Saturday");
      scheduleWeekClone.setSaturdaySchedule(s);
    }

    if( (scheduleDay = scheduleWeek.holidaySchedule()) )
    {
      model::ScheduleDay s = scheduleDay->clone(model).cast<model::ScheduleDay>();
      s.setName(name + " Week " + openstudio::string_conversions::number(i) + " Holiday");
      scheduleWeekClone.setHolidaySchedule(s);
    }

    if( (scheduleDay = scheduleWeek.summerDesignDaySchedule()) )
    {
      model::ScheduleDay s = scheduleDay->clone(model).cast<model::ScheduleDay>();
      s.setName(name + " Week " + openstudio::string_conversions::number(i) + " Summer");
      scheduleWeekClone.setSummerDesignDaySchedule(s);
    }

    if( (scheduleDay = scheduleWeek.winterDesignDaySchedule()) )
    {
      model::ScheduleDay s = scheduleDay->clone(model).cast<model::ScheduleDay>();
      s.setName(name + " Week " + openstudio::string_conversions::number(i) + " Winter");
      scheduleWeekClone.setWinterDesignDaySchedule(s);
    }

    if( (scheduleDay = scheduleWeek.customDay1Schedule()) )
    {
      model::ScheduleDay s = scheduleDay->clone(model).cast<model::ScheduleDay>();
      s.setName(name + " Week " + openstudio::string_conversions::number(i) + " Custom 1");
      scheduleWeekClone.setCustomDay1Schedule(s);
    }

    if( (scheduleDay = scheduleWeek.customDay2Schedule()) )
    {
      model::ScheduleDay s = scheduleDay->clone(model).cast<model::ScheduleDay>();
      s.setName(name + " Week " + openstudio::string_conversions::number(i) + " Custom 2");
      scheduleWeekClone.setCustomDay2Schedule(s);
    }

    ++dateIt;
    i++;
  }

  return scheduleYearClone;
}

model::Schedule ReverseTranslator::defaultDeckTempSchedule(openstudio::model::Model& model)
{
  if( ! m_defaultDeckTempSchedule )
  {
    model::ScheduleRuleset scheduleRuleset = model::ScheduleRuleset(model);

    scheduleRuleset.setName("Deck Temp");

    model::ScheduleDay scheduleDay = scheduleRuleset.defaultDaySchedule();

    scheduleDay.addValue(Time(1.0),12.8);

    m_defaultDeckTempSchedule = scheduleRuleset;
  }

  return m_defaultDeckTempSchedule.get();
}

model::Schedule ReverseTranslator::hotWaterPlantSetpointSchedule(openstudio::model::Model& model)
{
  if( ! m_hotWaterPlantSetpointSchedule )
  {
    model::ScheduleRuleset scheduleRuleset = model::ScheduleRuleset(model);

    scheduleRuleset.setName("Hot Water Plant Setpoint");

    model::ScheduleDay scheduleDay = scheduleRuleset.defaultDaySchedule();

    scheduleDay.addValue(Time(1.0),67.0);

    m_hotWaterPlantSetpointSchedule = scheduleRuleset;
  }

  return m_hotWaterPlantSetpointSchedule.get();
}

model::Schedule ReverseTranslator::chilledWaterPlantSetpointSchedule(openstudio::model::Model& model)
{
  if( ! m_chilledWaterPlantSetpointSchedule )
  {
    model::ScheduleRuleset scheduleRuleset = model::ScheduleRuleset(model);

    scheduleRuleset.setName("Chilled Water Plant Setpoint");

    model::ScheduleDay scheduleDay = scheduleRuleset.defaultDaySchedule();

    scheduleDay.addValue(Time(1.0),6.7);

    m_chilledWaterPlantSetpointSchedule = scheduleRuleset;
  }

  return m_chilledWaterPlantSetpointSchedule.get();
}

model::Schedule ReverseTranslator::serviceHotWaterSetpointSchedule(openstudio::model::Model& model)
{
  if( ! m_serviceHotWaterSetpointSchedule )
  {
    model::ScheduleRuleset scheduleRuleset = model::ScheduleRuleset(model);

    scheduleRuleset.setName("SHW Plant Setpoint");

    model::ScheduleDay scheduleDay = scheduleRuleset.defaultDaySchedule();

    scheduleDay.addValue(Time(1.0),50.0);

    m_serviceHotWaterSetpointSchedule = scheduleRuleset;
  }

  return m_serviceHotWaterSetpointSchedule.get();
}

boost::optional<openstudio::model::ModelObject> ReverseTranslator::translateVRFSys(
                                                  const pugi::xml_node& vrfSysElement,
                                                  openstudio::model::Model& model )
{
  boost::optional<openstudio::model::ModelObject> result;

  model::AirConditionerVariableRefrigerantFlow vrf(model);
  result = vrf;

  {
    std::string value = vrfSysElement.child("Name").text().as_string();
    vrf.setName(value);
  }

  {
    pugi::xml_node element = vrfSysElement.child("AvailSchRef");
    std::string name = escapeName(element.text().as_string());
    if( auto schedule = model.getModelObjectByName<model::Schedule>(name) ) {
      vrf.setAvailabilitySchedule(schedule.get());
    }
  }

  {
    auto element = vrfSysElement.child("HtRcvry");
    if( istringEqual(element.text().as_string(), "Yes") ) {
      vrf.setHeatPumpWasteHeatRecovery(true);
    }
  }

  //{
  //  auto element = vrfSysElement.child("CndsrType");
  //  vrf.setCondenserType(element.text().as_string());
  //}

  {
    auto element = vrfSysElement.child("FuelType");
    vrf.setFuelType(element.text().as_string());
  }

  {
    auto element = vrfSysElement.child("PLRMin");
    boost::optional<double> _value = lexicalCastToDouble(element);
    if( _value ) {
      vrf.setMinimumHeatPumpPartLoadRatio(_value.get());
    }
  }

  {
    auto element = vrfSysElement.child("CtrlPriority");
    vrf.setMasterThermostatPriorityControlType(element.text().as_string());
  }

  {
    auto element = vrfSysElement.child("CtrlZnRef");
    std::string text = element.text().as_string();
    if( ! text.empty() ) {
      m_vrfSystemControlZones.insert(std::pair<std::string,model::AirConditionerVariableRefrigerantFlow>(text,vrf));
    }
  }

  {
    auto element = vrfSysElement.child("CtrlSchRef");
    if( auto schedule = model.getModelObjectByName<model::Schedule>(element.text().as_string()) ) {
      vrf.setThermostatPrioritySchedule(schedule.get());
    }
  }

  if( ! autosize() ) {
    auto element = vrfSysElement.child("ClgCapGrossRtdSim");
    boost::optional<double> _value = lexicalCastToDouble(element);
    if( _value ) {
      double value = unitToUnit(_value.get(),"Btu/h","W").get();
      vrf.setGrossRatedTotalCoolingCapacity(value);
    }
  }

  {
    auto element = vrfSysElement.child("ClgEIR");
    boost::optional<double> _value = lexicalCastToDouble(element);
    if( _value ) {
      vrf.setGrossRatedCoolingCOP(1.0 / _value.get() );
    }
  }

  if( ! autosize() ) {
    auto element = vrfSysElement.child("HtgCapGrossRtdSim");
    boost::optional<double> _value = lexicalCastToDouble(element);
    if( _value ) {
      double value = unitToUnit(_value.get(),"Btu/h","W").get();
      vrf.setGrossRatedHeatingCapacity(value);
    }
  }

  {
    auto element = vrfSysElement.child("HtgEIR");
    boost::optional<double> _value = lexicalCastToDouble(element);
    if( _value ) {
      vrf.setRatedHeatingCOP(1.0 / _value.get() );
    }
  }

  {
    auto element = vrfSysElement.child("HtgToClgSizingRat");
    boost::optional<double> _value = lexicalCastToDouble(element);
    if( _value ) {
      vrf.setRatedHeatingCapacitySizingRatio(_value.get());
    }
  }

  {
    auto element = vrfSysElement.child("ClgCprsrLockoutTempLow");
    boost::optional<double> _value = lexicalCastToDouble(element);
    if( _value ) {
      double value = unitToUnit(_value.get(),"F","C").get();
      vrf.setMinimumOutdoorTemperatureinCoolingMode(value);
    }
  }

  {
    auto element = vrfSysElement.child("ClgCprsrLockoutTempHi");
    boost::optional<double> _value = lexicalCastToDouble(element);
    if( _value ) {
      double value = unitToUnit(_value.get(),"F","C").get();      vrf.setMaximumOutdoorTemperatureinCoolingMode(value);
    }
  }

  {
    auto element = vrfSysElement.child("HtgCprsrLockoutTempLow");
    boost::optional<double> _value = lexicalCastToDouble(element);
    if( _value ) {
      double value = unitToUnit(_value.get(),"F","C").get();
      vrf.setMinimumOutdoorTemperatureinHeatingMode(value);
    }
  }

  {
    auto element = vrfSysElement.child("HtgCprsrLockoutTempHi");
    boost::optional<double> _value = lexicalCastToDouble(element);
    if( _value ) {
      double value = unitToUnit(_value.get(),"F","C").get();
      vrf.setMaximumOutdoorTemperatureinHeatingMode(value);
    }
  }

  {
    auto element = vrfSysElement.child("HtRcvryCprsrLockoutTempLow");
    boost::optional<double> _value = lexicalCastToDouble(element);
    if( _value ) {
      double value = unitToUnit(_value.get(),"F","C").get();
      vrf.setMinimumOutdoorTemperatureinHeatRecoveryMode(value);
    }
  }

  {
    auto element = vrfSysElement.child("HtRcvryCprsrLockoutTempHi");
    boost::optional<double> _value = lexicalCastToDouble(element);
    if( _value ) {
      double value = unitToUnit(_value.get(),"F","C").get();
      vrf.setMaximumOutdoorTemperatureinHeatRecoveryMode(value);
    }
  }

  {
    auto element = vrfSysElement.child("HtRcvryInitialClgCapFrac");
    boost::optional<double> _value = lexicalCastToDouble(element);
    if( _value ) {
      vrf.setInitialHeatRecoveryCoolingCapacityFraction(_value.get());
    }
  }

  {
    auto element = vrfSysElement.child("HtRcvryInitialClgEngyFrac");
    boost::optional<double> _value = lexicalCastToDouble(element);
    if( _value ) {
      vrf.setInitialHeatRecoveryCoolingEnergyFraction(_value.get());
    }
  }

  {
    auto element = vrfSysElement.child("HtRcvryInitialClgTimeCnst");
    boost::optional<double> _value = lexicalCastToDouble(element);
    if( _value ) {
      vrf.setHeatRecoveryCoolingCapacityTimeConstant(_value.get());
      vrf.setHeatRecoveryCoolingEnergyTimeConstant(_value.get());
    }
  }

  {
    auto element = vrfSysElement.child("HtRcvryInitialHtgCapFrac");
    boost::optional<double> _value = lexicalCastToDouble(element);
    if( _value ) {
      vrf.setInitialHeatRecoveryHeatingCapacityFraction(_value.get());
    }
  }

  {
    auto element = vrfSysElement.child("HtRcvryInitialHtgTimeCnst");
    boost::optional<double> _value = lexicalCastToDouble(element);
    if( _value ) {
      vrf.setHeatRecoveryHeatingCapacityTimeConstant(_value.get());
      vrf.setHeatRecoveryHeatingEnergyTimeConstant(_value.get());
    }
  }

  {
    auto element = vrfSysElement.child("HtRcvryInitialHtgEngyFrac");
    boost::optional<double> _value = lexicalCastToDouble(element);
    if( _value ) {
      vrf.setInitialHeatRecoveryHeatingEnergyFraction(_value.get());
    }
  }

  {
    auto value = vrfSysElement.child("DefHtSrc").text().as_string();
    if( istringEqual("HotGas",value) ) {
      vrf.setDefrostStrategy("ReverseCycle");
    } else if( istringEqual("Electric",value) ) {
      vrf.setDefrostStrategy("Resistive");
      auto element = vrfSysElement.child("DefHtrCapSim");
      auto _value = lexicalCastToDouble(element);
      if( _value ) {
        vrf.setResistiveDefrostHeaterCapacity(_value.get());
      } else {
        vrf.autosizeResistiveDefrostHeaterCapacity();
      }
    }
  }

  {
    auto value = vrfSysElement.child("DefCtrl").text().as_string();
    if( istringEqual("OnDemand",value) ) {
      vrf.setDefrostControl("OnDemand");
    } else if( istringEqual("TimedCycle",value) ) {
      vrf.setDefrostControl("Timed");
    }
  }

  {
    auto element = vrfSysElement.child("DefTimeFrac");
    boost::optional<double> _value = lexicalCastToDouble(element);
    if( _value ) {
      vrf.setDefrostTimePeriodFraction(_value.get());
    }
  }

  {
    auto element = vrfSysElement.child("DefCtrlTemp");
    boost::optional<double> _value = lexicalCastToDouble(element);
    if( _value ) {
      double value = unitToUnit(_value.get(),"F","C").get();
      vrf.setMaximumOutdoorDrybulbTemperatureforDefrostOperation(value);
    }
  }

  if( ! autosize() ) {
    auto element = vrfSysElement.child("CprsrCrankcaseHtrCap");
    boost::optional<double> _value = lexicalCastToDouble(element);
    if( _value ) {
      double value = unitToUnit(_value.get(),"Btu/h","W").get();
      vrf.setCrankcaseHeaterPowerperCompressor(value);
    }
  }

  {
    auto element = vrfSysElement.child("CprsrQty");
    boost::optional<int> _value = lexicalCastToInt(element);
    if (_value ) {
      vrf.setNumberofCompressors(_value.get());
    }
  }

  {
    auto element = vrfSysElement.child("CrankcaseCtrlTemp");
    boost::optional<double> _value = lexicalCastToDouble(element);
    if( _value ) {
      double value = unitToUnit(_value.get(),"F","C").get();
      vrf.setMaximumOutdoorDrybulbTemperatureforCrankcaseHeater(value);
    }
  }

  {
    auto element = vrfSysElement.child("EquivalentPipeLen");
    boost::optional<double> _value = lexicalCastToDouble(element);
    if( _value ) {
      double value = unitToUnit(_value.get(),"ft","m").get();
      vrf.setEquivalentPipingLengthusedforPipingCorrectionFactorinCoolingMode(value);
      vrf.setEquivalentPipingLengthusedforPipingCorrectionFactorinHeatingMode(value);
    }
  }

  {
    auto element = vrfSysElement.child("MaxDeltaHgt");
    boost::optional<double> _value = lexicalCastToDouble(element);
    if( _value ) {
      double value = unitToUnit(_value.get(),"ft","m").get();
      vrf.setVerticalHeightusedforPipingCorrectionFactor(value);
    }
  }

  {
    auto value = vrfSysElement.child("HtgCrvOutdoorTempType").text().as_string();
    if( istringEqual(value,"WetBulb") ) {
      vrf.setHeatingPerformanceCurveOutdoorTemperatureType("WetBulbTemperature");
    } else if( istringEqual(value,"DryBulb") ) {
      vrf.setHeatingPerformanceCurveOutdoorTemperatureType("DryBulbTemperature");
    }
  }

  auto setCurve = [&](const std::string & elementName,
      const std::function<bool(model::AirConditionerVariableRefrigerantFlow &,const model::Curve &)> & osSetter,
      const std::function<boost::optional<model::Curve>(model::AirConditionerVariableRefrigerantFlow &)> & osGetter) {

    std::string value = vrfSysElement.child(elementName.c_str()).text().as_string();
    auto newcurve = model.getModelObjectByName<model::Curve>(value);
    if( newcurve ) {
      if( auto oldcurve = osGetter(vrf) ) {
        if( oldcurve.get() != newcurve.get() ) {
          oldcurve->remove();
        }
      }
      osSetter(vrf,newcurve.get());
    }
  };

  setCurve("HtgCap_fTempLowCrvRef",
    std::mem_fn(&model::AirConditionerVariableRefrigerantFlow::setHeatingCapacityRatioModifierFunctionofLowTemperatureCurve),
    std::mem_fn(&model::AirConditionerVariableRefrigerantFlow::heatingCapacityRatioModifierFunctionofLowTemperatureCurve));

  setCurve("HtgCapBndry_fTempCrvRef",
    std::mem_fn(&model::AirConditionerVariableRefrigerantFlow::setHeatingCapacityRatioBoundaryCurve),
    std::mem_fn(&model::AirConditionerVariableRefrigerantFlow::heatingCapacityRatioBoundaryCurve));

  setCurve("HtgCap_fTempHiCrvRef",
    std::mem_fn(&model::AirConditionerVariableRefrigerantFlow::setHeatingCapacityRatioModifierFunctionofHighTemperatureCurve),
    std::mem_fn(&model::AirConditionerVariableRefrigerantFlow::heatingCapacityRatioModifierFunctionofHighTemperatureCurve));

  setCurve("HtgEIR_fTempLowCrvRef",
    std::mem_fn(&model::AirConditionerVariableRefrigerantFlow::setHeatingEnergyInputRatioModifierFunctionofLowTemperatureCurve),
    std::mem_fn(&model::AirConditionerVariableRefrigerantFlow::heatingEnergyInputRatioModifierFunctionofLowTemperatureCurve));

  setCurve("HtgEIRBndry_fTempCrvRef",
    std::mem_fn(&model::AirConditionerVariableRefrigerantFlow::setHeatingEnergyInputRatioBoundaryCurve),
    std::mem_fn(&model::AirConditionerVariableRefrigerantFlow::heatingEnergyInputRatioBoundaryCurve));

  setCurve("HtgEIR_fTempHiCrvRef",
    std::mem_fn(&model::AirConditionerVariableRefrigerantFlow::setHeatingEnergyInputRatioModifierFunctionofHighTemperatureCurve),
    std::mem_fn(&model::AirConditionerVariableRefrigerantFlow::heatingEnergyInputRatioModifierFunctionofHighTemperatureCurve));

  setCurve("HtgEIR_fPLRLowCrvRef",
    std::mem_fn(&model::AirConditionerVariableRefrigerantFlow::setHeatingEnergyInputRatioModifierFunctionofLowPartLoadRatioCurve),
    std::mem_fn(&model::AirConditionerVariableRefrigerantFlow::heatingEnergyInputRatioModifierFunctionofLowPartLoadRatioCurve));

  setCurve("HtgEIR_fPLRHiCrvRef",
    std::mem_fn(&model::AirConditionerVariableRefrigerantFlow::setHeatingEnergyInputRatioModifierFunctionofHighPartLoadRatioCurve),
    std::mem_fn(&model::AirConditionerVariableRefrigerantFlow::heatingEnergyInputRatioModifierFunctionofHighPartLoadRatioCurve));

  setCurve("HtgCap_fCombRatCrvRef",
    std::mem_fn(&model::AirConditionerVariableRefrigerantFlow::setHeatingCombinationRatioCorrectionFactorCurve),
    std::mem_fn(&model::AirConditionerVariableRefrigerantFlow::heatingCombinationRatioCorrectionFactorCurve));

  setCurve("HtRcvryHtgCap_fTempCrvRef",
    std::mem_fn(&model::AirConditionerVariableRefrigerantFlow::setHeatRecoveryHeatingCapacityModifierCurve),
    std::mem_fn(&model::AirConditionerVariableRefrigerantFlow::heatRecoveryHeatingCapacityModifierCurve));

  setCurve("HtRcvryHtgEIR_fTempCrvRef",
    std::mem_fn(&model::AirConditionerVariableRefrigerantFlow::setHeatRecoveryHeatingEnergyModifierCurve),
    std::mem_fn(&model::AirConditionerVariableRefrigerantFlow::heatRecoveryHeatingEnergyModifierCurve));

  setCurve("ClgCap_fTempLowCrvRef",
    std::mem_fn(&model::AirConditionerVariableRefrigerantFlow::setCoolingCapacityRatioModifierFunctionofLowTemperatureCurve),
    std::mem_fn(&model::AirConditionerVariableRefrigerantFlow::coolingCapacityRatioModifierFunctionofLowTemperatureCurve));

  setCurve("ClgCapBndry_fTempCrvRef",
    std::mem_fn(&model::AirConditionerVariableRefrigerantFlow::setCoolingCapacityRatioBoundaryCurve),
    std::mem_fn(&model::AirConditionerVariableRefrigerantFlow::coolingCapacityRatioBoundaryCurve));

  setCurve("ClgCap_fTempHiCrvRef",
    std::mem_fn(&model::AirConditionerVariableRefrigerantFlow::setCoolingCapacityRatioModifierFunctionofHighTemperatureCurve),
    std::mem_fn(&model::AirConditionerVariableRefrigerantFlow::coolingCapacityRatioModifierFunctionofHighTemperatureCurve));

  setCurve("ClgEIR_fTempLowCrvRef",
    std::mem_fn(&model::AirConditionerVariableRefrigerantFlow::setCoolingEnergyInputRatioModifierFunctionofLowTemperatureCurve),
    std::mem_fn(&model::AirConditionerVariableRefrigerantFlow::coolingEnergyInputRatioModifierFunctionofLowTemperatureCurve));

  setCurve("ClgEIRBndry_fTempCrvRef",
    std::mem_fn(&model::AirConditionerVariableRefrigerantFlow::setCoolingEnergyInputRatioBoundaryCurve),
    std::mem_fn(&model::AirConditionerVariableRefrigerantFlow::coolingEnergyInputRatioBoundaryCurve));

  setCurve("ClgEIR_fTempHiCrvRef",
    std::mem_fn(&model::AirConditionerVariableRefrigerantFlow::setCoolingEnergyInputRatioModifierFunctionofHighTemperatureCurve),
    std::mem_fn(&model::AirConditionerVariableRefrigerantFlow::coolingEnergyInputRatioModifierFunctionofHighTemperatureCurve));

  setCurve("ClgEIR_fPLRLowCrvRef",
    std::mem_fn(&model::AirConditionerVariableRefrigerantFlow::setCoolingEnergyInputRatioModifierFunctionofLowPartLoadRatioCurve),
    std::mem_fn(&model::AirConditionerVariableRefrigerantFlow::coolingEnergyInputRatioModifierFunctionofLowPartLoadRatioCurve));

  setCurve("ClgEIR_fPLRHiCrvRef",
    std::mem_fn(&model::AirConditionerVariableRefrigerantFlow::setCoolingEnergyInputRatioModifierFunctionofHighPartLoadRatioCurve),
    std::mem_fn(&model::AirConditionerVariableRefrigerantFlow::coolingEnergyInputRatioModifierFunctionofHighPartLoadRatioCurve));

  setCurve("ClgEIR_fPLRHiCrvRef",
    std::mem_fn(&model::AirConditionerVariableRefrigerantFlow::setCoolingEnergyInputRatioModifierFunctionofHighPartLoadRatioCurve),
    std::mem_fn(&model::AirConditionerVariableRefrigerantFlow::coolingEnergyInputRatioModifierFunctionofHighPartLoadRatioCurve));

  setCurve("ClgCap_fCombRatCrvRef",
    std::mem_fn(&model::AirConditionerVariableRefrigerantFlow::setCoolingCombinationRatioCorrectionFactorCurve),
    std::mem_fn(&model::AirConditionerVariableRefrigerantFlow::coolingCombinationRatioCorrectionFactorCurve));

  setCurve("ClgEIR_fCyclingRatCrvRef",
    std::mem_fn(&model::AirConditionerVariableRefrigerantFlow::setCoolingPartLoadFractionCorrelationCurve),
    std::mem_fn(&model::AirConditionerVariableRefrigerantFlow::coolingPartLoadFractionCorrelationCurve));

  setCurve("HtRcvryClgCap_fTempCrvRef",
    std::mem_fn(&model::AirConditionerVariableRefrigerantFlow::setHeatRecoveryCoolingCapacityModifierCurve),
    std::mem_fn(&model::AirConditionerVariableRefrigerantFlow::heatRecoveryCoolingCapacityModifierCurve));

  setCurve("HtRcvryClgEIR_fTempCrvRef",
    std::mem_fn(&model::AirConditionerVariableRefrigerantFlow::setHeatRecoveryCoolingEnergyModifierCurve),
    std::mem_fn(&model::AirConditionerVariableRefrigerantFlow::heatRecoveryCoolingEnergyModifierCurve));

  setCurve("ClgPipeLoss_fPipeLenCrvRef",
    std::mem_fn(&model::AirConditionerVariableRefrigerantFlow::setPipingCorrectionFactorforLengthinCoolingModeCurve),
    std::mem_fn(&model::AirConditionerVariableRefrigerantFlow::pipingCorrectionFactorforLengthinCoolingModeCurve));

  setCurve("HtgPipeLoss_fPipeLenCrvRef",
    std::mem_fn(&model::AirConditionerVariableRefrigerantFlow::setPipingCorrectionFactorforLengthinHeatingModeCurve),
    std::mem_fn(&model::AirConditionerVariableRefrigerantFlow::pipingCorrectionFactorforLengthinHeatingModeCurve));

  setCurve("DefEIR_fTempCrvRef",
    std::mem_fn(&model::AirConditionerVariableRefrigerantFlow::setDefrostEnergyInputRatioModifierFunctionofTemperatureCurve),
    std::mem_fn(&model::AirConditionerVariableRefrigerantFlow::defrostEnergyInputRatioModifierFunctionofTemperatureCurve));

  setCurve("HtgEIR_fCyclingRatCrvRef",
    std::mem_fn(&model::AirConditionerVariableRefrigerantFlow::setHeatingPartLoadFractionCorrelationCurve),
    std::mem_fn(&model::AirConditionerVariableRefrigerantFlow::heatingPartLoadFractionCorrelationCurve));

  {
    auto element = vrfSysElement.child("ClgPipeLoss_fPipeHgtCoeff");
    boost::optional<double> _value = lexicalCastToDouble(element);
    if( _value ) {
      double value = unitToUnit(_value.get(),"1/ft","1/m").get();
      vrf.setPipingCorrectionFactorforHeightinCoolingModeCoefficient(value);
    }
  }

  {
    auto element = vrfSysElement.child("HtgPipeLoss_fPipeHgtCoeff");
    boost::optional<double> _value = lexicalCastToDouble(element);
    if( _value ) {
      double value = unitToUnit(_value.get(),"1/ft","1/m").get();
      vrf.setPipingCorrectionFactorforHeightinHeatingModeCoefficient(value);
    }
  }

  return result;
}

boost::optional<openstudio::model::ModelObject> ReverseTranslator::translateAirSystem(
                                                  const pugi::xml_node& airSystemElement,
                                                  openstudio::model::Model& model )
{
  boost::optional<openstudio::model::ModelObject> result;

  if( ! istringEqual(airSystemElement.name(),"AirSys") )
  {
    return result;
  }

  // Type
  pugi::xml_node airSystemTypeElement = airSystemElement.child("TypeSim");
  if( openstudio::istringEqual(airSystemTypeElement.text().as_string(), "Exhaust") )
  {
    return boost::none;
  }

  // Name
  pugi::xml_node nameElement = airSystemElement.child("Name");

  // Air System
  model::AirLoopHVAC airLoopHVAC(model);
  airLoopHVAC.setName(nameElement.text().as_string());
  result = airLoopHVAC;
  model::Node supplyOutletNode = airLoopHVAC.supplyOutletNode();
  model::Node supplyInletNode = airLoopHVAC.supplyInletNode();

  pugi::xml_node airHndlrAvailSchElement = airSystemElement.child("AvailSchRef");

  // Availability Schedule
  boost::optional<model::Schedule> availabilitySchedule;
  if( airHndlrAvailSchElement )
  {
      availabilitySchedule = model.getModelObjectByName<model::Schedule>(airHndlrAvailSchElement.text().as_string());
  }

  if( availabilitySchedule )
  {
    airLoopHVAC.setAvailabilitySchedule(availabilitySchedule.get());
  }

  auto controlZone = airSystemElement.child("CtrlZnRef").text().as_string();

  // Optimum Start
  auto optStartElement = airSystemElement.child("OptStart");
  if( optStartElement ) {
    model::AvailabilityManagerOptimumStart optimumStart(model);
    airLoopHVAC.addAvailabilityManager(optimumStart);

    auto optStartCtrlElement = airSystemElement.child("OptStartCtrl");
    if( istringEqual(optStartCtrlElement.text().as_string(),"ControlZone") ) {
      optimumStart.setControlType("ControlZone");
      auto pair = std::pair<std::string,model::AvailabilityManagerOptimumStart>(controlZone,optimumStart);
      m_optimumStartControlZones.insert(pair);
    } else {
      optimumStart.setControlType("MaximumZoneList");
    }

    auto optStartMaxValElement = airSystemElement.child("OptStartMaxVal");
    boost::optional<double> _optStartMaxValElement = lexicalCastToDouble(optStartMaxValElement);
    if (_optStartMaxValElement) {
      optimumStart.setMaximumValueforOptimumStartTime(_optStartMaxValElement.get());
    }

    optimumStart.setControlAlgorithm("AdaptiveTemperatureGradient");

    auto optStartClgGradientElement = airSystemElement.child("OptStartClgGradient");
    boost::optional<double> _optStartClgGradient = lexicalCastToDouble(optStartClgGradientElement);
    if (_optStartClgGradient) {
      // Convert dF to dC, these are delta temperatures
      optimumStart.setInitialTemperatureGradientduringCooling(_optStartClgGradient.get() * 5.0 / 9.0);
    }

    auto optStartHtgGradientElement = airSystemElement.child("OptStartHtgGradient");
    boost::optional<double> _optStartHtgGradient = lexicalCastToDouble(optStartHtgGradientElement);
    if (_optStartHtgGradient) {
      // Convert dF to dC, these are delta temperatures
      optimumStart.setInitialTemperatureGradientduringHeating(_optStartHtgGradient.get() * 5.0 / 9.0);
    }

    auto optStartNumDaysElement = airSystemElement.child("OptStartNumDays");
    boost::optional<int> _optStartNumDays = lexicalCastToInt(optStartNumDaysElement);
    if (_optStartNumDays) {
      optimumStart.setNumberofPreviousDays(_optStartNumDays.get());
    }
  }

  // Night Cycle
  pugi::xml_node nightCycleFanCtrlElement = airSystemElement.child("NightCycleFanCtrl");

  if( istringEqual(nightCycleFanCtrlElement.text().as_string(),"CycleOnCallAnyZone") )
  {
    airLoopHVAC.setNightCycleControlType("CycleOnAny");
  }
  else if( istringEqual(nightCycleFanCtrlElement.text().as_string(),"CycleOnCallPrimaryZone") )
  {
    model::AvailabilityManagerNightCycle nightCycle(model);
    nightCycle.setControlType("CycleOnControlZone");

    airLoopHVAC.addAvailabilityManager(nightCycle);

    auto pair = std::pair<std::string,model::AvailabilityManagerNightCycle>(controlZone,nightCycle);
    m_nightCycleControlZones.insert(pair);
  }
  else if( istringEqual(nightCycleFanCtrlElement.text().as_string(),"CycleZoneFansOnly") )
  {
    airLoopHVAC.setNightCycleControlType("CycleOnAnyZoneFansOnly");
  }
  else if( istringEqual(nightCycleFanCtrlElement.text().as_string(),"CycleOnAnyCooling") )
  {
    airLoopHVAC.setNightCycleControlType("CycleOnAnyCoolingZone");
  }
  else if( istringEqual(nightCycleFanCtrlElement.text().as_string(),"CycleOnAnyHeating") )
  {
    airLoopHVAC.setNightCycleControlType("CycleOnAnyHeatingZone");
  }

  auto ncms = subsetCastVector<model::AvailabilityManagerNightCycle>(airLoopHVAC.availabilityManagers());
  if (!ncms.empty()) {
    auto ncm = ncms.front();
    ncm.setCyclingRunTimeControlType("ThermostatWithMinimumRunTime");
    ncm.setCyclingRunTime(300.0);

    auto cycleTime = lexicalCastToDouble(airSystemElement.child("NightCycleRunTime"));
    if (cycleTime) {
      ncm.setCyclingRunTime(cycleTime.get());
    }
  }

  if( ! airLoopHVAC.availabilityManagers().empty() ) {
    auto avm = airLoopHVAC.availabilityManagers().front();
    if( auto nightCycle = avm.optionalCast<model::AvailabilityManagerNightCycle>() ) {
      auto nightCycleTstatToleranceElement = airSystemElement.child("NightCycleTstatTolerance");
      boost::optional<double> _nightCycleTstatTolerance = lexicalCastToDouble(nightCycleTstatToleranceElement);
      if( _nightCycleTstatTolerance ) {
        nightCycle->setThermostatTolerance(_nightCycleTstatTolerance.get() * 5.0 / 9.0);
      }

      auto nightCycleRunTimeElement = airSystemElement.child("NightCycleRunTime");
      boost::optional<double> _nightCycleRunTime = lexicalCastToDouble(nightCycleRunTimeElement);
      if( _nightCycleRunTime ) {
        nightCycle->setCyclingRunTime(_nightCycleRunTime.get());
      }
    }
  }

  // RetAirFlowRat
  auto retAirFlowRatElement = airSystemElement.child("RetAirFlowRat");
  auto _retAirFlowRat = lexicalCastToDouble(retAirFlowRatElement);
  if( _retAirFlowRat ) {
    airLoopHVAC.setDesignReturnAirFlowFractionofSupplyAirFlow(_retAirFlowRat.get());
  }

  // Adjust Sizing:System Object

  model::SizingSystem sizingSystem = airLoopHVAC.sizingSystem();

  // clgDsgnSupAirTemp
  double clgDsgnSupAirTemp = 12.8;
  pugi::xml_node clgSupAirTempElement = airSystemElement.child("ClgDsgnSupAirTemp");
  boost::optional<double> _clgSupAirTempElement = lexicalCastToDouble(clgSupAirTempElement);
  if( _clgSupAirTempElement )
  {
    clgDsgnSupAirTemp = unitToUnit(_clgSupAirTempElement.get(),"F","C").get();
  }
  sizingSystem.setCentralCoolingDesignSupplyAirTemperature(clgDsgnSupAirTemp);

  // HtgDsgnSupAirTemp
  double htgDsgnSupAirTemp = 40.0;
  pugi::xml_node htgSupAirTempElement = airSystemElement.child("HtgDsgnSupAirTemp");
  boost::optional<double> _htgSupAirTemp = lexicalCastToDouble(htgSupAirTempElement);
  if( _htgSupAirTemp )
  {
    htgDsgnSupAirTemp = unitToUnit(_htgSupAirTemp.get(),"F","C").get();
  }
  sizingSystem.setCentralHeatingDesignSupplyAirTemperature(htgDsgnSupAirTemp);

  // DsgnAirFlowMinRat
  pugi::xml_node dsgnAirFlowMinRatElement = airSystemElement.child("DsgnAirFlowMinRat");
  boost::optional<double> _dsgnAirFlowMinRat = lexicalCastToDouble(dsgnAirFlowMinRatElement);
  if( _dsgnAirFlowMinRat )
  {
    sizingSystem.setCentralHeatingMaximumSystemAirFlowRatio(_dsgnAirFlowMinRat.get());
  }
  else
  {
    sizingSystem.setCentralHeatingMaximumSystemAirFlowRatio(0.3);
  }

  // DsgnPrehtTemp
  pugi::xml_node dsgnPrehtTempElement = airSystemElement.child("DsgnPrehtTemp");
  boost::optional<double> _dsgnPrehtTemp = lexicalCastToDouble(dsgnPrehtTempElement);
  if( _dsgnPrehtTemp )
  {
    sizingSystem.setPreheatDesignTemperature(unitToUnit(_dsgnPrehtTemp.get(),"F","C").get());
  }
  else
  {
    sizingSystem.setPreheatDesignTemperature(7.0);
  }

  // DsgnPrehtHumidityRat
  pugi::xml_node dsgnPrehtHumidityRatElement = airSystemElement.child("DsgnPrehtHumidityRat");
  boost::optional<double> _dsgnPrehtHumidityRat = lexicalCastToDouble(dsgnPrehtHumidityRatElement);
  if( _dsgnPrehtHumidityRat )
  {
    sizingSystem.setPreheatDesignHumidityRatio(_dsgnPrehtHumidityRat.get());
  }
  else
  {
    sizingSystem.setPreheatDesignHumidityRatio(0.008);
  }

  // DsgnPreclTemp
  pugi::xml_node dsgnPreclTempElement = airSystemElement.child("DsgnPreclTemp");
  boost::optional<double> _dsgnPreclTemp = lexicalCastToDouble(dsgnPreclTempElement);
  if( _dsgnPreclTemp )
  {
    sizingSystem.setPrecoolDesignTemperature(unitToUnit(_dsgnPreclTemp.get(),"F","C").get());
  }
  else
  {
    sizingSystem.setPrecoolDesignTemperature(12.8);
  }

  // DsgnPreclHumidityRat
  pugi::xml_node dsgnPreclHumidityRatElement = airSystemElement.child("DsgnPreclHumidityRat");
  boost::optional<double> _dsgnPreclHumidityRat = lexicalCastToDouble(dsgnPreclHumidityRatElement);
  if( _dsgnPreclHumidityRat )
  {
    sizingSystem.setPrecoolDesignHumidityRatio(_dsgnPreclHumidityRat.get());
  }
  else
  {
    sizingSystem.setPrecoolDesignHumidityRatio(0.008);
  }

  // SizingOption
  pugi::xml_node sizingOptionElement = airSystemElement.child("SizingOption");
  if( openstudio::istringEqual(sizingOptionElement.text().as_string(), "NonCoincident") )
  {
    sizingSystem.setSizingOption("NonCoincident");
  }
  else
  {
    sizingSystem.setSizingOption("Coincident");
  }

  // ClgFullOutsdAir
  pugi::xml_node clgFullOutsdAirElement = airSystemElement.child("ClgFullOutsdAir");
  if( openstudio::istringEqual(clgFullOutsdAirElement.text().as_string(), "Yes") )
  {
    sizingSystem.setAllOutdoorAirinCooling(true);
  }
  else
  {
    sizingSystem.setAllOutdoorAirinCooling(false);
  }

  // HtgFullOutsdAir
  pugi::xml_node htgFullOutsdAirElement = airSystemElement.child("HtgFullOutsdAir");
  if( openstudio::istringEqual(htgFullOutsdAirElement.text().as_string(), "Yes") )
  {
    sizingSystem.setAllOutdoorAirinHeating(true);
  }
  else
  {
    sizingSystem.setAllOutdoorAirinHeating(false);
  }

  // ClgDsgnHumidityRat
  pugi::xml_node clgDsgnHumidityRatElement = airSystemElement.child("ClgDsgnHumidityRat");
  boost::optional<double> _clgDsgnHumidityRat = lexicalCastToDouble(clgDsgnHumidityRatElement);
  if( _clgDsgnHumidityRat )
  {
    sizingSystem.setCentralCoolingDesignSupplyAirHumidityRatio(_clgDsgnHumidityRat.get());
  }
  else
  {
    sizingSystem.setCentralCoolingDesignSupplyAirHumidityRatio(0.008);
  }

  // HtgDsgnHumidityRat

  pugi::xml_node htgDsgnHumidityRatElement = airSystemElement.child("HtgDsgnHumidityRat");
  boost::optional<double> _htgDsgnHumidityRat = lexicalCastToDouble(htgDsgnHumidityRatElement);
  if( _htgDsgnHumidityRat )
  {
    sizingSystem.setCentralHeatingDesignSupplyAirHumidityRatio(_htgDsgnHumidityRat.get());
  }
  else
  {
    sizingSystem.setCentralHeatingDesignSupplyAirHumidityRatio(0.008);
  }

  // FanPos

  bool isFanDrawthrough = false;
  pugi::xml_node fanPosElement = airSystemElement.child("FanPos");
  if( openstudio::istringEqual(fanPosElement.text().as_string(), "DrawThrough") )
  {
    isFanDrawthrough = true;
  }

  // Save Evap Cooler to draw from return air
  boost::optional<model::EvaporativeCoolerIndirectResearchSpecial> indirectEvapUsingReturn;

  // Air Segments
  std::vector<pugi::xml_node> airSegmentElements = makeVectorOfChildren(airSystemElement, "AirSeg");

  // Save the fan to add last
  boost::optional<model::Node> dropNode;

  std::vector<model::ModelObject> coolingComponents;
  std::vector<model::ModelObject> heatingComponents;

  for (std::vector<pugi::xml_node>::size_type i = 0; i < airSegmentElements.size(); i++)
  {
    pugi::xml_node airSegmentElement = airSegmentElements[i];

    pugi::xml_node airSegmentTypeElement = airSegmentElement.child("Type");

    // Supply Segments
    if(istringEqual(airSegmentTypeElement.text().as_string(),"Supply"))
    {
      // Get all children
      std::vector<pugi::xml_node> airSegmentChildElements = makeVectorOfChildren(airSegmentElement);

      // Locate and add the fan first
      // This is so we don't mess up control nodes

      for (std::vector<pugi::xml_node>::size_type j = 0; j < airSegmentChildElements.size(); j++)
      {
        pugi::xml_node airSegmentChildElement = airSegmentChildElements[j];

        // Fan
        if( (istringEqual(airSegmentChildElement.name(),"Fan")) )
        {
          if( boost::optional<model::ModelObject> mo =
                translateFan(airSegmentChildElement, model) )
          {
            if( boost::optional<model::StraightComponent> hvacComponent =
                  mo->optionalCast<model::StraightComponent>() )
            {
              // Determine fan position

              // TODO: unused
              // pugi::xml_node posElement = airSegmentChildElement.child("Pos");

              hvacComponent->addToNode(supplyOutletNode);

              if( isFanDrawthrough )
              {
                dropNode = hvacComponent->inletModelObject()->cast<model::Node>();
              }
              else
              {
                dropNode = supplyOutletNode;
              }

              if( availabilitySchedule )
              {
                if( boost::optional<model::FanConstantVolume> fan = hvacComponent->optionalCast<model::FanConstantVolume>() )
                {
                  if( ! fan->isMaximumFlowRateAutosized() )
                  {
                    airLoopHVAC.setDesignSupplyAirFlowRate(fan->maximumFlowRate().get());
                  }
                }
                else if( boost::optional<model::FanVariableVolume> fan = hvacComponent->optionalCast<model::FanVariableVolume>() )
                {
                  if( ! fan->isMaximumFlowRateAutosized() )
                  {
                    airLoopHVAC.setDesignSupplyAirFlowRate(fan->maximumFlowRate().get());
                  }
                }
              }
            }
            break;
          }
        }
      }

      if( ! dropNode )
      {
        dropNode = supplyOutletNode;
      }

      for(int j = (airSegmentChildElements.size() - 1); j > -1;  j--)
      {
        pugi::xml_node airSegmentChildElement = airSegmentChildElements[j];

        boost::optional<model::ModelObject> lastComponent = boost::none;

        // CoilCooling
        if( istringEqual(airSegmentChildElement.name(),"CoilClg") )
        {
          if( boost::optional<model::ModelObject> mo =
                translateCoilCooling(airSegmentChildElement, model) )
          {
            lastComponent = mo;

            model::HVACComponent hvacComponent = mo->cast<model::HVACComponent>();
            hvacComponent.addToNode(dropNode.get());

            coolingComponents.push_back(mo.get());

            if( boost::optional<model::CoilCoolingWater> coilCoolingWater = hvacComponent.optionalCast<model::CoilCoolingWater>() )
            {
              boost::optional<model::ControllerWaterCoil> controller = coilCoolingWater->controllerWaterCoil();
              OS_ASSERT(controller);

              controller->setMinimumActuatedFlow(0.0);

              if( ! autosize() )
              {
                controller->setControllerConvergenceTolerance(0.1);
                boost::optional<double> maxFlow = coilCoolingWater->designWaterFlowRate();
                OS_ASSERT(maxFlow);
                controller->setMaximumActuatedFlow(maxFlow.get() * 1.25);
              }
            }
          }
        }
        // CoilHeating
        else if( istringEqual(airSegmentChildElement.name(),"CoilHtg") )
        {
          if( auto mo = translateCoilHeating(airSegmentChildElement, model) ) {
            lastComponent = mo;

            model::HVACComponent hvacComponent = mo->cast<model::HVACComponent>();
            if (!hvacComponent.addToNode(dropNode.get())) {
              LOG(Error, "Cannot add HVACComponent named '" << hvacComponent.nameString() << "' to node '" << dropNode->nameString()
                     <<  "' on AirLoopHVAC '" << airLoopHVAC.nameString());
            }

            heatingComponents.push_back(mo.get());

            if( boost::optional<model::CoilHeatingWater> coilHeatingWater = hvacComponent.optionalCast<model::CoilHeatingWater>() )
            {
              boost::optional<model::ControllerWaterCoil> controller = coilHeatingWater->controllerWaterCoil();
              OS_ASSERT(controller);

              controller->setControllerConvergenceTolerance(0.1);
              controller->setMinimumActuatedFlow(0.0);

              if( ! autosize() )
              {
                boost::optional<double> capacity = coilHeatingWater->ratedCapacity();

                boost::optional<double> inletTemp = coilHeatingWater->ratedInletWaterTemperature();

                boost::optional<double> outletTemp = coilHeatingWater->ratedOutletWaterTemperature();

                OS_ASSERT(capacity);

                OS_ASSERT(inletTemp);

                OS_ASSERT(outletTemp);

                double density = 1000.0;

                double cp = 4180.0;

                double maxFlow = capacity.get() / ( density * cp * std::abs(inletTemp.get() - outletTemp.get()));

                controller->setMaximumActuatedFlow(maxFlow * 1.25);
              }
            }
          }
        }
        // EvapClr
        else if( istringEqual(airSegmentChildElement.name(),"EvapClr") )
        {
          if( boost::optional<model::ModelObject> mo =
                translateEvapClr(airSegmentChildElement, model) )
          {
            lastComponent = mo;

            model::HVACComponent hvacComponent = mo->cast<model::HVACComponent>();
            hvacComponent.addToNode(dropNode.get());

            coolingComponents.push_back(mo.get());

            if( boost::optional<model::EvaporativeCoolerIndirectResearchSpecial> evap = hvacComponent.optionalCast<model::EvaporativeCoolerIndirectResearchSpecial>() )
            {
              pugi::xml_node secAirSrcElement = airSegmentChildElement.first_child();
              if(openstudio::istringEqual(secAirSrcElement.text().as_string(), "Return"))
              {
                indirectEvapUsingReturn = evap;
              }
            }
          }
        }

        if( lastComponent )
        {
          if( boost::optional<model::StraightComponent> straightComponent = lastComponent->optionalCast<model::StraightComponent>() )
          {
            dropNode = straightComponent->inletModelObject()->cast<model::Node>();
          }
          else if( boost::optional<model::WaterToAirComponent> waterToAirComponent = lastComponent->optionalCast<model::WaterToAirComponent>() )
          {
            dropNode = waterToAirComponent->airInletModelObject()->cast<model::Node>();
          }
        }
      }
    }
  }

  // OACtrl
  pugi::xml_node airSystemOACtrlElement = airSystemElement.child("OACtrl");
  boost::optional<model::AirLoopHVACOutdoorAirSystem> oaSystem;

  if( airSystemOACtrlElement )
  {
    oaSystem = airLoopHVAC.airLoopHVACOutdoorAirSystem();
    if( ! oaSystem )
    {
      model::ControllerOutdoorAir oaController(model);

      model::AirLoopHVACOutdoorAirSystem newOASystem(model,oaController);

      newOASystem.addToNode(supplyInletNode);

      // Name
      pugi::xml_node nameElement = airSystemOACtrlElement.child("Name");
      std::string name = nameElement.text().as_string();
      if( ! name.empty() )
      {
        oaController.setName(name);
        newOASystem.setName(name + " OA System");
        oaController.controllerMechanicalVentilation().setName(name + " Mech Vent Controller");
      }

      // MinOAFracSchRef
      pugi::xml_node minOAFracSchRefElement = airSystemOACtrlElement.child("MinOAFracSchRef");
      if( boost::optional<model::Schedule> schedule =
          model.getModelObjectByName<model::Schedule>(minOAFracSchRefElement.text().as_string()) )
      {
        oaController.setMinimumFractionofOutdoorAirSchedule(schedule.get());
      }

      // MaxOAFracSchRef
      pugi::xml_node maxOAFracSchRefElement = airSystemOACtrlElement.child("MaxOAFracSchRef");
      if( boost::optional<model::Schedule> schedule =
          model.getModelObjectByName<model::Schedule>(maxOAFracSchRefElement.text().as_string()) ) {
        oaController.setMaximumFractionofOutdoorAirSchedule(schedule.get());
      } else {
        // MaxOARat
        pugi::xml_node maxOARatElement = airSystemOACtrlElement.child("MaxOARat");
        boost::optional<double> _maxOARat = lexicalCastToDouble(maxOARatElement);
        if( _maxOARat && _maxOARat.get() > 0.0 && _maxOARat.get() <= 1.0 )
        {
          model::ScheduleRuleset maxOARatSchedule(model);
          maxOARatSchedule.setName(oaController.name().get() + " Max OA Schedule");
          model::ScheduleDay scheduleDay = maxOARatSchedule.defaultDaySchedule();
          scheduleDay.addValue(Time(1.0),_maxOARat.get());
          oaController.setMaximumFractionofOutdoorAirSchedule(maxOARatSchedule);
        }
      }

      // CtrlType
      auto ctrlType = airSystemOACtrlElement.child("CtrlType").text().as_string();
      oaController.setEconomizerControlActionType(ctrlType);

      // MinLimitType
      auto minLimitType = airSystemOACtrlElement.child("MinLimitType").text().as_string();
      if( istringEqual(minLimitType,"Fixed") ) {
        oaController.setMinimumLimitType("FixedMinimum");
      } else if( istringEqual(minLimitType,"Proportional") ) {
        oaController.setMinimumLimitType("ProportionalMinimum");
      }

      // HtRcvryBypassCtrlType
      auto htRcvryBypassCtrlType = airSystemOACtrlElement.child("HtRcvryBypassCtrlType").text().as_string();
      oaController.setHeatRecoveryBypassControlType(htRcvryBypassCtrlType);

      // EconoAvailSchRef
      auto econoAvailSchRef = airSystemOACtrlElement.child("EconoAvailSchRef").text().as_string();
      if( auto schedule = model.getModelObjectByName<model::Schedule>(econoAvailSchRef) ) {
        oaController.setTimeofDayEconomizerControlSchedule(schedule.get());
      }

      boost::optional<double> _minOAFlow = lexicalCastToDouble(airSystemOACtrlElement.child("MinOAFlow"));
      if( _minOAFlow ) {
        double value = unitToUnit(_minOAFlow.get(),"cfm","m^3/s").get();
        oaController.setMinimumOutdoorAirFlowRate(value);
      }

      boost::optional<double> _maxOAFlow = lexicalCastToDouble(airSystemOACtrlElement.child("MaxOAFlow"));
      if( _maxOAFlow ) {
        double value = unitToUnit(_maxOAFlow.get(),"cfm","m^3/s").get();
        oaController.setMaximumOutdoorAirFlowRate(value);
      }

      // OASchMthd
      pugi::xml_node oaSchMthdElement = airSystemOACtrlElement.child("OASchMthd");
      if( istringEqual(oaSchMthdElement.text().as_string(),"Constant") )
      {
        model::Schedule schedule = model.alwaysOnDiscreteSchedule();
        oaController.setMinimumOutdoorAirSchedule(schedule);
      }
      else if( istringEqual(oaSchMthdElement.text().as_string(),"FollowHVACAvailability") && availabilitySchedule )
      {
        if( boost::optional<model::ScheduleYear> availabilityScheduleYear = availabilitySchedule->optionalCast<model::ScheduleYear>() )
        {
          model::ScheduleYear schedule = deepScheduleYearClone(availabilityScheduleYear.get(), name + " Schedule");

          int startOffset = 0;
          pugi::xml_node availSchOffsetStartElement = airSystemOACtrlElement.child("StartUpDelay");
          boost::optional<int> _offsetValue = lexicalCastToInt(availSchOffsetStartElement);

          if( _offsetValue )
          {
            startOffset = _offsetValue.get();
          }

          adjustSchedule(schedule,startOffset);

          oaController.setMinimumOutdoorAirSchedule(schedule);
        }
      }
      else if( istringEqual(oaSchMthdElement.text().as_string(),"Scheduled") )
      {
        pugi::xml_node oaSchRefElement = airSystemOACtrlElement.child("OASchRef");

        boost::optional<model::Schedule> schedule;
        schedule = model.getModelObjectByName<model::Schedule>(oaSchRefElement.text().as_string());

        if( schedule )
        {
          oaController.setMinimumOutdoorAirSchedule(schedule.get());
        }
      }

      // EconoCtrlMthd
      pugi::xml_node econoCtrlMthdElement = airSystemOACtrlElement.child("EconoCtrlMthd");

      if(istringEqual(econoCtrlMthdElement.text().as_string(),"FixedDryBulb"))
      {
        oaController.setEconomizerControlType("FixedDryBulb");
      }
      else if(istringEqual(econoCtrlMthdElement.text().as_string(),"DifferentialDryBulb"))
      {
        oaController.setEconomizerControlType("DifferentialDryBulb");
      }
      else if(istringEqual(econoCtrlMthdElement.text().as_string(),"DifferentialEnthalpy"))
      {
        oaController.setEconomizerControlType("DifferentialEnthalpy");
      }
      else if(istringEqual(econoCtrlMthdElement.text().as_string(),"DifferentialDryBulbAndEnthalpy"))
      {
        oaController.setEconomizerControlType("DifferentialEnthalpy");
      }

      // EconoIntegration
      pugi::xml_node econoIntegrationElement = airSystemOACtrlElement.child("EconoIntegration");
      if( istringEqual(econoIntegrationElement.text().as_string(),"LockoutWithHeating") )
      {
        oaController.setLockoutType("LockoutWithHeating");
      }
      else if( istringEqual(econoIntegrationElement.text().as_string(),"NonIntegrated") )
      {
        oaController.setLockoutType("LockoutWithCompressor");
      } else {
        oaController.setLockoutType("NoLockout");
      }

      // EconoHiTempLockout
      pugi::xml_node econoHiTempLockoutElement = airSystemOACtrlElement.child("EconoHiTempLockout");
      boost::optional<double> _econoHiTempLockout = lexicalCastToDouble(econoHiTempLockoutElement);
      if( _econoHiTempLockout )
      {
        Quantity valueIP(_econoHiTempLockout.get(),createFahrenheitTemperature());
        OptionalQuantity valueSI = QuantityConverter::instance().convert(valueIP, UnitSystem(UnitSystem::Celsius));
        OS_ASSERT(valueSI);
        oaController.setEconomizerMaximumLimitDryBulbTemperature(valueSI->value());
      }
      else
      {
        oaController.resetEconomizerMaximumLimitDryBulbTemperature();
      }

      // EconolowTempLockout
      pugi::xml_node econoLowTempLockoutElement = airSystemOACtrlElement.child("EconoLowTempLockout");
      boost::optional<double> _econoLowTempLockout = lexicalCastToDouble(econoLowTempLockoutElement);
      if( _econoLowTempLockout )
      {
        Quantity valueIP(_econoLowTempLockout.get(),createFahrenheitTemperature());
        OptionalQuantity valueSI = QuantityConverter::instance().convert(valueIP, UnitSystem(UnitSystem::Celsius));
        OS_ASSERT(valueSI);
        oaController.setEconomizerMinimumLimitDryBulbTemperature(valueSI->value());
      }
      else
      {
        oaController.resetEconomizerMinimumLimitDryBulbTemperature();
      }

      // EconoHiEnthLockout
      const auto econoHiEnthLockoutElement = airSystemOACtrlElement.child("EconoHiEnthLockout");
      if( econoHiEnthLockoutElement ) {
        // manual unit conversion Btu/lb -> J/kg
        const auto econoHiEnthLockout = econoHiEnthLockoutElement.text().as_double() * 2326.0;
        oaController.setEconomizerMaximumLimitEnthalpy(econoHiEnthLockout);
      }

      // TODO Remove this in the future, but for now play it safe and use legacy OpenStudio default.
      if( openstudio::istringEqual(airSystemTypeElement.text().as_string(), "SZVAVAC") ||
          openstudio::istringEqual(airSystemTypeElement.text().as_string(), "SZVAVHP") ) {
        oaController.setHeatRecoveryBypassControlType("BypassWhenOAFlowGreaterThanMinimum");
      }

      oaSystem = newOASystem;
    }
    // Attach IDEC
    if( indirectEvapUsingReturn )
    {
      if( boost::optional<model::Node> outboardReliefNode = oaSystem->outboardReliefNode() )
      {
        indirectEvapUsingReturn->getImpl<model::detail::EvaporativeCoolerIndirectResearchSpecial_Impl>()->setReliefAirInletNode(outboardReliefNode.get());
      }
    }

    OS_ASSERT(oaSystem);
    auto outboardOANode = oaSystem->outboardOANode().get();

    for (std::vector<pugi::xml_node>::size_type i = 0; i < airSegmentElements.size(); i++) {
      auto airSegmentElement = airSegmentElements[i];
      auto airSegmentTypeElement = airSegmentElement.child("Type");

      if(istringEqual(airSegmentTypeElement.text().as_string(),"OutdoorAir")) {

        for (auto it = airSegmentElement.end(); it != airSegmentElement.begin(); --it) {
          pugi::xml_node airSegmentChildElement = *it;
          if( (istringEqual(airSegmentChildElement.name(),"Fan")) ) {
            if( auto mo = translateFan(airSegmentChildElement, model) ) {
              mo->cast<model::HVACComponent>().addToNode(outboardOANode);
            }
          } else if( istringEqual(airSegmentChildElement.name(),"CoilHtg") ) {
            if( auto mo = translateCoilHeating(airSegmentChildElement, model) ) {
              mo->cast<model::HVACComponent>().addToNode(outboardOANode);
            }
          } else if( istringEqual(airSegmentChildElement.name(),"CoilClg") ) {
            if( auto mo = translateCoilCooling(airSegmentChildElement, model) ) {
              mo->cast<model::HVACComponent>().addToNode(outboardOANode);
            }
          } else if( istringEqual(airSegmentChildElement.name(),"EvapClr") ) {
            if( auto mo = translateEvapClr(airSegmentChildElement, model) ) {
              mo->cast<model::HVACComponent>().addToNode(outboardOANode);
            }
          }
        }
      }
    }

    // HtRecvryRef
    // TODO: this is unused!
    [[maybe_unused]] auto htRcvryRefElement = airSystemOACtrlElement.child("HtRcvryRef");

    std::vector<pugi::xml_node> htRcvryElements = makeVectorOfChildren(airSystemElement, "HtRcvry");
    for (std::vector<pugi::xml_node>::size_type i = 0; i < htRcvryElements.size(); i++) {
      auto htRcvryElement = htRcvryElements[i];
      if( auto mo = translateHtRcvry(htRcvryElement, model) ) {
        auto hx = mo->cast<model::HeatExchangerAirToAirSensibleAndLatent>();
        hx.addToNode(outboardOANode);

        // TempCtrlByEMS
        const auto tempCtrlByEMS = htRcvryElement.child("TempCtrlByEMS").text().as_bool(false);

        // TempCtrl
        auto tempCtrl = htRcvryElement.child("TempCtrl").text().as_string();
        if( istringEqual(tempCtrl,"None") ) {
          hx.setSupplyAirOutletTemperatureControl(false);
        } else if( istringEqual(tempCtrl,"Fixed") || tempCtrlByEMS ) {
          hx.setSupplyAirOutletTemperatureControl(true);
          boost::optional<double> _FixedSupTemp = lexicalCastToDouble(htRcvryElement.child("FixedSupTemp"));
          if( _FixedSupTemp ) {
            double value = unitToUnit(_FixedSupTemp.get(),"F","C").get();
            model::ScheduleRuleset schedule(model);
            schedule.setName(hx.nameString() + " Setpoint");
            model::ScheduleDay scheduleDay = schedule.defaultDaySchedule();
            scheduleDay.addValue(Time(1.0),value);

            model::SetpointManagerScheduled spm(model,schedule);
            spm.setName(hx.nameString() + " Setpoint");
            auto nodeMO = hx.primaryAirOutletModelObject();
            OS_ASSERT(nodeMO);
            auto node = nodeMO->cast<model::Node>();
            spm.addToNode(node);
          }
        } else if( istringEqual(tempCtrl,"Scheduled") ) {
          hx.setSupplyAirOutletTemperatureControl(true);
          auto schRef = htRcvryElement.child("TempSetptSchRef").text().as_string();
          auto sch = model.getModelObjectByName<model::Schedule>(schRef);
          if( sch ) {
            model::SetpointManagerScheduled spm(model,sch.get());
            spm.setName(hx.nameString() + " Setpoint");
            auto nodeMO = hx.primaryAirOutletModelObject();
            OS_ASSERT(nodeMO);
            auto node = nodeMO->cast<model::Node>();
            spm.addToNode(node);
          }
        }
      }
    }
  }

  for (std::vector<pugi::xml_node>::size_type i = 0; i < airSegmentElements.size(); i++)
  {
    pugi::xml_node airSegmentElement = airSegmentElements[i];
    pugi::xml_node airSegmentTypeElement = airSegmentElement.child("Type");

    if(istringEqual(airSegmentTypeElement.text().as_string(),"Return"))
    {
      pugi::xml_node fanElement = airSegmentElement.child("Fan");
      if (fanElement)
      {
        if( auto mo = translateFan(fanElement, model) )
        {
          if( auto fan = mo->optionalCast<model::HVACComponent>() ) {
            fan->addToNode(supplyInletNode);
          }
        }
      }
    }
    else if( istringEqual(airSegmentTypeElement.text().as_string(),"Relief") ||
             istringEqual(airSegmentTypeElement.text().as_string(),"Exhaust")
    ) {
      pugi::xml_node fanElement = airSegmentElement.child("Fan");
      if (fanElement)
      {
        if( boost::optional<model::ModelObject> mo = translateFan(fanElement, model) )
        {
          if( oaSystem )
          {
            if( boost::optional<model::Node> outboardReliefNode = oaSystem->outboardReliefNode() )
            {
              if( auto fan = mo->optionalCast<model::HVACComponent>() ) {
                fan->addToNode(outboardReliefNode.get());
              }
            }
          }
        }
      }
    }
  }

  // Add Setpoint managers

  // Preheat Control
  auto prehtCtrlElement = airSystemElement.child("PrehtCtrl");
  if (prehtCtrlElement) {
    if(istringEqual(prehtCtrlElement.text().as_string(),"Fixed")) {
      auto prehtFixedSupTempElement = airSystemElement.child("PrehtFixedSupTemp");
      if (prehtFixedSupTempElement) {
        boost::optional<double> _prehtFixedSupTemp = lexicalCastToDouble(prehtFixedSupTempElement);
        if( _prehtFixedSupTemp ) {
          double value = unitToUnit(_prehtFixedSupTemp.get(),"F","C").get();
          for( auto & comp : heatingComponents ) {
            auto nextcomps = airLoopHVAC.supplyComponents(comp.cast<model::HVACComponent>(),supplyOutletNode);
            OS_ASSERT(nextcomps.size() > 1u );
            auto mo = nextcomps[1];
            auto outletNode = mo.optionalCast<model::Node>();
            OS_ASSERT(outletNode);
            if( outletNode.get() != supplyOutletNode ) {
              model::ScheduleRuleset schedule(model);
              schedule.setName(comp.name().get() + " Preheat Schedule");
              auto scheduleDay = schedule.defaultDaySchedule();
              scheduleDay.addValue(Time(1.0),value);
              model::SetpointManagerScheduled spm(model,schedule);
              spm.setName(comp.name().get() + " Preheat SPM");
              spm.addToNode(outletNode.get());
            } else {
              LOG(Warn,"Ignoring \"PrehtCtrl\" for " + comp.name().get() + ", because it is the last component on the system");
            }
          }
        }
      }
    }
  }

  // Establish deck temperature
  pugi::xml_node clgCtrlElement = airSystemElement.child("ClgCtrl");

  // Lambda for OACtrl types FixedDualSetpoint and ScheduledDualSetpoint
  auto createDualSetpoints = [&](model::Schedule & coolingSchedule, model::Schedule & heatingSchedule) {
    model::SetpointManagerScheduled spm(model,coolingSchedule);
    spm.addToNode(supplyOutletNode);

    std::vector<model::ModelObject> downstreamComps;
    auto fan = airLoopHVAC.supplyFan();
    if( fan ) {
      downstreamComps = airLoopHVAC.supplyComponents(fan.get(),airLoopHVAC.supplyOutletNode());
    }

    // Do this so there is "something" between the supplyOutletNode (which already has an SPM) and the
    // other components where we are about to add a different SPM.
    // Don't want to squash the existing spm.
    if( downstreamComps.size() > 2u ) {
      model::Duct duct(model);
      duct.setName( airLoopHVAC.name().get() + " OS Generated Outlet Duct");
      duct.addToNode(supplyOutletNode);
    }

    std::vector<model::SetpointManagerMixedAir> newMixedAirSPMs;

    auto addDualSPMs = [&](const std::vector<model::ModelObject> & comps,model::Schedule & schedule) {
      for( const auto & comp : comps ) {
        boost::optional<model::Node> outletNode;
        {
          auto nextcomps = airLoopHVAC.supplyComponents(comp.cast<model::HVACComponent>(),supplyOutletNode);
          OS_ASSERT(nextcomps.size() > 1u );
          auto mo = nextcomps[1];
          outletNode = mo.optionalCast<model::Node>();
        }
        OS_ASSERT(outletNode);
        if( outletNode->setpointManagers().empty() ) {
          // Figure out if comp is before or after the fan
          if( std::find(downstreamComps.begin(),downstreamComps.end(),comp) == downstreamComps.end() ) {
            // Before fan
            model::Duct duct(model);
            duct.setName(comp.name().get() + " OS Generated Outlet Duct");
            auto ok = duct.addToNode(outletNode.get());
            OS_ASSERT(ok);
            model::SetpointManagerScheduled spm(model,schedule);
            auto ductOutletNode = duct.outletModelObject()->cast<model::Node>();
            spm.addToNode(ductOutletNode);
            model::SetpointManagerMixedAir spmMixedAir(model);
            newMixedAirSPMs.push_back(spmMixedAir);
            auto ductInletNode = duct.inletModelObject()->cast<model::Node>();
            spmMixedAir.addToNode(ductInletNode);
            spmMixedAir.setReferenceSetpointNode(ductOutletNode);
          } else {
            // After fan
            model::SetpointManagerScheduled spm(model,schedule);
            spm.addToNode(outletNode.get());
          }
        }
      }
    };

    addDualSPMs(coolingComponents,coolingSchedule);
    addDualSPMs(heatingComponents,heatingSchedule);

    // Fixup the fan reference nodes because adding ducts might have messed up the nodes around the fan
    if( fan ) {
      if( auto straightComponentFan = fan->optionalCast<model::StraightComponent>() ) {
        auto fanInletNode = straightComponentFan->inletModelObject()->cast<model::Node>();
        auto fanOutletNode = straightComponentFan->outletModelObject()->cast<model::Node>();
        for( auto & spm : newMixedAirSPMs ) {
          spm.setFanInletNode(fanInletNode);
          spm.setFanOutletNode(fanOutletNode);
        }
      }
    }
  };

  if( istringEqual(clgCtrlElement.text().as_string(),"Fixed") )
  {
    model::ScheduleRuleset schedule(model);

    schedule.setName(airLoopHVAC.name().get() + " Supply Air Temp Schedule");

    pugi::xml_node clgFixedSupTempElement = airSystemElement.child("ClgFixedSupTemp");
    boost::optional<double> _clgFixedSupTemp = lexicalCastToDouble(clgFixedSupTempElement);
    if( _clgFixedSupTemp )
    {
      model::ScheduleDay scheduleDay = schedule.defaultDaySchedule();
      double value = unitToUnit(_clgFixedSupTemp.get(),"F","C").get();
      scheduleDay.addValue(Time(1.0),value);
    }
    else
    {
      model::ScheduleDay scheduleDay = schedule.defaultDaySchedule();

      scheduleDay.addValue(Time(1.0),12.8);
    }

    model::SetpointManagerScheduled spm(model,schedule);
    spm.addToNode(supplyOutletNode);
  }
  else if(istringEqual(clgCtrlElement.text().as_string(),"NoSATControl"))
  {
    model::SetpointManagerSingleZoneReheat spm(model);
    spm.addToNode(supplyOutletNode);

    if( istringEqual("SZVAVAC",airSystemTypeElement.text().as_string()) ||
        istringEqual("SZVAVHP",airSystemTypeElement.text().as_string()) )
    {
      spm.setMaximumSupplyAirTemperature(htgDsgnSupAirTemp);
      spm.setMinimumSupplyAirTemperature(clgDsgnSupAirTemp);
    }
  }
  else if( istringEqual(clgCtrlElement.text().as_string(),"WarmestResetFlowFirst") )
  {
    model::SetpointManagerWarmestTemperatureFlow spm(model);
    spm.setStrategy("FlowFirst");
    spm.addToNode(supplyOutletNode);

    pugi::xml_node clRstSupHiElement = airSystemElement.child("ClRstSupHi");
    boost::optional<double> _clRstSupHi = lexicalCastToDouble(clRstSupHiElement);
    if( _clRstSupHi ) {
      double value = unitToUnit(_clRstSupHi.get(),"F","C").get();
      spm.setMaximumSetpointTemperature(value);
    }

    pugi::xml_node clRstSupLowElement = airSystemElement.child("ClRstSupLow");
    boost::optional<double> _clRstSupLow = lexicalCastToDouble(clRstSupLowElement);
    if( _clRstSupLow ) {
      double value = unitToUnit(_clRstSupLow.get(),"F","C").get();
      spm.setMinimumSetpointTemperature(value);
    }

    auto dsgnAirFlowMinElement = airSystemElement.child("DsgnAirFlowMin");
    boost::optional<double> _dsgnAirFlowMin = lexicalCastToDouble(dsgnAirFlowMinElement);
    if( _dsgnAirFlowMin ) {
      spm.setMinimumTurndownRatio(_dsgnAirFlowMin.get());
    }
  }
  else if( istringEqual(clgCtrlElement.text().as_string(),"WarmestResetTemperatureFirst") )
  {
    model::SetpointManagerWarmestTemperatureFlow spm(model);
    spm.setStrategy("TemperatureFirst");
    spm.addToNode(supplyOutletNode);

    pugi::xml_node clRstSupHiElement = airSystemElement.child("ClRstSupHi");
    boost::optional<double> _clRstSupHi = lexicalCastToDouble(clRstSupHiElement);
    if( _clRstSupHi ) {
      double value = unitToUnit(_clRstSupHi.get(),"F","C").get();
      spm.setMaximumSetpointTemperature(value);
    }

    pugi::xml_node clRstSupLowElement = airSystemElement.child("ClRstSupLow");
    boost::optional<double> _clRstSupLow = lexicalCastToDouble(clRstSupLowElement);
    if( _clRstSupLow ) {
      double value = unitToUnit(_clRstSupLow.get(),"F","C").get();
      spm.setMinimumSetpointTemperature(value);
    }

    auto dsgnAirFlowMinElement = airSystemElement.child("DsgnAirFlowMin");
    boost::optional<double> _dsgnAirFlowMin = lexicalCastToDouble(dsgnAirFlowMinElement);
    if( _dsgnAirFlowMin ) {
      spm.setMinimumTurndownRatio(_dsgnAirFlowMin.get());
    }
  }
  else if( istringEqual(clgCtrlElement.text().as_string(),"WarmestReset") )
  {
    model::SetpointManagerWarmest spm(model);
    spm.addToNode(supplyOutletNode);

    pugi::xml_node clRstSupHiElement = airSystemElement.child("ClRstSupHi");
    boost::optional<double> _clRstSupHi = lexicalCastToDouble(clRstSupHiElement);
    if( _clRstSupHi ) {
      double value = unitToUnit(_clRstSupHi.get(),"F","C").get();
      spm.setMaximumSetpointTemperature(value);
    }

    pugi::xml_node clRstSupLowElement = airSystemElement.child("ClRstSupLow");
    boost::optional<double> _clRstSupLow = lexicalCastToDouble(clRstSupLowElement);
    if( _clRstSupLow ) {
      double value = unitToUnit(_clRstSupLow.get(),"F","C").get();
      spm.setMinimumSetpointTemperature(value);
    }
  }
  else if( istringEqual(clgCtrlElement.text().as_string(),"Scheduled") )
  {
    pugi::xml_node clgSetPtSchRefElement = airSystemElement.child("ClgSetptSchRef");

    boost::optional<model::Schedule> schedule = model.getModelObjectByName<model::Schedule>(clgSetPtSchRefElement.text().as_string());

    if( ! schedule )
    {
      model::ScheduleRuleset schedule(model);

      schedule.setName(airLoopHVAC.name().get() + " Supply Air Temp Schedule");

      model::ScheduleDay scheduleDay = schedule.defaultDaySchedule();

      scheduleDay.addValue(Time(1.0),12.8);
    }

    model::SetpointManagerScheduled spm(model,schedule.get());
    spm.addToNode(supplyOutletNode);
  }
  else if( istringEqual(clgCtrlElement.text().as_string(),"OutsideAirReset") )
  {
    model::SetpointManagerOutdoorAirReset spm(model);
    spm.addToNode(supplyOutletNode);

    boost::optional<double> rstSupHi;
    boost::optional<double> rstSupLow;
    boost::optional<double> rstOutdrHi;
    boost::optional<double> rstOutdrLow;

    pugi::xml_node clRstSupHiElement = airSystemElement.child("ClRstSupHi");
    boost::optional<double> _clRstSupHi = lexicalCastToDouble(clRstSupHiElement);
    if( _clRstSupHi )
    {
      rstSupHi = unitToUnit(_clRstSupHi.get(),"F","C").get();
    }

    pugi::xml_node clRstSupLowElement = airSystemElement.child("ClRstSupLow");
    boost::optional<double> _clRstSupLow = lexicalCastToDouble(clRstSupLowElement);
    if( _clRstSupLow )
    {
      rstSupLow = unitToUnit(_clRstSupLow.get(),"F","C").get();
    }

    pugi::xml_node clRstOutdrHiElement = airSystemElement.child("ClRstOutdrHi");
    boost::optional<double> _clRstOutdrHi = lexicalCastToDouble(clRstOutdrHiElement);
    if( _clRstOutdrHi )
    {
      rstOutdrHi = unitToUnit(_clRstOutdrHi.get(),"F","C").get();
    }

    pugi::xml_node clRstOutdrLowElement = airSystemElement.child("ClRstOutdrLow");
    boost::optional<double> _clRstOutdrLow = lexicalCastToDouble(clRstOutdrLowElement);
    if( _clRstOutdrLow )
    {
      rstOutdrLow = unitToUnit(_clRstOutdrLow.get(),"F","C").get();
    }

    if( rstSupHi && rstSupLow && rstOutdrHi && rstOutdrLow )
    {
      if( rstOutdrHi > rstOutdrLow )
      {
        spm.setOutdoorHighTemperature(rstOutdrHi.get());
        spm.setSetpointatOutdoorHighTemperature(rstSupHi.get());
        spm.setOutdoorLowTemperature(rstOutdrLow.get());
        spm.setSetpointatOutdoorLowTemperature(rstSupLow.get());
      }
      else
      {
        spm.setOutdoorHighTemperature(rstOutdrLow.get());
        spm.setSetpointatOutdoorHighTemperature(rstSupLow.get());
        spm.setOutdoorLowTemperature(rstOutdrHi.get());
        spm.setSetpointatOutdoorLowTemperature(rstSupHi.get());
      }
    }
    else
    {
      LOG(Warn,nameElement.text().as_string() << " requests OA reset control, but does not define setpoints."
        << "  Using OpenStudio defaults.");

      spm.setOutdoorLowTemperature(10.0);
      spm.setSetpointatOutdoorLowTemperature(22.0);
      spm.setOutdoorHighTemperature(24.0);
      spm.setSetpointatOutdoorHighTemperature(10.0);
    }
  }
  else if( istringEqual(clgCtrlElement.text().as_string(),"OutsideAirResetDualSetpoint") )
  {
    // Add a default SingleZoneReheat to satisfy need to have SPM on the supplyOutletNode
    model::SetpointManagerSingleZoneReheat singleZoneReheat(model);
    singleZoneReheat.addToNode(supplyOutletNode);

    const auto createSPMOutsideAirReset = [&](
        const double& rstOutdrHi,
        const double& rstSPHi,
        const double& rstOutdrLow,
        const double& rstSPLow
    ) {
      model::SetpointManagerOutdoorAirReset spm(model);
      spm.setOutdoorHighTemperature(rstOutdrLow);
      spm.setSetpointatOutdoorHighTemperature(rstSPLow);
      spm.setOutdoorLowTemperature(rstOutdrHi);
      spm.setSetpointatOutdoorLowTemperature(rstSPHi);
      return spm;
    };

    auto clRstSupLow = lexicalCastToDouble(airSystemElement.child("ClRstSupLow"));
    if( clRstSupLow ) {
      clRstSupLow = unitToUnit(clRstSupLow.get(),"F","C").get();
    }
    auto clRstOutdrLow = lexicalCastToDouble(airSystemElement.child("ClRstOutdrLow"));
    if( clRstOutdrLow ) {
      clRstOutdrLow = unitToUnit(clRstOutdrLow.get(),"F","C").get();
    }
    auto clRstSupHi = lexicalCastToDouble(airSystemElement.child("ClRstSupHi"));
    if( clRstSupHi ) {
      clRstSupHi = unitToUnit(clRstSupHi.get(),"F","C").get();
    }
    auto clRstOutdrHi = lexicalCastToDouble(airSystemElement.child("ClRstOutdrHi"));
    if( clRstOutdrHi ) {
      clRstOutdrHi = unitToUnit(clRstOutdrHi.get(),"F","C").get();
    }

    if (clRstSupLow && clRstOutdrLow && clRstSupHi && clRstOutdrHi) {
      for (const auto &comp : coolingComponents) {
        auto spm = createSPMOutsideAirReset(clRstOutdrHi.get(), clRstSupHi.get(), clRstOutdrLow.get(), clRstSupLow.get());
        auto straightComp = comp.optionalCast<model::StraightComponent>();
        auto waterToAirComp = comp.optionalCast<model::WaterToAirComponent>();
        if (straightComp) {
          auto node = straightComp->outletModelObject()->cast<model::Node>();
          spm.addToNode(node);
        } else if (waterToAirComp) {
          auto node = waterToAirComp->airOutletModelObject()->cast<model::Node>();
          spm.addToNode(node);
        }
      }
    }

    auto htRstSupLow = lexicalCastToDouble(airSystemElement.child("HtRstSupLow"));
    if( htRstSupLow ) {
      htRstSupLow = unitToUnit(htRstSupLow.get(),"F","C").get();
    }
    auto htRstOutdrLow = lexicalCastToDouble(airSystemElement.child("HtRstOutdrLow"));
    if( htRstOutdrLow ) {
      htRstOutdrLow = unitToUnit(htRstOutdrLow.get(),"F","C").get();
    }
    auto htRstSupHi = lexicalCastToDouble(airSystemElement.child("HtRstSupHi"));
    if( htRstSupHi ) {
      htRstSupHi = unitToUnit(htRstSupHi.get(),"F","C").get();
    }
    auto htRstOutdrHi = lexicalCastToDouble(airSystemElement.child("HtRstOutdrHi"));
    if( htRstOutdrHi ) {
      htRstOutdrHi = unitToUnit(htRstOutdrHi.get(),"F","C").get();
    }

    if (htRstSupLow && htRstOutdrLow && htRstSupHi && htRstOutdrHi) {
      for (const auto &comp : heatingComponents) {
        auto spm = createSPMOutsideAirReset(htRstOutdrHi.get(), htRstSupHi.get(), htRstOutdrLow.get(), htRstSupLow.get());
        auto straightComp = comp.optionalCast<model::StraightComponent>();
        auto waterToAirComp = comp.optionalCast<model::WaterToAirComponent>();
        if (straightComp) {
          auto node = straightComp->outletModelObject()->cast<model::Node>();
          spm.addToNode(node);
        } else if (waterToAirComp) {
          auto node = waterToAirComp->airOutletModelObject()->cast<model::Node>();
          spm.addToNode(node);
        }
      }
    }
  }
  else if( istringEqual(clgCtrlElement.text().as_string(),"FixedDualSetpoint") )
  {
    double clgFixedSupTemp = 23.89;
    double htgFixedSupTemp = 15.56;

    pugi::xml_node clgFixedSupTempElement = airSystemElement.child("ClgFixedSupTemp");
    boost::optional<double> _clgFixedSupTemp = lexicalCastToDouble(clgFixedSupTempElement);
    if( _clgFixedSupTemp ) {
      clgFixedSupTemp = unitToUnit(_clgFixedSupTemp.get(),"F","C").get();
    } else {
      LOG(Warn,nameElement.text().as_string() << " requests fixed dual setpoint control, but does not define setpoints."
        << " Using OpenStudio defaults.");
    }

    pugi::xml_node htgFixedSupTempElement = airSystemElement.child("HtgFixedSupTemp");
    boost::optional<double> _htgFixedSupTemp = lexicalCastToDouble(htgFixedSupTempElement);
    if( _htgFixedSupTemp ) {
      htgFixedSupTemp = unitToUnit(_htgFixedSupTemp.get(),"F","C").get();
    } else {
      LOG(Warn,nameElement.text().as_string() << " requests fixed dual setpoint control, but does not define setpoints."
        << " Using OpenStudio defaults.");
    }

    model::ScheduleRuleset coolingSchedule(model);
    coolingSchedule.setName(airLoopHVAC.name().get() + " Cooling SP Schedule");
    coolingSchedule.defaultDaySchedule().addValue(Time(1.0), clgFixedSupTemp);

    model::ScheduleRuleset heatingSchedule(model);
    heatingSchedule.setName(airLoopHVAC.name().get() + " Heating SP Schedule");
    heatingSchedule.defaultDaySchedule().addValue(Time(1.0), htgFixedSupTemp);

    createDualSetpoints(coolingSchedule,heatingSchedule);
  }
  else if( istringEqual(clgCtrlElement.text().as_string(),"ScheduledDualSetPoint") )
  {
    boost::optional<model::Schedule> coolingSchedule;
    boost::optional<model::Schedule> heatingSchedule;

    double clgFixedSupTemp = 23.89;
    double htgFixedSupTemp = 15.56;

    pugi::xml_node clgSetptSchRefElement = airSystemElement.child("ClgSetptSchRef");
    std::string clgSetptSchRef = escapeName(clgSetptSchRefElement.text().as_string());
    coolingSchedule = model.getModelObjectByName<model::Schedule>(clgSetptSchRef);

    if( ! coolingSchedule ) {
      LOG(Warn,nameElement.text().as_string() << " requests scheduled dual setpoint control, but does not define schedules."
        << " Using OpenStudio defaults.");

      model::ScheduleRuleset schedule(model);
      schedule.setName(airLoopHVAC.name().get() + " Cooling SP Schedule");
      schedule.defaultDaySchedule().addValue(Time(1.0), clgFixedSupTemp);
      coolingSchedule = schedule;
    }

    pugi::xml_node htgSetptSchRefElement = airSystemElement.child("HtgSetptSchRef");
    std::string htgSetptSchRef = escapeName(htgSetptSchRefElement.text().as_string());
    heatingSchedule = model.getModelObjectByName<model::Schedule>(htgSetptSchRef);

    if( ! heatingSchedule ) {
      LOG(Warn,nameElement.text().as_string() << " requests scheduled dual setpoint control, but does not define schedules."
        << " Using OpenStudio defaults.");

      model::ScheduleRuleset schedule(model);
      schedule.setName(airLoopHVAC.name().get() + " Heating SP Schedule");
      schedule.defaultDaySchedule().addValue(Time(1.0), htgFixedSupTemp);
      heatingSchedule = schedule;
    }

    OS_ASSERT(coolingSchedule);
    OS_ASSERT(heatingSchedule);

    createDualSetpoints(coolingSchedule.get(),heatingSchedule.get());
  }

  return result;
}

boost::optional<openstudio::model::ModelObject> ReverseTranslator::translateCoilHeating(
  const pugi::xml_node& heatingCoilElement,
  openstudio::model::Model& model)
{
  boost::optional<model::ModelObject> result;

  if( ! istringEqual(heatingCoilElement.name(),"CoilHtg") )
  {
    return result;
  }

  bool znSys = false;
  if( openstudio::istringEqual(heatingCoilElement.parent().name(), "ZnSys"))
  {
    znSys = true;
  }

  // Type
  pugi::xml_node coilHeatingTypeElement = heatingCoilElement.child("Type");
  pugi::xml_node nameElement = heatingCoilElement.child("Name");
  std::string coilName = nameElement.text().as_string();
  pugi::xml_node capTotRtdElement = heatingCoilElement.child("CapTotGrossRtdSim");

  // AvailCtrlByEMS
  const auto availCtrlByEMS = heatingCoilElement.child("AvailCtrlByEMS").text().as_int(0);
  
  boost::optional<model::Schedule> availabilitySchedule;
  if(availCtrlByEMS == 1) {
    availabilitySchedule = model.alwaysOnDiscreteSchedule().clone(model).cast<model::Schedule>();
    availabilitySchedule->setName(coilName + " AvailSch");
  } else {
    availabilitySchedule = model.alwaysOnDiscreteSchedule();
  }

  // CapTotGrossRtd
  boost::optional<double> capTotGrossRtd;

  if( ! autosize() )
  {
    boost::optional<double> _capTotRtd = lexicalCastToDouble(capTotRtdElement);

    if( _capTotRtd )
    {
      Quantity valueIP(_capTotRtd.get(),createBTUPower());
      OptionalQuantity valueSI = QuantityConverter::instance().convert(valueIP, UnitSystem(UnitSystem::SI));
      OS_ASSERT(valueSI);
      capTotGrossRtd = valueSI->value();
    }
  }

  // Flow Capacity
  // Look for a sibling fan to figure out what the flow capacity should be
  boost::optional<double> _flowCap;

  if( ! autosize() )
  {
    pugi::xml_node flowCapElement;

    pugi::xml_node airSegElement = heatingCoilElement.parent();

    if (airSegElement)
    {
      pugi::xml_node fanElement = airSegElement.child("Fan");

      if (fanElement)
      {
        flowCapElement = fanElement.child("FlowCapSim");

        _flowCap = lexicalCastToDouble(flowCapElement);
        if( _flowCap )
        {
          _flowCap = unitToUnit(_flowCap.get(),"cfm","m^3/s").get();
        }
      }
    }
  }

  // Furnace
  if( istringEqual(coilHeatingTypeElement.text().as_string(),"Furnace") )
  {
    model::Schedule schedule = model.alwaysOnDiscreteSchedule();

    model::CoilHeatingGas coil(model,schedule);

    coil.setName(coilName);

    coil.setAvailableSchedule(availabilitySchedule.get());

    // CapTotGrossRtd
    if( capTotGrossRtd )
    {
      coil.setNominalCapacity(capTotGrossRtd.get());
    }

    // FurnHIR_fPLRCrvRef
    boost::optional<model::Curve> hirCurve;
    pugi::xml_node hirCurveElement =
      heatingCoilElement.child("FurnHIR_fPLRCrvRef");
    hirCurve =
      model.getModelObjectByName<model::Curve>(
        hirCurveElement.text().as_string());
    if( hirCurve )
    {
      coil.setPartLoadFractionCorrelationCurve(hirCurve.get());
    }

    // FurnThrmlEff
    pugi::xml_node furnThrmlEffElement = heatingCoilElement.child("FurnThrmlEff");
    boost::optional<double> _furnThrmlEff = lexicalCastToDouble(furnThrmlEffElement);
    if( _furnThrmlEff )
    {
      coil.setGasBurnerEfficiency(_furnThrmlEff.get());
    }

    // FurnPilotFuelInp
    pugi::xml_node furnPilotFuelInpElement = heatingCoilElement.child("FurnPilotFuelInp");
    boost::optional<double> _furnPilotFuelInp = lexicalCastToDouble(furnPilotFuelInpElement);
    if( _furnPilotFuelInp )
    {
      double value = unitToUnit(_furnPilotFuelInp.get(),"Btu/h","W").get();
      coil.setParasiticGasLoad(value);
    }

    result = coil;
  }
  else if( istringEqual(coilHeatingTypeElement.text().as_string(),"HotWater") )
  {
    model::Schedule schedule = model.alwaysOnDiscreteSchedule();

    model::CoilHeatingWater coil(model,schedule);

    coil.setName(coilName);

    coil.setAvailableSchedule(availabilitySchedule.get());

    coil.setRatedInletWaterTemperature(82.2);
    coil.setRatedInletAirTemperature(16.6);
    coil.setRatedOutletWaterTemperature(71.1);
    coil.setRatedOutletAirTemperature(32.2);

    pugi::xml_node fluidSegInRefElement = heatingCoilElement.child("FluidSegInRef");

    boost::optional<model::PlantLoop> plant;
    if (!fluidSegInRefElement) {
      LOG(Error, "CoilHeatingWater '" << coilName << "' doesn't have a FluidSegInRef element! It will not be connected.");
    } else {
      boost::optional<model::PlantLoop> plant = loopForSupplySegment(fluidSegInRefElement, model);

      if( plant )
      {
        plant->addDemandBranchForComponent(coil);
        // Figure out if this is connected to a ServiceHotWater system
        // If it is then make sure any supply segment pumps go on the branch,
        // as opposed to the demand side inlet branch
        auto supplySegmentElement = supplySegment(fluidSegInRefElement);
        auto fluidSysTypeElment = supplySegmentElement.parent().child("Type");
        if (openstudio::istringEqual(fluidSysTypeElment.text().as_string(), "ServiceHotWater")) {
          auto pumpElement = supplySegmentElement.child("Pump");
          if (pumpElement) {
            if( auto modelObject = translatePump(pumpElement, model) ) {
              auto hvacComponent = modelObject->cast<model::HVACComponent>();
              auto inletNode = coil.waterInletModelObject()->cast<model::Node>();
              hvacComponent.addToNode(inletNode);
            }
          }
        }

        // Replace the default temperature rating values, with values that align with the plant sizing.
        coil.setRatedInletWaterTemperature(plant->sizingPlant().designLoopExitTemperature());
        const auto outletTemp = plant->sizingPlant().designLoopExitTemperature() - plant->sizingPlant().loopDesignTemperatureDifference();
        coil.setRatedOutletWaterTemperature(outletTemp);
      } else {
        LOG(Error, "CoilHeatingWater '" << coilName << "' has a FluidSegInRef of '" << fluidSegInRefElement.text().as_string()
                << "' but we couldn't find a plantLoop that matches.");
      }
    }

    if( capTotGrossRtd )
    {
      coil.setPerformanceInputMethod("NominalCapacity");

      coil.setRatedCapacity(capTotGrossRtd.get());

      // Find related/containing systems (aka figure out the context of the coil)

      pugi::xml_node sysElement;

      pugi::xml_node airSysElement = heatingCoilElement.parent().parent();

      pugi::xml_node znSysElement = heatingCoilElement.parent();

      if( openstudio::istringEqual(airSysElement.name(), "AirSys"))
      {
        sysElement = airSysElement;
      }
      else if (openstudio::istringEqual(airSysElement.name(), "ZnSys"))
      {
        sysElement = znSysElement;
      }
    }

    auto fluidFlowRtDsgnSim = lexicalCastToDouble(heatingCoilElement.child("FluidFlowRtDsgnSim"));
    if(fluidFlowRtDsgnSim) {
      fluidFlowRtDsgnSim = unitToUnit(fluidFlowRtDsgnSim.get(),"gal/min","m^3/s").get();
      coil.setMaximumWaterFlowRate(fluidFlowRtDsgnSim.get());
    }

    result = coil;
  }
  else if( istringEqual(coilHeatingTypeElement.text().as_string(),"Resistance") )
  {
    model::Schedule schedule = model.alwaysOnDiscreteSchedule();

    model::CoilHeatingElectric coil(model,schedule);

    coil.setName(coilName);

    coil.setAvailabilitySchedule(availabilitySchedule.get());

    // CapTotGrossRtd
    if( capTotGrossRtd )
    {
      coil.setNominalCapacity(capTotGrossRtd.get());
    }

    result = coil;
  }
  else if( istringEqual(coilHeatingTypeElement.text().as_string(),"HeatPump") )
  {
    pugi::xml_node cndsrTypeElement = heatingCoilElement.child("CndsrType");
    if (openstudio::istringEqual(cndsrTypeElement.text().as_string(), "WaterSource"))
    {
      model::CoilHeatingWaterToAirHeatPumpEquationFit coil(model);
      coil.setName(coilName);
      boost::optional<model::AirLoopHVACUnitarySystem> unitary;
      if( znSys )
      {
        result = coil;
      }
      else
      {
        unitary = model::AirLoopHVACUnitarySystem(model);
        unitary->setName(coilName + " Unitary");
        unitary->getImpl<model::detail::AirLoopHVACUnitarySystem_Impl>()->setControlType("SetPoint");
        unitary->setSupplyAirFlowRateMethodWhenNoCoolingorHeatingisRequired("SupplyAirFlowRate");
        unitary->setSupplyAirFlowRateMethodDuringHeatingOperation("SupplyAirFlowRate");
        unitary->setSupplyAirFlowRateMethodDuringCoolingOperation("SupplyAirFlowRate");
        unitary->setHeatingCoil(coil);
        result = unitary.get();
      }

      // Plant
      pugi::xml_node fluidSegInRefElement = heatingCoilElement.child("FluidSegInRef");
      if (!fluidSegInRefElement) {
        LOG(Error, "CoilHeatingWaterToAirHeatPumpEquationFit '" << coilName << "' doesn't have a FluidSegInRef element! It will not be connected.");
      } else {
        boost::optional<model::PlantLoop> plant = loopForSupplySegment(fluidSegInRefElement, model);
        if( plant )
        {
          plant->addDemandBranchForComponent(coil);
        }
      }

      if( ! autosize() )
      {
        // Look for a sibling fan to figure out what the flow capacity should be
        pugi::xml_node flowCapElement;
        pugi::xml_node airSegElement = heatingCoilElement.parent();
        if (airSegElement)
        {
          pugi::xml_node fanElement = airSegElement.child("Fan");
          if (fanElement)
          {
            flowCapElement = fanElement.child("FlowCapSim");
          }
        }

        _flowCap = lexicalCastToDouble(flowCapElement);
        if( _flowCap )
        {
          double value = unitToUnit(_flowCap.get(),"cfm","m^3/s").get();
          coil.setRatedAirFlowRate(value);
          if( unitary )
          {
            unitary->setSupplyAirFlowRateDuringHeatingOperation(value);
          }
        }
      }

      if( ! autosize() )
      {
        pugi::xml_node fluidFlowRtDsgnSimElement = heatingCoilElement.child("FluidFlowRtDsgnSim");
        boost::optional<double> _fluidFlowRtDsgnSim = lexicalCastToDouble(fluidFlowRtDsgnSimElement);
        if( _fluidFlowRtDsgnSim )
        {
          double value = unitToUnit(_fluidFlowRtDsgnSim.get(),"gal/min","m^3/s").get();
          coil.setRatedWaterFlowRate(value);
        }
      }

      if( ! autosize() )
      {
        pugi::xml_node capTotRtdSimElement = heatingCoilElement.child("CapTotGrossRtdSim");
        boost::optional<double> _capTotRtdSim = lexicalCastToDouble(capTotRtdSimElement);
        if( _capTotRtdSim )
        {
          double value = unitToUnit(_capTotRtdSim.get(),"Btu/h","W").get();
          coil.setRatedHeatingCapacity(value);
        }
      }

      pugi::xml_node htPumpCOPElement = heatingCoilElement.child("HtPumpCOP");
      boost::optional<double> _htPumpCOP = lexicalCastToDouble(htPumpCOPElement);
      if( _htPumpCOP )
      {
        coil.setRatedHeatingCoefficientofPerformance(_htPumpCOP.get());
      }

      pugi::xml_node htPumpEIRElement = heatingCoilElement.child("HtPumpEIR");
      boost::optional<double> _htPumpEIR = lexicalCastToDouble(htPumpEIRElement);
      if( _htPumpEIR )
      {
        coil.setRatedHeatingCoefficientofPerformance(1.0 / _htPumpEIR.get());
      }

      coil.setHeatingCapacityCoefficient1(-1.361311959);
      coil.setHeatingCapacityCoefficient2(-2.471798046);
      coil.setHeatingCapacityCoefficient3(4.173164514);
      coil.setHeatingCapacityCoefficient4(0.640757401);
      coil.setHeatingCapacityCoefficient5(0.0);

      coil.setHeatingPowerConsumptionCoefficient1(-2.176941116);
      coil.setHeatingPowerConsumptionCoefficient2(0.832114286);
      coil.setHeatingPowerConsumptionCoefficient3(1.570743399);
      coil.setHeatingPowerConsumptionCoefficient4(0.690793651);
      coil.setHeatingPowerConsumptionCoefficient5(0.0);
    }
    else
    {
      // HtPumpCap_fTempCrvRef
      boost::optional<model::Curve> totalHeatingCapacityFunctionofTemperatureCurve;
      pugi::xml_node totalHeatingCapacityFunctionofTemperatureCurveElement =
        heatingCoilElement.child("HtPumpCap_fTempCrvRef");
      totalHeatingCapacityFunctionofTemperatureCurve =
        model.getModelObjectByName<model::Curve>(
          totalHeatingCapacityFunctionofTemperatureCurveElement.text().as_string());

      if( ! totalHeatingCapacityFunctionofTemperatureCurve )
      {
        model::CurveCubic _totalHeatingCapacityFunctionofTemperatureCurve(model);
        _totalHeatingCapacityFunctionofTemperatureCurve.setCoefficient1Constant(0.758746);
        _totalHeatingCapacityFunctionofTemperatureCurve.setCoefficient2x(0.027626);
        _totalHeatingCapacityFunctionofTemperatureCurve.setCoefficient3xPOW2(0.000148716);
        _totalHeatingCapacityFunctionofTemperatureCurve.setCoefficient4xPOW3(0.0000034992);
        _totalHeatingCapacityFunctionofTemperatureCurve.setMinimumValueofx(-20.0);
        _totalHeatingCapacityFunctionofTemperatureCurve.setMaximumValueofx(20.0);

        totalHeatingCapacityFunctionofTemperatureCurve = _totalHeatingCapacityFunctionofTemperatureCurve;
      }

      // HtPumpCap_fFlowCrvRef
      boost::optional<model::Curve> totalHeatingCapacityFunctionofFlowFractionCurve;
      pugi::xml_node totalHeatingCapacityFunctionofFlowFractionCurveElement =
        heatingCoilElement.child("HtPumpCap_fFlowCrvRef");
      totalHeatingCapacityFunctionofFlowFractionCurve =
        model.getModelObjectByName<model::Curve>(
          totalHeatingCapacityFunctionofFlowFractionCurveElement.text().as_string());

      if( ! totalHeatingCapacityFunctionofFlowFractionCurve )
      {
        model::CurveCubic _totalHeatingCapacityFunctionofFlowFractionCurve(model);
        _totalHeatingCapacityFunctionofFlowFractionCurve.setCoefficient1Constant(0.84);
        _totalHeatingCapacityFunctionofFlowFractionCurve.setCoefficient2x(0.16);
        _totalHeatingCapacityFunctionofFlowFractionCurve.setCoefficient3xPOW2(0.0);
        _totalHeatingCapacityFunctionofFlowFractionCurve.setCoefficient4xPOW3(0.0);
        _totalHeatingCapacityFunctionofFlowFractionCurve.setMinimumValueofx(0.5);
        _totalHeatingCapacityFunctionofFlowFractionCurve.setMaximumValueofx(1.5);

        totalHeatingCapacityFunctionofFlowFractionCurve = _totalHeatingCapacityFunctionofFlowFractionCurve;
      }

      // HtPumpEIR_fTempCrvRef
      boost::optional<model::Curve> energyInputRatioFunctionofTemperatureCurve;
      pugi::xml_node energyInputRatioFunctionofTemperatureCurveElement =
        heatingCoilElement.child("HtPumpEIR_fTempCrvRef");
      energyInputRatioFunctionofTemperatureCurve =
        model.getModelObjectByName<model::Curve>(
          energyInputRatioFunctionofTemperatureCurveElement.text().as_string());

      if( ! energyInputRatioFunctionofTemperatureCurve )
      {
        model::CurveCubic _energyInputRatioFunctionofTemperatureCurve(model);
        _energyInputRatioFunctionofTemperatureCurve.setCoefficient1Constant(1.19248);
        _energyInputRatioFunctionofTemperatureCurve.setCoefficient2x(-0.0300438);
        _energyInputRatioFunctionofTemperatureCurve.setCoefficient3xPOW2(0.00103745);
        _energyInputRatioFunctionofTemperatureCurve.setCoefficient4xPOW3(-0.000023328);
        _energyInputRatioFunctionofTemperatureCurve.setMinimumValueofx(-20.0);
        _energyInputRatioFunctionofTemperatureCurve.setMaximumValueofx(20.0);

        energyInputRatioFunctionofTemperatureCurve = _energyInputRatioFunctionofTemperatureCurve;
      }

      // HtPumpEIR_fFlowCrvRef
      boost::optional<model::Curve> energyInputRatioFunctionofFlowFractionCurve;
      pugi::xml_node energyInputRatioFunctionofFlowFractionCurveElement =
        heatingCoilElement.child("HtPumpEIR_fFlowCrvRef");
      energyInputRatioFunctionofFlowFractionCurve =
        model.getModelObjectByName<model::Curve>(
          energyInputRatioFunctionofFlowFractionCurveElement.text().as_string());

      if( ! energyInputRatioFunctionofFlowFractionCurve )
      {
        model::CurveQuadratic _energyInputRatioFunctionofFlowFractionCurve(model);
        _energyInputRatioFunctionofFlowFractionCurve.setCoefficient1Constant(1.3824);
        _energyInputRatioFunctionofFlowFractionCurve.setCoefficient2x(-0.4336);
        _energyInputRatioFunctionofFlowFractionCurve.setCoefficient3xPOW2(0.0512);
        _energyInputRatioFunctionofFlowFractionCurve.setMinimumValueofx(0.0);
        _energyInputRatioFunctionofFlowFractionCurve.setMaximumValueofx(1.0);

        energyInputRatioFunctionofFlowFractionCurve = _energyInputRatioFunctionofFlowFractionCurve;
      }

      // HtPumpEIR_fPLFCrvRef
      boost::optional<model::Curve> partLoadFractionCorrelationCurve;
      pugi::xml_node partLoadFractionCorrelationCurveElement =
        heatingCoilElement.child("HtPumpEIR_fPLFCrvRef");
      partLoadFractionCorrelationCurve =
        model.getModelObjectByName<model::Curve>(
          partLoadFractionCorrelationCurveElement.text().as_string());

      if( ! partLoadFractionCorrelationCurve )
      {
        model::CurveQuadratic _partLoadFractionCorrelationCurve(model);
        _partLoadFractionCorrelationCurve.setCoefficient1Constant(0.75);
        _partLoadFractionCorrelationCurve.setCoefficient2x(0.25);
        _partLoadFractionCorrelationCurve.setCoefficient3xPOW2(0.0);
        _partLoadFractionCorrelationCurve.setMinimumValueofx(0.0);
        _partLoadFractionCorrelationCurve.setMaximumValueofx(1.0);

        partLoadFractionCorrelationCurve = _partLoadFractionCorrelationCurve;
      }

      model::Schedule schedule = model.alwaysOnDiscreteSchedule();

      model::CoilHeatingDXSingleSpeed heatingCoil( model,
                                                   schedule,
                                                   totalHeatingCapacityFunctionofTemperatureCurve.get(),
                                                   totalHeatingCapacityFunctionofFlowFractionCurve.get(),
                                                   energyInputRatioFunctionofTemperatureCurve.get(),
                                                   energyInputRatioFunctionofFlowFractionCurve.get(),
                                                   partLoadFractionCorrelationCurve.get() );

      // Name
      heatingCoil.setName(coilName);

      heatingCoil.setAvailabilitySchedule(availabilitySchedule.get());

      // FlowCapSim
      if( _flowCap )
      {
        heatingCoil.setRatedAirFlowRate(_flowCap.get());
      }

      // CapTotGrossRtdSim
      if( capTotGrossRtd )
      {
        heatingCoil.setRatedTotalHeatingCapacity(capTotGrossRtd.get());
      }

      // HtPumpEIR
      pugi::xml_node htPumpEIRElement = heatingCoilElement.child("HtPumpEIR");
      boost::optional<double> _htPumpEIR = lexicalCastToDouble(htPumpEIRElement);
      if( _htPumpEIR )
      {
        heatingCoil.setRatedCOP(1.0 / _htPumpEIR.get());
      }

      // HtPumpCprsrLockoutTemp
      pugi::xml_node htPumpCprsrLockoutTempElement = heatingCoilElement.child("HtPumpCprsrLockoutTemp");
      boost::optional<double> _htPumpCprsrLockoutTemp = lexicalCastToDouble(htPumpCprsrLockoutTempElement);
      if( _htPumpCprsrLockoutTemp )
      {
        double value = unitToUnit(_htPumpCprsrLockoutTemp.get(),"F","C").get();
        heatingCoil.setMinimumOutdoorDryBulbTemperatureforCompressorOperation(value);
      }

      // HtPumpCrankcaseHtCap
      pugi::xml_node htPumpCrankcaseHtCapElement = heatingCoilElement.child("HtPumpCrankcaseHtrCapSim");
      boost::optional<double> _htPumpCrankcaseHtCap = lexicalCastToDouble(htPumpCrankcaseHtCapElement);
      if( _htPumpCrankcaseHtCap )
      {
        double value = unitToUnit(_htPumpCrankcaseHtCap.get(),"Btu/h","W").get();
        heatingCoil.setCrankcaseHeaterCapacity(value);
      }

      // HtPumpCrankcaseCtrlTemp
      pugi::xml_node htPumpCrankcaseCtrlTempElement = heatingCoilElement.child("HtPumpCrankcaseCtrlTemp");
      boost::optional<double> _htPumpCrankcaseCtrlTemp = lexicalCastToDouble(htPumpCrankcaseCtrlTempElement);
      if( _htPumpCrankcaseCtrlTemp )
      {
        double value = unitToUnit(_htPumpCrankcaseCtrlTemp.get(),"F","C").get();
        heatingCoil.setMaximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation(value);
      }

      // HtPumpDefHtSrc
      pugi::xml_node htPumpDefHtSrcElement = heatingCoilElement.child("HtPumpDefHtSrc");
      if( istringEqual(htPumpDefHtSrcElement.text().as_string(),"Electric") )
      {
        heatingCoil.setDefrostStrategy("Resistive");
      }
      else if( istringEqual(htPumpDefHtSrcElement.text().as_string(),"HotGas") )
      {
        heatingCoil.setDefrostStrategy("ReverseCycle");

        model::CurveBiquadratic defrostCurve(model);
        defrostCurve.setName(heatingCoil.name().get() + " Defrost Curve");
        defrostCurve.setCoefficient1Constant(1.0);
        heatingCoil.setDefrostEnergyInputRatioFunctionofTemperatureCurve(defrostCurve);
      }

      // HtPumpDefHtrCapSim
      if( ! autosize() )
      {
        pugi::xml_node htPumpDefHtCapElement = heatingCoilElement.child("HtPumpDefHtrCapSim");
        boost::optional<double> _htPumpDefHtCap = lexicalCastToDouble(htPumpDefHtCapElement);
        if( _htPumpDefHtCap )
        {
          double value = unitToUnit(_htPumpDefHtCap.get(),"Btu/h","W").get();
          heatingCoil.setResistiveDefrostHeaterCapacity(value);
        }
      }
      else
      {
        heatingCoil.autosizeResistiveDefrostHeaterCapacity();
      }

      // HtPumpDefCtrl
      pugi::xml_node htPumpDefCtrlElement = heatingCoilElement.child("HtPumpDefCtrl");
      if( istringEqual(htPumpDefCtrlElement.text().as_string(),"OnDemand") )
      {
        heatingCoil.setDefrostControl("OnDemand");
      }
      else if( istringEqual(htPumpDefCtrlElement.text().as_string(),"TimedCycle") )
      {
        heatingCoil.setDefrostControl("Timed");
      }

      // HtPumpDefCtrlTemp
      pugi::xml_node htPumpDefCtrlTempElement = heatingCoilElement.child("HtPumpDefCtrlTemp");
      boost::optional<double> _htPumpDefCtrlTemp = lexicalCastToDouble(htPumpDefCtrlTempElement);
      if( _htPumpDefCtrlTemp )
      {
        double value = unitToUnit(_htPumpDefCtrlTemp.get(),"F","C").get();
        heatingCoil.setMaximumOutdoorDryBulbTemperatureforDefrostOperation(value);
      }

      // DefrostTimePeriodFraction
      heatingCoil.setDefrostTimePeriodFraction(0.058333);

      result = heatingCoil;
    }
  }
  else
  {
    model::CoilHeatingElectric coil(model,availabilitySchedule.get());

    coil.setName(coilName);

    // CapTotGrossRtd
    if( capTotGrossRtd )
    {
      coil.setNominalCapacity(capTotGrossRtd.get());
    }

    result = coil;
  }

  return result;
}

boost::optional<openstudio::model::ModelObject> ReverseTranslator::translateFan(
  const pugi::xml_node& fanElement,
  openstudio::model::Model& model)
{
  boost::optional<model::ModelObject> result;

  if( ! istringEqual(fanElement.name(),"Fan") )
  {
    return result;
  }

  // FanControlMethod
  pugi::xml_node fanControlMethodElement = fanElement.child("CtrlMthdSim");

  // Name
  pugi::xml_node nameElement = fanElement.child("Name");

  // TotEff
  pugi::xml_node totEffElement = fanElement.child("TotEff");

  // MtrfEff
  pugi::xml_node mtrEffElement = fanElement.child("MtrEff");

  // FlowCap
  pugi::xml_node flowCapElement = fanElement.child("FlowCapSim");

  // TotStaticPress
  pugi::xml_node totStaticPressElement = fanElement.child("TotStaticPress");

  // MtrPos
  pugi::xml_node mtrPosElement = fanElement.child("MtrPos");
  bool motorInAirstream = false;
  if( istringEqual(mtrPosElement.text().as_string(),"InAirStream") )
  {
    motorInAirstream = true;
  }

  //AvailSchRef
  pugi::xml_node availSchRefElement = fanElement.child("AvailSchRef");
  std::string availSchRef = escapeName(availSchRefElement.text().as_string());
  auto availSch = model.getModelObjectByName<model::Schedule>(availSchRef);

  if (!availSch) {
    // AvailCtrlByEMS
    const auto availCtrlByEMS = fanElement.child("AvailCtrlByEMS").text().as_int(0);
    
    if(availCtrlByEMS == 1) {
      availSch = model.alwaysOnDiscreteSchedule().clone(model).cast<model::Schedule>();
      availSch->setName(std::string(nameElement.text().as_string()) + " AvailSch");
    }
  }

  boost::optional<double> flowCap;

  if( ! autosize() )
  {
    boost::optional<double> _flowCap = lexicalCastToDouble(flowCapElement);

    if( _flowCap )
    {
      Quantity flowRateIP(_flowCap.get(),createCFMVolumetricFlowrate());
      OptionalQuantity flowRateSI = QuantityConverter::instance().convert(flowRateIP, UnitSystem(UnitSystem::SI));
      OS_ASSERT(flowRateSI);
      flowCap = flowRateSI->value();
    }
  }

  // ConstantVolume
  if( istringEqual(fanControlMethodElement.text().as_string(),"ConstantVolume") ||
      istringEqual(fanControlMethodElement.text().as_string(),"TwoSpeed") )
  {
    // The type of fan is dependent on the context.  We use FanOnOff for cycling zone systems, FanConstantVolume for everything else
    pugi::xml_node parentElement = fanElement.parent();

    if (openstudio::istringEqual(parentElement.name(), "ZnSys"))
    {
      // Type

      auto znSysType = parentElement.child("TypeSim").text().as_string();
      auto znSysFanCtrl = parentElement.child("FanCtrl").text().as_string();
      auto nightCycleFanCtrl = parentElement.child("NightCycleFanCtrl").text().as_string();

      if( istringEqual(znSysFanCtrl,"Cycling") ||
          ( ! istringEqual(nightCycleFanCtrl,"StaysOff") ) ||
          (
            istringEqual(znSysType,"VRF") &&
            istringEqual(znSysFanCtrl,"Continuous") &&
            istringEqual(fanControlMethodElement.text().as_string(),"TwoSpeed")
          )
        )
      {
        model::Schedule schedule = model.alwaysOnDiscreteSchedule();

        model::FanOnOff fan(model,schedule);

        fan.setName(nameElement.text().as_string());

        if( availSch ) {
          fan.setAvailabilitySchedule(availSch.get());
        }

        // TotEff
        boost::optional<double> _totEff = lexicalCastToDouble(totEffElement);
        if( _totEff )
        {
          fan.setFanEfficiency(_totEff.get());
        }

        // MtrEff
        boost::optional<double> _mtrEff = lexicalCastToDouble(mtrEffElement);
        if( _mtrEff )
        {
          fan.setMotorEfficiency(_mtrEff.get());
        }

        // FlowCap
        if( flowCap )
        {
          fan.setMaximumFlowRate(flowCap.get());
        }

        // TotStaticPress
        boost::optional<double> _totStaticPress = lexicalCastToDouble(totStaticPressElement);
        if( _totStaticPress )
        {
          // Convert in WC to Pa
          fan.setPressureRise(_totStaticPress.get() * 249.0889 );
        }

        // MtrPos
        if( motorInAirstream )
        {
          fan.setMotorInAirstreamFraction(1.0);
        }
        else
        {
          fan.setMotorInAirstreamFraction(0.0);
        }

        // Pwr_fPLRCrvRef
        pugi::xml_node pwr_fPLRCrvElement = fanElement.child("Pwr_fPLRCrvRef");
        boost::optional<model::Curve> pwr_fPLRCrv;
        pwr_fPLRCrv = model.getModelObjectByName<model::Curve>(pwr_fPLRCrvElement.text().as_string());
        if( pwr_fPLRCrv )
        {
          fan.setFanPowerRatioFunctionofSpeedRatioCurve(pwr_fPLRCrv.get());
        }

        // End Use Subcategory
        fan.setEndUseSubcategory("Interior Fans");

        result = fan;
      }
    }

    if( ! result )
    {
      model::Schedule schedule = model.alwaysOnDiscreteSchedule();

      model::FanConstantVolume fan(model,schedule);

      fan.setName(nameElement.text().as_string());

      if( availSch ) {
        fan.setAvailabilitySchedule(availSch.get());
      }

      // TotEff
      boost::optional<double> _totEff = lexicalCastToDouble(totEffElement);
      if( _totEff )
      {
        fan.setFanEfficiency(_totEff.get());
      }

      // MtrEff
      boost::optional<double> _mtrEff = lexicalCastToDouble(mtrEffElement);
      if( _mtrEff )
      {
        fan.setMotorEfficiency(_mtrEff.get());
      }

      // FlowCap
      if( flowCap )
      {
        fan.setMaximumFlowRate(flowCap.get());
      }

      // TotStaticPress
      boost::optional<double> _totStaticPress = lexicalCastToDouble(totStaticPressElement);
      if( _totStaticPress )
      {
        // Convert in WC to Pa
        fan.setPressureRise(_totStaticPress.get() * 249.0889 );
      }

      // MtrPos
      if( motorInAirstream )
      {
        fan.setMotorInAirstreamFraction(1.0);
      }
      else
      {
        fan.setMotorInAirstreamFraction(0.0);
      }

      result = fan;
    }
  }
  // Variable Volume
  else if( istringEqual(fanControlMethodElement.text().as_string(),"VariableSpeedDrive") )
  {
    model::Schedule schedule = model.alwaysOnDiscreteSchedule();

    model::FanVariableVolume fan(model,schedule);

    fan.setName(nameElement.text().as_string());

    if( availSch ) {
      fan.setAvailabilitySchedule(availSch.get());
    }

    // TotEff
    boost::optional<double> _totEff = lexicalCastToDouble(totEffElement);
    if( _totEff )
    {
      fan.setFanEfficiency(_totEff.get());
    }

    // MtrEff
    boost::optional<double> _mtrEff = lexicalCastToDouble(mtrEffElement);
    if( _mtrEff )
    {
      fan.setMotorEfficiency(_mtrEff.get());
    }

    // FlowCap
    if( flowCap )
    {
      fan.setMaximumFlowRate(flowCap.get());
    }

    // TotStaticPress
    boost::optional<double> _totStaticPress = lexicalCastToDouble(totStaticPressElement);
    if( _totStaticPress )
    {
      // Convert in WC to Pa
      fan.setPressureRise(_totStaticPress.get() * 249.0889 );
    }

    // Pwr_fPLRCrvRef
    pugi::xml_node pwr_fPLRCrvElement = fanElement.child("Pwr_fPLRCrvRef");
    boost::optional<model::Curve> pwr_fPLRCrv;
    pwr_fPLRCrv = model.getModelObjectByName<model::Curve>(pwr_fPLRCrvElement.text().as_string());
    if( pwr_fPLRCrv )
    {
      if( boost::optional<model::CurveCubic> curveCubic = pwr_fPLRCrv->optionalCast<model::CurveCubic>() )
      {
        fan.setFanPowerCoefficient1(curveCubic->coefficient1Constant());
        fan.setFanPowerCoefficient2(curveCubic->coefficient2x());
        fan.setFanPowerCoefficient3(curveCubic->coefficient3xPOW2());
        fan.setFanPowerCoefficient4(curveCubic->coefficient4xPOW3());
        fan.setFanPowerCoefficient5(0.0);
      }
      else
      {
        LOG(Warn,"Fan: " << fan.name().get() << " references an unsupported curve type.");
      }
    }

    // MtrPos
    if( motorInAirstream )
    {
      fan.setMotorInAirstreamFraction(1.0);
    }
    else
    {
      fan.setMotorInAirstreamFraction(0.0);
    }

    pugi::xml_node flowMinSimElement = fanElement.child("FlowMinSim");
    boost::optional<double> _flowMinSim = lexicalCastToDouble(flowMinSimElement);
    if( _flowMinSim )
    {
      double value = unitToUnit(_flowMinSim.get(),"cfm","m^3/s").get();
      fan.setFanPowerMinimumAirFlowRate(value);
    }

    result = fan;
  }

  return result;
}

boost::optional<openstudio::model::ModelObject> ReverseTranslator::translateHtRcvry(
  const pugi::xml_node& element,
  openstudio::model::Model& model)
{
  if( ! istringEqual(element.name(),"HtRcvry") ) {
    return boost::none;
  }

  model::HeatExchangerAirToAirSensibleAndLatent hx(model);

  auto nameElement = element.child("Name");
  hx.setName(nameElement.text().as_string());

  // AvailSchRef
  auto availSchRefElement = element.child("AvailSchRef");
  auto availSchRef = escapeName(availSchRefElement.text().as_string());
  auto availSch = model.getModelObjectByName<model::Schedule>(availSchRef);
  if( availSch ) {
    hx.setAvailabilitySchedule(availSch.get());
  }

  // SupFlowRtd
  auto supFlowRtdElement = element.child("SupFlowRtdSim");
  boost::optional<double> _supFlowRtd = lexicalCastToDouble(supFlowRtdElement);
  if( _supFlowRtd ) {
    double value = unitToUnit(_supFlowRtd.get(),"cfm","m^3/s").get();
    hx.setNominalSupplyAirFlowRate(value);
  }

  // HtgSensEff100
  auto htgSensEff100Element = element.child("HtgSensEff100");
  boost::optional<double> _htgSensEff100 = lexicalCastToDouble(htgSensEff100Element);
  if( _htgSensEff100 ) {
    hx.setSensibleEffectivenessat100HeatingAirFlow(_htgSensEff100.get());
  }

  // HtgSensEff75
  auto htgSensEff75Element = element.child("HtgSensEff75");
  boost::optional<double> _htgSensEff75 = lexicalCastToDouble(htgSensEff75Element);
  if( _htgSensEff75 ) {
    hx.setSensibleEffectivenessat75HeatingAirFlow(_htgSensEff75.get());
  }

  // HtgLatEff100
  auto htgLatEff100Element = element.child("HtgLatEff100");
  boost::optional<double> _htgLatEff100 = lexicalCastToDouble(htgLatEff100Element);
  if( _htgLatEff100 ) {
    hx.setLatentEffectivenessat100HeatingAirFlow(_htgLatEff100.get());
  }

  // HtgLatEff75
  auto htgLatEff75Element = element.child("HtgLatEff75");
  boost::optional<double> _htgLatEff75 = lexicalCastToDouble(htgLatEff75Element);
  if( _htgLatEff75 ) {
    hx.setLatentEffectivenessat75HeatingAirFlow(_htgLatEff75.get());
  }

  // ClgSensEff100
  auto clgSensEff100Element = element.child("ClgSensEff100");
  boost::optional<double> _clgSensEff100 = lexicalCastToDouble(clgSensEff100Element);
  if( _clgSensEff100 ) {
    hx.setSensibleEffectivenessat100CoolingAirFlow(_clgSensEff100.get());
  }

  // ClgSensEff75
  auto clgSensEff75Element = element.child("ClgSensEff75");
  boost::optional<double> _clgSensEff75 = lexicalCastToDouble(clgSensEff75Element);
  if( _clgSensEff75 ) {
    hx.setSensibleEffectivenessat75CoolingAirFlow(_clgSensEff75.get());
  }

  // ClgLatEff100
  auto clgLatEff100Element = element.child("ClgLatEff100");
  boost::optional<double> _clgLatEff100 = lexicalCastToDouble(clgLatEff100Element);
  if( _clgLatEff100 ) {
    hx.setLatentEffectivenessat100CoolingAirFlow(_clgLatEff100.get());
  }

  // ClgLatEff75
  auto clgLatEff75Element = element.child("ClgLatEff75");
  boost::optional<double> _clgLatEff75 = lexicalCastToDouble(clgLatEff75Element);
  if( _clgLatEff75 ) {
    hx.setLatentEffectivenessat75CoolingAirFlow(_clgLatEff75.get());
  }

  // AuxPwr
  auto auxPwrElement = element.child("AuxPwr");
  boost::optional<double> _auxPwr = lexicalCastToDouble(auxPwrElement);
  if( _auxPwr ) {
    hx.setNominalElectricPower(_auxPwr.get());
  }

  auto type = element.child("Type").text().as_string();
  if( istringEqual(type,"Plate") ) {
    hx.setHeatExchangerType("Plate");
  } else if( istringEqual(type,"Wheel") ) {
    hx.setHeatExchangerType("Rotary");
  }

  auto defrostCtrl = element.child("DefrostCtrl").text().as_string();
  hx.setFrostControlType(defrostCtrl);

  auto defrostCtrlTempElement = element.child("DefrostCtrlTemp");
  boost::optional<double> _defrostCtrlTemp = lexicalCastToDouble(defrostCtrlTempElement);
  if( _defrostCtrlTemp ) {
    double value = unitToUnit(_defrostCtrlTemp.get(),"F","C").get();
    hx.setThresholdTemperature(value);
  }

  // DefrostTimeFrac
  auto defrostTimeFracElement = element.child("DefrostTimeFrac");
  boost::optional<double> _defrostTimeFrac = lexicalCastToDouble(defrostTimeFracElement);
  if( _defrostTimeFrac ) {
    hx.setInitialDefrostTimeFraction(_defrostTimeFrac.get());
  }


  // DefrostTimeFracRt
  auto defrostTimeFracRtElement = element.child("DefrostTimeFracRt");
  boost::optional<double> _defrostTimeFracRt = lexicalCastToDouble(defrostTimeFracRtElement);
  if( _defrostTimeFracRt ) {
    hx.setRateofDefrostTimeFractionIncrease(_defrostTimeFracRt.get());
  }

  // EconoLockout
  auto econoLockout = element.child("EconoLockout").text().as_string();
  if( istringEqual(econoLockout,"No") ) {
    hx.setEconomizerLockout(false);
  } else {
    hx.setEconomizerLockout(true);
  }

  return hx;
}

boost::optional<openstudio::model::ModelObject> ReverseTranslator::translateEvapClr(
  const pugi::xml_node& element,
  openstudio::model::Model& model)
{
  boost::optional<model::ModelObject> result;

  model::Schedule schedule = model.alwaysOnDiscreteSchedule();

  if( ! istringEqual(element.name(),"EvapClr") )
  {
    return result;
  }

  pugi::xml_node typeElement = element.child("Type");
  pugi::xml_node nameElement = element.child("Name");

  if (openstudio::istringEqual(typeElement.text().as_string(), "Direct"))
  {
    model::EvaporativeCoolerDirectResearchSpecial evap(model,schedule);

    evap.setName(nameElement.text().as_string());

    pugi::xml_node effElement = element.child("Eff");
    boost::optional<double> _eff = lexicalCastToDouble(effElement);
    if( _eff )
    {
      evap.setCoolerEffectiveness(_eff.get());
    }

    pugi::xml_node pumpPwrElement = element.child("PumpPwr");
    boost::optional<double> _pumpPwr = lexicalCastToDouble(pumpPwrElement);
    if( _pumpPwr )
    {
      double value = unitToUnit(_pumpPwr.get(),"Btu/h","W").get();
      evap.setRecirculatingWaterPumpPowerConsumption(value);
    }

    evap.setBlowdownConcentrationRatio(3.0);

    result = evap;
  }
  else if (openstudio::istringEqual(typeElement.text().as_string(), "Indirect"))
  {
    model::EvaporativeCoolerIndirectResearchSpecial evap(model);

    evap.setName(nameElement.text().as_string());

    pugi::xml_node effElement = element.child("Eff");
    boost::optional<double> _eff = lexicalCastToDouble(effElement);
    if( _eff )
    {
      evap.setCoolerMaximumEffectiveness(_eff.get());
    }

    pugi::xml_node pumpPwrElement = element.child("PumpPwr");
    boost::optional<double> _pumpPwr = lexicalCastToDouble(pumpPwrElement);
    if( _pumpPwr )
    {
      double value = unitToUnit(_pumpPwr.get(),"Btu/h","W").get();
      evap.setRecirculatingWaterPumpPowerConsumption(value);
    }

    pugi::xml_node secFanFlowCapElement = element.child("SecFanFlowCap");
    boost::optional<double> _secFanFlowCap = lexicalCastToDouble(secFanFlowCapElement);
    if( _secFanFlowCap )
    {
      double value = unitToUnit(_secFanFlowCap.get(),"cfm","m^3/s").get();
      evap.setSecondaryFanFlowRate(value);
    }

    pugi::xml_node secFanTotEffElemenet = element.child("SecFanTotEff");
    boost::optional<double> _secFanTotEff = lexicalCastToDouble(secFanTotEffElemenet);
    if( _secFanTotEff )
    {
      evap.setSecondaryFanTotalEfficiency(_secFanTotEff.get());
    }

    pugi::xml_node secFanTotStaticPressElement = element.child("SecFanTotStaticPress");
    boost::optional<double> _secFanTotStaticPress = lexicalCastToDouble(secFanTotStaticPressElement);
    if( _secFanTotStaticPress )
    {
      // Convert in WC to Pa
      evap.setSecondaryFanDeltaPressure(_secFanTotStaticPress.get() * 249.0889 );
    }

    pugi::xml_node indirectDewPtEffElement = element.child("IndirectDewPtEff");
    boost::optional<double> _indirectDewPtEff = lexicalCastToDouble(indirectDewPtEffElement);
    if( _indirectDewPtEff )
    {
      evap.setDewpointEffectivenessFactor(_indirectDewPtEff.get());
    }
    // TODO: reset dewpoint effectiveness

    evap.setBlowdownConcentrationRatio(3.0);

    // TODO: Unused
    // pugi::xml_node secAirSrcElement = element.child("SecAirSrc");

    evap.resetDriftLossFraction();

    result = evap;
  }
  else
  {
    LOG(Error,nameElement.text().as_string() << " is an unsupported type: defaulting to direct evaporative cooling");
    model::EvaporativeCoolerDirectResearchSpecial evap(model,schedule);
    result = evap;
  }

  return result;
}


boost::optional<openstudio::model::ModelObject> ReverseTranslator::translateCoilCooling(
  const pugi::xml_node& coolingCoilElement,
  openstudio::model::Model& model)
{
  boost::optional<model::ModelObject> result;

  if( ! istringEqual(coolingCoilElement.name(),"CoilClg") )
  {
    return result;
  }

  bool znSys = false;
  if( istringEqual(coolingCoilElement.parent().name(), "ZnSys") )
  {
    znSys = true;
  }

  // Look for a sibling fan to figure out what the flow capacity should be

  pugi::xml_node flowCapElement;

  pugi::xml_node airSegElement = coolingCoilElement.parent();

  if (airSegElement)
  {
    pugi::xml_node fanElement = airSegElement.child("Fan");

    if (fanElement)
    {
      flowCapElement = fanElement.child("FlowCapSim");
    }
  }

  // CapTotRtd
  pugi::xml_node capTotRtdElement = coolingCoilElement.child("CapTotGrossRtdSim");

  // CapSensRtd
  pugi::xml_node capSensRtdElement = coolingCoilElement.child("CapSensGrossRtdSim");

  // Type
  pugi::xml_node coilCoolingTypeElement = coolingCoilElement.child("Type");

  // DirectExpansion
  if( istringEqual(coilCoolingTypeElement.text().as_string(),"DirectExpansion") )
  {
    // Name
    pugi::xml_node nameElement = coolingCoilElement.child("Name");
    std::string coilName = nameElement.text().as_string();

    // AvailCtrlByEMS
    const auto availCtrlByEMS = coolingCoilElement.child("AvailCtrlByEMS").text().as_int(0);
    
    boost::optional<model::Schedule> availabilitySchedule;
    if(availCtrlByEMS == 1) {
      availabilitySchedule = model.alwaysOnDiscreteSchedule().clone(model).cast<model::Schedule>();
      availabilitySchedule->setName(coilName + " AvailSch");
    } else {
      availabilitySchedule = model.alwaysOnDiscreteSchedule();
    }

    // NumClgStages
    pugi::xml_node numClgStagesElement = coolingCoilElement.child("NumClgStages");

    // CndsrType
    pugi::xml_node cndsrTypeElement = coolingCoilElement.child("CndsrType");

    if( istringEqual(cndsrTypeElement.text().as_string(),"WaterSource") )
    {
      boost::optional<model::AirLoopHVACUnitarySystem> unitary;
      model::CoilCoolingWaterToAirHeatPumpEquationFit coil(model);
      coil.setName(nameElement.text().as_string());
      if( znSys )
      {
        result = coil;
      }
      else
      {
        unitary = model::AirLoopHVACUnitarySystem(model);
        unitary->setName(coilName + " Unitary");
        unitary->getImpl<model::detail::AirLoopHVACUnitarySystem_Impl>()->setControlType("SetPoint");
        unitary->setCoolingCoil(coil);
        unitary->setSupplyAirFlowRateMethodWhenNoCoolingorHeatingisRequired("SupplyAirFlowRate");
        unitary->setSupplyAirFlowRateMethodDuringHeatingOperation("SupplyAirFlowRate");
        unitary->setSupplyAirFlowRateMethodDuringCoolingOperation("SupplyAirFlowRate");
        unitary->setAvailabilitySchedule(availabilitySchedule.get());
        result = unitary.get();
      }

      // Plant
      pugi::xml_node fluidSegInRefElement = coolingCoilElement.child("FluidSegInRef");
      if (!fluidSegInRefElement) {
        LOG(Error, "CoilCoolingWaterToAirHeatPumpEquationFit '" << coilName << "' doesn't have a FluidSegInRef element! It will not be connected.");
      } else {
        boost::optional<model::PlantLoop> plant = loopForSupplySegment(fluidSegInRefElement, model);
        if( plant )
        {
          plant->addDemandBranchForComponent(coil);
        }
      }
      if( ! autosize() )
      {
        boost::optional<double> _flowCap = lexicalCastToDouble(flowCapElement);
        if( _flowCap )
        {
          double value = unitToUnit(_flowCap.get(),"cfm","m^3/s").get();
          coil.setRatedAirFlowRate(value);
          if( unitary )
          {
            unitary->setSupplyAirFlowRateDuringCoolingOperation(value);
          }
        }
      }

      if( ! autosize() )
      {
        pugi::xml_node fluidFlowRtDsgnSimElement = coolingCoilElement.child("FluidFlowRtDsgnSim");
        boost::optional<double> _fluidFlowRtDsgnSim = lexicalCastToDouble(fluidFlowRtDsgnSimElement);
        if( _fluidFlowRtDsgnSim )
        {
          double value = unitToUnit(_fluidFlowRtDsgnSim.get(),"gal/min","m^3/s").get();
          coil.setRatedWaterFlowRate(value);
        }
      }

      if( ! autosize() )
      {
        pugi::xml_node capTotGrossRtdSimElement = coolingCoilElement.child("CapTotGrossRtdSim");
        boost::optional<double> _capTotGrossRtdSim = lexicalCastToDouble(capTotGrossRtdSimElement);
        if( _capTotGrossRtdSim )
        {
          double value = unitToUnit(_capTotGrossRtdSim.get(),"Btu/h","W").get();
          coil.setRatedTotalCoolingCapacity(value);
        }
      }

      if( ! autosize() )
      {
        pugi::xml_node capSensGrossRtdSimElement = coolingCoilElement.child("CapSensGrossRtdSim");
        boost::optional<double> _capSensGrossRtdSim = lexicalCastToDouble(capSensGrossRtdSimElement);
        if( _capSensGrossRtdSim )
        {
          double value = unitToUnit(_capSensGrossRtdSim.get(),"Btu/h","W").get();
          coil.setRatedSensibleCoolingCapacity(value);
        }
      }

      pugi::xml_node dxEIRElement = coolingCoilElement.child("DXEIR");
      boost::optional<double> _dxEIR = lexicalCastToDouble(dxEIRElement);
      if( _dxEIR )
      {
        coil.setRatedCoolingCoefficientofPerformance( 1.0 / _dxEIR.get() );
      }

      coil.setTotalCoolingCapacityCoefficient1(-9.149069561);
      coil.setTotalCoolingCapacityCoefficient2(10.87814026);
      coil.setTotalCoolingCapacityCoefficient3(-1.718780157);
      coil.setTotalCoolingCapacityCoefficient4(0.746414818);
      coil.setTotalCoolingCapacityCoefficient5(0.0);

      coil.setSensibleCoolingCapacityCoefficient1(-5.462690012);
      coil.setSensibleCoolingCapacityCoefficient2(17.95968138);
      coil.setSensibleCoolingCapacityCoefficient3(-11.87818402);
      coil.setSensibleCoolingCapacityCoefficient4(-0.980163419);
      coil.setSensibleCoolingCapacityCoefficient5(0.767285761);
      coil.setSensibleCoolingCapacityCoefficient6(0.0);

      coil.setCoolingPowerConsumptionCoefficient1(-3.20456384);
      coil.setCoolingPowerConsumptionCoefficient2(-0.976409399);
      coil.setCoolingPowerConsumptionCoefficient3(3.97892546);
      coil.setCoolingPowerConsumptionCoefficient4(0.938181818);
      coil.setCoolingPowerConsumptionCoefficient5(-0.0);

      coil.setNominalTimeforCondensateRemovaltoBegin(0.0);
      coil.setRatioofInitialMoistureEvaporationRateandSteadyStateLatentCapacity(0.0);

    }
    // Unspecified=assume 1, or specified as 1 stage
    else if( !numClgStagesElement || (numClgStagesElement.text().as_int() == 1) )
    {
      // Cap_fTempCrvRef

      boost::optional<model::Curve> coolingCurveFofTemp;
      pugi::xml_node cap_fTempCrvRefElement = coolingCoilElement.child("Cap_fTempCrvRef");
      coolingCurveFofTemp = model.getModelObjectByName<model::Curve>(cap_fTempCrvRefElement.text().as_string());
      if( ! coolingCurveFofTemp )
      {
        LOG(Error,"Coil: " << nameElement.text().as_string() << "Broken Cap_fTempCrvRef");

        // Provide default curves
        model::CurveBiquadratic _coolingCurveFofTemp(model);
        _coolingCurveFofTemp.setCoefficient1Constant(0.942587793);
        _coolingCurveFofTemp.setCoefficient2x(0.009543347);
        _coolingCurveFofTemp.setCoefficient3xPOW2(0.000683770);
        _coolingCurveFofTemp.setCoefficient4y(-0.011042676);
        _coolingCurveFofTemp.setCoefficient5yPOW2(0.000005249);
        _coolingCurveFofTemp.setCoefficient6xTIMESY(-0.000009720);
        _coolingCurveFofTemp.setMinimumValueofx(17.0);
        _coolingCurveFofTemp.setMaximumValueofx(22.0);
        _coolingCurveFofTemp.setMinimumValueofy(13.0);
        _coolingCurveFofTemp.setMaximumValueofy(46.0);
        coolingCurveFofTemp = _coolingCurveFofTemp;
      }

      // Cap_fFlowCrvRef

      boost::optional<model::Curve> coolingCurveFofFlow;
      pugi::xml_node cap_fFlowCrvRefElement = coolingCoilElement.child("Cap_fFlowCrvRef");
      coolingCurveFofFlow = model.getModelObjectByName<model::Curve>(cap_fFlowCrvRefElement.text().as_string());
      if( ! coolingCurveFofFlow )
      {
        LOG(Error,"Coil: " << nameElement.text().as_string() << "Broken Cap_fFlowCrvRef");

        // Provide default curves
        model::CurveQuadratic _coolingCurveFofFlow(model);
        _coolingCurveFofFlow.setCoefficient1Constant(0.8);
        _coolingCurveFofFlow.setCoefficient2x(0.2);
        _coolingCurveFofFlow.setCoefficient3xPOW2(0.0);
        _coolingCurveFofFlow.setMinimumValueofx(0.5);
        _coolingCurveFofFlow.setMaximumValueofx(1.5);
        coolingCurveFofFlow = _coolingCurveFofFlow;
      }


      // DXEIR_fTempCrvRef

      boost::optional<model::Curve> energyInputRatioFofTemp;
      pugi::xml_node dxEIR_fTempCrvRefElement = coolingCoilElement.child("DXEIR_fTempCrvRef");
      energyInputRatioFofTemp = model.getModelObjectByName<model::Curve>(dxEIR_fTempCrvRefElement.text().as_string());
      if( ! energyInputRatioFofTemp )
      {
        LOG(Error,"Coil: " << nameElement.text().as_string() << "Broken DXEIR_fTempCrvRef");

        // Provide default curves
        model::CurveBiquadratic _energyInputRatioFofTemp(model);
        _energyInputRatioFofTemp.setCoefficient1Constant(0.342414409);
        _energyInputRatioFofTemp.setCoefficient2x(0.034885008);
        _energyInputRatioFofTemp.setCoefficient3xPOW2(-0.000623700);
        _energyInputRatioFofTemp.setCoefficient4y(0.004977216);
        _energyInputRatioFofTemp.setCoefficient5yPOW2(0.000437951);
        _energyInputRatioFofTemp.setCoefficient6xTIMESY(-0.000728028);
        _energyInputRatioFofTemp.setMinimumValueofx(17.0);
        _energyInputRatioFofTemp.setMaximumValueofx(22.0);
        _energyInputRatioFofTemp.setMinimumValueofy(13.0);
        _energyInputRatioFofTemp.setMaximumValueofy(46.0);
        energyInputRatioFofTemp = _energyInputRatioFofTemp;
      }

      // DX EIR function of Flow

      boost::optional<model::Curve> energyInputRatioFofFlow;
      pugi::xml_node dxEIR_fFlowCrvRefElement = coolingCoilElement.child("DXEIR_fFlowCrvRef");
      energyInputRatioFofFlow = model.getModelObjectByName<model::Curve>(dxEIR_fFlowCrvRefElement.text().as_string());
      if( ! energyInputRatioFofFlow )
      {
        model::CurveQuadratic _energyInputRatioFofFlow(model);
        _energyInputRatioFofFlow.setCoefficient1Constant(1.20550);
        _energyInputRatioFofFlow.setCoefficient2x(-0.32953);
        _energyInputRatioFofFlow.setCoefficient3xPOW2(0.12308);
        _energyInputRatioFofFlow.setMinimumValueofx(0.75918);
        _energyInputRatioFofFlow.setMaximumValueofx(1.13877);
        energyInputRatioFofFlow = _energyInputRatioFofFlow;
      }

      // DXEIR_fPLRCrvRef

      boost::optional<model::Curve> partLoadFraction;
      pugi::xml_node dxEIR_fPLFCrvRefElement = coolingCoilElement.child("DXEIR_fPLFCrvRef");
      partLoadFraction = model.getModelObjectByName<model::Curve>(dxEIR_fPLFCrvRefElement.text().as_string());
      if( ! partLoadFraction )
      {
        LOG(Error,"Coil: " << nameElement.text().as_string() << "Broken DXEIR_fPLFCrvRef");

        // Provide default curves
        model::CurveQuadratic _partLoadFraction(model);
        _partLoadFraction.setCoefficient1Constant(0.85);
        _partLoadFraction.setCoefficient2x(0.15);
        _partLoadFraction.setCoefficient3xPOW2(0.0);
        _partLoadFraction.setMinimumValueofx(0.0);
        _partLoadFraction.setMaximumValueofx(1.0);
        partLoadFraction = _partLoadFraction;
      }

      model::CoilCoolingDXSingleSpeed coilCooling( model,
                                                   availabilitySchedule.get(),
                                                   coolingCurveFofTemp.get(),
                                                   coolingCurveFofFlow.get(),
                                                   energyInputRatioFofTemp.get(),
                                                   energyInputRatioFofFlow.get(),
                                                   partLoadFraction.get() );

      coilCooling.setName(nameElement.text().as_string());

      // FlowCap
      if( ! autosize() )
      {
        boost::optional<double> _flowCap = lexicalCastToDouble(flowCapElement);

        if( _flowCap )
        {
          Quantity flowRateIP(_flowCap.get(),createCFMVolumetricFlowrate());
          OptionalQuantity flowRateSI = QuantityConverter::instance().convert(flowRateIP, UnitSystem(UnitSystem::SI));
          OS_ASSERT(flowRateSI);
          coilCooling.setRatedAirFlowRate(flowRateSI->value());
        }
      }

      boost::optional<double> totalCapacity;
      boost::optional<double> sensibleCapacity;

      // CapTotRtd
      if( ! autosize() )
      {
        boost::optional<double> _capTotRtd = lexicalCastToDouble(capTotRtdElement);

        if( _capTotRtd )
        {
          Quantity valueIP(_capTotRtd.get(),createBTUPower());
          OptionalQuantity valueSI = QuantityConverter::instance().convert(valueIP, UnitSystem(UnitSystem::SI));
          OS_ASSERT(valueSI);
          coilCooling.setRatedTotalCoolingCapacity(valueSI->value());
          totalCapacity = _capTotRtd.get();
        }
      }

      // CapSensRtd
      if( ! autosize() )
      {
        boost::optional<double> _capSensRtd = lexicalCastToDouble(capSensRtdElement);

        if( _capSensRtd )
        {
          sensibleCapacity = _capSensRtd.get();
        }
      }

      if( totalCapacity && sensibleCapacity )
      {
        coilCooling.setRatedSensibleHeatRatio( sensibleCapacity.get() / totalCapacity.get() );
      }

      // DXEIR
      pugi::xml_node dxEIRElement = coolingCoilElement.child("DXEIR");
      boost::optional<double> _dxEIR = lexicalCastToDouble(dxEIRElement);
      if( _dxEIR )
      {
        coilCooling.setRatedCOP( 1.0 / _dxEIR.get());
      }

      // DXCrankcaseHtCap
      pugi::xml_node dxCrankcaseHtCapElement = coolingCoilElement.child("DXCrankcaseHtrCapSim");
      boost::optional<double> _dxCrankcaseHtCap = lexicalCastToDouble(dxCrankcaseHtCapElement);
      if( _dxCrankcaseHtCap )
      {
        double value = unitToUnit(_dxCrankcaseHtCap.get(),"Btu/h","W").get();
        coilCooling.setCrankcaseHeaterCapacity(value);
      }

      // DXCrankcaseCtrlTemp
      pugi::xml_node dxCrankcaseCtrlTempElement = coolingCoilElement.child("DXCrankcaseCtrlTemp");
      boost::optional<double> _dxCrankcaseCtrlTemp = lexicalCastToDouble(dxCrankcaseCtrlTempElement);
      if( _dxCrankcaseCtrlTemp )
      {
        double value = unitToUnit(_dxCrankcaseCtrlTemp.get(),"F","C").get();
        coilCooling.setMaximumOutdoorDryBulbTemperatureForCrankcaseHeaterOperation(value);
      }

      result = coilCooling;
    }
    else if( numClgStagesElement.text().as_int() == 2 )
    {
      // Cap_fTempCrvRef

      boost::optional<model::Curve> coolingCurveFofTemp;
      pugi::xml_node cap_fTempCrvRefElement = coolingCoilElement.child("Cap_fTempCrvRef");
      coolingCurveFofTemp = model.getModelObjectByName<model::Curve>(cap_fTempCrvRefElement.text().as_string());
      if( ! coolingCurveFofTemp )
      {
        LOG(Error,"Coil: " << nameElement.text().as_string() << "Broken Cap_fTempCrvRef");

        // Provide default curves
        model::CurveBiquadratic _coolingCurveFofTemp(model);
        _coolingCurveFofTemp.setCoefficient1Constant(0.942587793);
        _coolingCurveFofTemp.setCoefficient2x(0.009543347);
        _coolingCurveFofTemp.setCoefficient3xPOW2(0.000683770);
        _coolingCurveFofTemp.setCoefficient4y(-0.011042676);
        _coolingCurveFofTemp.setCoefficient5yPOW2(0.000005249);
        _coolingCurveFofTemp.setCoefficient6xTIMESY(-0.000009720);
        _coolingCurveFofTemp.setMinimumValueofx(17.0);
        _coolingCurveFofTemp.setMaximumValueofx(22.0);
        _coolingCurveFofTemp.setMinimumValueofy(13.0);
        _coolingCurveFofTemp.setMaximumValueofy(46.0);
        coolingCurveFofTemp = _coolingCurveFofTemp;
      }

      // Cap_fFlowCrvRef

      boost::optional<model::Curve> coolingCurveFofFlow;
      pugi::xml_node cap_fFlowCrvRefElement = coolingCoilElement.child("Cap_fFlowCrvRef");
      coolingCurveFofFlow = model.getModelObjectByName<model::Curve>(cap_fFlowCrvRefElement.text().as_string());
      if( ! coolingCurveFofFlow )
      {
        LOG(Error,"Coil: " << nameElement.text().as_string() << "Broken Cap_fFlowCrvRef");

        // Provide default curves
        model::CurveQuadratic _coolingCurveFofFlow(model);
        _coolingCurveFofFlow.setCoefficient1Constant(0.8);
        _coolingCurveFofFlow.setCoefficient2x(0.2);
        _coolingCurveFofFlow.setCoefficient3xPOW2(0.0);
        _coolingCurveFofFlow.setMinimumValueofx(0.5);
        _coolingCurveFofFlow.setMaximumValueofx(1.5);
        coolingCurveFofFlow = _coolingCurveFofFlow;
      }


      // DXEIR_fTempCrvRef

      boost::optional<model::Curve> energyInputRatioFofTemp;
      pugi::xml_node dxEIR_fTempCrvRefElement = coolingCoilElement.child("DXEIR_fTempCrvRef");
      energyInputRatioFofTemp = model.getModelObjectByName<model::Curve>(dxEIR_fTempCrvRefElement.text().as_string());
      if( ! energyInputRatioFofTemp )
      {
        LOG(Error,"Coil: " << nameElement.text().as_string() << "Broken DXEIR_fTempCrvRef");

        // Provide default curves
        model::CurveBiquadratic _energyInputRatioFofTemp(model);
        _energyInputRatioFofTemp.setCoefficient1Constant(0.342414409);
        _energyInputRatioFofTemp.setCoefficient2x(0.034885008);
        _energyInputRatioFofTemp.setCoefficient3xPOW2(-0.000623700);
        _energyInputRatioFofTemp.setCoefficient4y(0.004977216);
        _energyInputRatioFofTemp.setCoefficient5yPOW2(0.000437951);
        _energyInputRatioFofTemp.setCoefficient6xTIMESY(-0.000728028);
        _energyInputRatioFofTemp.setMinimumValueofx(17.0);
        _energyInputRatioFofTemp.setMaximumValueofx(22.0);
        _energyInputRatioFofTemp.setMinimumValueofy(13.0);
        _energyInputRatioFofTemp.setMaximumValueofy(46.0);
        energyInputRatioFofTemp = _energyInputRatioFofTemp;
      }

      // DX EIR function of Flow

      boost::optional<model::Curve> energyInputRatioFofFlow;
      pugi::xml_node dxEIR_fFlowCrvRefElement = coolingCoilElement.child("DXEIR_fFlowCrvRef");
      energyInputRatioFofFlow = model.getModelObjectByName<model::Curve>(dxEIR_fFlowCrvRefElement.text().as_string());
      if( ! energyInputRatioFofFlow )
      {
        model::CurveQuadratic _energyInputRatioFofFlow(model);
        _energyInputRatioFofFlow.setCoefficient1Constant(1.20550);
        _energyInputRatioFofFlow.setCoefficient2x(-0.32953);
        _energyInputRatioFofFlow.setCoefficient3xPOW2(0.12308);
        _energyInputRatioFofFlow.setMinimumValueofx(0.75918);
        _energyInputRatioFofFlow.setMaximumValueofx(1.13877);
        energyInputRatioFofFlow = _energyInputRatioFofFlow;
      }

      // DXEIR_fPLRCrvRef

      boost::optional<model::Curve> partLoadFraction;
      pugi::xml_node dxEIR_fPLFCrvRefElement = coolingCoilElement.child("DXEIR_fPLFCrvRef");
      partLoadFraction = model.getModelObjectByName<model::Curve>(dxEIR_fPLFCrvRefElement.text().as_string());
      if( ! partLoadFraction )
      {
        LOG(Error,"Coil: " << nameElement.text().as_string() << "Broken DXEIR_fPLFCrvRef");

        // Provide default curves
        model::CurveQuadratic _partLoadFraction(model);
        _partLoadFraction.setCoefficient1Constant(0.85);
        _partLoadFraction.setCoefficient2x(0.15);
        _partLoadFraction.setCoefficient3xPOW2(0.0);
        _partLoadFraction.setMinimumValueofx(0.0);
        _partLoadFraction.setMaximumValueofx(1.0);
        partLoadFraction = _partLoadFraction;
      }

      model::CoilCoolingDXTwoSpeed coilCooling( model,
                                              availabilitySchedule.get(),
                                              coolingCurveFofTemp.get(),
                                              coolingCurveFofFlow.get(),
                                              energyInputRatioFofTemp.get(),
                                              energyInputRatioFofFlow.get(),
                                              partLoadFraction.get(),
                                              coolingCurveFofTemp->clone(model).cast<model::Curve>(),
                                              energyInputRatioFofTemp->clone(model).cast<model::Curve>());

      coilCooling.setName(nameElement.text().as_string());

      boost::optional<double> totalCapacity;
      boost::optional<double> sensibleCapacity;
      boost::optional<double> lowSpeedCapacityFraction;

      // CapTotRtdStageFrac
      for (const pugi::xml_node &capTotRtdStageFracElement : coolingCoilElement.children("CapTotRtdStageFrac")) {
        std::string indexAttribute = capTotRtdStageFracElement.attribute("index").value();
        if( istringEqual(indexAttribute, "0") )
        {
          lowSpeedCapacityFraction = lexicalCastToDouble(capTotRtdStageFracElement);
          break;
        }
      }

      // FlowCap
      if( ! autosize() )
      {
        boost::optional<double> _flowCap = lexicalCastToDouble(flowCapElement);

        if( _flowCap )
        {
          Quantity flowRateIP(_flowCap.get(),createCFMVolumetricFlowrate());
          OptionalQuantity flowRateSI = QuantityConverter::instance().convert(flowRateIP, UnitSystem(UnitSystem::SI));
          OS_ASSERT(flowRateSI);
          coilCooling.setRatedHighSpeedAirFlowRate(flowRateSI->value());
          if( lowSpeedCapacityFraction )
          {
            coilCooling.setRatedLowSpeedAirFlowRate(flowRateSI->value() * lowSpeedCapacityFraction.get());
          }
        }
      }

      // CapTotRtd
      if( ! autosize() )
      {
        boost::optional<double> _capTotRtd = lexicalCastToDouble(capTotRtdElement);

        if( _capTotRtd )
        {
          Quantity valueIP(_capTotRtd.get(),createBTUPower());
          OptionalQuantity valueSI = QuantityConverter::instance().convert(valueIP, UnitSystem(UnitSystem::SI));
          OS_ASSERT(valueSI);
          coilCooling.setRatedHighSpeedTotalCoolingCapacity(valueSI->value());
          if( lowSpeedCapacityFraction )
          {
            coilCooling.setRatedLowSpeedTotalCoolingCapacity(valueSI->value() * lowSpeedCapacityFraction.get());
          }

          totalCapacity = _capTotRtd.get();
        }
      }

      // CapSensRtd
      if( ! autosize() )
      {
        boost::optional<double> _capSensRtd = lexicalCastToDouble(capSensRtdElement);

        if( _capSensRtd )
        {
          sensibleCapacity = _capSensRtd;
        }
      }

      if( totalCapacity && sensibleCapacity )
      {
        coilCooling.setRatedHighSpeedSensibleHeatRatio( sensibleCapacity.get() / totalCapacity.get() );

        // CapSensRtdStageFrac
        if( lowSpeedCapacityFraction )
        {
          for (const pugi::xml_node& capSensRtdStageFracElement: coolingCoilElement.children("CapSensRtdStageFrac")) {
            std::string indexAttribute = capSensRtdStageFracElement.attribute("index").value();
            if( istringEqual(indexAttribute,"0") )
            {
              boost::optional<double> _fraction = lexicalCastToDouble(capSensRtdStageFracElement);
              if( _fraction )
              {
                double lowSHR = (sensibleCapacity.get() * _fraction.get()) / (totalCapacity.get() * lowSpeedCapacityFraction.get());
                coilCooling.setRatedLowSpeedSensibleHeatRatio(lowSHR);
              }
              break;
            }
          }
        }
      }

      // DXEIR

      pugi::xml_node dxEIRElement = coolingCoilElement.child("DXEIR");

      boost::optional<double> _dxEIR = lexicalCastToDouble(dxEIRElement);

      if( _dxEIR )
      {
        double cop = 1.0 / _dxEIR.get();
        coilCooling.setRatedHighSpeedCOP(cop);
        coilCooling.setRatedLowSpeedCOP(cop);
      }

      result = coilCooling;
    }
    else if( lexicalCastToInt(numClgStagesElement) > 2 )
    {
      LOG(Error, "DX cooling coils with more than two stages are not yet supported. See coil named " << coilName);
    }
  }
  // ChilledWater
  if( istringEqual(coilCoolingTypeElement.text().as_string(),"ChilledWater") )
  {

    // Name
    pugi::xml_node nameElement = coolingCoilElement.child("Name");
    std::string coilName = nameElement.text().as_string();

    // AvailCtrlByEMS
    const auto availCtrlByEMS = coolingCoilElement.child("AvailCtrlByEMS").text().as_int(0);
    
    boost::optional<model::Schedule> availabilitySchedule;
    if(availCtrlByEMS == 1) {
      availabilitySchedule = model.alwaysOnDiscreteSchedule().clone(model).cast<model::Schedule>();
      availabilitySchedule->setName(coilName + " AvailSch");
    } else {
      availabilitySchedule = model.alwaysOnDiscreteSchedule();
    }

    model::CoilCoolingWater coilCooling(model,availabilitySchedule.get());

    coilCooling.setName(coilName);

    coilCooling.setTypeOfAnalysis("DetailedAnalysis");

    // Plant
    boost::optional<model::PlantLoop> plant;

    pugi::xml_node fluidSegInRefElement = coolingCoilElement.child("FluidSegInRef");
    if (!fluidSegInRefElement) {
      LOG(Error, "CoilCoolingWater '" << coilName << "' doesn't have a FluidSegInRef element! It will not be connected.");
    } else {
      plant = loopForSupplySegment(fluidSegInRefElement, model);

      if( plant )
      {
        plant->addDemandBranchForComponent(coilCooling);
      }
    }

    coilCooling.autosizeDesignInletAirTemperature();
    coilCooling.autosizeDesignOutletAirTemperature();
    coilCooling.autosizeDesignInletAirHumidityRatio();
    coilCooling.autosizeDesignOutletAirHumidityRatio();

    if( ! autosize() )
    {
      // Design Water Volum Flow Rate of Coil
      auto _fluidFlowRtDsgn = lexicalCastToDouble(coolingCoilElement.child("FluidFlowRtDsgnSim"));
      if( _fluidFlowRtDsgn ) {
        _fluidFlowRtDsgn = unitToUnit(_fluidFlowRtDsgn.get(), "gal/min", "m^3/s");
        coilCooling.setDesignWaterFlowRate(_fluidFlowRtDsgn.get());
      }

      // Design defaults

      coilCooling.setDesignInletWaterTemperature(7.22); // From Sizing Plant

      if( plant )
      {
        coilCooling.setDesignInletWaterTemperature(plant->sizingPlant().designLoopExitTemperature());
      }

      // FlowCap
      boost::optional<double> _flowCap = lexicalCastToDouble(flowCapElement);
      if (_flowCap) {
        Quantity flowRateIP(_flowCap.get(),createCFMVolumetricFlowrate());
        OptionalQuantity flowRateSI = QuantityConverter::instance().convert(flowRateIP, UnitSystem(UnitSystem::SI));
        OS_ASSERT(flowRateSI);
        coilCooling.setDesignAirFlowRate(flowRateSI->value());
      }
    }

    result = coilCooling;
  }

  return result;
}

boost::optional<openstudio::model::ModelObject> ReverseTranslator::translateThermalZone(
  const pugi::xml_node& thermalZoneElement,
  openstudio::model::Model& model )
{
  UnitSystem siSys(UnitSystem::SI);
  UnitSystem whSys(UnitSystem::Wh);

  boost::optional<openstudio::model::ModelObject> result;

  if( ! istringEqual(thermalZoneElement.name(),"ThrmlZn") )
  {
    return result;
  }

  boost::optional<model::ThermalZone> optionalThermalZone;
  boost::optional<model::ThermostatSetpointDualSetpoint> optionalThermostat;

  // Name
  pugi::xml_node nameElement = thermalZoneElement.child("Name");
  std::string name = nameElement.text().as_string();
  optionalThermalZone = model.getModelObjectByName<model::ThermalZone>(name);

  if( ! optionalThermalZone )
  {
    return result;
  }

  model::ThermalZone thermalZone = optionalThermalZone.get();
  result = thermalZone;

  // Type
  pugi::xml_node typeElement = thermalZoneElement.child("Type");
  if (typeElement) {
    if (istringEqual(typeElement.text().as_string(), "Conditioned")) {
      optionalThermostat = model::ThermostatSetpointDualSetpoint(model);
      optionalThermostat->setName(name + " Thermostat");
      thermalZone.setThermostatSetpointDualSetpoint(*optionalThermostat);
    } else if (istringEqual(typeElement.text().as_string(), "Unconditioned")) {
      // no thermostat
    } else if (istringEqual(typeElement.text().as_string(), "Plenum")) {
      // no thermostat
    } else {
      LOG(Error, "Unknown thermal zone type '" << typeElement.text().as_string() << "'");
    }
  }

  // Volume
  pugi::xml_node volElement = thermalZoneElement.child("VolSim");
  boost::optional<double> _vol = lexicalCastToDouble(volElement);
  if (_vol) {
    // sdd units = ft^3, os units = m^3
    Quantity thermalZoneVolumeIP(_vol.get(), BTUUnit(BTUExpnt(0,3,0,0)));
    OptionalQuantity thermalZoneVolumeSI = QuantityConverter::instance().convert(thermalZoneVolumeIP, UnitSystem(UnitSystem::Wh));
    OS_ASSERT(thermalZoneVolumeSI);
    OS_ASSERT(thermalZoneVolumeSI->units() == WhUnit(WhExpnt(0,0,3,0)));
    thermalZone.setVolume(thermalZoneVolumeSI->value());
  }

  // Sizing

  double clgDsgnSupAirTemp = 14.0;
  double clgDsgnSizingFac = 1.0;
  double htgDsgnSupAirTemp = 40.0;
  double htgDsgnSizingFac = 1.0;

  model::SizingZone sizingZone = thermalZone.sizingZone();

  {
    pugi::xml_node clgDsgnSupAirTempElement = thermalZoneElement.child("ClgDsgnSupAirTemp");
    boost::optional<double> _clgDsgnSupAirTemp = lexicalCastToDouble(clgDsgnSupAirTempElement);
    if( _clgDsgnSupAirTemp )
    {
      double value = unitToUnit(_clgDsgnSupAirTemp.get(),"F","C").get();
      sizingZone.setZoneCoolingDesignSupplyAirTemperature(value);
      sizingZone.setZoneCoolingDesignSupplyAirTemperatureInputMethod("SupplyAirTemperature");
    }
  }

  {
    auto clgDsgnSupAirTempDiffElement = thermalZoneElement.child("ClgDsgnSupAirTempDiff");
    boost::optional<double> _clgDsgnSupAirTempDiff = lexicalCastToDouble(clgDsgnSupAirTempDiffElement);
    if( _clgDsgnSupAirTempDiff )
    {
      double value = _clgDsgnSupAirTempDiff.get() * 5.0 / 9.0; // delta F to C
      sizingZone.setZoneCoolingDesignSupplyAirTemperatureDifference(value);
      sizingZone.setZoneCoolingDesignSupplyAirTemperatureInputMethod("TemperatureDifference");
    }
  }

  {
    pugi::xml_node clgDsgnSizingFacElement = thermalZoneElement.child("ClgDsgnSizingFac");
    boost::optional<double> _clgDsgnSizingFac = lexicalCastToDouble(clgDsgnSizingFacElement);
    if( _clgDsgnSizingFac )
    {
      clgDsgnSizingFac = _clgDsgnSizingFac.get();
    }
  }

  {
    pugi::xml_node htgDsgnSupAirTempElement = thermalZoneElement.child("HtgDsgnSupAirTemp");
    boost::optional<double> _htgDsgnSupAirTemp = lexicalCastToDouble(htgDsgnSupAirTempElement);
    if( _htgDsgnSupAirTemp )
    {
      double value = unitToUnit(_htgDsgnSupAirTemp.get(),"F","C").get();
      sizingZone.setZoneHeatingDesignSupplyAirTemperature(value);
      sizingZone.setZoneHeatingDesignSupplyAirTemperatureInputMethod("SupplyAirTemperature");
    }
  }

  {
    auto htgDsgnSupAirTempDiffElement = thermalZoneElement.child("HtgDsgnSupAirTempDiff");
    boost::optional<double> _htgDsgnSupAirTempDiff = lexicalCastToDouble(htgDsgnSupAirTempDiffElement);
    if( _htgDsgnSupAirTempDiff )
    {
      double value = _htgDsgnSupAirTempDiff.get() * 5.0 / 9.0; // delta F to C
      sizingZone.setZoneHeatingDesignSupplyAirTemperatureDifference(value);
      sizingZone.setZoneHeatingDesignSupplyAirTemperatureInputMethod("TemperatureDifference");
    }
  }

  {
    pugi::xml_node htgDsgnSizingFacElement = thermalZoneElement.child("HtgDsgnSizingFac");
    boost::optional<double> _htgDsgnSizingFac = lexicalCastToDouble(htgDsgnSizingFacElement);
    if( _htgDsgnSizingFac )
    {
      htgDsgnSizingFac = _htgDsgnSizingFac.get();
    }
  }

  sizingZone.setZoneCoolingDesignSupplyAirTemperature(clgDsgnSupAirTemp);
  sizingZone.setZoneHeatingDesignSupplyAirTemperature(htgDsgnSupAirTemp);
  sizingZone.setZoneCoolingSizingFactor(clgDsgnSizingFac);
  sizingZone.setZoneHeatingSizingFactor(htgDsgnSizingFac);

  sizingZone.setCoolingMinimumAirFlowperZoneFloorArea(0.0);
  sizingZone.setCoolingMinimumAirFlow(0.0);
  sizingZone.setCoolingMinimumAirFlowFraction(0.0);
  sizingZone.setCoolingDesignAirFlowMethod("DesignDay");

  sizingZone.setHeatingMaximumAirFlowperZoneFloorArea(0.0);
  sizingZone.setHeatingMaximumAirFlow(0.0);
  sizingZone.setHeatingMaximumAirFlowFraction(0.0);
  sizingZone.setHeatingDesignAirFlowMethod("DesignDay");

  pugi::xml_node htgDsgnMaxFlowFracElement = thermalZoneElement.child("HtgDsgnMaxFlowFrac");
  boost::optional<double> _htgDsgnMaxFlowFrac = lexicalCastToDouble(htgDsgnMaxFlowFracElement);
  if( _htgDsgnMaxFlowFrac && _htgDsgnMaxFlowFrac.get() > 0.0 )
  {
    sizingZone.setHeatingDesignAirFlowMethod("DesignDayWithLimit");
    sizingZone.setHeatingMaximumAirFlowFraction(_htgDsgnMaxFlowFrac.get());
  }

  auto intValue = lexicalCastToInt(thermalZoneElement.child("SizeForDOAS"));
  if (intValue) {
    if (intValue.get() == 0) {
      sizingZone.setAccountforDedicatedOutdoorAirSystem(false);
    } else {
      sizingZone.setAccountforDedicatedOutdoorAirSystem(true);
    }
  }

  auto sizeForDOASCtrl = thermalZoneElement.child("SizeForDOASCtrl").text().as_string();

  // Enumerations for doas sizing
  //
  // CBECC
  // Neutral
  // NeutralDehumidified
  // Cold

  // OS
  // \key NeutralSupplyAir
  // \key NeutralDehumidifiedSupplyAir
  // \key ColdSupplyAir

  // EP
  // \key NeutralSupplyAir
  // \key NeutralDehumidifiedSupplyAir
  // \key ColdSupplyAir
  // \default NeutralSupplyAir

  if (std::string(sizeForDOASCtrl).empty()) {
    sizingZone.setDedicatedOutdoorAirSystemControlStrategy("NeutralSupplyAir");
  } else if (istringEqual(sizeForDOASCtrl, "Neutral")) {
    sizingZone.setAccountforDedicatedOutdoorAirSystem(true);
    sizingZone.setDedicatedOutdoorAirSystemControlStrategy("NeutralSupplyAir");
  } else if (istringEqual(sizeForDOASCtrl, "NeutralDehumidified")) {
    sizingZone.setAccountforDedicatedOutdoorAirSystem(true);
    sizingZone.setDedicatedOutdoorAirSystemControlStrategy("NeutralDehumidifiedSupplyAir");
  } else if (istringEqual(sizeForDOASCtrl, "Cold")) {
    sizingZone.setAccountforDedicatedOutdoorAirSystem(true);
    sizingZone.setDedicatedOutdoorAirSystemControlStrategy("ColdSupplyAir");
  }

  auto sizeForDOASTempLow = lexicalCastToDouble(thermalZoneElement.child("SizeForDOASTempLow"));
  if (sizeForDOASTempLow) {
    sizingZone.setDedicatedOutdoorAirLowSetpointTemperatureforDesign(unitToUnit(sizeForDOASTempLow.get(),"F","C").get());
  }

  auto sizeForDOASTempHi = lexicalCastToDouble(thermalZoneElement.child("SizeForDOASTempHi"));
  if (sizeForDOASTempHi) {
    sizingZone.setDedicatedOutdoorAirHighSetpointTemperatureforDesign(unitToUnit(sizeForDOASTempHi.get(),"F","C").get());
  }

  // ClgTstatSchRef
  boost::optional<model::Schedule> clgTstatSch;
  auto clgTstatSchRefElement = thermalZoneElement.child("ClgTstatSchRef");
  if (clgTstatSchRefElement) {
    std::string scheduleName = escapeName(clgTstatSchRefElement.text().as_string());
    clgTstatSch = model.getModelObjectByName<model::Schedule>(scheduleName);
    if (clgTstatSch) {
      if (optionalThermostat) {
        optionalThermostat->setCoolingSchedule(*clgTstatSch);
      } else {
        LOG(Error, "Cannot assign cooling schedule to unconditioned thermal zone '" << name << "'");
      }
    } else {
      LOG(Error, "Schedule named " << scheduleName << " cannot be found.");
    }
  } else {
    if (optionalThermostat) {
      model::ScheduleRuleset scheduleRuleset = model::ScheduleRuleset(model);
      scheduleRuleset.setName(thermalZone.name().get() + " Default Cooling Schedule");
      model::ScheduleDay scheduleDay = scheduleRuleset.defaultDaySchedule();
      scheduleDay.addValue(Time(1.0),90.0);
      optionalThermostat->setCoolingSchedule(scheduleRuleset);
      clgTstatSch = scheduleRuleset;
    }
  }

  // HtgTstatSchRef
  boost::optional<model::Schedule> htgTstatSch;
  auto htgTstatSchRefElement = thermalZoneElement.child("HtgTstatSchRef");
  if (htgTstatSchRefElement) {
    std::string scheduleName = escapeName(htgTstatSchRefElement.text().as_string());
    htgTstatSch = model.getModelObjectByName<model::Schedule>(scheduleName);
    if (htgTstatSch) {
      if (optionalThermostat) {
        optionalThermostat->setHeatingSchedule(*htgTstatSch);
      } else {
        LOG(Error, "Cannot assign heating schedule to unconditioned thermal zone '" << name << "'");
      }
    } else {
      LOG(Error, "Schedule named " << scheduleName << " cannot be found.");
    }
  } else {
    if (optionalThermostat) {
      model::ScheduleRuleset scheduleRuleset = model::ScheduleRuleset(model);
      scheduleRuleset.setName(thermalZone.name().get() + " Default Heating Schedule");
      model::ScheduleDay scheduleDay = scheduleRuleset.defaultDaySchedule();
      scheduleDay.addValue(Time(1.0),-100.0);
      optionalThermostat->setHeatingSchedule(scheduleRuleset);
      htgTstatSch = scheduleRuleset;
    }
  }

  // Ventilation

  double ventPerPersonSim = 0.0;
  double ventPerAreaSim = 0.0;

  std::string ventSpecMthdSim = "Maximum";

  pugi::xml_node ventPerPersonSimElement = thermalZoneElement.child("VentPerPersonSim");
  boost::optional<double> _ventPerPersonSim = lexicalCastToDouble(ventPerPersonSimElement);
  if( _ventPerPersonSim )
  {
    // cfm / person -> m^3/s / person
    ventPerPersonSim = unitToUnit(_ventPerPersonSim.get(),"cfm","m^3/s").get();
  }

  pugi::xml_node ventPerAreaSimElement = thermalZoneElement.child("VentPerAreaSim");
  boost::optional<double> _ventPerAreaSim = lexicalCastToDouble(ventPerAreaSimElement);
  if( _ventPerAreaSim )
  {
    // cfm / ft^2 -> m^3/s / m^2
    ventPerAreaSim = _ventPerAreaSim.get() * 0.00508;
  }

  pugi::xml_node ventSpecMthdSimElement = thermalZoneElement.child("VentSpecMthdSim");
  if( istringEqual(ventSpecMthdSimElement.text().as_string(),"Sum") )
  {
    ventSpecMthdSim = "Sum";
  }

  openstudio::model::DesignSpecificationOutdoorAir designSpecificationOutdoorAir(model);
  designSpecificationOutdoorAir.setName(name + " Design Specification Outdoor Air");
  designSpecificationOutdoorAir.setOutdoorAirFlowperPerson(ventPerPersonSim);
  designSpecificationOutdoorAir.setOutdoorAirFlowperFloorArea(ventPerAreaSim);
  designSpecificationOutdoorAir.setOutdoorAirMethod(ventSpecMthdSim);

  std::vector<model::Space> spaces = thermalZone.spaces();

  for( auto & space : spaces)
  {
    space.setDesignSpecificationOutdoorAir(designSpecificationOutdoorAir);
  }

  // Exhaust Fan(s)
  struct ExhaustFanInfo {
    pugi::xml_node exhFanNameElement;
    pugi::xml_node exhFlowRtSimElement;
    pugi::xml_node exhFanEndUseCatElement;
    pugi::xml_node exhAvailSchRefElement;
    pugi::xml_node exhTotEffElement;
    pugi::xml_node exhTotStaticPressElement;
    pugi::xml_node exhFlowSchRefElement;
    pugi::xml_node exhOperModeElement;
    pugi::xml_node exhMinTempSchRefElement;
    pugi::xml_node exhBalancedSchRefElement;
    pugi::xml_node exhHtRcvryRefElement;
  };

  std::map<int, ExhaustFanInfo> exhaustFanInfo;
  int defaultFanIndex = 0;

  auto getFanElement = [&](const std::string & elementName, pugi::xml_node ExhaustFanInfo::*member ) {
    for ( auto element : thermalZoneElement.children(elementName.c_str()) ) {
      auto index = lexicalCastToInt(element.attribute("index"));
      if(! index) {
        index = defaultFanIndex;
      }
      auto it = exhaustFanInfo.find(index.get());
      if ( it != exhaustFanInfo.end() ) {
        it->second.*member = element;
      }
    }
  };

  for ( auto element : thermalZoneElement.children("ExhFanName") ) {
    ExhaustFanInfo info;
    info.exhFanNameElement = element;
    auto index = lexicalCastToInt(info.exhFanNameElement.attribute("index"));
    if(index) {
      exhaustFanInfo[index.get()] = info;
    } else {
      exhaustFanInfo[defaultFanIndex] = info;
    }
  }

  getFanElement("ExhFlowSim", &ExhaustFanInfo::exhFlowRtSimElement);
  getFanElement("ExhFanEndUseCat", &ExhaustFanInfo::exhFanEndUseCatElement);
  getFanElement("ExhAvailSchRef", &ExhaustFanInfo::exhAvailSchRefElement);
  getFanElement("ExhTotEff", &ExhaustFanInfo::exhTotEffElement);
  getFanElement("ExhTotStaticPress", &ExhaustFanInfo::exhTotStaticPressElement);
  getFanElement("ExhFlowSchRef", &ExhaustFanInfo::exhFlowSchRefElement);
  getFanElement("ExhOperMode", &ExhaustFanInfo::exhOperModeElement);
  getFanElement("ExhMinTempSchRef", &ExhaustFanInfo::exhMinTempSchRefElement);
  getFanElement("ExhBalancedSchRef", &ExhaustFanInfo::exhBalancedSchRefElement);
  getFanElement("ExhHtRcvryRef", &ExhaustFanInfo::exhHtRcvryRefElement);

  for ( const auto & fanInfo: exhaustFanInfo ) {
    model::FanZoneExhaust exhaustFan(model);
    exhaustFan.setName(fanInfo.second.exhFanNameElement.text().as_string());
    if ( fanInfo.second.exhFanEndUseCatElement.text().empty() ) {
      exhaustFan.setEndUseSubcategory("General");
    } else {
      exhaustFan.setEndUseSubcategory(fanInfo.second.exhFanEndUseCatElement.text().as_string());
    }

    auto _exhFlowRtSim = lexicalCastToDouble(fanInfo.second.exhFlowRtSimElement);
    exhaustFan.setMaximumFlowRate(unitToUnit(_exhFlowRtSim.get(),"cfm","m^3/s").get());

    std::string exhAvailSchRef = escapeName(fanInfo.second.exhAvailSchRefElement.text().as_string());
    boost::optional<model::Schedule> exhAvailSch = model.getModelObjectByName<model::Schedule>(exhAvailSchRef);
    if ( exhAvailSch ) {
      exhaustFan.setAvailabilitySchedule(exhAvailSch.get());
    }

    auto _exhTotEff = lexicalCastToDouble(fanInfo.second.exhTotEffElement);
    if ( _exhTotEff ) {
      exhaustFan.setFanEfficiency(_exhTotEff.get());
    }

    auto _exhTotStaticPress = lexicalCastToDouble(fanInfo.second.exhTotStaticPressElement);
    if ( _exhTotStaticPress ) {
      // Convert in WC to Pa
      exhaustFan.setPressureRise(_exhTotStaticPress.get() * 249.0889 );
    }

    std::string exhFlowSchRef = escapeName(fanInfo.second.exhFlowSchRefElement.text().as_string());
    boost::optional<model::Schedule> exhFlowSch = model.getModelObjectByName<model::Schedule>(exhFlowSchRef);
    if( exhFlowSch )
    {
      exhaustFan.setFlowFractionSchedule(exhFlowSch.get());
    }

    if (istringEqual(fanInfo.second.exhOperModeElement.text().as_string(), "DecoupledFromSystem") ) {
      exhaustFan.setSystemAvailabilityManagerCouplingMode("Decoupled");
    } else if (istringEqual(fanInfo.second.exhOperModeElement.text().as_string(), "CoupledToSystem")) {
      exhaustFan.setSystemAvailabilityManagerCouplingMode("Coupled");
    }

    std::string exhMinTempSchRef = escapeName(fanInfo.second.exhMinTempSchRefElement.text().as_string());
    boost::optional<model::Schedule> exhMinTempSch = model.getModelObjectByName<model::Schedule>(exhMinTempSchRef);
    if( exhMinTempSch )
    {
      exhaustFan.setMinimumZoneTemperatureLimitSchedule(exhMinTempSch.get());
    }

    std::string exhBalancedSchRef = escapeName(fanInfo.second.exhBalancedSchRefElement.text().as_string());
    boost::optional<model::Schedule> exhBalancedSch = model.getModelObjectByName<model::Schedule>(exhBalancedSchRef);
    if( exhBalancedSch )
    {
      exhaustFan.setBalancedExhaustFractionSchedule(exhBalancedSch.get());
    }

    exhaustFan.addToThermalZone(thermalZone);

    // If there is a ExhHtRcvryRef then we make a copy of that heat recovery device
    // and attach the outlet of the exhaust fan to the relief side of the heat recovery device.
    // The oa stream of the HX is connected in series with other HX's on the oa system
    auto exhHtRcvryRef = escapeName(fanInfo.second.exhHtRcvryRefElement.text().as_string());
    auto exhHtRcvry = model.getModelObjectByName<model::HeatExchangerAirToAirSensibleAndLatent>(exhHtRcvryRef);
    if (exhHtRcvry) {
      attachExhFanHX(exhaustFan, exhHtRcvry.get());
    }
  }

  // Daylighting
  std::vector<pugi::xml_node> daylighting1CoordElements = makeVectorOfChildren(thermalZoneElement, "DayltgIllumRefPt1Coord");
  pugi::xml_node daylighting1SetpointElement = thermalZoneElement.child("DayltgIllumSetpt1");
  pugi::xml_node daylighting1FractionElement = thermalZoneElement.child("DayltgCtrlLtgFrac1");

  std::vector<pugi::xml_node> daylighting2CoordElements = makeVectorOfChildren(thermalZoneElement, "DayltgIllumRefPt2Coord");
  pugi::xml_node daylighting2SetpointElement = thermalZoneElement.child("DayltgIllumSetpt2");
  pugi::xml_node daylighting2FractionElement = thermalZoneElement.child("DayltgCtrlLtgFrac2");

  pugi::xml_node daylightingMinLightingElement = thermalZoneElement.child("DayltgMinDimLtgFrac");
  pugi::xml_node daylightingMinPowerElement = thermalZoneElement.child("DayltgMinDimPwrFrac");
  pugi::xml_node daylightingGlareAzimuthElement = thermalZoneElement.child("DayltgGlrAz");
  pugi::xml_node daylightingMaxGlareElement = thermalZoneElement.child("DayltgMaxGlrIdx");

  pugi::xml_node daylightingControlTypeElement = thermalZoneElement.child("DayltgCtrlType");
  pugi::xml_node daylightingNumberOfControlStepsElement = thermalZoneElement.child("DayltgNumOfCtrlSteps");

  boost::optional<double> daylightingMinLighting = lexicalCastToDouble(daylightingMinLightingElement);

  boost::optional<double> daylightingMinPower = lexicalCastToDouble(daylightingMinPowerElement);

  boost::optional<double> daylightingGlareAzimuth = lexicalCastToDouble(daylightingGlareAzimuthElement);

  boost::optional<double> daylightingMaxGlare = lexicalCastToDouble(daylightingMaxGlareElement);

  boost::optional<std::string> daylightingControlType;
  if (daylightingControlTypeElement){
    daylightingControlType = daylightingControlTypeElement.text().as_string();
  }
  boost::optional<int> daylightingNumberOfControlSteps = lexicalCastToInt(daylightingNumberOfControlStepsElement);

  // first point
  boost::optional<model::DaylightingControl> daylightingControl1;
  if (daylighting1CoordElements.size() == 3 && daylighting1SetpointElement && daylighting1FractionElement){
    double x = footToMeter*daylighting1CoordElements[0].text().as_double();
    double y = footToMeter*daylighting1CoordElements[1].text().as_double();
    double z = footToMeter*daylighting1CoordElements[2].text().as_double();

    // DLM: units in SDD are in lux
    //double setpoint = footCandleToLux*daylighting1SetpointElement.text().as_double();
    double setpoint = daylighting1SetpointElement.text().as_double();
    double fraction = daylighting1FractionElement.text().as_double();

    daylightingControl1 = model::DaylightingControl(model);
    daylightingControl1->setPositionXCoordinate(x);
    daylightingControl1->setPositionYCoordinate(y);
    daylightingControl1->setPositionZCoordinate(z);
    daylightingControl1->setIlluminanceSetpoint(setpoint);

    if (daylightingControlType){
      if (istringEqual(*daylightingControlType, "SteppedSwitching") || istringEqual(*daylightingControlType, "SteppedDimming")){
        if (!daylightingControl1->setLightingControlType("Stepped")) {
          LOG(Error, "Could not set daylighting control type to 'Stepped'");
        }
        if (daylightingNumberOfControlSteps){
          daylightingControl1->setNumberofSteppedControlSteps(*daylightingNumberOfControlSteps);
        }
      }else if (istringEqual(*daylightingControlType, "Continuous")){
        if (!daylightingControl1->setLightingControlType("Continuous")) {
          LOG(Error, "Could not set daylighting control type to 'Continuous'");
        }
      }else if (istringEqual(*daylightingControlType, "ContinuousPlusOff")){
        if (!daylightingControl1->setLightingControlType("Continuous/Off")) {
          LOG(Error, "Could not set daylighting control type to 'Continuous/Off'");
        }
      }else{
        LOG(Error, "Unknown DayltgCtrlType '" << *daylightingControlType << "'");
      }
    }

    if (daylightingMinLighting){
      daylightingControl1->setMinimumLightOutputFractionforContinuousDimmingControl(*daylightingMinLighting);
    }
    if (daylightingMinPower){
      daylightingControl1->setMinimumInputPowerFractionforContinuousDimmingControl(*daylightingMinPower);
    }
    if (daylightingGlareAzimuth){
      daylightingControl1->setPhiRotationAroundZAxis(degToRad(*daylightingGlareAzimuth));
    }
    if (daylightingMaxGlare){
      daylightingControl1->setMaximumAllowableDiscomfortGlareIndex(*daylightingMaxGlare);
    }

    BoundingBox pointBox;
    pointBox.addPoint(Point3d(x,y,z));
    for (model::Space space : spaces){
      BoundingBox boundingBox = space.boundingBox();
      if (boundingBox.intersects(pointBox)){
        daylightingControl1->setSpace(space);
        break;
      }
    }

    if (!daylightingControl1->space()){
      LOG(Error, "Primary daylighting control specified for Thermal Zone '" << name << "' could not be associated with a space in that zone");
      daylightingControl1->remove();
      daylightingControl1.reset();
    }else{
      thermalZone.setPrimaryDaylightingControl(*daylightingControl1);
      thermalZone.setFractionofZoneControlledbyPrimaryDaylightingControl(fraction);
    }

  }

  // second point
  boost::optional<model::DaylightingControl> daylightingControl2;
  if (daylighting2CoordElements.size() == 3 && daylighting2SetpointElement && daylighting2FractionElement){
    if (!daylightingControl1){
      LOG(Error, "Secondary daylighting control specified for Thermal Zone '" << name << "' but there is no primary daylighting control");
    }else{
      double x = footToMeter*daylighting2CoordElements[0].text().as_double();
      double y = footToMeter*daylighting2CoordElements[1].text().as_double();
      double z = footToMeter*daylighting2CoordElements[2].text().as_double();

      // DLM: units in SDD are in lux
      //double setpoint = footCandleToLux*daylighting2SetpointElement.text().as_double();
      double setpoint = daylighting2SetpointElement.text().as_double();
      double fraction = daylighting2FractionElement.text().as_double();

      daylightingControl2 = model::DaylightingControl(model);
      daylightingControl2->setPositionXCoordinate(x);
      daylightingControl2->setPositionYCoordinate(y);
      daylightingControl2->setPositionZCoordinate(z);
      daylightingControl2->setIlluminanceSetpoint(setpoint);

      if (daylightingControlType){
        if (istringEqual(*daylightingControlType, "SteppedSwitching") || istringEqual(*daylightingControlType, "SteppedDimming")){
          if (!daylightingControl2->setLightingControlType("Stepped")) {
            LOG(Error, "Could not set daylighting control type to 'Stepped'");
          }
          if (daylightingNumberOfControlSteps){
            daylightingControl2->setNumberofSteppedControlSteps(*daylightingNumberOfControlSteps);
          }
        }else if (istringEqual(*daylightingControlType, "Continuous")){
          if(!daylightingControl2->setLightingControlType("Continuous")) {
            LOG(Error, "Could not set daylighting control type to 'Continuous'");
          }
        }else if (istringEqual(*daylightingControlType, "ContinuousPlusOff")){
          if (!daylightingControl2->setLightingControlType("Continuous/Off")) {
            LOG(Error, "Could not set daylighting control type to 'Continuous/Off'");
          }
        }else{
          LOG(Error, "Unknown DayltgCtrlType '" << *daylightingControlType << "'");
        }
      }

      if (daylightingMinLighting){
        daylightingControl2->setMinimumLightOutputFractionforContinuousDimmingControl(*daylightingMinLighting);
      }
      if (daylightingMinPower){
        daylightingControl2->setMinimumInputPowerFractionforContinuousDimmingControl(*daylightingMinPower);
      }
      if (daylightingGlareAzimuth){
        daylightingControl2->setPhiRotationAroundZAxis(degToRad(*daylightingGlareAzimuth));
      }
      if (daylightingMaxGlare){
        daylightingControl2->setMaximumAllowableDiscomfortGlareIndex(*daylightingMaxGlare);
      }

      thermalZone.setSecondaryDaylightingControl(*daylightingControl2);

      BoundingBox pointBox;
      pointBox.addPoint(Point3d(x,y,z));
      for (model::Space space : spaces){
        BoundingBox boundingBox = space.boundingBox();
        if (boundingBox.intersects(pointBox)){
          daylightingControl2->setSpace(space);
          break;
        }
      }

      if (!daylightingControl2->space()){
        LOG(Error, "Secondary daylighting control specified for Thermal Zone '" << name << "' could not be associated with a space in that zone");
        daylightingControl2->remove();
        daylightingControl2.reset();
      }else{
        thermalZone.setSecondaryDaylightingControl(*daylightingControl2);
        thermalZone.setFractionofZoneControlledbySecondaryDaylightingControl(fraction);
      }
    }
  }


  // Mult
  pugi::xml_node multElement = thermalZoneElement.child("Mult");
  boost::optional<int> _mult = lexicalCastToInt(multElement);
  if (_mult){
    thermalZone.setMultiplier(_mult.get());
  }

  // Optional AirLoopHVAC
  boost::optional<model::AirLoopHVAC> airLoopHVAC;

  // Map of SysRefElement to SysElement. SysElement will be a zone sys or null
  struct SysInfo {
    pugi::xml_node SysRefElement;
    pugi::xml_node ZnSysElement;
    pugi::xml_node AirSysElement;
    int Index;
    boost::optional<model::ModelObject>ModelObject;
    //int Priority;
  };

  std::vector<SysInfo> priAirCondInfo;
  std::vector<SysInfo> simSysInfo;

  {
    std::vector<pugi::xml_node> elements = makeVectorOfChildren(thermalZoneElement, "PriAirCondgSysRef");
    for (std::vector<pugi::xml_node>::size_type i = 0; i < elements.size(); i++)
    {
      const auto & element = elements[i];
      SysInfo sysInfo;
      sysInfo.SysRefElement = element;
      sysInfo.ZnSysElement = findZnSysElement(element);
      sysInfo.AirSysElement = findAirSysElement(element);
      // Check if the node has an attribute 'index' that can be converted to an int
      boost::optional<int> _index = lexicalCastToInt(element.attribute("index"));
      if (_index) {
        sysInfo.Index = _index.get();
      }
      priAirCondInfo.push_back(sysInfo);
    }
  }

  {
    std::vector<pugi::xml_node> elements = makeVectorOfChildren(thermalZoneElement, "SimSysRef");
    for (std::vector<pugi::xml_node>::size_type i = 0; i < elements.size(); i++)
    {
      const auto & element = elements[i];
      SysInfo sysInfo;
      sysInfo.SysRefElement = element;
      sysInfo.ZnSysElement = findZnSysElement(element);
      sysInfo.AirSysElement = findAirSysElement(element);
      // Check if the node has an attribute 'index' that can be converted to an int
      boost::optional<int> _index = lexicalCastToInt(element.attribute("index"));
      if (_index) {
        sysInfo.Index = _index.get();
      }
      simSysInfo.push_back(sysInfo);
    }
  }

  pugi::xml_node ventSysRefElement = thermalZoneElement.child("VentSysRef");
  boost::optional<model::ModelObject> ventSysEquip;

  // ThermalZoneVentilationSystem
  // Find out if ventSys is already a simsys or a priaircondgsys (=they have the same inner TEXT)
  bool translateVentSys = true;
  for( const auto & info : priAirCondInfo ) {
    if( openstudio::istringEqual(info.SysRefElement.text().as_string(),
                                 ventSysRefElement.text().as_string()) ) {
      translateVentSys = false;
      break;
    }
  }

  if( translateVentSys ) {
    for( const auto & info : simSysInfo ) {
      if( openstudio::istringEqual(info.SysRefElement.text().as_string(),
                                   ventSysRefElement.text().as_string()) ) {
        translateVentSys = false;
        break;
      }
    }
  }
 
  // return true if zone is already connected to loop
  auto zoneisConnectedToLoop = [](const model::ThermalZone & zone, const model::AirLoopHVAC & loop) {
    const auto currentLoops = zone.airLoopHVACs();
    return (std::find(currentLoops.begin(), currentLoops.end(), loop) != currentLoops.end());
  };


  if( translateVentSys ) {
    airLoopHVAC = model.getModelObjectByName<model::AirLoopHVAC>(ventSysRefElement.text().as_string());

    if( airLoopHVAC && (! zoneisConnectedToLoop(thermalZone, airLoopHVAC.get())))
    {
      auto trmlUnitElement = findTrmlUnitElementForZone(nameElement, airLoopHVAC->nameString());
      if (trmlUnitElement) {
        if( boost::optional<model::ModelObject> trmlUnit = translateTrmlUnit(trmlUnitElement, model) )
        {
          ventSysEquip = trmlUnit;
          auto trmlUnitHVACComponent = trmlUnit->cast<model::HVACComponent>();
          airLoopHVAC->multiAddBranchForZone(thermalZone,trmlUnitHVACComponent);
          pugi::xml_node inducedAirZnRefElement = trmlUnitElement.child("InducedAirZnRef");
          if( boost::optional<model::ThermalZone> tz = model.getModelObjectByName<model::ThermalZone>(inducedAirZnRefElement.text().as_string()) )
          {
             if( tz->isPlenum() )
             {
               if( boost::optional<model::AirTerminalSingleDuctSeriesPIUReheat> piu = trmlUnit->optionalCast<model::AirTerminalSingleDuctSeriesPIUReheat>() )
               {
                 piu->getImpl<model::detail::AirTerminalSingleDuctSeriesPIUReheat_Impl>()->setInducedAirPlenumZone(tz.get());
               }
               else if( boost::optional<model::AirTerminalSingleDuctParallelPIUReheat> piu = trmlUnit->optionalCast<model::AirTerminalSingleDuctParallelPIUReheat>() )
               {
                 piu->getImpl<model::detail::AirTerminalSingleDuctParallelPIUReheat_Impl>()->setInducedAirPlenumZone(tz.get());
               }
             }
          }
        }
      }
    }

    // If the model has requested ventilation from the system,
    // make sure the system has an OA system even if it wasn't asked for.
    if( airLoopHVAC )
    {
      boost::optional<model::AirLoopHVACOutdoorAirSystem> oaSystem = airLoopHVAC->airLoopHVACOutdoorAirSystem();
      if( ! oaSystem )
      {
        model::ControllerOutdoorAir oaController(model);

        oaController.setName(airLoopHVAC->name().get() + " OA Controller");

        model::AirLoopHVACOutdoorAirSystem newOASystem(model,oaController);

        newOASystem.setName(airLoopHVAC->name().get() + " OA System");

        model::Node supplyInletNode = airLoopHVAC->supplyInletNode();

        newOASystem.addToNode(supplyInletNode);
      }
    }
  }

  auto translateSystemForZone = [&](SysInfo & sysInfo) {
    if (sysInfo.ZnSysElement)
    {
      boost::optional<model::ModelObject> mo = translateZnSys(sysInfo.ZnSysElement, thermalZone);
      sysInfo.ModelObject = mo;

      if( mo )
      {
        boost::optional<model::ZoneHVACComponent> zoneHVACComponent = mo->optionalCast<model::ZoneHVACComponent>();

        if( zoneHVACComponent )
        {
          zoneHVACComponent->addToThermalZone(thermalZone);

          // If not the ventilation system we lock down the oa system of the zone equipment
          if( !openstudio::istringEqual(sysInfo.SysRefElement.text().as_string(),
                                        ventSysRefElement.text().as_string()) ) {
            if( boost::optional<model::ZoneHVACPackagedTerminalAirConditioner> ptac =
                zoneHVACComponent->optionalCast<model::ZoneHVACPackagedTerminalAirConditioner>() ) {
              ptac->setOutdoorAirFlowRateDuringHeatingOperation(0.0);
              ptac->setOutdoorAirFlowRateDuringCoolingOperation(0.0);
              ptac->setOutdoorAirFlowRateWhenNoCoolingorHeatingisNeeded(0.0);
            } else if( boost::optional<model::ZoneHVACPackagedTerminalHeatPump> pthp =
                zoneHVACComponent->optionalCast<model::ZoneHVACPackagedTerminalHeatPump>() ) {
              pthp->setOutdoorAirFlowRateDuringHeatingOperation(0.0);
              pthp->setOutdoorAirFlowRateDuringCoolingOperation(0.0);
              pthp->setOutdoorAirFlowRateWhenNoCoolingorHeatingisNeeded(0.0);
            } else if( boost::optional<model::ZoneHVACFourPipeFanCoil> fanCoil =
                zoneHVACComponent->optionalCast<model::ZoneHVACFourPipeFanCoil>() ) {
              fanCoil->setMaximumOutdoorAirFlowRate(0.0);
            }
          }

          // Additional finishing touches for some zone equipment that needs data from the zone level...

          auto thrtlgRngElement = thermalZoneElement.child("ThrtlgRng");
          auto thrtlgRng = lexicalCastToDouble(thrtlgRngElement);
          if (thrtlgRng) {
            thrtlgRng = thrtlgRng.get() * 5.0 / 9.0; // delta F to C
          }

          auto zoneHVACLowTempRadiantVarFlow = zoneHVACComponent->optionalCast<model::ZoneHVACLowTempRadiantVarFlow>();
          if (zoneHVACLowTempRadiantVarFlow) {
            boost::optional<model::CoilCoolingLowTempRadiantVarFlow> coolingCoil;
            boost::optional<model::CoilHeatingLowTempRadiantVarFlow> heatingCoil;

            auto ccoil = zoneHVACLowTempRadiantVarFlow->coolingCoil();
            if (ccoil) {
              coolingCoil = ccoil->cast<model::CoilCoolingLowTempRadiantVarFlow>();
            }

            auto hcoil = zoneHVACLowTempRadiantVarFlow->heatingCoil();
            if (hcoil) {
              heatingCoil = hcoil->cast<model::CoilHeatingLowTempRadiantVarFlow>();
            }

            if (clgTstatSch && coolingCoil) {
              auto currentCoolingSchedule = coolingCoil->coolingControlTemperatureSchedule();
              if (currentCoolingSchedule) {
                currentCoolingSchedule->remove();
              }
              coolingCoil->setCoolingControlTemperatureSchedule(clgTstatSch.get());
            }
            if (htgTstatSch && heatingCoil) {
              auto currentHeatingSchedule = heatingCoil->heatingControlTemperatureSchedule();
              if (currentHeatingSchedule) {
                currentHeatingSchedule->remove();
              }
              heatingCoil->setHeatingControlTemperatureSchedule(htgTstatSch.get());
            }

            if (thrtlgRng) {
              if (heatingCoil) {
                heatingCoil->setHeatingControlThrottlingRange(thrtlgRng.get());
              }
              if (coolingCoil) {
                coolingCoil->setCoolingControlThrottlingRange(thrtlgRng.get());
              }
            }
          }

          auto zoneHVACLowTemperatureRadiantElectric = zoneHVACComponent->optionalCast<model::ZoneHVACLowTemperatureRadiantElectric>();
          if (zoneHVACLowTemperatureRadiantElectric) {
            if (thrtlgRng) {
              zoneHVACLowTemperatureRadiantElectric->setHeatingThrottlingRange(thrtlgRng.get());
            }

            if (htgTstatSch) {
              zoneHVACLowTemperatureRadiantElectric->heatingSetpointTemperatureSchedule().remove();
              zoneHVACLowTemperatureRadiantElectric->setHeatingSetpointTemperatureSchedule(htgTstatSch.get());
            }
          }
        }
      }
    }
    else
    {
      airLoopHVAC = model.getModelObjectByName<model::AirLoopHVAC>(sysInfo.SysRefElement.text().as_string());

      if( airLoopHVAC && (! zoneisConnectedToLoop(thermalZone, airLoopHVAC.get())))
      {
        auto trmlUnitElement = findTrmlUnitElementForZone(nameElement, airLoopHVAC->nameString());
        if (trmlUnitElement) {
          if( boost::optional<model::ModelObject> trmlUnit = translateTrmlUnit(trmlUnitElement, model) )
          {
            sysInfo.ModelObject = trmlUnit;
            auto trmlUnitHVACComponent = trmlUnit->cast<model::StraightComponent>();
            airLoopHVAC->multiAddBranchForZone(thermalZone,trmlUnitHVACComponent);
            pugi::xml_node inducedAirZnRefElement = trmlUnitElement.child("InducedAirZnRef");
            if( boost::optional<model::ThermalZone> tz = model.getModelObjectByName<model::ThermalZone>(inducedAirZnRefElement.text().as_string()) )
            {
               if( tz->isPlenum() )
               {
                 if( boost::optional<model::AirTerminalSingleDuctSeriesPIUReheat> piu = trmlUnit->optionalCast<model::AirTerminalSingleDuctSeriesPIUReheat>() )
                 {
                   piu->getImpl<model::detail::AirTerminalSingleDuctSeriesPIUReheat_Impl>()->setInducedAirPlenumZone(tz.get());
                 }
                 else if( boost::optional<model::AirTerminalSingleDuctParallelPIUReheat> piu = trmlUnit->optionalCast<model::AirTerminalSingleDuctParallelPIUReheat>() )
                 {
                   piu->getImpl<model::detail::AirTerminalSingleDuctParallelPIUReheat_Impl>()->setInducedAirPlenumZone(tz.get());
                 }
               }
            }
          }
        }
      }
    }
  };

  for( auto & sysInfo : priAirCondInfo ) {
    translateSystemForZone(sysInfo);
  }
  for( auto & sysInfo : simSysInfo ) {
    translateSystemForZone(sysInfo);
  }

  auto createInletSideMixerForZoneSys = [&](const SysInfo & sysInfo) {
    if( sysInfo.ModelObject ) {
    	auto elements = makeVectorOfChildren(thermalZoneElement, "VentByPriAirCondgSys");
    	for (std::size_t i = 0; i < elements.size(); i++) {
    	  const auto & element = elements[i];
    	  auto index = lexicalCastToInt(element.attribute("index"));
    	  if( index ) {
    	    if( index.get() == sysInfo.Index ) {
    	      auto flag = element.text().as_int();
            if (flag == 1) {
              auto zoneHVAC = sysInfo.ModelObject->optionalCast<model::ZoneHVACComponent>();
              if (zoneHVAC) {
                auto tz = zoneHVAC->thermalZone();
                if (tz) {
                  auto loop = tz->airLoopHVAC();
                  if (loop) {
                    zoneHVAC->removeFromThermalZone();
                    loop->removeBranchForZone(tz.get());
                    model::AirTerminalSingleDuctInletSideMixer inletSideMixer(model);
                    loop->multiAddBranchForZone(tz.get(), inletSideMixer);
                    auto node = inletSideMixer.outletModelObject()->cast<model::Node>();
                    zoneHVAC->addToNode(node);
                    break;
                  }
                }
              }
            }
    	    }
    	  }
    	}
    }
  };

  for( auto & sysInfo : priAirCondInfo ) {
    createInletSideMixerForZoneSys(sysInfo);
  }

  // Set priority
  auto setPriority = [&thermalZone, &thermalZoneElement](const SysInfo & sysInfo, const std::string& priorityElement) {
    if( sysInfo.ModelObject ) {
      // Clg and Htg
      for (const pugi::xml_node& element: thermalZoneElement.children(priorityElement.c_str()) ) {
        // Check if the node has an attribute 'index' that is an int *and* matches our sysInfo.index
        boost::optional<int> _index = lexicalCastToInt(element.attribute("index"));
        if (_index && _index.get() == sysInfo.Index) {
          // Now we check if it has a priority in its text()
          boost::optional<int> _priority = lexicalCastToInt(element);
          if (_priority) {
            thermalZone.setCoolingPriority(sysInfo.ModelObject.get(), _priority.get());
            thermalZone.setHeatingPriority(sysInfo.ModelObject.get(), _priority.get());
          }
          break;
        }
      }
      // Clg
      for (const pugi::xml_node& element: thermalZoneElement.children((priorityElement + "Clg").c_str()) ) {
        // Check if the node has an attribute 'index' that is an int *and* matches our sysInfo.index
        boost::optional<int> _index = lexicalCastToInt(element.attribute("index"));
        if (_index && _index.get() == sysInfo.Index) {
          // Now we check if it has a priority in its text()
          boost::optional<int> _priority = lexicalCastToInt(element);
          if (_priority) {
            thermalZone.setCoolingPriority(sysInfo.ModelObject.get(), _priority.get());
          }
          break;
        }
      }
      // Htg
      for (const pugi::xml_node& element: thermalZoneElement.children((priorityElement + "Htg").c_str()) ) {
        // Check if the node has an attribute 'index' that is an int *and* matches our sysInfo.index
        boost::optional<int> _index = lexicalCastToInt(element.attribute("index"));
        if (_index && _index.get() == sysInfo.Index) {
          // Now we check if it has a priority in its text()
          boost::optional<int> _priority = lexicalCastToInt(element);
          if (_priority) {
            thermalZone.setHeatingPriority(sysInfo.ModelObject.get(), _priority.get());
          }
          break;
        }
      }
    }
  };

  for( const SysInfo& sysInfo : priAirCondInfo ) {
    setPriority(sysInfo, "PriAirCondgSysPriority");
  }
  for( const SysInfo& sysInfo : simSysInfo ) {
    setPriority(sysInfo, "SimSysPriority");
  }

  if( ventSysEquip ) {
    pugi::xml_node ventSysPriorityElement = thermalZoneElement.child("VentSysPriority");
    boost::optional<int> _ventSysPriority = lexicalCastToInt(ventSysPriorityElement);
    if( _ventSysPriority ) {
      int value = _ventSysPriority.get();
      thermalZone.setCoolingPriority(ventSysEquip.get(),value);
      thermalZone.setHeatingPriority(ventSysEquip.get(),value);
    }

    pugi::xml_node ventSysPriorityClgElement = thermalZoneElement.child("VentSysPriorityClg");
    boost::optional<int> _ventSysPriorityClg = lexicalCastToInt(ventSysPriorityClgElement);
    if( _ventSysPriorityClg ) {
      thermalZone.setCoolingPriority(ventSysEquip.get(), _ventSysPriorityClg.get());
    }

    pugi::xml_node ventSysPriorityHtgElement = thermalZoneElement.child("VentSysPriorityHtg");
    boost::optional<int> _ventSysPriorityHtg = lexicalCastToInt(ventSysPriorityHtgElement);
    if( _ventSysPriorityHtg ) {
      thermalZone.setHeatingPriority(ventSysEquip.get(), _ventSysPriorityHtg.get());
    }
  }

  // Set zone for optimum start, if applicable
  {
    auto optimumStart = m_optimumStartControlZones.find(thermalZone.name().get());
    if( optimumStart != m_optimumStartControlZones.end() ) {
      optimumStart->second.setControlZone(thermalZone);
    }
  }

  // Set zone for night cycle, if applicable
  {
    auto nightCycle = m_nightCycleControlZones.find(thermalZone.name().get());
    if( nightCycle != m_nightCycleControlZones.end() ) {
      std::vector<model::ThermalZone> thermalZones = {thermalZone};
      nightCycle->second.setControlThermalZones(thermalZones);
    }
  }

  // Set the master control zone for vrf system, if applicable
  {
    auto vrfSystem = m_vrfSystemControlZones.find(thermalZone.name().get());
    if( vrfSystem != m_vrfSystemControlZones.end() ) {
      vrfSystem->second.setZoneforMasterThermostatLocation(thermalZone);
    }
  }

  // DCV?
  pugi::xml_node ventCtrlMthdElement = thermalZoneElement.child("VentCtrlMthd");
  if( istringEqual(ventCtrlMthdElement.text().as_string(),"OccupantSensors") ||
      istringEqual(ventCtrlMthdElement.text().as_string(),"CO2Sensors") )
  {
    if( airLoopHVAC )
    {
      if( boost::optional<model::AirLoopHVACOutdoorAirSystem> oaSystem = airLoopHVAC->airLoopHVACOutdoorAirSystem() )
      {
        oaSystem->getControllerOutdoorAir().controllerMechanicalVentilation().setDemandControlledVentilation(true);
      }
    }
  }

  airLoopHVAC = thermalZone.airLoopHVAC();

  // Connect to plenum(s) if required
  if( airLoopHVAC )
  {
    pugi::xml_node rtnPlenumZnRefElement = thermalZoneElement.child("RetPlenumZnRef");
    boost::optional<model::ThermalZone> returnPlenumZone;
    returnPlenumZone = model.getModelObjectByName<model::ThermalZone>(rtnPlenumZnRefElement.text().as_string());
    if( returnPlenumZone )
    {
      thermalZone.setReturnPlenum(returnPlenumZone.get());
    }

    pugi::xml_node supPlenumZnRefElement = thermalZoneElement.child("SupPlenumZnRef");
    boost::optional<model::ThermalZone> supplyPlenumZone;
    supplyPlenumZone = model.getModelObjectByName<model::ThermalZone>(supPlenumZnRefElement.text().as_string());
    if( supplyPlenumZone )
    {
      thermalZone.setSupplyPlenum(supplyPlenumZone.get());
    }
  }

  // Connect setpoint manager single zone reheat if required
  if( airLoopHVAC )
  {
    model::Node supplyOutletNode = airLoopHVAC->supplyOutletNode();
    boost::optional<model::SetpointManagerSingleZoneReheat> spm;
    std::vector<model::SetpointManagerSingleZoneReheat> _setpointManagers = subsetCastVector<model::SetpointManagerSingleZoneReheat>(supplyOutletNode.setpointManagers());
    if( !_setpointManagers.empty() ) {
      spm = _setpointManagers.front();
    }

    // Only set the control zone if there is a SetpointManagerSingleZoneReheat on the supply outlet node
    pugi::xml_node airSystemElement = findAirSysElement(airLoopHVAC->name().get(), getProjectElement(thermalZoneElement));
    if( spm && airSystemElement )
    {
      pugi::xml_node ctrlZnRefElement = airSystemElement.child("CtrlZnRef");
      if( istringEqual(ctrlZnRefElement.text().as_string(),thermalZone.name().get()) )
      {
        spm->setControlZone(thermalZone);
      }
    }
  }

  // OperableWinInterlock
  pugi::xml_node operableWinInterlockElement = thermalZoneElement.child("OperableWinInterlock");
  boost::optional<int> _operableWinInterlock = lexicalCastToInt(operableWinInterlockElement);
  if( _operableWinInterlock && (_operableWinInterlock.get() == 1) ) {
    model::ZoneVentilationDesignFlowRate zoneVent(model);
    zoneVent.setName(thermalZone.nameString() + " Operable Window Ventilation");
    zoneVent.setFlowRateperZoneFloorArea(0.000762);
    zoneVent.setVentilationType("Intake");
    zoneVent.setFanPressureRise(0.0);
    zoneVent.setFanTotalEfficiency(1.0);
    zoneVent.setConstantTermCoefficient(1.0);
    zoneVent.setTemperatureTermCoefficient(0.0);
    zoneVent.setVelocityTermCoefficient(0.0);
    zoneVent.setVelocitySquaredTermCoefficient(0.0);
    zoneVent.setMinimumOutdoorTemperature(10);
    zoneVent.setMaximumOutdoorTemperature(29.4);
    zoneVent.setMaximumWindSpeed(40);
    zoneVent.setMinimumIndoorTemperature(-100);
    zoneVent.setMaximumIndoorTemperature(100);
    zoneVent.setDeltaTemperature(-100);

    // OperableWinOAFlow
    auto _operableWinOAFlow = lexicalCastToDouble(thermalZoneElement.child("OperableWinOAFlow"));
    if( _operableWinOAFlow ) {
      auto operableWinOAFlow = unitToUnit(_operableWinOAFlow.get(),"cfm","m^3/s").get();
      zoneVent.setDesignFlowRate(operableWinOAFlow);
    }

    pugi::xml_node operableWinOALimLoElement = thermalZoneElement.child("OperableWinOALimLo");
    boost::optional<double> _operableWinOALimLo = lexicalCastToDouble(operableWinOALimLoElement);
    if( _operableWinOALimLo ) {
      double operableWinOALimLo = unitToUnit(_operableWinOALimLo.get(),"F","C").get();
      zoneVent.setMinimumOutdoorTemperature(operableWinOALimLo);
    }

    pugi::xml_node operableWinOALimHiElement = thermalZoneElement.child("OperableWinOALimHi");
    boost::optional<double> _operableWinOALimHi = lexicalCastToDouble(operableWinOALimHiElement);
    if( _operableWinOALimHi ) {
      double operableWinOALimHi = unitToUnit(_operableWinOALimHi.get(),"F","C").get();
      zoneVent.setMaximumOutdoorTemperature(operableWinOALimHi);
    }

    // priAirCondInfo zoneVent

    for( const auto & info : priAirCondInfo ) {
      if (info.ZnSysElement) {
        auto availSchRefElement = info.ZnSysElement.child("AvailSchRef");
        if( auto availSch = model.getModelObjectByName<model::Schedule>(availSchRefElement.text().as_string()) ) {
          zoneVent.setSchedule(availSch.get());
          break;
        }
      } else if (info.AirSysElement) {
        auto availSchRefElement = info.AirSysElement.child("AvailSchRef");
        auto availSch = model.getModelObjectByName<model::Schedule>(availSchRefElement.text().as_string());
        if( auto availSch = model.getModelObjectByName<model::Schedule>(availSchRefElement.text().as_string()) ) {
          zoneVent.setSchedule(availSch.get());
          break;
        }
      }
    }

    zoneVent.addToThermalZone(thermalZone);
  }

  return result;
}

void ReverseTranslator::attachExhFanHX(model::FanZoneExhaust & fan, model::HeatExchangerAirToAirSensibleAndLatent & exhHtRcvry)
{
  auto m = fan.model();
  auto exhHtRcvryClone = exhHtRcvry.clone(m).cast<model::HeatExchangerAirToAirSensibleAndLatent>();
  exhHtRcvryClone.setName(fan.nameString() + " Heat Recovery");

  // We want to attach the cloned HX exchanger to the oa system in the normal way on the outside air side,
  // but on the relief side the cloned HX is not connected in series to the other oa system components
  // instead the exhaust fan outlet node goes directly into the cloned HX relief air inlet node,
  // and the cloned HX relief outlet node simply dumps to outside
  //
  // This is not supported by OpenStudio's public API, but we can use the private connection API,
  // to get the job done. The only known consequence is that OS App will not render this topology correctly

  auto hxInletNode = exhHtRcvry.primaryAirInletModelObject()->cast<model::Node>();
  model::Node newHXOutletNode(m);
  newHXOutletNode.setName(exhHtRcvryClone.nameString() + " OA Outlet Node");

  m.connect(hxInletNode, hxInletNode.outletPort(), exhHtRcvryClone, exhHtRcvryClone.primaryAirInletPort());
  m.connect(exhHtRcvryClone, exhHtRcvryClone.primaryAirOutletPort(), newHXOutletNode, newHXOutletNode.inletPort());
  m.connect(newHXOutletNode, newHXOutletNode.outletPort(), exhHtRcvry, exhHtRcvry.primaryAirInletPort());

  auto fanOutletNode = fan.outletNode().get();
  model::Node newReliefNode(m);
  newReliefNode.setName(exhHtRcvryClone.nameString() + " Relief Outlet Node");

  m.connect(fanOutletNode, fanOutletNode.outletPort(), exhHtRcvryClone, exhHtRcvryClone.secondaryAirInletPort());
  m.connect(exhHtRcvryClone, exhHtRcvryClone.secondaryAirOutletPort(), newReliefNode, newReliefNode.inletPort());
}

boost::optional<model::ModelObject> ReverseTranslator::translateTrmlUnit(const pugi::xml_node& trmlUnitElement,
                                                                         openstudio::model::Model& model)
{
  boost::optional<openstudio::model::ModelObject> result;

  if( ! istringEqual(trmlUnitElement.name(),"TrmlUnit") )
  {
    return result;
  }

  // Name
  pugi::xml_node nameElement = trmlUnitElement.child("Name");
  auto name = nameElement.text().as_string();

  // AvailSchRef
  pugi::xml_node availSchRefElement = trmlUnitElement.child("AvailSchRef");
  boost::optional<model::Schedule> availSch = model.getModelObjectByName<model::Schedule>(availSchRefElement.text().as_string());

  // Type
  pugi::xml_node typeElement = trmlUnitElement.child("TypeSim");

  // PriAirFlow
  pugi::xml_node priAirFlowElement = trmlUnitElement.child("PriAirFlowMaxSim");
  pugi::xml_node priAirFlowMinElement = trmlUnitElement.child("PriAirFlowMinSim");
  boost::optional<double> primaryAirFlow;
  boost::optional<double> primaryAirFlowMin;

  if( ! autosize() )
  {
    boost::optional<double> _priAirFlow = lexicalCastToDouble(priAirFlowElement);
    if( _priAirFlow )
    {
      primaryAirFlow = unitToUnit(_priAirFlow.get(),"cfm","m^3/s").get();
    }
  }

  boost::optional<double> _priAirFlowMin = lexicalCastToDouble(priAirFlowMinElement);
  if( _priAirFlowMin )
  {
    primaryAirFlowMin = unitToUnit(_priAirFlowMin.get(),"cfm","m^3/s").get();
  }

  pugi::xml_node airSysElement = trmlUnitElement.parent();
  pugi::xml_node airSystemTypeElement;
  if (airSysElement)
  {
    airSystemTypeElement = airSysElement.child("TypeSim");
  }

  if( istringEqual("VAVNoReheatBox",typeElement.text().as_string()) )
  {
    model::Schedule schedule = model.alwaysOnDiscreteSchedule();

    model::AirTerminalSingleDuctVAVNoReheat terminal(model,schedule);

    pugi::xml_node minAirFracSchRefElement = trmlUnitElement.child("MinAirFracSchRef");
    if( boost::optional<model::Schedule> minAirFracSch = model.getModelObjectByName<model::Schedule>(minAirFracSchRefElement.text().as_string()) )
    {
      terminal.setZoneMinimumAirFlowInputMethod("Scheduled");
      terminal.setMinimumAirFlowFractionSchedule(minAirFracSch.get());
    }
    else if( primaryAirFlowMin )
    {
      terminal.setZoneMinimumAirFlowInputMethod("FixedFlowRate");
      terminal.setFixedMinimumAirFlowRate(primaryAirFlowMin.get());
    }
    else
    {
      terminal.setZoneMinimumAirFlowInputMethod("Constant");
      terminal.setConstantMinimumAirFlowFraction(0.5);
    }

    if( availSch )
    {
      terminal.setAvailabilitySchedule(availSch.get());
    }

    if( primaryAirFlow )
    {
      terminal.setMaximumAirFlowRate(primaryAirFlow.get());
    }

    auto ctrlForOAElement = trmlUnitElement.child("CtrlForOA");
    if( istringEqual("1",ctrlForOAElement.text().as_string()) ) {
      terminal.setControlForOutdoorAir(true);
    }

    result = terminal;
  }
  else if( istringEqual("VAVReheatBox",typeElement.text().as_string()) )
  {
    model::Schedule schedule = model.alwaysOnDiscreteSchedule();

    pugi::xml_node coilElement = trmlUnitElement.child("CoilHtg");

    boost::optional<model::HVACComponent> coil;

    if (coilElement)
    {
      if( auto mo = translateCoilHeating(coilElement, model) ) {
        coil = mo->optionalCast<model::HVACComponent>();
      }

    }
    if( ! coil ) { // If no coil is specified in the SDD, create an electric coil
      coil = model::CoilHeatingElectric(model,schedule);
      LOG(Warn,name << " is assigned a default electric heating coil.");
    }

    OS_ASSERT(coil);

    model::AirTerminalSingleDuctVAVReheat terminal(model,schedule,coil.get());

    pugi::xml_node minAirFracSchRefElement = trmlUnitElement.child("MinAirFracSchRef");
    if( boost::optional<model::Schedule> minAirFracSch = model.getModelObjectByName<model::Schedule>(minAirFracSchRefElement.text().as_string()) )
    {
      terminal.setZoneMinimumAirFlowInputMethod("Scheduled");
      terminal.setMinimumAirFlowFractionSchedule(minAirFracSch.get());
    }
    else if( primaryAirFlowMin )
    {
      terminal.setZoneMinimumAirFlowInputMethod("FixedFlowRate");
      terminal.setFixedMinimumAirFlowRate(primaryAirFlowMin.get());
    }
    else
    {
      terminal.setZoneMinimumAirFlowInputMethod("Constant");
      terminal.setConstantMinimumAirFlowFraction(0.2);
    }

    if( availSch )
    {
      terminal.setAvailabilitySchedule(availSch.get());
    }

    if( primaryAirFlow )
    {
      terminal.setMaximumAirFlowRate(primaryAirFlow.get());
    }

    terminal.resetMaximumFlowPerZoneFloorAreaDuringReheat();

    // ReheatCtrlMthd
    pugi::xml_node reheatCtrlMthdElement = trmlUnitElement.child("ReheatCtrlMthd");
    if( istringEqual(reheatCtrlMthdElement.text().as_string(),"DualMaximum") &&
        coil->optionalCast<model::CoilHeatingWater>() )
    {
      terminal.setDamperHeatingAction("Reverse");
    }
    else
    {
      terminal.setDamperHeatingAction("Normal");
    }

    pugi::xml_node htgAirFlowMaxElement = trmlUnitElement.child("HtgAirFlowMaxSim");
    boost::optional<double> _htgAirFlowMax = lexicalCastToDouble(htgAirFlowMaxElement);
    if( _htgAirFlowMax && primaryAirFlow )
    {
       double value = unitToUnit(_htgAirFlowMax.get(),"cfm","m^3/s").get();
       double fraction = value / primaryAirFlow.get();
       terminal.setMaximumFlowFractionDuringReheat(fraction);
    }
    else
    {
      bool found = false;

      pugi::xml_node zoneServedElement = trmlUnitElement.child("ZnServedRef");

      std::vector<pugi::xml_node> thrmlZnElements = makeVectorOfChildren(trmlUnitElement.parent().parent().parent(), "ThrmlZn");

      for (std::vector<pugi::xml_node>::size_type j = 0; j < thrmlZnElements.size(); j++ )
      {
        pugi::xml_node thrmlZnElement = thrmlZnElements[j];

        pugi::xml_node thrmlZnNameElement = thrmlZnElement.child("Name");

        if(istringEqual(thrmlZnNameElement.text().as_string(),zoneServedElement.text().as_string()))
        {
          pugi::xml_node htgDsgnMaxFlowFracElement = thrmlZnElement.child("HtgDsgnMaxFlowFrac");

          boost::optional<double> _htgDsgnMaxFlowFrac = lexicalCastToDouble(htgDsgnMaxFlowFracElement);

          if( _htgDsgnMaxFlowFrac )
          {
            terminal.setMaximumFlowFractionDuringReheat(_htgDsgnMaxFlowFrac.get());

            found = true;
          }

          break;
        }
      }

      if( ! found )
      {
        if( istringEqual(terminal.damperHeatingAction(),"Reverse") )
        {
          terminal.setMaximumFlowFractionDuringReheat(0.5);
        }
        else
        {
          terminal.setMaximumFlowFractionDuringReheat(0.2);
        }
      }
    }

    auto ctrlForOAElement = trmlUnitElement.child("CtrlForOA");
    if( istringEqual("1",ctrlForOAElement.text().as_string()) ) {
      terminal.setControlForOutdoorAir(true);
    }

    result = terminal;
  }
  else if( istringEqual("SeriesFanBox",typeElement.text().as_string()) )
  {
    model::Schedule schedule = model.alwaysOnDiscreteSchedule();

    // CoilHtg
    pugi::xml_node coilHtgElement = trmlUnitElement.child("CoilHtg");
    boost::optional<model::ModelObject> coil;
    coil = translateCoilHeating(coilHtgElement, model);
    if( ! coil ) {
      coil = model::CoilHeatingElectric(model,schedule);
      LOG(Warn,name << " is assigned a default electric heating coil.");
    }
    model::HVACComponent hvacComponentCoil = coil->cast<model::HVACComponent>();

    // Fan
    pugi::xml_node fanElement = trmlUnitElement.child("Fan");
    boost::optional<model::ModelObject> fan;
    fan = translateFan(fanElement, model);
    if( ! fan ) {
      fan = model::FanConstantVolume(model,schedule);
      LOG(Warn,name << " is assigned a default constant volume fan.");
    }
    model::HVACComponent hvacComponentFan = fan->cast<model::HVACComponent>();

    // Terminal
    model::AirTerminalSingleDuctSeriesPIUReheat terminal(model,hvacComponentFan,hvacComponentCoil);

    if( availSch )
    {
      terminal.setAvailabilitySchedule(availSch.get());
    }

    if( primaryAirFlow )
    {
      terminal.setMaximumAirFlowRate(primaryAirFlow.get());
    }

    // Maximum and Minimum Primary Air Flow Rate
    if( boost::optional<model::FanConstantVolume> constantFan = hvacComponentFan.optionalCast<model::FanConstantVolume>() )
    {
      if( boost::optional<double> flow = constantFan->maximumFlowRate() )
      {
        terminal.setMaximumPrimaryAirFlowRate(flow.get());
        if( primaryAirFlowMin )
        {
          terminal.setMinimumPrimaryAirFlowFraction(primaryAirFlowMin.get() / flow.get());
        }
      }
    }
    else if( boost::optional<model::FanVariableVolume> variableFan = hvacComponentFan.optionalCast<model::FanVariableVolume>() )
    {
      if( boost::optional<double> flow = variableFan->maximumFlowRate() )
      {
        terminal.setMaximumPrimaryAirFlowRate(flow.get());
        if( primaryAirFlowMin )
        {
          terminal.setMinimumPrimaryAirFlowFraction(primaryAirFlowMin.get() / flow.get());
        }
      }
    }

    // Hot Water Related Properties
    if( boost::optional<model::CoilHeatingWater> waterCoil = hvacComponentCoil.optionalCast<model::CoilHeatingWater>() )
    {
      terminal.setConvergenceTolerance(0.001);
      if( boost::optional<double> flow = waterCoil->maximumWaterFlowRate() )
      {
        terminal.setMaximumHotWaterorSteamFlowRate(flow.get());
      }
    }

    result = terminal;
  }
  else if( istringEqual("ParallelFanBox",typeElement.text().as_string()) )
  {
    model::Schedule schedule = model.alwaysOnDiscreteSchedule();

    // CoilHtg

    pugi::xml_node coilHtgElement = trmlUnitElement.child("CoilHtg");

    boost::optional<model::ModelObject> coil;

    coil = translateCoilHeating(coilHtgElement, model);

    if( ! coil )
    {
      coil = model::CoilHeatingElectric(model,schedule);
      LOG(Warn,name << " is assigned a default electric heating coil.");
    }

    model::HVACComponent hvacComponentCoil = coil->cast<model::HVACComponent>();

    // Fan

    pugi::xml_node fanElement = trmlUnitElement.child("Fan");

    boost::optional<model::ModelObject> fan;

    fan = translateFan(fanElement, model);

    if( ! fan ) {
      fan = model::FanConstantVolume(model,schedule);
      LOG(Warn,name << " is assigned a default constant volume fan.");
    }

    model::HVACComponent hvacComponentFan = fan->cast<model::HVACComponent>();

    // Terminal

    model::AirTerminalSingleDuctParallelPIUReheat terminal(model,schedule,hvacComponentFan,hvacComponentCoil);

    if( availSch )
    {
      terminal.setAvailabilitySchedule(availSch.get());
    }

    if( primaryAirFlow )
    {
      terminal.setMaximumPrimaryAirFlowRate(primaryAirFlow.get());
    }

    if( primaryAirFlow && primaryAirFlowMin )
    {
      terminal.setMinimumPrimaryAirFlowFraction(primaryAirFlowMin.get() / primaryAirFlow.get());
    }

    // Maximum Secondary Air Flow Rate

    if( boost::optional<model::FanConstantVolume> constantFan = hvacComponentFan.optionalCast<model::FanConstantVolume>() )
    {
      if( boost::optional<double> flow = constantFan->maximumFlowRate() )
      {
        terminal.setMaximumSecondaryAirFlowRate(flow.get());
      }
    }
    else if( boost::optional<model::FanVariableVolume> variableFan = hvacComponentFan.optionalCast<model::FanVariableVolume>() )
    {
      if( boost::optional<double> flow = variableFan->maximumFlowRate() )
      {
        terminal.setMaximumSecondaryAirFlowRate(flow.get());
      }
    }

    // Hot Water Related Properties

    if( boost::optional<model::CoilHeatingWater> waterCoil = hvacComponentCoil.optionalCast<model::CoilHeatingWater>() )
    {
      terminal.setConvergenceTolerance(0.001);

      if( boost::optional<double> flow = waterCoil->maximumWaterFlowRate() )
      {
        terminal.setMaximumHotWaterorSteamFlowRate(flow.get());
      }
    }

    // ParallelBoxFanFlowFrac
    pugi::xml_node parallelBoxFanFlowFracElement = trmlUnitElement.child("ParallelBoxFanFlowFrac");
    boost::optional<double> _parallelBoxFanFlowFrac = lexicalCastToDouble(parallelBoxFanFlowFracElement);
    if( _parallelBoxFanFlowFrac )
    {
      terminal.setFanOnFlowFraction(_parallelBoxFanFlowFrac.get());
    }

    result = terminal;
  }
  else if( istringEqual("Uncontrolled",typeElement.text().as_string()) )
  {
    model::Schedule schedule = model.alwaysOnDiscreteSchedule();

    model::AirTerminalSingleDuctConstantVolumeNoReheat terminal(model,schedule);

    if( availSch ) {
      terminal.setAvailabilitySchedule(availSch.get());
    }

    // PriAirFlow
    if( primaryAirFlow )
    {
      terminal.setMaximumAirFlowRate(primaryAirFlow.get());
    }

    result = terminal;
  }
  else
  {
    model::Schedule schedule = model.alwaysOnDiscreteSchedule();

    model::AirTerminalSingleDuctConstantVolumeNoReheat terminal(model,schedule);

    result = terminal;
  }

  if( result )
  {
    result->setName(nameElement.text().as_string());
  }

  return result;
}

boost::optional<openstudio::model::ModelObject> ReverseTranslator::translateFluidSys(
                                                  const pugi::xml_node& fluidSysElement,
                                                  openstudio::model::Model& model )
{
  boost::optional<openstudio::model::ModelObject> result;

  if( ! istringEqual(fluidSysElement.name(),"FluidSys") )
  {
    return result;
  }

  pugi::xml_node nameElement = fluidSysElement.child("Name");
  std::string plantName = nameElement.text().as_string();

  if( boost::optional<model::PlantLoop> plant = model.getModelObjectByName<model::PlantLoop>(plantName) )
  {
    return plant.get();
  }

  model::PlantLoop plantLoop(model);

  model::Node supplyInletNode = plantLoop.supplyInletNode();
  model::Node supplyOutletNode = plantLoop.supplyOutletNode();

  bool bypass = false;

  // Name

  plantLoop.setName(plantName);

  model::SizingPlant sizingPlant = plantLoop.sizingPlant();

  // Type

  auto type = fluidSysElement.child("Type").text().as_string();

  auto sizingOption = fluidSysElement.child("SizingOption").text().as_string();
  if (! std::string(sizingOption).empty()) {
    sizingPlant.setSizingOption(sizingOption);
  } else {
    sizingPlant.setSizingOption("Coincident");
  }

  // DsgnSupWtrTemp
  pugi::xml_node dsgnSupWtrTempElement = fluidSysElement.child("DsgnSupWtrTemp");
  boost::optional<double> _dsgnSupWtrTemp = lexicalCastToDouble(dsgnSupWtrTempElement);
  if( _dsgnSupWtrTemp )
  {
    double value = unitToUnit(_dsgnSupWtrTemp.get(),"F","C").get();
    sizingPlant.setDesignLoopExitTemperature(value);
  }

  // DsgnSupWtrDelT
  pugi::xml_node dsgnSupWtrDelTElement = fluidSysElement.child("DsgnSupWtrDelT");
  boost::optional<double> _dsgnSupWtrDelT = lexicalCastToDouble(dsgnSupWtrDelTElement);
  if( _dsgnSupWtrDelT )
  {
    double value = _dsgnSupWtrDelT.get() / 1.8;
    sizingPlant.setLoopDesignTemperatureDifference(value);
  }

  if( istringEqual(type,"HotWater") )
  {
    sizingPlant.setLoopType("Heating");
  }
  else if( istringEqual(type,"ChilledWater") )
  {
    sizingPlant.setLoopType("Cooling");
  }
  else if( istringEqual(type,"CondenserWater") )
  {
    sizingPlant.setLoopType("Condenser");
  }

  auto maxTemp = lexicalCastToDouble(fluidSysElement.child("MaxTemp"));
  if (maxTemp) {
    maxTemp = unitToUnit(maxTemp.get(), "F", "C").get();
    plantLoop.setMaximumLoopTemperature(maxTemp.get());
  }

  auto minTemp = lexicalCastToDouble(fluidSysElement.child("MinTemp"));
  if (minTemp) {
    minTemp = unitToUnit(minTemp.get(), "F", "C").get();
    plantLoop.setMinimumLoopTemperature(minTemp.get());
  }

  auto addBranchPump = [&](boost::optional<model::ModelObject> mo, pugi::xml_node baseElement){
    auto pumpElement = baseElement.child("Pump");
    if (pumpElement)
    {
      if( mo ) {
        if( auto inletNode = mo->optionalCast<model::Node>() ) {
          if( auto mo2 = translatePump(pumpElement, model) ) {
            if( boost::optional<model::PumpVariableSpeed> pump = mo2->optionalCast<model::PumpVariableSpeed>() ) {
              pump->addToNode(inletNode.get());
            } else if( boost::optional<model::PumpConstantSpeed> pump = mo2->optionalCast<model::PumpConstantSpeed>() ) {
              pump->addToNode(inletNode.get());
            }
          }
        }
      }
    }
  };

  // Boilers
  std::vector<pugi::xml_node> boilerElements = makeVectorOfChildren(fluidSysElement, "Blr");
  for (std::vector<pugi::xml_node>::size_type i = 0; i < boilerElements.size(); i++) {
    auto boilerElement = boilerElements[i];

    if( boost::optional<model::ModelObject> mo = translateBoiler(boilerElement, model) ) {
      auto boiler = mo->cast<model::BoilerHotWater>();
      plantLoop.addSupplyBranchForComponent(boiler);
      addBranchPump(boiler.inletModelObject(),boilerElement);
    }

    auto hasBypss = boilerElement.child("HasBypass").text().as_int();
    if( hasBypss > 0 ) {
      bypass = true;
    }
  }

  // ThrmlEngyStor
  const auto thrmlEngyStorElement = fluidSysElement.child("ThrmlEngyStor");
  const auto thrmlEngyStorType = thrmlEngyStorElement.child("Type").text().as_string();

  // These values will be used later when chilled water storage controls are setup for the plant
  boost::optional<model::ThermalStorageChilledWaterStratified> chilledWaterStorage;
  std::string thermalStorageDischargePriority;
  boost::optional<model::ScheduleRuleset> tesSchedule;
  double thermalStorageTankSetptTemp = 7.5;

  // These values will be used later when ice storage controls are setup for the plant
  boost::optional<model::ThermalStorageIceDetailed> iceStorage;
  boost::optional<std::string> iceStorageChlrRef;
  double iceChilledWaterTemp = 7.5;
  double iceChargeTemp = -5.0;
  boost::optional<model::Schedule> iceAvailabilityLowTempTurnOff;
  boost::optional<model::Schedule> iceTankAvailabilitySchedule;
  boost::optional<model::Schedule> icePrimaryChillerSetpointSchedule;
  boost::optional<model::Schedule> iceChargingChillerSetpointSchedule;

  if (thrmlEngyStorElement && istringEqual(thrmlEngyStorType, "Ice")) {
    if( auto mo = translateIceThrmlEngyStor(thrmlEngyStorElement, model) ) {
      iceStorage = mo->cast<model::ThermalStorageIceDetailed>();
      iceStorageChlrRef = thrmlEngyStorElement.child("ChlrRef").text().as_string();

      plantLoop.setFluidType("EthyleneGlycol");
      plantLoop.setGlycolConcentration(40);

      const auto inletTempElement = thrmlEngyStorElement.child("InletTemp");
      if (inletTempElement) {
        iceChargeTemp = unitToUnit(inletTempElement.text().as_double(), "F", "C").get();
      }

      const auto outletTempElement = thrmlEngyStorElement.child("OutletTemp");
      if (outletTempElement) {
        iceChilledWaterTemp = unitToUnit(outletTempElement.text().as_double(), "F", "C").get();
      }

      // Build key schedules
      // first parse time inputs
      int startTime = 9; 
      int startMonth = 5; 
      int startDayOfMonth = 15;
      int stopTime = 22; 
      int stopMonth = 9;
      int stopDayOfMonth = 30; 

      const auto startTimeElement = thrmlEngyStorElement.child("StartTime");
      if (startTimeElement) {
        startTime = startTimeElement.text().as_int();
      }

      const auto startDateElement = thrmlEngyStorElement.child("StartDate");
      if (startDateElement) {
        const std::string startDateString = startDateElement.text().as_string();
        const auto splitPosition = startDateString.find("/");
        const std::string startMonthString = startDateString.substr(0, splitPosition);
        const std::string startDayOfMonthString = startDateString.substr(splitPosition + 1);
        startMonth = stoi(startMonthString);
        startDayOfMonth= stoi(startDayOfMonthString);
      }

      const auto stopTimeElement = thrmlEngyStorElement.child("StopTime");
      if (stopTimeElement) {
        stopTime = stopTimeElement.text().as_int();
      }

      const auto stopDateElement = thrmlEngyStorElement.child("StopDate");
      if (stopDateElement) {
        const std::string stopDateString = stopDateElement.text().as_string();
        const auto splitPosition = stopDateString.find("/");
        const std::string stopMonthString = stopDateString.substr(0, splitPosition);
        const std::string stopDayOfMonthString = stopDateString.substr(splitPosition + 1);
        stopMonth = stoi(stopMonthString);
        stopDayOfMonth= stoi(stopDayOfMonthString);
      }

      // AvailabilityManagerLowTemperatureTurnOff schedule for ice storage
      {
        model::ScheduleRuleset schedule(model);
        schedule.setName(plantLoop.nameString() + " Low Temp TurnOff");
        auto defaultOff = schedule.defaultDaySchedule();
        schedule.defaultDaySchedule().addValue(Time(1, 0), 0.0);

        model::ScheduleRule rule(schedule);
        rule.setApplyAllDays(false);
        rule.setApplyWeekdays(true);
        auto daySchedule = rule.daySchedule();
        daySchedule.addValue(Time(0, startTime), 1.0);
        daySchedule.addValue(Time(0, stopTime), 0.0);
        daySchedule.addValue(Time(1, 0), 1.0);

        schedule.setSummerDesignDaySchedule(daySchedule);

        iceAvailabilityLowTempTurnOff = schedule;
      }

      // Ice Tank Availability
      {
        model::ScheduleRuleset schedule(model);
        schedule.setName(plantLoop.nameString() + " Ice Tank Availability");
        schedule.defaultDaySchedule().addValue(Time(1, 0), 0.0);

        model::ScheduleRule rule(schedule);
        rule.setStartDate(Date(startMonth, startDayOfMonth));
        rule.setStartDate(Date(stopMonth, stopDayOfMonth));
        rule.daySchedule().addValue(Time(1, 0), 1.0);

        iceTankAvailabilitySchedule = schedule;
        iceStorage->setAvailabilitySchedule(schedule);
      }

      // Main chiller setpoint schedule
      {
        model::ScheduleRuleset schedule(model);
        schedule.setName(plantLoop.nameString() + " Chiller Setpoint");
        schedule.defaultDaySchedule().addValue(Time(1, 0), iceChilledWaterTemp);

        model::ScheduleRule rule(schedule);
        rule.setStartDate(Date(startMonth, startDayOfMonth));
        rule.setStartDate(Date(stopMonth, stopDayOfMonth));
        rule.daySchedule().addValue(Time(0, startTime), 98.9);
        rule.daySchedule().addValue(Time(0, stopTime), iceChilledWaterTemp);
        rule.daySchedule().addValue(Time(1, 0), 98.9);

        icePrimaryChillerSetpointSchedule = schedule;
      }

      // Charging chiller setpoint schedule
      {
        model::ScheduleRuleset schedule(model);
        schedule.setName(plantLoop.nameString() + " Charging Setpoint");
        schedule.defaultDaySchedule().addValue(Time(1, 0), 98.9);

        model::ScheduleRule rule(schedule);
        rule.setStartDate(Date(startMonth, startDayOfMonth));
        rule.setStartDate(Date(stopMonth, stopDayOfMonth));
        rule.daySchedule().addValue(Time(0, startTime), iceChargeTemp);
        rule.daySchedule().addValue(Time(0, stopTime), 98.9);
        rule.daySchedule().addValue(Time(1, 0), iceChargeTemp);

        iceChargingChillerSetpointSchedule = schedule;
      }
    }
  } else if (thrmlEngyStorElement) {
    if( auto mo = translateChilledWaterThrmlEngyStor(thrmlEngyStorElement, model) ) {
      chilledWaterStorage = mo->cast<model::ThermalStorageChilledWaterStratified>();
      plantLoop.addSupplyBranchForComponent(chilledWaterStorage.get());
      addBranchPump(chilledWaterStorage->supplyInletModelObject(),thrmlEngyStorElement);
      plantLoop.addDemandBranchForComponent(chilledWaterStorage.get());

      thermalStorageDischargePriority = thrmlEngyStorElement.child("DischrgPriority").text().as_string();
      pugi::xml_node tankSetptTempElement = thrmlEngyStorElement.child("TankSetptTemp");
      boost::optional<double> _tankSetptTemp = lexicalCastToDouble(tankSetptTempElement);
      if (_tankSetptTemp) {
        thermalStorageTankSetptTemp = unitToUnit(_tankSetptTemp.get(),"F","C").get();
      } else {
        LOG(Warn, "Missing TankSetptTemp (or bad cast to double) for '" << chilledWaterStorage->briefDescription()
               << ", defaulting to " << thermalStorageTankSetptTemp << " C.");
      }

      // charging scheme, which is a component setpoint scheme so we add SPMs
      tesSchedule = model::ScheduleRuleset(model);
      tesSchedule->setName(plantLoop.nameString() + "TES SPM Schedule");
      tesSchedule->defaultDaySchedule().addValue(Time(1.0),thermalStorageTankSetptTemp);

      chilledWaterStorage->setSetpointTemperatureSchedule(tesSchedule.get());

      {
        auto schRef = thrmlEngyStorElement.child("ChlrOnlySchRef").text().as_string();
        if( auto sch = model.getModelObjectByName<model::Schedule>(schRef) ) {
          plantLoop.setPlantEquipmentOperationCoolingLoadSchedule(sch.get());
        }
      }

      {
        auto schRef = thrmlEngyStorElement.child("DischrgSchRef").text().as_string();
        if( auto sch = model.getModelObjectByName<model::Schedule>(schRef) ) {
          plantLoop.setPrimaryPlantEquipmentOperationSchemeSchedule(sch.get());
        }
      }

      {
        auto schRef = thrmlEngyStorElement.child("ChrgSchRef").text().as_string();
        if( auto sch = model.getModelObjectByName<model::Schedule>(schRef) ) {
          plantLoop.setComponentSetpointOperationSchemeSchedule(sch.get());
        }
      }
    }
  }

  // Chillers

  // Keep a map of chiller to a flag indicating if it should be used
  // in thermal energy storage discharge
  std::vector<std::pair<model::ModelObject,bool> > enableOnThrmlEngyStorDischargeMap;
  std::vector<pugi::xml_node> chillerElements = makeVectorOfChildren(fluidSysElement, "Chlr");
  for (std::vector<pugi::xml_node>::size_type i = 0; i < chillerElements.size(); i++) {
    auto chillerElement = chillerElements[i];

    if( auto mo = translateChiller(chillerElement, model) ) {
      auto chiller = mo->cast<model::WaterToWaterComponent>();
      plantLoop.addSupplyBranchForComponent(chiller);
      addBranchPump(chiller.supplyInletModelObject(),chillerElement);

      auto evapHasBypassElement = chillerElement.child("EvapHasBypass");
      if( evapHasBypassElement.text().as_int() > 0 ) {
        bypass = true;
      }

      if( chilledWaterStorage ) {
        if( chillerElement.child("EnableOnThrmlEngyStorDischrg").text().as_int() == 1 ) {
          enableOnThrmlEngyStorDischargeMap.push_back(std::make_pair(chiller,true));
        } else {
          enableOnThrmlEngyStorDischargeMap.push_back(std::make_pair(chiller,false));
        }
      }
    }
  }

  // HtRej
  std::vector<pugi::xml_node> htRejElements = makeVectorOfChildren(fluidSysElement, "HtRej");
  for (std::vector<pugi::xml_node>::size_type i = 0; i < htRejElements.size(); i++) {
    auto htRejElement = htRejElements[i];

    if( auto mo = translateHtRej(htRejElement, model) ) {
      auto tower = mo->cast<model::StraightComponent>();
      plantLoop.addSupplyBranchForComponent(tower);
      addBranchPump(tower.inletModelObject(),htRejElement);
    }

    auto hasBypss = htRejElement.child("HasBypass").text().as_int();
    if( hasBypss > 0 ) {
      bypass = true;
    }
  }

  // HX
  std::vector<pugi::xml_node> hxElements = makeVectorOfChildren(fluidSysElement, "HX");
  for (std::vector<pugi::xml_node>::size_type i = 0; i < hxElements.size(); i++) {
    auto hxElement = hxElements[i];

    if( auto mo = translateHX(hxElement, model) ) {
      auto hx = mo->cast<model::HeatExchangerFluidToFluid>();
      auto economizerIntegration = hxElement.child("EconoIntegration").text().as_string();
      if( istringEqual("Nonintegrated",economizerIntegration) ) {
        plantLoop.addSupplyBranchForComponent(hx);
      } else {
        auto node = plantLoop.supplyInletNode();
        hx.addToNode(node);
      }
    }
  }

  // Water Heaters
  std::vector<pugi::xml_node> wtrHtrElements = makeVectorOfChildren(fluidSysElement, "WtrHtr");

  for (std::vector<pugi::xml_node>::size_type i = 0; i < wtrHtrElements.size(); i++)
  {
    pugi::xml_node wtrHtrElement = wtrHtrElements[i];

    if( boost::optional<model::ModelObject> mo = translateWtrHtr(wtrHtrElement, model) )
    {
      plantLoop.addSupplyBranchForComponent(mo->cast<model::HVACComponent>());

      auto waterToWater = mo->optionalCast<model::WaterToWaterComponent>();
      if (waterToWater) {
        addBranchPump(waterToWater->supplyInletModelObject(), wtrHtrElement);
      }

      auto straight = mo->optionalCast<model::StraightComponent>();
      if (straight) {
        addBranchPump(straight->inletModelObject(), wtrHtrElement);
      }
    }
  }

  // Add a default bypass
  if(bypass)
  {
    model::PipeAdiabatic pipe(model);

    plantLoop.addSupplyBranchForComponent(pipe);
  }

  // Add a default hot water heater for servicehotwater systems
  if( openstudio::istringEqual(type, "servicehotwater") )
  {
    boost::optional<model::WaterHeaterMixed> waterHeater;

    boost::optional<model::Schedule> setpointSchedule;

    std::vector<model::ModelObject> waterHeaters = plantLoop.supplyComponents(model::WaterHeaterMixed::iddObjectType());

    if( waterHeaters.size() > 0 )
    {
      waterHeater = waterHeaters.front().cast<model::WaterHeaterMixed>();
    }

    if( waterHeater )
    {
      setpointSchedule = waterHeater->setpointTemperatureSchedule();
    }

    if( setpointSchedule )
    {
      model::SetpointManagerScheduled spm(model,setpointSchedule.get());
      spm.setName(plantLoop.nameString() + " Setpoint Manager");
      spm.addToNode(supplyOutletNode);
    }

    if( waterHeaters.size() > 0 )
    {
      model::PumpVariableSpeed pumpVariableSpeed(model);
      pumpVariableSpeed.setName(plantLoop.nameString() + " Pump");
      model::Node supplyInletNode = plantLoop.supplyInletNode();
      pumpVariableSpeed.addToNode(supplyInletNode);
      pumpVariableSpeed.setRatedPumpHead(0.0);
    }
  }


  std::vector<pugi::xml_node> fluidSegElements = makeVectorOfChildren(fluidSysElement, "FluidSeg");

  for (std::vector<pugi::xml_node>::size_type i = 0; i < fluidSegElements.size(); i++)
  {
    pugi::xml_node fluidSegElement = fluidSegElements[i];

    pugi::xml_node fluidSegTypeElement = fluidSegElement.child("Type");

    // Translate Secondary Supply
    if( openstudio::istringEqual(fluidSegTypeElement.text().as_string(), "secondarysupply") )
    {
      pugi::xml_node pumpElement = fluidSegElement.child("Pump");

      if( pumpElement && !openstudio::istringEqual(type, "ServiceHotWater") )
      {
        boost::optional<model::ModelObject> mo = translatePump(pumpElement, model);

        if( mo )
        {
          model::Node demandInletNode = plantLoop.demandInletNode();

          mo->cast<model::HVACComponent>().addToNode(demandInletNode);

          plantLoop.setCommonPipeSimulation("CommonPipe");
        }
      }
    }

    // If CommonPipeSim is defined then respect it
    auto commonPipeSimElement = fluidSysElement.child("CommonPipeSim");
    if (commonPipeSimElement) {
      plantLoop.setCommonPipeSimulation(commonPipeSimElement.text().as_string());
    }

    // Translate Primary Supply

    if( openstudio::istringEqual(fluidSegTypeElement.text().as_string(), "primaryreturn") ||
        openstudio::istringEqual(fluidSegTypeElement.text().as_string(), "primarysupply")
      )
    {
      pugi::xml_node pumpElement = fluidSegElement.child("Pump");

      if (pumpElement)
      {
        boost::optional<model::ModelObject> mo = translatePump(pumpElement, model);

        if( mo )
        {
          model::Node supplyInletNode = plantLoop.supplyInletNode();

          mo->cast<model::HVACComponent>().addToNode(supplyInletNode);
        }
      }
    }
  }

  // Translate ProcLd elements to LoadProfilePlant
  const auto procLdElements = makeVectorOfChildren(fluidSysElement, "ProcLd");
  for (const auto & procLdElement : procLdElements) {
    auto loadProfilePlant = translateProcLd(procLdElement, model);
    if (loadProfilePlant) {
      plantLoop.addDemandBranchForComponent(loadProfilePlant->cast<model::HVACComponent>());
    }
  }

  // Translate PlantLoop::MaximumLoopFlowRate and MinimumLoopFlowRate
  if( ! autosize() )
  {
    std::vector<model::ModelObject> constantPumps;
    constantPumps = plantLoop.supplyComponents(plantLoop.supplyInletNode(),
                                       plantLoop.supplySplitter(),
                                       model::PumpConstantSpeed::iddObjectType());

    std::vector<model::ModelObject> variablePumps;
    variablePumps = plantLoop.supplyComponents(plantLoop.supplyInletNode(),
                                       plantLoop.supplySplitter(),
                                       model::PumpVariableSpeed::iddObjectType());
    if( constantPumps.size() > 0 )
    {
      model::PumpConstantSpeed pump = constantPumps.front().cast<model::PumpConstantSpeed>();
      if( boost::optional<double> value = pump.ratedFlowRate() )
      {
        plantLoop.setMaximumLoopFlowRate(value.get());
        //plantLoop.setMinimumLoopFlowRate(value.get());
      }
    }
    else if( variablePumps.size() > 0 )
    {
      model::PumpVariableSpeed pump = variablePumps.front().cast<model::PumpVariableSpeed>();
      if( boost::optional<double> value = pump.ratedFlowRate() )
      {
        plantLoop.setMaximumLoopFlowRate(value.get());
      }
      //if( boost::optional<double> value = pump.minimumFlowRate() )
      //{
      //  plantLoop.setMinimumLoopFlowRate(value.get());
      //}
    }
    else
    {
      std::vector<double> minimums;

      double flowRate = 0.0;

      constantPumps = plantLoop.supplyComponents(plantLoop.supplySplitter(),
                                         plantLoop.supplyMixer(),
                                         model::PumpConstantSpeed::iddObjectType());
      if( constantPumps.size() > 0 )
      {
        for( auto it = constantPumps.begin();
             it != constantPumps.end();
             ++it )
        {
          if( boost::optional<double> ratedFlowRate = it->cast<model::PumpConstantSpeed>().ratedFlowRate() )
          {
            flowRate = flowRate + ratedFlowRate.get();

            minimums.push_back(ratedFlowRate.get());
          }
        }
      }

      variablePumps = plantLoop.supplyComponents(plantLoop.supplySplitter(),
                                                 plantLoop.supplyMixer(),
                                                 model::PumpVariableSpeed::iddObjectType());
      if( variablePumps.size() > 0 )
      {
        for( auto it = variablePumps.begin();
             it != variablePumps.end();
             ++it )
        {
          if( boost::optional<double> ratedFlowRate = it->cast<model::PumpVariableSpeed>().ratedFlowRate() )
          {
            flowRate = flowRate + ratedFlowRate.get();
          }
          if( boost::optional<double> minimumFlowRate = it->cast<model::PumpVariableSpeed>().minimumFlowRate() )
          {
            minimums.push_back(minimumFlowRate.get());
          }
        }
      }

      if( ! equal<double>(flowRate,0.0) )
      {
        plantLoop.setMaximumLoopFlowRate(flowRate);
      }

      //if( ! minimums.empty() )
      //{
      //  plantLoop.setMinimumLoopFlowRate(minimum(createVector(minimums)));
      //}
    }
  }

  // Plant Operation Schemes

  std::vector<double> ldRngLims;
  typedef std::vector<std::string> EquipmentList;
  std::vector<EquipmentList> equipmentLists;

  // Lambda to sort two pugi::xml_node according to their 'index' attribute
  auto sortByIndex = [&plantName](const pugi::xml_node& lhs, const::pugi::xml_node& rhs) {
    boost::optional<unsigned> _lhs_index = lexicalCastToUnsigned(lhs.attribute("index"));
    boost::optional<unsigned> _rhs_index = lexicalCastToUnsigned(rhs.attribute("index"));
    if (_lhs_index && _rhs_index) {
      return _lhs_index.get() < _rhs_index.get();
    } else if (_lhs_index) {
      LOG(Error, "Seems like some of the 'LdRngLim' for FluidSys '" << plantName << "' are missing the 'index' attribute.");
      return true;
    } else {
      LOG(Error, "Seems like some of the 'LdRngLim' for FluidSys '" << plantName << "' are missing the 'index' attribute.");
      return false;
    }
  };

  std::vector<pugi::xml_node> ldRngLimElements = makeVectorOfChildren(fluidSysElement, "LdRngLim");
  // Sort by their index attribute
  std::sort(ldRngLimElements.begin(), ldRngLimElements.end(), sortByIndex);

  // Get the range limits
  for (const pugi::xml_node& rngElement: ldRngLimElements) {
    boost::optional<double> _value = lexicalCastToDouble(rngElement);
    if( _value ) {
      double value = unitToUnit(_value.get(),"Btu/h","W").get();
      ldRngLims.push_back(value);
    }
  }


  // Try to get an equipment list for each load range
  for( std::vector<double>::size_type i = 0; i < ldRngLims.size(); ++i ) {
    // Create a tag like 'EqpList1Name'
    std::string nodeName = "EqpList" + openstudio::string_conversions::number(std::int32_t(i + 1)) + "Name";

    // Get the correspond children
    std::vector<pugi::xml_node> eqpListNameElements = makeVectorOfChildren(fluidSysElement, nodeName.c_str());

    if (eqpListNameElements.empty()) {
      LOG(Debug, "Couldn't find any '" << nodeName << "' for FluidSys '" << plantName << "'.");
    }

    // Create the plantEquipmentList, regardless of whether we found equipment to put on it
    // (assume if not there, means there's no equipment which is fine)
    EquipmentList equipmentList;

    // Sort them by index too so we push them in the right order
    std::sort(eqpListNameElements.begin(), eqpListNameElements.end(), sortByIndex);

    for (const auto& eqpListNameElement: eqpListNameElements) {
      equipmentList.push_back(eqpListNameElement.text().as_string());
    }

    // Push the equipment list for that range to the vector
    equipmentLists.push_back(equipmentList);
  }

  if( ! ldRngLims.empty() ) {
    auto allPlantComponents = plantLoop.supplyComponents();

    OS_ASSERT(ldRngLims.size() == equipmentLists.size());

    boost::optional<model::PlantEquipmentOperationRangeBasedScheme> scheme;

    if( istringEqual(type,"ChilledWater") ) {
      model::PlantEquipmentOperationCoolingLoad coolingScheme(model);
      coolingScheme.setName(plantLoop.nameString() + " cooling operation scheme");
      scheme = coolingScheme;
      plantLoop.setPlantEquipmentOperationCoolingLoad(coolingScheme);
    } else if(istringEqual(type,"HotWater")) {
      model::PlantEquipmentOperationHeatingLoad heatingScheme(model);
      heatingScheme.setName(plantLoop.nameString() + " heating operation scheme");
      scheme = heatingScheme;
      plantLoop.setPlantEquipmentOperationHeatingLoad(heatingScheme);
    }

    if( scheme ) {
      for( std::vector<double>::size_type i = 0; i < ldRngLims.size(); ++i ) {
        std::vector<model::HVACComponent> equipment;
        auto ldRngLim = ldRngLims[i];
        auto equipmentList = equipmentLists[i];

        for( auto & name : equipmentList ) {
          for( const auto & comp : allPlantComponents ) {
            if( istringEqual(name, comp.nameString()) ) {
              equipment.push_back(comp.cast<model::HVACComponent>());
              break;
            }
          }
        }

        if( i == (ldRngLims.size() - 1) ) {
          scheme->addLoadRange(1E8,equipment);
        } else {
          scheme->addLoadRange(ldRngLim,equipment);
        }
      }
    }
  }

  // Chilled Water Thermal Energy Storage needs special operation schemes
  if( chilledWaterStorage ) {
    // The end result should be three operations schemes
    // charging and standby, discharging, charging

    // plantLoop.plantEquipmentOperationCoolingLoad would exist because of load range spec in the SDD file
    // Or in other words standbyScheme would be the SDD controlled scheme, translated before this point
    auto standbyScheme = plantLoop.plantEquipmentOperationCoolingLoad();
    if( ! standbyScheme ) {
      standbyScheme = model::PlantEquipmentOperationCoolingLoad(model);
      standbyScheme->setName(plantLoop.nameString() + " TES Standby Scheme");
      plantLoop.setPlantEquipmentOperationCoolingLoad(standbyScheme.get());
      for( const auto & chillerItem : enableOnThrmlEngyStorDischargeMap ) {
        standbyScheme->addEquipment(chillerItem.first.cast<model::HVACComponent>());
      }
    }

    OS_ASSERT(standbyScheme);

    auto dischargingScheme = standbyScheme->clone(model).cast<model::PlantEquipmentOperationCoolingLoad>();
    plantLoop.setPrimaryPlantEquipmentOperationScheme(dischargingScheme);
    dischargingScheme.setName(plantLoop.nameString() + " TES Discharging Scheme");
    // Need to make sure that each chiller is allowed for discharge
    // Remove it from the scheme if not enabled for discharge
    // (We will readd it after the storage)
    for( const auto & chillerItem : enableOnThrmlEngyStorDischargeMap ) {
      if( ! chillerItem.second ) {
        dischargingScheme.removeEquipment(chillerItem.first.cast<model::HVACComponent>());
      }
    }

    if( istringEqual(thermalStorageDischargePriority,"Chiller") ) {
      dischargingScheme.addEquipment(chilledWaterStorage.get());
    } else {
      auto upperLimit = dischargingScheme.maximumUpperLimit();
      auto equipment = dischargingScheme.equipment(upperLimit);
      equipment.insert(equipment.begin(),chilledWaterStorage.get());
      dischargingScheme.replaceEquipment(upperLimit,equipment);
    }

    // Add / readd the disabled chillers after the storage
    for( const auto & chillerItem : enableOnThrmlEngyStorDischargeMap ) {
      if( ! chillerItem.second ) {
        dischargingScheme.addEquipment(chillerItem.first.cast<model::HVACComponent>());
      }
    }

    if( tesSchedule ) {
      auto chillers = subsetCastVector<model::ChillerElectricEIR>(plantLoop.supplyComponents(model::ChillerElectricEIR::iddObjectType()));
      for( auto & chiller : chillers ) {
        model::SetpointManagerScheduled spm(model,tesSchedule.get());
        auto node = chiller.supplyOutletModelObject()->cast<model::Node>();
        spm.addToNode(node);
      }
    }
  }

  // TempCtrl
  pugi::xml_node tempCtrlElement = fluidSysElement.child("TempCtrl");

  // Fixed
  if( istringEqual(tempCtrlElement.text().as_string(),"Fixed") )
  {
    pugi::xml_node fixedSupTempElement = fluidSysElement.child("FixedSupTemp");

    double fixedSupTemp = 21.1;

    boost::optional<double> _fixedSupTemp = lexicalCastToDouble(fixedSupTempElement);

    if( _fixedSupTemp )
    {
      fixedSupTemp = unitToUnit(_fixedSupTemp.get(),"F","C").get();
    }
    else
    {
      pugi::xml_node dsgnSupWtrTempElement = fluidSysElement.child("DsgnSupWtrTemp");
      boost::optional<double> _dsgnSupWtrTemp = lexicalCastToDouble(dsgnSupWtrTempElement);
      if( _dsgnSupWtrTemp )
      {
        fixedSupTemp = unitToUnit(_dsgnSupWtrTemp.get(),"F","C").get();

        LOG(Warn,plantLoop.name().get() << " Using DsgnSupWtrTemp for fixed supply temperature");
      }
      else
      {
        LOG(Error,plantLoop.name().get() << " Control type is fixed, but a valid temperature was not provided");
      }
    }

    model::ScheduleRuleset schedule(model);

    schedule.setName(plantLoop.name().get() + " Supply Temp Schedule");

    model::ScheduleDay scheduleDay = schedule.defaultDaySchedule();

    scheduleDay.addValue(Time(1.0),fixedSupTemp);

    model::SetpointManagerScheduled spm(model,schedule);

    spm.addToNode(supplyOutletNode);
  }
  else if( istringEqual(tempCtrlElement.text().as_string(),"Scheduled") )
  {
    pugi::xml_node tempSetPtSchRefElement = fluidSysElement.child("TempSetptSchRef");

    boost::optional<model::Schedule> schedule = model.getModelObjectByName<model::Schedule>(tempSetPtSchRefElement.text().as_string());

    if( ! schedule )
    {
      LOG(Error,plantLoop.name().get() << " Control type is scheduled, but a valid schedule could not be found.");

      model::ScheduleRuleset schedule(model);

      schedule.setName(plantLoop.name().get() + " Supply Temp Schedule");

      model::ScheduleDay scheduleDay = schedule.defaultDaySchedule();

      scheduleDay.addValue(Time(1.0),21.1);
    }

    model::SetpointManagerScheduled spm(model,schedule.get());

    spm.addToNode(supplyOutletNode);
  }
  else if( istringEqual(tempCtrlElement.text().as_string(),"WetBulbReset") )
  {
    model::SetpointManagerFollowOutdoorAirTemperature spm(model);

    spm.setReferenceTemperatureType("OutdoorAirWetBulb");

    spm.addToNode(supplyOutletNode);

    boost::optional<double> rstSupHi;
    boost::optional<double> rstSupLow;

    // RstSupHi
    pugi::xml_node rstSupHiElement = fluidSysElement.child("RstSupHi");
    boost::optional<double> _rstSupHi = lexicalCastToDouble(rstSupHiElement);
    if( _rstSupHi )
    {
      rstSupHi = unitToUnit(_rstSupHi.get(),"F","C").get();
    }

    // RstSupLow
    pugi::xml_node rstSupLoElement = fluidSysElement.child("RstSupLow");
    boost::optional<double> _rstSupLo = lexicalCastToDouble(rstSupLoElement);
    if( _rstSupLo )
    {
      rstSupLow = unitToUnit(_rstSupLo.get(),"F","C").get();
    }

    if( rstSupLow && rstSupHi )
    {
      spm.setMinimumSetpointTemperature(rstSupLow.get());

      spm.setMaximumSetpointTemperature(rstSupHi.get());
    }

    // WetBulbApproach
    pugi::xml_node wetBulbApproachElement = fluidSysElement.child("WetBulbApproach");
    boost::optional<double> _wetBulbApproach = lexicalCastToDouble(wetBulbApproachElement);
    if( _wetBulbApproach )
    {
      spm.setOffsetTemperatureDifference(_wetBulbApproach.get() * 5.0 / 9.0);
    }
  }
  else if( istringEqual(tempCtrlElement.text().as_string(),"OutsideAirReset") )
  {
    model::SetpointManagerOutdoorAirReset spm(model);

    spm.addToNode(supplyOutletNode);

    boost::optional<double> rstSupHi;
    boost::optional<double> rstSupLow;
    boost::optional<double> rstOutdrHi;
    boost::optional<double> rstOutdrLow;

    // RstSupHi
    pugi::xml_node rstSupHiElement = fluidSysElement.child("RstSupHi");
    boost::optional<double> _rstSupHi = lexicalCastToDouble(rstSupHiElement);
    if( _rstSupHi )
    {
      rstSupHi = unitToUnit(_rstSupHi.get(),"F","C").get();
    }

    // RstSupLow
    pugi::xml_node rstSupLoElement = fluidSysElement.child("RstSupLow");
    boost::optional<double> _rstSupLo = lexicalCastToDouble(rstSupLoElement);
    if( _rstSupLo )
    {
      rstSupLow = unitToUnit(_rstSupLo.get(),"F","C").get();
    }

    // RstOutdrHi
    pugi::xml_node rstOutDrHiElement = fluidSysElement.child("RstOutdrHi");
    boost::optional<double> _rstOutDrHi = lexicalCastToDouble(rstOutDrHiElement);
    if( _rstOutDrHi )
    {
      rstOutdrHi = unitToUnit(_rstOutDrHi.get(),"F","C").get();
    }

    // RstOutdrLow
    pugi::xml_node rstOutDrLoElement = fluidSysElement.child("RstOutdrLow");
    boost::optional<double> _rstOutDrLo = lexicalCastToDouble(rstOutDrLoElement);
    if( _rstOutDrLo )
    {
      rstOutdrLow = unitToUnit(_rstOutDrLo.get(),"F","C").get();
    }

    if( rstSupHi && rstSupLow && rstOutdrHi && rstOutdrLow )
    {
      if( rstOutdrHi > rstOutdrLow )
      {
        spm.setOutdoorHighTemperature(rstOutdrHi.get());
        spm.setSetpointatOutdoorHighTemperature(rstSupHi.get());
        spm.setOutdoorLowTemperature(rstOutdrLow.get());
        spm.setSetpointatOutdoorLowTemperature(rstSupLow.get());
      }
      else
      {
        spm.setOutdoorHighTemperature(rstOutdrLow.get());
        spm.setSetpointatOutdoorHighTemperature(rstSupLow.get());
        spm.setOutdoorLowTemperature(rstOutdrHi.get());
        spm.setSetpointatOutdoorLowTemperature(rstSupHi.get());
      }
    }
    else
    {
      LOG(Warn,  plantName << " requests OA reset control, but does not define setpoints."
        << "  Using OpenStudio defaults.");
    }
  }
  else if( istringEqual(tempCtrlElement.text().as_string(),"LoadReset") )
  {
    pugi::xml_node dsgnSupWtrTempElement = fluidSysElement.child("DsgnSupWtrTemp");
    boost::optional<double> _dsgnSupWtrTemp = lexicalCastToDouble(dsgnSupWtrTempElement);
    if( _dsgnSupWtrTemp )
    {
      double value = unitToUnit(_dsgnSupWtrTemp.get(),"F","C").get();

      model::ScheduleRuleset schedule(model);

      schedule.setName(plantLoop.name().get() + " Supply Temp Schedule");

      model::ScheduleDay scheduleDay = schedule.defaultDaySchedule();

      scheduleDay.addValue(Time(1.0),value);

      model::SetpointManagerScheduled spm(model,schedule);

      spm.addToNode(supplyOutletNode);

      LOG(Warn,plantLoop.name().get() << " Using DsgnSupWtrTemp for LoadReset temperature control.  This control scheme is not fully implemented.");
    }
    else
    {
      LOG(Error,plantLoop.name().get() << " no setpoint for LoadReset temperature control.");
    }
  }
  else if( istringEqual(tempCtrlElement.text().as_string(),"FixedDualSetpoint") )
  {
    pugi::xml_node fixedSupTempElement = fluidSysElement.child("FixedSupTemp");
    double fixedSupTemp = 10.0;
    boost::optional<double> _fixedSupTemp = lexicalCastToDouble(fixedSupTempElement);
    if( _fixedSupTemp )
    {
      fixedSupTemp = unitToUnit(_fixedSupTemp.get(),"F","C").get();
    }

    pugi::xml_node htgFixedSupTempElement = fluidSysElement.child("HtgFixedSupTemp");
    double htgFixedSupTemp = 26.7;
    boost::optional<double> _htgFixedSupTemp = lexicalCastToDouble(htgFixedSupTempElement);
    if( _htgFixedSupTemp )
    {
      htgFixedSupTemp = unitToUnit(_htgFixedSupTemp.get(),"F","C").get();
    }

    model::ScheduleRuleset schedule(model);
    schedule.setName(plantLoop.name().get() + " Supply Temp Schedule");
    model::ScheduleDay scheduleDay = schedule.defaultDaySchedule();
    scheduleDay.addValue(Time(1.0),fixedSupTemp);

    model::ScheduleRuleset heatingSchedule(model);
    heatingSchedule.setName(plantLoop.name().get() + " Heating Supply Temp Schedule");
    model::ScheduleDay heatingScheduleDay = heatingSchedule.defaultDaySchedule();
    heatingScheduleDay.addValue(Time(1.0),htgFixedSupTemp);

    const auto oldFixedDualSetptCtrl = fluidSysElement.child("OldFixedDualSetptCtrl").text().as_int(0);

    if (oldFixedDualSetptCtrl == 0) {
      model::SetpointManagerScheduledDualSetpoint spm(model);
      spm.setHighSetpointSchedule(schedule);
      spm.setLowSetpointSchedule(heatingSchedule);
      spm.addToNode(supplyOutletNode);
    } else {
      // This is the original implementation of FixedDualSetpoint,
      // which is being preserved for backwards compitibility.
      // The new approach is more simple, but wasn't possible when FixedDualSetpoint was originally implemented
      const auto addDualSPM = [&](const std::string & name, model::Node & node) {
        model::SetpointManagerScheduledDualSetpoint spm(model);
        spm.setHighSetpointSchedule(schedule);
        spm.setLowSetpointSchedule(heatingSchedule);
        spm.setName(name);
        spm.addToNode(node);
      };

      addDualSPM(plantLoop.name().get() + " Supply Outlet SPM", supplyOutletNode);

      // "Heating" components
      std::vector<model::BoilerHotWater> boilers =
      subsetCastVector<model::BoilerHotWater>(plantLoop.supplyComponents(model::BoilerHotWater::iddObjectType()));
      for(auto it = boilers.begin();
          it != boilers.end();
          ++it)
      {
        boost::optional<model::ModelObject> mo = it->outletModelObject();
        OS_ASSERT(mo);
        boost::optional<model::Node> node = mo->optionalCast<model::Node>();
        OS_ASSERT(node);

        addDualSPM(it->name().get() + " SPM", node.get());
      }

      std::vector<model::WaterHeaterMixed> waterHeaters =
      subsetCastVector<model::WaterHeaterMixed>(plantLoop.supplyComponents(model::WaterHeaterMixed::iddObjectType()));
      for(auto it = waterHeaters.begin();
          it != waterHeaters.end();
          ++it)
      {
        boost::optional<model::ModelObject> mo = it->supplyOutletModelObject();
        OS_ASSERT(mo);
        boost::optional<model::Node> node = mo->optionalCast<model::Node>();
        OS_ASSERT(node);

        addDualSPM(it->name().get() + " SPM", node.get());
      }

      // "Cooling" components
      std::vector<model::ChillerElectricEIR> chillers =
      subsetCastVector<model::ChillerElectricEIR>(plantLoop.supplyComponents(model::ChillerElectricEIR::iddObjectType()));
      for(auto it = chillers.begin();
          it != chillers.end();
          ++it)
      {
        boost::optional<model::ModelObject> mo = it->supplyOutletModelObject();
        OS_ASSERT(mo);
        boost::optional<model::Node> node = mo->optionalCast<model::Node>();
        OS_ASSERT(node);

        addDualSPM(it->name().get() + " SPM", node.get());
      }

      std::vector<model::CoolingTowerVariableSpeed> variableTowers =
      subsetCastVector<model::CoolingTowerVariableSpeed>(plantLoop.supplyComponents(model::CoolingTowerVariableSpeed::iddObjectType()));
      for(auto it = variableTowers.begin();
          it != variableTowers.end();
          ++it)
      {
        boost::optional<model::ModelObject> mo = it->outletModelObject();
        OS_ASSERT(mo);
        boost::optional<model::Node> node = mo->optionalCast<model::Node>();
        OS_ASSERT(node);

        addDualSPM(it->name().get() + " SPM", node.get());
      }

      std::vector<model::CoolingTowerTwoSpeed> twoSpeedTowers =
      subsetCastVector<model::CoolingTowerTwoSpeed>(plantLoop.supplyComponents(model::CoolingTowerTwoSpeed::iddObjectType()));
      for(auto it = twoSpeedTowers.begin();
          it != twoSpeedTowers.end();
          ++it)
      {
        boost::optional<model::ModelObject> mo = it->outletModelObject();
        OS_ASSERT(mo);
        boost::optional<model::Node> node = mo->optionalCast<model::Node>();
        OS_ASSERT(node);

        addDualSPM(it->name().get() + " SPM", node.get());
      }

      std::vector<model::CoolingTowerSingleSpeed> constantTowers =
      subsetCastVector<model::CoolingTowerSingleSpeed>(plantLoop.supplyComponents(model::CoolingTowerSingleSpeed::iddObjectType()));
      for(auto it = constantTowers.begin();
          it != constantTowers.end();
          ++it)
      {
        boost::optional<model::ModelObject> mo = it->outletModelObject();
        OS_ASSERT(mo);
        boost::optional<model::Node> node = mo->optionalCast<model::Node>();
        OS_ASSERT(node);

        addDualSPM(it->name().get() + " SPM", node.get());
      }
    }
  }
  else
  {
    LOG(Error,plantLoop.name().get() << " does not have a setpoint.");
  }

  // Ice Thermal Energy Storage needs special treatment
  if (iceStorage && iceStorageChlrRef) {
    // First, add it to the loop topology
    auto iceStorageChlr = model.getModelObjectByName<model::WaterToWaterComponent>(iceStorageChlrRef.get());
    if (iceStorageChlr) {
      auto node = iceStorageChlr->supplyOutletModelObject()->cast<model::Node>();
      iceStorage->addToNode(node);
    }

    // Second, apply control logic
    // Setpoint managers for all chillers and the ice storage
    // The charging chiller follows a different (colder) schedule
    // Other chillers will follow the plant supply outlet temperature
    const auto chargingChiller = model.getModelObjectByName<model::ChillerElectricEIR>(iceStorageChlrRef.get());
    const auto chillers = subsetCastVector<model::ChillerElectricEIR>(plantLoop.supplyComponents(model::ChillerElectricEIR::iddObjectType()));

    if (icePrimaryChillerSetpointSchedule) {
      for (const auto chiller: chillers) {
        if (chargingChiller && (chargingChiller.get() == chiller)) {
          continue;
        }
        auto node = chiller.supplyOutletModelObject()->cast<model::Node>();
        if (node.setpointManagers().empty()) {
          model::SetpointManagerScheduled spm(model,icePrimaryChillerSetpointSchedule.get());
          spm.addToNode(node);
        }
      }

      auto iceOutletNode = iceStorage->outletModelObject()->cast<model::Node>();
      model::SetpointManagerScheduled spm(model,icePrimaryChillerSetpointSchedule.get());
      spm.addToNode(iceOutletNode);
    }


    if (chargingChiller && iceChargingChillerSetpointSchedule) {
      auto chargeNode = chargingChiller->supplyOutletModelObject()->cast<model::Node>();
      model::SetpointManagerScheduled spm(model,iceChargingChillerSetpointSchedule.get());
      spm.addToNode(chargeNode);
    }

    // Third, apply an Availability Manager
    model::AvailabilityManagerLowTemperatureTurnOff avm(model);
    avm.setTemperature(-4.0);
    plantLoop.addAvailabilityManager(avm, 1u);
    avm.setApplicabilitySchedule(iceAvailabilityLowTempTurnOff.get());

    auto iceOutletNode = iceStorage->outletModelObject()->cast<model::Node>();
    avm.setSensorNode(iceOutletNode);
  }

  return plantLoop;
}

boost::optional<openstudio::model::ModelObject> ReverseTranslator::translatePump(
                                                  const pugi::xml_node& pumpElement,
                                                  openstudio::model::Model& model )
{
  boost::optional<openstudio::model::ModelObject> result;

  if( ! istringEqual(pumpElement.name(),"Pump") ) {
    return result;
  }

  boost::optional<double> mtrEff;
  boost::optional<double> totHd;
  boost::optional<double> flowCap;
  boost::optional<double> pwr;

  auto pumpName = pumpElement.child("Name").text().as_string();

  pugi::xml_node mtrEffElement = pumpElement.child("MtrEff");
  boost::optional<double> _mtrEff = lexicalCastToDouble(mtrEffElement);
  if( _mtrEff ) {
    mtrEff = _mtrEff.get();
  }

  pugi::xml_node totHdElement = pumpElement.child("TotHd");
  boost::optional<double> _totHd = lexicalCastToDouble(totHdElement);
  if( _totHd ) {
    // ft water to Pa
    totHd = _totHd.get() * 2989.067;
  }

  pugi::xml_node flowCapElement = pumpElement.child("FlowCapSim");
  boost::optional<double> _flowCap = lexicalCastToDouble(flowCapElement);
  if( _flowCap ) {
    flowCap = unitToUnit(_flowCap.get(), "gal/min", "m^3/s").get();
  }

  pugi::xml_node pwrElement = pumpElement.child("PwrSim");
  boost::optional<double> _pwr = lexicalCastToDouble(pwrElement);
  if( _pwr ) {
    // kW to W
    pwr = _pwr.get() * 1000.0;
  }

  // OperCtrl
  std::optional<std::string> pumpControlType;
  const auto operCtrlElement = pumpElement.child("OperCtrl");
  if (operCtrlElement) {
    const auto operCtrl = operCtrlElement.text().as_string();
    if (openstudio::istringEqual(operCtrl, "OnDemand")) {
      pumpControlType = "Intermittent";
    } else if (openstudio::istringEqual(operCtrl, "StandBy")) {
      pumpControlType = "Continuous";
    }
  }

  pugi::xml_node spdCtrlElement = pumpElement.child("SpdCtrl");
  if( openstudio::istringEqual(spdCtrlElement.text().as_string(), "constantspeed") ) {
    model::PumpConstantSpeed pump(model);
    pump.setName(pumpName);

    if (pumpControlType) {
      pump.setPumpControlType(pumpControlType.value());
    }

    if( mtrEff ) {
      pump.setMotorEfficiency(mtrEff.get());
    }

    if( totHd ) {
      pump.setRatedPumpHead(totHd.get());
    }

    if( autosize() ) {
      if( ! totHd ) {
        pump.setRatedPumpHead(149453.0);
        LOG(Warn,pump.name().get() << " TotHd set to OpenStudio default");
      }
    } else {
      if( flowCap ) {
        if( equal(flowCap.get(),0.0) ) {
          LOG(Warn,pump.name().get() << " has 0 capacity specified.");
        }
        pump.setRatedFlowRate(flowCap.get());
      }

      if( pwr ) {
        if( equal(pwr.get(),0.0) ) {
          LOG(Warn,pump.name().get() << " has 0 pwr specified.");
        }
        pump.setRatedPowerConsumption(pwr.get());
      }

      if( flowCap && pwr && (! totHd) ) {
        pump.setRatedPumpHead(0.80 * pwr.get() / flowCap.get());
        LOG(Warn,pump.name().get() << " TotHd set to OpenStudio default");
      } else if( ! totHd ) {
        pump.setRatedPumpHead(149453.0);
        LOG(Warn,pump.name().get() << " TotHd set to OpenStudio default");
      }
    }

    result = pump;
  } else {
    model::PumpVariableSpeed pump(model);
    pump.setName(pumpName);

    if (pumpControlType) {
      pump.setPumpControlType(pumpControlType.value());
    }

    if( mtrEff ) {
      pump.setMotorEfficiency(mtrEff.get());
    }

    if( totHd ) {
      pump.setRatedPumpHead(totHd.get());
    }

    // FlowMinSim
    const auto flowMinSimElement = pumpElement.child("FlowMinSim");
    if(flowMinSimElement) {
      const auto flowMinSim = unitToUnit(flowMinSimElement.text().as_double(), "gal/min", "m^3/s").get();
      pump.setMinimumFlowRate(flowMinSim);
    }

    // FlowMinRat
    const auto flowMinRatElement = pumpElement.child("FlowMinRat");
    if(flowMinRatElement) {
      pump.setDesignMinimumFlowRateFraction(flowMinRatElement.text().as_double());
    }

    if( autosize() )
    {
      if( ! totHd ) {
        pump.setRatedPumpHead(149453.0);
        LOG(Warn,pump.name().get() << " TotHd set to OpenStudio default");
      }
    }
    else
    {
      if( flowCap ) {
        if( equal(flowCap.get(),0.0) ) {
          LOG(Warn,pump.name().get() << " has 0 capacity specified.");
        }
        pump.setRatedFlowRate(flowCap.get());
      }

      if( pwr ) {
        if( equal(pwr.get(),0.0) ) {
          LOG(Warn,pump.name().get() << " has 0 pwr specified.");
        }
        pump.setRatedPowerConsumption(pwr.get());
      }

      if( flowCap && pwr && (! totHd) ) {
        pump.setRatedPumpHead(0.80 * pwr.get() / flowCap.get());
        LOG(Warn,pump.name().get() << " TotHd set to OpenStudio default");
      } else if( ! totHd ) {
        pump.setRatedPumpHead(149453.0);
        LOG(Warn,pump.name().get() << " TotHd set to OpenStudio default");
      }
    }

    // Pwr_fPLRCrvRef

    boost::optional<model::CurveCubic> pwr_fPLRCrv;
    pugi::xml_node pwr_fPLRCrvRefElement = pumpElement.child("Pwr_fPLRCrvRef");
    pwr_fPLRCrv = model.getModelObjectByName<model::CurveCubic>(pwr_fPLRCrvRefElement.text().as_string());

    if( pwr_fPLRCrv )
    {
      double c1 = pwr_fPLRCrv->coefficient1Constant();
      double c2 = pwr_fPLRCrv->coefficient2x();
      double c3 = pwr_fPLRCrv->coefficient3xPOW2();
      double c4 = pwr_fPLRCrv->coefficient4xPOW3();

      pump.setCoefficient1ofthePartLoadPerformanceCurve(c1);
      pump.setCoefficient2ofthePartLoadPerformanceCurve(c2);
      pump.setCoefficient3ofthePartLoadPerformanceCurve(c3);
      pump.setCoefficient4ofthePartLoadPerformanceCurve(c4);
    }

    result = pump;
  }

  if( result )
  {
    pugi::xml_node nameElement = pumpElement.child("Name");

    result->setName(nameElement.text().as_string());
  }

  return result;
}

boost::optional<openstudio::model::ModelObject> ReverseTranslator::translateBoiler(
                                                  const pugi::xml_node& boilerElement,
                                                  openstudio::model::Model& model )
{
  boost::optional<openstudio::model::ModelObject> result;

  if( ! istringEqual(boilerElement.name(),"Blr") )
  {
    return result;
  }

  model::BoilerHotWater boiler(model);

  // Name

  pugi::xml_node nameElement = boilerElement.child("Name");

  boiler.setName(nameElement.text().as_string());

  // ThrmlEff

  pugi::xml_node thrmlEffElement = boilerElement.child("ThrmlEff");
  boost::optional<double> _thrmlEff = lexicalCastToDouble(thrmlEffElement);
  if (_thrmlEff) {
    boiler.setNominalThermalEfficiency(_thrmlEff.get());
  }

  // HIR_fPLRCrvRef

  boost::optional<model::Curve> hirfPLRCrv;
  pugi::xml_node hirfPLRCrvRefElement = boilerElement.child("HIR_fPLRCrvRef");
  hirfPLRCrv = model.getModelObjectByName<model::Curve>(hirfPLRCrvRefElement.text().as_string());
  if( hirfPLRCrv )
  {
    boiler.setNormalizedBoilerEfficiencyCurve(hirfPLRCrv.get());

    if( hirfPLRCrv->optionalCast<model::CurveBiquadratic>() )
    {
      boiler.setEfficiencyCurveTemperatureEvaluationVariable("EnteringBoiler");
    }
    else
    {
      boiler.setEfficiencyCurveTemperatureEvaluationVariable("LeavingBoiler");
    }
  }

  // FuelSrc

  pugi::xml_node fuelSrcElement = boilerElement.child("FuelSrc");
  std::string fuelSrc = fuelSrcElement.text().as_string();

  if( openstudio::istringEqual(fuelSrc, "electric") )
  {
    boiler.setFuelType("Electricity");
  }
  else if ( openstudio::istringEqual(fuelSrc, "gas") )
  {
    boiler.setFuelType("NaturalGas");
  }
  else if( openstudio::istringEqual(fuelSrc, "oil") )
  {
    boiler.setFuelType("FuelOilNo2");
  }

  // ParasiticLd

  pugi::xml_node parasiticLdElement = boilerElement.child("ParasiticLdSim");

  boost::optional<double> _parasiticLd = lexicalCastToDouble(parasiticLdElement);

  if( _parasiticLd )
  {
    boiler.setParasiticElectricLoad(unitToUnit(_parasiticLd.get(),"Btu/h","W").get());
  }

  if( ! autosize() )
  {
    // CapRtd
    boost::optional<double> capRtd;
    pugi::xml_node capRtdElement = boilerElement.child("CapRtdSim");
    boost::optional<double> _capRtd = lexicalCastToDouble(capRtdElement);
    if( _capRtd )
    {
      capRtd = unitToUnit(_capRtd.get(),"Btu/h","W");
    }

    // EntTempDsgn
    boost::optional<double> entTempDsgn;
    pugi::xml_node entTempDsgnElement = boilerElement.child("EntTempDsgn");
    boost::optional<double> _entTempDsgn = lexicalCastToDouble(entTempDsgnElement);
    if( _entTempDsgn )
    {
      entTempDsgn = unitToUnit(_entTempDsgn.get(),"F","C");
    }

    // LvgTempDsgn
    boost::optional<double> lvgTempDsgn;
    pugi::xml_node lvgTempDsgnElement = boilerElement.child("LvgTempDsgn");
    boost::optional<double> _lvgTempDsgn = lexicalCastToDouble(lvgTempDsgnElement);
    if( _lvgTempDsgn )
    {
      lvgTempDsgn = unitToUnit(_lvgTempDsgn.get(),"F","C");
    }

    if( capRtd && entTempDsgn && lvgTempDsgn )
    {
      boiler.setNominalCapacity(capRtd.get());

      double flow = capRtd.get() / ( cpWater * densityWater * (lvgTempDsgn.get() - entTempDsgn.get()));

      boiler.setDesignWaterFlowRate(flow);
    }
  }

  auto mode = boilerElement.child("FlowMode").text().as_string();
  if (istringEqual(mode, "Constant")) {
    boiler.setBoilerFlowMode("ConstantFlow");
  } else if (istringEqual(mode, "Variable")) {
    boiler.setBoilerFlowMode("LeavingSetpointModulated");
  }

  return boiler;
}

boost::optional<openstudio::model::ModelObject> ReverseTranslator::translateHtRej(
                                                  const pugi::xml_node& htRejElement,
                                                  openstudio::model::Model& model )
{
  boost::optional<openstudio::model::ModelObject> result;

  std::string svalue;

  if( ! istringEqual(htRejElement.name(),"HtRej") )
  {
    return result;
  }

  // HtRej:ModCtrl
  // HtRej:CapRtd
  // HtRej:FanType
  // HtRej:TotFanHP
  // HtRej:AirFlowCap
  // HtRej:WtrFlowCap
  // HtRej:LowSpdAirFlowRat
  // HtRej:LowSpdPwrRat
  // HtRej:CellCnt

  auto type = htRejElement.child("Type").text().as_string();
  auto modCtrl = htRejElement.child("ModCtrl").text().as_string();
  auto calcMthd = htRejElement.child("CalcMthd").text().as_string();

  if ( istringEqual(type,"OpenTower") ) {
    if ( istringEqual(modCtrl,"Bypass") || istringEqual(modCtrl, "Cycling") ) {
      model::CoolingTowerSingleSpeed tower(model);

      auto name = htRejElement.child("Name").text().as_string();
      tower.setName(name);

      if( istringEqual(modCtrl,"Cycling") )
      {
        tower.setCapacityControl("FanCycling");
      }
      else if( istringEqual(modCtrl,"Bypass") )
      {
        tower.setCapacityControl("FluidBypass");
      }

      auto cellCntElement = htRejElement.child("CellCnt");
      auto cellCnt = lexicalCastToInt(cellCntElement);
      if( cellCnt )
      {
        tower.setNumberofCells(cellCnt.get());
      }

      tower.resetUFactorTimesAreaValueatFreeConvectionAirFlowRate();
      tower.resetUFactorTimesAreaValueatDesignAirFlowRate();

      if( ! autosize() )
      {
        // PerformanceInputMethod
        tower.setPerformanceInputMethod("NominalCapacity");

        // CapRtd
        auto capRtd = lexicalCastToDouble(htRejElement.child("CapRtdSim"));
        if( capRtd )
        {
          // NominalCapacity
          double cap = unitToUnit(capRtd.get(),"Btu/h","W").get();

          tower.setNominalCapacity(cap);
        }

        auto airFlowCap = lexicalCastToDouble(htRejElement.child("AirFlowCapSim"));
        if( airFlowCap)
        {
          tower.setDesignAirFlowRate(unitToUnit(airFlowCap.get(),"cfm","m^3/s").get());
        }

        auto wtrFlowCap = lexicalCastToDouble(htRejElement.child("WtrFlowCapSim"));
        if( wtrFlowCap )
        {
          tower.setDesignWaterFlowRate(unitToUnit(wtrFlowCap.get(),"gal","m^3").get());
        }

        auto totFanHP = lexicalCastToDouble(htRejElement.child("TotFanHPSim"));
        if( totFanHP )
        {
          tower.setFanPoweratDesignAirFlowRate(totFanHP.get() * 745.7);
        }
      }
      result = tower;
    } else if ( istringEqual(modCtrl,"VariableSpeedDrive") ) {
      model::CoolingTowerVariableSpeed tower(model);

      auto name = htRejElement.child("Name").text().as_string();
      tower.setName(name);

      tower.setModelType("CoolToolsCrossFlow");
      //tower.setModelCoefficient();

      auto wetBulbApproach = lexicalCastToDouble(htRejElement.parent().child("WetBulbApproach"));
      auto dsgnSupWtrTemp = lexicalCastToDouble(htRejElement.parent().child("DsgnSupWtrTemp"));
      auto dsgnSupWtrDelT = lexicalCastToDouble(htRejElement.parent().child("DsgnSupWtrDelT"));

      boost::optional<double> dsgnWBT;

      if( wetBulbApproach )
      {
        wetBulbApproach = wetBulbApproach.get() * 5.0 / 9.0;
      }

      if( dsgnSupWtrTemp )
      {
        dsgnSupWtrTemp = unitToUnit(dsgnSupWtrTemp.get(),"F","C");
      }

      if( dsgnSupWtrDelT )
      {
        dsgnSupWtrDelT = dsgnSupWtrDelT.get() * 5.0 / 9.0;
      }

      dsgnWBT = lexicalCastToDouble(htRejElement.root().child("Proj").child("DsgnWBT"));
      if ( dsgnWBT ) {
        dsgnWBT = unitToUnit(dsgnWBT.get(),"F","C");
      }

      if( dsgnSupWtrDelT )
      {
        tower.setDesignRangeTemperature(dsgnSupWtrDelT.get());
      }

      if ( dsgnWBT ) {
        tower.setDesignInletAirWetBulbTemperature(dsgnWBT.get());
      }

      tower.setDesignApproachTemperature( 5.6 );

      tower.setDesignInletAirWetBulbTemperature(20.01);

      if( dsgnSupWtrTemp && wetBulbApproach )
      {
        tower.setDesignInletAirWetBulbTemperature(dsgnSupWtrTemp.get() - wetBulbApproach.get());
      }
      else if( dsgnSupWtrTemp )
      {
        tower.setDesignInletAirWetBulbTemperature(dsgnSupWtrTemp.get() - 5.55556);
      }

      if( wetBulbApproach )
      {
        tower.setDesignApproachTemperature(wetBulbApproach.get());
      }
      else
      {
        tower.setDesignApproachTemperature(5.55556);
      }

      if( ! autosize() )
      {
        if ( dsgnWBT && dsgnSupWtrTemp ) {
          tower.setDesignApproachTemperature( dsgnWBT.get() - dsgnSupWtrTemp.get() );
        }

        auto airFlowCap = lexicalCastToDouble(htRejElement.child("AirFlowCapSim"));
        if( airFlowCap )
        {
          tower.setDesignAirFlowRate(unitToUnit(airFlowCap.get(),"cfm","m^3/s").get());
        }

        auto wtrFlowCap = lexicalCastToDouble(htRejElement.child("WtrFlowCapSim"));
        if( wtrFlowCap )
        {
          tower.setDesignWaterFlowRate(unitToUnit(wtrFlowCap.get(), "gal/min", "m^3/s").get());
        }

        auto totFanHP = lexicalCastToDouble(htRejElement.child("TotFanHPSim"));
        if( totFanHP )
        {
          tower.setDesignFanPower(totFanHP.get() * 745.7);
        }
      }

      auto minSpdRat = lexicalCastToDouble(htRejElement.child("LowSpdAirFlowRatSim"));
      if( minSpdRat )
      {
        tower.setMinimumAirFlowRateRatio(minSpdRat.get());
      }

      boost::optional<model::CurveCubic> vsdFanPwrRatio_fQRatio;
      auto vsdFanPwrRatio_fQRatioElement = htRejElement.child("VSDFanPwrRatio_fQRatio");
      vsdFanPwrRatio_fQRatio = model.getModelObjectByName<model::CurveCubic>(vsdFanPwrRatio_fQRatioElement.text().as_string());

      if( vsdFanPwrRatio_fQRatio )
      {
        tower.setFanPowerRatioFunctionofAirFlowRateRatioCurve(vsdFanPwrRatio_fQRatio.get());
      }

      auto cellCnt = lexicalCastToInt(htRejElement.child("CellCnt"));
      if( cellCnt )
      {
        tower.setNumberofCells(cellCnt.get());
      }

      result = tower;
    } else if ( istringEqual(modCtrl,"TwoSpeed") ) {
      model::CoolingTowerTwoSpeed tower(model);

      auto name = htRejElement.child("Name").text().as_string();
      tower.setName(name);

      auto wtrFlowCap = lexicalCastToDouble(htRejElement.child("WtrFlowCapSim"));
      if ( wtrFlowCap ) {
        wtrFlowCap = unitToUnit(wtrFlowCap.get(), "gal/min", "m^3/s").get();
        tower.setDesignWaterFlowRate(wtrFlowCap.get());
      }

      auto airFlowCap = lexicalCastToDouble(htRejElement.child("AirFlowCapSim"));
      if ( airFlowCap ) {
        airFlowCap = unitToUnit(airFlowCap.get(),"cfm","m^3/s").get();
        tower.setHighFanSpeedAirFlowRate(airFlowCap.get());
      }

      auto totFanHP = lexicalCastToDouble(htRejElement.child("TotFanHPSim"));
      if ( totFanHP ) {
        tower.setHighFanSpeedFanPower(totFanHP.get() * 745.7);
      }

      auto lowSpdAirFlowCap = lexicalCastToDouble(htRejElement.child("LowSpdAirFlowCapSim"));
      if ( lowSpdAirFlowCap ) {
        lowSpdAirFlowCap = unitToUnit(lowSpdAirFlowCap.get(),"cfm","m^3/s").get();
        tower.setLowFanSpeedAirFlowRate(lowSpdAirFlowCap.get());
      }

      auto lowSpdPwrRat = lexicalCastToDouble(htRejElement.child("LowSpdPwrRatSim"));
      if ( lowSpdPwrRat ) {
        tower.setLowFanSpeedFanPower(lowSpdPwrRat.get() * 745.7);
      }

      tower.setPerformanceInputMethod("NominalCapacity");

      auto capRtd = lexicalCastToDouble(htRejElement.child("CapRtdSim"));
      if ( capRtd ) {
        capRtd = unitToUnit(capRtd.get(),"Btu/h","W").get();
        tower.setHighSpeedNominalCapacity(capRtd.get());
      }

      auto lowSpdCapRtd = lexicalCastToDouble(htRejElement.child("LowSpdCapRtdSim"));
      if ( lowSpdCapRtd ) {
        lowSpdCapRtd = unitToUnit(lowSpdCapRtd.get(),"Btu/h","W").get();
        tower.setLowSpeedNominalCapacity(lowSpdCapRtd.get());
      }

      auto freeConvCap = lexicalCastToDouble(htRejElement.child("FreeConvCapRtdSim"));
      if ( freeConvCap ) {
        freeConvCap = unitToUnit(freeConvCap.get(),"Btu/h","W").get();
        tower.setFreeConvectionNominalCapacity(freeConvCap.get());
      }

      tower.resetLowFanSpeedUFactorTimesAreaValue();
      tower.resetHighFanSpeedUFactorTimesAreaValue();
      tower.autosizeFreeConvectionRegimeAirFlowRate();

      result = tower;
    }
  } else if ( istringEqual(type,"ClosedTower") )  {
    if ( istringEqual(modCtrl,"Bypass") || istringEqual(modCtrl,"Cycling") ) {
      model::FluidCoolerSingleSpeed fluidCooler(model);

      auto name = htRejElement.child("Name").text().as_string();
      fluidCooler.setName(name);

      if ( istringEqual(calcMthd,"UserSpecified") ) {
        fluidCooler.setPerformanceInputMethod("NominalCapacity");

        auto entTempDsgn = lexicalCastToDouble(htRejElement.child("EntTempDsgn"));
        if ( entTempDsgn ) {
          entTempDsgn = unitToUnit(entTempDsgn.get(),"F","C").get();
          fluidCooler.setDesignEnteringWaterTemperature(entTempDsgn.get());
        }

        auto dsgnDBT = lexicalCastToDouble(htRejElement.root().child("Proj").child("DsgnDBT"));
        if ( dsgnDBT ) {
          dsgnDBT = unitToUnit(dsgnDBT.get(),"F","C").get();
          fluidCooler.setDesignEnteringAirTemperature(dsgnDBT.get());
        }

        auto dsgnWBT = lexicalCastToDouble(htRejElement.root().child("Proj").child("DsgnWBT"));
        if ( dsgnWBT ) {
          dsgnWBT = unitToUnit(dsgnWBT.get(),"F","C").get();
          fluidCooler.setDesignEnteringAirWetbulbTemperature(dsgnWBT.get());
        }
      } else if ( istringEqual(calcMthd,"UAFactorSim") ) {
        fluidCooler.setPerformanceInputMethod("UFactorTimesAreaAndDesignWaterFlowRate");

        auto uaFactor = lexicalCastToDouble(htRejElement.child("UAFactorSim"));
        if ( uaFactor ) {
          // Convert Btu/h-F to W/K
          fluidCooler.setDesignAirFlowRateUfactorTimesAreaValue(uaFactor.get() * 0.5275);
        }
      }

      auto capRtd = lexicalCastToDouble(htRejElement.child("CapRtdSim"));
      if ( capRtd ) {
        capRtd = unitToUnit(capRtd.get(),"Btu/h","W").get();
        fluidCooler.setNominalCapacity(capRtd.get());
      }

      auto wtrFlowCap = lexicalCastToDouble(htRejElement.child("WtrFlowCapSim"));
      if ( wtrFlowCap ) {
        wtrFlowCap = unitToUnit(wtrFlowCap.get(),"gal/min","m^3/s").get();
        fluidCooler.setDesignWaterFlowRate(wtrFlowCap.get());
      }

      auto airFlowCap = lexicalCastToDouble(htRejElement.child("AirFlowCapSim"));
      if ( airFlowCap ) {
        fluidCooler.setDesignAirFlowRate(unitToUnit(airFlowCap.get(),"cfm","m^3/s").get());
      }

      auto totFanHPElement = htRejElement.child("TotFanHPSim");
      auto totFanHP = lexicalCastToDouble(totFanHPElement);
      if ( totFanHP ) {
        fluidCooler.setDesignAirFlowRateFanPower(totFanHP.get() * 745.7);
      }

      result = fluidCooler;
    } else if ( istringEqual(modCtrl,"VariableSpeedDrive") || istringEqual(modCtrl,"TwoSpeed") ) {
      model::FluidCoolerTwoSpeed fluidCooler(model);

      auto name = htRejElement.child("Name").text().as_string();
      fluidCooler.setName(name);

      fluidCooler.setPerformanceInputMethod("NominalCapacity");

      auto capRtd = lexicalCastToDouble(htRejElement.child("CapRtdSim"));
      if ( capRtd ) {
        capRtd = unitToUnit(capRtd.get(),"Btu/h","W").get();
        fluidCooler.setHighSpeedNominalCapacity(capRtd.get());
      }

      auto lowSpdCapRtd = lexicalCastToDouble(htRejElement.child("LowSpdCapRtdSim"));
      if ( lowSpdCapRtd ) {
        lowSpdCapRtd = unitToUnit(lowSpdCapRtd.get(),"Btu/h","W").get();
        fluidCooler.setLowSpeedNominalCapacity(lowSpdCapRtd.get());
      }

      if ( istringEqual(calcMthd,"UserSpecified") ) {
        fluidCooler.setPerformanceInputMethod("NominalCapacity");
      } else if ( istringEqual(calcMthd,"UAFactorSim") ) {
        fluidCooler.setPerformanceInputMethod("UFactorTimesAreaAndDesignWaterFlowRate");

        auto uaFactor = lexicalCastToDouble(htRejElement.child("UAFactorSim"));
        if ( uaFactor ) {
          // Convert Btu/h-F to W/K
          fluidCooler.setHighFanSpeedUfactorTimesAreaValue(uaFactor.get() * 0.5275);
        }

        auto lowSpdUAFactor = lexicalCastToDouble(htRejElement.child("LowSpdUAFactorSim"));
        if ( lowSpdUAFactor ) {
          // Convert Btu/h-F to W/K
          fluidCooler.setLowFanSpeedUfactorTimesAreaValue(lowSpdUAFactor.get() * 0.5275);
        }
      }

      auto entTempDsgn = lexicalCastToDouble(htRejElement.child("EntTempDsgn"));
      if ( entTempDsgn ) {
        entTempDsgn = unitToUnit(entTempDsgn.get(),"F","C").get();
        fluidCooler.setDesignEnteringWaterTemperature(entTempDsgn.get());
      }

      auto dsgnDBT = lexicalCastToDouble(htRejElement.root().child("Proj").child("DsgnDBT"));
      if ( dsgnDBT ) {
        dsgnDBT = unitToUnit(dsgnDBT.get(),"F","C").get();
        fluidCooler.setDesignEnteringAirTemperature(dsgnDBT.get());
      }

      auto dsgnWBT = lexicalCastToDouble(htRejElement.root().child("Proj").child("DsgnWBT"));
      if ( dsgnWBT ) {
        dsgnWBT = unitToUnit(dsgnWBT.get(),"F","C").get();
        fluidCooler.setDesignEnteringAirWetbulbTemperature(dsgnWBT.get());
      }

      auto wtrFlowCap = lexicalCastToDouble(htRejElement.child("WtrFlowCapSim"));
      if ( wtrFlowCap ) {
        wtrFlowCap = unitToUnit(wtrFlowCap.get(),"gal/min","m^3/s").get();
        fluidCooler.setDesignWaterFlowRate(wtrFlowCap.get());
      }

      auto airFlowCap = lexicalCastToDouble(htRejElement.child("AirFlowCapSim"));
      if ( airFlowCap ) {
        fluidCooler.setHighFanSpeedAirFlowRate(unitToUnit(airFlowCap.get(),"cfm","m^3/s").get());
      }

      auto lowSpdAirFlowCap = lexicalCastToDouble(htRejElement.child("LowSpdAirFlowCapSim"));
      if ( lowSpdAirFlowCap ) {
        fluidCooler.setLowFanSpeedAirFlowRate(unitToUnit(lowSpdAirFlowCap.get(),"cfm","m^3/s").get());
      }

      auto totFanHP = lexicalCastToDouble(htRejElement.child("TotFanHPSim"));
      if ( totFanHP ) {
        fluidCooler.setHighFanSpeedFanPower(totFanHP.get() * 745.7);
      }

      auto lowSpdPwrRat = lexicalCastToDouble(htRejElement.child("LowSpdPwrRatSim"));
      if ( lowSpdPwrRat ) {
        fluidCooler.setLowFanSpeedFanPowerSizingFactor(lowSpdPwrRat.get());
      }

      result = fluidCooler;
    }
  } else if ( istringEqual(type,"ClosedTowerEvaporative") )  {
    if ( istringEqual(modCtrl,"Bypass") || istringEqual(modCtrl,"Cycling") ) {
      model::EvaporativeFluidCoolerSingleSpeed fluidCooler(model);

      auto name = htRejElement.child("Name").text().as_string();
      fluidCooler.setName(name);

      auto airFlowCap = lexicalCastToDouble(htRejElement.child("AirFlowCapSim"));
      if( airFlowCap ) {
        fluidCooler.setDesignAirFlowRate(unitToUnit(airFlowCap.get(),"cfm","m^3/s").get());
      }

      auto totFanHP = lexicalCastToDouble(htRejElement.child("TotFanHPSim"));
      if( totFanHP )
      {
        fluidCooler.setFanPoweratDesignAirFlowRate(totFanHP.get() * 745.7);
      }

      auto sprayWtrFlowCap = lexicalCastToDouble(htRejElement.child("SprayWtrFlowCapSim"));
      if( sprayWtrFlowCap ) {
        fluidCooler.setDesignSprayWaterFlowRate(unitToUnit(sprayWtrFlowCap.get(), "gal/min", "m^3/s").get());
      }

      if ( istringEqual(calcMthd,"StandardDesign") ) {
        fluidCooler.setPerformanceInputMethod("StandardDesignCapacity");
      } else if ( istringEqual(calcMthd,"UserSpecified") ) {
        fluidCooler.setPerformanceInputMethod("UserSpecifiedDesignCapacity");

        auto capRtd = lexicalCastToDouble(htRejElement.child("CapRtdSim"));
        if ( capRtd ) {
          fluidCooler.setStandardDesignCapacity(unitToUnit(capRtd.get(),"Btu/h","W").get());
        }

        auto entTempDsgn = lexicalCastToDouble(htRejElement.child("EntTempDsgn"));
        if ( entTempDsgn ) {
          fluidCooler.setDesignEnteringWaterTemperature(unitToUnit(entTempDsgn.get(),"F","C").get());
        }

        auto dsgnDBT = lexicalCastToDouble(htRejElement.root().child("Proj").child("DsgnDBT"));
        if ( dsgnDBT ) {
          fluidCooler.setDesignEnteringAirTemperature(unitToUnit(dsgnDBT.get(),"F","C").get());
        }

        auto dsgnWBT = lexicalCastToDouble(htRejElement.root().child("Proj").child("DsgnWBT"));
        if ( dsgnWBT ) {
          fluidCooler.setDesignEnteringAirWetbulbTemperature(unitToUnit(dsgnWBT.get(),"F","C").get());
        }
      } else if ( istringEqual(calcMthd,"UAFactor") ) {
        fluidCooler.setPerformanceInputMethod("UFactorTimesAreaAndDesignWaterFlowRate");
        auto uaFactor = lexicalCastToDouble(htRejElement.child("UAFactorSim"));
        if ( uaFactor ) {
          // Convert Btu/h-F to W/K
          fluidCooler.setUfactorTimesAreaValueatDesignAirFlowRate(uaFactor.get() * 0.5275);
        }
      }

      auto wtrFlowCap = lexicalCastToDouble(htRejElement.child("WtrFlowCapSim"));
      if ( wtrFlowCap ) {
        fluidCooler.setDesignWaterFlowRate(unitToUnit(wtrFlowCap.get(),"gal/min","m^3/s").get());
      }

      fluidCooler.setCapacityControl(modCtrl);

      fluidCooler.setEvaporationLossMode("LossFactor");

      fluidCooler.setEvaporationLossFactor(0.2);

      fluidCooler.setDriftLossPercent(0.008);

      fluidCooler.setBlowdownCalculationMode("ConcentrationRatio");

      fluidCooler.setBlowdownConcentrationRatio(0.2);

      result = fluidCooler;
    } else if ( istringEqual(modCtrl,"VariableSpeedDrive") || istringEqual(modCtrl,"TwoSpeed") ) {
      model::EvaporativeFluidCoolerTwoSpeed fluidCooler(model);

      auto name = htRejElement.child("Name").text().as_string();
      fluidCooler.setName(name);

      fluidCooler.resetLowFanSpeedUfactorTimesAreaValue();

      auto airFlowCap = lexicalCastToDouble(htRejElement.child("AirFlowCapSim"));
      if( airFlowCap ) {
        fluidCooler.setHighFanSpeedAirFlowRate(unitToUnit(airFlowCap.get(),"cfm","m^3/s").get());
      }

      auto lowSpdAirFlowCap = lexicalCastToDouble(htRejElement.child("LowSpdAirFlowCapSim"));
      if ( lowSpdAirFlowCap ) {
        fluidCooler.setLowFanSpeedAirFlowRate(unitToUnit(lowSpdAirFlowCap.get(),"cfm","m^3/s").get());
      }

      auto totFanHP = lexicalCastToDouble(htRejElement.child("TotFanHPSim"));
      if ( totFanHP ) {
        fluidCooler.setHighFanSpeedFanPower(totFanHP.get() * 745.7);
      }

      auto lowSpdAirFlowRat = lexicalCastToDouble(htRejElement.child("LowSpdAirFlowRatSim"));
      if ( lowSpdAirFlowRat ) {
        fluidCooler.setLowFanSpeedAirFlowRateSizingFactor(lowSpdAirFlowRat.get());
      }

      auto lowSpdPwrRat = lexicalCastToDouble(htRejElement.child("LowSpdPwrRatSim"));
      if ( lowSpdPwrRat ) {
        fluidCooler.setLowFanSpeedFanPowerSizingFactor(lowSpdPwrRat.get());
      }

      auto sprayWtrFlowCap = lexicalCastToDouble(htRejElement.child("SprayWtrFlowCapSim"));
      if( sprayWtrFlowCap ) {
        fluidCooler.setDesignSprayWaterFlowRate(unitToUnit(sprayWtrFlowCap.get(), "gal/min", "m^3/s").get());
      }

      if ( istringEqual(calcMthd,"StandardDesign") ) {
        fluidCooler.setPerformanceInputMethod("StandardDesignCapacity");

        fluidCooler.setHeatRejectionCapacityandNominalCapacitySizingRatio(1.0);

        auto capRtd = lexicalCastToDouble(htRejElement.child("CapRtdSim"));
        if ( capRtd ) {
          fluidCooler.setHighSpeedStandardDesignCapacity(unitToUnit(capRtd.get(),"Btu/h","W").get());
        }

        auto lowSpdCapRtd = lexicalCastToDouble(htRejElement.child("LowSpdCapRtdSim"));
        if ( lowSpdCapRtd ) {
          fluidCooler.setLowSpeedStandardDesignCapacity(unitToUnit(lowSpdCapRtd.get(),"Btu/h","W").get());
        }
      } else if ( istringEqual(calcMthd,"UserSpecified") ) {
        fluidCooler.setPerformanceInputMethod("UserSpecifiedDesignCapacity");

        auto capRtd = lexicalCastToDouble(htRejElement.child("CapRtdSim"));
        if ( capRtd ) {
          fluidCooler.setHighSpeedUserSpecifiedDesignCapacity(unitToUnit(capRtd.get(),"Btu/h","W").get());
        }

        auto lowSpdCapRtd = lexicalCastToDouble(htRejElement.child("LowSpdCapRtdSim"));
        if ( lowSpdCapRtd ) {
          fluidCooler.setLowSpeedUserSpecifiedDesignCapacity(unitToUnit(lowSpdCapRtd.get(),"Btu/h","W").get());
        }
      } else if ( istringEqual(calcMthd,"UAFactor") ) {
        fluidCooler.setPerformanceInputMethod("UFactorTimesAreaAndDesignWaterFlowRate");

        auto uaFactor = lexicalCastToDouble(htRejElement.child("UAFactorSim"));
        if ( uaFactor ) {
          // Convert Btu/h-F to W/K
          fluidCooler.setHighFanSpeedUfactorTimesAreaValue(uaFactor.get() * 0.5275);
        }

        auto lowSpdUAFactor = lexicalCastToDouble(htRejElement.child("LowSpdUAFactorSim"));
        if ( lowSpdUAFactor ) {
          // Convert Btu/h-F to W/K
          fluidCooler.setLowFanSpeedUfactorTimesAreaValue(lowSpdUAFactor.get() * 0.5275);
        }
      }

      auto wtrFlowCap = lexicalCastToDouble(htRejElement.child("WtrFlowCapSim"));
      if ( wtrFlowCap ) {
        fluidCooler.setDesignWaterFlowRate(unitToUnit(wtrFlowCap.get(),"gal/min","m^3/s").get());
      }

      auto entTempDsgn = lexicalCastToDouble(htRejElement.child("EntTempDsgn"));
      if ( entTempDsgn ) {
        fluidCooler.setDesignEnteringWaterTemperature(unitToUnit(entTempDsgn.get(),"F","C").get());
      }

      auto dsgnDBT = lexicalCastToDouble(htRejElement.root().child("Proj").child("DsgnDBT"));
      if ( dsgnDBT ) {
        fluidCooler.setDesignEnteringAirTemperature(unitToUnit(dsgnDBT.get(),"F","C").get());
      }

      auto dsgnWBT = lexicalCastToDouble(htRejElement.root().child("Proj").child("DsgnWBT"));
      if ( dsgnWBT ) {
        fluidCooler.setDesignEnteringAirWetbulbTemperature(unitToUnit(dsgnWBT.get(),"F","C").get());
      }

      fluidCooler.setEvaporationLossMode("LossFactor");

      fluidCooler.setEvaporationLossFactor(0.2);

      fluidCooler.setDriftLossPercent(0.008);

      fluidCooler.setBlowdownCalculationMode("ConcentrationRatio");

      fluidCooler.setBlowdownConcentrationRatio(0.2);

      result = fluidCooler;
    }
  }

  return result;
}

boost::optional<openstudio::model::ModelObject> ReverseTranslator::translateHX(
                                                  const pugi::xml_node& hxElement,
                                                  openstudio::model::Model& model )
{
  boost::optional<openstudio::model::ModelObject> result;

  if( ! istringEqual(hxElement.name(),"HX") ) {
    return result;
  }

  model::HeatExchangerFluidToFluid hx(model);
  result = hx;

  std::string name = hxElement.child("Name").text().as_string();
  hx.setName(name);

  auto cWFluidSegInRefElement = hxElement.child("CWFluidSegInRef");
  if (!cWFluidSegInRefElement) {
    LOG(Error, "HeatExchangerFluidToFluid '" << name << "' doesn't have a CWFluidSegInRef element! It will not be connected.");
  } else {
    // The HX is on the demand side, so its inlet (CWFluidSegInRef) is the supply side
    if( auto condenserSystem = loopForSupplySegment(cWFluidSegInRefElement, model) ) {
      condenserSystem->addDemandBranchForComponent(hx);
    }
  }

  if( ! autosize() ) {
    auto cHWFluidFlowRtDsgnElement = hxElement.child("CHWFluidFlowRtDsgn");
    boost::optional<double> _cHWFluidFlowRtDsgn = lexicalCastToDouble(cHWFluidFlowRtDsgnElement);
    if( _cHWFluidFlowRtDsgn ) {
      double value = unitToUnit(_cHWFluidFlowRtDsgn.get(),"gal/min","m^3/s").get();
      hx.setLoopDemandSideDesignFlowRate(value);
    }

    auto cWFluidFlowRtDsgnElement = hxElement.child("CWFluidFlowRtDsgn");
    boost::optional<double> _cWFluidFlowRtDsgn = lexicalCastToDouble(cWFluidFlowRtDsgnElement);
    if( _cWFluidFlowRtDsgn ) {
      double value = unitToUnit(_cWFluidFlowRtDsgn.get(),"gal/min","m^3/s").get();
      hx.setLoopSupplySideDesignFlowRate(value);
    }

    auto uAElement = hxElement.child("UASim");
    boost::optional<double> _uA = lexicalCastToDouble(uAElement);
    if( _uA ) {
      // sdd units = Btu/(hr*ft^2*F), os units = W/(m^2*K)
      Quantity uaIP(_uA.get(), BTUUnit(BTUExpnt(1,-2,-1,-1)));
      auto uaSI = QuantityConverter::instance().convert(uaIP, UnitSystem(UnitSystem::Wh));
      OS_ASSERT(uaSI);
      OS_ASSERT(uaSI->units() == WhUnit(WhExpnt(1,0,-2,-1)));
      hx.setHeatExchangerUFactorTimesAreaValue(uaSI->value());
    }
  }

  std::string type = hxElement.child("Type").text().as_string();
  if( istringEqual("CrossFlowSupplyMixed",type) ) {
    hx.setHeatExchangeModelType("CrossFlowSupplyMixedDemandUnMixed");
  } else if( istringEqual("CrossFlowDemandMixed",type) ) {
    hx.setHeatExchangeModelType("CrossFlowDemandMixedSupplyUnMixed");
  } else if ( ! hx.setHeatExchangeModelType(type) ) {
    LOG(Warn,name + " Type field references an unsupported option " + type + ".");
  }

  hx.setMinimumTemperatureDifferencetoActivateHeatExchanger(1.1);

  hx.setHeatTransferMeteringEndUseType("FreeCooling");

  hx.setComponentOverrideCoolingControlTemperatureMode("Loop");

  hx.setSizingFactor(1.0);

  hx.setOperationMinimumTemperatureLimit(4.4);

  hx.setOperationMinimumTemperatureLimit(21.1);

  return result;
}

boost::optional<openstudio::model::ModelObject> ReverseTranslator::translateIceThrmlEngyStor(
                                                  const pugi::xml_node& tesElement,
                                                  openstudio::model::Model& model )
{
  boost::optional<openstudio::model::ModelObject> result;
  std::string name = tesElement.child("Name").text().as_string();
  model::ThermalStorageIceDetailed tes(model);
  result = tes;
  tes.setName(name);

  const auto capRtdSimElement = lexicalCastToDouble(tesElement.child("IceStorCapSim"));
  if (capRtdSimElement) {
    const double capRtdSim = unitToUnit(capRtdSimElement.get(),"Btu","GJ").get();
    tes.setCapacity(capRtdSim);
  }

  const auto dischrgCrvVarSpec = tesElement.child("DischrgCrvVarSpec").text().as_string();
  tes.setDischargingCurveVariableSpecifications(dischrgCrvVarSpec);

  auto dischargingCurve = tes.dischargingCurve().cast<model::CurveQuadraticLinear>();
  dischargingCurve.setName(name + " DischargeCurve");
  dischargingCurve.setCoefficient1Constant(0.0);
  dischargingCurve.setCoefficient2x(0.09);
  dischargingCurve.setCoefficient3xPOW2(-0.15);
  dischargingCurve.setCoefficient4y(0.612);
  dischargingCurve.setCoefficient5xTIMESY(-0.324);
  dischargingCurve.setCoefficient6xPOW2TIMESY(-0.216);
  dischargingCurve.setMinimumValueofx(0.0);
  dischargingCurve.setMaximumValueofx(1.0);
  dischargingCurve.setMinimumValueofy(0.0);
  dischargingCurve.setMaximumValueofy(9.9);

  const auto chrgCrvVarSpec = tesElement.child("ChrgCrvVarSpec").text().as_string();
  tes.setChargingCurveVariableSpecifications(chrgCrvVarSpec);

  auto chargingCurve = tes.chargingCurve().cast<model::CurveQuadraticLinear>();
  chargingCurve.setName(name + " ChargeCurve");
  chargingCurve.setCoefficient1Constant(0.0);
  chargingCurve.setCoefficient2x(0.09);
  chargingCurve.setCoefficient3xPOW2(-0.15);
  chargingCurve.setCoefficient4y(0.612);
  chargingCurve.setCoefficient5xTIMESY(-0.324);
  chargingCurve.setCoefficient6xPOW2TIMESY(-0.216);
  chargingCurve.setMinimumValueofx(0.0);
  chargingCurve.setMaximumValueofx(1.0);
  chargingCurve.setMinimumValueofy(0.0);
  chargingCurve.setMaximumValueofy(9.9);

  tes.setTimestepoftheCurveData(1.0);
  tes.setParasiticElectricLoadDuringDischarging(0.0001);
  tes.setParasiticElectricLoadDuringCharging(0.0002);
  tes.setTankLossCoefficient(0.0003);
  tes.setFreezingTemperatureofStorageMedium(0.0);

  return result;
}

boost::optional<openstudio::model::ModelObject> ReverseTranslator::translateChilledWaterThrmlEngyStor(
                                                  const pugi::xml_node& tesElement,
                                                  openstudio::model::Model& model )
{
  boost::optional<openstudio::model::ModelObject> result;
  std::string name = tesElement.child("Name").text().as_string();
  model::ThermalStorageChilledWaterStratified tes(model);
  result = tes;
  tes.setName(name);

  pugi::xml_node storCapElement = tesElement.child("StorCapSim");
  boost::optional<double> _storCap = lexicalCastToDouble(storCapElement);
  if( _storCap ) {
   double value = unitToUnit(_storCap.get(),"gal","m^3").get();
   tes.setTankVolume(value);
  }

  boost::optional<double> _tankHgt = lexicalCastToDouble(tesElement.child("TankHgtSim"));
  if( _tankHgt ) {
   double value = unitToUnit(_tankHgt.get(),"ft","m").get();
   tes.setTankHeight(value);
   tes.setSourceSideOutletHeight(value);
   tes.setUseSideInletHeight(value);
  }

  std::string tankShapeSim = tesElement.child("TankShapeSim").text().as_string();
  tes.setTankShape(tankShapeSim);
  if( istringEqual("Other",tankShapeSim) ) {
    // Used to be TankPerim I think, but now I'm sure it's TankPerimSim
    boost::optional<double> _tankPerim = lexicalCastToDouble(tesElement.child("TankPerimSim"));
    if( _tankPerim ) {
      double value = unitToUnit(_tankPerim.get(),"ft","m").get();
      tes.setTankPerimeter(value);
    }
  } else {
    tes.resetTankPerimeter();
  }

  tes.setDeadbandTemperatureDifference(0.5);
  tes.setTemperatureSensorHeight(0.0);
  tes.setMinimumTemperatureLimit(1.0);

  boost::optional<double> _capRtd = lexicalCastToDouble(tesElement.child("CapRtdSim"));
  if( _capRtd ) {
    double value = unitToUnit(_capRtd.get(),"Btu/h","W").get();
    tes.setNominalCoolingCapacity(value);
  }

  std::string storLctnSim = tesElement.child("StorLctnSim").text().as_string();
  if( istringEqual("Zone",storLctnSim) ) {
    tes.setAmbientTemperatureIndicator("Zone");
    std::string storZnRef = tesElement.child("StorZnRef").text().as_string();
    if( auto tz = model.getModelObjectByName<model::ThermalZone>(storZnRef) ) {
      tes.setAmbientTemperatureThermalZone(tz.get());
    }
  } else {
    tes.setAmbientTemperatureIndicator("Outdoors");
  }

  tes.resetAmbientTemperatureSchedule();

  boost::optional<double> _tankUFac = lexicalCastToDouble(tesElement.child("TankUFac"));
  if( _tankUFac ) {
    // sdd units = Btu/(hr*ft^2*F), os units = W/(m^2*K)
    Quantity uaIP(_tankUFac.get(), BTUUnit(BTUExpnt(1,-2,-1,-1)));
    auto uaSI = QuantityConverter::instance().convert(uaIP, UnitSystem(UnitSystem::Wh));
    OS_ASSERT(uaSI);
    OS_ASSERT(uaSI->units() == WhUnit(WhExpnt(1,0,-2,-1)));
    tes.setUniformSkinLossCoefficientperUnitAreatoAmbientTemperature(uaSI->value());
  }

  tes.setUseSideHeatTransferEffectiveness(1.0);

  std::string dischrgSchRef = tesElement.child("DischrgSchRef").text().as_string();
  if( auto schedule = model.getModelObjectByName<model::Schedule>(dischrgSchRef) ) {
    tes.setUseSideAvailabilitySchedule(schedule.get());
  }

  tes.setUseSideOutletHeight(0.0);

  boost::optional<double> _wtrFlowCap = lexicalCastToDouble(tesElement.child("WtrFlowCapSim"));
  if( _wtrFlowCap ) {
    double value = unitToUnit(_wtrFlowCap.get(),"gal/min","m^3/s").get();
    tes.setUseSideDesignFlowRate(value);
    tes.setSourceSideDesignFlowRate(value);
  }

  tes.setSourceSideHeatTransferEffectiveness(1.0);

  std::string chrgSchRef = tesElement.child("ChrgSchRef").text().as_string();
  if( auto schedule = model.getModelObjectByName<model::Schedule>(chrgSchRef) ) {
    tes.setSourceSideAvailabilitySchedule(schedule.get());
  }

  boost::optional<int> _numberOfNodes = lexicalCastToInt(tesElement.child("TankNodeCnt"));
  if( _numberOfNodes ) {
    tes.setNumberofNodes(_numberOfNodes.get());
  } else {
    tes.setNumberofNodes(5);
  }

  tes.setSourceSideInletHeight(0.0);
  tes.setInletMode("Fixed");
  tes.setAdditionalDestratificationConductivity(0.0);

  return result;
}

boost::optional<openstudio::model::ModelObject> ReverseTranslator::translateProcLd(
                                                  const pugi::xml_node& element,
                                                  openstudio::model::Model& model )
{
  if( ! istringEqual(element.name(),"ProcLd") ) {
    return boost::none;
  }

  std::string name = element.child("Name").text().as_string();
  openstudio::model::LoadProfilePlant load(model);
  load.setName(name);

  const auto ldSchRefElement = element.child("LdSchRef");
  auto ldSch = model.getModelObjectByName<model::Schedule>(ldSchRefElement.text().as_string());
  if (ldSch) {
    auto typeLimits = ldSch->scheduleTypeLimits();
    if (! typeLimits) {
      typeLimits = model.getModelObjectByName<model::ScheduleTypeLimits>("Load");
      if (! typeLimits) {
        typeLimits = model::ScheduleTypeLimits(model);
        typeLimits->setName("Load");
        typeLimits->setNumericType("Continuous");
      }
    }
    OS_ASSERT(typeLimits);
    ldSch->setScheduleTypeLimits(typeLimits.get());

    load.setLoadSchedule(ldSch.get());
  }

  const auto pkFlowRtElement = element.child("PkFlowRt");
  if (pkFlowRtElement) {
    const auto pkFlowRt = unitToUnit(pkFlowRtElement.text().as_double(), "gal/min", "m^3/s").get();
    load.setPeakFlowRate(pkFlowRt);
  }

  const auto flowFracSchRefElement = element.child("FlowFracSchRef");
  auto flowFracSch = model.getModelObjectByName<model::Schedule>(flowFracSchRefElement.text().as_string());
  if (flowFracSch) {
    auto typeLimits = flowFracSch->scheduleTypeLimits();
    if (! typeLimits) {
      typeLimits = model.getModelObjectByName<model::ScheduleTypeLimits>("Flow Rate Fraction");
      if (! typeLimits) {
        typeLimits = model::ScheduleTypeLimits(model);
        typeLimits->setName("Flow Rate Fraction");
        typeLimits->setLowerLimitValue(0);
        typeLimits->setUpperLimitValue(1);
        typeLimits->setNumericType("Continuous");
        typeLimits->setUnitType("Dimensionless");
      }
    }
    OS_ASSERT(typeLimits);
    flowFracSch->setScheduleTypeLimits(typeLimits.get());

    load.setFlowRateFractionSchedule(flowFracSch.get());
  }

  return load;
}

boost::optional<openstudio::model::ModelObject> ReverseTranslator::translateChiller(
                                                  const pugi::xml_node& chillerElement,
                                                  openstudio::model::Model& model )
{
  boost::optional<openstudio::model::ModelObject> result;

  if( ! istringEqual(chillerElement.name(),"Chlr") ) {
    return result;
  }


  std::string name = chillerElement.child("Name").text().as_string();
  auto typeElement = chillerElement.child("Type");

  if( istringEqual("AbsorptionSingleEffect",typeElement.text().as_string()) ) {
    model::ChillerAbsorptionIndirect chiller(model);
    chiller.setName(name);
    result = chiller;

    auto cndsrInRefElement = chillerElement.child("CndsrFluidSegInRef");
    if (!cndsrInRefElement) {
      LOG(Error, "ChillerAbsorptionIndirect '" << name << "' doesn't have a CndsrFluidSegInRef element! It will not be connected.");
    } else {
      auto condenserSystem = loopForSupplySegment(cndsrInRefElement ,model);
      if( condenserSystem ) {
        condenserSystem->addDemandBranchForComponent(chiller);
        double value = condenserSystem->sizingPlant().designLoopExitTemperature();
        chiller.setDesignCondenserWaterFlowRate(value);
      }
    }

    auto genFluidSegInRefElement = chillerElement.child("GenFluidSegInRef");
    if (!genFluidSegInRefElement) {
      LOG(Error, "ChillerAbsorptionIndirect '" << name << "' doesn't have a GenFluidSegInRef element! It will not be connected.");
    } else {
      auto genSystem = loopForSupplySegment(genFluidSegInRefElement, model);
      if( genSystem ) {
        genSystem->addDemandBranchForComponent(chiller,true);
      }
    }

    if( ! autosize() ) {
      auto capRtdElement = chillerElement.child("CapRtdSim");
      boost::optional<double> _capRtd = lexicalCastToDouble(capRtdElement);
      if( _capRtd ) {
        double value = unitToUnit(_capRtd.get(),"Btu/h","W").get();
        chiller.setNominalCapacity(value);
      }

      auto auxPwrElement = chillerElement.child("AuxPwrSim");
      boost::optional<double> _auxPwr = lexicalCastToDouble(auxPwrElement);
      if( _auxPwr ) {
        double value = unitToUnit(_auxPwr.get(),"Btu/h","W").get();
        chiller.setNominalPumpingPower(value);
      }

      auto wtrFlowCapElement = chillerElement.child("WtrFlowCapSim");
      boost::optional<double> _wtrFlowCap = lexicalCastToDouble(wtrFlowCapElement);
      if( _wtrFlowCap ) {
        double value = unitToUnit(_wtrFlowCap.get(),"gal/min","m^3/s").get();
        chiller.setDesignChilledWaterFlowRate(value);
      }

      auto genFluidFlowCapElement = chillerElement.child("GenFluidFlowCapSim");
      boost::optional<double> _genFluidFlowCap = lexicalCastToDouble(genFluidFlowCapElement);
      if( _genFluidFlowCap ) {
        double value = unitToUnit(_genFluidFlowCap.get(),"gal/min","m^3/s").get();
        chiller.setDesignGeneratorFluidFlowRate(value);
      }

      auto cndsrFlowCapElement = chillerElement.child("CndsrFlowCapSim");
      boost::optional<double> _cndsrFlowCap = lexicalCastToDouble(cndsrFlowCapElement);
      if( _cndsrFlowCap ) {
        double value = unitToUnit(_cndsrFlowCap.get(),"gal/min","m^3/s").get();
        chiller.setDesignCondenserWaterFlowRate(value);
      }
    }

    auto partLdRatMinElement = chillerElement.child("PLRMin");
    boost::optional<double> _partLdRatMin = lexicalCastToDouble(partLdRatMinElement);
    if( _partLdRatMin ) {
      chiller.setMinimumPartLoadRatio(_partLdRatMin.get());
    }

    chiller.setMaximumPartLoadRatio(1.1);

    chiller.setOptimumPartLoadRatio(1.0);

    auto flowMode = chillerElement.child("FlowMode").text().as_string();
    if (istringEqual(flowMode,"Constant")) {
      chiller.setChillerFlowMode("ConstantFlow");
    } else {
      chiller.setChillerFlowMode("LeavingSetpointModulated");
    }

    if( chiller.tertiaryPlantLoop() ) {
      chiller.setGeneratorHeatSourceType("HotWater");
    } else {
      chiller.setGeneratorHeatSourceType("Steam");
    }

    chiller.setTemperatureLowerLimitGeneratorInlet(60.0);

    chiller.setSizingFactor(1.0);

    {
      auto curveElement = chillerElement.child("HIR_fPLRCrvRef");
      if( auto curve = model.getModelObjectByName<model::Curve>(curveElement.text().as_string()) ) {
        auto oldCurve = chiller.generatorHeatInputFunctionofPartLoadRatioCurve();
        if( chiller.setGeneratorHeatInputFunctionofPartLoadRatioCurve(curve.get()) ) {
          oldCurve.remove();
        }
      }
    }

    {
      auto curveElement = chillerElement.child("HIR_fCndTempCrvRef");
      if( auto curve = model.getModelObjectByName<model::Curve>(curveElement.text().as_string()) ) {
        auto oldCurve = chiller.generatorHeatInputCorrectionFunctionofCondenserTemperatureCurve();
        if( chiller.setGeneratorHeatInputCorrectionFunctionofCondenserTemperatureCurve(curve.get()) ) {
          oldCurve.remove();
        }
      }
    }

    {
      auto curveElement = chillerElement.child("HIR_fEvapTempCrvRef");
      if( auto curve = model.getModelObjectByName<model::Curve>(curveElement.text().as_string()) ) {
        auto oldCurve = chiller.generatorHeatInputCorrectionFunctionofChilledWaterTemperatureCurve();
        if( chiller.setGeneratorHeatInputCorrectionFunctionofChilledWaterTemperatureCurve(curve.get()) ) {
          oldCurve.remove();
        }
      }
    }

    {
      auto curveElement = chillerElement.child("Cap_fCndTempCrvRef");
      if( auto curve = model.getModelObjectByName<model::Curve>(curveElement.text().as_string()) ) {
        auto oldCurve = chiller.capacityCorrectionFunctionofCondenserTemperatureCurve();
        if( chiller.setCapacityCorrectionFunctionofCondenserTemperatureCurve(curve.get()) ) {
          oldCurve.remove();
        }
      }
    }

    {
      auto curveElement = chillerElement.child("Cap_fEvapTempCrvRef");
      if( auto curve = model.getModelObjectByName<model::Curve>(curveElement.text().as_string()) ) {
        auto oldCurve = chiller.capacityCorrectionFunctionofChilledWaterTemperatureCurve();
        if( chiller.setCapacityCorrectionFunctionofChilledWaterTemperatureCurve(curve.get()) ) {
          oldCurve.remove();
        }
      }
    }

    {
      auto curveElement = chillerElement.child("Cap_fGenTempCrvRef");
      if( auto curve = model.getModelObjectByName<model::Curve>(curveElement.text().as_string()) ) {
        auto oldCurve = chiller.capacityCorrectionFunctionofGeneratorTemperatureCurve();
        if( chiller.setCapacityCorrectionFunctionofGeneratorTemperatureCurve(curve.get()) ) {
          oldCurve.remove();
        }
      }
    }
  } else {
    // Cap_fTempCrvRef
    boost::optional<model::CurveBiquadratic> cap_fTempCrv;
    pugi::xml_node cap_fTempCrvElement = chillerElement.child("Cap_fTempCrvRef");
    cap_fTempCrv = model.getModelObjectByName<model::CurveBiquadratic>(cap_fTempCrvElement.text().as_string());
    if( ! cap_fTempCrv ) {
      LOG(Error,"Coil: " << name << " Broken Cap_fTempCrv");

      cap_fTempCrv = model::CurveBiquadratic(model);
      cap_fTempCrv->setCoefficient1Constant(1.0215158);
      cap_fTempCrv->setCoefficient2x(0.037035864);
      cap_fTempCrv->setCoefficient3xPOW2(0.0002332476);
      cap_fTempCrv->setCoefficient4y(-0.003894048);
      cap_fTempCrv->setCoefficient5yPOW2(-6.52536e-005);
      cap_fTempCrv->setCoefficient6xTIMESY(-0.0002680452);
      cap_fTempCrv->setMinimumValueofx(5.0);
      cap_fTempCrv->setMaximumValueofx(10.0);
      cap_fTempCrv->setMinimumValueofy(24.0);
      cap_fTempCrv->setMaximumValueofy(35.0);
    }

    // EIR_fTempCrvRef
    boost::optional<model::CurveBiquadratic> eir_fTempCrv;
    pugi::xml_node eir_fTempCrvElement = chillerElement.child("EIR_fTempCrvRef");
    eir_fTempCrv = model.getModelObjectByName<model::CurveBiquadratic>(eir_fTempCrvElement.text().as_string());
    if( ! eir_fTempCrv ) {
      LOG(Error,"Coil: " << name << "Broken EIR_fTempCrvRef");

      eir_fTempCrv = model::CurveBiquadratic(model);
      eir_fTempCrv->setCoefficient1Constant(0.70176857);
      eir_fTempCrv->setCoefficient2x(-0.00452016);
      eir_fTempCrv->setCoefficient3xPOW2(0.0005331096);
      eir_fTempCrv->setCoefficient4y(-0.005498208);
      eir_fTempCrv->setCoefficient5yPOW2(0.0005445792);
      eir_fTempCrv->setCoefficient6xTIMESY(-0.0007290324);
      eir_fTempCrv->setMinimumValueofx(5.0);
      eir_fTempCrv->setMaximumValueofx(10.0);
      eir_fTempCrv->setMinimumValueofy(24.0);
      eir_fTempCrv->setMaximumValueofy(35.0);
    }

    // EIR_fPLRCrvRef
    boost::optional<model::CurveQuadratic> eir_fPLRCrv;
    pugi::xml_node eir_fPLRCrvElement = chillerElement.child("EIR_fPLRCrvRef");
    eir_fPLRCrv = model.getModelObjectByName<model::CurveQuadratic>(eir_fPLRCrvElement.text().as_string());
    if( ! eir_fPLRCrv ) {
      LOG(Error,"Coil: " << name << "Broken EIR_fPLRCrvRef");

      eir_fPLRCrv = model::CurveQuadratic(model);
      eir_fPLRCrv->setCoefficient1Constant(0.06369119);
      eir_fPLRCrv->setCoefficient2x(0.58488832);
      eir_fPLRCrv->setCoefficient3xPOW2(0.35280274);
      eir_fPLRCrv->setMinimumValueofx(0.0);
      eir_fPLRCrv->setMaximumValueofx(1.0);
    }

    model::ChillerElectricEIR chiller(model,cap_fTempCrv.get(),eir_fTempCrv.get(),eir_fPLRCrv.get());
    result = chiller;

    // Name
    chiller.setName(name);

    auto flowMode = chillerElement.child("FlowMode").text().as_string();
    if (istringEqual(flowMode,"Constant")) {
      chiller.setChillerFlowMode("ConstantFlow");
    } else {
      chiller.setChillerFlowMode("LeavingSetpointModulated");
    }

    // CndsrInRef
    // Will be aircooled if Condenser not specified
    pugi::xml_node cndsrInRefElement = chillerElement.child("CndsrFluidSegInRef");
    if (cndsrInRefElement) {
      boost::optional<model::PlantLoop> condenserSystem = loopForSupplySegment(cndsrInRefElement, model);
      if( condenserSystem ) {
        condenserSystem->addDemandBranchForComponent(chiller);
        double value = condenserSystem->sizingPlant().designLoopExitTemperature();
        chiller.setReferenceEnteringCondenserFluidTemperature(value);
      }
    }

    // COP
    boost::optional<double> cop;
    pugi::xml_node copElement = chillerElement.child("COP");
    boost::optional<double> _cop = lexicalCastToDouble(copElement);
    if( _cop ) {
      double value = _cop.get();
      chiller.setReferenceCOP(value);
      cop = value;
    }

    // PartLdRatMin
    pugi::xml_node partLdRatMinElement = chillerElement.child("PLRMin");
    boost::optional<double> _partLdRatMin = lexicalCastToDouble(partLdRatMinElement);
    if( _partLdRatMin ) {
      chiller.setMinimumPartLoadRatio(_partLdRatMin.get());
    }

    // UnldRatMin
    pugi::xml_node unldRatMinElement = chillerElement.child("UnldRatMin");
    boost::optional<double> _unldRatMin = lexicalCastToDouble(unldRatMinElement);
    if( _unldRatMin ) {
      chiller.setMinimumUnloadingRatio(_unldRatMin.get());
    }

    if( ! autosize() ) {
      // CapRtd
      boost::optional<double> capRtd;
      pugi::xml_node capRtdElement = chillerElement.child("CapRtdSim");
      boost::optional<double> _capRtd = lexicalCastToDouble(capRtdElement);
      if( _capRtd ) {
        capRtd = unitToUnit(_capRtd.get(),"Btu/h","W");
      }

      // EntTempDsgn
      boost::optional<double> entTempDsgn;
      pugi::xml_node entTempDsgnElement = chillerElement.child("EntTempDsgn");
      boost::optional<double> _entTempDsgn = lexicalCastToDouble(entTempDsgnElement);
      if( _entTempDsgn ) {
        entTempDsgn = unitToUnit(_entTempDsgn.get(),"F","C");
      }

      // LvgTempDsgn
      boost::optional<double> lvgTempDsgn;
      pugi::xml_node lvgTempDsgnElement = chillerElement.child("LvgTempDsgn");
      boost::optional<double> _lvgTempDsgn = lexicalCastToDouble(lvgTempDsgnElement);
      if( _lvgTempDsgn ) {
        lvgTempDsgn = unitToUnit(_lvgTempDsgn.get(),"F","C");
      }

      if( capRtd && entTempDsgn && lvgTempDsgn ) {
        chiller.setReferenceCapacity(capRtd.get());
        double flow = capRtd.get() / ( cpWater * densityWater * (entTempDsgn.get() - lvgTempDsgn.get()));
        chiller.setReferenceChilledWaterFlowRate(flow);
      }

      auto cndsrFlowCapElement = chillerElement.child("CndsrFlowCapSim");
      boost::optional<double> _cndsrFlowCap = lexicalCastToDouble(cndsrFlowCapElement);
      if( _cndsrFlowCap ) {
        double value = unitToUnit(_cndsrFlowCap.get(),"gal/min","m^3/s").get();
        chiller.setReferenceCondenserFluidFlowRate(value);
      }
    }
  }

  return result;
}

boost::optional<openstudio::model::ModelObject> ReverseTranslator::translateWtrHtr(
                                                  const pugi::xml_node& element,
                                                  openstudio::model::Model& model )
{
  boost::optional<openstudio::model::ModelObject> result;

  if( ! istringEqual(element.name(),"WtrHtr") )
  {
    return result;
  }

  auto fluidSysElement = element.parent();
  auto fluidSysType = fluidSysElement.child("Type").text().as_string();
  const std::string name = element.child("Name").text().as_string();

  model::ScheduleRuleset ambientTempSchedule(model);
  ambientTempSchedule.setName(name + " Ambient Temperature");
  model::ScheduleDay scheduleDay = ambientTempSchedule.defaultDaySchedule();
  scheduleDay.addValue(Time(1.0),20.0);


  std::string typeSim = element.child("TypeSim").text().as_string();
  if( istringEqual(typeSim,"HeatPumpSplit") ) {
    // Create objects and set defaults
    model::WaterHeaterHeatPump heatPump(model);
    heatPump.setName(name);
    heatPump.setDeadBandTemperatureDifference(0.01);
    heatPump.autosizeCondenserWaterFlowRate();
    heatPump.setFanPlacement("DrawThrough");
    heatPump.setOnCycleParasiticElectricLoad(0.0);
    heatPump.setOffCycleParasiticElectricLoad(0.0);
    heatPump.setMinimumInletAirTemperatureforCompressorOperation(5.0);
    heatPump.setMaximumInletAirTemperatureforCompressorOperation(94.0);

    auto waterHeater = heatPump.tank().cast<model::WaterHeaterMixed>();
    waterHeater.setName(name + " Storage Tank");
    waterHeater.setDeadbandTemperatureDifference(0.01);
    waterHeater.setHeaterControlType("Cycle");
    waterHeater.setHeaterMaximumCapacity(0.0);
    waterHeater.setHeaterMinimumCapacity(0.0);
    waterHeater.setHeaterIgnitionMinimumFlowRate(0.0);
    waterHeater.setHeaterIgnitionDelay(0.0);
    waterHeater.setHeaterThermalEfficiency(0.8);
    waterHeater.setOnCycleLossFractiontoThermalZone(1.0);
    waterHeater.setUseSideEffectiveness(1.0);
    waterHeater.setSourceSideEffectiveness(1.0);
    waterHeater.setIndirectWaterHeatingRecoveryTime(1.5);
    waterHeater.setOnCycleParasiticHeatFractiontoTank(0.8);

    auto coil = heatPump.dXCoil().cast<model::CoilWaterHeatingAirToWaterHeatPump>();
    coil.setName(name + " Heating Coil");
    coil.setRatedEvaporatorInletAirDryBulbTemperature(19.7);
    coil.setRatedEvaporatorInletAirWetBulbTemperature(13.5);
    coil.setRatedCondenserInletWaterTemperature(57.5);
    coil.setEvaporatorFanPowerIncludedinRatedCOP(true);
    coil.setCondenserPumpHeatIncludedinRatedHeatingCapacityandRatedCOP(true);
    coil.setCondenserPumpPowerIncludedinRatedCOP(true);
    coil.setFractionofCondenserPumpHeattoWater(0.2);
    coil.setMaximumAmbientTemperatureforCrankcaseHeaterOperation(10.0);
    coil.setEvaporatorAirTemperatureTypeforCurveObjects("DryBulbTemperature");

    auto fan = heatPump.fan().cast<model::FanOnOff>();
    fan.setName(name + " Fan");
    fan.setFanEfficiency(0.5);
    fan.setMotorEfficiency(0.85);
    fan.setMotorInAirstreamFraction(1.0);
    fan.setEndUseSubcategory(" ");

    // Save coil so that a meter can be created later
    if( istringEqual(fluidSysType, "HotWater") ) {
      m_spaceHeatingWaterHeaters.push_back(waterHeater);
      m_spaceHeatingAirToWaterHeatPumps.push_back(coil);
      m_spaceHeatingFans.push_back(fan);
    }

    // Heat pump inputs
    auto minInletAirTempCprsrOper = lexicalCastToDouble(element.child("MinInletAirTempCprsrOper"));
    if ( minInletAirTempCprsrOper ) {
      heatPump.setMinimumInletAirTemperatureforCompressorOperation( unitToUnit(minInletAirTempCprsrOper.get(), "F", "C").get() );
    }

    auto maxInletAirTempCprsrOper = lexicalCastToDouble(element.child("MaxInletAirTempCprsrOper"));
    if ( maxInletAirTempCprsrOper ) {
      heatPump.setMaximumInletAirTemperatureforCompressorOperation( unitToUnit(maxInletAirTempCprsrOper.get(), "F", "C").get() );
    }

    {
      auto schedule = model.alwaysOnDiscreteSchedule();
      heatPump.setAvailabilitySchedule(schedule);
    }

    std::string cprsrZn = element.child("CprsrZn").text().as_string();
    if( istringEqual(cprsrZn,"Zone") ) {
      heatPump.setInletAirConfiguration("ZoneAirOnly");
      heatPump.setCompressorLocation("Zone");
      heatPump.setParasiticHeatRejectionLocation("Zone");
    } else {
      heatPump.setInletAirConfiguration("OutdoorAirOnly");
      heatPump.setCompressorLocation("Outdoors");
      heatPump.setParasiticHeatRejectionLocation("Outdoors");
    }

    // Might have to relocate after zones are available
    std::string cprsrZnRef = element.child("CprsrZnRef").text().as_string();
    if( auto zone = model.getModelObjectByName<model::ThermalZone>(cprsrZnRef) ) {
      heatPump.addToThermalZone(zone.get());
    }

    auto _fanFlowCapSim = lexicalCastToDouble(element.child("FanFlowCapSim"));
    if( _fanFlowCapSim ) {
      double value = unitToUnit(_fanFlowCapSim.get(),"cfm","m^3/s").get();
      heatPump.setEvaporatorAirFlowRate(value);
    } else {
      heatPump.autosizeEvaporatorAirFlowRate();
    }

    std::string fanPlcmt = element.child("FanPlcmt").text().as_string();
    if (! fanPlcmt.empty()) {
      heatPump.setFanPlacement(fanPlcmt);
    }

    auto dsgnSupWtrTemp = lexicalCastToDouble(fluidSysElement.child("DsgnSupWtrTemp"));
    if (dsgnSupWtrTemp) {
      dsgnSupWtrTemp = unitToUnit(dsgnSupWtrTemp.get(),"F","C").get();
    }
    auto tankDeadBandTempHtPump = lexicalCastToDouble(element.child("TankDeadBandTempHtPump"));
    if (tankDeadBandTempHtPump) {
      tankDeadBandTempHtPump = tankDeadBandTempHtPump.get() * 5.0 / 9.0; // delta F to C
    }

    if (tankDeadBandTempHtPump) {
      heatPump.setDeadBandTemperatureDifference(tankDeadBandTempHtPump.get());
    }

    // Water heater inputs
    boost::optional<double> _storCapSim = lexicalCastToDouble(element.child("StorCapSim"));
    if( _storCapSim ) {
      waterHeater.setTankVolume(unitToUnit(_storCapSim.get(),"gal","m^3").get());
    }

    auto wtrHtrSetPtTemp = lexicalCastToDouble(element.child("WtrHtrSetPtTemp"));
    if (wtrHtrSetPtTemp) {
      wtrHtrSetPtTemp = unitToUnit(wtrHtrSetPtTemp.get(),"F","C").get();
      model::ScheduleConstant schedule(model);
      schedule.setName(waterHeater.nameString() + " Setpoint");
      schedule.setValue(wtrHtrSetPtTemp.get());
      waterHeater.setSetpointTemperatureSchedule(schedule);
    }

    auto tankDeadBandTempInternal = lexicalCastToDouble(element.child("TankDeadBandTempInternal"));
    if (tankDeadBandTempInternal) {
      tankDeadBandTempInternal = tankDeadBandTempInternal.get() * 5.0 / 9.0; // delta F to C
      waterHeater.setDeadbandTemperatureDifference(tankDeadBandTempInternal.get());
    } else {
      tankDeadBandTempInternal = 0.0;
    }

    if (dsgnSupWtrTemp && tankDeadBandTempHtPump && tankDeadBandTempInternal) {
      heatPump.compressorSetpointTemperatureSchedule().remove();
      model::ScheduleConstant schedule(model);
      schedule.setName(heatPump.name().get() + " Compressor Setpoint Temp");
      schedule.setValue(dsgnSupWtrTemp.get() + tankDeadBandTempHtPump.get() + tankDeadBandTempInternal.get() + 0.5);
      heatPump.setCompressorSetpointTemperatureSchedule(schedule);
    }

    auto capRtdSupp = lexicalCastToDouble(element.child("CapRtdSupp"));
    if (capRtdSupp) {
      capRtdSupp = unitToUnit(capRtdSupp.get(),"Btu/h","W").get();
      waterHeater.setHeaterMaximumCapacity(capRtdSupp.get());
    }

    std::string storZn = element.child("StorZn").text().as_string();
    if( istringEqual(storZn,"Zone") ) {
      waterHeater.setAmbientTemperatureIndicator("ThermalZone");
    } else if (istringEqual(storZn, "Outdoor")) {
      waterHeater.setAmbientTemperatureIndicator("Outdoors");
    } else {
      waterHeater.setAmbientTemperatureIndicator("Schedule");
      waterHeater.setAmbientTemperatureSchedule(ambientTempSchedule);
    }

    std::string storZnRef = element.child("StorZnRef").text().as_string();
    if( auto zone = model.getModelObjectByName<model::ThermalZone>(storZnRef) ) {
      waterHeater.setAmbientTemperatureThermalZone(zone.get());
    }

    std::string fuelSrc = element.child("FuelSrc").text().as_string();
    waterHeater.setHeaterFuelType(fuelSrc);

    if(istringEqual(fuelSrc, "electricity")) {
      // Default for electric water heaters
      waterHeater.setHeaterThermalEfficiency(1.0);
    }

    auto thrmlEff = lexicalCastToDouble(element.child("ThrmlEff"));
    if (thrmlEff) {
      waterHeater.setHeaterThermalEfficiency(thrmlEff.get());
    }

    {
      auto curveRef = element.child("HIR_fPLRCrvRef").text().as_string();
      auto newcurve = model.getModelObjectByName<model::Curve>(curveRef);
      if( newcurve ) {
        auto oldcurve = waterHeater.partLoadFactorCurve();
        if( oldcurve && (oldcurve.get() != newcurve.get()) ) {
          oldcurve->remove();
        }
        waterHeater.setPartLoadFactorCurve(newcurve->cast<model::CurveCubic>());
      }
    }

    boost::optional<double> _offCyclePrstcLossSim = lexicalCastToDouble(element.child("OffCyclePrstcLossSim"));
    if( _offCyclePrstcLossSim ) {
      waterHeater.setOffCycleParasiticFuelConsumptionRate(_offCyclePrstcLossSim.get());
    }

    std::string offCycleFuelSrc = element.child("OffCycleFuelSrc").text().as_string();
    waterHeater.setOffCycleParasiticFuelType(offCycleFuelSrc);

    waterHeater.setOffCycleParasiticHeatFractiontoTank(0.8);

    boost::optional<double> _onCyclePrstcLossSim = lexicalCastToDouble(element.child("OnCyclePrstcLossSim"));
    if( _onCyclePrstcLossSim ) {
      waterHeater.setOnCycleParasiticFuelConsumptionRate(_onCyclePrstcLossSim.get());
    }

    std::string onCycleFuelSrc = element.child("OnCycleFuelSrc").text().as_string();
    waterHeater.setOnCycleParasiticFuelType(onCycleFuelSrc);

    boost::optional<double> _tankOffCycleLossCoefSim = lexicalCastToDouble(element.child("TankOffCycleLossCoefSim"));
    if( _tankOffCycleLossCoefSim ) {
      waterHeater.setOffCycleLossCoefficienttoAmbientTemperature(_tankOffCycleLossCoefSim.get() * 0.5275);
    }

    waterHeater.setOffCycleLossFractiontoThermalZone(1.0);

    boost::optional<double> _tankOnCycleLossCoefSim = lexicalCastToDouble(element.child("TankOnCycleLossCoefSim"));
    if( _tankOnCycleLossCoefSim ) {
      waterHeater.setOnCycleLossCoefficienttoAmbientTemperature(_tankOnCycleLossCoefSim.get() * 0.5275);
    }

    // Coil inputs
    boost::optional<double> _capRtdSim = lexicalCastToDouble(element.child("CapRtdSim"));
    if( _capRtdSim ) {
      coil.setRatedHeatingCapacity(unitToUnit(_capRtdSim.get(),"Btu/h","W").get());
    }

    auto copSim = lexicalCastToDouble(element.child("COPSim"));
    if( copSim ) {
      coil.setRatedCOP(copSim.get());
    }

    boost::optional<double> _cndsrPumpPwrSim = lexicalCastToDouble(element.child("CndsrPumpPwrSim"));
    if( _cndsrPumpPwrSim ) {
      coil.setCondenserWaterPumpPower(_cndsrPumpPwrSim.get());
    }

    boost::optional<double> _crankcaseHtrCapSim = lexicalCastToDouble(element.child("CrankcaseHtrCapSim"));
    if( _crankcaseHtrCapSim ) {
      coil.setCrankcaseHeaterCapacity(_crankcaseHtrCapSim.get());
    }

    boost::optional<double> _rtdSensHtRat = lexicalCastToDouble(element.child("RtdSensHtRat"));
    if( _rtdSensHtRat ) {
      coil.setRatedSensibleHeatRatio(_rtdSensHtRat.get());
    } else {
      coil.setRatedSensibleHeatRatio(0.85);
    }

    auto rtdEvapInletAirDBTemp = lexicalCastToDouble(element.child("RtdEvapInletAirDBTemp"));
    if (rtdEvapInletAirDBTemp) {
      coil.setRatedEvaporatorInletAirDryBulbTemperature(unitToUnit(rtdEvapInletAirDBTemp.get(),"F","C").get());
    }

    auto rtdEvapInletAirWBTemp = lexicalCastToDouble(element.child("RtdEvapInletAirWBTemp"));
    if (rtdEvapInletAirWBTemp) {
      coil.setRatedEvaporatorInletAirWetBulbTemperature(unitToUnit(rtdEvapInletAirWBTemp.get(),"F","C").get());
    }

    auto rtdCondInletWtrTemp = lexicalCastToDouble(element.child("RtdCondInletWtrTemp"));
    if (rtdCondInletWtrTemp) {
      coil.setRatedCondenserInletWaterTemperature(unitToUnit(rtdCondInletWtrTemp.get(),"F","C").get());
    }

    auto evapFanPwrInCOP = element.child("EvapFanPwrInCOP").text().as_string();
    if (istringEqual(evapFanPwrInCOP, "No")) {
      coil.setEvaporatorFanPowerIncludedinRatedCOP(false);
    } else if (istringEqual(evapFanPwrInCOP, "Yes")) {
      coil.setEvaporatorFanPowerIncludedinRatedCOP(true);
    }

    auto cndsrPumpPwrInCOP = element.child("CndsrPumpPwrInCOP").text().as_string();
    if (istringEqual(cndsrPumpPwrInCOP, "No")) {
      coil.setCondenserPumpPowerIncludedinRatedCOP(false);
    } else if (istringEqual(cndsrPumpPwrInCOP, "Yes")) {
      coil.setCondenserPumpPowerIncludedinRatedCOP(true);
    }

    auto cndsrPumpHtInCapAndCOP = element.child("CndsrPumpHtInCapAndCOP").text().as_string();
    if (istringEqual(cndsrPumpHtInCapAndCOP, "No")) {
      coil.setCondenserPumpHeatIncludedinRatedHeatingCapacityandRatedCOP(false);
    } else if (istringEqual(cndsrPumpHtInCapAndCOP, "Yes")) {
      coil.setCondenserPumpHeatIncludedinRatedHeatingCapacityandRatedCOP(true);
    }

    auto evapAirTempType = element.child("EvapAirTempType");
    if (evapAirTempType) {
      coil.setEvaporatorAirTemperatureTypeforCurveObjects(evapAirTempType.text().as_string());
    }

    auto cndsrPumpHtFracToWtr = lexicalCastToDouble(element.child("CndsrPumpHtFracToWtr"));
    if (cndsrPumpHtFracToWtr) {
      coil.setFractionofCondenserPumpHeattoWater(cndsrPumpHtFracToWtr.get());
    }

    auto crankcaseHtrMaxT = lexicalCastToDouble(element.child("CrankcaseHtrMaxT"));
    if (crankcaseHtrMaxT) {
      crankcaseHtrMaxT = unitToUnit(crankcaseHtrMaxT.get(),"F","C").get();
      coil.setMaximumAmbientTemperatureforCrankcaseHeaterOperation(crankcaseHtrMaxT.get());
    }

    auto setCurve = [&](const std::string & elementName,
        const std::function<bool(model::CoilWaterHeatingAirToWaterHeatPump &,const model::Curve &)> & osSetter,
        const std::function<model::Curve(model::CoilWaterHeatingAirToWaterHeatPump &)> & osGetter) {

      auto value = element.child(elementName.c_str()).text().as_string();
      auto newcurve = model.getModelObjectByName<model::Curve>(value);
      if( newcurve ) {
        auto oldcurve = osGetter(coil);
        if( oldcurve != newcurve.get() ) {
          oldcurve.remove();
        }
        osSetter(coil,newcurve.get());
      }
    };

    setCurve("Cap_fTempCrvRef",
      std::mem_fn(&model::CoilWaterHeatingAirToWaterHeatPump::setHeatingCapacityFunctionofTemperatureCurve),
      std::mem_fn(&model::CoilWaterHeatingAirToWaterHeatPump::heatingCapacityFunctionofTemperatureCurve)
    );

    setCurve("Cap_fAirFlowFracCrvRef",
      std::mem_fn(&model::CoilWaterHeatingAirToWaterHeatPump::setHeatingCapacityFunctionofAirFlowFractionCurve),
      std::mem_fn(&model::CoilWaterHeatingAirToWaterHeatPump::heatingCapacityFunctionofAirFlowFractionCurve)
    );

    setCurve("Cap_fWtrFlowFracCrvRef",
      std::mem_fn(&model::CoilWaterHeatingAirToWaterHeatPump::setHeatingCapacityFunctionofWaterFlowFractionCurve),
      std::mem_fn(&model::CoilWaterHeatingAirToWaterHeatPump::heatingCapacityFunctionofWaterFlowFractionCurve)
    );

    setCurve("COP_fTempCrvRef",
      std::mem_fn(&model::CoilWaterHeatingAirToWaterHeatPump::setHeatingCOPFunctionofTemperatureCurve),
      std::mem_fn(&model::CoilWaterHeatingAirToWaterHeatPump::heatingCOPFunctionofTemperatureCurve)
    );

    setCurve("COP_fAirFlowFracCrvRef",
      std::mem_fn(&model::CoilWaterHeatingAirToWaterHeatPump::setHeatingCOPFunctionofAirFlowFractionCurve),
      std::mem_fn(&model::CoilWaterHeatingAirToWaterHeatPump::heatingCOPFunctionofAirFlowFractionCurve)
    );

    setCurve("COP_fWtrFlowFracCrvRef",
      std::mem_fn(&model::CoilWaterHeatingAirToWaterHeatPump::setHeatingCOPFunctionofWaterFlowFractionCurve),
      std::mem_fn(&model::CoilWaterHeatingAirToWaterHeatPump::heatingCOPFunctionofWaterFlowFractionCurve)
    );

    {
      auto curve = coil.partLoadFractionCorrelationCurve().cast<model::CurveQuadratic>();
      curve.setCoefficient1Constant(1.0);
      curve.setCoefficient2x(0.0);
      curve.setCoefficient3xPOW2(0.0);
      curve.setMinimumValueofx(0.0);
      curve.setMaximumValueofx(1.0);
      coil.setPartLoadFractionCorrelationCurve(curve);
    }

    // fan inputs

    boost::optional<double> _fanTotStaticPressSim = lexicalCastToDouble(element.child("FanTotStaticPressSim"));
    if( _fanTotStaticPressSim ) {
      fan.setPressureRise(_fanTotStaticPressSim.get() * 249.0889 );
    }

    if( _fanFlowCapSim ) {
      double value = unitToUnit(_fanFlowCapSim.get(),"cfm","m^3/s").get();
      fan.setMaximumFlowRate(value);
      coil.setRatedEvaporatorAirFlowRate(value);
    } else {
      coil.autosizeRatedEvaporatorAirFlowRate();
      fan.autosizeMaximumFlowRate();
    }

    {
      auto fanPwr_fPLRCvRef = element.child("FanPwr_fPLRCvRef").text().as_string();
      auto curve = model.getModelObjectByName<model::Curve>(fanPwr_fPLRCvRef);
      if (curve) {
        fan.setFanPowerRatioFunctionofSpeedRatioCurve(curve.get());
      }
    }

    {
      auto fanEff_fPLRCvRef = element.child("FanEff_fPLRCvRef").text().as_string();
      auto curve = model.getModelObjectByName<model::Curve>(fanEff_fPLRCvRef);
      if (curve) {
        fan.setFanEfficiencyRatioFunctionofSpeedRatioCurve(curve.get());
      }
    }

    return waterHeater;
  } else {
    model::WaterHeaterMixed waterHeaterMixed(model);

    waterHeaterMixed.setName(name);

    // Ambient water temperature indicator
    waterHeaterMixed.setAmbientTemperatureIndicator("Schedule");
    waterHeaterMixed.setAmbientTemperatureSchedule(ambientTempSchedule);

    // StorCap

    pugi::xml_node wtrHtrStorCapElement = element.child("StorCapSim");
    boost::optional<double> _wtrHtrStorCap = lexicalCastToDouble(wtrHtrStorCapElement);
    if( _wtrHtrStorCap )
    {
      waterHeaterMixed.setTankVolume(unitToUnit(_wtrHtrStorCap.get(),"gal","m^3").get());
    }

    // CapRtd

    pugi::xml_node wtrHtrMaxCapElement = element.child("CapRtdSim");

    boost::optional<double> _wtrHtrMaxCap = lexicalCastToDouble(wtrHtrMaxCapElement);

    if( _wtrHtrMaxCap )
    {
      waterHeaterMixed.setHeaterMaximumCapacity(unitToUnit(_wtrHtrMaxCap.get(),"Btu/h","W").get());
    }

    // MinCap

    pugi::xml_node minCapElement = element.child("MinCap");

    boost::optional<double> _minCap = lexicalCastToDouble(minCapElement);

    if( _minCap )
    {
      waterHeaterMixed.setHeaterMinimumCapacity(unitToUnit(_minCap.get(),"Btu/h","W").get());
    }

    // OffCyclePrstcLoss

    pugi::xml_node offCyclePrstcLossElement = element.child("OffCyclePrstcLossSim");

    boost::optional<double> _offCyclePrstcLoss = lexicalCastToDouble(offCyclePrstcLossElement);

    if( _offCyclePrstcLoss )
    {
      waterHeaterMixed.setOffCycleParasiticFuelConsumptionRate(_offCyclePrstcLoss.get());
    }

    // OnCyclePrstcLoss

    pugi::xml_node onCyclePrstcLossElement = element.child("OnCyclePrstcLossSim");

    boost::optional<double> _onCyclePrstcLoss = lexicalCastToDouble(onCyclePrstcLossElement);

    if( _onCyclePrstcLoss )
    {
      waterHeaterMixed.setOnCycleParasiticFuelConsumptionRate(_onCyclePrstcLoss.get());
    }

    // TankOffCycleLossCoef

    pugi::xml_node tankOffCycleLossCoefElement = element.child("TankOffCycleLossCoefSim");

    boost::optional<double> _tankOffCycleLossCoef = lexicalCastToDouble(tankOffCycleLossCoefElement);

    if( _tankOffCycleLossCoef )
    {
      // Convert Btu/h-F to W/K
      waterHeaterMixed.setOffCycleLossCoefficienttoAmbientTemperature(_tankOffCycleLossCoef.get() * 0.5275);
    }

    // TankOnCycleLossCoef

    pugi::xml_node tankOnCycleLossCoefElement = element.child("TankOnCycleLossCoefSim");

    boost::optional<double> _tankOnCycleLossCoef = lexicalCastToDouble(tankOnCycleLossCoefElement);

    if( _tankOnCycleLossCoef )
    {
      // Convert Btu/h-F to W/K
      waterHeaterMixed.setOnCycleLossCoefficienttoAmbientTemperature(_tankOnCycleLossCoef.get() * 0.5275);
    }

    // Setpoint schedule
    auto fixedSupTempElement = element.parent().child("FixedSupTemp");
    if (fixedSupTempElement) {
      boost::optional<double> _fixedSupTemp = lexicalCastToDouble(fixedSupTempElement);
      if( _fixedSupTemp ) {
        double value = unitToUnit(_fixedSupTemp.get(),"F","C").get();
        model::ScheduleRuleset schedule(model);
        schedule.setName(waterHeaterMixed.name().get() + " Setpoint Temp");
        auto scheduleDay = schedule.defaultDaySchedule();
        scheduleDay.addValue(Time(1.0),value);
        waterHeaterMixed.setSetpointTemperatureSchedule(schedule);
      }
    } else {
      model::Schedule setpointTempSchedule = serviceHotWaterSetpointSchedule(model);
      waterHeaterMixed.setSetpointTemperatureSchedule(setpointTempSchedule);
    }

    // HIR_fPLRCrvRef

    pugi::xml_node hirfPLRCrvRefElement = element.child("HIR_fPLRCrvRef");
    boost::optional<model::CurveCubic> hirfPLRCrv = model.getModelObjectByName<model::CurveCubic>(hirfPLRCrvRefElement.text().as_string());
    if( hirfPLRCrv )
    {
      waterHeaterMixed.setPartLoadFactorCurve(hirfPLRCrv.get());
    }

    // FuelSrc
    auto fuelSrc = element.child("FuelSrc").text().as_string();
    waterHeaterMixed.setHeaterFuelType(fuelSrc);

    if(istringEqual(fuelSrc, "electricity")) {
      // Default for electric water heaters
      waterHeaterMixed.setHeaterThermalEfficiency(1.0);
    }

    // OffCycleFuelSrc
    auto offCycleFuelSrcElement = element.child("OffCycleFuelSrc");
    waterHeaterMixed.setOffCycleParasiticFuelType(offCycleFuelSrcElement.text().as_string());

    // OnCycleFuelSrc
    auto onCycleFuelSrcElement = element.child("OnCycleFuelSrc");
    waterHeaterMixed.setOnCycleParasiticFuelType(onCycleFuelSrcElement.text().as_string());

    // ThrmlEff
    pugi::xml_node thrmlEffElement = element.child("ThrmlEff");
    boost::optional<double> _thrmlEff = lexicalCastToDouble(thrmlEffElement);
    if( _thrmlEff )
    {
      // XML file can override default ThrmlEff
      waterHeaterMixed.setHeaterThermalEfficiency(_thrmlEff.get());
    }

    return waterHeaterMixed;
  }
}

boost::optional<openstudio::model::ModelObject> ReverseTranslator::translateZnSys(
    const pugi::xml_node& element,
    openstudio::model::ThermalZone& thermalZone)
{
  if( ! istringEqual(element.name(),"ZnSys") )
  {
    return boost::none;
  }

  auto model = thermalZone.model();

  boost::optional<model::ModelObject> result;

  // Name
  std::string name = element.child("Name").text().as_string();

  // Type
  pugi::xml_node typeElement = element.child("TypeSim");
  if (openstudio::istringEqual(typeElement.text().as_string(), "Exhaust"))
  {
    return boost::none;
  }
  std::string type = typeElement.text().as_string();

  // Availability Schedule
  pugi::xml_node scheduleElement = element.child("AvailSchRef");

  boost::optional<model::Schedule> schedule;

  if (scheduleElement)
  {
    schedule = model.getModelObjectByName<model::Schedule>(scheduleElement.text().as_string());
  }

  if( ! schedule )
  {
    schedule = model.alwaysOnDiscreteSchedule();
  }

  // FlowCap

  pugi::xml_node fanElement = element.child("Fan");

  pugi::xml_node flowCapElement = fanElement.child("FlowCapSim");
  pugi::xml_node flowMinElement = fanElement.child("FlowMinSim");

  boost::optional<double> flowCap;
  boost::optional<double> flowMin;

  if( ! autosize() )
  {
    boost::optional<double> _flowCap = lexicalCastToDouble(flowCapElement);

    if( _flowCap )
    {
      flowCap = unitToUnit(_flowCap.get(),"cfm","m^3/s").get();
    }

    boost::optional<double> _flowMin = lexicalCastToDouble(flowMinElement);

    if( _flowMin )
    {
      flowMin = unitToUnit(_flowMin.get(),"cfm","m^3/s").get();
    }
  }

  // HtgDsgnSupAirTemp
  boost::optional<double> htgDsgnSupAirTemp;

  if( ! autosize() )
  {
    pugi::xml_node htgDsgnSupAirTempElement = element.child("HtgDsgnSupAirTemp");

    boost::optional<double> _htgDsgnSupAirTemp = lexicalCastToDouble(htgDsgnSupAirTempElement);
    if (_htgDsgnSupAirTemp) {
      htgDsgnSupAirTemp = unitToUnit(_htgDsgnSupAirTemp.get(),"F","C").get();
    }
  }

  boost::optional<model::Schedule> fanOperModeSch;
  boost::optional<double> clgSupFanCapSim;
  boost::optional<double> htgSupFanCapSim;
  boost::optional<double> noClgSupFanCapSim;
  boost::optional<double> noHtgSupFanCapSim;
  boost::optional<double> noClgHtgSupFanCapSim;
  boost::optional<double> clgOAFlowCapSim;
  boost::optional<double> htgOAFlowCapSim;
  boost::optional<double> noClgHtgOAFlowCapSim;

  auto text = element.child("FanOperModeSchRef").text().as_string();
  fanOperModeSch = model.getModelObjectByName<model::Schedule>(text);

  clgSupFanCapSim = lexicalCastToDouble(element.child("ClgSupFanCapSim"));
  if (clgSupFanCapSim) {
    clgSupFanCapSim = unitToUnit(clgSupFanCapSim.get(),"cfm","m^3/s").get();
  }
  htgSupFanCapSim = lexicalCastToDouble(element.child("HtgSupFanCapSim"));
  if (htgSupFanCapSim) {
    htgSupFanCapSim = unitToUnit(htgSupFanCapSim.get(),"cfm","m^3/s").get();
  }
  noClgHtgSupFanCapSim = lexicalCastToDouble(element.child("NoClgHtgSupFanCapSim"));
  if (noClgHtgSupFanCapSim) {
    noClgHtgSupFanCapSim = unitToUnit(noClgHtgSupFanCapSim.get(),"cfm","m^3/s").get();
  }
  noClgSupFanCapSim = lexicalCastToDouble(element.child("NoClgSupFanCapSim"));
  if (noClgSupFanCapSim) {
    noClgSupFanCapSim = unitToUnit(noClgSupFanCapSim.get(),"cfm","m^3/s").get();
  }
  noHtgSupFanCapSim = lexicalCastToDouble(element.child("NoHtgSupFanCapSim"));
  if (noHtgSupFanCapSim) {
    noHtgSupFanCapSim = unitToUnit(noHtgSupFanCapSim.get(),"cfm","m^3/s").get();
  }
  clgOAFlowCapSim = lexicalCastToDouble(element.child("ClgOAFlowCapSim"));
  if (clgOAFlowCapSim) {
    clgOAFlowCapSim = unitToUnit(clgOAFlowCapSim.get(),"cfm","m^3/s").get();
  }
  htgOAFlowCapSim = lexicalCastToDouble(element.child("HtgOAFlowCapSim"));
  if (htgOAFlowCapSim) {
    htgOAFlowCapSim = unitToUnit(htgOAFlowCapSim.get(),"cfm","m^3/s").get();
  }
  noClgHtgOAFlowCapSim = lexicalCastToDouble(element.child("NoClgHtgOAFlowCapSim"));
  if (noClgHtgOAFlowCapSim) {
    noClgHtgOAFlowCapSim = unitToUnit(noClgHtgOAFlowCapSim.get(),"cfm","m^3/s").get();
  }

  if( istringEqual(type,"PTAC") || istringEqual(type,"SZAC") )
  {
    // Fan
    boost::optional<model::HVACComponent> fan;
    if( auto mo = translateFan(fanElement, model) ) {
      fan = mo->optionalCast<model::HVACComponent>();
    }
    if( ! fan ) {
      LOG(Error,name << " references an undefined fan.");
    }

    // Heating Coil
    pugi::xml_node heatingCoilElement = element.child("CoilHtg");
    boost::optional<model::HVACComponent> heatingCoil;
    if( auto mo = translateCoilHeating(heatingCoilElement, model) ) {
      heatingCoil = mo->optionalCast<model::HVACComponent>();
    }
    if( ! heatingCoil ) {
      LOG(Error,name << " references an undefined heating coil.");
    }


    // Cooling Coil
    pugi::xml_node coolingCoilElement = element.child("CoilClg");
    boost::optional<model::HVACComponent> coolingCoil;
    if( auto mo = translateCoilCooling(coolingCoilElement, model) ) {
      coolingCoil = mo->optionalCast<model::HVACComponent>();
    }
    if( ! coolingCoil ) {
      LOG(Error,name << " references an undefined cooling coil.");
    }

    if( ! schedule ) {
      LOG(Error,name << " references an undefined schedule.");
    }

    // PTAC
    if( schedule && fan && heatingCoil && coolingCoil ) {
      model::ZoneHVACPackagedTerminalAirConditioner ptac(model,schedule.get(),fan.get(),heatingCoil.get(),coolingCoil.get());

      if( flowCap )
      {
        ptac.setSupplyAirFlowRateDuringCoolingOperation(flowCap.get());
        ptac.setSupplyAirFlowRateDuringHeatingOperation(flowCap.get());
        ptac.setSupplyAirFlowRateWhenNoCoolingorHeatingisNeeded(flowCap.get());
      }

      // Name
      ptac.setName(name);

      // FanCtrl
      pugi::xml_node fanCtrlElement = element.child("FanCtrl");
      if( istringEqual(fanCtrlElement.text().as_string(),"Continuous") )
      {
        model::Schedule schedule = model.alwaysOnDiscreteSchedule();
        ptac.setSupplyAirFanOperatingModeSchedule(schedule);
      }

      if (fanOperModeSch) {
        ptac.setSupplyAirFanOperatingModeSchedule(fanOperModeSch.get());
      }
      if (clgSupFanCapSim) {
        ptac.setSupplyAirFlowRateDuringCoolingOperation(clgSupFanCapSim.get());
      }
      if (htgSupFanCapSim) {
        ptac.setSupplyAirFlowRateDuringHeatingOperation(htgSupFanCapSim.get());
      }
      if (noClgHtgSupFanCapSim) {
        ptac.setSupplyAirFlowRateWhenNoCoolingorHeatingisNeeded(noClgHtgSupFanCapSim.get());
      }
      if (clgOAFlowCapSim) {
        ptac.setOutdoorAirFlowRateDuringCoolingOperation(clgOAFlowCapSim.get());
      }
      if (htgOAFlowCapSim) {
        ptac.setOutdoorAirFlowRateDuringHeatingOperation(htgOAFlowCapSim.get());
      }
      if (noClgHtgOAFlowCapSim) {
        ptac.setOutdoorAirFlowRateWhenNoCoolingorHeatingisNeeded(noClgHtgOAFlowCapSim.get());
      }

      result = ptac;
    }
  }
  else if( istringEqual(type,"WSHP") )
  {
    pugi::xml_node htPumpSuppTempElement;
    pugi::xml_node htPumpCprsrLockoutTempElement;

    // Fan
    pugi::xml_node fanElement = element.child("Fan");
    boost::optional<model::HVACComponent> fan;
    if( auto mo = translateFan(fanElement, model) ) {
      fan = mo->optionalCast<model::HVACComponent>();
    }
    if( ! fan ) {
      LOG(Error,name << " references an undefined fan.");
    }

    // Heating Coil
    boost::optional<model::CoilHeatingWaterToAirHeatPumpEquationFit> heatingCoil;
    boost::optional<model::HVACComponent> suppHeatingCoil;
    std::vector<pugi::xml_node> heatingCoilElements = makeVectorOfChildren(element, "CoilHtg");
    for (std::vector<pugi::xml_node>::size_type i = 0; i < heatingCoilElements.size(); i++)
    {
      pugi::xml_node heatingCoilElement = heatingCoilElements[i];
      pugi::xml_node typeElement = heatingCoilElement.child("Type");
      if( auto mo = translateCoilHeating(heatingCoilElement, model) ) {
        if(istringEqual(typeElement.text().as_string(),"HeatPump"))
        {
          if( auto hvacComp = mo->optionalCast<model::HVACComponent>() ) {
            if( (heatingCoil = hvacComp->optionalCast<model::CoilHeatingWaterToAirHeatPumpEquationFit>()) ) {
              htPumpSuppTempElement = heatingCoilElement.child("HtPumpSuppTemp");
              htPumpCprsrLockoutTempElement = heatingCoilElement.child("HtPumpCprsrLockoutTemp");
            }
          }
        } else {
          suppHeatingCoil = mo->optionalCast<model::HVACComponent>();
        }
      }

      if( i > 1 ) {
        LOG(Warn,name << " has unused heating coils defined.");
      }
    }

    if( ! heatingCoil ) {
      LOG(Error,name << " has an undefined heating coil.");
    }
    if( ! suppHeatingCoil ) {
      LOG(Error,name << " has an undefined supplementary heating coil.");
    }

    // Cooling Coil
    pugi::xml_node coolingCoilElement = element.child("CoilClg");
    boost::optional<model::HVACComponent> coolingCoil;
    if( auto mo = translateCoilCooling(coolingCoilElement, model) ) {
      if( auto hvacComp = mo->optionalCast<model::HVACComponent>() ) {
        coolingCoil = hvacComp->optionalCast<model::CoilCoolingWaterToAirHeatPumpEquationFit>();
      }
    }
    if( ! coolingCoil ) {
      LOG(Error,name << " has an undefined cooling coil.");
    }

    boost::optional<model::ZoneHVACWaterToAirHeatPump> wshp;

    // Default Supplemental Heating Coil if one is not provided
    if( ! suppHeatingCoil )
    {
      model::Schedule alwaysOnDiscreteSchedule = model.alwaysOnDiscreteSchedule();
      suppHeatingCoil = model::CoilHeatingElectric(model,alwaysOnDiscreteSchedule);
      LOG(Warn,name << " creating a default supplementary heating coil");
    }

    if( ! schedule ) {
      LOG(Error,name << " has an undefined schedule.");
    }

    if( schedule && fan && heatingCoil && coolingCoil && suppHeatingCoil ) {
      wshp = model::ZoneHVACWaterToAirHeatPump(model,
                                               schedule.get(),
                                               fan.get(),
                                               heatingCoil.get(),
                                               coolingCoil.get(),
                                               suppHeatingCoil.get());
      result = wshp;

      // Name
      wshp->setName(name);

      if( flowCap )
      {
        wshp->setSupplyAirFlowRateDuringCoolingOperation(flowCap.get());
        wshp->setSupplyAirFlowRateDuringHeatingOperation(flowCap.get());
        wshp->setSupplyAirFlowRateWhenNoCoolingorHeatingisNeeded(flowCap.get());
      }

      wshp->setMaximumCyclingRate(2.5);
      wshp->setHeatPumpTimeConstant(60.0);
      wshp->setFractionofOnCyclePowerUse(0.01);

      pugi::xml_node htPumpFanDelayElement = element.child("HtPumpFanDelay");
      boost::optional<double> _htPumpFanDelay = lexicalCastToDouble(htPumpFanDelayElement);
      if( _htPumpFanDelay )
      {
        wshp->setHeatPumpFanDelayTime(_htPumpFanDelay.get());
      }
      else
      {
        wshp->setHeatPumpFanDelayTime(60.0);
      }

      if( htgDsgnSupAirTemp )
      {
        wshp->setMaximumSupplyAirTemperaturefromSupplementalHeater(htgDsgnSupAirTemp.get());
      }

      boost::optional<double> _htPumpSuppTemp = lexicalCastToDouble(htPumpSuppTempElement);
      if( _htPumpSuppTemp )
      {
        double value = unitToUnit(_htPumpSuppTemp.get(),"F","C").get();
        wshp->setMaximumOutdoorDryBulbTemperatureforSupplementalHeaterOperation(value);
      }

      // FanCtrl
      pugi::xml_node fanCtrlElement = element.child("FanCtrl");
      if( istringEqual(fanCtrlElement.text().as_string(),"Continuous") )
      {
        model::Schedule schedule = model.alwaysOnDiscreteSchedule();
        wshp->setSupplyAirFanOperatingModeSchedule(schedule);
      }

      // OffCyclePwrFrac
      const auto offCyclePwrFrac = lexicalCastToDouble(element.child("OffCyclePwrFrac"));
      if (offCyclePwrFrac) {
        wshp->setFractionofOnCyclePowerUse(offCyclePwrFrac.get());
      }
    }

    if (fanOperModeSch) {
      wshp->setSupplyAirFanOperatingModeSchedule(fanOperModeSch.get());
    }
    if (clgSupFanCapSim) {
      wshp->setSupplyAirFlowRateDuringCoolingOperation(clgSupFanCapSim.get());
    }
    if (htgSupFanCapSim) {
      wshp->setSupplyAirFlowRateDuringHeatingOperation(htgSupFanCapSim.get());
    }
    if (noClgHtgSupFanCapSim) {
      wshp->setSupplyAirFlowRateWhenNoCoolingorHeatingisNeeded(noClgHtgSupFanCapSim.get());
    }
    if (clgOAFlowCapSim) {
      wshp->setOutdoorAirFlowRateDuringCoolingOperation(clgOAFlowCapSim.get());
    }
    if (htgOAFlowCapSim) {
      wshp->setOutdoorAirFlowRateDuringHeatingOperation(htgOAFlowCapSim.get());
    }
    if (noClgHtgOAFlowCapSim) {
      wshp->setOutdoorAirFlowRateWhenNoCoolingorHeatingisNeeded(noClgHtgOAFlowCapSim.get());
    }
  }
  else if( istringEqual(type,"PTHP") || istringEqual(type,"SZHP") )
  {
    pugi::xml_node htPumpSuppTempElement;
    pugi::xml_node htPumpCprsrLockoutTempElement;

    // Fan
    pugi::xml_node fanElement = element.child("Fan");
    boost::optional<model::HVACComponent> fan;
    if( auto mo = translateFan(fanElement, model) ) {
      fan = mo->optionalCast<model::HVACComponent>();
    }
    if( ! fan ) {
      LOG(Error,name << " has an undefined fan.");
    }

    // Heating Coil
    boost::optional<model::HVACComponent> heatingCoil;
    boost::optional<model::HVACComponent> suppHeatingCoil;
    std::vector<pugi::xml_node> heatingCoilElements = makeVectorOfChildren(element, "CoilHtg");
    for (std::vector<pugi::xml_node>::size_type i = 0; i < heatingCoilElements.size(); i++)
    {
      pugi::xml_node heatingCoilElement = heatingCoilElements[i];
      pugi::xml_node typeElement = heatingCoilElement.child("Type");
      if( auto mo = translateCoilHeating(heatingCoilElement, model) ) {
        if(istringEqual(typeElement.text().as_string(),"HeatPump"))
        {
          if( (heatingCoil = mo->optionalCast<model::HVACComponent>()) ) {
            htPumpSuppTempElement = heatingCoilElement.child("HtPumpSuppTemp");
            htPumpCprsrLockoutTempElement = heatingCoilElement.child("HtPumpCprsrLockoutTemp");
          }
        } else {
          suppHeatingCoil = mo->optionalCast<model::HVACComponent>();
        }
      }

      if( i > 1 ) {
        LOG(Warn,name << " has unused heating coils defined.");
      }
    }

    if( ! heatingCoil ) {
      LOG(Error,name << " has an undefined heating coil.");
    }
    if( ! suppHeatingCoil ) {
      LOG(Error,name << " has an undefined supplementary heating coil.");
    }

    // Cooling Coil
    pugi::xml_node coolingCoilElement = element.child("CoilClg");
    boost::optional<model::HVACComponent> coolingCoil;
    if( auto mo = translateCoilCooling(coolingCoilElement, model) ) {
      coolingCoil = mo->optionalCast<model::HVACComponent>();
    }
    if( ! coolingCoil ) {
      LOG(Error,name << " has an undefined cooling coil.");
    }

    // Default Supplemental Heating Coil if one is not provided
    if( ! suppHeatingCoil )
    {
      model::Schedule alwaysOnDiscreteSchedule = model.alwaysOnDiscreteSchedule();
      suppHeatingCoil = model::CoilHeatingElectric(model,alwaysOnDiscreteSchedule);
      auto offSchedule = model.alwaysOffDiscreteSchedule();
      suppHeatingCoil->cast<model::CoilHeatingElectric>().setAvailabilitySchedule(offSchedule);
      suppHeatingCoil->cast<model::CoilHeatingElectric>().setNominalCapacity(0.0);
      LOG(Warn,name << " creating a default supplementary heating coil, with no capacity");
    }

    if( ! heatingCoil )
    {
      model::Schedule alwaysOnDiscreteSchedule = model.alwaysOnDiscreteSchedule();

      model::CurveCubic totalHeatingCapacityFunctionofTemperatureCurve(model);
      totalHeatingCapacityFunctionofTemperatureCurve.setCoefficient1Constant(0.758746);
      totalHeatingCapacityFunctionofTemperatureCurve.setCoefficient2x(0.027626);
      totalHeatingCapacityFunctionofTemperatureCurve.setCoefficient3xPOW2(0.000148716);
      totalHeatingCapacityFunctionofTemperatureCurve.setCoefficient4xPOW3(0.0000034992);
      totalHeatingCapacityFunctionofTemperatureCurve.setMinimumValueofx(-20.0);
      totalHeatingCapacityFunctionofTemperatureCurve.setMaximumValueofx(20.0);

      model::CurveCubic totalHeatingCapacityFunctionofFlowFractionCurve(model);
      totalHeatingCapacityFunctionofFlowFractionCurve.setCoefficient1Constant(0.84);
      totalHeatingCapacityFunctionofFlowFractionCurve.setCoefficient2x(0.16);
      totalHeatingCapacityFunctionofFlowFractionCurve.setCoefficient3xPOW2(0.0);
      totalHeatingCapacityFunctionofFlowFractionCurve.setCoefficient4xPOW3(0.0);
      totalHeatingCapacityFunctionofFlowFractionCurve.setMinimumValueofx(0.5);
      totalHeatingCapacityFunctionofFlowFractionCurve.setMaximumValueofx(1.5);

      model::CurveCubic energyInputRatioFunctionofTemperatureCurve(model);
      energyInputRatioFunctionofTemperatureCurve.setCoefficient1Constant(1.19248);
      energyInputRatioFunctionofTemperatureCurve.setCoefficient2x(-0.0300438);
      energyInputRatioFunctionofTemperatureCurve.setCoefficient3xPOW2(0.00103745);
      energyInputRatioFunctionofTemperatureCurve.setCoefficient4xPOW3(-0.000023328);
      energyInputRatioFunctionofTemperatureCurve.setMinimumValueofx(-20.0);
      energyInputRatioFunctionofTemperatureCurve.setMaximumValueofx(20.0);

      model::CurveQuadratic energyInputRatioFunctionofFlowFractionCurve(model);
      energyInputRatioFunctionofFlowFractionCurve.setCoefficient1Constant(1.3824);
      energyInputRatioFunctionofFlowFractionCurve.setCoefficient2x(-0.4336);
      energyInputRatioFunctionofFlowFractionCurve.setCoefficient3xPOW2(0.0512);
      energyInputRatioFunctionofFlowFractionCurve.setMinimumValueofx(0.0);
      energyInputRatioFunctionofFlowFractionCurve.setMaximumValueofx(1.0);

      model::CurveQuadratic partLoadFractionCorrelationCurve(model);
      partLoadFractionCorrelationCurve.setCoefficient1Constant(0.75);
      partLoadFractionCorrelationCurve.setCoefficient2x(0.25);
      partLoadFractionCorrelationCurve.setCoefficient3xPOW2(0.0);
      partLoadFractionCorrelationCurve.setMinimumValueofx(0.0);
      partLoadFractionCorrelationCurve.setMaximumValueofx(1.0);

      suppHeatingCoil = model::CoilHeatingDXSingleSpeed( model,
                               alwaysOnDiscreteSchedule,
                               totalHeatingCapacityFunctionofTemperatureCurve,
                               totalHeatingCapacityFunctionofFlowFractionCurve,
                               energyInputRatioFunctionofTemperatureCurve,
                               energyInputRatioFunctionofFlowFractionCurve,
                               partLoadFractionCorrelationCurve );

      LOG(Warn,name << " creating a default heating coil");
    }

    boost::optional<model::ZoneHVACPackagedTerminalHeatPump> pthp;
    boost::optional<model::ZoneHVACWaterToAirHeatPump> wshp;

    if( schedule && fan && heatingCoil && coolingCoil && suppHeatingCoil ) {
      pthp = model::ZoneHVACPackagedTerminalHeatPump(model,
                                                     schedule.get(),
                                                     fan.get(),
                                                     heatingCoil.get(),
                                                     coolingCoil.get(),
                                                     suppHeatingCoil.get());
      result = pthp;

      if( flowCap )
      {
        pthp->setSupplyAirFlowRateDuringCoolingOperation(flowCap.get());
        pthp->setSupplyAirFlowRateDuringHeatingOperation(flowCap.get());
        pthp->setSupplyAirFlowRateWhenNoCoolingorHeatingisNeeded(flowCap.get());
      }

      // Name
      pthp->setName(name);

      // HtPumpSuppTemp
      boost::optional<double> _htPumpSuppTemp = lexicalCastToDouble(htPumpSuppTempElement);
      if( _htPumpSuppTemp )
      {
        double value = unitToUnit(_htPumpSuppTemp.get(),"F","C").get();
        pthp->setMaximumOutdoorDryBulbTemperatureforSupplementalHeaterOperation(value);
      }

      // HtPumpCprsrLockoutTemp
      boost::optional<double> _htPumpCprsrLockoutTemp = lexicalCastToDouble(htPumpCprsrLockoutTempElement);
      if( _htPumpCprsrLockoutTemp )
      {
        double value = unitToUnit(_htPumpCprsrLockoutTemp.get(),"F","C").get();
        pthp->setMinimumOutdoorDryBulbTemperatureforCompressorOperation(value);
      }

      // FanCtrl
      pugi::xml_node fanCtrlElement = element.child("FanCtrl");
      if( istringEqual(fanCtrlElement.text().as_string(),"Continuous") )
      {
        model::Schedule schedule = model.alwaysOnDiscreteSchedule();
        pthp->setSupplyAirFanOperatingModeSchedule(schedule);
      }

      // htgDsgnSupAirTemp
      if( htgDsgnSupAirTemp )
      {
        pthp->setMaximumSupplyAirTemperaturefromSupplementalHeater(htgDsgnSupAirTemp.get());
      }

      if (fanOperModeSch) {
        pthp->setSupplyAirFanOperatingModeSchedule(fanOperModeSch.get());
      }
      if (clgSupFanCapSim) {
        pthp->setSupplyAirFlowRateDuringCoolingOperation(clgSupFanCapSim.get());
      }
      if (htgSupFanCapSim) {
        pthp->setSupplyAirFlowRateDuringHeatingOperation(htgSupFanCapSim.get());
      }
      if (noClgHtgSupFanCapSim) {
        pthp->setSupplyAirFlowRateWhenNoCoolingorHeatingisNeeded(noClgHtgSupFanCapSim.get());
      }
      if (clgOAFlowCapSim) {
        pthp->setOutdoorAirFlowRateDuringCoolingOperation(clgOAFlowCapSim.get());
      }
      if (htgOAFlowCapSim) {
        pthp->setOutdoorAirFlowRateDuringHeatingOperation(htgOAFlowCapSim.get());
      }
      if (noClgHtgOAFlowCapSim) {
        pthp->setOutdoorAirFlowRateWhenNoCoolingorHeatingisNeeded(noClgHtgOAFlowCapSim.get());
      }
    }
  }
  else if( istringEqual(type,"Baseboard") )
  {
    // Heating Coil
    pugi::xml_node heatingCoilElement = element.child("CoilHtg");

    // Heating Coil Type
    std::string htgCoilType = heatingCoilElement.child("Type").text().as_string();

    if ( istringEqual(htgCoilType,"Resistance") )
    {
      // ZoneHVAC Baseboard Convective Electric
      model::ZoneHVACBaseboardConvectiveElectric baseboard(model);
      result = baseboard;

      // Nominal Capacity
      boost::optional<double> nomHtgCap;

      if( ! autosize() )
      {
        boost::optional<double> _capTotGrossRtdSim = lexicalCastToDouble(heatingCoilElement.child("CapTotGrossRtdSim"));

        if( _capTotGrossRtdSim )
        {
          nomHtgCap = unitToUnit(_capTotGrossRtdSim.get(), "Btu/hr", "W");
        }
      }

      //set the nominal capacity of the baseboard unit
      if ( nomHtgCap )
      {
        baseboard.setNominalCapacity(nomHtgCap.get());
      }

      // Name
      baseboard.setName(name);
    }
    else if (istringEqual(htgCoilType, "HotWater") )
    {
      // Coil Heating Water Baseboard (special coil type for baseboard only)
      model::CoilHeatingWaterBaseboard coil(model);

      // Autosize the max flow rate and the u-factor times area value
      coil.autosizeMaximumWaterFlowRate();
      coil.autosizeUFactorTimesAreaValue();

      // Maximum Water Flow Rate (units are gpm in the sdd file)
      boost::optional<double> dsnHtgFlowRt;

      if( ! autosize() )
      {
        boost::optional<double> _fluidFlowRtDsgnSim = lexicalCastToDouble(heatingCoilElement.child("FluidFlowRtDsgnSim"));

        if( _fluidFlowRtDsgnSim )
        {
          dsnHtgFlowRt = unitToUnit(_fluidFlowRtDsgnSim.get(), "gal/min", "m^3/s");
        }
      }

      if( dsnHtgFlowRt )
      {
        coil.setMaximumWaterFlowRate(*dsnHtgFlowRt);
      }

      //Hook Coil to PlantLoop
      pugi::xml_node fluidSegInRefElement = heatingCoilElement.child("FluidSegInRef");
      if (!fluidSegInRefElement) {
        LOG(Error, "CoilHeatingWaterBaseboard for ZnSys '" << name << "' doesn't have a FluidSegInRef element! It will not be connected.");
      } else {
        if( boost::optional<model::PlantLoop> plant = loopForSupplySegment(fluidSegInRefElement, model) )
        {
          plant->addDemandBranchForComponent(coil);
          // Figure out if this is connected to a ServiceHotWater system
          // If it is then make sure any supply segment pumps go on the branch,
          // as opposed to the demand side inlet branch
          auto supplySegmentElement = supplySegment(fluidSegInRefElement);
          auto fluidSysTypeElment = supplySegmentElement.parent().child("Type");
          if (openstudio::istringEqual(fluidSysTypeElment.text().as_string(), "ServiceHotWater")) {
            auto pumpElement = supplySegmentElement.child("Pump");
            if (pumpElement) {
              if( auto modelObject = translatePump(pumpElement, model) ) {
                auto hvacComponent = modelObject->cast<model::HVACComponent>();
                auto inletNode = coil.inletModelObject()->cast<model::Node>();
                hvacComponent.addToNode(inletNode);
              }
            }
          }
        }
      }

      // ZoneHVAC Baseboard Convective Electric
      model::ZoneHVACBaseboardConvectiveWater baseboard(model, schedule.get(), coil);
      result = baseboard;

      // Name
      baseboard.setName(name);
    }
  }
  else if( istringEqual(type,"FPFC") )
  {
    boost::optional<openstudio::model::ModelObject> mo;

    // Fan
    pugi::xml_node fanElement = element.child("Fan");
    boost::optional<model::HVACComponent> fan;
    if( auto mo = translateFan(fanElement, model) ) {
      fan = mo->optionalCast<model::HVACComponent>();
    }
    if( ! fan ) {
      LOG(Error,name << " has an undefined fan.");
    }

    // Heating Coil
    pugi::xml_node heatingCoilElement = element.child("CoilHtg");
    boost::optional<model::HVACComponent> heatingCoil;
    if( (mo = translateCoilHeating(heatingCoilElement, model)) ) {
      heatingCoil = mo->optionalCast<model::HVACComponent>();
    }
    if( ! heatingCoil ) {
      LOG(Error,name << " has an undefined heating coil.");
    }

    // get the heating coil's max flow rate (units are gpm in the sdd file)
    pugi::xml_node dsnHtgFlowRtElement = heatingCoilElement.child("FluidFlowRtDsgnSim");

    boost::optional<double> dsnHtgFlowRt;

    if( ! autosize() ) {
      boost::optional<double> _dsnHtgFlowRt = lexicalCastToDouble(dsnHtgFlowRtElement);
      if( _dsnHtgFlowRt ) {
        dsnHtgFlowRt = unitToUnit(_dsnHtgFlowRt.get(), "gal/min", "m^3/s");
      }
    }

    // Cooling Coil
    pugi::xml_node coolingCoilElement = element.child("CoilClg");
    boost::optional<model::HVACComponent> coolingCoil;
    if( auto mo = translateCoilCooling(coolingCoilElement, model) ) {
      coolingCoil = mo->optionalCast<model::HVACComponent>();
    }
    if( ! coolingCoil ) {
      LOG(Error,name << " has an undefined cooling coil.");
    }

    // get the cooling coil's max flow rate (units are gpm in the sdd file)
    pugi::xml_node dsnClgFlowRtElement = coolingCoilElement.child("FluidFlowRtDsgnSim");
    boost::optional<double> dsnClgFlowRt;
    if( ! autosize() ) {
      boost::optional<double> _dsnClgFlowRt = lexicalCastToDouble(dsnClgFlowRtElement);
      if( _dsnClgFlowRt )  {
        dsnClgFlowRt = unitToUnit(_dsnClgFlowRt.get(), "gal/min", "m^3/s");
      }
    }

    if( ! schedule ) {
      LOG(Error,name << " has an undefined schedule.");
    }

    // FPFC (Four Pipe Fan Coil)
    if( schedule && fan && coolingCoil && heatingCoil ) {
      model::ZoneHVACFourPipeFanCoil fpfc(model,schedule.get(),fan.get(),coolingCoil.get(),heatingCoil.get());
      result = fpfc;


      if( fanOperModeSch ) {
        fpfc.setSupplyAirFanOperatingModeSchedule(fanOperModeSch.get());
      }

      // pull the max air flow rate from the fan and set in the fpfc
      if (clgSupFanCapSim && htgSupFanCapSim) {
        fpfc.setMaximumSupplyAirFlowRate(std::max(clgSupFanCapSim.get(),htgSupFanCapSim.get()));
      } else if( flowCap ) {
        fpfc.setMaximumSupplyAirFlowRate(flowCap.get());
      }

      //set the max heating water flow rate
      if( dsnHtgFlowRt )
      {
        fpfc.setMaximumHotWaterFlowRate(*dsnHtgFlowRt);
      }

      //set the max cooling water flow rate
      if( dsnClgFlowRt )
      {
        fpfc.setMaximumColdWaterFlowRate(dsnClgFlowRt.get());
      }

      // FanElementControlMethod

      pugi::xml_node fanElementControlMethodElement = fanElement.child("CtrlMthdSim");

      // FanElementMinFlow
      // Already handled above
      //pugi::xml_node fanElementFlowMinElement = fanElement.child("FlowMinSim");

      // FanElementFlowCap
      // Already handled abovd
      // pugi::xml_node fanElementFlowCapElement = fanElement.child("FlowCapSim");

      boost::optional<double> flowMinRatio;

      if( flowMin && flowCap )
      {
        flowMinRatio = flowMin.get() / flowCap.get();
      }

      if (noClgSupFanCapSim && noHtgSupFanCapSim && fpfc.maximumSupplyAirFlowRate()) {
        double value = std::max(noClgSupFanCapSim.get(), noHtgSupFanCapSim.get()) / fpfc.maximumSupplyAirFlowRate().get();
        fpfc.setLowSpeedSupplyAirFlowRatio(value);
        fpfc.setMediumSpeedSupplyAirFlowRatio(value);
      } else {
        fpfc.setLowSpeedSupplyAirFlowRatio(1.0);
        fpfc.setMediumSpeedSupplyAirFlowRatio(1.0);
      }

      if (clgOAFlowCapSim && htgOAFlowCapSim) {
        fpfc.setMaximumOutdoorAirFlowRate(std::max(clgOAFlowCapSim.get(),htgOAFlowCapSim.get()));
      }

      pugi::xml_node fanCtrlElement = element.child("FanCtrl");

      if (openstudio::istringEqual(fanCtrlElement.text().as_string(), "Continuous"))
      {
        if (openstudio::istringEqual(fanElementControlMethodElement.text().as_string(), "ConstantVolume"))
        {
          fpfc.setCapacityControlMethod("ConstantFanVariableFlow");
        }
        else if (openstudio::istringEqual(fanElementControlMethodElement.text().as_string(), "VariableSpeedDrive"))
        {
          fpfc.setCapacityControlMethod("VariableFanVariableFlow");

          if( flowMinRatio )
          {
            fpfc.setLowSpeedSupplyAirFlowRatio(flowMinRatio.get());

            fpfc.setMediumSpeedSupplyAirFlowRatio(flowMinRatio.get());
          }
        }
      }
      else if (openstudio::istringEqual(fanCtrlElement.text().as_string(), "Cycling"))
      {
        if (openstudio::istringEqual(fanElementControlMethodElement.text().as_string(), "TwoSpeed"))
        {
          fpfc.setCapacityControlMethod("CyclingFan");

          if( flowMinRatio )
          {
            fpfc.setLowSpeedSupplyAirFlowRatio(flowMinRatio.get());

            fpfc.setMediumSpeedSupplyAirFlowRatio(flowMinRatio.get());
          }
        }
        else if (openstudio::istringEqual(fanElementControlMethodElement.text().as_string(), "ConstantVolume"))
        {
          fpfc.setCapacityControlMethod("CyclingFan");
        }
      }

      // Explicit control for capacity control method was added after the above logic
      auto capCtrlMthd = element.child("CapCtrlMthd").text().as_string();
      if (! std::string(capCtrlMthd).empty()) {
        fpfc.setCapacityControlMethod(capCtrlMthd);
      }

      // Name
      fpfc.setName(name);
    }
  }
  else if( istringEqual(type,"VRF") )
  {
    boost::optional<model::HVACComponent> fan;
    if( auto mo = translateFan(fanElement, model) ) {
      fan = mo->optionalCast<model::HVACComponent>();
    }
    if( ! fan ) {
      LOG(Error,name << " references an undefined fan.");
    }

    boost::optional<model::CoilCoolingDXVariableRefrigerantFlow> coolingCoil;
    {
      auto coolingCoilElement = element.child("CoilClg");
      if (coolingCoilElement) {
        if( auto mo = translateCoilCoolingDXVariableRefrigerantFlow(coolingCoilElement, model) ) {
          coolingCoil = mo->optionalCast<model::CoilCoolingDXVariableRefrigerantFlow>();
        }
      }
    }
    if( ! coolingCoil ) {
      LOG(Error,name << " references an undefined cooling coil");
    }

    boost::optional<model::CoilHeatingDXVariableRefrigerantFlow> heatingCoil;
    {
      auto heatingCoilElement = element.child("CoilHtg");
      if (heatingCoilElement) {
        if( auto mo = translateCoilHeatingDXVariableRefrigerantFlow(heatingCoilElement, model) ) {
          heatingCoil = mo->optionalCast<model::CoilHeatingDXVariableRefrigerantFlow>();
        }
      }
    }
    if( ! heatingCoil ) {
      LOG(Error,name << " references an undefined heating coil");
    }

    if( fan && coolingCoil && heatingCoil ) {
      model::ZoneHVACTerminalUnitVariableRefrigerantFlow vrfTerminal(model,coolingCoil.get(),heatingCoil.get(),fan.get());
      vrfTerminal.setName(name);
      if( schedule ) {
        vrfTerminal.setTerminalUnitAvailabilityschedule(schedule.get());
      }

      result = vrfTerminal;

      {
        auto value = element.child("VRFSysRef").text().as_string();
        auto vrfSys = model.getModelObjectByName<model::AirConditionerVariableRefrigerantFlow>(value);
        if( vrfSys ) {
          vrfSys->addTerminal(vrfTerminal);
        } else {
          LOG(Error,name << " references an undefined vrf system");
        }
      }

      //FanPos
      {
        // fan placement is not implemented in OS
        // TODO: Fix this!
        // auto fanPosElement = element.child("FanPos");
      }

      // FanCtrl
      {
        auto fanCtrlElement = element.child("FanCtrl");
        if( istringEqual(fanCtrlElement.text().as_string(),"Continuous") )
        {
          auto schedule = model.alwaysOnDiscreteSchedule();
          vrfTerminal.setSupplyAirFanOperatingModeSchedule(schedule);
        } else {
          auto schedule = model.alwaysOffDiscreteSchedule();
          vrfTerminal.setSupplyAirFanOperatingModeSchedule(schedule);
        }
      }

      if( fanOperModeSch ) {
        vrfTerminal.setSupplyAirFanOperatingModeSchedule(fanOperModeSch.get());
      }

      // ClgSupFanCapSim
      if( ! autosize() ) {
        boost::optional<double> _clgSupFanCapSim = lexicalCastToDouble(element.child("ClgSupFanCapSim"));
        if( _clgSupFanCapSim ) {
          double value = unitToUnit(_clgSupFanCapSim.get(),"cfm","m^3/s").get();
          vrfTerminal.setSupplyAirFlowRateDuringCoolingOperation(value);
        }
      }

      // HtgSupFanCapSim
      if( ! autosize() ) {
        boost::optional<double> _htgSupFanCapSim = lexicalCastToDouble(element.child("HtgSupFanCapSim"));
        if( _htgSupFanCapSim ) {
          double value = unitToUnit(_htgSupFanCapSim.get(),"cfm","m^3/s").get();
          vrfTerminal.setSupplyAirFlowRateDuringHeatingOperation(value);
        }
      }

      // NoClgSupFanCapSim
      if( ! autosize() ) {
        boost::optional<double> _noClgSupFanCapSim = lexicalCastToDouble(element.child("NoClgSupFanCapSim"));
        if( _noClgSupFanCapSim ) {
          double value = unitToUnit(_noClgSupFanCapSim.get(),"cfm","m^3/s").get();
          vrfTerminal.setSupplyAirFlowRateWhenNoCoolingisNeeded(value);
        }
      }

      // NoHtgSupFanCapSim
      if( ! autosize() ) {
        boost::optional<double> _noHtgSupFanCapSim = lexicalCastToDouble(element.child("NoHtgSupFanCapSim"));
        if( _noHtgSupFanCapSim ) {
          double value = unitToUnit(_noHtgSupFanCapSim.get(),"cfm","m^3/s").get();
          vrfTerminal.setSupplyAirFlowRateWhenNoHeatingisNeeded(value);
        }
      }

      // ClgOAFlowCapSim
      if( ! autosize() ) {
        boost::optional<double> _clgOAFlowCapSim = lexicalCastToDouble(element.child("ClgOAFlowCapSim"));
        if( _clgOAFlowCapSim ) {
          double value = unitToUnit(_clgOAFlowCapSim.get(),"cfm","m^3/s").get();
          vrfTerminal.setOutdoorAirFlowRateDuringCoolingOperation(value);
        }
      }

      // HtgOAFlowCapSim
      if( ! autosize() ) {
        boost::optional<double> _htgOAFlowCapSim = lexicalCastToDouble(element.child("HtgOAFlowCapSim"));
        if( _htgOAFlowCapSim ) {
          double value = unitToUnit(_htgOAFlowCapSim.get(),"cfm","m^3/s").get();
          vrfTerminal.setOutdoorAirFlowRateDuringHeatingOperation(value);
        }
      }

      // NoClgHtgOAFlowCapSim
      if( ! autosize() ) {
        boost::optional<double> _noClgHtgOAFlowCapSim = lexicalCastToDouble(element.child("NoClgHtgOAFlowCapSim"));
        if( _noClgHtgOAFlowCapSim ) {
          double value = unitToUnit(_noClgHtgOAFlowCapSim.get(),"cfm","m^3/s").get();
          vrfTerminal.setOutdoorAirFlowRateWhenNoCoolingorHeatingisNeeded(value);
        }
      }

      //<ClgDsgnSupAirTemp>55</ClgDsgnSupAirTemp>
      //<HtgDsgnSupAirTemp>95</HtgDsgnSupAirTemp>

      // AuxPwrOnSim
      if( ! autosize() ) {
        boost::optional<double> _auxPwrOnSim = lexicalCastToDouble(element.child("AuxPwrOnSim"));
        if( _auxPwrOnSim ) {
          double value = unitToUnit(_auxPwrOnSim.get(),"Btu/h","W").get();
          vrfTerminal.setZoneTerminalUnitOnParasiticElectricEnergyUse(value);
        }
      }

      // AuxPwrOffSim
      if( ! autosize() ) {
        boost::optional<double> _auxPwrOffSim = lexicalCastToDouble(element.child("AuxPwrOffSim"));
        if( _auxPwrOffSim ) {
          double value = unitToUnit(_auxPwrOffSim.get(),"Btu/h","W").get();
          vrfTerminal.setZoneTerminalUnitOffParasiticElectricEnergyUse(value);
        }
      }

      // See ZoneSizing
      // ClgDsgnSupAirTemp
      // HtgDsgnSupAirTemp

    } else {
      LOG(Error,name << " could not be created.");
    }
  } else if (istringEqual(type, "Radiant")) {
    result = translateRadiantZnSys(element, thermalZone);
  }

  return result;
}

boost::optional<openstudio::model::ModelObject> ReverseTranslator::translateRadiantZnSys(const pugi::xml_node& element,
                                                                                         openstudio::model::ThermalZone& thermalZone) {
  if (!istringEqual(element.name(), "ZnSys")) {
    return boost::none;
  }

  auto typeElement = element.child("TypeSim");
  if (!istringEqual(typeElement.text().as_string(), "Radiant")) {
    return boost::none;
  }

  auto model = thermalZone.model();
  auto name = element.child("Name").text().as_string();

  enum CoilType
  {
    Undefined,
    HotWater,
    ChilledWater,
    Resistance
  };
  CoilType coilType = CoilType::Undefined;

  boost::optional<model::ModelObject> result;

  auto availSchRefElement = element.child("AvailSchRef");
  auto availSch = translateAvailSchRef(availSchRefElement, model)->cast<model::Schedule>();

  auto heatingCoilElement = element.child("CoilHtg");
  auto coolingCoilElement = element.child("CoilClg");

  auto surfaceInfo = m_radiantSurfaces[name];

  // RadSysSurfList
  // Make the radiant surface use a construction with internal source
  // return the surface type of the last surface
  auto translateRadiantSurfaces = [&]() {
    std::string result = "AllSurfaces";
    // This function finds the OpenStudio Surface objects given the surface names,
    // and updates the surface constructions to be constructions with internal source,
    for (const auto& info : surfaceInfo) {
      // Look for surfaces related to this zone, by "info.name" and the reverse surface name
      auto surface = model.getModelObjectByName<model::Surface>(info.name);
      auto rsurface = model.getModelObjectByName<model::Surface>(info.name + " Reversed");

      std::vector<model::Surface> allSurfaces;
      for (const auto& space : thermalZone.spaces()) {
        auto surfaces = space.surfaces();
        allSurfaces.insert(allSurfaces.end(), surfaces.begin(), surfaces.end());
      }

      // surfaceit will be an iterator to the surface or reverse surface that needs to be 
      // attached to this system
      std::vector<model::Surface>::iterator surfaceit = allSurfaces.end();
      if (surface) {
        surfaceit = std::find(allSurfaces.begin(), allSurfaces.end(), surface.get());
      }

      if (surfaceit == allSurfaces.end()) {
        if (rsurface) {
          surfaceit = std::find(allSurfaces.begin(), allSurfaces.end(), rsurface.get());
        }
      }

      if (surfaceit != allSurfaces.end()) {
        result = surfaceit->surfaceType();
        auto construction = surfaceit->construction();
        OS_ASSERT(construction);
        auto layeredConstruction = construction->optionalCast<model::LayeredConstruction>();
        // If not LayeredConstruction then it might be a CFactorUndergroundWallConstruction which we do not support
        if (layeredConstruction) {
          const auto layers = layeredConstruction->layers();
          const auto opaqueMaterials = subsetCastVector<model::OpaqueMaterial>(layers);
          model::ConstructionWithInternalSource internalSourceConstruction(opaqueMaterials);
          const auto radiantConstructionName = surfaceit->nameString() + " Radiant Construction";
          internalSourceConstruction.setName(radiantConstructionName);
          internalSourceConstruction.setSourcePresentAfterLayerNumber(info.srcAftConsAssmLrNum);
          internalSourceConstruction.setTemperatureCalculationRequestedAfterLayerNumber(info.tempCalcAftConsAssmLrNum);
          internalSourceConstruction.setDimensionsForTheCTFCalculation(info.cTFCalcDim);
          internalSourceConstruction.setTubeSpacing(info.tubeSpacing);

          // Update the surface construction to internalSourceConstruction
          if (surface) {
            surface->setConstruction(internalSourceConstruction);
          }
          if (rsurface) {
            rsurface->setConstruction(internalSourceConstruction);
          }
        }
      }
    }
    // convert to radiant system surface group types
    if (istringEqual("RoofCeiling", result)) {
      result = "Ceilings";
    } else if (istringEqual("Floor", result)) {
      result = "Floors";
    } else if (istringEqual("Wall", result)) {
      result = "Walls";
    }
    return result;
  };

  auto surfaceType = translateRadiantSurfaces();

  if (heatingCoilElement) {
    auto coilTypeElement = heatingCoilElement.child("Type");
    if (istringEqual(coilTypeElement.text().as_string(), "HotWater")) {
      coilType = CoilType::HotWater;
    } else if (istringEqual(coilTypeElement.text().as_string(), "Resistance")) {
      coilType = CoilType::Resistance;
    }
  } else if (coolingCoilElement) {
    auto coilTypeElement = coolingCoilElement.child("Type");
    if (istringEqual(coilTypeElement.text().as_string(), "ChilledWater")) {
      coilType = CoilType::ChilledWater;
    }
  }

  // TemperatureControlType
  auto radSysTempCtrlType = element.child("RadSysTempCtrlType").text().as_string();

  // SetpointControlType
  auto setpointControlType = element.child("RadSysSetptCtrlType").text().as_string();

  if ((coilType == CoilType::ChilledWater) || (coilType == CoilType::HotWater)) {
    model::ZoneHVACLowTempRadiantVarFlow zoneHVAC(model);
    zoneHVAC.setAvailabilitySchedule(availSch);

    // Name
    zoneHVAC.setName(name);

    // RadiantSurfaceType
    zoneHVAC.setRadiantSurfaceType(surfaceType);

    // FluidtoRadiantSurfaceHeatTransferModel
    zoneHVAC.setFluidtoRadiantSurfaceHeatTransferModel("ConvectionOnly");

    // hydronicTubingInsideDiameter
    auto radSysTubeDiaIn = lexicalCastToDouble(element.child("RadSysTubeDiaIn"));
    if (radSysTubeDiaIn) {
      // convert
      radSysTubeDiaIn = unitToUnit(radSysTubeDiaIn.get(), "in", "m").get();
    } else {
      radSysTubeDiaIn = 0.013;  // meters
    }
    zoneHVAC.setHydronicTubingInsideDiameter(radSysTubeDiaIn.get());

    // HydronicTubingLength
    auto radSysTubeLen = lexicalCastToDouble(element.child("RadSysTubeLen"));
    if (radSysTubeLen) {
      radSysTubeLen = unitToUnit(radSysTubeLen.get(), "ft", "m").get();
      zoneHVAC.setHydronicTubingLength(radSysTubeLen.get());
    }

    // TemperatureControlType
    auto radSysTempCtrlType = element.child("RadSysTempCtrlType").text().as_string();
    zoneHVAC.setTemperatureControlType(radSysTempCtrlType);

    // SetpointControlType
    auto setpointControlType = element.child("RadSysSetptCtrlType").text().as_string();
    zoneHVAC.setSetpointControlType(setpointControlType);

    // RadSysCircuitLen
    zoneHVAC.setNumberofCircuits("OnePerSurface");
    auto radSysCircuitLen = lexicalCastToDouble(element.child("RadSysCircuitLen"));
    if (radSysCircuitLen) {
      if (radSysCircuitLen.get() > 0.00001) {
        zoneHVAC.setNumberofCircuits("CalculateFromCircuitLength");
      }
      radSysCircuitLen = unitToUnit(radSysCircuitLen.get(), "ft", "m");
      zoneHVAC.setCircuitLength(radSysCircuitLen.get());
    }

    // RadSysChangeOverPeriod
    auto radSysChangeOverPeriod = lexicalCastToDouble(element.child("RadSysChangeOverPeriod"));
    if (radSysChangeOverPeriod) {
      model::ScheduleConstant radSysChangeOverPeriodSchedule(model);
      radSysChangeOverPeriodSchedule.setName(zoneHVAC.nameString() + " Changeover Period");
      radSysChangeOverPeriodSchedule.setValue(radSysChangeOverPeriod.get());
      zoneHVAC.setChangeoverDelayTimePeriodSchedule(radSysChangeOverPeriodSchedule);
    }

    if (heatingCoilElement) {
      model::ScheduleConstant heatingControlTemperatureSchedule(model);
      heatingControlTemperatureSchedule.setValue(10.0);

      model::CoilHeatingLowTempRadiantVarFlow heatingCoil(model, heatingControlTemperatureSchedule);
      heatingCoil.setName(name + std::string(" Heating Coil"));
      zoneHVAC.setHeatingCoil(heatingCoil);

      // HeatingDesignCapacityMethod
      heatingCoil.setHeatingDesignCapacityMethod("HeatingDesignCapacity");

      // HeatingDesignCapacity
      auto capTotGrossRtdSim = lexicalCastToDouble(heatingCoilElement.child("CapTotGrossRtdSim"));
      if (capTotGrossRtdSim) {
        capTotGrossRtdSim = unitToUnit(capTotGrossRtdSim.get(), "Btu/h","W").get();
        heatingCoil.setHeatingDesignCapacity(capTotGrossRtdSim.get());
      }

      // HeatingControlThrottlingRange set in translateThermalZone
      // defaulted here
      heatingCoil.setHeatingControlThrottlingRange(0.5);

      // MaximumHotWaterFlow
      auto maximumHotWaterFlow = lexicalCastToDouble(heatingCoilElement.child("FluidFlowRtDsgnSim"));
      if (maximumHotWaterFlow) {
        maximumHotWaterFlow = unitToUnit(maximumHotWaterFlow.get(), "gal/min", "m^3/s").get();
        heatingCoil.setMaximumHotWaterFlow(maximumHotWaterFlow.get());
      }

      // HeatingControlTemperatureScheduleName set in translateThermalZone

      // FluidSegInRef
      auto fluidSegInRefElement = heatingCoilElement.child("FluidSegInRef");
      if (auto plant = loopForSupplySegment(fluidSegInRefElement, model)) {
        plant->addDemandBranchForComponent(heatingCoil);
      }
    }

    if (coolingCoilElement) {
      model::ScheduleConstant coolingControlTemperatureSchedule(model);
      coolingControlTemperatureSchedule.setValue(15.0);

      model::CoilCoolingLowTempRadiantVarFlow coolingCoil(model, coolingControlTemperatureSchedule);
      coolingCoil.setName(name + std::string(" Cooling Coil"));
      zoneHVAC.setCoolingCoil(coolingCoil);

      // coolingDesignCapacityMethod
      coolingCoil.setCoolingDesignCapacityMethod("CoolingDesignCapacity");

      // coolingDesignCapacity
      auto capTotGrossRtdSim = lexicalCastToDouble(coolingCoilElement.child("CapTotGrossRtdSim"));
      if (capTotGrossRtdSim) {
        capTotGrossRtdSim = unitToUnit(capTotGrossRtdSim.get(), "Btu/h","W").get();
        coolingCoil.setCoolingDesignCapacity(capTotGrossRtdSim.get());
      }

      // CoolingControlThrottlingRange set in translateThermalZone
      // defaulted here
      coolingCoil.setCoolingControlThrottlingRange(0.5);

      // MaximumColdWaterFlow
      auto maximumColdWaterFlow = lexicalCastToDouble(coolingCoilElement.child("FluidFlowRtDsgnSim"));
      if (maximumColdWaterFlow) {
        maximumColdWaterFlow = unitToUnit(maximumColdWaterFlow.get(), "gal/min", "m^3/s").get();
        coolingCoil.setMaximumColdWaterFlow(maximumColdWaterFlow.get());
      }

      // CoolingControlTemperatureScheduleName set in translateThermalZone

      // FluidSegInRef
      auto fluidSegInRefElement = coolingCoilElement.child("FluidSegInRef");
      if (auto plant = loopForSupplySegment(fluidSegInRefElement, model)) {
        plant->addDemandBranchForComponent(coolingCoil);
      }

      // RadSysCondCtrlType
      auto radSysCondCtrlType = element.child("RadSysCondCtrlType").text().as_string();
      coolingCoil.setCondensationControlType(radSysCondCtrlType);

      // RadSysCondCtrlOffset
      auto radSysCondCtrlOffset = lexicalCastToDouble(element.child("RadSysCondCtrlOffset"));
      if (radSysCondCtrlOffset) {
        radSysCondCtrlOffset = radSysCondCtrlOffset.get() * 5.0 / 9.0;
        coolingCoil.setCondensationControlDewpointOffset(radSysCondCtrlOffset.get());
      }
    }

    result = zoneHVAC;
  } else if (coilType == CoilType::Resistance) {
    model::ScheduleConstant heatingControlTemperatureSchedule(model);
    heatingControlTemperatureSchedule.setValue(10.0);

    model::ZoneHVACLowTemperatureRadiantElectric zoneHVAC(model, availSch, heatingControlTemperatureSchedule);
    zoneHVAC.setName(name);

    // RadiantSurfaceType
    zoneHVAC.setRadiantSurfaceType(surfaceType);

    if (heatingCoilElement) {
      // HeatingDesignCapacity
      auto capTotGrossRtdSim = lexicalCastToDouble(heatingCoilElement.child("CapTotGrossRtdSim"));
      if (capTotGrossRtdSim) {
        capTotGrossRtdSim = unitToUnit(capTotGrossRtdSim.get(), "Btu/h","W").get();
        zoneHVAC.setMaximumElectricalPowertoPanel(capTotGrossRtdSim.get());
      }
    }

    // TemperatureControlType
    zoneHVAC.setTemperatureControlType(radSysTempCtrlType);

    // SetpointControlType
    zoneHVAC.setSetpointControlType(setpointControlType);

    result = zoneHVAC;
  }

  return result;
}

boost::optional<openstudio::model::ModelObject> ReverseTranslator::translateCoilHeatingDXVariableRefrigerantFlow(
    const pugi::xml_node& element,
    openstudio::model::Model& model)
{
  if( ! istringEqual(element.name(),"CoilHtg") ) {
    return boost::none;
  }

  model::CoilHeatingDXVariableRefrigerantFlow coil(model);

  {
    auto value = element.child("Name").text().as_string();
    coil.setName(value);
  }

  if( ! autosize() ) {
    boost::optional<double> _capTotGrossRtdSim = lexicalCastToDouble(element.child("CapTotGrossRtdSim"));
    if( _capTotGrossRtdSim ) {
      double value = unitToUnit(_capTotGrossRtdSim.get(),"Btu/h","W").get();
      coil.setRatedTotalHeatingCapacity(value);
    }
  }

  auto setCurve = [&](const std::string & elementName,
      const std::function<bool(model::CoilHeatingDXVariableRefrigerantFlow &,const model::Curve &)> & osSetter,
      const std::function<model::Curve(model::CoilHeatingDXVariableRefrigerantFlow &)> & osGetter) {

    auto value = element.child(elementName.c_str()).text().as_string();
    auto newcurve = model.getModelObjectByName<model::Curve>(value);
    if( newcurve ) {
      auto oldcurve = osGetter(coil);
      if( oldcurve != newcurve.get() ) {
        oldcurve.remove();
      }
      osSetter(coil,newcurve.get());
    }
  };

  setCurve("HtPumpCap_fTempCrvRef",
    std::mem_fn(&model::CoilHeatingDXVariableRefrigerantFlow::setHeatingCapacityRatioModifierFunctionofTemperatureCurve),
    std::mem_fn(&model::CoilHeatingDXVariableRefrigerantFlow::heatingCapacityRatioModifierFunctionofTemperatureCurve) );

  setCurve("HtPumpCap_fFlowCrvRef",
    std::mem_fn(&model::CoilHeatingDXVariableRefrigerantFlow::setHeatingCapacityModifierFunctionofFlowFractionCurve),
    std::mem_fn(&model::CoilHeatingDXVariableRefrigerantFlow::heatingCapacityModifierFunctionofFlowFractionCurve));

  if( ! autosize() ) {
    boost::optional<double> _value = lexicalCastToDouble(element.parent().child("Fan").child("FlowCapSim"));
    if( _value ) {
      double value = unitToUnit(_value.get(),"cfm","m^3/s").get();
      coil.setRatedAirFlowRate(value);
    }
  }

  return coil;
}

boost::optional<openstudio::model::ModelObject> ReverseTranslator::translateCoilCoolingDXVariableRefrigerantFlow(
    const pugi::xml_node& element,
    openstudio::model::Model& model)
{
  if( ! istringEqual(element.name(),"CoilClg") ) {
    return boost::none;
  }

  model::CoilCoolingDXVariableRefrigerantFlow coil(model);

  {
    auto value = element.child("Name").text().as_string();
    coil.setName(value);
  }

  if( ! autosize() ) {
    boost::optional<double> _capTotGrossRtdSim = lexicalCastToDouble(element.child("CapTotGrossRtdSim"));
    if( _capTotGrossRtdSim ) {
      double value = unitToUnit(_capTotGrossRtdSim.get(),"Btu/h","W").get();
      coil.setRatedTotalCoolingCapacity(value);
    }
  }

  auto setCurve = [&](const std::string & elementName,
      const std::function<bool(model::CoilCoolingDXVariableRefrigerantFlow &,const model::Curve &)> & osSetter,
      const std::function<model::Curve(model::CoilCoolingDXVariableRefrigerantFlow &)> & osGetter) {

    auto value = element.child(elementName.c_str()).text().as_string();
    auto newcurve = model.getModelObjectByName<model::Curve>(value);
    if( newcurve ) {
      auto oldcurve = osGetter(coil);
      if( oldcurve != newcurve.get() ) {
        oldcurve.remove();
      }
      osSetter(coil,newcurve.get());
    }
  };

  setCurve("Cap_fTempCrvRef",
    std::mem_fn(&model::CoilCoolingDXVariableRefrigerantFlow::setCoolingCapacityRatioModifierFunctionofTemperatureCurve),
    std::mem_fn(&model::CoilCoolingDXVariableRefrigerantFlow::coolingCapacityRatioModifierFunctionofTemperatureCurve));

  setCurve("Cap_fFlowCrvRef",
    std::mem_fn(&model::CoilCoolingDXVariableRefrigerantFlow::setCoolingCapacityModifierCurveFunctionofFlowFraction),
    std::mem_fn(&model::CoilCoolingDXVariableRefrigerantFlow::coolingCapacityModifierCurveFunctionofFlowFraction));

  if( ! autosize() ) {
    boost::optional<double> _value = lexicalCastToDouble(element.parent().child("Fan").child("FlowCapSim"));

    if( _value ) {
      double value = unitToUnit(_value.get(),"cfm","m^3/s").get();
      coil.setRatedAirFlowRate(value);
    }
  }

  return coil;
}

boost::optional<openstudio::model::ModelObject> ReverseTranslator::translateCrvDblQuad(
    const pugi::xml_node& element,
    openstudio::model::Model& model)
{
  if( ! istringEqual(element.name(),"CrvDblQuad") )
  {
    return boost::none;
  }

  model::CurveBiquadratic curve(model);

  // Name

  pugi::xml_node nameElement = element.child("Name");

  curve.setName(nameElement.text().as_string());

  // Coef1

  pugi::xml_node coef1Element = element.child("Coef1");

  curve.setCoefficient1Constant(coef1Element.text().as_double());

  // Coef2

  pugi::xml_node coef2Element = element.child("Coef2");

  curve.setCoefficient2x(coef2Element.text().as_double());

  // Coef3

  pugi::xml_node coef3Element = element.child("Coef3");

  curve.setCoefficient3xPOW2(coef3Element.text().as_double());

  // Coef4

  pugi::xml_node coef4Element = element.child("Coef4");

  curve.setCoefficient4y(coef4Element.text().as_double());

  // Coef5

  pugi::xml_node coef5Element = element.child("Coef5");

  curve.setCoefficient5yPOW2(coef5Element.text().as_double());

  // Coef6

  pugi::xml_node coef6Element = element.child("Coef6");

  curve.setCoefficient6xTIMESY(coef6Element.text().as_double());

  // MinVar1

  pugi::xml_node minVar1Element = element.child("MinVar1");

  boost::optional<double> _minVar1 = lexicalCastToDouble(minVar1Element);

  if(_minVar1)
  {
    curve.setMinimumValueofx(_minVar1.get());
  }
  else
  {
    curve.setMinimumValueofx(0.0);

    LOG(Warn,"Curve: " << nameElement.text().as_string() << " Missing X Minimum Limit");
  }

  // MaxVar1

  pugi::xml_node maxVar1Element = element.child("MaxVar1");

  boost::optional<double> _maxVar1 = lexicalCastToDouble(maxVar1Element);

  if(_maxVar1)
  {
    curve.setMaximumValueofx(_maxVar1.get());
  }
  else
  {
    curve.setMaximumValueofx(100.0);

    LOG(Warn,"Curve: " << nameElement.text().as_string() << " Missing X Maximum Limit");
  }

  // MinVar2

  pugi::xml_node minVar2Element = element.child("MinVar2");

  boost::optional<double> _minVar2 = lexicalCastToDouble(minVar2Element);

  if(_minVar2)
  {
    curve.setMinimumValueofy(_minVar2.get());
  }
  else
  {
    curve.setMinimumValueofy(0.0);

    LOG(Warn,"Curve: " << nameElement.text().as_string() << " Missing Y Minimum Limit");
  }

  // MaxVar2

  pugi::xml_node maxVar2Element = element.child("MaxVar2");

  boost::optional<double> _maxVar2 = lexicalCastToDouble(maxVar2Element);

  if(_maxVar2)
  {
    curve.setMaximumValueofy(_maxVar2.get());
  }
  else
  {
    curve.setMaximumValueofy(100.0);

    LOG(Warn,"Curve: " << nameElement.text().as_string() << " Missing Y Maximum Limit");
  }

  // MaxOut
  pugi::xml_node maxOutElement = element.child("MaxOut");
  boost::optional<double> _maxOut = lexicalCastToDouble(maxOutElement);
  if( _maxOut )
  {
    curve.setMaximumCurveOutput(_maxOut.get());
  }

  // MinOut
  pugi::xml_node minOutElement = element.child("MinOut");
  boost::optional<double> _minOut = lexicalCastToDouble(minOutElement);
  if( _minOut )
  {
    curve.setMinimumCurveOutput(_minOut.get());
  }

  return curve;
}

boost::optional<openstudio::model::ModelObject> ReverseTranslator::translateCrvCubic(
    const pugi::xml_node& element,
    openstudio::model::Model& model)
{
  if( ! istringEqual(element.name(),"CrvCubic") )
  {
    return boost::none;
  }

  model::CurveCubic curve(model);

  // Name

  pugi::xml_node nameElement = element.child("Name");

  curve.setName(nameElement.text().as_string());

  // Coef1

  pugi::xml_node coef1Element = element.child("Coef1");

  curve.setCoefficient1Constant(coef1Element.text().as_double());

  // Coef2

  pugi::xml_node coef2Element = element.child("Coef2");

  curve.setCoefficient2x(coef2Element.text().as_double());

  // Coef3

  pugi::xml_node coef3Element = element.child("Coef3");

  curve.setCoefficient3xPOW2(coef3Element.text().as_double());

  // Coef4

  pugi::xml_node coef4Element = element.child("Coef4");

  curve.setCoefficient4xPOW3(coef4Element.text().as_double());

  // MinVar1

  pugi::xml_node minVar1Element = element.child("MinVar1");


  boost::optional<double> _minVar1 = lexicalCastToDouble(minVar1Element);

  if(_minVar1)
  {
    curve.setMinimumValueofx(_minVar1.get());
  }
  else
  {
    curve.setMinimumValueofx(0.0);

    LOG(Warn,"Curve: " << nameElement.text().as_string() << " Missing X Minimum Limit");
  }

  // MaxVar1

  pugi::xml_node maxVar1Element = element.child("MaxVar1");

  boost::optional<double> _maxVar1 = lexicalCastToDouble(maxVar1Element);

  if(_maxVar1)
  {
    curve.setMaximumValueofx(_maxVar1.get());
  }
  else
  {
    curve.setMaximumValueofx(100.0);

    LOG(Warn,"Curve: " << nameElement.text().as_string() << " Missing X Maximum Limit");
  }

  // MaxOut
  pugi::xml_node maxOutElement = element.child("MaxOut");
  boost::optional<double> _maxOut = lexicalCastToDouble(maxOutElement);
  if( _maxOut )
  {
    curve.setMaximumCurveOutput(_maxOut.get());
  }

  // MinOut
  pugi::xml_node minOutElement = element.child("MinOut");
  boost::optional<double> _minOut = lexicalCastToDouble(minOutElement);
  if( _minOut )
  {
    curve.setMinimumCurveOutput(_minOut.get());
  }

  return curve;
}

boost::optional<openstudio::model::ModelObject> ReverseTranslator::translateCrvLin(
    const pugi::xml_node& element,
    openstudio::model::Model& model)
{
  if( ! istringEqual(element.name(),"CrvLin") ) {
    return boost::none;
  }

  model::CurveLinear curve(model);

  // Name
  pugi::xml_node nameElement = element.child("Name");
  curve.setName(nameElement.text().as_string());

  // Coef1
  pugi::xml_node coef1Element = element.child("Coef1");
  curve.setCoefficient1Constant(coef1Element.text().as_double());

  // Coef2
  pugi::xml_node coef2Element = element.child("Coef2");
  curve.setCoefficient2x(coef2Element.text().as_double());

  // MinVar1
  auto minVar1Element = element.child("MinVar1");
  boost::optional<double> _minVar1 = lexicalCastToDouble(minVar1Element);
  if(_minVar1) {
    curve.setMinimumValueofx(_minVar1.get());
  } else {
    curve.setMinimumValueofx(0.0);
    LOG(Warn,"Curve: " << nameElement.text().as_string() << " Missing X Minimum Limit");
  }

  // MaxVar1
  auto maxVar1Element = element.child("MaxVar1");
  boost::optional<double> _maxVar1 = lexicalCastToDouble(maxVar1Element);
  if(_maxVar1) {
    curve.setMaximumValueofx(_maxVar1.get());
  } else {
    curve.setMaximumValueofx(100.0);
    LOG(Warn,"Curve: " << nameElement.text().as_string() << " Missing X Maximum Limit");
  }

  // MaxOut
  auto maxOutElement = element.child("MaxOut");
  boost::optional<double> _maxOut = lexicalCastToDouble(maxOutElement);
  if( _maxOut ) {
    curve.setMaximumCurveOutput(_maxOut.get());
  }

  // MinOut
  auto minOutElement = element.child("MinOut");
  boost::optional<double> _minOut = lexicalCastToDouble(minOutElement);
  if( _minOut ) {
    curve.setMinimumCurveOutput(_minOut.get());
  }

  return curve;
}

boost::optional<openstudio::model::ModelObject> ReverseTranslator::translateCrvQuad(
    const pugi::xml_node& element,
    openstudio::model::Model& model)
{
  if( ! istringEqual(element.name(),"CrvQuad") )
  {
    return boost::none;
  }

  model::CurveQuadratic curve(model);

  // Name

  pugi::xml_node nameElement = element.child("Name");

  curve.setName(nameElement.text().as_string());

  // Coef1

  pugi::xml_node coef1Element = element.child("Coef1");

  curve.setCoefficient1Constant(coef1Element.text().as_double());

  // Coef2

  pugi::xml_node coef2Element = element.child("Coef2");

  curve.setCoefficient2x(coef2Element.text().as_double());

  // Coef3

  pugi::xml_node coef3Element = element.child("Coef3");

  curve.setCoefficient3xPOW2(coef3Element.text().as_double());

  // MinVar1

  pugi::xml_node minVar1Element = element.child("MinVar1");


  boost::optional<double> _minVar1 = lexicalCastToDouble(minVar1Element);

  if(_minVar1)
  {
    curve.setMinimumValueofx(_minVar1.get());
  }
  else
  {
    curve.setMinimumValueofx(0.0);

    LOG(Warn,"Curve: " << nameElement.text().as_string() << " Missing X Minimum Limit");
  }

  // MaxVar1

  pugi::xml_node maxVar1Element = element.child("MaxVar1");

  boost::optional<double> _maxVar1 = lexicalCastToDouble(maxVar1Element);

  if(_maxVar1)
  {
    curve.setMaximumValueofx(_maxVar1.get());
  }
  else
  {
    curve.setMaximumValueofx(100.0);

    LOG(Warn,"Curve: " << nameElement.text().as_string() << " Missing X Maximum Limit");
  }

  // MaxOut
  pugi::xml_node maxOutElement = element.child("MaxOut");
  boost::optional<double> _maxOut = lexicalCastToDouble(maxOutElement);
  if( _maxOut )
  {
    curve.setMaximumCurveOutput(_maxOut.get());
  }

  // MinOut
  pugi::xml_node minOutElement = element.child("MinOut");
  boost::optional<double> _minOut = lexicalCastToDouble(minOutElement);
  if( _minOut )
  {
    curve.setMinimumCurveOutput(_minOut.get());
  }

  return curve;
}

boost::optional<openstudio::model::ModelObject> ReverseTranslator::translateCrvMapDblVar(
    const pugi::xml_node& element,
    openstudio::model::Model& model)
{
  if( ! istringEqual(element.name(),"CrvMapDblVar") ) {
    return boost::none;
  }

  model::TableMultiVariableLookup table(model,2);

  auto nameElement = element.child("Name");
  table.setName(nameElement.text().as_string());

  std::vector<pugi::xml_node> arrayVar1Elements;
  std::vector<pugi::xml_node> arrayVar2Elements;
  std::vector<pugi::xml_node> arrayOutElements;

  for (const pugi::xml_node& child: element.children()) {
    if( istringEqual(child.name(),"ArrayVar1") ) {
      arrayVar1Elements.push_back(child);
    } else if( istringEqual(child.name(),"ArrayVar2") ) {
      arrayVar2Elements.push_back(child);
    } else if( istringEqual(child.name(),"ArrayOut") ) {
      arrayOutElements.push_back(child);
    }
  }

  if( arrayVar1Elements.size() != arrayOutElements.size() ) {
    return table;
  }

  if( arrayVar2Elements.size() != arrayOutElements.size() ) {
    return table;
  }

  for(std::vector<pugi::xml_node>::size_type i = 0; i < arrayVar1Elements.size(); i++) {

    boost::optional<double> _arrayVar1 = lexicalCastToDouble(arrayVar1Elements[i]);
    boost::optional<double> _arrayVar2 = lexicalCastToDouble(arrayVar2Elements[i]);
    boost::optional<double> _arrayOut = lexicalCastToDouble(arrayOutElements[i]);

    if( _arrayVar1 && _arrayVar2 && _arrayOut ) {
      table.addPoint(_arrayVar1.get(), _arrayVar2.get(), _arrayOut.get());
    }
  }

  std::string text;

  boost::optional<double> _minOut = lexicalCastToDouble(element.child("MinOut"));
  if( _minOut ) {
    table.setMinimumTableOutput(_minOut.get());
  }

  boost::optional<double> _maxOut = lexicalCastToDouble(element.child("MaxOut"));
  if( _maxOut ) {
    table.setMaximumTableOutput(_maxOut.get());
  }

  text = element.child("UnitTypeVar1").text().as_string();
  table.setInputUnitTypeforX1(text);

  text = element.child("UnitTypeVar2").text().as_string();
  table.setInputUnitTypeforX2(text);

  text = element.child("UnitTypeOut").text().as_string();
  table.setOutputUnitType(text);

  boost::optional<double> _minVar1 = lexicalCastToDouble(element.child("MinVar1"));
  if( _minVar1 ) {
    table.setMinimumValueofX1(_minVar1.get());
  }

  boost::optional<double> _maxVar1 = lexicalCastToDouble(element.child("MaxVar1"));
  if( _maxVar1 ) {
    table.setMaximumValueofX1(_maxVar1.get());
  }

  boost::optional<double> _minVar2 = lexicalCastToDouble(element.child("MinVar2"));
  if( _minVar2 ) {
    table.setMinimumValueofX2(_minVar2.get());
  }

  boost::optional<double> _maxVar2 = lexicalCastToDouble(element.child("MaxVar2"));
  if( _maxVar2 ) {
    table.setMaximumValueofX2(_maxVar2.get());
  }

  text = element.child("InterpMthd").text().as_string();
  table.setInterpolationMethod(text);

  boost::optional<double> _normalizationPt = lexicalCastToDouble(element.child("NormalizationPt"));
  if( _normalizationPt ) {
    table.setNormalizationReference(_normalizationPt.get());
  }

  text = element.child("Type").text().as_string();
  table.setCurveType(text);

  return table;
}

boost::optional<openstudio::model::ModelObject> ReverseTranslator::translateCrvMapSglVar(
    const pugi::xml_node& element,
    openstudio::model::Model& model)
{
  if( ! istringEqual(element.name(),"CrvMapSglVar") ) {
    return boost::none;
  }

  model::TableMultiVariableLookup table(model,1);

  auto nameElement = element.child("Name");
  table.setName(nameElement.text().as_string());

  std::vector<pugi::xml_node> arrayVar1Elements;
  std::vector<pugi::xml_node> arrayOutElements;

  for (const pugi::xml_node& child: element.children()) {
    if( istringEqual(child.name(), "ArrayVar1") ) {
      arrayVar1Elements.push_back(child);
    } else if( istringEqual(child.name(),"ArrayOut") ) {
      arrayOutElements.push_back(child);
    }
  }

  if( arrayVar1Elements.size() != arrayOutElements.size() ) {
    return table;
  }

  for(std::vector<pugi::xml_node>::size_type i = 0; i < arrayVar1Elements.size(); i++) {
    boost::optional<double> _arrayVar1 = lexicalCastToDouble(arrayVar1Elements[i]);
    boost::optional<double> _arrayOut = lexicalCastToDouble(arrayOutElements[i]);

    if( _arrayVar1 && _arrayOut ) {
      table.addPoint(_arrayVar1.get(), _arrayOut.get());
    }
  }

  boost::optional<double> _minOut = lexicalCastToDouble(element.child("MinOut"));
  if( _minOut ) {
    table.setMinimumTableOutput(_minOut.get());
  }

  boost::optional<double> _maxOut = lexicalCastToDouble(element.child("MaxOut"));
  if( _maxOut ) {
    table.setMaximumTableOutput(_maxOut.get());
  }

  table.setInputUnitTypeforX1(element.child("UnitTypeVar1").text().as_string());

  table.setOutputUnitType(element.child("UnitTypeOut").text().as_string());

  boost::optional<double> _minVar1 = lexicalCastToDouble(element.child("MinVar1"));
  if( _minVar1 ) {
    table.setMinimumValueofX1(_minVar1.get());
  }

  boost::optional<double> _maxVar1 = lexicalCastToDouble(element.child("MaxVar1"));
  if( _maxVar1 ) {
    table.setMaximumValueofX1(_maxVar1.get());
  }

  table.setInterpolationMethod(element.child("InterpMthd").text().as_string());

  boost::optional<double> _normalizationPt = lexicalCastToDouble(element.child("NormalizationPt"));
  if( _normalizationPt ) {
    table.setNormalizationReference(_normalizationPt.get());
  }

  table.setCurveType(element.child("Type").text().as_string());

  return table;
}

pugi::xml_node ReverseTranslator::findThrmlZnElement(const pugi::xml_node& thrmlZnRefElement)
{
  auto projectElement = getProjectElement(thrmlZnRefElement);

  // Proj > Bldg > [ThrmlZn]
  auto thrmlZnElements = makeVectorOfChildren(projectElement.child("Bldg"), "ThrmlZn");
  auto thrmlZnName = thrmlZnRefElement.text().as_string();

  for (std::vector<pugi::xml_node>::size_type i = 0; i < thrmlZnElements.size(); i++) {
    auto thrmlZnElement = thrmlZnElements[i];
    auto thisName = thrmlZnElement.child("Name").text().as_string();

    if (openstudio::istringEqual(thrmlZnName, thisName)) {
      return thrmlZnElement;
    }
  }

  LOG(Debug, "Couldn't locate the ThrmlZn element with name '" << thrmlZnName << "'.");
  return pugi::xml_node();
}

pugi::xml_node ReverseTranslator::findZnSysElement(const pugi::xml_node& znSysRefElement)
{
  pugi::xml_node projectElement = getProjectElement(znSysRefElement);
  // TODO: perhaps we should rely on the makeVectorOfChildrenRecursive instead (uses Xpath instead of having to harcoded the path)
  // Not sure about speed penalties
  // makeVectorOfChildrenRecursive("ZnSys") would work, regardless of which level 'ZnSys' is at
  // Proj > Bldg > [ZnSys]
  std::vector<pugi::xml_node> znSysElements = makeVectorOfChildren(projectElement.child("Bldg"), "ZnSys");
  std::string znSysName = znSysRefElement.text().as_string();

  if (znSysName.empty()) {
    LOG(Error, "findZnSysElement called with an empty znSysName");
    OS_ASSERT(false);
  }

  for (std::vector<pugi::xml_node>::size_type i = 0; i < znSysElements.size(); i++)
  {
    pugi::xml_node znSysElement = znSysElements[i];
    std::string znSysElementName = znSysElement.child("Name").text().as_string();

    if (openstudio::istringEqual(znSysElementName, znSysName)) {
      return znSysElement;
    }
  }

  LOG(Debug, "Couldn't locate the ZnSys element with name '" << znSysName << "'.");
  return pugi::xml_node();
}

pugi::xml_node ReverseTranslator::findTrmlUnitElementForZone(const pugi::xml_node& znNameElement, const std::string & airLoopName)
{
  pugi::xml_node projectElement = getProjectElement(znNameElement);
  // Proj > Bldg > [AirSys]
  std::vector<pugi::xml_node> airSystemElements = makeVectorOfChildren(projectElement.child("Bldg"), "AirSys");
  std::string zoneName = znNameElement.text().as_string();
  if (zoneName.empty()) {
    LOG(Error, "findTrmlUnitElementForZone called with an empty zoneName");
    OS_ASSERT(false);
  }

  for( std::vector<pugi::xml_node>::size_type i = 0; i < airSystemElements.size(); i++ )
  {
    pugi::xml_node airSystemElement = airSystemElements[i];

    const auto & thisSystemName = airSystemElement.child("Name").text().as_string();
    if(airLoopName != thisSystemName) {
      continue;
    }

    std::vector<pugi::xml_node> terminalElements = makeVectorOfChildren(airSystemElement, "TrmlUnit");
    for (std::vector<pugi::xml_node>::size_type j = 0; j < terminalElements.size(); j++ )
    {
      pugi::xml_node terminalElement = terminalElements[j];
      std::string zoneServedElementName = terminalElement.child("ZnServedRef").text().as_string();

      if (openstudio::istringEqual(zoneName, zoneServedElementName)) {
        return terminalElement;
      }
    }
  }

  LOG(Debug, "Couldn't locate the TrmlUnit element for zoneName '" << zoneName << "'.");

  return pugi::xml_node();
}

pugi::xml_node ReverseTranslator::findAirSysElement(const pugi::xml_node& airSyRefElement)
{

  pugi::xml_node projectElement = getProjectElement(airSyRefElement);
  std::string airSysName = airSyRefElement.text().as_string();
  return  findAirSysElement(airSysName, projectElement);
}

pugi::xml_node ReverseTranslator::findAirSysElement(const std::string& airSysName, const pugi::xml_node& projectElement)
{
  if (!projectElement || (strcmp(projectElement.name(), "Proj") != 0)) {
    LOG(Error, "findAirSysElement has to be called with the projectElement");
    OS_ASSERT(false);
  }
  if (airSysName.empty()) {
    LOG(Error, "findAirSysElement called with an empty airSysName");
    OS_ASSERT(false);
  }

  // Proj > Bldg > [AirSys]
  std::vector<pugi::xml_node> airSystemElements = makeVectorOfChildren(projectElement.child("Bldg"), "AirSys");

  for( std::vector<pugi::xml_node>::size_type i = 0; i < airSystemElements.size(); i++ )
  {
    pugi::xml_node airSystemElement = airSystemElements[i];
    std::string airSystemElementName = airSystemElement.child("Name").text().as_string();

    if (openstudio::istringEqual(airSysName, airSystemElementName)) {
      return airSystemElement;
    }
  }

  LOG(Debug, "Couldn't locate the AirSys element for airSysName '" << airSysName << "'.");

  return pugi::xml_node();
}

boost::optional<pugi::xml_node> ForwardTranslator::translateAirLoopHVAC(const model::AirLoopHVAC& airLoop, pugi::xml_node& root)
{
  auto result = root.append_child("AirSys");
  m_translatedObjects[airLoop.handle()] = result;

  // Gather info about the system makeup
  auto variableFans = airLoop.supplyComponents(model::FanVariableVolume::iddObjectType());
  auto coilHeatingDXSingleSpeeds = airLoop.supplyComponents(model::CoilHeatingDXSingleSpeed::iddObjectType());
  auto coilCoolingDXSingleSpeeds = airLoop.supplyComponents(model::CoilCoolingDXSingleSpeed::iddObjectType());
  auto coilCoolingDXTwoSpeeds = airLoop.supplyComponents(model::CoilCoolingDXTwoSpeed::iddObjectType());
  auto coilCoolingDXMultiSpeeds = airLoop.supplyComponents(model::CoilCoolingDXMultiSpeed::iddObjectType());
  auto coilCoolingWaters = airLoop.supplyComponents(model::CoilCoolingWater::iddObjectType());
  auto zones = airLoop.thermalZones();
  auto supplyOutletNode = airLoop.supplyOutletNode();
  auto spms = supplyOutletNode.setpointManagers();
  auto singleZoneReheats = subsetCastVector<model::SetpointManagerSingleZoneReheat>(spms);

  // Name
  auto name = airLoop.name().get();
  auto nameElement = result.append_child("Name");
  nameElement.text() = escapeName(name).c_str();

  // Type
  // 1:  "PVAV"
  // 2:  "VAV"
  // 3:  "SZAC"
  // 4:  "SZHP"
  // 5:  "SZVAVAC"
  // 6:  "SZVAVHP"
  // 7:  "HV"
  // 8:  "Exhaust" - TODO figure out how to identify this one

  const std::string PVAV = "PVAV";
  const std::string VAV = "VAV";
  const std::string SZAC = "SZAC";
  const std::string SZHP = "SZHP";
  const std::string SZVAVAC = "SZVAVAC";
  const std::string SZVAVHP = "SZVAVHP";
  const std::string HV = "HV";
  const std::string EXHAUST = "EXHAUST";

  std::string type;

  if( coilCoolingDXTwoSpeeds.empty() &&
    coilCoolingDXSingleSpeeds.empty() &&
    coilCoolingDXMultiSpeeds.empty() &&
    coilCoolingWaters.empty() )
  {
    type = HV;
  } else {
    if( ! variableFans.empty() ) { // Variable speed fan
      if( ! singleZoneReheats.empty() ) { // Single zone reheat SPM
        if( ! coilHeatingDXSingleSpeeds.empty() ) { // DX Heating (Heat pump)
          type = SZVAVHP;
        } else { // Not DX Heating (No heat pump)
          type = SZVAVAC;
        }
      } else { // Anything besides single zone reheat
        if( ! coilCoolingWaters.empty() ) { // Chilled water coil
          type = VAV;
        } else { // No chilled water coil
          type = PVAV;
        }
      }
    } else { // Not variable speed fan
      if( ! singleZoneReheats.empty() ) { // Single zone reheat SPM
        if( ! coilHeatingDXSingleSpeeds.empty() ) { // DX Heating (Heat pump)
          type = SZHP;
        } else { // Not DX Heating (No heat pump)
          type = SZAC;
        }
      } else {
        LOG(Warn,airLoop.briefDescription() << " does not directly map to an air system type. Assuming VAV even though there is no variable speed fan.")
        type = VAV;
      }
    }
  }

  OS_ASSERT( ! type.empty() );

  auto typeElement = result.append_child("TypeSim");
  typeElement.text() = escapeName(type).c_str();

  // TODO
  // SubType
  // 1:  "SinglePackage"
  // 2:  "SplitSystem"
  // 3:  "CRAC"
  // 4:  "CRAH"

  // NightCycleFanCtrl
  auto nightCycleFanCtrlElement = result.append_child("NightCycleFanCtrl");

  // OpenStudio string to represent night cycle control method
  auto osNightCycleFanCtrl = airLoop.nightCycleControlType();
  // Convert to SDD string
  std::string nightCycleFanCtrl;
  if( istringEqual(osNightCycleFanCtrl,"CycleOnAny") ) {
    nightCycleFanCtrl = "CycleOnCallAnyZone";
  } else if( istringEqual(osNightCycleFanCtrl,"CycleOnAnyZoneFansOnly") ) {
    nightCycleFanCtrl = "CycleZoneFansOnly";
  } else {
    nightCycleFanCtrl = "StaysOff";
  }

  nightCycleFanCtrlElement.text() = escapeName(nightCycleFanCtrl).c_str();

  // Cnt
  auto cntElement = result.append_child("Cnt");
  cntElement.text() = "1";

  // FanPos
  std::string fanPos;
  auto inletComp = supplyOutletNode.inletModelObject();
  OS_ASSERT(inletComp);
  if( inletComp->optionalCast<model::FanConstantVolume>() || inletComp->optionalCast<model::FanVariableVolume>() )
    fanPos = "DrawThrough";
  else
    fanPos = "BlowThrough";
  auto fanPosElement = result.append_child("FanPos");
  fanPosElement.text() = fanPos.c_str();

  // ClgCtrl
  for( const auto & spm : spms ) {
    if( istringEqual(spm.controlVariable(),"Temperature") ) {
      if( auto tempSPM = spm.optionalCast<model::SetpointManagerFollowOutdoorAirTemperature>() ) {
        // SetpointManagerFollowOutdoorAirTemperature is not supported by CBECC so it is converted to oa reset.
        LOG(Warn,tempSPM->briefDescription() << " is translated to ClgCtrl type OutsideAirReset in CBECC.");
        auto clgCtrlElement = result.append_child("ClgCtrl");
        clgCtrlElement.text() = "OutsideAirReset";

        auto clRstSupHiElement = result.append_child("ClRstSupHi");
        clRstSupHiElement.text() = "100";

        auto clRstSupLowElement = result.append_child("ClRstSupLow");
        clRstSupLowElement.text() = "0";

        auto clRstOutdrHiElement = result.append_child("ClRstOutdrHi");
        clRstOutdrHiElement.text() = "100";

        auto clRstOutdrLowElement = result.append_child("ClRstOutdrLow");
        clRstOutdrLowElement.text() = "0";

        m_translatedObjects[tempSPM->handle()] = clgCtrlElement;
      } else if( auto tempSPM = spm.optionalCast<model::SetpointManagerOutdoorAirReset>() ) {
        auto clgCtrlElement = result.append_child("ClgCtrl");
        clgCtrlElement.text() = "OutsideAirReset";

        auto clRstSupHiElement = result.append_child("ClRstSupHi");
        auto clRstSupHi = convert(tempSPM->setpointatOutdoorHighTemperature(),"C","F").get();
        clRstSupHiElement.text() = openstudio::string_conversions::number(clRstSupHi).c_str();

        auto clRstSupLowElement = result.append_child("ClRstSupLow");
        auto clRstSupLow = convert(tempSPM->setpointatOutdoorLowTemperature(),"C","F").get();
        clRstSupLowElement.text() = openstudio::string_conversions::number(clRstSupLow).c_str();

        auto clRstOutdrHiElement = result.append_child("ClRstOutdrHi");
        auto clRstOutdrHi = convert(tempSPM->outdoorHighTemperature(),"C","F").get();
        clRstOutdrHiElement.text() = openstudio::string_conversions::number(clRstOutdrHi).c_str();

        auto clRstOutdrLowElement = result.append_child("ClRstOutdrLow");
        auto clRstOutdrLow = convert(tempSPM->outdoorHighTemperature(),"C","F").get();
        clRstOutdrLowElement.text() = openstudio::string_conversions::number(clRstOutdrLow).c_str();

        m_translatedObjects[tempSPM->handle()] = clgCtrlElement;
      } else if( auto tempSPM = spm.optionalCast<model::SetpointManagerScheduled>() ) {
        auto clgCtrlElement = result.append_child("ClgCtrl");
        clgCtrlElement.text() = "Scheduled";

        auto clgSetPtSchRefElement = result.append_child("ClgSetptSchRef");
        const auto & schedule = tempSPM->schedule();
        clgSetPtSchRefElement.text() = escapeName(schedule.name().get()).c_str();

        m_translatedObjects[tempSPM->handle()] = clgCtrlElement;
      } else if( auto tempSPM = spm.optionalCast<model::SetpointManagerScheduledDualSetpoint>() ) {
        LOG(Error,tempSPM->briefDescription() << " is not supported by CBECC.");
      } else if( auto tempSPM = spm.optionalCast<model::SetpointManagerSingleZoneReheat>() ) {
        auto clgCtrlElement = result.append_child("ClgCtrl");
        clgCtrlElement.text() = "NoSATControl";

        m_translatedObjects[tempSPM->handle()] = clgCtrlElement;
      } else if( auto tempSPM = spm.optionalCast<model::SetpointManagerWarmestTemperatureFlow>() ) {
        auto clgCtrlElement = result.append_child("ClgCtrl");
        if( istringEqual(tempSPM->strategy(),"FlowFirst") ) {
          clgCtrlElement.text() = "WarmestResetFlowFirst";
        } else {
          clgCtrlElement.text() = "WarmestResetTemperatureFirst";
        }

        auto clRstSupHiElement = result.append_child("ClRstSupHi");
        auto clRstSupHi = convert(tempSPM->maximumSetpointTemperature(),"C","F").get();
        clRstSupHiElement.text() = openstudio::string_conversions::number(clRstSupHi).c_str();

        auto clRstSupLowElement = result.append_child("ClRstSupLow");
        auto clRstSupLow = convert(tempSPM->maximumSetpointTemperature(),"C","F").get();
        clRstSupLowElement.text() = openstudio::string_conversions::number(clRstSupLow).c_str();

        auto dsgnAirFlowMinElement = result.append_child("DsgnAirFlowMin");
        auto dsgnAirFlowMin = tempSPM->minimumTurndownRatio();
        dsgnAirFlowMinElement.text() = openstudio::string_conversions::number(dsgnAirFlowMin).c_str();

        m_translatedObjects[tempSPM->handle()] = clgCtrlElement;
      } else if( auto tempSPM = spm.optionalCast<model::SetpointManagerWarmest>() ) {
        auto clgCtrlElement = result.append_child("ClgCtrl");
        clgCtrlElement.text() = "WarmestReset";

        auto clRstSupHiElement = result.append_child("ClRstSupHi");
        auto clRstSupHi = convert(tempSPM->maximumSetpointTemperature(),"C","F").get();
        clRstSupHiElement.text() = openstudio::string_conversions::number(clRstSupHi).c_str();

        auto clRstSupLowElement = result.append_child("ClRstSupLow");
        auto clRstSupLow = convert(tempSPM->maximumSetpointTemperature(),"C","F").get();
        clRstSupLowElement.text() = openstudio::string_conversions::number(clRstSupLow).c_str();

        m_translatedObjects[tempSPM->handle()] = clgCtrlElement;
      } else {
        LOG(Error,spm.briefDescription() << " does not currently map into SDD format.")
        // TODO Handle other SPMs
      }

      break;
    }
  }

  // Translate supply components

  auto airSegElement = result.append_child("AirSeg");

  auto airSegNameElement = airSegElement.append_child("Name");
  airSegNameElement.text() = (name + " Supply AirSeg").c_str();

  auto airSegTypeElement = airSegElement.append_child("Type");
  airSegTypeElement.text() = "Supply";

  for( auto & comp : airLoop.supplyComponents() ) {
    if( auto fan = comp.optionalCast<model::FanConstantVolume>() ) {
      translateFanConstantVolume(fan.get(),airSegElement);
    } else if ( auto coil = comp.optionalCast<model::CoilCoolingDXSingleSpeed>() ) {
      translateCoilCoolingDXSingleSpeed(coil.get(),airSegElement);
    } else if ( auto coil = comp.optionalCast<model::CoilHeatingGas>() ) {
      translateCoilHeatingGas(coil.get(),airSegElement);
    } else if ( auto oasys = comp.optionalCast<model::AirLoopHVACOutdoorAirSystem>() ) {
      translateAirLoopHVACOutdoorAirSystem(oasys.get(),result);
    } else {
      // TODO Handle other supply component types
      LOG(Warn,comp.briefDescription() << " does not currently map into SDD format.")
    }
  }

  return result;
}

boost::optional<pugi::xml_node> ForwardTranslator::translateAirLoopHVACOutdoorAirSystem(const openstudio::model::AirLoopHVACOutdoorAirSystem& oasys, pugi::xml_node& airSegElement)
{
  // TODO: nothing is happening here really
  auto result = airSegElement.append_child("OACtrl");
  m_translatedObjects[oasys.handle()] = result;
  return result;
}

boost::optional<pugi::xml_node> ForwardTranslator::translateCoilHeatingGas(const openstudio::model::CoilHeatingGas& coil, pugi::xml_node& airSegElement)
{
  auto result = airSegElement.append_child("CoilHtg");
  m_translatedObjects[coil.handle()] = result;

  // Type
  auto typeElement = result.append_child("Type");
  typeElement.text() = "Furnace";

  // FuelSrc
  auto fuelSrcElement = result.append_child("FuelSrc");
  fuelSrcElement.text() = "NaturalGas";

  // CapTotGrossRtd
  if( coil.isNominalCapacityAutosized() ) {
    m_autoHardSize = true;
  } else if( auto value = coil.nominalCapacity() ) {
    auto capTotGrossRtdElement = result.append_child("CapTotGrossRtd");
    capTotGrossRtdElement.text() = openstudio::string_conversions::number(convert(value.get(),"W","Btu/h").get()).c_str();
  }

  // FurnThrmlEff
  auto furnThrmlEffElement = result.append_child("FurnThrmlEff");
  furnThrmlEffElement.text() = openstudio::string_conversions::number(convert(coil.gasBurnerEfficiency(),"W","Btu/h").get()).c_str();

  return result;
}

boost::optional<pugi::xml_node> ForwardTranslator::translateCoilCoolingDXSingleSpeed(const openstudio::model::CoilCoolingDXSingleSpeed& coil, pugi::xml_node& airSegElement)
{
  auto result = airSegElement.append_child("CoilClg");
  m_translatedObjects[coil.handle()] = result;

  // Type
  auto typeElement = result.append_child("Type");
  typeElement.text() = "DirectExpansion";

  // NumClgStages
  auto numClgStagesElement = result.append_child("NumClgStages");
  numClgStagesElement.text() = "1";

  // CapTotGrossRtd
  if( coil.isRatedTotalCoolingCapacityAutosized() ) {
    m_autoHardSize = true;
  } else if( auto value = coil.ratedTotalCoolingCapacity() ) {
    auto capTotGrossRtdElement = result.append_child("CapTotGrossRtd");
    capTotGrossRtdElement.text() = openstudio::string_conversions::number(convert(value.get(),"W","Btu/h").get()).c_str();
  }

  // DXEER
  if( auto cop = coil.ratedCOP() ) {
    auto r = 0.12;
    auto eer = (cop.get() * (1 - r)  - r) * 3.413;

    auto dxEERElement = result.append_child("DXEER");
    dxEERElement.text() = openstudio::string_conversions::number(eer).c_str();
  } else {
    m_autoEfficiency = true;
  }

  // CndsrType
  auto cndsrTypeElement = result.append_child("CndsrType");
  cndsrTypeElement.text() = "Air";

  return result;
}

boost::optional<pugi::xml_node> ForwardTranslator::translateFanConstantVolume(const openstudio::model::FanConstantVolume& fan, pugi::xml_node& airSegElement)
{
  auto result = airSegElement.append_child("Fan");
  m_translatedObjects[fan.handle()] = result;

  // CtrlMthd
  auto ctrlMthdElement = result.append_child("CtrlMthdSim");
  ctrlMthdElement.text() = "ConstantVolume";

  // Class - Centrifugal | Axial
  // TODO Define a way to deterimine or specify
  auto classElement = result.append_child("Class");
  classElement.text() = "Axial";

  // ModelingMthd
  auto modelingMthdElement = result.append_child("ModelingMthd");
  modelingMthdElement.text() = "StaticPressure";

  // FlowCap
  if( fan.isMaximumFlowRateAutosized() ) {
    m_autoHardSize = true;
  } else if( auto value = fan.maximumFlowRate() ) {
    auto flowCapElement = result.append_child("FlowCap");
    // Note JM 2019-01-16: I use string_conversions::number here to mimic QString::number's number of decimals
    flowCapElement.text() = openstudio::string_conversions::number(convert(value.get(),"m^3/s","cfm").get()).c_str();
  }

  // FlowEff
  auto flowEffElement = result.append_child("FlowEff");
  flowEffElement.text() = openstudio::string_conversions::number(fan.fanEfficiency()).c_str();

  // TotStaticPress
  auto totStaticPressElement = result.append_child("TotStaticPress");
  // Convert in PA to WC
  totStaticPressElement.text() = openstudio::string_conversions::number(fan.pressureRise() / 249.0889).c_str();

  // MtrEff
  auto mtrEffElement = result.append_child("MtrEff");
  mtrEffElement.text() = openstudio::string_conversions::number(fan.motorEfficiency()).c_str();

  // MtrPos
  auto mtrPosElement = result.append_child("MtrfPos");
  if( fan.motorInAirstreamFraction() >= 0.5 ) {
    mtrPosElement.text() = "InAirStream";
  } else {
    mtrPosElement.text() = "NotInAirStream";
  }


  return result;
}

boost::optional<openstudio::model::ModelObject> ReverseTranslator::translateAvailSchRef(
  const pugi::xml_node& element, 
  openstudio::model::Model& model
) {
  if (! istringEqual(element.name(), "AvailSchRef")) {
    return boost::none;
  }

  boost::optional<model::ModelObject> result;

  result = model.getModelObjectByName<model::Schedule>(element.text().as_string());

  if (! result) {
    result = model.alwaysOnDiscreteSchedule();
  }

  return result;
}

} // sdd
} // openstudio

