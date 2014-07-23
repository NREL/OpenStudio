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
