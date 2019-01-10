/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2019, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
*  following conditions are met:
*
*  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
*  disclaimer.
*
*  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
*  disclaimer in the documentation and/or other materials provided with the distribution.
*
*  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote products
*  derived from this software without specific prior written permission from the respective party.
*
*  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative works
*  may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without specific prior
*  written permission from Alliance for Sustainable Energy, LLC.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER(S) AND ANY CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
*  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
*  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER(S), ANY CONTRIBUTORS, THE UNITED STATES GOVERNMENT, OR THE UNITED
*  STATES DEPARTMENT OF ENERGY, NOR ANY OF THEIR EMPLOYEES, BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
*  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
*  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
*  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
*  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
***********************************************************************************************************************/

#include <gtest/gtest.h>
#include "IdfFixture.hpp"
#include "../WorkspaceWatcher.hpp"
#include "../Workspace.hpp"
#include "../WorkspaceObject.hpp"
#include "../IdfExtensibleGroup.hpp"
#include <utilities/idd/IddEnums.hxx>

#include <resources.hxx>

using namespace std;
using namespace boost;
using namespace openstudio;

TEST_F(IdfFixture,WorkspaceWatcher_ObjectChanges)
{
  Workspace workspace(epIdfFile);
  WorkspaceWatcher watcher(workspace);

  WorkspaceObjectVector result = workspace.getObjectsByName("C5-1");
  ASSERT_TRUE(result.size() > 0);
  EXPECT_EQ(static_cast<unsigned>(1),result.size());
  EXPECT_TRUE(result[0].iddObject().type() == IddObjectType::BuildingSurface_Detailed);
  EXPECT_FALSE(watcher.dirty());

  IdfExtensibleGroup eg = result[0].pushExtensibleGroup();
  EXPECT_FALSE(eg.empty());
  EXPECT_TRUE(watcher.dirty());
  EXPECT_FALSE(watcher.objectAdded());
  EXPECT_FALSE(watcher.objectRemoved());
  watcher.clearState();
  EXPECT_FALSE(watcher.dirty());

  EXPECT_TRUE(eg.setDouble(0,4.3));
  EXPECT_TRUE(watcher.dirty());
  EXPECT_FALSE(watcher.objectAdded());
  EXPECT_FALSE(watcher.objectRemoved());
  watcher.clearState();
  EXPECT_FALSE(watcher.dirty());
}

TEST_F(IdfFixture,WorkspaceWatcher_AddObjects)
{
  Workspace workspace(epIdfFile);
  WorkspaceWatcher watcher(workspace);

  EXPECT_FALSE(watcher.dirty());
  OptionalWorkspaceObject owo = workspace.addObject(IdfObject(IddObjectType::Lights));
  EXPECT_TRUE(owo);
  EXPECT_TRUE(watcher.dirty());
  EXPECT_TRUE(watcher.objectAdded());
  EXPECT_FALSE(watcher.objectRemoved());
  watcher.clearState();
}

TEST_F(IdfFixture,WorkspaceWatcher_RemoveObjects)
{
  Workspace workspace(epIdfFile);
  WorkspaceWatcher watcher(workspace);

  EXPECT_FALSE(watcher.dirty());
  WorkspaceObjectVector result = workspace.getObjectsByName("C5-1");
  ASSERT_TRUE(result.size() > 0);
  EXPECT_EQ(static_cast<unsigned>(1),result.size());
  workspace.removeObject(result[0].handle());
  EXPECT_TRUE(watcher.dirty());
  EXPECT_FALSE(watcher.objectAdded());
  EXPECT_TRUE(watcher.objectRemoved());
  watcher.clearState();
  EXPECT_FALSE(watcher.dirty());
  EXPECT_FALSE(result[0].initialized());
  EXPECT_TRUE(result[0].handle().isNull());
}

