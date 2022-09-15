/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2022, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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
#include "../../model/ThermalZone.hpp"
#include "../../model/Space.hpp"
#include "../../model/SurfacePropertyLocalEnvironment.hpp"
#include "../../model/SurfacePropertyLocalEnvironment_Impl.hpp"
#include "../../model/SurfacePropertySurroundingSurfaces.hpp"
#include "../../model/SurfacePropertySurroundingSurfaces_Impl.hpp"
#include "../../model/ScheduleConstant.hpp"
#include "../../model/Surface.hpp"
#include "../../model/Surface_Impl.hpp"
#include "../../model/SubSurface.hpp"

#include "../../utilities/idf/WorkspaceObject.hpp"
#include "../../utilities/geometry/Point3d.hpp"
#include "../../utilities/idf/IdfExtensibleGroup.hpp"
#include "../../utilities/idf/WorkspaceExtensibleGroup.hpp"

#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/IddFactory.hxx>
#include <utilities/idd/SurfaceProperty_LocalEnvironment_FieldEnums.hxx>
#include <utilities/idd/SurfaceProperty_SurroundingSurfaces_FieldEnums.hxx>
#include <utilities/idd/BuildingSurface_Detailed_FieldEnums.hxx>
#include <utilities/idd/Schedule_Constant_FieldEnums.hxx>

#include <vector>

using namespace openstudio::energyplus;
using namespace openstudio::model;
using namespace openstudio;

TEST_F(EnergyPlusFixture, ForwardTranslator_SurfacePropertyLocalEnvironment) {

  Model m;
  ThermalZone tz(m);
  Space space(m);
  EXPECT_TRUE(space.setThermalZone(tz));

  std::vector<Point3d> sPoints{
    {0, 2, 0},
    {0, 0, 0},
    {2, 0, 0},
    {2, 2, 0},
  };
  Surface surface(sPoints, m);
  surface.setSpace(space);

  std::vector<Point3d> ssPoints{
    {0, 1, 0},
    {0, 0, 0},
    {1, 0, 0},
    {1, 1, 0},
  };

  SubSurface subSurface(ssPoints, m);
  subSurface.setSurface(surface);
  subSurface.assignDefaultSubSurfaceType();

  SurfacePropertyLocalEnvironment localEnv(surface);
  localEnv.setName(surface.nameString() + " LocalEnv");
  EXPECT_EQ(surface, localEnv.exteriorSurface());
  ASSERT_TRUE(localEnv.exteriorSurfaceAsSurface());
  EXPECT_EQ(surface, localEnv.exteriorSurfaceAsSurface().get());

  ScheduleConstant shadingSch(m);
  shadingSch.setName("External Shading Fraction Schedule");
  shadingSch.setValue(0.5);
  EXPECT_TRUE(localEnv.setExternalShadingFractionSchedule(shadingSch));
  ASSERT_TRUE(localEnv.externalShadingFractionSchedule());
  EXPECT_EQ(shadingSch, localEnv.externalShadingFractionSchedule().get());

  SurfacePropertySurroundingSurfaces sp(m);
  sp.setName("SurfaceProperty Surrounding Surfaces");

  EXPECT_TRUE(localEnv.setSurfacePropertySurroundingSurfaces(sp));
  ASSERT_TRUE(localEnv.surfacePropertySurroundingSurfaces());
  EXPECT_EQ(sp, localEnv.surfacePropertySurroundingSurfaces().get());
  ScheduleConstant tempSch(m);
  tempSch.setName("External Surface Temperature Schedule");
  tempSch.setValue(23.5);
  EXPECT_TRUE(sp.addSurroundingSurfaceGroup("External Surface 1", 0.4, tempSch));
  EXPECT_EQ(1, sp.numberofSurroundingSurfaceGroups());

  ForwardTranslator ft;

  {
    // Assigned to a Surface
    Workspace w = ft.translateModel(m);

    ASSERT_EQ(1, w.getObjectsByType(IddObjectType::BuildingSurface_Detailed).size());
    ASSERT_EQ(1, w.getObjectsByType(IddObjectType::FenestrationSurface_Detailed).size());

    std::vector<WorkspaceObject> idfObjs = w.getObjectsByType(IddObjectType::SurfaceProperty_LocalEnvironment);
    ASSERT_EQ(1, idfObjs.size());
    auto& wo_localEnv = idfObjs[0];
    EXPECT_EQ(localEnv.nameString(), wo_localEnv.nameString());

    auto wo_extSf_ = wo_localEnv.getTarget(SurfaceProperty_LocalEnvironmentFields::ExteriorSurfaceName);
    ASSERT_TRUE(wo_extSf_);
    EXPECT_EQ(wo_extSf_->iddObject().type(), IddObjectType::BuildingSurface_Detailed);

    EXPECT_EQ(shadingSch.nameString(), wo_localEnv.getString(SurfaceProperty_LocalEnvironmentFields::ExternalShadingFractionScheduleName).get());

    EXPECT_EQ(sp.nameString(), wo_localEnv.getString(SurfaceProperty_LocalEnvironmentFields::SurroundingSurfacesObjectName).get());
    auto wo_sp_ = wo_localEnv.getTarget(SurfaceProperty_LocalEnvironmentFields::SurroundingSurfacesObjectName);
    ASSERT_TRUE(wo_sp_);
    EXPECT_EQ(wo_sp_->iddObject().type(), IddObjectType::SurfaceProperty_SurroundingSurfaces);
  }

  EXPECT_TRUE(localEnv.setExteriorSurface(subSurface));
  EXPECT_EQ(subSurface, localEnv.exteriorSurface());
  ASSERT_TRUE(localEnv.exteriorSurfaceAsSubSurface());
  EXPECT_EQ(subSurface, localEnv.exteriorSurfaceAsSubSurface().get());

  {
    // Assigned to a SubSurface
    Workspace w = ft.translateModel(m);

    ASSERT_EQ(1, w.getObjectsByType(IddObjectType::BuildingSurface_Detailed).size());
    ASSERT_EQ(1, w.getObjectsByType(IddObjectType::FenestrationSurface_Detailed).size());

    std::vector<WorkspaceObject> idfObjs = w.getObjectsByType(IddObjectType::SurfaceProperty_LocalEnvironment);
    ASSERT_EQ(1, idfObjs.size());
    auto& wo_localEnv = idfObjs[0];
    EXPECT_EQ(localEnv.nameString(), wo_localEnv.nameString());

    auto wo_extSf_ = wo_localEnv.getTarget(SurfaceProperty_LocalEnvironmentFields::ExteriorSurfaceName);
    ASSERT_TRUE(wo_extSf_);
    EXPECT_EQ(wo_extSf_->iddObject().type(), IddObjectType::FenestrationSurface_Detailed);

    EXPECT_EQ(shadingSch.nameString(), wo_localEnv.getString(SurfaceProperty_LocalEnvironmentFields::ExternalShadingFractionScheduleName).get());

    EXPECT_EQ(sp.nameString(), wo_localEnv.getString(SurfaceProperty_LocalEnvironmentFields::SurroundingSurfacesObjectName).get());
    auto wo_sp_ = wo_localEnv.getTarget(SurfaceProperty_LocalEnvironmentFields::SurroundingSurfacesObjectName);
    ASSERT_TRUE(wo_sp_);
    EXPECT_EQ(wo_sp_->iddObject().type(), IddObjectType::SurfaceProperty_SurroundingSurfaces);
  }
}

TEST_F(EnergyPlusFixture, ReverseTranslator_SurfacePropertyLocalEnvironment) {

  ReverseTranslator rt;

  Workspace w(StrictnessLevel::Minimal, IddFileType::EnergyPlus);

  auto wo_zone = w.addObject(IdfObject(IddObjectType::Zone)).get();
  wo_zone.setName("Thermal Zone 1");

  auto wo_sf = w.addObject(IdfObject(IddObjectType::BuildingSurface_Detailed)).get();
  wo_sf.setName("Surface 1");
  EXPECT_TRUE(wo_sf.setString(BuildingSurface_DetailedFields::SurfaceType, "Wall"));
  EXPECT_TRUE(wo_sf.setString(BuildingSurface_DetailedFields::ConstructionName, ""));
  EXPECT_TRUE(wo_sf.setPointer(BuildingSurface_DetailedFields::ZoneName, wo_zone.handle()));
  EXPECT_TRUE(wo_sf.setString(BuildingSurface_DetailedFields::OutsideBoundaryCondition, "Outdoors"));
  EXPECT_TRUE(wo_sf.setString(BuildingSurface_DetailedFields::OutsideBoundaryConditionObject, ""));
  EXPECT_TRUE(wo_sf.setString(BuildingSurface_DetailedFields::SunExposure, "SunExposed"));
  EXPECT_TRUE(wo_sf.setString(BuildingSurface_DetailedFields::WindExposure, "WindExposed"));
  EXPECT_TRUE(wo_sf.setString(BuildingSurface_DetailedFields::ViewFactortoGround, ""));
  EXPECT_TRUE(wo_sf.setString(BuildingSurface_DetailedFields::NumberofVertices, ""));
  IdfExtensibleGroup group1 = wo_sf.pushExtensibleGroup();  // vertex 1
  group1.setDouble(0, 0);
  group1.setDouble(1, 2);
  group1.setDouble(2, 0);
  IdfExtensibleGroup group2 = wo_sf.pushExtensibleGroup();  // vertex 2
  group2.setDouble(0, 0);
  group2.setDouble(1, 0);
  group2.setDouble(2, 0);
  IdfExtensibleGroup group3 = wo_sf.pushExtensibleGroup();  // vertex 3
  group3.setDouble(0, 2);
  group3.setDouble(1, 0);
  group3.setDouble(2, 0);
  IdfExtensibleGroup group4 = wo_sf.pushExtensibleGroup();  // vertex 4
  group4.setDouble(0, 2);
  group4.setDouble(1, 2);
  group4.setDouble(2, 0);

  auto wo_sp = w.addObject(IdfObject(IddObjectType::SurfaceProperty_SurroundingSurfaces)).get();
  wo_sp.setName("SfProps");

  EXPECT_TRUE(wo_sp.setDouble(SurfaceProperty_SurroundingSurfacesFields::SkyViewFactor, 0.5));
  EXPECT_TRUE(wo_sp.setDouble(SurfaceProperty_SurroundingSurfacesFields::GroundViewFactor, 0.2));

  auto w_eg = wo_sp.pushExtensibleGroup().cast<WorkspaceExtensibleGroup>();
  EXPECT_TRUE(w_eg.setString(SurfaceProperty_SurroundingSurfacesExtensibleFields::SurroundingSurfaceName, "External Surface 1"));
  EXPECT_TRUE(w_eg.setDouble(SurfaceProperty_SurroundingSurfacesExtensibleFields::SurroundingSurfaceViewFactor, 0.25));

  auto tempSch1 = w.addObject(IdfObject(IddObjectType::Schedule_Constant)).get();
  tempSch1.setName("tempSch1");
  EXPECT_TRUE(tempSch1.setDouble(Schedule_ConstantFields::HourlyValue, 13.5));
  EXPECT_TRUE(w_eg.setPointer(SurfaceProperty_SurroundingSurfacesExtensibleFields::SurroundingSurfaceTemperatureScheduleName, tempSch1.handle()));

  auto wo_localEnv = w.addObject(IdfObject(IddObjectType::SurfaceProperty_LocalEnvironment)).get();
  wo_localEnv.setName("LocalEnv");

  auto externalShadingSch = w.addObject(IdfObject(IddObjectType::Schedule_Constant)).get();
  externalShadingSch.setName("externalShadingSch");
  EXPECT_TRUE(externalShadingSch.setDouble(Schedule_ConstantFields::HourlyValue, 0.5));
  EXPECT_TRUE(wo_localEnv.setPointer(SurfaceProperty_LocalEnvironmentFields::ExternalShadingFractionScheduleName, externalShadingSch.handle()));

  EXPECT_TRUE(wo_localEnv.setPointer(SurfaceProperty_LocalEnvironmentFields::ExteriorSurfaceName, wo_sf.handle()));

  EXPECT_TRUE(wo_localEnv.setPointer(SurfaceProperty_LocalEnvironmentFields::SurroundingSurfacesObjectName, wo_sp.handle()));

  // RT
  Model m = rt.translateWorkspace(w);

  ASSERT_EQ(1, m.getConcreteModelObjects<SurfacePropertySurroundingSurfaces>().size());
  ASSERT_EQ(1, m.getConcreteModelObjects<SurfacePropertyLocalEnvironment>().size());
  auto surfaces = m.getConcreteModelObjects<Surface>();
  ASSERT_EQ(1, surfaces.size());
  auto& surface = surfaces[0];

  ASSERT_TRUE(surface.surfacePropertyLocalEnvironment());
  auto localEnv = surface.surfacePropertyLocalEnvironment().get();
  EXPECT_EQ("LocalEnv", localEnv.nameString());
  EXPECT_EQ(surface, localEnv.exteriorSurface());

  ASSERT_TRUE(localEnv.externalShadingFractionSchedule());
  EXPECT_EQ("externalShadingSch", localEnv.externalShadingFractionSchedule()->nameString());

  ASSERT_TRUE(localEnv.surfacePropertySurroundingSurfaces());
  EXPECT_EQ("SfProps", localEnv.surfacePropertySurroundingSurfaces()->nameString());
}
