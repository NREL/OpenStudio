/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2016, Alliance for Sustainable Energy, LLC. All rights reserved.
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

#include "ProjectFixture.hpp"

#include "../ProjectDatabase.hpp"

#include "../../utilities/core/Path.hpp"
#include "../../utilities/idd/IddEnums.hpp"

using openstudio::Logger;
using openstudio::FileLogSink;
using openstudio::toPath;

void ProjectFixture::SetUp() {}

void ProjectFixture::TearDown() {}

void ProjectFixture::SetUpTestCase() {
  // set up logging
  logFile = FileLogSink(toPath("./ProjectFixture.log"));
  logFile->setLogLevel(Info);
  openstudio::Logger::instance().standardOutLogger().disable();

  // set up data folder
  if (!boost::filesystem::exists(toPath("ProjectFixtureData"))) {
    boost::filesystem::create_directory(toPath("ProjectFixtureData"));
  }

}

void ProjectFixture::TearDownTestCase()
{
  logFile->disable();
  removeAllDatabases(); // comment out for debugging, uncomment to preserve space
}

openstudio::project::ProjectDatabase ProjectFixture::getCleanDatabase(
    const std::string& projectDatabaseName)
{
  return getDatabase(projectDatabaseName,true);
}

openstudio::project::ProjectDatabase ProjectFixture::getExistingDatabase(
    const std::string& projectDatabaseName)
{
  return getDatabase(projectDatabaseName, false);
}

openstudio::project::ProjectDatabase ProjectFixture::getDatabase(
    const std::string& projectDatabaseName,
    bool getCleanDatabase)
{
  openstudio::path projectDir = toPath("ProjectFixtureData") / toPath(projectDatabaseName);
  if (getCleanDatabase) {
    boost::filesystem::remove_all(projectDir);
  }
  if (!boost::filesystem::exists(projectDir)) {
    boost::filesystem::create_directory(projectDir);
    databaseDirs.push_back(projectDir);
  }

  openstudio::path runPath = projectDir / toPath("run.db");
  openstudio::path projectPath = projectDir / toPath("project.osp");

  openstudio::runmanager::RunManager runManager(runPath, true, false, false);
  openstudio::project::ProjectDatabase database(projectPath, runManager, getCleanDatabase);

  return database;
}

void ProjectFixture::removeAllDatabases() {
  for (const openstudio::path& dbDir : databaseDirs) {
    if (boost::filesystem::exists(dbDir)) {
      try{
        boost::filesystem::remove_all(dbDir);
      } catch (const std::exception&){

      }
    }
  }
  databaseDirs.clear();
}

boost::optional<openstudio::FileLogSink> ProjectFixture::logFile;
std::vector<openstudio::path> ProjectFixture::databaseDirs;
