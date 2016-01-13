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

#include "../../model/ScheduleWeek.hpp"
#include "../../model/ScheduleWeek_Impl.hpp"
#include "../../model/ScheduleDay.hpp"
#include "../../model/ScheduleDay_Impl.hpp"

#include <utilities/idd/Schedule_Week_Daily_FieldEnums.hxx>
#include <utilities/idd/OS_Schedule_Week_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

OptionalModelObject ReverseTranslator::translateScheduleWeekDaily(const WorkspaceObject & workspaceObject){
  if (workspaceObject.iddObject().type() != IddObjectType::Schedule_Week_Daily){
    LOG(Error, "WorkspaceObject is not IddObjectType: Schedule:Week");
    return boost::none;
  }

  // create the schedule
  ScheduleWeek scheduleWeek(m_model);

  OptionalString s = workspaceObject.name();
  if (s){
    scheduleWeek.setName(*s);
  }

  OptionalWorkspaceObject target = workspaceObject.getTarget(Schedule_Week_DailyFields::SundaySchedule_DayName);
  if (target){
    OptionalModelObject modelObject = translateAndMapWorkspaceObject(*target);
    if (modelObject){
      if (modelObject->optionalCast<ScheduleDay>()){
        scheduleWeek.setSundaySchedule(modelObject->cast<ScheduleDay>());
      }
    }
  }

  target = workspaceObject.getTarget(Schedule_Week_DailyFields::MondaySchedule_DayName);
  if (target){
    OptionalModelObject modelObject = translateAndMapWorkspaceObject(*target);
    if (modelObject){
      if (modelObject->optionalCast<ScheduleDay>()){
        scheduleWeek.setMondaySchedule(modelObject->cast<ScheduleDay>());
      }
    }
  }

  target = workspaceObject.getTarget(Schedule_Week_DailyFields::TuesdaySchedule_DayName);
  if (target){
    OptionalModelObject modelObject = translateAndMapWorkspaceObject(*target);
    if (modelObject){
      if (modelObject->optionalCast<ScheduleDay>()){
        scheduleWeek.setTuesdaySchedule(modelObject->cast<ScheduleDay>());
      }
    }
  }

  target = workspaceObject.getTarget(Schedule_Week_DailyFields::WednesdaySchedule_DayName);
  if (target){
    OptionalModelObject modelObject = translateAndMapWorkspaceObject(*target);
    if (modelObject){
      if (modelObject->optionalCast<ScheduleDay>()){
        scheduleWeek.setWednesdaySchedule(modelObject->cast<ScheduleDay>());
      }
    }
  }
  
  target = workspaceObject.getTarget(Schedule_Week_DailyFields::ThursdaySchedule_DayName);
  if (target){
    OptionalModelObject modelObject = translateAndMapWorkspaceObject(*target);
    if (modelObject){
      if (modelObject->optionalCast<ScheduleDay>()){
        scheduleWeek.setThursdaySchedule(modelObject->cast<ScheduleDay>());
      }
    }
  }

  target = workspaceObject.getTarget(Schedule_Week_DailyFields::FridaySchedule_DayName);
  if (target){
    OptionalModelObject modelObject = translateAndMapWorkspaceObject(*target);
    if (modelObject){
      if (modelObject->optionalCast<ScheduleDay>()){
        scheduleWeek.setFridaySchedule(modelObject->cast<ScheduleDay>());
      }
    }
  }

  target = workspaceObject.getTarget(Schedule_Week_DailyFields::SaturdaySchedule_DayName);
  if (target){
    OptionalModelObject modelObject = translateAndMapWorkspaceObject(*target);
    if (modelObject){
      if (modelObject->optionalCast<ScheduleDay>()){
        scheduleWeek.setSaturdaySchedule(modelObject->cast<ScheduleDay>());
      }
    }
  }

  target = workspaceObject.getTarget(Schedule_Week_DailyFields::HolidaySchedule_DayName);
  if (target){
    OptionalModelObject modelObject = translateAndMapWorkspaceObject(*target);
    if (modelObject){
      if (modelObject->optionalCast<ScheduleDay>()){
        scheduleWeek.setHolidaySchedule(modelObject->cast<ScheduleDay>());
      }
    }
  }

  target = workspaceObject.getTarget(Schedule_Week_DailyFields::SummerDesignDaySchedule_DayName);
  if (target){
    OptionalModelObject modelObject = translateAndMapWorkspaceObject(*target);
    if (modelObject){
      if (modelObject->optionalCast<ScheduleDay>()){
        scheduleWeek.setSummerDesignDaySchedule(modelObject->cast<ScheduleDay>());
      }
    }
  }

  target = workspaceObject.getTarget(Schedule_Week_DailyFields::WinterDesignDaySchedule_DayName);
  if (target){
    OptionalModelObject modelObject = translateAndMapWorkspaceObject(*target);
    if (modelObject){
      if (modelObject->optionalCast<ScheduleDay>()){
        scheduleWeek.setWinterDesignDaySchedule(modelObject->cast<ScheduleDay>());
      }
    }
  }

  target = workspaceObject.getTarget(Schedule_Week_DailyFields::CustomDay1Schedule_DayName);
  if (target){
    OptionalModelObject modelObject = translateAndMapWorkspaceObject(*target);
    if (modelObject){
      if (modelObject->optionalCast<ScheduleDay>()){
        scheduleWeek.setCustomDay1Schedule(modelObject->cast<ScheduleDay>());
      }
    }
  }

  target = workspaceObject.getTarget(Schedule_Week_DailyFields::CustomDay2Schedule_DayName);
  if (target){
    OptionalModelObject modelObject = translateAndMapWorkspaceObject(*target);
    if (modelObject){
      if (modelObject->optionalCast<ScheduleDay>()){
        scheduleWeek.setCustomDay2Schedule(modelObject->cast<ScheduleDay>());
      }
    }
  }

  return scheduleWeek;
}

} // energyplus

} // openstudio
