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
