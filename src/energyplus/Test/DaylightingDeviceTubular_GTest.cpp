/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>
#include "EnergyPlusFixture.hpp"

#include "../ForwardTranslator.hpp"

#include "../../model/Model.hpp"
#include "../../model/Construction.hpp"
#include "../../model/ThermalZone.hpp"
#include "../../model/Space.hpp"
#include "../../model/Surface.hpp"
#include "../../model/SubSurface.hpp"
#include "../../model/DaylightingDeviceTubular.hpp"
#include "../../model/DaylightingDeviceTubular_Impl.hpp"

#include "../../utilities/geometry/Point3d.hpp"
#include <utilities/idd/DaylightingDevice_Tubular_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::energyplus;
using namespace openstudio::model;
using namespace openstudio;

TEST_F(EnergyPlusFixture, ForwardTranslator_DaylightingDeviceTubular) {
  Model model;
  Construction construction(model);
  ThermalZone zone(model);
  Space space(model);
  space.setThermalZone(zone);

  Point3dVector points{
    {0, 10, 3},
    {0, 10, 0},
    {0, 0, 0},
    {0, 0, 3},
  };
  Surface surface(points, model);
  surface.setSpace(space);
  EXPECT_EQ("Wall", surface.surfaceType());

  Point3dVector points2{
    {0, 0, 1},
    {0, 0, 0},
    {0, 1, 0},
    {0, 1, 1},
  };
  SubSurface dome(points2, model);
  dome.setSubSurfaceType("TubularDaylightDome");
  dome.setSurface(surface);

  Point3dVector points3{
    {0, 0, 2},
    {0, 0, 0},
    {0, 2, 0},
    {0, 2, 2},
  };
  SubSurface diffuser(points3, model);
  diffuser.setSubSurfaceType("TubularDaylightDiffuser");
  diffuser.setSurface(surface);

  EXPECT_FALSE(dome.daylightingDeviceTubular());
  EXPECT_FALSE(diffuser.daylightingDeviceTubular());
  DaylightingDeviceTubular tubular(dome, diffuser, construction, 1, 2, 3);
  EXPECT_TRUE(dome.daylightingDeviceTubular());
  EXPECT_TRUE(diffuser.daylightingDeviceTubular());
  EXPECT_EQ(1u, model.getConcreteModelObjects<DaylightingDeviceTubular>().size());

  ThermalZone thermalZone1(model);
  ThermalZone thermalZone2(model);

  std::vector<TransitionZone> transitionZonesToAdd;
  TransitionZone transitionZone1(thermalZone1, 4);
  transitionZonesToAdd.push_back(transitionZone1);
  TransitionZone transitionZone2(thermalZone2, 5);
  transitionZonesToAdd.push_back(transitionZone2);
  ASSERT_TRUE(tubular.addTransitionZones(transitionZonesToAdd));

  ForwardTranslator ft;
  Workspace w = ft.translateModel(model);

  std::vector<WorkspaceObject> wos = w.getObjectsByType(IddObjectType::DaylightingDevice_Tubular);
  ASSERT_EQ(1u, wos.size());
  WorkspaceObject wo(wos[0]);

  boost::optional<WorkspaceObject> idf_dome(wo.getTarget(DaylightingDevice_TubularFields::DomeName));
  ASSERT_TRUE(idf_dome);
  EXPECT_EQ(dome.nameString(), idf_dome->nameString());

  boost::optional<WorkspaceObject> idf_diffuser(wo.getTarget(DaylightingDevice_TubularFields::DiffuserName));
  ASSERT_TRUE(idf_diffuser);
  EXPECT_EQ(diffuser.nameString(), idf_diffuser->nameString());

  boost::optional<WorkspaceObject> idf_constr(wo.getTarget(DaylightingDevice_TubularFields::ConstructionName));
  ASSERT_TRUE(idf_constr);
  EXPECT_EQ(construction.nameString(), idf_constr->nameString());

  EXPECT_EQ(1, wo.getDouble(DaylightingDevice_TubularFields::Diameter, false).get());
  EXPECT_EQ(2, wo.getDouble(DaylightingDevice_TubularFields::TotalLength, false).get());
  EXPECT_EQ(3, wo.getDouble(DaylightingDevice_TubularFields::EffectiveThermalResistance, false).get());

  EXPECT_EQ(2u, wo.numExtensibleGroups());
}
