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
#include "../../model/CoilCoolingWater.hpp"
#include "../../model/ThermalStorageIceDetailed.hpp"
#include "../../model/AirLoopHVAC.hpp"
#include "../../model/Node.hpp"
#include "../../model/AirLoopHVACUnitaryHeatPumpAirToAir.hpp"
#include "../../model/Schedule.hpp"
#include "../../model/FanConstantVolume.hpp"
#include "../../model/CoilHeatingElectric.hpp"

#include <utilities/idd/CoilSystem_IntegratedHeatPump_AirSource_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::energyplus;
using namespace openstudio::model;
using namespace openstudio;

TEST_F(EnergyPlusFixture, ForwardTranslator_CoilSystemIntegratedHeatPumpAirSource) {
  Model m;

  CoilSystemIntegratedHeatPumpAirSource coilSystem(m);

  CoilHeatingDXVariableSpeed heatingCoil(m);
  CoilCoolingDXVariableSpeed enhancedDehumidificationCoolingCoil(m);
  CoilCoolingDXVariableSpeed gridResponseCoolingCoil(m);
  CoilHeatingDXVariableSpeed gridResponseHeatingCoil(m);
  CoilChillerAirSourceVariableSpeed chillingCoil(m);
  CoilCoolingWater supplementalChillingCoil(m);
  ThermalStorageIceDetailed ts(m);

  coilSystem.setHeatingCoil(heatingCoil);
  coilSystem.setEnhancedDehumidificationCoolingCoil(enhancedDehumidificationCoolingCoil);
  coilSystem.setGridResponseCoolingCoil(gridResponseCoolingCoil);
  coilSystem.setGridResponseHeatingCoil(gridResponseHeatingCoil);
  coilSystem.setChillingCoil(chillingCoil);
  coilSystem.setSupplementalChillingCoil(supplementalChillingCoil);
  coilSystem.setStorageTank(ts);

  Schedule s = m.alwaysOnDiscreteSchedule();
  FanConstantVolume supplyFan(m, s);
  CoilHeatingElectric coilHeatingElectric(m, s);

  AirLoopHVACUnitaryHeatPumpAirToAir coil(m, s, supplyFan, coilSystem, coilSystem, coilHeatingElectric);

  AirLoopHVAC airLoop(m);
  Node supplyOutletNode = airLoop.supplyOutletNode();
  EXPECT_TRUE(coil.addToNode(supplyOutletNode));

  ForwardTranslator ft;
  Workspace w = ft.translateModel(m);

  EXPECT_EQ(3u, w.getObjectsByType(IddObjectType::Coil_Cooling_DX_VariableSpeed).size());
  EXPECT_EQ(2u, w.getObjectsByType(IddObjectType::Coil_Heating_DX_VariableSpeed).size());
  EXPECT_EQ(1u, w.getObjectsByType(IddObjectType::Coil_Chiller_AirSource_VariableSpeed).size());
  EXPECT_EQ(1u, w.getObjectsByType(IddObjectType::Coil_Cooling_Water).size());
  EXPECT_EQ(1u, w.getObjectsByType(IddObjectType::ThermalStorage_Ice_Detailed).size());

  WorkspaceObjectVector idf_coilSystems(w.getObjectsByType(IddObjectType::CoilSystem_IntegratedHeatPump_AirSource));
  EXPECT_EQ(1u, idf_coilSystems.size());
  WorkspaceObject idf_coilSystem(idf_coilSystems[0]);

  boost::optional<WorkspaceObject> idf_coolingCoil(idf_coilSystem.getTarget(CoilSystem_IntegratedHeatPump_AirSourceFields::SpaceCoolingCoilName));
  EXPECT_TRUE(idf_coolingCoil);
  EXPECT_EQ(idf_coolingCoil->iddObject().type(), IddObjectType::Coil_Cooling_DX_VariableSpeed);
  boost::optional<WorkspaceObject> idf_heatingCoil(idf_coilSystem.getTarget(CoilSystem_IntegratedHeatPump_AirSourceFields::SpaceHeatingCoilName));
  EXPECT_TRUE(idf_heatingCoil);
  EXPECT_EQ(idf_heatingCoil->iddObject().type(), IddObjectType::Coil_Heating_DX_VariableSpeed);
  boost::optional<WorkspaceObject> idf_enhancedDehumidificationCoolingCoil(
    idf_coilSystem.getTarget(CoilSystem_IntegratedHeatPump_AirSourceFields::EnhancedDehumidificationCoolingCoilName));
  EXPECT_TRUE(idf_enhancedDehumidificationCoolingCoil);
  EXPECT_EQ(idf_enhancedDehumidificationCoolingCoil->iddObject().type(), IddObjectType::Coil_Cooling_DX_VariableSpeed);
  boost::optional<WorkspaceObject> idf_gridResponseCoolingCoil(
    idf_coilSystem.getTarget(CoilSystem_IntegratedHeatPump_AirSourceFields::GridResponseCoolingCoilName));
  EXPECT_TRUE(idf_gridResponseCoolingCoil);
  EXPECT_EQ(idf_gridResponseCoolingCoil->iddObject().type(), IddObjectType::Coil_Cooling_DX_VariableSpeed);
  boost::optional<WorkspaceObject> idf_gridResponseHeatingCoil(
    idf_coilSystem.getTarget(CoilSystem_IntegratedHeatPump_AirSourceFields::GridResponseHeatingCoilName));
  EXPECT_TRUE(idf_gridResponseHeatingCoil);
  EXPECT_EQ(idf_gridResponseHeatingCoil->iddObject().type(), IddObjectType::Coil_Heating_DX_VariableSpeed);
  boost::optional<WorkspaceObject> idf_chillingCoil(idf_coilSystem.getTarget(CoilSystem_IntegratedHeatPump_AirSourceFields::ChillerCoilName));
  EXPECT_TRUE(idf_chillingCoil);
  EXPECT_EQ(idf_chillingCoil->iddObject().type(), IddObjectType::Coil_Chiller_AirSource_VariableSpeed);
  EXPECT_EQ("Single",
            idf_coilSystem.getString(CoilSystem_IntegratedHeatPump_AirSourceFields::ChillerCoilBelongstoaSingleorSeparateUnit, false).get());
  EXPECT_EQ(1, idf_coilSystem.getInt(CoilSystem_IntegratedHeatPump_AirSourceFields::ChillerCompressorRunSpeed, false).get());
  boost::optional<WorkspaceObject> idf_supplementalChillingCoil(
    idf_coilSystem.getTarget(CoilSystem_IntegratedHeatPump_AirSourceFields::CoilObjectName));
  EXPECT_TRUE(idf_supplementalChillingCoil);
  EXPECT_EQ(idf_supplementalChillingCoil->iddObject().type(),
            idf_coilSystem.getString(CoilSystem_IntegratedHeatPump_AirSourceFields::CoilObjectType).get());
  EXPECT_EQ(idf_supplementalChillingCoil->iddObject().type(), IddObjectType::Coil_Cooling_Water);
  boost::optional<WorkspaceObject> idf_ts(idf_coilSystem.getTarget(CoilSystem_IntegratedHeatPump_AirSourceFields::TankName));
  EXPECT_TRUE(idf_ts);
  EXPECT_EQ(idf_ts->iddObject().type(), IddObjectType::ThermalStorage_Ice_Detailed);
  EXPECT_EQ(idf_ts->iddObject().type(), idf_coilSystem.getString(CoilSystem_IntegratedHeatPump_AirSourceFields::TankObjectType).get());
  EXPECT_EQ(0.9, idf_coilSystem.getDouble(CoilSystem_IntegratedHeatPump_AirSourceFields::IceFractionbelowwhichchargingstarts, false).get());
  EXPECT_EQ(-0.5, idf_coilSystem.getDouble(CoilSystem_IntegratedHeatPump_AirSourceFields::ChillerEnteringTemperatureat0TankFraction, false).get());
  boost::optional<WorkspaceObject> idf_curve(
    idf_coilSystem.getTarget(CoilSystem_IntegratedHeatPump_AirSourceFields::TemperatureDeviationCurveNameasaFunctionoftheTankFraction));
  EXPECT_FALSE(idf_curve);
}
