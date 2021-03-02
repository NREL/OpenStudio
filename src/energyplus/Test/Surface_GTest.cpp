/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2020, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

  Point3dVector points;
  points.push_back(Point3d(0, 1, 0));
  points.push_back(Point3d(0, 0, 0));
  points.push_back(Point3d(1, 0, 0));
  points.push_back(Point3d(1, 1, 0));
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

  Point3dVector points;
  points.push_back(Point3d(0, 1, 0));
  points.push_back(Point3d(0, 0, 0));
  points.push_back(Point3d(1, 0, 0));
  points.push_back(Point3d(1, 1, 0));
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

  Point3dVector points;
  points.push_back(Point3d(0, 1, 0));
  points.push_back(Point3d(0, 0, 0));
  points.push_back(Point3d(1, 0, 0));
  points.push_back(Point3d(1, 1, 0));
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
