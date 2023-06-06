/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "ModelFixture.hpp"

#include "../../utilities/idd/IddKey.hpp"
#include "../../utilities/core/Finder.hpp"

#include <resources.hxx>

using openstudio::Logger;
using openstudio::FileLogSink;
using openstudio::toPath;

void ModelFixture::SetUp() {}

void ModelFixture::TearDown() {}

void ModelFixture::SetUpTestSuite() {
  // set up logging
  logFile = FileLogSink(toPath("./ModelTestFixture.log"));
  logFile->setLogLevel(Debug);
  Logger::instance().standardOutLogger().disable();

  // create resources folder for scratch files
  openstudio::filesystem::create_directory(resourcesPath() / toPath("model"));
}

void ModelFixture::TearDownTestSuite() {
  logFile->disable();
}

void ModelFixture::testBooleanIddField(const openstudio::IddField& iddField, const boost::optional<std::string>& defaultValue) {
  EXPECT_TRUE(iddField.properties().type == openstudio::IddFieldType::ChoiceType);
  openstudio::IddKeyVector keys = iddField.keys();
  EXPECT_EQ(2u, keys.size());
  openstudio::NameFinder<openstudio::IddKey> finder("Yes");
  auto it = std::find_if(keys.begin(), keys.end(), finder);
  EXPECT_FALSE(it == keys.end());
  finder = openstudio::NameFinder<openstudio::IddKey>("No");
  it = std::find_if(keys.begin(), keys.end(), finder);
  EXPECT_FALSE(it == keys.end());
  if (defaultValue) {
    ASSERT_TRUE(iddField.properties().stringDefault);
    EXPECT_EQ(*defaultValue, iddField.properties().stringDefault.get());
  }
}

bool ModelFixture::testFuelTypeEquality(const std::vector<openstudio::FuelType>& expectedFuelTypes,
                                        const std::vector<openstudio::FuelType>& actualFuelTypes) {
  EXPECT_EQ(expectedFuelTypes, actualFuelTypes);
  return expectedFuelTypes == actualFuelTypes;
}

bool ModelFixture::testAppGFuelTypeEquality(const std::vector<openstudio::AppGFuelType>& expectedAppGFuelTypes,
                                            const std::vector<openstudio::AppGFuelType>& actualAppGFuelTypes) {
  EXPECT_EQ(expectedAppGFuelTypes, actualAppGFuelTypes);
  return expectedAppGFuelTypes == actualAppGFuelTypes;
}

double ModelFixture::tol(1.0E-5);
boost::optional<openstudio::FileLogSink> ModelFixture::logFile;
