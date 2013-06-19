/**********************************************************************
*  Copyright (c) 2008-2013, Alliance for Sustainable Energy.
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

#include <model/test/ModelFixture.hpp>
#include <model/Model.hpp>
#include <model/Node.hpp>
#include <model/Node_Impl.hpp>
#include <model/Building.hpp>
#include <model/Building_Impl.hpp>
#include <model/ModelObject_Impl.hpp>
#include <model/StraightComponent_Impl.hpp>

using namespace openstudio::model;

TEST_F(ModelFixture, Node_Casting)
{
  Model model;

  Node node = Node(model);
  EXPECT_NO_THROW(node.cast<Node>());
  EXPECT_NO_THROW(node.cast<StraightComponent>());
  EXPECT_NO_THROW(node.cast<ModelObject>());
  EXPECT_THROW(node.cast<Building>(), std::bad_cast);

  EXPECT_TRUE(node.optionalCast<Node>());
  EXPECT_TRUE(node.optionalCast<StraightComponent>());
  EXPECT_TRUE(node.optionalCast<ModelObject>());
  EXPECT_FALSE(node.optionalCast<Building>());

  ModelObject obj = Node(model);
  EXPECT_NO_THROW(obj.cast<Node>());
  EXPECT_NO_THROW(obj.cast<StraightComponent>());
  EXPECT_NO_THROW(obj.cast<ModelObject>());
  EXPECT_THROW(obj.cast<Building>(), std::bad_cast);

  EXPECT_TRUE(obj.optionalCast<Node>());
  EXPECT_TRUE(obj.optionalCast<StraightComponent>());
  EXPECT_TRUE(obj.optionalCast<ModelObject>());
  EXPECT_FALSE(obj.optionalCast<Building>());
}
