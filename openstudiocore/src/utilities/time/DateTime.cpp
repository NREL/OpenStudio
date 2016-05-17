/**********************************************************************
*  Copyright (c) 2008-2016, Alliance for Sustainable Energy.  
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

#include "DateTime.hpp"
#include <QDateTime>
#include <cmath>

using namespace std;
using namespace boost;

namespace openstudio{

/// get the local time now
DateTime DateTime::now()
{
  posix_time::ptime now = posix_time::second_clock::local_time();
  return DateTime(Date(now.date()), Time(now.time_of_day()), DateTime::localOffsetUTC());
}

/// get the utc time now
DateTime DateTime::nowUTC()
{
  posix_time::ptime now = posix_time::second_clock::universal_time();
  return DateTime(Date(now.date()), Time(now.time_of_day()), 0.0);
}

/// get the current UTC offset
double DateTime::localOffsetUTC()
{
  boost::posix_time::time_duration offset = posix_time::second_clock::local_time() - posix_time::second_clock::universal_time();
  return HOURS_PER_SECOND*offset.total_seconds();
}

/// default constructor
DateTime::DateTime()
  : m_utcOffset(0.0)
{
}

/// constructor from Date
DateTime::DateTime(const Date& date)
  : m_date(date), m_time(Time(0, 0, 0, 0)), m_utcOffset(0.0)
{
  // should not need to normalize
}

/// constructor from Date and Time
DateTime::DateTime(const Date& date, const Time& timeFromDate)
  : m_date(date), m_time(timeFromDate), m_utcOffset(0.0)
{
  normalize();
}

DateTime::DateTime(const Date& date, const Time& timeFromDate, double utcOffset)
  : m_date(date), m_time(timeFromDate), m_utcOffset(utcOffset)
{
  normalize();
}

/// copy constructor
DateTime::DateTime(const DateTime& other)
  : m_date(other.date()), m_time(other.time()), m_utcOffset(other.utcOffset())
{
  // should not need to normalize
}

DateTime::DateTime(const std::string& string)
  : m_date(), m_time()
{
  boost::posix_time::ptime pt(boost::posix_time::time_from_string(string));
  m_date = Date(pt.date());
  m_time = Time(pt.time_of_day());
  m_utcOffset = 0.0;
}

DateTime::DateTime(tm t_tm)
  : m_date(t_tm), m_time(t_tm), m_utcOffset(0.0)
{
}

/// assignment operator
DateTime& DateTime::operator= (const DateTime& other)
{
  m_date = Date(other.date());
  m_time = Time(other.time());
  m_utcOffset = other.utcOffset();

  // should not need to normalize

  return *this;
}

/// addition operator
DateTime DateTime::operator+ (const Time& time) const
{
  DateTime result(*this);
  result += time;
  return result;
}

/// assignment by addition operator
DateTime& DateTime::operator+= (const Time& time)
{
  m_time += time;
  normalize();
  return *this;
}

/// difference operator
DateTime DateTime::operator- (const Time& time) const
{
  DateTime result(*this);
  result -= time;
  return result;
}

/// assignment by difference operator
DateTime& DateTime::operator-= (const Time& time)
{
  m_time -= time;
  normalize();
  return *this;
}

/// time duration
Time DateTime::operator- (const DateTime& dateTime) const
{
  Time t1 = (m_date - dateTime.date());
  Time t2 = (m_time - dateTime.time());
  Time t3((dateTime.utcOffset() - m_utcOffset) / HOURS_PER_DAY);
  return t1 + t2 + t3;
}

/// time duration
Time DateTime::operator- (const Date& date) const
{
  return m_time + (m_date-date);
}

/// equality operator
bool DateTime::operator== (const DateTime& other) const
{
  return (toEpoch() == other.toEpoch());
}

/// non-equality operator
bool DateTime::operator!= (const DateTime& other) const
{
  return (toEpoch() != other.toEpoch());;
}

/// less than operator
bool DateTime::operator< (const DateTime& rhs) const
{
  return (toEpoch() < rhs.toEpoch());
}

/// less than equals operator
bool DateTime::operator<= (const DateTime& rhs) const
{
  return (toEpoch() <= rhs.toEpoch());
}

/// greater than operator
bool DateTime::operator> (const DateTime& rhs) const
{
  return (toEpoch() > rhs.toEpoch());
}

/// greater than equals operator
bool DateTime::operator>= (const DateTime& rhs) const
{
  return (toEpoch() >= rhs.toEpoch());
}

/// getter to date
Date DateTime::date() const
{
  return m_date;
}

/// getter to time
Time DateTime::time() const
{
  return m_time;
}

/// UTC offset in hours
double DateTime::utcOffset() const
{
  return m_utcOffset;
}
    
/// convert to string
std::string DateTime::toString() const
{
  boost::posix_time::ptime pt(m_date.impl(), m_time.impl() - boost::posix_time::time_duration(m_utcOffset, 0, 0, 0));
  return boost::posix_time::to_simple_string(pt);
}

std::string DateTime::toISO8601() const {
  boost::posix_time::ptime pt(m_date.impl(), m_time.impl());
  std::string result = boost::posix_time::to_iso_string(pt);
  if (m_utcOffset == 0.0){
    result += "Z";
  } else{
    Time temp(0,m_utcOffset,0,0);
    char buffer[64];
    if (temp.totalHours() < 0){
      sprintf(buffer, "-%02d%02d", std::abs(temp.hours()), std::abs(temp.minutes()));
    } else{
      sprintf(buffer, "+%02d%02d", std::abs(temp.hours()), std::abs(temp.minutes()));
    }
    result += buffer;
  }

  return result;
}

std::time_t DateTime::toEpoch() const {
  // credit: http://stackoverflow.com/questions/4461586/how-do-i-convert-boostposix-timeptime-to-time-t
  boost::posix_time::ptime pt(m_date.impl(), m_time.impl() - boost::posix_time::time_duration(m_utcOffset, 0, 0, 0));
  boost::posix_time::ptime epoch(boost::gregorian::date(1970,1,1));
  boost::posix_time::time_duration::sec_type x = (pt - epoch).total_seconds();
  return std::time_t(x);
}

boost::optional<DateTime> DateTime::fromISO8601(const std::string& str) {

  std::string finalStr(str);
  boost::replace_all(finalStr, ".", "");
  boost::replace_all(finalStr, ":", "");
  boost::replace_all(finalStr, "Z", "+0000");

  std::string timeZone;

  const static boost::regex yyyymmddThhmmssffRegex("^(\\d{4}-?\\d{2}-?\\d{2}T\\d{6})\\d{2}(.*?)$");
  const static boost::regex yyyymmddThhmmssRegex("^(\\d{4}-?\\d{2}-?\\d{2}T\\d{6})(.*?)$");
  const static boost::regex yyyymmddThhmmRegex("^(\\d{4}-?\\d{2}-?\\d{2}T\\d{4})(.*?)$");
  boost::smatch m;
  if (boost::regex_match(finalStr, m, yyyymmddThhmmssffRegex)) {
    timeZone = std::string(m[2].first, m[2].second); // pull out timeZone first because reassigning finalStr will invalidate iterators
    finalStr = std::string(m[1].first, m[1].second);
  }else if (boost::regex_match(finalStr, m, yyyymmddThhmmssRegex)) {
    timeZone = std::string(m[2].first, m[2].second); // pull out timeZone first because reassigning finalStr will invalidate iterators
    finalStr = std::string(m[1].first, m[1].second); 
  } else if (boost::regex_match(finalStr, m, yyyymmddThhmmRegex)) {
    timeZone = std::string(m[2].first, m[2].second); // pull out timeZone first because reassigning finalStr will invalidate iterators
    finalStr = std::string(m[1].first, m[1].second) + "00";
  }
  boost::replace_all(finalStr, "-", "");

  double utcOffset(0.0);
  if (!timeZone.empty()){
    const static boost::regex utcOffsetRegex("^([+-])(\\d{2})(\\d{1,2})$");
    if (boost::regex_match(timeZone, m, utcOffsetRegex)) {
      std::string sign(m[1].first, m[1].second);
      std::string hour(m[2].first, m[2].second);
      std::string min(m[3].first, m[3].second);
      utcOffset += boost::lexical_cast<double>(sign + hour);
      if (!min.empty()){
        utcOffset += boost::lexical_cast<double>(sign + min) / MINUTES_PER_HOUR;
      }
    }
  }

  OptionalDateTime result;
  try {
    boost::posix_time::ptime pt = boost::posix_time::from_iso_string(finalStr);
    result = DateTime(Date(pt.date()), Time(pt.time_of_day()), utcOffset);
  }
  catch (...) {
    LOG(Error,"Could not convert '" << str << "' to DateTime using boost::posix_time::from_iso_string.");
  }

  return result;
}

DateTime DateTime::fromEpoch(const std::time_t& time) {
  boost::posix_time::ptime pt = boost::posix_time::from_time_t(time);
  return DateTime(Date(pt.date()),Time(pt.time_of_day()),0.0);
}

// ensure that stored time is less than 24 hrs and adjust date accordingly
void DateTime::normalize()
{
  // total number of whole days in the time 
  int wholeDays = m_time.days();

  // if time is longer than a day (either positive or negative) normalize it to remainder less than 24 hrs
  if (wholeDays != 0){
    Time adjustTime(wholeDays,0,0,0);
    m_date += adjustTime;
    m_time -= adjustTime;
  }

  // total number of fractional days in the time 
  double fracDays = m_time.totalDays();

  // if time is negative add a day to make it positive
  if (fracDays < 0){
    Time adjustTime(1,0,0,0);
    m_date -= adjustTime;
    m_time += adjustTime;
  }
}

// std::ostream operator<<
std::ostream& operator<<(std::ostream& os, const DateTime& dateTime)
{
  os << dateTime.toString();
  return os;
}

// conversion for QDateTime
DateTime toDateTime(const QDateTime &qdt)
{
  Date d = Date::fromDayOfYear(qdt.date().dayOfYear(), qdt.date().year());
  Time t(0, qdt.time().hour(), qdt.time().minute(), qdt.time().second());
  int secondsFromUtc = qdt.offsetFromUtc();
  double utcOffset = secondsFromUtc * HOURS_PER_SECOND;

  return DateTime(d, t, utcOffset);
}

QDateTime toQDateTime(const DateTime& dt) {
  return QDateTime(
      QDate(dt.date().year(), 
            (dt.date().monthOfYear().value()) - openstudio::MonthOfYear::Jan + 1, 
            dt.date().dayOfMonth()),
      QTime(dt.time().hours(), 
            dt.time().minutes(), 
            dt.time().seconds()),
      Qt::OffsetFromUTC,
      dt.utcOffset() * SECONDS_PER_MINUTE * MINUTES_PER_HOUR
      );
}

namespace detail{
  DateTimeMetaTypeInitializer::DateTimeMetaTypeInitializer()
  {
    // int type;
    /* type =*/ qRegisterMetaType<openstudio::DateTime>("openstudio::DateTime");
    /* type =*/ qRegisterMetaType<openstudio::Date>("openstudio::Date");
    /* type =*/ qRegisterMetaType<openstudio::Time>("openstudio::Time");
  }
  static DateTimeMetaTypeInitializer __dateTimeMetaTypeInitializer__;
}

} // openstudio
