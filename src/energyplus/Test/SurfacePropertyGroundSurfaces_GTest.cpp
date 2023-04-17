/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2023, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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
#include "../../model/SurfacePropertyGroundSurfaces.hpp"
#include "../../model/SurfacePropertyGroundSurfaces_Impl.hpp"
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
#include <utilities/idd/SurfaceProperty_GroundSurfaces_FieldEnums.hxx>
#include <utilities/idd/Schedule_Constant_FieldEnums.hxx>

#include <vector>

using namespace openstudio::energyplus;
using namespace openstudio::model;
using namespace openstudio;

TEST_F(EnergyPlusFixture, ForwardTranslator_SurfacePropertyGroundSurfaces) {

  ForwardTranslator ft;

  // Zone translates Space, Space translates Surface, Surface translates its LocalEnv object, and this in turns translates our
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

  SurfacePropertyGroundSurfaces sp(m);
  sp.setName("SurfaceProperty Ground Surfaces");

  // Not assigned to a LocalEnv? not translate
  {
    // Assigned to a Surface
    Workspace w = ft.translateModel(m);

    EXPECT_EQ(0, w.getObjectsByType(IddObjectType::SurfaceProperty_LocalEnvironment).size());
    EXPECT_EQ(0, w.getObjectsByType(IddObjectType::SurfaceProperty_GroundSurfaces).size());
  }

  SurfacePropertyLocalEnvironment localEnv(surface);
  localEnv.setName(surface.nameString() + " LocalEnv");
  EXPECT_TRUE(localEnv.setSurfacePropertyGroundSurfaces(sp));
  ASSERT_TRUE(localEnv.surfacePropertyGroundSurfaces());
  EXPECT_EQ(sp, localEnv.surfacePropertyGroundSurfaces().get());

  // Create 5 groups
  for (int i = 1; i <= 5; ++i) {
    ScheduleConstant tempSch(m);
    tempSch.setName("tempSch" + std::to_string(i));

    ScheduleConstant reflectanceSch(m);
    reflectanceSch.setName("reflectanceSch" + std::to_string(i));

    std::string sfName = "External Surface " + std::to_string(i);
    double viewFactor = 0.01 * i;

    if (i % 2 == 0) {
      EXPECT_TRUE(sp.addGroundSurfaceGroup(sfName, viewFactor, tempSch, boost::none));
    } else {
      GroundSurfaceGroup group(sfName, viewFactor, boost::none, reflectanceSch);
      EXPECT_TRUE(sp.addGroundSurfaceGroup(group));
    }
    EXPECT_EQ(i, sp.numberofGroundSurfaceGroups());
  }
  EXPECT_EQ(5, sp.numberofGroundSurfaceGroups());

  std::vector<GroundSurfaceGroup> groups = sp.groundSurfaceGroups();

  {
    // Only assigned the groups here
    Workspace w = ft.translateModel(m);

    std::vector<WorkspaceObject> woEnvs = w.getObjectsByType(IddObjectType::SurfaceProperty_LocalEnvironment);
    ASSERT_EQ(1, woEnvs.size());
    std::vector<WorkspaceObject> woSps = w.getObjectsByType(IddObjectType::SurfaceProperty_GroundSurfaces);
    ASSERT_EQ(1, woSps.size());

    auto& wo_localEnv = woEnvs[0];
    EXPECT_EQ(localEnv.nameString(), wo_localEnv.nameString());

    EXPECT_EQ(sp.nameString(), wo_localEnv.getString(SurfaceProperty_LocalEnvironmentFields::GroundSurfacesObjectName).get());
    auto& woSp = woSps[0];
    EXPECT_EQ(sp.nameString(), woSp.nameString());

    ASSERT_EQ(5, woSp.numExtensibleGroups());
    for (const auto& idf_eg : woSp.extensibleGroups()) {
      auto& group = groups[idf_eg.groupIndex()];

      EXPECT_EQ(group.groundSurfaceName(), idf_eg.getString(SurfaceProperty_GroundSurfacesExtensibleFields::GroundSurfaceName).get());
      EXPECT_EQ(group.viewFactor(), idf_eg.getDouble(SurfaceProperty_GroundSurfacesExtensibleFields::GroundSurfaceViewFactor).get());
      if ((idf_eg.groupIndex() + 1) % 2 == 0) {
        ASSERT_TRUE(group.temperatureSchedule());
        EXPECT_EQ(group.temperatureSchedule()->nameString(),
                  idf_eg.getString(SurfaceProperty_GroundSurfacesExtensibleFields::GroundSurfaceTemperatureScheduleName).get());
        EXPECT_FALSE(group.reflectanceSchedule());
      } else {
        EXPECT_FALSE(group.temperatureSchedule());
        ASSERT_TRUE(group.reflectanceSchedule());
        EXPECT_EQ(group.reflectanceSchedule()->nameString(),
                  idf_eg.getString(SurfaceProperty_GroundSurfacesExtensibleFields::GroundSurfaceReflectanceScheduleName).get());
      }
    }
  }
}

TEST_F(EnergyPlusFixture, ReverseTranslator_SurfacePropertyGroundSurfaces) {

  ReverseTranslator rt;

  Workspace w(StrictnessLevel::Minimal, IddFileType::EnergyPlus);

  auto wo_zone = w.addObject(IdfObject(IddObjectType::Zone)).get();
  wo_zone.setName("Thermal Zone 1");

  auto wo_sp = w.addObject(IdfObject(IddObjectType::SurfaceProperty_GroundSurfaces)).get();

  auto tempSch = w.addObject(IdfObject(IddObjectType::Schedule_Constant)).get();
  tempSch.setName("tempSch1");
  EXPECT_TRUE(tempSch.setDouble(Schedule_ConstantFields::HourlyValue, 13.5));

  auto reflectanceSch = w.addObject(IdfObject(IddObjectType::Schedule_Constant)).get();
  reflectanceSch.setName("reflectanceSch");
  EXPECT_TRUE(reflectanceSch.setDouble(Schedule_ConstantFields::HourlyValue, 0.9));

  struct Group
  {
    Group(std::string s, double v, std::string tempSch, std::string refSch)
      : groundSurfaceName(std::move(s)), viewFactor(v), temperatureScheduleName(std::move(tempSch)), reflectanceScheduleName(std::move(refSch)) {}

    std::string groundSurfaceName;
    double viewFactor;
    std::string temperatureScheduleName;
    std::string reflectanceScheduleName;
  };

  std::vector<Group> check_groups;

  for (int i = 1; i <= 5; ++i) {
    auto w_eg = wo_sp.pushExtensibleGroup().cast<WorkspaceExtensibleGroup>();
    std::string sfName = "External Surface " + std::to_string(i);
    EXPECT_TRUE(w_eg.setString(SurfaceProperty_GroundSurfacesExtensibleFields::GroundSurfaceName, sfName));

    double viewFactor = 0.01 * i;
    EXPECT_TRUE(w_eg.setDouble(SurfaceProperty_GroundSurfacesExtensibleFields::GroundSurfaceViewFactor, viewFactor));

    if (i % 2 == 0) {
      EXPECT_TRUE(w_eg.setPointer(SurfaceProperty_GroundSurfacesExtensibleFields::GroundSurfaceTemperatureScheduleName, tempSch.handle()));
      check_groups.emplace_back(sfName, viewFactor, tempSch.nameString(), "");
    } else {
      EXPECT_TRUE(w_eg.setPointer(SurfaceProperty_GroundSurfacesExtensibleFields::GroundSurfaceReflectanceScheduleName, reflectanceSch.handle()));
      check_groups.emplace_back(sfName, viewFactor, "", reflectanceSch.nameString());
    }
  }

  Model m = rt.translateWorkspace(w);

  auto sps = m.getConcreteModelObjects<SurfacePropertyGroundSurfaces>();
  ASSERT_EQ(1, sps.size());
  auto& sp = sps[0];

  ASSERT_EQ(5, sp.numberofGroundSurfaceGroups());
  auto groups = sp.groundSurfaceGroups();
  ASSERT_EQ(5, groups.size());

  for (size_t i = 0; i < 5; ++i) {
    auto& group = groups[i];
    auto& check_group = check_groups[i];
    EXPECT_EQ(check_group.groundSurfaceName, group.groundSurfaceName());
    EXPECT_EQ(check_group.viewFactor, group.viewFactor());
    if (!check_group.temperatureScheduleName.empty()) {
      ASSERT_TRUE(group.temperatureSchedule());
      EXPECT_EQ(check_group.temperatureScheduleName, group.temperatureSchedule()->nameString());
    } else {
      EXPECT_FALSE(group.temperatureSchedule());
    }
    if (!check_group.reflectanceScheduleName.empty()) {
      ASSERT_TRUE(group.reflectanceSchedule());
      EXPECT_EQ(check_group.reflectanceScheduleName, group.reflectanceSchedule()->nameString());
    } else {
      EXPECT_FALSE(group.reflectanceSchedule());
    }
  }
}
