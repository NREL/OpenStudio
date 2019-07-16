/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2019, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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
