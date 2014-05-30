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

#ifndef UTILITIES_CORE_QUEUE_HPP
#define UTILITIES_CORE_QUEUE_HPP

#include <boost/thread/condition.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/locks.hpp>
#include <stdexcept>
#include <deque>

namespace openstudio
{
  /**
   * Exception thrown if the user attempts to read data from openstudio::Queue<>, but the
   * Queue is canceled or shutdown
   */
  struct queue_read_canceled : public std::runtime_error
  {
    queue_read_canceled()
      : std::runtime_error("Queue read canceled during read operation")
    {
    }
  };

  /**
   * Exception thrown if the user attempts to read data from openstudio::Queue<>
   * with a non-blocking call and there is no data available.
   */
  struct queue_read_empty : public std::runtime_error
  {
    queue_read_empty()
      : std::runtime_error("Queue read empty on non-blocking call")
    {
    }
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
        Queue() 
          : m_canceled(false)
        {
        }

        ~Queue()
        {
          cancel_get();
        }

        //! get the next available item in the message Queue. Method blocks
        //! until a new item is available
        //! \return true if item is populated false if not,
        //!         if the read was canceled before an item was
        //!         made available.
        T get_next() 
        {
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
        T get_next_non_blocking()
        {
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
        bool has_next()
        {
          boost::shared_lock<boost::shared_mutex> l(m_mutex);

          return !m_canceled && !m_deque.empty();
        }

        //! Cancel the current read operation. 
        //! Pending readers will be woken and with throw an exception.
        void cancel_get()
        {
          boost::unique_lock<boost::shared_mutex> l(m_mutex);
          m_canceled = true;
          m_condition.notify_one();
        }

        //! Reset canceled state to false
        void reset()
        {
          boost::unique_lock<boost::shared_mutex> l(m_mutex);
          m_canceled = false;
        }

        //! Returns if in canceled state
        //! \return true if Queue had been canceled and not reset
        bool canceled()
        {
          boost::shared_lock<boost::shared_mutex> l(m_mutex);
          return m_canceled;
        }

        //! Add an item to the Queue for processing
        //! \param[in] item item to add
        void add(const T &item)
        {
          boost::unique_lock<boost::shared_mutex> l(m_mutex);

          if (!m_canceled)
          {
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
        Queue(const Queue<T> &);

        //! Assignment operator, private and explicitly not implemented
        Queue<T> &operator=(const Queue<T> &);

        //! Set to true if a read operation was canceled
        bool m_canceled;
    };
}

#endif // UTILITIES_CORE_QUEUE_HPP
