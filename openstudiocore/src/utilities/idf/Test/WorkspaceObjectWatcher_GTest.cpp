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
#include "../WorkspaceObjectWatcher.hpp"
#include "../Workspace.hpp"
#include "../WorkspaceObject.hpp"
#include "../IdfExtensibleGroup.hpp"

#include <utilities/idd/Lights_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>


#include <resources.hxx>

using namespace std;
using namespace boost;
using namespace openstudio;

TEST_F(IdfFixture,WorkspaceObjectWatcher_CommentChanges)
{
  IdfObject object(IddObjectType::Lights);
  Workspace workspace(StrictnessLevel::Draft, IddFileType::EnergyPlus);

  OptionalWorkspaceObject owo = workspace.addObject(object);
  ASSERT_TRUE(owo);
  WorkspaceObject lights = *owo;
  WorkspaceObjectWatcher watcher(lights);
  EXPECT_FALSE(watcher.dirty());

  lights.setComment("Change from default to this");
  EXPECT_TRUE(watcher.dirty());
  EXPECT_FALSE(watcher.dataChanged());
  EXPECT_FALSE(watcher.relationshipChanged());
  watcher.clearState();
  EXPECT_FALSE(watcher.dirty());
  EXPECT_FALSE(watcher.dataChanged());
  EXPECT_FALSE(watcher.relationshipChanged());

  lights.setFieldComment(1,"Change from default to that");
  EXPECT_TRUE(watcher.dirty());
  EXPECT_FALSE(watcher.dataChanged());
  EXPECT_FALSE(watcher.relationshipChanged());
  watcher.clearState();
  EXPECT_FALSE(watcher.dirty());
  EXPECT_FALSE(watcher.dataChanged());
  EXPECT_FALSE(watcher.relationshipChanged());

  lights.setFieldComment(1000,"Way out of bounds");
  EXPECT_FALSE(watcher.dirty());
}

TEST_F(IdfFixture,WorkspaceObjectWatcher_DataFieldChanges)
{
  IdfObject object(IddObjectType::Lights);
  Workspace workspace(StrictnessLevel::Draft, IddFileType::EnergyPlus);

  OptionalWorkspaceObject owo = workspace.addObject(object);
  ASSERT_TRUE(owo);
  WorkspaceObject lights = *owo;
  WorkspaceObjectWatcher watcher(lights);
  EXPECT_FALSE(watcher.dirty());

  EXPECT_TRUE(lights.setName(""));
  EXPECT_TRUE(watcher.dirty());
  EXPECT_FALSE(watcher.dataChanged());
  EXPECT_TRUE(watcher.nameChanged());
  EXPECT_FALSE(watcher.relationshipChanged());
  watcher.clearState();
  EXPECT_TRUE(lights.createName(false));
  EXPECT_TRUE(watcher.dirty());
  EXPECT_FALSE(watcher.dataChanged());
  EXPECT_TRUE(watcher.nameChanged());
  EXPECT_FALSE(watcher.relationshipChanged());
  watcher.clearState();

  ASSERT_TRUE(lights.setString(4,"22.3"));
  EXPECT_TRUE(watcher.dirty());
  EXPECT_TRUE(watcher.dataChanged());
  EXPECT_FALSE(watcher.nameChanged());
  EXPECT_FALSE(watcher.relationshipChanged());
  watcher.clearState();
  EXPECT_FALSE(watcher.dirty());
  EXPECT_FALSE(watcher.dataChanged());
  EXPECT_FALSE(watcher.nameChanged());
  EXPECT_FALSE(watcher.relationshipChanged());

  EXPECT_FALSE(lights.setString(1000,"22.3"));
  EXPECT_FALSE(watcher.dirty());
  EXPECT_FALSE(watcher.dataChanged());
  EXPECT_FALSE(watcher.nameChanged());
  EXPECT_FALSE(watcher.relationshipChanged());
}

TEST_F(IdfFixture,WorkspaceObjectWatcher_RelationshipFieldChanges)
{
  IdfObject object(IddObjectType::DaylightingDevice_Tubular);
  Workspace workspace(StrictnessLevel::Draft, IddFileType::EnergyPlus);

  OptionalWorkspaceObject owo = workspace.addObject(object);
  ASSERT_TRUE(owo);
  WorkspaceObject tdd = *owo;
  WorkspaceObjectWatcher watcher(tdd);
  EXPECT_FALSE(watcher.dirty());

  // add zone objects
  object = IdfObject(IddObjectType::Zone);
  owo = workspace.addObject(object);
  ASSERT_TRUE(owo);
  EXPECT_EQ("Zone 1",owo->name().get());
  owo = workspace.addObject(object);
  ASSERT_TRUE(owo);
  EXPECT_EQ("Zone 2",owo->name().get());
  owo = workspace.addObject(object);
  ASSERT_TRUE(owo);
  EXPECT_EQ("Zone 3",owo->name().get());
  EXPECT_FALSE(watcher.dirty());

  vector<string> vals;
  vals.push_back("Zone 1");
  vals.push_back("1.0");
  IdfExtensibleGroup eg = tdd.pushExtensibleGroup(vals);
  ASSERT_FALSE(eg.empty());
  EXPECT_TRUE(watcher.dirty());
  EXPECT_TRUE(watcher.dataChanged());
  EXPECT_FALSE(watcher.nameChanged());
  EXPECT_TRUE(watcher.relationshipChanged());
  watcher.clearState();
  EXPECT_FALSE(watcher.dirty());
  EXPECT_FALSE(watcher.dataChanged());
  EXPECT_FALSE(watcher.nameChanged());
  EXPECT_FALSE(watcher.relationshipChanged());

  EXPECT_TRUE(eg.setString(0,"Zone 2"));
  EXPECT_TRUE(watcher.dirty());
  EXPECT_FALSE(watcher.dataChanged());
  EXPECT_FALSE(watcher.nameChanged());
  EXPECT_TRUE(watcher.relationshipChanged());
  watcher.clearState();
  EXPECT_FALSE(watcher.dirty());
  EXPECT_FALSE(watcher.dataChanged());
  EXPECT_FALSE(watcher.nameChanged());
  EXPECT_FALSE(watcher.relationshipChanged());

  EXPECT_TRUE(eg.setDouble(1,2.0));
  EXPECT_TRUE(watcher.dirty());
  EXPECT_TRUE(watcher.dataChanged());
  EXPECT_FALSE(watcher.nameChanged());
  EXPECT_FALSE(watcher.relationshipChanged());
  watcher.clearState();
  EXPECT_FALSE(watcher.dirty());
  EXPECT_FALSE(watcher.dataChanged());
  EXPECT_FALSE(watcher.nameChanged());
  EXPECT_FALSE(watcher.relationshipChanged());
}


TEST_F(IdfFixture,WorkspaceObjectWatcher_RelationshipFieldChanges_TargetDeleted)
{
  IdfObject object(IddObjectType::Lights);
  Workspace workspace(StrictnessLevel::Draft, IddFileType::EnergyPlus);

  OptionalWorkspaceObject owo = workspace.addObject(object);
  ASSERT_TRUE(owo);
  WorkspaceObject workspaceObject = *owo;
  WorkspaceObjectWatcher watcher(workspaceObject);
  EXPECT_FALSE(watcher.dirty());

  OptionalWorkspaceObject oZone = workspace.addObject(IdfObject(IddObjectType::Zone));
  ASSERT_TRUE(oZone);

  EXPECT_TRUE(workspaceObject.setPointer(LightsFields::ZoneorZoneListName, oZone->handle()));
  EXPECT_TRUE(watcher.dirty());
  EXPECT_FALSE(watcher.dataChanged());
  EXPECT_FALSE(watcher.nameChanged());
  EXPECT_TRUE(watcher.relationshipChanged());

  watcher.clearState();
  EXPECT_FALSE(watcher.dirty());
  EXPECT_FALSE(watcher.dataChanged());
  EXPECT_FALSE(watcher.nameChanged());
  EXPECT_FALSE(watcher.relationshipChanged());

  oZone->remove();
  EXPECT_TRUE(watcher.dirty());
  EXPECT_FALSE(watcher.dataChanged());
  EXPECT_FALSE(watcher.nameChanged());
  EXPECT_TRUE(watcher.relationshipChanged());
}
