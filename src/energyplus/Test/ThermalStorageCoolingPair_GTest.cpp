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
#include "../../model/ThermalStorageCoolingPair.hpp"
#include "../../model/ThermalStorageCoolingPair_Impl.hpp"
#include "../../model/ThermalStorageIceDetailed.hpp"
#include "../../model/CoilCoolingDXVariableSpeed.hpp"
#include "../../model/CoilHeatingDXVariableSpeed.hpp"
#include "../../model/ChillerElectricEIR.hpp"
#include "../../model/PlantLoop.hpp"
#include "../../model/AirLoopHVAC.hpp"
#include "../../model/Node.hpp"
#include "../../model/AirLoopHVACUnitaryHeatPumpAirToAir.hpp"
#include "../../model/Schedule.hpp"
#include "../../model/FanConstantVolume.hpp"
#include "../../model/CoilHeatingElectric.hpp"

#include <utilities/idd/ThermalStorage_Cooling_Pair_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::energyplus;
using namespace openstudio::model;
using namespace openstudio;

TEST_F(EnergyPlusFixture, ForwardTranslator_ThermalStorageCoolingPair) {
  Model m;

  ThermalStorageCoolingPair ts(m);

  CoilCoolingDXVariableSpeed coilCooling(m);
  CoilHeatingDXVariableSpeed coilHeating(m);
  ThermalStorageIceDetailed ice(m);
  ChillerElectricEIR ch(m);

  ts.setCoolingCoil(coilCooling);
  ts.setTank(ice);
  ts.setRecoveryUnit(ch);

  PlantLoop p(m);
  ASSERT_TRUE(p.addSupplyBranchForComponent(ice));

  Schedule s = m.alwaysOnDiscreteSchedule();
  FanConstantVolume supplyFan(m, s);
  CoilHeatingElectric coilHeatingElectric(m, s);

  AirLoopHVACUnitaryHeatPumpAirToAir coil(m, s, supplyFan, coilHeating, coilCooling, coilHeatingElectric);

  AirLoopHVAC airLoop(m);
  Node supplyOutletNode = airLoop.supplyOutletNode();
  EXPECT_TRUE(coil.addToNode(supplyOutletNode));

  ForwardTranslator ft;
  Workspace w = ft.translateModel(m);

  EXPECT_EQ(1u, w.getObjectsByType(IddObjectType::Coil_Cooling_DX_VariableSpeed).size());
  EXPECT_EQ(1u, w.getObjectsByType(IddObjectType::Coil_Heating_DX_VariableSpeed).size());
  EXPECT_EQ(1u, w.getObjectsByType(IddObjectType::ThermalStorage_Ice_Detailed).size());
  EXPECT_EQ(1u, w.getObjectsByType(IddObjectType::Chiller_Electric_EIR).size());

  WorkspaceObjectVector idf_tss(w.getObjectsByType(IddObjectType::ThermalStorage_Cooling_Pair));
  EXPECT_EQ(1u, idf_tss.size());
  WorkspaceObject idf_ts(idf_tss[0]);

  boost::optional<WorkspaceObject> idf_coolingCoil(idf_ts.getTarget(ThermalStorage_Cooling_PairFields::CoolingCoilName));
  ASSERT_TRUE(idf_coolingCoil);
  EXPECT_EQ(idf_coolingCoil->iddObject().type(), IddObjectType::Coil_Cooling_DX_VariableSpeed);
  boost::optional<WorkspaceObject> idf_tank(idf_ts.getTarget(ThermalStorage_Cooling_PairFields::TankName));
  ASSERT_TRUE(idf_tank);
  EXPECT_EQ(idf_tank->iddObject().type(), IddObjectType::ThermalStorage_Ice_Detailed);
  EXPECT_EQ(0, idf_ts.getDouble(ThermalStorage_Cooling_PairFields::MaximumPeakOperationHours, false).get());
  EXPECT_EQ(0, idf_ts.getDouble(ThermalStorage_Cooling_PairFields::TemperatureOrConcentrationChangeInTankThroughOperation, false).get());
  EXPECT_EQ("Total", idf_ts.getString(ThermalStorage_Cooling_PairFields::LoadType, false).get());
  boost::optional<WorkspaceObject> idf_chiller(idf_ts.getTarget(ThermalStorage_Cooling_PairFields::RecoveryUnitName));
  ASSERT_TRUE(idf_chiller);
  EXPECT_EQ(idf_chiller->iddObject().type(), IddObjectType::Chiller_Electric_EIR);
  EXPECT_EQ(1, idf_ts.getDouble(ThermalStorage_Cooling_PairFields::CapacityRatioOfRecoveryUnitToMainCoolingCoil, false).get());
}
