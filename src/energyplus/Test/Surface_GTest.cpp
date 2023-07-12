/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>
#include "EnergyPlusFixture.hpp"

#include "../ForwardTranslator.hpp"
#include "../ReverseTranslator.hpp"

#include "../../model/Model.hpp"
#include "../../model/Surface.hpp"
#include "../../model/Surface_Impl.hpp"
#include "../../model/Space.hpp"
#include "../../model/Space_Impl.hpp"
#include "../../model/ThermalZone.hpp"
#include "../../model/ThermalZone_Impl.hpp"
#include "../../model/SpaceType.hpp"
#include "../../model/SpaceType_Impl.hpp"
#include "../../model/DefaultSurfaceConstructions.hpp"
#include "../../model/DefaultSurfaceConstructions_Impl.hpp"
#include "../../model/DefaultConstructionSet.hpp"
#include "../../model/DefaultConstructionSet_Impl.hpp"
#include "../../model/Construction.hpp"
#include "../../model/Construction_Impl.hpp"
#include "../../model/Building.hpp"
#include "../../model/Building_Impl.hpp"

#include <utilities/idd/BuildingSurface_Detailed_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include <resources.hxx>

#include <sstream>

using namespace openstudio::energyplus;
using namespace openstudio::model;
using namespace openstudio;

TEST_F(EnergyPlusFixture, ForwardTranslator_Surface) {
  Model model;

  Point3dVector points{
    {0, 1, 0},
    {0, 0, 0},
    {1, 0, 0},
    {1, 1, 0},
  };
  Surface surface(points, model);

  ForwardTranslator forwardTranslator;
  Workspace workspace = forwardTranslator.translateModel(model);
  EXPECT_EQ(0u, forwardTranslator.errors().size());
  EXPECT_EQ(1u, forwardTranslator.warnings().size());

  EXPECT_EQ(0u, workspace.getObjectsByType(IddObjectType::BuildingSurface_Detailed).size());
}

TEST_F(EnergyPlusFixture, ForwardTranslator_Surface_Zone) {
  Model model;

  ThermalZone thermalZone(model);

  Space space(model);
  space.setThermalZone(thermalZone);

  Point3dVector points{
    {0, 1, 0},
    {0, 0, 0},
    {1, 0, 0},
    {1, 1, 0},
  };
  Surface surface(points, model);
  surface.setSpace(space);

  ForwardTranslator forwardTranslator;
  Workspace workspace = forwardTranslator.translateModel(model);
  EXPECT_EQ(0u, forwardTranslator.errors().size());
  EXPECT_EQ(0u, forwardTranslator.warnings().size());

  ASSERT_EQ(1u, workspace.getObjectsByType(IddObjectType::BuildingSurface_Detailed).size());
  ASSERT_EQ(1u, workspace.getObjectsByType(IddObjectType::Zone).size());

  WorkspaceObject surfaceObject = workspace.getObjectsByType(IddObjectType::BuildingSurface_Detailed)[0];
  WorkspaceObject zoneObject = workspace.getObjectsByType(IddObjectType::Zone)[0];

  ASSERT_TRUE(surfaceObject.getTarget(BuildingSurface_DetailedFields::ZoneName));
  EXPECT_EQ(zoneObject.handle(), surfaceObject.getTarget(BuildingSurface_DetailedFields::ZoneName)->handle());
}

TEST_F(EnergyPlusFixture, ForwardTranslator_Surface_DefaultConstruction) {
  Model model;

  Construction construction(model);

  DefaultSurfaceConstructions defaultSurfaceConstructions(model);
  defaultSurfaceConstructions.setRoofCeilingConstruction(construction);

  DefaultConstructionSet defaultConstructionSet(model);
  defaultConstructionSet.setDefaultExteriorSurfaceConstructions(defaultSurfaceConstructions);

  Building building = model.getUniqueModelObject<Building>();
  building.setDefaultConstructionSet(defaultConstructionSet);

  Space space(model);
  ThermalZone zone(model);
  EXPECT_TRUE(space.setThermalZone(zone));

  Point3dVector points{
    {0, 1, 0},
    {0, 0, 0},
    {1, 0, 0},
    {1, 1, 0},
  };
  Surface surface(points, model);
  surface.setSpace(space);

  EXPECT_EQ("RoofCeiling", surface.surfaceType());
  EXPECT_EQ("Outdoors", surface.outsideBoundaryCondition());
  EXPECT_FALSE(surface.adjacentSurface());
  ASSERT_TRUE(surface.construction());
  EXPECT_TRUE(surface.isConstructionDefaulted());
  EXPECT_EQ(construction.handle(), surface.construction()->handle());

  ForwardTranslator forwardTranslator;
  Workspace workspace = forwardTranslator.translateModel(model);
  EXPECT_EQ(0u, forwardTranslator.errors().size());
  EXPECT_EQ(0u, forwardTranslator.warnings().size());

  ASSERT_EQ(1u, workspace.getObjectsByType(IddObjectType::BuildingSurface_Detailed).size());
  ASSERT_EQ(1u, workspace.getObjectsByType(IddObjectType::Construction).size());

  WorkspaceObject surfaceObject = workspace.getObjectsByType(IddObjectType::BuildingSurface_Detailed)[0];
  WorkspaceObject constructionObject = workspace.getObjectsByType(IddObjectType::Construction)[0];

  ASSERT_TRUE(surfaceObject.getTarget(BuildingSurface_DetailedFields::ConstructionName));
  EXPECT_EQ(constructionObject.handle(), surfaceObject.getTarget(BuildingSurface_DetailedFields::ConstructionName)->handle());
}
