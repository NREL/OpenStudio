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

#include "../../model/ZoneHVACUnitHeater.hpp"
#include "../../model/CoilHeatingElectric.hpp"
#include "../../model/CoilHeatingGas.hpp"
#include "../../model/CoilHeatingWater.hpp"
#include "../../model/FanConstantVolume.hpp"
#include "../../model/FanOnOff.hpp"
#include "../../model/FanSystemModel.hpp"
#include "../../model/FanVariableVolume.hpp"

#include "../../model/ThermalZone.hpp"
#include "../../model/Space.hpp"
#include "../../model/Schedule.hpp"
#include "../../model/PlantLoop.hpp"

#include <utilities/idd/IddObject.hpp>
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/ZoneHVAC_UnitHeater_FieldEnums.hxx>
#include <utilities/idd/Fan_OnOff_FieldEnums.hxx>
#include <utilities/idd/Fan_ConstantVolume_FieldEnums.hxx>
#include <utilities/idd/Fan_VariableVolume_FieldEnums.hxx>
#include <utilities/idd/Fan_SystemModel_FieldEnums.hxx>
#include <utilities/idd/Coil_Heating_Water_FieldEnums.hxx>
#include <utilities/idd/Coil_Heating_Fuel_FieldEnums.hxx>
#include <utilities/idd/Coil_Heating_Electric_FieldEnums.hxx>

#include "../../utilities/idf/IdfExtensibleGroup.hpp"

#include <resources.hxx>

#include <sstream>

using namespace openstudio::energyplus;
using namespace openstudio::model;
using namespace openstudio;

TEST_F(EnergyPlusFixture, ForwardTranslator_ZoneHVACUnitHeater_FanConstantVolume_CoilElectric) {

  Model m;

  Schedule alwaysOn = m.alwaysOnDiscreteSchedule();
  FanConstantVolume fan(m);
  CoilHeatingElectric hc(m);

  ZoneHVACUnitHeater uh(m, alwaysOn, fan, hc);

  EXPECT_EQ(alwaysOn, uh.availabilitySchedule());
  EXPECT_EQ(fan, uh.supplyAirFan());
  EXPECT_EQ(hc, uh.heatingCoil());

  EXPECT_TRUE(uh.setName("Zone HVAC Unit Heater"));
  EXPECT_TRUE(uh.setMaximumSupplyAirFlowRate(1.0));
  EXPECT_TRUE(uh.setFanControlType("OnOff"));

  EXPECT_TRUE(uh.setMaximumHotWaterFlowRate(0.05));
  EXPECT_TRUE(uh.setMinimumHotWaterFlowRate(0.0));
  EXPECT_TRUE(uh.setHeatingConvergenceTolerance(0.001));

  // Need to be in a thermal zone to be translated, with at least one space
  ThermalZone z(m);
  uh.addToThermalZone(z);
  Space s(m);
  s.setThermalZone(z);

  ForwardTranslator ft;
  Workspace workspace = ft.translateModel(m);

  WorkspaceObjectVector idfObjs(workspace.getObjectsByType(IddObjectType::ZoneHVAC_UnitHeater));
  ASSERT_EQ(1, idfObjs.size());
  WorkspaceObject idf_uh = idfObjs[0];

  EXPECT_EQ(uh.nameString(), idf_uh.getString(ZoneHVAC_UnitHeaterFields::Name).get());
  EXPECT_EQ(alwaysOn.nameString(), idf_uh.getString(ZoneHVAC_UnitHeaterFields::AvailabilityScheduleName).get());
  EXPECT_EQ(uh.airInletModelObject()->nameString(), idf_uh.getString(ZoneHVAC_UnitHeaterFields::AirInletNodeName).get());
  EXPECT_EQ(uh.airOutletModelObject()->nameString(), idf_uh.getString(ZoneHVAC_UnitHeaterFields::AirOutletNodeName).get());
  EXPECT_EQ(1.0, idf_uh.getDouble(ZoneHVAC_UnitHeaterFields::MaximumSupplyAirFlowRate).get());
  EXPECT_EQ(0.05, idf_uh.getDouble(ZoneHVAC_UnitHeaterFields::MaximumHotWaterorSteamFlowRate).get());
  EXPECT_EQ(0.0, idf_uh.getDouble(ZoneHVAC_UnitHeaterFields::MinimumHotWaterorSteamFlowRate).get());
  EXPECT_EQ(0.001, idf_uh.getDouble(ZoneHVAC_UnitHeaterFields::HeatingConvergenceTolerance).get());
  EXPECT_TRUE(idf_uh.isEmpty(ZoneHVAC_UnitHeaterFields::AvailabilityManagerListName));

  EXPECT_EQ(fan.nameString(), idf_uh.getString(ZoneHVAC_UnitHeaterFields::SupplyAirFanName).get());
  EXPECT_EQ("Fan:ConstantVolume", idf_uh.getString(ZoneHVAC_UnitHeaterFields::SupplyAirFanObjectType).get());
  auto idf_fan = idf_uh.getTarget(ZoneHVAC_UnitHeaterFields::SupplyAirFanName).get();
  EXPECT_EQ(idf_fan.iddObject().type(), IddObjectType::Fan_ConstantVolume);
  EXPECT_EQ(uh.airInletModelObject()->nameString(), idf_fan.getString(Fan_ConstantVolumeFields::AirInletNodeName).get());
  EXPECT_EQ(uh.nameString() + " Fan Outlet Node", idf_fan.getString(Fan_ConstantVolumeFields::AirOutletNodeName).get());

  EXPECT_EQ(hc.nameString(), idf_uh.getString(ZoneHVAC_UnitHeaterFields::HeatingCoilName).get());
  EXPECT_EQ("Coil:Heating:Electric", idf_uh.getString(ZoneHVAC_UnitHeaterFields::HeatingCoilObjectType).get());
  auto idf_hc = idf_uh.getTarget(ZoneHVAC_UnitHeaterFields::HeatingCoilName).get();
  EXPECT_EQ(idf_hc.iddObject().type(), IddObjectType::Coil_Heating_Electric);

  EXPECT_EQ(uh.nameString() + " Fan Outlet Node", idf_hc.getString(Coil_Heating_ElectricFields::AirInletNodeName).get());
  EXPECT_EQ(idf_fan.getString(Fan_ConstantVolumeFields::AirOutletNodeName).get(),
            idf_hc.getString(Coil_Heating_ElectricFields::AirInletNodeName).get());

  EXPECT_EQ(uh.airOutletModelObject()->nameString(), idf_hc.getString(Coil_Heating_ElectricFields::AirOutletNodeName).get());
}

TEST_F(EnergyPlusFixture, ForwardTranslator_ZoneHVACUnitHeater_FanOnOff_CoilFuel) {

  Model m;

  Schedule alwaysOn = m.alwaysOnDiscreteSchedule();
  FanOnOff fan(m);
  CoilHeatingGas hc(m);

  ZoneHVACUnitHeater uh(m, alwaysOn, fan, hc);

  EXPECT_EQ(alwaysOn, uh.availabilitySchedule());
  EXPECT_EQ(fan, uh.supplyAirFan());
  EXPECT_EQ(hc, uh.heatingCoil());

  EXPECT_TRUE(uh.setName("Zone HVAC Unit Heater"));
  EXPECT_TRUE(uh.setMaximumSupplyAirFlowRate(1.0));
  EXPECT_TRUE(uh.setFanControlType("OnOff"));

  EXPECT_TRUE(uh.setMaximumHotWaterFlowRate(0.05));
  EXPECT_TRUE(uh.setMinimumHotWaterFlowRate(0.0));
  EXPECT_TRUE(uh.setHeatingConvergenceTolerance(0.001));

  // Need to be in a thermal zone to be translated, with at least one space
  ThermalZone z(m);
  uh.addToThermalZone(z);
  Space s(m);
  s.setThermalZone(z);

  ForwardTranslator ft;
  Workspace workspace = ft.translateModel(m);

  WorkspaceObjectVector idfObjs(workspace.getObjectsByType(IddObjectType::ZoneHVAC_UnitHeater));
  ASSERT_EQ(1, idfObjs.size());
  WorkspaceObject idf_uh = idfObjs[0];

  EXPECT_EQ(uh.nameString(), idf_uh.getString(ZoneHVAC_UnitHeaterFields::Name).get());
  EXPECT_EQ(alwaysOn.nameString(), idf_uh.getString(ZoneHVAC_UnitHeaterFields::AvailabilityScheduleName).get());
  EXPECT_EQ(uh.airInletModelObject()->nameString(), idf_uh.getString(ZoneHVAC_UnitHeaterFields::AirInletNodeName).get());
  EXPECT_EQ(uh.airOutletModelObject()->nameString(), idf_uh.getString(ZoneHVAC_UnitHeaterFields::AirOutletNodeName).get());
  EXPECT_EQ(1.0, idf_uh.getDouble(ZoneHVAC_UnitHeaterFields::MaximumSupplyAirFlowRate).get());
  EXPECT_EQ(0.05, idf_uh.getDouble(ZoneHVAC_UnitHeaterFields::MaximumHotWaterorSteamFlowRate).get());
  EXPECT_EQ(0.0, idf_uh.getDouble(ZoneHVAC_UnitHeaterFields::MinimumHotWaterorSteamFlowRate).get());
  EXPECT_EQ(0.001, idf_uh.getDouble(ZoneHVAC_UnitHeaterFields::HeatingConvergenceTolerance).get());
  EXPECT_TRUE(idf_uh.isEmpty(ZoneHVAC_UnitHeaterFields::AvailabilityManagerListName));

  EXPECT_EQ(fan.nameString(), idf_uh.getString(ZoneHVAC_UnitHeaterFields::SupplyAirFanName).get());
  EXPECT_EQ("Fan:OnOff", idf_uh.getString(ZoneHVAC_UnitHeaterFields::SupplyAirFanObjectType).get());
  auto idf_fan = idf_uh.getTarget(ZoneHVAC_UnitHeaterFields::SupplyAirFanName).get();
  EXPECT_EQ(idf_fan.iddObject().type(), IddObjectType::Fan_OnOff);
  EXPECT_EQ(uh.airInletModelObject()->nameString(), idf_fan.getString(Fan_OnOffFields::AirInletNodeName).get());
  EXPECT_EQ(uh.nameString() + " Fan Outlet Node", idf_fan.getString(Fan_OnOffFields::AirOutletNodeName).get());

  EXPECT_EQ(hc.nameString(), idf_uh.getString(ZoneHVAC_UnitHeaterFields::HeatingCoilName).get());
  EXPECT_EQ("Coil:Heating:Fuel", idf_uh.getString(ZoneHVAC_UnitHeaterFields::HeatingCoilObjectType).get());
  auto idf_hc = idf_uh.getTarget(ZoneHVAC_UnitHeaterFields::HeatingCoilName).get();
  EXPECT_EQ(idf_hc.iddObject().type(), IddObjectType::Coil_Heating_Fuel);

  EXPECT_EQ(uh.nameString() + " Fan Outlet Node", idf_hc.getString(Coil_Heating_FuelFields::AirInletNodeName).get());
  EXPECT_EQ(idf_fan.getString(Fan_OnOffFields::AirOutletNodeName).get(), idf_hc.getString(Coil_Heating_FuelFields::AirInletNodeName).get());

  EXPECT_EQ(uh.airOutletModelObject()->nameString(), idf_hc.getString(Coil_Heating_FuelFields::AirOutletNodeName).get());
}

TEST_F(EnergyPlusFixture, ForwardTranslator_ZoneHVACUnitHeater_FanSystemModel_CoilElectric) {

  Model m;

  Schedule alwaysOn = m.alwaysOnDiscreteSchedule();
  FanSystemModel fan(m);
  CoilHeatingElectric hc(m);

  ZoneHVACUnitHeater uh(m, alwaysOn, fan, hc);

  EXPECT_EQ(alwaysOn, uh.availabilitySchedule());
  EXPECT_EQ(fan, uh.supplyAirFan());
  EXPECT_EQ(hc, uh.heatingCoil());

  EXPECT_TRUE(uh.setName("Zone HVAC Unit Heater"));
  EXPECT_TRUE(uh.setMaximumSupplyAirFlowRate(1.0));
  EXPECT_TRUE(uh.setFanControlType("OnOff"));

  EXPECT_TRUE(uh.setMaximumHotWaterFlowRate(0.05));
  EXPECT_TRUE(uh.setMinimumHotWaterFlowRate(0.0));
  EXPECT_TRUE(uh.setHeatingConvergenceTolerance(0.001));

  // Need to be in a thermal zone to be translated, with at least one space
  ThermalZone z(m);
  uh.addToThermalZone(z);
  Space s(m);
  s.setThermalZone(z);

  ForwardTranslator ft;
  Workspace workspace = ft.translateModel(m);

  WorkspaceObjectVector idfObjs(workspace.getObjectsByType(IddObjectType::ZoneHVAC_UnitHeater));
  ASSERT_EQ(1, idfObjs.size());
  WorkspaceObject idf_uh = idfObjs[0];

  EXPECT_EQ(uh.nameString(), idf_uh.getString(ZoneHVAC_UnitHeaterFields::Name).get());
  EXPECT_EQ(alwaysOn.nameString(), idf_uh.getString(ZoneHVAC_UnitHeaterFields::AvailabilityScheduleName).get());
  EXPECT_EQ(uh.airInletModelObject()->nameString(), idf_uh.getString(ZoneHVAC_UnitHeaterFields::AirInletNodeName).get());
  EXPECT_EQ(uh.airOutletModelObject()->nameString(), idf_uh.getString(ZoneHVAC_UnitHeaterFields::AirOutletNodeName).get());
  EXPECT_EQ(1.0, idf_uh.getDouble(ZoneHVAC_UnitHeaterFields::MaximumSupplyAirFlowRate).get());
  EXPECT_EQ(0.05, idf_uh.getDouble(ZoneHVAC_UnitHeaterFields::MaximumHotWaterorSteamFlowRate).get());
  EXPECT_EQ(0.0, idf_uh.getDouble(ZoneHVAC_UnitHeaterFields::MinimumHotWaterorSteamFlowRate).get());
  EXPECT_EQ(0.001, idf_uh.getDouble(ZoneHVAC_UnitHeaterFields::HeatingConvergenceTolerance).get());
  EXPECT_TRUE(idf_uh.isEmpty(ZoneHVAC_UnitHeaterFields::AvailabilityManagerListName));

  EXPECT_EQ(fan.nameString(), idf_uh.getString(ZoneHVAC_UnitHeaterFields::SupplyAirFanName).get());
  EXPECT_EQ("Fan:SystemModel", idf_uh.getString(ZoneHVAC_UnitHeaterFields::SupplyAirFanObjectType).get());
  auto idf_fan = idf_uh.getTarget(ZoneHVAC_UnitHeaterFields::SupplyAirFanName).get();
  EXPECT_EQ(idf_fan.iddObject().type(), IddObjectType::Fan_SystemModel);
  EXPECT_EQ(uh.airInletModelObject()->nameString(), idf_fan.getString(Fan_SystemModelFields::AirInletNodeName).get());
  EXPECT_EQ(uh.nameString() + " Fan Outlet Node", idf_fan.getString(Fan_SystemModelFields::AirOutletNodeName).get());

  EXPECT_EQ(hc.nameString(), idf_uh.getString(ZoneHVAC_UnitHeaterFields::HeatingCoilName).get());
  EXPECT_EQ("Coil:Heating:Electric", idf_uh.getString(ZoneHVAC_UnitHeaterFields::HeatingCoilObjectType).get());
  auto idf_hc = idf_uh.getTarget(ZoneHVAC_UnitHeaterFields::HeatingCoilName).get();
  EXPECT_EQ(idf_hc.iddObject().type(), IddObjectType::Coil_Heating_Electric);

  EXPECT_EQ(uh.nameString() + " Fan Outlet Node", idf_hc.getString(Coil_Heating_ElectricFields::AirInletNodeName).get());
  EXPECT_EQ(idf_fan.getString(Fan_SystemModelFields::AirOutletNodeName).get(), idf_hc.getString(Coil_Heating_ElectricFields::AirInletNodeName).get());

  EXPECT_EQ(uh.airOutletModelObject()->nameString(), idf_hc.getString(Coil_Heating_ElectricFields::AirOutletNodeName).get());
}

TEST_F(EnergyPlusFixture, ForwardTranslator_ZoneHVACUnitHeater_FanVariableVolume_CoilWater) {

  Model m;

  Schedule alwaysOn = m.alwaysOnDiscreteSchedule();
  FanVariableVolume fan(m);
  CoilHeatingWater hc(m);
  PlantLoop p(m);
  EXPECT_TRUE(p.addDemandBranchForComponent(hc));

  ZoneHVACUnitHeater uh(m, alwaysOn, fan, hc);

  EXPECT_EQ(alwaysOn, uh.availabilitySchedule());
  EXPECT_EQ(fan, uh.supplyAirFan());
  EXPECT_EQ(hc, uh.heatingCoil());

  EXPECT_TRUE(uh.setName("Zone HVAC Unit Heater"));
  EXPECT_TRUE(uh.setMaximumSupplyAirFlowRate(1.0));
  EXPECT_TRUE(uh.setFanControlType("OnOff"));

  EXPECT_TRUE(uh.setMaximumHotWaterFlowRate(0.05));
  EXPECT_TRUE(uh.setMinimumHotWaterFlowRate(0.0));
  EXPECT_TRUE(uh.setHeatingConvergenceTolerance(0.001));

  // Need to be in a thermal zone to be translated, with at least one space
  ThermalZone z(m);
  uh.addToThermalZone(z);
  Space s(m);
  s.setThermalZone(z);

  ForwardTranslator ft;
  Workspace workspace = ft.translateModel(m);

  WorkspaceObjectVector idfObjs(workspace.getObjectsByType(IddObjectType::ZoneHVAC_UnitHeater));
  ASSERT_EQ(1, idfObjs.size());
  WorkspaceObject idf_uh = idfObjs[0];

  EXPECT_EQ(uh.nameString(), idf_uh.getString(ZoneHVAC_UnitHeaterFields::Name).get());
  EXPECT_EQ(alwaysOn.nameString(), idf_uh.getString(ZoneHVAC_UnitHeaterFields::AvailabilityScheduleName).get());
  EXPECT_EQ(uh.airInletModelObject()->nameString(), idf_uh.getString(ZoneHVAC_UnitHeaterFields::AirInletNodeName).get());
  EXPECT_EQ(uh.airOutletModelObject()->nameString(), idf_uh.getString(ZoneHVAC_UnitHeaterFields::AirOutletNodeName).get());
  EXPECT_EQ(1.0, idf_uh.getDouble(ZoneHVAC_UnitHeaterFields::MaximumSupplyAirFlowRate).get());
  EXPECT_EQ(0.05, idf_uh.getDouble(ZoneHVAC_UnitHeaterFields::MaximumHotWaterorSteamFlowRate).get());
  EXPECT_EQ(0.0, idf_uh.getDouble(ZoneHVAC_UnitHeaterFields::MinimumHotWaterorSteamFlowRate).get());
  EXPECT_EQ(0.001, idf_uh.getDouble(ZoneHVAC_UnitHeaterFields::HeatingConvergenceTolerance).get());
  EXPECT_TRUE(idf_uh.isEmpty(ZoneHVAC_UnitHeaterFields::AvailabilityManagerListName));

  EXPECT_EQ(fan.nameString(), idf_uh.getString(ZoneHVAC_UnitHeaterFields::SupplyAirFanName).get());
  EXPECT_EQ("Fan:VariableVolume", idf_uh.getString(ZoneHVAC_UnitHeaterFields::SupplyAirFanObjectType).get());
  auto idf_fan = idf_uh.getTarget(ZoneHVAC_UnitHeaterFields::SupplyAirFanName).get();
  EXPECT_EQ(idf_fan.iddObject().type(), IddObjectType::Fan_VariableVolume);
  EXPECT_EQ(uh.airInletModelObject()->nameString(), idf_fan.getString(Fan_VariableVolumeFields::AirInletNodeName).get());
  EXPECT_EQ(uh.nameString() + " Fan Outlet Node", idf_fan.getString(Fan_VariableVolumeFields::AirOutletNodeName).get());

  EXPECT_EQ(hc.nameString(), idf_uh.getString(ZoneHVAC_UnitHeaterFields::HeatingCoilName).get());
  EXPECT_EQ("Coil:Heating:Water", idf_uh.getString(ZoneHVAC_UnitHeaterFields::HeatingCoilObjectType).get());
  auto idf_hc = idf_uh.getTarget(ZoneHVAC_UnitHeaterFields::HeatingCoilName).get();
  EXPECT_EQ(idf_hc.iddObject().type(), IddObjectType::Coil_Heating_Water);

  EXPECT_EQ(uh.nameString() + " Fan Outlet Node", idf_hc.getString(Coil_Heating_WaterFields::AirInletNodeName).get());
  EXPECT_EQ(idf_fan.getString(Fan_VariableVolumeFields::AirOutletNodeName).get(), idf_hc.getString(Coil_Heating_WaterFields::AirInletNodeName).get());

  EXPECT_EQ(uh.airOutletModelObject()->nameString(), idf_hc.getString(Coil_Heating_WaterFields::AirOutletNodeName).get());
}
