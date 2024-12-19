/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "SqlFileFixture.hpp"
#include <iomanip>
#include <cmath>

using openstudio::Logger;
using openstudio::FileLogSink;
using openstudio::toPath;

void SqlFileFixture::SetUp() {}

void SqlFileFixture::TearDown() {}

void SqlFileFixture::SetUpTestSuite() {
  logFile = FileLogSink(toPath("./SqlFileFixture.log"));
  logFile->setLogLevel(Debug);

  openstudio::path path;
  path = resourcesPath() / toPath("energyplus/5ZoneAirCooled/eplusout.sql");
  sqlFile = openstudio::SqlFile(path);
  ASSERT_TRUE(sqlFile.connectionOpen());

  openstudio::path path2;
  path2 = resourcesPath() / toPath("energyplus/Office_With_Many_HVAC_Types/eplusout.sql");
  sqlFile2 = openstudio::SqlFile(path2);
  ASSERT_TRUE(sqlFile2.connectionOpen());

  openstudio::path path3;
  path3 = resourcesPath() / toPath("energyplus/AllFuelTypes/eplusout.sql");
  sqlFile3 = openstudio::SqlFile(path3);
  ASSERT_TRUE(sqlFile3.connectionOpen());
}

void SqlFileFixture::TearDownTestSuite() {
  logFile->disable();
}

::testing::AssertionResult SqlFileFixture::IsWithinRelativeTolerance(double expectedValue, double actualValue, double tolerance) {
  double percentageDifference = (actualValue - expectedValue) / expectedValue;
  if (std::fabs(percentageDifference) <= tolerance) {
    return ::testing::AssertionSuccess();
  } else {
    // Google Test seems to be ingoring the fixed && precision iomanips
    return ::testing::AssertionFailure() << "Value isn't within the required tolerance of " << std::fixed << std::setprecision(2) << (tolerance * 100)
                                         << "%. " << "Expected Value = " << expectedValue << ", Sql Value = " << actualValue
                                         << ", Difference = " << (100 * percentageDifference) << "%.";
  }
}

// define static storage
openstudio::SqlFile SqlFileFixture::sqlFile;
openstudio::SqlFile SqlFileFixture::sqlFile2;
openstudio::SqlFile SqlFileFixture::sqlFile3;
boost::optional<openstudio::FileLogSink> SqlFileFixture::logFile;
