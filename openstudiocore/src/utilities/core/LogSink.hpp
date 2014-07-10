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

#ifndef UTILITIES_CORE_LOGSINK_HPP
#define UTILITIES_CORE_LOGSINK_HPP

#include "../UtilitiesAPI.hpp"

#include "LogMessage.hpp"

#include <boost/regex.hpp>
#include <boost/shared_ptr.hpp>

#include <ostream>

class QThread;

namespace openstudio{

  namespace detail {
    class LogSink_Impl;
  }

  /// LogSink is a class for managing sinks for log messages, e.g. files, streams, etc.
  class UTILITIES_API LogSink
  {
    public:

    /// is the sink enabled
    bool isEnabled() const;

    /// enable the sink
    void enable();

    /// disable the sink
    void disable();

    /// get the logging level
    boost::optional<LogLevel> logLevel() const;

    /// set the logging level
    void setLogLevel(LogLevel logLevel);

    /// reset the core logging level
    void resetLogLevel();

    /// get the regular expression to match log channels
    boost::optional<boost::regex> channelRegex() const;

    /// set the regular expression to match log channels
    void setChannelRegex(const boost::regex& filter);

    /// reset the regular expression to match log channels
    void resetChannelRegex();

    /// get if messages are automatically flushed
    bool autoFlush() const;

    /// set if messages are automatically flushed
    void setAutoFlush(bool autoFlush);

    /// get the thread id that messages are filtered by
    /// thread id is the value returned by QThread::currentThread()
    QThread* threadId() const;

    /// set the thread id that messages are filtered by
    /// thread id is the value returned by QThread::currentThread()
    void setThreadId(QThread* threadId);

    /// reset the thread id that messages are filtered by
    void resetThreadId();

  protected:

    friend class LoggerSingleton;

    // does not register in the global logger
    LogSink();

    // does not register in the global logger
    LogSink(const boost::shared_ptr<detail::LogSink_Impl>& impl);

    // must be set in the constructor
    void setStream(boost::shared_ptr<std::ostream> os);

    // for adding cout and cerr sinks to logger
    boost::shared_ptr<LogSinkBackend> sink() const;

    // get the impl
    template<typename T>
    boost::shared_ptr<T> getImpl() const
    {  return boost::dynamic_pointer_cast<T>(m_impl); }

  private:

    boost::shared_ptr<detail::LogSink_Impl> m_impl;
  };

} // openstudio

#endif // UTILITIES_CORE_LOGSINK_HPP
