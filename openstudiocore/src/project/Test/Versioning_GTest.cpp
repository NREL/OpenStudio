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
      EXPECT_TRUE(ProjectDatabase::isExistingProjectDatabase(testPath)) << toString(testPath);

      EXPECT_TRUE(ProjectDatabase::requiresUpdate(testPath)) << toString(testPath);

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

