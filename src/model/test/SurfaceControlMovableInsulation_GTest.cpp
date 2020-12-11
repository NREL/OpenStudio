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
  SurfaceControlMovableInsulation miClone = mi.clone(model).cast<SurfaceControlMovableInsulation>();
  EXPECT_EQ("Inside", miClone.insulationType());

  // clone it into a different model
  Model model2;
  SurfaceControlMovableInsulation miClone2 = mi.clone(model2).cast<SurfaceControlMovableInsulation>();
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
