/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
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
