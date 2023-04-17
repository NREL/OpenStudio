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
