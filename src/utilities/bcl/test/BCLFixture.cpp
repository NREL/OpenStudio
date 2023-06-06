/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "BCLFixture.hpp"

#include "../LocalBCL.hpp"
#include "../../core/ApplicationPathHelpers.hpp"

#include <algorithm>
#include <regex>

using openstudio::LocalBCL;
using openstudio::Logger;
using openstudio::toPath;
using openstudio::FileLogSink;

void BCLFixture::SetUp() {

  // Use a unique libraryPath to avoid concurrent access issues when running tests in parallel
  // This gets the name of the test that's being run (eg 'RemoteBCLTest')
  std::string currentTestName = ::testing::UnitTest::GetInstance()->current_test_info()->name();
  currentLocalBCLPath = resourcesPath() / toPath("utilities/BCL") / toPath(currentTestName);

  // If for some reason (like CTRL+C) the previous pass didn't get cleaned up, do it
  try {
    openstudio::filesystem::remove_all(currentLocalBCLPath);
  } catch (...) {
  }

  // Initialize the LocalBCL Singleton at the given library path
  LocalBCL& bcl = LocalBCL::instance(currentLocalBCLPath);

  if (bcl.prodAuthKey().empty()) {
    prodAuthKey = defaultProdAuthKey;
    bcl.setProdAuthKey(prodAuthKey);
  } else {
    prodAuthKey = bcl.prodAuthKey();
  }

  // TODO Uncomment after network error handling is in place
  /*if (LocalBCL::instance().devAuthKey().empty()) {
    devAuthKey = defaultDevAuthKey;
    bcl.setDevAuthKey(devAuthKey);
  } else {
    devAuthKey = bcl.devAuthKey();
  }*/

  openstudio::path testDir = openstudio::filesystem::system_complete(openstudio::getApplicationBuildDirectory() / toPath("Testing"));
  if (!openstudio::filesystem::exists(testDir)) {
    openstudio::filesystem::create_directories(testDir);
  }
}

void BCLFixture::TearDown() {

  LocalBCL::close();
  try {
    openstudio::filesystem::remove_all(currentLocalBCLPath);
  } catch (...) {
  }
}

void BCLFixture::SetUpTestSuite() {
  // set up logging
  logFile = FileLogSink(toPath("./BCLFixture.log"));
  logFile->setLogLevel(Debug);
  Logger::instance().standardOutLogger().disable();
}

void BCLFixture::TearDownTestSuite() {
  logFile->disable();
}

::testing::AssertionResult BCLFixture::checkLogMessagesContain(const std::vector<openstudio::LogMessage>& logMessages,
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

// define static storage
boost::optional<openstudio::FileLogSink> BCLFixture::logFile;
// these are Dan's API keys labelled under "Testing", delete when there is a better way to do this
const std::string BCLFixture::defaultProdAuthKey("2da842aa2d457703d8fdcb5c53080ace");
const std::string BCLFixture::defaultDevAuthKey("e8051bca77787c0df16cbe13452e7580");
