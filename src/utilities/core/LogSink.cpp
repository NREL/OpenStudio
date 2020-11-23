/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2020, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
*  following conditions are met:
*
*  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
*  disclaimer.
*
*  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
*  disclaimer in the documentation and/or other materials provided with the distribution.
*
*  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote products
*  derived from this software without specific prior written permission from the respective party.
*
*  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative works
*  may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without specific prior
*  written permission from Alliance for Sustainable Energy, LLC.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER(S) AND ANY CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
*  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
*  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER(S), ANY CONTRIBUTORS, THE UNITED STATES GOVERNMENT, OR THE UNITED
*  STATES DEPARTMENT OF ENERGY, NOR ANY OF THEIR EMPLOYEES, BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
*  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
*  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
*  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
*  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
***********************************************************************************************************************/

#include "LogSink.hpp"
#include "LogSink_Impl.hpp"

#include "Logger.hpp"

#include <boost/log/support/regex.hpp>
#include <boost/log/expressions.hpp>

namespace sinks = boost::log::sinks;
namespace keywords = boost::log::keywords;
namespace expr = boost::log::expressions;

namespace openstudio {

namespace detail {

  LogSink_Impl::LogSink_Impl() : m_mutex{}, m_threadId{}, m_sink{boost::shared_ptr<LogSinkBackend>(new LogSinkBackend())} {}

  LogSink_Impl::~LogSink_Impl() {}

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

    // set formatting, seems like you have to call this after the stream is added
    // DLM@20110701: would like to format Severity as string but can't figure out how to do it
    // because you can't overload operator<< for an enum type
    // this seems to suggest this should work: http://www.edm2.com/0405/enumeration.html
    m_sink->set_formatter(expr::stream << "[" << expr::attr<LogChannel>("Channel") << "] <" << expr::attr<LogLevel>("Severity") << "> "
                                       << expr::smessage);

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

  void LogSink_Impl::updateFilter(const std::unique_lock<std::shared_mutex>& l) {
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

}  // namespace openstudio
