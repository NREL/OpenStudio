/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef UTILITIES_CORE_THREADSAFEDEQUE_HPP
#define UTILITIES_CORE_THREADSAFEDEQUE_HPP

#include <condition_variable>
#include <deque>
#include <mutex>

namespace openstudio {

// Thank you https://codereview.stackexchange.com/questions/238347/a-simple-thread-safe-deque-in-c
template <class T>
class ThreadSafeDeque
{
 public:
  T wait_for_one() {
    // unique_lock can be unlocked, lock_guard can not
    std::unique_lock<std::mutex> lock{m_mutex};  // locks
    while (m_deque.empty()) {
      m_condition.wait(lock);  // unlocks, sleeps and relocks when woken up
    }
    auto t = std::move(m_deque.front());
    m_deque.pop_front();
    return t;
  }  // unlocks as goes out of scope

  void push_back(T&& t) {
    std::unique_lock<std::mutex> lock{m_mutex};
    m_deque.push_back(std::move(t));
    lock.unlock();
    m_condition.notify_one();  // wakes up pop_front_waiting
  }

 private:
  std::deque<T> m_deque;
  std::mutex m_mutex;
  std::condition_variable m_condition{};
};

}  // namespace openstudio

#endif  // UTILITIES_CORE_THREADSAFEDEQUE_HPP
