/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "LogSink.hpp"
#include "LogSink_Impl.hpp"

#include "Logger.hpp"
#include "LogMessage.hpp"

#include <boost/log/support/regex.hpp>
#include <boost/log/expressions.hpp>
#include <boost/phoenix/bind.hpp>
#include <boost/log/support/date_time.hpp>

namespace sinks = boost::log::sinks;
namespace keywords = boost::log::keywords;
namespace expr = boost::log::expressions;

namespace openstudio {

namespace detail {

  // Custom severity level formatting function
  std::string formatter_severity_upcase(boost::log::value_ref<LogLevel> const& logLevel_) {
    static constexpr std::array<std::string_view, 6> logLevelStrs = {"TRACE", "DEBUG", "INFO", "WARN", "ERROR", "FATAL"};
    LogLevel logLevel = Trace;
    if (logLevel_) {
      // if (auto logLevel_ = rec[expr::attr<LogLevel>("Severity")]) {
      logLevel = logLevel_.get();
    }

    return std::string{logLevelStrs[static_cast<size_t>(logLevel) - static_cast<size_t>(LogLevel::Trace)]};
  }

  LogSink_Impl::LogSink_Impl()
    : m_sink{boost::shared_ptr<LogSinkBackend>(new LogSinkBackend())},
      // set formatting, seems like you have to call this after the stream is added
      // DLM@20110701: would like to format Severity as string but can't figure out how to do it
      // because you can't overload operator<< for an enum type
      // this seems to suggest this should work: http://www.edm2.com/0405/enumeration.html
      m_formatter{expr::stream << "[" << expr::attr<LogChannel>("Channel") << "] <" << expr::attr<LogLevel>("Severity") << "> " << expr::smessage} {}

  void LogSink_Impl::setFormatter(const boost::log::formatter& fmter) {
    std::unique_lock l{m_mutex};
    m_formatter = fmter;
    m_sink->set_formatter(m_formatter);
  }

  void LogSink_Impl::useWorkflowGemFormatter(bool use, bool include_channel) {
    std::unique_lock l{m_mutex};
    if (use) {
      Logger::instance().addTimeStampToLogger();
      if (include_channel) {
        m_formatter = expr::stream << "[" << expr::format_date_time<boost::posix_time::ptime>("TimeStamp", "%H:%M:%S.%f") << " "
                                   << boost::phoenix::bind(&formatter_severity_upcase, expr::attr<LogLevel>("Severity")) << "] " << "["
                                   << expr::attr<LogChannel>("Channel") << "] " << expr::smessage;
      } else {
        m_formatter = expr::stream << "[" << expr::format_date_time<boost::posix_time::ptime>("TimeStamp", "%H:%M:%S.%f") << " "
                                   << boost::phoenix::bind(&formatter_severity_upcase, expr::attr<LogLevel>("Severity")) << "] " << expr::smessage;
      }

    } else {
      m_formatter = expr::stream << "[" << expr::attr<LogChannel>("Channel") << "] <" << expr::attr<LogLevel>("Severity") << "> " << expr::smessage;
    }
    m_sink->set_formatter(m_formatter);
  }

  bool LogSink_Impl::isEnabled() const {
    return Logger::instance().findSink(m_sink);
  }

  void LogSink_Impl::enable() {
    Logger::instance().addSink(m_sink);
  }

  void LogSink_Impl::disable() {
    Logger::instance().removeSink(m_sink);
  }

  boost::optional<LogLevel> LogSink_Impl::logLevel() const {
    std::shared_lock l{m_mutex};

    return m_logLevel;
  }

  void LogSink_Impl::setLogLevel(LogLevel logLevel) {
    std::unique_lock l{m_mutex};

    m_logLevel = logLevel;

    this->updateFilter(l);
  }

  void LogSink_Impl::resetLogLevel() {
    std::unique_lock l{m_mutex};

    m_logLevel.reset();

    this->updateFilter(l);
  }

  boost::optional<boost::regex> LogSink_Impl::channelRegex() const {
    std::shared_lock l{m_mutex};

    return m_channelRegex;
  }

  void LogSink_Impl::setChannelRegex(const boost::regex& channelRegex) {
    std::unique_lock l{m_mutex};

    m_channelRegex = channelRegex;

    this->updateFilter(l);
  }

  void LogSink_Impl::setChannelRegex(const std::string& channelRegex) {
    boost::regex br(channelRegex);
    setChannelRegex(br);
  }

  void LogSink_Impl::resetChannelRegex() {
    std::unique_lock l{m_mutex};

    m_channelRegex.reset();

    this->updateFilter(l);
  }

  bool LogSink_Impl::autoFlush() const {
    std::shared_lock l{m_mutex};

    return m_autoFlush;
  }

  void LogSink_Impl::setAutoFlush(bool autoFlush) {
    std::unique_lock l{m_mutex};

    m_autoFlush = autoFlush;

    m_sink->locked_backend()->auto_flush(autoFlush);
  }

  std::thread::id LogSink_Impl::threadId() const {
    std::shared_lock l{m_mutex};

    return m_threadId;
  }

  void LogSink_Impl::setThreadId(const std::thread::id threadId) {
    std::unique_lock l{m_mutex};

    m_threadId = threadId;

    this->updateFilter(l);
  }

  void LogSink_Impl::resetThreadId() {
    std::unique_lock l{m_mutex};

    m_threadId = std::thread::id{};  // default represents no thread

    this->updateFilter(l);
  }

  void LogSink_Impl::setStream(boost::shared_ptr<std::ostream> os) {
    std::unique_lock l{m_mutex};

    m_sink->locked_backend()->add_stream(os);

    m_sink->set_formatter(m_formatter);

    //m_sink->locked_backend()->set_formatter(fmt::stream
    //  << "[" << fmt::attr< LogChannel >("Channel")
    //  << "] <" << fmt::attr< LogLevel >("Severity")
    //  << "> @" << fmt::attr< std::thread::id >("ThreadId") << ", " << fmt::message());

    this->updateFilter(l);

    // avoid deadlock
    l.unlock();

    this->setAutoFlush(true);
  }

  boost::shared_ptr<LogSinkBackend> LogSink_Impl::sink() const {
    std::shared_lock l{m_mutex};

    return m_sink;
  }

  void LogSink_Impl::updateFilter(const std::unique_lock<std::shared_mutex>& /*l*/) {
    m_sink->reset_filter();

    LogLevel filterLogLevel = Trace;
    if (m_logLevel) {
      filterLogLevel = *m_logLevel;
    }

    boost::regex filterChannelRegex(".*");
    if (m_channelRegex) {
      filterChannelRegex = *m_channelRegex;
    }

    if (m_threadId != std::thread::id{}) {
      m_sink->set_filter(expr::attr<LogLevel>("Severity") >= filterLogLevel && expr::attr<std::thread::id>("ThreadId") == m_threadId
                         && expr::matches(expr::attr<LogChannel>("Channel"), filterChannelRegex));
    } else {
      m_sink->set_filter(expr::attr<LogLevel>("Severity") >= filterLogLevel && expr::matches(expr::attr<LogChannel>("Channel"), filterChannelRegex));
    }
  }

}  // namespace detail

LogSink::LogSink() : m_impl(new detail::LogSink_Impl()) {}

LogSink::LogSink(const boost::shared_ptr<detail::LogSink_Impl>& impl) : m_impl(impl) {}

bool LogSink::isEnabled() const {
  return m_impl->isEnabled();
}

void LogSink::enable() {
  m_impl->enable();
}

void LogSink::disable() {
  m_impl->disable();
}

boost::optional<LogLevel> LogSink::logLevel() const {
  return m_impl->logLevel();
}

void LogSink::setLogLevel(LogLevel logLevel) {
  m_impl->setLogLevel(logLevel);
}

void LogSink::resetLogLevel() {
  m_impl->resetLogLevel();
}

boost::optional<boost::regex> LogSink::channelRegex() const {
  return m_impl->channelRegex();
}

void LogSink::setChannelRegex(const boost::regex& channelRegex) {
  m_impl->setChannelRegex(channelRegex);
}

void LogSink::setChannelRegex(const std::string& channelRegex) {
  m_impl->setChannelRegex(channelRegex);
}

void LogSink::resetChannelRegex() {
  m_impl->resetChannelRegex();
}

bool LogSink::autoFlush() const {
  return m_impl->autoFlush();
}

void LogSink::setAutoFlush(bool autoFlush) {
  m_impl->setAutoFlush(autoFlush);
}

std::thread::id LogSink::threadId() const {
  return m_impl->threadId();
}

void LogSink::setThreadId(const std::thread::id threadId) {
  m_impl->setThreadId(threadId);
}

void LogSink::resetThreadId() {
  m_impl->resetThreadId();
}

void LogSink::setStream(boost::shared_ptr<std::ostream> os) {
  m_impl->setStream(os);
}

boost::shared_ptr<LogSinkBackend> LogSink::sink() const {
  return m_impl->sink();
}

void LogSink::useWorkflowGemFormatter(bool use, bool include_channel) {
  m_impl->useWorkflowGemFormatter(use, include_channel);
}

void LogSink::setFormatter(const boost::log::formatter& fmter) {
  m_impl->setFormatter(fmter);
}

}  // namespace openstudio
