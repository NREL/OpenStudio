/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "Logger.hpp"
#include "Logger_Impl.hpp"

#include <boost/log/common.hpp>
#include <boost/log/attributes/function.hpp>
#include <boost/log/attributes/clock.hpp>

#include <boost/core/null_deleter.hpp>
#include <utility>

namespace sinks = boost::log::sinks;
namespace keywords = boost::log::keywords;

namespace openstudio {

/// convenience function for SWIG, prefer macros in C++
void logFree(LogLevel level, const std::string& channel, const std::string& message) {
  BOOST_LOG_SEV(openstudio::Logger::instance().loggerFromChannel(channel), level) << message;
}

namespace detail {

  Logger_Impl::Logger_Impl() {
    // Make current thread id attribute available to logging
    boost::log::core::get()->add_global_attribute("ThreadId", boost::log::attributes::make_function(&std::this_thread::get_id));

    // We have to provide an null deleter to avoid destroying the global stream
    boost::shared_ptr<std::ostream> stdOut(&std::cout, boost::null_deleter());
    m_standardOutLogger.setStream(stdOut);
    m_standardOutLogger.setLogLevel(Warn);
    this->addSink(m_standardOutLogger.sink());

    // We have to provide an empty deleter to avoid destroying the global stream
    boost::shared_ptr<std::ostream> stdErr(&std::cerr, boost::null_deleter());
    m_standardErrLogger.setStream(stdErr);
    m_standardErrLogger.setLogLevel(Warn);
    //this->addSink(m_standardErrLogger.sink());
  }

  LogSink Logger_Impl::standardOutLogger() const {
    std::shared_lock l{m_mutex};

    return m_standardOutLogger;
  }

  LogSink Logger_Impl::standardErrLogger() const {
    std::shared_lock l{m_mutex};

    return m_standardErrLogger;
  }

  LoggerType& Logger_Impl::loggerFromChannel(const LogChannel& logChannel) {
    std::shared_lock l{m_mutex};

    auto it = m_loggerMap.find(logChannel);
    if (it == m_loggerMap.end()) {
      //LoggerType newLogger(keywords::channel = logChannel, keywords::severity = Debug);
      LoggerType newLogger(keywords::channel = logChannel);

      std::pair<LogChannel, LoggerType> newPair(logChannel, newLogger);

      // Drop the read lock and grab a write lock - we need to add the new file to the map
      // this will reduce contention when multiple threads trying to log at once.
      l.unlock();
      std::unique_lock l2{m_mutex};

      std::pair<LoggerMapType::iterator, bool> inserted = m_loggerMap.insert(newPair);

      return inserted.first->second;
    }

    return it->second;
  }

  bool Logger_Impl::findSink(boost::shared_ptr<LogSinkBackend> sink) {
    std::unique_lock l{m_mutex};

    auto it = m_sinks.find(sink);

    return (it != m_sinks.end());
  }

  void Logger_Impl::addSink(boost::shared_ptr<LogSinkBackend> sink) {
    std::shared_lock l{m_mutex};

    auto it = m_sinks.find(sink);
    if (it == m_sinks.end()) {

      // Drop the read lock and grab a write lock - we need to add the new file to the map
      // this will reduce contention when multiple threads trying to log at once.
      l.unlock();
      std::unique_lock l2{m_mutex};

      m_sinks.insert(sink);

      // Register the sink in the logging core
      boost::log::core::get()->add_sink(sink);
    }
  }

  void Logger_Impl::removeSink(boost::shared_ptr<LogSinkBackend> sink) {
    std::shared_lock l{m_mutex};

    auto it = m_sinks.find(sink);
    if (it != m_sinks.end()) {

      // Drop the read lock and grab a write lock - we need to add the new file to the map
      // this will reduce contention when multiple threads trying to log at once.
      l.unlock();
      std::unique_lock l2{m_mutex};

      m_sinks.erase(it);

      // Register the sink in the logging core
      boost::log::core::get()->remove_sink(sink);
    }
  }

  void Logger_Impl::addTimeStampToLogger() {
    std::unique_lock l{m_mutex};

    // Add a TimeStamp attribute, same as boost::log::add_common_attributes() would do
    [[maybe_unused]] auto [it, inserted] = boost::log::core::get()->add_global_attribute("TimeStamp", boost::log::attributes::local_clock{});
    // if (!use) {
    //   boost::log::core::get()->remove_global_attribute(it);
    // }
  }

}  // namespace detail

Logger::Logger() : m_impl(std::shared_ptr<detail::Logger_Impl>(new detail::Logger_Impl())) {}

Logger& Logger::instance() {
  static Logger instance;
  return instance;
}

/// get logger for standard out
LogSink Logger::standardOutLogger() const {
  return m_impl->standardOutLogger();
}

LogSink Logger::standardErrLogger() const {
  return m_impl->standardErrLogger();
}

/// get a logger from a LogChannel enumeration, if logChannel does not
/// exist a new logger will be set up at the default level
LoggerType& Logger::loggerFromChannel(const LogChannel& logChannel) {
  return m_impl->loggerFromChannel(logChannel);
}

/// is the sink found in the logging core
bool Logger::findSink(boost::shared_ptr<LogSinkBackend> sink) {
  return m_impl->findSink(std::move(sink));
}

/// adds a sink to the logging core, equivalent to logSink.enable()
void Logger::addSink(boost::shared_ptr<LogSinkBackend> sink) {
  m_impl->addSink(std::move(sink));
}

/// removes a sink to the logging core, equivalent to logSink.disable()
void Logger::removeSink(boost::shared_ptr<LogSinkBackend> sink) {
  m_impl->removeSink(std::move(sink));
}

// cppcheck-suppress functionConst
void Logger::addTimeStampToLogger() {
  m_impl->addTimeStampToLogger();
}

}  // namespace openstudio
