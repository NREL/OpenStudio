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
#include "EnergyPlusFixture.hpp"

#include "../ErrorFile.hpp"
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
#include "../../model/ShadingControl.hpp"
#include "../../model/ShadingControl_Impl.hpp"
#include "../../model/Blind.hpp"
#include "../../model/Blind_Impl.hpp"

#include "../../utilities/core/Optional.hpp"
#include "../../utilities/core/Checksum.hpp"
#include "../../utilities/core/Logger.hpp"
#include "../../utilities/geometry/Point3d.hpp"
#include "../../utilities/geometry/Vector3d.hpp"
#include "../../utilities/sql/SqlFile.hpp"
#include "../../utilities/idd/IddEnums.hpp"
#include "../../utilities/idd/IddFile.hpp"
#include "../../utilities/idf/IdfFile.hpp"

#include <utilities/idd/FenestrationSurface_Detailed_FieldEnums.hxx>
#include <utilities/idd/BuildingSurface_Detailed_FieldEnums.hxx>
#include <utilities/idd/WindowShadingControl_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include "../../utilities/idf/IdfExtensibleGroup.hpp"
#include "../../utilities/idf/WorkspaceExtensibleGroup.hpp"

#include <resources.hxx>

#include <sstream>

using namespace openstudio;
using namespace openstudio::energyplus;
using namespace openstudio::model;

TEST_F(EnergyPlusFixture, ForwardTranslator_ShadingControls)
{
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

  Blind blind1(model);
  ShadingControl shadingControl1(blind1);
  subSurface.addShadingControl(shadingControl1);

  Blind blind2(model);
  ShadingControl shadingControl2(blind2);
  shadingControl2.setMultipleSurfaceControlType("Group");
  subSurface.addShadingControl(shadingControl2);

  ForwardTranslator forwardTranslator;
  Workspace workspace = forwardTranslator.translateModel(model);

  EXPECT_EQ(1u, workspace.getObjectsByType(IddObjectType::FenestrationSurface_Detailed).size());

  std::vector<WorkspaceObject> objVector(workspace.getObjectsByType(IddObjectType::WindowShadingControl));
  ASSERT_EQ(2u, objVector.size());
  WorkspaceObject wo1(objVector.at(0));
  WorkspaceObject wo2(objVector.at(1));

  EXPECT_EQ("Sequential", wo1.getString(WindowShadingControlFields::MultipleSurfaceControlType, false).get());
  EXPECT_EQ("Group", wo2.getString(WindowShadingControlFields::MultipleSurfaceControlType, false).get());

  ASSERT_EQ(1u, wo1.extensibleGroups().size());
  ASSERT_EQ(1u, wo2.extensibleGroups().size());
}

TEST_F(EnergyPlusFixture, ReverseTranslator_ShadingControls)
{
  openstudio::Workspace workspace(openstudio::StrictnessLevel::None, openstudio::IddFileType::EnergyPlus);

  openstudio::IdfObject idfObject1(openstudio::IddObjectType::BuildingSurface_Detailed);
  idfObject1.setString(BuildingSurface_DetailedFields::Name, "Surface 1");
  idfObject1.setString(BuildingSurface_DetailedFields::SurfaceType, "Wall");
  idfObject1.setString(BuildingSurface_DetailedFields::ConstructionName, "");
  idfObject1.setString(BuildingSurface_DetailedFields::ZoneName, "");
  idfObject1.setString(BuildingSurface_DetailedFields::OutsideBoundaryCondition, "Outdoors");
  idfObject1.setString(BuildingSurface_DetailedFields::OutsideBoundaryConditionObject, "");
  idfObject1.setString(BuildingSurface_DetailedFields::SunExposure, "SunExposed");
  idfObject1.setString(BuildingSurface_DetailedFields::WindExposure, "WindExposed");
  idfObject1.setString(BuildingSurface_DetailedFields::ViewFactortoGround, "");
  idfObject1.setString(BuildingSurface_DetailedFields::NumberofVertices, "");
  IdfExtensibleGroup group1 = idfObject1.pushExtensibleGroup(); // vertex 1
  group1.setDouble(0, 0);
  group1.setDouble(1, 2);
  group1.setDouble(2, 0);
  IdfExtensibleGroup group2 = idfObject1.pushExtensibleGroup(); // vertex 2
  group2.setDouble(0, 0);
  group2.setDouble(1, 0);
  group2.setDouble(2, 0);
  IdfExtensibleGroup group3 = idfObject1.pushExtensibleGroup(); // vertex 3
  group3.setDouble(0, 2);
  group3.setDouble(1, 0);
  group3.setDouble(2, 0);
  IdfExtensibleGroup group4 = idfObject1.pushExtensibleGroup(); // vertex 4
  group4.setDouble(0, 2);
  group4.setDouble(1, 2);
  group4.setDouble(2, 0);

  openstudio::WorkspaceObject epSurface = workspace.addObject(idfObject1).get();

  openstudio::IdfObject idfObject2(openstudio::IddObjectType::FenestrationSurface_Detailed);
  idfObject2.setString(FenestrationSurface_DetailedFields::Name, "Sub Surface 1");
  idfObject2.setString(FenestrationSurface_DetailedFields::SurfaceType, "Window");
  idfObject2.setString(FenestrationSurface_DetailedFields::ConstructionName, "");
  idfObject2.setString(FenestrationSurface_DetailedFields::BuildingSurfaceName, "Surface 1");
  idfObject2.setString(FenestrationSurface_DetailedFields::OutsideBoundaryConditionObject, "");
  idfObject2.setString(FenestrationSurface_DetailedFields::ViewFactortoGround, "Autocalculate");
  idfObject2.setString(FenestrationSurface_DetailedFields::FrameandDividerName, "");
  idfObject2.setString(FenestrationSurface_DetailedFields::Multiplier, "1.0");
  idfObject2.setString(FenestrationSurface_DetailedFields::NumberofVertices, "");
  IdfExtensibleGroup group5 = idfObject2.pushExtensibleGroup(); // vertex 1
  group5.setDouble(0, 0);
  group5.setDouble(1, 1);
  group5.setDouble(2, 0);
  IdfExtensibleGroup group6 = idfObject2.pushExtensibleGroup(); // vertex 2
  group6.setDouble(0, 0);
  group6.setDouble(1, 0);
  group6.setDouble(2, 0);
  IdfExtensibleGroup group7 = idfObject2.pushExtensibleGroup(); // vertex 3
  group7.setDouble(0, 1);
  group7.setDouble(1, 0);
  group7.setDouble(2, 0);
  IdfExtensibleGroup group8 = idfObject2.pushExtensibleGroup(); // vertex 4
  group8.setDouble(0, 1);
  group8.setDouble(1, 1);
  group8.setDouble(2, 0);

  openstudio::WorkspaceObject epSubSurface = workspace.addObject(idfObject2).get();

  openstudio::IdfObject idfObject3(openstudio::IddObjectType::WindowMaterial_Shade);
  idfObject3.setString(WindowMaterial_ShadeFields::Name, "Shade 1");

  openstudio::IdfObject idfObject4(openstudio::IddObjectType::WindowShadingControl);
  idfObject4.setString(WindowShadingControlFields::Name, "Shading Control 1");
  idfObject4.setString(WindowShadingControlFields::ZoneName, "");
  idfObject4.setString(WindowShadingControlFields::ShadingControlType, "AlwaysOn");
  idfObject4.setInt(WindowShadingControlFields::ShadingControlSequenceNumber, 1);
  idfObject4.setString(WindowShadingControlFields::ShadingType, "InteriorShade");
  idfObject4.setString(WindowShadingControlFields::ShadingDeviceMaterialName, "Shade 1");
  idfObject4.setDouble(WindowShadingControlFields::Setpoint, 100);
  idfObject4.setString(WindowShadingControlFields::GlareControlIsActive, "No");
  idfObject4.setString(WindowShadingControlFields::MultipleSurfaceControlType, "Sequential");
  IdfExtensibleGroup group9 = idfObject4.pushExtensibleGroup(); // sub surface
  group9.setString(0, "Sub Surface 1");

  openstudio::WorkspaceObject epWindowShadingControl = workspace.addObject(idfObject4).get();

  ReverseTranslator trans;
  ASSERT_NO_THROW(trans.translateWorkspace(workspace));
  Model model = trans.translateWorkspace(workspace);

  EXPECT_EQ(1u, model.getModelObjects<Surface>().size());
  EXPECT_EQ(1u, model.getModelObjects<SubSurface>().size());

  std::vector<ShadingControl> shadingControls = model.getModelObjects<ShadingControl>();
  ASSERT_EQ(1u, shadingControls.size());
  ShadingControl shadingControl = shadingControls[0];
  EXPECT_EQ(1, shadingControl.numberofSubSurfaces());
  EXPECT_EQ("Sequential", shadingControl.multipleSurfaceControlType());

  std::vector<SubSurface> subSurfaces = model.getModelObjects<SubSurface>();
  ASSERT_EQ(1u, subSurfaces.size());
  SubSurface subSurface = subSurfaces[0];
  EXPECT_EQ("Sub Surface 1", subSurface.name().get());

  ASSERT_EQ(1, subSurface.numberofShadingControls());
  ASSERT_TRUE(subSurface.shadingControl());
  ShadingControl shadingControl2 = subSurface.shadingControl().get();
  EXPECT_EQ(shadingControl, shadingControl2);
}
