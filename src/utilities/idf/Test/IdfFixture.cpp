/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2020, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
*  following conditions are met:
*
*  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
*  disclaimer.
*
*  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
*  disclaimer in the documentation and/or other materials provided with the distribution.
*
*  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote products
*  derived from this software without specific prior written permission from the respective party.
*
*  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative works
*  may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without specific prior
*  written permission from Alliance for Sustainable Energy, LLC.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER(S) AND ANY CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
*  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
*  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER(S), ANY CONTRIBUTORS, THE UNITED STATES GOVERNMENT, OR THE UNITED
*  STATES DEPARTMENT OF ENERGY, NOR ANY OF THEIR EMPLOYEES, BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
*  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
*  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
*  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
*  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
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
