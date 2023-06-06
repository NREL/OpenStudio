/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>
#include "ModelFixture.hpp"
#include "../ZoneControlContaminantController.hpp"
#include "../ZoneControlContaminantController_Impl.hpp"

using namespace openstudio;

TEST_F(ModelFixture, ZoneControlContaminantController) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  ASSERT_EXIT(
    {
      model::Model m;

      model::ZoneControlContaminantController controller(m);

      exit(0);
    },
    ::testing::ExitedWithCode(0), "");
}
