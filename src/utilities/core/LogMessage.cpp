/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "LogMessage.hpp"

#include <boost/regex.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>

namespace openstudio {

LogMessage::LogMessage(LogLevel logLevel, const LogChannel& logChannel, const std::string& logMessage)
  : m_logLevel(logLevel), m_logChannel(logChannel), m_logMessage(logMessage) {}

std::vector<LogMessage> LogMessage::parseLogText(const std::string& logText) {
  std::vector<LogMessage> result;

  std::string text = logText;

  // get preceding comments
  boost::smatch matches;
  boost::regex messageRegex("\\[([^\\]]*)\\]\\s*<([\\-0-9]+)>([^\\[]*)(.*)");
  while (boost::regex_search(text, matches, messageRegex)) {
    std::string channel(matches[1].first, matches[1].second);
    boost::trim(channel);
    std::string levelString(matches[2].first, matches[2].second);
    boost::trim(levelString);
    std::string message(matches[3].first, matches[3].second);
    boost::trim(message);
    std::string remainingText(matches[4].first, matches[4].second);
    boost::trim(remainingText);

    int level = boost::lexical_cast<int>(levelString);

    result.push_back(LogMessage((LogLevel)level, channel, message));

    // reduce the parsed text
    text = remainingText;
  }

  return result;
}

LogLevel LogMessage::logLevel() const {
  return m_logLevel;
}

LogChannel LogMessage::logChannel() const {
  return m_logChannel;
}

std::string LogMessage::logMessage() const {
  return m_logMessage;
}

}  // namespace openstudio
