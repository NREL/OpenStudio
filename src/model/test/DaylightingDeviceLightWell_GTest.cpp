/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>

#include "ModelFixture.hpp"
#include "../Model_Impl.hpp"
#include "../DaylightingDeviceLightWell.hpp"
#include "../DaylightingDeviceLightWell_Impl.hpp"
#include "../SubSurface.hpp"

#include "../../utilities/geometry/Point3d.hpp"

using namespace openstudio::model;
using namespace openstudio;

TEST_F(ModelFixture, DaylightingDeviceLightWell) {
  Model model;

  Point3dVector points{
    {0, 0, 1},
    {0, 0, 0},
    {0, 1, 0},
    {0, 1, 1},
  };
  SubSurface window(points, model);
  EXPECT_EQ("FixedWindow", window.subSurfaceType());

  EXPECT_FALSE(window.daylightingDeviceLightWell());

  EXPECT_EQ(0u, model.getConcreteModelObjects<DaylightingDeviceLightWell>().size());
  DaylightingDeviceLightWell lightWell(window, 1, 2, 3, 0.75);
  UUID lightWellHandle = lightWell.handle();
  ASSERT_TRUE(window.daylightingDeviceLightWell());
  EXPECT_EQ(lightWellHandle, window.daylightingDeviceLightWell()->handle());
  ASSERT_TRUE(window.addDaylightingDeviceLightWell());
  EXPECT_EQ(lightWellHandle, window.addDaylightingDeviceLightWell()->handle());
  EXPECT_EQ(window.addDaylightingDeviceLightWell()->handle(), window.daylightingDeviceLightWell()->handle());
  EXPECT_EQ(window.handle(), lightWell.subSurface().handle());
  EXPECT_EQ(1u, model.getConcreteModelObjects<DaylightingDeviceLightWell>().size());

  lightWell.remove();
  EXPECT_EQ(0u, model.getConcreteModelObjects<DaylightingDeviceLightWell>().size());

  EXPECT_FALSE(window.daylightingDeviceLightWell());
  ASSERT_TRUE(window.addDaylightingDeviceLightWell());
  lightWellHandle = window.addDaylightingDeviceLightWell()->handle();
  EXPECT_EQ(1u, model.getConcreteModelObjects<DaylightingDeviceLightWell>().size());
  // Calling it a second time while a Light Well already exists returns the existing one, doesn't create a second
  ASSERT_TRUE(window.addDaylightingDeviceLightWell());
  EXPECT_EQ(lightWellHandle, window.addDaylightingDeviceLightWell()->handle());
  EXPECT_EQ(1u, model.getConcreteModelObjects<DaylightingDeviceLightWell>().size());

  ASSERT_TRUE(window.addDaylightingDeviceLightWell());
  EXPECT_EQ(lightWellHandle, window.addDaylightingDeviceLightWell()->handle());
  ASSERT_TRUE(window.daylightingDeviceLightWell());
  EXPECT_EQ(lightWellHandle, window.daylightingDeviceLightWell()->handle());

  // changing to door removes light light well
  EXPECT_TRUE(window.setSubSurfaceType("Door"));
  EXPECT_EQ("Door", window.subSurfaceType());
  EXPECT_FALSE(window.daylightingDeviceLightWell());
  EXPECT_FALSE(window.addDaylightingDeviceLightWell());
  EXPECT_EQ(0u, model.getConcreteModelObjects<DaylightingDeviceLightWell>().size());
}

TEST_F(ModelFixture, DaylightingDeviceLightWell_Throw) {
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
  EXPECT_EQ(0, model.getConcreteModelObjects<DaylightingDeviceLightWell>().size());

  EXPECT_ANY_THROW({ DaylightingDeviceLightWell lightWell(door); });
  EXPECT_EQ(0, model.getConcreteModelObjects<DaylightingDeviceLightWell>().size());

  // change to a window
  EXPECT_TRUE(door.setSubSurfaceType("FixedWindow"));
  EXPECT_EQ("FixedWindow", door.subSurfaceType());

  // first one succeeds
  EXPECT_NO_THROW({ DaylightingDeviceLightWell lightWell(door); });
  EXPECT_EQ(1, model.getConcreteModelObjects<DaylightingDeviceLightWell>().size());

  // second call throws
  EXPECT_ANY_THROW({ DaylightingDeviceLightWell lightWell(door); });
  EXPECT_EQ(1, model.getConcreteModelObjects<DaylightingDeviceLightWell>().size());

  // changing to door removes light light well
  EXPECT_TRUE(door.setSubSurfaceType("Door"));
  EXPECT_EQ("Door", door.subSurfaceType());
  EXPECT_FALSE(door.daylightingDeviceLightWell());
  EXPECT_FALSE(door.addDaylightingDeviceLightWell());
  EXPECT_EQ(0, model.getConcreteModelObjects<DaylightingDeviceLightWell>().size());
}

TEST_F(ModelFixture, DaylightingDeviceLightWell_Throw2) {
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
  EXPECT_EQ(0, model.getConcreteModelObjects<DaylightingDeviceLightWell>().size());

  bool didThrow = false;
  try {
    DaylightingDeviceLightWell lightWell(door);
  } catch (const openstudio::Exception&) {
    didThrow = true;
  }
  EXPECT_TRUE(didThrow);
  EXPECT_EQ(0, model.getConcreteModelObjects<DaylightingDeviceLightWell>().size());

  // change to a window
  EXPECT_TRUE(door.setSubSurfaceType("FixedWindow"));
  EXPECT_EQ("FixedWindow", door.subSurfaceType());

  // first one succeeds
  didThrow = false;
  try {
    DaylightingDeviceLightWell lightWell(door);
  } catch (const openstudio::Exception&) {
    didThrow = true;
  }
  EXPECT_FALSE(didThrow);
  EXPECT_EQ(1, model.getConcreteModelObjects<DaylightingDeviceLightWell>().size());

  // second call throws
  didThrow = false;
  try {
    DaylightingDeviceLightWell lightWell(door);
  } catch (const openstudio::Exception&) {
    didThrow = true;
  }
  EXPECT_TRUE(didThrow);
  EXPECT_EQ(1, model.getConcreteModelObjects<DaylightingDeviceLightWell>().size());

  // changing to door removes light light well
  EXPECT_TRUE(door.setSubSurfaceType("Door"));
  EXPECT_EQ("Door", door.subSurfaceType());
  EXPECT_FALSE(door.daylightingDeviceLightWell());
  EXPECT_FALSE(door.addDaylightingDeviceLightWell());
  EXPECT_EQ(0, model.getConcreteModelObjects<DaylightingDeviceLightWell>().size());
}

TEST_F(ModelFixture, DaylightingDeviceLightWell_SettersGetters) {
  Model model;

  Point3dVector points{
    {0, 0, 1},
    {0, 0, 0},
    {0, 1, 0},
    {0, 1, 1},
  };
  SubSurface window(points, model);
  EXPECT_EQ("FixedWindow", window.subSurfaceType());

  DaylightingDeviceLightWell lightWell(window);

  EXPECT_EQ(window, lightWell.subSurface());
  EXPECT_EQ(1.2, lightWell.heightofWell());
  EXPECT_EQ(12.0, lightWell.perimeterofBottomofWell());
  EXPECT_EQ(9.0, lightWell.areaofBottomofWell());
  EXPECT_EQ(0.7, lightWell.visibleReflectanceofWellWalls());

  EXPECT_TRUE(lightWell.setHeightofWell(1));
  EXPECT_TRUE(lightWell.setPerimeterofBottomofWell(2));
  EXPECT_TRUE(lightWell.setAreaofBottomofWell(3));
  EXPECT_TRUE(lightWell.setVisibleReflectanceofWellWalls(0.9));

  EXPECT_EQ(1, lightWell.heightofWell());
  EXPECT_EQ(2, lightWell.perimeterofBottomofWell());
  EXPECT_EQ(3, lightWell.areaofBottomofWell());
  EXPECT_EQ(0.9, lightWell.visibleReflectanceofWellWalls());
}
