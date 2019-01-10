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
#include "ModelFixture.hpp"
#include "../MeterCustomDecrement.hpp"
#include "../MeterCustomDecrement_Impl.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, MeterCustomDecrement_DefaultConstructor)
{
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  ASSERT_EXIT (
  {
    Model model;
    MeterCustomDecrement testObject(model, "Electricity:Facility");

    exit(0);
  } ,
    ::testing::ExitedWithCode(0), "" );
}


TEST_F(ModelFixture, MeterCustomDecrement_Remove)
{
  Model model;
  MeterCustomDecrement testObject(model, "Electricity:Facility");

  EXPECT_EQ((unsigned)1, model.getModelObjects<MeterCustomDecrement>().size());

  testObject.remove();
  EXPECT_EQ((unsigned)0, model.getModelObjects<MeterCustomDecrement>().size());
}

TEST_F(ModelFixture, MeterCustomDecrement_KeyVarGroups)
{
  Model model;
  MeterCustomDecrement testObject(model, "Electricity:Facility");

  EXPECT_EQ("Electricity:Facility", testObject.sourceMeterName());
  testObject.setSourceMeterName("Electricity:Building");
  EXPECT_EQ("Electricity:Building", testObject.sourceMeterName());

  std::vector< std::pair<std::string,std::string> > keyVarGroups = testObject.keyVarGroups();

  EXPECT_TRUE(testObject.addKeyVarGroup("SPACE1-1 Lights 1", "Lights Electric Energy"));
  keyVarGroups = testObject.keyVarGroups();
  EXPECT_EQ(1, keyVarGroups.size());
  // Also test the numKeyVarGroups method
  EXPECT_EQ(1, testObject.numKeyVarGroups());

  testObject.removeAllKeyVarGroups();
  EXPECT_EQ(0, testObject.numKeyVarGroups());

  EXPECT_TRUE(testObject.addKeyVarGroup("SPACE1-1 Lights 1", "Lights Electric Energy"));
  EXPECT_EQ(1, testObject.numKeyVarGroups());

  EXPECT_TRUE(testObject.addKeyVarGroup("SPACE1-1 Equipment 1", "Equipment Electric Energy"));
  EXPECT_EQ(2, testObject.numKeyVarGroups());

  // Try removing at index 0
  testObject.removeKeyVarGroup(0);
  keyVarGroups = testObject.keyVarGroups();
  EXPECT_EQ(1, keyVarGroups.size());

  EXPECT_EQ("SPACE1-1 Equipment 1", keyVarGroups[0].first);
  EXPECT_EQ("Equipment Electric Energy", keyVarGroups[0].second);

  // Try changing the keyName at index 0
  testObject.setKeyName(0, "A new key name");
  keyVarGroups = testObject.keyVarGroups();
  EXPECT_EQ("A new key name", keyVarGroups[0].first);

  // Try changing the setOutputVariableorMeterName at index 0
  testObject.setOutputVariableorMeterName(0, "A new output var name");
  keyVarGroups = testObject.keyVarGroups();
  EXPECT_EQ("A new output var name", keyVarGroups[0].second);


  testObject.removeAllKeyVarGroups();
  for (int i=0; i<100; i++) {
    testObject.addKeyVarGroup("key " + std::to_string(i), "var " + std::to_string(i));
  }
  keyVarGroups = testObject.keyVarGroups();
  EXPECT_EQ(100, keyVarGroups.size());
  EXPECT_EQ(100, testObject.numKeyVarGroups());

}
