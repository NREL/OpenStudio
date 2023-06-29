/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>
#include "EnergyPlusFixture.hpp"

#include "../ForwardTranslator.hpp"

#include "../../model/Model.hpp"
#include "../../model/ThermalZone.hpp"
#include "../../model/Space.hpp"
#include "../../model/Surface.hpp"
#include "../../model/SubSurface.hpp"
#include "../../model/DaylightingDeviceLightWell.hpp"
#include "../../model/DaylightingDeviceLightWell_Impl.hpp"

#include "../../utilities/geometry/Point3d.hpp"
#include <utilities/idd/DaylightingDevice_LightWell_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::energyplus;
using namespace openstudio::model;
using namespace openstudio;

TEST_F(EnergyPlusFixture, ForwardTranslator_DaylightingDeviceLightWell) {
  Model model;
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
  SubSurface window(points2, model);
  window.setSubSurfaceType("FixedWindow");
  window.setSurface(surface);

  EXPECT_FALSE(window.daylightingDeviceLightWell());
  DaylightingDeviceLightWell lightWell(window, 1, 2, 3, 0.5);
  EXPECT_TRUE(window.daylightingDeviceLightWell());
  EXPECT_EQ(1u, model.getConcreteModelObjects<DaylightingDeviceLightWell>().size());

  ForwardTranslator ft;
  Workspace w = ft.translateModel(model);

  std::vector<WorkspaceObject> wos = w.getObjectsByType(IddObjectType::DaylightingDevice_LightWell);
  ASSERT_EQ(1u, wos.size());
  WorkspaceObject wo(wos[0]);

  boost::optional<WorkspaceObject> idf_window(wo.getTarget(DaylightingDevice_LightWellFields::ExteriorWindowName));
  ASSERT_TRUE(idf_window);
  EXPECT_EQ(window.nameString(), idf_window->nameString());

  EXPECT_EQ(1, wo.getDouble(DaylightingDevice_LightWellFields::HeightofWell, false).get());
  EXPECT_EQ(2, wo.getDouble(DaylightingDevice_LightWellFields::PerimeterofBottomofWell, false).get());
  EXPECT_EQ(3, wo.getDouble(DaylightingDevice_LightWellFields::AreaofBottomofWell, false).get());
  EXPECT_EQ(0.5, wo.getDouble(DaylightingDevice_LightWellFields::VisibleReflectanceofWellWalls, false).get());
}
