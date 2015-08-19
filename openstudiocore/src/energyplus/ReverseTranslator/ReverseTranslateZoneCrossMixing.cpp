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

#include "../ReverseTranslator.hpp"

#include "../../model/ZoneMixing.hpp"
#include "../../model/ZoneMixing_Impl.hpp"
#include "../../model/ThermalZone.hpp"
#include "../../model/ThermalZone_Impl.hpp"
#include "../../model/Schedule.hpp"
#include "../../model/Schedule_Impl.hpp"

#include <utilities/idd/ZoneCrossMixing_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

#include "../../utilities/core/Assert.hpp"

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

OptionalModelObject ReverseTranslator::translateZoneCrossMixing( const WorkspaceObject & workspaceObject )
{
  if( workspaceObject.iddObject().type() != IddObjectType::ZoneCrossMixing ){
    LOG(Error, "WorkspaceObject is not IddObjectType: ZoneCrossMixing");
    return boost::none;
  }

  OptionalWorkspaceObject target = workspaceObject.getTarget(openstudio::ZoneCrossMixingFields::ZoneName);
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

  target = workspaceObject.getTarget(openstudio::ZoneCrossMixingFields::SourceZoneName);
  OptionalThermalZone sourceZone;
  if (target){
    OptionalModelObject modelObject = translateAndMapWorkspaceObject(*target);
    if (modelObject){
      sourceZone = modelObject->optionalCast<ThermalZone>();
    }
  }

  if (!sourceZone){
    return boost::none;
  }

  // Create the mixing object
  openstudio::model::ZoneMixing mixing(*zone);
  mixing.setSourceZone(*sourceZone);

  // Create the reverse mixing object
  openstudio::model::ZoneMixing reverseMixing(*sourceZone);
  reverseMixing.setSourceZone(*zone);
  
  OptionalString s = workspaceObject.name();
  if(s){
    mixing.setName(*s);
    reverseMixing.setName(*s + " Reverse");
  }

  target = workspaceObject.getTarget(openstudio::ZoneCrossMixingFields::ScheduleName);
  if (target){
    OptionalModelObject modelObject = translateAndMapWorkspaceObject(*target);
    if (modelObject){
      if (modelObject->optionalCast<Schedule>()){
        mixing.setSchedule(modelObject->cast<Schedule>());
        reverseMixing.setSchedule(modelObject->cast<Schedule>());
      }
    }
  }

  s = workspaceObject.getString(openstudio::ZoneCrossMixingFields::DesignFlowRateCalculationMethod, true);
  OS_ASSERT(s);

  OptionalDouble d;
  if (istringEqual("Flow/Zone", *s)){
    d = workspaceObject.getDouble(openstudio::ZoneCrossMixingFields::DesignFlowRate);
    if (d){
      mixing.setDesignFlowRate(*d);
      reverseMixing.setDesignFlowRate(*d);
    } else{
      LOG(Error, "Flow/Zone value not found for workspace object " << workspaceObject);
    }
  } else if (istringEqual("Flow/Area", *s)){
    d = workspaceObject.getDouble(openstudio::ZoneCrossMixingFields::FlowRateperZoneFloorArea);
    if (d){
      mixing.setFlowRateperZoneFloorArea(*d);
      reverseMixing.setFlowRateperZoneFloorArea(*d);
    } else{
      LOG(Error, "Flow/Area value not found for workspace object " << workspaceObject);
    }
  } else if (istringEqual("Flow/Person", *s)){
    d = workspaceObject.getDouble(openstudio::ZoneCrossMixingFields::FlowRateperPerson);
    if (d){
      mixing.setFlowRateperPerson(*d);
      reverseMixing.setFlowRateperPerson(*d);
    } else{
      LOG(Error, "Flow/Person value not found for workspace object " << workspaceObject);
    }
  } else if (istringEqual("AirChanges/Hour", *s)){
    d = workspaceObject.getDouble(openstudio::ZoneCrossMixingFields::AirChangesperHour);
    if (d){
      mixing.setAirChangesperHour(*d);
      reverseMixing.setAirChangesperHour(*d);
    } else{
      LOG(Error, "AirChanges/Hour value not found for workspace object " << workspaceObject);
    }
  } else{
    LOG(Error, "Unknown DesignFlowRateCalculationMethod value for workspace object" << workspaceObject);
  }


  d = workspaceObject.getDouble(openstudio::ZoneCrossMixingFields::DeltaTemperature);
  if (d){
    mixing.setDeltaTemperature(*d);
    reverseMixing.setDeltaTemperature(*d);
  }

  target = workspaceObject.getTarget(openstudio::ZoneCrossMixingFields::DeltaTemperatureScheduleName);
  if (target){
    OptionalModelObject modelObject = translateAndMapWorkspaceObject(*target);
    if (modelObject){
      if (modelObject->optionalCast<Schedule>()){
        mixing.setDeltaTemperatureSchedule(modelObject->cast<Schedule>());
        reverseMixing.setDeltaTemperatureSchedule(modelObject->cast<Schedule>());
      }
    }
  }

  target = workspaceObject.getTarget(openstudio::ZoneCrossMixingFields::MinimumZoneTemperatureScheduleName);
  if (target){
    OptionalModelObject modelObject = translateAndMapWorkspaceObject(*target);
    if (modelObject){
      if (modelObject->optionalCast<Schedule>()){
        mixing.setMinimumZoneTemperatureSchedule(modelObject->cast<Schedule>());
        reverseMixing.setMinimumZoneTemperatureSchedule(modelObject->cast<Schedule>());
      }
    }
  }

  target = workspaceObject.getTarget(openstudio::ZoneCrossMixingFields::MaximumZoneTemperatureScheduleName);
  if (target){
    OptionalModelObject modelObject = translateAndMapWorkspaceObject(*target);
    if (modelObject){
      if (modelObject->optionalCast<Schedule>()){
        mixing.setMaximumZoneTemperatureSchedule(modelObject->cast<Schedule>());
        reverseMixing.setMaximumZoneTemperatureSchedule(modelObject->cast<Schedule>());
      }
    }
  }

  target = workspaceObject.getTarget(openstudio::ZoneCrossMixingFields::MinimumSourceZoneTemperatureScheduleName);
  if (target){
    OptionalModelObject modelObject = translateAndMapWorkspaceObject(*target);
    if (modelObject){
      if (modelObject->optionalCast<Schedule>()){
        mixing.setMinimumSourceZoneTemperatureSchedule(modelObject->cast<Schedule>());
        reverseMixing.setMinimumSourceZoneTemperatureSchedule(modelObject->cast<Schedule>());
      }
    }
  }

  target = workspaceObject.getTarget(openstudio::ZoneCrossMixingFields::MaximumSourceZoneTemperatureScheduleName);
  if (target){
    OptionalModelObject modelObject = translateAndMapWorkspaceObject(*target);
    if (modelObject){
      if (modelObject->optionalCast<Schedule>()){
        mixing.setMaximumSourceZoneTemperatureSchedule(modelObject->cast<Schedule>());
        reverseMixing.setMaximumSourceZoneTemperatureSchedule(modelObject->cast<Schedule>());
      }
    }
  }

  target = workspaceObject.getTarget(openstudio::ZoneCrossMixingFields::MinimumOutdoorTemperatureScheduleName);
  if (target){
    OptionalModelObject modelObject = translateAndMapWorkspaceObject(*target);
    if (modelObject){
      if (modelObject->optionalCast<Schedule>()){
        mixing.setMinimumOutdoorTemperatureSchedule(modelObject->cast<Schedule>());
        reverseMixing.setMinimumOutdoorTemperatureSchedule(modelObject->cast<Schedule>());
      }
    }
  }

  target = workspaceObject.getTarget(openstudio::ZoneCrossMixingFields::MaximumOutdoorTemperatureScheduleName);
  if (target){
    OptionalModelObject modelObject = translateAndMapWorkspaceObject(*target);
    if (modelObject){
      if (modelObject->optionalCast<Schedule>()){
        mixing.setMaximumOutdoorTemperatureSchedule(modelObject->cast<Schedule>());
        reverseMixing.setMaximumOutdoorTemperatureSchedule(modelObject->cast<Schedule>());
      }
    }
  }

  return mixing;
}

} // energyplus

} // openstudio

