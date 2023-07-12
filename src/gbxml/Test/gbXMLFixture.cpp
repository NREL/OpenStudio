/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "gbXMLFixture.hpp"

#include <resources.hxx>

void gbXMLFixture::SetUp() {}

void gbXMLFixture::TearDown() {}

void gbXMLFixture::SetUpTestSuite() {
  // set up logging
  openstudio::Logger::instance().standardOutLogger().disable();
  logFile = std::shared_ptr<openstudio::FileLogSink>(new openstudio::FileLogSink(openstudio::toPath("./gbXMLFixture.log")));
}

void gbXMLFixture::TearDownTestSuite() {
  logFile->disable();
}

std::shared_ptr<openstudio::FileLogSink> gbXMLFixture::logFile;
