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
#include "../../model/Construction.hpp"
#include "../../model/ThermalZone.hpp"
#include "../../model/Space.hpp"
#include "../../model/Surface.hpp"
#include "../../model/Surface_Impl.hpp"
#include "../../model/SubSurface.hpp"
#include "../../model/ShadingSurface.hpp"
#include "../../model/ShadingSurface_Impl.hpp"
#include "../../model/ShadingSurfaceGroup.hpp"
#include "../../model/InteriorPartitionSurface.hpp"
#include "../../model/InteriorPartitionSurface_Impl.hpp"
#include "../../model/InteriorPartitionSurfaceGroup.hpp"
#include "../../model/InternalMass.hpp"
#include "../../model/InternalMass_Impl.hpp"
#include "../../model/DaylightingDeviceShelf.hpp"
#include "../../model/DaylightingDeviceShelf_Impl.hpp"

#include "../../utilities/geometry/Point3d.hpp"
#include <utilities/idd/DaylightingDevice_Shelf_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::energyplus;
using namespace openstudio::model;
using namespace openstudio;

TEST_F(EnergyPlusFixture,ForwardTranslator_DaylightingDeviceShelf)
{
  Model model;
  Construction construction(model);
  ThermalZone zone(model);
  Space space(model);
  space.setThermalZone(zone);

  Point3dVector points;
  points.push_back(Point3d(0, 10, 3));
  points.push_back(Point3d(0, 10, 0));
  points.push_back(Point3d(0, 0, 0));
  points.push_back(Point3d(0, 0, 3));
  Surface surface(points, model);
  surface.setSpace(space);
  EXPECT_EQ("Wall", surface.surfaceType());

  double viewGlassToWallRatio = 0.3; 
  double daylightingGlassToWallRatio = 0.2;
  double desiredViewGlassSillHeight = 1.0;
  double desiredDaylightingGlassHeaderHeight = 1.0;
  double exteriorShadingProjectionFactor = 0.5;
  double interiorShelfProjectionFactor = 0.5;

  std::vector<SubSurface> result = surface.applyViewAndDaylightingGlassRatios(viewGlassToWallRatio, daylightingGlassToWallRatio, 
                                                                              desiredViewGlassSillHeight, desiredDaylightingGlassHeaderHeight,
                                                                              exteriorShadingProjectionFactor, interiorShelfProjectionFactor, 
                                                                              construction, construction);
  ASSERT_EQ(2u, result.size());
  EXPECT_EQ(1u, result[0].shadingSurfaceGroups().size());
  EXPECT_FALSE(result[0].daylightingDeviceShelf());
  EXPECT_EQ(0, result[1].shadingSurfaceGroups().size());
  ASSERT_TRUE(result[1].daylightingDeviceShelf());
  EXPECT_TRUE(result[1].daylightingDeviceShelf()->insideShelf());
  EXPECT_FALSE(result[1].daylightingDeviceShelf()->outsideShelf());

  EXPECT_EQ(1u, model.getModelObjects<Surface>().size());
  EXPECT_EQ(1u, model.getModelObjects<DaylightingDeviceShelf>().size());
  EXPECT_EQ(1u, model.getModelObjects<InteriorPartitionSurface>().size());
  EXPECT_EQ(1u, model.getModelObjects<ShadingSurface>().size());
  EXPECT_EQ(0, model.getModelObjects<InternalMass>().size());

  {
    ForwardTranslator ft;
    Workspace workspace = ft.translateModel(model);
    std::vector<WorkspaceObject> wos = workspace.getObjectsByType(IddObjectType::DaylightingDevice_Shelf);
    ASSERT_EQ(1u, wos.size());
    EXPECT_TRUE(wos[0].getTarget(DaylightingDevice_ShelfFields::WindowName));
    EXPECT_TRUE(wos[0].getTarget(DaylightingDevice_ShelfFields::InsideShelfName));
    EXPECT_FALSE(wos[0].getTarget(DaylightingDevice_ShelfFields::OutsideShelfName));

    // interior partition surface becomes building surface detailed
    wos = workspace.getObjectsByType(IddObjectType::BuildingSurface_Detailed);
    EXPECT_EQ(2u, wos.size());

    wos = workspace.getObjectsByType(IddObjectType::Shading_Zone_Detailed);
    EXPECT_EQ(1u, wos.size());

    wos = workspace.getObjectsByType(IddObjectType::InternalMass);
    EXPECT_EQ(0, wos.size());
  }

  // remove the shelf, does not remove shelf surfaces
  result[1].daylightingDeviceShelf()->remove();

  EXPECT_EQ(1u, model.getModelObjects<Surface>().size());
  EXPECT_EQ(0, model.getModelObjects<DaylightingDeviceShelf>().size());
  EXPECT_EQ(1u, model.getModelObjects<InteriorPartitionSurface>().size());
  EXPECT_EQ(1u, model.getModelObjects<ShadingSurface>().size());
  EXPECT_EQ(0, model.getModelObjects<InternalMass>().size());

  {
    ForwardTranslator ft;
    Workspace workspace = ft.translateModel(model);
    std::vector<WorkspaceObject> wos = workspace.getObjectsByType(IddObjectType::DaylightingDevice_Shelf);
    EXPECT_EQ(0, wos.size());

    wos = workspace.getObjectsByType(IddObjectType::BuildingSurface_Detailed);
    EXPECT_EQ(1u, wos.size());

    wos = workspace.getObjectsByType(IddObjectType::Shading_Zone_Detailed);
    EXPECT_EQ(1u, wos.size());

    // DLM: this will still be zero because the light shelf has no construction attached
    wos = workspace.getObjectsByType(IddObjectType::InternalMass);
    EXPECT_EQ(0, wos.size());
  }
}
