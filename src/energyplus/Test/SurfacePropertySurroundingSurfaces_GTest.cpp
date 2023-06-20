/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
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

TEST_F(EnergyPlusFixture, ForwardTranslator_SurfacePropertySurroundingSurfaces) {

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

  SurfacePropertySurroundingSurfaces sp(m);
  sp.setName("SurfaceProperty Surrounding Surfaces");

  // Not assigned to a LocalEnv? not translate
  {
    // Assigned to a Surface
    Workspace w = ft.translateModel(m);

    EXPECT_EQ(0, w.getObjectsByType(IddObjectType::SurfaceProperty_LocalEnvironment).size());
    EXPECT_EQ(0, w.getObjectsByType(IddObjectType::SurfaceProperty_SurroundingSurfaces).size());
  }

  SurfacePropertyLocalEnvironment localEnv(surface);
  localEnv.setName(surface.nameString() + " LocalEnv");
  EXPECT_TRUE(localEnv.setSurfacePropertySurroundingSurfaces(sp));
  ASSERT_TRUE(localEnv.surfacePropertySurroundingSurfaces());
  EXPECT_EQ(sp, localEnv.surfacePropertySurroundingSurfaces().get());

  // Create 5 groups
  for (int i = 1; i <= 5; ++i) {
    ScheduleConstant tempSch(m);
    tempSch.setName("tempSch" + std::to_string(i));

    std::string sfName = "External Surface " + std::to_string(i);
    double viewFactor = 0.01 * i;

    if (i % 2 == 0) {
      EXPECT_TRUE(sp.addSurroundingSurfaceGroup(sfName, viewFactor, tempSch));
    } else {
      SurroundingSurfaceGroup group(sfName, viewFactor, tempSch);
      EXPECT_TRUE(sp.addSurroundingSurfaceGroup(group));
    }
    EXPECT_EQ(i, sp.numberofSurroundingSurfaceGroups());
  }
  EXPECT_EQ(5, sp.numberofSurroundingSurfaceGroups());

  std::vector<SurroundingSurfaceGroup> groups = sp.surroundingSurfaceGroups();

  {
    // Only assigned the groups here
    Workspace w = ft.translateModel(m);

    std::vector<WorkspaceObject> woEnvs = w.getObjectsByType(IddObjectType::SurfaceProperty_LocalEnvironment);
    ASSERT_EQ(1, woEnvs.size());
    std::vector<WorkspaceObject> woSps = w.getObjectsByType(IddObjectType::SurfaceProperty_SurroundingSurfaces);
    ASSERT_EQ(1, woSps.size());

    auto& wo_localEnv = woEnvs[0];
    EXPECT_EQ(localEnv.nameString(), wo_localEnv.nameString());

    EXPECT_EQ(sp.nameString(), wo_localEnv.getString(SurfaceProperty_LocalEnvironmentFields::SurroundingSurfacesObjectName).get());
    auto& woSp = woSps[0];
    EXPECT_EQ(sp.nameString(), woSp.nameString());

    EXPECT_TRUE(woSp.isEmpty(SurfaceProperty_SurroundingSurfacesFields::SkyViewFactor));
    EXPECT_TRUE(woSp.isEmpty(SurfaceProperty_SurroundingSurfacesFields::SkyTemperatureScheduleName));
    EXPECT_TRUE(woSp.isEmpty(SurfaceProperty_SurroundingSurfacesFields::GroundViewFactor));
    EXPECT_TRUE(woSp.isEmpty(SurfaceProperty_SurroundingSurfacesFields::GroundTemperatureScheduleName));

    ASSERT_EQ(5, woSp.numExtensibleGroups());
    for (const auto& idf_eg : woSp.extensibleGroups()) {
      auto& group = groups[idf_eg.groupIndex()];

      EXPECT_EQ(group.surroundingSurfaceName(), idf_eg.getString(SurfaceProperty_SurroundingSurfacesExtensibleFields::SurroundingSurfaceName).get());
      EXPECT_EQ(group.viewFactor(), idf_eg.getDouble(SurfaceProperty_SurroundingSurfacesExtensibleFields::SurroundingSurfaceViewFactor).get());
      EXPECT_EQ(group.temperatureSchedule().nameString(),
                idf_eg.getString(SurfaceProperty_SurroundingSurfacesExtensibleFields::SurroundingSurfaceTemperatureScheduleName).get());
    }
  }

  EXPECT_TRUE(sp.setSkyViewFactor(0.06));
  EXPECT_EQ(0.06, sp.skyViewFactor().get());

  ScheduleConstant skyTempSch(m);
  skyTempSch.setName("Sky Temperature Schedule");
  EXPECT_TRUE(sp.setSkyTemperatureSchedule(skyTempSch));
  EXPECT_EQ(skyTempSch, sp.skyTemperatureSchedule().get());

  EXPECT_TRUE(sp.setGroundViewFactor(0.07));
  EXPECT_EQ(0.07, sp.groundViewFactor().get());

  ScheduleConstant groundTempSch(m);
  groundTempSch.setName("Ground Temperature Schedule");
  EXPECT_TRUE(sp.setGroundTemperatureSchedule(groundTempSch));
  EXPECT_EQ(groundTempSch, sp.groundTemperatureSchedule().get());

  {
    // Only assigned the groups here
    Workspace w = ft.translateModel(m);

    std::vector<WorkspaceObject> woEnvs = w.getObjectsByType(IddObjectType::SurfaceProperty_LocalEnvironment);
    ASSERT_EQ(1, woEnvs.size());
    std::vector<WorkspaceObject> woSps = w.getObjectsByType(IddObjectType::SurfaceProperty_SurroundingSurfaces);
    ASSERT_EQ(1, woSps.size());

    auto& wo_localEnv = woEnvs[0];
    EXPECT_EQ(localEnv.nameString(), wo_localEnv.nameString());

    EXPECT_EQ(sp.nameString(), wo_localEnv.getString(SurfaceProperty_LocalEnvironmentFields::SurroundingSurfacesObjectName).get());
    auto& woSp = woSps[0];
    EXPECT_EQ(sp.nameString(), woSp.nameString());

    EXPECT_EQ(0.06, woSp.getDouble(SurfaceProperty_SurroundingSurfacesFields::SkyViewFactor).get());
    EXPECT_EQ("Sky Temperature Schedule", woSp.getString(SurfaceProperty_SurroundingSurfacesFields::SkyTemperatureScheduleName).get());
    EXPECT_EQ(0.07, woSp.getDouble(SurfaceProperty_SurroundingSurfacesFields::GroundViewFactor).get());
    EXPECT_EQ("Ground Temperature Schedule", woSp.getString(SurfaceProperty_SurroundingSurfacesFields::GroundTemperatureScheduleName).get());

    ASSERT_EQ(5, woSp.numExtensibleGroups());
    for (const auto& idf_eg : woSp.extensibleGroups()) {
      auto& group = groups[idf_eg.groupIndex()];

      EXPECT_EQ(group.surroundingSurfaceName(), idf_eg.getString(SurfaceProperty_SurroundingSurfacesExtensibleFields::SurroundingSurfaceName).get());
      EXPECT_EQ(group.viewFactor(), idf_eg.getDouble(SurfaceProperty_SurroundingSurfacesExtensibleFields::SurroundingSurfaceViewFactor).get());
      EXPECT_EQ(group.temperatureSchedule().nameString(),
                idf_eg.getString(SurfaceProperty_SurroundingSurfacesExtensibleFields::SurroundingSurfaceTemperatureScheduleName).get());
    }
  }
}

TEST_F(EnergyPlusFixture, ReverseTranslator_SurfacePropertySurroundingSurfaces) {

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

  EXPECT_TRUE(wo_sp.setDouble(SurfaceProperty_SurroundingSurfacesFields::SkyViewFactor, 0.5));

  auto skyTempSch = w.addObject(IdfObject(IddObjectType::Schedule_Constant)).get();
  skyTempSch.setName("skyTempSch");
  EXPECT_TRUE(skyTempSch.setDouble(Schedule_ConstantFields::HourlyValue, 23.5));
  EXPECT_TRUE(wo_sp.setPointer(SurfaceProperty_SurroundingSurfacesFields::SkyTemperatureScheduleName, skyTempSch.handle()));

  EXPECT_TRUE(wo_sp.setDouble(SurfaceProperty_SurroundingSurfacesFields::GroundViewFactor, 0.2));

  auto groundTempSch = w.addObject(IdfObject(IddObjectType::Schedule_Constant)).get();
  groundTempSch.setName("groundTempSch");
  EXPECT_TRUE(groundTempSch.setDouble(Schedule_ConstantFields::HourlyValue, 25.5));
  EXPECT_TRUE(wo_sp.setPointer(SurfaceProperty_SurroundingSurfacesFields::GroundTemperatureScheduleName, groundTempSch.handle()));

  auto tempSch1 = w.addObject(IdfObject(IddObjectType::Schedule_Constant)).get();
  tempSch1.setName("tempSch1");
  EXPECT_TRUE(tempSch1.setDouble(Schedule_ConstantFields::HourlyValue, 13.5));

  auto tempSch2 = w.addObject(IdfObject(IddObjectType::Schedule_Constant)).get();
  tempSch2.setName("tempSch2");
  EXPECT_TRUE(tempSch2.setDouble(Schedule_ConstantFields::HourlyValue, 33.5));

  struct Group
  {
    Group(std::string s, double v, std::string sch) : surroundingSurfaceName(std::move(s)), viewFactor(v), scheduleName(std::move(sch)) {}

    std::string surroundingSurfaceName;
    double viewFactor;
    std::string scheduleName;
  };

  std::vector<Group> check_groups;

  for (int i = 1; i <= 5; ++i) {
    auto w_eg = wo_sp.pushExtensibleGroup().cast<WorkspaceExtensibleGroup>();
    std::string sfName = "External Surface " + std::to_string(i);
    EXPECT_TRUE(w_eg.setString(SurfaceProperty_SurroundingSurfacesExtensibleFields::SurroundingSurfaceName, sfName));

    double viewFactor = 0.01 * i;
    EXPECT_TRUE(w_eg.setDouble(SurfaceProperty_SurroundingSurfacesExtensibleFields::SurroundingSurfaceViewFactor, viewFactor));

    if (i % 2 == 0) {
      EXPECT_TRUE(w_eg.setPointer(SurfaceProperty_SurroundingSurfacesExtensibleFields::SurroundingSurfaceTemperatureScheduleName, tempSch1.handle()));
      check_groups.emplace_back(sfName, viewFactor, tempSch1.nameString());
    } else {
      EXPECT_TRUE(w_eg.setPointer(SurfaceProperty_SurroundingSurfacesExtensibleFields::SurroundingSurfaceTemperatureScheduleName, tempSch2.handle()));
      check_groups.emplace_back(sfName, viewFactor, tempSch2.nameString());
    }
  }

  Model m = rt.translateWorkspace(w);

  EXPECT_EQ(1, m.getConcreteModelObjects<Surface>().size());

  auto sps = m.getConcreteModelObjects<SurfacePropertySurroundingSurfaces>();
  ASSERT_EQ(1, sps.size());
  auto& sp = sps[0];

  ASSERT_TRUE(sp.skyViewFactor());
  EXPECT_EQ(0.5, sp.skyViewFactor().get());
  ASSERT_TRUE(sp.skyTemperatureSchedule());
  EXPECT_EQ("skyTempSch", sp.skyTemperatureSchedule()->nameString());

  ASSERT_TRUE(sp.groundViewFactor());
  EXPECT_EQ(0.2, sp.groundViewFactor().get());
  ASSERT_TRUE(sp.groundTemperatureSchedule());
  EXPECT_EQ("groundTempSch", sp.groundTemperatureSchedule()->nameString());

  ASSERT_EQ(5, sp.numberofSurroundingSurfaceGroups());
  auto groups = sp.surroundingSurfaceGroups();
  ASSERT_EQ(5, groups.size());

  for (size_t i = 0; i < 5; ++i) {
    auto& group = groups[i];
    auto& check_group = check_groups[i];
    EXPECT_EQ(check_group.surroundingSurfaceName, group.surroundingSurfaceName());
    EXPECT_EQ(check_group.viewFactor, group.viewFactor());
    EXPECT_EQ(check_group.scheduleName, group.temperatureSchedule().nameString());
  }
}
