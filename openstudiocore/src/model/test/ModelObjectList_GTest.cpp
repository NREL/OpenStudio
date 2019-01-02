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

#include "../ModelObjectList.hpp"
#include "../ModelObjectList_Impl.hpp"
#include "../Connection.hpp"
#include "../Connection_Impl.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, ModelObjectList_DefaultConstructor)
{
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  ASSERT_EXIT (
  {
    Model model;
    ModelObjectList testObject = ModelObjectList(model);

    exit(0);
  } ,
    ::testing::ExitedWithCode(0), "" );
}

TEST_F(ModelFixture, ModelObjectList_Remove)
{
  Model model;
  ModelObjectList testObject = ModelObjectList(model);
  Connection connection = Connection(model);

  testObject.addModelObject(connection);

  std::vector<ModelObjectList> modelObjectLists = model.getModelObjects<ModelObjectList>();
  EXPECT_EQ(1, modelObjectLists.size());

  std::vector<Connection> modelObjects = model.getModelObjects<Connection>();
  EXPECT_EQ(1, modelObjects.size());

  testObject.remove();

  modelObjectLists = model.getModelObjects<ModelObjectList>();
  EXPECT_EQ(0, modelObjectLists.size());

  modelObjects = model.getModelObjects<Connection>();
  EXPECT_EQ(0, modelObjects.size());
}

TEST_F(ModelFixture, ModelObjectList_CloneOneModel)
{
  Model model;
  ModelObjectList testObject = ModelObjectList(model);
  Connection connection = Connection(model);

  testObject.addModelObject(connection);

  std::vector<ModelObject> modelObjects = testObject.modelObjects();

  ModelObjectList testObjectClone = testObject.clone(model).cast<ModelObjectList>();
  std::vector<ModelObject> modelObjectsClone = testObjectClone.modelObjects();
  std::vector<Connection> connObjects = model.getModelObjects<Connection>();
  EXPECT_EQ(2, connObjects.size());
  EXPECT_EQ(1, modelObjectsClone.size());
  EXPECT_NE(modelObjectsClone[0].handle(), modelObjects[0].handle());
}

TEST_F(ModelFixture, ModelObjectList_CloneTwoModels)
{
  Model model;
  ModelObjectList testObject = ModelObjectList(model);
  Connection connection = Connection(model);

  testObject.addModelObject(connection);

  std::vector<ModelObject> modelObjects = testObject.modelObjects();

  ModelObjectList testObjectClone = testObject.clone(model).cast<ModelObjectList>();

  Model model2;

  ModelObjectList testObjectClone2 = testObject.clone(model2).cast<ModelObjectList>();
  std::vector<ModelObject> modelObjectsClone2 = testObjectClone2.modelObjects();
  std::vector<Connection> connObjects = model.getModelObjects<Connection>();
  std::vector<Connection> connObjects2 = model2.getModelObjects<Connection>();
  EXPECT_EQ(2, connObjects.size());
  EXPECT_EQ(1, connObjects2.size());
  EXPECT_EQ(1, modelObjectsClone2.size());
  EXPECT_NE(testObjectClone2, testObjectClone);
  EXPECT_NE(testObjectClone2.handle(), testObjectClone.handle());
  EXPECT_NE(modelObjects[0].handle(), modelObjectsClone2[0].handle());
}

TEST_F(ModelFixture, ModelObjectList_ModelObjects)
{
  Model model;
  ModelObjectList testObject = ModelObjectList(model);

  std::vector<ModelObject> modelObjects = testObject.modelObjects();
  EXPECT_TRUE(modelObjects.empty());
}

TEST_F(ModelFixture, ModelObjectList_AddModelObject)
{
  Model model;
  ModelObjectList testObject = ModelObjectList(model);
  Connection connection = Connection(model);

  EXPECT_TRUE(testObject.addModelObject(connection));

  std::vector<ModelObject> modelObjects = testObject.modelObjects();
  EXPECT_EQ(1, modelObjects.size());
}

TEST_F(ModelFixture, ModelObjectList_RemoveModelObject)
{
  Model model;
  ModelObjectList testObject = ModelObjectList(model);
  Connection connection1 = Connection(model);
  Connection connection2 = Connection(model);

  testObject.addModelObject(connection1);
  testObject.addModelObject(connection2);
  testObject.removeModelObject(connection1);

  std::vector<Connection> testModelObjects = model.getModelObjects<Connection>();
  EXPECT_EQ(2, testModelObjects.size());

  std::vector<ModelObject> modelObjects = testObject.modelObjects();
  EXPECT_EQ(1, modelObjects.size());
  EXPECT_EQ(connection2.handle(), modelObjects[0].handle());

  testObject.removeModelObject(connection2);
  testModelObjects = model.getModelObjects<Connection>();
  modelObjects = testObject.modelObjects();
  EXPECT_EQ(2, testModelObjects.size());
  EXPECT_TRUE(modelObjects.empty());
}

TEST_F(ModelFixture, ModelObjectList_RemoveAllModelObjects)
{
  Model model;
  ModelObjectList testObject = ModelObjectList(model);
  Connection connection1 = Connection(model);
  Connection connection2 = Connection(model);

  testObject.addModelObject(connection1);
  testObject.addModelObject(connection2);
  testObject.removeAllModelObjects();

  std::vector<Connection> testModelObjects = model.getModelObjects<Connection>();
  EXPECT_EQ(2, testModelObjects.size());

  std::vector<ModelObject> modelObjects = testObject.modelObjects();
  EXPECT_TRUE(modelObjects.empty());
}
