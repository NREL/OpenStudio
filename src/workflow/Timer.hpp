/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef WORKFLOW_UTIL_TIMER_HPP
#define WORKFLOW_UTIL_TIMER_HPP

#include <chrono>
#include <string>
#include <vector>

namespace Json {
class Value;
}

namespace openstudio::workflow::util {

class Timer
{
 public:
  // start is initialized to now() when ctor is called, end is not initialized
  Timer(std::string message, unsigned level = 0);

  auto start() const;
  auto end() const;

  unsigned level() const;

  // Reset start to now, end to none
  void tick();

  // Capture end time
  void tock();

  std::string message() const;

  auto duration() const;

  std::string format() const;

  std::string formatRow(size_t message_len, size_t timepoint_len, size_t duration_len) const;

  Json::Value toJSON() const;

 private:
  using DurationType = std::chrono::milliseconds;
  using ClockType = std::chrono::system_clock;
  using TimePointType = decltype(ClockType::now());

  std::string m_message;
  TimePointType m_start = ClockType::now();
  TimePointType m_end;
  unsigned m_level;
};

class TimerCollection
{
 public:
  TimerCollection() = default;

  // TODO: maybe it's a bad idea to return the Timer reference... code writer is responsible for ensuring they call tockCurrentTimer
  // We could just return a size_t (currentTimerIndex) and have tockTimer(size_t index);
  Timer& newTimer(std::string message, unsigned level = 0);
  void tockCurrentTimer();

  // line_length is the maximum terminal width, fit = true will cause the table to be resized down as much as possible, fit = false means the table will take exactly line_length
  std::string timeReport(int line_length = 80, bool fit = true) const;

  Json::Value toJSON(bool useFlatArray) const;

 private:
  std::vector<Timer> m_timers;
  mutable Timer m_totalTimer{"Total"};

  // this is so we can nest timings and detailed (sub) timings
  std::vector<size_t> m_timerIndices;
};

}  // namespace openstudio::workflow::util

#endif  // WORKFLOW_UTIL_TIMER_HPP
