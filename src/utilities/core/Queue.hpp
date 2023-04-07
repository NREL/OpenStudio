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

#ifndef UTILITIES_CORE_QUEUE_HPP
#define UTILITIES_CORE_QUEUE_HPP

#include <boost/thread/condition.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/locks.hpp>
#include <stdexcept>
#include <deque>

namespace openstudio {
/**
   * Exception thrown if the user attempts to read data from openstudio::Queue<>, but the
   * Queue is canceled or shutdown
   */
struct queue_read_canceled : public std::runtime_error
{
  queue_read_canceled() : std::runtime_error("Queue read canceled during read operation") {}
};

/**
   * Exception thrown if the user attempts to read data from openstudio::Queue<>
   * with a non-blocking call and there is no data available.
   */
struct queue_read_empty : public std::runtime_error
{
  queue_read_empty() : std::runtime_error("Queue read empty on non-blocking call") {}
};

/**
   * Thread safe Queue for message processing.
   *
   * Queue allows the user to push messages in a thread-safe non-blocking way
   * and read queue messages in either blocking or non-blocking methods.
   * The blocking methods allow a thread to wait on new messages without
   * being in a busy loop and be notified when the new message is received.
   */
template <typename T>
class Queue
{
 public:
  Queue() : m_canceled(false) {}

  ~Queue() {
    cancel_get();
  }

  //! get the next available item in the message Queue. Method blocks
  //! until a new item is available
  //! \return true if item is populated false if not,
  //!         if the read was canceled before an item was
  //!         made available.
  T get_next() {
    boost::unique_lock<boost::shared_mutex> l(m_mutex);

    if (m_canceled) {
      throw queue_read_canceled();
    }

    if (m_deque.empty()) {
      m_condition.wait(l);
    }

    if (m_deque.empty() || m_canceled) {
      throw queue_read_canceled();
    } else {
      T retitem = m_deque.front();
      m_deque.pop_front();
      return retitem;
    }
  }

  //! get the next available item, but do not block if there are none available
  //! instead throw()
  T get_next_non_blocking() {
    boost::unique_lock<boost::shared_mutex> l(m_mutex);

    if (m_canceled) {
      throw queue_read_canceled();
    } else if (m_deque.empty()) {
      throw queue_read_empty();
    } else {
      T retitem = m_deque.front();
      m_deque.pop_front();
      return retitem;
    }
  }

  //! Return true if there is currently data available
  bool has_next() {
    boost::shared_lock<boost::shared_mutex> l(m_mutex);

    return !m_canceled && !m_deque.empty();
  }

  //! Cancel the current read operation.
  //! Pending readers will be woken and with throw an exception.
  void cancel_get() {
    boost::unique_lock<boost::shared_mutex> l(m_mutex);
    m_canceled = true;
    m_condition.notify_one();
  }

  //! Reset canceled state to false
  void reset() {
    boost::unique_lock<boost::shared_mutex> l(m_mutex);
    m_canceled = false;
  }

  //! Returns if in canceled state
  //! \return true if Queue had been canceled and not reset
  bool canceled() {
    boost::shared_lock<boost::shared_mutex> l(m_mutex);
    return m_canceled;
  }

  //! Add an item to the Queue for processing
  //! \param[in] item item to add
  void add(const T& item) {
    boost::unique_lock<boost::shared_mutex> l(m_mutex);

    if (!m_canceled) {
      m_deque.push_back(item);
      m_condition.notify_one();
    }
  }

 private:
  //! Variable to wait on when waiting for more items
  boost::condition m_condition;

  //! Protection mutex for local data
  boost::shared_mutex m_mutex;

  //! stl deque of incoming items.
  std::deque<T> m_deque;

  //! Copy constructor, private and explicitly not implemented
  Queue(const Queue<T>&);

  //! Assignment operator, private and explicitly not implemented
  Queue<T>& operator=(const Queue<T>&);

  //! Set to true if a read operation was canceled
  bool m_canceled;
};
}  // namespace openstudio

#endif  // UTILITIES_CORE_QUEUE_HPP
