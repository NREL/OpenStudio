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
#include "../IdfObjectWatcher.hpp"
#include "../IdfExtensibleGroup.hpp"
#include "../../idd/Comments.hpp"
#include "../../core/Optional.hpp"

#include <resources.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace std;
using namespace boost;
using namespace openstudio;

TEST_F(IdfFixture,IdfObjectWatcher_NonExtensible)
{
  IdfObject object(IddObjectType::Lights);
  IdfObjectWatcher watcher(object);
  EXPECT_FALSE(watcher.dirty());

  object.setComment("Change from default to this");
  EXPECT_TRUE(watcher.dirty());
  EXPECT_FALSE(watcher.dataChanged());
  EXPECT_FALSE(watcher.nameChanged());
  watcher.clearState();
  EXPECT_FALSE(watcher.dirty());
  EXPECT_FALSE(watcher.dataChanged());
  EXPECT_FALSE(watcher.nameChanged());

  object.setFieldComment(1,"Change from default to that");
  EXPECT_TRUE(watcher.dirty());
  EXPECT_FALSE(watcher.dataChanged());
  watcher.clearState();
  EXPECT_FALSE(watcher.dirty());
  EXPECT_FALSE(watcher.dataChanged());
  EXPECT_FALSE(watcher.nameChanged());

  object.setFieldComment(1000,"Way out of bounds");
  EXPECT_FALSE(watcher.dirty());

  // this causes object to become dirty because it doesn't have a name yet
  object.createName(false);
  EXPECT_TRUE(watcher.dirty());
  EXPECT_FALSE(watcher.dataChanged());
  EXPECT_TRUE(watcher.nameChanged());
  watcher.clearState();
  // this does not, because now it has a name
  object.createName(false);
  EXPECT_FALSE(watcher.dirty());

  ASSERT_TRUE(object.setString(4,"22.3"));
  EXPECT_TRUE(watcher.dirty());
  EXPECT_TRUE(watcher.dataChanged());
  EXPECT_FALSE(watcher.nameChanged());
  watcher.clearState();
  EXPECT_FALSE(watcher.dirty());
  EXPECT_FALSE(watcher.dataChanged());
  EXPECT_FALSE(watcher.nameChanged());

  EXPECT_FALSE(object.setString(1000,"22.3"));
  EXPECT_FALSE(watcher.dirty());
  EXPECT_FALSE(watcher.dataChanged());
  EXPECT_FALSE(watcher.nameChanged());
}

TEST_F(IdfFixture,IdfObjectWatcher_Extensible)
{
  IdfObject object(IddObjectType::DaylightingDevice_Tubular);
  IdfObjectWatcher watcher(object);
  EXPECT_FALSE(watcher.dirty());

  vector<string> vals;
  vals.push_back("zone1");
  vals.push_back("1.0");
  object.pushExtensibleGroup(vals);
  EXPECT_TRUE(watcher.dirty());
  EXPECT_TRUE(watcher.dataChanged());
  watcher.clearState();
  EXPECT_FALSE(watcher.dirty());
  EXPECT_FALSE(watcher.dataChanged());

  object.insertExtensibleGroup(0,vals);
  EXPECT_TRUE(watcher.dirty());
  EXPECT_TRUE(watcher.dataChanged());
  watcher.clearState();
  EXPECT_FALSE(watcher.dirty());
  EXPECT_FALSE(watcher.dataChanged());

  vals=object.popExtensibleGroup();
  ASSERT_TRUE(vals.size()==2);
  EXPECT_TRUE(watcher.dirty());
  EXPECT_TRUE(watcher.dataChanged());
  watcher.clearState();
  EXPECT_FALSE(watcher.dirty());
  EXPECT_FALSE(watcher.dataChanged());

  vals = object.eraseExtensibleGroup(0);
  ASSERT_TRUE(vals.size()==2);
  EXPECT_TRUE(watcher.dirty());
  EXPECT_TRUE(watcher.dataChanged());
  watcher.clearState();
  EXPECT_FALSE(watcher.dirty());
  EXPECT_FALSE(watcher.dataChanged());

  object.pushExtensibleGroup(vals);
  object.pushExtensibleGroup(vals);
  object.pushExtensibleGroup(vals);
  EXPECT_TRUE(watcher.dirty());
  EXPECT_TRUE(watcher.dataChanged());
  watcher.clearState();
  EXPECT_FALSE(watcher.dirty());
  EXPECT_FALSE(watcher.dataChanged());

  object.clearExtensibleGroups();
  EXPECT_TRUE(watcher.dirty());
  watcher.clearState();
  EXPECT_FALSE(watcher.dirty());
}
