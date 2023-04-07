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

#include "../CoilHeatingDXVariableRefrigerantFlowFluidTemperatureControl.hpp"
#include "../CoilHeatingDXVariableRefrigerantFlowFluidTemperatureControl_Impl.hpp"

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

TEST_F(ModelFixture, CoilHeatingDXVariableRefrigerantFlowFluidTemperatureControl_CoilHeatingDXVariableRefrigerantFlowFluidTemperatureControl) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  ASSERT_EXIT(
    {
      Model model;

      CoilHeatingDXVariableRefrigerantFlowFluidTemperatureControl coil(model);

      exit(0);
    },
    ::testing::ExitedWithCode(0), "");

  Model model;

  CoilHeatingDXVariableRefrigerantFlowFluidTemperatureControl coil(model);

  auto alwaysOn = model.alwaysOnDiscreteSchedule();

  EXPECT_EQ(alwaysOn, coil.availabilitySchedule());
  EXPECT_FALSE(coil.ratedTotalHeatingCapacity());
  EXPECT_TRUE(coil.isRatedTotalHeatingCapacityAutosized());
  EXPECT_FALSE(coil.autosizedRatedTotalHeatingCapacity());
  EXPECT_EQ(5.0, coil.indoorUnitReferenceSubcooling());
  Curve curve = coil.indoorUnitCondensingTemperatureFunctionofSubcoolingCurve();
  boost::optional<CurveQuadratic> indoorUnitCondensingTemperatureFunctionofSubcoolingCurve = curve.optionalCast<CurveQuadratic>();
  EXPECT_TRUE(indoorUnitCondensingTemperatureFunctionofSubcoolingCurve);
}

TEST_F(ModelFixture, CoilHeatingDXVariableRefrigerantFlowFluidTemperatureControl_SetGetFields) {
  Model model;

  CoilHeatingDXVariableRefrigerantFlowFluidTemperatureControl coil(model);

  ScheduleConstant scheduleConstant(model);
  scheduleConstant.setValue(0.5);
  EXPECT_TRUE(coil.setAvailabilitySchedule(scheduleConstant));
  EXPECT_TRUE(coil.setRatedTotalHeatingCapacity(1));
  EXPECT_TRUE(coil.setIndoorUnitReferenceSubcooling(2));
  CurveCubic curve(model);
  EXPECT_TRUE(coil.setIndoorUnitCondensingTemperatureFunctionofSubcoolingCurve(curve));

  Schedule schedule = coil.availabilitySchedule();
  boost::optional<ScheduleConstant> _scheduleConstant = schedule.optionalCast<ScheduleConstant>();
  ASSERT_TRUE(_scheduleConstant);
  EXPECT_EQ((*_scheduleConstant).value(), 0.5);
  ASSERT_TRUE(coil.ratedTotalHeatingCapacity());
  EXPECT_EQ(1, coil.ratedTotalHeatingCapacity().get());
  EXPECT_FALSE(coil.isRatedTotalHeatingCapacityAutosized());
  EXPECT_FALSE(coil.autosizedRatedTotalHeatingCapacity());
  EXPECT_EQ(2, coil.indoorUnitReferenceSubcooling());
  Curve _curve = coil.indoorUnitCondensingTemperatureFunctionofSubcoolingCurve();
  boost::optional<CurveCubic> indoorUnitCondensingTemperatureFunctionofSubcoolingCurve = _curve.optionalCast<CurveCubic>();
  EXPECT_TRUE(indoorUnitCondensingTemperatureFunctionofSubcoolingCurve);

  coil.autosizeRatedTotalHeatingCapacity();

  EXPECT_FALSE(coil.ratedTotalHeatingCapacity());
  EXPECT_TRUE(coil.isRatedTotalHeatingCapacityAutosized());
  EXPECT_FALSE(coil.autosizedRatedTotalHeatingCapacity());
}

TEST_F(ModelFixture, CoilHeatingDXVariableRefrigerantFlowFluidTemperatureControl_Clone) {
  Model model;

  CoilHeatingDXVariableRefrigerantFlowFluidTemperatureControl coil(model);

  EXPECT_TRUE(coil.setRatedTotalHeatingCapacity(10));

  CoilHeatingDXVariableRefrigerantFlowFluidTemperatureControl coilClone =
    coil.clone(model).cast<CoilHeatingDXVariableRefrigerantFlowFluidTemperatureControl>();
  ASSERT_TRUE(coilClone.ratedTotalHeatingCapacity());
  EXPECT_EQ(10, coilClone.ratedTotalHeatingCapacity().get());

  Model model2;
  CoilHeatingDXVariableRefrigerantFlowFluidTemperatureControl coilClone2 =
    coil.clone(model2).cast<CoilHeatingDXVariableRefrigerantFlowFluidTemperatureControl>();
  ASSERT_TRUE(coilClone2.ratedTotalHeatingCapacity());
  EXPECT_EQ(10, coilClone2.ratedTotalHeatingCapacity().get());
}

TEST_F(ModelFixture, CoilHeatingDXVariableRefrigerantFlowFluidTemperatureControl_Remove) {
  Model model;
  auto size = model.modelObjects().size();
  CoilHeatingDXVariableRefrigerantFlowFluidTemperatureControl coil(model);
  EXPECT_EQ(1u, model.getObjectsByType(CurveQuadratic::iddObjectType()).size());
  EXPECT_FALSE(coil.remove().empty());
  EXPECT_EQ(0u, model.getObjectsByType(CurveQuadratic::iddObjectType()).size());
  EXPECT_EQ(0u, model.getObjectsByType(CoilHeatingDXVariableRefrigerantFlowFluidTemperatureControl::iddObjectType()).size());
  EXPECT_EQ(size + 2, model.modelObjects().size());  // Always On Discrete, OnOff
}
