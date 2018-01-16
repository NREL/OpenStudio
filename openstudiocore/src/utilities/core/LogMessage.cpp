/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2018, Alliance for Sustainable Energy, LLC. All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 *  following conditions are met:
 *
 *  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
 *  disclaimer.
 *
 *  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *  following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 *  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote
 *  products derived from this software without specific prior written permission from the respective party.
 *
 *  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative
 *  works may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without
 *  specific prior written permission from Alliance for Sustainable Energy, LLC.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER, THE UNITED STATES GOVERNMENT, OR ANY CONTRIBUTORS BE LIABLE FOR
 *  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **********************************************************************************************************************/

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
