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

#include "ProjectVersioningFixture.hpp"
#include "../ProjectDatabase.hpp"

#include <boost/filesystem.hpp>

#include <OpenStudio.hxx>

using namespace openstudio;
using namespace openstudio::project;

TEST_F(ProjectVersioningFixture,Versioning_OnOpen) {
  // create separate copy of databases
  std::vector<openstudio::path> testPaths = copyProjectDatabases("open_update");

  for (const openstudio::path& testPath : testPaths) {
    {
      OptionalProjectDatabase database = ProjectDatabase::open(testPath);
      if (!database){
        EXPECT_TRUE(false) << "Could not open '" << toString(testPath) << "'";
        continue;
      }
      EXPECT_EQ(openStudioVersion(),database->version());
      
      // also check explicit query into the database
      QSqlQuery query(*(database->qSqlDatabase()));
      query.prepare(QString("SELECT version FROM ProjectDatabaseRecords"));
      assertExec(query);
      bool first = true;
      while (query.next()) {
        QVariant value = query.value(0);
        ASSERT_TRUE(value.isValid());
        ASSERT_FALSE(value.isNull());
        EXPECT_EQ(openStudioVersion(),value.toString().toStdString());
        EXPECT_TRUE(first);
        first = false;
      }
    }
    {
      // should be able to re-open
      OptionalProjectDatabase database = ProjectDatabase::open(testPath);
      ASSERT_TRUE(database);
      EXPECT_EQ(openStudioVersion(),database->version());
    }
  }
}

TEST_F(ProjectVersioningFixture,Versioning_OnConstruction) {
  // create separate copy of databases
  std::vector<openstudio::path> testPaths = copyProjectDatabases("constructor_update");

  for (const openstudio::path& testPath : testPaths) {
    {
      try{
        ProjectDatabase database(testPath);
        EXPECT_EQ(openStudioVersion(),database.version());

        // also check explicit query into the database
        QSqlQuery query(*(database.qSqlDatabase()));
        query.prepare(QString("SELECT version FROM ProjectDatabaseRecords"));
        assertExec(query);
        bool first = true;
        while (query.next()) {
          QVariant value = query.value(0);
          ASSERT_TRUE(value.isValid());
          ASSERT_FALSE(value.isNull());
          EXPECT_EQ(openStudioVersion(),value.toString().toStdString());
          EXPECT_TRUE(first);
          first = false;
        }
      }catch(const std::exception&){
        EXPECT_TRUE(false) << "Could not create project database at '" << toString(testPath) << "'";
      }
    }
    {
      // should be able to re-open
      try {
        ProjectDatabase database(testPath);
        EXPECT_EQ(openStudioVersion(),database.version());
      }catch(const std::exception&){
        EXPECT_TRUE(false) << "Could not re-open project database at '" << toString(testPath) << "'";
      }
    }
  }
}

