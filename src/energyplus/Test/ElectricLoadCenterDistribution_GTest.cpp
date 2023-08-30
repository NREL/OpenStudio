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

#include "../../model/ElectricLoadCenterDistribution.hpp"
#include "../../model/ElectricLoadCenterDistribution_Impl.hpp"
#include "../../model/GeneratorPVWatts.hpp"
#include "../../model/GeneratorPVWatts_Impl.hpp"
#include "../../model/ElectricLoadCenterInverterPVWatts.hpp"
#include "../../model/ElectricLoadCenterInverterPVWatts_Impl.hpp"
#include "../../model/ElectricLoadCenterStorageLiIonNMCBattery.hpp"
#include "../../model/ElectricLoadCenterStorageLiIonNMCBattery_Impl.hpp"

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
#include "../../utilities/idf/WorkspaceExtensibleGroup.hpp"

#include <utilities/idd/ElectricLoadCenter_Distribution_FieldEnums.hxx>
#include <utilities/idd/ElectricLoadCenter_Generators_FieldEnums.hxx>
#include <utilities/idd/Generator_PVWatts_FieldEnums.hxx>
#include <utilities/idd/ElectricLoadCenter_Inverter_PVWatts_FieldEnums.hxx>

#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/IddFactory.hxx>

#include <boost/algorithm/string/predicate.hpp>

#include <resources.hxx>

#include <sstream>

#include <vector>

using namespace openstudio::energyplus;
using namespace openstudio::model;
using namespace openstudio;

TEST_F(EnergyPlusFixture, ForwardTranslator_ElectricLoadCenterDistribution_NoInverter) {
  Model model;

  ElectricLoadCenterDistribution elcd(model);

  EXPECT_EQ(1u, model.getObjectsByType(ElectricLoadCenterDistribution::iddObjectType()).size());

  GeneratorPVWatts gntr(model, 1);
  elcd.addGenerator(gntr);

  // This is pretty dumb in this context of a PV gen, but I just want to avoid the FT not translating ELCD
  // Object of type 'OS:ElectricLoadCenter:Distribution' and named 'Electric Load Center Distribution 1':
  // Your Electric Buss Type 'DirectCurrentWithInverter' Requires an inverter but you didn't specify one
  elcd.setElectricalBussType("AlternatingCurrent");

  EXPECT_EQ(1u, model.getObjectsByType(ElectricLoadCenterDistribution::iddObjectType()).size());

  ForwardTranslator forwardTranslator;
  Workspace workspace = forwardTranslator.translateModel(model);

  ASSERT_EQ(1u, workspace.getObjectsByType(IddObjectType::ElectricLoadCenter_Distribution).size());
  ASSERT_EQ(1u, workspace.getObjectsByType(IddObjectType::ElectricLoadCenter_Generators).size());
  ASSERT_EQ(1u, workspace.getObjectsByType(IddObjectType::Generator_PVWatts).size());
  EXPECT_EQ(0u, workspace.getObjectsByType(IddObjectType::ElectricLoadCenter_Inverter_PVWatts).size());

  WorkspaceObject distribution = workspace.getObjectsByType(IddObjectType::ElectricLoadCenter_Distribution)[0];
  WorkspaceObject generators = workspace.getObjectsByType(IddObjectType::ElectricLoadCenter_Generators)[0];
  WorkspaceObject generator = workspace.getObjectsByType(IddObjectType::Generator_PVWatts)[0];

  EXPECT_EQ(generators.nameString(), distribution.getString(ElectricLoadCenter_DistributionFields::GeneratorListName, false).get());

  ASSERT_EQ(1u, generators.extensibleGroups().size());
  auto w_eg = generators.extensibleGroups()[0].cast<WorkspaceExtensibleGroup>();
  EXPECT_EQ(generator.nameString(), w_eg.getString(ElectricLoadCenter_GeneratorsExtensibleFields::GeneratorName, false).get());
}

TEST_F(EnergyPlusFixture, ForwardTranslator_ElectricLoadCenterDistribution_NoGenerators) {
  Model model;

  ElectricLoadCenterDistribution elcd(model);
  elcd.setElectricalBussType("DirectCurrentWithInverter");

  EXPECT_EQ(1u, model.getObjectsByType(ElectricLoadCenterDistribution::iddObjectType()).size());

  ElectricLoadCenterInverterPVWatts invtr(model);
  elcd.setInverter(invtr);

  EXPECT_EQ(1u, model.getObjectsByType(ElectricLoadCenterDistribution::iddObjectType()).size());

  ForwardTranslator forwardTranslator;
  Workspace workspace = forwardTranslator.translateModel(model);

  ASSERT_EQ(1u, workspace.getObjectsByType(IddObjectType::ElectricLoadCenter_Distribution).size());
  EXPECT_EQ(0u, workspace.getObjectsByType(IddObjectType::ElectricLoadCenter_Generators).size());
  EXPECT_EQ(0u, workspace.getObjectsByType(IddObjectType::Generator_PVWatts).size());
  ASSERT_EQ(1u, workspace.getObjectsByType(IddObjectType::ElectricLoadCenter_Inverter_PVWatts).size());

  WorkspaceObject distribution = workspace.getObjectsByType(IddObjectType::ElectricLoadCenter_Distribution)[0];
  WorkspaceObject inverter = workspace.getObjectsByType(IddObjectType::ElectricLoadCenter_Inverter_PVWatts)[0];

  EXPECT_EQ(inverter.nameString(), distribution.getString(ElectricLoadCenter_DistributionFields::InverterName, false).get());
}

TEST_F(EnergyPlusFixture, ForwardTranslator_ElectricLoadCenterDistribution_TrackFacilityElectricDemandStoreExcessOnSite) {
  // Test for #4495: ElectricLoadCenterDistribution FT has incomplete charge/discharge logic

  Model model;

  ElectricLoadCenterDistribution elcd(model);
  elcd.setStorageOperationScheme("TrackFacilityElectricDemandStoreExcessOnSite");
  elcd.setDesignStorageControlChargePower(10000);
  elcd.setDesignStorageControlDischargePower(15000);

  ElectricLoadCenterStorageLiIonNMCBattery elcs(model);
  elcd.setElectricalBussType("AlternatingCurrentWithStorage");
  elcd.setElectricalStorage(elcs);

  ForwardTranslator forwardTranslator;
  Workspace workspace = forwardTranslator.translateModel(model);

  ASSERT_EQ(1u, workspace.getObjectsByType(IddObjectType::ElectricLoadCenter_Storage_LiIonNMCBattery).size());

  WorkspaceObjectVector idf_elcds(workspace.getObjectsByType(IddObjectType::ElectricLoadCenter_Distribution));
  EXPECT_EQ(1u, idf_elcds.size());
  WorkspaceObject idf_elcd(idf_elcds[0]);

  ASSERT_TRUE(idf_elcd.getDouble(ElectricLoadCenter_DistributionFields::DesignStorageControlChargePower, false));
  ASSERT_TRUE(idf_elcd.getDouble(ElectricLoadCenter_DistributionFields::DesignStorageControlDischargePower, false).get());

  EXPECT_EQ(10000, idf_elcd.getDouble(ElectricLoadCenter_DistributionFields::DesignStorageControlChargePower, false).get());
  EXPECT_EQ(15000, idf_elcd.getDouble(ElectricLoadCenter_DistributionFields::DesignStorageControlDischargePower, false).get());
}
