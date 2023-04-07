/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2023, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

#ifndef UTILITIES_TIME_CALENDAR_HPP
#define UTILITIES_TIME_CALENDAR_HPP

#include "../UtilitiesAPI.hpp"

#include "Date.hpp"
#include "../core/Macro.hpp"
#include "../core/String.hpp"
#include "../core/Logger.hpp"

#include <map>

namespace openstudio {

/// Calendar is a description of a year, noting special days like holidays,
/// special periods like seasons, and daylight savings time
class UTILITIES_API Calendar
{
 private:
  // private class for storing information about days on the calendar
  struct CalendarDay
  {
    bool isHoliday;
    std::string name;
  };

  // map of Date to CalendarDay
  using MapType = std::map<Date, CalendarDay>;

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
using OptionalCalendar = boost::optional<Calendar>;

/// vector of Calendar
using CalendarVector = std::vector<Calendar>;

}  // namespace openstudio

#endif  //UTILITIES_TIME_CALENDAR_HPP
