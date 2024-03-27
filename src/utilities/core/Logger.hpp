/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef UTILITIES_CORE_LOGGER_HPP
#define UTILITIES_CORE_LOGGER_HPP

#include "../UtilitiesAPI.hpp"

#include "Compare.hpp"
#include "LogMessage.hpp"
#include "LogSink.hpp"
#include "Exception.hpp"

#include <boost/shared_ptr.hpp>

#include <map>
#include <memory>
#include <set>
#include <shared_mutex>
#include <sstream>

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
class LogSink;
namespace detail {
  class LogSink_Impl;
}  // namespace detail

/// convenience function for SWIG, prefer macros in C++
UTILITIES_API void logFree(LogLevel level, const std::string& channel, const std::string& message);

class UTILITIES_API Logger
{
 public:
  static Logger& instance();

  Logger(const Logger& other) = delete;
  Logger(Logger&& other) = delete;
  Logger& operator=(const Logger&) = delete;
  Logger& operator=(Logger&&) = delete;

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
  Logger();
  ~Logger() = default;

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

}  // namespace openstudio

#endif  // UTILITIES_CORE_LOGGER_HPP
