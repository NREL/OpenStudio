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

#include "LogMessage.hpp"

#include <boost/regex.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>

namespace openstudio{

  LogMessage::LogMessage(LogLevel logLevel, const LogChannel& logChannel, const std::string& logMessage)
    : m_logLevel(logLevel), m_logChannel(logChannel), m_logMessage(logMessage)
  {}

  std::vector<LogMessage> LogMessage::parseLogText(const std::string& logText)
  {
    std::vector<LogMessage> result;

    std::string text = logText;

    // get preceding comments
    boost::smatch matches;
    boost::regex messageRegex("\\[([^\\]]*)\\]\\s*<([\\-0-9]+)>([^\\[]*)(.*)");
    while(boost::regex_search(text, matches, messageRegex)){
      std::string channel(matches[1].first, matches[1].second); boost::trim(channel);
      std::string levelString(matches[2].first, matches[2].second); boost::trim(levelString);
      std::string message(matches[3].first, matches[3].second); boost::trim(message);
      std::string remainingText(matches[4].first, matches[4].second); boost::trim(remainingText);

      int level = boost::lexical_cast<int>(levelString);

      result.push_back(LogMessage((LogLevel)level, channel, message));

      // reduce the parsed text
      text = remainingText;
    }

    return result;
  }

  LogLevel LogMessage::logLevel() const
  {
    return m_logLevel;
  }

  LogChannel LogMessage::logChannel() const
  {
    return m_logChannel;
  }

  std::string LogMessage::logMessage() const
  {
    return m_logMessage;
  }

} // openstudio
