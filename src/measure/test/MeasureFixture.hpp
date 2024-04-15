/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MEASURE_TEST_MEASUREFIXTURE_HPP
#define MEASURE_TEST_MEASUREFIXTURE_HPP

#include <gtest/gtest.h>

#include "../../utilities/core/Logger.hpp"
#include "../../utilities/core/FileLogSink.hpp"
#include "../../utilities/core/Path.hpp"

#include <boost/optional.hpp>

#include <resources.hxx>

class MeasureFixture : public ::testing::Test
{
 protected:
  // TEST FIXTURE METHODS

  /// initialize for each test
  virtual void SetUp() override;

  /// tear down after each test
  virtual void TearDown() override;

  /// initialize static members
  static void SetUpTestSuite();

  /// tear down static members
  static void TearDownTestSuite();

  /// set up logging
  REGISTER_LOGGER("MeasureFixture");

  /// static data
  static boost::optional<openstudio::FileLogSink> logFile;

  static openstudio::path scratchDir;

  virtual ~MeasureFixture() = default;
};

#endif  // MEASURE_TEST_MEASUREFIXTURE_HPP
