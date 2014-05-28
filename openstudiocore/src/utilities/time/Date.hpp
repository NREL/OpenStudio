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

#ifndef UTILITIES_TIME_DATE_HPP
#define UTILITIES_TIME_DATE_HPP

#include "../UtilitiesAPI.hpp"

#include "Time.hpp"
#include "../core/Macro.hpp"
#include "../core/Logger.hpp"

#include <ostream>
#include <vector>

#include <boost/optional.hpp>
#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/date_time/date_defs.hpp>
#include <boost/date_time/gregorian/greg_calendar.hpp>
#include "../core/Enum.hpp"

namespace openstudio{

  /** \class DayOfWeek
   *  \brief DayOfWeek is an enumeration of the days of the week: Sunday, Monday, Tuesday,
   *  Wednesday, Thursday, Friday, Saturday.
   *  \details See the OPENSTUDIO_ENUM documentation in utilities/core/Enum.hpp. The actual
   *  macro call is:
   *  \code
  OPENSTUDIO_ENUM(DayOfWeek,
      ((Sunday)(Sunday)(boost::date_time::Sunday))
      ((Monday)(Monday)(boost::date_time::Monday))
      ((Tuesday)(Tuesday)(boost::date_time::Tuesday))
      ((Wednesday)(Wednesday)(boost::date_time::Wednesday))
      ((Thursday)(Thursday)(boost::date_time::Thursday))
      ((Friday)(Friday)(boost::date_time::Friday))
      ((Saturday)(Saturday)(boost::date_time::Saturday))
  );
   *  \endcode */
  OPENSTUDIO_ENUM(DayOfWeek,
      ((Sunday)(Sunday)(boost::date_time::Sunday))
      ((Monday)(Monday)(boost::date_time::Monday))
      ((Tuesday)(Tuesday)(boost::date_time::Tuesday))
      ((Wednesday)(Wednesday)(boost::date_time::Wednesday))
      ((Thursday)(Thursday)(boost::date_time::Thursday))
      ((Friday)(Friday)(boost::date_time::Friday))
      ((Saturday)(Saturday)(boost::date_time::Saturday))
  );

  /// Convert string to DayOfWeek. Accepts full string or first three letters.
  UTILITIES_API DayOfWeek dayOfWeek(const std::string& day);

  /** \class MonthOfYear
   *  \brief MonthOfYear is an enumeration of months: January, February, March,
   *  April, May, June, July, August, September, October, November, December.
   *  \details See the OPENSTUDIO_ENUM documentation in utilities/core/Enum.hpp. The actual
   *  macro call is:
   *  \code
  OPENSTUDIO_ENUM(MonthOfYear,
      ((Jan)(January)(boost::date_time::Jan))
      ((Feb)(February)(boost::date_time::Feb))
      ((Mar)(March)(boost::date_time::Mar))
      ((Apr)(April)(boost::date_time::Apr))
      ((May)(May)(boost::date_time::May))
      ((Jun)(June)(boost::date_time::Jun))
      ((Jul)(July)(boost::date_time::Jul))
      ((Aug)(August)(boost::date_time::Aug))
      ((Sep)(September)(boost::date_time::Sep))
      ((Oct)(October)(boost::date_time::Oct))
      ((Nov)(November)(boost::date_time::Nov))
      ((Dec)(December)(boost::date_time::Dec))
      ((NotAMonth)(Not a month)(boost::date_time::NotAMonth))
      ((NumMonths)(Number of months)(boost::date_time::NumMonths))
  );
   *  \endcode */
  OPENSTUDIO_ENUM(MonthOfYear,
      ((Jan)(January)(boost::date_time::Jan))
      ((Feb)(February)(boost::date_time::Feb))
      ((Mar)(March)(boost::date_time::Mar))
      ((Apr)(April)(boost::date_time::Apr))
      ((May)(May)(boost::date_time::May))
      ((Jun)(June)(boost::date_time::Jun))
      ((Jul)(July)(boost::date_time::Jul))
      ((Aug)(August)(boost::date_time::Aug))
      ((Sep)(September)(boost::date_time::Sep))
      ((Oct)(October)(boost::date_time::Oct))
      ((Nov)(November)(boost::date_time::Nov))
      ((Dec)(December)(boost::date_time::Dec))
      ((NotAMonth)(Not a month)(boost::date_time::NotAMonth))
      ((NumMonths)(Number of months)(boost::date_time::NumMonths))
  );
   
  /// Converts string to MonthOfYear. Accepts full string or first three letters.
  UTILITIES_API MonthOfYear monthOfYear(const std::string& month);

  /// convert unsigned to MonthOfYear
  UTILITIES_API MonthOfYear monthOfYear(unsigned month);

  /// convert MonthOfYear to unsigned 
  UTILITIES_API unsigned month(MonthOfYear monthOfYear);

  /** \class NthDayOfWeekInMonth 
   *  \brief NthDayOfWeekInMonth is an enum for the nth DayOfWeek in MonthOfYear
   *  (e.g. first Saturday in January).
   *  \details The fifth NthDayOfWeekInMonth is equivalent to the last DayOfWeek in
   *  MonthOfYear. See the OPENSTUDIO_ENUM documentation in utilities/core/Enum.hpp
   *  for documentation of the class structure. The actual macro call is:
   *  \code
  OPENSTUDIO_ENUM( NthDayOfWeekInMonth,
      ((first)(first)(boost::gregorian::nth_day_of_the_week_in_month::first))
      ((second)(second)(boost::gregorian::nth_day_of_the_week_in_month::second))
      ((third)(third)(boost::gregorian::nth_day_of_the_week_in_month::third))
      ((fourth)(fourth)(boost::gregorian::nth_day_of_the_week_in_month::fourth))
      ((fifth)(fifth)(boost::gregorian::nth_day_of_the_week_in_month::fifth))
  );
   *  \endcode */
  OPENSTUDIO_ENUM( NthDayOfWeekInMonth,
      ((first)(first)(boost::gregorian::nth_day_of_the_week_in_month::first))
      ((second)(second)(boost::gregorian::nth_day_of_the_week_in_month::second))
      ((third)(third)(boost::gregorian::nth_day_of_the_week_in_month::third))
      ((fourth)(fourth)(boost::gregorian::nth_day_of_the_week_in_month::fourth))
      ((fifth)(fifth)(boost::gregorian::nth_day_of_the_week_in_month::fifth))
  );

  /// Convert string to NthDayOfWeekInMonth. Accepts full string, also accepts 1, 1st, 2, 2nd, etc.
  UTILITIES_API NthDayOfWeekInMonth nthDayOfWeekInMonth(const std::string& nth);

  /// YearDescription describes a year with several optional parameters
  struct UTILITIES_API YearDescription{

    /// constructor
    YearDescription();

    /// get the assumed year
    int assumedYear() const;

    /// is the year a leap year, defaults to false
    bool isLeapYear;

    /// first day of the year, defaults to not set
    boost::optional<DayOfWeek> yearStartsOnDayOfWeek;

    /// look for year in future, defaults to false
    bool lookInFuture;
  };

  class DateTime;

  /// Date is an absolute unit of time, resolution to the day
  /// Date is simple wrapper around boost::gregorian::date
  class UTILITIES_API Date
  {
  public:

    friend UTILITIES_API std::ostream& operator<<(std::ostream& os, const Date& date);
    friend UTILITIES_API std::ostream& operator<<(std::ostream& os, const DateTime& dateTime);
    friend class DateTime;

    /// impl type is boost::gregorian::date
    typedef boost::gregorian::date ImplType;
    typedef std::shared_ptr<ImplType> ImplPtr;

    REGISTER_LOGGER("utilities.time.Date");

    /// nth weekday of month generators
    static Date fromNthDayOfMonth(NthDayOfWeekInMonth n, DayOfWeek dayOfWeek, MonthOfYear monthOfYear);
    static Date fromNthDayOfMonth(NthDayOfWeekInMonth n, DayOfWeek dayOfWeek, MonthOfYear monthOfYear, int year);

    /// Day of year generators
    static Date fromDayOfYear(unsigned dayOfYear);
    static Date fromDayOfYear(unsigned dayOfYear, int year);
    static Date fromDayOfYear(unsigned dayOfYear, const YearDescription& yearDescription);

    /// is year a leap year
    static bool isLeapYear(int year);

    /// get current Date
    static Date currentDate();

    /// default constructor
    Date();

    /// destructor
    ~Date(){}

    /// from impl
    Date(const ImplType& impl);

    /// Date from month, day of month
    Date(MonthOfYear monthOfYear, unsigned dayOfMonth);

    /// Date from month, day of month, year
    Date(MonthOfYear monthOfYear, unsigned dayOfMonth, int year);

    /// Date from YearDescription, month, and day of month
    Date(MonthOfYear monthOfYear, unsigned dayOfMonth, const YearDescription& yearDescription);

    /// constructor from string
    Date(const std::string& string);

    /// Date from a system tm structure
    Date(tm t_tm);

    /// copy constructor
    Date(const Date& other);

    /// addition operator
    Date operator+ (const Time& time) const;

    /// assignment by addition operator
    Date& operator+= (const Time& time);

    /// difference operator
    Date operator- (const Time& time) const;

    /// assignment by difference operator
    Date& operator-= (const Time& time);

    /// time duration
    Time operator- (const Date& date) const;

    /// equality operator
    bool operator== (const Date& other) const;

    /// non-equality operator
    bool operator!= (const Date& other) const;

    /// less than operator
    bool operator< (const Date& rhs) const;

    /// less than equals operator
    bool operator<= (const Date& rhs) const;

    /// greater than operator
    bool operator> (const Date& rhs) const;

    /// greater than equals operator
    bool operator>= (const Date& rhs) const;

    /// assignment operator
    Date &operator=(const Date &other);

    /// user provided base year
    boost::optional<int> baseYear() const;

    /// set base year
    void baseYear(int baseYear);

    /// assumed base year
    int assumedBaseYear() const;

    /// year
    int year() const;

    /// month of year
    MonthOfYear monthOfYear() const;

    /// day of month
    unsigned dayOfMonth() const;

    /// day of year
    unsigned dayOfYear() const;

    /// is assumed year a leap year
    bool isLeapYear() const;

    /// day of the week
    DayOfWeek dayOfWeek() const;

  protected:

    // reference to impl
    const ImplType& impl() const;

  private:

    // initFromYearMonthDay
    void initFromYearMonthDay(int year, MonthOfYear monthOfYear, unsigned dayOfMonth);

    // initFromYearDayOfYear
    void initFromYearDayOfYear(int year, unsigned dayOfYear);

    // pointer to impl
    ImplPtr m_impl;

    // user provided base year is optional
    boost::optional<int> m_baseYear;

    // always have assumed base year
    int m_assumedBaseYear;
  };

  /// optional Date
  typedef boost::optional<Date> OptionalDate;

  /// vector of Date
  typedef std::vector<Date> DateVector;

  // std::ostream operator<<
  UTILITIES_API std::ostream& operator<<(std::ostream& os, const Date& date);

  /// DateCompare compares two dates, returns true if lhs is less than rhs
  class UTILITIES_API DateCompare{
  public:
    /// operator()
    bool operator()(const Date& lhs, const Date& rhs) const;
  };

  /// return nthDayOfWeekInMonth for given date
  UTILITIES_API NthDayOfWeekInMonth nthDayOfWeekInMonth(const Date& date);

} // openstudio


#endif // UTILITIES_TIME_DATE_HPP
