/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "ModelFixture.hpp"

#include "../OutputEnvironmentalImpactFactors.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, OutputEnvironmentalImpactFactors_GettersSetters) {
  Model m;
  OutputEnvironmentalImpactFactors outputEnvironmentalImpactFactors(m);

  // Reporting Frequency: Required String
  EXPECT_TRUE(outputEnvironmentalImpactFactors.setReportingFrequency("Timestep"));
  EXPECT_EQ("Timestep", outputEnvironmentalImpactFactors.reportingFrequency());
  // Bad Value
  EXPECT_FALSE(outputEnvironmentalImpactFactors.setReportingFrequency("BADENUM"));
  EXPECT_EQ("Timestep", outputEnvironmentalImpactFactors.reportingFrequency());
}
