/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef UTILITIES_CORE_LOGMESSAGE_HPP
#define UTILITIES_CORE_LOGMESSAGE_HPP

#include "../UtilitiesAPI.hpp"

#include "String.hpp"

#include <boost/log/sinks/sync_frontend.hpp>
#include <boost/log/sinks/text_ostream_backend.hpp>
#include <boost/log/sources/severity_channel_logger.hpp>

#include <vector>

/** Severity levels for logging, Warn is default = 0
 *  Trace = debug messages not currently needed, but may be useful later
 *  Debug = only makes sense to developer trying to track down bug
 *  Info = something a user might want to see if interested (e.g. assumptions made, etc)
 *  Warning = something that probably should not have happened
 *  Error = something that definitely should not have happened
 *  Fatal = something so bad execution can't proceed
 *  Defined at the root namespace level for simplicity */
enum LogLevel
{
  Trace = -3,
  Debug = -2,
  Info = -1,
  Warn = 0,
  Error = 1,
  Fatal = 2
};

namespace openstudio {

/// LogChannel identifies a logger
using LogChannel = std::string;

/// Type of stream sink used
using LogSinkBackend = boost::log::sinks::synchronous_sink<boost::log::sinks::text_ostream_backend>;

/// Type of logger used
using LoggerType = boost::log::sources::severity_channel_logger_mt<LogLevel>;

/// LogMessage encapsulates a single logging message
class UTILITIES_API LogMessage
{
 public:
  /// constructor
  LogMessage(LogLevel logLevel, const std::string& logChannel, const std::string& logMessage);

  /// parse logText and get log messages
  static std::vector<LogMessage> parseLogText(const std::string& logText);

  /// get the message's log level
  LogLevel logLevel() const;

  /// get the messages log channel
  LogChannel logChannel() const;

  /// get the content of the log message
  std::string logMessage() const;

 private:
  LogLevel m_logLevel;
  std::string m_logChannel;
  std::string m_logMessage;
};

}  // namespace openstudio

#endif  // UTILITIES_CORE_LOGMESSAGE_HPP
