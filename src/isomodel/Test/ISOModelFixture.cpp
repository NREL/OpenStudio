/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "ISOModelFixture.hpp"

#include <resources.hxx>

void ISOModelFixture::SetUp() {}

void ISOModelFixture::TearDown() {}

void ISOModelFixture::SetUpTestSuite() {
  // set up logging
  openstudio::Logger::instance().standardOutLogger().disable();
  logFile = std::shared_ptr<openstudio::FileLogSink>(new openstudio::FileLogSink(openstudio::toPath("./ISOModelFixture.log")));
}

void ISOModelFixture::TearDownTestSuite() {
  logFile->disable();
}

std::shared_ptr<openstudio::FileLogSink> ISOModelFixture::logFile;
