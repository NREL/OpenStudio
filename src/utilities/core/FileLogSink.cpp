/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "FileLogSink.hpp"
#include "FileLogSink_Impl.hpp"

#include "Assert.hpp"

#include <shared_mutex>

namespace openstudio {

namespace detail {

  FileLogSink_Impl::FileLogSink_Impl(const openstudio::path& path)
    : m_path{path}, m_ofs{boost::shared_ptr<openstudio::filesystem::ofstream>(new openstudio::filesystem::ofstream(path))} {
    this->setStream(m_ofs);
    this->enable();
  }

  FileLogSink_Impl::~FileLogSink_Impl() {
    // already called
    //LogSink_Impl::~LogSink_Impl();
  }

  openstudio::path FileLogSink_Impl::path() const {
    std::shared_lock l{m_mutex};

    return m_path;
  }

  std::vector<LogMessage> FileLogSink_Impl::logMessages() const {
    openstudio::filesystem::ifstream ifs(m_path);
    std::string line;
    std::string text;
    while (std::getline(ifs, line)) {
      text += line + "\n";
    }
    return LogMessage::parseLogText(text);
  }
}  // namespace detail

FileLogSink::FileLogSink(const openstudio::path& path) : LogSink(boost::shared_ptr<detail::FileLogSink_Impl>(new detail::FileLogSink_Impl(path))) {
  OS_ASSERT(getImpl<detail::FileLogSink_Impl>());
}

openstudio::path FileLogSink::path() const {
  return this->getImpl<detail::FileLogSink_Impl>()->path();
}

std::vector<LogMessage> FileLogSink::logMessages() const {
  return this->getImpl<detail::FileLogSink_Impl>()->logMessages();
}

}  // namespace openstudio
