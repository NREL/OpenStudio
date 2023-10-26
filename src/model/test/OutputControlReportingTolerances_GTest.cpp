/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>

#include "ModelFixture.hpp"
#include "../OutputControlReportingTolerances.hpp"
#include "../OutputControlReportingTolerances_Impl.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, OutputControlReportingTolerances_TolerancesGettersSetters) {
  Model model;
  auto outputControlReportingTolerances = model.getUniqueModelObject<OutputControlReportingTolerances>();

  EXPECT_TRUE(outputControlReportingTolerances.setToleranceforTimeHeatingSetpointNotMet(1.16));
  EXPECT_DOUBLE_EQ(1.16, outputControlReportingTolerances.toleranceforTimeHeatingSetpointNotMet());

  EXPECT_TRUE(outputControlReportingTolerances.setToleranceforTimeCoolingSetpointNotMet(0.75));
  EXPECT_DOUBLE_EQ(0.75, outputControlReportingTolerances.toleranceforTimeCoolingSetpointNotMet());
}
