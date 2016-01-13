/**********************************************************************
 *  Copyright (c) 2008-2016, Alliance for Sustainable Energy.
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
#include "../../model/Model.hpp"
#include "../../model/Schedule.hpp"
#include "../../model/Schedule_Impl.hpp"
#include "../../model/ModelObject.hpp"
#include "../../model/ModelObject_Impl.hpp"
#include "../../model/ZoneMixing.hpp"
#include "../../model/ZoneMixing_Impl.hpp"
#include "../../model/ThermalZone.hpp"
#include "../../model/ThermalZone_Impl.hpp"

#include <utilities/idd/ZoneMixing_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/IddFactory.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

boost::optional<IdfObject> ForwardTranslator::translateZoneMixing( ZoneMixing & modelObject )
{
  // Makes sure the modelObject gets put in the map, and that the new idfObject gets put in 
  // the final file. Also set's the idfObject's name.
  IdfObject idfObject = createRegisterAndNameIdfObject(IddObjectType::ZoneMixing, modelObject);

  boost::optional<double> value;

  // ZoneName 
  ThermalZone zone = modelObject.zone();
  translateAndMapModelObject(zone);
  idfObject.setString(ZoneMixingFields::ZoneName, zone.name().get());

  // ScheduleName 
  Schedule schedule = modelObject.schedule();
  translateAndMapModelObject(schedule);
  idfObject.setString(ZoneMixingFields::ScheduleName, schedule.name().get());

  // DesignFlowRateCalculationMethod
  idfObject.setString(ZoneMixingFields::DesignFlowRateCalculationMethod, modelObject.designFlowRateCalculationMethod());

  // DesignFlowRate
  value = modelObject.designFlowRate();
  if (value){
    idfObject.setDouble(ZoneMixingFields::DesignFlowRate, *value);
  }

  // FlowRateperZoneFloorArea
  value = modelObject.flowRateperZoneFloorArea();
  if (value){
    idfObject.setDouble(ZoneMixingFields::FlowRateperZoneFloorArea, *value);
  }

  // FlowRateperPerson
  value = modelObject.flowRateperPerson();
  if (value){
    idfObject.setDouble(ZoneMixingFields::FlowRateperPerson, *value);
  }

  // AirChangesperHour
  value = modelObject.airChangesperHour();
  if (value){
    idfObject.setDouble(ZoneMixingFields::AirChangesperHour, *value);
  }

  // SourceZone
  boost::optional<ThermalZone> sourceZone = modelObject.sourceZone();
  if (sourceZone){
    // DLM: do not translate source zone now, it will be translated at the right time
    idfObject.setString(ZoneMixingFields::SourceZoneName, sourceZone->name().get());
  }

  // DeltaTemperature
  value = modelObject.deltaTemperature();
  if (value){
    idfObject.setDouble(ZoneMixingFields::DeltaTemperature, *value);
  }

  // DeltaTemperatureScheduleName
  boost::optional<Schedule> optSchedule = modelObject.deltaTemperatureSchedule();
  if (optSchedule){
    translateAndMapModelObject(*optSchedule);
    idfObject.setString(ZoneMixingFields::DeltaTemperatureScheduleName, optSchedule->name().get());
  }
  
  // MinimumZoneTemperatureScheduleName
  optSchedule = modelObject.minimumZoneTemperatureSchedule();
  if (optSchedule){
    translateAndMapModelObject(*optSchedule);
    idfObject.setString(ZoneMixingFields::MinimumZoneTemperatureScheduleName, optSchedule->name().get());
  }

  // MaximumZoneTemperatureScheduleName
  optSchedule = modelObject.maximumZoneTemperatureSchedule();
  if (optSchedule){
    translateAndMapModelObject(*optSchedule);
    idfObject.setString(ZoneMixingFields::MaximumZoneTemperatureScheduleName, optSchedule->name().get());
  }

  // MinimumSourceZoneTemperatureScheduleName
  optSchedule = modelObject.minimumSourceZoneTemperatureSchedule();
  if (optSchedule){
    translateAndMapModelObject(*optSchedule);
    idfObject.setString(ZoneMixingFields::MinimumSourceZoneTemperatureScheduleName, optSchedule->name().get());
  }

  // MaximumSourceZoneTemperatureScheduleName
  optSchedule = modelObject.maximumSourceZoneTemperatureSchedule();
  if (optSchedule){
    translateAndMapModelObject(*optSchedule);
    idfObject.setString(ZoneMixingFields::MaximumSourceZoneTemperatureScheduleName, optSchedule->name().get());
  }

  // MinimumOutdoorTemperatureScheduleName
  optSchedule = modelObject.minimumOutdoorTemperatureSchedule();
  if (optSchedule){
    translateAndMapModelObject(*optSchedule);
    idfObject.setString(ZoneMixingFields::MinimumOutdoorTemperatureScheduleName, optSchedule->name().get());
  }

  // MaximumOutdoorTemperatureScheduleName
  optSchedule = modelObject.maximumOutdoorTemperatureSchedule();
  if (optSchedule){
    translateAndMapModelObject(*optSchedule);
    idfObject.setString(ZoneMixingFields::MaximumOutdoorTemperatureScheduleName, optSchedule->name().get());
  }

  return idfObject;
}

} // energyplus

} // openstudio

