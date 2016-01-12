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

#include "../../model/Space.hpp"
#include "../../model/Space_Impl.hpp"
#include "../../model/SpaceType.hpp"
#include "../../model/SpaceType_Impl.hpp"
#include "../../model/Schedule.hpp"
#include "../../model/Schedule_Impl.hpp"

#include <utilities/idd/ZoneVentilation_DesignFlowRate_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

OptionalModelObject ReverseTranslator::translateZoneVentilationDesignFlowRate( const WorkspaceObject & workspaceObject )
{
  if( workspaceObject.iddObject().type() != IddObjectType::ZoneVentilation_DesignFlowRate ){
    LOG(Error, "WorkspaceObject is not IddObjectType: ZoneVentilation:DesignFlowRate");
    return boost::none;
  }

  return boost::none;

/*
  openstudio::model::SpaceVentilationDesignFlowRate ventilation(m_model);
  
  OptionalString s = workspaceObject.name();
  if(s){
    ventilation.setName(*s);
  }

  OptionalWorkspaceObject target = workspaceObject.getTarget(openstudio::ZoneVentilation_DesignFlowRateFields::ZoneorZoneListName);
  if (target){
    OptionalModelObject modelObject = translateAndMapWorkspaceObject(*target);
    if (modelObject){
      if (modelObject->optionalCast<Space>()){
        ventilation.setSpace(modelObject->cast<Space>());
      }else if (modelObject->optionalCast<SpaceType>()){
        ventilation.setSpaceType(modelObject->cast<SpaceType>());
      }
    }
  }

  target = workspaceObject.getTarget(openstudio::ZoneVentilation_DesignFlowRateFields::ScheduleName);
  if (target){
    OptionalModelObject modelObject = translateAndMapWorkspaceObject(*target);
    if (modelObject){
      if (modelObject->optionalCast<Schedule>()){
        ventilation.setSchedule(modelObject->cast<Schedule>());
      }
    }
  }

  s = workspaceObject.getString(openstudio::ZoneVentilation_DesignFlowRateFields::DesignFlowRateCalculationMethod, true);
  OS_ASSERT(s);

  OptionalDouble d;
  if (istringEqual("Flow/Zone", *s)){
    d = workspaceObject.getDouble(openstudio::ZoneVentilation_DesignFlowRateFields::DesignFlowRate);
    if (d){
      ventilation.setDesignFlowRate(*d);
    }else{
      LOG(Error, "Flow/Zone value not found for workspace object " << workspaceObject);
    }
  }else if(istringEqual("Flow/Area", *s)){
    d = workspaceObject.getDouble(openstudio::ZoneVentilation_DesignFlowRateFields::FlowRateperZoneFloorArea);
    if (d){
      ventilation.setFlowRateperSpaceFloorArea(*d);
    }else{
      LOG(Error, "Flow/Area value not found for workspace object " << workspaceObject);
    }
  }else if(istringEqual("Flow/Person", *s)){
    d = workspaceObject.getDouble(openstudio::ZoneVentilation_DesignFlowRateFields::FlowRateperPerson);
    if (d){
      ventilation.setFlowRateperPerson(*d);
    }else{
      LOG(Error, "Flow/Person value not found for workspace object " << workspaceObject);
    }
  }else if(istringEqual("AirChanges/Hour", *s)){
    d = workspaceObject.getDouble(openstudio::ZoneVentilation_DesignFlowRateFields::AirChangesperHour);
    if (d){
      ventilation.setAirChangesperHour(*d);
    }else{
      LOG(Error, "AirChanges/Hour value not found for workspace object " << workspaceObject);
    }
  }else{
    LOG(Error, "Unknown DesignLevelCalculationMethod value for workspace object" << workspaceObject);
  }

  s = workspaceObject.getString(openstudio::ZoneVentilation_DesignFlowRateFields::VentilationType);
  if (s){
    ventilation.setVentilationType(*s);
  }

  d = workspaceObject.getDouble(openstudio::ZoneVentilation_DesignFlowRateFields::FanPressureRise);
  if (d){
    ventilation.setFanPressureRise(*d);
  }
    
  d = workspaceObject.getDouble(openstudio::ZoneVentilation_DesignFlowRateFields::FanTotalEfficiency);
  if (d){
    ventilation.setFanTotalEfficiency(*d);
  }

  d = workspaceObject.getDouble(openstudio::ZoneVentilation_DesignFlowRateFields::ConstantTermCoefficient);
  if (d){
    ventilation.setConstantTermCoefficient(*d);
  }

  d = workspaceObject.getDouble(openstudio::ZoneVentilation_DesignFlowRateFields::TemperatureTermCoefficient);
  if (d){
    ventilation.setTemperatureTermCoefficient(*d);
  }

  d = workspaceObject.getDouble(openstudio::ZoneVentilation_DesignFlowRateFields::VelocityTermCoefficient);
  if (d){
    ventilation.setVelocityTermCoefficient(*d);
  }

  d = workspaceObject.getDouble(openstudio::ZoneVentilation_DesignFlowRateFields::VelocitySquaredTermCoefficient);
  if (d){
    ventilation.setVelocitySquaredTermCoefficient(*d);
  }

  d = workspaceObject.getDouble(openstudio::ZoneVentilation_DesignFlowRateFields::MinimumIndoorTemperature);
  if (d){
    ventilation.setMinimumIndoorTemperature(*d);
  }

  target = workspaceObject.getTarget(openstudio::ZoneVentilation_DesignFlowRateFields::MinimumIndoorTemperatureScheduleName);
  if (target){
    OptionalModelObject modelObject = translateAndMapWorkspaceObject(*target);
    if (modelObject){
      if (modelObject->optionalCast<Schedule>()){
        ventilation.setMinimumIndoorTemperatureSchedule(modelObject->cast<Schedule>());
      }
    }
  }

  d = workspaceObject.getDouble(openstudio::ZoneVentilation_DesignFlowRateFields::MaximumIndoorTemperature);
  if (d){
    ventilation.setMaximumIndoorTemperature(*d);
  }

  target = workspaceObject.getTarget(openstudio::ZoneVentilation_DesignFlowRateFields::MaximumIndoorTemperatureScheduleName);
  if (target){
    OptionalModelObject modelObject = translateAndMapWorkspaceObject(*target);
    if (modelObject){
      if (modelObject->optionalCast<Schedule>()){
        ventilation.setMaximumIndoorTemperatureSchedule(modelObject->cast<Schedule>());
      }
    }
  }

  d = workspaceObject.getDouble(openstudio::ZoneVentilation_DesignFlowRateFields::DeltaTemperature);
  if (d){
    ventilation.setDeltaTemperature(*d);
  }

  target = workspaceObject.getTarget(openstudio::ZoneVentilation_DesignFlowRateFields::DeltaTemperatureScheduleName);
  if (target){
    OptionalModelObject modelObject = translateAndMapWorkspaceObject(*target);
    if (modelObject){
      if (modelObject->optionalCast<Schedule>()){
        ventilation.setDeltaTemperatureSchedule(modelObject->cast<Schedule>());
      }
    }
  }

  d = workspaceObject.getDouble(openstudio::ZoneVentilation_DesignFlowRateFields::MinimumOutdoorTemperature);
  if (d){
    ventilation.setMinimumOutdoorTemperature(*d);
  }

  target = workspaceObject.getTarget(openstudio::ZoneVentilation_DesignFlowRateFields::MinimumOutdoorTemperatureScheduleName);
  if (target){
    OptionalModelObject modelObject = translateAndMapWorkspaceObject(*target);
    if (modelObject){
      if (modelObject->optionalCast<Schedule>()){
        ventilation.setMinimumOutdoorTemperatureSchedule(modelObject->cast<Schedule>());
      }
    }
  }

  d = workspaceObject.getDouble(openstudio::ZoneVentilation_DesignFlowRateFields::MaximumOutdoorTemperature);
  if (d){
    ventilation.setMaximumOutdoorTemperature(*d);
  }

  target = workspaceObject.getTarget(openstudio::ZoneVentilation_DesignFlowRateFields::MaximumOutdoorTemperatureScheduleName);
  if (target){
    OptionalModelObject modelObject = translateAndMapWorkspaceObject(*target);
    if (modelObject){
      if (modelObject->optionalCast<Schedule>()){
        ventilation.setMaximumOutdoorTemperatureSchedule(modelObject->cast<Schedule>());
      }
    }
  }

  d = workspaceObject.getDouble(openstudio::ZoneVentilation_DesignFlowRateFields::MaximumWindSpeed);
  if (d){
    ventilation.setMaximumWindSpeed(*d);
  }

  return ventilation;
  */
}

} // energyplus

} // openstudio

