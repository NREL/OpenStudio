/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>

#include "ModelFixture.hpp"

#include "../CoilCoolingDXMultiSpeedStageData.hpp"
#include "../CoilCoolingDXMultiSpeedStageData_Impl.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, CoilCoolingDXMultiSpeedStageData_CoilCoolingDXMultiSpeedStageData) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  ASSERT_EXIT(
    {
      // create a model to use
      Model model;

      // create a coil cooling dx curve fit speed object to use
      CoilCoolingDXMultiSpeedStageData speed(model);

      exit(0);
    },
    ::testing::ExitedWithCode(0), "");
}

TEST_F(ModelFixture, CoilCoolingDXMultiSpeedStageData_Unicity) {
  Model model;

  CoilCoolingDXMultiSpeedStageData stage(model);
  EXPECT_FALSE(stage.parentCoil());

  CoilCoolingDXMultiSpeed dx1(model);
  EXPECT_TRUE(dx1.addStage(stage));
  ASSERT_TRUE(stage.parentCoil());
  EXPECT_EQ(dx1, stage.parentCoil().get());

  // StageData is already used, so refuse
  CoilCoolingDXMultiSpeed dx2(model);
  EXPECT_FALSE(dx2.addStage(stage));
  EXPECT_EQ(1u, model.getConcreteModelObjects<CoilCoolingDXMultiSpeedStageData>().size());

  EXPECT_EQ(1u, dx1.numExtensibleGroups());
  EXPECT_EQ(1u, dx1.numberOfStages());
  EXPECT_EQ(1u, dx1.stages().size());
  EXPECT_EQ(0u, dx2.numExtensibleGroups());
  EXPECT_EQ(0u, dx2.numberOfStages());
  EXPECT_EQ(0u, dx2.stages().size());
}

TEST_F(ModelFixture, CoilCoolingDXMultiSpeedStageData_remove) {
  // If we remove a stage, we would like any CoilCoolingDXMultiSpeed that use it to have their extensible groups adjusted
  Model model;

  CoilCoolingDXMultiSpeedStageData stage1(model);
  CoilCoolingDXMultiSpeedStageData stage2(model);

  CoilCoolingDXMultiSpeed dx1(model);
  EXPECT_TRUE(dx1.addStage(stage1));
  EXPECT_TRUE(dx1.addStage(stage2));

  EXPECT_EQ(2u, dx1.numExtensibleGroups());
  EXPECT_EQ(2u, dx1.numberOfStages());
  EXPECT_EQ(2u, dx1.stages().size());

  EXPECT_EQ(2u, model.getConcreteModelObjects<CoilCoolingDXMultiSpeedStageData>().size());

  stage1.remove();

  EXPECT_EQ(1u, model.getConcreteModelObjects<CoilCoolingDXMultiSpeedStageData>().size());

  EXPECT_EQ(1u, dx1.numExtensibleGroups());
  EXPECT_EQ(1u, dx1.numberOfStages());
  EXPECT_EQ(1u, dx1.stages().size());

  EXPECT_EQ(stage2, dx1.stages()[0]);
}

TEST_F(ModelFixture, CoilCoolingDXMultiSpeedStageData_2017and2023) {
  Model model;

  CoilCoolingDXMultiSpeedStageData speed(model);

  EXPECT_EQ(773.3, speed.ratedEvaporatorFanPowerPerVolumeFlowRate2017());
  EXPECT_EQ(934.4, speed.ratedEvaporatorFanPowerPerVolumeFlowRate2023());

  EXPECT_TRUE(speed.setRatedEvaporatorFanPowerPerVolumeFlowRate2017(8.0));
  EXPECT_TRUE(speed.setRatedEvaporatorFanPowerPerVolumeFlowRate2023(9.0));

  EXPECT_EQ(8.0, speed.ratedEvaporatorFanPowerPerVolumeFlowRate2017());
  EXPECT_EQ(9.0, speed.ratedEvaporatorFanPowerPerVolumeFlowRate2023());
}
