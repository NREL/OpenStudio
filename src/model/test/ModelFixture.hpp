/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_TEST_MODELFIXTURE_HPP
#define MODEL_TEST_MODELFIXTURE_HPP

#include <gtest/gtest.h>

#include <resources.hxx>

#include "../Model.hpp"

#include "../../utilities/idd/IddField.hpp"
#include "../../utilities/sql/SqlFile.hpp"
#include "../../utilities/core/Logger.hpp"
#include "../../utilities/core/FileLogSink.hpp"
#include "../../utilities/data/DataEnums.hpp"

class ModelFixture : public ::testing::Test
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

  // test that the given iddField is a boolean Yes/No choice field
  void testBooleanIddField(const openstudio::IddField& iddField, const boost::optional<std::string>& defaultValue);

  bool testFuelTypeEquality(const std::vector<openstudio::FuelType>& expectedFuelTypes, const std::vector<openstudio::FuelType>& actualFuelTypes);

  bool testAppGFuelTypeEquality(const std::vector<openstudio::AppGFuelType>& expectedAppGFuelTypes,
                                const std::vector<openstudio::AppGFuelType>& actualAppGFuelTypes);

  // set up logging
  REGISTER_LOGGER("ModelFixture");

  // Note: storage for static variables must be defined in a separate .cpp file
  static double tol;
  static boost::optional<openstudio::FileLogSink> logFile;
};

#endif  // MODEL_TEST_MODELFIXTURE_HPP
