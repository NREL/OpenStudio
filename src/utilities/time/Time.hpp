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

#ifndef UTILITIES_TIME_TIME_HPP
#define UTILITIES_TIME_TIME_HPP

#include "../UtilitiesAPI.hpp"

#include "../core/Macro.hpp"
#include "../core/Logger.hpp"

#include <vector>

#include <boost/optional.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

namespace openstudio {

constexpr unsigned HOURS_PER_DAY = 24;
constexpr unsigned MINUTES_PER_HOUR = 60;
constexpr unsigned SECONDS_PER_MINUTE = 60;

constexpr double MINUTES_PER_SECOND = 1.0 / 60.0;
constexpr double HOURS_PER_SECOND = MINUTES_PER_SECOND / 60.0;
constexpr double DAYS_PER_SECOND = HOURS_PER_SECOND / 24.0;

/// round towards zero, need to move this
UTILITIES_API int floor0(double value);

class DateTime;

/// Time is a relative unit of time, resolution to the second
/// Time is simple wrapper around boost::posix_time::time_duration
/// Internally totalSeconds is the primary definition of time
/// (i.e. it does not matter how time is divided into hours, minutes, seconds)
class UTILITIES_API Time
{
 public:
  friend UTILITIES_API std::ostream& operator<<(std::ostream& os, const Time& time);
  friend UTILITIES_API std::ostream& operator<<(std::ostream& os, const DateTime& dateTime);
  friend class Date;
  friend class DateTime;

  /// impl type is boost::posix_time::time_duration
  using ImplType = boost::posix_time::time_duration;
  //typedef std::shared_ptr<ImplType> ImplPtr;

  /// get current local time of day
  static Time currentTime();

  /// get the current UTC time of day
  static Time currentTimeUTC();

  /// default constructor
  Time();

  /// from impl
  Time(const ImplType& implType);

  /// Time from number of days, fractional values ok
  Time(double fracDays);

  /// Time from days, hours, minutes, seconds
  Time(int days, int hours, int minutes = 0, int seconds = 0);

  /// Time from std tm structure
  Time(tm t_tm);

  /// constructor from string
  Time(const std::string& string);

  /// copy constructor
  Time(const Time& other);

  /// assignment operator
  Time& operator=(const Time& other);

  /// addition operator
  Time operator+(const Time& time) const;

  /// assignment by addition operator
  Time& operator+=(const Time& time);

  /// difference operator
  Time operator-(const Time& time) const;

  /// assignment by difference operator
  Time& operator-=(const Time& time);

  /// multiplication operator
  Time operator*(double mult) const;

  /// division operator
  Time operator/(double div) const;

  /// equality operator
  bool operator==(const Time& other) const;

  /// non-equality operator
  bool operator!=(const Time& other) const;

  /// less than operator
  bool operator<(const Time& rhs) const;

  /// less than equals operator
  bool operator<=(const Time& rhs) const;

  /// greater than operator
  bool operator>(const Time& rhs) const;

  /// greater than equals operator
  bool operator>=(const Time& rhs) const;

  /// whole number of days
  int days() const;

  /// whole number of hours remaining after days
  int hours() const;

  /// whole number of minutes remaining after hours
  int minutes() const;

  /// whole number of seconds remaining after minutes
  int seconds() const;

  /// entire time in days
  double totalDays() const;

  /// entire time in hours
  double totalHours() const;

  /// entire time in minutes
  double totalMinutes() const;

  /// entire time in seconds
  int totalSeconds() const;

  /// Convert to a string
  std::string toString() const;

 protected:
  const ImplType impl() const;

 private:
  REGISTER_LOGGER("utilities.time.Time");

  ImplType m_impl;
};

/// optional Time
using OptionalTime = boost::optional<Time>;

/// vector of Time
using TimeVector = std::vector<Time>;

/// std::ostream operator<<
UTILITIES_API std::ostream& operator<<(std::ostream& os, const Time& time);

/// TimeCompare compares two times, returns true if lhs is less than rhs
class UTILITIES_API TimeCompare
{
 public:
  /// operator()
  bool operator()(const Time& lhs, const Time& rhs) const;
};

}  // namespace openstudio

#endif  // UTILITIES_TIME_TIME_HPP
