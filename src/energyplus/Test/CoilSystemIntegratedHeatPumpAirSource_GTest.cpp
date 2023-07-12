/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>
#include "EnergyPlusFixture.hpp"

#include "../ForwardTranslator.hpp"

#include "../../model/Model.hpp"
#include "../../model/CoilSystemIntegratedHeatPumpAirSource.hpp"
#include "../../model/CoilSystemIntegratedHeatPumpAirSource_Impl.hpp"
#include "../../model/CoilCoolingDXVariableSpeed.hpp"
#include "../../model/CoilHeatingDXVariableSpeed.hpp"
#include "../../model/CoilWaterHeatingAirToWaterHeatPumpVariableSpeed.hpp"
#include "../../model/CoilWaterHeatingAirToWaterHeatPumpVariableSpeedSpeedData.hpp"
#include "../../model/CoilCoolingWater.hpp"
#include "../../model/ThermalStorageIceDetailed.hpp"
#include "../../model/AirLoopHVAC.hpp"
#include "../../model/Node.hpp"
#include "../../model/AirLoopHVACUnitaryHeatPumpAirToAir.hpp"
#include "../../model/WaterHeaterHeatPump.hpp"
#include "../../model/Schedule.hpp"
#include "../../model/FanConstantVolume.hpp"
#include "../../model/CoilHeatingElectric.hpp"
#include "../../model/ThermalZone.hpp"
#include "../../model/Space.hpp"

#include <utilities/idd/CoilSystem_IntegratedHeatPump_AirSource_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::energyplus;
using namespace openstudio::model;
using namespace openstudio;

TEST_F(EnergyPlusFixture, ForwardTranslator_CoilSystemIntegratedHeatPumpAirSource) {
  Model m;

  CoilCoolingDXVariableSpeed spaceCoolingCoil(m);
  spaceCoolingCoil.setName("spaceCoolingCoil");
  CoilHeatingDXVariableSpeed spaceHeatingCoil(m);
  spaceHeatingCoil.setName("spaceHeatingCoil");

  CoilWaterHeatingAirToWaterHeatPumpVariableSpeed dedicatedWaterHeatingCoil(m);
  CoilWaterHeatingAirToWaterHeatPumpVariableSpeedSpeedData dedicatedWaterHeatingCoilSpeed1(m);
  dedicatedWaterHeatingCoil.addSpeed(dedicatedWaterHeatingCoilSpeed1);
  dedicatedWaterHeatingCoil.setName("dedicatedWaterHeatingCoil");

  CoilWaterHeatingAirToWaterHeatPumpVariableSpeed scwhCoil(m);
  CoilWaterHeatingAirToWaterHeatPumpVariableSpeedSpeedData scwhCoilSpeed1(m);
  scwhCoil.addSpeed(scwhCoilSpeed1);
  scwhCoil.setName("scwhCoil");

  CoilCoolingDXVariableSpeed scdwhCoolingCoil(m);
  scdwhCoolingCoil.setName("scdwhCoolingCoil");

  CoilWaterHeatingAirToWaterHeatPumpVariableSpeed scdwhWaterHeatingCoil(m);
  CoilWaterHeatingAirToWaterHeatPumpVariableSpeedSpeedData scdwhWaterHeatingCoilSpeed1(m);
  scdwhWaterHeatingCoil.addSpeed(scdwhWaterHeatingCoilSpeed1);
  scdwhWaterHeatingCoil.setName("scdwhWaterHeatingCoil");

  CoilHeatingDXVariableSpeed shdwhHeatingCoil(m);
  shdwhHeatingCoil.setName("shdwhHeatingCoil");

  CoilWaterHeatingAirToWaterHeatPumpVariableSpeed shdwhWaterHeatingCoil(m);
  CoilWaterHeatingAirToWaterHeatPumpVariableSpeedSpeedData shdwhWaterHeatingCoilSpeed1(m);
  shdwhWaterHeatingCoil.addSpeed(shdwhWaterHeatingCoilSpeed1);
  shdwhWaterHeatingCoil.setName("shdwhWaterHeatingCoil");

  CoilSystemIntegratedHeatPumpAirSource coilSystem(m, spaceCoolingCoil, spaceHeatingCoil, dedicatedWaterHeatingCoil, scwhCoil, scdwhCoolingCoil,
                                                   scdwhWaterHeatingCoil, shdwhHeatingCoil, shdwhWaterHeatingCoil);

  EXPECT_EQ(spaceCoolingCoil, coilSystem.spaceCoolingCoil());
  EXPECT_EQ(spaceHeatingCoil, coilSystem.spaceHeatingCoil());
  EXPECT_EQ(dedicatedWaterHeatingCoil, coilSystem.dedicatedWaterHeatingCoil());
  EXPECT_EQ(scwhCoil, coilSystem.scwhCoil());
  EXPECT_EQ(scdwhCoolingCoil, coilSystem.scdwhCoolingCoil());
  EXPECT_EQ(scdwhWaterHeatingCoil, coilSystem.scdwhWaterHeatingCoil());
  EXPECT_EQ(shdwhHeatingCoil, coilSystem.shdwhHeatingCoil());
  EXPECT_EQ(shdwhWaterHeatingCoil, coilSystem.shdwhWaterHeatingCoil());

  WaterHeaterHeatPump hpwh(m);
  ThermalZone tz(m);
  Space space(m);
  space.setThermalZone(tz);

  hpwh.setDXCoil(coilSystem);
  hpwh.addToThermalZone(tz);
  EXPECT_TRUE(hpwh.dXCoil().optionalCast<CoilSystemIntegratedHeatPumpAirSource>());

  Schedule s = m.alwaysOnDiscreteSchedule();
  FanConstantVolume supplyFan(m, s);
  CoilHeatingElectric coilHeatingElectric(m, s);

  AirLoopHVACUnitaryHeatPumpAirToAir coil(m, s, supplyFan, coilSystem, coilSystem, coilHeatingElectric);

  AirLoopHVAC airLoop(m);
  Node supplyOutletNode = airLoop.supplyOutletNode();
  EXPECT_TRUE(coil.addToNode(supplyOutletNode));

  ForwardTranslator ft;
  Workspace w = ft.translateModel(m);

  EXPECT_EQ(1u, w.getObjectsByType(IddObjectType::AirLoopHVAC_UnitaryHeatPump_AirToAir).size());
  EXPECT_EQ(1u, w.getObjectsByType(IddObjectType::WaterHeater_HeatPump_PumpedCondenser).size());
  EXPECT_EQ(2u, w.getObjectsByType(IddObjectType::Coil_Cooling_DX_VariableSpeed).size());
  EXPECT_EQ(2u, w.getObjectsByType(IddObjectType::Coil_Heating_DX_VariableSpeed).size());
  EXPECT_EQ(4u, w.getObjectsByType(IddObjectType::Coil_WaterHeating_AirToWaterHeatPump_VariableSpeed).size());

  WorkspaceObjectVector idf_coilSystems(w.getObjectsByType(IddObjectType::CoilSystem_IntegratedHeatPump_AirSource));
  EXPECT_EQ(1u, idf_coilSystems.size());
  WorkspaceObject idf_coilSystem(idf_coilSystems[0]);

  boost::optional<WorkspaceObject> idf_coolingCoil(idf_coilSystem.getTarget(CoilSystem_IntegratedHeatPump_AirSourceFields::SpaceCoolingCoilName));
  ASSERT_TRUE(idf_coolingCoil);
  EXPECT_EQ(idf_coolingCoil->iddObject().type(), IddObjectType::Coil_Cooling_DX_VariableSpeed);

  boost::optional<WorkspaceObject> idf_heatingCoil(idf_coilSystem.getTarget(CoilSystem_IntegratedHeatPump_AirSourceFields::SpaceHeatingCoilName));
  ASSERT_TRUE(idf_heatingCoil);
  EXPECT_EQ(idf_heatingCoil->iddObject().type(), IddObjectType::Coil_Heating_DX_VariableSpeed);

  boost::optional<WorkspaceObject> idf_dedicatedWaterHeatingCoil(
    idf_coilSystem.getTarget(CoilSystem_IntegratedHeatPump_AirSourceFields::DedicatedWaterHeatingCoilName));
  ASSERT_TRUE(idf_dedicatedWaterHeatingCoil);
  EXPECT_EQ(idf_dedicatedWaterHeatingCoil->iddObject().type(), IddObjectType::Coil_WaterHeating_AirToWaterHeatPump_VariableSpeed);

  boost::optional<WorkspaceObject> idf_scwhCoil(idf_coilSystem.getTarget(CoilSystem_IntegratedHeatPump_AirSourceFields::SCWHCoilName));
  ASSERT_TRUE(idf_scwhCoil);
  EXPECT_EQ(idf_scwhCoil->iddObject().type(), IddObjectType::Coil_WaterHeating_AirToWaterHeatPump_VariableSpeed);

  boost::optional<WorkspaceObject> idf_scdwhCoolingCoil(
    idf_coilSystem.getTarget(CoilSystem_IntegratedHeatPump_AirSourceFields::SCDWHCoolingCoilName));
  ASSERT_TRUE(idf_scdwhCoolingCoil);
  EXPECT_EQ(idf_scdwhCoolingCoil->iddObject().type(), IddObjectType::Coil_Cooling_DX_VariableSpeed);

  boost::optional<WorkspaceObject> idf_scdwhWaterHeatingCoil(
    idf_coilSystem.getTarget(CoilSystem_IntegratedHeatPump_AirSourceFields::SCDWHWaterHeatingCoilName));
  ASSERT_TRUE(idf_scdwhWaterHeatingCoil);
  EXPECT_EQ(idf_scdwhWaterHeatingCoil->iddObject().type(), IddObjectType::Coil_WaterHeating_AirToWaterHeatPump_VariableSpeed);

  boost::optional<WorkspaceObject> idf_shdwhHeatingCoil(
    idf_coilSystem.getTarget(CoilSystem_IntegratedHeatPump_AirSourceFields::SHDWHHeatingCoilName));
  ASSERT_TRUE(idf_shdwhHeatingCoil);
  EXPECT_EQ(idf_shdwhHeatingCoil->iddObject().type(), IddObjectType::Coil_Heating_DX_VariableSpeed);

  boost::optional<WorkspaceObject> idf_shdwhWaterHeatingCoil(
    idf_coilSystem.getTarget(CoilSystem_IntegratedHeatPump_AirSourceFields::SHDWHWaterHeatingCoilName));
  ASSERT_TRUE(idf_shdwhWaterHeatingCoil);
  EXPECT_EQ(idf_shdwhWaterHeatingCoil->iddObject().type(), IddObjectType::Coil_WaterHeating_AirToWaterHeatPump_VariableSpeed);

  EXPECT_EQ(20.0, idf_coilSystem.getDouble(CoilSystem_IntegratedHeatPump_AirSourceFields::IndoorTemperatureLimitforSCWHMode, false).get());
  EXPECT_EQ(27.0, idf_coilSystem.getDouble(CoilSystem_IntegratedHeatPump_AirSourceFields::AmbientTemperatureLimitforSCWHMode, false).get());
  EXPECT_EQ(20.0,
            idf_coilSystem.getDouble(CoilSystem_IntegratedHeatPump_AirSourceFields::IndoorTemperatureaboveWhichWHhasHigherPriority, false).get());
  EXPECT_EQ(20.0,
            idf_coilSystem.getDouble(CoilSystem_IntegratedHeatPump_AirSourceFields::AmbientTemperatureaboveWhichWHhasHigherPriority, false).get());
  EXPECT_EQ(0, idf_coilSystem.getInt(CoilSystem_IntegratedHeatPump_AirSourceFields::FlagtoIndicateLoadControlinSCWHMode, false).get());
  EXPECT_EQ(1, idf_coilSystem.getInt(CoilSystem_IntegratedHeatPump_AirSourceFields::MinimumSpeedLevelforSCWHMode, false).get());
  EXPECT_EQ(
    0.0,
    idf_coilSystem.getDouble(CoilSystem_IntegratedHeatPump_AirSourceFields::MaximumWaterFlowVolumebeforeSwitchingfromSCDWHtoSCWHMode, false).get());
  EXPECT_EQ(1, idf_coilSystem.getInt(CoilSystem_IntegratedHeatPump_AirSourceFields::MinimumSpeedLevelforSCDWHMode, false).get());
  EXPECT_EQ(360.0, idf_coilSystem
                     .getDouble(
                       CoilSystem_IntegratedHeatPump_AirSourceFields::MaximumRunningTimebeforeAllowingElectricResistanceHeatUseduringSHDWHMode, false)
                     .get());
  EXPECT_EQ(1, idf_coilSystem.getInt(CoilSystem_IntegratedHeatPump_AirSourceFields::MinimumSpeedLevelforSHDWHMode, false).get());
}
