/**********************************************************************
 *  Copyright (c) 2008-2015, Alliance for Sustainable Energy.
 *  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 **********************************************************************/

#include "ReverseTranslator.hpp"
#include "ForwardTranslator.hpp"
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
#include "../model/CoilHeatingGas.hpp"
#include "../model/CoilHeatingGas_Impl.hpp"
#include "../model/CoilHeatingElectric.hpp"
#include "../model/CoilHeatingDXSingleSpeed.hpp"
#include "../model/ControllerWaterCoil.hpp"
#include "../model/ControllerWaterCoil_Impl.hpp"
#include "../model/CurveBiquadratic.hpp"
#include "../model/CurveBiquadratic_Impl.hpp"
#include "../model/CurveCubic.hpp"
#include "../model/CurveCubic_Impl.hpp"
#include "../model/CurveQuadratic.hpp"
#include "../model/CurveQuadratic_Impl.hpp"
#include "../model/Curve.hpp"
#include "../model/Curve_Impl.hpp"
#include "../model/ScheduleRuleset.hpp"
#include "../model/ScheduleDay.hpp"
#include "../model/ScheduleDay_Impl.hpp"
#include "../model/ScheduleYear.hpp"
#include "../model/ScheduleYear_Impl.hpp"
#include "../model/ScheduleWeek.hpp"
#include "../model/ScheduleWeek_Impl.hpp"
#include "../model/SizingZone.hpp"
#include "../model/SizingZone_Impl.hpp"
#include "../model/Node.hpp"
#include "../model/Node_Impl.hpp"
#include "../model/Space.hpp"
#include "../model/Space_Impl.hpp"
#include "../model/DesignSpecificationOutdoorAir.hpp"
#include "../model/DesignSpecificationOutdoorAir_Impl.hpp"
#include "../model/CoolingTowerSingleSpeed.hpp"
#include "../model/CoolingTowerSingleSpeed_Impl.hpp"
#include "../model/CoolingTowerVariableSpeed.hpp"
#include "../model/CoolingTowerVariableSpeed_Impl.hpp"
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
#include "../model/ThermalZone.hpp"
#include "../model/ThermalZone_Impl.hpp"
#include "../model/AirTerminalSingleDuctConstantVolumeCooledBeam.hpp"
#include "../model/AirTerminalSingleDuctUncontrolled.hpp"
#include "../model/AirTerminalSingleDuctVAVReheat.hpp"
#include "../model/AirTerminalSingleDuctVAVNoReheat.hpp"
#include "../model/AirTerminalSingleDuctParallelPIUReheat.hpp"
#include "../model/AirTerminalSingleDuctParallelPIUReheat_Impl.hpp"
#include "../model/AirTerminalSingleDuctSeriesPIUReheat.hpp"
#include "../model/AirTerminalSingleDuctSeriesPIUReheat_Impl.hpp"
#include "../model/ThermostatSetpointDualSetpoint.hpp"
#include "../model/ThermostatSetpointDualSetpoint_Impl.hpp"
#include "../model/Schedule.hpp"
#include "../model/Schedule_Impl.hpp"
#include "../model/PumpConstantSpeed.hpp"
#include "../model/PumpConstantSpeed_Impl.hpp"
#include "../model/PumpVariableSpeed.hpp"
#include "../model/PumpVariableSpeed_Impl.hpp"
#include "../model/BoilerHotWater.hpp"
#include "../model/BoilerHotWater_Impl.hpp"
#include "../model/ChillerElectricEIR.hpp"
#include "../model/ChillerElectricEIR_Impl.hpp"
#include "../model/CoilCoolingCooledBeam.hpp"
#include "../model/CoilCoolingCooledBeam_Impl.hpp"
#include "../model/CoilCoolingWater.hpp"
#include "../model/CoilCoolingWater_Impl.hpp"
#include "../model/CoilHeatingWater.hpp"
#include "../model/CoilHeatingWater_Impl.hpp"
#include "../model/WaterToAirComponent.hpp"
#include "../model/WaterToAirComponent_Impl.hpp"
#include "../model/PlantLoop.hpp"
#include "../model/PlantLoop_Impl.hpp"
#include "../model/ZoneHVACComponent.hpp"
#include "../model/ZoneHVACComponent_Impl.hpp"
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
#include "../model/CoilHeatingWaterBaseboard.hpp"
#include "../model/CoilHeatingWaterBaseboard_Impl.hpp"
#include "../model/WaterHeaterMixed.hpp"
#include "../model/WaterHeaterMixed_Impl.hpp"
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
#include "../model/EvaporativeCoolerDirectResearchSpecial.hpp"
#include "../model/EvaporativeCoolerDirectResearchSpecial_Impl.hpp"
#include "../model/EvaporativeCoolerIndirectResearchSpecial.hpp"
#include "../model/EvaporativeCoolerIndirectResearchSpecial_Impl.hpp"
#include "../model/DaylightingControl.hpp"
#include "../utilities/units/QuantityConverter.hpp"
#include "../utilities/units/IPUnit.hpp"
#include "../utilities/units/SIUnit.hpp"
#include "../utilities/units/BTUUnit.hpp"
#include "../utilities/units/CFMUnit.hpp"
#include "../utilities/units/FahrenheitUnit.hpp"
#include "../utilities/units/MPHUnit.hpp"
#include "../utilities/units/WhUnit.hpp"
#include "../utilities/core/Assert.hpp"
#include "../utilities/time/Time.hpp"
#include "../utilities/time/Date.hpp"
#include "../utilities/units/UnitFactory.hpp"
#include "../utilities/math/FloatCompare.hpp"
#include "../utilities/geometry/Geometry.hpp"
#include "../utilities/geometry/BoundingBox.hpp"

#include <QFile>
#include <QDomDocument>
#include <QDomElement>
#include <QStringList>
#include <cmath>

namespace openstudio {
namespace bec {

const double footToMeter =  0.3048;
const double cpWater = 4180.0;
const double densityWater = 1000.0;

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
    scheduleWeekClone.setName(name + " Week " + QString::number(i).toStdString());
    scheduleYearClone.addScheduleWeek(*dateIt,scheduleWeekClone);

    boost::optional<model::ScheduleDay> scheduleDay;

    if( (scheduleDay = scheduleWeek.sundaySchedule()) )
    {
      model::ScheduleDay s = scheduleDay->clone(model).cast<model::ScheduleDay>();
      s.setName(name + " Week " + QString::number(i).toStdString() + " Sunday");
      scheduleWeekClone.setSundaySchedule(s);
    }

    if( (scheduleDay = scheduleWeek.mondaySchedule()) )
    {
      model::ScheduleDay s = scheduleDay->clone(model).cast<model::ScheduleDay>();
      s.setName(name + " Week " + QString::number(i).toStdString() + " Monday");
      scheduleWeekClone.setMondaySchedule(s);
    }

    if( (scheduleDay = scheduleWeek.tuesdaySchedule()) )
    {
      model::ScheduleDay s = scheduleDay->clone(model).cast<model::ScheduleDay>();
      s.setName(name + " Week " + QString::number(i).toStdString() + " Tuesday");
      scheduleWeekClone.setTuesdaySchedule(s);
    }

    if( (scheduleDay = scheduleWeek.wednesdaySchedule()) )
    {
      model::ScheduleDay s = scheduleDay->clone(model).cast<model::ScheduleDay>();
      s.setName(name + " Week " + QString::number(i).toStdString() + " Wednesday");
      scheduleWeekClone.setWednesdaySchedule(s);
    }

    if( (scheduleDay = scheduleWeek.thursdaySchedule()) )
    {
      model::ScheduleDay s = scheduleDay->clone(model).cast<model::ScheduleDay>();
      s.setName(name + " Week " + QString::number(i).toStdString() + " Thursday");
      scheduleWeekClone.setThursdaySchedule(s);
    }

    if( (scheduleDay = scheduleWeek.fridaySchedule()) )
    {
      model::ScheduleDay s = scheduleDay->clone(model).cast<model::ScheduleDay>();
      s.setName(name + " Week " + QString::number(i).toStdString() + " Friday");
      scheduleWeekClone.setFridaySchedule(s);
    }

    if( (scheduleDay = scheduleWeek.saturdaySchedule()) )
    {
      model::ScheduleDay s = scheduleDay->clone(model).cast<model::ScheduleDay>();
      s.setName(name + " Week " + QString::number(i).toStdString() + " Saturday");
      scheduleWeekClone.setSaturdaySchedule(s);
    }

    if( (scheduleDay = scheduleWeek.holidaySchedule()) )
    {
      model::ScheduleDay s = scheduleDay->clone(model).cast<model::ScheduleDay>();
      s.setName(name + " Week " + QString::number(i).toStdString() + " Holiday");
      scheduleWeekClone.setHolidaySchedule(s);
    }

    if( (scheduleDay = scheduleWeek.summerDesignDaySchedule()) )
    {
      model::ScheduleDay s = scheduleDay->clone(model).cast<model::ScheduleDay>();
      s.setName(name + " Week " + QString::number(i).toStdString() + " Summer");
      scheduleWeekClone.setSummerDesignDaySchedule(s);
    }

    if( (scheduleDay = scheduleWeek.winterDesignDaySchedule()) )
    {
      model::ScheduleDay s = scheduleDay->clone(model).cast<model::ScheduleDay>();
      s.setName(name + " Week " + QString::number(i).toStdString() + " Winter");
      scheduleWeekClone.setWinterDesignDaySchedule(s);
    }

    if( (scheduleDay = scheduleWeek.customDay1Schedule()) )
    {
      model::ScheduleDay s = scheduleDay->clone(model).cast<model::ScheduleDay>();
      s.setName(name + " Week " + QString::number(i).toStdString() + " Custom 1");
      scheduleWeekClone.setCustomDay1Schedule(s);
    }

    if( (scheduleDay = scheduleWeek.customDay2Schedule()) )
    {
      model::ScheduleDay s = scheduleDay->clone(model).cast<model::ScheduleDay>();
      s.setName(name + " Week " + QString::number(i).toStdString() + " Custom 2");
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

model::Schedule ReverseTranslator::alwaysOnSchedule(openstudio::model::Model& model)
{
  if( ! m_alwaysOnSchedule )
  {
    model::ScheduleRuleset scheduleRuleset = model::ScheduleRuleset(model);

    scheduleRuleset.setName("Always On");

    model::ScheduleDay scheduleDay = scheduleRuleset.defaultDaySchedule();

    scheduleDay.addValue(Time(1.0),1.0);

    m_alwaysOnSchedule = scheduleRuleset;
  }

  return m_alwaysOnSchedule.get();
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

boost::optional<openstudio::model::ModelObject> ReverseTranslator::translateAirSystem(
                                                  const QDomElement& airSystemElement, 
                                                  const QDomDocument& doc,
                                                  openstudio::model::Model& model )
{
  boost::optional<openstudio::model::ModelObject> result;

  double value;

  if( ! istringEqual(airSystemElement.tagName().toStdString(),"AirSys") )
  {
    return result;
  }

  // Type
  QDomElement typeElement = airSystemElement.firstChildElement("Type");
  if( typeElement.text().compare("Exhaust",Qt::CaseInsensitive) == 0 )
  {
    return boost::none;
  }

  // Name
  QDomElement nameElement = airSystemElement.firstChildElement("Name");

  // Air System
  model::AirLoopHVAC airLoopHVAC(model);
  airLoopHVAC.setName(nameElement.text().toStdString());
  result = airLoopHVAC;
  model::Node supplyOutletNode = airLoopHVAC.supplyOutletNode();
  model::Node supplyInletNode = airLoopHVAC.supplyInletNode();

  QDomElement airSystemTypeElement = airSystemElement.firstChildElement("Type");

  QDomElement airSystemClgCtrlElement = airSystemElement.firstChildElement("ClgCtrl");

  QDomElement airHndlrAvailSchElement = airSystemElement.firstChildElement("AvailSchRef");

  // Availability Schedule
  boost::optional<model::Schedule> availabilitySchedule; 
  if( ! airHndlrAvailSchElement.isNull() )
  {
      availabilitySchedule = model.getModelObjectByName<model::Schedule>(airHndlrAvailSchElement.text().toStdString());
  }

  if( availabilitySchedule )
  {
    airLoopHVAC.setAvailabilitySchedule(availabilitySchedule.get());
  }

  QDomElement nightCycleFanCtrlElement = airSystemElement.firstChildElement("NightCycleFanCtrl");
  
  if( istringEqual(nightCycleFanCtrlElement.text().toStdString(),"CycleOnCallAnyZone") )
  {
    airLoopHVAC.setNightCycleControlType("CycleOnAny");
  }
  else if( istringEqual(nightCycleFanCtrlElement.text().toStdString(),"CycleOnCallPrimaryZone") )
  {
    airLoopHVAC.setNightCycleControlType("CycleOnAny");

    LOG(Warn,airLoopHVAC.name().get() << ": CycleOnAnyZoneFansOnly is not supported, using CycleOnAny.");
  }
  else if( istringEqual(nightCycleFanCtrlElement.text().toStdString(),"CycleZoneFansOnly") )
  {
    airLoopHVAC.setNightCycleControlType("CycleOnAnyZoneFansOnly");
  }

  // Adjust Sizing:System Object

  model::SizingSystem sizingSystem = airLoopHVAC.sizingSystem();

  // clgDsgnSupAirTemp
  double clgDsgnSupAirTemp = 12.8;
  QDomElement clgSupAirTempElement = airSystemElement.firstChildElement("ClgDsgnSupAirTemp");
  bool ok;
  value = clgSupAirTempElement.text().toDouble(&ok);
  if( ok )
  {
    clgDsgnSupAirTemp = unitToUnit(value,"F","C").get();
  }
  sizingSystem.setCentralCoolingDesignSupplyAirTemperature(clgDsgnSupAirTemp);

  // HtgDsgnSupAirTemp
  double htgDsgnSupAirTemp = 40.0;
  QDomElement htgSupAirTempElement = airSystemElement.firstChildElement("HtgDsgnSupAirTemp");
  value = htgSupAirTempElement.text().toDouble(&ok);
  if( ok )
  {
    htgDsgnSupAirTemp = unitToUnit(value,"F","C").get();
  }
  sizingSystem.setCentralHeatingDesignSupplyAirTemperature(htgDsgnSupAirTemp);

  // DsgnAirFlowMin
  QDomElement dsgnAirFlowMinElement = airSystemElement.firstChildElement("DsgnAirFlowMin");
  value = dsgnAirFlowMinElement.text().toDouble(&ok);
  if( ok )
  {
    sizingSystem.setMinimumSystemAirFlowRatio(value);
  }
  else
  {
    sizingSystem.setMinimumSystemAirFlowRatio(0.3);
  }

  // DsgnPrehtTemp

  QDomElement dsgnPrehtTempElement = airSystemElement.firstChildElement("DsgnPrehtTemp");

  value = dsgnPrehtTempElement.text().toDouble(&ok);

  if( ok )
  {
    sizingSystem.setPreheatDesignTemperature(unitToUnit(value,"F","C").get());
  }
  else
  {
    sizingSystem.setPreheatDesignTemperature(7.0);
  }

  // DsgnPrehtHumidityRat

  QDomElement dsgnPrehtHumidityRatElement = airSystemElement.firstChildElement("DsgnPrehtHumidityRat");

  value = dsgnPrehtHumidityRatElement.text().toDouble(&ok);

  if( ok )
  {
    sizingSystem.setPreheatDesignHumidityRatio(value);
  }
  else
  {
    sizingSystem.setPreheatDesignHumidityRatio(0.008);
  }

  // DsgnPreclTemp

  QDomElement dsgnPreclTempElement = airSystemElement.firstChildElement("DsgnPreclTemp");

  value = dsgnPreclTempElement.text().toDouble(&ok);

  if( ok )
  {
    sizingSystem.setPrecoolDesignTemperature(unitToUnit(value,"F","C").get());
  }
  else
  {
    sizingSystem.setPrecoolDesignTemperature(12.8);
  }

  // DsgnPreclHumidityRat

  QDomElement dsgnPreclHumidityRatElement = airSystemElement.firstChildElement("DsgnPreclHumidityRat");

  value = dsgnPreclHumidityRatElement.text().toDouble(&ok);

  if( ok )
  {
    sizingSystem.setPrecoolDesignHumidityRatio(value);
  }
  else
  {
    sizingSystem.setPrecoolDesignHumidityRatio(0.008);
  }

  // SizingOption

  QDomElement sizingOptionElement = airSystemElement.firstChildElement("SizingOption");

  if( sizingOptionElement.text().compare("NonCoincident",Qt::CaseInsensitive) == 0 )
  {
    sizingSystem.setSizingOption("NonCoincident");
  }
  else
  {
    sizingSystem.setSizingOption("Coincident");
  }

  // ClgFullOutsdAir

  QDomElement clgFullOutsdAirElement = airSystemElement.firstChildElement("ClgFullOutsdAir");

  if( clgFullOutsdAirElement.text().compare("Yes",Qt::CaseInsensitive) == 0 )
  {
    sizingSystem.setAllOutdoorAirinCooling(true);
  }
  else
  {
    sizingSystem.setAllOutdoorAirinCooling(false);
  }

  // HtgFullOutsdAir
  
  QDomElement htgFullOutsdAirElement = airSystemElement.firstChildElement("HtgFullOutsdAir");

  if( htgFullOutsdAirElement.text().compare("Yes",Qt::CaseInsensitive) == 0 )
  {
    sizingSystem.setAllOutdoorAirinHeating(true);
  }
  else
  {
    sizingSystem.setAllOutdoorAirinHeating(false);
  }

  // ClgDsgnHumidityRat 

  QDomElement clgDsgnHumidityRatElement = airSystemElement.firstChildElement("ClgDsgnHumidityRat");

  value = clgDsgnHumidityRatElement.text().toDouble(&ok);

  if( ok )
  {
    sizingSystem.setCentralCoolingDesignSupplyAirHumidityRatio(value);
  }
  else
  {
    sizingSystem.setCentralCoolingDesignSupplyAirHumidityRatio(0.008);
  }

  // HtgDsgnHumidityRat

  QDomElement htgDsgnHumidityRatElement = airSystemElement.firstChildElement("HtgDsgnHumidityRat");

  value = htgDsgnHumidityRatElement.text().toDouble(&ok);

  if( ok )
  {
    sizingSystem.setCentralHeatingDesignSupplyAirHumidityRatio(value);
  }
  else
  {
    sizingSystem.setCentralHeatingDesignSupplyAirHumidityRatio(0.008);
  }

  // FanPos

  bool isFanDrawthrough = false;
  QDomElement fanPosElement = airSystemElement.firstChildElement("FanPos");

  if( fanPosElement.text().compare("DrawThrough",Qt::CaseInsensitive) == 0 )
  {
    isFanDrawthrough = true;
  }

  // Save Evap Cooler to draw from return air
  boost::optional<model::EvaporativeCoolerIndirectResearchSpecial> indirectEvapUsingReturn;

  // Air Segments
  QDomNodeList airSegmentElements = airSystemElement.elementsByTagName("AirSeg");

  // Save the fan to add last
  boost::optional<model::Node> dropNode;

  for (int i = 0; i < airSegmentElements.count(); i++)
  {
    QDomElement airSegmentElement = airSegmentElements.at(i).toElement();

    QDomElement airSegmentTypeElement = airSegmentElement.firstChildElement("Type");

    // Supply Segments
    if(istringEqual(airSegmentTypeElement.text().toStdString(),"Supply"))
    {
      QDomNodeList airSegmentChildElements = airSegmentElement.childNodes();

      // Locate and add the fan first
      // This is so we don't mess up control nodes

      for(int j = 0; j != airSegmentChildElements.count(); j++)
      {
        QDomElement airSegmentChildElement = airSegmentChildElements.at(j).toElement();

        // Fan
        if( (istringEqual(airSegmentChildElement.tagName().toStdString(),"Fan")) )
        {
          if( boost::optional<model::ModelObject> mo = 
                translateFan(airSegmentChildElement,doc,model) )
          {
            if( boost::optional<model::StraightComponent> hvacComponent = 
                  mo->optionalCast<model::StraightComponent>() )
            {
              // Determine fan position

              QDomElement posElement = airSegmentChildElement.firstChildElement("Pos");

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

      for(int j = (airSegmentChildElements.count() - 1); j > -1;  j--)
      {
        QDomElement airSegmentChildElement = airSegmentChildElements.at(j).toElement();

        boost::optional<model::ModelObject> lastComponent = boost::none;
        
        // CoilCooling
        if( istringEqual(airSegmentChildElement.tagName().toStdString(),"CoilClg") )
        {
          if( boost::optional<model::ModelObject> mo = 
                translateCoilCooling(airSegmentChildElement,doc,model) )
          {
            OS_ASSERT(mo);

            lastComponent = mo;

            model::HVACComponent hvacComponent = mo->cast<model::HVACComponent>();

            hvacComponent.addToNode(dropNode.get());

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
        else if( istringEqual(airSegmentChildElement.tagName().toStdString(),"CoilHtg") )
        {
          boost::optional<model::ModelObject> mo = translateCoilHeating(airSegmentChildElement,doc,model);

          OS_ASSERT(mo);

          lastComponent = mo;

          model::HVACComponent hvacComponent = mo->cast<model::HVACComponent>();

          hvacComponent.addToNode(dropNode.get());

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
        // EvapClr
        else if( istringEqual(airSegmentChildElement.tagName().toStdString(),"EvapClr") )
        {
          if( boost::optional<model::ModelObject> mo = 
                translateEvapClr(airSegmentChildElement,doc,model) )
          {
            OS_ASSERT(mo);

            lastComponent = mo;

            model::HVACComponent hvacComponent = mo->cast<model::HVACComponent>();

            hvacComponent.addToNode(dropNode.get());

            if( boost::optional<model::EvaporativeCoolerIndirectResearchSpecial> evap = hvacComponent.optionalCast<model::EvaporativeCoolerIndirectResearchSpecial>() )
            {
              QDomElement secAirSrcElement = airSegmentChildElement.firstChildElement();
              if(secAirSrcElement.text().compare("Return",Qt::CaseInsensitive) == 0)
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
  QDomElement airSystemOACtrlElement = airSystemElement.firstChildElement("OACtrl");
  boost::optional<model::AirLoopHVACOutdoorAirSystem> oaSystem;

  if( ! airSystemOACtrlElement.isNull() )
  {
    oaSystem = airLoopHVAC.airLoopHVACOutdoorAirSystem();
    if( ! oaSystem )
    {
      model::ControllerOutdoorAir oaController(model);

      model::AirLoopHVACOutdoorAirSystem newOASystem(model,oaController);

      newOASystem.addToNode(supplyInletNode);

      // Name
      QDomElement nameElement = airSystemOACtrlElement.firstChildElement("Name");
      if( ! nameElement.text().isEmpty() )
      {
        oaController.setName(nameElement.text().toStdString());
        newOASystem.setName(nameElement.text().toStdString() + " OA System");
        oaController.controllerMechanicalVentilation().setName(nameElement.text().toStdString() + " Mech Vent Controller");
      }

      // MinOAFracSchRef
      QDomElement minOAFracSchRefElement = airSystemOACtrlElement.firstChildElement("MinOAFracSchRef");
      if( boost::optional<model::Schedule> schedule = 
          model.getModelObjectByName<model::Schedule>(minOAFracSchRefElement.text().toStdString()) )
      {
        oaController.setMinimumFractionofOutdoorAirSchedule(schedule.get());
      }
      
      // MaxOAFracSchRef
      QDomElement maxOAFracSchRefElement = airSystemOACtrlElement.firstChildElement("MaxOAFracSchRef");
      if( boost::optional<model::Schedule> schedule = 
          model.getModelObjectByName<model::Schedule>(maxOAFracSchRefElement.text().toStdString()) ) {
        oaController.setMaximumFractionofOutdoorAirSchedule(schedule.get());
      } else {
        // MaxOARat
        QDomElement maxOARatElement = airSystemOACtrlElement.firstChildElement("MaxOARat");
        bool ok;
        double maxOARat = maxOARatElement.text().toDouble(&ok);
        if( ok && maxOARat > 0.0 && maxOARat <= 1.0 )
        {
          model::ScheduleRuleset maxOARatSchedule(model);
          maxOARatSchedule.setName(oaController.name().get() + " Max OA Schedule");
          model::ScheduleDay scheduleDay = maxOARatSchedule.defaultDaySchedule();
          scheduleDay.addValue(Time(1.0),maxOARat);
          oaController.setMaximumFractionofOutdoorAirSchedule(maxOARatSchedule);
        }
      }

      // OASchMthd 
      QDomElement oaSchMthdElement = airSystemOACtrlElement.firstChildElement("OASchMthd");
      if( istringEqual(oaSchMthdElement.text().toStdString(),"Constant") )
      {
        model::Schedule schedule = alwaysOnSchedule(model);
        oaController.setMinimumOutdoorAirSchedule(schedule);
      }
      else if( istringEqual(oaSchMthdElement.text().toStdString(),"FollowHVACAvailability") && availabilitySchedule )
      {
        if( boost::optional<model::ScheduleYear> availabilityScheduleYear = availabilitySchedule->optionalCast<model::ScheduleYear>() )
        {
          model::ScheduleYear schedule = deepScheduleYearClone(availabilityScheduleYear.get(),nameElement.text().toStdString() + " Schedule");

          int startOffset = 0;
          int offsetValue;

          QDomElement availSchOffsetStartElement = airSystemOACtrlElement.firstChildElement("StartUpDelay");

          offsetValue = availSchOffsetStartElement.text().toInt(&ok);
      
          if( ok )
          {
            startOffset = offsetValue;
          }

          adjustSchedule(schedule,startOffset);

          oaController.setMinimumOutdoorAirSchedule(schedule);
        }
      }
      else if( istringEqual(oaSchMthdElement.text().toStdString(),"Scheduled") )
      {
        QDomElement oaSchRefElement = airSystemOACtrlElement.firstChildElement("OASchRef");

        boost::optional<model::Schedule> schedule; 
        schedule = model.getModelObjectByName<model::Schedule>(oaSchRefElement.text().toStdString());

        if( schedule )
        {
          oaController.setMinimumOutdoorAirSchedule(schedule.get());
        }
      }

      // EconoCtrlMthd
      QDomElement econoCtrlMthdElement = airSystemOACtrlElement.firstChildElement("EconoCtrlMthd");

      if(istringEqual(econoCtrlMthdElement.text().toStdString(),"FixedDryBulb"))
      {
        oaController.setEconomizerControlType("FixedDryBulb");
      }
      else if(istringEqual(econoCtrlMthdElement.text().toStdString(),"DifferentialDryBulb"))
      {
        oaController.setEconomizerControlType("DifferentialDryBulb");
      }
      else if(istringEqual(econoCtrlMthdElement.text().toStdString(),"DifferentialEnthalpy"))
      {
        oaController.setEconomizerControlType("DifferentialEnthalpy");
      }
      else if(istringEqual(econoCtrlMthdElement.text().toStdString(),"DifferentialDryBulbAndEnthalpy"))
      {
        oaController.setEconomizerControlType("DifferentialEnthalpy");
      }

      // EconoIntegration
      QDomElement econoIntegrationElement = airSystemOACtrlElement.firstChildElement("EconoIntegration");
      if( istringEqual(econoIntegrationElement.text().toStdString(),"Integrated") )
      {
        oaController.setLockoutType("NoLockout");
      }
      else if( istringEqual(econoIntegrationElement.text().toStdString(),"NonIntegrated") )
      {
        oaController.setLockoutType("LockoutWithCompressor");
      }

      // EconoHiTempLockout
      QDomElement econoHiTempLockoutElement = airSystemOACtrlElement.firstChildElement("EconoHiTempLockout");

      value = econoHiTempLockoutElement.text().toDouble(&ok);

      if( ok ) 
      {
        Quantity valueIP(value,createFahrenheitTemperature());
        OptionalQuantity valueSI = QuantityConverter::instance().convert(valueIP, UnitSystem(UnitSystem::Celsius));
        OS_ASSERT(valueSI);
        oaController.setEconomizerMaximumLimitDryBulbTemperature(valueSI->value());
      }
      else
      {
        oaController.resetEconomizerMaximumLimitDryBulbTemperature();
      }

      // EconolowTempLockout
      QDomElement econoLowTempLockoutElement = airSystemOACtrlElement.firstChildElement("EconoLowTempLockout");

      value = econoLowTempLockoutElement.text().toDouble(&ok);

      if( ok ) 
      {
        Quantity valueIP(value,createFahrenheitTemperature());
        OptionalQuantity valueSI = QuantityConverter::instance().convert(valueIP, UnitSystem(UnitSystem::Celsius));
        OS_ASSERT(valueSI);
        oaController.setEconomizerMinimumLimitDryBulbTemperature(valueSI->value());
      }
      else
      {
        oaController.resetEconomizerMinimumLimitDryBulbTemperature();
      }

      // TODO Remove this in the future, but for now play it safe and use legacy OpenStudio default.
      if( (typeElement.text().compare("SZVAVAC",Qt::CaseInsensitive) == 0) ||
          (typeElement.text().compare("SZVAVHP",Qt::CaseInsensitive) ==0) ) {
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
  }

  for (int i = 0; i < airSegmentElements.count(); i++)
  {
    QDomElement airSegmentElement = airSegmentElements.at(i).toElement();
    QDomElement airSegmentTypeElement = airSegmentElement.firstChildElement("Type");

    if(istringEqual(airSegmentTypeElement.text().toStdString(),"Return"))
    {
      QDomElement fanElement = airSegmentElement.firstChildElement("Fan");
      if( ! fanElement.isNull() )
      {
        if( boost::optional<model::ModelObject> mo = translateFan(fanElement,doc,model) )
        {
          boost::optional<model::HVACComponent> fan = mo->optionalCast<model::HVACComponent>();
          OS_ASSERT(fan);
          fan->addToNode(supplyInletNode);
        }
      }
    }
    else if(istringEqual(airSegmentTypeElement.text().toStdString(),"Relief"))
    {
      QDomElement fanElement = airSegmentElement.firstChildElement("Fan");
      if( ! fanElement.isNull() )
      {
        if( boost::optional<model::ModelObject> mo = translateFan(fanElement,doc,model) )
        {
          if( oaSystem )
          {
            if( boost::optional<model::Node> outboardReliefNode = oaSystem->outboardReliefNode() )
            {
              boost::optional<model::HVACComponent> fan = mo->optionalCast<model::HVACComponent>();
              OS_ASSERT(fan);
              fan->addToNode(outboardReliefNode.get());
            }
          } 
        }
      }
    }
  }

  // Add Setpoint managers

  QDomElement clgCtrlElement = airSystemElement.firstChildElement("ClgCtrl");

  // Establish deck temperature

  boost::optional<model::HVACComponent> deckSPM;

  if( istringEqual(clgCtrlElement.text().toStdString(),"Fixed") )
  {
    model::ScheduleRuleset schedule(model);

    schedule.setName(airLoopHVAC.name().get() + " Supply Air Temp Schedule");

    QDomElement clgFixedSupTempElement = airSystemElement.firstChildElement("ClgFixedSupTemp");

    value = clgFixedSupTempElement.text().toDouble(&ok);

    if( ok )
    {
      model::ScheduleDay scheduleDay = schedule.defaultDaySchedule();

      value = unitToUnit(value,"F","C").get();

      scheduleDay.addValue(Time(1.0),value);
    }
    else
    {
      model::ScheduleDay scheduleDay = schedule.defaultDaySchedule();

      scheduleDay.addValue(Time(1.0),12.8);
    }

    model::SetpointManagerScheduled spm(model,schedule);

    deckSPM = spm;

    spm.addToNode(supplyOutletNode);
  }
  else if(istringEqual(clgCtrlElement.text().toStdString(),"NoSATControl"))
  {
    model::SetpointManagerSingleZoneReheat spm(model);

    deckSPM = spm;

    spm.addToNode(supplyOutletNode);

    if( istringEqual("SZVAVAC",airSystemTypeElement.text().toStdString()) || 
        istringEqual("SZVAVHP",airSystemTypeElement.text().toStdString()) ) 
    {
      spm.setMaximumSupplyAirTemperature(htgDsgnSupAirTemp);
      spm.setMinimumSupplyAirTemperature(clgDsgnSupAirTemp);
    }
  }
  else if( istringEqual(clgCtrlElement.text().toStdString(),"WarmestResetFlowFirst") )
  {
    model::SetpointManagerWarmestTemperatureFlow spm(model);
    spm.setStrategy("FlowFirst");
    deckSPM = spm;
    spm.addToNode(supplyOutletNode);

    QDomElement clRstSupHiElement = airSystemElement.firstChildElement("ClRstSupHi");
    value = clRstSupHiElement.text().toDouble(&ok);
    if( ok ) {
      value = unitToUnit(value,"F","C").get();
      spm.setMaximumSetpointTemperature(value);
    }

    QDomElement clRstSupLowElement = airSystemElement.firstChildElement("ClRstSupLow");
    value = clRstSupLowElement.text().toDouble(&ok);
    if( ok ) {
      value = unitToUnit(value,"F","C").get();
      spm.setMinimumSetpointTemperature(value);
    }

    auto dsgnAirFlowMinElement = airSystemElement.firstChildElement("DsgnAirFlowMin");
    value = dsgnAirFlowMinElement.text().toDouble(&ok);
    if( ok ) {
      spm.setMinimumTurndownRatio(value);
    }
  }
  else if( istringEqual(clgCtrlElement.text().toStdString(),"WarmestResetTemperatureFirst") )
  {
    model::SetpointManagerWarmestTemperatureFlow spm(model);
    spm.setStrategy("TemperatureFirst");
    deckSPM = spm;
    spm.addToNode(supplyOutletNode);

    QDomElement clRstSupHiElement = airSystemElement.firstChildElement("ClRstSupHi");
    value = clRstSupHiElement.text().toDouble(&ok);
    if( ok ) {
      value = unitToUnit(value,"F","C").get();
      spm.setMaximumSetpointTemperature(value);
    }

    QDomElement clRstSupLowElement = airSystemElement.firstChildElement("ClRstSupLow");
    value = clRstSupLowElement.text().toDouble(&ok);
    if( ok ) {
      value = unitToUnit(value,"F","C").get();
      spm.setMinimumSetpointTemperature(value);
    }

    auto dsgnAirFlowMinElement = airSystemElement.firstChildElement("DsgnAirFlowMin");
    value = dsgnAirFlowMinElement.text().toDouble(&ok);
    if( ok ) {
      spm.setMinimumTurndownRatio(value);
    }
  }
  else if( istringEqual(clgCtrlElement.text().toStdString(),"WarmestReset") )
  {
    model::SetpointManagerWarmest spm(model);
    deckSPM = spm;
    spm.addToNode(supplyOutletNode);

    QDomElement clRstSupHiElement = airSystemElement.firstChildElement("ClRstSupHi");
    value = clRstSupHiElement.text().toDouble(&ok);
    if( ok ) {
      value = unitToUnit(value,"F","C").get();
      spm.setMaximumSetpointTemperature(value);
    }

    QDomElement clRstSupLowElement = airSystemElement.firstChildElement("ClRstSupLow");
    value = clRstSupLowElement.text().toDouble(&ok);
    if( ok ) {
      value = unitToUnit(value,"F","C").get();
      spm.setMinimumSetpointTemperature(value);
    }
  }
  else if( istringEqual(clgCtrlElement.text().toStdString(),"Scheduled") )
  {
    QDomElement clgSetPtSchRefElement = airSystemElement.firstChildElement("ClgSetptSchRef");

    boost::optional<model::Schedule> schedule = model.getModelObjectByName<model::Schedule>(clgSetPtSchRefElement.text().toStdString());

    if( ! schedule )
    {
      model::ScheduleRuleset schedule(model);

      schedule.setName(airLoopHVAC.name().get() + " Supply Air Temp Schedule");

      model::ScheduleDay scheduleDay = schedule.defaultDaySchedule();

      scheduleDay.addValue(Time(1.0),12.8);
    }

    model::SetpointManagerScheduled spm(model,schedule.get());

    deckSPM = spm;

    spm.addToNode(supplyOutletNode);
  }
  else if( istringEqual(clgCtrlElement.text().toStdString(),"OutsideAirReset") )
  {
    model::SetpointManagerOutdoorAirReset spm(model);

    deckSPM = spm;

    spm.addToNode(supplyOutletNode);

    boost::optional<double> rstSupHi;
    boost::optional<double> rstSupLow;
    boost::optional<double> rstOutdrHi;
    boost::optional<double> rstOutdrLow;

    QDomElement clRstSupHiElement = airSystemElement.firstChildElement("ClRstSupHi");
    value = clRstSupHiElement.text().toDouble(&ok);
    if( ok )
    {
      rstSupHi = unitToUnit(value,"F","C").get();
    }

    QDomElement clRstSupLowElement = airSystemElement.firstChildElement("ClRstSupLow");
    value = clRstSupLowElement.text().toDouble(&ok);
    if( ok )
    {
      rstSupLow = unitToUnit(value,"F","C").get();
    }

    QDomElement clRstOutdrHiElement = airSystemElement.firstChildElement("ClRstOutdrHi");
    value = clRstOutdrHiElement.text().toDouble(&ok);
    if( ok )
    {
      rstOutdrHi = unitToUnit(value,"F","C").get();
    }

    QDomElement clRstOutdrLowElement = airSystemElement.firstChildElement("ClRstOutdrLow");
    value = clRstOutdrLowElement.text().toDouble(&ok);
    if( ok )
    {
      rstOutdrLow = unitToUnit(value,"F","C").get();
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
      LOG(Warn,nameElement.text().toStdString() << " requests OA reset control, but does not define setpoints."
        << "  Using OpenStudio defaults.");
      
      spm.setOutdoorLowTemperature(10.0);
      spm.setSetpointatOutdoorLowTemperature(22.0);
      spm.setOutdoorHighTemperature(24.0);
      spm.setSetpointatOutdoorHighTemperature(10.0);
    }
  }

  // Mixed air setpoint managers

  if( isFanDrawthrough )
  {
    std::vector<model::ModelObject> supplyNodes = airLoopHVAC.supplyComponents(model::Node::iddObjectType());

    for( const auto & supplyNode : supplyNodes)
    {
      if( supplyNode != airLoopHVAC.supplyInletNode() &&
          supplyNode != airLoopHVAC.supplyOutletNode() )
      {
        model::Node node = supplyNode.cast<model::Node>();

        model::SetpointManagerMixedAir spm(model);

        spm.addToNode(node);
      }
    }
  }
  else
  {
    if( deckSPM )
    {
      deckSPM->setName(airLoopHVAC.name().get() + " Deck SPM");
      std::vector<model::ModelObject> supplyNodes;

      if( boost::optional<model::Node> mixedAirNode = airLoopHVAC.mixedAirNode() )
      {
        supplyNodes = airLoopHVAC.supplyComponents(mixedAirNode.get(),supplyOutletNode,model::Node::iddObjectType());

        model::SetpointManagerMixedAir spm(model);

        spm.addToNode(mixedAirNode.get());
      }
      else
      {
        supplyNodes = airLoopHVAC.supplyComponents(supplyInletNode,supplyOutletNode,model::Node::iddObjectType());
      }

      if( supplyNodes.size() > 2 )
      {
        supplyNodes.erase(supplyNodes.begin());

        supplyNodes.erase(supplyNodes.end() - 1);

        for( const auto & supplyNode : supplyNodes)
        {
          model::Node node = supplyNode.cast<model::Node>();

          model::HVACComponent spmClone = deckSPM->clone(model).cast<model::HVACComponent>();

          spmClone.addToNode(node);
        }
      }
    }
  }

  //// Give the nodes better names
  //std::vector<model::ModelObject> comps = airLoopHVAC.supplyComponents();
  //for( std::vector<model::ModelObject>::iterator it = comps.begin();
  //     it != comps.end();
  //     ++it )
  //{
  //  boost::optional<model::Node> node = it->optionalCast<model::Node>();
  //  if( node && 
  //      (it->handle() != supplyInletNode.handle()) )
  //  {
  //    model::ModelObject last = *(--it);
  //    it->setName(last.name().get() + " Outlet Node");
  //  }
  //}

  //supplyInletNode.setName(airLoopHVAC.name().get() + " Supply Inlet Node");
  //airLoopHVAC.demandInletNode().setName(airLoopHVAC.name().get() + " Demand Inlet Node");
  //airLoopHVAC.demandOutletNode().setName(airLoopHVAC.name().get() + " Demand Outlet Node");

  //std::vector<model::ThermalZone> zones = subsetCastVector<model::ThermalZone>(airLoopHVAC.demandComponents());
  //for(std::vector<model::ThermalZone>::iterator it = zones.begin();
  //    it != zones.end();
  //    ++it)
  //{
  //  std::string baseName = it->name().get();
  //  if( boost::optional<model::ModelObject> mo = it->returnAirModelObject() )
  //  {
  //    mo->setName(baseName + " Outlet Node");
  //  }

  //  int i = 1;
  //  std::vector<model::ModelObject> inletModelObjects = it->inletPortList().modelObjects();
  //  for(std::vector<model::ModelObject>::iterator inletIt = inletModelObjects.begin();
  //      inletIt != inletModelObjects.end();
  //      ++inletIt)
  //  {
  //    inletIt->setName(it->name().get() + " Inlet Node " + boost::lexical_cast<std::string>(i));
  //  }

  //  i = 1;
  //  std::vector<model::ModelObject> exhaustModelObjects = it->exhaustPortList().modelObjects();
  //  for(std::vector<model::ModelObject>::iterator exhaustIt = exhaustModelObjects.begin();
  //      exhaustIt != exhaustModelObjects.end();
  //      ++exhaustIt)
  //  {
  //    exhaustIt->setName(it->name().get() + " Exhaust Node " + boost::lexical_cast<std::string>(i));
  //  }
  //}
  
  //model::Splitter splitter = airLoopHVAC.demandSplitter();
  //std::vector<model::ModelObject> branchOutletObjects = splitter.outletModelObjects();
  //int i = 1;
  //for( std::vector<model::ModelObject>::iterator it = branchOutletObjects.begin();
  //     it != branchOutletObjects.end();
  //     ++it )
  //{
  //  it->setName(splitter.name().get() + " " + boost::lexical_cast<std::string>(i));
  //  ++i;
  //}

  return result;
}

boost::optional<openstudio::model::ModelObject> ReverseTranslator::translateCoilHeating(
  const QDomElement& heatingCoilElement, 
  const QDomDocument& doc, 
  openstudio::model::Model& model)
{
  boost::optional<model::ModelObject> result;
  bool ok;
  double value;

  if( ! istringEqual(heatingCoilElement.tagName().toStdString(),"CoilHtg") )
  {
    return result;
  }

  bool znSys = false;
  if( heatingCoilElement.parentNode().toElement().tagName().compare("ZnSys",Qt::CaseInsensitive) == 0 )
  {
    znSys = true;
  }

  // Type
  QDomElement coilHeatingTypeElement = heatingCoilElement.firstChildElement("Type");
  QDomElement nameElement = heatingCoilElement.firstChildElement("Name");
  QDomElement capTotRtdElement = heatingCoilElement.firstChildElement("CapTotGrossRtdSim");

  // CapTotGrossRtd
  boost::optional<double> capTotGrossRtd;
  
  if( ! autosize() )
  {
    value = capTotRtdElement.text().toDouble(&ok);

    if( ok ) 
    {
      Quantity valueIP(value,createBTUPower());
      OptionalQuantity valueSI = QuantityConverter::instance().convert(valueIP, UnitSystem(UnitSystem::SI));
      OS_ASSERT(valueSI);
      capTotGrossRtd = valueSI->value();
    }
  }

  // Flow Capacity
  // Look for a sibling fan to figure out what the flow capacity should be
  boost::optional<double> flowCap;

  if( ! autosize() )
  {
    QDomElement flowCapElement;

    QDomElement airSegElement = heatingCoilElement.parentNode().toElement();

    if( ! airSegElement.isNull() )
    {
      QDomElement fanElement = airSegElement.firstChildElement("Fan");

      if( ! fanElement.isNull() )
      {
        flowCapElement = fanElement.firstChildElement("FlowCapSim");

        value = flowCapElement.text().toDouble(&ok);

        if( ok )
        {
          flowCap = unitToUnit(value,"cfm","m^3/s").get();
        }
      }
    }
  }

  // Furnace
  if( istringEqual(coilHeatingTypeElement.text().toStdString(),"Furnace") )
  {
    model::Schedule schedule = alwaysOnSchedule(model);

    model::CoilHeatingGas coil(model,schedule);

    coil.setName(nameElement.text().toStdString());

    // CapTotGrossRtd
    if( capTotGrossRtd )
    {
      coil.setNominalCapacity(capTotGrossRtd.get());
    }

    // FurnHIR_fPLRCrvRef
    boost::optional<model::Curve> hirCurve;
    QDomElement hirCurveElement = 
      heatingCoilElement.firstChildElement("FurnHIR_fPLRCrvRef");
    hirCurve = 
      model.getModelObjectByName<model::Curve>(
        hirCurveElement.text().toStdString());
    if( hirCurve )
    {
      coil.setPartLoadFractionCorrelationCurve(hirCurve.get());
    }

    // FurnThrmlEff
    QDomElement furnThrmlEffElement = heatingCoilElement.firstChildElement("FurnThrmlEff");
    value = furnThrmlEffElement.text().toDouble(&ok);
    if( ok )
    {
      coil.setGasBurnerEfficiency(value);
    }

    // FurnPilotFuelInp
    QDomElement furnPilotFuelInpElement = heatingCoilElement.firstChildElement("FurnPilotFuelInp");
    value = furnPilotFuelInpElement.text().toDouble(&ok);
    if( ok )
    {
      value = unitToUnit(value,"Btu/h","W").get();
      coil.setParasiticGasLoad(value);
    }

    result = coil;
  }
  else if( istringEqual(coilHeatingTypeElement.text().toStdString(),"HotWater") )
  {
    model::Schedule schedule = model.alwaysOnDiscreteSchedule();

    model::CoilHeatingWater coil(model,schedule);

    coil.setName(nameElement.text().toStdString());

    QDomElement fluidSegInRefElement = heatingCoilElement.firstChildElement("FluidSegInRef");

    boost::optional<model::PlantLoop> plant = loopForSupplySegment(fluidSegInRefElement.text(),doc,model);

    if( plant )
    {
      plant->addDemandBranchForComponent(coil);
    }

    if( capTotGrossRtd )
    {
      coil.setPerformanceInputMethod("NominalCapacity");

      coil.setRatedCapacity(capTotGrossRtd.get());

      coil.setRatedInletWaterTemperature(82.2);

      coil.setRatedInletAirTemperature(16.6);

      coil.setRatedOutletWaterTemperature(71.1);

      coil.setRatedOutletAirTemperature(32.2);

      // Find related/containing systems (aka figure out the context of the coil)

      QDomElement sysElement;

      QDomElement airSysElement = heatingCoilElement.parentNode().parentNode().toElement();

      QDomElement znSysElement = heatingCoilElement.parentNode().toElement();

      if( airSysElement.tagName().compare("AirSys",Qt::CaseInsensitive) == 0 )
      {
        sysElement = airSysElement;
      }
      else if( znSysElement.tagName().compare("ZnSys",Qt::CaseInsensitive) == 0 )
      {
        sysElement = znSysElement;
      }

      //QDomElement htgDsgnSupAirTempElement = sysElement.firstChildElement("HtgDsgnSupAirTemp");

      //value = htgDsgnSupAirTempElement.text().toDouble(&ok);

      //if( ok )
      //{
      //  coil.setRatedOutletAirTemperature(unitToUnit(value,"F","C").get());
      //}

      //if( plant )
      //{
      //  model::SizingPlant sizingPlant = plant->sizingPlant();

      //  coil.setRatedInletWaterTemperature(sizingPlant.designLoopExitTemperature());

      //  coil.setRatedOutletWaterTemperature(sizingPlant.designLoopExitTemperature() - sizingPlant.loopDesignTemperatureDifference());
      //}
    }

    result = coil;
  }
  else if( istringEqual(coilHeatingTypeElement.text().toStdString(),"Resistance") )
  {
    model::Schedule schedule = alwaysOnSchedule(model);

    model::CoilHeatingElectric coil(model,schedule);

    coil.setName(nameElement.text().toStdString());

    // CapTotGrossRtd
    if( capTotGrossRtd )
    {
      coil.setNominalCapacity(capTotGrossRtd.get());
    }

    result = coil;
  }
  else if( istringEqual(coilHeatingTypeElement.text().toStdString(),"HeatPump") )
  {
    QDomElement cndsrTypeElement = heatingCoilElement.firstChildElement("CndsrType");
    if(cndsrTypeElement.text().compare("WaterSource",Qt::CaseInsensitive) == 0)
    {
      model::CoilHeatingWaterToAirHeatPumpEquationFit coil(model);
      coil.setName(nameElement.text().toStdString());
      boost::optional<model::AirLoopHVACUnitarySystem> unitary;
      if( znSys )
      {
        result = coil;
      }
      else
      {
        unitary = model::AirLoopHVACUnitarySystem(model);
        unitary->setName(nameElement.text().toStdString() + " Unitary");
        unitary->getImpl<model::detail::AirLoopHVACUnitarySystem_Impl>()->setControlType("SetPoint");
        unitary->setHeatingCoil(coil);
        result = unitary.get();
      }

      // Plant
      QDomElement fluidSegNameElement = heatingCoilElement.firstChildElement("FluidSegInRef");
      boost::optional<model::PlantLoop> plant = loopForSupplySegment(fluidSegNameElement.text(),doc,model);
      if( plant )
      {
        plant->addDemandBranchForComponent(coil);
      }

      if( ! autosize() )
      {
        // Look for a sibling fan to figure out what the flow capacity should be
        QDomElement flowCapElement;
        QDomElement airSegElement = heatingCoilElement.parentNode().toElement();
        if( ! airSegElement.isNull() )
        {
          QDomElement fanElement = airSegElement.firstChildElement("Fan");
          if( ! fanElement.isNull() )
          {
            flowCapElement = fanElement.firstChildElement("FlowCapSim");
          }
        }

        value = flowCapElement.text().toDouble(&ok);
        if( ok ) 
        {
          value = unitToUnit(value,"cfm","m^3/s").get();
          coil.setRatedAirFlowRate(value);
          if( unitary )
          {
            unitary->setSupplyAirFlowRateDuringHeatingOperation(value);
          }
        }
      }

      if( ! autosize() )
      {
        QDomElement fluidFlowRtDsgnSimElement = heatingCoilElement.firstChildElement("FluidFlowRtDsgnSim");
        value = fluidFlowRtDsgnSimElement.text().toDouble(&ok);
        if( ok )
        {
          value = unitToUnit(value,"gal/min","m^3/s").get();
          coil.setRatedWaterFlowRate(value);
        }
      }

      if( ! autosize() )
      {
        QDomElement capTotRtdSimElement = heatingCoilElement.firstChildElement("CapTotGrossRtdSim");
        value = capTotRtdSimElement.text().toDouble(&ok);
        if( ok )
        {
          value = unitToUnit(value,"Btu/h","W").get();
          coil.setRatedHeatingCapacity(value);
        }
      }

      QDomElement htPumpCOPElement = heatingCoilElement.firstChildElement("HtPumpCOP");
      value = htPumpCOPElement.text().toDouble(&ok);
      if( ok )
      {
        coil.setRatedHeatingCoefficientofPerformance(value);
      }

      coil.setHeatingCapacityCoefficient1(-5.50102734);
      coil.setHeatingCapacityCoefficient2(-0.96688754);
      coil.setHeatingCapacityCoefficient3(7.70755007);
      coil.setHeatingCapacityCoefficient4(0.031928881);
      coil.setHeatingCapacityCoefficient5(0.028112522);

      coil.setHeatingPowerConsumptionCoefficient1(-7.47517858);
      coil.setHeatingPowerConsumptionCoefficient2(6.40876653);
      coil.setHeatingPowerConsumptionCoefficient3(1.99711665);
      coil.setHeatingPowerConsumptionCoefficient4(-0.050682973);
      coil.setHeatingPowerConsumptionCoefficient5(0.011385145);
    }
    else
    {
      // HtPumpCap_fTempCrvRef
      boost::optional<model::Curve> totalHeatingCapacityFunctionofTemperatureCurve;
      QDomElement totalHeatingCapacityFunctionofTemperatureCurveElement = 
        heatingCoilElement.firstChildElement("HtPumpCap_fTempCrvRef");
      totalHeatingCapacityFunctionofTemperatureCurve = 
        model.getModelObjectByName<model::Curve>(
          totalHeatingCapacityFunctionofTemperatureCurveElement.text().toStdString());

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
      QDomElement totalHeatingCapacityFunctionofFlowFractionCurveElement = 
        heatingCoilElement.firstChildElement("HtPumpCap_fFlowCrvRef");
      totalHeatingCapacityFunctionofFlowFractionCurve = 
        model.getModelObjectByName<model::Curve>(
          totalHeatingCapacityFunctionofFlowFractionCurveElement.text().toStdString());

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
      QDomElement energyInputRatioFunctionofTemperatureCurveElement = 
        heatingCoilElement.firstChildElement("HtPumpEIR_fTempCrvRef");
      energyInputRatioFunctionofTemperatureCurve = 
        model.getModelObjectByName<model::Curve>(
          energyInputRatioFunctionofTemperatureCurveElement.text().toStdString());

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
      QDomElement energyInputRatioFunctionofFlowFractionCurveElement = 
        heatingCoilElement.firstChildElement("HtPumpEIR_fFlowCrvRef");
      energyInputRatioFunctionofFlowFractionCurve = 
        model.getModelObjectByName<model::Curve>(
          energyInputRatioFunctionofFlowFractionCurveElement.text().toStdString());

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
      QDomElement partLoadFractionCorrelationCurveElement = 
        heatingCoilElement.firstChildElement("HtPumpEIR_fPLFCrvRef");
      partLoadFractionCorrelationCurve = 
        model.getModelObjectByName<model::Curve>(
          partLoadFractionCorrelationCurveElement.text().toStdString());

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
      heatingCoil.setName(nameElement.text().toStdString());

      // FlowCapSim
      if( flowCap )
      {
        heatingCoil.setRatedAirFlowRate(flowCap.get());
      }

      // CapTotGrossRtdSim
      if( capTotGrossRtd )
      {
        heatingCoil.setRatedTotalHeatingCapacity(capTotGrossRtd.get());
      }

      // HtPumpEIR
      QDomElement htPumpEIRElement = heatingCoilElement.firstChildElement("HtPumpEIR");
      value = htPumpEIRElement.text().toDouble(&ok);
      if( ok ) 
      { 
        heatingCoil.setRatedCOP(1.0 / value);
      } 

      // HtPumpCprsrLockoutTemp
      QDomElement htPumpCprsrLockoutTempElement = heatingCoilElement.firstChildElement("HtPumpCprsrLockoutTemp");
      value = htPumpCprsrLockoutTempElement.text().toDouble(&ok);
      if( ok )
      {
        value = unitToUnit(value,"F","C").get();
        heatingCoil.setMinimumOutdoorDryBulbTemperatureforCompressorOperation(value);
      }

      // HtPumpCrankcaseHtCap
      QDomElement htPumpCrankcaseHtCapElement = heatingCoilElement.firstChildElement("HtPumpCrankcaseHtrCapSim");
      value = htPumpCrankcaseHtCapElement.text().toDouble(&ok);
      if( ok )
      {
        value = unitToUnit(value,"Btu/h","W").get();
        heatingCoil.setCrankcaseHeaterCapacity(value);
      }

      // HtPumpCrankcaseCtrlTemp
      QDomElement htPumpCrankcaseCtrlTempElement = heatingCoilElement.firstChildElement("HtPumpCrankcaseCtrlTemp");
      value = htPumpCrankcaseCtrlTempElement.text().toDouble(&ok);
      if( ok )
      {
        value = unitToUnit(value,"F","C").get();
        heatingCoil.setMaximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation(value);
      }

      // HtPumpDefHtSrc
      QDomElement htPumpDefHtSrcElement = heatingCoilElement.firstChildElement("HtPumpDefHtSrc");
      if( istringEqual(htPumpDefHtSrcElement.text().toStdString(),"Electric") )
      {
        heatingCoil.setDefrostStrategy("Resistive");
      }
      else if( istringEqual(htPumpDefHtSrcElement.text().toStdString(),"HotGas") )
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
        QDomElement htPumpDefHtCapElement = heatingCoilElement.firstChildElement("HtPumpDefHtrCapSim");
        value = htPumpDefHtCapElement.text().toDouble(&ok);
        if( ok )
        {
          value = unitToUnit(value,"Btu/h","W").get();
          heatingCoil.setResistiveDefrostHeaterCapacity(value);
        }
      }
      else
      {
        heatingCoil.autosizeResistiveDefrostHeaterCapacity();
      }

      // HtPumpDefCtrl 
      QDomElement htPumpDefCtrlElement = heatingCoilElement.firstChildElement("HtPumpDefCtrl");
      if( istringEqual(htPumpDefCtrlElement.text().toStdString(),"OnDemand") )
      {
        heatingCoil.setDefrostControl("OnDemand");
      }
      else if( istringEqual(htPumpDefCtrlElement.text().toStdString(),"TimedCycle") )
      {
        heatingCoil.setDefrostControl("Timed");
      }

      // HtPumpDefCtrlTemp
      QDomElement htPumpDefCtrlTempElement = heatingCoilElement.firstChildElement("HtPumpDefCtrlTemp");
      value = htPumpDefCtrlTempElement.text().toDouble(&ok);
      if( ok )
      {
        value = unitToUnit(value,"F","C").get();
        heatingCoil.setMaximumOutdoorDryBulbTemperatureforDefrostOperation(value);
      }

      // DefrostTimePeriodFraction
      heatingCoil.setDefrostTimePeriodFraction(0.058333);

      result = heatingCoil;
    }
  }
  else
  {
    model::Schedule schedule = alwaysOnSchedule(model);

    model::CoilHeatingElectric coil(model,schedule);

    coil.setName(nameElement.text().toStdString());

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
  const QDomElement& fanElement, 
  const QDomDocument& doc, 
  openstudio::model::Model& model)
{
  boost::optional<model::ModelObject> result;
  bool ok;
  double value;

  if( ! istringEqual(fanElement.tagName().toStdString(),"Fan") )
  {
    return result;
  }

  // FanControlMethod
  QDomElement fanControlMethodElement = fanElement.firstChildElement("CtrlMthd");

  // Name
  QDomElement nameElement = fanElement.firstChildElement("Name");

  // TotEff
  QDomElement totEffElement = fanElement.firstChildElement("TotEff");

  // MtrfEff 
  QDomElement mtrEffElement = fanElement.firstChildElement("MtrEff");

  // FlowCap
  QDomElement flowCapElement = fanElement.firstChildElement("FlowCapSim");

  // TotStaticPress
  QDomElement totStaticPressElement = fanElement.firstChildElement("TotStaticPress"); 

  // MtrPos
  QDomElement mtrPosElement = fanElement.firstChildElement("MtrPos");
  bool motorInAirstream = false;
  if( istringEqual(mtrPosElement.text().toStdString(),"InAirStream") )
  {
    motorInAirstream = true;
  }

  boost::optional<double> flowCap;

  if( ! autosize() )
  {
    value = flowCapElement.text().toDouble(&ok);

    if( ok ) 
    {
      Quantity flowRateIP(value,createCFMVolumetricFlowrate());
      OptionalQuantity flowRateSI = QuantityConverter::instance().convert(flowRateIP, UnitSystem(UnitSystem::SI));
      OS_ASSERT(flowRateSI);
      flowCap = flowRateSI->value();
    }
  }

  // ConstantVolume
  if( istringEqual(fanControlMethodElement.text().toStdString(),"ConstantVolume") ||
      istringEqual(fanControlMethodElement.text().toStdString(),"TwoSpeed") )
  {
    // The type of fan is dependent on the context.  We use FanOnOff for fan coil units, FanConstantVolume for everything else
    QDomElement parentElement = fanElement.parentNode().toElement();

    if( parentElement.nodeName().compare("ZnSys",Qt::CaseInsensitive) == 0 )
    {
      // Type 

      QDomElement znSysTypeElement = parentElement.firstChildElement("Type");

      if( znSysTypeElement.text().compare("FPFC",Qt::CaseInsensitive) == 0 || 
          znSysTypeElement.text().compare("PTHP",Qt::CaseInsensitive) == 0 ||
          znSysTypeElement.text().compare("WSHP",Qt::CaseInsensitive) == 0 )
      {
        model::Schedule schedule = alwaysOnSchedule(model);

        model::FanOnOff fan(model,schedule);

        fan.setName(nameElement.text().toStdString());

        // TotEff
        boost::optional<double> totEff;
        value = totEffElement.text().toDouble(&ok);
        if( ok )
        {
          totEff = value;
        }

        // MtrEff
        boost::optional<double> mtrEff;
        value = mtrEffElement.text().toDouble(&ok);
        if( ok )
        {
          mtrEff = value;
        }

        if( mtrEff )
        {
          fan.setMotorEfficiency(mtrEff.get());
        }

        if( totEff )
        {
          fan.setFanEfficiency(totEff.get());
        }

        // FlowCap
        if( flowCap )
        {
          fan.setMaximumFlowRate(flowCap.get());
        }

        // TotStaticPress
        value = totStaticPressElement.text().toDouble(&ok);
        if( ok )
        {
          // Convert in WC to Pa
          fan.setPressureRise(value * 249.0889 );
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
        QDomElement pwr_fPLRCrvElement = fanElement.firstChildElement("Pwr_fPLRCrvRef");
        boost::optional<model::Curve> pwr_fPLRCrv;
        pwr_fPLRCrv = model.getModelObjectByName<model::Curve>(pwr_fPLRCrvElement.text().toStdString());
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
      model::Schedule schedule = alwaysOnSchedule(model);

      model::FanConstantVolume fan(model,schedule);

      fan.setName(nameElement.text().toStdString());

      // TotEff
      boost::optional<double> totEff;
      value = totEffElement.text().toDouble(&ok);
      if( ok )
      {
        totEff = value;
      }

      // MtrEff
      boost::optional<double> mtrEff;
      value = mtrEffElement.text().toDouble(&ok);
      if( ok )
      {
        mtrEff = value;
      }

      if( mtrEff )
      {
        fan.setMotorEfficiency(mtrEff.get());
      }

      if( totEff )
      {
        fan.setFanEfficiency(totEff.get());
      }

      // FlowCap
      if( flowCap )
      {
        fan.setMaximumFlowRate(flowCap.get());
      }

      // TotStaticPress
      value = totStaticPressElement.text().toDouble(&ok);
      if( ok )
      {
        // Convert in WC to Pa
        fan.setPressureRise(value * 249.0889 );
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
  else if( istringEqual(fanControlMethodElement.text().toStdString(),"VariableSpeedDrive") ) 
  {
    model::Schedule schedule = alwaysOnSchedule(model);

    model::FanVariableVolume fan(model,schedule);

    fan.setName(nameElement.text().toStdString());

    // TotEff
    boost::optional<double> totEff;
    value = totEffElement.text().toDouble(&ok);
    if( ok )
    {
      totEff = value;
    }

    // MtrEff
    boost::optional<double> mtrEff;
    value = mtrEffElement.text().toDouble(&ok);
    if( ok )
    {
      mtrEff = value;
    }

    if( mtrEff )
    {
      fan.setMotorEfficiency(mtrEff.get());
    }

    if( totEff )
    {
      fan.setFanEfficiency(totEff.get());
    }

    // FlowCap
    if( flowCap )
    {
      fan.setMaximumFlowRate(flowCap.get());
    }

    // TotStaticPress
    value = totStaticPressElement.text().toDouble(&ok);
    if( ok )
    {
      // Convert in WC to Pa
      fan.setPressureRise(value * 249.0889 );
    }

    // Pwr_fPLRCrvRef
    QDomElement pwr_fPLRCrvElement = fanElement.firstChildElement("Pwr_fPLRCrvRef");
    boost::optional<model::Curve> pwr_fPLRCrv;
    pwr_fPLRCrv = model.getModelObjectByName<model::Curve>(pwr_fPLRCrvElement.text().toStdString());
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

    QDomElement flowMinSimElement = fanElement.firstChildElement("FlowMinSim");

    value = flowMinSimElement.text().toDouble(&ok);

    if( ok )
    {
      value = unitToUnit(value,"cfm","m^3/s").get();
      
      fan.setFanPowerMinimumAirFlowRate(value);
    }

    result = fan;
  }

  return result;
}

boost::optional<openstudio::model::ModelObject> ReverseTranslator::translateEvapClr(
  const QDomElement& element,
  const QDomDocument& doc,
  openstudio::model::Model& model)
{
  boost::optional<model::ModelObject> result;
  bool ok;
  double value;

  model::Schedule schedule = model.alwaysOnDiscreteSchedule();

  if( ! istringEqual(element.tagName().toStdString(),"EvapClr") )
  {
    return result;
  }

  QDomElement typeElement = element.firstChildElement("Type");
  QDomElement nameElement = element.firstChildElement("Name");

  if( typeElement.text().compare("Direct",Qt::CaseInsensitive) == 0 )
  {
    model::EvaporativeCoolerDirectResearchSpecial evap(model,schedule);

    evap.setName(nameElement.text().toStdString());
    
    QDomElement effElement = element.firstChildElement("Eff");
    value = effElement.text().toDouble(&ok);
    if( ok )
    {
      evap.setCoolerEffectiveness(value);
    }

    QDomElement pumpPwrElement = element.firstChildElement("PumpPwr");
    value = pumpPwrElement.text().toDouble(&ok);
    if( ok )
    {
      value = unitToUnit(value,"Btu/h","W").get();
      evap.setRecirculatingWaterPumpPowerConsumption(value);
    }

    evap.setBlowdownConcentrationRatio(3.0);

    result = evap;
  }
  else if( typeElement.text().compare("Indirect",Qt::CaseInsensitive) == 0 )
  {
    model::EvaporativeCoolerIndirectResearchSpecial evap(model);

    evap.setName(nameElement.text().toStdString());

    QDomElement effElement = element.firstChildElement("Eff");
    value = effElement.text().toDouble(&ok);
    if( ok )
    {
      evap.setCoolerMaximumEffectiveness(value);
    }

    QDomElement pumpPwrElement = element.firstChildElement("PumpPwr");
    value = pumpPwrElement.text().toDouble(&ok);
    if( ok )
    {
      value = unitToUnit(value,"Btu/h","W").get();
      evap.setRecirculatingWaterPumpPowerConsumption(value);
    }

    QDomElement secFanFlowCapElement = element.firstChildElement("SecFanFlowCap");
    value = secFanFlowCapElement.text().toDouble(&ok);
    if( ok )
    {
      value = unitToUnit(value,"cfm","m^3/s").get();
      evap.setSecondaryFanFlowRate(value);
    }

    QDomElement secFanTotEffElemenet = element.firstChildElement("SecFanTotEff");
    value = secFanTotEffElemenet.text().toDouble(&ok);
    if( ok )
    {
      evap.setSecondaryFanTotalEfficiency(value);
    }

    QDomElement secFanTotStaticPressElement = element.firstChildElement("SecFanTotStaticPress");
    value = secFanTotStaticPressElement.text().toDouble(&ok);
    if( ok )
    {
      // Convert in WC to Pa
      evap.setSecondaryFanDeltaPressure(value * 249.0889 );
    }

    QDomElement indirectDewPtEffElement = element.firstChildElement("IndirectDewPtEff");
    value = indirectDewPtEffElement.text().toDouble(&ok);
    if( ok )
    {
      evap.setDewpointEffectivenessFactor(value);
    }
    // TODO: reset dewpoint effectiveness

    evap.setBlowdownConcentrationRatio(3.0);

    QDomElement secAirSrcElement = element.firstChildElement("SecAirSrc");

    evap.resetDriftLossFraction();

    result = evap;
  }
  else
  {
    LOG(Error,nameElement.text().toStdString() << " is an unsupported type: defaulting to direct evaporative cooling");
    model::EvaporativeCoolerDirectResearchSpecial evap(model,schedule);
    result = evap;
  }

  return result;
}


boost::optional<openstudio::model::ModelObject> ReverseTranslator::translateCoilCooling(
  const QDomElement& coolingCoilElement, 
  const QDomDocument& doc, 
  openstudio::model::Model& model)
{
  boost::optional<model::ModelObject> result;
  bool ok;
  double value;

  if( ! istringEqual(coolingCoilElement.tagName().toStdString(),"CoilClg") )
  {
    return result;
  }

  bool znSys = false;
  if( coolingCoilElement.parentNode().toElement().tagName().compare("ZnSys",Qt::CaseInsensitive) == 0 )
  {
    znSys = true;
  }

  // Look for a sibling fan to figure out what the flow capacity should be

  QDomElement flowCapElement;

  QDomElement airSegElement = coolingCoilElement.parentNode().toElement();

  if( ! airSegElement.isNull() )
  {
    QDomElement fanElement = airSegElement.firstChildElement("Fan");

    if( ! fanElement.isNull() )
    {
      flowCapElement = fanElement.firstChildElement("FlowCapSim");
    }
  }

  // CapTotRtd
  QDomElement capTotRtdElement = coolingCoilElement.firstChildElement("CapTotGrossRtdSim");

  // CapSensRtd
  QDomElement capSensRtdElement = coolingCoilElement.firstChildElement("CapSensGrossRtdSim");

  // Type
  QDomElement coilCoolingTypeElement = coolingCoilElement.firstChildElement("Type");

  // DirectExpansion
  if( istringEqual(coilCoolingTypeElement.text().toStdString(),"DirectExpansion") )
  {
    // Name
    QDomElement nameElement = coolingCoilElement.firstChildElement("Name");

    // NumClgStages
    QDomElement numClgStagesElement = coolingCoilElement.firstChildElement("NumClgStages");

    // CndsrType
    QDomElement cndsrTypeElement = coolingCoilElement.firstChildElement("CndsrType");

    if( cndsrTypeElement.text().compare("WaterSource",Qt::CaseInsensitive) == 0 )
    {
      boost::optional<model::AirLoopHVACUnitarySystem> unitary;
      model::CoilCoolingWaterToAirHeatPumpEquationFit coil(model);
      coil.setName(nameElement.text().toStdString());
      if( znSys )
      {
        result = coil;
      }
      else
      {
        unitary = model::AirLoopHVACUnitarySystem(model);
        unitary->setName(nameElement.text().toStdString() + " Unitary");
        unitary->getImpl<model::detail::AirLoopHVACUnitarySystem_Impl>()->setControlType("SetPoint");
        unitary->setCoolingCoil(coil);
        result = unitary.get();
      }

      // Plant
      QDomElement fluidSegNameElement = coolingCoilElement.firstChildElement("FluidSegInRef");
      boost::optional<model::PlantLoop> plant = loopForSupplySegment(fluidSegNameElement.text(),doc,model);
      if( plant )
      {
        plant->addDemandBranchForComponent(coil);
      }

      if( ! autosize() )
      {
        value = flowCapElement.text().toDouble(&ok);
        if( ok ) 
        {
          value = unitToUnit(value,"cfm","m^3/s").get();
          coil.setRatedAirFlowRate(value);
          if( unitary )
          {
            unitary->setSupplyAirFlowRateDuringCoolingOperation(value);
          }
        }
      }

      if( ! autosize() )
      {
        QDomElement fluidFlowRtDsgnSimElement = coolingCoilElement.firstChildElement("FluidFlowRtDsgnSim");
        value = fluidFlowRtDsgnSimElement.text().toDouble(&ok);
        if( ok )
        {
          value = unitToUnit(value,"gal/min","m^3/s").get();
          coil.setRatedWaterFlowRate(value);
        }
      }

      if( ! autosize() )
      {
        QDomElement capTotGrossRtdSimElement = coolingCoilElement.firstChildElement("CapTotGrossRtdSim");
        value = capTotGrossRtdSimElement.text().toDouble(&ok);
        if( ok )
        {
          value = unitToUnit(value,"Btu/h","W").get();
          coil.setRatedTotalCoolingCapacity(value);
        }
      }

      if( ! autosize() )
      {
        QDomElement capSensGrossRtdSimElement = coolingCoilElement.firstChildElement("CapSensGrossRtdSim");
        value = capSensGrossRtdSimElement.text().toDouble(&ok);
        if( ok )
        {
          value = unitToUnit(value,"Btu/h","W").get();
          coil.setRatedSensibleCoolingCapacity(value);
        }
      }

      QDomElement dxEIRElement = coolingCoilElement.firstChildElement("DXEIR");
      value = dxEIRElement.text().toDouble(&ok);
      if( ok )
      {
        coil.setRatedCoolingCoefficientofPerformance( 1.0 / value );
      }

      coil.setTotalCoolingCapacityCoefficient1(-0.68126221);
      coil.setTotalCoolingCapacityCoefficient2(1.99529297);
      coil.setTotalCoolingCapacityCoefficient3(-0.93611888);
      coil.setTotalCoolingCapacityCoefficient4(0.02081177);
      coil.setTotalCoolingCapacityCoefficient5(0.008438868);

      coil.setSensibleCoolingCapacityCoefficient1(2.24209455);
      coil.setSensibleCoolingCapacityCoefficient2(7.28913391);
      coil.setSensibleCoolingCapacityCoefficient3(-9.06079896);
      coil.setSensibleCoolingCapacityCoefficient4(-0.36729404);
      coil.setSensibleCoolingCapacityCoefficient5(0.218826161);
      coil.setSensibleCoolingCapacityCoefficient6(0.00901534);

      coil.setCoolingPowerConsumptionCoefficient1(-3.20456384);
      coil.setCoolingPowerConsumptionCoefficient2(0.47656454);
      coil.setCoolingPowerConsumptionCoefficient3(3.16734236);
      coil.setCoolingPowerConsumptionCoefficient4(0.10244637);
      coil.setCoolingPowerConsumptionCoefficient5(-0.038132556);

      coil.setNominalTimeforCondensateRemovaltoBegin(0.0);
      coil.setRatioofInitialMoistureEvaporationRateandSteadyStateLatentCapacity(0.0);

    }
    else if( numClgStagesElement.text().toInt() == 1 || numClgStagesElement.isNull() )
    {
      model::Schedule schedule = model.alwaysOnDiscreteSchedule();

      // Cap_fTempCrvRef

      boost::optional<model::Curve> coolingCurveFofTemp;
      QDomElement cap_fTempCrvRefElement = coolingCoilElement.firstChildElement("Cap_fTempCrvRef");
      coolingCurveFofTemp = model.getModelObjectByName<model::Curve>(cap_fTempCrvRefElement.text().toStdString());
      if( ! coolingCurveFofTemp )
      {
        LOG(Error,"Coil: " << nameElement.text().toStdString() << "Broken Cap_fTempCrvRef");

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
      QDomElement cap_fFlowCrvRefElement = coolingCoilElement.firstChildElement("Cap_fFlowCrvRef");
      coolingCurveFofFlow = model.getModelObjectByName<model::Curve>(cap_fFlowCrvRefElement.text().toStdString());
      if( ! coolingCurveFofFlow )
      {
        LOG(Error,"Coil: " << nameElement.text().toStdString() << "Broken Cap_fFlowCrvRef");

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
      QDomElement dxEIR_fTempCrvRefElement = coolingCoilElement.firstChildElement("DXEIR_fTempCrvRef");
      energyInputRatioFofTemp = model.getModelObjectByName<model::Curve>(dxEIR_fTempCrvRefElement.text().toStdString());
      if( ! energyInputRatioFofTemp )
      {
        LOG(Error,"Coil: " << nameElement.text().toStdString() << "Broken DXEIR_fTempCrvRef");

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
      QDomElement dxEIR_fFlowCrvRefElement = coolingCoilElement.firstChildElement("DXEIR_fFlowCrvRef");
      energyInputRatioFofFlow = model.getModelObjectByName<model::Curve>(dxEIR_fFlowCrvRefElement.text().toStdString());
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
      QDomElement dxEIR_fPLFCrvRefElement = coolingCoilElement.firstChildElement("DXEIR_fPLFCrvRef");
      partLoadFraction = model.getModelObjectByName<model::Curve>(dxEIR_fPLFCrvRefElement.text().toStdString());
      if( ! partLoadFraction )
      {
        LOG(Error,"Coil: " << nameElement.text().toStdString() << "Broken DXEIR_fPLFCrvRef");

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
                                                   schedule,
                                                   coolingCurveFofTemp.get(),
                                                   coolingCurveFofFlow.get(),
                                                   energyInputRatioFofTemp.get(),
                                                   energyInputRatioFofFlow.get(),
                                                   partLoadFraction.get() );

      coilCooling.setName(nameElement.text().toStdString());

      // FlowCap
      if( ! autosize() )
      {
        value = flowCapElement.text().toDouble(&ok);

        if( ok ) 
        {
          Quantity flowRateIP(value,createCFMVolumetricFlowrate());
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
        value = capTotRtdElement.text().toDouble(&ok);

        if( ok ) 
        {
          Quantity valueIP(value,createBTUPower());
          OptionalQuantity valueSI = QuantityConverter::instance().convert(valueIP, UnitSystem(UnitSystem::SI));
          OS_ASSERT(valueSI);
          coilCooling.setRatedTotalCoolingCapacity(valueSI->value());

          totalCapacity = value;
        }
      }

      // CapSensRtd
      if( ! autosize() )
      {
        value = capSensRtdElement.text().toDouble(&ok);

        if( ok )
        {
          sensibleCapacity = value;
        }
      }

      if( totalCapacity && sensibleCapacity )
      {
        coilCooling.setRatedSensibleHeatRatio( sensibleCapacity.get() / totalCapacity.get() );
      }

      // DXEIR
      QDomElement dxEIRElement = coolingCoilElement.firstChildElement("DXEIR");
      value = dxEIRElement.text().toDouble(&ok);
      if( ok )
      {
        coilCooling.setRatedCOP( 1.0 / value);
      }

      // DXCrankcaseHtCap
      QDomElement dxCrankcaseHtCapElement = coolingCoilElement.firstChildElement("DXCrankcaseHtrCapSim");
      value = dxCrankcaseHtCapElement.text().toDouble(&ok);
      if( ok )
      {
        value = unitToUnit(value,"Btu/h","W").get();
        coilCooling.setCrankcaseHeaterCapacity(value);
      }

      // DXCrankcaseCtrlTemp
      QDomElement dxCrankcaseCtrlTempElement = coolingCoilElement.firstChildElement("DXCrankcaseCtrlTemp");
      value = dxCrankcaseCtrlTempElement.text().toDouble(&ok);
      if( ok )
      {
        value = unitToUnit(value,"F","C").get();
        coilCooling.setMaximumOutdoorDryBulbTemperatureForCrankcaseHeaterOperation(value);
      }

      result = coilCooling;
    }
    else if( numClgStagesElement.text().toInt() == 2 || numClgStagesElement.isNull() )
    {
      model::Schedule schedule = alwaysOnSchedule(model);

      // Cap_fTempCrvRef

      boost::optional<model::Curve> coolingCurveFofTemp;
      QDomElement cap_fTempCrvRefElement = coolingCoilElement.firstChildElement("Cap_fTempCrvRef");
      coolingCurveFofTemp = model.getModelObjectByName<model::Curve>(cap_fTempCrvRefElement.text().toStdString());
      if( ! coolingCurveFofTemp )
      {
        LOG(Error,"Coil: " << nameElement.text().toStdString() << "Broken Cap_fTempCrvRef");

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
      QDomElement cap_fFlowCrvRefElement = coolingCoilElement.firstChildElement("Cap_fFlowCrvRef");
      coolingCurveFofFlow = model.getModelObjectByName<model::Curve>(cap_fFlowCrvRefElement.text().toStdString());
      if( ! coolingCurveFofFlow )
      {
        LOG(Error,"Coil: " << nameElement.text().toStdString() << "Broken Cap_fFlowCrvRef");

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
      QDomElement dxEIR_fTempCrvRefElement = coolingCoilElement.firstChildElement("DXEIR_fTempCrvRef");
      energyInputRatioFofTemp = model.getModelObjectByName<model::Curve>(dxEIR_fTempCrvRefElement.text().toStdString());
      if( ! energyInputRatioFofTemp )
      {
        LOG(Error,"Coil: " << nameElement.text().toStdString() << "Broken DXEIR_fTempCrvRef");

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
      QDomElement dxEIR_fFlowCrvRefElement = coolingCoilElement.firstChildElement("DXEIR_fFlowCrvRef");
      energyInputRatioFofFlow = model.getModelObjectByName<model::Curve>(dxEIR_fFlowCrvRefElement.text().toStdString());
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
      QDomElement dxEIR_fPLFCrvRefElement = coolingCoilElement.firstChildElement("DXEIR_fPLFCrvRef");
      partLoadFraction = model.getModelObjectByName<model::Curve>(dxEIR_fPLFCrvRefElement.text().toStdString());
      if( ! partLoadFraction )
      {
        LOG(Error,"Coil: " << nameElement.text().toStdString() << "Broken DXEIR_fPLFCrvRef");

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
                                              schedule,
                                              coolingCurveFofTemp.get(),
                                              coolingCurveFofFlow.get(),
                                              energyInputRatioFofTemp.get(),
                                              energyInputRatioFofFlow.get(),
                                              partLoadFraction.get(), 
                                              coolingCurveFofTemp->clone(model).cast<model::Curve>(),
                                              energyInputRatioFofTemp->clone(model).cast<model::Curve>());

      coilCooling.setName(nameElement.text().toStdString());

      boost::optional<double> totalCapacity;
      boost::optional<double> sensibleCapacity;
      boost::optional<double> lowSpeedCapacityFraction;

      // CapTotRtdStageFrac
      QDomNodeList capTotRtdStageFracElements = coolingCoilElement.elementsByTagName("CapTotRtdStageFrac");
      for( int j = 0; j < capTotRtdStageFracElements.count(); j++ )
      {
        QDomElement capTotRtdStageFracElement = capTotRtdStageFracElements.at(j).toElement();
        QString indexAttribute = capTotRtdStageFracElement.attribute("index");
        if( istringEqual(indexAttribute.toStdString(),"0") )
        {
          double fraction = capTotRtdStageFracElement.text().toDouble(&ok);
          if( ok )
          {
            lowSpeedCapacityFraction = fraction;
          }

          break;
        }
      }

      // FlowCap
      if( ! autosize() )
      {
        value = flowCapElement.text().toDouble(&ok);

        if( ok ) 
        {
          Quantity flowRateIP(value,createCFMVolumetricFlowrate());
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
        value = capTotRtdElement.text().toDouble(&ok);

        if( ok ) 
        {
          Quantity valueIP(value,createBTUPower());
          OptionalQuantity valueSI = QuantityConverter::instance().convert(valueIP, UnitSystem(UnitSystem::SI));
          OS_ASSERT(valueSI);
          coilCooling.setRatedHighSpeedTotalCoolingCapacity(valueSI->value());
          if( lowSpeedCapacityFraction )
          {
            coilCooling.setRatedLowSpeedTotalCoolingCapacity(valueSI->value() * lowSpeedCapacityFraction.get());
          }

          totalCapacity = value;
        }
      }

      // CapSensRtd
      if( ! autosize() )
      {
        value = capSensRtdElement.text().toDouble(&ok);

        if( ok )
        {
          sensibleCapacity = value;
        }
      }

      if( totalCapacity && sensibleCapacity )
      {
        coilCooling.setRatedHighSpeedSensibleHeatRatio( sensibleCapacity.get() / totalCapacity.get() );

        // CapSensRtdStageFrac
        if( lowSpeedCapacityFraction )
        {
          QDomNodeList capSensRtdStageFracElements = coolingCoilElement.elementsByTagName("CapSensRtdStageFrac");
          for( int j = 0; j < capSensRtdStageFracElements.count(); j++ )
          {
            QDomElement capSensRtdStageFracElement = capSensRtdStageFracElements.at(j).toElement();
            QString indexAttribute = capSensRtdStageFracElement.attribute("index");
            if( istringEqual(indexAttribute.toStdString(),"0") )
            {
              double fraction = capSensRtdStageFracElement.text().toDouble(&ok);
              if( ok )
              {
                double lowSHR = (sensibleCapacity.get() * fraction) / (totalCapacity.get() * lowSpeedCapacityFraction.get());
                coilCooling.setRatedLowSpeedSensibleHeatRatio(lowSHR);
              }
              break;
            }
          }
        }
      }

      // DXEIR
      
      QDomElement dxEIRElement = coolingCoilElement.firstChildElement("DXEIR");

      value = dxEIRElement.text().toDouble(&ok);

      if( ok )
      {
        double cop = 1.0 / value;

        coilCooling.setRatedHighSpeedCOP(cop);

        coilCooling.setRatedLowSpeedCOP(cop);
      }

      result = coilCooling;
    }
    else if( numClgStagesElement.text().toInt() > 2 || numClgStagesElement.isNull() )
    {
      LOG(Error, "DX cooling coils with more than two stages are not yet supported. See coil named " << nameElement.text().toStdString());
    }
  }
  // ChilledWater
  if( istringEqual(coilCoolingTypeElement.text().toStdString(),"ChilledWater") )
  {
    model::Schedule schedule = model.alwaysOnDiscreteSchedule();

    model::CoilCoolingWater coilCooling(model,schedule);

    // Name

    QDomElement nameElement = coolingCoilElement.firstChildElement("Name");

    coilCooling.setName(nameElement.text().toStdString());

    coilCooling.setTypeOfAnalysis("DetailedAnalysis");

    // Plant

    QDomElement fluidSegNameElement = coolingCoilElement.firstChildElement("FluidSegInRef");

    boost::optional<model::PlantLoop> plant = loopForSupplySegment(fluidSegNameElement.text(),doc,model);

    if( plant )
    {
      plant->addDemandBranchForComponent(coilCooling);
    }

    coilCooling.autosizeDesignInletAirTemperature();
    coilCooling.autosizeDesignOutletAirTemperature();
    coilCooling.autosizeDesignInletAirHumidityRatio();
    coilCooling.autosizeDesignOutletAirHumidityRatio();

    if( ! autosize() )
    {
      // Design Water Volum Flow Rate of Coil

      QDomElement fluidFlowRtDsgnElement = coolingCoilElement.firstChildElement("FluidFlowRtDsgnSim");

      coilCooling.setDesignWaterFlowRate(fluidFlowRtDsgnElement.text().toDouble() * 0.00006309);

      // Design defaults

      coilCooling.setDesignInletWaterTemperature(7.22); // From Sizing Plant

      if( plant )
      {
        coilCooling.setDesignInletWaterTemperature(plant->sizingPlant().designLoopExitTemperature());
      }

      // FlowCap

      double value = flowCapElement.text().toDouble();

      Quantity flowRateIP(value,createCFMVolumetricFlowrate());
      OptionalQuantity flowRateSI = QuantityConverter::instance().convert(flowRateIP, UnitSystem(UnitSystem::SI));
      OS_ASSERT(flowRateSI);
      coilCooling.setDesignAirFlowRate(flowRateSI->value());
    }

    result = coilCooling;
  }

  return result;
}

boost::optional<openstudio::model::ModelObject> ReverseTranslator::translateThermalZone(
  const QDomElement& thermalZoneElement, 
  const QDomDocument& doc, 
  openstudio::model::Model& model )
{
  UnitSystem siSys(UnitSystem::SI);
  UnitSystem whSys(UnitSystem::Wh);

  boost::optional<openstudio::model::ModelObject> result;

  if( ! istringEqual(thermalZoneElement.tagName().toStdString(),"ThrmlZn") )
  {
    return result;
  }

  boost::optional<model::ThermalZone> optionalThermalZone;
  boost::optional<model::ThermostatSetpointDualSetpoint> optionalThermostat;

  // Name
  QDomElement nameElement = thermalZoneElement.firstChildElement("Name");
  std::string name = nameElement.text().toStdString();
  optionalThermalZone = model.getModelObjectByName<model::ThermalZone>(name);

  if( ! optionalThermalZone )
  {
    return result;
  }

  model::ThermalZone thermalZone = optionalThermalZone.get();
  result = thermalZone;

  // Type
  QDomElement typeElement = thermalZoneElement.firstChildElement("Type");
  if(!typeElement.isNull()){
    if (typeElement.text() == "Conditioned"){
      optionalThermostat = model::ThermostatSetpointDualSetpoint(model);
      optionalThermostat->setName(name + " Thermostat");
      thermalZone.setThermostatSetpointDualSetpoint(*optionalThermostat);
    }else if (typeElement.text() == "Unconditioned"){
      // no thermostat
    }else if (typeElement.text() == "Plenum"){
      // no thermostat
    }else{
      LOG(Error, "Unknown thermal zone type '" << toString(typeElement.text()) << "'");
    }
  }

  // Volume
  QDomElement volElement = thermalZoneElement.firstChildElement("VolSim");
  if (!volElement.isNull()){
    // bec units = ft^3, os units = m^3
    Quantity thermalZoneVolumeIP(volElement.text().toDouble(), BTUUnit(BTUExpnt(0,3,0,0)));
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
  double value;
  bool ok;

  QDomElement clgDsgnSupAirTempElement = thermalZoneElement.firstChildElement("ClgDsgnSupAirTemp");
  value = clgDsgnSupAirTempElement.text().toDouble(&ok);
  if( ok )
  {
    clgDsgnSupAirTemp = unitToUnit(value,"F","C").get();
  }

  QDomElement clgDsgnSizingFacElement = thermalZoneElement.firstChildElement("ClgDsgnSizingFac");
  value = clgDsgnSizingFacElement.text().toDouble(&ok);
  if( ok )
  {
    clgDsgnSizingFac = value;
  }

  QDomElement htgDsgnSupAirTempElement = thermalZoneElement.firstChildElement("HtgDsgnSupAirTemp");
  value = htgDsgnSupAirTempElement.text().toDouble(&ok);
  if( ok )
  {
    htgDsgnSupAirTemp = unitToUnit(value,"F","C").get();
  }

  QDomElement htgDsgnSizingFacElement = thermalZoneElement.firstChildElement("HtgDsgnSizingFac");
  value = htgDsgnSizingFacElement.text().toDouble(&ok);
  if( ok )
  {
    htgDsgnSizingFac = value; 
  }

  model::SizingZone sizingZone = thermalZone.sizingZone();
  sizingZone.setZoneCoolingDesignSupplyAirTemperature(clgDsgnSupAirTemp);
  sizingZone.setZoneCoolingSizingFactor(clgDsgnSizingFac);
  sizingZone.setZoneHeatingDesignSupplyAirTemperature(htgDsgnSupAirTemp);
  sizingZone.setZoneHeatingSizingFactor(htgDsgnSizingFac);

  sizingZone.setCoolingMinimumAirFlowperZoneFloorArea(0.0);
  sizingZone.setCoolingMinimumAirFlow(0.0);
  sizingZone.setCoolingMinimumAirFlowFraction(0.0);
  sizingZone.setCoolingDesignAirFlowMethod("DesignDay");

  sizingZone.setHeatingMaximumAirFlowperZoneFloorArea(0.0);
  sizingZone.setHeatingMaximumAirFlow(0.0);
  sizingZone.setHeatingMaximumAirFlowFraction(0.0);
  sizingZone.setHeatingDesignAirFlowMethod("DesignDay");

  QDomElement htgDsgnMaxFlowFracElement = thermalZoneElement.firstChildElement("HtgDsgnMaxFlowFrac");
  double htgDsgnMaxFlowFrac = htgDsgnMaxFlowFracElement.text().toDouble(&ok);
  if( ok && htgDsgnMaxFlowFrac > 0.0 )
  {
    sizingZone.setHeatingDesignAirFlowMethod("DesignDayWithLimit");
    sizingZone.setHeatingMaximumAirFlowFraction(htgDsgnMaxFlowFrac);
  }

  // Ventilation

  double ventPerPersonSim = 0.0;
  double ventPerAreaSim = 0.0;

  std::string ventSpecMthdSim = "Maximum";

  QDomElement ventPerPersonSimElement = thermalZoneElement.firstChildElement("VentPerPersonSim");
  value = ventPerPersonSimElement.text().toDouble(&ok);
  if( ok )
  {
    // cfm / person -> m^3/s / person
    ventPerPersonSim = unitToUnit(value,"cfm","m^3/s").get();
  }

  QDomElement ventPerAreaSimElement = thermalZoneElement.firstChildElement("VentPerAreaSim");
  value = ventPerAreaSimElement.text().toDouble(&ok);
  if( ok )
  {
    // cfm / ft^2 -> m^3/s / m^2
    ventPerAreaSim = value * 0.00508;
  }

  QDomElement ventSpecMthdSimElement = thermalZoneElement.firstChildElement("VentSpecMthdSim");
  if( istringEqual(ventSpecMthdSimElement.text().toStdString(),"Sum") )
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

  QDomElement exhFlowRtSimElement = thermalZoneElement.firstChildElement("ExhFlowSim");
  QDomElement exhFanNameElement = thermalZoneElement.firstChildElement("ExhFanName");

  value = exhFlowRtSimElement.text().toDouble(&ok);
  if( ok && (! exhFanNameElement.isNull()) )
  {
    model::FanZoneExhaust exhaustFan(model);
    exhaustFan.setName(exhFanNameElement.text().toStdString());

    value = unitToUnit(value,"cfm","m^3/s").get();
    exhaustFan.setMaximumFlowRate(value);

    QDomElement exhAvailSchRefElement = thermalZoneElement.firstChildElement("ExhAvailSchRef");
    std::string exhAvailSchRef = escapeName(exhAvailSchRefElement.text());
    boost::optional<model::Schedule> exhAvailSch = model.getModelObjectByName<model::Schedule>(exhAvailSchRef);
    if( exhAvailSch )
    {
      exhaustFan.setAvailabilitySchedule(exhAvailSch.get());
    }

    QDomElement exhTotEffElement = thermalZoneElement.firstChildElement("ExhTotEff");
    value = exhTotEffElement.text().toDouble(&ok);
    if( ok )
    {
      exhaustFan.setFanEfficiency(value);
    }

    QDomElement exhTotStaticPressElement = thermalZoneElement.firstChildElement("ExhTotStaticPress");
    value = exhTotStaticPressElement.text().toDouble(&ok);
    if( ok )
    {
      // Convert in WC to Pa
      exhaustFan.setPressureRise(value * 249.0889 );
    }

    QDomElement exhFlowSchRefElement = thermalZoneElement.firstChildElement("ExhFlowSchRef");
    std::string exhFlowSchRef = escapeName(exhFlowSchRefElement.text());
    boost::optional<model::Schedule> exhFlowSch = model.getModelObjectByName<model::Schedule>(exhFlowSchRef);
    if( exhFlowSch )
    {
      exhaustFan.setFlowFractionSchedule(exhFlowSch.get());
    }

    QDomElement exhOperModeElement = thermalZoneElement.firstChildElement("ExhOperMode");
    if( exhOperModeElement.text().compare("DecoupledFromSystem",Qt::CaseInsensitive) == 0 )
    {
      exhaustFan.setSystemAvailabilityManagerCouplingMode("Decoupled");
    }
    else if( exhOperModeElement.text().compare("CoupledToSystem",Qt::CaseInsensitive) == 0 )
    {
      exhaustFan.setSystemAvailabilityManagerCouplingMode("Coupled");
    }

    QDomElement exhMinTempSchRefElement = thermalZoneElement.firstChildElement("ExhMinTempSchRef");
    std::string exhMinTempSchRef = escapeName(exhMinTempSchRefElement.text());
    boost::optional<model::Schedule> exhMinTempSch = model.getModelObjectByName<model::Schedule>(exhMinTempSchRef);
    if( exhMinTempSch )
    {
      exhaustFan.setMinimumZoneTemperatureLimitSchedule(exhMinTempSch.get());
    }

    QDomElement exhBalancedSchRefElement = thermalZoneElement.firstChildElement("ExhBalancedSchRef");
    std::string exhBalancedSchRef = escapeName(exhBalancedSchRefElement.text());
    boost::optional<model::Schedule> exhBalancedSch = model.getModelObjectByName<model::Schedule>(exhBalancedSchRef);
    if( exhBalancedSch )
    {
      exhaustFan.setBalancedExhaustFractionSchedule(exhBalancedSch.get());
    }

    exhaustFan.addToThermalZone(thermalZone);
  }

  // Daylighting
  QDomNodeList daylighting1CoordElements = thermalZoneElement.elementsByTagName("DayltgIllumRefPt1Coord");
  QDomElement daylighting1SetpointElement = thermalZoneElement.firstChildElement("DayltgIllumSetpt1");
  QDomElement daylighting1FractionElement = thermalZoneElement.firstChildElement("DayltgCtrlLtgFrac1");

  QDomNodeList daylighting2CoordElements = thermalZoneElement.elementsByTagName("DayltgIllumRefPt2Coord");
  QDomElement daylighting2SetpointElement = thermalZoneElement.firstChildElement("DayltgIllumSetpt2");
  QDomElement daylighting2FractionElement = thermalZoneElement.firstChildElement("DayltgCtrlLtgFrac2");

  QDomElement daylightingMinLightingElement = thermalZoneElement.firstChildElement("DayltgMinDimLtgFrac");
  QDomElement daylightingMinPowerElement = thermalZoneElement.firstChildElement("DayltgMinDimPwrFrac");
  QDomElement daylightingGlareAzimuthElement = thermalZoneElement.firstChildElement("DayltgGlrAz");
  QDomElement daylightingMaxGlareElement = thermalZoneElement.firstChildElement("DayltgMaxGlrIdx");

  QDomElement daylightingControlTypeElement = thermalZoneElement.firstChildElement("DayltgCtrlType");
  QDomElement daylightingNumberOfControlStepsElement = thermalZoneElement.firstChildElement("DayltgNumOfCtrlSteps");

  boost::optional<double> daylightingMinLighting;
  if (!daylightingMinLightingElement.isNull()){
    daylightingMinLighting = daylightingMinLightingElement.text().toDouble();
  }
  boost::optional<double> daylightingMinPower;
  if (!daylightingMinPowerElement.isNull()){
    daylightingMinPower = daylightingMinPowerElement.text().toDouble();
  }
  boost::optional<double> daylightingGlareAzimuth;
  if (!daylightingGlareAzimuthElement.isNull()){
    daylightingGlareAzimuth = daylightingGlareAzimuthElement.text().toDouble();
  }
  boost::optional<double> daylightingMaxGlare;
  if (!daylightingMaxGlareElement.isNull()){
    daylightingMaxGlare = daylightingMaxGlareElement.text().toDouble();
  }
  boost::optional<std::string> daylightingControlType;
  if (!daylightingControlTypeElement.isNull()){
    daylightingControlType = toString(daylightingControlTypeElement.text());
  }
  boost::optional<int> daylightingNumberOfControlSteps;
  if (!daylightingNumberOfControlStepsElement.isNull()){
    daylightingNumberOfControlSteps = daylightingNumberOfControlStepsElement.text().toInt();
  }

  // first point
  boost::optional<model::DaylightingControl> daylightingControl1;
  if (daylighting1CoordElements.size() == 3 && !daylighting1SetpointElement.isNull() && !daylighting1FractionElement.isNull()){
    double x = footToMeter*daylighting1CoordElements.at(0).toElement().text().toDouble();
    double y = footToMeter*daylighting1CoordElements.at(1).toElement().text().toDouble();
    double z = footToMeter*daylighting1CoordElements.at(2).toElement().text().toDouble();
    
    // DLM: units in BEC are in lux
    //double setpoint = footCandleToLux*daylighting1SetpointElement.text().toDouble();
    double setpoint = daylighting1SetpointElement.text().toDouble();
    double fraction = daylighting1FractionElement.text().toDouble();

    daylightingControl1 = model::DaylightingControl(model);
    daylightingControl1->setPositionXCoordinate(x);
    daylightingControl1->setPositionYCoordinate(y);
    daylightingControl1->setPositionZCoordinate(z);
    daylightingControl1->setIlluminanceSetpoint(setpoint);

    if (daylightingControlType){
      if (istringEqual(*daylightingControlType, "SteppedSwitching") || istringEqual(*daylightingControlType, "SteppedDimming")){
        ok = daylightingControl1->setLightingControlType("Stepped");
        if (!ok){
          LOG(Error, "Could not set daylighting control type to 'Stepped'");
        }
        if (daylightingNumberOfControlSteps){
          daylightingControl1->setNumberofSteppedControlSteps(*daylightingNumberOfControlSteps);
        }
      }else if (istringEqual(*daylightingControlType, "Continuous")){
        ok = daylightingControl1->setLightingControlType("Continuous");
        if (!ok){
          LOG(Error, "Could not set daylighting control type to 'Continuous'");
        }
      }else if (istringEqual(*daylightingControlType, "ContinuousPlusOff")){
        ok = daylightingControl1->setLightingControlType("Continuous/Off");
        if (!ok){
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
  if (daylighting2CoordElements.size() == 3 && !daylighting2SetpointElement.isNull() && !daylighting2FractionElement.isNull()){
    if (!daylightingControl1){
      LOG(Error, "Secondary daylighting control specified for Thermal Zone '" << name << "' but there is no primary daylighting control");
    }else{
      double x = footToMeter*daylighting2CoordElements.at(0).toElement().text().toDouble();
      double y = footToMeter*daylighting2CoordElements.at(1).toElement().text().toDouble();
      double z = footToMeter*daylighting2CoordElements.at(2).toElement().text().toDouble();

      // DLM: units in BEC are in lux
      //double setpoint = footCandleToLux*daylighting2SetpointElement.text().toDouble();
      double setpoint = daylighting2SetpointElement.text().toDouble();
      double fraction = daylighting2FractionElement.text().toDouble();

      daylightingControl2 = model::DaylightingControl(model);
      daylightingControl2->setPositionXCoordinate(x);
      daylightingControl2->setPositionYCoordinate(y);
      daylightingControl2->setPositionZCoordinate(z);
      daylightingControl2->setIlluminanceSetpoint(setpoint);

      if (daylightingControlType){
        if (istringEqual(*daylightingControlType, "SteppedSwitching") || istringEqual(*daylightingControlType, "SteppedDimming")){
          ok = daylightingControl2->setLightingControlType("Stepped");
          if (!ok){
            LOG(Error, "Could not set daylighting control type to 'Stepped'");
          }
          if (daylightingNumberOfControlSteps){
            daylightingControl2->setNumberofSteppedControlSteps(*daylightingNumberOfControlSteps);
          }
        }else if (istringEqual(*daylightingControlType, "Continuous")){
          ok = daylightingControl2->setLightingControlType("Continuous");
          if (!ok){
            LOG(Error, "Could not set daylighting control type to 'Continuous'");
          }
        }else if (istringEqual(*daylightingControlType, "ContinuousPlusOff")){
          ok = daylightingControl2->setLightingControlType("Continuous/Off");
          if (!ok){
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
  QDomElement multElement = thermalZoneElement.firstChildElement("Mult");
  if(!multElement.isNull()){
    thermalZone.setMultiplier(multElement.text().toInt());
  }

  // Optional AirLoopHVAC
  boost::optional<model::AirLoopHVAC> airLoopHVAC;

  QDomElement primAirCondSysRefElement = thermalZoneElement.firstChildElement("PriAirCondgSysRef");
  QDomElement znSysElement = findZnSysElement(primAirCondSysRefElement.text(),doc);
  QDomElement ventSysRefElement = thermalZoneElement.firstChildElement("VentSysRef");

  // ThermalZoneVentilationSystem
  if( ventSysRefElement.text() != primAirCondSysRefElement.text() )
  {
    airLoopHVAC = model.getModelObjectByName<model::AirLoopHVAC>(ventSysRefElement.text().toStdString());

    if( airLoopHVAC && ! thermalZone.airLoopHVAC() )
    {
      QDomElement trmlUnitElement = findTrmlUnitElementForZone(nameElement.text(),doc);
      if( ! trmlUnitElement.isNull() ) {
        if( boost::optional<model::ModelObject> trmlUnit = translateTrmlUnit(trmlUnitElement,doc,model) )
        {
          airLoopHVAC->addBranchForZone(thermalZone,trmlUnit->cast<model::StraightComponent>());
          QDomElement inducedAirZnRefElement = trmlUnitElement.firstChildElement("InducedAirZnRef");
          if( boost::optional<model::ThermalZone> tz = model.getModelObjectByName<model::ThermalZone>(inducedAirZnRefElement.text().toStdString()) )
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

  // PrimaryAirConditioningSystemReference
  if( ! znSysElement.isNull() )
  {
    boost::optional<model::ModelObject> mo = translateZnSys(znSysElement,doc,model);

    if( mo )
    {
      boost::optional<model::ZoneHVACComponent> zoneHVACComponent = mo->optionalCast<model::ZoneHVACComponent>();

      if( zoneHVACComponent )
      {
        zoneHVACComponent->addToThermalZone(thermalZone);

        // If not the ventilation system we lock down the oa system of the zone equipment
        if( primAirCondSysRefElement.text() != ventSysRefElement.text() )
        {
          if( boost::optional<model::ZoneHVACPackagedTerminalAirConditioner> ptac = 
              zoneHVACComponent->optionalCast<model::ZoneHVACPackagedTerminalAirConditioner>() )
          {
            ptac->setOutdoorAirFlowRateDuringHeatingOperation(0.0);
            ptac->setOutdoorAirFlowRateDuringCoolingOperation(0.0);
            ptac->setOutdoorAirFlowRateWhenNoCoolingorHeatingisNeeded(0.0);
          }
          else if( boost::optional<model::ZoneHVACPackagedTerminalHeatPump> pthp = 
              zoneHVACComponent->optionalCast<model::ZoneHVACPackagedTerminalHeatPump>() )
          {
            pthp->setOutdoorAirFlowRateDuringHeatingOperation(0.0);
            pthp->setOutdoorAirFlowRateDuringCoolingOperation(0.0);
            pthp->setOutdoorAirFlowRateWhenNoCoolingorHeatingisNeeded(0.0);
          }
          else if( boost::optional<model::ZoneHVACFourPipeFanCoil> fanCoil = 
              zoneHVACComponent->optionalCast<model::ZoneHVACFourPipeFanCoil>() )
          {
            fanCoil->setMaximumOutdoorAirFlowRate(0.0);
          }
        }
      }
    }
  }
  else
  {
    airLoopHVAC = model.getModelObjectByName<model::AirLoopHVAC>(primAirCondSysRefElement.text().toStdString());

    if( airLoopHVAC && ! thermalZone.airLoopHVAC() )
    {
      QDomElement trmlUnitElement = findTrmlUnitElementForZone(nameElement.text(),doc);
      if( ! trmlUnitElement.isNull() ) {
        if( boost::optional<model::ModelObject> trmlUnit = translateTrmlUnit(trmlUnitElement,doc,model) )
        {
          airLoopHVAC->addBranchForZone(thermalZone,trmlUnit->cast<model::StraightComponent>());
          QDomElement inducedAirZnRefElement = trmlUnitElement.firstChildElement("InducedAirZnRef");
          if( boost::optional<model::ThermalZone> tz = model.getModelObjectByName<model::ThermalZone>(inducedAirZnRefElement.text().toStdString()) )
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

  // DCV?
  QDomElement ventCtrlMthdElement = thermalZoneElement.firstChildElement("VentCtrlMthd");
  if( istringEqual(ventCtrlMthdElement.text().toStdString(),"OccupantSensors") ||
      istringEqual(ventCtrlMthdElement.text().toStdString(),"CO2Sensors") )
  {
    if( airLoopHVAC )
    {
      if( boost::optional<model::AirLoopHVACOutdoorAirSystem> oaSystem = airLoopHVAC->airLoopHVACOutdoorAirSystem() )
      {
        oaSystem->getControllerOutdoorAir().controllerMechanicalVentilation().setDemandControlledVentilation(true);
      }
    }
  }

  // ClgTstatSchRef
  QDomElement clgTstatSchRefElement = thermalZoneElement.firstChildElement("ClgTstatSchRef");
  if (!clgTstatSchRefElement.isNull()){
    std::string scheduleName = escapeName(clgTstatSchRefElement.text());
    boost::optional<model::Schedule> schedule = model.getModelObjectByName<model::Schedule>(scheduleName);
    if (schedule){
      if (optionalThermostat){
        optionalThermostat->setCoolingSchedule(*schedule);
      }else{
        LOG(Error, "Cannot assign cooling schedule to unconditioned thermal zone '" << name << "'");
      }
    }else{
      LOG(Error, "Schedule named " << scheduleName << " cannot be found.");
    }
  }
  else
  {
    if (optionalThermostat){
      model::ScheduleRuleset scheduleRuleset = model::ScheduleRuleset(model);

      scheduleRuleset.setName(thermalZone.name().get() + " Default Cooling Schedule");

      model::ScheduleDay scheduleDay = scheduleRuleset.defaultDaySchedule();

      scheduleDay.addValue(Time(1.0),90.0);

      optionalThermostat->setCoolingSchedule(scheduleRuleset);
    }
  }

  // HtgTstatSchRef
  QDomElement htgTstatSchRefElement = thermalZoneElement.firstChildElement("HtgTstatSchRef");
  if (!htgTstatSchRefElement.isNull()){
    std::string scheduleName = escapeName(htgTstatSchRefElement.text());
    boost::optional<model::Schedule> schedule = model.getModelObjectByName<model::Schedule>(scheduleName);
    if (schedule){
      if (optionalThermostat){
        optionalThermostat->setHeatingSchedule(*schedule);
      }else{
        LOG(Error, "Cannot assign heating schedule to unconditioned thermal zone '" << name << "'");
      }
    }else{
      LOG(Error, "Schedule named " << scheduleName << " cannot be found.");
    }
  }
  else
  {
    if (optionalThermostat){
      model::ScheduleRuleset scheduleRuleset = model::ScheduleRuleset(model);

      scheduleRuleset.setName(thermalZone.name().get() + " Default Heating Schedule");

      model::ScheduleDay scheduleDay = scheduleRuleset.defaultDaySchedule();

      scheduleDay.addValue(Time(1.0),-100.0);

      optionalThermostat->setHeatingSchedule(scheduleRuleset);
    }
  }

  airLoopHVAC = thermalZone.airLoopHVAC();

  // Connect to plenum(s) if required
  if( airLoopHVAC )
  {
    QDomElement rtnPlenumZnRefElement = thermalZoneElement.firstChildElement("RetPlenumZnRef");
    boost::optional<model::ThermalZone> returnPlenumZone;
    returnPlenumZone = model.getModelObjectByName<model::ThermalZone>(rtnPlenumZnRefElement.text().toStdString()); 
    if( returnPlenumZone )
    {
      thermalZone.setReturnPlenum(returnPlenumZone.get());  
    }

    QDomElement supPlenumZnRefElement = thermalZoneElement.firstChildElement("SupPlenumZnRef");
    boost::optional<model::ThermalZone> supplyPlenumZone;
    supplyPlenumZone = model.getModelObjectByName<model::ThermalZone>(supPlenumZnRefElement.text().toStdString());
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
    QDomElement airSystemElement = findAirSysElement(QString::fromStdString(airLoopHVAC->name().get()),doc);
    if( spm && ! airSystemElement.isNull() )
    {
      QDomElement ctrlZnRefElement = airSystemElement.firstChildElement("CtrlZnRef");
      if( istringEqual(ctrlZnRefElement.text().toStdString(),thermalZone.name().get()) )
      {
        spm->setControlZone(thermalZone);
      }
    }
  }

  return result;
}

boost::optional<model::ModelObject> ReverseTranslator::translateTrmlUnit(const QDomElement& trmlUnitElement, 
                                                                         const QDomDocument& doc, 
                                                                         openstudio::model::Model& model)
{
  boost::optional<openstudio::model::ModelObject> result;
  bool ok;
  double value;

  if( ! istringEqual(trmlUnitElement.tagName().toStdString(),"TrmlUnit") )
  {
    return result;
  }

  // Name
  QDomElement nameElement = trmlUnitElement.firstChildElement("Name");

  // AvailSchRef
  QDomElement availSchRefElement = trmlUnitElement.firstChildElement("AvailSchRef");
  boost::optional<model::Schedule> availSch = model.getModelObjectByName<model::Schedule>(availSchRefElement.text().toStdString());

  // Type
  QDomElement typeElement = trmlUnitElement.firstChildElement("Type");

  // PriAirFlow
  QDomElement priAirFlowElement = trmlUnitElement.firstChildElement("PriAirFlowMaxSim");
  QDomElement priAirFlowMinElement = trmlUnitElement.firstChildElement("PriAirFlowMinSim");
  boost::optional<double> primaryAirFlow;
  boost::optional<double> primaryAirFlowMin;

  if( ! autosize() )
  {
    value = priAirFlowElement.text().toDouble(&ok);
    if( ok ) 
    {
      primaryAirFlow = unitToUnit(value,"cfm","m^3/s").get();
    }
  }

  value = priAirFlowMinElement.text().toDouble(&ok);
  if( ok )
  {
    primaryAirFlowMin = unitToUnit(value,"cfm","m^3/s").get();
  }

  QDomElement airSysElement = trmlUnitElement.parentNode().toElement();
  QDomElement airSystemTypeElement;
  if( ! airSysElement.isNull() )
  {
    airSystemTypeElement = airSysElement.firstChildElement("Type");
  }

  if( istringEqual("VAVNoReheatBox",typeElement.text().toStdString()) )
  {
    model::Schedule schedule = alwaysOnSchedule(model);

    model::AirTerminalSingleDuctVAVNoReheat terminal(model,schedule);

    QDomElement minAirFracSchRefElement = trmlUnitElement.firstChildElement("MinAirFracSchRef");
    if( boost::optional<model::Schedule> minAirFracSch = model.getModelObjectByName<model::Schedule>(minAirFracSchRefElement.text().toStdString()) )
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

    result = terminal;
  }
  else if( istringEqual("VAVReheatBox",typeElement.text().toStdString()) )
  {
    model::Schedule schedule = alwaysOnSchedule(model);

    QDomElement coilElement = trmlUnitElement.firstChildElement("CoilHtg");

    boost::optional<model::HVACComponent> coil;

    if( ! coilElement.isNull() )
    {
      boost::optional<model::ModelObject> mo = translateCoilHeating(coilElement,doc,model);

      OS_ASSERT(mo);

      coil = mo->cast<model::HVACComponent>();
    }
    else // If no coil is specified in the BEC, create an electric coil
    {
      coil = model::CoilHeatingElectric(model,schedule);
    }

    OS_ASSERT(coil);

    model::AirTerminalSingleDuctVAVReheat terminal(model,schedule,coil.get());

    QDomElement minAirFracSchRefElement = trmlUnitElement.firstChildElement("MinAirFracSchRef");
    if( boost::optional<model::Schedule> minAirFracSch = model.getModelObjectByName<model::Schedule>(minAirFracSchRefElement.text().toStdString()) )
    {
      terminal.setZoneMinimumAirFlowMethod("Scheduled");
      terminal.setMinimumAirFlowFractionSchedule(minAirFracSch.get());
    }
    else if( primaryAirFlowMin )
    {
      terminal.setZoneMinimumAirFlowMethod("FixedFlowRate");
      terminal.setFixedMinimumAirFlowRate(primaryAirFlowMin.get());
    }
    else
    {
      terminal.setZoneMinimumAirFlowMethod("Constant");
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
    QDomElement reheatCtrlMthdElement = trmlUnitElement.firstChildElement("ReheatCtrlMthd");
    if( istringEqual(reheatCtrlMthdElement.text().toStdString(),"DualMaximum") &&
        coil->optionalCast<model::CoilHeatingWater>() )
    {
      terminal.setDamperHeatingAction("Reverse");
    }
    else
    {
      terminal.setDamperHeatingAction("Normal");
    }

    QDomElement htgAirFlowMaxElement = trmlUnitElement.firstChildElement("HtgAirFlowMaxSim");
    value = htgAirFlowMaxElement.text().toDouble(&ok);
    if( ok && primaryAirFlow )
    {
       value = unitToUnit(value,"cfm","m^3/s").get();
       double fraction = value / primaryAirFlow.get();
       terminal.setMaximumFlowFractionDuringReheat(fraction);
    }
    else
    {
      bool found = false;

      QDomElement zoneServedElement = trmlUnitElement.firstChildElement("ZnServedRef");

      QDomNodeList thrmlZnElements = trmlUnitElement.parentNode().parentNode().parentNode().toElement().elementsByTagName("ThrmlZn");

      for( int j = 0; j < thrmlZnElements.count(); j++ )
      {
        QDomElement thrmlZnElement = thrmlZnElements.at(j).toElement();

        QDomElement thrmlZnNameElement = thrmlZnElement.firstChildElement("Name");

        if(istringEqual(thrmlZnNameElement.text().toStdString(),zoneServedElement.text().toStdString()))
        {
          QDomElement htgDsgnMaxFlowFracElement = thrmlZnElement.firstChildElement("HtgDsgnMaxFlowFrac");

          value = htgDsgnMaxFlowFracElement.text().toDouble(&ok);

          if( ok )
          {
            terminal.setMaximumFlowFractionDuringReheat(value);

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

    result = terminal;
  }
  else if( istringEqual("SeriesFanBox",typeElement.text().toStdString()) )
  {
    model::Schedule schedule = alwaysOnSchedule(model);

    // CoilHtg
    QDomElement coilHtgElement = trmlUnitElement.firstChildElement("CoilHtg");
    boost::optional<model::ModelObject> coil;
    coil = translateCoilHeating(coilHtgElement,doc,model);
    if( ! coil )
    {
      coil = model::CoilHeatingElectric(model,schedule);
    }
    model::HVACComponent hvacComponentCoil = coil->cast<model::HVACComponent>();

    // Fan
    QDomElement fanElement = trmlUnitElement.firstChildElement("Fan");
    boost::optional<model::ModelObject> fan;
    fan = translateFan(fanElement,doc,model);
    if( ! fan )
    {
      fan = model::FanConstantVolume(model,schedule);
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
  else if( istringEqual("ParallelFanBox",typeElement.text().toStdString()) )
  {
    model::Schedule schedule = alwaysOnSchedule(model);

    // CoilHtg

    QDomElement coilHtgElement = trmlUnitElement.firstChildElement("CoilHtg");

    boost::optional<model::ModelObject> coil;

    coil = translateCoilHeating(coilHtgElement,doc,model);

    if( ! coil )
    {
      coil = model::CoilHeatingElectric(model,schedule);
    }

    model::HVACComponent hvacComponentCoil = coil->cast<model::HVACComponent>();

    // Fan

    QDomElement fanElement = trmlUnitElement.firstChildElement("Fan");

    boost::optional<model::ModelObject> fan;

    fan = translateFan(fanElement,doc,model);

    if( ! fan )
    {
      fan = model::FanConstantVolume(model,schedule);
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

    QDomElement parallelBoxFanFlowFracElement = trmlUnitElement.firstChildElement("ParallelBoxFanFlowFrac");

    value = parallelBoxFanFlowFracElement.text().toDouble(&ok);

    if( ok )
    {
      terminal.setFanOnFlowFraction(value);
    }

    result = terminal;
  }
  else if( istringEqual("Uncontrolled",typeElement.text().toStdString()) )
  {
    model::Schedule schedule = alwaysOnSchedule(model);

    model::AirTerminalSingleDuctUncontrolled terminal(model,schedule);

    // PriAirFlow
    if( primaryAirFlow )
    {
      terminal.setMaximumAirFlowRate(primaryAirFlow.get());
    }

    result = terminal;
  }
  else
  {
    model::Schedule schedule = alwaysOnSchedule(model);

    model::AirTerminalSingleDuctUncontrolled terminal(model,schedule);

    result = terminal;
  }

  if( result )
  {
    result->setName(nameElement.text().toStdString());
  }

  return result;
}

boost::optional<openstudio::model::ModelObject> ReverseTranslator::translateFluidSys(
                                                  const QDomElement& fluidSysElement, 
                                                  const QDomDocument& doc,
                                                  openstudio::model::Model& model )
{
  boost::optional<openstudio::model::ModelObject> result;

  double value;

  bool ok;

  if( ! istringEqual(fluidSysElement.tagName().toStdString(),"FluidSys") )
  {
    return result;
  }

  QDomElement nameElement = fluidSysElement.firstChildElement("Name");

  if( boost::optional<model::PlantLoop> plant = model.getModelObjectByName<model::PlantLoop>(nameElement.text().toStdString()) )
  {
    return plant.get();
  }

  model::PlantLoop plantLoop(model);

  model::Node supplyInletNode = plantLoop.supplyInletNode();
  model::Node supplyOutletNode = plantLoop.supplyOutletNode();

  bool bypass = false;

  // Name

  plantLoop.setName(nameElement.text().toStdString());

  model::SizingPlant sizingPlant = plantLoop.sizingPlant();

  // Type

  QDomElement typeElement = fluidSysElement.firstChildElement("Type");

  // DsgnSupWtrTemp
  QDomElement dsgnSupWtrTempElement = fluidSysElement.firstChildElement("DsgnSupWtrTemp");
  value = dsgnSupWtrTempElement.text().toDouble(&ok);
  if( ok )
  {
    value = unitToUnit(value,"F","C").get();

    sizingPlant.setDesignLoopExitTemperature(value);
  }

  // DsgnSupWtrDelT 
  QDomElement dsgnSupWtrDelTElement = fluidSysElement.firstChildElement("DsgnSupWtrDelT");
  value = dsgnSupWtrDelTElement.text().toDouble(&ok);
  if( ok )
  {
    value = value / 1.8;
    sizingPlant.setLoopDesignTemperatureDifference(value);
  }

  if( istringEqual(typeElement.text().toStdString(),"HotWater") )
  {
    sizingPlant.setLoopType("Heating");
  }
  else if( istringEqual(typeElement.text().toStdString(),"ChilledWater") )
  {
    sizingPlant.setLoopType("Cooling");
  }
  else if( istringEqual(typeElement.text().toStdString(),"CondenserWater") )
  {
    sizingPlant.setLoopType("Condenser");
  }

  // Boilers

  QDomNodeList boilerElements = fluidSysElement.elementsByTagName("Blr");

  for (int i = 0; i < boilerElements.count(); i++)
  {
    QDomElement boilerElement = boilerElements.at(i).toElement();

    if( boost::optional<model::ModelObject> mo = translateBoiler(boilerElement,doc,model) )
    {
      model::BoilerHotWater boiler = mo->cast<model::BoilerHotWater>();

      plantLoop.addSupplyBranchForComponent(boiler);

      QDomElement pumpElement = boilerElement.firstChildElement("Pump"); 

      if( ! pumpElement.isNull() )
      {
        boost::optional<model::ModelObject> mo2 = translatePump(pumpElement,doc,model);

        if( mo2 )
        {
          model::Node inletNode = boiler.inletModelObject()->cast<model::Node>();

          if( boost::optional<model::PumpVariableSpeed> pump = mo2->optionalCast<model::PumpVariableSpeed>() )
          {
            pump->addToNode(inletNode);
          }
          else if( boost::optional<model::PumpConstantSpeed> pump = mo2->optionalCast<model::PumpConstantSpeed>() )
          {
            pump->addToNode(inletNode);
          }
        }
      }
    }
  }

  // Chillers

  QDomNodeList chillerElements = fluidSysElement.elementsByTagName("Chlr");

  for (int i = 0; i < chillerElements.count(); i++)
  {
    QDomElement chillerElement = chillerElements.at(i).toElement();

    if( boost::optional<model::ModelObject> mo = translateChiller(chillerElement,doc,model) )
    {
      model::ChillerElectricEIR chiller = mo->cast<model::ChillerElectricEIR>();

      plantLoop.addSupplyBranchForComponent(chiller);

      QDomElement pumpElement = chillerElement.firstChildElement("Pump"); 

      if( ! pumpElement.isNull() )
      {
        boost::optional<model::ModelObject> mo2 = translatePump(pumpElement,doc,model);

        if( mo2 )
        {
          model::Node inletNode = chiller.supplyInletModelObject()->cast<model::Node>();

          if( boost::optional<model::PumpVariableSpeed> pump = mo2->optionalCast<model::PumpVariableSpeed>() )
          {
            pump->addToNode(inletNode);
          }
          else if( boost::optional<model::PumpConstantSpeed> pump = mo2->optionalCast<model::PumpConstantSpeed>() )
          {
            pump->addToNode(inletNode);
          }
        }
      }

      QDomElement evapHasBypassElement = chillerElement.firstChildElement("EvapHasBypass");

      if( evapHasBypassElement.text() == "1" )
      {
        bypass = true;
      }
    }
  }

  // HtRej

  QDomNodeList htRejElements = fluidSysElement.elementsByTagName("HtRej");

  for(int i = 0; i < htRejElements.count(); i++)
  {
    QDomElement htRejElement = htRejElements.at(i).toElement();

    if( boost::optional<model::ModelObject> mo = translateHtRej(htRejElement,doc,model) )
    {
      model::StraightComponent tower = mo->cast<model::StraightComponent>();

      plantLoop.addSupplyBranchForComponent(tower);

      QDomElement pumpElement = htRejElement.firstChildElement("Pump"); 

      if( ! pumpElement.isNull() )
      {
        boost::optional<model::ModelObject> mo2 = translatePump(pumpElement,doc,model);

        if( mo2 )
        {
          model::Node inletNode = tower.inletModelObject()->cast<model::Node>();

          if( boost::optional<model::PumpVariableSpeed> pump = mo2->optionalCast<model::PumpVariableSpeed>() )
          {
            pump->addToNode(inletNode);
          }
          else if( boost::optional<model::PumpConstantSpeed> pump = mo2->optionalCast<model::PumpConstantSpeed>() )
          {
            pump->addToNode(inletNode);
          }
        }
      }
    }
  }

  // Water Heaters

  QDomNodeList wtrHtrElements = fluidSysElement.elementsByTagName("WtrHtr");

  for (int i = 0; i < wtrHtrElements.count(); i++)
  {
    QDomElement wtrHtrElement = wtrHtrElements.at(i).toElement();

    if( boost::optional<model::ModelObject> mo = translateWtrHtr(wtrHtrElement,doc,model) )
    {
      plantLoop.addSupplyBranchForComponent(mo->cast<model::HVACComponent>());
    }
  }

  // Add a default bypass

  if(bypass)
  {
    model::PipeAdiabatic pipe(model);

    plantLoop.addSupplyBranchForComponent(pipe);
  }

  // Add a default hot water heater for servicehotwater systems

  if( typeElement.text().toLower() == "servicehotwater" )
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

      spm.addToNode(supplyOutletNode);
    }

    if( waterHeaters.size() > 0 )
    {
      model::PumpVariableSpeed pumpVariableSpeed(model);

      model::Node supplyInletNode = plantLoop.supplyInletNode();

      pumpVariableSpeed.addToNode(supplyInletNode);

      pumpVariableSpeed.setRatedPumpHead(0.0);
    }
  }


  QDomNodeList fluidSegElements = fluidSysElement.elementsByTagName("FluidSeg");

  for (int i = 0; i < fluidSegElements.count(); i++)
  {
    QDomElement fluidSegElement = fluidSegElements.at(i).toElement();

    QDomElement typeElement = fluidSegElement.firstChildElement("Type");

    // Translate Secondary Supply
    
    if( typeElement.text().toLower() == "secondarysupply" )
    {
      QDomElement pumpElement = fluidSegElement.firstChildElement("Pump");

      if( ! pumpElement.isNull() )
      {
        boost::optional<model::ModelObject> mo = translatePump(pumpElement,doc,model);

        if( mo )
        {
          model::Node demandInletNode = plantLoop.demandInletNode();

          mo->cast<model::HVACComponent>().addToNode(demandInletNode);

          plantLoop.setCommonPipeSimulation("CommonPipe");
        }
      }
    }
    
    // Translate Primary Supply
    
    if( typeElement.text().toLower() == "primaryreturn" || 
        typeElement.text().toLower() == "primarysupply"
      )
    {
      QDomElement pumpElement = fluidSegElement.firstChildElement("Pump");

      if( ! pumpElement.isNull() )
      {
        boost::optional<model::ModelObject> mo = translatePump(pumpElement,doc,model);

        if( mo )
        {
          model::Node supplyInletNode = plantLoop.supplyInletNode();

          mo->cast<model::HVACComponent>().addToNode(supplyInletNode);
        }
      }
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
        for( std::vector<model::ModelObject>::iterator it = constantPumps.begin();
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
        for( std::vector<model::ModelObject>::iterator it = variablePumps.begin();
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

  // TempCtrl
  QDomElement tempCtrlElement = fluidSysElement.firstChildElement("TempCtrl");

  // Fixed
  if( istringEqual(tempCtrlElement.text().toStdString(),"Fixed") )
  {
    QDomElement fixedSupTempElement = fluidSysElement.firstChildElement("FixedSupTemp");

    double fixedSupTemp = 21.1;

    value = fixedSupTempElement.text().toDouble(&ok);

    if( ok )
    {
      fixedSupTemp = unitToUnit(value,"F","C").get();
    }
    else
    {
      QDomElement dsgnSupWtrTempElement = fluidSysElement.firstChildElement("DsgnSupWtrTemp");
      value = dsgnSupWtrTempElement.text().toDouble(&ok);
      if( ok )
      {
        fixedSupTemp = unitToUnit(value,"F","C").get();

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
  else if( istringEqual(tempCtrlElement.text().toStdString(),"Scheduled") )
  {
    QDomElement tempSetPtSchRefElement = fluidSysElement.firstChildElement("TempSetptSchRef");

    boost::optional<model::Schedule> schedule = model.getModelObjectByName<model::Schedule>(tempSetPtSchRefElement.text().toStdString());

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
  else if( istringEqual(tempCtrlElement.text().toStdString(),"WetBulbReset") )
  {
    model::SetpointManagerFollowOutdoorAirTemperature spm(model);

    spm.setReferenceTemperatureType("OutdoorAirWetBulb");

    spm.addToNode(supplyOutletNode);

    boost::optional<double> rstSupHi;
    boost::optional<double> rstSupLow;

    // RstSupHi
    QDomElement rstSupHiElement = fluidSysElement.firstChildElement("RstSupHi");
    value = rstSupHiElement.text().toDouble(&ok);
    if( ok )
    {
      rstSupHi = unitToUnit(value,"F","C").get();
    }

    // RstSupLow
    QDomElement rstSupLoElement = fluidSysElement.firstChildElement("RstSupLow");
    value = rstSupLoElement.text().toDouble(&ok);
    if( ok )
    {
      rstSupLow = unitToUnit(value,"F","C").get();
    }

    if( rstSupLow && rstSupHi )
    {
      spm.setMinimumSetpointTemperature(rstSupLow.get());

      spm.setMaximumSetpointTemperature(rstSupHi.get());
    }

    // WetBulbApproach
    QDomElement wetBulbApproachElement = fluidSysElement.firstChildElement("WetBulbApproach");
    value = wetBulbApproachElement.text().toDouble(&ok);
    if( ok )
    {
      spm.setOffsetTemperatureDifference(value * 5.0 / 9.0);
    }
  }
  else if( istringEqual(tempCtrlElement.text().toStdString(),"OutsideAirReset") )
  {
    model::SetpointManagerOutdoorAirReset spm(model);

    spm.addToNode(supplyOutletNode);

    boost::optional<double> rstSupHi;
    boost::optional<double> rstSupLow;
    boost::optional<double> rstOutdrHi;
    boost::optional<double> rstOutdrLow;

    // RstSupHi
    QDomElement rstSupHiElement = fluidSysElement.firstChildElement("RstSupHi");
    value = rstSupHiElement.text().toDouble(&ok);
    if( ok )
    {
      rstSupHi = unitToUnit(value,"F","C").get();
    }

    // RstSupLow
    QDomElement rstSupLoElement = fluidSysElement.firstChildElement("RstSupLow");
    value = rstSupLoElement.text().toDouble(&ok);
    if( ok )
    {
      rstSupLow = unitToUnit(value,"F","C").get();
    }

    // RstOutdrHi
    QDomElement rstOutDrHiElement = fluidSysElement.firstChildElement("RstOutdrHi");
    value = rstOutDrHiElement.text().toDouble(&ok);
    if( ok )
    {
      rstOutdrHi = unitToUnit(value,"F","C").get();
    }

    // RstOutdrLow
    QDomElement rstOutDrLoElement = fluidSysElement.firstChildElement("RstOutdrLow");
    value = rstOutDrLoElement.text().toDouble(&ok);
    if( ok )
    {
      rstOutdrLow = unitToUnit(value,"F","C").get();
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
      LOG(Warn,nameElement.text().toStdString() << " requests OA reset control, but does not define setpoints."
        << "  Using OpenStudio defaults.");
    }
  }
  else if( istringEqual(tempCtrlElement.text().toStdString(),"LoadReset") )
  {
    QDomElement dsgnSupWtrTempElement = fluidSysElement.firstChildElement("DsgnSupWtrTemp");
    value = dsgnSupWtrTempElement.text().toDouble(&ok);
    if( ok )
    {
      value = unitToUnit(value,"F","C").get();

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
  else if( istringEqual(tempCtrlElement.text().toStdString(),"FixedDualSetpoint") )
  {
    // It would be better to use setpoint manager dual setpoint, but
    // until it is available we use component setpoint operation and but 
    // scheduled SPMs on the heating and cooling components.

    QDomElement fixedSupTempElement = fluidSysElement.firstChildElement("FixedSupTemp");
    double fixedSupTemp = 10.0;
    value = fixedSupTempElement.text().toDouble(&ok);
    if( ok )
    {
      fixedSupTemp = unitToUnit(value,"F","C").get();
    }

    QDomElement htgFixedSupTempElement = fluidSysElement.firstChildElement("HtgFixedSupTemp");
    double htgFixedSupTemp = 26.7;
    value = htgFixedSupTempElement.text().toDouble(&ok);
    if( ok )
    {
      htgFixedSupTemp = unitToUnit(value,"F","C").get();
    }

    model::ScheduleRuleset schedule(model);
    schedule.setName(plantLoop.name().get() + " Supply Temp Schedule");
    model::ScheduleDay scheduleDay = schedule.defaultDaySchedule();
    scheduleDay.addValue(Time(1.0),fixedSupTemp);

    model::ScheduleRuleset heatingSchedule(model);
    heatingSchedule.setName(plantLoop.name().get() + " Heating Supply Temp Schedule");
    model::ScheduleDay heatingScheduleDay = heatingSchedule.defaultDaySchedule();
    heatingScheduleDay.addValue(Time(1.0),htgFixedSupTemp);

    model::SetpointManagerScheduled spm(model,schedule);
    spm.setName(plantLoop.name().get() + " Supply Outlet SPM");
    spm.addToNode(supplyOutletNode);

    // "Heating" components
    std::vector<model::BoilerHotWater> boilers = 
    subsetCastVector<model::BoilerHotWater>(plantLoop.supplyComponents(model::BoilerHotWater::iddObjectType()));
    for(std::vector<model::BoilerHotWater>::iterator it = boilers.begin();
        it != boilers.end();
        ++it)
    {
      boost::optional<model::ModelObject> mo = it->outletModelObject();
      OS_ASSERT(mo);
      boost::optional<model::Node> node = mo->optionalCast<model::Node>();
      OS_ASSERT(node);

      model::SetpointManagerScheduled spm2(model,heatingSchedule);
      spm2.setName(it->name().get() + " SPM");
      spm2.addToNode(node.get());
    }

    std::vector<model::WaterHeaterMixed> waterHeaters =
    subsetCastVector<model::WaterHeaterMixed>(plantLoop.supplyComponents(model::WaterHeaterMixed::iddObjectType()));
    for(std::vector<model::WaterHeaterMixed>::iterator it = waterHeaters.begin();
        it != waterHeaters.end();
        ++it)
    {
      boost::optional<model::ModelObject> mo = it->supplyOutletModelObject();
      OS_ASSERT(mo);
      boost::optional<model::Node> node = mo->optionalCast<model::Node>();
      OS_ASSERT(node);

      model::SetpointManagerScheduled spm2(model,heatingSchedule);
      spm2.setName(it->name().get() + " SPM");
      spm2.addToNode(node.get());
    }

    // "Cooling" components
    std::vector<model::ChillerElectricEIR> chillers =
    subsetCastVector<model::ChillerElectricEIR>(plantLoop.supplyComponents(model::ChillerElectricEIR::iddObjectType()));
    for(std::vector<model::ChillerElectricEIR>::iterator it = chillers.begin();
        it != chillers.end();
        ++it)
    {
      boost::optional<model::ModelObject> mo = it->supplyOutletModelObject();
      OS_ASSERT(mo);
      boost::optional<model::Node> node = mo->optionalCast<model::Node>();
      OS_ASSERT(node);

      model::SetpointManagerScheduled spm2(model,schedule);
      spm2.setName(it->name().get() + " SPM");
      spm2.addToNode(node.get());
    }

    std::vector<model::CoolingTowerVariableSpeed> variableTowers =
    subsetCastVector<model::CoolingTowerVariableSpeed>(plantLoop.supplyComponents(model::CoolingTowerVariableSpeed::iddObjectType()));
    for(std::vector<model::CoolingTowerVariableSpeed>::iterator it = variableTowers.begin();
        it != variableTowers.end();
        ++it)
    {
      boost::optional<model::ModelObject> mo = it->outletModelObject();
      OS_ASSERT(mo);
      boost::optional<model::Node> node = mo->optionalCast<model::Node>();
      OS_ASSERT(node);

      model::SetpointManagerScheduled spm2(model,schedule);
      spm2.setName(it->name().get() + " SPM");
      spm2.addToNode(node.get());
    }

    std::vector<model::CoolingTowerSingleSpeed> constantTowers =
    subsetCastVector<model::CoolingTowerSingleSpeed>(plantLoop.supplyComponents(model::CoolingTowerSingleSpeed::iddObjectType()));
    for(std::vector<model::CoolingTowerSingleSpeed>::iterator it = constantTowers.begin();
        it != constantTowers.end();
        ++it)
    {
      boost::optional<model::ModelObject> mo = it->outletModelObject();
      OS_ASSERT(mo);
      boost::optional<model::Node> node = mo->optionalCast<model::Node>();
      OS_ASSERT(node);

      model::SetpointManagerScheduled spm2(model,schedule);
      spm2.setName(it->name().get() + " SPM");
      spm2.addToNode(node.get());
    }
  }
  else
  {
    LOG(Error,plantLoop.name().get() << " does not have a setpoint.");
  }
  

  return plantLoop;
}

boost::optional<openstudio::model::ModelObject> ReverseTranslator::translatePump(
                                                  const QDomElement& pumpElement, 
                                                  const QDomDocument& doc,
                                                  openstudio::model::Model& model )
{
  boost::optional<openstudio::model::ModelObject> result;

  double value;

  bool ok;

  boost::optional<double> mtrEff;

  QDomElement mtrEffElement = pumpElement.firstChildElement("MtrEff");
  value = mtrEffElement.text().toDouble(&ok);
  if( ok )
  {
    mtrEff = value;
  }

  if( ! istringEqual(pumpElement.tagName().toStdString(),"Pump") )
  {
    return result;
  }

  QDomElement spdCtrlElement = pumpElement.firstChildElement("SpdCtrl");
  
  if( spdCtrlElement.text().toLower() == "constantspeed" )
  {
    model::PumpConstantSpeed pump(model);

    pump.setRatedPumpHead(149453.0);

    if( mtrEff )
    {
      pump.setMotorEfficiency(mtrEff.get());
    }

    if( autosize() )
    {
      QDomElement totHdElement = pumpElement.firstChildElement("TotHd");
      value = totHdElement.text().toDouble(&ok);
      if( ok )
      {
        // ft water to Pa
        double totHd = value * 2989.067;
        pump.setRatedPumpHead(totHd);
      }
    }
    else
    {
      boost::optional<double> flowCap;
      boost::optional<double> pwr;

      QDomElement flowCapElement = pumpElement.firstChildElement("FlowCap");
      value = flowCapElement.text().toDouble(&ok);
      if( ok )
      {
        flowCap = unitToUnit(value, "gal/min", "m^3/s").get();
      }

      QDomElement pwrElement = pumpElement.firstChildElement("Pwr");
      value = pwrElement.text().toDouble(&ok);
      if( ok )
      {
        // kW to W
        pwr = value * 1000.0;
      }

      if( flowCap && pwr )
      {
        if( equal(flowCap.get(),0.0) || equal(pwr.get(),0.0)  )
        {
          LOG(Warn,pump.name().get() << " has 0 capacity specified.");
        }
        else
        {
          pump.setRatedFlowRate(flowCap.get());
          pump.setRatedPowerConsumption(pwr.get());
          pump.setRatedPumpHead(0.80 * pwr.get() / flowCap.get());
        }
      }
    }

    result = pump;
  }
  else
  {
    model::PumpVariableSpeed pump(model);

    pump.setRatedPumpHead(149453.0);

    if( mtrEff )
    {
      pump.setMotorEfficiency(mtrEff.get());
    }

    if( autosize() )
    {
      QDomElement totHdElement = pumpElement.firstChildElement("TotHd");
      value = totHdElement.text().toDouble(&ok);
      if( ok )
      {
        // ft water to Pa
        double totHd = value * 2989.067;
        pump.setRatedPumpHead(totHd);
      }
    }
    else
    {
      boost::optional<double> flowCap;
      boost::optional<double> flowMin;
      boost::optional<double> pwr;

      QDomElement flowCapElement = pumpElement.firstChildElement("FlowCap");
      value = flowCapElement.text().toDouble(&ok);
      if( ok )
      {
        flowCap = unitToUnit(value, "gal/min", "m^3/s");
      }

      QDomElement flowMinElement = pumpElement.firstChildElement("FlowMin");
      value = flowMinElement.text().toDouble(&ok);
      if( ok )
      {
        flowMin = unitToUnit(value, "gal/min", "m^3/s");
      }

      QDomElement pwrElement = pumpElement.firstChildElement("Pwr");
      value = pwrElement.text().toDouble(&ok);
      if( ok )
      {
        // kW to W
        pwr = value * 1000.0;
      }

      if( flowMin && flowCap && pwr )
      {
        if( equal(flowCap.get(),0.0) || equal(pwr.get(),0.0)  )
        {
          LOG(Warn,pump.name().get() << " has 0 capacity specified.");
        }
        else
        {
          pump.setRatedFlowRate(flowCap.get());
          pump.setMinimumFlowRate(flowMin.get());
          pump.setRatedPowerConsumption(pwr.get());
          pump.setRatedPumpHead(0.80 * pwr.get() / flowCap.get());
        }
      }
    }

    // Pwr_fPLRCrvRef

    boost::optional<model::CurveCubic> pwr_fPLRCrv;
    QDomElement pwr_fPLRCrvRefElement = pumpElement.firstChildElement("Pwr_fPLRCrvRef");
    pwr_fPLRCrv = model.getModelObjectByName<model::CurveCubic>(pwr_fPLRCrvRefElement.text().toStdString());

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

    // TODO Figure out a way to set a more realistic minimum
    LOG(Warn,pump.name().get() << " ignores minimum flow specification from BEC, defaulting to 0.");
    pump.setMinimumFlowRate(0.0);

    result = pump;
  }

  if( result )
  {
    QDomElement nameElement = pumpElement.firstChildElement("Name");

    result->setName(nameElement.text().toStdString());
  }

  return result;
}

boost::optional<openstudio::model::ModelObject> ReverseTranslator::translateBoiler(
                                                  const QDomElement& boilerElement, 
                                                  const QDomDocument& doc,
                                                  openstudio::model::Model& model )
{
  boost::optional<openstudio::model::ModelObject> result;

  if( ! istringEqual(boilerElement.tagName().toStdString(),"Blr") )
  {
    return result;
  }

  model::BoilerHotWater boiler(model);

  // Name

  QDomElement nameElement = boilerElement.firstChildElement("Name");

  boiler.setName(nameElement.text().toStdString());

  // ThrmlEff

  QDomElement thrmlEffElement = boilerElement.firstChildElement("ThrmlEff");

  boiler.setNominalThermalEfficiency(thrmlEffElement.text().toDouble());

  // HIR_fPLRCrvRef

  boost::optional<model::Curve> hirfPLRCrv;
  QDomElement hirfPLRCrvRefElement = boilerElement.firstChildElement("HIR_fPLRCrvRef");
  hirfPLRCrv = model.getModelObjectByName<model::Curve>(hirfPLRCrvRefElement.text().toStdString());
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

  QDomElement fuelSrcElement = boilerElement.firstChildElement("FuelSrc");
  QString fuelSrc = fuelSrcElement.text().toLower();

  if( fuelSrc == "electric" )
  {
    boiler.setFuelType("Electricity");
  }
  else if ( fuelSrc == "gas" )
  {
    boiler.setFuelType("NaturalGas");
  }
  else if( fuelSrc == "oil" )
  {
    boiler.setFuelType("FuelOil#2");
  }

  // ParasiticLd
  
  QDomElement parasiticLdElement = boilerElement.firstChildElement("ParasiticLd");

  bool ok;

  double parasiticLd = parasiticLdElement.text().toDouble(&ok);

  if( ok )
  {
    boiler.setParasiticElectricLoad(unitToUnit(parasiticLd,"Btu/h","W").get());
  }

  if( ! autosize() )
  {
    // CapRtd
    boost::optional<double> capRtd;
    QDomElement capRtdElement = boilerElement.firstChildElement("CapRtd");
    double value = capRtdElement.text().toDouble(&ok);
    if( ok )
    {
      capRtd = unitToUnit(value,"Btu/h","W");
    }

    // EntTempDsgn
    boost::optional<double> entTempDsgn;
    QDomElement entTempDsgnElement = boilerElement.firstChildElement("EntTempDsgn");
    value = entTempDsgnElement.text().toDouble(&ok);
    if( ok )
    {
      entTempDsgn = unitToUnit(value,"F","C");
    }

    // LvgTempDsgn
    boost::optional<double> lvgTempDsgn;
    QDomElement lvgTempDsgnElement = boilerElement.firstChildElement("LvgTempDsgn");
    value = lvgTempDsgnElement.text().toDouble(&ok);
    if( ok )
    {
      lvgTempDsgn = unitToUnit(value,"F","C");
    }

    if( capRtd && entTempDsgn && lvgTempDsgn )
    {
      boiler.setNominalCapacity(capRtd.get());

      double flow = capRtd.get() / ( cpWater * densityWater * (lvgTempDsgn.get() - entTempDsgn.get()));

      boiler.setDesignWaterFlowRate(flow);
    }
  }

  return boiler;
}

boost::optional<openstudio::model::ModelObject> ReverseTranslator::translateHtRej(
                                                  const QDomElement& htRejElement, 
                                                  const QDomDocument& doc,
                                                  openstudio::model::Model& model )
{
  boost::optional<openstudio::model::ModelObject> result;

  bool ok;
  double value;

  if( ! istringEqual(htRejElement.tagName().toStdString(),"HtRej") )
  {
    return result;
  }

  // ModCtrl

  QDomElement modCtrlElement = htRejElement.firstChildElement("ModCtrl");

  if( modCtrlElement.text().compare("VariableSpeedDrive",Qt::CaseInsensitive) == 0 )
  {
    model::CoolingTowerVariableSpeed tower(model);

    QDomElement wetBulbApproachElement = htRejElement.parentNode().firstChildElement("WetBulbApproach");
    QDomElement dsgnSupWtrTempElement = htRejElement.parentNode().firstChildElement("DsgnSupWtrTemp");
    QDomElement dsgnSupWtrDelTElement = htRejElement.parentNode().firstChildElement("DsgnSupWtrDelT");

    boost::optional<double> wetBulbApproach;
    boost::optional<double> dsgnSupWtrTemp;
    boost::optional<double> dsgnSupWtrDelT;

    value = wetBulbApproachElement.text().toDouble(&ok);
    if( ok )
    {
      wetBulbApproach = value * 5.0 / 9.0;
    }

    value = dsgnSupWtrTempElement.text().toDouble(&ok);
    if( ok )
    {
      dsgnSupWtrTemp = unitToUnit(value,"F","C");
    }

    value = dsgnSupWtrDelTElement.text().toDouble(&ok);
    if( ok )
    {
      dsgnSupWtrDelT = value * 5.0 / 9.0; 
    }

    if( dsgnSupWtrDelT )
    {
      tower.setDesignRangeTemperature(dsgnSupWtrDelT.get());
    }

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
      QDomElement airFlowCapElement = htRejElement.firstChildElement("AirFlowCap");

      value = airFlowCapElement.text().toDouble(&ok);
      if( ok )
      {
        tower.setDesignAirFlowRate(unitToUnit(value,"cfm","m^3/s").get());
      }
      
      QDomElement wtrFlowCapElement = htRejElement.firstChildElement("WtrFlowCap");

      value = wtrFlowCapElement.text().toDouble(&ok);
      if( ok )
      {
        tower.setDesignWaterFlowRate(unitToUnit(value, "gal/min", "m^3/s").get());
      }

      QDomElement totFanHPElement = htRejElement.firstChildElement("TotFanHP");

      value = totFanHPElement.text().toDouble(&ok);
      if( ok )
      {
        tower.setDesignFanPower(value * 745.7);
      }
    }

    QDomElement minSpdRatElement = htRejElement.firstChildElement("LowSpdAirFlowRat");

    value = minSpdRatElement.text().toDouble(&ok);
    if( ok )
    {
      tower.setMinimumAirFlowRateRatio(value);
    }

    boost::optional<model::CurveCubic> vsdFanPwrRatio_fQRatio;
    QDomElement vsdFanPwrRatio_fQRatioElement = htRejElement.firstChildElement("VSDFanPwrRatio_fQRatio");
    vsdFanPwrRatio_fQRatio = model.getModelObjectByName<model::CurveCubic>(vsdFanPwrRatio_fQRatioElement.text().toStdString());

    if( vsdFanPwrRatio_fQRatio )
    {
      tower.setFanPowerRatioFunctionofAirFlowRateRatioCurve(vsdFanPwrRatio_fQRatio.get());
    }

    QDomElement cellCntElement = htRejElement.firstChildElement("CellCnt");
    int cellCnt = cellCntElement.text().toInt(&ok);
    if( ok )
    {
      tower.setNumberofCells(cellCnt);
    }

    result = tower;
  }
  else
  {
    model::CoolingTowerSingleSpeed tower(model);

    result = tower;

    if( modCtrlElement.text().compare("Cycling",Qt::CaseInsensitive) == 0 )
    {
      tower.setCapacityControl("FanCycling");
    }
    else if( modCtrlElement.text().compare("Bypass",Qt::CaseInsensitive) == 0 )
    {
      tower.setCapacityControl("FluidBypass");
    }

    QDomElement cellCntElement = htRejElement.firstChildElement("CellCnt");
    int cellCnt = cellCntElement.text().toInt(&ok);
    if( ok )
    {
      tower.setNumberofCells(cellCnt);
    }
    
    if( ! autosize() )
    {
      // PerformanceInputMethod
      tower.setPerformanceInputMethod("NominalCapacity");

      tower.resetDesignWaterFlowRate();

      tower.resetUFactorTimesAreaValueatDesignAirFlowRate();

      tower.resetUFactorTimesAreaValueatFreeConvectionAirFlowRate();

      // CapRtd
      QDomElement capRtdElement = htRejElement.firstChildElement("CapRtd");
      value = capRtdElement.text().toDouble(&ok);

      if( ok )
      {
        // NominalCapacity
        double cap = unitToUnit(value,"Btu/h","W").get();

        tower.setNominalCapacity(cap);
      }

      QDomElement airFlowCapElement = htRejElement.firstChildElement("AirFlowCap");

      value = airFlowCapElement.text().toDouble(&ok);
      if( ok )
      {
        tower.setDesignAirFlowRate(unitToUnit(value,"cfm","m^3/s").get());
      }
      
      QDomElement wtrFlowCapElement = htRejElement.firstChildElement("WtrFlowCap");

      value = wtrFlowCapElement.text().toDouble(&ok);
      if( ok )
      {
        tower.setDesignWaterFlowRate(unitToUnit(value,"gal","m^3").get());
      }

      QDomElement totFanHPElement = htRejElement.firstChildElement("TotFanHP");

      value = totFanHPElement.text().toDouble(&ok);
      if( ok )
      {
        tower.setFanPoweratDesignAirFlowRate(value * 745.7);
      }
    }
  }

  if( result )
  {
    // Name

    QDomElement nameElement = htRejElement.firstChildElement("Name");
    
    result->setName(nameElement.text().toStdString());
  }

  return result;
}

boost::optional<openstudio::model::ModelObject> ReverseTranslator::translateChiller(
                                                  const QDomElement& chillerElement, 
                                                  const QDomDocument& doc,
                                                  openstudio::model::Model& model )
{
  boost::optional<openstudio::model::ModelObject> result;

  if( ! istringEqual(chillerElement.tagName().toStdString(),"Chlr") )
  {
    return result;
  }

  QDomElement nameElement = chillerElement.firstChildElement("Name");

  // Cap_fTempCrvRef

  boost::optional<model::CurveBiquadratic> cap_fTempCrv;
  QDomElement cap_fTempCrvElement = chillerElement.firstChildElement("Cap_fTempCrvRef");
  cap_fTempCrv = model.getModelObjectByName<model::CurveBiquadratic>(cap_fTempCrvElement.text().toStdString());
  if( ! cap_fTempCrv )
  {
    LOG(Error,"Coil: " << nameElement.text().toStdString() << " Broken Cap_fTempCrv");

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
  QDomElement eir_fTempCrvElement = chillerElement.firstChildElement("EIR_fTempCrvRef");
  eir_fTempCrv = model.getModelObjectByName<model::CurveBiquadratic>(eir_fTempCrvElement.text().toStdString());
  if( ! eir_fTempCrv )
  {
    LOG(Error,"Coil: " << nameElement.text().toStdString() << "Broken EIR_fTempCrvRef");

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
  QDomElement eir_fPLRCrvElement = chillerElement.firstChildElement("EIR_fPLRCrvRef");
  eir_fPLRCrv = model.getModelObjectByName<model::CurveQuadratic>(eir_fPLRCrvElement.text().toStdString());
  if( ! eir_fPLRCrv )
  {
    LOG(Error,"Coil: " << nameElement.text().toStdString() << "Broken EIR_fPLRCrvRef");

    eir_fPLRCrv = model::CurveQuadratic(model);
    eir_fPLRCrv->setCoefficient1Constant(0.06369119);
    eir_fPLRCrv->setCoefficient2x(0.58488832);
    eir_fPLRCrv->setCoefficient3xPOW2(0.35280274);
    eir_fPLRCrv->setMinimumValueofx(0.0);
    eir_fPLRCrv->setMaximumValueofx(1.0);
  }

  model::ChillerElectricEIR chiller(model,cap_fTempCrv.get(),eir_fTempCrv.get(),eir_fPLRCrv.get());

  // Name

  chiller.setName(nameElement.text().toStdString());

  double value;
  bool ok;

  // CndsrInRef
  boost::optional<double> condDsgnSupWtrDelT;
  QDomElement cndsrInRefElement = chillerElement.firstChildElement("CndsrFluidSegInRef");
  boost::optional<model::PlantLoop> condenserSystem = loopForSupplySegment(cndsrInRefElement.text(),doc,model);
  if( condenserSystem )
  {
    condenserSystem->addDemandBranchForComponent(chiller);
    condDsgnSupWtrDelT = condenserSystem->sizingPlant().loopDesignTemperatureDifference();
  }

  // COP
  boost::optional<double> cop;
  QDomElement copElement = chillerElement.firstChildElement("COP");
  value = copElement.text().toDouble(&ok);
  if( ok )
  {
    chiller.setReferenceCOP(value);
    cop = value;
  }

  // PartLdRatMin
  QDomElement partLdRatMinElement = chillerElement.firstChildElement("PartLdRatMin");
  value = partLdRatMinElement.text().toDouble(&ok);
  if( ok )
  {
    chiller.setMinimumPartLoadRatio(value);
  }

  // UnldRatMin
  QDomElement unldRatMinElement = chillerElement.firstChildElement("UnldRatMin");
  value = unldRatMinElement.text().toDouble(&ok);
  if( ok )
  {
    chiller.setMinimumUnloadingRatio(value);
  }

  if( ! autosize() )
  {
    // CapRtd
    boost::optional<double> capRtd;
    QDomElement capRtdElement = chillerElement.firstChildElement("CapRtd");
    value = capRtdElement.text().toDouble(&ok);
    if( ok )
    {
      capRtd = unitToUnit(value,"Btu/h","W");
    }

    // EntTempDsgn
    boost::optional<double> entTempDsgn;
    QDomElement entTempDsgnElement = chillerElement.firstChildElement("EntTempDsgn");
    value = entTempDsgnElement.text().toDouble(&ok);
    if( ok )
    {
      entTempDsgn = unitToUnit(value,"F","C");
    }

    // LvgTempDsgn
    boost::optional<double> lvgTempDsgn;
    QDomElement lvgTempDsgnElement = chillerElement.firstChildElement("LvgTempDsgn");
    value = lvgTempDsgnElement.text().toDouble(&ok);
    if( ok )
    {
      lvgTempDsgn = unitToUnit(value,"F","C");
    }

    if( capRtd && entTempDsgn && lvgTempDsgn && cop )
    {
      chiller.setReferenceCapacity(capRtd.get());

      double flow = capRtd.get() / ( cpWater * densityWater * (entTempDsgn.get() - lvgTempDsgn.get()));

      chiller.setReferenceChilledWaterFlowRate(flow);

      if( condDsgnSupWtrDelT )
      {
        double condFlow = capRtd.get() * (1.0 + 1.0 / cop.get()) / ( cpWater * densityWater * condDsgnSupWtrDelT.get() );
        chiller.setReferenceCondenserFluidFlowRate(condFlow);
      }
    }
  }

  return chiller;
}

boost::optional<openstudio::model::ModelObject> ReverseTranslator::translateWtrHtr(
                                                  const QDomElement& element, 
                                                  const QDomDocument& doc,
                                                  openstudio::model::Model& model )
{
  boost::optional<openstudio::model::ModelObject> result;
  bool ok;

  if( ! istringEqual(element.tagName().toStdString(),"WtrHtr") )
  {
    return result;
  }

  model::WaterHeaterMixed waterHeaterMixed(model);

  // Name

  QDomElement nameElement = element.firstChildElement("Name");

  waterHeaterMixed.setName(nameElement.text().toStdString());

  // ThrmlEff

  QDomElement thrmlEffElement = element.firstChildElement("ThrmlEff");
  
  double thrmlEff = thrmlEffElement.text().toDouble(&ok);

  if( ok )
  {
    waterHeaterMixed.setHeaterThermalEfficiency(thrmlEff);
  }

  // Ambient water temperature indicator

  waterHeaterMixed.setAmbientTemperatureIndicator("Schedule");

  model::ScheduleRuleset scheduleRuleset = model::ScheduleRuleset(model);

  scheduleRuleset.setName(QString(nameElement.text() + " Ambient Temperature").toStdString());

  model::ScheduleDay scheduleDay = scheduleRuleset.defaultDaySchedule();

  scheduleDay.addValue(Time(1.0),20.0);

  waterHeaterMixed.setAmbientTemperatureSchedule(scheduleRuleset);

  // StorCap

  QDomElement wtrHtrStorCapElement = element.firstChildElement("StorCap");

  double wtrHtrStorCap = wtrHtrStorCapElement.text().toDouble(&ok);

  if( ok )
  {
    waterHeaterMixed.setTankVolume(unitToUnit(wtrHtrStorCap,"gal","m^3").get());
  }

  // CapRtd
  
  QDomElement wtrHtrMaxCapElement = element.firstChildElement("CapRtd");

  double wtrHtrMaxCap = wtrHtrMaxCapElement.text().toDouble(&ok);

  if( ok )
  {
    waterHeaterMixed.setHeaterMaximumCapacity(unitToUnit(wtrHtrMaxCap,"Btu/h","W").get());
  }

  // MinCap

  QDomElement minCapElement = element.firstChildElement("MinCap");

  double minCap = minCapElement.text().toDouble(&ok);

  if( ok )
  {
    waterHeaterMixed.setHeaterMinimumCapacity(unitToUnit(minCap,"Btu/h","W").get());
  }

  // OffCyclePrstcLoss

  QDomElement offCyclePrstcLossElement = element.firstChildElement("OffCyclePrstcLoss");

  double offCyclePrstcLoss = offCyclePrstcLossElement.text().toDouble(&ok);

  if( ok )
  {
    waterHeaterMixed.setOffCycleParasiticFuelConsumptionRate(offCyclePrstcLoss);
  }

  // OnCyclePrstcLoss

  QDomElement onCyclePrstcLossElement = element.firstChildElement("OnCyclePrstcLoss");

  double onCyclePrstcLoss = onCyclePrstcLossElement.text().toDouble(&ok);

  if( ok )
  {
    waterHeaterMixed.setOnCycleParasiticFuelConsumptionRate(onCyclePrstcLoss);
  }

  // TankOffCycleLossCoef

  QDomElement tankOffCycleLossCoefElement = element.firstChildElement("TankOffCycleLossCoef");

  double tankOffCycleLossCoef = tankOffCycleLossCoefElement.text().toDouble(&ok);

  if( ok )
  {
    // Convert Btu/h-F to W/K
    waterHeaterMixed.setOffCycleLossCoefficienttoAmbientTemperature(tankOffCycleLossCoef * 0.5275);
  }

  // TankOnCycleLossCoef

  QDomElement tankOnCycleLossCoefElement = element.firstChildElement("TankOnCycleLossCoef");

  double tankOnCycleLossCoef = tankOnCycleLossCoefElement.text().toDouble(&ok);

  if( ok )
  {
    // Convert Btu/h-F to W/K
    waterHeaterMixed.setOnCycleLossCoefficienttoAmbientTemperature(tankOnCycleLossCoef * 0.5275);
  }

  // Setpoint schedule

  model::Schedule setpointTempSchedule = serviceHotWaterSetpointSchedule(model);

  waterHeaterMixed.setSetpointTemperatureSchedule(setpointTempSchedule);

  // HIR_fPLRCrvRef

  QDomElement hirfPLRCrvRefElement = element.firstChildElement("HIR_fPLRCrvRef");
  boost::optional<model::CurveCubic> hirfPLRCrv = model.getModelObjectByName<model::CurveCubic>(hirfPLRCrvRefElement.text().toStdString());
  if( hirfPLRCrv )
  {
    waterHeaterMixed.setPartLoadFactorCurve(hirfPLRCrv.get());
  }

  // FuelSrc
  auto fuelSrcElement = element.firstChildElement("FuelSrc");
  waterHeaterMixed.setHeaterFuelType(fuelSrcElement.text().toStdString());

  // OffCycleFuelSrc
  auto offCycleFuelSrcElement = element.firstChildElement("OffCycleFuelSrc");
  waterHeaterMixed.setOffCycleParasiticFuelType(offCycleFuelSrcElement.text().toStdString());

  // OnCycleFuelSrc
  auto onCycleFuelSrcElement = element.firstChildElement("OnCycleFuelSrc");
  waterHeaterMixed.setOnCycleParasiticFuelType(onCycleFuelSrcElement.text().toStdString());

  return waterHeaterMixed;
}

boost::optional<openstudio::model::ModelObject> ReverseTranslator::translateZnSys(const QDomElement& element, const QDomDocument& doc, openstudio::model::Model& model)
{
  if( ! istringEqual(element.tagName().toStdString(),"ZnSys") )
  {
    return boost::none;
  }

  boost::optional<model::ModelObject> result;

  // Name
  std::string name = element.firstChildElement("Name").text().toStdString();

  // Type
  QDomElement typeElement = element.firstChildElement("Type");
  if( typeElement.text().compare("Exhaust",Qt::CaseInsensitive) == 0 )
  {
    return boost::none;
  }
  std::string type = typeElement.text().toStdString();

  // Availability Schedule
  QDomElement scheduleElement = element.firstChildElement("AvailSchRef");

  boost::optional<model::Schedule> schedule;

  if( ! scheduleElement.isNull() )
  {
    schedule = model.getModelObjectByName<model::Schedule>(scheduleElement.text().toStdString()); 
  }

  if( ! schedule )
  {
    schedule = model.alwaysOnDiscreteSchedule();
  }

  // FlowCap

  QDomElement fanElement = element.firstChildElement("Fan"); 

  QDomElement flowCapElement = fanElement.firstChildElement("FlowCapSim");
  QDomElement flowMinElement = fanElement.firstChildElement("FlowMinSim");

  boost::optional<double> flowCap;
  boost::optional<double> flowMin;
  double value;
  bool ok;

  if( ! autosize() )
  {
    value = flowCapElement.text().toDouble(&ok);

    if( ok ) 
    {
      flowCap = unitToUnit(value,"cfm","m^3/s").get();
    }

    value = flowMinElement.text().toDouble(&ok);

    if( ok ) 
    {
      flowMin = unitToUnit(value,"cfm","m^3/s").get();
    }
  }

  // HtgDsgnSupAirTemp

  boost::optional<double> htgDsgnSupAirTemp;

  if( ! autosize() )
  {
    QDomElement htgDsgnSupAirTempElement = element.firstChildElement("HtgDsgnSupAirTemp");

    value = htgDsgnSupAirTempElement.text().toDouble(&ok);

    htgDsgnSupAirTemp = unitToUnit(value,"F","C").get();
  }

  if( istringEqual(type,"PTAC") )
  {
    boost::optional<openstudio::model::ModelObject> mo;

    // Fan
    
    QDomElement fanElement = element.firstChildElement("Fan"); 

    mo = translateFan(fanElement,doc,model);    

    OS_ASSERT(mo);

    model::HVACComponent fan = mo->cast<model::HVACComponent>();
    
    // Heating Coil

    QDomElement heatingCoilElement = element.firstChildElement("CoilHtg"); 
    
    mo = translateCoilHeating(heatingCoilElement,doc,model);

    OS_ASSERT(mo);

    model::HVACComponent heatingCoil = mo->cast<model::HVACComponent>();

    // Cooling Coil

    QDomElement coolingCoilElement = element.firstChildElement("CoilClg");

    mo = translateCoilCooling(coolingCoilElement,doc,model);

    OS_ASSERT(mo);

    model::HVACComponent coolingCoil = mo->cast<model::HVACComponent>();

    // PTAC

    model::ZoneHVACPackagedTerminalAirConditioner ptac(model,schedule.get(),fan,heatingCoil,coolingCoil);

    if( flowCap )
    {
      ptac.setSupplyAirFlowRateDuringCoolingOperation(flowCap.get());
      ptac.setSupplyAirFlowRateDuringHeatingOperation(flowCap.get());
      ptac.setSupplyAirFlowRateWhenNoCoolingorHeatingisNeeded(flowCap.get());
    }

    // Name

    ptac.setName(name);

    // FanCtrl
    
    QDomElement fanCtrlElement = element.firstChildElement("FanCtrl"); 

    if( istringEqual(fanCtrlElement.text().toStdString(),"Continuous") )
    {
      model::Schedule schedule = model.alwaysOnDiscreteSchedule();

      ptac.setSupplyAirFanOperatingModeSchedule(schedule);
    }

    result = ptac;
  }
  else if( istringEqual(type,"WSHP") )
  {
    boost::optional<openstudio::model::ModelObject> mo;
    QDomElement htPumpSuppTempElement;
    QDomElement htPumpCprsrLockoutTempElement;

    // Fan
    QDomElement fanElement = element.firstChildElement("Fan"); 
    mo = translateFan(fanElement,doc,model);    
    OS_ASSERT(mo);
    model::HVACComponent fan = mo->cast<model::HVACComponent>();
 
    // Heating Coil
    boost::optional<model::HVACComponent> heatingCoil;
    boost::optional<model::HVACComponent> suppHeatingCoil;
    QDomNodeList heatingCoilElements = element.elementsByTagName("CoilHtg"); 
    for(int i = 0; i < heatingCoilElements.count(); i++)
    {
      QDomElement heatingCoilElement = heatingCoilElements.at(i).toElement();
      QDomElement typeElement = heatingCoilElement.firstChildElement("Type");
      mo = translateCoilHeating(heatingCoilElement,doc,model);
      OS_ASSERT(mo);

      if(istringEqual(typeElement.text().toStdString(),"HeatPump"))
      {
        heatingCoil = mo->cast<model::HVACComponent>();
        htPumpSuppTempElement = heatingCoilElement.firstChildElement("HtPumpSuppTemp");
        htPumpCprsrLockoutTempElement = heatingCoilElement.firstChildElement("HtPumpCprsrLockoutTemp");
      }
      else
      {
        suppHeatingCoil = mo->cast<model::HVACComponent>();
      }

      if( i > 1 )
      {
        LOG(Warn,name << " has unused heating coils defined.");
      }
    }

    // Cooling Coil
    QDomElement coolingCoilElement = element.firstChildElement("CoilClg");
    mo = translateCoilCooling(coolingCoilElement,doc,model);
    OS_ASSERT(mo);
    model::HVACComponent coolingCoil = mo->cast<model::HVACComponent>();

    boost::optional<model::ZoneHVACWaterToAirHeatPump> wshp;

    // Validate that everything we need is provided
    OS_ASSERT(heatingCoil);
    OS_ASSERT(heatingCoil->optionalCast<model::CoilHeatingWaterToAirHeatPumpEquationFit>());
    OS_ASSERT(coolingCoil.optionalCast<model::CoilCoolingWaterToAirHeatPumpEquationFit>());

    // Default Supplemental Heating Coil if one is not provided
    if( ! suppHeatingCoil )
    {
      model::Schedule alwaysOnDiscreteSchedule = model.alwaysOnDiscreteSchedule();
      suppHeatingCoil = model::CoilHeatingElectric(model,alwaysOnDiscreteSchedule);
      LOG(Warn,name << " creating a default supplementary heating coil");
    }

    wshp = model::ZoneHVACWaterToAirHeatPump(model,
                                             schedule.get(),
                                             fan,
                                             heatingCoil.get(),
                                             coolingCoil,
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

    QDomElement htPumpFanDelayElement = element.firstChildElement("HtPumpFanDelay");
    value = htPumpFanDelayElement.text().toDouble(&ok);
    if( ok )
    {
      wshp->setHeatPumpFanDelayTime(value);
    }
    else
    {
      wshp->setHeatPumpFanDelayTime(60.0);
    }

    if( htgDsgnSupAirTemp )
    {
      wshp->setMaximumSupplyAirTemperaturefromSupplementalHeater(htgDsgnSupAirTemp.get());
    }

    value = htPumpSuppTempElement.text().toDouble(&ok);
    if( ok )
    {
      value = unitToUnit(value,"F","C").get();
      wshp->setMaximumOutdoorDryBulbTemperatureforSupplementalHeaterOperation(value);
    }
  }
  else if( istringEqual(type,"PTHP") )
  {
    boost::optional<openstudio::model::ModelObject> mo;
    QDomElement htPumpSuppTempElement;
    QDomElement htPumpCprsrLockoutTempElement;

    // Fan
    QDomElement fanElement = element.firstChildElement("Fan"); 
    mo = translateFan(fanElement,doc,model);    
    OS_ASSERT(mo);
    model::HVACComponent fan = mo->cast<model::HVACComponent>();
 
    // Heating Coil
    boost::optional<model::HVACComponent> heatingCoil;
    boost::optional<model::HVACComponent> suppHeatingCoil;
    QDomNodeList heatingCoilElements = element.elementsByTagName("CoilHtg"); 
    for(int i = 0; i < heatingCoilElements.count(); i++)
    {
      QDomElement heatingCoilElement = heatingCoilElements.at(i).toElement();

      QDomElement typeElement = heatingCoilElement.firstChildElement("Type");

      mo = translateCoilHeating(heatingCoilElement,doc,model);

      OS_ASSERT(mo);

      if(istringEqual(typeElement.text().toStdString(),"HeatPump"))
      {
        heatingCoil = mo->cast<model::HVACComponent>();

        htPumpSuppTempElement = heatingCoilElement.firstChildElement("HtPumpSuppTemp");

        htPumpCprsrLockoutTempElement = heatingCoilElement.firstChildElement("HtPumpCprsrLockoutTemp");
      }
      else
      {
        suppHeatingCoil = mo->cast<model::HVACComponent>();
      }

      if( i > 1 )
      {
        LOG(Warn,name << " has unused heating coils defined.");
      }
    }

    // Cooling Coil
    QDomElement coolingCoilElement = element.firstChildElement("CoilClg");
    mo = translateCoilCooling(coolingCoilElement,doc,model);
    OS_ASSERT(mo);
    model::HVACComponent coolingCoil = mo->cast<model::HVACComponent>();

    // Default Supplemental Heating Coil if one is not provided
    if( ! suppHeatingCoil )
    {
      model::Schedule alwaysOnDiscreteSchedule = model.alwaysOnDiscreteSchedule();
      suppHeatingCoil = model::CoilHeatingElectric(model,alwaysOnDiscreteSchedule);
      LOG(Warn,name << " creating a default supplementary heating coil");
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

    pthp = model::ZoneHVACPackagedTerminalHeatPump(model,
                                                   schedule.get(),
                                                   fan,
                                                   heatingCoil.get(),
                                                   coolingCoil,
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
    value = htPumpSuppTempElement.text().toDouble(&ok);
    if( ok )
    {
      value = unitToUnit(value,"F","C").get();
      pthp->setMaximumOutdoorDryBulbTemperatureforSupplementalHeaterOperation(value);
    }

    // HtPumpCprsrLockoutTemp
    value = htPumpCprsrLockoutTempElement.text().toDouble(&ok);
    if( ok )
    {
      value = unitToUnit(value,"F","C").get();
      pthp->setMinimumOutdoorDryBulbTemperatureforCompressorOperation(value);
    }
  
    // FanCtrl
    QDomElement fanCtrlElement = element.firstChildElement("FanCtrl"); 
    if( istringEqual(fanCtrlElement.text().toStdString(),"Continuous") )
    {
      model::Schedule schedule = model.alwaysOnDiscreteSchedule();
      pthp->setSupplyAirFanOperatingModeSchedule(schedule);
    }

    // htgDsgnSupAirTemp
    if( htgDsgnSupAirTemp )
    {
      pthp->setMaximumSupplyAirTemperaturefromSupplementalHeater(htgDsgnSupAirTemp.get());
    }
  }
  else if( istringEqual(type,"Baseboard") )
  {
    // Heating Coil
    QDomElement heatingCoilElement = element.firstChildElement("CoilHtg"); 
    
    // Heating Coil Type
    std::string htgCoilType = heatingCoilElement.firstChildElement("Type").text().toStdString();

    if ( istringEqual(htgCoilType,"Resistance") )
    {
      // ZoneHVAC Baseboard Convective Electric
      model::ZoneHVACBaseboardConvectiveElectric baseboard(model);
      result = baseboard;
      
      // Nominal Capacity
      boost::optional<double> nomHtgCap;
      
      if( ! autosize() )
      {
        bool ok;

        double value = heatingCoilElement.firstChildElement("CapTotGrossRtdSim").text().toDouble(&ok);

        if( ok ) 
        {
          nomHtgCap = unitToUnit(value, "Btu/hr", "W");
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

      // Maximum Water Flow Rate (units are gpm in the bec file)
      boost::optional<double> dsnHtgFlowRt;

      if( ! autosize() )
      {
        bool ok;

        double value = heatingCoilElement.firstChildElement("FluidFlowRtDsgnSim").text().toDouble(&ok);

        if( ok ) 
        {
          dsnHtgFlowRt = unitToUnit(value, "gal/min", "m^3/s");
        }
      }
     
      if( dsnHtgFlowRt )
      {
        coil.setMaximumWaterFlowRate(*dsnHtgFlowRt);
      }

      //Hook Coil to PlantLoop
      QDomElement fluidSegInRefElement = heatingCoilElement.firstChildElement("FluidSegInRef");

      if( boost::optional<model::PlantLoop> plant = loopForSupplySegment(fluidSegInRefElement.text(),doc,model) )
      {
        plant->addDemandBranchForComponent(coil);
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
    
    QDomElement fanElement = element.firstChildElement("Fan"); 

    mo = translateFan(fanElement,doc,model);    

    OS_ASSERT(mo);

    model::HVACComponent fan = mo->cast<model::HVACComponent>();

    // Heating Coil

    QDomElement heatingCoilElement = element.firstChildElement("CoilHtg"); 
    
    mo = translateCoilHeating(heatingCoilElement,doc,model);

    OS_ASSERT(mo);

    model::HVACComponent heatingCoil = mo->cast<model::HVACComponent>();

    // get the heating coil's max flow rate (units are gpm in the bec file)
    QDomElement dsnHtgFlowRtElement = heatingCoilElement.firstChildElement("FluidFlowRtDsgnSim");

    boost::optional<double> dsnHtgFlowRt;

    if( ! autosize() )
    {
      bool ok;

      double value = dsnHtgFlowRtElement.text().toDouble(&ok);

      if( ok ) 
      {
        dsnHtgFlowRt = unitToUnit(value, "gal/min", "m^3/s");
      }
    }
    
    // Cooling Coil

    QDomElement coolingCoilElement = element.firstChildElement("CoilClg");

    mo = translateCoilCooling(coolingCoilElement,doc,model);

    OS_ASSERT(mo);

    model::HVACComponent coolingCoil = mo->cast<model::HVACComponent>();

    // get the cooling coil's max flow rate (units are gpm in the bec file)
    QDomElement dsnClgFlowRtElement = coolingCoilElement.firstChildElement("FluidFlowRtDsgnSim");

    boost::optional<double> dsnClgFlowRt;

    if( ! autosize() )
    {
      bool ok;

      double value = dsnClgFlowRtElement.text().toDouble(&ok);

      if( ok ) 
      {
        dsnClgFlowRt = unitToUnit(value, "gal/min", "m^3/s");
      }
    }
    
    // FPFC (Four Pipe Fan Coil)
    model::ZoneHVACFourPipeFanCoil fpfc(model,schedule.get(),fan,coolingCoil,heatingCoil);
    result = fpfc;

    // pull the max air flow rate from the fan and set in the fpfc
    if( flowCap )
    {
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

    QDomElement fanElementControlMethodElement = fanElement.firstChildElement("CtrlMthd");

    // FanElementMinFlow

    QDomElement fanElementFlowMinElement = fanElement.firstChildElement("FlowMinSim");
    
    // FanElementFlowCap

    QDomElement fanElementFlowCapElement = fanElement.firstChildElement("FlowCapSim");

    boost::optional<double> flowMinRatio;
 
    if( flowMin && flowCap )
    {
      flowMinRatio = flowMin.get() / flowCap.get();
    }

    // FanCtrl

    fpfc.setLowSpeedSupplyAirFlowRatio(1.0);

    fpfc.setMediumSpeedSupplyAirFlowRatio(1.0);
    
    QDomElement fanCtrlElement = element.firstChildElement("FanCtrl");

    if( (fanCtrlElement.text().compare("Continuous",Qt::CaseInsensitive)) == 0 )
    {
      if( (fanElementControlMethodElement.text().compare("ConstantVolume",Qt::CaseInsensitive)) == 0 )
      {
        fpfc.setCapacityControlMethod("ConstantFanVariableFlow");
      }
      else if( (fanElementControlMethodElement.text().compare("VariableSpeedDrive",Qt::CaseInsensitive)) == 0 )
      {
        fpfc.setCapacityControlMethod("VariableFanVariableFlow");

        if( flowMinRatio )
        {
          fpfc.setLowSpeedSupplyAirFlowRatio(flowMinRatio.get());

          fpfc.setMediumSpeedSupplyAirFlowRatio(flowMinRatio.get());
        }
      }
    }
    else if( (fanCtrlElement.text().compare("Cycling",Qt::CaseInsensitive)) == 0 )
    {
      if( (fanElementControlMethodElement.text().compare("TwoSpeed",Qt::CaseInsensitive)) == 0 )
      {
        fpfc.setCapacityControlMethod("CyclingFan");

        if( flowMinRatio )
        {
          fpfc.setLowSpeedSupplyAirFlowRatio(flowMinRatio.get());

          fpfc.setMediumSpeedSupplyAirFlowRatio(flowMinRatio.get());
        }
      }
      else if( (fanElementControlMethodElement.text().compare("ConstantVolume",Qt::CaseInsensitive)) == 0 )
      {
        fpfc.setCapacityControlMethod("CyclingFan");
      }
    }

    // Name
    fpfc.setName(name);
  }

  return result;
}

boost::optional<openstudio::model::ModelObject> ReverseTranslator::translateCrvDblQuad(const QDomElement& element, const QDomDocument& doc, openstudio::model::Model& model)
{
  if( ! istringEqual(element.tagName().toStdString(),"CrvDblQuad") )
  {
    return boost::none;
  }

  model::CurveBiquadratic curve(model);

  // Name

  QDomElement nameElement = element.firstChildElement("Name");

  curve.setName(nameElement.text().toStdString());

  // Coef1 

  QDomElement coef1Element = element.firstChildElement("Coef1");

  curve.setCoefficient1Constant(coef1Element.text().toDouble());

  // Coef2

  QDomElement coef2Element = element.firstChildElement("Coef2");

  curve.setCoefficient2x(coef2Element.text().toDouble());

  // Coef3

  QDomElement coef3Element = element.firstChildElement("Coef3");

  curve.setCoefficient3xPOW2(coef3Element.text().toDouble());

  // Coef4

  QDomElement coef4Element = element.firstChildElement("Coef4");

  curve.setCoefficient4y(coef4Element.text().toDouble());

  // Coef5

  QDomElement coef5Element = element.firstChildElement("Coef5");

  curve.setCoefficient5yPOW2(coef5Element.text().toDouble());

  // Coef6

  QDomElement coef6Element = element.firstChildElement("Coef6");

  curve.setCoefficient6xTIMESY(coef6Element.text().toDouble());

  // MinVar1
  
  QDomElement minVar1Element = element.firstChildElement("MinVar1");

  bool ok;

  double value = minVar1Element.text().toDouble(&ok); 

  if(ok)
  {
    curve.setMinimumValueofx(value);
  }
  else
  {
    curve.setMinimumValueofx(0.0);

    LOG(Warn,"Curve: " << nameElement.text().toStdString() << " Missing X Minimum Limit");
  }

  // MaxVar1
  
  QDomElement maxVar1Element = element.firstChildElement("MaxVar1");

  value = maxVar1Element.text().toDouble(&ok);

  if(ok)
  {
    curve.setMaximumValueofx(value);
  }
  else
  {
    curve.setMaximumValueofx(100.0);

    LOG(Warn,"Curve: " << nameElement.text().toStdString() << " Missing X Maximum Limit");
  }

  // MinVar2
  
  QDomElement minVar2Element = element.firstChildElement("MinVar2");

  value = minVar2Element.text().toDouble(&ok);

  if(ok)
  {
    curve.setMinimumValueofy(value);
  }
  else
  {
    curve.setMinimumValueofy(0.0);

    LOG(Warn,"Curve: " << nameElement.text().toStdString() << " Missing Y Minimum Limit");
  }

  // MaxVar2
  
  QDomElement maxVar2Element = element.firstChildElement("MaxVar2");

  value = maxVar2Element.text().toDouble(&ok);

  if(ok)
  {
    curve.setMaximumValueofy(value);
  }
  else
  {
    curve.setMaximumValueofy(100.0);

    LOG(Warn,"Curve: " << nameElement.text().toStdString() << " Missing Y Maximum Limit");
  }

  // MaxOut
  QDomElement maxOutElement = element.firstChildElement("MaxOut");
  value = maxOutElement.text().toDouble(&ok);
  if( ok )
  {
    curve.setMaximumCurveOutput(value);
  }

  // MinOut
  QDomElement minOutElement = element.firstChildElement("MinOut");
  value = minOutElement.text().toDouble(&ok);
  if( ok )
  {
    curve.setMinimumCurveOutput(value);
  }

  return curve;
}

boost::optional<openstudio::model::ModelObject> ReverseTranslator::translateCrvCubic(const QDomElement& element, const QDomDocument& doc, openstudio::model::Model& model)
{
  if( ! istringEqual(element.tagName().toStdString(),"CrvCubic") )
  {
    return boost::none;
  }

  model::CurveCubic curve(model);

  // Name

  QDomElement nameElement = element.firstChildElement("Name");

  curve.setName(nameElement.text().toStdString());

  // Coef1

  QDomElement coef1Element = element.firstChildElement("Coef1");

  curve.setCoefficient1Constant(coef1Element.text().toDouble());

  // Coef2

  QDomElement coef2Element = element.firstChildElement("Coef2");

  curve.setCoefficient2x(coef2Element.text().toDouble());

  // Coef3

  QDomElement coef3Element = element.firstChildElement("Coef3");

  curve.setCoefficient3xPOW2(coef3Element.text().toDouble());

  // Coef4

  QDomElement coef4Element = element.firstChildElement("Coef4");

  curve.setCoefficient4xPOW3(coef4Element.text().toDouble());

  // MinVar1
  
  QDomElement minVar1Element = element.firstChildElement("MinVar1");

  bool ok;

  double value = minVar1Element.text().toDouble(&ok);

  if(ok)
  {
    curve.setMinimumValueofx(value);
  }
  else
  {
    curve.setMinimumValueofx(0.0);

    LOG(Warn,"Curve: " << nameElement.text().toStdString() << " Missing X Minimum Limit");
  }

  // MaxVar1
  
  QDomElement maxVar1Element = element.firstChildElement("MaxVar1");

  value = maxVar1Element.text().toDouble(&ok);

  if(ok)
  {
    curve.setMaximumValueofx(value);
  }
  else
  {
    curve.setMaximumValueofx(100.0);

    LOG(Warn,"Curve: " << nameElement.text().toStdString() << " Missing X Maximum Limit");
  }

  // MaxOut
  QDomElement maxOutElement = element.firstChildElement("MaxOut");
  value = maxOutElement.text().toDouble(&ok);
  if( ok )
  {
    curve.setMaximumCurveOutput(value);
  }

  // MinOut
  QDomElement minOutElement = element.firstChildElement("MinOut");
  value = minOutElement.text().toDouble(&ok);
  if( ok )
  {
    curve.setMinimumCurveOutput(value);
  }

  return curve;
}

boost::optional<openstudio::model::ModelObject> ReverseTranslator::translateCrvQuad(const QDomElement& element, const QDomDocument& doc, openstudio::model::Model& model)
{
  if( ! istringEqual(element.tagName().toStdString(),"CrvQuad") )
  {
    return boost::none;
  }

  model::CurveQuadratic curve(model);

  // Name

  QDomElement nameElement = element.firstChildElement("Name");

  curve.setName(nameElement.text().toStdString());

  // Coef1

  QDomElement coef1Element = element.firstChildElement("Coef1");

  curve.setCoefficient1Constant(coef1Element.text().toDouble());

  // Coef2

  QDomElement coef2Element = element.firstChildElement("Coef2");

  curve.setCoefficient2x(coef2Element.text().toDouble());

  // Coef3

  QDomElement coef3Element = element.firstChildElement("Coef3");

  curve.setCoefficient3xPOW2(coef3Element.text().toDouble());

  // MinVar1
  
  QDomElement minVar1Element = element.firstChildElement("MinVar1");

  bool ok;

  double value = minVar1Element.text().toDouble(&ok);

  if(ok)
  {
    curve.setMinimumValueofx(value);
  }
  else
  {
    curve.setMinimumValueofx(0.0);

    LOG(Warn,"Curve: " << nameElement.text().toStdString() << " Missing X Minimum Limit");
  }

  // MaxVar1
  
  QDomElement maxVar1Element = element.firstChildElement("MaxVar1");

  value = maxVar1Element.text().toDouble(&ok);

  if(ok)
  {
    curve.setMaximumValueofx(value);
  }
  else
  {
    curve.setMaximumValueofx(100.0);

    LOG(Warn,"Curve: " << nameElement.text().toStdString() << " Missing X Maximum Limit");
  }

  // MaxOut
  QDomElement maxOutElement = element.firstChildElement("MaxOut");
  value = maxOutElement.text().toDouble(&ok);
  if( ok )
  {
    curve.setMaximumCurveOutput(value);
  }

  // MinOut
  QDomElement minOutElement = element.firstChildElement("MinOut");
  value = minOutElement.text().toDouble(&ok);
  if( ok )
  {
    curve.setMinimumCurveOutput(value);
  }

  return curve;
}

QDomElement ReverseTranslator::findZnSysElement(const QString & znSysName,const QDomDocument & doc)
{
  QDomNodeList znSysElements = doc.documentElement().firstChildElement("Proj").elementsByTagName("ZnSys");

  for (int i = 0; i < znSysElements.count(); i++)
  {
    QDomElement znSysElement = znSysElements.at(i).toElement();

    QDomElement znSysNameElement = znSysElement.firstChildElement("Name");

    if( znSysNameElement.text() == znSysName )
    {
      return znSysElement;
    }
  }

  return QDomElement();
}

QDomElement ReverseTranslator::findTrmlUnitElementForZone(const QString & zoneName,const QDomDocument & doc)
{
  QDomNodeList airSystemElements = doc.documentElement().elementsByTagName("AirSys");
  
  for( int i = 0; i < airSystemElements.count(); i++ )
  {
    QDomElement airSystemElement = airSystemElements.at(i).toElement();
    QDomNodeList terminalElements = airSystemElement.elementsByTagName("TrmlUnit");
    for( int j = 0; j < terminalElements.count(); j++ )
    {
      QDomElement terminalElement = terminalElements.at(j).toElement();
      QDomElement zoneServedElement = terminalElement.firstChildElement("ZnServedRef");
  
      if(zoneName.compare(zoneServedElement.text(),Qt::CaseInsensitive) == 0)
      {
        return terminalElement;
      }
    }
  }

  return QDomElement();
}

QDomElement ReverseTranslator::findAirSysElement(const QString & airSysName,const QDomDocument & doc)
{
  QDomNodeList airSystemElements = doc.documentElement().elementsByTagName("AirSys");
  
  for( int i = 0; i < airSystemElements.count(); i++ )
  {
    QDomElement airSystemElement = airSystemElements.at(i).toElement();
    QDomElement airSystemNameElement = airSystemElement.firstChildElement("Name");
  
    if(airSysName.compare(airSystemNameElement.text(),Qt::CaseInsensitive) == 0)
    {
      return airSystemElement;
    }
  }

  return QDomElement();
}

boost::optional<QDomElement> ForwardTranslator::translateAirLoopHVAC(const model::AirLoopHVAC& airLoop, QDomDocument& doc)
{
  auto result = doc.createElement("AirSys");
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
  auto nameElement = doc.createElement("Name");
  result.appendChild(nameElement);
  nameElement.appendChild(doc.createTextNode(escapeName(name)));
  
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

  auto typeElement = doc.createElement("Type");
  result.appendChild(typeElement);
  typeElement.appendChild(doc.createTextNode(escapeName(type)));

  // TODO
  // SubType
  // 1:  "SinglePackage"
  // 2:  "SplitSystem"
  // 3:  "CRAC"
  // 4:  "CRAH"

  // NightCycleFanCtrl
  auto nightCycleFanCtrlElement = doc.createElement("NightCycleFanCtrl");
  result.appendChild(nightCycleFanCtrlElement);

  // OpenStudio string to represent night cycle control method
  auto osNightCycleFanCtrl = airLoop.nightCycleControlType();
  // Convert to BEC string
  std::string nightCycleFanCtrl;
  if( istringEqual(osNightCycleFanCtrl,"CycleOnAny") ) {
    nightCycleFanCtrl = "CycleOnCallAnyZone";
  } else if( istringEqual(osNightCycleFanCtrl,"CycleOnAnyZoneFansOnly") ) {
    nightCycleFanCtrl = "CycleZoneFansOnly";
  } else {
    nightCycleFanCtrl = "StaysOff";
  }

  nightCycleFanCtrlElement.appendChild(doc.createTextNode(escapeName(nightCycleFanCtrl)));

  // Cnt
  auto cntElement = doc.createElement("Cnt");
  result.appendChild(cntElement);
  cntElement.appendChild(doc.createTextNode("1"));

  // FanPos
  QString fanPos;
  auto inletComp = supplyOutletNode.inletModelObject();
  OS_ASSERT(inletComp);
  if( inletComp->optionalCast<model::FanConstantVolume>() || inletComp->optionalCast<model::FanVariableVolume>() )
    fanPos = "DrawThrough"; 
  else
    fanPos = "BlowThrough";
  auto fanPosElement = doc.createElement("FanPos");
  result.appendChild(fanPosElement);
  fanPosElement.appendChild(doc.createTextNode(fanPos));

  // ClgCtrl 
  for( const auto & spm : spms ) {
    if( istringEqual(spm.controlVariable(),"Temperature") ) {
      if( auto tempSPM = spm.optionalCast<model::SetpointManagerFollowOutdoorAirTemperature>() ) {
        // SetpointManagerFollowOutdoorAirTemperature is not supported by CBECC so it is converted to oa reset.
        LOG(Warn,tempSPM->briefDescription() << " is translated to ClgCtrl type OutsideAirReset in CBECC.");
        auto clgCtrlElement = doc.createElement("ClgCtrl");
        result.appendChild(clgCtrlElement);
        clgCtrlElement.appendChild(doc.createTextNode("OutsideAirReset"));
 
        auto clRstSupHiElement = doc.createElement("ClRstSupHi");
        result.appendChild(clRstSupHiElement);
        clRstSupHiElement.appendChild(doc.createTextNode("100"));
 
        auto clRstSupLowElement = doc.createElement("ClRstSupLow");
        result.appendChild(clRstSupLowElement);
        clRstSupLowElement.appendChild(doc.createTextNode("0"));

        auto clRstOutdrHiElement = doc.createElement("ClRstOutdrHi");
        result.appendChild(clRstOutdrHiElement);
        clRstOutdrHiElement.appendChild(doc.createTextNode("100"));

        auto clRstOutdrLowElement = doc.createElement("ClRstOutdrLow");
        result.appendChild(clRstOutdrLowElement);
        clRstOutdrLowElement.appendChild(doc.createTextNode("0"));

        m_translatedObjects[tempSPM->handle()] = clgCtrlElement;
      } else if( auto tempSPM = spm.optionalCast<model::SetpointManagerOutdoorAirReset>() ) {
        auto clgCtrlElement = doc.createElement("ClgCtrl");
        result.appendChild(clgCtrlElement);
        clgCtrlElement.appendChild(doc.createTextNode("OutsideAirReset"));
 
        auto clRstSupHiElement = doc.createElement("ClRstSupHi");
        result.appendChild(clRstSupHiElement);
        auto clRstSupHi = convert(tempSPM->setpointatOutdoorHighTemperature(),"C","F").get();
        clRstSupHiElement.appendChild(doc.createTextNode(QString::number(clRstSupHi)));
 
        auto clRstSupLowElement = doc.createElement("ClRstSupLow");
        result.appendChild(clRstSupLowElement);
        auto clRstSupLow = convert(tempSPM->setpointatOutdoorLowTemperature(),"C","F").get();
        clRstSupLowElement.appendChild(doc.createTextNode(QString::number(clRstSupLow)));

        auto clRstOutdrHiElement = doc.createElement("ClRstOutdrHi");
        result.appendChild(clRstOutdrHiElement);
        auto clRstOutdrHi = convert(tempSPM->outdoorHighTemperature(),"C","F").get();
        clRstOutdrHiElement.appendChild(doc.createTextNode(QString::number(clRstOutdrHi)));

        auto clRstOutdrLowElement = doc.createElement("ClRstOutdrLow");
        result.appendChild(clRstOutdrLowElement);
        auto clRstOutdrLow = convert(tempSPM->outdoorHighTemperature(),"C","F").get();
        clRstOutdrLowElement.appendChild(doc.createTextNode(QString::number(clRstOutdrLow)));

        m_translatedObjects[tempSPM->handle()] = clgCtrlElement;
      } else if( auto tempSPM = spm.optionalCast<model::SetpointManagerScheduled>() ) {
        auto clgCtrlElement = doc.createElement("ClgCtrl");
        result.appendChild(clgCtrlElement);
        clgCtrlElement.appendChild(doc.createTextNode("Scheduled"));

        auto clgSetPtSchRefElement = doc.createElement("ClgSetptSchRef");
        result.appendChild(clgSetPtSchRefElement);
        const auto & schedule = tempSPM->schedule();
        clgSetPtSchRefElement.appendChild(doc.createTextNode(escapeName(schedule.name().get())));

        m_translatedObjects[tempSPM->handle()] = clgCtrlElement;
      } else if( auto tempSPM = spm.optionalCast<model::SetpointManagerScheduledDualSetpoint>() ) {
        LOG(Error,tempSPM->briefDescription() << " is not supported by CBECC.");
      } else if( auto tempSPM = spm.optionalCast<model::SetpointManagerSingleZoneReheat>() ) {
        auto clgCtrlElement = doc.createElement("ClgCtrl");
        result.appendChild(clgCtrlElement);
        clgCtrlElement.appendChild(doc.createTextNode("NoSATControl"));

        m_translatedObjects[tempSPM->handle()] = clgCtrlElement;
      } else if( auto tempSPM = spm.optionalCast<model::SetpointManagerWarmestTemperatureFlow>() ) {
        auto clgCtrlElement = doc.createElement("ClgCtrl");
        result.appendChild(clgCtrlElement);
        if( istringEqual(tempSPM->strategy(),"FlowFirst") ) {
          clgCtrlElement.appendChild(doc.createTextNode("WarmestResetFlowFirst"));
        } else {
          clgCtrlElement.appendChild(doc.createTextNode("WarmestResetTemperatureFirst"));
        }

        auto clRstSupHiElement = doc.createElement("ClRstSupHi");
        result.appendChild(clRstSupHiElement);
        auto clRstSupHi = convert(tempSPM->maximumSetpointTemperature(),"C","F").get();
        clRstSupHiElement.appendChild(doc.createTextNode(QString::number(clRstSupHi)));

        auto clRstSupLowElement = doc.createElement("ClRstSupLow");
        result.appendChild(clRstSupLowElement);
        auto clRstSupLow = convert(tempSPM->maximumSetpointTemperature(),"C","F").get();
        clRstSupLowElement.appendChild(doc.createTextNode(QString::number(clRstSupLow)));

        auto dsgnAirFlowMinElement = doc.createElement("DsgnAirFlowMin");
        result.appendChild(dsgnAirFlowMinElement);
        auto dsgnAirFlowMin = tempSPM->minimumTurndownRatio();
        dsgnAirFlowMinElement.appendChild(doc.createTextNode(QString::number(dsgnAirFlowMin)));

        m_translatedObjects[tempSPM->handle()] = clgCtrlElement;
      } else if( auto tempSPM = spm.optionalCast<model::SetpointManagerWarmest>() ) {
        auto clgCtrlElement = doc.createElement("ClgCtrl");
        result.appendChild(clgCtrlElement);
        clgCtrlElement.appendChild(doc.createTextNode("WarmestReset"));

        auto clRstSupHiElement = doc.createElement("ClRstSupHi");
        result.appendChild(clRstSupHiElement);
        auto clRstSupHi = convert(tempSPM->maximumSetpointTemperature(),"C","F").get();
        clRstSupHiElement.appendChild(doc.createTextNode(QString::number(clRstSupHi)));

        auto clRstSupLowElement = doc.createElement("ClRstSupLow");
        result.appendChild(clRstSupLowElement);
        auto clRstSupLow = convert(tempSPM->maximumSetpointTemperature(),"C","F").get();
        clRstSupLowElement.appendChild(doc.createTextNode(QString::number(clRstSupLow)));

        m_translatedObjects[tempSPM->handle()] = clgCtrlElement;
      } else {
        LOG(Error,spm.briefDescription() << " does not currently map into BEC format.")
        // TODO Handle other SPMs
      }

      break;
    }
  }
    
  // Translate supply components

  auto airSegElement = doc.createElement("AirSeg");
  result.appendChild(airSegElement);

  auto airSegNameElement = doc.createElement("Name");
  airSegNameElement.appendChild(doc.createTextNode(QString::fromStdString(name + " Supply AirSeg")));
  airSegElement.appendChild(airSegNameElement);

  auto airSegTypeElement = doc.createElement("Type");
  airSegTypeElement.appendChild(doc.createTextNode("Supply"));
  airSegElement.appendChild(airSegTypeElement);

  for( auto & comp : airLoop.supplyComponents() ) {
    if( auto fan = comp.optionalCast<model::FanConstantVolume>() ) {
      translateFanConstantVolume(fan.get(),airSegElement,doc);
    } else if ( auto coil = comp.optionalCast<model::CoilCoolingDXSingleSpeed>() ) {
      translateCoilCoolingDXSingleSpeed(coil.get(),airSegElement,doc);
    } else if ( auto coil = comp.optionalCast<model::CoilHeatingGas>() ) {
      translateCoilHeatingGas(coil.get(),airSegElement,doc);
    } else if ( auto oasys = comp.optionalCast<model::AirLoopHVACOutdoorAirSystem>() ) {
      translateAirLoopHVACOutdoorAirSystem(oasys.get(),result,doc);
    } else {
      // TODO Handle other supply component types
      LOG(Warn,comp.briefDescription() << " does not currently map into BEC format.")
    }
  }

  return result;
}

boost::optional<QDomElement> ForwardTranslator::translateAirLoopHVACOutdoorAirSystem(const openstudio::model::AirLoopHVACOutdoorAirSystem& oasys, QDomElement & airSysElement, QDomDocument& doc)
{
  auto result = doc.createElement("OACtrl");
  airSysElement.appendChild(result);
  m_translatedObjects[oasys.handle()] = result;

  return result;
}

boost::optional<QDomElement> ForwardTranslator::translateCoilHeatingGas(const openstudio::model::CoilHeatingGas& coil, QDomElement & airSegElement, QDomDocument& doc)
{
  auto result = doc.createElement("CoilClg");
  airSegElement.appendChild(result);
  m_translatedObjects[coil.handle()] = result;

  // Type
  auto typeElement = doc.createElement("Type");
  result.appendChild(typeElement);
  typeElement.appendChild(doc.createTextNode("Furnace"));

  // FuelSrc
  auto fuelSrcElement = doc.createElement("FuelSrc");
  result.appendChild(fuelSrcElement);
  fuelSrcElement.appendChild(doc.createTextNode("NaturalGas"));

  // CapTotGrossRtd
  if( coil.isNominalCapacityAutosized() ) {
    m_autoHardSize = true;
  } else if( auto value = coil.nominalCapacity() ) {
    auto capTotGrossRtdElement = doc.createElement("CapTotGrossRtd");
    result.appendChild(capTotGrossRtdElement);
    capTotGrossRtdElement.appendChild(doc.createTextNode(QString::number(convert(value.get(),"W","Btu/h").get())));
  }

  // FurnThrmlEff
  auto furnThrmlEffElement = doc.createElement("FurnThrmlEff");
  result.appendChild(furnThrmlEffElement);
  furnThrmlEffElement.appendChild(doc.createTextNode(QString::number(convert(coil.gasBurnerEfficiency(),"W","Btu/h").get())));

  return result;
}

boost::optional<QDomElement> ForwardTranslator::translateCoilCoolingDXSingleSpeed(const openstudio::model::CoilCoolingDXSingleSpeed& coil, QDomElement & airSegElement, QDomDocument& doc)
{
  auto result = doc.createElement("CoilClg");
  airSegElement.appendChild(result);
  m_translatedObjects[coil.handle()] = result;

  // Type
  auto typeElement = doc.createElement("Type");
  result.appendChild(typeElement);
  typeElement.appendChild(doc.createTextNode("DirectExpansion"));

  // NumClgStages
  auto numClgStagesElement = doc.createElement("NumClgStages");
  result.appendChild(numClgStagesElement);
  numClgStagesElement.appendChild(doc.createTextNode("1"));

  // CapTotGrossRtd
  if( coil.isRatedTotalCoolingCapacityAutosized() ) {
    m_autoHardSize = true;
  } else if( auto value = coil.ratedTotalCoolingCapacity() ) {
    auto capTotGrossRtdElement = doc.createElement("CapTotGrossRtd");
    result.appendChild(capTotGrossRtdElement);
    capTotGrossRtdElement.appendChild(doc.createTextNode(QString::number(convert(value.get(),"W","Btu/h").get())));
  }

  // DXEER
  if( auto cop = coil.ratedCOP() ) {
    auto r = 0.12;
    auto eer = (cop.get() * (1 - r)  - r) * 3.413;

    auto dxEERElement = doc.createElement("DXEER");
    result.appendChild(dxEERElement);
    dxEERElement.appendChild(doc.createTextNode(QString::number(eer)));
  } else {
    m_autoEfficiency = true;
  }

  // CndsrType
  auto cndsrTypeElement = doc.createElement("CndsrType");
  result.appendChild(cndsrTypeElement);
  cndsrTypeElement.appendChild(doc.createTextNode("Air"));

  return result;
}

boost::optional<QDomElement> ForwardTranslator::translateFanConstantVolume(const openstudio::model::FanConstantVolume& fan, QDomElement & airSegElement, QDomDocument& doc)
{
  auto result = doc.createElement("Fan");
  airSegElement.appendChild(result);
  m_translatedObjects[fan.handle()] = result;

  // CtrlMthd
  auto ctrlMthdElement = doc.createElement("CtrlMthd"); 
  result.appendChild(ctrlMthdElement);
  ctrlMthdElement.appendChild(doc.createTextNode("ConstantVolume"));

  // Class - Centrifugal | Axial
  // TODO Define a way to deterimine or specify 
  auto classElement = doc.createElement("Class");
  result.appendChild(classElement);
  classElement.appendChild(doc.createTextNode("Axial"));

  // ModelingMthd  
  auto modelingMthdElement = doc.createElement("ModelingMthd");
  result.appendChild(modelingMthdElement);
  modelingMthdElement.appendChild(doc.createTextNode("StaticPressure"));

  // FlowCap
  if( fan.isMaximumFlowRateAutosized() ) {
    m_autoHardSize = true;
  } else if( auto value = fan.maximumFlowRate() ) {
    auto flowCapElement = doc.createElement("FlowCap");
    result.appendChild(flowCapElement);
    flowCapElement.appendChild(doc.createTextNode(QString::number(convert(value.get(),"m^3/s","cfm").get())));
  }

  // FlowEff
  auto flowEffElement = doc.createElement("FlowEff");
  result.appendChild(flowEffElement);
  flowEffElement.appendChild(doc.createTextNode(QString::number(fan.fanEfficiency())));

  // TotStaticPress
  auto totStaticPressElement = doc.createElement("TotStaticPress");
  result.appendChild(totStaticPressElement);
  // Convert in PA to WC
  totStaticPressElement.appendChild(doc.createTextNode(QString::number(fan.pressureRise() / 249.0889)));

  // MtrEff
  auto mtrEffElement = doc.createElement("MtrEff");
  result.appendChild(mtrEffElement);
  mtrEffElement.appendChild(doc.createTextNode(QString::number(fan.motorEfficiency())));

  // MtrPos
  auto mtrPosElement = doc.createElement("MtrfPos");
  result.appendChild(mtrPosElement);
  if( fan.motorInAirstreamFraction() >= 0.5 ) {
    mtrPosElement.appendChild(doc.createTextNode("InAirStream"));
  } else {
    mtrPosElement.appendChild(doc.createTextNode("NotInAirStream"));
  }


  return result;
}

} // bec
} // openstudio

