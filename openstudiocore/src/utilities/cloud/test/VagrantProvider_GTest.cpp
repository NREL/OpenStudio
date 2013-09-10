/**********************************************************************
*  Copyright (c) 2008-2013, Alliance for Sustainable Energy.  
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

#include <utilities/cloud/test/VagrantFixture.hpp>

#include <utilities/cloud/VagrantProvider.hpp>
#include <utilities/cloud/VagrantProvider_Impl.hpp>

#include <utilities/cloud/test/Vagrant.hxx>

using namespace openstudio;

TEST_F(VagrantFixture,Vagrant_UpProvisionHalt) {
  VagrantProvider provider;
  VagrantSettings settings;

  settings.setServerPath(vagrantServerPath());
  settings.setServerUrl(Url("http://localhost:8080"));
  settings.setWorkerPath(vagrantWorkerPath());
  settings.setWorkerUrl(Url("http://localhost:8081"));
  settings.setHaltOnStop(true);
  settings.setUsername("vagrant");
  settings.setPassword("vagrant");
  settings.signUserAgreement(true);
  provider.setSettings(settings);

  // ensures vagrant -v works in server directory
  ASSERT_TRUE(provider.serviceAvailable());

  // check state
  EXPECT_FALSE(provider.serverUrl());
  EXPECT_FALSE(provider.running());
  EXPECT_EQ(1u,provider.numWorkers());

  // start service
  ASSERT_TRUE(provider.startServer());
  ASSERT_TRUE(provider.startWorkers());

  // check state
  EXPECT_TRUE(provider.serverUrl());
  EXPECT_TRUE(provider.running());

  // stop service
  EXPECT_TRUE(provider.terminate());

  // check state
  EXPECT_FALSE(provider.serverUrl());
  EXPECT_FALSE(provider.running());
}
