/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "StringStreamLogSink.hpp"
#include "StringStreamLogSink_Impl.hpp"

#include "Assert.hpp"

namespace openstudio {

namespace detail {

  StringStreamLogSink_Impl::StringStreamLogSink_Impl() : m_stringstream(new std::stringstream) {
    this->setStream(m_stringstream);
    this->enable();
  }

  StringStreamLogSink_Impl::~StringStreamLogSink_Impl() {
    this->disable();

    // already called
    //LogSink_Impl::~LogSink_Impl();
  }

  std::string StringStreamLogSink_Impl::string() const {
    std::shared_lock l{m_mutex};

    return m_stringstream->str();
  }

  std::vector<LogMessage> StringStreamLogSink_Impl::logMessages() const {
    return LogMessage::parseLogText(this->string());
  }

  void StringStreamLogSink_Impl::resetStringStream() {
    std::unique_lock l{m_mutex};

    m_stringstream->str("");
  }

}  // namespace detail

StringStreamLogSink::StringStreamLogSink() : LogSink(boost::shared_ptr<detail::StringStreamLogSink_Impl>(new detail::StringStreamLogSink_Impl())) {
  OS_ASSERT(getImpl<detail::StringStreamLogSink_Impl>());
}

std::string StringStreamLogSink::string() const {
  return this->getImpl<detail::StringStreamLogSink_Impl>()->string();
}

std::vector<LogMessage> StringStreamLogSink::logMessages() const {
  return this->getImpl<detail::StringStreamLogSink_Impl>()->logMessages();
}

void StringStreamLogSink::resetStringStream() {
  this->getImpl<detail::StringStreamLogSink_Impl>()->resetStringStream();
}

}  // namespace openstudio
