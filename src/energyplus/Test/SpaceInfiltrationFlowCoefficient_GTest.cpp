/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2022, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

#include "../../model/SpaceInfiltrationFlowCoefficient.hpp"
#include "../../model/SpaceInfiltrationFlowCoefficient_Impl.hpp"

#include "../../model/Model.hpp"
#include "../../model/Building.hpp"
#include "../../model/Building_Impl.hpp"
#include "../../model/SpaceType.hpp"
#include "../../model/ThermalZone.hpp"
#include "../../model/Space.hpp"
#include "../../model/ScheduleConstant.hpp"
#include "../../model/Schedule.hpp"

#include "../../utilities/idf/IdfObject.hpp"
#include "../../utilities/idf/IdfObject_Impl.hpp"
#include "../../utilities/idf/WorkspaceObject.hpp"
#include "../../utilities/idf/WorkspaceObject_Impl.hpp"
#include "../../utilities/idf/IdfExtensibleGroup.hpp"
#include "../../utilities/idf/WorkspaceExtensibleGroup.hpp"

#include "../../utilities/geometry/Geometry.hpp"
#include "../../utilities/geometry/Point3d.hpp"

#include <utilities/idd/ZoneInfiltration_FlowCoefficient_FieldEnums.hxx>
#include <utilities/idd/Zone_FieldEnums.hxx>
#include <utilities/idd/ZoneList_FieldEnums.hxx>
#include <utilities/idd/Schedule_Constant_FieldEnums.hxx>
#include <utilities/idd/GlobalGeometryRules_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::energyplus;
using namespace openstudio::model;
using namespace openstudio;

TEST_F(EnergyPlusFixture, ForwardTranslator_SpaceInfiltrationFlowCoefficient) {
  Model model;
  SpaceInfiltrationFlowCoefficient infiltration(model);
  infiltration.setName("My Infiltration");

  infiltration.resetSchedule();
  EXPECT_FALSE(infiltration.schedule());
  EXPECT_TRUE(infiltration.setFlowCoefficient(0.1));
  EXPECT_TRUE(infiltration.setStackCoefficient(0.2));
  EXPECT_TRUE(infiltration.setPressureExponent(0.3));
  EXPECT_TRUE(infiltration.setWindCoefficient(0.4));
  EXPECT_TRUE(infiltration.setShelterFactor(0.5));

  ForwardTranslator ft;
  // Not assigned a space or spacetype? => not translated
  {

    Workspace w = ft.translateModel(model);

    std::string errors;
    for (const auto& error : ft.errors()) {
      errors += error.logMessage() + "\n";
    }
    EXPECT_EQ(0u, ft.errors().size()) << errors;

    std::string warnings;
    for (const auto& warning : ft.warnings()) {
      warnings += warning.logMessage() + "\n";
    }
    EXPECT_EQ(1u, ft.warnings().size()) << warnings;

    EXPECT_EQ(0u, w.getObjectsByType(IddObjectType::ZoneInfiltration_FlowCoefficient).size());
  }

  ThermalZone zone(model);
  zone.setName("Zone1");
  Space space(model);
  space.setName("Space1");
  space.setThermalZone(zone);
  infiltration.setSpace(space);

  for (bool excludeSpaceTranslation : {true, false}) {
    ft.setExcludeSpaceTranslation(excludeSpaceTranslation);

    Workspace w = ft.translateModel(model);
    EXPECT_EQ(0u, ft.errors().size());
    EXPECT_EQ(0u, ft.warnings().size());

    WorkspaceObjectVector idfObjs = w.getObjectsByType(IddObjectType::ZoneInfiltration_FlowCoefficient);
    ASSERT_EQ(1u, idfObjs.size());

    WorkspaceObject idfObject(idfObjs[0]);

    EXPECT_EQ("My Infiltration", idfObject.getString(ZoneInfiltration_FlowCoefficientFields::Name).get());
    if (excludeSpaceTranslation) {
      EXPECT_EQ("Zone1", idfObject.getString(ZoneInfiltration_FlowCoefficientFields::ZoneorSpaceName).get());
    } else {
      EXPECT_EQ("Space1", idfObject.getString(ZoneInfiltration_FlowCoefficientFields::ZoneorSpaceName).get());
    }

    EXPECT_FALSE(infiltration.schedule());
    EXPECT_EQ("Always On Discrete", idfObject.getString(ZoneInfiltration_FlowCoefficientFields::ScheduleName).get());

    EXPECT_EQ(0.1, idfObject.getDouble(ZoneInfiltration_FlowCoefficientFields::FlowCoefficient).get());
    EXPECT_EQ(0.2, idfObject.getDouble(ZoneInfiltration_FlowCoefficientFields::StackCoefficient).get());
    EXPECT_EQ(0.3, idfObject.getDouble(ZoneInfiltration_FlowCoefficientFields::PressureExponent).get());
    EXPECT_EQ(0.4, idfObject.getDouble(ZoneInfiltration_FlowCoefficientFields::WindCoefficient).get());
    EXPECT_EQ(0.5, idfObject.getDouble(ZoneInfiltration_FlowCoefficientFields::ShelterFactor).get());
  }

  {
    ScheduleConstant sch(model);
    sch.setName("My Infiltration Schedule");
    EXPECT_TRUE(infiltration.setSchedule(sch));

    Workspace w = ft.translateModel(model);

    WorkspaceObjectVector idfObjs = w.getObjectsByType(IddObjectType::ZoneInfiltration_FlowCoefficient);
    ASSERT_EQ(1u, idfObjs.size());

    WorkspaceObject idfObject(idfObjs[0]);

    ASSERT_TRUE(infiltration.schedule());
    EXPECT_EQ(infiltration.schedule()->nameString(), idfObject.getString(ZoneInfiltration_FlowCoefficientFields::ScheduleName).get());
  }
}

TEST_F(EnergyPlusFixture, ReverseTranslator_SpaceInfiltrationFlowCoefficient) {

  ReverseTranslator rt;

  Workspace w(StrictnessLevel::Minimal, IddFileType::EnergyPlus);

  OptionalWorkspaceObject _i_infiltration = w.addObject(IdfObject(IddObjectType::ZoneInfiltration_FlowCoefficient));
  ASSERT_TRUE(_i_infiltration);
  _i_infiltration->setName("Infiltration");

  OptionalWorkspaceObject _i_sch = w.addObject(IdfObject(IddObjectType::Schedule_Constant));
  ASSERT_TRUE(_i_sch);
  _i_sch->setName("InfiltrationSchedule");
  EXPECT_TRUE(_i_sch->setDouble(Schedule_ConstantFields::HourlyValue, 1.0));
  EXPECT_TRUE(_i_infiltration->setPointer(ZoneInfiltration_FlowCoefficientFields::ScheduleName, _i_sch->handle()));

  EXPECT_TRUE(_i_infiltration->setDouble(ZoneInfiltration_FlowCoefficientFields::FlowCoefficient, 0.1));
  EXPECT_TRUE(_i_infiltration->setDouble(ZoneInfiltration_FlowCoefficientFields::StackCoefficient, 0.2));
  EXPECT_TRUE(_i_infiltration->setDouble(ZoneInfiltration_FlowCoefficientFields::PressureExponent, 0.3));
  EXPECT_TRUE(_i_infiltration->setDouble(ZoneInfiltration_FlowCoefficientFields::WindCoefficient, 0.4));
  EXPECT_TRUE(_i_infiltration->setDouble(ZoneInfiltration_FlowCoefficientFields::ShelterFactor, 0.5));

  OptionalWorkspaceObject _i_globalGeometryRules = w.addObject(IdfObject(IddObjectType::GlobalGeometryRules));
  ASSERT_TRUE(_i_globalGeometryRules);

  _i_globalGeometryRules->setString(openstudio::GlobalGeometryRulesFields::StartingVertexPosition, "UpperLeftCorner");
  _i_globalGeometryRules->setString(openstudio::GlobalGeometryRulesFields::VertexEntryDirection, "Counterclockwise");
  _i_globalGeometryRules->setString(openstudio::GlobalGeometryRulesFields::CoordinateSystem, "Relative");
  _i_globalGeometryRules->setString(openstudio::GlobalGeometryRulesFields::DaylightingReferencePointCoordinateSystem, "Relative");
  _i_globalGeometryRules->setString(openstudio::GlobalGeometryRulesFields::RectangularSurfaceCoordinateSystem, "Relative");

  OptionalWorkspaceObject _i_building = w.addObject(IdfObject(IddObjectType::Building));
  ASSERT_TRUE(_i_building);

  // No Zone assigned: not RT'ed
  {
    ASSERT_NO_THROW(rt.translateWorkspace(w));
    Model model = rt.translateWorkspace(w);
    EXPECT_EQ(1, rt.errors().size());
    EXPECT_TRUE(rt.warnings().empty());

    auto infiltrations = model.getConcreteModelObjects<openstudio::model::SpaceInfiltrationFlowCoefficient>();
    EXPECT_EQ(0, infiltrations.size());
  }

  // Zone Name assigned: RT'ed
  {
    OptionalWorkspaceObject _i_zone = w.addObject(IdfObject(IddObjectType::Zone));
    ASSERT_TRUE(_i_zone);
    _i_zone->setName("Zone1");

    EXPECT_TRUE(_i_infiltration->setPointer(ZoneInfiltration_FlowCoefficientFields::ZoneorSpaceName, _i_zone->handle()));

    ASSERT_NO_THROW(rt.translateWorkspace(w));
    Model model = rt.translateWorkspace(w);
    EXPECT_TRUE(rt.errors().empty());
    EXPECT_TRUE(rt.warnings().empty());

    auto infiltrations = model.getConcreteModelObjects<openstudio::model::SpaceInfiltrationFlowCoefficient>();
    ASSERT_EQ(1, infiltrations.size());
    auto infiltration = infiltrations[0];

    EXPECT_EQ("Infiltration", infiltration.nameString());
    ASSERT_TRUE(infiltration.schedule());
    EXPECT_EQ("InfiltrationSchedule", infiltration.schedule()->nameString());
    ASSERT_TRUE(infiltration.space());
    EXPECT_FALSE(infiltration.spaceType());
    EXPECT_EQ("Zone1 Space", infiltration.space()->nameString());
    EXPECT_EQ(0.1, infiltration.flowCoefficient());
    EXPECT_EQ(0.2, infiltration.stackCoefficient());
    EXPECT_EQ(0.3, infiltration.pressureExponent());
    EXPECT_EQ(0.4, infiltration.windCoefficient());
    EXPECT_EQ(0.5, infiltration.shelterFactor());
  }
}

TEST_F(EnergyPlusFixture, ForwardTranslator_SpaceInfiltrationFlowCoefficient_SpaceTypes) {

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

  SpaceInfiltrationFlowCoefficient infilBuilding(m);
  infilBuilding.setName("infiltrationBuilding");
  EXPECT_TRUE(infilBuilding.setSpaceType(buildingSpaceType));

  auto building = m.getUniqueModelObject<Building>();
  EXPECT_TRUE(building.setSpaceType(buildingSpaceType));

  // Create an Office Space Type. Assign to Space 1 & 2 only (not 3 nor 4), it also has an infiltration object
  SpaceType officeSpaceType(m);
  officeSpaceType.setName("officeSpaceType");
  SpaceInfiltrationFlowCoefficient infilOffice(m);
  infilOffice.setName("infilOffice");
  EXPECT_TRUE(infilOffice.setSpaceType(officeSpaceType));
  EXPECT_TRUE(space1.setSpaceType(officeSpaceType));
  EXPECT_TRUE(space2.setSpaceType(officeSpaceType));

  // Space 1 and 3 have a space-level infiltration
  SpaceInfiltrationFlowCoefficient infilSpace1(m);
  infilSpace1.setName("infilSpace1");
  EXPECT_TRUE(infilSpace1.setSpace(space1));

  SpaceInfiltrationFlowCoefficient infilSpace3(m);
  infilSpace3.setName("infilSpace3");
  EXPECT_TRUE(infilSpace3.setSpace(space3));

  // Infiltration Characteristics: This object is absolute, so it's a lot more simple than SpaceInfiltrationDesignFlowRate for eg
  // =============================
  //
  // | Infiltration Name |  Flow Coefficient |  Stack Coefficient |  Pressure Exponent |  Wind Coefficient | Shelter Factor |
  // |-------------------|-------------------|--------------------|--------------------|-------------------|----------------|
  // | infilSpace1       | 0.001             | 0.01               | 0.1                | 0.04              | 0.025          |
  // | infilSpace3       | 0.002             | 0.02               | 0.2                | 0.08              | 0.05           |
  // | infilOffice       | 0.003             | 0.03               | 0.3                | 0.12              | 0.075          |
  // | infilBuilding     | 0.004             | 0.04               | 0.4                | 0.16              | 0.1            |

  // Which infiltation object(s) apply to each space
  // ================================================
  //
  // | Space    | Space Specific Infiltration ║ SpaceType                     | Additional Inherited Infiltration |
  // |----------|-----------------------------║-------------------------------|-----------------------------------|
  // | Space 1  | infilSpace1                 ║ officeSpaceType               | infilOffice                       |
  // | Space 2  |                             ║ officeSpaceType               | infilOffice                       |
  // | Space 3  | infilSpace3                 ║ Inherited: buildingSpaceType  | infilBuilding                     |
  // | Space 4  |                             ║ Inherited: buildingSpaceType  | infilBuilding                     |

  constexpr double flowCoefficient = 0.001;
  constexpr double stackCoefficient = 0.01;
  constexpr double pressureExponent = 0.1;
  constexpr double windCoefficient = 0.04;
  constexpr double shelterFactor = 0.025;

  auto setCoefficients = [&, multiplier = 1](SpaceInfiltrationFlowCoefficient& infil) mutable {
    EXPECT_TRUE(infil.setFlowCoefficient(flowCoefficient * multiplier));
    EXPECT_TRUE(infil.setStackCoefficient(stackCoefficient * multiplier));
    EXPECT_TRUE(infil.setPressureExponent(pressureExponent * multiplier));
    EXPECT_TRUE(infil.setWindCoefficient(windCoefficient * multiplier));
    EXPECT_TRUE(infil.setShelterFactor(shelterFactor * multiplier));
    multiplier++;
  };
  setCoefficients(infilSpace1);
  setCoefficients(infilSpace3);
  setCoefficients(infilOffice);
  setCoefficients(infilBuilding);
  EXPECT_EQ(0.001, infilSpace1.flowCoefficient());
  EXPECT_EQ(0.002, infilSpace3.flowCoefficient());
  EXPECT_EQ(0.003, infilOffice.flowCoefficient());
  EXPECT_EQ(0.004, infilBuilding.flowCoefficient());

  ForwardTranslator ft;

  // When excluding space translation (historical behavior)
  //
  {
    ft.setExcludeSpaceTranslation(true);

    Workspace w = ft.translateModel(m);

    auto zones = w.getObjectsByType(IddObjectType::Zone);
    ASSERT_EQ(1, zones.size());
    auto zone = zones[0];
    EXPECT_EQ(0, w.getObjectsByType(IddObjectType::ZoneList).size());
    EXPECT_EQ(0, w.getObjectsByType(IddObjectType::Space).size());
    EXPECT_EQ(0, w.getObjectsByType(IddObjectType::SpaceList).size());

    auto infils = w.getObjectsByType(IddObjectType::ZoneInfiltration_FlowCoefficient);
    // I expect infilSpace1, infilSpace3, two infilOffice and two infilBuilding, so 6 total
    ASSERT_EQ(6, infils.size());

    for (const auto& infil : infils) {
      auto name = infil.nameString();
      auto z_ = infil.getTarget(ZoneInfiltration_FlowCoefficientFields::ZoneorSpaceName);
      ASSERT_TRUE(z_);
      EXPECT_EQ(zone, z_.get());
      if (name.find(infilSpace1.nameString()) != std::string::npos) {
        EXPECT_EQ(infilSpace1.flowCoefficient(), infil.getDouble(ZoneInfiltration_FlowCoefficientFields::FlowCoefficient).get());
        EXPECT_EQ(infilSpace1.stackCoefficient(), infil.getDouble(ZoneInfiltration_FlowCoefficientFields::StackCoefficient).get());
        EXPECT_EQ(infilSpace1.pressureExponent(), infil.getDouble(ZoneInfiltration_FlowCoefficientFields::PressureExponent).get());
        EXPECT_EQ(infilSpace1.windCoefficient(), infil.getDouble(ZoneInfiltration_FlowCoefficientFields::WindCoefficient).get());
        EXPECT_EQ(infilSpace1.shelterFactor(), infil.getDouble(ZoneInfiltration_FlowCoefficientFields::ShelterFactor).get());
      } else if (name.find(infilSpace3.nameString()) != std::string::npos) {
        EXPECT_EQ(infilSpace3.flowCoefficient(), infil.getDouble(ZoneInfiltration_FlowCoefficientFields::FlowCoefficient).get());
        EXPECT_EQ(infilSpace3.stackCoefficient(), infil.getDouble(ZoneInfiltration_FlowCoefficientFields::StackCoefficient).get());
        EXPECT_EQ(infilSpace3.pressureExponent(), infil.getDouble(ZoneInfiltration_FlowCoefficientFields::PressureExponent).get());
        EXPECT_EQ(infilSpace3.windCoefficient(), infil.getDouble(ZoneInfiltration_FlowCoefficientFields::WindCoefficient).get());
        EXPECT_EQ(infilSpace3.shelterFactor(), infil.getDouble(ZoneInfiltration_FlowCoefficientFields::ShelterFactor).get());
      } else if (name.find(infilOffice.nameString()) != std::string::npos) {
        EXPECT_EQ(infilOffice.flowCoefficient(), infil.getDouble(ZoneInfiltration_FlowCoefficientFields::FlowCoefficient).get());
        EXPECT_EQ(infilOffice.stackCoefficient(), infil.getDouble(ZoneInfiltration_FlowCoefficientFields::StackCoefficient).get());
        EXPECT_EQ(infilOffice.pressureExponent(), infil.getDouble(ZoneInfiltration_FlowCoefficientFields::PressureExponent).get());
        EXPECT_EQ(infilOffice.windCoefficient(), infil.getDouble(ZoneInfiltration_FlowCoefficientFields::WindCoefficient).get());
        EXPECT_EQ(infilOffice.shelterFactor(), infil.getDouble(ZoneInfiltration_FlowCoefficientFields::ShelterFactor).get());
      } else if (name.find(infilBuilding.nameString()) != std::string::npos) {
        EXPECT_EQ(infilBuilding.flowCoefficient(), infil.getDouble(ZoneInfiltration_FlowCoefficientFields::FlowCoefficient).get());
        EXPECT_EQ(infilBuilding.stackCoefficient(), infil.getDouble(ZoneInfiltration_FlowCoefficientFields::StackCoefficient).get());
        EXPECT_EQ(infilBuilding.pressureExponent(), infil.getDouble(ZoneInfiltration_FlowCoefficientFields::PressureExponent).get());
        EXPECT_EQ(infilBuilding.windCoefficient(), infil.getDouble(ZoneInfiltration_FlowCoefficientFields::WindCoefficient).get());
        EXPECT_EQ(infilBuilding.shelterFactor(), infil.getDouble(ZoneInfiltration_FlowCoefficientFields::ShelterFactor).get());
      }
    }
  }

  // When including Space translation (new E+ 9.6.0)
  // same thing as before: we place them for the Zone itself. Note that this object DOES NOT ACCEPT a ZoneList, only a Zone!
  {
    ft.setExcludeSpaceTranslation(false);

    Workspace w = ft.translateModel(m);

    auto zones = w.getObjectsByType(IddObjectType::Zone);
    ASSERT_EQ(1, zones.size());
    auto zone = zones[0];
    EXPECT_EQ(0, w.getObjectsByType(IddObjectType::ZoneList).size());
    EXPECT_EQ(4, w.getObjectsByType(IddObjectType::Space).size());
    EXPECT_EQ(2, w.getObjectsByType(IddObjectType::SpaceList).size());

    auto infils = w.getObjectsByType(IddObjectType::ZoneInfiltration_FlowCoefficient);
    // I expect infilSpace1, infilSpace3, two infilOffice and two infilBuilding, so 6 total, all pointing to a Space
    ASSERT_EQ(6, infils.size());

    bool foundOfficeSpace1 = false;
    bool foundOfficeSpace2 = false;
    bool foundBuildingSpace3 = false;
    bool foundBuildingSpace4 = false;

    for (const auto& infil : infils) {
      auto name = infil.nameString();
      auto target_ = infil.getTarget(ZoneInfiltration_FlowCoefficientFields::ZoneorSpaceName);
      ASSERT_TRUE(target_);
      if (name.find(infilSpace1.nameString()) != std::string::npos) {
        EXPECT_EQ(target_->iddObject().type(), IddObjectType::Space);
        EXPECT_EQ(space1.nameString(), target_->nameString());

        EXPECT_EQ(infilSpace1.flowCoefficient(), infil.getDouble(ZoneInfiltration_FlowCoefficientFields::FlowCoefficient).get());
        EXPECT_EQ(infilSpace1.stackCoefficient(), infil.getDouble(ZoneInfiltration_FlowCoefficientFields::StackCoefficient).get());
        EXPECT_EQ(infilSpace1.pressureExponent(), infil.getDouble(ZoneInfiltration_FlowCoefficientFields::PressureExponent).get());
        EXPECT_EQ(infilSpace1.windCoefficient(), infil.getDouble(ZoneInfiltration_FlowCoefficientFields::WindCoefficient).get());
        EXPECT_EQ(infilSpace1.shelterFactor(), infil.getDouble(ZoneInfiltration_FlowCoefficientFields::ShelterFactor).get());
      } else if (name.find(infilSpace3.nameString()) != std::string::npos) {
        EXPECT_EQ(target_->iddObject().type(), IddObjectType::Space);
        EXPECT_EQ(space3.nameString(), target_->nameString());

        EXPECT_EQ(infilSpace3.flowCoefficient(), infil.getDouble(ZoneInfiltration_FlowCoefficientFields::FlowCoefficient).get());
        EXPECT_EQ(infilSpace3.stackCoefficient(), infil.getDouble(ZoneInfiltration_FlowCoefficientFields::StackCoefficient).get());
        EXPECT_EQ(infilSpace3.pressureExponent(), infil.getDouble(ZoneInfiltration_FlowCoefficientFields::PressureExponent).get());
        EXPECT_EQ(infilSpace3.windCoefficient(), infil.getDouble(ZoneInfiltration_FlowCoefficientFields::WindCoefficient).get());
        EXPECT_EQ(infilSpace3.shelterFactor(), infil.getDouble(ZoneInfiltration_FlowCoefficientFields::ShelterFactor).get());
      } else if (name.find(infilOffice.nameString()) != std::string::npos) {
        if (space1.nameString() == target_->nameString()) {
          foundOfficeSpace1 = true;
        } else if (space2.nameString() == target_->nameString()) {
          foundOfficeSpace2 = true;
        }

        EXPECT_EQ(infilOffice.flowCoefficient(), infil.getDouble(ZoneInfiltration_FlowCoefficientFields::FlowCoefficient).get());
        EXPECT_EQ(infilOffice.stackCoefficient(), infil.getDouble(ZoneInfiltration_FlowCoefficientFields::StackCoefficient).get());
        EXPECT_EQ(infilOffice.pressureExponent(), infil.getDouble(ZoneInfiltration_FlowCoefficientFields::PressureExponent).get());
        EXPECT_EQ(infilOffice.windCoefficient(), infil.getDouble(ZoneInfiltration_FlowCoefficientFields::WindCoefficient).get());
        EXPECT_EQ(infilOffice.shelterFactor(), infil.getDouble(ZoneInfiltration_FlowCoefficientFields::ShelterFactor).get());
      } else if (name.find(infilBuilding.nameString()) != std::string::npos) {
        if (space3.nameString() == target_->nameString()) {
          foundBuildingSpace3 = true;
        } else if (space4.nameString() == target_->nameString()) {
          foundBuildingSpace4 = true;
        }

        EXPECT_EQ(infilBuilding.flowCoefficient(), infil.getDouble(ZoneInfiltration_FlowCoefficientFields::FlowCoefficient).get());
        EXPECT_EQ(infilBuilding.stackCoefficient(), infil.getDouble(ZoneInfiltration_FlowCoefficientFields::StackCoefficient).get());
        EXPECT_EQ(infilBuilding.pressureExponent(), infil.getDouble(ZoneInfiltration_FlowCoefficientFields::PressureExponent).get());
        EXPECT_EQ(infilBuilding.windCoefficient(), infil.getDouble(ZoneInfiltration_FlowCoefficientFields::WindCoefficient).get());
        EXPECT_EQ(infilBuilding.shelterFactor(), infil.getDouble(ZoneInfiltration_FlowCoefficientFields::ShelterFactor).get());
      }
    }

    EXPECT_TRUE(foundOfficeSpace1);
    EXPECT_TRUE(foundOfficeSpace2);
    EXPECT_TRUE(foundBuildingSpace3);
    EXPECT_TRUE(foundBuildingSpace4);
  }
}
