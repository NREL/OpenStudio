/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "AirflowFixture.hpp"

#include <resources.hxx>

void AirflowFixture::SetUp() {}

void AirflowFixture::TearDown() {}

void AirflowFixture::SetUpTestSuite() {
  // set up logging
  openstudio::Logger::instance().standardOutLogger().disable();
  //openstudio::Logger::instance().standardOutLogger().enable();
  //openstudio::Logger::instance().standardOutLogger().setLogLevel(Debug);
  logFile = std::shared_ptr<openstudio::FileLogSink>(new openstudio::FileLogSink(openstudio::toPath("./AirflowFixture.log")));
}

void AirflowFixture::TearDownTestSuite() {
  logFile->disable();
}

std::shared_ptr<openstudio::FileLogSink> AirflowFixture::logFile;
