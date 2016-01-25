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

#include "../../model/ScheduleTypeLimits.hpp"
#include "../../model/ScheduleTypeLimits_Impl.hpp"
#include "../../model/ScheduleRuleset.hpp"
#include "../../model/ScheduleRuleset_Impl.hpp"
#include "../../model/ScheduleRule.hpp"
#include "../../model/ScheduleRule_Impl.hpp"
#include "../../model/ScheduleDay.hpp"
#include "../../model/ScheduleDay_Impl.hpp"
#include "../../model/YearDescription.hpp"
#include "../../model/YearDescription_Impl.hpp"

#include "../../utilities/idf/IdfExtensibleGroup.hpp"
#include "../../utilities/idf/Workspace.hpp"
#include "../../utilities/time/Date.hpp"
#include "../../utilities/time/Time.hpp"

#include "../../utilities/core/Logger.hpp"
#include "../../utilities/core/Assert.hpp"

#include <utilities/idd/Schedule_Week_Daily_FieldEnums.hxx>
#include <utilities/idd/Schedule_Year_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

using namespace std;

namespace openstudio {

namespace energyplus {

struct WeekScheduleStruct{

  std::string name;
  std::string sundaySchedule;
  std::string mondaySchedule;
  std::string tuesdaySchedule;
  std::string wednesdaySchedule;
  std::string thursdaySchedule;
  std::string fridaySchedule;
  std::string saturdaySchedule;
  std::string holidayDaySchedule;
  std::string summerDesignDaySchedule;
  std::string winterDesignDaySchedule;
  std::string customDay1Schedule;
  std::string customDay2Schedule;

  bool operator==(const WeekScheduleStruct& other)
  {
    return (sundaySchedule == other.sundaySchedule &&
            mondaySchedule == other.mondaySchedule &&
            tuesdaySchedule == other.tuesdaySchedule &&
            wednesdaySchedule == other.wednesdaySchedule &&
            thursdaySchedule == other.thursdaySchedule &&
            fridaySchedule == other.fridaySchedule &&
            saturdaySchedule == other.saturdaySchedule &&
            holidayDaySchedule == other.holidayDaySchedule &&
            summerDesignDaySchedule == other.summerDesignDaySchedule &&
            winterDesignDaySchedule == other.winterDesignDaySchedule &&
            customDay1Schedule == other.customDay1Schedule &&
            customDay2Schedule == other.customDay2Schedule);
  }

  IdfObject toIdfObject()
  {
    IdfObject weekSchedule(openstudio::IddObjectType::Schedule_Week_Daily);
    weekSchedule.createName();
    this->name = weekSchedule.name().get();
    weekSchedule.setString(Schedule_Week_DailyFields::SundaySchedule_DayName, sundaySchedule);
    weekSchedule.setString(Schedule_Week_DailyFields::MondaySchedule_DayName, mondaySchedule);
    weekSchedule.setString(Schedule_Week_DailyFields::TuesdaySchedule_DayName, tuesdaySchedule);
    weekSchedule.setString(Schedule_Week_DailyFields::WednesdaySchedule_DayName, wednesdaySchedule);
    weekSchedule.setString(Schedule_Week_DailyFields::ThursdaySchedule_DayName, thursdaySchedule);
    weekSchedule.setString(Schedule_Week_DailyFields::FridaySchedule_DayName, fridaySchedule);
    weekSchedule.setString(Schedule_Week_DailyFields::SaturdaySchedule_DayName, saturdaySchedule);
    weekSchedule.setString(Schedule_Week_DailyFields::HolidaySchedule_DayName, holidayDaySchedule);
    weekSchedule.setString(Schedule_Week_DailyFields::SummerDesignDaySchedule_DayName, summerDesignDaySchedule);
    weekSchedule.setString(Schedule_Week_DailyFields::WinterDesignDaySchedule_DayName, winterDesignDaySchedule);
    weekSchedule.setString(Schedule_Week_DailyFields::CustomDay1Schedule_DayName, customDay1Schedule);
    weekSchedule.setString(Schedule_Week_DailyFields::CustomDay2Schedule_DayName, customDay2Schedule);
    return weekSchedule;
  }

};


boost::optional<IdfObject> ForwardTranslator::translateScheduleRuleset( ScheduleRuleset & modelObject )
{

  IdfObject scheduleYear( openstudio::IddObjectType::Schedule_Year );

  scheduleYear.setName(modelObject.name().get());

  boost::optional<ScheduleTypeLimits> scheduleTypeLimits = modelObject.scheduleTypeLimits();
  if (scheduleTypeLimits){
    boost::optional<IdfObject> idfObject = translateAndMapModelObject(*scheduleTypeLimits);
    if (idfObject){
      scheduleYear.setString(Schedule_YearFields::ScheduleTypeLimitsName, idfObject->name().get());
    }
  }

  try {
    ScheduleDay defaultDaySchedule = modelObject.defaultDaySchedule();
    ScheduleDay summerDesignDaySchedule = modelObject.summerDesignDaySchedule();
    ScheduleDay winterDesignDaySchedule = modelObject.winterDesignDaySchedule();

    // initialize day of week schedules
    ScheduleDay sundaySchedule = defaultDaySchedule;
    ScheduleDay mondaySchedule = defaultDaySchedule;
    ScheduleDay tuesdaySchedule = defaultDaySchedule;
    ScheduleDay wednesdaySchedule = defaultDaySchedule;
    ScheduleDay thursdaySchedule = defaultDaySchedule;
    ScheduleDay fridaySchedule = defaultDaySchedule;
    ScheduleDay saturdaySchedule = defaultDaySchedule;

    // these are not yet exposed
    ScheduleDay holidayDaySchedule = defaultDaySchedule;
    ScheduleDay customDay1Schedule = defaultDaySchedule;
    ScheduleDay customDay2Schedule = defaultDaySchedule;

    // loop over entire year
    model::YearDescription yd = modelObject.model().getUniqueModelObject<model::YearDescription>();
    openstudio::Date jan1 = yd.makeDate(MonthOfYear::Jan, 1);
    openstudio::Date dec31 = yd.makeDate(MonthOfYear::Dec, 31);

    // this is the current date we are at in the year
    openstudio::Date date = jan1;

    // this is the week schedule for the week ending on the last saturday before the current date
    boost::optional<WeekScheduleStruct> weekSchedule;

    // this is the week schedule for the week before weekSchedule
    boost::optional<WeekScheduleStruct> lastWeekSchedule;

    // this is the saturday on which lastWeekSchedule ends
    openstudio::Date lastDate;

    // iterate over the schedule for each day of the year
    std::vector<ScheduleDay> daySchedules = modelObject.getDaySchedules(jan1, dec31);
    for (ScheduleDay& daySchedule : daySchedules){

      // translate the day schedule
      translateAndMapModelObject(daySchedule);

      // set day of week schedule
      switch(date.dayOfWeek().value()){
        case DayOfWeek::Sunday:
          sundaySchedule = daySchedule;
          break;
        case DayOfWeek::Monday:
          mondaySchedule = daySchedule;
          break;
        case DayOfWeek::Tuesday:
          tuesdaySchedule = daySchedule;
          break;
        case DayOfWeek::Wednesday:
          wednesdaySchedule = daySchedule;
          break;
        case DayOfWeek::Thursday:
          thursdaySchedule = daySchedule;
          break;
        case DayOfWeek::Friday:
          fridaySchedule = daySchedule;
          break;
        case DayOfWeek::Saturday:
          saturdaySchedule = daySchedule;
          break;
        default:
          OS_ASSERT(false);
      }

      // update week schedules each saturday
      if((date.dayOfWeek().value() == DayOfWeek::Saturday)){

        // set last week schedule before we overwrite week schedule
        lastDate = date - Time(7);
        lastWeekSchedule = weekSchedule;

        // set the week schedule
        weekSchedule = WeekScheduleStruct();
        weekSchedule->sundaySchedule = sundaySchedule.name().get();
        weekSchedule->mondaySchedule = mondaySchedule.name().get();
        weekSchedule->tuesdaySchedule = tuesdaySchedule.name().get();
        weekSchedule->wednesdaySchedule = wednesdaySchedule.name().get();
        weekSchedule->thursdaySchedule = thursdaySchedule.name().get();
        weekSchedule->fridaySchedule = fridaySchedule.name().get();
        weekSchedule->saturdaySchedule = saturdaySchedule.name().get();
        weekSchedule->holidayDaySchedule = holidayDaySchedule.name().get();
        weekSchedule->summerDesignDaySchedule = summerDesignDaySchedule.name().get();
        weekSchedule->winterDesignDaySchedule = winterDesignDaySchedule.name().get();
        weekSchedule->customDay1Schedule = customDay1Schedule.name().get();
        weekSchedule->customDay2Schedule = customDay2Schedule.name().get();

        // check if this schedule is equal to last week schedule
        if (weekSchedule && lastWeekSchedule && ( !(weekSchedule.get() == lastWeekSchedule.get()) )){
          // if not write out last week schedule
          m_idfObjects.push_back(lastWeekSchedule->toIdfObject());

          // get last extensible group, if any, to find start date otherwise use jan1
          openstudio::Date startDate;
          std::vector<IdfExtensibleGroup> extensibleGroups = scheduleYear.extensibleGroups();
          if (extensibleGroups.empty()){
            startDate = jan1;
          }else{
            // day after last end date
            boost::optional<int> startMonth = extensibleGroups.back().getInt(3,true);
            OS_ASSERT(startMonth);
            boost::optional<int> startDay = extensibleGroups.back().getInt(4,true);
            OS_ASSERT(startDay);
            startDate = yd.makeDate(*startMonth, *startDay) + Time(1);
          }

          OS_ASSERT(startDate <= lastDate);

          // add the values
          std::vector<std::string> values;
          values.push_back(lastWeekSchedule->name);
          values.push_back(boost::lexical_cast<std::string>(startDate.monthOfYear().value()));
          values.push_back(boost::lexical_cast<std::string>(startDate.dayOfMonth()));
          values.push_back(boost::lexical_cast<std::string>(lastDate.monthOfYear().value()));
          values.push_back(boost::lexical_cast<std::string>(lastDate.dayOfMonth()));
          IdfExtensibleGroup test = scheduleYear.pushExtensibleGroup(values);
          OS_ASSERT(!test.empty());
        }
      }

      // if last day of year update and write out current week schedule
      if (date == dec31){

        // we may now think of date as being the next saturday on or after 12/31

        // set last week schedule before we overwrite week schedule
        lastDate = date - Time(1);
        while (lastDate.dayOfWeek().value() != DayOfWeek::Saturday){
          lastDate = lastDate - Time(1);
          lastWeekSchedule = weekSchedule;
        }

        // set the week schedule, some of these dates may extend past 12/31
        weekSchedule = WeekScheduleStruct();
        weekSchedule->sundaySchedule = sundaySchedule.name().get();
        weekSchedule->mondaySchedule = mondaySchedule.name().get();
        weekSchedule->tuesdaySchedule = tuesdaySchedule.name().get();
        weekSchedule->wednesdaySchedule = wednesdaySchedule.name().get();
        weekSchedule->thursdaySchedule = thursdaySchedule.name().get();
        weekSchedule->fridaySchedule = fridaySchedule.name().get();
        weekSchedule->saturdaySchedule = saturdaySchedule.name().get();
        weekSchedule->holidayDaySchedule = holidayDaySchedule.name().get();
        weekSchedule->summerDesignDaySchedule = summerDesignDaySchedule.name().get();
        weekSchedule->winterDesignDaySchedule = winterDesignDaySchedule.name().get();
        weekSchedule->customDay1Schedule = customDay1Schedule.name().get();
        weekSchedule->customDay2Schedule = customDay2Schedule.name().get();

        // check if this schedule is equal to last week schedule
        if (weekSchedule && lastWeekSchedule && ( !(weekSchedule.get() == lastWeekSchedule.get()) )){
          // if not write out last week schedule
          m_idfObjects.push_back(lastWeekSchedule->toIdfObject());

          // get last extensible group, if any, to find start date otherwise use jan1
          openstudio::Date startDate;
          std::vector<IdfExtensibleGroup> extensibleGroups = scheduleYear.extensibleGroups();
          if (extensibleGroups.empty()){
            startDate = jan1;
          }else{
            // day after last end date
            boost::optional<int> startMonth = extensibleGroups.back().getInt(3,true);
            OS_ASSERT(startMonth);
            boost::optional<int> startDay = extensibleGroups.back().getInt(4,true);
            OS_ASSERT(startDay);
            startDate = yd.makeDate(*startMonth, *startDay) + Time(1);
          }

          OS_ASSERT(startDate <= lastDate);

          // add the values
          std::vector<std::string> values;
          values.push_back(lastWeekSchedule->name);
          values.push_back(boost::lexical_cast<std::string>(startDate.monthOfYear().value()));
          values.push_back(boost::lexical_cast<std::string>(startDate.dayOfMonth()));
          values.push_back(boost::lexical_cast<std::string>(lastDate.monthOfYear().value()));
          values.push_back(boost::lexical_cast<std::string>(lastDate.dayOfMonth()));
          IdfExtensibleGroup test = scheduleYear.pushExtensibleGroup(values);
          OS_ASSERT(!test.empty());
        }

        // write out the last week schedule
        m_idfObjects.push_back(weekSchedule->toIdfObject());

        // get last extensible group, if any, to find start date otherwise use jan1
        openstudio::Date startDate;
        std::vector<IdfExtensibleGroup> extensibleGroups = scheduleYear.extensibleGroups();
        if (extensibleGroups.empty()){
          startDate = jan1;
        }else{
          // day after last end date
          boost::optional<int> startMonth = extensibleGroups.back().getInt(3,true);
          OS_ASSERT(startMonth);
          boost::optional<int> startDay = extensibleGroups.back().getInt(4,true);
          OS_ASSERT(startDay);
          startDate = yd.makeDate(*startMonth, *startDay) + Time(1);
        }

        OS_ASSERT(startDate <= date);

        // add the values
        std::vector<std::string> values;
        values.push_back(weekSchedule->name);
        values.push_back(boost::lexical_cast<std::string>(startDate.monthOfYear().value()));
        values.push_back(boost::lexical_cast<std::string>(startDate.dayOfMonth()));
        values.push_back(boost::lexical_cast<std::string>(date.monthOfYear().value()));
        values.push_back(boost::lexical_cast<std::string>(date.dayOfMonth()));
        IdfExtensibleGroup test = scheduleYear.pushExtensibleGroup(values);
        OS_ASSERT(!test.empty());
      }

      // increment date
      date += Time(1);
    }

  }
  catch (std::exception& e) {
    LOG(Error,"Unable to translate " << modelObject.briefDescription() 
        << " to EnergyPlus IDF, because " << e.what() << ".");
    return boost::none;
  }

  m_idfObjects.push_back(scheduleYear);

  m_map.insert(std::make_pair(modelObject.handle(), scheduleYear));

  // translate day schedules

  ScheduleDay defaultDaySchedule = modelObject.defaultDaySchedule();
  ScheduleDay summerDesignDaySchedule = modelObject.summerDesignDaySchedule();
  ScheduleDay winterDesignDaySchedule = modelObject.winterDesignDaySchedule();

  translateAndMapModelObject(defaultDaySchedule);
  translateAndMapModelObject(summerDesignDaySchedule);
  translateAndMapModelObject(winterDesignDaySchedule);

  // translate schedule rules, these are returned in order
  for (ScheduleRule scheduleRule : modelObject.scheduleRules()){
    ScheduleDay daySchedule = scheduleRule.daySchedule();
    translateAndMapModelObject(daySchedule);
  }

  return scheduleYear;
}

} // energyplus

} // openstudio

