/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

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

TEST_F(ModelFixture, DaylightingDeviceShelf) {
  Model model;

  Point3dVector points{
    {0, 0, 1},
    {0, 0, 0},
    {0, 1, 0},
    {0, 1, 1},
  };
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

TEST_F(ModelFixture, DaylightingDeviceShelf_Throw) {
  Model model;

  Point3dVector points{
    {0, 0, 1},
    {0, 0, 0},
    {0, 1, 0},
    {0, 1, 1},
  };
  SubSurface door(points, model);
  EXPECT_TRUE(door.setSubSurfaceType("Door"));
  EXPECT_EQ("Door", door.subSurfaceType());
  EXPECT_EQ(0, model.getConcreteModelObjects<DaylightingDeviceShelf>().size());

  bool didThrow = false;
  try {
    DaylightingDeviceShelf shelf(door);
  } catch (const openstudio::Exception&) {
    didThrow = true;
  }
  EXPECT_TRUE(didThrow);
  EXPECT_EQ(0, model.getConcreteModelObjects<DaylightingDeviceShelf>().size());

  // change to a window
  EXPECT_TRUE(door.setSubSurfaceType("FixedWindow"));
  EXPECT_EQ("FixedWindow", door.subSurfaceType());

  // first one succeeds
  didThrow = false;
  try {
    DaylightingDeviceShelf shelf(door);
  } catch (const openstudio::Exception&) {
    didThrow = true;
  }
  EXPECT_FALSE(didThrow);
  EXPECT_EQ(1, model.getConcreteModelObjects<DaylightingDeviceShelf>().size());

  // second call throws
  didThrow = false;
  try {
    DaylightingDeviceShelf shelf(door);
  } catch (const openstudio::Exception&) {
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
