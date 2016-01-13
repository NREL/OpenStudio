/**********************************************************************
*  Copyright (c) 2008-2016, Alliance for Sustainable Energy.
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

