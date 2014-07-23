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

#include "SqlFileLargeFixture.hpp"
  /*
  To run test 
  1. uncomment in src/utilities/sql/CMakeLists.txt
  2. Copy Y:/5500/HPBldg/DannysFiles/eplusout.sql to build/resources/utilites folder (file is 533,983 kB)
  */

using openstudio::toPath;

void SqlFileLargeFixture::SetUp() {}

void SqlFileLargeFixture::TearDown() {}

void SqlFileLargeFixture::SetUpTestCase()
{
  openstudio::Logger::instance().disableStandardOut();
  openstudio::Logger::instance().logLevel(Debug);
  logFile = openstudio::Logger::instance().addLogFile(openstudio::toPath("./SqlFileLargeFixture.log"));

  openstudio::path path;
  path = resourcesPath()/toPath("utilities/eplusout.sql");
  sqlFile = openstudio::SqlFile(path);
  ASSERT_TRUE(sqlFile.connectionOpen());
}

void SqlFileLargeFixture::TearDownTestCase() {
  openstudio::Logger::instance().removeLogFile(logFile);
}

// define static storage
openstudio::SqlFile SqlFileLargeFixture::sqlFile;
std::shared_ptr<openstudio::TextLogType> SqlFileLargeFixture::logFile;
