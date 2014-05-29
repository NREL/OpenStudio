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

#include "../UUID.hpp"
#include "../String.hpp"

#include <iostream>
#include <set>

#include <QVariant>

using std::cout;
using openstudio::UUID;
using openstudio::createUUID;
using openstudio::toUUID;
using openstudio::removeBraces;
using openstudio::toString;
using std::string;
using std::stringstream;

TEST(UUID, Constuctors)
{
  // default constructor should be null
  UUID null;
  EXPECT_TRUE(null.isNull());

  // created UUID is not null
  UUID uuid = createUUID();
  EXPECT_FALSE(uuid.isNull());

  // copy UUID
  UUID uuid2(uuid);
  EXPECT_FALSE(uuid2.isNull());
  EXPECT_TRUE(uuid == uuid2);

  // from string
  string str = toString(uuid);
  UUID uuid3 = toUUID(str);
  EXPECT_FALSE(uuid3.isNull());
  EXPECT_TRUE(uuid == uuid3);

  // from bad string
  UUID uuid4 = toUUID("Not a UUID");
  EXPECT_TRUE(uuid4.isNull());
}

TEST(UUID, BigSet)
{
  // create a bunch of uuids, insert into set, make sure no uuids collided
  unsigned numUUIDS = 1000000;
  std::set<UUID> uuids;

  for(unsigned i=0; i < numUUIDS; ++i){
    uuids.insert(createUUID());
  }

  // check no duplicates
  EXPECT_EQ(numUUIDS, uuids.size());
}

TEST(UUID, UUID_QVariant)
{
  UUID uuid = createUUID();
  QVariant variant = QVariant::fromValue(uuid);
  EXPECT_EQ("openstudio::UUID", std::string(variant.typeName()));
  ASSERT_TRUE(variant.canConvert<UUID>());
  UUID uuid2 = variant.value<UUID>();
  EXPECT_TRUE(uuid == uuid2);
}

TEST(UUID, UUID_UID) {
  UUID uuid = createUUID();
  std::string uuidStr = toString(uuid);
  std::string uidStr = removeBraces(uuid);
  EXPECT_EQ("{" + uidStr + "}",uuidStr);
  EXPECT_EQ(uuid,toUUID(uuidStr));
  EXPECT_EQ(uuid,toUUID(uidStr)); // no extra conversion process
}
