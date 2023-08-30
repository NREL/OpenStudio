/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef SDD_TEST_SDDFIXTURE_HPP
#define SDD_TEST_SDDFIXTURE_HPP

#include <gtest/gtest.h>

#include "../../utilities/core/Logger.hpp"
#include "../../utilities/core/FileLogSink.hpp"
#include "../../utilities/core/Path.hpp"

#include <vector>
#include <string>

class SDDFixture : public ::testing::Test
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

  static std::shared_ptr<openstudio::FileLogSink> logFile;

  REGISTER_LOGGER("sdd");
};

#endif  // SDD_TEST_SDDFIXTURE_HPP
