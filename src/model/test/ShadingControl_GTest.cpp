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

#include <gtest/gtest.h>

#include "ModelFixture.hpp"

#include "../ShadingControl.hpp"
#include "../ShadingControl_Impl.hpp"
#include "../Construction.hpp"
#include "../Schedule.hpp"
#include "../Blind.hpp"
#include "../Blind_Impl.hpp"
#include "../SimpleGlazing.hpp"
#include "../SubSurface.hpp"

#include "../../utilities/geometry/Point3d.hpp"

using namespace openstudio;
using namespace openstudio::model;


TEST_F(ModelFixture, ShadingControl_Construction) {
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
  //ASSERT_TRUE(subSurface.shadingControl());
  //ASSERT_TRUE(subSurface.shadingControl()->construction());
  //EXPECT_EQ(construction.handle(), subSurface.shadingControl()->construction()->handle());
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

TEST_F(ModelFixture, ShadingControl_SubSurfaces) {
  Model model;

  Blind blind(model);
  ShadingControl shadingControl(blind);

  std::vector<Point3d> vertices1;
  vertices1.push_back(Point3d(0,0,1));
  vertices1.push_back(Point3d(0,0,0));
  vertices1.push_back(Point3d(1,0,0));
  vertices1.push_back(Point3d(1,0,1));
  SubSurface subSurface1(vertices1, model);

  std::vector<Point3d> vertices2;
  vertices2.push_back(Point3d(0,0,1));
  vertices2.push_back(Point3d(0,0,0));
  vertices2.push_back(Point3d(1,0,0));
  vertices2.push_back(Point3d(1,0,1));
  SubSurface subSurface2(vertices2, model);

  EXPECT_EQ(0u, shadingControl.numberofSubSurfaces());
  EXPECT_TRUE(shadingControl.addSubSurface(subSurface1));
  EXPECT_EQ(1u, shadingControl.numberofSubSurfaces());
  shadingControl.removeSubSurface(subSurface1);
  EXPECT_EQ(0u, shadingControl.numberofSubSurfaces());

  std::vector<SubSurface> subSurfaces;
  subSurfaces.push_back(subSurface1);
  subSurfaces.push_back(subSurface2);
  EXPECT_TRUE(shadingControl.addSubSurfaces(subSurfaces));
  EXPECT_EQ(2u, shadingControl.numberofSubSurfaces());

  EXPECT_EQ("Sequential", shadingControl.multipleSurfaceControlType());
  EXPECT_TRUE(shadingControl.setMultipleSurfaceControlType("Group"));
  EXPECT_EQ("Group", shadingControl.multipleSurfaceControlType());
  EXPECT_FALSE(shadingControl.setMultipleSurfaceControlType("BADENUM"));
  EXPECT_EQ("Group", shadingControl.multipleSurfaceControlType());

  shadingControl.removeAllSubSurfaces();
  EXPECT_EQ(0u, shadingControl.numberofSubSurfaces());

  EXPECT_TRUE(shadingControl.addSubSurfaces(subSurfaces));
  EXPECT_EQ(2u, shadingControl.numberofSubSurfaces());
  subSurface1.remove();
  EXPECT_EQ(1u, shadingControl.numberofSubSurfaces());
  EXPECT_EQ(1u, shadingControl.numExtensibleGroups());
  ASSERT_TRUE(shadingControl.subSurfaceIndex(subSurface2));
  EXPECT_EQ(1u, shadingControl.subSurfaceIndex(subSurface2).get());
}

TEST_F(ModelFixture, ShadingControl_Clone) {
  Model model;

  Blind blind(model);
  ShadingControl shadingControl(blind);

  std::vector<Point3d> vertices1;
  vertices1.push_back(Point3d(0,0,1));
  vertices1.push_back(Point3d(0,0,0));
  vertices1.push_back(Point3d(1,0,0));
  vertices1.push_back(Point3d(1,0,1));
  SubSurface subSurface1(vertices1, model);

  std::vector<Point3d> vertices2;
  vertices2.push_back(Point3d(0,0,1));
  vertices2.push_back(Point3d(0,0,0));
  vertices2.push_back(Point3d(1,0,0));
  vertices2.push_back(Point3d(1,0,1));
  SubSurface subSurface2(vertices2, model);

  shadingControl.setSubSurfaces({subSurface1, subSurface2});
  EXPECT_EQ(2, shadingControl.numberofSubSurfaces());

  // Clone same model
  {
    // TODO: what is the expected behavior here? Should the clone reference the same subSurfaces as the original?
    ShadingControl shadingControlClone = shadingControl.clone(model).cast<ShadingControl>();
    ASSERT_TRUE(shadingControlClone.shadingMaterial());
    EXPECT_EQ(shadingControl.shadingMaterial()->handle(), shadingControlClone.shadingMaterial()->handle());
    EXPECT_EQ(2u, shadingControl.numberofSubSurfaces());
    EXPECT_EQ(2u, shadingControl.numExtensibleGroups());
    EXPECT_EQ(2u, shadingControlClone.numberofSubSurfaces());
    EXPECT_EQ(2u, shadingControlClone.numExtensibleGroups());
    EXPECT_EQ(shadingControl.subSurfaces(), shadingControlClone.subSurfaces());

  }

  // Clone other model
  {
    Model model2;

    // TODO: what is the expected behavior here? It will clone the Construction and ShadingMaterial (ResourceObject...) referenced here, but not the subSurfaces
    ShadingControl shadingControlClone = shadingControl.clone(model2).cast<ShadingControl>();
    EXPECT_EQ(2u, shadingControl.numberofSubSurfaces());
    EXPECT_EQ(2u, shadingControl.numExtensibleGroups());
    EXPECT_EQ(0u, shadingControlClone.numberofSubSurfaces());
    EXPECT_EQ(0u, shadingControlClone.numExtensibleGroups());
    EXPECT_EQ(1u, model2.getConcreteModelObjects<Blind>().size());
    EXPECT_TRUE(shadingControlClone.shadingMaterial());
    EXPECT_NE(shadingControl.shadingMaterial().get(), shadingControlClone.shadingMaterial().get());
  }
}

TEST_F(ModelFixture, ShadingControl_RemoveRequiredObject) {
  Model model;

  Blind blind(model);
  ShadingControl shadingControl(blind);

  blind.remove();

  EXPECT_FALSE(shadingControl.shadingMaterial());
  EXPECT_FALSE(shadingControl.construction());

  // TODO: You're now in a broken, and unfixable state: you neither have a Construction nor a ShadingMaterial,
  // and the model API has no setters to help you fix the state (setShadingMaterial / setConstruction)
  // This WILL get forward translated anyways. We need to determine whether we want to add setters in the Model API or have the FT do a check and not
  // translate the object if it doesn't have either. Throwing here so we do not forget to do it
  ASSERT_TRUE(false);
}
