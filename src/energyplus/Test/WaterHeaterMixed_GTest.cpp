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
#include "../../model/WaterHeaterMixed.hpp"
// #include "../../model/WaterHeaterMixed_Impl.hpp"
#include "../../model/Schedule.hpp"

#include "../../model/PlantLoop.hpp"
#include "../../model/PlantLoop_Impl.hpp"
#include "../../model/PipeAdiabatic.hpp"
#include "../../model/PipeAdiabatic_Impl.hpp"
#include "../../model/Node.hpp"
#include "../../model/Node_Impl.hpp"
#include "../../model/ScheduleConstant.hpp"
#include "../../model/ScheduleConstant_Impl.hpp"

#include "../../utilities/idf/IdfFile.hpp"
#include "../../utilities/idf/Workspace.hpp"
#include "../../utilities/idf/IdfObject.hpp"
#include "../../utilities/idf/WorkspaceObject.hpp"
#include "../../utilities/idf/IdfExtensibleGroup.hpp"
#include "../../utilities/idf/WorkspaceExtensibleGroup.hpp"

#include <utilities/idd/WaterHeater_Mixed_FieldEnums.hxx>
#include <utilities/idd/PlantLoop_FieldEnums.hxx>
#include <utilities/idd/PlantEquipmentList_FieldEnums.hxx>
#include <utilities/idd/PlantEquipmentOperationSchemes_FieldEnums.hxx>
#include <utilities/idd/PlantEquipmentOperation_HeatingLoad_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::energyplus;
using namespace openstudio::model;
using namespace openstudio;

// Test for #1675
TEST_F(EnergyPlusFixture, ForwardTranslatorWaterHeaterMixed_Condition) {
  ForwardTranslator ft;
  Model m;

  WaterHeaterMixed wh(m);
  EXPECT_FALSE(wh.peakUseFlowRate());
  EXPECT_FALSE(wh.useFlowRateFractionSchedule());

  // Not on a PlantLoop, and no Peak use Flow rate => Not translated
  {
    Workspace w = ft.translateModel(m);
    EXPECT_EQ(0u, w.getObjectsByType(IddObjectType::WaterHeater_Mixed).size());
  }

  // Has a peak flow rate, but still missing the Flow Rate schedule
  EXPECT_TRUE(wh.setPeakUseFlowRate(0.1));
  EXPECT_TRUE(wh.peakUseFlowRate());
  {
    Workspace w = ft.translateModel(m);
    EXPECT_EQ(0u, w.getObjectsByType(IddObjectType::WaterHeater_Mixed).size());
  }

  // Has both required conditions for standalone
  auto alwaysOnContinuous = m.alwaysOnContinuousSchedule();
  EXPECT_TRUE(wh.setUseFlowRateFractionSchedule(alwaysOnContinuous));
  EXPECT_TRUE(wh.useFlowRateFractionSchedule());
  {
    Workspace w = ft.translateModel(m);
    EXPECT_EQ(1u, w.getObjectsByType(IddObjectType::WaterHeater_Mixed).size());
  }

  // Now it has the schedule, but not the peak use flow rate
  wh.resetPeakUseFlowRate();
  EXPECT_FALSE(wh.peakUseFlowRate());
  {
    Workspace w = ft.translateModel(m);
    EXPECT_EQ(0u, w.getObjectsByType(IddObjectType::WaterHeater_Mixed).size());
  }

  // Now it has neither
  wh.resetUseFlowRateFractionSchedule();
  EXPECT_FALSE(wh.useFlowRateFractionSchedule());
  {
    Workspace w = ft.translateModel(m);
    EXPECT_EQ(0u, w.getObjectsByType(IddObjectType::WaterHeater_Mixed).size());
  }

  PlantLoop p(m);
  // Add a supply branch for the wh
  EXPECT_TRUE(p.addSupplyBranchForComponent(wh));
  {
    Workspace w = ft.translateModel(m);
    EXPECT_EQ(1u, w.getObjectsByType(IddObjectType::WaterHeater_Mixed).size());
  }
}

TEST_F(EnergyPlusFixture, ForwardTranslatorWaterHeaterMixed_PlantLoopConnections) {
  ForwardTranslator ft;
  Model m;

  WaterHeaterMixed wh(m);

  PlantLoop useSidePlantLoop(m);

  EXPECT_TRUE(useSidePlantLoop.addSupplyBranchForComponent(wh));
  std::string useSideOutletNodeName;
  {
    Workspace w = ft.translateModel(m);

    EXPECT_EQ(1, w.getObjectsByType(IddObjectType::PlantLoop).size());

    std::vector<WorkspaceObject> idfWHMixeds(w.getObjectsByType(IddObjectType::WaterHeater_Mixed));
    ASSERT_EQ(1, idfWHMixeds.size());
    WorkspaceObject idfWHMixed(idfWHMixeds[0]);

    EXPECT_FALSE(idfWHMixed.isEmpty(WaterHeater_MixedFields::UseSideInletNodeName));
    ASSERT_FALSE(idfWHMixed.isEmpty(WaterHeater_MixedFields::UseSideOutletNodeName));
    useSideOutletNodeName = idfWHMixed.getString(WaterHeater_MixedFields::UseSideOutletNodeName, false).get();
    EXPECT_NE("", useSideOutletNodeName);

    EXPECT_TRUE(idfWHMixed.isEmpty(WaterHeater_MixedFields::SourceSideInletNodeName));
    EXPECT_TRUE(idfWHMixed.isEmpty(WaterHeater_MixedFields::SourceSideOutletNodeName));

    EXPECT_EQ("", idfWHMixed.getString(WaterHeater_MixedFields::SourceSideInletNodeName, false).get());
    EXPECT_EQ("", idfWHMixed.getString(WaterHeater_MixedFields::SourceSideOutletNodeName, false).get());

    // Check PlantEquipmentOperationScheme for Use Side
    {
      auto _wo = w.getObjectByTypeAndName(IddObjectType::PlantLoop, useSidePlantLoop.nameString());
      ASSERT_TRUE(_wo.is_initialized());
      WorkspaceObject idf_coolingPlant = _wo.get();
      WorkspaceObject idf_plant_op = idf_coolingPlant.getTarget(PlantLoopFields::PlantEquipmentOperationSchemeName).get();

      // Should have created a Heating Load one only
      ASSERT_EQ(1, idf_plant_op.extensibleGroups().size());
      auto w_eg = idf_plant_op.extensibleGroups()[0].cast<WorkspaceExtensibleGroup>();
      ASSERT_EQ("PlantEquipmentOperation:HeatingLoad", w_eg.getString(PlantEquipmentOperationSchemesExtensibleFields::ControlSchemeObjectType).get());

      // Get the Operation Scheme
      _wo = w_eg.getTarget(PlantEquipmentOperationSchemesExtensibleFields::ControlSchemeName);
      ASSERT_TRUE(_wo.is_initialized());
      WorkspaceObject idf_op_scheme = _wo.get();

      // Get the Plant Equipment List of this HeatingLoad scheme
      // There should only be one Load Range
      ASSERT_EQ(1, idf_op_scheme.extensibleGroups().size());
      // Load range 1
      w_eg = idf_op_scheme.extensibleGroups()[0].cast<WorkspaceExtensibleGroup>();
      _wo = w_eg.getTarget(PlantEquipmentOperation_HeatingLoadExtensibleFields::RangeEquipmentListName);
      ASSERT_TRUE(_wo.is_initialized());
      WorkspaceObject idf_peq_list = _wo.get();

      // Should have one equipment on it: WaterHeater
      ASSERT_EQ(1, idf_peq_list.extensibleGroups().size());
      IdfExtensibleGroup idf_eg(idf_peq_list.extensibleGroups()[0]);

      ASSERT_EQ(wh.nameString(), idf_eg.getString(PlantEquipmentListExtensibleFields::EquipmentName).get());
    }
  }

  // Now add connect it on a supply side Source plant
  PlantLoop sourceSidePlantLoop(m);
  EXPECT_TRUE(sourceSidePlantLoop.addSupplyBranchForComponent(wh));

  ASSERT_TRUE(wh.supplyInletModelObject());
  ASSERT_TRUE(wh.supplyOutletModelObject());
  ASSERT_TRUE(wh.demandInletModelObject());
  ASSERT_TRUE(wh.demandOutletModelObject());

  {
    Workspace w = ft.translateModel(m);

    EXPECT_EQ(2, w.getObjectsByType(IddObjectType::PlantLoop).size());

    std::vector<WorkspaceObject> idfWHMixeds(w.getObjectsByType(IddObjectType::WaterHeater_Mixed));
    ASSERT_EQ(1, idfWHMixeds.size());
    WorkspaceObject idfWHMixed(idfWHMixeds[0]);

    EXPECT_FALSE(idfWHMixed.isEmpty(WaterHeater_MixedFields::UseSideInletNodeName));
    EXPECT_FALSE(idfWHMixed.isEmpty(WaterHeater_MixedFields::UseSideOutletNodeName));
    EXPECT_FALSE(idfWHMixed.isEmpty(WaterHeater_MixedFields::SourceSideInletNodeName));
    EXPECT_FALSE(idfWHMixed.isEmpty(WaterHeater_MixedFields::SourceSideOutletNodeName));

    EXPECT_EQ(wh.supplyInletModelObject()->nameString(), idfWHMixed.getString(WaterHeater_MixedFields::UseSideInletNodeName, false).get());
    EXPECT_EQ(wh.supplyOutletModelObject()->nameString(), idfWHMixed.getString(WaterHeater_MixedFields::UseSideOutletNodeName, false).get());
    EXPECT_EQ(useSideOutletNodeName, idfWHMixed.getString(WaterHeater_MixedFields::UseSideOutletNodeName, false).get());  // doesn't change
    EXPECT_EQ(wh.demandInletModelObject()->nameString(), idfWHMixed.getString(WaterHeater_MixedFields::SourceSideInletNodeName, false).get());
    EXPECT_EQ(wh.demandOutletModelObject()->nameString(), idfWHMixed.getString(WaterHeater_MixedFields::SourceSideOutletNodeName, false).get());

    // Check PlantEquipmentOperationScheme for Use Side AND Source side
    for (const auto& plant : {useSidePlantLoop, sourceSidePlantLoop}) {
      auto _wo = w.getObjectByTypeAndName(IddObjectType::PlantLoop, plant.nameString());
      ASSERT_TRUE(_wo.is_initialized());
      WorkspaceObject idf_coolingPlant = _wo.get();
      WorkspaceObject idf_plant_op = idf_coolingPlant.getTarget(PlantLoopFields::PlantEquipmentOperationSchemeName).get();

      // Should have created a Heating Load one only
      ASSERT_EQ(1, idf_plant_op.extensibleGroups().size());
      auto w_eg = idf_plant_op.extensibleGroups()[0].cast<WorkspaceExtensibleGroup>();
      ASSERT_EQ("PlantEquipmentOperation:HeatingLoad", w_eg.getString(PlantEquipmentOperationSchemesExtensibleFields::ControlSchemeObjectType).get());

      // Get the Operation Scheme
      _wo = w_eg.getTarget(PlantEquipmentOperationSchemesExtensibleFields::ControlSchemeName);
      ASSERT_TRUE(_wo.is_initialized());
      WorkspaceObject idf_op_scheme = _wo.get();

      // Get the Plant Equipment List of this HeatingLoad scheme
      // There should only be one Load Range
      ASSERT_EQ(1, idf_op_scheme.extensibleGroups().size());
      // Load range 1
      w_eg = idf_op_scheme.extensibleGroups()[0].cast<WorkspaceExtensibleGroup>();
      _wo = w_eg.getTarget(PlantEquipmentOperation_HeatingLoadExtensibleFields::RangeEquipmentListName);
      ASSERT_TRUE(_wo.is_initialized());
      WorkspaceObject idf_peq_list = _wo.get();

      // Should have one equipment on it: WaterHeater
      ASSERT_EQ(1, idf_peq_list.extensibleGroups().size());
      IdfExtensibleGroup idf_eg(idf_peq_list.extensibleGroups()[0]);

      ASSERT_EQ(wh.nameString(), idf_eg.getString(PlantEquipmentListExtensibleFields::EquipmentName).get());
    }
  }
}

TEST_F(EnergyPlusFixture, ForwardTranslatorWaterHeaterMixed_InvalidValue) {
  // Test for #4477 - Improve handling of invalid values passed to SDK

  for (StrictnessLevel strictnessLevel : {StrictnessLevel::None, StrictnessLevel::Minimal, StrictnessLevel::Draft}) {

    // make sure a valid value works across strictness levels
    {
      Model m;
      EXPECT_TRUE(m.setStrictnessLevel(strictnessLevel));
      EXPECT_EQ(m.strictnessLevel(), strictnessLevel);

      WaterHeaterMixed wh(m);
      PlantLoop p(m);
      EXPECT_TRUE(p.addSupplyBranchForComponent(wh));

      double effValue = 0.5;

      EXPECT_TRUE(wh.setHeaterThermalEfficiency(effValue));
      EXPECT_EQ(effValue, wh.heaterThermalEfficiency().get());

      ForwardTranslator ft;
      ASSERT_NO_THROW(ft.translateModel(m));
      Workspace w = ft.translateModel(m);

      WorkspaceObjectVector idf_whs(w.getObjectsByType(IddObjectType::WaterHeater_Mixed));
      EXPECT_EQ(1u, idf_whs.size());
      WorkspaceObject idf_wh(idf_whs[0]);

      EXPECT_EQ(effValue, idf_wh.getDouble(WaterHeater_MixedFields::HeaterThermalEfficiency, false).get());
    }

    // check setting of invalid value across strictness levels
    {
      Model m;
      EXPECT_TRUE(m.setStrictnessLevel(strictnessLevel));
      EXPECT_EQ(m.strictnessLevel(), strictnessLevel);

      WaterHeaterMixed wh(m);
      PlantLoop p(m);
      EXPECT_TRUE(p.addSupplyBranchForComponent(wh));

      double effValue = -0.5;

      if (strictnessLevel > StrictnessLevel::Minimal) {
        EXPECT_FALSE(wh.setHeaterThermalEfficiency(effValue));
        EXPECT_NE(effValue, wh.heaterThermalEfficiency().get());
      } else {
        EXPECT_TRUE(wh.setHeaterThermalEfficiency(effValue));
        EXPECT_EQ(effValue, wh.heaterThermalEfficiency().get());
      }

      ForwardTranslator ft;
      ASSERT_NO_THROW(ft.translateModel(m));
      Workspace w = ft.translateModel(m);

      WorkspaceObjectVector idf_whs(w.getObjectsByType(IddObjectType::WaterHeater_Mixed));
      EXPECT_EQ(1u, idf_whs.size());
      WorkspaceObject idf_wh(idf_whs[0]);

      if (strictnessLevel > StrictnessLevel::Minimal) {
        EXPECT_NE(effValue, idf_wh.getDouble(WaterHeater_MixedFields::HeaterThermalEfficiency, false).get());
      } else {
        EXPECT_EQ(effValue, idf_wh.getDouble(WaterHeater_MixedFields::HeaterThermalEfficiency, false).get());
      }
    }

    // check pointers across strictness levels
    {
      Model m;
      EXPECT_TRUE(m.setStrictnessLevel(strictnessLevel));
      EXPECT_EQ(m.strictnessLevel(), strictnessLevel);

      ScheduleConstant sch(m);
      WaterHeaterMixed wh(m);

      double effValue = -0.5;

      EXPECT_TRUE(wh.setSetpointTemperatureSchedule(sch));
      EXPECT_TRUE(wh.setpointTemperatureSchedule());  // begin with a pointer

      wh.setHeaterThermalEfficiency(effValue);
      EXPECT_TRUE(wh.setpointTemperatureSchedule());  // still has pointer

      EXPECT_TRUE(wh.setSetpointTemperatureSchedule(sch));
      EXPECT_TRUE(wh.setpointTemperatureSchedule());  // able to set pointer

      if (strictnessLevel > StrictnessLevel::Minimal) {
        EXPECT_NE(effValue, wh.heaterThermalEfficiency().get());
      } else {
        EXPECT_EQ(effValue, wh.heaterThermalEfficiency().get());  // still has invalid value
      }
    }
  }
}
