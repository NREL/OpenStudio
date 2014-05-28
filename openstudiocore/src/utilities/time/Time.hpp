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

#ifndef UTILITIES_TIME_TIME_HPP
#define UTILITIES_TIME_TIME_HPP

#include "../UtilitiesAPI.hpp"

#include "../core/Macro.hpp"
#include "../core/Logger.hpp"

#include <vector>

#include <boost/optional.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

namespace openstudio{

  const unsigned HOURS_PER_DAY = 24;
  const unsigned MINUTES_PER_HOUR = 60;
  const unsigned SECONDS_PER_MINUTE = 60;

  const double MINUTES_PER_SECOND = 1.0/60.0;
  const double HOURS_PER_SECOND = MINUTES_PER_SECOND/60.0;
  const double DAYS_PER_SECOND = HOURS_PER_SECOND/24.0;

  /// round towards zero, need to move this
  UTILITIES_API int floor0( double value );

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
      typedef boost::posix_time::time_duration ImplType;
      typedef std::shared_ptr<ImplType> ImplPtr;


      /// get current time of day
      static Time currentTime();

      /// default constructor
      Time();

      /// from impl 
      Time(const ImplType& implType);

      /// Time from number of days, fractional values ok
      Time(double fracDays);

      /// Time from days, hours, minutes, seconds
      Time(int days, int hours, int minutes=0, int seconds=0);

      /// Time from std tm structure
      Time(tm t_tm);

      /// constructor from string
      Time(const std::string& string);

      /// copy constructor
      Time(const Time& other);

      /// assignment operator
      Time& operator= (const Time& other);

      /// addition operator
      Time operator+ (const Time& time) const;

      /// assignment by addition operator
      Time& operator+= (const Time& time);

      /// difference operator
      Time operator- (const Time& time) const;

      /// assignment by difference operator
      Time& operator-= (const Time& time);

      /// multiplication operator
      Time operator* (double mult) const;

      /// division operator
      Time operator/ (double div) const;

      /// equality operator
      bool operator== (const Time& other) const;

      /// non-equality operator
      bool operator!= (const Time& other) const;

      /// less than operator
      bool operator< (const Time& rhs) const;

      /// less than equals operator
      bool operator<= (const Time& rhs) const;

      /// greater than operator
      bool operator> (const Time& rhs) const;

      /// greater than equals operator
      bool operator>= (const Time& rhs) const;

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

      // reference to impl
      const ImplType& impl() const;

    private:
      REGISTER_LOGGER("utilities.time.Time");

      // pointer to impl
      ImplPtr m_impl;
  };

  /// optional Time
  typedef boost::optional<Time> OptionalTime;

  /// vector of Time
  typedef std::vector<Time> TimeVector;

  /// std::ostream operator<<
  UTILITIES_API std::ostream& operator<<(std::ostream& os, const Time& time);

  /// TimeCompare compares two times, returns true if lhs is less than rhs
  class UTILITIES_API TimeCompare{
    public:
      /// operator()
      bool operator()(const Time& lhs, const Time& rhs) const;
  };

} // openstudio

#endif // UTILITIES_TIME_TIME_HPP
