/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>
#include "ModelFixture.hpp"
#include "../CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFit.hpp"
#include "../CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFit_Impl.hpp"
#include "../Curve.hpp"
#include "../Curve_Impl.hpp"
#include "../CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData.hpp"
#include "../CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFit) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  ASSERT_EXIT(
    {
      Model m;
      CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFit coil(m);

      exit(0);
    },
    ::testing::ExitedWithCode(0), "");
}

TEST_F(ModelFixture, CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFit_Remove) {
  Model m;
  auto count = m.modelObjects().size();

  CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFit coil(m);
  CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData speed(m);
  coil.addSpeed(speed);
  coil.remove();

  auto curves = m.getModelObjects<model::Curve>();

  EXPECT_EQ(count, m.modelObjects().size() - curves.size());
}
