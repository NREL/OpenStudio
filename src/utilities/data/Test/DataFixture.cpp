/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "DataFixture.hpp"

using openstudio::Logger;
using openstudio::FileLogSink;
using openstudio::toPath;

// initialize for each test
void DataFixture::SetUp() {}

// tear down after for each test
void DataFixture::TearDown() {}

// initialize static members
void DataFixture::SetUpTestSuite() {
  logFile = FileLogSink(toPath("./DataFixture.log"));
  logFile->setLogLevel(Info);
}

// tear down static members
void DataFixture::TearDownTestSuite() {
  logFile->disable();
}

boost::optional<openstudio::FileLogSink> DataFixture::logFile;
