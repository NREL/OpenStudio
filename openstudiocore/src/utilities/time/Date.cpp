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

#include "Date.hpp"
#include "../core/Exception.hpp"

#include <sstream>

using namespace std;
using namespace boost;
using namespace boost::gregorian;

namespace openstudio{

  /// convert string to DayOfWeek
  DayOfWeek dayOfWeek(const std::string& day)
  {
    if (istringEqual("Sun", day) || istringEqual("Sunday", day)){
      return DayOfWeek(DayOfWeek::Sunday);
    }else if (istringEqual("Mon", day) || istringEqual("Monday", day)){
      return DayOfWeek(DayOfWeek::Monday);
    }else if (istringEqual("Tue", day) || istringEqual("Tuesday", day)){
      return DayOfWeek(DayOfWeek::Tuesday);
    }else if (istringEqual("Wed", day) || istringEqual("Wednesday", day)){
      return DayOfWeek(DayOfWeek::Wednesday);
    }else if (istringEqual("Thu", day) || istringEqual("Thursday", day)){
      return DayOfWeek(DayOfWeek::Thursday);
    }else if (istringEqual("Fri", day) || istringEqual("Friday", day)){
      return DayOfWeek(DayOfWeek::Friday);
    }else if (istringEqual("Sat", day) || istringEqual("Saturday", day)){
      return DayOfWeek(DayOfWeek::Saturday);
    }
    LOG_FREE_AND_THROW("Date", "'" << day << "' is not a day of the week");
  }

  /// convert string to MonthOfYear
  MonthOfYear monthOfYear(const std::string& month)
  {
    if (istringEqual("Jan", month) || istringEqual("January", month)){
      return MonthOfYear(MonthOfYear::Jan);
    }else if (istringEqual("Feb", month) || istringEqual("February", month)){
      return MonthOfYear(MonthOfYear::Feb);
    }else if (istringEqual("Mar", month) || istringEqual("March", month)){
      return MonthOfYear(MonthOfYear::Mar);
    }else if (istringEqual("Apr", month) || istringEqual("April", month)){
      return MonthOfYear(MonthOfYear::Apr);
    }else if (istringEqual("May", month) || istringEqual("May", month)){
      return MonthOfYear(MonthOfYear::May);
    }else if (istringEqual("Jun", month) || istringEqual("June", month)){
      return MonthOfYear(MonthOfYear::Jun);
    }else if (istringEqual("Jul", month) || istringEqual("July", month)){
      return MonthOfYear(MonthOfYear::Jul);
    }else if (istringEqual("Aug", month) || istringEqual("August", month)){
      return MonthOfYear(MonthOfYear::Aug);
    }else if (istringEqual("Sep", month) || istringEqual("September", month)){
      return MonthOfYear(MonthOfYear::Sep);
    }else if (istringEqual("Oct", month) || istringEqual("October", month)){
      return MonthOfYear(MonthOfYear::Oct);
    }else if (istringEqual("Nov", month) || istringEqual("November", month)){
      return MonthOfYear(MonthOfYear::Nov);
    }else if (istringEqual("Dec", month) || istringEqual("December", month)){
      return MonthOfYear(MonthOfYear::Dec);
    }

    return MonthOfYear(MonthOfYear::NotAMonth);
  }

  /// convert unsigned to MonthOfYear
  MonthOfYear monthOfYear(unsigned month)
  {
    MonthOfYear result(NotAMonth);
    if (month >= 1 && month <= 12)
    {
      return MonthOfYear(month - (1 - MonthOfYear::Jan));
    } else {
      return MonthOfYear(MonthOfYear::NotAMonth);
    }
  }

  /// convert MonthOfYear to unsigned
  unsigned month(MonthOfYear monthOfYear)
  {
    return monthOfYear.value() + (1 - MonthOfYear::Jan);
  }

  /// convert string to NthDayOfWeekInMonth
  NthDayOfWeekInMonth nthDayOfWeekInMonth(const std::string& nth)
  {
    if (istringEqual("first", nth) || istringEqual("1", nth) || istringEqual("1st", nth)){
      return NthDayOfWeekInMonth(NthDayOfWeekInMonth::first);
    }else if (istringEqual("second", nth) || istringEqual("2", nth) || istringEqual("2nd", nth)){
      return NthDayOfWeekInMonth(NthDayOfWeekInMonth::second);
    }else if (istringEqual("third", nth) || istringEqual("3", nth) || istringEqual("3rd", nth)){
      return NthDayOfWeekInMonth(NthDayOfWeekInMonth::third);
    }else if (istringEqual("fourth", nth) || istringEqual("4", nth) || istringEqual("4th", nth)){
      return NthDayOfWeekInMonth(NthDayOfWeekInMonth::fourth);
    }else if (istringEqual("fifth", nth) || istringEqual("5", nth) || istringEqual("5th", nth)){
      return NthDayOfWeekInMonth(NthDayOfWeekInMonth::fifth);
    }else if (istringEqual("last", nth)){
      return NthDayOfWeekInMonth(NthDayOfWeekInMonth::fifth);
    }
    LOG_FREE_AND_THROW("Date", "'" << nth << "' is not a valid input for nthDayOfWeekInMonth");
  }

  /// constructor
  YearDescription::YearDescription()
    : isLeapYear(false), lookInFuture(false)
  {}

  /// get the assumed year
  int YearDescription::assumedYear() const
  {
    // start out assuming 2009
    int result(2009);

    // if the desired start day is set
    if(yearStartsOnDayOfWeek){

      // start incrementing or decrementing assumedYear until we match *yearStartsOn and assumedLeapYear
      while( (Date::isLeapYear(result) != isLeapYear) ||
          (Date(Jan, 1, result).dayOfWeek() != *yearStartsOnDayOfWeek) ){
        if (lookInFuture){
          ++result;
        }else{
          --result;
        }
      }

    }else if(isLeapYear){

      // if we just want a leap year
      if(lookInFuture){
        result = 2012;
      }else{
        result = 2008;
      }
    }

    return result;
  }

  /// nth weekday of month generators
  Date Date::fromNthDayOfMonth(NthDayOfWeekInMonth n, DayOfWeek dayOfWeek, MonthOfYear monthOfYear)
  {
    return fromNthDayOfMonth(n, dayOfWeek, monthOfYear, YearDescription().assumedYear());
  }

  Date Date::fromNthDayOfMonth(NthDayOfWeekInMonth n, DayOfWeek dayOfWeek, MonthOfYear monthOfYear, int year)
  {
    nth_day_of_the_week_in_month ndm(static_cast<nth_day_of_the_week_in_month::week_num>(n.value()), dayOfWeek.value(), monthOfYear.value());
    date d = ndm.get_date(year);
    return Date(d.month().as_enum(), d.day(), d.year());
  }

  /// Day of year generators
  Date Date::fromDayOfYear(unsigned dayOfYear)
  {
    Date result;
    result.m_impl.reset();
    result.m_assumedBaseYear = YearDescription().assumedYear();
    result.initFromYearDayOfYear(result.m_assumedBaseYear, dayOfYear);
    return result;
  }

  Date Date::fromDayOfYear(unsigned dayOfYear, int year)
  {
    Date result;
    result.m_impl.reset();
    result.m_baseYear = year;
    result.m_assumedBaseYear = year;
    result.initFromYearDayOfYear(year, dayOfYear);
    return result;
  }

  Date Date::fromDayOfYear(unsigned dayOfYear, const YearDescription& yearDescription)
  {
    Date result;
    result.m_impl.reset();
    result.m_assumedBaseYear = yearDescription.assumedYear();
    result.initFromYearDayOfYear(result.m_assumedBaseYear, dayOfYear);
    return result;
  }

  /// is year a leap year
  bool Date::isLeapYear(int year)
  {
    return gregorian_calendar::is_leap_year(year);
  }

  /// get current Date
  Date Date::currentDate()
  {
    posix_time::ptime now = posix_time::second_clock::local_time();
    return Date(now.date());
  }

  /// default constructor
  Date::Date()
    : m_assumedBaseYear(YearDescription().assumedYear())
  {
    initFromYearDayOfYear(m_assumedBaseYear, 1);
  }

  /// from impl
  Date::Date(const Date::ImplType& impl)
    : m_assumedBaseYear(impl.year())
  {
    m_impl = ImplPtr(new ImplType(impl));
  }

  /// Date from month, day of month
  Date::Date(MonthOfYear monthOfYear, unsigned dayOfMonth)
    : m_assumedBaseYear(YearDescription().assumedYear())
  {
    initFromYearMonthDay(m_assumedBaseYear, monthOfYear, dayOfMonth);
  }

  /// Date from month, day, year
  Date::Date(MonthOfYear monthOfYear, unsigned dayOfMonth, int year)
    : m_baseYear(year), m_assumedBaseYear(year)
  {
    initFromYearMonthDay(year, monthOfYear, dayOfMonth);
  }

  /// Date from month, day of month, and YearDescription
  Date::Date(MonthOfYear monthOfYear, unsigned dayOfMonth, const YearDescription& yearDescription)
    : m_assumedBaseYear(yearDescription.assumedYear())
  {
    initFromYearMonthDay(m_assumedBaseYear, monthOfYear, dayOfMonth);
  }

  Date::Date(const std::string& string)
  {
    std::stringstream ss;
    ss << string;
    boost::gregorian::date date;
    ss >> date;

    m_impl = std::shared_ptr<boost::gregorian::date>(new boost::gregorian::date(date));
  }

  /// from tm
  Date::Date(tm t_tm)
    : m_impl(new ImplType(boost::gregorian::date_from_tm(t_tm))),
      m_baseYear(m_impl->year()),
      m_assumedBaseYear(m_impl->year())
  {
  }

  /// copy constructor
  Date::Date(const Date& other)
    : m_baseYear(other.baseYear()), m_assumedBaseYear(other.assumedBaseYear())
  {
    initFromYearMonthDay(other.year(), other.monthOfYear(), other.dayOfMonth());
  }

  Date &Date::operator=(const Date &other)
  {
    m_baseYear = other.m_baseYear;
    m_assumedBaseYear = other.m_assumedBaseYear;
    m_impl = std::shared_ptr<ImplType>(new ImplType(*other.m_impl));
    return *this;
  }

  /// assignment by addition operator
  Date Date::operator+ (const Time& time) const
  {
    Date result(*this);
    result += time;
    return result;
  }

  /// assignment by addition operator
  Date& Date::operator+= (const Time& time)
  {
    (*m_impl) += date_duration(time.days());
    return *this;
  }

  /// assignment by difference operator
  Date Date::operator- (const Time& time) const
  {
    Date result(*this);
    result -= time;
    return result;
  }

  /// assignment by difference operator
  Date& Date::operator-= (const Time& time)
  {
    (*m_impl) -= date_duration(time.days());
    return *this;
  }

  /// time duration
  Time Date::operator- (const Date& date) const
  {
    boost::gregorian::date_duration duration( (*m_impl)-(*date.m_impl) );
    return Time(duration.days());
  }

  /// equality operator
  bool Date::operator== (const Date& other) const
  {
    bool result = ( (year() == other.year()) &&
          (monthOfYear() == other.monthOfYear()) &&
          (dayOfMonth() == other.dayOfMonth()) ); 

    return result;
  }

  /// non-equality operator
  bool Date::operator!= (const Date& other) const
  {
    return !(*this == other);
  }

  /// less than operator
  bool Date::operator< (const Date& rhs) const
  {
    if( (!baseYear() || !rhs.baseYear()) && 
        (assumedBaseYear() != rhs.assumedBaseYear()) ){
      LOG(Error, "Comparing Dates with improper base years");
    }

    bool result = false;
    if (year() < rhs.year()){
      result = true;
    }else if (year() == rhs.year()){
      if (monthOfYear() < rhs.monthOfYear()){
        result = true;
      }else if (monthOfYear() == rhs.monthOfYear()){
        if (dayOfMonth() < rhs.dayOfMonth()){
          result = true;
        }
      }
    }
    return result;
  }

  /// less than equals operator
  bool Date::operator<= (const Date& rhs) const
  {
    return ((*this < rhs) || (*this == rhs));
  }

  /// greater than operator
  bool Date::operator> (const Date& rhs) const
  {
    return !(*this <= rhs);
  }

  /// greater than equals operator
  bool Date::operator>= (const Date& rhs) const
  {
    return !(*this < rhs);
  }

  /// base year
  optional<int> Date::baseYear() const
  {
    return m_baseYear;
  }

  /// set  base year
  void Date::baseYear(int baseYear)
  {
    m_baseYear = baseYear;
    m_assumedBaseYear = baseYear;
  }

  /// assumed base year
  int Date::assumedBaseYear() const
  {
    return m_assumedBaseYear;
  }

  int Date::year() const
  {
    return m_impl->year();
  }

  /// month of year
  MonthOfYear Date::monthOfYear() const
  {
    return m_impl->month().as_enum();
  }

  /// day of month
  unsigned Date::dayOfMonth() const
  {
    return m_impl->day();
  };

  /// day of year
  unsigned Date::dayOfYear() const
  {
    return m_impl->day_of_year();
  };

  /// is year a leap year
  bool Date::isLeapYear() const
  {
    return isLeapYear(year());
  };

  /// day of the week
  DayOfWeek Date::dayOfWeek() const
  {
    return DayOfWeek(m_impl->day_of_week());
  };

  // initFromYearMonthDay
  void Date::initFromYearMonthDay(int year, MonthOfYear monthOfYear, unsigned dayOfMonth)
  {
    try{
      // construct with year, month, day
      m_impl = ImplPtr(new ImplType(year, monthOfYear.value(), dayOfMonth));
    }catch(...){
    }

    if (!m_impl || m_impl->is_not_a_date()){
      LOG_AND_THROW("Bad Date: year = " << year << ", month = " << monthOfYear << 
          ", day = " << dayOfMonth << ". ");
    }
  }

  // initFromYearDayOfYear
  void Date::initFromYearDayOfYear(int year, unsigned dayOfYear)
  {
    try{
      // get days in this year, can throw bad_year
      unsigned daysInYear;
      if (gregorian_calendar::is_leap_year(year)){
        daysInYear = 366;
      }else{
        daysInYear = 365;
      };

      // we override wrapped class and only allow dayOfYear >= 1 and <= daysInYear
      if ((dayOfYear >= 1) && (dayOfYear <= daysInYear)){

        // construct at Jan, 1 of year
        m_impl = ImplPtr(new ImplType(year, 1, 1));

        // add day of year minus one to impl
        (*m_impl) += date_duration(dayOfYear-1);
      };

    }catch(...){
    }

    if (!m_impl || m_impl->is_not_a_date() ){
      LOG_AND_THROW("Bad Date: year = " << year << ", dayOfYear = " << dayOfYear <<  ". ");
    }
  }

  // reference to impl
  const Date::ImplType& Date::impl() const
  {
    return *m_impl;
  }

  // std::ostream operator<<
  std::ostream& operator<<(std::ostream& os, const Date& date)
  {
    os << date.impl();
    return os;
  };

  // compare two Dates
  bool DateCompare::operator()(const Date& lhs, const Date& rhs) const
  {
    return lhs < rhs;
  };

  NthDayOfWeekInMonth nthDayOfWeekInMonth(const Date& date)
  {
    int result = 0;

    for(unsigned dayOfMonth = 1; dayOfMonth <= date.dayOfMonth(); ++dayOfMonth)
    {
      if( Date(date.monthOfYear(), dayOfMonth, date.year()).dayOfWeek() == date.dayOfWeek() )
      {
        ++result;
      }
    }

    if( (result < 1) || (result > 5) )
    {
      LOG_FREE_AND_THROW("Date","Nth day of week in month not found");
    }

    return NthDayOfWeekInMonth(result);
  }

} // openstudio
