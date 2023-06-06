/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
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
