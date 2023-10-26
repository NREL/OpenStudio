/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef OSVERSION_TEST_OSVERSIONFIXTURE_HPP
#define OSVERSION_TEST_OSVERSIONFIXTURE_HPP

#include <gtest/gtest.h>

#include "../../utilities/core/Logger.hpp"
#include "../../utilities/core/FileLogSink.hpp"

#include <boost/optional.hpp>

namespace openstudio {
class VersionString;
class IddFile;
}  // namespace openstudio

class OSVersionFixture : public ::testing::Test
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

  static openstudio::path versionResourcesPath(const openstudio::VersionString& version);

  static openstudio::path exampleModelPath(const openstudio::VersionString& version);

  static openstudio::path exampleComponentPath(const openstudio::VersionString& version);

  static openstudio::path iddPath(const openstudio::VersionString& version);

  static openstudio::IddFile getOpenStudioIddFileForVersion(const openstudio::VersionString& version);

  REGISTER_LOGGER("OSVersionFixture");

  // static variables
  static boost::optional<openstudio::FileLogSink> logFile;
};

#endif  // OSVERSION_TEST_OSVERSIONFIXTURE_HPP
