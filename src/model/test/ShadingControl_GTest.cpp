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
#include "../ScheduleConstant.hpp"
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
  EXPECT_TRUE(construction.insertLayer(0, glazing));

  Blind blind(model);
  EXPECT_TRUE(construction.insertLayer(1, blind));

  ShadingControl shadingControl(construction);
  ASSERT_TRUE(shadingControl.construction());
  EXPECT_EQ(construction.handle(), shadingControl.construction()->handle());
  EXPECT_FALSE(shadingControl.shadingMaterial());
  EXPECT_EQ("InteriorBlind", shadingControl.shadingType());
  EXPECT_TRUE(shadingControl.isTypeValueAllowingSlatAngleControl());
  EXPECT_EQ("OnIfHighSolarOnWindow", shadingControl.shadingControlType());
  EXPECT_FALSE(shadingControl.schedule());

  std::vector<Point3d> vertices;
  vertices.push_back(Point3d(0, 0, 1));
  vertices.push_back(Point3d(0, 0, 0));
  vertices.push_back(Point3d(1, 0, 0));
  vertices.push_back(Point3d(1, 0, 1));

  SubSurface subSurface(vertices, model);
  EXPECT_TRUE(subSurface.setConstruction(construction));
  EXPECT_FALSE(subSurface.shadingControl());
  EXPECT_TRUE(subSurface.setShadingControl(shadingControl));
  //ASSERT_TRUE(subSurface.shadingControl());
  //ASSERT_TRUE(subSurface.shadingControl()->construction());
  //EXPECT_EQ(construction.handle(), subSurface.shadingControl()->construction()->handle());
}

TEST_F(ModelFixture, ShadingControl_Material) {
  Model model;

  Blind blind(model);

  ShadingControl shadingControl(blind);
  EXPECT_FALSE(shadingControl.construction());
  ASSERT_TRUE(shadingControl.shadingMaterial());
  EXPECT_EQ(blind.handle(), shadingControl.shadingMaterial()->handle());
  EXPECT_EQ("InteriorBlind", shadingControl.shadingType());
  EXPECT_TRUE(shadingControl.isTypeValueAllowingSlatAngleControl());
  EXPECT_EQ("OnIfHighSolarOnWindow", shadingControl.shadingControlType());
  EXPECT_FALSE(shadingControl.schedule());

  std::vector<Point3d> vertices;
  vertices.push_back(Point3d(0, 0, 1));
  vertices.push_back(Point3d(0, 0, 0));
  vertices.push_back(Point3d(1, 0, 0));
  vertices.push_back(Point3d(1, 0, 1));

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
  vertices1.push_back(Point3d(0, 0, 1));
  vertices1.push_back(Point3d(0, 0, 0));
  vertices1.push_back(Point3d(1, 0, 0));
  vertices1.push_back(Point3d(1, 0, 1));
  SubSurface subSurface1(vertices1, model);

  std::vector<Point3d> vertices2;
  vertices2.push_back(Point3d(0, 0, 1));
  vertices2.push_back(Point3d(0, 0, 0));
  vertices2.push_back(Point3d(1, 0, 0));
  vertices2.push_back(Point3d(1, 0, 1));
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
  vertices1.push_back(Point3d(0, 0, 1));
  vertices1.push_back(Point3d(0, 0, 0));
  vertices1.push_back(Point3d(1, 0, 0));
  vertices1.push_back(Point3d(1, 0, 1));
  SubSurface subSurface1(vertices1, model);

  std::vector<Point3d> vertices2;
  vertices2.push_back(Point3d(0, 0, 1));
  vertices2.push_back(Point3d(0, 0, 0));
  vertices2.push_back(Point3d(1, 0, 0));
  vertices2.push_back(Point3d(1, 0, 1));
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

TEST_F(ModelFixture, ShadingControl_ShadingControlTypeLogic) {

  Model m;

  Blind b(m);
  ShadingControl sc(b);

  EXPECT_TRUE(sc.setSetpoint(32.0));
  ASSERT_TRUE(sc.setpoint());
  EXPECT_EQ(32.0, sc.setpoint().get());

  ScheduleConstant sch(m);

  // Allows nothing
  EXPECT_TRUE(sc.setShadingControlType("AlwaysOn"));
  EXPECT_FALSE(sc.isControlTypeValueNeedingSetpoint1());
  EXPECT_FALSE(sc.isControlTypeValueNeedingSetpoint2());
  EXPECT_FALSE(sc.isControlTypeValueAllowingSchedule());
  EXPECT_TRUE(sc.isTypeValueAllowingSlatAngleControl());
  EXPECT_FALSE(sc.setSetpoint(30.0));
  EXPECT_FALSE(sc.setSetpoint2(500.0));
  EXPECT_FALSE(sc.setSchedule(sch));
  EXPECT_FALSE(sc.setpoint());
  EXPECT_FALSE(sc.setpoint2());
  EXPECT_FALSE(sc.schedule());

  // Allows schedules, required two setpoints
  EXPECT_TRUE(sc.setShadingControlType("OnIfHighZoneAirTempAndHighSolarOnWindow"));
  EXPECT_TRUE(sc.isControlTypeValueNeedingSetpoint1());
  EXPECT_TRUE(sc.isControlTypeValueNeedingSetpoint2());
  EXPECT_TRUE(sc.isControlTypeValueAllowingSchedule());
  EXPECT_TRUE(sc.isTypeValueAllowingSlatAngleControl());
  EXPECT_TRUE(sc.setSetpoint(30.0));
  EXPECT_TRUE(sc.setSetpoint2(500.0));
  EXPECT_TRUE(sc.setSchedule(sch));
  ASSERT_TRUE(sc.setpoint());
  EXPECT_EQ(30.0, sc.setpoint().get());
  ASSERT_TRUE(sc.setpoint2());
  EXPECT_EQ(500.0, sc.setpoint2().get());
  ASSERT_TRUE(sc.schedule());
  EXPECT_EQ(sch, sc.schedule().get());

  // Shouldn't allow reseting setpoints since required
  sc.resetSetpoint();
  ASSERT_TRUE(sc.setpoint());
  EXPECT_EQ(30.0, sc.setpoint().get());
  ASSERT_TRUE(sc.setpoint2());
  EXPECT_EQ(500.0, sc.setpoint2().get());
  // Note: I'm not providing resetSetpoint2 on purpose

  sc.resetSchedule();
  EXPECT_FALSE(sc.schedule());
  EXPECT_TRUE(sc.setSchedule(sch));

  // OnIfHighGlare: needs setpoint1 only and not Setpoint2, allows schedule
  EXPECT_TRUE(sc.setShadingControlType("OnIfHighZoneCooling"));
  EXPECT_TRUE(sc.isControlTypeValueNeedingSetpoint1());
  EXPECT_FALSE(sc.isControlTypeValueNeedingSetpoint2());
  EXPECT_TRUE(sc.isControlTypeValueAllowingSchedule());
  EXPECT_TRUE(sc.isTypeValueAllowingSlatAngleControl());

  // Setpoint 1 has been cleared because we're switching Shading Control Type and that was the historical behavior
  EXPECT_FALSE(sc.setpoint2());
  // Setpoint2 is cleared because the new Shading Control Type does not support Setpoint 2
  EXPECT_FALSE(sc.setpoint2());
  // Schedule is kept, user should be responsible to ensure the Schedule still makes sense, instead of always clearing it
  EXPECT_TRUE(sc.schedule());

  EXPECT_TRUE(sc.isTypeofSlatAngleControlforBlindsDefaulted());
  EXPECT_TRUE(sc.setSlatAngleSchedule(sch));
  EXPECT_TRUE(sc.slatAngleSchedule());
  EXPECT_FALSE(sc.isTypeofSlatAngleControlforBlindsDefaulted());
  EXPECT_EQ("ScheduledSlatAngle", sc.typeofSlatAngleControlforBlinds());
  sc.resetSlatAngleSchedule();
  EXPECT_FALSE(sc.slatAngleSchedule());
  EXPECT_TRUE(sc.isTypeofSlatAngleControlforBlindsDefaulted());
  EXPECT_TRUE(sc.setTypeofSlatAngleControlforBlinds("BlockBeamSolar"));
  EXPECT_FALSE(sc.isTypeofSlatAngleControlforBlindsDefaulted());
  EXPECT_EQ("BlockBeamSolar", sc.typeofSlatAngleControlforBlinds());
  sc.resetTypeofSlatAngleControlforBlinds();
  EXPECT_TRUE(sc.isTypeofSlatAngleControlforBlindsDefaulted());
  EXPECT_FALSE(sc.glareControlIsActive());
  EXPECT_TRUE(sc.setGlareControlIsActive(true));
  EXPECT_TRUE(sc.glareControlIsActive());
  sc.resetGlareControlIsActive();
  EXPECT_FALSE(sc.glareControlIsActive());
}
