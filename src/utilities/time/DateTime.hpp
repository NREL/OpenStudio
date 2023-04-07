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

#ifndef UTILITIES_TIME_DATETIME_HPP
#define UTILITIES_TIME_DATETIME_HPP

#include "../UtilitiesAPI.hpp"

#include "Date.hpp"
#include "Time.hpp"
#include "../core/Macro.hpp"
#include "../core/Logger.hpp"

#include <vector>

#include <boost/date_time/gregorian/greg_date.hpp>

#include <ctime>

namespace openstudio {

/// DateTime is an absolute unit of time, resolution to the second
/// date is a valid Date
/// time is normalized to 0 <= time < 24 hrs
class UTILITIES_API DateTime
{
 public:
  REGISTER_LOGGER("utilities.time.DateTime");

  /// get the local time now
  static DateTime now();

  /// get the UTC time now
  static DateTime nowUTC();

  /// get the current UTC offset
  static double localOffsetUTC();

  /// default constructor, UTC time is assumed
  DateTime();

  /// constructor from Date, UTC time is assumed
  DateTime(const Date& date);

  /// constructor from Date and Time, UTC time is assumed
  DateTime(const Date& date, const Time& timeFromDate);

  /// constructor from Date and Time and UTC Offset
  DateTime(const Date& date, const Time& timeFromDate, double utcOffset);

  /// copy constructor
  DateTime(const DateTime& other);

  /// constructor from simple string format, UTC time is assumed
  DateTime(const std::string& string);

  /// from system tm struct
  DateTime(tm t_tm);

  /// assignment operator
  DateTime& operator=(const DateTime& other);

  /// addition operator
  DateTime operator+(const Time& time) const;

  /// assignment by addition operator
  DateTime& operator+=(const Time& time);

  /// difference operator
  DateTime operator-(const Time& time) const;

  /// assignment by difference operator
  DateTime& operator-=(const Time& time);

  /// time duration
  Time operator-(const DateTime& dateTime) const;

  /// time duration
  Time operator-(const Date& date) const;

  /// equality operator
  bool operator==(const DateTime& other) const;

  /// non-equality operator
  bool operator!=(const DateTime& other) const;

  /// less than operator
  bool operator<(const DateTime& rhs) const;

  /// less than equals operator
  bool operator<=(const DateTime& rhs) const;

  /// greater than operator
  bool operator>(const DateTime& rhs) const;

  /// greater than equals operator
  bool operator>=(const DateTime& rhs) const;

  /// getter to date member
  Date date() const;

  /// getter to time member
  Time time() const;

  /// UTC offset in hours, local time = UTC time + UTC offset
  double utcOffset() const;

  /// convert to simple string format in UTC
  std::string toString() const;

  /// convert to ISO 8601 string
  std::string toISO8601() const;

  /// convert to xsd:dateTime format
  std::string toXsdDateTime() const;

  /// convert to epoch time_t in UTC time
  time_t toEpoch() const;

  /// construct from ISO 8601 string
  static boost::optional<DateTime> fromISO8601(const std::string& str);

  /// construct from xsd:dateTime string
  static boost::optional<DateTime> fromXsdDateTime(const std::string& str);

  /// construct from epoch time_t in UTC
  static DateTime fromEpoch(const std::time_t& time);

 private:
  // ensure that stored time is less than 24 hrs and adjust date accordingly
  void normalize();

  int utcOffsetHours() const;
  int utcOffsetMinutes() const;

  Date m_date;
  Time m_time;

  // utc offset in hours, some places have half hour offsets
  double m_utcOffset;
};

/// optional DateTime
using OptionalDateTime = boost::optional<DateTime>;

/// vector of DateTime
using DateTimeVector = std::vector<DateTime>;

// std::ostream operator<<
UTILITIES_API std::ostream& operator<<(std::ostream& os, const DateTime& dateTime);

}  // namespace openstudio

#endif  // UTILITIES_TIME_DATETIME_HPP
