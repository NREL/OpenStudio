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
