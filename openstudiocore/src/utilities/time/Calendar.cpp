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

#include "Calendar.hpp"
#include "DateTime.hpp"

using namespace std;
using namespace boost;

namespace openstudio{

  /// default constructor, uses assumed year
  Calendar::Calendar()
    :  m_startDate(Date(MonthOfYear::Jan, 1, YearDescription().assumedYear())), 
    m_endDate(Date(MonthOfYear::Dec, 31, YearDescription().assumedYear()))
  {}

  /// constructor with year
  Calendar::Calendar(int year)
    : m_startDate(Date(MonthOfYear::Jan, 1, year)), m_endDate(Date(MonthOfYear::Dec, 31, year))
  {}

  /// constructor with start and end dates
  Calendar::Calendar(const Date& startDate, const Date& endDate)
    : m_startDate(startDate), m_endDate(endDate)
  {}

  /// set daylight savings time, [daylightSavingsStart, daylightSavingsEnd)
  void Calendar::daylightSavings(const Date& daylightSavingsStart, const Date& daylightSavingsEnd)
  {
    if (daylightSavingsEnd <= daylightSavingsStart){
      LOG(Error, "End Date " << daylightSavingsEnd << " is before or equal to start date " << daylightSavingsStart);
      return;
    }

    if (!includesDate(daylightSavingsStart)){
      LOG(Error, "Calendar does not include daylight savings start date " << daylightSavingsStart);
      return;
    }

    if (!includesDate(daylightSavingsEnd)){
      LOG(Error, "Calendar does not include daylight savings end date " << daylightSavingsEnd);
      return;
    }

    m_daylightSavingsStart = daylightSavingsStart;
    m_daylightSavingsEnd = daylightSavingsEnd;
  }

  /// standard daylight savings time, [2nd Sunday in March, 1st Sunday in November)
  void Calendar::standardDaylightSavings()
  {
    m_daylightSavingsStart = Date::fromNthDayOfMonth(NthDayOfWeekInMonth::second, 
        DayOfWeek::Sunday, MonthOfYear::Mar, m_startDate.year());
    m_daylightSavingsEnd = Date::fromNthDayOfMonth(NthDayOfWeekInMonth::first, 
        DayOfWeek::Sunday, MonthOfYear::Nov, m_startDate.year());
  }

  /// add a standard set of holidays
  void Calendar::standardHolidays()
  {
    // DLM@2009_07_14: do we need to "round these" to nearest Friday or Monday?  maybe call it "XXX Observed"
    // holidays with specific dates
    addHoliday(Date(MonthOfYear::Jan, 1, m_startDate.year()), "New Years Day");
    addHoliday(Date(MonthOfYear::Jul, 4, m_startDate.year()), "Independence Day");
    addHoliday(Date(MonthOfYear::Nov, 11, m_startDate.year()), "Veterans Day");
    addHoliday(Date(MonthOfYear::Dec, 25, m_startDate.year()), "Christmas");

    // holidays expressed as nth day of month
    addHoliday(Date::fromNthDayOfMonth(NthDayOfWeekInMonth::third, 
          DayOfWeek::Monday, MonthOfYear::Jan, m_startDate.year()), "MLK Day");
    addHoliday(Date::fromNthDayOfMonth(NthDayOfWeekInMonth::third, 
          DayOfWeek::Monday, MonthOfYear::Feb, m_startDate.year()), "Presidents Day");
    addHoliday(Date::fromNthDayOfMonth(NthDayOfWeekInMonth::fifth, 
          DayOfWeek::Monday, MonthOfYear::May, m_startDate.year()), "Memorial Day");
    addHoliday(Date::fromNthDayOfMonth(NthDayOfWeekInMonth::first, 
          DayOfWeek::Monday, MonthOfYear::Sep, m_startDate.year()), "Labor Day");
    addHoliday(Date::fromNthDayOfMonth(NthDayOfWeekInMonth::second, 
          DayOfWeek::Monday, MonthOfYear::Oct, m_startDate.year()), "Columbus Day");

    // Thanksgiving is kind of weird, should we include Friday afterwards (e.g. office) or not (e.g. retail)
    Date thanksgiving = Date::fromNthDayOfMonth(NthDayOfWeekInMonth::fourth, DayOfWeek::Thursday, 
        MonthOfYear::Nov, m_startDate.year());
    addHoliday(thanksgiving, "Thanksgiving");
    addHoliday(DateTime(thanksgiving, Time(1,0,0,0)).date(), "Day After Thanksgiving");
  }

  /// add a holiday
  void Calendar::addHoliday(const Date& date, const std::string& name)
  {
    if (!includesDate(date)){
      LOG(Error, "Calendar does not include date " << date);
      return;
    }

    CalendarDay day;
    day.isHoliday = true;
    day.name = name;

    m_dateMap[date] = day;
  }

  /// add a non holiday named day
  void Calendar::addNamedDay(const Date& date, const std::string& name)
  {
    if (!includesDate(date)){
      LOG(Error, "Calendar does not include date " << date);
      return;
    }

    CalendarDay day;
    day.isHoliday = false;
    day.name = name;

    m_dateMap[date] = day;
  }

  /// does the calendar include this date, e.g. are they from the same year
  bool Calendar::includesDate(const Date& date) const
  {
    return ((date >= m_startDate) && (date <= m_endDate));
  }

  /// is date within daylight savings
  bool Calendar::isDaylightSavings(const Date& date) const
  {
    if (!includesDate(date)){
      LOG(Error, "Calendar does not include date " << date);
      return false;
    }

    bool result(false);
    if (m_daylightSavingsStart && m_daylightSavingsEnd){
      result = ((date >= *m_daylightSavingsStart) && (date < *m_daylightSavingsEnd));
    }
    return result;
  }

  /// is date a holiday
  bool Calendar::isHoliday(const Date& date) const
  {
    if (!includesDate(date)){
      LOG(Error, "Calendar does not include date " << date);
      return false;
    }

    bool result = false;
    auto it = m_dateMap.find(date);
    if (it != m_dateMap.end()){
      result = it->second.isHoliday;
    }

    return result;
  }

  /// is the date on the calendar, either holiday or non holiday
  bool Calendar::isNamedDay(const Date& date) const
  {
    if (!includesDate(date)){
      LOG(Error, "Calendar does not include date " << date);
      return false;
    }

    return (m_dateMap.find(date) != m_dateMap.end());
  }

  Date Calendar::startDate() const
  {
    return m_startDate;
  }

  Date Calendar::endDate() const
  {
    return m_endDate;
  }


  /// get the name of date, return empty if not on calendar
  std::string Calendar::getName(const Date& date) const
  {
    if (!includesDate(date)){
      LOG(Error, "Calendar does not include date " << date);
      return std::string();
    }

    std::string result;
    auto it = m_dateMap.find(date);
    if (it != m_dateMap.end()){
      result = it->second.name;
    }

    return result;
  }

} // openstudio
