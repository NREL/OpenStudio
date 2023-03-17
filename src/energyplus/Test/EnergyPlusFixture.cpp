/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2022, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

#include "EnergyPlusFixture.hpp"

#include <resources.hxx>

#include <algorithm>
#include <regex>

using openstudio::FileLogSink;
using openstudio::toPath;

void EnergyPlusFixture::SetUp() {}

void EnergyPlusFixture::TearDown() {}

void EnergyPlusFixture::SetUpTestSuite() {
  // set up logging
  logFile = FileLogSink(toPath("./EnergyPlusFixture.log"));
  logFile->setLogLevel(Trace);
  openstudio::Logger::instance().standardOutLogger().disable();

  // initialize component paths
  openstudio::path basePath = resourcesPath() / openstudio::toPath("energyplus/Components/");
  // idfComponents consists of .first = path to directory, .second = component type
  idfComponents.push_back(std::pair<openstudio::path, std::string>(basePath / openstudio::toPath("idf_roof_1"), "roof"));
  idfComponents.push_back(std::pair<openstudio::path, std::string>(basePath / openstudio::toPath("idf_roof_2"), "roof"));
  idfComponents.push_back(std::pair<openstudio::path, std::string>(basePath / openstudio::toPath("idf_roof_3"), "roof"));
  idfComponents.push_back(std::pair<openstudio::path, std::string>(basePath / openstudio::toPath("idf_roof_4"), "roof"));
  idfComponents.push_back(std::pair<openstudio::path, std::string>(basePath / openstudio::toPath("idf_roof_5"), "roof"));
  idfComponents.push_back(std::pair<openstudio::path, std::string>(basePath / openstudio::toPath("idf_designday_1"), "designday"));
  idfComponents.push_back(std::pair<openstudio::path, std::string>(basePath / openstudio::toPath("idf_designday_2"), "designday"));
  idfComponents.push_back(std::pair<openstudio::path, std::string>(basePath / openstudio::toPath("idf_designday_3"), "designday"));
  idfComponents.push_back(std::pair<openstudio::path, std::string>(basePath / openstudio::toPath("idf_designday_4"), "designday"));
  idfComponents.push_back(std::pair<openstudio::path, std::string>(basePath / openstudio::toPath("idf_designday_5"), "designday"));

  // delete translated components
  for (const ComponentDirectoryAndType& idfComponent : idfComponents) {
    // delete any *.osc and oscomponent.xml files in the directory
    for (openstudio::filesystem::directory_iterator it(idfComponent.first), itEnd; it != itEnd; ++it) {
      if (openstudio::filesystem::is_regular_file(it->status())) {
        std::string ext = openstudio::toString(openstudio::filesystem::extension(*it));
        if (ext == ".osc") {
          openstudio::filesystem::remove(it->path());
        }
        if ((ext == ".xml") && (openstudio::toString(it->path().filename()) == "oscomponent")) {
          openstudio::filesystem::remove(it->path());
        }
      }
    }  // for iterator over directory
  }    // foreach component
}

void EnergyPlusFixture::TearDownTestSuite() {
  logFile->disable();
}

::testing::AssertionResult EnergyPlusFixture::checkLogMessagesSizeWithExclusions(int size, const std::vector<openstudio::LogMessage>& logMessages,
                                                                                 const std::vector<std::string>& exclusions, bool use_regex) {
  std::stringstream ss;
  int filtered_size = 0;

  std::vector<std::string> logStrings;
  std::transform(logMessages.cbegin(), logMessages.cend(), std::back_inserter(logStrings),
                 [](const auto& logMessage) { return logMessage.logMessage(); });

  for (const auto& logString : logStrings) {
    bool is_excluded = false;
    for (const std::string& exclusion : exclusions) {
      if (use_regex) {
        is_excluded = std::regex_match(logString, std::regex(exclusion));
      } else {
        is_excluded = logString.find(exclusion) != std::string::npos;
      }
      if (is_excluded) {
        break;
      }
    }
    if (!is_excluded) {
      ++filtered_size;
      ss << " * " << filtered_size << ": '" << logString << "'\n";
    }
  }

  if (size == filtered_size) {
    return ::testing::AssertionSuccess();
  } else {
    return ::testing::AssertionFailure() << "Expected " << size << " warnings but got " << filtered_size << ":\n" << ss.str();
  }
}

::testing::AssertionResult EnergyPlusFixture::checkLogMessagesContain(const std::vector<openstudio::LogMessage>& logMessages,
                                                                      const std::vector<std::string>& searchStrings, bool use_regex) {

  std::vector<std::string> logStrings;
  std::transform(logMessages.cbegin(), logMessages.cend(), std::back_inserter(logStrings),
                 [](const auto& logMessage) { return logMessage.logMessage(); });

  std::stringstream ss;
  int not_found = 0;

  for (const std::string& searchString : searchStrings) {
    bool match_found = false;
    for (const std::string& logString : logStrings) {
      if (use_regex) {
        match_found = std::regex_match(logString, std::regex(searchString));
      } else {
        match_found = logString.find(searchString) != std::string::npos;
      }
      if (match_found) {
        break;
      }
    }
    if (!match_found) {
      ++not_found;
      ss << " * " << not_found << ": '" << searchString << "'\n";
    }
  }

  if (not_found == 0) {
    return ::testing::AssertionSuccess();
  } else {
    return ::testing::AssertionFailure() << "Could not find " << not_found << " messages:\n" << ss.str();
  }
}

// static variables
boost::optional<openstudio::FileLogSink> EnergyPlusFixture::logFile;
std::vector<std::pair<openstudio::path, std::string>> EnergyPlusFixture::idfComponents;
