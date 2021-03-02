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
#include "../../model/ScheduleConstant.hpp"
#include "../../model/ScheduleConstant_Impl.hpp"
#include "../../model/Construction.hpp"

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
#include <utilities/idd/WindowMaterial_Blind_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include "../../utilities/idf/IdfExtensibleGroup.hpp"
#include "../../utilities/idf/WorkspaceExtensibleGroup.hpp"

#include <resources.hxx>

#include <sstream>

using namespace openstudio;
using namespace openstudio::energyplus;
using namespace openstudio::model;

TEST_F(EnergyPlusFixture, ForwardTranslator_ShadingControls) {

  ForwardTranslator forwardTranslator;

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

  // Here's the position of the two subSurfaces onto the base Surface
  // 2 _____________
  //   |     |  B  |
  // 1 |_____|_____|
  //   |  A  |     |
  //   |_____|_____|
  //  0      1     2

  // A
  vertices.clear();
  vertices.push_back(Point3d(0, 1, 0));
  vertices.push_back(Point3d(0, 0, 0));
  vertices.push_back(Point3d(1, 0, 0));
  vertices.push_back(Point3d(1, 1, 0));

  SubSurface subSurfaceA(vertices, model);
  subSurfaceA.setName("SubSurface A");
  subSurfaceA.setSurface(surface);
  subSurfaceA.assignDefaultSubSurfaceType();

  // B
  vertices.clear();
  vertices.push_back(Point3d(1, 2, 0));
  vertices.push_back(Point3d(1, 1, 0));
  vertices.push_back(Point3d(2, 1, 0));
  vertices.push_back(Point3d(2, 2, 0));

  SubSurface subSurfaceB(vertices, model);
  subSurfaceB.setName("SubSurface B");
  subSurfaceB.setSurface(surface);
  subSurfaceB.assignDefaultSubSurfaceType();

  Blind blind1(model);
  ShadingControl shadingControl1(blind1);
  EXPECT_TRUE(shadingControl1.setShadingType("ExteriorBlind"));

  EXPECT_TRUE(shadingControl1.setShadingControlType("OnIfHighZoneAirTempAndHighSolarOnWindow"));
  EXPECT_TRUE(shadingControl1.isControlTypeValueAllowingSchedule());
  EXPECT_TRUE(shadingControl1.isControlTypeValueNeedingSetpoint1());
  EXPECT_TRUE(shadingControl1.isControlTypeValueNeedingSetpoint2());
  EXPECT_TRUE(shadingControl1.isTypeValueAllowingSlatAngleControl());

  ScheduleConstant shadingControl1Schedule(model);
  EXPECT_TRUE(shadingControl1.setSchedule(shadingControl1Schedule));
  EXPECT_TRUE(shadingControl1.setSetpoint(25.0));
  EXPECT_TRUE(shadingControl1.setSetpoint2(500.0));
  EXPECT_TRUE(shadingControl1.setGlareControlIsActive(true));
  EXPECT_TRUE(shadingControl1.setMultipleSurfaceControlType("Sequential"));
  EXPECT_TRUE(shadingControl1.addSubSurface(subSurfaceA));
  // Convenience method that calls ShadingControl::addSubSurface()
  EXPECT_TRUE(subSurfaceB.addShadingControl(shadingControl1));
  ASSERT_EQ(2u, shadingControl1.subSurfaces().size());
  EXPECT_EQ(1u, shadingControl1.subSurfaceIndex(subSurfaceA).get());
  EXPECT_EQ(2u, shadingControl1.subSurfaceIndex(subSurfaceB).get());

  Blind blind2(model);
  ShadingControl shadingControl2(blind2);
  EXPECT_TRUE(shadingControl2.setTypeofSlatAngleControlforBlinds("BlockBeamSolar"));
  EXPECT_TRUE(shadingControl2.setMultipleSurfaceControlType("Group"));
  EXPECT_TRUE(subSurfaceA.addShadingControl(shadingControl2));
  ASSERT_EQ(1u, shadingControl2.subSurfaces().size());
  EXPECT_EQ(1u, shadingControl2.subSurfaceIndex(subSurfaceA).get());

  EXPECT_EQ(2u, subSurfaceA.shadingControls().size());
  EXPECT_EQ(1u, subSurfaceB.shadingControls().size());

  {
    Workspace workspace = forwardTranslator.translateModel(model);

    EXPECT_EQ(2u, workspace.getObjectsByType(IddObjectType::FenestrationSurface_Detailed).size());
    ASSERT_EQ(2u, workspace.getObjectsByType(IddObjectType::WindowShadingControl).size());

    // Test ShadingControl1: it has all the fields set. We also check that it's control sequence number is 1.
    {
      boost::optional<WorkspaceObject> _wo = workspace.getObjectByTypeAndName(IddObjectType::WindowShadingControl, shadingControl1.nameString());
      ASSERT_TRUE(_wo);

      EXPECT_EQ(thermalZone.nameString(), _wo->getString(WindowShadingControlFields::ZoneName, false, true).get());
      EXPECT_EQ(1, _wo->getInt(WindowShadingControlFields::ShadingControlSequenceNumber, false).get());
      EXPECT_EQ("ExteriorBlind", _wo->getString(WindowShadingControlFields::ShadingType, false, true).get());
      EXPECT_FALSE(_wo->getString(WindowShadingControlFields::ConstructionwithShadingName, false, true));
      EXPECT_EQ("OnIfHighZoneAirTempAndHighSolarOnWindow", _wo->getString(WindowShadingControlFields::ShadingControlType, false, true).get());
      EXPECT_EQ(25.0, _wo->getDouble(WindowShadingControlFields::Setpoint, false).get());
      EXPECT_EQ(500.0, _wo->getDouble(WindowShadingControlFields::Setpoint2, false).get());

      EXPECT_EQ(shadingControl1Schedule.nameString(), _wo->getString(WindowShadingControlFields::ScheduleName, false, true).get());
      EXPECT_EQ("Yes", _wo->getString(WindowShadingControlFields::ShadingControlIsScheduled, false, true).get());

      EXPECT_EQ("Yes", _wo->getString(WindowShadingControlFields::GlareControlIsActive, false, true).get());
      EXPECT_EQ(blind1.nameString(), _wo->getString(WindowShadingControlFields::ShadingDeviceMaterialName, false, true).get());
      EXPECT_FALSE(_wo->getString(WindowShadingControlFields::SlatAngleScheduleName, false, true));
      EXPECT_EQ("FixedSlatAngle", _wo->getString(WindowShadingControlFields::TypeofSlatAngleControlforBlinds, false, true).get());
      EXPECT_FALSE(_wo->getString(WindowShadingControlFields::DaylightingControlObjectName, false, true));
      EXPECT_EQ("Sequential", _wo->getString(WindowShadingControlFields::MultipleSurfaceControlType, false).get());
      ASSERT_EQ(2u, _wo->extensibleGroups().size());
      {
        WorkspaceExtensibleGroup w_eg = _wo->extensibleGroups()[0].cast<WorkspaceExtensibleGroup>();
        EXPECT_EQ(subSurfaceA.nameString(), w_eg.getString(WindowShadingControlExtensibleFields::FenestrationSurfaceName, false).get());
      }
      {
        WorkspaceExtensibleGroup w_eg = _wo->extensibleGroups()[1].cast<WorkspaceExtensibleGroup>();
        EXPECT_EQ(subSurfaceB.nameString(), w_eg.getString(WindowShadingControlExtensibleFields::FenestrationSurfaceName, false).get());
      }
    }

    {
      boost::optional<WorkspaceObject> _wo = workspace.getObjectByTypeAndName(IddObjectType::WindowShadingControl, shadingControl2.nameString());
      ASSERT_TRUE(_wo);

      EXPECT_EQ(thermalZone.nameString(), _wo->getString(WindowShadingControlFields::ZoneName, false, true).get());
      // Number 2 this time
      EXPECT_EQ(2, _wo->getInt(WindowShadingControlFields::ShadingControlSequenceNumber, false).get());
      EXPECT_EQ("InteriorBlind", _wo->getString(WindowShadingControlFields::ShadingType, false, true).get());
      EXPECT_FALSE(_wo->getString(WindowShadingControlFields::ConstructionwithShadingName, false, true));
      EXPECT_EQ("OnIfHighSolarOnWindow", _wo->getString(WindowShadingControlFields::ShadingControlType, false, true).get());
      EXPECT_EQ(shadingControl2.setpoint().get(),
                _wo->getDouble(WindowShadingControlFields::Setpoint, false).get());  // Model setpoint at 27 W/m2 for some reason

      EXPECT_FALSE(_wo->getString(WindowShadingControlFields::ScheduleName, false, true));
      EXPECT_EQ("No", _wo->getString(WindowShadingControlFields::ShadingControlIsScheduled, false, true).get());

      EXPECT_EQ("No", _wo->getString(WindowShadingControlFields::GlareControlIsActive, false, true).get());
      EXPECT_EQ(blind2.nameString(), _wo->getString(WindowShadingControlFields::ShadingDeviceMaterialName, false, true).get());
      EXPECT_FALSE(_wo->getString(WindowShadingControlFields::SlatAngleScheduleName, false, true));
      EXPECT_EQ("BlockBeamSolar", _wo->getString(WindowShadingControlFields::TypeofSlatAngleControlforBlinds, false, true).get());
      EXPECT_FALSE(_wo->getString(WindowShadingControlFields::Setpoint2, false, true));
      EXPECT_FALSE(_wo->getString(WindowShadingControlFields::DaylightingControlObjectName, false, true));
      EXPECT_EQ("Group", _wo->getString(WindowShadingControlFields::MultipleSurfaceControlType, false).get());
      ASSERT_EQ(1u, _wo->extensibleGroups().size());
      {
        WorkspaceExtensibleGroup w_eg = _wo->extensibleGroups()[0].cast<WorkspaceExtensibleGroup>();
        EXPECT_EQ(subSurfaceA.nameString(), w_eg.getString(WindowShadingControlExtensibleFields::FenestrationSurfaceName, false).get());
      }
    }
  }

  {
    blind1.remove();

    EXPECT_FALSE(shadingControl1.shadingMaterial());
    EXPECT_FALSE(shadingControl1.construction());

    // You're now in a broken, and unfixable state: you neither have a Construction nor a ShadingMaterial,
    // and the model API has no setters to help you fix the state (setShadingMaterial / setConstruction)
    // We test that the FT catches this and does not translate the ShadingControl

    Workspace workspace = forwardTranslator.translateModel(model);

    EXPECT_EQ(2u, workspace.getObjectsByType(IddObjectType::FenestrationSurface_Detailed).size());

    std::vector<WorkspaceObject> objVector(workspace.getObjectsByType(IddObjectType::WindowShadingControl));
    EXPECT_EQ(1u, objVector.size());
    EXPECT_EQ(shadingControl2.nameString(), objVector[0].nameString());
  }
}

TEST_F(EnergyPlusFixture, ReverseTranslator_ShadingControls) {
  openstudio::Workspace workspace(openstudio::StrictnessLevel::None, openstudio::IddFileType::EnergyPlus);

  openstudio::IdfObject idf_zone(openstudio::IddObjectType::Zone);
  idf_zone.setName("Thermal Zone 1");

  openstudio::WorkspaceObject epZone = workspace.addObject(idf_zone).get();

  openstudio::IdfObject idfObject1(openstudio::IddObjectType::BuildingSurface_Detailed);
  idfObject1.setString(BuildingSurface_DetailedFields::Name, "Surface 1");
  idfObject1.setString(BuildingSurface_DetailedFields::SurfaceType, "Wall");
  idfObject1.setString(BuildingSurface_DetailedFields::ConstructionName, "");
  idfObject1.setString(BuildingSurface_DetailedFields::ZoneName, "Thermal Zone 1");
  idfObject1.setString(BuildingSurface_DetailedFields::OutsideBoundaryCondition, "Outdoors");
  idfObject1.setString(BuildingSurface_DetailedFields::OutsideBoundaryConditionObject, "");
  idfObject1.setString(BuildingSurface_DetailedFields::SunExposure, "SunExposed");
  idfObject1.setString(BuildingSurface_DetailedFields::WindExposure, "WindExposed");
  idfObject1.setString(BuildingSurface_DetailedFields::ViewFactortoGround, "");
  idfObject1.setString(BuildingSurface_DetailedFields::NumberofVertices, "");
  IdfExtensibleGroup group1 = idfObject1.pushExtensibleGroup();  // vertex 1
  group1.setDouble(0, 0);
  group1.setDouble(1, 2);
  group1.setDouble(2, 0);
  IdfExtensibleGroup group2 = idfObject1.pushExtensibleGroup();  // vertex 2
  group2.setDouble(0, 0);
  group2.setDouble(1, 0);
  group2.setDouble(2, 0);
  IdfExtensibleGroup group3 = idfObject1.pushExtensibleGroup();  // vertex 3
  group3.setDouble(0, 2);
  group3.setDouble(1, 0);
  group3.setDouble(2, 0);
  IdfExtensibleGroup group4 = idfObject1.pushExtensibleGroup();  // vertex 4
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
  IdfExtensibleGroup group5 = idfObject2.pushExtensibleGroup();  // vertex 1
  group5.setDouble(0, 0);
  group5.setDouble(1, 1);
  group5.setDouble(2, 0);
  IdfExtensibleGroup group6 = idfObject2.pushExtensibleGroup();  // vertex 2
  group6.setDouble(0, 0);
  group6.setDouble(1, 0);
  group6.setDouble(2, 0);
  IdfExtensibleGroup group7 = idfObject2.pushExtensibleGroup();  // vertex 3
  group7.setDouble(0, 1);
  group7.setDouble(1, 0);
  group7.setDouble(2, 0);
  IdfExtensibleGroup group8 = idfObject2.pushExtensibleGroup();  // vertex 4
  group8.setDouble(0, 1);
  group8.setDouble(1, 1);
  group8.setDouble(2, 0);

  openstudio::WorkspaceObject epSubSurface = workspace.addObject(idfObject2).get();

  // Currently WindowMaterial:Blind is not ReverseTranslated... So we'll fake something using a Construction (and not a Shading Material Name) that
  // has no blind on it...
  // openstudio::IdfObject idfObject3(openstudio::IddObjectType::WindowMaterial_Blind);
  // idfObject3.setString(WindowMaterial_BlindFields::Name, "Shade 1");

  // openstudio::WorkspaceObject epBlind = workspace.addObject(idfObject3).get();

  IdfObject glazing(IddObjectType::WindowMaterial_Glazing);
  IdfObject gas(IddObjectType::WindowMaterial_Gas);
  glazing.setName("Glazing Material");
  gas.setName("Gas Material");
  IdfObject construction(IddObjectType::Construction);
  construction.setName("Construction A");
  ASSERT_EQ(0u, construction.numExtensibleGroups());
  EXPECT_FALSE(construction.pushExtensibleGroup(StringVector(1u, glazing.name().get())).empty());
  EXPECT_FALSE(construction.pushExtensibleGroup(StringVector(1u, gas.name().get())).empty());
  EXPECT_FALSE(construction.pushExtensibleGroup(StringVector(1u, glazing.name().get())).empty());
  IdfObjectVector objects;
  objects.push_back(glazing);
  objects.push_back(gas);
  objects.push_back(construction);
  EXPECT_EQ(3u, workspace.addObjects(objects).size());

  openstudio::IdfObject idfObject4(openstudio::IddObjectType::Schedule_Constant);
  idfObject4.setString(0, "Schedule 1");
  idfObject4.setString(1, "0.5");

  openstudio::WorkspaceObject epSchedule = workspace.addObject(idfObject4).get();

  openstudio::IdfObject idfObject5(openstudio::IddObjectType::WindowShadingControl);
  idfObject5.setString(WindowShadingControlFields::Name, "Shading Control 1");
  idfObject5.setString(WindowShadingControlFields::ZoneName, "");
  idfObject5.setString(WindowShadingControlFields::ShadingControlSequenceNumber, "1");
  idfObject5.setString(WindowShadingControlFields::ShadingType, "InteriorBlind");
  idfObject5.setString(WindowShadingControlFields::ConstructionwithShadingName, construction.nameString());
  // This Shading Control Type accepts 2 setpoints and a Schedule
  idfObject5.setString(WindowShadingControlFields::ShadingControlType, "OnIfHighZoneAirTempAndHighSolarOnWindow");
  idfObject5.setString(WindowShadingControlFields::ScheduleName, "Schedule 1");
  idfObject5.setString(WindowShadingControlFields::Setpoint, "30");
  idfObject5.setString(WindowShadingControlFields::ShadingControlIsScheduled, "Yes");
  idfObject5.setString(WindowShadingControlFields::GlareControlIsActive, "No");
  // idfObject5.setString(WindowShadingControlFields::ShadingDeviceMaterialName, "Shade 1");
  idfObject5.setString(WindowShadingControlFields::TypeofSlatAngleControlforBlinds, "");
  idfObject5.setString(WindowShadingControlFields::SlatAngleScheduleName, "");
  idfObject5.setString(WindowShadingControlFields::Setpoint2, "500.0");
  idfObject5.setString(WindowShadingControlFields::MultipleSurfaceControlType, "Sequential");
  IdfExtensibleGroup group9 = idfObject5.pushExtensibleGroup();  // sub surface
  group9.setString(0, "Sub Surface 1");

  openstudio::WorkspaceObject epWindowShadingControl = workspace.addObject(idfObject5).get();

  std::vector<WorkspaceObject> objVector(workspace.getObjectsByType(IddObjectType::WindowShadingControl));
  ASSERT_EQ(1u, objVector.size());
  WorkspaceObject wo1(objVector.at(0));
  EXPECT_EQ("Sequential", wo1.getString(WindowShadingControlFields::MultipleSurfaceControlType, false).get());
  ASSERT_EQ(1u, wo1.extensibleGroups().size());

  ReverseTranslator trans;
  ASSERT_NO_THROW(trans.translateWorkspace(workspace));
  Model model = trans.translateWorkspace(workspace);

  EXPECT_EQ(1u, model.getModelObjects<Surface>().size());
  EXPECT_EQ(1u, model.getModelObjects<SubSurface>().size());
  EXPECT_EQ(1u, model.getModelObjects<ScheduleConstant>().size());

  std::vector<ShadingControl> shadingControls = model.getModelObjects<ShadingControl>();
  ASSERT_EQ(1u, shadingControls.size());
  ShadingControl shadingControl = shadingControls[0];
  EXPECT_EQ("InteriorDaylightRedirectionDevice",
            shadingControl.shadingType());  // InteriorBlind maps to InteriorDaylightRedirectionDevice for some reason
  EXPECT_EQ("OnIfHighZoneAirTempAndHighSolarOnWindow", shadingControl.shadingControlType());
  EXPECT_TRUE(shadingControl.schedule());
  ASSERT_TRUE(shadingControl.setpoint());
  EXPECT_EQ(30.0, shadingControl.setpoint().get());
  ASSERT_TRUE(shadingControl.setpoint2());
  EXPECT_EQ(500.0, shadingControl.setpoint2().get());
  EXPECT_EQ("Sequential", shadingControl.multipleSurfaceControlType());
  EXPECT_EQ(1, shadingControl.numberofSubSurfaces());
  EXPECT_FALSE(shadingControl.slatAngleSchedule());
  EXPECT_TRUE(shadingControl.isTypeofSlatAngleControlforBlindsDefaulted());

  std::vector<SubSurface> subSurfaces = model.getModelObjects<SubSurface>();
  ASSERT_EQ(1u, subSurfaces.size());
  SubSurface subSurface = subSurfaces[0];
  EXPECT_EQ("Sub Surface 1", subSurface.name().get());

  ASSERT_EQ(1, subSurface.numberofShadingControls());
  ASSERT_TRUE(subSurface.shadingControl());
  ShadingControl shadingControl2 = subSurface.shadingControl().get();
  EXPECT_EQ(shadingControl, shadingControl2);
}
