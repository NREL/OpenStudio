/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef UTILITIES_IDD_TEST_IDDFIXTURE_HPP
#define UTILITIES_IDD_TEST_IDDFIXTURE_HPP

#include "../IddFile.hpp"
#include <utilities/idd/IddFactory.hxx>
#include "../../core/Logger.hpp"
#include "../../core/FileLogSink.hpp"
#include "../../time/Time.hpp"

#include <resources.hxx>

#include <fstream>

#include <gtest/gtest.h>

class IddFixture : public ::testing::Test
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
  REGISTER_LOGGER("IddFixture");

  // Note: storage for static variables must be defined in a separate .cpp file
  static openstudio::IddFile epIddFile;
  static openstudio::IddFile osIddFile;
  static openstudio::Time iddLoadTime;
  static boost::optional<openstudio::FileLogSink> logFile;
};

#endif  // UTILITIES_IDD_TEST_IDDFIXTURE_HPP
