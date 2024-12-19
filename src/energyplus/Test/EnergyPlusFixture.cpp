/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
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
  }  // foreach component
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
