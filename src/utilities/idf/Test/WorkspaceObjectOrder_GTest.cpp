/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2020, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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
  for (WorkspaceObjectVector::const_iterator it = objects.begin(), itEnd = objects.end() - 1; it != itEnd; ++it) {
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
