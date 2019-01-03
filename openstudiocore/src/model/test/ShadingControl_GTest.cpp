/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2019, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

#include "ModelFixture.hpp"

#include "../ShadingControl.hpp"
#include "../Construction.hpp"
#include "../Schedule.hpp"
#include "../Blind.hpp"
#include "../SimpleGlazing.hpp"
#include "../SubSurface.hpp"

#include "../../utilities/geometry/Point3d.hpp"

using namespace openstudio;
using namespace openstudio::model;


TEST_F(ModelFixture,ShadingControl_Construction) {
  Model model;

  Construction construction(model);

  SimpleGlazing glazing(model);
  EXPECT_TRUE(construction.insertLayer(0,glazing));

  Blind blind(model);
  EXPECT_TRUE(construction.insertLayer(1,blind));

  ShadingControl shadingControl(construction);
  ASSERT_TRUE(shadingControl.construction());
  EXPECT_EQ(construction.handle(), shadingControl.construction()->handle());
  EXPECT_FALSE(shadingControl.shadingMaterial());
  EXPECT_EQ("InteriorBlind", shadingControl.shadingType());
  EXPECT_EQ("OnIfHighSolarOnWindow", shadingControl.shadingControlType());
  EXPECT_FALSE(shadingControl.schedule());

  std::vector<Point3d> vertices;
  vertices.push_back(Point3d(0,0,1));
  vertices.push_back(Point3d(0,0,0));
  vertices.push_back(Point3d(1,0,0));
  vertices.push_back(Point3d(1,0,1));

  SubSurface subSurface(vertices, model);
  EXPECT_TRUE(subSurface.setConstruction(construction));
  EXPECT_FALSE(subSurface.shadingControl());
  EXPECT_TRUE(subSurface.setShadingControl(shadingControl));
  ASSERT_TRUE(subSurface.shadingControl());
  ASSERT_TRUE(subSurface.shadingControl()->construction());
  EXPECT_EQ(construction.handle(), subSurface.shadingControl()->construction()->handle());
}


TEST_F(ModelFixture,ShadingControl_Material) {
  Model model;

  Blind blind(model);

  ShadingControl shadingControl(blind);
  EXPECT_FALSE(shadingControl.construction());
  ASSERT_TRUE(shadingControl.shadingMaterial());
  EXPECT_EQ(blind.handle(), shadingControl.shadingMaterial()->handle());
  EXPECT_EQ("InteriorBlind", shadingControl.shadingType());
  EXPECT_EQ("OnIfHighSolarOnWindow", shadingControl.shadingControlType());
  EXPECT_FALSE(shadingControl.schedule());

  std::vector<Point3d> vertices;
  vertices.push_back(Point3d(0,0,1));
  vertices.push_back(Point3d(0,0,0));
  vertices.push_back(Point3d(1,0,0));
  vertices.push_back(Point3d(1,0,1));

  SubSurface subSurface(vertices, model);
  EXPECT_FALSE(subSurface.shadingControl());
  EXPECT_TRUE(subSurface.setShadingControl(shadingControl));
  ASSERT_TRUE(subSurface.shadingControl());
  ASSERT_TRUE(subSurface.shadingControl()->shadingMaterial());
  EXPECT_EQ(blind.handle(), subSurface.shadingControl()->shadingMaterial()->handle());
}

TEST_F(ModelFixture, ShadingControl_PurgeUnusedModelObjects) {
  Model model;

  Blind blind(model);
  ShadingControl shadingControl(blind);

  EXPECT_EQ(2u, model.objects().size());

  model.purgeUnusedResourceObjects();

  EXPECT_EQ(0, model.objects().size());
}