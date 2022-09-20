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

#include "../CoilHeatingElectricMultiStage.hpp"
#include "../CoilHeatingElectricMultiStage_Impl.hpp"
#include "../CoilHeatingElectricMultiStageStageData.hpp"
#include "../CoilHeatingElectricMultiStageStageData_Impl.hpp"
#include "../Schedule.hpp"
#include "../Schedule_Impl.hpp"
#include "../ScheduleConstant.hpp"
#include "../ScheduleConstant_Impl.hpp"
#include "../AirLoopHVAC.hpp"
#include "../Node.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, CoilHeatingElectricMultiStage_GettersSetters) {
  Model m;
  CoilHeatingElectricMultiStage coil(m);

  auto alwaysOn = m.alwaysOnDiscreteSchedule();
  EXPECT_EQ(alwaysOn, coil.availabilitySchedule());
  EXPECT_EQ(0u, coil.stages().size());

  ScheduleConstant scheduleConstant(m);
  EXPECT_TRUE(coil.setAvailabilitySchedule(scheduleConstant));

  EXPECT_EQ(scheduleConstant, coil.availabilitySchedule());
}

TEST_F(ModelFixture, CoilHeatingElectricMultiStage_Remove) {
  Model m;
  CoilHeatingElectricMultiStage coil(m);
  CoilHeatingElectricMultiStageStageData stage(m);
  coil.addStage(stage);
  coil.remove();

  EXPECT_EQ(0u, m.modelObjects().size());
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

TEST_F(ModelFixture, CoilHeatingElectricMultiStage_clone) {
  Model m;

  CoilHeatingElectricMultiStage h(m);
  CoilHeatingElectricMultiStageStageData stageData(m);

  EXPECT_EQ(1u, m.getConcreteModelObjects<CoilHeatingElectricMultiStage>().size());

  auto hClone = h.clone(m).cast<CoilHeatingElectricMultiStage>();
  EXPECT_EQ(2u, m.getConcreteModelObjects<CoilHeatingElectricMultiStage>().size());

  h.remove();
  EXPECT_EQ(1u, m.getConcreteModelObjects<CoilHeatingElectricMultiStage>().size());

  hClone.remove();
  EXPECT_EQ(0u, m.getConcreteModelObjects<CoilHeatingElectricMultiStage>().size());
}
