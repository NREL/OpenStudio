/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>
#include "../../idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>
#include "IdfFixture.hpp"
#include "../WorkspaceObject.hpp"
#include "../WorkspaceObjectOrder.hpp"

using namespace openstudio;

// Test in context of Workspace, since order needs objectGetter.
TEST_F(IdfFixture, WorkspaceObjectOrder) {
  Workspace workspace(IdfFixture::epIdfFile, openstudio::StrictnessLevel::Draft);

  WorkspaceObjectOrder wsOrder = workspace.order();

  EXPECT_TRUE(wsOrder.inOrder(workspace.objects()[0].handle()));

  // save current order
  OptionalHandleVector workspaceOrder = wsOrder.directOrder();
  ASSERT_TRUE(workspaceOrder);

  // order by enum
  wsOrder.setOrderByIddEnum();
  WorkspaceObjectVector objects = workspace.objects(true);
  for (auto it = objects.begin(), itEnd = objects.end() - 1; it != itEnd; ++it) {
    auto nxt = it;
    ++nxt;
    EXPECT_TRUE(it->iddObject().type() <= nxt->iddObject().type());
  }

  // restore order
  wsOrder.setDirectOrder(*workspaceOrder);
  HandleVector handles = workspace.handles(true);
  // handles does not include version object, while direct order does
  HandleVector tempOrder = *workspaceOrder;
  auto it = std::find(tempOrder.begin(), tempOrder.end(), workspace.versionObject()->handle());
  tempOrder.erase(it);
  EXPECT_TRUE(tempOrder == handles);

  // move objects directly
  wsOrder.insert(handles[32], handles[12]);
  HandleVector newOrder = workspace.handles(true);
  EXPECT_EQ(handles.size(), newOrder.size());
  EXPECT_TRUE(handles[32] == newOrder[12]);
  EXPECT_TRUE(handles[12] == newOrder[13]);

  wsOrder.swap(handles[80], handles[100]);
  newOrder = workspace.handles(true);
  EXPECT_EQ(handles.size(), newOrder.size());
  EXPECT_TRUE(handles[80] == newOrder[100]);
  EXPECT_TRUE(handles[100] == newOrder[80]);
}

TEST_F(IdfFixture, WorkspaceObjectOrder_ByIddObjectType) {
  Workspace workspace(IdfFixture::epIdfFile, openstudio::StrictnessLevel::Draft);
  WorkspaceObjectVector objectsInOriginalOrder = workspace.objects(true);

  WorkspaceObjectOrder wsOrder = workspace.order();
  IddObjectTypeVector orderByType;
  IntSet enumValues = IddObjectType::getValues();
  for (int val : enumValues) {
    orderByType.push_back(IddObjectType(val));
  }
  wsOrder.setIddOrder(orderByType);
  WorkspaceObjectVector objectsInNewOrder = workspace.objects(true);
  EXPECT_EQ(objectsInOriginalOrder.size(), objectsInNewOrder.size());
  EXPECT_FALSE(objectsInOriginalOrder == objectsInNewOrder);

  // expect Materials before Constructions
  OptionalWorkspaceObject oMaterial;
  OptionalWorkspaceObject oConstruction;
  for (const WorkspaceObject& object : objectsInNewOrder) {
    if (!oMaterial && (object.iddObject().type() == IddObjectType::Material)) {
      oMaterial = object;
      EXPECT_FALSE(oConstruction);
      OptionalUnsigned oIndex = wsOrder.indexInOrder(object.handle());
      EXPECT_FALSE(oIndex);
    }
    if (!oConstruction && (object.iddObject().type() == IddObjectType::Construction)) {
      oConstruction = object;
    }
    if (oMaterial && oConstruction) {
      break;
    }
  }

  // change to Constructions before Materials
  wsOrder.move(IddObjectType::Construction, IddObjectType::Material);
  objectsInNewOrder = workspace.objects(true);
  oMaterial = boost::none;
  oConstruction = boost::none;
  for (const WorkspaceObject& object : objectsInNewOrder) {
    if (!oMaterial && (object.iddObject().type() == IddObjectType::Material)) {
      oMaterial = object;
    }
    if (!oConstruction && (object.iddObject().type() == IddObjectType::Construction)) {
      oConstruction = object;
      EXPECT_FALSE(oMaterial);
    }
    if (oMaterial && oConstruction) {
      break;
    }
  }
}
