/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

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
#include <memory>
#include <shared_mutex>

/// defines method logChannel() to get a logger for a class
#define REGISTER_LOGGER(__logChannel__) \
  static openstudio::LogChannel logChannel() { return __logChannel__; }

/// log a message from within a registered class
#define LOG(__level__, __message__) LOG_FREE(__level__, logChannel(), __message__);

/// log a message from within a registered class and throw an exception
#define LOG_AND_THROW(__message__) LOG_FREE_AND_THROW(logChannel(), __message__);

/// log a message from outside a registered class
#define LOG_FREE(__level__, __channel__, __message__)        \
  {                                                          \
    std::stringstream _ss1;                                  \
    _ss1 << __message__;                                     \
    openstudio::logFree(__level__, __channel__, _ss1.str()); \
  }

/// log a message from outside a registered class and throw an exception
#define LOG_FREE_AND_THROW(__channel__, __message__)             \
  {                                                              \
    LOG_FREE(Fatal, __channel__, __message__);                   \
    std::stringstream _ss2;                                      \
    _ss2 << __FILE__ << "@" << __LINE__ << " : " << __message__; \
    throw openstudio::Exception(_ss2.str());                     \
  }

namespace openstudio {

class OSWorkflow;

/// convenience function for SWIG, prefer macros in C++
UTILITIES_API void logFree(LogLevel level, const std::string& channel, const std::string& message);

class Logger;

/** Primary logging class.
   */
class UTILITIES_API LoggerImpl
{
  friend class Logger;

 public:
  /// destructor, cleans up, writes xml file footers, etc
  ~LoggerImpl();

  /// get logger for standard out
  LogSink standardOutLogger() const;

  /// get logger for standard error
  LogSink standardErrLogger() const;

  /// get a logger from a LogChannel enumeration, if logChannel does not
  /// exist a new logger will be set up at the default level
  LoggerType& loggerFromChannel(const LogChannel& logChannel);

 protected:
  friend class detail::LogSink_Impl;
  friend class openstudio::OSWorkflow;

  /// is the sink found in the logging core
  bool findSink(boost::shared_ptr<LogSinkBackend> sink);

  /// adds a sink to the logging core, equivalent to logSink.enable()
  void addSink(boost::shared_ptr<LogSinkBackend> sink);

  /// removes a sink to the logging core, equivalent to logSink.disable()
  void removeSink(boost::shared_ptr<LogSinkBackend> sink);

  // cppcheck-suppress functionConst
  void addTimeStampToLogger();

 private:
  /// private constructor
  LoggerImpl();

  mutable std::shared_mutex m_mutex;

  /// standard out logger
  LogSink m_standardOutLogger;

  /// standard err logger
  LogSink m_standardErrLogger;

  /// map of std::string to logger
  using LoggerMapType = std::map<std::string, LoggerType, openstudio::IstringCompare>;
  LoggerMapType m_loggerMap;

  /// current sinks, kept here so don't destruct when LogSink wrapper goes out of scope
  using SinkSetType = std::set<boost::shared_ptr<LogSinkBackend>>;
  SinkSetType m_sinks;
};

class UTILITIES_API Logger
{
 public:
  Logger() = delete;

  static LoggerImpl& instance() {
    if (!obj) {
      obj = std::shared_ptr<LoggerImpl>(new LoggerImpl());
    }
    return *obj;
  }

 private:
  static std::shared_ptr<LoggerImpl> obj;
};

}  // namespace openstudio

#endif  // UTILITIES_CORE_LOGGER_HPP
