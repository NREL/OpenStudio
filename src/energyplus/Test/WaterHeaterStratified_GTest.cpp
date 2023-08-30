/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>
#include "EnergyPlusFixture.hpp"

#include "../ForwardTranslator.hpp"
#include "../ReverseTranslator.hpp"

#include "../../model/Model.hpp"
#include "../../model/WaterHeaterStratified.hpp"
// #include "../../model/WaterHeaterStratified_Impl.hpp"
#include "../../model/Schedule.hpp"

#include "../../model/PlantLoop.hpp"
#include "../../model/PlantLoop_Impl.hpp"
#include "../../model/PipeAdiabatic.hpp"
#include "../../model/PipeAdiabatic_Impl.hpp"
#include "../../model/Node.hpp"
#include "../../model/Node_Impl.hpp"

#include "../../utilities/idf/IdfFile.hpp"
#include "../../utilities/idf/Workspace.hpp"
#include "../../utilities/idf/IdfObject.hpp"
#include "../../utilities/idf/WorkspaceObject.hpp"
#include "../../utilities/idf/IdfExtensibleGroup.hpp"
#include "../../utilities/idf/WorkspaceExtensibleGroup.hpp"

#include <utilities/idd/WaterHeater_Stratified_FieldEnums.hxx>
#include <utilities/idd/PlantLoop_FieldEnums.hxx>
#include <utilities/idd/PlantEquipmentList_FieldEnums.hxx>
#include <utilities/idd/PlantEquipmentOperationSchemes_FieldEnums.hxx>
#include <utilities/idd/PlantEquipmentOperation_HeatingLoad_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::energyplus;
using namespace openstudio::model;
using namespace openstudio;

// Test for #1675
TEST_F(EnergyPlusFixture, ForwardTranslatorWaterHeaterStratified_Condition) {
  ForwardTranslator ft;
  Model m;

  WaterHeaterStratified wh(m);
  EXPECT_FALSE(wh.peakUseFlowRate());
  EXPECT_FALSE(wh.useFlowRateFractionSchedule());

  // Not on a PlantLoop, and no Peak use Flow rate => Not translated
  {
    Workspace w = ft.translateModel(m);
    EXPECT_EQ(0u, w.getObjectsByType(IddObjectType::WaterHeater_Stratified).size());
  }

  // Has a peak flow rate, but still missing the Flow Rate schedule
  EXPECT_TRUE(wh.setPeakUseFlowRate(0.1));
  EXPECT_TRUE(wh.peakUseFlowRate());
  {
    Workspace w = ft.translateModel(m);
    EXPECT_EQ(0u, w.getObjectsByType(IddObjectType::WaterHeater_Stratified).size());
  }

  // Has both required conditions for standalone
  auto alwaysOnContinuous = m.alwaysOnContinuousSchedule();
  EXPECT_TRUE(wh.setUseFlowRateFractionSchedule(alwaysOnContinuous));
  EXPECT_TRUE(wh.useFlowRateFractionSchedule());
  {
    Workspace w = ft.translateModel(m);
    EXPECT_EQ(1u, w.getObjectsByType(IddObjectType::WaterHeater_Stratified).size());
  }

  // Now it has the schedule, but not the peak use flow rate
  wh.resetPeakUseFlowRate();
  EXPECT_FALSE(wh.peakUseFlowRate());
  {
    Workspace w = ft.translateModel(m);
    EXPECT_EQ(0u, w.getObjectsByType(IddObjectType::WaterHeater_Stratified).size());
  }

  // Now it has neither
  wh.resetUseFlowRateFractionSchedule();
  EXPECT_FALSE(wh.useFlowRateFractionSchedule());
  {
    Workspace w = ft.translateModel(m);
    EXPECT_EQ(0u, w.getObjectsByType(IddObjectType::WaterHeater_Stratified).size());
  }

  PlantLoop p(m);
  // Add a supply branch for the wh
  EXPECT_TRUE(p.addSupplyBranchForComponent(wh));
  {
    Workspace w = ft.translateModel(m);
    EXPECT_EQ(1u, w.getObjectsByType(IddObjectType::WaterHeater_Stratified).size());
  }
}

TEST_F(EnergyPlusFixture, ForwardTranslatorWaterHeaterStratified_PlantLoopConnections) {
  ForwardTranslator ft;
  Model m;

  WaterHeaterStratified wh(m);

  PlantLoop useSidePlantLoop(m);

  EXPECT_TRUE(useSidePlantLoop.addSupplyBranchForComponent(wh));
  std::string useSideOutletNodeName;
  {
    Workspace w = ft.translateModel(m);

    EXPECT_EQ(1, w.getObjectsByType(IddObjectType::PlantLoop).size());

    std::vector<WorkspaceObject> idfWHStratifieds(w.getObjectsByType(IddObjectType::WaterHeater_Stratified));
    ASSERT_EQ(1, idfWHStratifieds.size());
    WorkspaceObject idfWHStratified(idfWHStratifieds[0]);

    EXPECT_FALSE(idfWHStratified.isEmpty(WaterHeater_StratifiedFields::UseSideInletNodeName));
    ASSERT_FALSE(idfWHStratified.isEmpty(WaterHeater_StratifiedFields::UseSideOutletNodeName));
    useSideOutletNodeName = idfWHStratified.getString(WaterHeater_StratifiedFields::UseSideOutletNodeName, false).get();
    EXPECT_NE("", useSideOutletNodeName);

    EXPECT_TRUE(idfWHStratified.isEmpty(WaterHeater_StratifiedFields::SourceSideInletNodeName));
    EXPECT_TRUE(idfWHStratified.isEmpty(WaterHeater_StratifiedFields::SourceSideOutletNodeName));

    EXPECT_EQ("", idfWHStratified.getString(WaterHeater_StratifiedFields::SourceSideInletNodeName, false).get());
    EXPECT_EQ("", idfWHStratified.getString(WaterHeater_StratifiedFields::SourceSideOutletNodeName, false).get());

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

    std::vector<WorkspaceObject> idfWHStratifieds(w.getObjectsByType(IddObjectType::WaterHeater_Stratified));
    ASSERT_EQ(1, idfWHStratifieds.size());
    WorkspaceObject idfWHStratified(idfWHStratifieds[0]);

    EXPECT_FALSE(idfWHStratified.isEmpty(WaterHeater_StratifiedFields::UseSideInletNodeName));
    EXPECT_FALSE(idfWHStratified.isEmpty(WaterHeater_StratifiedFields::UseSideOutletNodeName));
    EXPECT_FALSE(idfWHStratified.isEmpty(WaterHeater_StratifiedFields::SourceSideInletNodeName));
    EXPECT_FALSE(idfWHStratified.isEmpty(WaterHeater_StratifiedFields::SourceSideOutletNodeName));

    EXPECT_EQ(wh.supplyInletModelObject()->nameString(), idfWHStratified.getString(WaterHeater_StratifiedFields::UseSideInletNodeName, false).get());
    EXPECT_EQ(wh.supplyOutletModelObject()->nameString(),
              idfWHStratified.getString(WaterHeater_StratifiedFields::UseSideOutletNodeName, false).get());
    EXPECT_EQ(useSideOutletNodeName, idfWHStratified.getString(WaterHeater_StratifiedFields::UseSideOutletNodeName, false).get());  // doesn't change
    EXPECT_EQ(wh.demandInletModelObject()->nameString(),
              idfWHStratified.getString(WaterHeater_StratifiedFields::SourceSideInletNodeName, false).get());
    EXPECT_EQ(wh.demandOutletModelObject()->nameString(),
              idfWHStratified.getString(WaterHeater_StratifiedFields::SourceSideOutletNodeName, false).get());

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
