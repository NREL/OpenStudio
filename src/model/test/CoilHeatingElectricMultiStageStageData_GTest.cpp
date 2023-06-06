/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>

#include "ModelFixture.hpp"

#include "../CoilHeatingElectricMultiStageStageData.hpp"
#include "../CoilHeatingElectricMultiStageStageData_Impl.hpp"
#include "../CoilHeatingElectricMultiStage.hpp"
#include "../CoilHeatingElectricMultiStage_Impl.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, CoilHeatingElectricMultiStageStageData_GettersSetters) {
  Model m;
  CoilHeatingElectricMultiStage coil(m);
  CoilHeatingElectricMultiStageStageData stageData1(m);
  CoilHeatingElectricMultiStageStageData stageData2(m);

  EXPECT_EQ(0u, coil.stages().size());
  EXPECT_FALSE(stageData1.parentCoil());
  EXPECT_TRUE(coil.addStage(stageData1));
  EXPECT_TRUE(coil.addStage(stageData2));
  EXPECT_TRUE(stageData1.parentCoil());
  EXPECT_EQ(2u, coil.stages().size());

  EXPECT_EQ(1.0, stageData1.efficiency());
  EXPECT_FALSE(stageData1.nominalCapacity());
  EXPECT_TRUE(stageData1.isNominalCapacityAutosized());

  EXPECT_TRUE(stageData1.setEfficiency(0.75));
  EXPECT_TRUE(stageData1.setNominalCapacity(1000.0));

  EXPECT_EQ(0.75, stageData1.efficiency());
  ASSERT_TRUE(stageData1.nominalCapacity());
  EXPECT_EQ(1000.0, stageData1.nominalCapacity().get());
  EXPECT_FALSE(stageData1.isNominalCapacityAutosized());

  stageData1.autosizeNominalCapacity();
  EXPECT_FALSE(stageData1.nominalCapacity());
  EXPECT_TRUE(stageData1.isNominalCapacityAutosized());
}

TEST_F(ModelFixture, CoilHeatingElectricMultiStageStageData_remove) {
  // If we remove a stage, we would like any CoilHeatingElectricMultiStage that use it to have their extensible groups adjusted
  Model model;

  CoilHeatingElectricMultiStageStageData stage1(model);
  CoilHeatingElectricMultiStageStageData stage2(model);

  CoilHeatingElectricMultiStage coil(model);
  EXPECT_TRUE(coil.addStage(stage1));
  EXPECT_TRUE(coil.addStage(stage2));

  EXPECT_EQ(2, coil.numExtensibleGroups());
  EXPECT_EQ(2, coil.numberOfStages());
  EXPECT_EQ(2, coil.stages().size());

  EXPECT_EQ(2, model.getConcreteModelObjects<CoilHeatingElectricMultiStageStageData>().size());

  stage1.remove();

  EXPECT_EQ(1, model.getConcreteModelObjects<CoilHeatingElectricMultiStageStageData>().size());

  EXPECT_EQ(1, coil.numExtensibleGroups());
  EXPECT_EQ(1, coil.numberOfStages());
  EXPECT_EQ(1, coil.stages().size());

  EXPECT_EQ(stage2, coil.stages().front());
}

TEST_F(ModelFixture, CoilHeatingElectricMultiStageStageData_Unicity) {
  Model model;

  CoilHeatingElectricMultiStageStageData stage(model);
  EXPECT_FALSE(stage.parentCoil());

  CoilHeatingElectricMultiStage coil1(model);
  EXPECT_TRUE(coil1.addStage(stage));
  ASSERT_TRUE(stage.parentCoil());
  EXPECT_EQ(coil1, stage.parentCoil().get());

  // StageData is already used, so refuse
  CoilHeatingElectricMultiStage coil2(model);
  EXPECT_FALSE(coil2.addStage(stage));
  EXPECT_EQ(1u, model.getConcreteModelObjects<CoilHeatingElectricMultiStageStageData>().size());

  EXPECT_EQ(1u, coil1.numExtensibleGroups());
  EXPECT_EQ(1u, coil1.numberOfStages());
  EXPECT_EQ(1u, coil1.stages().size());
  EXPECT_EQ(0u, coil2.numExtensibleGroups());
  EXPECT_EQ(0u, coil2.numberOfStages());
  EXPECT_EQ(0u, coil2.stages().size());
}
