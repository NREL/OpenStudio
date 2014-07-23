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

class QDateTime;

namespace openstudio{

/// DateTime is an absolute unit of time, resolution to the second
/// date is a valid Date
/// time is normalized to 0 <= time < 24 hrs
class UTILITIES_API DateTime {
 public:

  REGISTER_LOGGER("utilities.time.DateTime");

  /// get the local time now
  static DateTime now();

  /// default constructor
  DateTime();

  /// constructor from Date
  DateTime(const Date& date);

  /// constructor from Date and Time
  DateTime(const Date& date, const Time& timeFromDate);

  /// copy constructor
  DateTime(const DateTime& other);

  /// constructor from string
  DateTime(const std::string& string);

  /// from system tm struct
  DateTime(tm t_tm);

  /// assignment operator
  DateTime& operator= (const DateTime& other);

  /// addition operator
  DateTime operator+ (const Time& time) const;

  /// assignment by addition operator
  DateTime& operator+= (const Time& time);

  /// difference operator
  DateTime operator- (const Time& time) const;

  /// assignment by difference operator
  DateTime& operator-= (const Time& time);

  /// time duration
  Time operator- (const DateTime& dateTime) const;

  /// time duration
  Time operator- (const Date& date) const;

  /// equality operator
  bool operator== (const DateTime& other) const;

  /// non-equality operator
  bool operator!= (const DateTime& other) const;

  /// less than operator
  bool operator< (const DateTime& rhs) const;

  /// less than equals operator
  bool operator<= (const DateTime& rhs) const;

  /// greater than operator
  bool operator> (const DateTime& rhs) const;

  /// greater than equals operator
  bool operator>= (const DateTime& rhs) const;

  /// getter to date member
  const Date& date() const;

  /// getter to time member
  const Time& time() const;

  /// convert to string
  std::string toString() const;

  /// convert to ISO 8601 string
  std::string toISO8601() const;

  /// convert to epoch time_t
  time_t toEpoch() const;
  
  /// construct from ISO 8601 string
  static boost::optional<DateTime> fromISO8601(const std::string& str);
    
  /// construct from epoch time_t
  static DateTime fromEpoch(const std::time_t& time);

 private:

  // ensure that stored time is less than 24 hrs and adjust date accordingly
  void normalize();

  Date m_date;
  Time m_time;
};

/// optional DateTime
typedef boost::optional<DateTime> OptionalDateTime;

/// vector of DateTime
typedef std::vector<DateTime> DateTimeVector;

// std::ostream operator<<
UTILITIES_API std::ostream& operator<<(std::ostream& os, const DateTime& dateTime);

// conversion from QDateTime
UTILITIES_API DateTime toDateTime(const QDateTime &qdt);

// conversion to QDateTime
UTILITIES_API QDateTime toQDateTime(const DateTime& dt);

} // openstudio

namespace openstudio {
namespace detail {

  // register meta datatypes
  struct DateTimeMetaTypeInitializer{
    DateTimeMetaTypeInitializer();
  };
}
}


#endif // UTILITIES_TIME_DATETIME_HPP
