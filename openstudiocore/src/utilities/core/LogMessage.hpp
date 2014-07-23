/**********************************************************************
*  Copyright (c) 2008-2014, Alliance for Sustainable Energy.
*  All rights reserved.
*
*  This library is free software; you can redistribute it and/or
*  modify it under the terms of the GNU Lesser General Public
*  License as published by the Free Software Foundation; either
*  version 2.1 of the License, or (at your option) any later version.
*
*  This library is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
*  Lesser General Public License for more details.
*
*  You should have received a copy of the GNU Lesser General Public
*  License along with this library; if not, write to the Free Software
*  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
**********************************************************************/

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
enum LogLevel{Trace = -3, Debug = -2, Info = -1, Warn = 0, Error = 1, Fatal = 2};

namespace openstudio{

  /// LogChannel identifies a logger
  typedef std::string LogChannel;

  /// Type of stream sink used
  typedef boost::log::sinks::synchronous_sink<boost::log::sinks::text_ostream_backend> LogSinkBackend;

  /// Type of logger used
  typedef boost::log::sources::severity_channel_logger_mt<LogLevel> LoggerType;

  /// LogMessage encapsulates a single logging message
  class UTILITIES_API LogMessage
  {
    public:

      /// constructor
      LogMessage(LogLevel logLevel, const std::string& channel, const std::string& message);

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

} // openstudio

#endif // UTILITIES_CORE_LOGMESSAGE_HPP
