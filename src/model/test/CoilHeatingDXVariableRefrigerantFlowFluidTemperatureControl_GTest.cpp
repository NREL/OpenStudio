/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
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
