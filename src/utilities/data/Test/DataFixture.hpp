/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef UTILITIES_DATA_TEST_DATAFIXTURE_HPP
#define UTILITIES_DATA_TEST_DATAFIXTURE_HPP

#include <gtest/gtest.h>

#include "../../core/Logger.hpp"
#include "../../core/FileLogSink.hpp"

class DataFixture : public ::testing::Test
{
 protected:
  // initialize for each test
  virtual void SetUp() override;

  // tear down after for each test
  virtual void TearDown() override;

  // initialize static members
  static void SetUpTestSuite();

  // tear down static members
  static void TearDownTestSuite();

 public:
  // set up logging
  REGISTER_LOGGER("DataFixture");

  static boost::optional<openstudio::FileLogSink> logFile;
};

#endif  // UTILITIES_DATA_TEST_DATAFIXTURE_HPP
