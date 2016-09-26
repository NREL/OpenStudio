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

#include "../VagrantSettingsRecord.hpp"
#include "../VagrantSettingsRecord_Impl.hpp"

#include "../../utilities/cloud/AWSProvider.hpp"
#include "../../utilities/cloud/AWSProvider_Impl.hpp"
#include "../../utilities/cloud/VagrantProvider.hpp"
#include "../../utilities/cloud/VagrantProvider_Impl.hpp"

using namespace openstudio;
using namespace openstudio::project;

TEST_F(ProjectFixture,CloudSettingsRecord_VagrantDefault) {
  {
    ProjectDatabase database = getCleanDatabase("CloudSettingsRecord_VagrantDefault");

    VagrantProvider provider;
    CloudSettings settings = provider.settings();

    CloudSettingsRecord record = CloudSettingsRecord::factoryFromCloudSettings(settings,database);
    database.save();

    EXPECT_EQ(1u,CloudSettingsRecord::getCloudSettingsRecords(database).size());
  }
  {
    ProjectDatabase database = getExistingDatabase("CloudSettingsRecord_VagrantDefault");

    EXPECT_EQ(1u,CloudSettingsRecord::getCloudSettingsRecords(database).size());
    ASSERT_FALSE(CloudSettingsRecord::getCloudSettingsRecords(database).empty());

    CloudSettingsRecord record = CloudSettingsRecord::getCloudSettingsRecords(database)[0];

    CloudSettings settings = record.cloudSettings();
    EXPECT_TRUE(settings.optionalCast<VagrantSettings>());
  }
}

TEST_F(ProjectFixture,CloudSettingsRecord_VagrantWithSettings) {
  UUID uuid, versionUUID;
  {
    ProjectDatabase database = getCleanDatabase("CloudSettingsRecord_VagrantWithSettings");

    VagrantProvider provider;
    VagrantSettings settings;
    settings.setServerPath(toPath("C:/projects/openstudio-server/vagrant/server")); // fake, but realistic path
    settings.setServerUrl(Url("http://localhost:8080"));
    settings.setWorkerPath(toPath("C:/projects/openstudio-server/vagrant/worker")); // fake, but realistic path
    settings.setWorkerUrl(Url("http://localhost:8081"));
    settings.setHaltOnStop(true);
    settings.setUsername("vagrant");
    settings.setPassword("vagrant");
    settings.signUserAgreement(true);
    provider.setSettings(settings);
    uuid = settings.uuid();
    versionUUID = settings.versionUUID();

    settings = provider.settings().cast<VagrantSettings>();
    CloudSettingsRecord record = CloudSettingsRecord::factoryFromCloudSettings(settings,database);
    database.save();

    EXPECT_EQ(1u,CloudSettingsRecord::getCloudSettingsRecords(database).size());
  }
  {
    ProjectDatabase database = getExistingDatabase("CloudSettingsRecord_VagrantWithSettings");

    EXPECT_EQ(1u,CloudSettingsRecord::getCloudSettingsRecords(database).size());
    ASSERT_FALSE(CloudSettingsRecord::getCloudSettingsRecords(database).empty());

    CloudSettingsRecord record = CloudSettingsRecord::getCloudSettingsRecords(database)[0];
    
    CloudSettings settings = record.cloudSettings();
    ASSERT_TRUE(settings.optionalCast<VagrantSettings>());
    VagrantSettings vagrantSettings = settings.cast<VagrantSettings>();
    EXPECT_EQ(uuid,vagrantSettings.uuid());
    EXPECT_EQ(versionUUID,vagrantSettings.versionUUID());
    EXPECT_EQ("VagrantProvider",vagrantSettings.cloudProviderType());
    EXPECT_TRUE(vagrantSettings.userAgreementSigned());
    EXPECT_EQ(toPath("C:/projects/openstudio-server/vagrant/server"),vagrantSettings.serverPath());
    EXPECT_EQ(Url("http://localhost:8080"),vagrantSettings.serverUrl());
    EXPECT_EQ(toPath("C:/projects/openstudio-server/vagrant/worker"),vagrantSettings.workerPath());
    EXPECT_EQ(Url("http://localhost:8081"),vagrantSettings.workerUrl());
    EXPECT_TRUE(vagrantSettings.haltOnStop());
    EXPECT_EQ("vagrant",vagrantSettings.username());
    EXPECT_EQ("",vagrantSettings.password());
  }
}

TEST_F(ProjectFixture,CloudSettingsRecord_AWSDefault) {
  {
    ProjectDatabase database = getCleanDatabase("CloudSettingsRecord_AWSDefault");

    AWSProvider provider;
    CloudSettings settings = provider.settings();

    CloudSettingsRecord record = CloudSettingsRecord::factoryFromCloudSettings(settings,database);
    database.save();

    EXPECT_EQ(1u,CloudSettingsRecord::getCloudSettingsRecords(database).size());
  }
  {
    ProjectDatabase database = getExistingDatabase("CloudSettingsRecord_AWSDefault");

    EXPECT_EQ(1u,CloudSettingsRecord::getCloudSettingsRecords(database).size());
    ASSERT_FALSE(CloudSettingsRecord::getCloudSettingsRecords(database).empty());

    CloudSettingsRecord record = CloudSettingsRecord::getCloudSettingsRecords(database)[0];

    CloudSettings settings = record.cloudSettings();
    EXPECT_TRUE(settings.optionalCast<AWSSettings>());
  }
}
