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

#include "ProjectFixture.hpp"

#include "../ProjectDatabase.hpp"

#include "../../utilities/core/Path.hpp"
#include <utilities/idd/IddEnums.hxx>

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
      boost::filesystem::remove_all(dbDir);
    }
  }
  databaseDirs.clear();
}

boost::optional<openstudio::FileLogSink> ProjectFixture::logFile;
std::vector<openstudio::path> ProjectFixture::databaseDirs;
