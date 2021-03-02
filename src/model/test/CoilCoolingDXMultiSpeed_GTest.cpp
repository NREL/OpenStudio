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
#include "ModelFixture.hpp"
#include "../CoilCoolingDXMultiSpeed.hpp"
#include "../CoilCoolingDXMultiSpeed_Impl.hpp"
#include "../CoilCoolingDXMultiSpeedStageData.hpp"
#include "../CoilCoolingDXMultiSpeedStageData_Impl.hpp"
#include "../CurveBiquadratic.hpp"
#include "../CurveBiquadratic_Impl.hpp"
#include "../CurveQuadratic.hpp"
#include "../CurveQuadratic_Impl.hpp"

using namespace openstudio;
using namespace openstudio::model;

CoilCoolingDXMultiSpeedStageData makeStage(Model& model) {
  CurveBiquadratic cooling_curve_1(model);
  cooling_curve_1.setCoefficient1Constant(0.766956);
  cooling_curve_1.setCoefficient2x(0.0107756);
  cooling_curve_1.setCoefficient3xPOW2(-0.0000414703);
  cooling_curve_1.setCoefficient4y(0.00134961);
  cooling_curve_1.setCoefficient5yPOW2(-0.000261144);
  cooling_curve_1.setCoefficient6xTIMESY(0.000457488);
  cooling_curve_1.setMinimumValueofx(17.0);
  cooling_curve_1.setMaximumValueofx(22.0);
  cooling_curve_1.setMinimumValueofy(13.0);
  cooling_curve_1.setMaximumValueofy(46.0);

  CurveQuadratic cooling_curve_2(model);
  cooling_curve_2.setCoefficient1Constant(0.8);
  cooling_curve_2.setCoefficient2x(0.2);
  cooling_curve_2.setCoefficient3xPOW2(0.0);
  cooling_curve_2.setMinimumValueofx(0.5);
  cooling_curve_2.setMaximumValueofx(1.5);

  CurveBiquadratic cooling_curve_3(model);
  cooling_curve_3.setCoefficient1Constant(0.297145);
  cooling_curve_3.setCoefficient2x(0.0430933);
  cooling_curve_3.setCoefficient3xPOW2(-0.000748766);
  cooling_curve_3.setCoefficient4y(0.00597727);
  cooling_curve_3.setCoefficient5yPOW2(0.000482112);
  cooling_curve_3.setCoefficient6xTIMESY(-0.000956448);
  cooling_curve_3.setMinimumValueofx(17.0);
  cooling_curve_3.setMaximumValueofx(22.0);
  cooling_curve_3.setMinimumValueofy(13.0);
  cooling_curve_3.setMaximumValueofy(46.0);

  CurveQuadratic cooling_curve_4(model);
  cooling_curve_4.setCoefficient1Constant(1.156);
  cooling_curve_4.setCoefficient2x(-0.1816);
  cooling_curve_4.setCoefficient3xPOW2(0.0256);
  cooling_curve_4.setMinimumValueofx(0.5);
  cooling_curve_4.setMaximumValueofx(1.5);

  CurveQuadratic cooling_curve_5(model);
  cooling_curve_5.setCoefficient1Constant(0.75);
  cooling_curve_5.setCoefficient2x(0.25);
  cooling_curve_5.setCoefficient3xPOW2(0.0);
  cooling_curve_5.setMinimumValueofx(0.0);
  cooling_curve_5.setMaximumValueofx(1.0);

  CurveBiquadratic cooling_curve_6(model);
  cooling_curve_6.setCoefficient1Constant(1.0);
  cooling_curve_6.setCoefficient2x(0.0);
  cooling_curve_6.setCoefficient3xPOW2(0.0);
  cooling_curve_6.setCoefficient4y(0.0);
  cooling_curve_6.setCoefficient5yPOW2(0.0);
  cooling_curve_6.setCoefficient6xTIMESY(0.0);
  cooling_curve_6.setMinimumValueofx(0.0);
  cooling_curve_6.setMaximumValueofx(0.0);
  cooling_curve_6.setMinimumValueofy(0.0);
  cooling_curve_6.setMaximumValueofy(0.0);

  CoilCoolingDXMultiSpeedStageData stage(model, cooling_curve_1, cooling_curve_2, cooling_curve_3, cooling_curve_4, cooling_curve_5, cooling_curve_6);

  return stage;
}

TEST_F(ModelFixture, CoilCoolingDXMultiSpeed_DefaultConstructors) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  ASSERT_EXIT(
    {
      Model m;
      CoilCoolingDXMultiSpeed coil(m);

      exit(0);
    },
    ::testing::ExitedWithCode(0), "");
}

TEST_F(ModelFixture, CoilCoolingDXMultiSpeed_Stages_API) {
  Model m;
  CoilCoolingDXMultiSpeed coil(m);

  auto stage1 = makeStage(m);
  coil.addStage(stage1);

  auto stage2 = makeStage(m);
  coil.addStage(stage2);

  ASSERT_EQ(2u, coil.stages().size());

  // #3976 - Minimum Outdoor Dry-Bulb Temperature for Compressor Operation
  // IDD Default
  EXPECT_EQ(-25.0, coil.minimumOutdoorDryBulbTemperatureforCompressorOperation());
  // There are no IDD limits, so everything should work
  EXPECT_TRUE(coil.setMinimumOutdoorDryBulbTemperatureforCompressorOperation(-5));
  EXPECT_EQ(-5, coil.minimumOutdoorDryBulbTemperatureforCompressorOperation());
}
TEST_F(ModelFixture, CoilCoolingDXMultiSpeed_Stages) {
  Model model;
  CoilCoolingDXMultiSpeed dx(model);

  std::vector<CoilCoolingDXMultiSpeedStageData> stages;
  for (unsigned i = 1; i <= 4; ++i) {
    CoilCoolingDXMultiSpeedStageData stage(model);
    stage.setName("Stage " + std::to_string(i));
    stages.push_back(stage);
    EXPECT_TRUE(dx.addStage(stage));
    EXPECT_EQ(i, dx.numberOfStages());
    EXPECT_EQ(stages, dx.stages());
  }

  // Can't add more than 4 Stages;
  CoilCoolingDXMultiSpeedStageData anotherStage(model);
  EXPECT_FALSE(dx.addStage(anotherStage));
  EXPECT_EQ(4, dx.numberOfStages());
  EXPECT_EQ(stages, dx.stages());

  // Can't remove a stage that's not in there...
  EXPECT_FALSE(dx.removeStage(anotherStage));
  EXPECT_EQ(4, dx.numberOfStages());
  EXPECT_EQ(stages, dx.stages());

  {
    int stageIndex = 3;
    std::vector<CoilCoolingDXMultiSpeedStageData> thisStages = dx.stages();
    const auto& stageAtIndex = thisStages[stageIndex - 1];
    EXPECT_TRUE(std::find(thisStages.begin(), thisStages.end(), stageAtIndex) != thisStages.end());
    auto optIndex = dx.stageIndex(stageAtIndex);
    ASSERT_TRUE(optIndex);
    EXPECT_EQ(stageIndex, optIndex.get());
    EXPECT_TRUE(dx.removeStage(stageIndex));
    EXPECT_EQ(3, dx.numberOfStages());
    thisStages = dx.stages();
    EXPECT_FALSE(std::find(thisStages.begin(), thisStages.end(), stageAtIndex) != thisStages.end());
    // Do the same on our vector, so we're up to date...
    stages.erase(stages.begin() + stageIndex - 1);
    EXPECT_EQ(stages, dx.stages());
  }

  {
    int stageIndex = 2;
    std::vector<CoilCoolingDXMultiSpeedStageData> thisStages = dx.stages();
    const auto& stageAtIndex = thisStages[stageIndex - 1];
    EXPECT_TRUE(std::find(thisStages.begin(), thisStages.end(), stageAtIndex) != thisStages.end());
    auto optIndex = dx.stageIndex(stageAtIndex);
    ASSERT_TRUE(optIndex);
    EXPECT_EQ(stageIndex, optIndex.get());
    EXPECT_TRUE(dx.removeStage(stageAtIndex));
    EXPECT_EQ(2, dx.numberOfStages());
    thisStages = dx.stages();
    EXPECT_FALSE(std::find(thisStages.begin(), thisStages.end(), stageAtIndex) != thisStages.end());
    // Do the same on our vector, so we're up to date...
    stages.erase(std::find(stages.begin(), stages.end(), stageAtIndex));
    EXPECT_EQ(stages, dx.stages());
  }

  dx.removeAllStages();
  EXPECT_EQ(0, dx.numberOfStages());

  EXPECT_TRUE(dx.setStages(stages));
  EXPECT_EQ(2, dx.numberOfStages());
  EXPECT_EQ(stages, dx.stages());

  for (unsigned i = 5; i <= 7; ++i) {
    CoilCoolingDXMultiSpeedStageData stage(model);
    dx.setName("Stage " + std::to_string(i));
    stages.push_back(stage);
  }
  EXPECT_EQ(5u, stages.size());
  dx.removeAllStages();
  EXPECT_TRUE(dx.addStage(anotherStage));

  // This should clear, then assign the first 4, but then return false since the 5th failed
  EXPECT_FALSE(dx.setStages(stages));
  EXPECT_EQ(4, dx.numberOfStages());
  {
    std::vector<CoilCoolingDXMultiSpeedStageData> thisStages = dx.stages();
    for (unsigned i = 0; i < 4; ++i) {
      EXPECT_EQ(stages[i], thisStages[i]);
    }
  }
  stages.pop_back();
  EXPECT_EQ(4u, stages.size());

  {
    const auto& stageAtEnd = stages.back();
    auto optIndex = dx.stageIndex(stageAtEnd);
    ASSERT_TRUE(optIndex);
    EXPECT_EQ(dx.numberOfStages(), optIndex.get());

    EXPECT_TRUE(dx.setStageIndex(stageAtEnd, 2));
    std::vector<CoilCoolingDXMultiSpeedStageData> thisStages = dx.stages();
    optIndex = dx.stageIndex(stageAtEnd);
    ASSERT_TRUE(optIndex);
    EXPECT_EQ(2, optIndex.get());
    EXPECT_EQ(4, dx.numberOfStages());
    for (unsigned i = 1; i <= dx.numberOfStages(); ++i) {
      if (i < optIndex.get()) {
        EXPECT_EQ(stages[i - 1], dx.stages()[i - 1]);
      } else if (i > optIndex.get()) {
        EXPECT_EQ(stages[i - 2], dx.stages()[i - 1]);
      }
    }
  }

  dx.removeAllStages();
  EXPECT_EQ(0u, dx.numExtensibleGroups());
  EXPECT_EQ(0u, dx.numberOfStages());
  EXPECT_EQ(0u, dx.stages().size());

  // Test that added a stage from another model will fail but not add a blank extensible group
  Model model2;
  CoilCoolingDXMultiSpeedStageData stageFromAnotherModel(model2);
  EXPECT_FALSE(dx.addStage(stageFromAnotherModel));
  EXPECT_EQ(0u, dx.numExtensibleGroups());
  EXPECT_EQ(0u, dx.numberOfStages());
  EXPECT_EQ(0u, dx.stages().size());
}
