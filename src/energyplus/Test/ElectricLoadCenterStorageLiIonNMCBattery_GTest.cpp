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

#include "../../model/Model.hpp"
#include "../../model/ElectricLoadCenterStorageLiIonNMCBattery.hpp"
#include "../../model/ElectricLoadCenterStorageLiIonNMCBattery_Impl.hpp"

#include "../../model/ElectricLoadCenterDistribution.hpp"
#include "../../model/ElectricLoadCenterDistribution_Impl.hpp"

#include <utilities/idd/ElectricLoadCenter_Storage_LiIonNMCBattery_FieldEnums.hxx>
#include <utilities/idd/ElectricLoadCenter_Distribution_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include <boost/algorithm/string/predicate.hpp>
#include <resources.hxx>
#include <sstream>
#include <vector>

// Debug
#include "../../utilities/core/Logger.hpp"

using namespace openstudio::energyplus;
using namespace openstudio::model;
using namespace openstudio;

TEST_F(EnergyPlusFixture, ForwardTranslator_ElectricLoadCenterStorageLiIonNMCBattery) {
  Model m;

  ElectricLoadCenterDistribution elcd(m);

  const int nSeries = 139;
  const int nParallel = 25;
  const double mass = 342.0;
  const double surfaceArea = 4.26;

  ElectricLoadCenterStorageLiIonNMCBattery battery(m, nSeries, nParallel, mass, surfaceArea);

  elcd.setElectricalBussType("AlternatingCurrentWithStorage");
  elcd.setElectricalStorage(battery);

  ForwardTranslator ft;
  Workspace w = ft.translateModel(m);

  EXPECT_EQ(1u, w.getObjectsByType(IddObjectType::ElectricLoadCenter_Distribution).size());

  WorkspaceObjectVector idf_batteries(w.getObjectsByType(IddObjectType::ElectricLoadCenter_Storage_LiIonNMCBattery));
  EXPECT_EQ(1u, idf_batteries.size());
  WorkspaceObject idf_battery(idf_batteries[0]);

  EXPECT_EQ("Always On Discrete", idf_battery.getString(ElectricLoadCenter_Storage_LiIonNMCBatteryFields::AvailabilityScheduleName, false).get());
  EXPECT_EQ("", idf_battery.getString(ElectricLoadCenter_Storage_LiIonNMCBatteryFields::ZoneName, false).get());
  EXPECT_EQ(0, idf_battery.getDouble(ElectricLoadCenter_Storage_LiIonNMCBatteryFields::RadiativeFraction, false).get());
  EXPECT_EQ("KandlerSmith", idf_battery.getString(ElectricLoadCenter_Storage_LiIonNMCBatteryFields::LifetimeModel, false).get());
  EXPECT_EQ(nSeries, idf_battery.getInt(ElectricLoadCenter_Storage_LiIonNMCBatteryFields::NumberofCellsinSeries, false).get());
  EXPECT_EQ(nParallel, idf_battery.getInt(ElectricLoadCenter_Storage_LiIonNMCBatteryFields::NumberofStringsinParallel, false).get());
  EXPECT_EQ(0.5, idf_battery.getDouble(ElectricLoadCenter_Storage_LiIonNMCBatteryFields::InitialFractionalStateofCharge, false).get());
  EXPECT_EQ(0.95, idf_battery.getDouble(ElectricLoadCenter_Storage_LiIonNMCBatteryFields::DCtoDCChargingEfficiency, false).get());
  EXPECT_EQ(mass, idf_battery.getDouble(ElectricLoadCenter_Storage_LiIonNMCBatteryFields::BatteryMass, false).get());
  EXPECT_EQ(surfaceArea, idf_battery.getDouble(ElectricLoadCenter_Storage_LiIonNMCBatteryFields::BatterySurfaceArea, false).get());
  EXPECT_EQ(1500, idf_battery.getDouble(ElectricLoadCenter_Storage_LiIonNMCBatteryFields::BatterySpecificHeatCapacity, false).get());
  EXPECT_EQ(7.5,
            idf_battery.getDouble(ElectricLoadCenter_Storage_LiIonNMCBatteryFields::HeatTransferCoefficientBetweenBatteryandAmbient, false).get());
  EXPECT_EQ(4.2, idf_battery.getDouble(ElectricLoadCenter_Storage_LiIonNMCBatteryFields::FullyChargedCellVoltage, false).get());
  EXPECT_EQ(3.53, idf_battery.getDouble(ElectricLoadCenter_Storage_LiIonNMCBatteryFields::CellVoltageatEndofExponentialZone, false).get());
  EXPECT_EQ(3.342, idf_battery.getDouble(ElectricLoadCenter_Storage_LiIonNMCBatteryFields::CellVoltageatEndofNominalZone, false).get());
  EXPECT_EQ(3.342, idf_battery.getDouble(ElectricLoadCenter_Storage_LiIonNMCBatteryFields::DefaultNominalCellVoltage, false).get());
  EXPECT_EQ(3.2, idf_battery.getDouble(ElectricLoadCenter_Storage_LiIonNMCBatteryFields::FullyChargedCellCapacity, false).get());
  EXPECT_EQ(
    0.8075,
    idf_battery.getDouble(ElectricLoadCenter_Storage_LiIonNMCBatteryFields::FractionofCellCapacityRemovedattheEndofExponentialZone, false).get());
  EXPECT_EQ(0.976875,
            idf_battery.getDouble(ElectricLoadCenter_Storage_LiIonNMCBatteryFields::FractionofCellCapacityRemovedattheEndofNominalZone, false).get());
  EXPECT_EQ(
    1, idf_battery.getDouble(ElectricLoadCenter_Storage_LiIonNMCBatteryFields::ChargeRateatWhichVoltagevsCapacityCurveWasGenerated, false).get());
  EXPECT_EQ(0.09, idf_battery.getDouble(ElectricLoadCenter_Storage_LiIonNMCBatteryFields::BatteryCellInternalElectricalResistance, false).get());
}

TEST_F(EnergyPlusFixture, ReverseTranslator_ElectricLoadCenterStorageLiIonNMCBattery) {
  openstudio::Workspace workspace(openstudio::StrictnessLevel::Minimal, openstudio::IddFileType::EnergyPlus);

  // electric load center storage li ion nmc battery
  openstudio::IdfObject idfObject1(openstudio::IddObjectType::ElectricLoadCenter_Storage_LiIonNMCBattery);
  idfObject1.setString(ElectricLoadCenter_Storage_LiIonNMCBatteryFields::Name, "Electric Load Center Storage Li Ion NMC Battery 1");
  idfObject1.setInt(ElectricLoadCenter_Storage_LiIonNMCBatteryFields::NumberofCellsinSeries, 139);
  idfObject1.setInt(ElectricLoadCenter_Storage_LiIonNMCBatteryFields::NumberofStringsinParallel, 25);
  idfObject1.setDouble(ElectricLoadCenter_Storage_LiIonNMCBatteryFields::BatteryMass, 342.0);
  idfObject1.setDouble(ElectricLoadCenter_Storage_LiIonNMCBatteryFields::BatterySurfaceArea, 4.26);

  openstudio::WorkspaceObject epGenerator = workspace.addObject(idfObject1).get();

  ReverseTranslator trans;
  ASSERT_NO_THROW(trans.translateWorkspace(workspace));
  Model model = trans.translateWorkspace(workspace);

  std::vector<ElectricLoadCenterStorageLiIonNMCBattery> batteries = model.getConcreteModelObjects<ElectricLoadCenterStorageLiIonNMCBattery>();
  ASSERT_EQ(1u, batteries.size());
  ElectricLoadCenterStorageLiIonNMCBattery battery = batteries[0];
  EXPECT_EQ("Electric Load Center Storage Li Ion NMC Battery 1", battery.name().get());
  EXPECT_EQ(139, battery.numberofCellsinSeries());
  EXPECT_EQ(25, battery.numberofStringsinParallel());
  EXPECT_DOUBLE_EQ(342.0, battery.batteryMass());
  EXPECT_DOUBLE_EQ(4.26, battery.batterySurfaceArea());
}
