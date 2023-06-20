/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "GltfFixture.hpp"

#include <utilities/idd/IddFactory.hxx>
#include "../../utilities/core/Compare.hpp"

#include <resources.hxx>
#include <OpenStudio.hxx>

using namespace openstudio;

void GltfFixture::SetUp() {}

void GltfFixture::TearDown() {}

void GltfFixture::SetUpTestSuite() {
  // set up logging
  logFile = FileLogSink(toPath("./GltfFixture.log"));
  logFile->setLogLevel(Debug);
  Logger::instance().standardOutLogger().disable();
}

void GltfFixture::TearDownTestSuite() {
  logFile->disable();
}

// static variables
boost::optional<openstudio::FileLogSink> GltfFixture::logFile;
