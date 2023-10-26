/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>
#include "EnergyPlusFixture.hpp"

#include "../ForwardTranslator.hpp"
#include "../ReverseTranslator.hpp"

#include "../../model/Model.hpp"
#include "../../model/ShadingSurface.hpp"
#include "../../model/ShadingSurface_Impl.hpp"
#include "../../model/ShadingSurfaceGroup.hpp"
#include "../../model/ShadingSurfaceGroup_Impl.hpp"
#include "../../model/Space.hpp"
#include "../../model/Space_Impl.hpp"
#include "../../model/ThermalZone.hpp"
#include "../../model/ThermalZone_Impl.hpp"

#include <utilities/idd/Shading_Site_Detailed_FieldEnums.hxx>
#include <utilities/idd/Shading_Building_Detailed_FieldEnums.hxx>
#include <utilities/idd/Shading_Zone_Detailed_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include <resources.hxx>

#include <sstream>

using namespace openstudio::energyplus;
using namespace openstudio::model;
using namespace openstudio;

TEST_F(EnergyPlusFixture, ForwardTranslator_ShadingSurface_Site) {
  Model model;
  ShadingSurfaceGroup shadingSurfaceGroup(model);
  EXPECT_TRUE(shadingSurfaceGroup.setShadingSurfaceType("Site"));

  Point3dVector points{
    {0, 1, 0},
    {1, 1, 0},
    {1, 0, 0},
    {0, 0, 0},
  };

  ShadingSurface shadingSurface(points, model);
  EXPECT_TRUE(shadingSurface.setShadingSurfaceGroup(shadingSurfaceGroup));

  ForwardTranslator forwardTranslator;
  Workspace workspace = forwardTranslator.translateModel(model);

  EXPECT_EQ(1u, workspace.getObjectsByType(IddObjectType::Shading_Site_Detailed).size());
}

TEST_F(EnergyPlusFixture, ForwardTranslator_ShadingSurface_Building) {
  Model model;
  ShadingSurfaceGroup shadingSurfaceGroup(model);
  EXPECT_TRUE(shadingSurfaceGroup.setShadingSurfaceType("Building"));

  Point3dVector points{
    {0, 1, 0},
    {1, 1, 0},
    {1, 0, 0},
    {0, 0, 0},
  };

  ShadingSurface shadingSurface(points, model);
  EXPECT_TRUE(shadingSurface.setShadingSurfaceGroup(shadingSurfaceGroup));

  ForwardTranslator forwardTranslator;
  Workspace workspace = forwardTranslator.translateModel(model);

  EXPECT_EQ(1u, workspace.getObjectsByType(IddObjectType::Shading_Building_Detailed).size());
}

TEST_F(EnergyPlusFixture, ForwardTranslator_ShadingSurface_Space) {
  Model model;

  ThermalZone thermalZone(model);

  Point3dVector points{
    {0, 1, 0},
    {1, 1, 0},
    {1, 0, 0},
    {0, 0, 0},
  };

  boost::optional<Space> space = Space::fromFloorPrint(points, 1, model);
  ASSERT_TRUE(space);
  space->setThermalZone(thermalZone);

  ShadingSurfaceGroup shadingSurfaceGroup(model);
  EXPECT_TRUE(shadingSurfaceGroup.setSpace(*space));

  ShadingSurface shadingSurface(points, model);
  EXPECT_TRUE(shadingSurface.setShadingSurfaceGroup(shadingSurfaceGroup));

  ForwardTranslator forwardTranslator;
  Workspace workspace = forwardTranslator.translateModel(model);

  ASSERT_EQ(1u, workspace.getObjectsByType(IddObjectType::Zone).size());
  ASSERT_EQ(1u, workspace.getObjectsByType(IddObjectType::Shading_Zone_Detailed).size());

  WorkspaceObject zoneObject = workspace.getObjectsByType(IddObjectType::Zone)[0];
  WorkspaceObject shadingSurfaceObject = workspace.getObjectsByType(IddObjectType::Shading_Zone_Detailed)[0];

  EXPECT_TRUE(shadingSurfaceObject.getTarget(Shading_Zone_DetailedFields::BaseSurfaceName));
}
