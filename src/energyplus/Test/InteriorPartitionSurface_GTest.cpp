/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>
#include "EnergyPlusFixture.hpp"

#include "../ForwardTranslator.hpp"
#include "../ReverseTranslator.hpp"

#include "../../model/Model.hpp"
#include "../../model/InteriorPartitionSurface.hpp"
#include "../../model/InteriorPartitionSurface_Impl.hpp"
#include "../../model/InteriorPartitionSurfaceGroup.hpp"
#include "../../model/InteriorPartitionSurfaceGroup_Impl.hpp"
#include "../../model/Construction.hpp"
#include "../../model/Construction_Impl.hpp"
#include "../../model/DefaultConstructionSet.hpp"
#include "../../model/DefaultConstructionSet_Impl.hpp"
#include "../../model/DefaultSurfaceConstructions.hpp"
#include "../../model/DefaultSurfaceConstructions_Impl.hpp"
#include "../../model/Surface.hpp"
#include "../../model/Surface_Impl.hpp"
#include "../../model/Space.hpp"
#include "../../model/Space_Impl.hpp"
#include "../../model/ThermalZone.hpp"
#include "../../model/ThermalZone_Impl.hpp"
#include "../../model/StandardOpaqueMaterial.hpp"
#include "../../model/StandardOpaqueMaterial_Impl.hpp"

#include <utilities/idd/InternalMass_FieldEnums.hxx>
#include <utilities/idd/BuildingSurface_Detailed_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include <resources.hxx>

#include <sstream>

using namespace openstudio::energyplus;
using namespace openstudio::model;
using namespace openstudio;

TEST_F(EnergyPlusFixture, ForwardTranslator_InteriorPartitionSurface) {
  Model model;
  Point3dVector points{
    {0, 1, 0},
    {1, 1, 0},
    {1, 0, 0},
    {0, 0, 0},
  };

  boost::optional<Space> space1 = Space::fromFloorPrint(points, 1, model);
  boost::optional<Space> space2 = Space::fromFloorPrint(points, 1, model);

  ASSERT_TRUE(space1);
  ASSERT_TRUE(space2);
  space2->setXOrigin(1);

  StandardOpaqueMaterial material(model);

  std::vector<Material> layers;
  layers.push_back(material);

  Construction construction(model);
  EXPECT_TRUE(construction.setLayers(layers));

  // interior walls will be converted to internal mass
  DefaultSurfaceConstructions defaultSurfaceConstructions(model);
  defaultSurfaceConstructions.setWallConstruction(construction);

  DefaultConstructionSet defaultConstructionSet(model);
  defaultConstructionSet.setDefaultInteriorSurfaceConstructions(defaultSurfaceConstructions);

  space1->setDefaultConstructionSet(defaultConstructionSet);
  space2->setDefaultConstructionSet(defaultConstructionSet);

  ThermalZone zone(model);
  space1->setThermalZone(zone);
  space2->setThermalZone(zone);
  space1->matchSurfaces(*space2);

  EXPECT_EQ(12u, model.getConcreteModelObjects<Surface>().size());

  unsigned n = 0;
  for (const Surface& surface : space1->surfaces()) {
    EXPECT_EQ(1.0, surface.grossArea());
    if (surface.adjacentSurface()) {
      ++n;
    }
  }
  EXPECT_EQ(1u, n);

  ForwardTranslator forwardTranslator;

  // When excluding space translation (historical behavior)
  {
    forwardTranslator.setExcludeSpaceTranslation(true);

    // In this case, the surface that is between the two Spaces will be converted to an InternalMass object
    Workspace workspace = forwardTranslator.translateModel(model);

    ASSERT_EQ(1u, workspace.getObjectsByType(IddObjectType::Zone).size());
    EXPECT_EQ(10u, workspace.getObjectsByType(IddObjectType::BuildingSurface_Detailed).size());
    ASSERT_EQ(1u, workspace.getObjectsByType(IddObjectType::InternalMass).size());
    ASSERT_EQ(1u, workspace.getObjectsByType(IddObjectType::Construction).size());

    WorkspaceObject internalMassObject = workspace.getObjectsByType(IddObjectType::InternalMass)[0];
    ASSERT_TRUE(internalMassObject.getDouble(InternalMassFields::SurfaceArea));
    EXPECT_EQ(1.0, internalMassObject.getDouble(InternalMassFields::SurfaceArea).get());

    WorkspaceObject constructionObject = workspace.getObjectsByType(IddObjectType::Construction)[0];
    ASSERT_TRUE(internalMassObject.getTarget(InternalMassFields::ConstructionName));
    EXPECT_EQ(constructionObject.handle(), internalMassObject.getTarget(InternalMassFields::ConstructionName)->handle());

    WorkspaceObject zoneObject = workspace.getObjectsByType(IddObjectType::Zone)[0];
    ASSERT_TRUE(internalMassObject.getTarget(InternalMassFields::ZoneorZoneListName));
    EXPECT_EQ(zoneObject.handle(), internalMassObject.getTarget(InternalMassFields::ZoneorZoneListName)->handle());
  }

  // When including Space translation (new E+ 9.6.0)
  {
    forwardTranslator.setExcludeSpaceTranslation(false);

    // In this case, the surface between the two spaces does exist. Each space is its own enclosure
    Workspace workspace = forwardTranslator.translateModel(model);

    ASSERT_EQ(1u, workspace.getObjectsByType(IddObjectType::Zone).size());
    EXPECT_EQ(12u, workspace.getObjectsByType(IddObjectType::BuildingSurface_Detailed).size());
    ASSERT_EQ(0u, workspace.getObjectsByType(IddObjectType::InternalMass).size());
    ASSERT_EQ(1u, workspace.getObjectsByType(IddObjectType::Construction).size());
  }
}
