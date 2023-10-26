/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef UTILITIES_CORE_TEST_COREFIXTURE_HPP
#define UTILITIES_CORE_TEST_COREFIXTURE_HPP

#include "../Logger.hpp"
#include "../FileLogSink.hpp"

#include <resources.hxx>

#include <fstream>

#include <gtest/gtest.h>

class CoreFixture : public ::testing::Test
{
 protected:
  // initialize for each test
  virtual void SetUp() override {}

  // tear down after for each test
  virtual void TearDown() override {}

  // initialize static members
  static void SetUpTestSuite();

  // tear down static members
  static void TearDownTestSuite();

 public:
  REGISTER_LOGGER("CoreFixture");

  // Note: storage for static variables must be defined in a separate .cpp file
  static boost::optional<openstudio::FileLogSink> logFile;
};

#endif  //UTILITIES_CORE_TEST_COREFIXTURE_HPP
