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

#ifndef UTILITIES_TIME_CALENDAR_HPP
#define UTILITIES_TIME_CALENDAR_HPP

#include "../UtilitiesAPI.hpp"

#include "Date.hpp"
#include "../core/Macro.hpp"
#include "../core/String.hpp"
#include "../core/Logger.hpp"

#include <map>

namespace openstudio{

  /// Calendar is a description of a year, noting special days like holidays,
  /// special periods like seasons, and daylight savings time
  class UTILITIES_API Calendar
  {
  private:

    // private class for storing information about days on the calendar
    struct CalendarDay{
      bool isHoliday;
      std::string name;
    };

    // map of Date to CalendarDay
    typedef std::map<Date, CalendarDay> MapType;

  public:

    REGISTER_LOGGER("utilities.time.Calendar");
    /// default constructor, uses full assumed year
    Calendar();

    /// constructor with year, uses full assumed year
    Calendar(int year);

    /// constructor with start and end dates, [startDate, endDate]
    Calendar(const Date& startDate, const Date& endDate);

    /// standard daylight savings time, [2nd Sunday in March, 1st Sunday in November)
    void standardDaylightSavings();

    /// add a standard set of holidays
    void standardHolidays();

    /// set daylight savings time, [daylightSavingsStart, daylightSavingsEnd)
    void daylightSavings(const Date& daylightSavingsStart, const Date& daylightSavingsEnd);

    /// add a holiday
    void addHoliday(const Date& date, const std::string& name);

    /// add a non holiday named day
    void addNamedDay(const Date& date, const std::string& name);

    /// does the calendar include this date, e.g. are they from the same year
    bool includesDate(const Date& date) const;

    /// is date within daylight savings
    bool isDaylightSavings(const Date& date) const;

    /// is date a holiday
    bool isHoliday(const Date& date) const;

    /// is the date named on the calendar, either holiday or non holiday
    bool isNamedDay(const Date& date) const;

    /// get the name of date, return empty if not on calendar
    std::string getName(const Date& date) const;

    /// get the first date in the calendar
    Date startDate() const;


    /// get the last date in the calendar
    Date endDate() const;

    
  private:

    Date m_startDate;
    Date m_endDate;
    MapType m_dateMap;
    OptionalDate m_daylightSavingsStart;
    OptionalDate m_daylightSavingsEnd;

  };

  /// optional Calendar
  typedef boost::optional<Calendar> OptionalCalendar;

  /// vector of Calendar
  typedef std::vector<Calendar> CalendarVector;

} // openstudio


#endif //UTILITIES_TIME_CALENDAR_HPP
