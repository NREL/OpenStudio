/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef UTILITIES_CORE_LOGSINK_IMPL_HPP
#define UTILITIES_CORE_LOGSINK_IMPL_HPP

#include "../UtilitiesAPI.hpp"

#include "LogMessage.hpp"
#include "LogSink.hpp"

#include <boost/optional.hpp>

#include <shared_mutex>

namespace openstudio {

namespace detail {

  /// LogSink is a class for managing sinks for log messages, e.g. files, streams, etc.
  class UTILITIES_API LogSink_Impl
  {
   public:
    /// destructor
    virtual ~LogSink_Impl() = default;

    /// is the sink enabled
    bool isEnabled() const;

    /// enable the sink
    // cppcheck-suppress functionConst
    void enable();

    /// disable the sink
    // cppcheck-suppress functionConst
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

    void setFormatter(const boost::log::formatter& fmter);

    void useWorkflowGemFormatter(bool use, bool include_channel = false);

   protected:
    friend class openstudio::LogSink;

    // does not register in the global logger
    LogSink_Impl();

    // must be set in the constructor
    void setStream(boost::shared_ptr<std::ostream> os);

    // for adding cout and cerr sinks to logger
    boost::shared_ptr<LogSinkBackend> sink() const;

    mutable std::shared_mutex m_mutex;

   private:
    void updateFilter(const std::unique_lock<std::shared_mutex>& l);

    boost::optional<LogLevel> m_logLevel;
    boost::optional<boost::regex> m_channelRegex;
    bool m_autoFlush = false;
    std::thread::id m_threadId;
    boost::shared_ptr<LogSinkBackend> m_sink;
    boost::log::formatter m_formatter;
  };

}  // namespace detail

}  // namespace openstudio

#endif  // UTILITIES_CORE_LOGSINK_IMPL_HPP
