/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>
#include "ModelFixture.hpp"
#include "../CoilHeatingDXVariableSpeed.hpp"
#include "../CoilHeatingDXVariableSpeed_Impl.hpp"
#include "../Curve.hpp"
#include "../Curve_Impl.hpp"
#include "../CoilHeatingDXVariableSpeedSpeedData.hpp"
#include "../CoilHeatingDXVariableSpeedSpeedData_Impl.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, CoilHeatingDXVariableSpeed) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  ASSERT_EXIT(
    {
      Model m;
      CoilHeatingDXVariableSpeed coil(m);

      exit(0);
    },
    ::testing::ExitedWithCode(0), "");
}

TEST_F(ModelFixture, CoilHeatingDXVariableSpeed_Remove) {
  Model m;
  auto count = m.modelObjects().size();

  CoilHeatingDXVariableSpeed coil(m);
  CoilHeatingDXVariableSpeedSpeedData speed1(m);
  coil.addSpeed(speed1);
  coil.remove();

  auto curves = m.getModelObjects<model::Curve>();

  EXPECT_EQ(count, m.modelObjects().size() - curves.size());
}
