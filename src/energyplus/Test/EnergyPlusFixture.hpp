/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef ENERGYPLUS_TEST_ENERGYPLUSFIXTURE_HPP
#define ENERGYPLUS_TEST_ENERGYPLUSFIXTURE_HPP

#include <gtest/gtest.h>

#include "../../utilities/core/Logger.hpp"
#include "../../utilities/core/FileLogSink.hpp"
#include "../../utilities/core/Path.hpp"

#include <vector>
#include <string>

class EnergyPlusFixture : public ::testing::Test
{
 protected:
  /// initialize for each test
  virtual void SetUp() override;

  /// tear down after each test
  virtual void TearDown() override;

  /// initialize static members
  static void SetUpTestSuite();

  /// tear down static members
  static void TearDownTestSuite();

  REGISTER_LOGGER("EnergyPlusFixture");

  static ::testing::AssertionResult checkLogMessagesSizeWithExclusions(int size, const std::vector<openstudio::LogMessage>& logMessages,
                                                                       const std::vector<std::string>& exclusions = {}, bool use_regex = false);

  static ::testing::AssertionResult checkLogMessagesContain(const std::vector<openstudio::LogMessage>& logMessages,
                                                            const std::vector<std::string>& searchStrings, bool use_regex = false);

  // static variables
  static boost::optional<openstudio::FileLogSink> logFile;

  using ComponentDirectoryAndType = std::pair<openstudio::path, std::string>;
  static std::vector<ComponentDirectoryAndType> idfComponents;
};

#endif  // ENERGYPLUS_TEST_ENERGYPLUSFIXTURE_HPP
