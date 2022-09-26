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

#include "ModelFixture.hpp"

#include "../CoilHeatingGasMultiStage.hpp"
#include "../CoilHeatingGasMultiStage_Impl.hpp"
#include "../CoilHeatingGasMultiStageStageData.hpp"
#include "../CoilHeatingGasMultiStageStageData_Impl.hpp"
#include "../Schedule.hpp"
#include "../Schedule_Impl.hpp"
#include "../ScheduleConstant.hpp"
#include "../ScheduleConstant_Impl.hpp"
#include "../ScheduleTypeLimits.hpp"
#include "../ScheduleTypeLimits_Impl.hpp"
#include "../CurveQuadratic.hpp"
#include "../CurveQuadratic_Impl.hpp"
#include "../AirLoopHVAC.hpp"
#include "../Node.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, CoilHeatingGasMultiStage_GettersSetters) {
  Model m;
  CoilHeatingGasMultiStage coil(m);

  auto alwaysOn = m.alwaysOnDiscreteSchedule();
  EXPECT_EQ(alwaysOn, coil.availabilitySchedule());
  EXPECT_FALSE(coil.partLoadFractionCorrelationCurve());
  EXPECT_FALSE(coil.parasiticGasLoad());
  EXPECT_EQ(0u, coil.stages().size());

  ScheduleConstant scheduleConstant(m);
  EXPECT_TRUE(coil.setAvailabilitySchedule(scheduleConstant));
  CurveQuadratic curveQuadratic(m);
  EXPECT_TRUE(coil.setPartLoadFractionCorrelationCurve(curveQuadratic));
  EXPECT_TRUE(coil.setParasiticGasLoad(100.0));

  EXPECT_EQ(scheduleConstant, coil.availabilitySchedule());
  ASSERT_TRUE(coil.partLoadFractionCorrelationCurve());
  EXPECT_EQ(curveQuadratic.handle(), coil.partLoadFractionCorrelationCurve().get().handle());
  ASSERT_TRUE(coil.parasiticGasLoad());
  EXPECT_EQ(100.0, coil.parasiticGasLoad().get());

  coil.resetPartLoadFractionCorrelationCurve();
  coil.resetParasiticGasLoad();

  EXPECT_FALSE(coil.partLoadFractionCorrelationCurve());
  EXPECT_FALSE(coil.parasiticGasLoad());
  EXPECT_EQ(0u, coil.stages().size());
}

TEST_F(ModelFixture, CoilHeatingGasMultiStage_addToNode) {
  // Should not be allowed, only meant to be inside a Unitary
  Model m;

  CoilHeatingGasMultiStage h(m);
  CoilHeatingGasMultiStageStageData stageData(m);

  AirLoopHVAC a(m);
  Node supplyOutlet = a.supplyOutletNode();
  EXPECT_FALSE(h.addToNode(supplyOutlet));
}

TEST_F(ModelFixture, CoilHeatingGasMultiStage_Stages) {
  Model model;
  CoilHeatingGasMultiStage coil(model);

  std::vector<CoilHeatingGasMultiStageStageData> stages;
  for (unsigned i = 1; i <= 4; ++i) {
    CoilHeatingGasMultiStageStageData stage(model);
    stage.setName("Stage " + std::to_string(i));
    stages.push_back(stage);
    EXPECT_TRUE(coil.addStage(stage));
    EXPECT_EQ(i, coil.numberOfStages());
    EXPECT_EQ(stages, coil.stages());
  }

  // Can't add more than 4 Stages;
  CoilHeatingGasMultiStageStageData anotherStage(model);
  EXPECT_FALSE(coil.addStage(anotherStage));
  EXPECT_EQ(4, coil.numberOfStages());
  EXPECT_EQ(stages, coil.stages());

  // Can't remove a stage that's not in there...
  EXPECT_FALSE(coil.removeStage(anotherStage));
  EXPECT_EQ(4, coil.numberOfStages());
  EXPECT_EQ(stages, coil.stages());

  {
    int stageIndex = 3;
    std::vector<CoilHeatingGasMultiStageStageData> thisStages = coil.stages();
    const auto& stageAtIndex = thisStages[stageIndex - 1];
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
    std::vector<CoilHeatingGasMultiStageStageData> thisStages = coil.stages();
    const auto& stageAtIndex = thisStages[stageIndex - 1];
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
    CoilHeatingGasMultiStageStageData stage(model);
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
    std::vector<CoilHeatingGasMultiStageStageData> thisStages = coil.stages();
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
    std::vector<CoilHeatingGasMultiStageStageData> thisStages = coil.stages();
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
  CoilHeatingGasMultiStageStageData stageFromAnotherModel(model2);
  EXPECT_FALSE(coil.addStage(stageFromAnotherModel));
  EXPECT_EQ(0u, coil.numExtensibleGroups());
  EXPECT_EQ(0u, coil.numberOfStages());
  EXPECT_EQ(0u, coil.stages().size());
}

TEST_F(ModelFixture, CoilHeatingGasMultiStage_Remove) {
  Model m;

  CoilHeatingGasMultiStage coil(m);
  CoilHeatingGasMultiStageStageData stage(m);
  EXPECT_TRUE(coil.addStage(stage));
  EXPECT_EQ(1, m.getConcreteModelObjects<CoilHeatingGasMultiStage>().size());
  EXPECT_EQ(1, m.getConcreteModelObjects<CoilHeatingGasMultiStageStageData>().size());
  coil.remove();

  EXPECT_EQ(0, m.getConcreteModelObjects<CoilHeatingGasMultiStage>().size());
  EXPECT_EQ(0, m.getConcreteModelObjects<CoilHeatingGasMultiStageStageData>().size());
  auto schedules = m.getModelObjects<model::ScheduleConstant>();
  auto limits = m.getModelObjects<model::ScheduleTypeLimits>();
  EXPECT_EQ(schedules.size() + limits.size(), m.modelObjects().size());
}

TEST_F(ModelFixture, CoilHeatingGasMultiStage_Clone) {
  // Ref #4663
  Model m;
  CoilHeatingGasMultiStage coil(m);

  std::vector<CoilHeatingGasMultiStageStageData> stages{CoilHeatingGasMultiStageStageData{m}, CoilHeatingGasMultiStageStageData{m}};
  EXPECT_TRUE(coil.setStages(stages));

  EXPECT_EQ(2, coil.stages().size());
  EXPECT_EQ(2, coil.numberOfStages());
  EXPECT_EQ(1, m.getConcreteModelObjects<CoilHeatingGasMultiStage>().size());
  EXPECT_EQ(2, m.getConcreteModelObjects<CoilHeatingGasMultiStageStageData>().size());

  {
    // Clone in another model first, so we don't affect the original one. Cloning also clones the stages
    Model m2;
    auto coilClone = coil.clone(m2).cast<CoilHeatingGasMultiStage>();
    std::vector<CoilHeatingGasMultiStageStageData> stages2 = coilClone.stages();
    EXPECT_EQ(2, stages2.size());
    EXPECT_EQ(1, m2.getConcreteModelObjects<CoilHeatingGasMultiStage>().size());
    EXPECT_EQ(2, m2.getConcreteModelObjects<CoilHeatingGasMultiStageStageData>().size());
    for (auto& stage : stages) {
      EXPECT_TRUE(std::find_if(stages2.cbegin(), stages2.cend(), [&stage](const auto& s) { return s.handle() == stage.handle(); }) == stages2.cend())
        << "Did not expect to find, in the cloned Coil, the CoilHeatingGasMultiStageStageData '" << stage.nameString() << "'";
    }

    coilClone.remove();
    EXPECT_EQ(0, m2.getConcreteModelObjects<CoilHeatingGasMultiStage>().size());
    EXPECT_EQ(0, m2.getConcreteModelObjects<CoilHeatingGasMultiStageStageData>().size());
  }

  {
    // Same model: we expect stages to be cloned and reassigned to the clone
    auto coilClone = coil.clone(m).cast<CoilHeatingGasMultiStage>();
    std::vector<CoilHeatingGasMultiStageStageData> stages2 = coilClone.stages();
    EXPECT_EQ(2, stages2.size());
    EXPECT_EQ(2, m.getConcreteModelObjects<CoilHeatingGasMultiStage>().size());
    EXPECT_EQ(4, m.getConcreteModelObjects<CoilHeatingGasMultiStageStageData>().size());
    for (auto& stage : stages) {
      EXPECT_TRUE(std::find_if(stages2.cbegin(), stages2.cend(), [&stage](const auto& s) { return s.handle() == stage.handle(); }) == stages2.cend())
        << "Did not expect to find, in the cloned Coil, the CoilHeatingGasMultiStageStageData '" << stage.nameString() << "'";
    }
    coil.remove();
    EXPECT_EQ(1, m.getConcreteModelObjects<CoilHeatingGasMultiStage>().size());
    EXPECT_EQ(2, m.getConcreteModelObjects<CoilHeatingGasMultiStageStageData>().size());

    coilClone.remove();
    EXPECT_EQ(0, m.getConcreteModelObjects<CoilHeatingGasMultiStage>().size());
    EXPECT_EQ(0, m.getConcreteModelObjects<CoilHeatingGasMultiStageStageData>().size());
  }
}
