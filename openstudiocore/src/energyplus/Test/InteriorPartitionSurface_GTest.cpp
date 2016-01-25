/**********************************************************************
*  Copyright (c) 2008-2016, Alliance for Sustainable Energy.
*  All rights reserved.
*
*  This library is free software; you can redistribute it and/or
*  modify it under the terms of the GNU Lesser General Public
*  License as published by the Free Software Foundation; either
*  version 2.1 of the License, or (at your option) any later version.
*
*  This library is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
*  Lesser General Public License for more details.
*
*  You should have received a copy of the GNU Lesser General Public
*  License along with this library; if not, write to the Free Software
*  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
**********************************************************************/

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
#include <utilities/idd/IddEnums.hxx>

#include <resources.hxx>

#include <sstream>

using namespace openstudio::energyplus;
using namespace openstudio::model;
using namespace openstudio;

TEST_F(EnergyPlusFixture,ForwardTranslator_InteriorPartitionSurface)
{
  Model model;
  Point3dVector points;
  points.push_back(Point3d(0,1,0));
  points.push_back(Point3d(1,1,0));
  points.push_back(Point3d(1,0,0));
  points.push_back(Point3d(0,0,0));

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

  EXPECT_EQ(12u, model.getModelObjects<Surface>().size());

  unsigned n = 0;
  for (Surface surface : space1->surfaces()){
    EXPECT_EQ(1.0, surface.grossArea());
    if (surface.adjacentSurface()){
      ++n;
    }
  }
  EXPECT_EQ(1u, n);

  ForwardTranslator forwardTranslator;
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
  ASSERT_TRUE(internalMassObject.getTarget(InternalMassFields::ZoneName));
  EXPECT_EQ(zoneObject.handle(), internalMassObject.getTarget(InternalMassFields::ZoneName)->handle());
}


