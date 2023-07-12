/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>

#include "../UUID.hpp"
#include "../String.hpp"

#include <iostream>
#include <set>

using std::cout;
using openstudio::UUID;
using openstudio::createUUID;
using openstudio::toUUID;
using openstudio::removeBraces;
using openstudio::toString;
using std::string;
using std::stringstream;

TEST(UUID, Constuctors) {
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
  UUID uuid4 = toUUID(std::string("Not a UUID"));
  EXPECT_TRUE(uuid4.isNull());
}

TEST(UUID, BigSet) {
  // create a bunch of uuids, insert into set, make sure no uuids collided
  //unsigned numUUIDS = 1000000;
  unsigned numUUIDS = 1000;
  std::set<UUID> uuids;

  for (unsigned i = 0; i < numUUIDS; ++i) {
    uuids.insert(createUUID());
  }

  // check no duplicates
  EXPECT_EQ(numUUIDS, uuids.size());
}

TEST(UUID, UUID_UID) {
  UUID uuid = createUUID();
  std::string uuidStr = toString(uuid);
  std::string uidStr = removeBraces(uuid);
  EXPECT_EQ("{" + uidStr + "}", uuidStr);
  EXPECT_EQ(uuid, toUUID(uuidStr));
  EXPECT_EQ(uuid, toUUID(uidStr));  // no extra conversion process
}
