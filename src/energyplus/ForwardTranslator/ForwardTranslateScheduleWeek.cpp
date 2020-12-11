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

#include "../ForwardTranslator.hpp"
#include "../../model/Model.hpp"

#include "../../model/ScheduleTypeLimits.hpp"
#include "../../model/ScheduleTypeLimits_Impl.hpp"
#include "../../model/ScheduleWeek.hpp"
#include "../../model/ScheduleWeek_Impl.hpp"
#include "../../model/ScheduleDay.hpp"
#include "../../model/ScheduleDay_Impl.hpp"

#include "../../utilities/idf/IdfExtensibleGroup.hpp"
#include "../../utilities/idf/Workspace.hpp"

#include "../../utilities/core/Logger.hpp"
#include "../../utilities/core/Assert.hpp"

#include <utilities/idd/Schedule_Week_Daily_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

using namespace std;

namespace openstudio {

namespace energyplus {

#define MAP_SCHEDULE(_idfObject, _fieldIndex, _scheduleDay)                                     \
  if (_scheduleDay) {                                                                           \
    boost::optional<IdfObject> idfScheduleDay = translateAndMapModelObject(_scheduleDay.get()); \
    if (idfScheduleDay) {                                                                       \
      test = _idfObject.setString(_fieldIndex, idfScheduleDay->name().get()) && test;           \
    } else {                                                                                    \
      test = false;                                                                             \
    }                                                                                           \
  }

  boost::optional<IdfObject> ForwardTranslator::translateScheduleWeek(ScheduleWeek& modelObject) {
    IdfObject scheduleWeek(openstudio::IddObjectType::Schedule_Week_Daily);

    m_idfObjects.push_back(scheduleWeek);

    scheduleWeek.setName(modelObject.name().get());

    // all fields in scheduleWeek are required, attempt to make sure they are all set
    if (!modelObject.holidaySchedule()) {
      if (modelObject.sundaySchedule()) {
        modelObject.setHolidaySchedule(modelObject.sundaySchedule().get());
      }
    }
    if (!modelObject.customDay1Schedule()) {
      if (modelObject.holidaySchedule()) {
        modelObject.setCustomDay1Schedule(modelObject.holidaySchedule().get());
      }
    }
    if (!modelObject.customDay2Schedule()) {
      if (modelObject.holidaySchedule()) {
        modelObject.setCustomDay2Schedule(modelObject.holidaySchedule().get());
      }
    }

    bool test = true;
    MAP_SCHEDULE(scheduleWeek, Schedule_Week_DailyFields::SundaySchedule_DayName, modelObject.sundaySchedule());
    MAP_SCHEDULE(scheduleWeek, Schedule_Week_DailyFields::MondaySchedule_DayName, modelObject.mondaySchedule());
    MAP_SCHEDULE(scheduleWeek, Schedule_Week_DailyFields::TuesdaySchedule_DayName, modelObject.tuesdaySchedule());
    MAP_SCHEDULE(scheduleWeek, Schedule_Week_DailyFields::WednesdaySchedule_DayName, modelObject.wednesdaySchedule());
    MAP_SCHEDULE(scheduleWeek, Schedule_Week_DailyFields::ThursdaySchedule_DayName, modelObject.thursdaySchedule());
    MAP_SCHEDULE(scheduleWeek, Schedule_Week_DailyFields::FridaySchedule_DayName, modelObject.fridaySchedule());
    MAP_SCHEDULE(scheduleWeek, Schedule_Week_DailyFields::SaturdaySchedule_DayName, modelObject.saturdaySchedule());
    MAP_SCHEDULE(scheduleWeek, Schedule_Week_DailyFields::HolidaySchedule_DayName, modelObject.holidaySchedule());
    MAP_SCHEDULE(scheduleWeek, Schedule_Week_DailyFields::SummerDesignDaySchedule_DayName, modelObject.summerDesignDaySchedule());
    MAP_SCHEDULE(scheduleWeek, Schedule_Week_DailyFields::WinterDesignDaySchedule_DayName, modelObject.winterDesignDaySchedule());
    MAP_SCHEDULE(scheduleWeek, Schedule_Week_DailyFields::CustomDay1Schedule_DayName, modelObject.customDay1Schedule());
    MAP_SCHEDULE(scheduleWeek, Schedule_Week_DailyFields::CustomDay2Schedule_DayName, modelObject.customDay2Schedule());

    if (!test) {
      LOG(Warn, "ScheduleWeek '" << modelObject.name().get() << "' is missing required fields");
    }

    return scheduleWeek;
  }

}  // namespace energyplus

}  // namespace openstudio
