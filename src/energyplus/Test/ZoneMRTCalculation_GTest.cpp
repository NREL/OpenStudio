/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Energy Innovation, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>

#include "EnergyPlusFixture.hpp"

#include "../ForwardTranslator.hpp"
#include "../ReverseTranslator.hpp"

#include "../../model/Model.hpp"
#include "../../model/People.hpp"
#include "../../model/People_Impl.hpp"
#include "../../model/PeopleDefinition.hpp"
#include "../../model/Space.hpp"
#include "../../model/ThermalZone.hpp"
#include "../../model/ThermalZone_Impl.hpp"
#include "../../model/ZoneMRTCalculation.hpp"
#include "../../model/ZoneMRTCalculation_Impl.hpp"

#include "../../utilities/idf/IdfExtensibleGroup.hpp"
#include "../../utilities/idf/WorkspaceExtensibleGroup.hpp"
#include "../../utilities/idf/WorkspaceObject_Impl.hpp"
#include "../../utilities/idf/WorkspaceObject.hpp"

#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/People_FieldEnums.hxx>
#include <utilities/idd/ZoneMRTCalculation_FieldEnums.hxx>

using namespace openstudio::energyplus;
using namespace openstudio::model;
using namespace openstudio;

TEST_F(EnergyPlusFixture, ForwardTranslator_ZoneMRTCalculation) {
  Model model;
  ThermalZone zone(model);
  zone.setName("Thermal Zone 1");

  Space space(model);
  space.setName("Space 1");
  EXPECT_TRUE(space.setThermalZone(zone));

  PeopleDefinition definition(model);
  EXPECT_TRUE(definition.setNumberofPeople(1.0));
  EXPECT_TRUE(definition.setThermalComfortModelType(0, "Fanger"));

  People people1(definition);
  people1.setName("People 1");
  EXPECT_TRUE(people1.setSpace(space));

  People people2(definition);
  people2.setName("People 2");
  EXPECT_TRUE(people2.setSpace(space));

  ZoneMRTCalculation zoneMRTCalculation = zone.getZoneMRTCalculation();
  EXPECT_TRUE(zoneMRTCalculation.addMRTWeightingFactor(people1, 0.25));
  EXPECT_TRUE(zoneMRTCalculation.addMRTWeightingFactor(people2, 0.75));

  ForwardTranslator ft;
  Workspace workspace = ft.translateModel(model);

  std::vector<WorkspaceObject> idfZoneMRTCalculations = workspace.getObjectsByType(IddObjectType::ZoneMRTCalculation);
  ASSERT_EQ(1u, idfZoneMRTCalculations.size());

  WorkspaceObject idfZoneMRTCalculation = idfZoneMRTCalculations[0];
  EXPECT_EQ("Thermal Zone 1", idfZoneMRTCalculation.getString(ZoneMRTCalculationFields::ZoneName).get());
  ASSERT_EQ(2u, idfZoneMRTCalculation.numExtensibleGroups());

  auto group0 = idfZoneMRTCalculation.extensibleGroups()[0].cast<WorkspaceExtensibleGroup>();
  EXPECT_EQ("People 1", group0.getString(ZoneMRTCalculationExtensibleFields::PeopleName).get());
  EXPECT_DOUBLE_EQ(0.25, group0.getDouble(ZoneMRTCalculationExtensibleFields::MRTWeightingFactor).get());

  auto group1 = idfZoneMRTCalculation.extensibleGroups()[1].cast<WorkspaceExtensibleGroup>();
  EXPECT_EQ("People 2", group1.getString(ZoneMRTCalculationExtensibleFields::PeopleName).get());
  EXPECT_DOUBLE_EQ(0.75, group1.getDouble(ZoneMRTCalculationExtensibleFields::MRTWeightingFactor).get());
}

TEST_F(EnergyPlusFixture, ForwardTranslator_ZoneMRTCalculation_Empty) {
  Model model;
  ThermalZone zone(model);
  zone.setName("Thermal Zone 1");
  zone.getZoneMRTCalculation();

  ForwardTranslator ft;
  Workspace workspace = ft.translateModel(model);

  EXPECT_TRUE(workspace.getObjectsByType(IddObjectType::ZoneMRTCalculation).empty());
}

TEST_F(EnergyPlusFixture, ForwardTranslator_ZoneMRTCalculation_StalePeopleReferences) {
  Model model;
  ThermalZone zone(model);
  zone.setName("Thermal Zone 1");

  Space space(model);
  space.setName("Space 1");
  EXPECT_TRUE(space.setThermalZone(zone));

  PeopleDefinition definition1(model);
  EXPECT_TRUE(definition1.setNumberofPeople(1.0));
  EXPECT_TRUE(definition1.setThermalComfortModelType(0, "Fanger"));
  People people1(definition1);
  people1.setName("People 1");
  EXPECT_TRUE(people1.setSpace(space));

  PeopleDefinition definition2(model);
  EXPECT_TRUE(definition2.setNumberofPeople(1.0));
  EXPECT_TRUE(definition2.setThermalComfortModelType(0, "Fanger"));
  People people2(definition2);
  people2.setName("People 2");
  EXPECT_TRUE(people2.setSpace(space));

  ZoneMRTCalculation zoneMRTCalculation = zone.getZoneMRTCalculation();
  EXPECT_TRUE(zoneMRTCalculation.addMRTWeightingFactor(people1, 0.25));
  EXPECT_TRUE(zoneMRTCalculation.addMRTWeightingFactor(people2, 0.50));

  ThermalZone otherZone(model);
  Space otherSpace(model);
  EXPECT_TRUE(otherSpace.setThermalZone(otherZone));
  EXPECT_TRUE(people1.setSpace(otherSpace));
  EXPECT_TRUE(definition2.eraseThermalComfortModelType(0));

  ForwardTranslator ft;
  Workspace workspace = ft.translateModel(model);

  EXPECT_TRUE(workspace.getObjectsByType(IddObjectType::ZoneMRTCalculation).empty());
}

TEST_F(EnergyPlusFixture, ReverseTranslator_ZoneMRTCalculation) {
  Workspace workspace(StrictnessLevel::Minimal, IddFileType::EnergyPlus);

  OptionalWorkspaceObject zone = workspace.addObject(IdfObject(IddObjectType::Zone));
  ASSERT_TRUE(zone);
  zone->setName("Thermal Zone 1");

  OptionalWorkspaceObject people = workspace.addObject(IdfObject(IddObjectType::People));
  ASSERT_TRUE(people);
  people->setName("People 1");
  EXPECT_TRUE(people->setPointer(PeopleFields::ZoneorZoneListorSpaceorSpaceListName, zone->handle()));
  EXPECT_TRUE(people->setString(PeopleFields::NumberofPeopleCalculationMethod, "People"));
  EXPECT_TRUE(people->setDouble(PeopleFields::NumberofPeople, 1.0));
  EXPECT_TRUE(people->setString(PeopleFields::ThermalComfortModel1Type, "Fanger"));

  IdfObject idfZoneMRTCalculation(IddObjectType::ZoneMRTCalculation);
  idfZoneMRTCalculation.setString(ZoneMRTCalculationFields::ZoneName, "Thermal Zone 1");
  IdfExtensibleGroup group = idfZoneMRTCalculation.pushExtensibleGroup();
  group.setString(ZoneMRTCalculationExtensibleFields::PeopleName, "People 1");
  group.setDouble(ZoneMRTCalculationExtensibleFields::MRTWeightingFactor, 0.5);
  OptionalWorkspaceObject epZoneMRTCalculation = workspace.addObject(idfZoneMRTCalculation);
  ASSERT_TRUE(epZoneMRTCalculation);

  ReverseTranslator rt;
  Model model = rt.translateWorkspace(workspace);

  std::vector<ThermalZone> thermalZones = model.getConcreteModelObjects<ThermalZone>();
  ASSERT_EQ(1u, thermalZones.size());
  ThermalZone thermalZone = thermalZones[0];
  EXPECT_EQ("Thermal Zone 1", thermalZone.nameString());

  std::vector<People> modelPeople = model.getConcreteModelObjects<People>();
  ASSERT_EQ(1u, modelPeople.size());
  EXPECT_EQ("People 1", modelPeople[0].nameString());

  ZoneMRTCalculation zoneMRTCalculation = thermalZone.getZoneMRTCalculation();
  EXPECT_EQ(1u, zoneMRTCalculation.numberofMRTWeightingFactors());
  std::vector<MRTWeightingFactor> mrtWeightingFactors = zoneMRTCalculation.mrtWeightingFactors();
  ASSERT_EQ(1u, mrtWeightingFactors.size());
  EXPECT_EQ("People 1", mrtWeightingFactors[0].people().nameString());
  EXPECT_DOUBLE_EQ(0.5, mrtWeightingFactors[0].mrtWeightingFactor());
}

TEST_F(EnergyPlusFixture, ReverseTranslator_ZoneMRTCalculation_InvalidMRTWeightingFactor) {
  Workspace workspace(StrictnessLevel::Minimal, IddFileType::EnergyPlus);

  OptionalWorkspaceObject zone = workspace.addObject(IdfObject(IddObjectType::Zone));
  ASSERT_TRUE(zone);
  zone->setName("Thermal Zone 1");

  OptionalWorkspaceObject people = workspace.addObject(IdfObject(IddObjectType::People));
  ASSERT_TRUE(people);
  people->setName("People 1");
  EXPECT_TRUE(people->setPointer(PeopleFields::ZoneorZoneListorSpaceorSpaceListName, zone->handle()));
  EXPECT_TRUE(people->setString(PeopleFields::NumberofPeopleCalculationMethod, "People"));
  EXPECT_TRUE(people->setDouble(PeopleFields::NumberofPeople, 1.0));
  EXPECT_TRUE(people->setString(PeopleFields::ThermalComfortModel1Type, "Fanger"));

  IdfObject idfZoneMRTCalculation(IddObjectType::ZoneMRTCalculation);
  idfZoneMRTCalculation.setString(ZoneMRTCalculationFields::ZoneName, "Thermal Zone 1");
  IdfExtensibleGroup group = idfZoneMRTCalculation.pushExtensibleGroup();
  group.setString(ZoneMRTCalculationExtensibleFields::PeopleName, "People 1");
  group.setDouble(ZoneMRTCalculationExtensibleFields::MRTWeightingFactor, 1.1);
  OptionalWorkspaceObject epZoneMRTCalculation = workspace.addObject(idfZoneMRTCalculation);
  ASSERT_TRUE(epZoneMRTCalculation);

  ReverseTranslator rt;
  Model model;
  EXPECT_NO_THROW(model = rt.translateWorkspace(workspace));

  std::vector<ThermalZone> thermalZones = model.getConcreteModelObjects<ThermalZone>();
  ASSERT_EQ(1u, thermalZones.size());
  std::vector<ZoneMRTCalculation> zoneMRTCalculations = model.getConcreteModelObjects<ZoneMRTCalculation>();
  ASSERT_EQ(1u, zoneMRTCalculations.size());
  ZoneMRTCalculation zoneMRTCalculation = zoneMRTCalculations[0];
  EXPECT_EQ(thermalZones[0].handle(), zoneMRTCalculation.thermalZone().handle());
  EXPECT_EQ(0u, zoneMRTCalculation.numberofMRTWeightingFactors());
}
