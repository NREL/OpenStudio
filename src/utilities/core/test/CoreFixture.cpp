/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "CoreFixture.hpp"

using openstudio::FileLogSink;
using openstudio::toPath;

void CoreFixture::SetUpTestSuite() {
  // set up logging
  logFile = FileLogSink(toPath("./CoreFixture.log"));
  logFile->setLogLevel(Info);
}

void CoreFixture::TearDownTestSuite() {
  logFile->disable();
}

boost::optional<openstudio::FileLogSink> CoreFixture::logFile;
