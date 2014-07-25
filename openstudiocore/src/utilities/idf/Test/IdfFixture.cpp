/**********************************************************************
*  Copyright (c) 2008-2014, Alliance for Sustainable Energy.  
*  All rights reserved.
*  
*  This library is free software; you can redistribute it and/or
*  modify it under the terms of the GNU Lesser General Public
*  License as published by the Free Software Foundation; either
*  version 2.1 of the License, or (at your option) any later version.
*  
*  This library is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
*  Lesser General Public License for more details.
*  
*  You should have received a copy of the GNU Lesser General Public
*  License along with this library; if not, write to the Free Software
*  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
**********************************************************************/

#include "IdfFixture.hpp"

#include "../../time/Time.hpp"

using openstudio::Logger;
using openstudio::FileLogSink;
using openstudio::toPath;

void IdfFixture::SetUp() {}

void IdfFixture::TearDown() {}

void IdfFixture::SetUpTestCase() {
  // set up logging
  logFile = FileLogSink(toPath("./IdfFixture.log"));
  logFile->setLogLevel(Info);

  // load idfFile and time it
  openstudio::Time start = openstudio::Time::currentTime();
  openstudio::path path = resourcesPath()/toPath("energyplus/5ZoneAirCooled/in.idf");
  openstudio::OptionalIdfFile oidf = openstudio::IdfFile::load(path); // should assume IddFileType::EnergyPlus
  ASSERT_TRUE(oidf);
  epIdfFile = *oidf;
  openstudio::Time idfFileLoadTime = openstudio::Time::currentTime() - start;
  LOG(Info, "EnergyPlus IdfFile load time = " << idfFileLoadTime);
  LOG(Info, "EnergyPlus IdfFile contains " << epIdfFile.objects().size() << " objects.");

  // load imfFile and time it
  start = openstudio::Time::currentTime();
  path = resourcesPath()/toPath("energyplus/ImfFiles/HPBScheduleSets.imf");
  openstudio::IddFileType iddType(openstudio::IddFileType::EnergyPlus);
  openstudio::OptionalImfFile oimf = openstudio::ImfFile::load(path,iddType);
  ASSERT_TRUE(oimf);
  imfFile = *oimf;
  idfFileLoadTime = openstudio::Time::currentTime() - start;
  LOG(Info, "EnergyPlus ImfFile load time = " << idfFileLoadTime);
  LOG(Info, "EnergyPlus ImfFile contains " << imfFile.numSections() << " sections.");
}

void IdfFixture::TearDownTestCase() {
  logFile->disable();
}

double IdfFixture::tol(1.0E-5);
openstudio::path IdfFixture::outDir(resourcesPath()/toPath("energyplus/5ZoneAirCooled/"));
openstudio::IdfFile IdfFixture::epIdfFile;
openstudio::ImfFile IdfFixture::imfFile;
boost::optional<openstudio::FileLogSink> IdfFixture::logFile;
