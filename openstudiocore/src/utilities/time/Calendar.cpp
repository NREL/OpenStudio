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
