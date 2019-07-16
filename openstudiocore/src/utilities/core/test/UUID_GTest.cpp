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
  UUID uuid4 = toUUID(std::string("Not a UUID"));
  EXPECT_TRUE(uuid4.isNull());
}

TEST(UUID, BigSet)
{
  // create a bunch of uuids, insert into set, make sure no uuids collided
  //unsigned numUUIDS = 1000000;
  unsigned numUUIDS = 1000;
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
