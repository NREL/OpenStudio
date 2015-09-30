/**********************************************************************
*  Copyright (c) 2008-2015, Alliance for Sustainable Energy.
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

#include "../URLSearchPathRecord.hpp"
#include "../URLSearchPathRecord_Impl.hpp"
#include "../ProjectDatabase.hpp"

#include "../../utilities/idf/URLSearchPath.hpp"

#include <resources.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio;
using namespace openstudio::project;

TEST_F(ProjectFixture, URLSearchPathRecord)
{
  ProjectDatabase database = getCleanDatabase("URLSearchPathRecord");

  URLSearchPath localSearchPath(toPath("./local/"), URLSearchPath::ToCurrentWorkingDir, IddObjectType::OS_WeatherFile);
  URLSearchPath remoteSearchPath(QUrl("https://www.openstudio.net"), URLSearchPath::ToInputFile);

  URLSearchPathRecord localSearchPathRecord(localSearchPath, database);
  URLSearchPathRecord remoteSearchPathRecord(remoteSearchPath, database);

  std::vector<URLSearchPathRecord> urlSearchPathRecords = URLSearchPathRecord::getURLSearchPathRecords(database);
  ASSERT_EQ(2u, urlSearchPathRecords.size());

  URLSearchPath localSearchPath2 = localSearchPathRecord.urlSearchPath();
  EXPECT_TRUE(localSearchPath.getUrl() == localSearchPath2.getUrl());
  EXPECT_TRUE(localSearchPath.getRelativity() == localSearchPath2.getRelativity());
  ASSERT_TRUE(localSearchPath.getIddObjectType());
  ASSERT_TRUE(localSearchPath2.getIddObjectType());
  EXPECT_TRUE(localSearchPath.getIddObjectType()->value() == localSearchPath2.getIddObjectType()->value());

  URLSearchPath remoteSearchPath2 = remoteSearchPathRecord.urlSearchPath();
  EXPECT_TRUE(remoteSearchPath.getUrl() == remoteSearchPath2.getUrl());
  EXPECT_TRUE(remoteSearchPath.getRelativity() == remoteSearchPath2.getRelativity());
  EXPECT_FALSE(remoteSearchPath.getIddObjectType());
  EXPECT_FALSE(remoteSearchPath2.getIddObjectType());

}
