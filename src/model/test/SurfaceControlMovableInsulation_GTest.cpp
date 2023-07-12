/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <algorithm>
#include <gtest/gtest.h>

#include "ModelFixture.hpp"

#include "../Surface.hpp"
#include "../Surface_Impl.hpp"
#include "../SurfaceControlMovableInsulation.hpp"
#include "../SurfaceControlMovableInsulation_Impl.hpp"
#include "../StandardOpaqueMaterial.hpp"
#include "../StandardOpaqueMaterial_Impl.hpp"
#include "../ScheduleConstant.hpp"
#include "../ScheduleConstant_Impl.hpp"
#include "../ScheduleTypeLimits.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, SurfaceControlMovableInsulation_SurfaceControlMovableInsulation) {
  // create a model to use
  Model model;

  // create a surface to use
  std::vector<Point3d> vertices;
  vertices.push_back(Point3d());
  vertices.push_back(Point3d(1, 0, 0));
  vertices.push_back(Point3d(1, 1, 0));
  vertices.push_back(Point3d(0, 1, 0));
  Surface surface(vertices, model);

  // create a material object to use
  StandardOpaqueMaterial material(model);

  // surface doesn't have control movable insulation yet
  ASSERT_FALSE(surface.surfaceControlMovableInsulation());

  // create a surface control movable insulation object to use
  SurfaceControlMovableInsulation mi(surface, material);
  ASSERT_TRUE(surface.surfaceControlMovableInsulation());
  EXPECT_EQ(mi.insulationType(), "Outside");
  Surface surface2 = mi.surface();
  EXPECT_EQ(surface, surface2);
  EXPECT_EQ(surface2.surfaceControlMovableInsulation().get(), mi);
  Schedule schedule = mi.schedule();
  boost::optional<ScheduleConstant> scheduleConstant = schedule.optionalCast<ScheduleConstant>();
  ASSERT_TRUE(scheduleConstant);
  EXPECT_EQ((*scheduleConstant).value(), 1.0);
}

// test setting and getting
TEST_F(ModelFixture, SurfaceControlMovableInsulation_SetGetFields) {
  // create a model to use
  Model model;

  // create a surface to use
  std::vector<Point3d> vertices;
  vertices.push_back(Point3d());
  vertices.push_back(Point3d(1, 0, 0));
  vertices.push_back(Point3d(1, 1, 0));
  vertices.push_back(Point3d(0, 1, 0));
  Surface surface(vertices, model);

  // create a material object to use
  StandardOpaqueMaterial material(model);

  // create a schedule object to use
  ScheduleConstant sched(model);
  sched.setValue(0.5);

  // create a surface control movable insulation object to use
  SurfaceControlMovableInsulation mi(surface, material);

  // check the fields
  EXPECT_EQ(mi.insulationType(), "Outside");
  mi.setInsulationType("Outside");
  EXPECT_EQ(mi.insulationType(), "Outside");
  mi.setInsulationType("Inside");
  EXPECT_EQ(mi.insulationType(), "Inside");
  mi.setInsulationType("Outsid");
  EXPECT_EQ(mi.insulationType(), "Inside");

  mi.setMaterial(material);
  EXPECT_EQ(mi.material(), material);

  Schedule schedule = mi.schedule();
  boost::optional<ScheduleConstant> scheduleConstant = schedule.optionalCast<ScheduleConstant>();
  ASSERT_TRUE(scheduleConstant);
  EXPECT_EQ((*scheduleConstant).value(), 1.0);
  EXPECT_EQ(model.alwaysOnContinuousScheduleName(), schedule.nameString());

  mi.setSchedule(sched);
  Schedule schedule2 = mi.schedule();
  boost::optional<ScheduleConstant> scheduleConstant2 = schedule2.optionalCast<ScheduleConstant>();
  ASSERT_TRUE(scheduleConstant2);
  EXPECT_EQ(mi.schedule(), sched);
  EXPECT_EQ(mi.schedule(), *scheduleConstant2);
  EXPECT_EQ((*scheduleConstant2).value(), 0.5);

  // The Schedule Type Registry should have done it's thing and assigned a scheduleTypeLimits
  ASSERT_TRUE(schedule2.scheduleTypeLimits());
  ASSERT_TRUE(schedule2.scheduleTypeLimits()->lowerLimitValue());
  EXPECT_EQ(0.0, schedule2.scheduleTypeLimits()->lowerLimitValue().get());
  ASSERT_TRUE(schedule2.scheduleTypeLimits()->upperLimitValue());
  EXPECT_EQ(1.0, schedule2.scheduleTypeLimits()->upperLimitValue().get());
  ASSERT_TRUE(schedule2.scheduleTypeLimits()->numericType());
  EXPECT_EQ("Continuous", schedule2.scheduleTypeLimits()->numericType().get());
}

// test cloning it
TEST_F(ModelFixture, SurfaceControlMovableInsulation_Clone) {
  // create a model to use
  Model model;

  // create a surface to use
  std::vector<Point3d> vertices;
  vertices.push_back(Point3d());
  vertices.push_back(Point3d(1, 0, 0));
  vertices.push_back(Point3d(1, 1, 0));
  vertices.push_back(Point3d(0, 1, 0));
  Surface surface(vertices, model);

  // create a material object to use
  StandardOpaqueMaterial material(model);

  // create a surface control movable insulation object to use
  SurfaceControlMovableInsulation mi(surface, material);

  // change some of the fields
  mi.setInsulationType("Inside");

  // clone it into the same model
  auto miClone = mi.clone(model).cast<SurfaceControlMovableInsulation>();
  EXPECT_EQ("Inside", miClone.insulationType());

  // clone it into a different model
  Model model2;
  auto miClone2 = mi.clone(model2).cast<SurfaceControlMovableInsulation>();
  EXPECT_EQ("Inside", miClone2.insulationType());
}

// check that remove works
TEST_F(ModelFixture, SurfaceControlMovableInsulation_Remove) {
  Model model;
  std::vector<Point3d> vertices;
  vertices.push_back(Point3d());
  vertices.push_back(Point3d(1, 0, 0));
  vertices.push_back(Point3d(1, 1, 0));
  vertices.push_back(Point3d(0, 1, 0));
  Surface surface(vertices, model);
  StandardOpaqueMaterial material(model);
  auto size = model.modelObjects().size();  // 2: Surface, Material
  SurfaceControlMovableInsulation mi(surface, material);
  EXPECT_EQ(size + 3, model.modelObjects().size());  // 5: Surface, Material, SurfaceControlMovableInsulation, ScheduleConstant, ScheduleTypeLimits
  ASSERT_TRUE(surface.surfaceControlMovableInsulation());
  EXPECT_FALSE(mi.remove().empty());
  ASSERT_FALSE(surface.surfaceControlMovableInsulation());
  EXPECT_EQ(size + 2, model.modelObjects().size());  // 4: Surface, Material, ScheduleConstant, ScheduleTypeLimits
}
