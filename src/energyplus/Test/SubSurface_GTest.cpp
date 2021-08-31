/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2021, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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
#include "EnergyPlusFixture.hpp"

#include "../ForwardTranslator.hpp"
#include "../ReverseTranslator.hpp"

#include "../../model/Model.hpp"
#include "../../model/SubSurface.hpp"
#include "../../model/SubSurface_Impl.hpp"
#include "../../model/Surface.hpp"
#include "../../model/Surface_Impl.hpp"
#include "../../model/Space.hpp"
#include "../../model/Space_Impl.hpp"
#include "../../model/ThermalZone.hpp"
#include "../../model/ThermalZone_Impl.hpp"
#include "../../model/WindowPropertyFrameAndDivider.hpp"
#include "../../model/WindowPropertyFrameAndDivider_Impl.hpp"

#include <utilities/idd/FenestrationSurface_Detailed_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include <resources.hxx>

#include <sstream>

using namespace openstudio::energyplus;
using namespace openstudio::model;
using namespace openstudio;

TEST_F(EnergyPlusFixture, ReverseTranslator_GlassDoorToSubSurface) {

  std::string text = "\
  FenestrationSurface:Detailed,\n\
    Perimeter_ZN_1_wall_south_door,  !- Name\n\
    GlassDoor,               !- Surface Type\n\
    window_south,            !- Construction Name\n\
    Perimeter_ZN_1_wall_south,  !- Building Surface Name\n\
    ,                        !- Outside Boundary Condition Object\n\
    AutoCalculate,           !- View Factor to Ground\n\
    ,                        !- Frame and Divider Name\n\
    1.0000,                  !- Multiplier\n\
    4,                       !- Number of Vertices\n\
    12.930,0.0000,2.1340,  !- X,Y,Z ==> Vertex 1 {m}\n\
    12.930,0.0000,0.0000,  !- X,Y,Z ==> Vertex 2 {m}\n\
    14.760,0.0000,0.0000,  !- X,Y,Z ==> Vertex 3 {m}\n\
    14.760,0.0000,2.1340;  !- X,Y,Z ==> Vertex 4 {m}";

  IdfObject idfObject = IdfObject::load(text).get();
  Workspace ws(StrictnessLevel::Draft, IddFileType::EnergyPlus);
  OptionalWorkspaceObject owo = ws.addObject(idfObject);
  ASSERT_TRUE(owo);

  ReverseTranslator rt;
  Model model = rt.translateWorkspace(ws);
  SubSurfaceVector subSurfaces = model.getModelObjects<SubSurface>();
  ASSERT_EQ(1u, subSurfaces.size());
  EXPECT_EQ("GlassDoor", subSurfaces[0].subSurfaceType());
}

TEST_F(EnergyPlusFixture, ForwardTranslator_SubSurface) {
  Model model;

  ThermalZone thermalZone(model);

  Space space(model);
  space.setThermalZone(thermalZone);

  std::vector<Point3d> vertices;
  vertices.push_back(Point3d(0, 2, 0));
  vertices.push_back(Point3d(0, 0, 0));
  vertices.push_back(Point3d(2, 0, 0));
  vertices.push_back(Point3d(2, 2, 0));
  Surface surface(vertices, model);
  surface.setSpace(space);

  vertices.clear();
  vertices.push_back(Point3d(0, 1, 0));
  vertices.push_back(Point3d(0, 0, 0));
  vertices.push_back(Point3d(1, 0, 0));
  vertices.push_back(Point3d(1, 1, 0));

  SubSurface subSurface(vertices, model);
  subSurface.setSurface(surface);
  subSurface.assignDefaultSubSurfaceType();

  WindowPropertyFrameAndDivider frame(model);
  subSurface.setWindowPropertyFrameAndDivider(frame);

  ForwardTranslator forwardTranslator;
  Workspace workspace = forwardTranslator.translateModel(model);

  ASSERT_EQ(1u, workspace.getObjectsByType(IddObjectType::FenestrationSurface_Detailed).size());
  ASSERT_EQ(1u, workspace.getObjectsByType(IddObjectType::WindowProperty_FrameAndDivider).size());

  WorkspaceObject subSurfaceObject = workspace.getObjectsByType(IddObjectType::FenestrationSurface_Detailed)[0];
  WorkspaceObject frameObject = workspace.getObjectsByType(IddObjectType::WindowProperty_FrameAndDivider)[0];

  ASSERT_TRUE(subSurfaceObject.getTarget(FenestrationSurface_DetailedFields::FrameandDividerName));
  EXPECT_EQ(frameObject.handle(), subSurfaceObject.getTarget(FenestrationSurface_DetailedFields::FrameandDividerName)->handle());
}

// https://github.com/NREL/OpenStudio/issues/4361
TEST_F(EnergyPlusFixture, Issue_4361) {
  Model model;

  ThermalZone thermalZone(model);

  Space space(model);
  space.setThermalZone(thermalZone);

  // Create a surface and subsurface
  // get the surface net and gross area & subsurface gross area
  // surface gross area == surface net area + subsurface gross area
  // That is before the subsurface is assigned a frame and divider

  std::vector<Point3d> vertices;
  vertices.push_back(Point3d(0, 0, 2));
  vertices.push_back(Point3d(0, 0, 0));
  vertices.push_back(Point3d(2, 0, 0));
  vertices.push_back(Point3d(2, 0, 2));
  Surface surface(vertices, model);
  surface.setSpace(space);
  surface.setSurfaceType("Wall");

  vertices.clear();
  vertices.push_back(Point3d(0.5, 0, 1.5));
  vertices.push_back(Point3d(0.5, 0, 0.5));
  vertices.push_back(Point3d(1.5, 0, 0.5));
  vertices.push_back(Point3d(1.5, 0, 1.5));

  SubSurface subSurface(vertices, model);
  subSurface.setSurface(surface);
  subSurface.setSubSurfaceType("FixedWindow");

  double surfaceGrossArea = surface.grossArea();
  double surfaceNetArea = surface.netArea();
  double subSurfaceGrossArea = subSurface.grossArea();
  double subSurfaceTotalArea = subSurface.roughOpeningArea();

  EXPECT_EQ(surfaceGrossArea, surfaceNetArea + subSurfaceGrossArea);
  double windowWallRatio = surface.windowToWallRatio();
  EXPECT_EQ(windowWallRatio, 0.25);

  // Then assign a frame and divider to the subsurface
  // then do the same thing with the areas

  WindowPropertyFrameAndDivider frame(model);
  frame.setFrameWidth(0.030);
  subSurface.setWindowPropertyFrameAndDivider(frame);

  surfaceNetArea = surface.netArea();
  subSurfaceGrossArea = subSurface.grossArea();
  subSurfaceTotalArea = subSurface.roughOpeningArea();
  EXPECT_EQ(surfaceGrossArea, surfaceNetArea + subSurfaceTotalArea);

  windowWallRatio = surface.windowToWallRatio();
  EXPECT_NEAR(windowWallRatio, 0.28, 0.01);
}
TEST_F(EnergyPlusFixture, Issue_4361_Subsurface_Outside_Parent) {
  Model model;

  ThermalZone thermalZone(model);

  Space space(model);
  space.setThermalZone(thermalZone);

  // Create a surface and subsurface
  // get the surface net and gross area & subsurface gross area
  // surface gross area == surface net area + subsurface gross area
  // That is before the subsurface is assigned a frame and divider

  std::vector<Point3d> vertices;
  vertices.push_back(Point3d(0, 0, 2));
  vertices.push_back(Point3d(0, 0, 0));
  vertices.push_back(Point3d(2, 0, 0));
  vertices.push_back(Point3d(2, 0, 2));
  Surface surface(vertices, model);
  surface.setSpace(space);
  surface.setSurfaceType("Wall");

  vertices.clear();
  vertices.push_back(Point3d(0.5, 0, 1.99));
  vertices.push_back(Point3d(0.5, 0, 0.99));
  vertices.push_back(Point3d(1.5, 0, 0.99));
  vertices.push_back(Point3d(1.5, 0, 1.99));

  SubSurface subSurface(vertices, model);
  subSurface.setSurface(surface);
  subSurface.setSubSurfaceType("FixedWindow");

  double surfaceGrossArea = surface.grossArea();
  double surfaceNetArea = surface.netArea();
  double subSurfaceGrossArea = subSurface.grossArea();
  double subSurfaceTotalArea = subSurface.roughOpeningArea();

  EXPECT_EQ(surfaceGrossArea, surfaceNetArea + subSurfaceGrossArea);
  double windowWallRatio = surface.windowToWallRatio();
  EXPECT_EQ(windowWallRatio, 0.25);

  // Then assign a frame and divider to the subsurface
  // then do the same thing with the areas
  // The area should be unchanged because the subsurface is not enclosed
  // by the parent serface after including the frame

  WindowPropertyFrameAndDivider frame(model);
  frame.setFrameWidth(0.030);
  subSurface.setWindowPropertyFrameAndDivider(frame);

  surfaceNetArea = surface.netArea();
  subSurfaceGrossArea = subSurface.grossArea();
  subSurfaceTotalArea = subSurface.roughOpeningArea();
  EXPECT_EQ(surfaceGrossArea, surfaceNetArea + subSurfaceTotalArea);

  windowWallRatio = surface.windowToWallRatio();
  EXPECT_NEAR(windowWallRatio, 0.25, 0.01);
}

TEST_F(EnergyPlusFixture, Issue_4361_Multi_Subsurfaces_Non_Overlapping) {
  Model model;

  ThermalZone thermalZone(model);

  Space space(model);
  space.setThermalZone(thermalZone);

  std::vector<Point3d> vertices;
  vertices.push_back(Point3d(0, 0, 2));
  vertices.push_back(Point3d(0, 0, 0));
  vertices.push_back(Point3d(4, 0, 0));
  vertices.push_back(Point3d(4, 0, 2));
  Surface surface(vertices, model);
  surface.setSpace(space);
  surface.setSurfaceType("Wall");

  vertices.clear();
  vertices.push_back(Point3d(0.5, 0, 1.5));
  vertices.push_back(Point3d(0.5, 0, 0.5));
  vertices.push_back(Point3d(1.5, 0, 0.5));
  vertices.push_back(Point3d(1.5, 0, 1.5));

  SubSurface subSurface1(vertices, model);
  subSurface1.setSurface(surface);
  subSurface1.setSubSurfaceType("FixedWindow");

  vertices.clear();
  vertices.push_back(Point3d(2.5, 0, 1.5));
  vertices.push_back(Point3d(2.5, 0, 0.5));
  vertices.push_back(Point3d(3.5, 0, 0.5));
  vertices.push_back(Point3d(3.5, 0, 1.5));

  SubSurface subSurface2(vertices, model);
  subSurface2.setSurface(surface);
  subSurface2.setSubSurfaceType("FixedWindow");

  double windowWallRatio = surface.windowToWallRatio();
  EXPECT_NEAR(windowWallRatio, 0.25, 0.01);

  WindowPropertyFrameAndDivider frame1(model);
  frame1.setFrameWidth(0.030);
  subSurface1.setWindowPropertyFrameAndDivider(frame1);

  WindowPropertyFrameAndDivider frame2(model);
  frame2.setFrameWidth(0.030);
  subSurface2.setWindowPropertyFrameAndDivider(frame2);

  windowWallRatio = surface.windowToWallRatio();
  EXPECT_NEAR(windowWallRatio, 0.281, 0.01);
}

TEST_F(EnergyPlusFixture, Issue_4361_Multi_Subsurfaces_Overlapping) {
  Model model;

  ThermalZone thermalZone(model);

  Space space(model);
  space.setThermalZone(thermalZone);

  std::vector<Point3d> vertices;
  vertices.push_back(Point3d(0, 0, 2));
  vertices.push_back(Point3d(0, 0, 0));
  vertices.push_back(Point3d(4, 0, 0));
  vertices.push_back(Point3d(4, 0, 2));
  Surface surface(vertices, model);
  surface.setSpace(space);
  surface.setSurfaceType("Wall");

  vertices.clear();
  vertices.push_back(Point3d(0.5, 0, 1.5));
  vertices.push_back(Point3d(0.5, 0, 0.5));
  vertices.push_back(Point3d(1.5, 0, 0.5));
  vertices.push_back(Point3d(1.5, 0, 1.5));

  SubSurface subSurface1(vertices, model);
  subSurface1.setSurface(surface);
  subSurface1.setSubSurfaceType("FixedWindow");

  vertices.clear();
  vertices.push_back(Point3d(1.51, 0, 1.5));
  vertices.push_back(Point3d(1.51, 0, 0.5));
  vertices.push_back(Point3d(2.51, 0, 0.5));
  vertices.push_back(Point3d(2.51, 0, 1.5));

  SubSurface subSurface2(vertices, model);
  subSurface2.setSurface(surface);
  subSurface2.setSubSurfaceType("FixedWindow");

  double windowWallRatio = surface.windowToWallRatio();
  EXPECT_NEAR(windowWallRatio, 0.25, 0.01);

  WindowPropertyFrameAndDivider frame1(model);
  frame1.setFrameWidth(0.030);
  subSurface1.setWindowPropertyFrameAndDivider(frame1);

  WindowPropertyFrameAndDivider frame2(model);
  frame2.setFrameWidth(0.030);
  subSurface2.setWindowPropertyFrameAndDivider(frame2);

  windowWallRatio = surface.windowToWallRatio();
  EXPECT_NEAR(windowWallRatio, 0.2654, 0.01);
}