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

#include "ErrorFile.hpp"

#include <boost/regex.hpp>
#include <boost/algorithm/string.hpp>

namespace openstudio {
namespace energyplus {

  /// constructor
  ErrorFile::ErrorFile(const openstudio::path& errPath) : m_completed(false), m_completedSuccessfully(false) {
    openstudio::filesystem::ifstream ifs(errPath);
    parse(ifs);
    ifs.close();
  }

  /// get warnings
  std::vector<std::string> ErrorFile::warnings() const {
    return m_warnings;
  }

  /// get severe errors
  std::vector<std::string> ErrorFile::severeErrors() const {
    return m_severeErrors;
  }

  /// get fatal errors
  std::vector<std::string> ErrorFile::fatalErrors() const {
    return m_fatalErrors;
  }

  /// did EnergyPlus complete or crash
  bool ErrorFile::completed() const {
    return m_completed;
  }

  /// completed successfully
  bool ErrorFile::completedSuccessfully() const {
    return m_completedSuccessfully;
  }

  void ErrorFile::parse(openstudio::filesystem::ifstream& is) {
    std::string line;

    // matches[1], warning/error type
    // matches[2], rest of line
    boost::regex warningOrError("^\\s*\\**\\s+\\*\\*\\s*([[:alpha:]]+)\\s*\\*\\*(.*)$");

    // matches[1], rest of line
    boost::regex warningOrErrorContinue("^\\s*\\**\\s+\\*\\*\\s*~~~\\s*\\*\\*(.*)$");

    // completed successfully
    boost::regex completedSuccessful("^\\s*\\*+ EnergyPlus Completed Successfully.*");

    // ground temp completed successfully
    boost::regex groundTempCompletedSuccessful("^\\s*\\*+ GroundTempCalc\\S* Completed Successfully.*");

    // completed unsuccessfully
    boost::regex completedUnsuccessful("^\\s*\\*+ EnergyPlus Terminated.*");

    // repeat count

    // read the file line by line using regexes
    bool alreadyGotLine = false;

    while (alreadyGotLine || std::getline(is, line)) {

      alreadyGotLine = false;
      boost::smatch matches;

      LOG(Debug, "Parsing ErrorFile Line '" << line << "'");

      // parse the file
      if (boost::regex_search(line, matches, warningOrError)) {

        std::string warningOrErrorType = std::string(matches[1].first, matches[1].second);
        boost::trim(warningOrErrorType);
        std::string warningOrErrorString = std::string(matches[2].first, matches[2].second);
        boost::trim(warningOrErrorString);

        // read the rest of the multi line warning or error
        while (true) {
          // std::streampos pos = is.tellg();
          if (!std::getline(is, line)) {
            break;
          }
          if (boost::regex_search(line, matches, warningOrErrorContinue)) {
            std::string temp = std::string(matches[1].first, matches[1].second);
            boost::trim_right(temp);
            warningOrErrorString += "\n" + temp;
          } else {
            // unget the line
            // is.seekg(pos);
            // Instead of rewind then reread (which fails on Windows if you have LF line endings)
            // We just use this bool to avoid having to re-read the line.
            alreadyGotLine = true;
            break;
          }
        }

        LOG(Trace, "Error parsed: " << warningOrErrorString);

        // correctly sort warnings and errors
        try {
          ErrorLevel level(warningOrErrorType);

          switch (level.value()) {
            case ErrorLevel::Warning:
              m_warnings.push_back(warningOrErrorString);
              break;
            case ErrorLevel::Severe:
              m_severeErrors.push_back(warningOrErrorString);
              break;
            case ErrorLevel::Fatal:
              m_fatalErrors.push_back(warningOrErrorString);
              break;
          }

        } catch (...) {
          LOG(Error, "Unknown warning or error level '" << warningOrErrorType << "'");
        }

      } else if (boost::regex_match(line, completedSuccessful) || boost::regex_match(line, groundTempCompletedSuccessful)) {
        m_completed = true;
        m_completedSuccessfully = true;
        break;
      } else if (boost::regex_match(line, completedUnsuccessful)) {
        m_completed = true;
        m_completedSuccessfully = false;
        break;
      }
    }
  }

}  // namespace energyplus
}  // namespace openstudio
