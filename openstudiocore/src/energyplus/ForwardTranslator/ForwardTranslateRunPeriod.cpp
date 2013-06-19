/**********************************************************************
 *  Copyright (c) 2008-2013, Alliance for Sustainable Energy.
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

#include <energyplus/ForwardTranslator.hpp>
#include <model/Model.hpp>
#include <model/AirLoopHVAC.hpp>
#include <model/AirLoopHVAC_Impl.hpp>
#include <model/AirLoopHVACZoneSplitter.hpp>
#include <model/AirLoopHVACZoneSplitter_Impl.hpp>
#include <model/AirLoopHVACZoneMixer.hpp>
#include <model/AirLoopHVACZoneMixer_Impl.hpp>
#include <model/AirLoopHVACOutdoorAirSystem.hpp>
#include <model/AirLoopHVACOutdoorAirSystem_Impl.hpp>
#include <model/AirTerminalSingleDuctUncontrolled.hpp>
#include <model/AirTerminalSingleDuctUncontrolled_Impl.hpp>
#include <model/Building.hpp>
#include <model/Building_Impl.hpp>
#include <model/ControllerOutdoorAir.hpp>
#include <model/ControllerOutdoorAir_Impl.hpp>
#include <model/CoilCoolingDXSingleSpeed.hpp>
#include <model/CoilCoolingDXSingleSpeed_Impl.hpp>
#include <model/CoilHeatingGas.hpp>
#include <model/CoilHeatingGas_Impl.hpp>
#include <model/Construction.hpp>
#include <model/Construction_Impl.hpp>
#include <model/CurveBiquadratic.hpp>
#include <model/CurveBiquadratic_Impl.hpp>
#include <model/CurveQuadratic.hpp>
#include <model/CurveQuadratic_Impl.hpp>
#include <model/ElectricEquipment.hpp>
#include <model/ElectricEquipment_Impl.hpp>
#include <model/GasEquipment.hpp>
#include <model/GasEquipment_Impl.hpp>
#include <model/FanConstantVolume.hpp>
#include <model/FanConstantVolume_Impl.hpp>
#include <model/FenestrationMaterial.hpp>
#include <model/FenestrationMaterial_Impl.hpp>
#include <model/Lights.hpp>
#include <model/Lights_Impl.hpp>
#include <model/LightsDefinition.hpp>
#include <model/LightsDefinition_Impl.hpp>
#include <model/MasslessOpaqueMaterial.hpp>
#include <model/MasslessOpaqueMaterial_Impl.hpp>
#include <model/Node.hpp>
#include <model/Node_Impl.hpp>
#include <model/OpaqueMaterial.hpp>
#include <model/OpaqueMaterial_Impl.hpp>
#include <model/People.hpp>
#include <model/People_Impl.hpp>
#include <model/RunPeriod.hpp>
#include <model/RunPeriod_Impl.hpp>
#include <model/Schedule.hpp>
#include <model/Schedule_Impl.hpp>
#include <model/ScheduleCompact.hpp>
#include <model/ScheduleCompact_Impl.hpp>
#include <model/SetpointManagerMixedAir.hpp>
#include <model/SetpointManagerSingleZoneReheat.hpp>
#include <model/SimpleGlazing.hpp>
#include <model/SimpleGlazing_Impl.hpp>
#include <model/SimulationControl.hpp>
#include <model/SimulationControl_Impl.hpp>
#include <model/Site.hpp>
#include <model/Site_Impl.hpp>
#include <model/Space.hpp>
#include <model/Space_Impl.hpp>
#include <model/SpaceInfiltrationDesignFlowRate.hpp>
#include <model/SpaceInfiltrationDesignFlowRate_Impl.hpp>
#include <model/SpaceInfiltrationEffectiveLeakageArea.hpp>
#include <model/SpaceInfiltrationEffectiveLeakageArea_Impl.hpp>
#include <model/SpaceType.hpp>
#include <model/SpaceType_Impl.hpp>
#include <model/StandardOpaqueMaterial.hpp>
#include <model/StandardOpaqueMaterial_Impl.hpp>
#include <model/StraightComponent.hpp>
#include <model/StraightComponent_Impl.hpp>
#include <model/ThermalZone.hpp>
#include <model/ThermalZone_Impl.hpp>
#include <model/ThermostatSetpointDualSetpoint.hpp>
#include <model/ThermostatSetpointDualSetpoint_Impl.hpp>
#include <model/Version.hpp>
#include <model/Version_Impl.hpp>
#include <model/YearDescription.hpp>
#include <model/YearDescription_Impl.hpp>
#include <utilities/idf/IdfExtensibleGroup.hpp>
#include <utilities/idf/Workspace.hpp>
#include <utilities/idf/WorkspaceObjectOrder.hpp>
#include <utilities/core/Logger.hpp>
#include <utilities/core/Assert.hpp>
#include <utilities/geometry/Geometry.hpp>
#include <utilities/geometry/Point3d.hpp>
#include <utilities/geometry/Vector3d.hpp>
#include <utilities/geometry/EulerAngles.hpp>
#include <utilities/geometry/Transformation.hpp>
#include <utilities/idd/AirLoopHVAC_FieldEnums.hxx>
#include <utilities/idd/AirLoopHVAC_ControllerList_FieldEnums.hxx>
#include <utilities/idd/AirLoopHVAC_OutdoorAirSystem_FieldEnums.hxx>
#include <utilities/idd/AirLoopHVAC_SupplyPath_FieldEnums.hxx>
#include <utilities/idd/AirLoopHVAC_ReturnPath_FieldEnums.hxx>
#include <utilities/idd/AirLoopHVAC_ZoneMixer_FieldEnums.hxx>
#include <utilities/idd/AirLoopHVAC_ZoneSplitter_FieldEnums.hxx>
#include <utilities/idd/AirTerminal_SingleDuct_Uncontrolled_FieldEnums.hxx>
#include <utilities/idd/AvailabilityManagerAssignmentList_FieldEnums.hxx>
#include <utilities/idd/AvailabilityManager_Scheduled_FieldEnums.hxx>
#include <utilities/idd/BranchList_FieldEnums.hxx>
#include <utilities/idd/Branch_FieldEnums.hxx>
#include <utilities/idd/Building_FieldEnums.hxx>
#include <utilities/idd/Coil_Cooling_DX_SingleSpeed_FieldEnums.hxx>
#include <utilities/idd/Coil_Heating_Gas_FieldEnums.hxx>
#include <utilities/idd/Construction_FieldEnums.hxx>
#include <utilities/idd/Controller_OutdoorAir_FieldEnums.hxx>
#include <utilities/idd/Curve_Biquadratic_FieldEnums.hxx>
#include <utilities/idd/Curve_Quadratic_FieldEnums.hxx>
#include <utilities/idd/ElectricEquipment_FieldEnums.hxx>
#include <utilities/idd/Fan_ConstantVolume_FieldEnums.hxx>
#include <utilities/idd/Fan_VariableVolume_FieldEnums.hxx>
#include <utilities/idd/GasEquipment_FieldEnums.hxx>
#include <utilities/idd/Lights_FieldEnums.hxx>
#include <utilities/idd/Material_NoMass_FieldEnums.hxx>
#include <utilities/idd/Material_FieldEnums.hxx>
#include <utilities/idd/OutdoorAir_Mixer_FieldEnums.hxx>
#include <utilities/idd/People_FieldEnums.hxx>
#include <utilities/idd/RunPeriod_FieldEnums.hxx>
#include <utilities/idd/Schedule_Compact_FieldEnums.hxx>
#include <utilities/idd/SetpointManager_MixedAir_FieldEnums.hxx>
#include <utilities/idd/SetpointManager_SingleZone_Reheat_FieldEnums.hxx>
#include <utilities/idd/SimulationControl_FieldEnums.hxx>
#include <utilities/idd/Site_Location_FieldEnums.hxx>
#include <utilities/idd/Sizing_System_FieldEnums.hxx>
#include <utilities/idd/Sizing_Zone_FieldEnums.hxx>
#include <utilities/idd/ThermostatSetpoint_DualSetpoint_FieldEnums.hxx>
#include <utilities/idd/Version_FieldEnums.hxx>
#include <utilities/idd/WindowMaterial_SimpleGlazingSystem_FieldEnums.hxx>
#include <utilities/idd/Zone_FieldEnums.hxx>
#include <utilities/idd/ZoneList_FieldEnums.hxx>
#include <utilities/idd/ZoneControl_Thermostat_FieldEnums.hxx>
#include <utilities/idd/ZoneInfiltration_DesignFlowRate_FieldEnums.hxx>
#include <utilities/idd/ZoneInfiltration_EffectiveLeakageArea_FieldEnums.hxx>
#include <utilities/idd/ZoneHVAC_EquipmentConnections_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/IddFactory.hxx>

#include <utilities/time/Date.hpp>

using namespace openstudio::model;

using namespace std;

namespace openstudio {

namespace energyplus {

boost::optional<IdfObject> ForwardTranslator::translateRunPeriod( RunPeriod& modelObject )
{
  IdfObject runPeriod( openstudio::IddObjectType::RunPeriod );

  m_idfObjects.push_back(runPeriod);

  OptionalString s;

  s = modelObject.name();
  if( s )
  {
    runPeriod.setName(*s);
  }
  runPeriod.setInt(openstudio::RunPeriodFields::BeginMonth,modelObject.getBeginMonth());
  runPeriod.setInt(openstudio::RunPeriodFields::BeginDayofMonth,modelObject.getBeginDayOfMonth());
  runPeriod.setInt(openstudio::RunPeriodFields::EndMonth,modelObject.getEndMonth());
  runPeriod.setInt(openstudio::RunPeriodFields::EndDayofMonth,modelObject.getEndDayOfMonth());

  model::YearDescription yd = modelObject.model().getUniqueModelObject<model::YearDescription>();
  Date jan1 = yd.makeDate(1,1);

  // ETH@20121219 - This always hard codes a day of the week to start on, even if the user 
  // specified "UseWeatherFile". It is important to keep it this way for now, since we are
  // not parsing the start day out of the weather file, and we are hard-setting the day schedules
  // based on the settings/assumptions of YearDescription (assumes 2009 if 'UseWeatherFile').
  switch(jan1.dayOfWeek().value()){
    case DayOfWeek::Sunday:
      runPeriod.setString(openstudio::RunPeriodFields::DayofWeekforStartDay,"Sunday");
      break;
    case DayOfWeek::Monday:
      runPeriod.setString(openstudio::RunPeriodFields::DayofWeekforStartDay,"Monday");
      break;
    case DayOfWeek::Tuesday:
      runPeriod.setString(openstudio::RunPeriodFields::DayofWeekforStartDay,"Tuesday");
      break;
    case DayOfWeek::Wednesday:
      runPeriod.setString(openstudio::RunPeriodFields::DayofWeekforStartDay,"Wednesday");
      break;
    case DayOfWeek::Thursday:
      runPeriod.setString(openstudio::RunPeriodFields::DayofWeekforStartDay,"Thursday");
      break;
    case DayOfWeek::Friday:
      runPeriod.setString(openstudio::RunPeriodFields::DayofWeekforStartDay,"Friday");
      break;
    case DayOfWeek::Saturday:
      runPeriod.setString(openstudio::RunPeriodFields::DayofWeekforStartDay,"Saturday");
      break;
    default:
      ;
  }

  // ETH@20121219 - We don't currently support holidays. This makes sure that default day schedules 
  // do not get used unintentionally.
  // if( modelObject.getUseWeatherFileHolidays() )
  // {
  //   runPeriod.setString(openstudio::RunPeriodFields::UseWeatherFileHolidaysandSpecialDays,"Yes");
  // }
  // else
  // {
    runPeriod.setString(openstudio::RunPeriodFields::UseWeatherFileHolidaysandSpecialDays,"No");
  // }

  // ETH@20121219 - We also do not support pulling the daylight savings period in from the weather
  // file. Explicit RunPeriodControl:DaylightSavingTime objects already override this field, so 
  // making this change only affects files without such objects (and likely doesn't change results
  // for those since weather files from the E+ website have no such designations).
  // if( modelObject.getUseWeatherFileDaylightSavings() )
  // {
  //   runPeriod.setString(openstudio::RunPeriodFields::UseWeatherFileDaylightSavingPeriod,"Yes");
  // }
  // else
  // {
    runPeriod.setString(openstudio::RunPeriodFields::UseWeatherFileDaylightSavingPeriod,"No");
  // }

  if( modelObject.getApplyWeekendHolidayRule() )
  {
    runPeriod.setString(openstudio::RunPeriodFields::ApplyWeekendHolidayRule,"Yes");
  }
  else
  {
    runPeriod.setString(openstudio::RunPeriodFields::ApplyWeekendHolidayRule,"No");
  }

  if( modelObject.getUseWeatherFileRainInd() )
  {
    runPeriod.setString(openstudio::RunPeriodFields::UseWeatherFileRainIndicators,"Yes");
  }
  else
  {
    runPeriod.setString(openstudio::RunPeriodFields::UseWeatherFileRainIndicators,"No");
  }

  if( modelObject.getUseWeatherFileSnowInd() )
  {
    runPeriod.setString(openstudio::RunPeriodFields::UseWeatherFileSnowIndicators,"Yes");
  }
  else
  {
    runPeriod.setString(openstudio::RunPeriodFields::UseWeatherFileSnowIndicators,"No");
  }

  runPeriod.setInt(openstudio::RunPeriodFields::NumberofTimesRunperiodtobeRepeated,modelObject.getNumTimePeriodRepeats());

  return boost::optional<IdfObject>(runPeriod);
}

} // energyplus

} // openstudio

