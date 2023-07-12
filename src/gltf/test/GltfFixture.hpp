/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef GLTF_TEST_GLTFFIXTURE_HPP
#define GLTF_TEST_GLTFFIXTURE_HPP

#include <gtest/gtest.h>

#include "../../utilities/core/Logger.hpp"
#include "../../utilities/core/FileLogSink.hpp"

#include <boost/optional.hpp>

class GltfFixture : public ::testing::Test
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

  REGISTER_LOGGER("GltfFixture");

  // static variables
  static boost::optional<openstudio::FileLogSink> logFile;
};

#endif  // GLTF_TEST_GLTFFIXTURE_HPP
