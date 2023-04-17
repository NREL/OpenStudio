/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2023, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

#include "../../model/SpaceInfiltrationEffectiveLeakageArea.hpp"
#include "../../model/SpaceInfiltrationEffectiveLeakageArea_Impl.hpp"

#include "../../model/Model.hpp"
#include "../../model/Building.hpp"
#include "../../model/Building_Impl.hpp"
#include "../../model/SpaceType.hpp"
#include "../../model/ThermalZone.hpp"
#include "../../model/Space.hpp"

#include "../../utilities/idf/WorkspaceObject.hpp"
#include "../../utilities/idf/IdfExtensibleGroup.hpp"
#include "../../utilities/idf/WorkspaceExtensibleGroup.hpp"

#include "../../utilities/geometry/Geometry.hpp"
#include "../../utilities/geometry/Point3d.hpp"

#include <utilities/idd/ZoneInfiltration_EffectiveLeakageArea_FieldEnums.hxx>
#include <utilities/idd/Zone_FieldEnums.hxx>
#include <utilities/idd/ZoneList_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::energyplus;
using namespace openstudio::model;
using namespace openstudio;

TEST_F(EnergyPlusFixture, ForwardTranslator_SpaceInfiltrationEffectiveLeakageArea) {
  Model model;
  SpaceInfiltrationEffectiveLeakageArea infiltration(model);

  ForwardTranslator forwardTranslator;
  Workspace workspace = forwardTranslator.translateModel(model);
  EXPECT_EQ(0u, forwardTranslator.errors().size());
  EXPECT_EQ(1u, forwardTranslator.warnings().size());

  ASSERT_EQ(0u, workspace.getObjectsByType(IddObjectType::ZoneInfiltration_EffectiveLeakageArea).size());

  ThermalZone zone(model);
  Space space(model);
  space.setThermalZone(zone);
  infiltration.setSpace(space);

  workspace = forwardTranslator.translateModel(model);
  EXPECT_EQ(0u, forwardTranslator.errors().size());
  EXPECT_EQ(0u, forwardTranslator.warnings().size());

  ASSERT_EQ(1u, workspace.getObjectsByType(IddObjectType::ZoneInfiltration_EffectiveLeakageArea).size());
}

TEST_F(EnergyPlusFixture, ForwardTranslator_SpaceInfiltrationEffectiveLeakageArea_SpaceTypes) {

  Model m;

  constexpr double width = 10.0;
  constexpr double height = 3.6;  // It's convenient for ACH, since 3600 s/hr
  constexpr double spaceFloorArea = width * width;
  // constexpr double spaceVolume = spaceFloorArea * height;
  constexpr double oneWallArea = width * height;

  //            y (=North)
  //   ▲
  //   │                  building height = 3m
  // 10├────────┼────────┼────────┼────────┤
  //   │        │        │        │        │
  //   │        │        │        │        │
  //   │ Space 1│ Space 2│ Space 3│ Space 4│
  //   │        │        │        │        │
  //   └────────┴────────┴────────┴────────┴──► x
  //  0        10       20        30       40

  // Counterclockwise points
  std::vector<Point3d> floorPointsSpace1{{0.0, 0.0, 0.0}, {0.0, width, 0.0}, {width, width, 0.0}, {width, 0.0, 0.0}};

  auto space1 = Space::fromFloorPrint(floorPointsSpace1, height, m).get();
  auto space2 = Space::fromFloorPrint(floorPointsSpace1, height, m).get();
  space2.setXOrigin(width);
  auto space3 = Space::fromFloorPrint(floorPointsSpace1, height, m).get();
  space3.setXOrigin(width * 2);
  auto space4 = Space::fromFloorPrint(floorPointsSpace1, height, m).get();
  space4.setXOrigin(width * 3);

  ThermalZone z(m);
  EXPECT_TRUE(space1.setThermalZone(z));
  EXPECT_TRUE(space2.setThermalZone(z));
  EXPECT_TRUE(space3.setThermalZone(z));
  EXPECT_TRUE(space4.setThermalZone(z));

  // This isn't necessary for this object as it only can have absolute values (cm2), but doing it anyways
  space1.matchSurfaces(space2);
  space2.matchSurfaces(space3);
  space3.matchSurfaces(space4);
  double outerSpaceExteriorSurfaceArea = spaceFloorArea + 3 * oneWallArea;  // 1 wall in common with another space (floor is ground)
  EXPECT_EQ(outerSpaceExteriorSurfaceArea, space1.exteriorArea());
  EXPECT_EQ(outerSpaceExteriorSurfaceArea, space4.exteriorArea());
  double innerSpaceExteriorSurfaceArea = spaceFloorArea + 2 * oneWallArea;  // 2 walls in common with another space (floor is ground)
  EXPECT_EQ(innerSpaceExteriorSurfaceArea, space2.exteriorArea());
  EXPECT_EQ(innerSpaceExteriorSurfaceArea, space3.exteriorArea());

  // Assign a default space type at building-level, with its own infiltration object
  SpaceType buildingSpaceType(m);
  buildingSpaceType.setName("buildingSpaceType");

  SpaceInfiltrationEffectiveLeakageArea infilBuilding(m);
  infilBuilding.setName("infiltrationBuilding");
  EXPECT_TRUE(infilBuilding.setSpaceType(buildingSpaceType));

  auto building = m.getUniqueModelObject<Building>();
  EXPECT_TRUE(building.setSpaceType(buildingSpaceType));

  // Create an Office Space Type. Assign to Space 1 & 2 only (not 3 nor 4), it also has an infiltration object
  SpaceType officeSpaceType(m);
  officeSpaceType.setName("officeSpaceType");
  SpaceInfiltrationEffectiveLeakageArea infilOffice(m);
  infilOffice.setName("infilOffice");
  EXPECT_TRUE(infilOffice.setSpaceType(officeSpaceType));
  EXPECT_TRUE(space1.setSpaceType(officeSpaceType));
  EXPECT_TRUE(space2.setSpaceType(officeSpaceType));

  // Space 1 and 3 have a space-level infiltration
  SpaceInfiltrationEffectiveLeakageArea infilSpace1(m);
  infilSpace1.setName("infilSpace1");
  EXPECT_TRUE(infilSpace1.setSpace(space1));

  SpaceInfiltrationEffectiveLeakageArea infilSpace3(m);
  infilSpace3.setName("infilSpace3");
  EXPECT_TRUE(infilSpace3.setSpace(space3));

  // Infiltration Characteristics: This object is absolute, so it's a lot more simple than SpaceInfiltrationDesignFlowRate for eg
  // =============================
  //
  // |  Infiltration | Leakage Area |
  // |     Name      |   (cm2 )     |
  // |---------------|--------------|
  // | infilSpace1   | 0.1          |
  // | infilSpace3   | 0.2          |
  // | infilOffice   | 0.3          |
  // | infilBuilding | 0.4          |

  // Which infiltation object(s) apply to each space
  // ================================================
  //
  // | Space    | Space Specific Infiltration | Space specific cm2  ║ SpaceType                     | Additional Inherited Infiltration | cm2  |
  // |----------|-----------------------------|---------------------║-------------------------------|-----------------------------------|------|
  // | Space 1  | infilSpace1                 | 0.1                 ║ officeSpaceType               | infilOffice                       | 0.3  |
  // | Space 2  |                             |                     ║ officeSpaceType               | infilOffice                       | 0.3  |
  // | Space 3  | infilSpace3                 | 0.2                 ║ Inherited: buildingSpaceType  | infilBuilding                     | 0.4  |
  // | Space 4  |                             |                     ║ Inherited: buildingSpaceType  | infilBuilding                     | 0.4  |
  // |-----------------------------------------------------------------------------------------------------------------------------------------|
  // | Subtotal |                               0.3                 ║                                                                     1.4  |
  // |=========================================================================================================================================|
  // | Total    |                                                  1.7                                                                         |
  // |=========================================================================================================================================|

  EXPECT_TRUE(infilSpace1.setEffectiveAirLeakageArea(0.1));
  EXPECT_TRUE(infilSpace3.setEffectiveAirLeakageArea(0.2));
  EXPECT_TRUE(infilOffice.setEffectiveAirLeakageArea(0.3));
  EXPECT_TRUE(infilBuilding.setEffectiveAirLeakageArea(0.4));

  ForwardTranslator ft;

  // When excluding space translation (historical behavior)
  {
    ft.setExcludeSpaceTranslation(true);

    Workspace w = ft.translateModel(m);

    auto zones = w.getObjectsByType(IddObjectType::Zone);
    ASSERT_EQ(1, zones.size());
    auto zone = zones[0];
    EXPECT_EQ(0, w.getObjectsByType(IddObjectType::ZoneList).size());
    EXPECT_EQ(0, w.getObjectsByType(IddObjectType::Space).size());
    EXPECT_EQ(0, w.getObjectsByType(IddObjectType::SpaceList).size());

    auto infils = w.getObjectsByType(IddObjectType::ZoneInfiltration_EffectiveLeakageArea);
    // I expect infilSpace1, infilSpace3, two infilOffice and two infilBuilding, so 6 total
    ASSERT_EQ(6, infils.size());

    double totalInfiltration = 0.0;  // cm2
    for (const auto& infil : infils) {
      auto name = infil.nameString();
      auto z_ = infil.getTarget(ZoneInfiltration_EffectiveLeakageAreaFields::ZoneorSpaceName);
      ASSERT_TRUE(z_);
      EXPECT_EQ(zone, z_.get());
      double i = infil.getDouble(ZoneInfiltration_EffectiveLeakageAreaFields::EffectiveAirLeakageArea).get();
      totalInfiltration += i;
      if (name.find(infilSpace1.nameString()) != std::string::npos) {
        EXPECT_EQ(0.1, i);
        EXPECT_EQ(infilSpace1.effectiveAirLeakageArea(), i);
      } else if (name.find(infilSpace3.nameString()) != std::string::npos) {
        EXPECT_DOUBLE_EQ(0.2, i);
        EXPECT_DOUBLE_EQ(infilSpace3.effectiveAirLeakageArea(), i);
      } else if (name.find(infilOffice.nameString()) != std::string::npos) {
        EXPECT_EQ(0.3, i);
        EXPECT_EQ(infilOffice.effectiveAirLeakageArea(), i);
      } else if (name.find(infilBuilding.nameString()) != std::string::npos) {
        EXPECT_EQ(0.4, i);
        EXPECT_EQ(infilBuilding.effectiveAirLeakageArea(), i);
      }
    }

    EXPECT_DOUBLE_EQ(1.7, totalInfiltration);
  }

  // When including Space translation (new E+ 9.6.0)
  // same thing as before: we place them for the Zone itself
  {
    ft.setExcludeSpaceTranslation(false);

    Workspace w = ft.translateModel(m);

    auto zones = w.getObjectsByType(IddObjectType::Zone);
    ASSERT_EQ(1, zones.size());
    auto zone = zones[0];
    EXPECT_EQ(0, w.getObjectsByType(IddObjectType::ZoneList).size());
    EXPECT_EQ(4, w.getObjectsByType(IddObjectType::Space).size());
    EXPECT_EQ(2, w.getObjectsByType(IddObjectType::SpaceList).size());

    auto infils = w.getObjectsByType(IddObjectType::ZoneInfiltration_EffectiveLeakageArea);
    // I expect infilSpace1, infilSpace3, two infilOffice and two infilBuilding, so 6 total. All pointing to a Space
    ASSERT_EQ(6, infils.size());

    bool foundOfficeSpace1 = false;
    bool foundOfficeSpace2 = false;
    bool foundBuildingSpace3 = false;
    bool foundBuildingSpace4 = false;

    double totalInfiltration = 0.0;  // cm2
    for (const auto& infil : infils) {
      auto name = infil.nameString();
      auto target_ = infil.getTarget(ZoneInfiltration_EffectiveLeakageAreaFields::ZoneorSpaceName);
      ASSERT_TRUE(target_);
      double i = infil.getDouble(ZoneInfiltration_EffectiveLeakageAreaFields::EffectiveAirLeakageArea).get();
      totalInfiltration += i;
      if (name.find(infilSpace1.nameString()) != std::string::npos) {
        EXPECT_EQ(target_->iddObject().type(), IddObjectType::Space);
        EXPECT_EQ(space1.nameString(), target_->nameString());
        EXPECT_EQ(0.1, i);
        EXPECT_EQ(infilSpace1.effectiveAirLeakageArea(), i);
      } else if (name.find(infilSpace3.nameString()) != std::string::npos) {
        EXPECT_EQ(target_->iddObject().type(), IddObjectType::Space);
        EXPECT_EQ(space3.nameString(), target_->nameString());
        EXPECT_DOUBLE_EQ(0.2, i);
        EXPECT_DOUBLE_EQ(infilSpace3.effectiveAirLeakageArea(), i);
      } else if (name.find(infilOffice.nameString()) != std::string::npos) {
        EXPECT_EQ(target_->iddObject().type(), IddObjectType::Space);
        if (space1.nameString() == target_->nameString()) {
          foundOfficeSpace1 = true;
        } else if (space2.nameString() == target_->nameString()) {
          foundOfficeSpace2 = true;
        }
        EXPECT_EQ(0.3, i);
        EXPECT_EQ(infilOffice.effectiveAirLeakageArea(), i);
      } else if (name.find(infilBuilding.nameString()) != std::string::npos) {
        EXPECT_EQ(target_->iddObject().type(), IddObjectType::Space);
        if (space3.nameString() == target_->nameString()) {
          foundBuildingSpace3 = true;
        } else if (space4.nameString() == target_->nameString()) {
          foundBuildingSpace4 = true;
        }
        EXPECT_EQ(0.4, i);
        EXPECT_EQ(infilBuilding.effectiveAirLeakageArea(), i);
      }
    }

    EXPECT_TRUE(foundOfficeSpace1);
    EXPECT_TRUE(foundOfficeSpace2);
    EXPECT_TRUE(foundBuildingSpace3);
    EXPECT_TRUE(foundBuildingSpace4);

    EXPECT_DOUBLE_EQ(1.7, totalInfiltration);
  }
}
