/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef UTILITIES_CORE_LOGSINK_HPP
#define UTILITIES_CORE_LOGSINK_HPP

#include "../UtilitiesAPI.hpp"

#include "LogMessage.hpp"

#include <boost/regex.hpp>
#include <boost/shared_ptr.hpp>

#include <ostream>
#include <thread>

namespace openstudio {

namespace detail {
  class LogSink_Impl;
  class Logger_Impl;
}  // namespace detail

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
  void setChannelRegex(const boost::regex& channelRegex);

  /// set the regular expression to match log channels
  void setChannelRegex(const std::string& channelRegex);

  /// reset the regular expression to match log channels
  void resetChannelRegex();

  /// get if messages are automatically flushed
  bool autoFlush() const;

  /// set if messages are automatically flushed
  void setAutoFlush(bool autoFlush);

  /// get the thread id that messages are filtered by
  std::thread::id threadId() const;

  /// set the thread id that messages are filtered by
  void setThreadId(std::thread::id threadId);

  /// reset the thread id that messages are filtered by
  void resetThreadId();

  void useWorkflowGemFormatter(bool use, bool include_channel = true);

  void setFormatter(const boost::log::formatter& fmter);

 protected:
  friend class detail::Logger_Impl;

  // does not register in the global logger
  LogSink();

  // does not register in the global logger
  LogSink(const boost::shared_ptr<detail::LogSink_Impl>& impl);

  // must be set in the constructor
  void setStream(boost::shared_ptr<std::ostream> os);

  // for adding cout and cerr sinks to logger
  boost::shared_ptr<LogSinkBackend> sink() const;

  // get the impl
  template <typename T>
  boost::shared_ptr<T> getImpl() const {
    return boost::dynamic_pointer_cast<T>(m_impl);
  }

 private:
  boost::shared_ptr<detail::LogSink_Impl> m_impl;
};

}  // namespace openstudio

#endif  // UTILITIES_CORE_LOGSINK_HPP
