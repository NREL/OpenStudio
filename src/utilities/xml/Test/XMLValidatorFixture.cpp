/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "XMLValidatorFixture.hpp"

#include "../../core/ApplicationPathHelpers.hpp"

using openstudio::Logger;
using openstudio::toPath;
using openstudio::FileLogSink;

void XMLValidatorFixture::SetUp() {}

void XMLValidatorFixture::TearDown() {}

void XMLValidatorFixture::SetUpTestSuite() {
  logFile = FileLogSink(toPath("./XMLValidatorFixture.log"));
  logFile->setLogLevel(Debug);
  openstudio::Logger::instance().standardOutLogger().disable();
}

void XMLValidatorFixture::TearDownTestSuite() {
  logFile->disable();
}

// define static storage
openstudio::path XMLValidatorFixture::schemaPath;
boost::optional<openstudio::FileLogSink> XMLValidatorFixture::logFile;
