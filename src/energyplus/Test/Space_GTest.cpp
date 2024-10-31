/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
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
#include "../../model/ElectricEquipment.hpp"
#include "../../model/ElectricEquipment_Impl.hpp"
#include "../../model/ElectricEquipmentDefinition.hpp"
#include "../../model/ElectricEquipmentDefinition_Impl.hpp"
#include "../../model/Lights.hpp"
#include "../../model/Lights_Impl.hpp"
#include "../../model/LightsDefinition.hpp"
#include "../../model/LightsDefinition_Impl.hpp"
#include "../../model/People.hpp"
#include "../../model/People_Impl.hpp"
#include "../../model/PeopleDefinition.hpp"
#include "../../model/PeopleDefinition_Impl.hpp"
#include "../../model/Schedule.hpp"

#include "../../model/InternalMass.hpp"
#include "../../model/InternalMass_Impl.hpp"
#include "../../model/InteriorPartitionSurface.hpp"
#include "../../model/InteriorPartitionSurface_Impl.hpp"

#include <utilities/idd/Zone_FieldEnums.hxx>
#include <utilities/idd/Space_FieldEnums.hxx>
#include <utilities/idd/SpaceList_FieldEnums.hxx>
#include <utilities/idd/Lights_FieldEnums.hxx>
#include <utilities/idd/People_FieldEnums.hxx>
#include <utilities/idd/ElectricEquipment_FieldEnums.hxx>
#include <utilities/idd/BuildingSurface_Detailed_FieldEnums.hxx>

#include <utilities/idd/IddEnums.hxx>

#include "../../utilities/idf/IdfExtensibleGroup.hpp"
#include "../../utilities/idf/WorkspaceExtensibleGroup.hpp"
#include "../../utilities/core/Compare.hpp"

#include <resources.hxx>

#include <algorithm>
#include <iterator>
#include <map>
#include <vector>

using namespace openstudio::energyplus;
using namespace openstudio::model;
using namespace openstudio;

TEST_F(EnergyPlusFixture, ForwardTranslator_Space) {
  Model model;

  Space space(model);

  ForwardTranslator ft;

  // space not translated when not in thermal zone
  Workspace workspace = ft.translateModel(model);
  EXPECT_EQ(0U, workspace.getObjectsByType(IddObjectType::Zone).size());
  EXPECT_EQ(0U, workspace.getObjectsByType(IddObjectType::ComponentCost_LineItem).size());

  // space not translated when not in thermal zone
  workspace = ft.translateModel(model);
  EXPECT_EQ(0U, workspace.getObjectsByType(IddObjectType::Zone).size());
  EXPECT_EQ(0U, workspace.getObjectsByType(IddObjectType::ComponentCost_LineItem).size());

  ThermalZone thermalZone(model);
  space.setThermalZone(thermalZone);

  workspace = ft.translateModel(model);
  EXPECT_EQ(1U, workspace.getObjectsByType(IddObjectType::Zone).size());

  // TODO: add another space to the thermal zone
  Space space2(model);
  space2.setThermalZone(thermalZone);

  workspace = ft.translateModel(model);
  EXPECT_EQ(1U, workspace.getObjectsByType(IddObjectType::Zone).size());
}

TEST_F(EnergyPlusFixture, ForwardTranslator_Space2) {
  Model model;

  Point3dVector points1{
    {0, 1, 0},
    {1, 1, 0},
    {1, 0, 0},
    {0, 0, 0},
  };
  Surface floor1(points1, model);

  Point3dVector points2{
    {0, 2, 0},
    {2, 2, 0},
    {2, 0, 0},
    {0, 0, 0},
  };
  Surface floor2(points2, model);

  Space space1(model);
  EXPECT_TRUE(space1.setFloorArea(1));
  Space space2(model);
  EXPECT_TRUE(space2.setFloorArea(4));
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

  ElectricEquipmentDefinition definition1(model);
  ElectricEquipment electricEquipment1(definition1);
  electricEquipment1.setSpaceType(spaceType1);

  ElectricEquipmentDefinition definition2(model);
  ElectricEquipment electricEquipment2(definition2);
  electricEquipment2.setSpaceType(spaceType2);

  ForwardTranslator ft;
  ft.setExcludeSpaceTranslation(false);

  Workspace workspace = ft.translateModel(model);

  WorkspaceObjectVector idf_spaces(workspace.getObjectsByType(IddObjectType::Space));
  ASSERT_EQ(2U, idf_spaces.size());
  std::sort(idf_spaces.begin(), idf_spaces.end(), IdfObjectNameLess());
  WorkspaceObject idf_space1(idf_spaces[0]);
  WorkspaceObject idf_space2(idf_spaces[1]);

  WorkspaceObjectVector idf_zones(workspace.getObjectsByType(IddObjectType::Zone));
  EXPECT_EQ(1U, idf_zones.size());
  WorkspaceObject idf_zone(idf_zones[0]);

  boost::optional<WorkspaceObject> idf_zone1(idf_space1.getTarget(SpaceFields::ZoneName));
  ASSERT_TRUE(idf_zone1);
  EXPECT_EQ(idf_zone1->iddObject().type(), IddObjectType::Zone);
  EXPECT_EQ(thermalZone.nameString(), idf_zone1->name().get());
  EXPECT_EQ(idf_zone1.get(), idf_zone);

  boost::optional<WorkspaceObject> idf_zone2(idf_space2.getTarget(SpaceFields::ZoneName));
  ASSERT_TRUE(idf_zone2);
  EXPECT_EQ(idf_zone2->iddObject().type(), IddObjectType::Zone);
  EXPECT_EQ(thermalZone.nameString(), idf_zone2->name().get());
  EXPECT_EQ(idf_zone2.get(), idf_zone);

  double space1_floorArea = idf_space1.getDouble(SpaceFields::FloorArea, false).get();
  double space2_floorArea = idf_space2.getDouble(SpaceFields::FloorArea, false).get();
  EXPECT_NEAR(space1.floorArea(), space1_floorArea, 0.0001);
  EXPECT_NEAR(space2.floorArea(), space2_floorArea, 0.0001);
  EXPECT_NE("", idf_space1.getString(SpaceFields::SpaceType).get());
  EXPECT_NE("", idf_space2.getString(SpaceFields::SpaceType).get());

  EXPECT_FALSE(idf_zone.getDouble(ZoneFields::FloorArea, false));  // 'Part of Total Floor Area' not mixed

  WorkspaceObjectVector idf_spaceLists(workspace.getObjectsByType(IddObjectType::SpaceList));
  ASSERT_EQ(2U, idf_spaceLists.size());
  std::sort(idf_spaceLists.begin(), idf_spaceLists.end(), IdfObjectNameLess());
  WorkspaceObject idf_spaceList1(idf_spaceLists[0]);
  WorkspaceObject idf_spaceList2(idf_spaceLists[1]);

  EXPECT_EQ(spaceType1.name().get(), idf_spaceList1.name().get());
  EXPECT_EQ(spaceType2.name().get(), idf_spaceList2.name().get());

  ASSERT_EQ(1U, idf_spaceList1.extensibleGroups().size());
  ASSERT_EQ(1U, idf_spaceList2.extensibleGroups().size());

  auto eg_spaceList1 = idf_spaceList1.extensibleGroups()[0].cast<WorkspaceExtensibleGroup>();
  ASSERT_TRUE(eg_spaceList1.getString(SpaceListExtensibleFields::SpaceName));
  EXPECT_EQ(space1.name().get(), eg_spaceList1.getString(SpaceListExtensibleFields::SpaceName).get());

  auto eg_spaceList2 = idf_spaceList2.extensibleGroups()[0].cast<WorkspaceExtensibleGroup>();
  ASSERT_TRUE(eg_spaceList2.getString(SpaceListExtensibleFields::SpaceName));
  EXPECT_EQ(space2.name().get(), eg_spaceList2.getString(SpaceListExtensibleFields::SpaceName).get());

  EXPECT_EQ(0U, workspace.getObjectsByType(IddObjectType::ZoneList).size());

  // Check floor area when space1 is not part of total floor area but space2 is
  EXPECT_TRUE(space1.setPartofTotalFloorArea(false));

  workspace = ft.translateModel(model);

  idf_zones = workspace.getObjectsByType(IddObjectType::Zone);
  EXPECT_EQ(1U, idf_zones.size());
  idf_zone = idf_zones[0];

  ASSERT_TRUE(idf_zone.getDouble(ZoneFields::FloorArea, false));  // 'Part of Total Floor Area' is mixed
  EXPECT_NEAR(space2_floorArea, idf_zone.getDouble(ZoneFields::FloorArea, false).get(), 0.0001);

  // Check floor area when space1 and space2 are not part of total floor area
  EXPECT_TRUE(space2.setPartofTotalFloorArea(false));

  workspace = ft.translateModel(model);

  idf_zones = workspace.getObjectsByType(IddObjectType::Zone);
  EXPECT_EQ(1U, idf_zones.size());
  idf_zone = idf_zones[0];

  EXPECT_FALSE(idf_zone.getDouble(ZoneFields::FloorArea, false));  // 'Part of Total Floor Area' not mixed
}

TEST_F(EnergyPlusFixture, Space_exampleModel_Loads) {

  Model m = model::exampleModel();

  ASSERT_EQ(1U, m.getConcreteModelObjects<SpaceType>().size());
  SpaceType spaceType = m.getConcreteModelObjects<SpaceType>()[0];

  ASSERT_EQ(1U, m.getConcreteModelObjects<ThermalZone>().size());
  ThermalZone thermalZone = m.getConcreteModelObjects<ThermalZone>()[0];

  auto spaces = m.getConcreteModelObjects<Space>();
  EXPECT_EQ(4U, spaces.size());
  for (const Space& space : m.getConcreteModelObjects<Space>()) {
    ASSERT_TRUE(space.spaceType());
    EXPECT_EQ(spaceType.handle(), space.spaceType()->handle());
    ASSERT_TRUE(space.thermalZone());
    EXPECT_EQ(thermalZone.handle(), space.thermalZone()->handle());
  }

  ASSERT_EQ(1U, m.getConcreteModelObjects<Lights>().size());
  auto lights = m.getConcreteModelObjects<Lights>()[0];
  auto lightsDefinition = lights.definition().cast<LightsDefinition>();
  EXPECT_FALSE(lights.space());
  ASSERT_TRUE(lights.spaceType());
  EXPECT_EQ(spaceType.handle(), lights.spaceType()->handle());

  ASSERT_EQ(1U, m.getConcreteModelObjects<People>().size());
  auto people = m.getConcreteModelObjects<People>()[0];
  auto peopleDefinition = people.definition().cast<PeopleDefinition>();
  EXPECT_FALSE(people.space());
  ASSERT_TRUE(people.spaceType());
  EXPECT_EQ(spaceType.handle(), people.spaceType()->handle());

  auto eqs = m.getConcreteModelObjects<ElectricEquipment>();
  ASSERT_EQ(2U, eqs.size());

  auto [eqSpace, eqSpaceType] = [&eqs]() -> std::tuple<ElectricEquipment, ElectricEquipment> {
    auto eq1 = eqs[0];
    auto eq2 = eqs[1];
    if (eq1.space()) {
      return std::make_tuple(eq1, eq2);
    } else {
      return std::make_tuple(eq2, eq1);
    }
  }();

  ASSERT_TRUE(eqSpace.space());
  EXPECT_FALSE(eqSpace.spaceType());
  EXPECT_FALSE(eqSpaceType.space());
  ASSERT_TRUE(eqSpaceType.spaceType());

  auto eqSpaceDefinition = eqSpace.definition().cast<ElectricEquipmentDefinition>();
  auto eqSpaceTypeDefinition = eqSpaceType.definition().cast<ElectricEquipmentDefinition>();

  std::vector<std::string> spaceNames;
  spaceNames.reserve(spaces.size());
  std::transform(spaces.begin(), spaces.end(), std::back_inserter(spaceNames), [](const auto& s) { return s.nameString(); });
  std::sort(spaceNames.begin(), spaceNames.end());

  auto spaceWithDirectElectricEquipment = eqSpace.space().get();

  ForwardTranslator ft;

  {
    ft.setExcludeSpaceTranslation(false);

    Workspace w = ft.translateModel(m);
    EXPECT_EQ(0U, ft.errors().size());

    // 1 Zone, 4 spaces
    auto i_zones = w.getObjectsByType(IddObjectType::Zone);
    ASSERT_EQ(1U, i_zones.size());
    auto i_zone = i_zones[0];

    // Each space references the zone correctly, and has a SpaceType tag that matches the SpaceType (for reporting purposes)
    auto i_spaces = w.getObjectsByType(IddObjectType::Space);
    ASSERT_EQ(4U, i_spaces.size());
    for (const auto& i_space : i_spaces) {
      EXPECT_EQ(i_zone.nameString(), i_space.getString(SpaceFields::ZoneName).get());
      EXPECT_EQ(spaceType.nameString(), i_space.getString(SpaceFields::SpaceType).get());

      auto it =
        std::find_if(spaceNames.begin(), spaceNames.end(), [&i_space](const auto& s) { return openstudio::istringEqual(s, i_space.nameString()); });
      EXPECT_NE(spaceNames.end(), it);

      // let E+ figure it out
      EXPECT_TRUE(i_space.isEmpty(SpaceFields::FloorArea));
    }

    // There's one SpaceList corresponding to the SpaceType (for load attribution) and it does have the 4 spaces in there
    auto i_spaceLists = w.getObjectsByType(IddObjectType::SpaceList);
    ASSERT_EQ(1U, i_spaceLists.size());
    auto i_spaceList = i_spaceLists[0];
    EXPECT_EQ(spaceType.nameString(), i_spaceList.nameString());
    EXPECT_EQ(4U, i_spaceList.numExtensibleGroups());
    std::vector<IdfExtensibleGroup> egs = i_spaceList.extensibleGroups();
    std::vector<std::string> i_spaceNames;
    i_spaceNames.reserve(spaces.size());
    std::transform(egs.begin(), egs.end(), std::back_inserter(i_spaceNames),
                   [](const auto& eg) { return eg.getString(SpaceListExtensibleFields::SpaceName).get(); });
    std::sort(i_spaceNames.begin(), i_spaceNames.end());
    EXPECT_TRUE(std::equal(spaceNames.begin(), spaceNames.end(), i_spaceNames.begin(), i_spaceNames.end()));

    // Now get cranking on loads. We must find 4 loads as in the model
    auto i_lightss = w.getObjectsByType(IddObjectType::Lights);
    auto i_peoples = w.getObjectsByType(IddObjectType::People);
    auto i_eqs = w.getObjectsByType(IddObjectType::ElectricEquipment);
    ASSERT_EQ(1U, i_lightss.size());
    ASSERT_EQ(1U, i_peoples.size());
    ASSERT_EQ(2U, i_eqs.size());

    {
      auto i_lights = i_lightss[0];
      EXPECT_EQ(lights.nameString(), i_lights.nameString());

      ASSERT_TRUE(i_lights.getTarget(LightsFields::ZoneorZoneListorSpaceorSpaceListName));
      EXPECT_EQ(spaceType.nameString(), i_lights.getString(LightsFields::ZoneorZoneListorSpaceorSpaceListName).get());
      EXPECT_EQ(i_spaceList, i_lights.getTarget(LightsFields::ZoneorZoneListorSpaceorSpaceListName).get());

      EXPECT_EQ(lights.schedule()->nameString(), i_lights.getString(LightsFields::ScheduleName).get());
      EXPECT_EQ(lightsDefinition.designLevelCalculationMethod(), i_lights.getString(LightsFields::DesignLevelCalculationMethod).get());
      EXPECT_EQ("Watts/Area", i_lights.getString(LightsFields::DesignLevelCalculationMethod).get());

      EXPECT_TRUE(i_lights.isEmpty(LightsFields::LightingLevel));
      EXPECT_EQ(lightsDefinition.wattsperSpaceFloorArea().get(), i_lights.getDouble(LightsFields::WattsperFloorArea).get());
      EXPECT_TRUE(i_lights.isEmpty(LightsFields::WattsperPerson));
      EXPECT_TRUE(i_lights.isEmpty(LightsFields::ReturnAirFraction));
      EXPECT_TRUE(i_lights.isEmpty(LightsFields::FractionRadiant));
      EXPECT_TRUE(i_lights.isEmpty(LightsFields::FractionVisible));
      EXPECT_EQ(lights.fractionReplaceable(), i_lights.getDouble(LightsFields::FractionReplaceable).get());
      EXPECT_EQ(lights.endUseSubcategory(), i_lights.getString(LightsFields::EndUseSubcategory).get());
    }
    {
      auto i_people = i_peoples[0];
      EXPECT_EQ(people.nameString(), i_people.nameString());

      ASSERT_TRUE(i_people.getTarget(PeopleFields::ZoneorZoneListorSpaceorSpaceListName));
      EXPECT_EQ(spaceType.nameString(), i_people.getString(PeopleFields::ZoneorZoneListorSpaceorSpaceListName).get());
      EXPECT_EQ(i_spaceList, i_people.getTarget(PeopleFields::ZoneorZoneListorSpaceorSpaceListName).get());
      EXPECT_EQ(people.numberofPeopleSchedule()->nameString(), i_people.getString(PeopleFields::NumberofPeopleScheduleName).get());
      EXPECT_EQ(peopleDefinition.numberofPeopleCalculationMethod(), i_people.getString(PeopleFields::NumberofPeopleCalculationMethod).get());
      EXPECT_EQ("People/Area", i_people.getString(PeopleFields::NumberofPeopleCalculationMethod).get());
      EXPECT_TRUE(i_people.isEmpty(PeopleFields::NumberofPeople));
      EXPECT_EQ(peopleDefinition.peopleperSpaceFloorArea().get(), i_people.getDouble(PeopleFields::PeopleperFloorArea).get());
      EXPECT_TRUE(i_people.isEmpty(PeopleFields::FloorAreaperPerson));
      EXPECT_EQ(0.3, i_people.getDouble(PeopleFields::FractionRadiant).get());
      EXPECT_TRUE(i_people.isEmpty(PeopleFields::SensibleHeatFraction));
      EXPECT_EQ(people.activityLevelSchedule()->nameString(), i_people.getString(PeopleFields::ActivityLevelScheduleName).get());
    }
    {
      auto i_eqSpace_ = w.getObjectByTypeAndName(IddObjectType::ElectricEquipment, eqSpace.nameString());
      ASSERT_TRUE(i_eqSpace_);
      auto i_spaceWithDirectElectricEquipment_ = w.getObjectByTypeAndName(IddObjectType::Space, spaceWithDirectElectricEquipment.nameString());
      ASSERT_TRUE(i_spaceWithDirectElectricEquipment_);
      EXPECT_EQ(eqSpace.nameString(), i_eqSpace_->nameString());

      ASSERT_TRUE(i_eqSpace_->getTarget(ElectricEquipmentFields::ZoneorZoneListorSpaceorSpaceListName));
      EXPECT_EQ(spaceWithDirectElectricEquipment.nameString(),
                i_eqSpace_->getString(ElectricEquipmentFields::ZoneorZoneListorSpaceorSpaceListName).get());
      EXPECT_EQ(i_spaceWithDirectElectricEquipment_.get(),
                i_eqSpace_->getTarget(ElectricEquipmentFields::ZoneorZoneListorSpaceorSpaceListName).get());

      EXPECT_EQ(eqSpace.schedule()->nameString(), i_eqSpace_->getString(ElectricEquipmentFields::ScheduleName).get());

      EXPECT_EQ(eqSpaceDefinition.designLevelCalculationMethod(), i_eqSpace_->getString(ElectricEquipmentFields::DesignLevelCalculationMethod).get());
      EXPECT_EQ("EquipmentLevel", i_eqSpace_->getString(ElectricEquipmentFields::DesignLevelCalculationMethod).get());
      EXPECT_EQ(eqSpaceDefinition.designLevel().get(), i_eqSpace_->getDouble(ElectricEquipmentFields::DesignLevel).get());
      EXPECT_TRUE(i_eqSpace_->isEmpty(ElectricEquipmentFields::WattsperFloorArea));
      EXPECT_TRUE(i_eqSpace_->isEmpty(ElectricEquipmentFields::WattsperPerson));
      EXPECT_TRUE(i_eqSpace_->isEmpty(ElectricEquipmentFields::FractionRadiant));
      EXPECT_TRUE(i_eqSpace_->isEmpty(ElectricEquipmentFields::FractionLost));

      EXPECT_EQ(eqSpace.endUseSubcategory(), i_eqSpace_->getString(ElectricEquipmentFields::EndUseSubcategory).get());
    }
    {
      auto i_eqSpaceType_ = w.getObjectByTypeAndName(IddObjectType::ElectricEquipment, eqSpaceType.nameString());
      ASSERT_TRUE(i_eqSpaceType_);
      EXPECT_EQ(eqSpaceType.nameString(), i_eqSpaceType_->nameString());

      ASSERT_TRUE(i_eqSpaceType_->getTarget(ElectricEquipmentFields::ZoneorZoneListorSpaceorSpaceListName));
      EXPECT_EQ(spaceType.nameString(), i_eqSpaceType_->getString(ElectricEquipmentFields::ZoneorZoneListorSpaceorSpaceListName).get());
      EXPECT_EQ(i_spaceList, i_eqSpaceType_->getTarget(ElectricEquipmentFields::ZoneorZoneListorSpaceorSpaceListName).get());

      EXPECT_EQ(eqSpaceType.schedule()->nameString(), i_eqSpaceType_->getString(ElectricEquipmentFields::ScheduleName).get());

      EXPECT_EQ(eqSpaceTypeDefinition.designLevelCalculationMethod(),
                i_eqSpaceType_->getString(ElectricEquipmentFields::DesignLevelCalculationMethod).get());
      EXPECT_EQ("Watts/Area", i_eqSpaceType_->getString(ElectricEquipmentFields::DesignLevelCalculationMethod).get());

      EXPECT_TRUE(i_eqSpaceType_->isEmpty(ElectricEquipmentFields::DesignLevel));
      EXPECT_EQ(eqSpaceTypeDefinition.wattsperSpaceFloorArea().get(), i_eqSpaceType_->getDouble(ElectricEquipmentFields::WattsperFloorArea).get());
      EXPECT_TRUE(i_eqSpaceType_->isEmpty(ElectricEquipmentFields::WattsperPerson));
      EXPECT_TRUE(i_eqSpaceType_->isEmpty(ElectricEquipmentFields::FractionRadiant));
      EXPECT_TRUE(i_eqSpaceType_->isEmpty(ElectricEquipmentFields::FractionLost));

      EXPECT_EQ(eqSpaceType.endUseSubcategory(), i_eqSpaceType_->getString(ElectricEquipmentFields::EndUseSubcategory).get());
    }
  }
}

TEST_F(EnergyPlusFixture, Space_exampleModel_Geometry) {

  //            y (=North)
  //  Site      ▲
  //  Shading   │                  building height = 3m
  //   ║      20├────────┬────────┐
  //   ║        │        │        │
  //   ║        │        │        │
  //   ║        │ Space 3│ Space 4│
  //   ║        │        │        │
  //   ║      10├────────┼────────┤
  //   ║        │        │        │
  //   ║        │        │        ├
  //   ║        │ Space 1│ Space 2│◄─── window + space shading
  //   ║        │        │        ├
  //   ║        └──┬─┬───┴────────┴──────► x
  //           0    ▲    10       20
  //                └─ door+building shading

  Model m = model::exampleModel();

  ASSERT_EQ(1U, m.getConcreteModelObjects<SpaceType>().size());
  SpaceType spaceType = m.getConcreteModelObjects<SpaceType>()[0];

  ASSERT_EQ(1U, m.getConcreteModelObjects<ThermalZone>().size());
  ThermalZone thermalZone = m.getConcreteModelObjects<ThermalZone>()[0];

  auto spaces = m.getConcreteModelObjects<Space>();
  EXPECT_EQ(4U, spaces.size());
  for (const auto& space : m.getConcreteModelObjects<Space>()) {
    ASSERT_TRUE(space.spaceType());
    EXPECT_EQ(spaceType.handle(), space.spaceType()->handle());
    ASSERT_TRUE(space.thermalZone());
    EXPECT_EQ(thermalZone.handle(), space.thermalZone()->handle());
  }

  auto allSurfaces = m.getConcreteModelObjects<Surface>();

  std::vector<Surface> adjacentSurfaces;
  std::copy_if(allSurfaces.cbegin(), allSurfaces.cend(), std::back_inserter(adjacentSurfaces),
               [](const auto& s) { return s.adjacentSurface().has_value(); });
  EXPECT_EQ(8, adjacentSurfaces.size());

  unsigned nWalls = 0;
  unsigned nRoofs = 0;
  unsigned nFloors = 0;
  unsigned nExtWalls = 0;
  unsigned nAdjacentSurfaces = 0;
  for (const auto& surface : allSurfaces) {
    std::string surfaceType = surface.surfaceType();
    if (surface.adjacentSurface()) {
      ++nAdjacentSurfaces;
    }
    if (openstudio::istringEqual("RoofCeiling", surfaceType)) {
      ++nRoofs;
      EXPECT_EQ(10.0 * 10.0, surface.grossArea());
      EXPECT_TRUE(openstudio::istringEqual("Outdoors", surface.outsideBoundaryCondition()));
    } else if (openstudio::istringEqual("Floor", surfaceType)) {
      ++nFloors;
      EXPECT_EQ(10.0 * 10.0, surface.grossArea());
      EXPECT_TRUE(openstudio::istringEqual("Ground", surface.outsideBoundaryCondition()));
    } else if (openstudio::istringEqual("Wall", surfaceType)) {
      EXPECT_EQ(3.0 * 10.0, surface.grossArea());
      ++nWalls;
      if (openstudio::istringEqual("Outdoors", surface.outsideBoundaryCondition())) {
        ++nExtWalls;
      } else {
        ASSERT_TRUE(openstudio::istringEqual("Surface", surface.outsideBoundaryCondition()));
        ASSERT_TRUE(surface.adjacentSurface());
      }
    } else {
      ASSERT_TRUE(false) << "Unknown surfaceType '" << surfaceType << "'.";
    }
  }

  EXPECT_EQ(4 * 6, m.getConcreteModelObjects<Surface>().size());

  EXPECT_EQ(4, nRoofs);
  EXPECT_EQ(4, nFloors);
  EXPECT_EQ(4 * 4, nWalls);
  EXPECT_EQ(8, nAdjacentSurfaces);
  EXPECT_EQ(8, nWalls - nExtWalls);
  EXPECT_EQ(8, nExtWalls);

  // Model has no explicit InternalMass, but it has one explicit InteriorPartitionSurfaces (which maps to InternalMass)
  EXPECT_EQ(0, m.getConcreteModelObjects<InternalMass>().size());
  EXPECT_EQ(1, m.getConcreteModelObjects<InteriorPartitionSurface>().size());

  ForwardTranslator ft;

  {
    ft.setExcludeSpaceTranslation(false);

    // In this case, the surface between the two spaces does exist. Each space is its own enclosure
    Workspace w = ft.translateModel(m);

    ASSERT_EQ(1, w.getObjectsByType(IddObjectType::Zone).size());
    ASSERT_EQ(1, w.getObjectsByType(IddObjectType::SpaceList).size());
    ASSERT_EQ(4, w.getObjectsByType(IddObjectType::Space).size());

    EXPECT_EQ(24, w.getObjectsByType(IddObjectType::BuildingSurface_Detailed).size());
    ASSERT_EQ(1, w.getObjectsByType(IddObjectType::InternalMass).size());  // One explicit
    ASSERT_EQ(10, w.getObjectsByType(IddObjectType::Construction).size());

    struct SurfaceCountForSpace
    {
      int nWalls = 0;
      int nExtWalls = 0;
      int nRoofs = 0;
      int nFloors = 0;
      int nAdjacentSurfaces = 0;
    };

    std::map<std::string, SurfaceCountForSpace> countMap{{"Space 1", SurfaceCountForSpace{}},
                                                         {"Space 2", SurfaceCountForSpace{}},
                                                         {"Space 3", SurfaceCountForSpace{}},
                                                         {"Space 4", SurfaceCountForSpace{}}};

    for (const auto& s : w.getObjectsByType(IddObjectType::BuildingSurface_Detailed)) {
      auto sfName = s.nameString();

      // All in the same ThermalZone
      EXPECT_EQ(thermalZone.nameString(), s.getString(BuildingSurface_DetailedFields::ZoneName).get());

      // The Space Name field is filled out too
      ASSERT_TRUE(s.getString(BuildingSurface_DetailedFields::SpaceName));
      auto spaceName = s.getString(BuildingSurface_DetailedFields::SpaceName).get();

      auto& sfCount = countMap.at(spaceName);

      auto surfaceType = s.getString(BuildingSurface_DetailedFields::SurfaceType).get();
      auto outsideBoundaryCondition = s.getString(BuildingSurface_DetailedFields::OutsideBoundaryCondition).get();

      if (openstudio::istringEqual("Roof", surfaceType)) {
        ++sfCount.nRoofs;
        EXPECT_TRUE(openstudio::istringEqual("Outdoors", outsideBoundaryCondition));
      } else if (openstudio::istringEqual("Floor", surfaceType)) {
        ++sfCount.nFloors;
        EXPECT_TRUE(openstudio::istringEqual("Ground", outsideBoundaryCondition));
      } else if (openstudio::istringEqual("Wall", surfaceType)) {
        ++sfCount.nWalls;
        if (openstudio::istringEqual("Outdoors", outsideBoundaryCondition)) {
          ++sfCount.nExtWalls;
        } else {
          ASSERT_TRUE(openstudio::istringEqual("Surface", outsideBoundaryCondition));
          EXPECT_TRUE(s.getTarget(BuildingSurface_DetailedFields::OutsideBoundaryConditionObject));

          auto it = std::find_if(adjacentSurfaces.cbegin(), adjacentSurfaces.cend(),
                                 [&sfName](const auto& s) { return openstudio::istringEqual(sfName, s.nameString()); });
          EXPECT_NE(adjacentSurfaces.cend(), it);

          ++sfCount.nAdjacentSurfaces;
        }

      } else {
        ASSERT_TRUE(false) << "Unknown surfaceType '" << surfaceType << "'.";
      }
    }

    for (auto& [key, sfCount] : countMap) {
      EXPECT_EQ(1, sfCount.nRoofs) << "Failed for " << key;
      EXPECT_EQ(4, sfCount.nWalls) << "Failed for " << key;
      EXPECT_EQ(2, sfCount.nExtWalls) << "Failed for " << key;
      EXPECT_EQ(1, sfCount.nFloors) << "Failed for " << key;
      EXPECT_EQ(2, sfCount.nAdjacentSurfaces) << "Failed for " << key;
    }
  }
}
