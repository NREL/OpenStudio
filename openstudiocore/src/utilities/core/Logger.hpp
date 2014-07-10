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

#ifndef UTILITIES_CORE_LOGGER_HPP
#define UTILITIES_CORE_LOGGER_HPP

#include "../UtilitiesAPI.hpp"

#include "Singleton.hpp"
#include "Exception.hpp"
#include "Compare.hpp"
#include "LogSink.hpp"

#include <boost/shared_ptr.hpp>

#include <sstream>
#include <set>
#include <map>

class QReadWriteLock;
class QWriteLocker;

/// defines method logChannel() to get a logger for a class
#define REGISTER_LOGGER(__logChannel__) \
  static openstudio::LogChannel logChannel(){ return __logChannel__; } \

/// log a message from within a registered class
#define LOG(__level__, __message__) \
  LOG_FREE(__level__, logChannel(), __message__);

/// log a message from within a registered class and throw an exception
#define LOG_AND_THROW(__message__) \
  LOG_FREE_AND_THROW(logChannel(), __message__);

/// log a message from outside a registered class
#define LOG_FREE(__level__, __channel__, __message__) \
  { \
    std::stringstream _ss1; \
    _ss1 << __message__; \
    openstudio::logFree(__level__, __channel__, _ss1.str()); \
  }

/// log a message from outside a registered class and throw an exception
#define LOG_FREE_AND_THROW(__channel__, __message__) \
  { \
    LOG_FREE(Fatal, __channel__, __message__); \
    std::stringstream _ss2; \
    _ss2 << __FILE__ << "@" << __LINE__ << " : " << __message__; \
    throw openstudio::Exception(_ss2.str()); \
  }

namespace openstudio{

  /// convenience function for SWIG, prefer macros in C++
  UTILITIES_API void logFree(LogLevel level, const std::string& channel, const std::string& message);

  /** Singleton logger class.  Singleton Logger object maintains logging state throughout
   *   program execution.
   */
  class UTILITIES_API LoggerSingleton {

    friend class Singleton<LoggerSingleton>;

    public:

    /// destructor, cleans up, writes xml file footers, etc
    ~LoggerSingleton();

    /// get logger for standard out
    LogSink standardOutLogger() const;

    /// get logger for standard error
    LogSink standardErrLogger() const;

    /// get a logger from a LogChannel enumeration, if logChannel does not
    /// exist a new logger will be set up at the default level
    LoggerType& loggerFromChannel(const LogChannel& logChannel);

   protected:

    friend class detail::LogSink_Impl;

    /// is the sink found in the logging core
    bool findSink(boost::shared_ptr<LogSinkBackend> sink);

    /// adds a sink to the logging core, equivalent to logSink.enable()
    void addSink(boost::shared_ptr<LogSinkBackend> sink);

    /// removes a sink to the logging core, equivalent to logSink.disable()
    void removeSink(boost::shared_ptr<LogSinkBackend> sink);

   private:

    /// private constructor
    LoggerSingleton();

    mutable QReadWriteLock* m_mutex;

    /// standard out logger
    LogSink m_standardOutLogger;

    /// standard err logger
    LogSink m_standardErrLogger;

    /// map of std::string to logger
    typedef std::map<std::string, LoggerType, openstudio::IstringCompare> LoggerMapType;
    LoggerMapType m_loggerMap;

    /// current sinks, kept here so don't destruct when LogSink wrapper goes out of scope
    typedef std::set<boost::shared_ptr<LogSinkBackend> > SinkSetType;
    SinkSetType m_sinks;
  };

#if _WIN32 || _MSC_VER

  /// Explicitly instantiate and export LoggerSingleton Singleton template instance
  /// so that the same instance is shared between the DLL's that link to Utilities.dll
  UTILITIES_TEMPLATE_EXT template class UTILITIES_API openstudio::Singleton<LoggerSingleton>;

#endif

  typedef openstudio::Singleton<LoggerSingleton> Logger;
} // openstudio

#endif // UTILITIES_CORE_LOGGER_HPP
