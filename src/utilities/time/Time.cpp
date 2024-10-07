/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "Time.hpp"

using namespace std;
using namespace boost;
using namespace posix_time;

namespace openstudio {

/// round towards zero, need to move this
int floor0(double value) {
  int result(0);
  if (value < 0.0) {
    result = static_cast<int>(ceil(value));
  } else {
    result = static_cast<int>(floor(value));
  };
  return result;
}

/// get current local time of day
Time Time::currentTime() {
  posix_time::ptime now = posix_time::second_clock::local_time();
  return Time(now.time_of_day());
}

/// get current UTC time of day
Time Time::currentTimeUTC() {
  posix_time::ptime now = posix_time::second_clock::universal_time();
  return Time(now.time_of_day());
}

/// Default constructor
Time::Time() : m_impl(0, 0, 0, 0) {}

/// Time from number of days, fractional values ok
Time::Time(double fracDays) {
  double fracHours = HOURS_PER_DAY * fracDays;
  int hours = floor0(fracHours);

  double fracMinutes = MINUTES_PER_HOUR * (fracHours - hours);
  int minutes = floor0(fracMinutes);

  double fracSeconds = SECONDS_PER_MINUTE * (fracMinutes - minutes);
  int seconds = floor0(fracSeconds);

  m_impl = ImplType(hours, minutes, seconds, 0);
}

/// Time from days, hours, minutes, seconds
Time::Time(int days, int hours, int minutes, int seconds) {
  hours += HOURS_PER_DAY * days;
  if ((hours * minutes >= 0) && (hours * seconds >= 0) && (minutes * seconds >= 0)) {
    // same sign, carry on
    m_impl = ImplType(hours, minutes, seconds, 0);
  } else {
    // mixed sign
    ImplType negativeDuration(std::min(hours, 0), std::min(minutes, 0), std::min(seconds, 0), 0);
    m_impl = ImplType(std::max(hours, 0), std::max(minutes, 0), std::max(seconds, 0), 0);
    m_impl += negativeDuration;
  }
}

Time::Time(const std::string& string) {
  boost::posix_time::time_duration td = boost::posix_time::duration_from_string(string);

  m_impl = boost::posix_time::time_duration(td);
}

Time::Time(tm t_tm) : m_impl(t_tm.tm_hour, t_tm.tm_min, t_tm.tm_sec) {}

/// copy constructor
Time::Time(const Time& other) : m_impl(other.impl()) {}

/// Time from impl
Time::Time(const ImplType& implType) : m_impl(implType) {}

/// assignment operator
Time& Time::operator=(const Time& other) {
  m_impl = other.impl();
  return *this;
}

/// addition operator
Time Time::operator+(const Time& time) const {
  Time result(*this);
  result += time;
  return result;
}

/// assignment by addition operator
Time& Time::operator+=(const Time& time) {
  m_impl += time.impl();
  return *this;
}

std::string Time::toString() const {
  return boost::posix_time::to_simple_string(m_impl);
}

/// difference operator
Time Time::operator-(const Time& time) const {
  Time result(*this);
  result -= time;
  return result;
}

/// assignment by difference operator
Time& Time::operator-=(const Time& time) {
  m_impl -= time.impl();
  return *this;
}

/// multiplication operator
Time Time::operator*(double mult) const {
  return {this->totalDays() * mult};
}

/// division operator
Time Time::operator/(double div) const {
  return {this->totalDays() / div};
}

/// equality operator
bool Time::operator==(const Time& other) const {
  return totalSeconds() == other.totalSeconds();
}

/// non-equality operator
bool Time::operator!=(const Time& other) const {
  return totalSeconds() != other.totalSeconds();
}

/// less than operator
bool Time::operator<(const Time& rhs) const {
  return totalSeconds() < rhs.totalSeconds();
}

/// less than equals operator
bool Time::operator<=(const Time& rhs) const {
  return totalSeconds() <= rhs.totalSeconds();
}

/// greater than operator
bool Time::operator>(const Time& rhs) const {
  return totalSeconds() > rhs.totalSeconds();
}

/// greater than equals operator
bool Time::operator>=(const Time& rhs) const {
  return totalSeconds() >= rhs.totalSeconds();
}

/// whole number of days
int Time::days() const {
  return floor0(totalDays());
}

/// whole number of hours remaining after days
int Time::hours() const {
  return (m_impl.hours() % 24);
}

/// whole number of minutes remaining after hours
int Time::minutes() const {
  return (int)m_impl.minutes();
}

/// whole number of seconds remaining after minutes
int Time::seconds() const {
  return (int)m_impl.seconds();
}

/// entire time in days
double Time::totalDays() const {
  return DAYS_PER_SECOND * totalSeconds();
}

/// entire time in hours
double Time::totalHours() const {
  return HOURS_PER_SECOND * totalSeconds();
}

/// entire time in minutes
double Time::totalMinutes() const {
  return MINUTES_PER_SECOND * totalSeconds();
}

/// entire time in seconds
int Time::totalSeconds() const {
  return (int)m_impl.total_seconds();
}

// reference to impl
const Time::ImplType Time::impl() const {
  return m_impl;
}

// std::ostream operator<<
std::ostream& operator<<(std::ostream& os, const Time& time) {
  // DLM@2009_07_09: os << time.impl(); does not work on windows as strftime limits
  // hours from 0 to 23.

  os << boost::posix_time::to_simple_string(time.impl());
  return os;
}

// compare two Dates
bool TimeCompare::operator()(const Time& lhs, const Time& rhs) const {
  return lhs < rhs;
}

}  // namespace openstudio
