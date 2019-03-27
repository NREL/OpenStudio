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
