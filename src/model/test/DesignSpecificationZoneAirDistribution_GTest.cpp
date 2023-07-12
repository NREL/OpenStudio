/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>
#include "ModelFixture.hpp"
#include "../DesignSpecificationZoneAirDistribution.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, DesignSpecificationZoneAirDistribution) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  ASSERT_EXIT(
    {
      model::Model m;

      model::DesignSpecificationZoneAirDistribution dspec(m);

      exit(0);
    },
    ::testing::ExitedWithCode(0), "");
}
