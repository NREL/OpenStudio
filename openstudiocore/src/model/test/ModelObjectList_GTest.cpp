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
