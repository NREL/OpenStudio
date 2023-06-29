/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "SDDFixture.hpp"

#include <resources.hxx>

void SDDFixture::SetUp() {}

void SDDFixture::TearDown() {}

void SDDFixture::SetUpTestSuite() {
  // set up logging
  openstudio::Logger::instance().standardOutLogger().disable();
  logFile = std::shared_ptr<openstudio::FileLogSink>(new openstudio::FileLogSink(openstudio::toPath("./SDDFixture.log")));
  logFile->setLogLevel(Info);
}

void SDDFixture::TearDownTestSuite() {
  logFile->disable();
}

std::shared_ptr<openstudio::FileLogSink> SDDFixture::logFile;
