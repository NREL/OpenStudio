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

#include "ModelFixture.hpp"

#include "../CoilHeatingGasMultiStageStageData.hpp"
#include "../CoilHeatingGasMultiStageStageData_Impl.hpp"
#include "../CoilHeatingGasMultiStage.hpp"
#include "../CoilHeatingGasMultiStage_Impl.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, CoilHeatingGasMultiStageStageData_GettersSetters) {
  Model m;
  CoilHeatingGasMultiStage coil(m);
  CoilHeatingGasMultiStageStageData stageData1(m);
  CoilHeatingGasMultiStageStageData stageData2(m);

  EXPECT_EQ(0u, coil.stages().size());
  EXPECT_FALSE(stageData1.parentCoil());
  EXPECT_TRUE(coil.addStage(stageData1));
  EXPECT_TRUE(coil.addStage(stageData2));
  EXPECT_TRUE(stageData1.parentCoil());
  EXPECT_EQ(2u, coil.stages().size());

  EXPECT_EQ(0.8, stageData1.gasBurnerEfficiency());
  EXPECT_FALSE(stageData1.nominalCapacity());
  EXPECT_TRUE(stageData1.isNominalCapacityAutosized());
  EXPECT_EQ(0.0, stageData1.parasiticElectricLoad());

  EXPECT_TRUE(stageData1.setGasBurnerEfficiency(0.75));
  EXPECT_TRUE(stageData1.setNominalCapacity(1000.0));
  EXPECT_TRUE(stageData1.setParasiticElectricLoad(1.5));

  EXPECT_EQ(0.75, stageData1.gasBurnerEfficiency());
  ASSERT_TRUE(stageData1.nominalCapacity());
  EXPECT_EQ(1000.0, stageData1.nominalCapacity().get());
  EXPECT_FALSE(stageData1.isNominalCapacityAutosized());
  EXPECT_EQ(1.5, stageData1.parasiticElectricLoad());

  stageData1.autosizeNominalCapacity();
  EXPECT_FALSE(stageData1.nominalCapacity());
  EXPECT_TRUE(stageData1.isNominalCapacityAutosized());
}

TEST_F(ModelFixture, CoilHeatingGasMultiStageStageData_remove) {
  // If we remove a stage, we would like any CoilHeatingGasMultiStage that use it to have their extensible groups adjusted
  Model model;

  CoilHeatingGasMultiStageStageData stage1(model);
  CoilHeatingGasMultiStageStageData stage2(model);

  CoilHeatingGasMultiStage coil(model);
  EXPECT_TRUE(coil.addStage(stage1));
  EXPECT_TRUE(coil.addStage(stage2));

  EXPECT_EQ(2, coil.numExtensibleGroups());
  EXPECT_EQ(2, coil.numberOfStages());
  EXPECT_EQ(2, coil.stages().size());

  EXPECT_EQ(2, model.getConcreteModelObjects<CoilHeatingGasMultiStageStageData>().size());

  stage1.remove();

  EXPECT_EQ(1, model.getConcreteModelObjects<CoilHeatingGasMultiStageStageData>().size());

  EXPECT_EQ(1, coil.numExtensibleGroups());
  EXPECT_EQ(1, coil.numberOfStages());
  EXPECT_EQ(1, coil.stages().size());

  EXPECT_EQ(stage2, coil.stages().front());
}

TEST_F(ModelFixture, CoilHeatingGasMultiStageStageData_Unicity) {
  Model model;

  CoilHeatingGasMultiStageStageData stage(model);
  EXPECT_FALSE(stage.parentCoil());

  CoilHeatingGasMultiStage coil1(model);
  EXPECT_TRUE(coil1.addStage(stage));
  ASSERT_TRUE(stage.parentCoil());
  EXPECT_EQ(coil1, stage.parentCoil().get());

  // StageData is already used, so refuse
  CoilHeatingGasMultiStage coil2(model);
  EXPECT_FALSE(coil2.addStage(stage));
  EXPECT_EQ(1u, model.getConcreteModelObjects<CoilHeatingGasMultiStageStageData>().size());

  EXPECT_EQ(1u, coil1.numExtensibleGroups());
  EXPECT_EQ(1u, coil1.numberOfStages());
  EXPECT_EQ(1u, coil1.stages().size());
  EXPECT_EQ(0u, coil2.numExtensibleGroups());
  EXPECT_EQ(0u, coil2.numberOfStages());
  EXPECT_EQ(0u, coil2.stages().size());
}
