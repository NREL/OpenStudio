/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef UTILITIES_BCL_TEST_BCLFIXTURE_HPP
#define UTILITIES_BCL_TEST_BCLFIXTURE_HPP

#include <gtest/gtest.h>

#include <resources.hxx>

#include "../../core/Logger.hpp"
#include "../../core/FileLogSink.hpp"

#include <string>
#include <vector>

class BCLFixture : public ::testing::Test
{
 protected:
  // initialize for each test: create unique library path to call LocalBCL::instance(currentLocalBCLPath)
  virtual void SetUp() override;

  // tear down after each test: delete currentLocalBCLPath
  virtual void TearDown() override;

  // initialize static members
  static void SetUpTestSuite();

  // tear down static members
  static void TearDownTestSuite();

  // set up logging
  REGISTER_LOGGER("BCLFixture");

  static ::testing::AssertionResult checkLogMessagesContain(const std::vector<openstudio::LogMessage>& logMessages,
                                                            const std::vector<std::string>& searchStrings, bool use_regex = false);

 public:
  // This is assigned in SetUp, per test, so doesn't have to be static
  std::string prodAuthKey;
  std::string devAuthKey;

  // Note: storage for static variables must be defined in a separate .cpp file
  const static std::string defaultProdAuthKey;
  const static std::string defaultDevAuthKey;
  static boost::optional<openstudio::FileLogSink> logFile;

  // Unique path to the current test's BCL location
  openstudio::path currentLocalBCLPath;
};

#endif  // UTILITIES_BCL_TEST_BCLFIXTURE_HPP
