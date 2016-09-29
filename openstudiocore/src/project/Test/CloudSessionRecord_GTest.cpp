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

