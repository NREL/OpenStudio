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
#include "../Model_Impl.hpp"
#include "../DaylightingDeviceShelf.hpp"
#include "../DaylightingDeviceShelf_Impl.hpp"
#include "../Space.hpp"
#include "../Surface.hpp"
#include "../SubSurface.hpp"
#include "../ShadingSurfaceGroup.hpp"
#include "../ShadingSurface.hpp"
#include "../InteriorPartitionSurfaceGroup.hpp"
#include "../InteriorPartitionSurface.hpp"

#include "../../utilities/geometry/Point3d.hpp"

using namespace openstudio::model;
using namespace openstudio;

TEST_F(ModelFixture, DaylightingDeviceShelf)
{
  Model model;

  Point3dVector points;
  points.push_back(Point3d(0, 0, 1));
  points.push_back(Point3d(0, 0, 0));
  points.push_back(Point3d(0, 1, 0));
  points.push_back(Point3d(0, 1, 1));
  SubSurface window(points, model);
  EXPECT_EQ("FixedWindow", window.subSurfaceType());

  EXPECT_FALSE(window.daylightingDeviceShelf());

  DaylightingDeviceShelf shelf(window);
  UUID shelfHandle = shelf.handle();
  ASSERT_TRUE(window.daylightingDeviceShelf());
  EXPECT_EQ(shelfHandle, window.daylightingDeviceShelf()->handle());
  ASSERT_TRUE(window.addDaylightingDeviceShelf());
  EXPECT_EQ(shelfHandle, window.addDaylightingDeviceShelf()->handle());
  EXPECT_EQ(window.addDaylightingDeviceShelf()->handle(), window.daylightingDeviceShelf()->handle());
  EXPECT_EQ(window.handle(), shelf.subSurface().handle());

  shelf.remove();
  EXPECT_FALSE(window.daylightingDeviceShelf());
  ASSERT_TRUE(window.addDaylightingDeviceShelf());
  shelfHandle = window.addDaylightingDeviceShelf()->handle();
  ASSERT_TRUE(window.addDaylightingDeviceShelf());
  EXPECT_EQ(shelfHandle, window.addDaylightingDeviceShelf()->handle());
  ASSERT_TRUE(window.addDaylightingDeviceShelf());
  EXPECT_EQ(shelfHandle, window.addDaylightingDeviceShelf()->handle());
  ASSERT_TRUE(window.daylightingDeviceShelf());
  EXPECT_EQ(shelfHandle, window.daylightingDeviceShelf()->handle());

  // changing to door removes light shelf
  EXPECT_TRUE(window.setSubSurfaceType("Door"));
  EXPECT_EQ("Door", window.subSurfaceType());
  EXPECT_FALSE(window.daylightingDeviceShelf());
  EXPECT_FALSE(window.addDaylightingDeviceShelf());
}

TEST_F(ModelFixture, DaylightingDeviceShelf_Throw)
{
  Model model;

  Point3dVector points;
  points.push_back(Point3d(0, 0, 1));
  points.push_back(Point3d(0, 0, 0));
  points.push_back(Point3d(0, 1, 0));
  points.push_back(Point3d(0, 1, 1));
  SubSurface door(points, model);
  EXPECT_TRUE(door.setSubSurfaceType("Door"));
  EXPECT_EQ("Door", door.subSurfaceType());
  EXPECT_EQ(0, model.getConcreteModelObjects<DaylightingDeviceShelf>().size());

  bool didThrow = false;
  try{
    DaylightingDeviceShelf shelf(door);
  }catch(const openstudio::Exception&){
    didThrow = true;
  }
  EXPECT_TRUE(didThrow);
  EXPECT_EQ(0, model.getConcreteModelObjects<DaylightingDeviceShelf>().size());

  // change to a window
  EXPECT_TRUE(door.setSubSurfaceType("FixedWindow"));
  EXPECT_EQ("FixedWindow", door.subSurfaceType());

  // first one succeeds
  didThrow = false;
  try{
    DaylightingDeviceShelf shelf(door);
  }catch(const openstudio::Exception&){
    didThrow = true;
  }
  EXPECT_FALSE(didThrow);
  EXPECT_EQ(1, model.getConcreteModelObjects<DaylightingDeviceShelf>().size());

  // second call throws
  didThrow = false;
  try{
    DaylightingDeviceShelf shelf(door);
  }catch(const openstudio::Exception&){
    didThrow = true;
  }
  EXPECT_TRUE(didThrow);
  EXPECT_EQ(1, model.getConcreteModelObjects<DaylightingDeviceShelf>().size());

  // changing to door removes light shelf
  EXPECT_TRUE(door.setSubSurfaceType("Door"));
  EXPECT_EQ("Door", door.subSurfaceType());
  EXPECT_FALSE(door.daylightingDeviceShelf());
  EXPECT_FALSE(door.addDaylightingDeviceShelf());
  EXPECT_EQ(0, model.getConcreteModelObjects<DaylightingDeviceShelf>().size());
}
