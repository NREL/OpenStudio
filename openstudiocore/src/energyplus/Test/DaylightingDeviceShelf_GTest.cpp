/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2019, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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
