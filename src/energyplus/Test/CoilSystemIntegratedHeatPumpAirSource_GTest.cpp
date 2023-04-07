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
