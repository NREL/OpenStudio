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

  ASSERT_FALSE(surface.surfaceControlMovableInsulation());

  // create a surface control movable insulation object to use
  SurfaceControlMovableInsulation mi(surface);
  ASSERT_TRUE(surface.surfaceControlMovableInsulation());
  Surface surface2 = mi.surface();
  EXPECT_EQ(surface, surface2);
  EXPECT_EQ(surface2.surfaceControlMovableInsulation().get(), mi);
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

  // create a surface control movable insulation object to use
  SurfaceControlMovableInsulation mi(surface);

  // check the fields
  EXPECT_FALSE(mi.insulationType());
  mi.setInsulationType("Outside");
  ASSERT_TRUE(mi.insulationType());
  EXPECT_EQ(mi.insulationType().get(), "Outside");
  mi.setInsulationType("Inside");
  ASSERT_TRUE(mi.insulationType());
  EXPECT_EQ(mi.insulationType().get(), "Inside");
  mi.setInsulationType("Outsid");
  ASSERT_TRUE(mi.insulationType());
  EXPECT_EQ(mi.insulationType().get(), "Inside");
  mi.resetInsulationType();
  EXPECT_FALSE(mi.insulationType());

  EXPECT_FALSE(mi.material());
  mi.setMaterial(material);
  ASSERT_TRUE(mi.material());
  EXPECT_EQ(mi.material().get(), material);
  mi.resetMaterial();
  EXPECT_FALSE(mi.material());

  EXPECT_FALSE(mi.schedule());
  mi.setSchedule(sched);
  ASSERT_TRUE(mi.schedule());
  EXPECT_EQ(mi.schedule().get(), sched);
  mi.resetSchedule();
  EXPECT_FALSE(mi.schedule());
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

  // create a surface control movable insulation object to use
  SurfaceControlMovableInsulation mi(surface);

  // change some of the fields
  mi.setInsulationType("Outside");

  // clone it into the same model
  SurfaceControlMovableInsulation miClone = mi.clone(model).cast<SurfaceControlMovableInsulation>();
  ASSERT_EQ("Outside", miClone.insulationType().get());

  // clone it into a different model
  Model model2;
  SurfaceControlMovableInsulation miClone2 = mi.clone(model2).cast<SurfaceControlMovableInsulation>();
  ASSERT_EQ("Outside", miClone2.insulationType().get());
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
  auto size = model.modelObjects().size();
  SurfaceControlMovableInsulation mi(surface);
  EXPECT_FALSE(mi.remove().empty());
  EXPECT_EQ(size, model.modelObjects().size());
}