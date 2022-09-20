/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2022, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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
#include "../../model/AirLoopHVAC.hpp"
#include "../../model/AirLoopHVACUnitarySystem.hpp"
#include "../../model/AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed.hpp"
#include "../../model/Node.hpp"
#include "../../model/CoilHeatingElectricMultiStage.hpp"
#include "../../model/CoilHeatingElectricMultiStage_Impl.hpp"
#include "../../model/CoilHeatingElectricMultiStageStageData.hpp"
#include "../../model/CoilHeatingElectricMultiStageStageData_Impl.hpp"
#include "../../model/CoilHeatingDXSingleSpeed.hpp"
#include "../../model/CoilCoolingDXSingleSpeed.hpp"
#include "../../model/CoilCoolingDXMultiSpeed.hpp"
#include "../../model/CoilHeatingElectric.hpp"
#include "../../model/FanConstantVolume.hpp"

#include "../../utilities/idf/IdfObject.hpp"
#include "../../utilities/idf/IdfExtensibleGroup.hpp"
#include "../../utilities/idf/WorkspaceObject.hpp"
#include "../../utilities/idf/WorkspaceExtensibleGroup.hpp"

#include <utilities/idd/Coil_Heating_Electric_MultiStage_FieldEnums.hxx>
#include <utilities/idd/AirLoopHVAC_UnitaryHeatPump_AirToAir_MultiSpeed_FieldEnums.hxx>
#include <utilities/idd/AirLoopHVAC_UnitarySystem_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::energyplus;
using namespace openstudio::model;
using namespace openstudio;

TEST_F(EnergyPlusFixture, ForwardTranslator_CoilHeatingElectricMultiStage_HeatingCoil) {
  {  // AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed
    Model m;

    CoilCoolingDXMultiSpeed c(m);
    c.setName("HP CC");
    CoilHeatingElectricMultiStage h(m);
    h.setName("HP HC");
    FanConstantVolume f(m);
    f.setName("HP FanConstantVol");
    CoilHeatingElectric s(m);
    s.setName("HP SupHC");

    auto alwaysOn = m.alwaysOnDiscreteSchedule();
    alwaysOn.setName("HP HC AvailSch");
    h.setAvailabilitySchedule(alwaysOn);

    CoilHeatingElectricMultiStageStageData stageData(m);
    stageData.setEfficiency(1.5);
    stageData.setNominalCapacity(1000.0);
    h.addStage(stageData);

    AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed unitary(m, f, h, c, s);

    AirLoopHVAC airLoop(m);

    Node supplyOutletNode = airLoop.supplyOutletNode();
    unitary.addToNode(supplyOutletNode);

    ForwardTranslator ft;
    Workspace workspace = ft.translateModel(m);

    EXPECT_EQ(1u, workspace.getObjectsByType(IddObjectType::AirLoopHVAC_UnitaryHeatPump_AirToAir_MultiSpeed).size());
    EXPECT_EQ(1u, workspace.getObjectsByType(IddObjectType::Coil_Heating_Electric_MultiStage).size());

    IdfObject idf_hp = workspace.getObjectsByType(IddObjectType::AirLoopHVAC_UnitaryHeatPump_AirToAir_MultiSpeed)[0];
    IdfObject idf_coil = workspace.getObjectsByType(IddObjectType::Coil_Heating_Electric_MultiStage)[0];

    EXPECT_EQ("HP FanConstantVol", idf_hp.getString(AirLoopHVAC_UnitaryHeatPump_AirToAir_MultiSpeedFields::SupplyAirFanName, false).get());
    EXPECT_EQ("Coil:Heating:Electric:MultiStage",
              idf_unitary.getString(AirLoopHVAC_UnitaryHeatPump_AirToAir_MultiSpeedFields::HeatingCoilObjectType, false).get());
    EXPECT_EQ("HP HC", idf_hp.getString(AirLoopHVAC_UnitaryHeatPump_AirToAir_MultiSpeedFields::HeatingCoilName, false).get());
    EXPECT_EQ("HP CC", idf_hp.getString(AirLoopHVAC_UnitaryHeatPump_AirToAir_MultiSpeedFields::CoolingCoilName, false).get());
    EXPECT_EQ("HP SupHC", idf_hp.getString(AirLoopHVAC_UnitaryHeatPump_AirToAir_MultiSpeedFields::SupplementalHeatingCoilName, false).get());

    EXPECT_EQ("HP HC AvailSch", idf_coil.getString(Coil_Heating_Electric_MultiStageFields::AvailabilityScheduleName, false).get());
    EXPECT_NE("", idf_coil.getString(Coil_Heating_Electric_MultiStageFields::AirInletNodeName, false).get());
    EXPECT_NE("", idf_coil.getString(Coil_Heating_Electric_MultiStageFields::AirOutletNodeName, false).get());
    EXPECT_TRUE(idf_coil.isEmpty(Coil_Heating_Electric_MultiStageFields::TemperatureSetpointNodeName));
    EXPECT_EQ(1, idf_coil.getInt(Coil_Heating_Electric_MultiStageFields::NumberofStages, false).get());
    ASSERT_EQ(1, idf_coil.numExtensibleGroups());
    auto egs = idf_coil.extensibleGroups();
    auto stages = h.stages();
    for (size_t i = 0; i < idf_coil.extensibleGroups().size(); ++i) {
      const IdfExtensibleGroup& eg = egs[i];
      const auto stage = stages[i];

      EXPECT_EQ(1.5, eg.getDouble(Coil_Heating_Electric_MultiStageExtensibleFields::StageEfficiency).get());
      EXPECT_EQ(1000.0, eg.getDouble(Coil_Heating_Electric_MultiStageExtensibleFields::StageNominalCapacity).get());
    }
  }

  {  // AirLoopHVACUnitarySystem
    Model m;

    CoilCoolingDXSingleSpeed c(m);
    c.setName("HP CC");
    CoilHeatingElectricMultiStage h(m);
    h.setName("HP HC");
    FanConstantVolume f(m);
    f.setName("HP FanConstantVol");
    CoilHeatingElectric s(m);
    s.setName("HP SupHC");

    auto alwaysOn = m.alwaysOnDiscreteSchedule();
    alwaysOn.setName("HP HC AvailSch");
    h.setAvailabilitySchedule(alwaysOn);

    CoilHeatingElectricMultiStageStageData stageData(m);
    stageData.setEfficiency(1.5);
    stageData.setNominalCapacity(1000.0);
    h.addStage(stageData);

    AirLoopHVACUnitarySystem unitary(m);
    EXPECT_TRUE(unitary.setCoolingCoil(c));
    EXPECT_TRUE(unitary.setHeatingCoil(h));
    EXPECT_TRUE(unitary.setSupplyFan(f));
    EXPECT_TRUE(unitary.setSupplementalHeatingCoil(s));

    AirLoopHVAC airLoop(m);

    Node supplyOutletNode = airLoop.supplyOutletNode();
    unitary.addToNode(supplyOutletNode);

    ForwardTranslator ft;
    Workspace workspace = ft.translateModel(m);

    EXPECT_EQ(1u, workspace.getObjectsByType(IddObjectType::AirLoopHVAC_UnitarySystem).size());
    EXPECT_EQ(1u, workspace.getObjectsByType(IddObjectType::Coil_Heating_Electric_MultiStage).size());

    IdfObject idf_unitary = workspace.getObjectsByType(IddObjectType::AirLoopHVAC_UnitarySystem)[0];
    IdfObject idf_coil = workspace.getObjectsByType(IddObjectType::Coil_Heating_Electric_MultiStage)[0];

    EXPECT_EQ("HP FanConstantVol", idf_unitary.getString(AirLoopHVAC_UnitarySystemFields::SupplyFanName, false).get());
    EXPECT_EQ("Coil:Heating:Electric:MultiStage", idf_unitary.getString(AirLoopHVAC_UnitarySystemFields::HeatingCoilObjectType, false).get());
    EXPECT_EQ("HP HC", idf_unitary.getString(AirLoopHVAC_UnitarySystemFields::HeatingCoilName, false).get());
    EXPECT_EQ("HP CC", idf_unitary.getString(AirLoopHVAC_UnitarySystemFields::CoolingCoilName, false).get());
    EXPECT_EQ("HP SupHC", idf_unitary.getString(AirLoopHVAC_UnitarySystemFields::SupplementalHeatingCoilName, false).get());

    EXPECT_EQ("HP HC AvailSch", idf_coil.getString(Coil_Heating_Electric_MultiStageFields::AvailabilityScheduleName, false).get());
    EXPECT_NE("", idf_coil.getString(Coil_Heating_Electric_MultiStageFields::AirInletNodeName, false).get());
    EXPECT_NE("", idf_coil.getString(Coil_Heating_Electric_MultiStageFields::AirOutletNodeName, false).get());
    EXPECT_TRUE(idf_coil.isEmpty(Coil_Heating_Electric_MultiStageFields::TemperatureSetpointNodeName));
    EXPECT_EQ(1, idf_coil.getInt(Coil_Heating_Electric_MultiStageFields::NumberofStages, false).get());
    ASSERT_EQ(1, idf_coil.numExtensibleGroups());
    auto egs = idf_coil.extensibleGroups();
    auto stages = h.stages();
    for (size_t i = 0; i < idf_coil.extensibleGroups().size(); ++i) {
      const IdfExtensibleGroup& eg = egs[i];
      const auto stage = stages[i];

      EXPECT_EQ(1.5, eg.getDouble(Coil_Heating_Electric_MultiStageExtensibleFields::StageEfficiency).get());
      EXPECT_EQ(1000.0, eg.getDouble(Coil_Heating_Electric_MultiStageExtensibleFields::StageNominalCapacity).get());
    }
  }
}

TEST_F(EnergyPlusFixture, ForwardTranslator_CoilHeatingElectricMultiStage_SuppHeatingCoil) {
  Model m;

  CoilCoolingDXSingleSpeed c(m);
  c.setName("HP CC");
  CoilHeatingDXSingleSpeed h(m);
  h.setName("HP HC");
  FanConstantVolume f(m);
  f.setName("HP FanConstantVol");
  CoilHeatingElectricMultiStage s(m);
  s.setName("HP SupHC");

  auto alwaysOn = m.alwaysOnDiscreteSchedule();
  alwaysOn.setName("HP SupHC AvailSch");
  s.setAvailabilitySchedule(alwaysOn);

  CoilHeatingElectricMultiStageStageData stageData(m);
  stageData.setEfficiency(1.5);
  stageData.setNominalCapacity(1000.0);
  s.addStage(stageData);

  AirLoopHVACUnitarySystem unitary(m);
  EXPECT_TRUE(unitary.setCoolingCoil(c));
  EXPECT_TRUE(unitary.setHeatingCoil(h));
  EXPECT_TRUE(unitary.setSupplyFan(f));
  EXPECT_TRUE(unitary.setSupplementalHeatingCoil(s));

  AirLoopHVAC airLoop(m);

  Node supplyOutletNode = airLoop.supplyOutletNode();
  unitary.addToNode(supplyOutletNode);

  ForwardTranslator ft;
  Workspace workspace = ft.translateModel(m);

  EXPECT_EQ(1u, workspace.getObjectsByType(IddObjectType::AirLoopHVAC_UnitarySystem).size());
  EXPECT_EQ(1u, workspace.getObjectsByType(IddObjectType::Coil_Heating_Electric_MultiStage).size());

  IdfObject idf_unitary = workspace.getObjectsByType(IddObjectType::AirLoopHVAC_UnitarySystem)[0];
  IdfObject idf_coil = workspace.getObjectsByType(IddObjectType::Coil_Heating_Electric_MultiStage)[0];

  EXPECT_EQ("HP FanConstantVol", idf_unitary.getString(AirLoopHVAC_UnitarySystemFields::SupplyFanName, false).get());
  EXPECT_EQ("HP HC", idf_unitary.getString(AirLoopHVAC_UnitarySystemFields::HeatingCoilName, false).get());
  EXPECT_EQ("HP CC", idf_unitary.getString(AirLoopHVAC_UnitarySystemFields::CoolingCoilName, false).get());
  EXPECT_EQ("Coil:Heating:Electric:MultiStage",
            idf_unitary.getString(AirLoopHVAC_UnitarySystemFields::SupplementalHeatingCoilObjectType, false).get());
  EXPECT_EQ("HP SupHC", idf_unitary.getString(AirLoopHVAC_UnitarySystemFields::SupplementalHeatingCoilName, false).get());

  EXPECT_EQ("HP SupHC AvailSch", idf_coil.getString(Coil_Heating_Electric_MultiStageFields::AvailabilityScheduleName, false).get());
  EXPECT_NE("", idf_coil.getString(Coil_Heating_Electric_MultiStageFields::AirInletNodeName, false).get());
  EXPECT_NE("", idf_coil.getString(Coil_Heating_Electric_MultiStageFields::AirOutletNodeName, false).get());
  EXPECT_TRUE(idf_coil.isEmpty(Coil_Heating_Electric_MultiStageFields::TemperatureSetpointNodeName));
  EXPECT_EQ(1, idf_coil.getInt(Coil_Heating_Electric_MultiStageFields::NumberofStages, false).get());
  ASSERT_EQ(1, idf_coil.numExtensibleGroups());
  auto egs = idf_coil.extensibleGroups();
  auto stages = s.stages();
  for (size_t i = 0; i < idf_coil.extensibleGroups().size(); ++i) {
    const IdfExtensibleGroup& eg = egs[i];
    const auto stage = stages[i];

    EXPECT_EQ(1.5, eg.getDouble(Coil_Heating_Electric_MultiStageExtensibleFields::StageEfficiency).get());
    EXPECT_EQ(1000.0, eg.getDouble(Coil_Heating_Electric_MultiStageExtensibleFields::StageNominalCapacity).get());
  }
}
