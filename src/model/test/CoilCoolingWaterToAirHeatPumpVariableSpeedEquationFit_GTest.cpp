/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>
#include "ModelFixture.hpp"
#include "../CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit.hpp"
#include "../CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit_Impl.hpp"
#include "../Curve.hpp"
#include "../Curve_Impl.hpp"
#include "../CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData.hpp"
#include "../CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  ASSERT_EXIT(
    {
      Model m;
      CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit coil(m);

      exit(0);
    },
    ::testing::ExitedWithCode(0), "");
}

TEST_F(ModelFixture, CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit_Remove) {
  Model m;
  auto count = m.modelObjects().size();

  CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit coil(m);
  CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData speed(m);
  coil.addSpeed(speed);
  coil.remove();

  auto curves = m.getModelObjects<model::Curve>();

  EXPECT_EQ(count, m.modelObjects().size() - curves.size());
}
