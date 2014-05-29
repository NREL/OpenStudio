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

#include "../VagrantSessionRecord.hpp"
#include "../VagrantSessionRecord_Impl.hpp"

#include "../../utilities/cloud/AWSProvider.hpp"
#include "../../utilities/cloud/AWSProvider_Impl.hpp"
#include "../../utilities/cloud/VagrantProvider.hpp"
#include "../../utilities/cloud/VagrantProvider_Impl.hpp"

using namespace openstudio;
using namespace openstudio::project;

TEST_F(ProjectFixture,CloudSessionRecord_VagrantDefault) {
  {
    ProjectDatabase database = getCleanDatabase("CloudSessionRecord_VagrantDefault");

    VagrantProvider provider;
    CloudSession session = provider.session();

    CloudSessionRecord record = CloudSessionRecord::factoryFromCloudSession(session,database);
    database.save();

    EXPECT_EQ(1u,CloudSessionRecord::getCloudSessionRecords(database).size());
  }
  {
    ProjectDatabase database = getExistingDatabase("CloudSessionRecord_VagrantDefault");

    EXPECT_EQ(1u,CloudSessionRecord::getCloudSessionRecords(database).size());
    ASSERT_FALSE(CloudSessionRecord::getCloudSessionRecords(database).empty());

    CloudSessionRecord record = CloudSessionRecord::getCloudSessionRecords(database)[0];

    CloudSession session = record.cloudSession();
    EXPECT_TRUE(session.optionalCast<VagrantSession>());
  }
}

TEST_F(ProjectFixture,CloudSessionRecord_VagrantPopulated) {
  std::string id;
  UUID uuid, versionUUID;
  {
    ProjectDatabase database = getCleanDatabase("CloudSessionRecord_VagrantPopulated");

    VagrantSession session(toString(createUUID()),
                           Url("http://localhost:8080"),
                           UrlVector(1u,Url("http://localhost:8081")));
    id = session.sessionId();
    uuid = session.uuid();
    versionUUID = session.versionUUID();

    CloudSessionRecord record = CloudSessionRecord::factoryFromCloudSession(session,database);
    database.save();

    EXPECT_EQ(1u,CloudSessionRecord::getCloudSessionRecords(database).size());    
  }
  {
    ProjectDatabase database = getExistingDatabase("CloudSessionRecord_VagrantPopulated");

    EXPECT_EQ(1u,CloudSessionRecord::getCloudSessionRecords(database).size());
    ASSERT_FALSE(CloudSessionRecord::getCloudSessionRecords(database).empty());

    CloudSessionRecord record = CloudSessionRecord::getCloudSessionRecords(database)[0];

    CloudSession session = record.cloudSession();
    ASSERT_TRUE(session.optionalCast<VagrantSession>());

    VagrantSession vagrantSession = session.cast<VagrantSession>();
    EXPECT_EQ(uuid,vagrantSession.uuid());
    EXPECT_EQ(versionUUID,vagrantSession.versionUUID());
    EXPECT_EQ("VagrantProvider",vagrantSession.cloudProviderType());
    EXPECT_EQ(id,vagrantSession.sessionId());
    ASSERT_TRUE(vagrantSession.serverUrl());
    EXPECT_EQ(Url("http://localhost:8080"),vagrantSession.serverUrl().get());
    ASSERT_EQ(1u,vagrantSession.workerUrls().size());
    EXPECT_EQ(Url("http://localhost:8081"),vagrantSession.workerUrls()[0]);
  }
}

TEST_F(ProjectFixture,CloudSessionRecord_AWSDefault) {
  {
    ProjectDatabase database = getCleanDatabase("CloudSessionRecord_AWSDefault");

    AWSProvider provider;
    CloudSession session = provider.session();

    CloudSessionRecord record = CloudSessionRecord::factoryFromCloudSession(session,database);
    database.save();

    EXPECT_EQ(1u,CloudSessionRecord::getCloudSessionRecords(database).size());
  }
  {
    ProjectDatabase database = getExistingDatabase("CloudSessionRecord_AWSDefault");

    EXPECT_EQ(1u,CloudSessionRecord::getCloudSessionRecords(database).size());
    ASSERT_FALSE(CloudSessionRecord::getCloudSessionRecords(database).empty());

    CloudSessionRecord record = CloudSessionRecord::getCloudSessionRecords(database)[0];

    CloudSession session = record.cloudSession();
    EXPECT_TRUE(session.optionalCast<AWSSession>());
  }
}

