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
#include "../../model/SurfacePropertyIncidentSolarMultiplier.hpp"
#include "../../model/SurfacePropertyIncidentSolarMultiplier_Impl.hpp"
#include "../../model/ScheduleConstant.hpp"
#include "../../model/Surface.hpp"
#include "../../model/Surface_Impl.hpp"
#include "../../model/SubSurface.hpp"
#include "../../model/SubSurface_Impl.hpp"

#include "../../utilities/idf/WorkspaceObject.hpp"
#include "../../utilities/geometry/Point3d.hpp"
#include "../../utilities/idf/IdfExtensibleGroup.hpp"
#include "../../utilities/idf/WorkspaceExtensibleGroup.hpp"

#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/IddFactory.hxx>
#include <utilities/idd/SurfaceProperty_IncidentSolarMultiplier_FieldEnums.hxx>
#include <utilities/idd/BuildingSurface_Detailed_FieldEnums.hxx>
#include <utilities/idd/FenestrationSurface_Detailed_FieldEnums.hxx>
#include <utilities/idd/Schedule_Constant_FieldEnums.hxx>

#include <vector>

using namespace openstudio::energyplus;
using namespace openstudio::model;
using namespace openstudio;

TEST_F(EnergyPlusFixture, ForwardTranslator_SurfacePropertyIncidentSolarMultiplier) {

  ForwardTranslator ft;

  // Zone translates Space, Space translates Surface, Surface translates its LocalEnv object, and this in turns translates our
  Model m;
  ThermalZone tz(m);
  Space space(m);
  EXPECT_TRUE(space.setThermalZone(tz));

  std::vector<Point3d> sPoints{{0, 2, 0}, {0, 0, 0}, {2, 0, 0}, {2, 2, 0}};
  Surface surface(sPoints, m);
  surface.setSpace(space);

  std::vector<Point3d> ssPoints{{0, 0, 1}, {0, 0, 0}, {1, 0, 0}, {1, 0, 1}};

  SubSurface ss(ssPoints, m);
  ss.setName("SubSurface1");
  ss.setSurface(surface);

  SurfacePropertyIncidentSolarMultiplier sp(ss);
  // Incident Solar Multiplier: Required Double
  EXPECT_TRUE(sp.setIncidentSolarMultiplier(0.75));

  // Incident Solar Multiplier Schedule Name: Optional Object
  ScheduleConstant sch(m);
  EXPECT_TRUE(sp.setIncidentSolarMultiplierSchedule(sch));

  // Only assigned the groups here
  Workspace w = ft.translateModel(m);

  std::vector<WorkspaceObject> woSps = w.getObjectsByType(IddObjectType::SurfaceProperty_IncidentSolarMultiplier);
  ASSERT_EQ(1, woSps.size());
  auto& woSp = woSps.front();

  ASSERT_EQ(1, w.getObjectsByType(IddObjectType::BuildingSurface_Detailed).size());
  ASSERT_EQ(1, w.getObjectsByType(IddObjectType::FenestrationSurface_Detailed).size());

  EXPECT_EQ(ss.nameString(), woSp.getTarget(SurfaceProperty_IncidentSolarMultiplierFields::SurfaceName)->nameString());
  EXPECT_EQ("SubSurface1", woSp.getTarget(SurfaceProperty_IncidentSolarMultiplierFields::SurfaceName)->nameString());
  EXPECT_EQ(sp.incidentSolarMultiplier(), woSp.getDouble(SurfaceProperty_IncidentSolarMultiplierFields::IncidentSolarMultiplier).get());
  EXPECT_EQ(sch.nameString(), woSp.getString(SurfaceProperty_IncidentSolarMultiplierFields::IncidentSolarMultiplierScheduleName).get());
}

TEST_F(EnergyPlusFixture, ReverseTranslator_SurfacePropertyIncidentSolarMultiplier) {

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
  {
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
  }

  auto wo_ss = w.addObject(IdfObject(IddObjectType::FenestrationSurface_Detailed)).get();
  wo_ss.setName("SubSurface 1");
  EXPECT_TRUE(wo_ss.setString(FenestrationSurface_DetailedFields::SurfaceType, "Window"));
  EXPECT_TRUE(wo_ss.setString(FenestrationSurface_DetailedFields::ConstructionName, ""));
  EXPECT_TRUE(wo_ss.setPointer(FenestrationSurface_DetailedFields::BuildingSurfaceName, wo_sf.handle()));
  EXPECT_TRUE(wo_ss.setString(FenestrationSurface_DetailedFields::OutsideBoundaryConditionObject, ""));
  EXPECT_TRUE(wo_ss.setDouble(FenestrationSurface_DetailedFields::ViewFactortoGround, 0.0));
  EXPECT_TRUE(wo_ss.setString(FenestrationSurface_DetailedFields::FrameandDividerName, ""));
  EXPECT_TRUE(wo_ss.setDouble(FenestrationSurface_DetailedFields::Multiplier, 0.0));
  EXPECT_TRUE(wo_ss.setDouble(FenestrationSurface_DetailedFields::NumberofVertices, 0.0));
  {
    IdfExtensibleGroup group1 = wo_ss.pushExtensibleGroup();  // vertex 1
    group1.setDouble(0, 0);
    group1.setDouble(1, 1);
    group1.setDouble(2, 0);
    IdfExtensibleGroup group2 = wo_ss.pushExtensibleGroup();  // vertex 2
    group2.setDouble(0, 0);
    group2.setDouble(1, 0);
    group2.setDouble(2, 0);
    IdfExtensibleGroup group3 = wo_ss.pushExtensibleGroup();  // vertex 3
    group3.setDouble(0, 1);
    group3.setDouble(1, 0);
    group3.setDouble(2, 0);
    IdfExtensibleGroup group4 = wo_ss.pushExtensibleGroup();  // vertex 4
    group4.setDouble(0, 1);
    group4.setDouble(1, 1);
    group4.setDouble(2, 0);
  }

  auto multSch = w.addObject(IdfObject(IddObjectType::Schedule_Constant)).get();
  multSch.setName("multSch");
  EXPECT_TRUE(multSch.setDouble(Schedule_ConstantFields::HourlyValue, 0.9));

  auto wo_sp = w.addObject(IdfObject(IddObjectType::SurfaceProperty_IncidentSolarMultiplier)).get();

  EXPECT_TRUE(wo_sp.setPointer(SurfaceProperty_IncidentSolarMultiplierFields::SurfaceName, wo_ss.handle()));
  EXPECT_TRUE(wo_sp.setDouble(SurfaceProperty_IncidentSolarMultiplierFields::IncidentSolarMultiplier, 0.8));
  EXPECT_TRUE(wo_sp.setPointer(SurfaceProperty_IncidentSolarMultiplierFields::IncidentSolarMultiplierScheduleName, multSch.handle()));

  Model m = rt.translateWorkspace(w);

  EXPECT_EQ(1, m.getConcreteModelObjects<Surface>().size());
  EXPECT_EQ(1, m.getConcreteModelObjects<SubSurface>().size());

  auto sps = m.getConcreteModelObjects<SurfacePropertyIncidentSolarMultiplier>();
  ASSERT_EQ(1, sps.size());
  auto& sp = sps[0];
  EXPECT_EQ("SubSurface 1", sp.subSurface().nameString());
  EXPECT_EQ(0.8, sp.incidentSolarMultiplier());
  ASSERT_TRUE(sp.incidentSolarMultiplierSchedule());
  EXPECT_EQ("multSch", sp.incidentSolarMultiplierSchedule()->nameString());
}
