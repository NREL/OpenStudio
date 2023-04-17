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
