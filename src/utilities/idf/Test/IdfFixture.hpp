/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef UTILITIES_IDF_TEST_IDFFIXTURE_HPP
#define UTILITIES_IDF_TEST_IDFFIXTURE_HPP

#include <gtest/gtest.h>

#include <resources.hxx>

#include "../IdfFile.hpp"
#include "../Workspace.hpp"
#include "../ImfFile.hpp"

#include "../../core/Logger.hpp"
#include "../../core/FileLogSink.hpp"

class IdfFixture : public ::testing::Test
{
 protected:
  // initialize for each test
  virtual void SetUp() override;

  // tear down after each test
  virtual void TearDown() override;

  // initialize static members
  static void SetUpTestSuite();

  // tear down static members
  static void TearDownTestSuite();

  // set up logging
  REGISTER_LOGGER("IdfFixture");

  // Note: storage for static variables must be defined in a separate .cpp file
  static double tol;
  static openstudio::path outDir;
  static openstudio::IdfFile epIdfFile;
  static openstudio::ImfFile imfFile;
  static boost::optional<openstudio::FileLogSink> logFile;

  virtual ~IdfFixture() = default;
};

#endif  // UTILITIES_IDF_TEST_IDFFIXTURE_HPP
