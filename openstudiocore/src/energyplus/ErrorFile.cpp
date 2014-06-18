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

#include "ErrorFile.hpp"

#include <boost/regex.hpp>
#include <boost/algorithm/string.hpp>

namespace openstudio {
namespace energyplus {

  /// constructor
  ErrorFile::ErrorFile(const openstudio::path& errPath)
    : m_completed(false), m_completedSuccessfully(false)
  {
    boost::filesystem::ifstream ifs(errPath);
    parse(ifs);
    ifs.close();
  }

  /// get warnings
  std::vector<std::string> ErrorFile::warnings() const
  {
    return m_warnings;
  }

  /// get severe errors
  std::vector<std::string> ErrorFile::severeErrors() const
  {
    return m_severeErrors;
  }

  /// get fatal errors
  std::vector<std::string> ErrorFile::fatalErrors() const
  {
    return m_fatalErrors;
  }


  /// did EnergyPlus complete or crash
  bool ErrorFile::completed() const
  {
    return m_completed;
  }

  /// completed successfully
  bool ErrorFile::completedSuccessfully() const
  {
    return m_completedSuccessfully;
  }

  void ErrorFile::parse(boost::filesystem::ifstream& is)
  {
    std::string line;


    // matches[1], warning/error type
    // matches[2], rest of line
    boost::regex warningOrError("^\\s*\\**\\s+\\*\\*\\s*([^\\s\\*]+)\\s*\\*\\*(.*)$");

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
    while(std::getline(is, line)){

      boost::smatch matches;

//      LOG(Debug, "Parsing ErrorFile Line: " << line);


      // parse the file
      if (boost::regex_search(line, matches, warningOrError)){
        std::string warningOrErrorType = std::string(matches[1].first, matches[1].second); boost::trim(warningOrErrorType);
        std::string warningOrErrorString = std::string(matches[2].first, matches[2].second); boost::trim(warningOrErrorString);

        // read the rest of the multi line warning or error
        while(true){
          std::streampos pos = is.tellg();
          if (!std::getline(is, line)){
            break;
          }
          if (boost::regex_search(line, matches, warningOrErrorContinue)){
            std::string temp = std::string(matches[1].first, matches[1].second); boost::trim_right(temp);
            warningOrErrorString += "\n" + temp;
          }else{
            // unget the line
            is.seekg(pos);
            break;
          }
        }

        LOG(Trace, "Error parsed: " << warningOrErrorString);


        // correctly sort warnings and errors
        try{
          ErrorLevel level(warningOrErrorType);

          switch(level.value()){
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

        }catch(...){
          LOG(Error, "Unknown warning or error level '" << warningOrErrorType << "'");
        }

      }else if (boost::regex_match(line, completedSuccessful) 
                || boost::regex_match(line, groundTempCompletedSuccessful)) {
        m_completed = true;
        m_completedSuccessfully = true;
        break;
      }else if (boost::regex_match(line, completedUnsuccessful)){
        m_completed = true;
        m_completedSuccessfully = false;
        break;
      }
    }

  }

} // energyplus
} // openstudio
