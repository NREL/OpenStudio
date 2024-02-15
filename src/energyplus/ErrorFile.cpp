/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
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
    boost::regex warningOrError(R"(^\s*\**\s+\*\*\s*([[:alpha:]]+)\s*\*\*(.*)$)");

    // matches[1], rest of line
    boost::regex warningOrErrorContinue(R"(^\s*\**\s+\*\*\s*~~~\s*\*\*(.*)$)");

    // completed successfully
    boost::regex completedSuccessful(R"(^\s*\*+ EnergyPlus Completed Successfully.*)");

    // ground temp completed successfully
    boost::regex groundTempCompletedSuccessful(R"(^\s*\*+ GroundTempCalc\S* Completed Successfully.*)");

    // completed unsuccessfully
    boost::regex completedUnsuccessful(R"(^\s*\*+ EnergyPlus Terminated.*)");

    // repeat count

    // read the file line by line using regexes
    bool alreadyGotLine = false;

    while (alreadyGotLine || std::getline(is, line)) {

      alreadyGotLine = false;
      boost::smatch matches;

      // LOG(Trace, "Parsing ErrorFile Line '" << line << "'");

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

        // LOG(Trace, "Error parsed: " << warningOrErrorString);

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
          LOG(Error, "Unknown warning or error level '" << warningOrErrorType << "' for line '" << line << "'");
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
