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

#include "../ReverseTranslator.hpp"

#include "../../model/ZoneMixing.hpp"
#include "../../model/ZoneMixing_Impl.hpp"
#include "../../model/ThermalZone.hpp"
#include "../../model/ThermalZone_Impl.hpp"
#include "../../model/Schedule.hpp"
#include "../../model/Schedule_Impl.hpp"

#include <utilities/idd/ZoneMixing_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

#include "../../utilities/core/Assert.hpp"

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

OptionalModelObject ReverseTranslator::translateZoneMixing( const WorkspaceObject & workspaceObject )
{
  if( workspaceObject.iddObject().type() != IddObjectType::ZoneMixing ){
    LOG(Error, "WorkspaceObject is not IddObjectType: ZoneMixing");
    return boost::none;
  }

  OptionalWorkspaceObject target = workspaceObject.getTarget(openstudio::ZoneMixingFields::ZoneName);
  OptionalThermalZone zone;
  if (target){
    OptionalModelObject modelObject = translateAndMapWorkspaceObject(*target);
    if (modelObject){
      zone = modelObject->optionalCast<ThermalZone>();
    }
  }
  
  if (!zone){
    return boost::none;
  }

  openstudio::model::ZoneMixing mixing(*zone);
  
  OptionalString s = workspaceObject.name();
  if(s){
    mixing.setName(*s);
  }

  target = workspaceObject.getTarget(openstudio::ZoneMixingFields::ScheduleName);
  if (target){
    OptionalModelObject modelObject = translateAndMapWorkspaceObject(*target);
    if (modelObject){
      if (auto s = modelObject->optionalCast<Schedule>()){
        mixing.setSchedule(s.get());
      }
    }
  }

  s = workspaceObject.getString(openstudio::ZoneMixingFields::DesignFlowRateCalculationMethod, true);
  OS_ASSERT(s);

  OptionalDouble d;
  if (istringEqual("Flow/Zone", *s)){
    d = workspaceObject.getDouble(openstudio::ZoneMixingFields::DesignFlowRate);
    if (d){
      mixing.setDesignFlowRate(*d);
    } else{
      LOG(Error, "Flow/Zone value not found for workspace object " << workspaceObject);
    }
  } else if (istringEqual("Flow/Area", *s)){
    d = workspaceObject.getDouble(openstudio::ZoneMixingFields::FlowRateperZoneFloorArea);
    if (d){
      mixing.setFlowRateperZoneFloorArea(*d);
    } else{
      LOG(Error, "Flow/Area value not found for workspace object " << workspaceObject);
    }
  } else if (istringEqual("Flow/Person", *s)){
    d = workspaceObject.getDouble(openstudio::ZoneMixingFields::FlowRateperPerson);
    if (d){
      mixing.setFlowRateperPerson(*d);
    } else{
      LOG(Error, "Flow/Person value not found for workspace object " << workspaceObject);
    }
  } else if (istringEqual("AirChanges/Hour", *s)){
    d = workspaceObject.getDouble(openstudio::ZoneMixingFields::AirChangesperHour);
    if (d){
      mixing.setAirChangesperHour(*d);
    } else{
      LOG(Error, "AirChanges/Hour value not found for workspace object " << workspaceObject);
    }
  } else{
    LOG(Error, "Unknown DesignFlowRateCalculationMethod value for workspace object" << workspaceObject);
  }

  target = workspaceObject.getTarget(openstudio::ZoneMixingFields::SourceZoneName);
  if (target){
    OptionalModelObject modelObject = translateAndMapWorkspaceObject(*target);
    if (modelObject){
      if (modelObject->optionalCast<ThermalZone>()){
        mixing.setSourceZone(modelObject->cast<ThermalZone>());
      }
    }
  }

  d = workspaceObject.getDouble(openstudio::ZoneMixingFields::DeltaTemperature);
  if (d){
    mixing.setDeltaTemperature(*d);
  }

  target = workspaceObject.getTarget(openstudio::ZoneMixingFields::DeltaTemperatureScheduleName);
  if (target){
    OptionalModelObject modelObject = translateAndMapWorkspaceObject(*target);
    if (modelObject){
      if (auto s = modelObject->optionalCast<Schedule>()){
        mixing.setDeltaTemperatureSchedule(s.get());
      }
    }
  }

  target = workspaceObject.getTarget(openstudio::ZoneMixingFields::MinimumZoneTemperatureScheduleName);
  if (target){
    OptionalModelObject modelObject = translateAndMapWorkspaceObject(*target);
    if (modelObject){
      if (auto s = modelObject->optionalCast<Schedule>()){
        mixing.setMinimumZoneTemperatureSchedule(s.get());
      }
    }
  }

  target = workspaceObject.getTarget(openstudio::ZoneMixingFields::MaximumZoneTemperatureScheduleName);
  if (target){
    OptionalModelObject modelObject = translateAndMapWorkspaceObject(*target);
    if (modelObject){
      if (auto s = modelObject->optionalCast<Schedule>()){
        mixing.setMaximumZoneTemperatureSchedule(s.get());
      }
    }
  }

  target = workspaceObject.getTarget(openstudio::ZoneMixingFields::MinimumSourceZoneTemperatureScheduleName);
  if (target){
    OptionalModelObject modelObject = translateAndMapWorkspaceObject(*target);
    if (modelObject){
      if (auto s = modelObject->optionalCast<Schedule>()){
        mixing.setMinimumSourceZoneTemperatureSchedule(s.get());
      }
    }
  }

  target = workspaceObject.getTarget(openstudio::ZoneMixingFields::MaximumSourceZoneTemperatureScheduleName);
  if (target){
    OptionalModelObject modelObject = translateAndMapWorkspaceObject(*target);
    if (modelObject){
      if (auto s = modelObject->optionalCast<Schedule>()){
        mixing.setMaximumSourceZoneTemperatureSchedule(s.get());
      }
    }
  }

  target = workspaceObject.getTarget(openstudio::ZoneMixingFields::MinimumOutdoorTemperatureScheduleName);
  if (target){
    OptionalModelObject modelObject = translateAndMapWorkspaceObject(*target);
    if (modelObject){
      if (auto s = modelObject->optionalCast<Schedule>()){
        mixing.setMinimumOutdoorTemperatureSchedule(s.get());
      }
    }
  }

  target = workspaceObject.getTarget(openstudio::ZoneMixingFields::MaximumOutdoorTemperatureScheduleName);
  if (target){
    OptionalModelObject modelObject = translateAndMapWorkspaceObject(*target);
    if (modelObject){
      if (auto s = modelObject->optionalCast<Schedule>()){
        mixing.setMaximumOutdoorTemperatureSchedule(s.get());
      }
    }
  }

  return mixing;
}

} // energyplus

} // openstudio

