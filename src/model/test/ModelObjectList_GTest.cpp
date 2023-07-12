/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>

#include "ModelFixture.hpp"

#include "../ModelObjectList.hpp"
#include "../ModelObjectList_Impl.hpp"
#include "../Connection.hpp"
#include "../Connection_Impl.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, ModelObjectList_DefaultConstructor) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  ASSERT_EXIT(
    {
      Model model;
      ModelObjectList testObject = ModelObjectList(model);

      exit(0);
    },
    ::testing::ExitedWithCode(0), "");
}

TEST_F(ModelFixture, ModelObjectList_Remove) {
  Model model;
  ModelObjectList testObject = ModelObjectList(model);
  Connection connection = Connection(model);

  testObject.addModelObject(connection);

  std::vector<ModelObjectList> modelObjectLists = model.getConcreteModelObjects<ModelObjectList>();
  EXPECT_EQ(1, modelObjectLists.size());

  std::vector<Connection> modelObjects = model.getConcreteModelObjects<Connection>();
  EXPECT_EQ(1, modelObjects.size());

  testObject.remove();

  modelObjectLists = model.getConcreteModelObjects<ModelObjectList>();
  EXPECT_EQ(0, modelObjectLists.size());

  modelObjects = model.getConcreteModelObjects<Connection>();
  EXPECT_EQ(0, modelObjects.size());
}

TEST_F(ModelFixture, ModelObjectList_CloneOneModel) {
  Model model;
  ModelObjectList testObject = ModelObjectList(model);
  Connection connection = Connection(model);

  testObject.addModelObject(connection);

  std::vector<ModelObject> modelObjects = testObject.modelObjects();

  auto testObjectClone = testObject.clone(model).cast<ModelObjectList>();
  std::vector<ModelObject> modelObjectsClone = testObjectClone.modelObjects();
  std::vector<Connection> connObjects = model.getConcreteModelObjects<Connection>();
  EXPECT_EQ(2, connObjects.size());
  EXPECT_EQ(1, modelObjectsClone.size());
  EXPECT_NE(modelObjectsClone[0].handle(), modelObjects[0].handle());
}

TEST_F(ModelFixture, ModelObjectList_CloneTwoModels) {
  Model model;
  ModelObjectList testObject = ModelObjectList(model);
  Connection connection = Connection(model);

  testObject.addModelObject(connection);

  std::vector<ModelObject> modelObjects = testObject.modelObjects();

  auto testObjectClone = testObject.clone(model).cast<ModelObjectList>();

  Model model2;

  auto testObjectClone2 = testObject.clone(model2).cast<ModelObjectList>();
  std::vector<ModelObject> modelObjectsClone2 = testObjectClone2.modelObjects();
  std::vector<Connection> connObjects = model.getConcreteModelObjects<Connection>();
  std::vector<Connection> connObjects2 = model2.getConcreteModelObjects<Connection>();
  EXPECT_EQ(2, connObjects.size());
  EXPECT_EQ(1, connObjects2.size());
  EXPECT_EQ(1, modelObjectsClone2.size());
  EXPECT_NE(testObjectClone2, testObjectClone);
  EXPECT_NE(testObjectClone2.handle(), testObjectClone.handle());
  EXPECT_NE(modelObjects[0].handle(), modelObjectsClone2[0].handle());
}

TEST_F(ModelFixture, ModelObjectList_ModelObjects) {
  Model model;
  ModelObjectList testObject = ModelObjectList(model);

  std::vector<ModelObject> modelObjects = testObject.modelObjects();
  EXPECT_TRUE(modelObjects.empty());
}

TEST_F(ModelFixture, ModelObjectList_AddModelObject) {
  Model model;
  ModelObjectList testObject = ModelObjectList(model);
  Connection connection = Connection(model);

  EXPECT_TRUE(testObject.addModelObject(connection));

  std::vector<ModelObject> modelObjects = testObject.modelObjects();
  EXPECT_EQ(1, modelObjects.size());
}

TEST_F(ModelFixture, ModelObjectList_RemoveModelObject) {
  Model model;
  ModelObjectList testObject = ModelObjectList(model);
  Connection connection1 = Connection(model);
  Connection connection2 = Connection(model);

  testObject.addModelObject(connection1);
  testObject.addModelObject(connection2);
  testObject.removeModelObject(connection1);

  std::vector<Connection> testModelObjects = model.getConcreteModelObjects<Connection>();
  EXPECT_EQ(2, testModelObjects.size());

  std::vector<ModelObject> modelObjects = testObject.modelObjects();
  EXPECT_EQ(1, modelObjects.size());
  EXPECT_EQ(connection2.handle(), modelObjects[0].handle());

  testObject.removeModelObject(connection2);
  testModelObjects = model.getConcreteModelObjects<Connection>();
  modelObjects = testObject.modelObjects();
  EXPECT_EQ(2, testModelObjects.size());
  EXPECT_TRUE(modelObjects.empty());
}

TEST_F(ModelFixture, ModelObjectList_RemoveAllModelObjects) {
  Model model;
  ModelObjectList testObject = ModelObjectList(model);
  Connection connection1 = Connection(model);
  Connection connection2 = Connection(model);

  testObject.addModelObject(connection1);
  testObject.addModelObject(connection2);
  testObject.removeAllModelObjects();

  std::vector<Connection> testModelObjects = model.getConcreteModelObjects<Connection>();
  EXPECT_EQ(2, testModelObjects.size());

  std::vector<ModelObject> modelObjects = testObject.modelObjects();
  EXPECT_TRUE(modelObjects.empty());
}
