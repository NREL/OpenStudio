/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <algorithm>
#include <gtest/gtest.h>

#include "ModelFixture.hpp"

#include "../SurfacePropertyOtherSideCoefficients.hpp"
#include "../SurfacePropertyOtherSideCoefficients_Impl.hpp"
#include "../Surface.hpp"
#include "../Schedule.hpp"
#include "../ScheduleConstant.hpp"

using namespace openstudio;
using namespace openstudio::model;

// Test for #3848
TEST_F(ModelFixture, SurfacePropertyOtherSideCoefficients_ScheduleTypeLimits) {

  Model m;
  SurfacePropertyOtherSideCoefficients sp(m);
  Schedule alwaysOn = m.alwaysOnDiscreteSchedule();
  ASSERT_NO_THROW(sp.setConstantTemperatureSchedule(alwaysOn));
}
