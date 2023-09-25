/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>

#include "ModelFixture.hpp"

#include "../CoilHeatingElectricMultiStage.hpp"
#include "../CoilHeatingElectricMultiStage_Impl.hpp"
#include "../CoilHeatingElectricMultiStageStageData.hpp"
#include "../CoilHeatingElectricMultiStageStageData_Impl.hpp"
#include "../Schedule.hpp"
#include "../Schedule_Impl.hpp"
#include "../ScheduleConstant.hpp"
#include "../ScheduleConstant_Impl.hpp"
#include "../ScheduleTypeLimits.hpp"
#include "../ScheduleTypeLimits_Impl.hpp"
#include "../AirLoopHVAC.hpp"
#include "../Node.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, CoilHeatingElectricMultiStage_GettersSetters) {
  Model m;
  CoilHeatingElectricMultiStage coil(m);

  auto alwaysOn = m.alwaysOnDiscreteSchedule();
  EXPECT_EQ(alwaysOn, coil.availabilitySchedule());
  EXPECT_EQ(0, coil.stages().size());

  ScheduleConstant scheduleConstant(m);
  EXPECT_TRUE(coil.setAvailabilitySchedule(scheduleConstant));

  EXPECT_EQ(scheduleConstant, coil.availabilitySchedule());
}

TEST_F(ModelFixture, CoilHeatingElectricMultiStage_addToNode) {
  // Should not be allowed, only meant to be inside a Unitary
  Model m;

  CoilHeatingElectricMultiStage h(m);
  CoilHeatingElectricMultiStageStageData stageData(m);

  AirLoopHVAC a(m);
  Node supplyOutlet = a.supplyOutletNode();
  EXPECT_FALSE(h.addToNode(supplyOutlet));
}

TEST_F(ModelFixture, CoilHeatingElectricMultiStage_Stages) {
  Model model;
  CoilHeatingElectricMultiStage coil(model);

  std::vector<CoilHeatingElectricMultiStageStageData> stages;
  for (unsigned i = 1; i <= 4; ++i) {
    CoilHeatingElectricMultiStageStageData stage(model);
    stage.setName("Stage " + std::to_string(i));
    stages.push_back(stage);
    EXPECT_TRUE(coil.addStage(stage));
    EXPECT_EQ(i, coil.numberOfStages());
    EXPECT_EQ(stages, coil.stages());
  }

  // Can't add more than 4 Stages;
  CoilHeatingElectricMultiStageStageData anotherStage(model);
  EXPECT_FALSE(coil.addStage(anotherStage));
  EXPECT_EQ(4, coil.numberOfStages());
  EXPECT_EQ(stages, coil.stages());

  // Can't remove a stage that's not in there...
  EXPECT_FALSE(coil.removeStage(anotherStage));
  EXPECT_EQ(4, coil.numberOfStages());
  EXPECT_EQ(stages, coil.stages());

  {
    int stageIndex = 3;
    std::vector<CoilHeatingElectricMultiStageStageData> thisStages = coil.stages();
    // Explicit copy, so we can keep using it after it's been removed
    const auto stageAtIndex = thisStages[stageIndex - 1];
    EXPECT_TRUE(std::find(thisStages.begin(), thisStages.end(), stageAtIndex) != thisStages.end());
    auto optIndex = coil.stageIndex(stageAtIndex);
    ASSERT_TRUE(optIndex);
    EXPECT_EQ(stageIndex, optIndex.get());
    EXPECT_TRUE(coil.removeStage(stageIndex));
    EXPECT_EQ(3, coil.numberOfStages());
    thisStages = coil.stages();
    EXPECT_FALSE(std::find(thisStages.begin(), thisStages.end(), stageAtIndex) != thisStages.end());
    // Do the same on our vector, so we're up to date...
    stages.erase(stages.begin() + stageIndex - 1);
    EXPECT_EQ(stages, coil.stages());
  }

  {
    int stageIndex = 2;
    std::vector<CoilHeatingElectricMultiStageStageData> thisStages = coil.stages();
    // Explicit copy, so we can keep using it after it's been removed
    const auto stageAtIndex = thisStages[stageIndex - 1];
    EXPECT_TRUE(std::find(thisStages.begin(), thisStages.end(), stageAtIndex) != thisStages.end());
    auto optIndex = coil.stageIndex(stageAtIndex);
    ASSERT_TRUE(optIndex);
    EXPECT_EQ(stageIndex, optIndex.get());
    EXPECT_TRUE(coil.removeStage(stageAtIndex));
    EXPECT_EQ(2, coil.numberOfStages());
    thisStages = coil.stages();
    EXPECT_FALSE(std::find(thisStages.begin(), thisStages.end(), stageAtIndex) != thisStages.end());
    // Do the same on our vector, so we're up to date...
    stages.erase(std::find(stages.begin(), stages.end(), stageAtIndex));
    EXPECT_EQ(stages, coil.stages());
  }

  coil.removeAllStages();
  EXPECT_EQ(0, coil.numberOfStages());

  EXPECT_TRUE(coil.setStages(stages));
  EXPECT_EQ(2, coil.numberOfStages());
  EXPECT_EQ(stages, coil.stages());

  for (unsigned i = 5; i <= 7; ++i) {
    CoilHeatingElectricMultiStageStageData stage(model);
    coil.setName("Stage " + std::to_string(i));
    stages.push_back(stage);
  }
  EXPECT_EQ(5u, stages.size());
  coil.removeAllStages();
  EXPECT_TRUE(coil.addStage(anotherStage));

  // This should clear, then assign the first 4, but then return false since the 5th failed
  EXPECT_FALSE(coil.setStages(stages));
  EXPECT_EQ(4, coil.numberOfStages());
  {
    std::vector<CoilHeatingElectricMultiStageStageData> thisStages = coil.stages();
    for (unsigned i = 0; i < 4; ++i) {
      EXPECT_EQ(stages[i], thisStages[i]);
    }
  }
  stages.pop_back();
  EXPECT_EQ(4u, stages.size());

  {
    const auto& stageAtEnd = stages.back();
    auto optIndex = coil.stageIndex(stageAtEnd);
    ASSERT_TRUE(optIndex);
    EXPECT_EQ(coil.numberOfStages(), optIndex.get());

    EXPECT_TRUE(coil.setStageIndex(stageAtEnd, 2));
    std::vector<CoilHeatingElectricMultiStageStageData> thisStages = coil.stages();
    optIndex = coil.stageIndex(stageAtEnd);
    ASSERT_TRUE(optIndex);
    EXPECT_EQ(2, optIndex.get());
    EXPECT_EQ(4, coil.numberOfStages());
    for (unsigned i = 1; i <= coil.numberOfStages(); ++i) {
      if (i < optIndex.get()) {
        EXPECT_EQ(stages[i - 1], coil.stages()[i - 1]);
      } else if (i > optIndex.get()) {
        EXPECT_EQ(stages[i - 2], coil.stages()[i - 1]);
      }
    }
  }

  coil.removeAllStages();
  EXPECT_EQ(0u, coil.numExtensibleGroups());
  EXPECT_EQ(0u, coil.numberOfStages());
  EXPECT_EQ(0u, coil.stages().size());

  // Test that added a stage from another model will fail but not add a blank extensible group
  Model model2;
  CoilHeatingElectricMultiStageStageData stageFromAnotherModel(model2);
  EXPECT_FALSE(coil.addStage(stageFromAnotherModel));
  EXPECT_EQ(0u, coil.numExtensibleGroups());
  EXPECT_EQ(0u, coil.numberOfStages());
  EXPECT_EQ(0u, coil.stages().size());
}

TEST_F(ModelFixture, CoilHeatingElectricMultiStage_Remove) {
  Model m;

  CoilHeatingElectricMultiStage coil(m);
  CoilHeatingElectricMultiStageStageData stage(m);
  EXPECT_TRUE(coil.addStage(stage));
  EXPECT_EQ(1, m.getConcreteModelObjects<CoilHeatingElectricMultiStage>().size());
  EXPECT_EQ(1, m.getConcreteModelObjects<CoilHeatingElectricMultiStageStageData>().size());
  coil.remove();

  EXPECT_EQ(0, m.getConcreteModelObjects<CoilHeatingElectricMultiStage>().size());
  EXPECT_EQ(0, m.getConcreteModelObjects<CoilHeatingElectricMultiStageStageData>().size());
  auto schedules = m.getConcreteModelObjects<model::ScheduleConstant>();
  auto limits = m.getConcreteModelObjects<model::ScheduleTypeLimits>();
  EXPECT_EQ(schedules.size() + limits.size(), m.modelObjects().size());
}

TEST_F(ModelFixture, CoilHeatingElectricMultiStage_Clone) {
  // Ref #4663
  Model m;
  CoilHeatingElectricMultiStage coil(m);

  std::vector<CoilHeatingElectricMultiStageStageData> stages{CoilHeatingElectricMultiStageStageData{m}, CoilHeatingElectricMultiStageStageData{m}};
  EXPECT_TRUE(coil.setStages(stages));

  EXPECT_EQ(2, coil.stages().size());
  EXPECT_EQ(2, coil.numberOfStages());
  EXPECT_EQ(1, m.getConcreteModelObjects<CoilHeatingElectricMultiStage>().size());
  EXPECT_EQ(2, m.getConcreteModelObjects<CoilHeatingElectricMultiStageStageData>().size());

  {
    // Clone in another model first, so we don't affect the original one. Cloning also clones the stages
    Model m2;
    auto coilClone = coil.clone(m2).cast<CoilHeatingElectricMultiStage>();
    std::vector<CoilHeatingElectricMultiStageStageData> stages2 = coilClone.stages();
    EXPECT_EQ(2, stages2.size());
    EXPECT_EQ(1, m2.getConcreteModelObjects<CoilHeatingElectricMultiStage>().size());
    EXPECT_EQ(2, m2.getConcreteModelObjects<CoilHeatingElectricMultiStageStageData>().size());
    for (auto& stage : stages) {
      EXPECT_TRUE(std::find_if(stages2.cbegin(), stages2.cend(), [&stage](const auto& s) { return s.handle() == stage.handle(); }) == stages2.cend())
        << "Did not expect to find, in the cloned Coil, the CoilHeatingElectricMultiStageStageData '" << stage.nameString() << "'";
    }

    coilClone.remove();
    EXPECT_EQ(0, m2.getConcreteModelObjects<CoilHeatingElectricMultiStage>().size());
    EXPECT_EQ(0, m2.getConcreteModelObjects<CoilHeatingElectricMultiStageStageData>().size());
  }

  {
    // Same model: we expect stages to be cloned and reassigned to the clone
    auto coilClone = coil.clone(m).cast<CoilHeatingElectricMultiStage>();
    std::vector<CoilHeatingElectricMultiStageStageData> stages2 = coilClone.stages();
    EXPECT_EQ(2, stages2.size());
    EXPECT_EQ(2, m.getConcreteModelObjects<CoilHeatingElectricMultiStage>().size());
    EXPECT_EQ(4, m.getConcreteModelObjects<CoilHeatingElectricMultiStageStageData>().size());
    for (auto& stage : stages) {
      EXPECT_TRUE(std::find_if(stages2.cbegin(), stages2.cend(), [&stage](const auto& s) { return s.handle() == stage.handle(); }) == stages2.cend())
        << "Did not expect to find, in the cloned Coil, the CoilHeatingElectricMultiStageStageData '" << stage.nameString() << "'";
    }
    coil.remove();
    EXPECT_EQ(1, m.getConcreteModelObjects<CoilHeatingElectricMultiStage>().size());
    EXPECT_EQ(2, m.getConcreteModelObjects<CoilHeatingElectricMultiStageStageData>().size());

    coilClone.remove();
    EXPECT_EQ(0, m.getConcreteModelObjects<CoilHeatingElectricMultiStage>().size());
    EXPECT_EQ(0, m.getConcreteModelObjects<CoilHeatingElectricMultiStageStageData>().size());
  }
}
