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
}

void SqlFileFixture::TearDownTestCase() {
  logFile->disable();
}

// define static storage
openstudio::SqlFile SqlFileFixture::sqlFile;
boost::optional<openstudio::FileLogSink> SqlFileFixture::logFile;
