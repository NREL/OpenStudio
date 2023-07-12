/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "IddFixture.hpp"
#include <utilities/idd/IddEnums.hxx>

using openstudio::FileLogSink;
using openstudio::toPath;

void IddFixture::SetUpTestSuite() {
  // set up logging
  logFile = FileLogSink(toPath("./IddFixture.log"));
  logFile->setLogLevel(Debug);

  // load from factory and time it
  openstudio::Time start = openstudio::Time::currentTime();
  epIddFile = openstudio::IddFactory::instance().getIddFile(openstudio::IddFileType::EnergyPlus);
  iddLoadTime = openstudio::Time::currentTime() - start;

  LOG(Info, "EnergyPlus IddFile load time (from IddFactory) = " << iddLoadTime);

  start = openstudio::Time::currentTime();
  osIddFile = openstudio::IddFactory::instance().getIddFile(openstudio::IddFileType::OpenStudio);
  iddLoadTime = openstudio::Time::currentTime() - start;

  LOG(Info, "OpenStudio IddFile load time (from IddFactory) = " << iddLoadTime);
}

void IddFixture::TearDownTestSuite() {
  logFile->disable();
}

openstudio::IddFile IddFixture::epIddFile;
openstudio::IddFile IddFixture::osIddFile;
openstudio::Time IddFixture::iddLoadTime;
boost::optional<openstudio::FileLogSink> IddFixture::logFile;
