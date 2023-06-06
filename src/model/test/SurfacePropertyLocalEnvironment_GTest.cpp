/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "ModelFixture.hpp"

#include "../SurfacePropertyLocalEnvironment.hpp"
#include "../SurfacePropertyLocalEnvironment_Impl.hpp"
#include "../Surface.hpp"
#include "../SubSurface.hpp"
#include "../Schedule.hpp"
#include "../ScheduleConstant.hpp"
#include "../SurfacePropertySurroundingSurfaces.hpp"
#include "../SurfacePropertyGroundSurfaces.hpp"

#include "../../utilities/geometry/Point3d.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, SurfacePropertyLocalEnvironment_GettersSetters) {
  Model m;

  double w = 10.0;
  std::vector<Point3d> points{
    {0, 0, 0},
    {0, w, 0},
    {w, w, 0},
    {w, 0, 0},
  };

  Surface surface(points, m);

  SurfacePropertyLocalEnvironment surfacePropertyLocalEnvironment(surface);
  surfacePropertyLocalEnvironment.setName("My SurfacePropertyLocalEnvironment");

  // Exterior Surface Name: Required Object
  EXPECT_EQ(surface, surfacePropertyLocalEnvironment.exteriorSurface());
  std::vector<Point3d> points2{
    {0, 0, 2},
    {0, w, 2},
    {w, w, 2},
    {w, 0, 2},
  };
  Surface surface2(points2, m);
  EXPECT_FALSE(surface2.surfacePropertyLocalEnvironment());

  EXPECT_TRUE(surfacePropertyLocalEnvironment.setExteriorSurface(surface2));
  EXPECT_EQ(surface2, surfacePropertyLocalEnvironment.exteriorSurface());
  ASSERT_TRUE(surfacePropertyLocalEnvironment.exteriorSurfaceAsSurface());
  EXPECT_EQ(surface2, surfacePropertyLocalEnvironment.exteriorSurfaceAsSurface().get());
  EXPECT_FALSE(surfacePropertyLocalEnvironment.exteriorSurfaceAsSubSurface());

  ASSERT_TRUE(surface2.surfacePropertyLocalEnvironment());
  EXPECT_EQ(surfacePropertyLocalEnvironment, surface2.surfacePropertyLocalEnvironment().get());

  // External Shading Fraction Schedule Name: Optional Object
  EXPECT_FALSE(surfacePropertyLocalEnvironment.externalShadingFractionSchedule());
  ScheduleConstant shadingSch(m);
  EXPECT_TRUE(surfacePropertyLocalEnvironment.setExternalShadingFractionSchedule(shadingSch));
  ASSERT_TRUE(surfacePropertyLocalEnvironment.externalShadingFractionSchedule());
  EXPECT_EQ(shadingSch, surfacePropertyLocalEnvironment.externalShadingFractionSchedule().get());
  surfacePropertyLocalEnvironment.resetExternalShadingFractionSchedule();
  EXPECT_FALSE(surfacePropertyLocalEnvironment.externalShadingFractionSchedule());

  // Surrounding Surfaces Object Name: Optional Object
  {
    EXPECT_FALSE(surfacePropertyLocalEnvironment.surfacePropertySurroundingSurfaces());
    SurfacePropertySurroundingSurfaces sp(m);
    EXPECT_FALSE(sp.surfacePropertyLocalEnvironment());

    EXPECT_TRUE(surfacePropertyLocalEnvironment.setSurfacePropertySurroundingSurfaces(sp));
    ASSERT_TRUE(surfacePropertyLocalEnvironment.surfacePropertySurroundingSurfaces());
    EXPECT_EQ(sp, surfacePropertyLocalEnvironment.surfacePropertySurroundingSurfaces().get());

    ASSERT_TRUE(sp.surfacePropertyLocalEnvironment());
    EXPECT_EQ(surfacePropertyLocalEnvironment, sp.surfacePropertyLocalEnvironment().get());

    surfacePropertyLocalEnvironment.resetSurfacePropertySurroundingSurfaces();
    EXPECT_FALSE(surfacePropertyLocalEnvironment.surfacePropertySurroundingSurfaces());
  }

  // Ground Surfaces Object Name: Optional Object
  {
    EXPECT_FALSE(surfacePropertyLocalEnvironment.surfacePropertyGroundSurfaces());
    SurfacePropertyGroundSurfaces sp(m);
    EXPECT_FALSE(sp.surfacePropertyLocalEnvironment());

    EXPECT_TRUE(surfacePropertyLocalEnvironment.setSurfacePropertyGroundSurfaces(sp));
    ASSERT_TRUE(surfacePropertyLocalEnvironment.surfacePropertyGroundSurfaces());
    EXPECT_EQ(sp, surfacePropertyLocalEnvironment.surfacePropertyGroundSurfaces().get());

    ASSERT_TRUE(sp.surfacePropertyLocalEnvironment());
    EXPECT_EQ(surfacePropertyLocalEnvironment, sp.surfacePropertyLocalEnvironment().get());

    surfacePropertyLocalEnvironment.resetSurfacePropertyGroundSurfaces();
    EXPECT_FALSE(surfacePropertyLocalEnvironment.surfacePropertyGroundSurfaces());
  }

  // It should also accept a SubSurface
  SubSurface subSurface(points2, m);
  EXPECT_TRUE(surface2.surfacePropertyLocalEnvironment());
  EXPECT_FALSE(subSurface.surfacePropertyLocalEnvironment());

  EXPECT_TRUE(surfacePropertyLocalEnvironment.setExteriorSurface(subSurface));

  EXPECT_EQ(subSurface, surfacePropertyLocalEnvironment.exteriorSurface());
  ASSERT_TRUE(surfacePropertyLocalEnvironment.exteriorSurfaceAsSubSurface());
  EXPECT_EQ(subSurface, surfacePropertyLocalEnvironment.exteriorSurfaceAsSubSurface().get());
  EXPECT_FALSE(surfacePropertyLocalEnvironment.exteriorSurfaceAsSurface());

  EXPECT_FALSE(surface2.surfacePropertyLocalEnvironment());
  ASSERT_TRUE(subSurface.surfacePropertyLocalEnvironment());
  EXPECT_EQ(surfacePropertyLocalEnvironment, subSurface.surfacePropertyLocalEnvironment().get());

  // Try to create a **second** LocalEnv for the **same** object: it should delete the initial one, to enforce unicity
  EXPECT_EQ(1, m.getConcreteModelObjects<SurfacePropertyLocalEnvironment>().size());
  SurfacePropertyLocalEnvironment surfacePropertyLocalEnvironment2(subSurface);
  EXPECT_EQ(1, m.getConcreteModelObjects<SurfacePropertyLocalEnvironment>().size());
  EXPECT_EQ(surfacePropertyLocalEnvironment2, m.getConcreteModelObjects<SurfacePropertyLocalEnvironment>()[0]);

  // Outdoor Air Node Name: Optional Object
  // boost::optional<OutdoorAirNode> obj(m);
  // EXPECT_TRUE(surfacePropertyLocalEnvironment.setOutdoorAirNode(obj));
  // ASSERT_TRUE(surfacePropertyLocalEnvironment.outdoorAirNode());
  // EXPECT_EQ(obj, surfacePropertyLocalEnvironment.outdoorAirNode().get());
}
