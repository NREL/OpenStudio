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

#include <gtest/gtest.h>
#include "ProjectFixture.hpp"

#include "../ProjectDatabase.hpp"

#include "../../runmanager/lib/RunManager.hpp"

#include "../../utilities/core/System.hpp"
#include "../../utilities/core/PathHelpers.hpp"

#include <resources.hxx>
#include <OpenStudio.hxx>

using namespace openstudio;
using namespace openstudio::project;
using namespace openstudio::runmanager;

TEST_F(ProjectFixture, ProjectDatabaseRecord) {
  DateTime databaseTimestampCreate;
  UUID databaseHandle;

  openstudio::path projectPath = toPath("ProjectFixtureData/ProjectDatabaseRecord") / toPath("project.osp");
  openstudio::path runPath = toPath("ProjectFixtureData/ProjectDatabaseRecord") / toPath("run.db");

  {
    // intialize
    ProjectDatabase database = getCleanDatabase("ProjectDatabaseRecord");
    RunManager runManager = database.runManager();

    databaseTimestampCreate = database.timestampCreate();
    databaseHandle = database.handle();

    EXPECT_EQ(openStudioVersion(), database.version());
    EXPECT_EQ(completeAndNormalize(projectPath), database.path());
    EXPECT_EQ(completeAndNormalize(runPath), database.runManagerDBPath());
    EXPECT_EQ(runManager.dbPath(), database.runManager().dbPath());
  }

  openstudio::System::msleep(1000);

  {
    // re-open
    ProjectDatabase database = getExistingDatabase("ProjectDatabaseRecord");
    RunManager runManager = database.runManager();

    EXPECT_EQ(databaseTimestampCreate, database.timestampCreate());
    EXPECT_EQ(databaseHandle, database.handle());

    EXPECT_EQ(openStudioVersion(), database.version());
    EXPECT_EQ(completeAndNormalize(projectPath), database.path());
    EXPECT_EQ(completeAndNormalize(runPath), database.runManagerDBPath());
    EXPECT_EQ(runManager.dbPath(), database.runManager().dbPath());
  }

  openstudio::System::msleep(1000);

  {
    // re-open
    boost::optional<ProjectDatabase> database = ProjectDatabase::open(projectPath);
    ASSERT_TRUE(database);
    RunManager runManager = database->runManager();

    EXPECT_EQ(databaseTimestampCreate, database->timestampCreate());
    EXPECT_EQ(databaseHandle, database->handle());

    EXPECT_EQ(openStudioVersion(), database->version());
    EXPECT_EQ(completeAndNormalize(projectPath), database->path());
    EXPECT_EQ(completeAndNormalize(runPath), database->runManagerDBPath());
    EXPECT_EQ(runManager.dbPath(), database->runManager().dbPath());
  }

}
