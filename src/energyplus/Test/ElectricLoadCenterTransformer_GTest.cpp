/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>
#include "EnergyPlusFixture.hpp"

#include "../ErrorFile.hpp"
#include "../ForwardTranslator.hpp"

#include "../../model/Model.hpp"
#include "../../model/Model_Impl.hpp"
#include "../../model/Building.hpp"
#include "../../model/Building_Impl.hpp"
#include "../../model/Schedule.hpp"
#include "../../model/Schedule_Impl.hpp"
#include "../../model/OutputMeter.hpp"
#include "../../model/OutputMeter_Impl.hpp"
#include "../../model/ThermalZone.hpp"
#include "../../model/Space.hpp"
#include "../../model/ElectricLoadCenterTransformer.hpp"
#include "../../model/ElectricLoadCenterTransformer_Impl.hpp"
#include "../../model/ElectricLoadCenterDistribution.hpp"
#include "../../model/ElectricLoadCenterDistribution_Impl.hpp"

#include "../../model/Version.hpp"
#include "../../model/Version_Impl.hpp"

#include "../../utilities/core/Optional.hpp"
#include "../../utilities/core/Checksum.hpp"
#include "../../utilities/core/UUID.hpp"
#include "../../utilities/core/Logger.hpp"
#include "../../utilities/sql/SqlFile.hpp"
#include "../../utilities/idf/IdfFile.hpp"
#include "../../utilities/idf/IdfObject.hpp"
#include "../../utilities/idf/IdfExtensibleGroup.hpp"

#include <utilities/idd/OS_ElectricLoadCenter_Transformer_FieldEnums.hxx>
#include <utilities/idd/ElectricLoadCenter_Transformer_FieldEnums.hxx>
#include <utilities/idd/ElectricLoadCenter_Distribution_FieldEnums.hxx>

#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/IddFactory.hxx>

#include <boost/algorithm/string/predicate.hpp>

#include <resources.hxx>

#include <sstream>

#include <vector>

using namespace openstudio::energyplus;
using namespace openstudio::model;
using namespace openstudio;

TEST_F(EnergyPlusFixture, ForwardTranslatorElectricLoadCenterTransformer) {

  Model model;

  Building building = model.getUniqueModelObject<Building>();

  ThermalZone zone1(model);

  ElectricLoadCenterTransformer elct(model);

  ForwardTranslator forwardTranslator;
  Workspace workspace = forwardTranslator.translateModel(model);

  EXPECT_EQ(0u, forwardTranslator.errors().size());
  EXPECT_EQ(1u, workspace.getObjectsByType(IddObjectType::ElectricLoadCenter_Transformer).size());
  EXPECT_EQ(1u, workspace.getObjectsByType(IddObjectType::Schedule_Compact).size());

  WorkspaceObject object = workspace.getObjectsByType(IddObjectType::ElectricLoadCenter_Transformer)[0];
  WorkspaceObject schedule = workspace.getObjectsByType(IddObjectType::Schedule_Compact)[0];

  EXPECT_EQ(schedule.nameString(), object.getString(ElectricLoadCenter_TransformerFields::AvailabilityScheduleName, false).get());
  EXPECT_EQ("", object.getString(ElectricLoadCenter_TransformerFields::ZoneName, false).get());
  EXPECT_EQ("0", object.getString(ElectricLoadCenter_TransformerFields::RadiativeFraction, false).get());
  EXPECT_EQ("15000", object.getString(ElectricLoadCenter_TransformerFields::RatedCapacity, false).get());
  EXPECT_EQ("3", object.getString(ElectricLoadCenter_TransformerFields::Phase, false).get());
  EXPECT_EQ("Aluminum", object.getString(ElectricLoadCenter_TransformerFields::ConductorMaterial, false).get());
  EXPECT_EQ("150", object.getString(ElectricLoadCenter_TransformerFields::FullLoadTemperatureRise, false).get());
  EXPECT_EQ("0.1", object.getString(ElectricLoadCenter_TransformerFields::FractionofEddyCurrentLosses, false).get());
  EXPECT_EQ("NominalEfficiency", object.getString(ElectricLoadCenter_TransformerFields::PerformanceInputMethod, false).get());
  EXPECT_EQ("", object.getString(ElectricLoadCenter_TransformerFields::RatedNoLoadLoss, false).get());
  EXPECT_EQ("", object.getString(ElectricLoadCenter_TransformerFields::RatedLoadLoss, false).get());
  EXPECT_EQ("0.98", object.getString(ElectricLoadCenter_TransformerFields::NameplateEfficiency, false).get());
  EXPECT_EQ("0.35", object.getString(ElectricLoadCenter_TransformerFields::PerUnitLoadforNameplateEfficiency, false).get());
  EXPECT_EQ("75", object.getString(ElectricLoadCenter_TransformerFields::ReferenceTemperatureforNameplateEfficiency, false).get());
  EXPECT_EQ("", object.getString(ElectricLoadCenter_TransformerFields::PerUnitLoadforMaximumEfficiency, false).get());
  EXPECT_EQ("Yes", object.getString(ElectricLoadCenter_TransformerFields::ConsiderTransformerLossforUtilityCost, false).get());
  std::vector<IdfExtensibleGroup> extensibleGroups = object.extensibleGroups();
  EXPECT_EQ(0u, extensibleGroups.size());

  // model.save(toPath("./ElectricLoadCenterTransformer.osm"), true);
  // workspace.save(toPath("./ElectricLoadCenterTransformer.idf"), true);
}

TEST_F(EnergyPlusFixture, ForwardTranslatorElectricLoadCenterTransformer2) {

  Model model;

  Building building = model.getUniqueModelObject<Building>();

  ThermalZone zone1(model);
  Space space(model);
  space.setThermalZone(zone1);

  //add schedule
  Schedule s = model.alwaysOffDiscreteSchedule();
  ElectricLoadCenterTransformer elct(model);

  elct.setAvailabilitySchedule(s);
  elct.setZone(zone1);

  ForwardTranslator forwardTranslator;
  Workspace workspace = forwardTranslator.translateModel(model);

  WorkspaceObject object = workspace.getObjectsByType(IddObjectType::ElectricLoadCenter_Transformer)[0];

  EXPECT_EQ(s.nameString(), object.getString(ElectricLoadCenter_TransformerFields::AvailabilityScheduleName, false).get());
  EXPECT_EQ(zone1.nameString(), object.getString(ElectricLoadCenter_TransformerFields::ZoneName, false).get());

  // model.save(toPath("./ElectricLoadCenterTransformer2.osm"), true);
  // workspace.save(toPath("./ElectricLoadCenterTransformer2.idf"), true);
}

TEST_F(EnergyPlusFixture, ForwardTranslatorElectricLoadCenterTransformer3) {

  Model model;

  Building building = model.getUniqueModelObject<Building>();

  ThermalZone zone1(model);

  // add Meter
  OutputMeter meter(model);
  meter.setName("Electricity:Building");
  OutputMeter meter2(model);
  meter2.setName("Electricity:HVAC");
  OutputMeter meter3(model);
  meter3.setName("Electricity:Fans");

  ElectricLoadCenterTransformer elct(model);

  elct.addMeter(meter.nameString());
  elct.addMeter(meter2.nameString());
  elct.addMeter(meter3.nameString());

  ForwardTranslator forwardTranslator;
  Workspace workspace = forwardTranslator.translateModel(model);

  WorkspaceObject object = workspace.getObjectsByType(IddObjectType::ElectricLoadCenter_Transformer)[0];

  std::vector<IdfExtensibleGroup> extensibleGroups = object.extensibleGroups();
  EXPECT_EQ(3u, extensibleGroups.size());

  // model.save(toPath("./ElectricLoadCenterTransformer2.osm"), true);
  // workspace.save(toPath("./ElectricLoadCenterTransformer2.idf"), true);
}

TEST_F(EnergyPlusFixture, ForwardTranslatorElectricLoadCenterTransformer_Distribution) {

  Model model;

  Building building = model.getUniqueModelObject<Building>();

  ThermalZone zone1(model);

  ElectricLoadCenterTransformer elct(model);
  ElectricLoadCenterDistribution elcd(model);

  elcd.setTransformer(elct);

  // Need to set buss type accordingly, or it won't be translated
  elcd.setElectricalBussType("AlternatingCurrent");

  ForwardTranslator forwardTranslator;
  Workspace workspace = forwardTranslator.translateModel(model);

  // model.save(toPath("./ElectricLoadCenterDistribution.osm"), true);
  // workspace.save(toPath("./ElectricLoadCenterDistribution.idf"), true);

  ASSERT_EQ(1u, workspace.getObjectsByType(IddObjectType::ElectricLoadCenter_Transformer).size());
  ASSERT_EQ(1u, workspace.getObjectsByType(IddObjectType::ElectricLoadCenter_Distribution).size());
  WorkspaceObject transformer = workspace.getObjectsByType(IddObjectType::ElectricLoadCenter_Transformer)[0];
  WorkspaceObject distribution = workspace.getObjectsByType(IddObjectType::ElectricLoadCenter_Distribution)[0];

  EXPECT_EQ(transformer.nameString(), distribution.getString(ElectricLoadCenter_DistributionFields::TransformerObjectName, false).get());

  // model.save(toPath("./ElectricLoadCenterDistribution.osm"), true);
  // workspace.save(toPath("./ElectricLoadCenterDistribution.idf"), true);
}
