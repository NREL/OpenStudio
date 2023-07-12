/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef UTILITIES_SQL_TEST_SQLFILEFIXTURE_HPP
#define UTILITIES_SQL_TEST_SQLFILEFIXTURE_HPP

#include <gtest/gtest.h>

#include "../SqlFile.hpp"
#include "../../core/Path.hpp"
#include "../../core/FileLogSink.hpp"

#include <resources.hxx>

class SqlFileFixture : public ::testing::Test
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

  // Tolerance defaults to 0.001=0.1%
  ::testing::AssertionResult IsWithinRelativeTolerance(double expectedValue, double actualValue, double tolerance = 0.001);

  // set up logging
  REGISTER_LOGGER("SqlFileFixture");

 public:
  // sql files
  static openstudio::SqlFile sqlFile;
  static openstudio::SqlFile sqlFile2;
  static openstudio::SqlFile sqlFile3;
  static boost::optional<openstudio::FileLogSink> logFile;
};

#endif  // UTILITIES_SQL_TEST_SQLFILEFIXTURE_HPP
