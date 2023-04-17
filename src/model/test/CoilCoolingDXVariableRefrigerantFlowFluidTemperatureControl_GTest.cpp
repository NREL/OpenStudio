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

#include "ModelFixture.hpp"

#include "../CoilCoolingDXVariableRefrigerantFlowFluidTemperatureControl.hpp"
#include "../CoilCoolingDXVariableRefrigerantFlowFluidTemperatureControl_Impl.hpp"

#include "../Schedule.hpp"
#include "../Schedule_Impl.hpp"
#include "../ScheduleConstant.hpp"
#include "../ScheduleConstant_Impl.hpp"
#include "../Curve.hpp"
#include "../Curve_Impl.hpp"
#include "../CurveQuadratic.hpp"
#include "../CurveQuadratic_Impl.hpp"
#include "../CurveCubic.hpp"
#include "../CurveCubic_Impl.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, CoilCoolingDXVariableRefrigerantFlowFluidTemperatureControl_CoilCoolingDXVariableRefrigerantFlowFluidTemperatureControl) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  ASSERT_EXIT(
    {
      Model model;

      CoilCoolingDXVariableRefrigerantFlowFluidTemperatureControl coil(model);

      exit(0);
    },
    ::testing::ExitedWithCode(0), "");

  Model model;

  CoilCoolingDXVariableRefrigerantFlowFluidTemperatureControl coil(model);

  auto alwaysOn = model.alwaysOnDiscreteSchedule();

  EXPECT_EQ(alwaysOn, coil.availabilitySchedule());
  EXPECT_FALSE(coil.ratedTotalCoolingCapacity());
  EXPECT_TRUE(coil.isRatedTotalCoolingCapacityAutosized());
  EXPECT_FALSE(coil.autosizedRatedTotalCoolingCapacity());
  EXPECT_FALSE(coil.ratedSensibleHeatRatio());
  EXPECT_TRUE(coil.isRatedSensibleHeatRatioAutosized());
  EXPECT_FALSE(coil.autosizedRatedSensibleHeatRatio());
  EXPECT_EQ(5.0, coil.indoorUnitReferenceSuperheating());
  Curve curve = coil.indoorUnitEvaporatingTemperatureFunctionofSuperheatingCurve();
  boost::optional<CurveQuadratic> indoorUnitEvaporatingTemperatureFunctionofSuperheatingCurve = curve.optionalCast<CurveQuadratic>();
  EXPECT_TRUE(indoorUnitEvaporatingTemperatureFunctionofSuperheatingCurve);
}

TEST_F(ModelFixture, CoilCoolingDXVariableRefrigerantFlowFluidTemperatureControl_SetGetFields) {
  Model model;

  CoilCoolingDXVariableRefrigerantFlowFluidTemperatureControl coil(model);

  ScheduleConstant scheduleConstant(model);
  scheduleConstant.setValue(0.5);
  EXPECT_TRUE(coil.setAvailabilitySchedule(scheduleConstant));
  EXPECT_TRUE(coil.setRatedTotalCoolingCapacity(1));
  EXPECT_TRUE(coil.setRatedSensibleHeatRatio(2));
  EXPECT_TRUE(coil.setIndoorUnitReferenceSuperheating(3));
  CurveCubic curve(model);
  EXPECT_TRUE(coil.setIndoorUnitEvaporatingTemperatureFunctionofSuperheatingCurve(curve));

  Schedule schedule = coil.availabilitySchedule();
  boost::optional<ScheduleConstant> _scheduleConstant = schedule.optionalCast<ScheduleConstant>();
  ASSERT_TRUE(_scheduleConstant);
  EXPECT_EQ((*_scheduleConstant).value(), 0.5);
  ASSERT_TRUE(coil.ratedTotalCoolingCapacity());
  EXPECT_EQ(1, coil.ratedTotalCoolingCapacity().get());
  EXPECT_FALSE(coil.isRatedTotalCoolingCapacityAutosized());
  EXPECT_FALSE(coil.autosizedRatedTotalCoolingCapacity());
  ASSERT_TRUE(coil.ratedSensibleHeatRatio());
  EXPECT_EQ(2, coil.ratedSensibleHeatRatio().get());
  EXPECT_FALSE(coil.autosizedRatedSensibleHeatRatio());
  EXPECT_EQ(3, coil.indoorUnitReferenceSuperheating());
  Curve _curve = coil.indoorUnitEvaporatingTemperatureFunctionofSuperheatingCurve();
  boost::optional<CurveCubic> indoorUnitEvaporatingTemperatureFunctionofSuperheatingCurve = _curve.optionalCast<CurveCubic>();
  EXPECT_TRUE(indoorUnitEvaporatingTemperatureFunctionofSuperheatingCurve);

  coil.autosizeRatedTotalCoolingCapacity();
  coil.autosizeRatedSensibleHeatRatio();

  EXPECT_FALSE(coil.ratedTotalCoolingCapacity());
  EXPECT_TRUE(coil.isRatedTotalCoolingCapacityAutosized());
  EXPECT_FALSE(coil.autosizedRatedTotalCoolingCapacity());
  EXPECT_FALSE(coil.ratedSensibleHeatRatio());
  EXPECT_TRUE(coil.isRatedSensibleHeatRatioAutosized());
  EXPECT_FALSE(coil.autosizedRatedSensibleHeatRatio());
}

TEST_F(ModelFixture, CoilCoolingDXVariableRefrigerantFlowFluidTemperatureControl_Clone) {
  Model model;

  CoilCoolingDXVariableRefrigerantFlowFluidTemperatureControl coil(model);

  EXPECT_TRUE(coil.setRatedTotalCoolingCapacity(10));

  CoilCoolingDXVariableRefrigerantFlowFluidTemperatureControl coilClone =
    coil.clone(model).cast<CoilCoolingDXVariableRefrigerantFlowFluidTemperatureControl>();
  ASSERT_TRUE(coilClone.ratedTotalCoolingCapacity());
  EXPECT_EQ(10, coilClone.ratedTotalCoolingCapacity().get());

  Model model2;
  CoilCoolingDXVariableRefrigerantFlowFluidTemperatureControl coilClone2 =
    coil.clone(model2).cast<CoilCoolingDXVariableRefrigerantFlowFluidTemperatureControl>();
  ASSERT_TRUE(coilClone2.ratedTotalCoolingCapacity());
  EXPECT_EQ(10, coilClone2.ratedTotalCoolingCapacity().get());
}

TEST_F(ModelFixture, CoilCoolingDXVariableRefrigerantFlowFluidTemperatureControl_Remove) {
  Model model;
  auto size = model.modelObjects().size();
  CoilCoolingDXVariableRefrigerantFlowFluidTemperatureControl coil(model);
  EXPECT_EQ(1u, model.getObjectsByType(CurveQuadratic::iddObjectType()).size());
  EXPECT_FALSE(coil.remove().empty());
  EXPECT_EQ(0u, model.getObjectsByType(CurveQuadratic::iddObjectType()).size());
  EXPECT_EQ(0u, model.getObjectsByType(CoilCoolingDXVariableRefrigerantFlowFluidTemperatureControl::iddObjectType()).size());
  EXPECT_EQ(size + 2, model.modelObjects().size());  // Always On Discrete, OnOff
}