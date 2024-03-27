/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "Logger.hpp"

#include <boost/log/common.hpp>
#include <boost/log/attributes/function.hpp>
#include <boost/log/attributes/clock.hpp>

#include <boost/core/null_deleter.hpp>

#include <utility>

namespace openstudio {

/// convenience function for SWIG, prefer macros in C++
void logFree(LogLevel level, const std::string& channel, const std::string& message) {
  BOOST_LOG_SEV(openstudio::Logger::instance().loggerFromChannel(channel), level) << message;
}

// Meyers' singleton
Logger& Logger::instance() {
  static Logger instance;
  return instance;
}

Logger::Logger() {
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

LogSink Logger::standardOutLogger() const {
  std::shared_lock l{m_mutex};

  return m_standardOutLogger;
}

LogSink Logger::standardErrLogger() const {
  std::shared_lock l{m_mutex};

  return m_standardErrLogger;
}

LoggerType& Logger::loggerFromChannel(const LogChannel& logChannel) {
  std::shared_lock l{m_mutex};

  auto it = m_loggerMap.find(logChannel);
  if (it == m_loggerMap.end()) {
    // Drop the read lock and grab a write lock - we need to add the new file to the map
    // this will reduce contention when multiple threads trying to log at once.
    l.unlock();
    std::unique_lock l2{m_mutex};

    //LoggerType newLogger(boost::log::keywords::channel = logChannel, boost::log::keywords::severity = Debug);
    auto [it, inserted] = m_loggerMap.try_emplace(logChannel, LoggerType(boost::log::keywords::channel = logChannel));

    return it->second;
  }

  return it->second;
}

bool Logger::findSink(boost::shared_ptr<LogSinkBackend> sink) {
  std::unique_lock l{m_mutex};

  auto it = m_sinks.find(sink);

  return (it != m_sinks.end());
}

void Logger::addSink(boost::shared_ptr<LogSinkBackend> sink) {
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

void Logger::removeSink(boost::shared_ptr<LogSinkBackend> sink) {
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

void Logger::addTimeStampToLogger() {
  std::unique_lock l{m_mutex};

  // Add a TimeStamp attribute, same as boost::log::add_common_attributes() would do
  [[maybe_unused]] auto [it, inserted] = boost::log::core::get()->add_global_attribute("TimeStamp", boost::log::attributes::local_clock{});
  // if (!use) {
  //   boost::log::core::get()->remove_global_attribute(it);
  // }
}

}  // namespace openstudio
