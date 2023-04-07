/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2023, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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
}

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

 protected:
  friend class LoggerSingleton;

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
