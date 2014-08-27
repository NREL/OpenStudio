/**********************************************************************
 *  Copyright (c) 2008-2014, Alliance for Sustainable Energy.
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

#include "../ForwardTranslator.hpp"
#include "../../model/Schedule.hpp"
#include "../../model/Schedule_Impl.hpp"
#include "../../model/ThermalZone.hpp"
#include "../../model/ThermalZone_Impl.hpp"
#include "../../model/ZoneControlThermostatStagedDualSetpoint.hpp"
#include "../../model/ZoneControlThermostatStagedDualSetpoint_Impl.hpp"
#include <utilities/idd/ZoneControl_Thermostat_StagedDualSetpoint_FieldEnums.hxx>

using namespace openstudio::model;

using namespace std;

namespace openstudio {

namespace energyplus {

boost::optional<IdfObject> ForwardTranslator::translateZoneControlThermostatStagedDualSetpoint( ZoneControlThermostatStagedDualSetpoint& modelObject )
{
  boost::optional<std::string> s;
  boost::optional<double> value;
  int num;

  IdfObject idfObject(openstudio::IddObjectType::ThermostatSetpoint_DualSetpoint);

  m_idfObjects.push_back(idfObject);

  s = modelObject.name();
  idfObject.setName(s.get());

  // ZoneorZoneListName
  if( auto zone = modelObject.thermalZone() ) {
    if( auto _zone = translateAndMapModelObject(zone.get()) ) {
      idfObject.setString(ZoneControl_Thermostat_StagedDualSetpointFields::ZoneorZoneListName,_zone->name().get());
    }
  }

  // NumberofHeatingStages
  num = modelObject.numberofHeatingStages();
  idfObject.setInt(ZoneControl_Thermostat_StagedDualSetpointFields::NumberofHeatingStages,num);

  // HeatingTemperatureSetpointScheduleName
  if( auto schedule = modelObject.heatingTemperatureSetpointSchedule() ) {
    if( auto _schedule = translateAndMapModelObject(schedule.get()) ) {
      idfObject.setString(ZoneControl_Thermostat_StagedDualSetpointFields::HeatingTemperatureSetpointScheduleName,_schedule->name().get());
    }
  }

  // HeatingThrottlingTemperatureRange
  if( value = modelObject.heatingThrottlingTemperatureRange() ) {
    idfObject.setDouble(ZoneControl_Thermostat_StagedDualSetpointFields::HeatingThrottlingTemperatureRange,value.get());
  }

  // Stage1HeatingTemperatureOffset
  if( value = modelObject.stage1HeatingTemperatureOffset() ) {
    idfObject.setDouble(ZoneControl_Thermostat_StagedDualSetpointFields::Stage1HeatingTemperatureOffset,value.get());
  }

  // Stage2HeatingTemperatureOffset
  if( value = modelObject.stage2HeatingTemperatureOffset() ) {
    idfObject.setDouble(ZoneControl_Thermostat_StagedDualSetpointFields::Stage2HeatingTemperatureOffset,value.get());
  }

  // Stage3HeatingTemperatureOffset
  if( value = modelObject.stage3HeatingTemperatureOffset() ) {
    idfObject.setDouble(ZoneControl_Thermostat_StagedDualSetpointFields::Stage3HeatingTemperatureOffset,value.get());
  }

  // Stage4HeatingTemperatureOffset
  if( value = modelObject.stage4HeatingTemperatureOffset() ) {
    idfObject.setDouble(ZoneControl_Thermostat_StagedDualSetpointFields::Stage4HeatingTemperatureOffset,value.get());
  }

  // NumberofCoolingStages
  num = modelObject.numberofCoolingStages();
  idfObject.setInt(ZoneControl_Thermostat_StagedDualSetpointFields::NumberofCoolingStages,num);

  // CoolingTemperatureSetpointBaseScheduleName
  if( auto schedule = modelObject.coolingTemperatureSetpointBaseSchedule() ) {
    if( auto _schedule = translateAndMapModelObject(schedule.get()) ) {
      idfObject.setString(ZoneControl_Thermostat_StagedDualSetpointFields::CoolingTemperatureSetpointBaseScheduleName,_schedule->name().get());
    }
  }

  // CoolingThrottlingTemperatureRange
  if( value = modelObject.coolingThrottlingTemperatureRange() ) {
    idfObject.setDouble(ZoneControl_Thermostat_StagedDualSetpointFields::CoolingThrottlingTemperatureRange,value.get());
  }

  // Stage1CoolingTemperatureOffset
  if( value = modelObject.stage1CoolingTemperatureOffset() ) {
    idfObject.setDouble(ZoneControl_Thermostat_StagedDualSetpointFields::Stage1CoolingTemperatureOffset,value.get());
  }

  // Stage2CoolingTemperatureOffset
  if( value = modelObject.stage2CoolingTemperatureOffset() ) {
    idfObject.setDouble(ZoneControl_Thermostat_StagedDualSetpointFields::Stage2CoolingTemperatureOffset,value.get());
  }

  // Stage3CoolingTemperatureOffset
  if( value = modelObject.stage3CoolingTemperatureOffset() ) {
    idfObject.setDouble(ZoneControl_Thermostat_StagedDualSetpointFields::Stage3CoolingTemperatureOffset,value.get());
  }

  // Stage4CoolingTemperatureOffset
  if( value = modelObject.stage4CoolingTemperatureOffset() ) {
    idfObject.setDouble(ZoneControl_Thermostat_StagedDualSetpointFields::Stage4CoolingTemperatureOffset,value.get());
  }

  return boost::optional<IdfObject>(idfObject);
}

} // energyplus

} // openstudio

