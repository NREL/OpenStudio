/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "IdfFixture.hpp"
#include <utilities/idd/IddEnums.hxx>

#include "../../time/Time.hpp"

using openstudio::Logger;
using openstudio::FileLogSink;
using openstudio::toPath;

void IdfFixture::SetUp() {}

void IdfFixture::TearDown() {}

void IdfFixture::SetUpTestSuite() {
  // set up logging
  logFile = FileLogSink(toPath("./IdfFixture.log"));
  logFile->setLogLevel(Info);

  // load idfFile and time it
  openstudio::Time start = openstudio::Time::currentTime();
  // Note: The name implies 5 zones; but there's actually a plenum, so 6 zones in total
  openstudio::path path = resourcesPath() / toPath("energyplus/5ZoneAirCooled/in.idf");
  openstudio::OptionalIdfFile oidf = openstudio::IdfFile::load(path);  // should assume IddFileType::EnergyPlus
  ASSERT_TRUE(oidf);
  epIdfFile = *oidf;
  openstudio::Time idfFileLoadTime = openstudio::Time::currentTime() - start;
  LOG(Info, "EnergyPlus IdfFile load time = " << idfFileLoadTime);
  LOG(Info, "EnergyPlus IdfFile contains " << epIdfFile.objects().size() << " objects.");

  // load imfFile and time it
  start = openstudio::Time::currentTime();
  path = resourcesPath() / toPath("energyplus/ImfFiles/HPBScheduleSets.imf");
  openstudio::IddFileType iddType(openstudio::IddFileType::EnergyPlus);
  openstudio::OptionalImfFile oimf = openstudio::ImfFile::load(path, iddType);
  ASSERT_TRUE(oimf);
  imfFile = *oimf;
  idfFileLoadTime = openstudio::Time::currentTime() - start;
  LOG(Info, "EnergyPlus ImfFile load time = " << idfFileLoadTime);
  LOG(Info, "EnergyPlus ImfFile contains " << imfFile.numSections() << " sections.");
}

void IdfFixture::TearDownTestSuite() {
  logFile->disable();
}

double IdfFixture::tol(1.0E-5);
openstudio::path IdfFixture::outDir(resourcesPath() / toPath("energyplus/5ZoneAirCooled/"));
openstudio::IdfFile IdfFixture::epIdfFile;
openstudio::ImfFile IdfFixture::imfFile;
boost::optional<openstudio::FileLogSink> IdfFixture::logFile;
