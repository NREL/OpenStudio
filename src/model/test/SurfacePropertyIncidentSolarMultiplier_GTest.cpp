/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "ModelFixture.hpp"

#include "../SurfacePropertyIncidentSolarMultiplier.hpp"
#include "../SurfacePropertyIncidentSolarMultiplier_Impl.hpp"

#include "../SubSurface.hpp"

#include "../Schedule.hpp"
#include "../ScheduleConstant.hpp"

#include "../../utilities/geometry/Point3d.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, SurfacePropertyIncidentSolarMultiplier_GettersSetters) {
  Model m;

  std::vector<Point3d> vertices{{0, 0, 1}, {0, 0, 0}, {1, 0, 0}, {1, 0, 1}};

  SubSurface ss(vertices, m);

  SurfacePropertyIncidentSolarMultiplier sp(ss);

  EXPECT_EQ(1, sp.incidentSolarMultiplier());
  EXPECT_FALSE(sp.incidentSolarMultiplierSchedule());
  EXPECT_EQ(ss, sp.subSurface());
  Schedule alwaysOnContinuous = m.alwaysOnContinuousSchedule();

  // Surface Name: Required Object
  SubSurface ss2(vertices, m);
  EXPECT_TRUE(sp.setSubSurface(ss2));
  EXPECT_EQ(ss2, sp.subSurface());
  ASSERT_TRUE(ss2.surfacePropertyIncidentSolarMultiplier());
  EXPECT_EQ(sp, ss2.surfacePropertyIncidentSolarMultiplier().get());

  // Incident Solar Multiplier: Required Double
  EXPECT_TRUE(sp.setIncidentSolarMultiplier(0.75));
  EXPECT_EQ(0.75, sp.incidentSolarMultiplier());
  // Bad Value
  EXPECT_FALSE(sp.setIncidentSolarMultiplier(-10.0));
  EXPECT_EQ(0.75, sp.incidentSolarMultiplier());

  // Incident Solar Multiplier Schedule Name: Optional Object
  ScheduleConstant sch(m);
  EXPECT_TRUE(sp.setIncidentSolarMultiplierSchedule(sch));
  ASSERT_TRUE(sp.incidentSolarMultiplierSchedule());
  EXPECT_EQ(sch, sp.incidentSolarMultiplierSchedule().get());
}
