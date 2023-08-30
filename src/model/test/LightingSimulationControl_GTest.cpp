/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>

#include "ModelFixture.hpp"
#include "../LightingSimulationControl.hpp"
#include "../LightingSimulationControl_Impl.hpp"
#include "../Model.hpp"
#include "../Model_Impl.hpp"

using namespace openstudio::model;
using namespace openstudio;

TEST_F(ModelFixture, LightingSimulationControl_Construction) {
  Model model;
  auto lsc = model.getUniqueModelObject<LightingSimulationControl>();
}
