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

#ifndef PROJECT_TEST_PROJECTFIXTURE_HPP
#define PROJECT_TEST_PROJECTFIXTURE_HPP

#include <gtest/gtest.h>
#include "../ProjectDatabase.hpp"

#include <resources.hxx>

#include "../../utilities/core/Logger.hpp"
#include "../../utilities/core/FileLogSink.hpp"

#include <boost/optional.hpp>

class ProjectFixture : public ::testing::Test {
 protected:
  // initialize for each test
  virtual void SetUp();

  // tear down after each test
  virtual void TearDown();

  // initialize static members
  static void SetUpTestCase();

  // tear down static members
  static void TearDownTestCase();

  static boost::optional<openstudio::FileLogSink> logFile;

  // set up logging
  REGISTER_LOGGER("ProjectFixture");

  /** Returns a clean database for testing. */
  openstudio::project::ProjectDatabase getCleanDatabase(
      const std::string& projectDatabaseName);

    /** Returns an existing database for testing. */
  openstudio::project::ProjectDatabase getExistingDatabase(
      const std::string& projectDatabaseName);

private:
  openstudio::project::ProjectDatabase getDatabase(
      const std::string& projectDatabaseName,
      bool getCleanDatabase);

  static std::vector<openstudio::path> databaseDirs;

  static void removeAllDatabases();
};

#endif // PROJECT_TEST_PROJECTFIXTURE_HPP
