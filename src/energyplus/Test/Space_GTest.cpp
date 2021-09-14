/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2021, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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
#include "../../model/Space.hpp"
#include "../../model/Space_Impl.hpp"
#include "../../model/LifeCycleCost.hpp"
#include "../../model/ThermalZone.hpp"
#include "../../model/ThermalZone_Impl.hpp"
#include "../../model/Surface.hpp"
#include "../../model/Surface_Impl.hpp"
#include "../../model/SpaceType.hpp"
#include "../../model/SpaceType_Impl.hpp"

#include <utilities/idd/Space_FieldEnums.hxx>
#include <utilities/idd/Zone_FieldEnums.hxx>

#include <utilities/idd/IddEnums.hxx>

#include <resources.hxx>

#include <sstream>

using namespace openstudio::energyplus;
using namespace openstudio::model;
using namespace openstudio;

TEST_F(EnergyPlusFixture, ForwardTranslator_Space) {
  Model model;

  Space space(model);

  ForwardTranslator ft;

  // space not translated when not in thermal zone
  Workspace workspace = ft.translateModel(model);
  EXPECT_EQ(0u, workspace.getObjectsByType(IddObjectType::Zone).size());
  EXPECT_EQ(0u, workspace.getObjectsByType(IddObjectType::ComponentCost_LineItem).size());

  // space not translated when not in thermal zone
  workspace = ft.translateModel(model);
  EXPECT_EQ(0u, workspace.getObjectsByType(IddObjectType::Zone).size());
  EXPECT_EQ(0u, workspace.getObjectsByType(IddObjectType::ComponentCost_LineItem).size());

  ThermalZone thermalZone(model);
  space.setThermalZone(thermalZone);

  workspace = ft.translateModel(model);
  EXPECT_EQ(1u, workspace.getObjectsByType(IddObjectType::Zone).size());

  // TODO: add another space to the thermal zone
  Space space2(model);
  space2.setThermalZone(thermalZone);

  workspace = ft.translateModel(model);
  EXPECT_EQ(1u, workspace.getObjectsByType(IddObjectType::Zone).size());
}

TEST_F(EnergyPlusFixture, ForwardTranslator_Space2) {
  Model model;

  Point3dVector points1;
  points1.push_back(Point3d(0, 1, 0));
  points1.push_back(Point3d(1, 1, 0));
  points1.push_back(Point3d(1, 0, 0));
  points1.push_back(Point3d(0, 0, 0));
  Surface floor1(points1, model);

  Point3dVector points2;
  points2.push_back(Point3d(0, 2, 0));
  points2.push_back(Point3d(2, 2, 0));
  points2.push_back(Point3d(2, 0, 0));
  points2.push_back(Point3d(0, 0, 0));
  Surface floor2(points2, model);

  Space space1(model);
  Space space2(model);
  floor1.setParent(space1);
  floor2.setParent(space2);

  EXPECT_NEAR(1, space1.floorArea(), 0.0001);
  EXPECT_NEAR(4, space2.floorArea(), 0.0001);
  EXPECT_TRUE(space1.partofTotalFloorArea());
  EXPECT_TRUE(space2.partofTotalFloorArea());

  SpaceType spaceType1(model);
  SpaceType spaceType2(model);
  space1.setSpaceType(spaceType1);
  space2.setSpaceType(spaceType2);

  ThermalZone thermalZone(model);
  space1.setThermalZone(thermalZone);
  space2.setThermalZone(thermalZone);

  ForwardTranslator ft;
  ft.setExcludeSpaceTranslation(false);
  Workspace workspace = ft.translateModel(model);

  WorkspaceObjectVector idf_spaces(workspace.getObjectsByType(IddObjectType::Space));
  EXPECT_EQ(2u, idf_spaces.size());
  WorkspaceObject idf_space1(idf_spaces[0]);
  WorkspaceObject idf_space2(idf_spaces[1]);

  WorkspaceObjectVector idf_zones(workspace.getObjectsByType(IddObjectType::Zone));
  EXPECT_EQ(1u, idf_zones.size());
  WorkspaceObject idf_zone(idf_zones[0]);

  boost::optional<WorkspaceObject> idf_zone1(idf_space1.getTarget(SpaceFields::ZoneName));
  ASSERT_TRUE(idf_zone1);
  EXPECT_EQ(idf_zone1->iddObject().type(), IddObjectType::Zone);
  EXPECT_EQ(thermalZone.nameString(), idf_zone1->name().get());
  EXPECT_EQ(idf_zone1, idf_zone);

  boost::optional<WorkspaceObject> idf_zone2(idf_space2.getTarget(SpaceFields::ZoneName));
  ASSERT_TRUE(idf_zone2);
  EXPECT_EQ(idf_zone2->iddObject().type(), IddObjectType::Zone);
  EXPECT_EQ(thermalZone.nameString(), idf_zone2->name().get());
  EXPECT_EQ(idf_zone2, idf_zone);

  double space1_floorArea = idf_space1.getDouble(SpaceFields::FloorArea, false).get();
  double space2_floorArea = idf_space2.getDouble(SpaceFields::FloorArea, false).get();
  EXPECT_NEAR(space1.floorArea(), space1_floorArea, 0.0001);
  EXPECT_NEAR(space2.floorArea(), space2_floorArea, 0.0001);
  EXPECT_NE("", idf_space1.getString(SpaceFields::SpaceType).get());
  EXPECT_NE("", idf_space2.getString(SpaceFields::SpaceType).get());

  EXPECT_FALSE(idf_zone.getDouble(ZoneFields::FloorArea, false));

  // Check floor area
  EXPECT_TRUE(space1.setPartofTotalFloorArea(false));

  workspace = ft.translateModel(model);

  idf_zones = workspace.getObjectsByType(IddObjectType::Zone);
  EXPECT_EQ(1u, idf_zones.size());
  idf_zone = idf_zones[0];

  EXPECT_NEAR(space2_floorArea, idf_zone.getDouble(ZoneFields::FloorArea, false).get(), 0.0001);

  EXPECT_TRUE(space2.setPartofTotalFloorArea(false));

  workspace = ft.translateModel(model);

  idf_zones = workspace.getObjectsByType(IddObjectType::Zone);
  EXPECT_EQ(1u, idf_zones.size());
  idf_zone = idf_zones[0];

  EXPECT_FALSE(idf_zone.getDouble(ZoneFields::FloorArea, false));
}
