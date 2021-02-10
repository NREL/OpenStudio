/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2020, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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
#include "../../model/CoilChillerAirSourceVariableSpeed.hpp"
#include "../../model/CoilWaterHeatingAirToWaterHeatPumpVariableSpeed.hpp"
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

#include <utilities/idd/CoilSystem_IntegratedHeatPump_AirSource_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::energyplus;
using namespace openstudio::model;
using namespace openstudio;

TEST_F(EnergyPlusFixture, ForwardTranslator_CoilSystemIntegratedHeatPumpAirSource) {
  Model m;

  CoilCoolingDXVariableSpeed spaceCoolingCoil(m);
  CoilHeatingDXVariableSpeed spaceHeatingCoil(m);
  CoilWaterHeatingAirToWaterHeatPumpVariableSpeed dedicatedWaterHeatingCoil(m);
  CoilWaterHeatingAirToWaterHeatPumpVariableSpeed scwhCoil(m);
  CoilCoolingDXVariableSpeed scdwhCoolingCoil(m);
  CoilWaterHeatingAirToWaterHeatPumpVariableSpeed scdwhWaterHeatingCoil(m);
  CoilHeatingDXVariableSpeed shdwhHeatingCoil(m);
  CoilWaterHeatingAirToWaterHeatPumpVariableSpeed shdwhWaterHeatingCoil(m);
  CoilCoolingDXVariableSpeed enhancedDehumidificationCoolingCoil(m);
  CoilCoolingDXVariableSpeed gridResponseCoolingCoil(m);
  CoilHeatingDXVariableSpeed gridResponseHeatingCoil(m);
  CoilChillerAirSourceVariableSpeed chillerCoil(m);
  CoilCoolingWater supplementalChillerCoil(m);
  ThermalStorageIceDetailed ts(m);

  CoilSystemIntegratedHeatPumpAirSource coilSystem(m, spaceCoolingCoil, spaceHeatingCoil);

  EXPECT_TRUE(coilSystem.spaceCoolingCoil().optionalCast<StraightComponent>());
  EXPECT_TRUE(coilSystem.spaceHeatingCoil().optionalCast<StraightComponent>());
  EXPECT_TRUE(coilSystem.setDedicatedWaterHeatingCoil(dedicatedWaterHeatingCoil));
  EXPECT_TRUE(coilSystem.setSCWHCoil(scwhCoil));
  EXPECT_TRUE(coilSystem.setSCDWHCoolingCoil(scdwhCoolingCoil));
  EXPECT_TRUE(coilSystem.setSCDWHWaterHeatingCoil(scdwhWaterHeatingCoil));
  EXPECT_TRUE(coilSystem.setSHDWHHeatingCoil(shdwhHeatingCoil));
  EXPECT_TRUE(coilSystem.setSHDWHWaterHeatingCoil(shdwhWaterHeatingCoil));
  EXPECT_TRUE(coilSystem.setEnhancedDehumidificationCoolingCoil(enhancedDehumidificationCoolingCoil));
  EXPECT_TRUE(coilSystem.setGridResponseCoolingCoil(gridResponseCoolingCoil));
  EXPECT_TRUE(coilSystem.setGridResponseHeatingCoil(gridResponseHeatingCoil));
  EXPECT_TRUE(coilSystem.setChillerCoil(chillerCoil));
  EXPECT_TRUE(coilSystem.setSupplementalChillerCoil(supplementalChillerCoil));
  EXPECT_TRUE(coilSystem.setStorageTank(ts));

  EXPECT_TRUE(coilSystem.dedicatedWaterHeatingCoil());
  EXPECT_TRUE(coilSystem.scwhCoil());
  EXPECT_TRUE(coilSystem.scdwhCoolingCoil());
  EXPECT_TRUE(coilSystem.scdwhWaterHeatingCoil());
  EXPECT_TRUE(coilSystem.shdwhHeatingCoil());
  EXPECT_TRUE(coilSystem.shdwhWaterHeatingCoil());
  EXPECT_TRUE(coilSystem.enhancedDehumidificationCoolingCoil());
  EXPECT_TRUE(coilSystem.gridResponseCoolingCoil());
  EXPECT_TRUE(coilSystem.gridResponseHeatingCoil());
  EXPECT_EQ(0, coilSystem.flagtoIndicateLoadControlInSCWHMode());
  EXPECT_TRUE(coilSystem.chillerCoil());
  EXPECT_TRUE(coilSystem.supplementalChillerCoil());
  EXPECT_TRUE(coilSystem.storageTank());

  WaterHeaterHeatPump hpwh(m);
  ThermalZone tz(m);
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
  EXPECT_EQ(4u, w.getObjectsByType(IddObjectType::Coil_Cooling_DX_VariableSpeed).size());
  EXPECT_EQ(3u, w.getObjectsByType(IddObjectType::Coil_Heating_DX_VariableSpeed).size());
  EXPECT_EQ(1u, w.getObjectsByType(IddObjectType::Coil_Chiller_AirSource_VariableSpeed).size());
  EXPECT_EQ(4u, w.getObjectsByType(IddObjectType::Coil_WaterHeating_AirToWaterHeatPump_VariableSpeed).size());
  EXPECT_EQ(1u, w.getObjectsByType(IddObjectType::Coil_Cooling_Water).size());
  EXPECT_EQ(1u, w.getObjectsByType(IddObjectType::ThermalStorage_Ice_Detailed).size());

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

  boost::optional<WorkspaceObject> idf_edc(
    idf_coilSystem.getTarget(CoilSystem_IntegratedHeatPump_AirSourceFields::EnhancedDehumidificationCoolingCoilName));
  ASSERT_TRUE(idf_edc);
  EXPECT_EQ(idf_edc->iddObject().type(), IddObjectType::Coil_Cooling_DX_VariableSpeed);

  boost::optional<WorkspaceObject> idf_grc(idf_coilSystem.getTarget(CoilSystem_IntegratedHeatPump_AirSourceFields::GridResponseCoolingCoilName));
  ASSERT_TRUE(idf_grc);
  EXPECT_EQ(idf_grc->iddObject().type(), IddObjectType::Coil_Cooling_DX_VariableSpeed);

  boost::optional<WorkspaceObject> idf_grh(idf_coilSystem.getTarget(CoilSystem_IntegratedHeatPump_AirSourceFields::GridResponseSpaceHeatingCoilName));
  ASSERT_TRUE(idf_grh);
  EXPECT_EQ(idf_grh->iddObject().type(), IddObjectType::Coil_Heating_DX_VariableSpeed);

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
  EXPECT_EQ(1.0,
            idf_coilSystem.getDouble(CoilSystem_IntegratedHeatPump_AirSourceFields::SizingRatioofSpaceHeatingCoiltoSpaceCoolingCoil, false).get());
  EXPECT_EQ(
    1.0,
    idf_coilSystem.getDouble(CoilSystem_IntegratedHeatPump_AirSourceFields::SizingRatioofDedicatedWaterHeatingCoiltoSpaceCoolingCoil, false).get());
  EXPECT_EQ(
    1.0, idf_coilSystem
           .getDouble(
             CoilSystem_IntegratedHeatPump_AirSourceFields::SizingRatioofCombinedSpaceCoolingandWaterHeatingCoilwithFullCondensingtoSpaceCoolingCoil,
             false)
           .get());
  EXPECT_EQ(1.0, idf_coilSystem
                   .getDouble(CoilSystem_IntegratedHeatPump_AirSourceFields::
                                SizingRatioofCombinedSpaceCoolingandWaterHeatingCoilwithDesuperheatingCoolingCapacitytoSpaceCoolingCoil,
                              false)
                   .get());
  EXPECT_EQ(0.15, idf_coilSystem
                    .getDouble(CoilSystem_IntegratedHeatPump_AirSourceFields::
                                 SizingRatioofCombinedSpaceCoolingandWaterHeatingCoilwithDesuperheatingWaterHeatingCapacitytoSpaceCoolingCoil,
                               false)
                    .get());
  EXPECT_EQ(1.0, idf_coilSystem
                   .getDouble(CoilSystem_IntegratedHeatPump_AirSourceFields::
                                SizingRatioofCombinedSpaceHeatingandWaterHeatingCoilwithDesuperheatingSpaceHeatingCapacitytoSpaceCoolingCoil,
                              false)
                   .get());
  EXPECT_EQ(0.1, idf_coilSystem
                   .getDouble(CoilSystem_IntegratedHeatPump_AirSourceFields::
                                SizingRatioofCombinedSpaceHeatingandWaterHeatingCoilwithDesuperheatingWaterHeatingCapacitytoSpaceCoolingCoil,
                              false)
                   .get());
  EXPECT_EQ(
    0.9, idf_coilSystem.getDouble(CoilSystem_IntegratedHeatPump_AirSourceFields::SizingRatioofEnhancedDehumidificationCoiltoSpaceCoolingCoil, false)
           .get());
  EXPECT_EQ(
    0.9,
    idf_coilSystem.getDouble(CoilSystem_IntegratedHeatPump_AirSourceFields::SizingRatioofGridResponseCoolingCoiltoSpaceCoolingCoil, false).get());
  EXPECT_EQ(
    0.9,
    idf_coilSystem.getDouble(CoilSystem_IntegratedHeatPump_AirSourceFields::SizingRatioofGridResponseHeatingCoiltoSpaceCoolingCoil, false).get());

  boost::optional<WorkspaceObject> idf_chillerCoil(idf_coilSystem.getTarget(CoilSystem_IntegratedHeatPump_AirSourceFields::ChillerCoilName));
  ASSERT_TRUE(idf_chillerCoil);
  EXPECT_EQ(idf_chillerCoil->iddObject().type(), IddObjectType::Coil_Chiller_AirSource_VariableSpeed);

  EXPECT_EQ("Single",
            idf_coilSystem.getString(CoilSystem_IntegratedHeatPump_AirSourceFields::ChillerCoilBelongstoaSingleorSeparateUnit, false).get());
  EXPECT_EQ(1, idf_coilSystem.getInt(CoilSystem_IntegratedHeatPump_AirSourceFields::ChillerCompressorRunSpeed, false).get());

  EXPECT_EQ(1.0, idf_coilSystem.getDouble(CoilSystem_IntegratedHeatPump_AirSourceFields::SizingRatioofChillerCoiltoSpaceCoolingCoil, false).get());

  boost::optional<WorkspaceObject> idf_supplementalChillerCoil(
    idf_coilSystem.getTarget(CoilSystem_IntegratedHeatPump_AirSourceFields::CoilObjectName));
  ASSERT_TRUE(idf_supplementalChillerCoil);
  EXPECT_EQ(idf_supplementalChillerCoil->iddObject().type(),
            idf_coilSystem.getString(CoilSystem_IntegratedHeatPump_AirSourceFields::CoilObjectType).get());
  EXPECT_EQ(idf_supplementalChillerCoil->iddObject().type(), IddObjectType::Coil_Cooling_Water);

  EXPECT_EQ(1.0, idf_coilSystem.getDouble(CoilSystem_IntegratedHeatPump_AirSourceFields::AirFlowRatioofWaterCoiltotheSpaceCoolingCoil, false).get());
  EXPECT_EQ(1.0, idf_coilSystem.getDouble(CoilSystem_IntegratedHeatPump_AirSourceFields::WaterFlowRatioofWaterCoiltotheChillerCoil, false).get());

  boost::optional<WorkspaceObject> idf_ts(idf_coilSystem.getTarget(CoilSystem_IntegratedHeatPump_AirSourceFields::TankName));
  ASSERT_TRUE(idf_ts);
  EXPECT_EQ(idf_ts->iddObject().type(), IddObjectType::ThermalStorage_Ice_Detailed);
  EXPECT_EQ(idf_ts->iddObject().type(), idf_coilSystem.getString(CoilSystem_IntegratedHeatPump_AirSourceFields::TankObjectType).get());
  EXPECT_EQ(0.9, idf_coilSystem.getDouble(CoilSystem_IntegratedHeatPump_AirSourceFields::IceFractionbelowwhichchargingstarts, false).get());
  EXPECT_EQ(-0.5, idf_coilSystem.getDouble(CoilSystem_IntegratedHeatPump_AirSourceFields::ChillerEnteringTemperatureat0TankFraction, false).get());
  boost::optional<WorkspaceObject> idf_curve(
    idf_coilSystem.getTarget(CoilSystem_IntegratedHeatPump_AirSourceFields::TemperatureDeviationCurveNameasaFunctionoftheTankFraction));
  EXPECT_FALSE(idf_curve);
}
