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

#include "../CoilHeatingDXMultiSpeedStageData.hpp"
#include "../CoilHeatingDXMultiSpeedStageData_Impl.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, CoilHeatingDXMultiSpeedStageData_CoilHeatingDXMultiSpeedStageData) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  ASSERT_EXIT(
    {
      // create a model to use
      Model model;

      // create a coil cooling dx curve fit speed object to use
      CoilHeatingDXMultiSpeedStageData speed(model);

      exit(0);
    },
    ::testing::ExitedWithCode(0), "");
}

TEST_F(ModelFixture, CoilHeatingDXMultiSpeedStageData_Unicity) {
  Model model;

  CoilHeatingDXMultiSpeedStageData stage(model);
  EXPECT_FALSE(stage.parentCoil());

  CoilHeatingDXMultiSpeed dx1(model);
  EXPECT_TRUE(dx1.addStage(stage));
  ASSERT_TRUE(stage.parentCoil());
  EXPECT_EQ(dx1, stage.parentCoil().get());

  // StageData is already used, so refuse
  CoilHeatingDXMultiSpeed dx2(model);
  EXPECT_FALSE(dx2.addStage(stage));
  EXPECT_EQ(1u, model.getConcreteModelObjects<CoilHeatingDXMultiSpeedStageData>().size());

  EXPECT_EQ(1u, dx1.numExtensibleGroups());
  EXPECT_EQ(1u, dx1.numberOfStages());
  EXPECT_EQ(1u, dx1.stages().size());
  EXPECT_EQ(0u, dx2.numExtensibleGroups());
  EXPECT_EQ(0u, dx2.numberOfStages());
  EXPECT_EQ(0u, dx2.stages().size());
}

TEST_F(ModelFixture, CoilHeatingDXMultiSpeedStageData_remove) {
  // If we remove a stage, we would like any CoilHeatingDXMultiSpeed that use it to have their extensible groups adjusted
  Model model;

  CoilHeatingDXMultiSpeedStageData stage1(model);
  CoilHeatingDXMultiSpeedStageData stage2(model);

  CoilHeatingDXMultiSpeed dx1(model);
  EXPECT_TRUE(dx1.addStage(stage1));
  EXPECT_TRUE(dx1.addStage(stage2));

  EXPECT_EQ(2u, dx1.numExtensibleGroups());
  EXPECT_EQ(2u, dx1.numberOfStages());
  EXPECT_EQ(2u, dx1.stages().size());

  EXPECT_EQ(2u, model.getConcreteModelObjects<CoilHeatingDXMultiSpeedStageData>().size());

  stage1.remove();

  EXPECT_EQ(1u, model.getConcreteModelObjects<CoilHeatingDXMultiSpeedStageData>().size());

  EXPECT_EQ(1u, dx1.numExtensibleGroups());
  EXPECT_EQ(1u, dx1.numberOfStages());
  EXPECT_EQ(1u, dx1.stages().size());

  EXPECT_EQ(stage2, dx1.stages()[0]);
}

TEST_F(ModelFixture, CoilHeatingDXMultiSpeedStageData_2017and2023) {
  Model model;

  CoilHeatingDXMultiSpeedStageData speed(model);

  EXPECT_EQ(773.3, speed.ratedSupplyAirFanPowerPerVolumeFlowRate2017());
  EXPECT_EQ(934.4, speed.ratedSupplyAirFanPowerPerVolumeFlowRate2023());

  EXPECT_TRUE(speed.setRatedSupplyAirFanPowerPerVolumeFlowRate2017(8.0));
  EXPECT_TRUE(speed.setRatedSupplyAirFanPowerPerVolumeFlowRate2023(9.0));

  EXPECT_EQ(8.0, speed.ratedSupplyAirFanPowerPerVolumeFlowRate2017());
  EXPECT_EQ(9.0, speed.ratedSupplyAirFanPowerPerVolumeFlowRate2023());
}
