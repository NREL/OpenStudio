/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2018, Alliance for Sustainable Energy, LLC. All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 *  following conditions are met:
 *
 *  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
 *  disclaimer.
 *
 *  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *  following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 *  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote
 *  products derived from this software without specific prior written permission from the respective party.
 *
 *  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative
 *  works may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without
 *  specific prior written permission from Alliance for Sustainable Energy, LLC.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER, THE UNITED STATES GOVERNMENT, OR ANY CONTRIBUTORS BE LIABLE FOR
 *  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **********************************************************************************************************************/

#include "SqlFileFixture.hpp"

using openstudio::Logger;
using openstudio::FileLogSink;
using openstudio::toPath;

void SqlFileFixture::SetUp() {}

void SqlFileFixture::TearDown() {}

void SqlFileFixture::SetUpTestCase()
{
  logFile = FileLogSink(toPath("./SqlFileFixture.log"));
  logFile->setLogLevel(Debug);

  openstudio::path path;
  path = resourcesPath()/toPath("energyplus/5ZoneAirCooled/eplusout.sql");
  sqlFile = openstudio::SqlFile(path);
  ASSERT_TRUE(sqlFile.connectionOpen());

  openstudio::path path2;
  path2 = resourcesPath() / toPath("energyplus/Office_With_Many_HVAC_Types/eplusout.sql");
  sqlFile2 = openstudio::SqlFile(path2);
  ASSERT_TRUE(sqlFile2.connectionOpen());

}

void SqlFileFixture::TearDownTestCase() {
  logFile->disable();
}

// define static storage
openstudio::SqlFile SqlFileFixture::sqlFile;
openstudio::SqlFile SqlFileFixture::sqlFile2;
boost::optional<openstudio::FileLogSink> SqlFileFixture::logFile;
