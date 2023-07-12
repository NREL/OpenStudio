/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>

#include "ModelFixture.hpp"
#include "../OutputSchedules.hpp"
#include "../OutputSchedules_Impl.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, OutputSchedules_GettersSetters) {
  Model m;
  OutputSchedules mo = m.getUniqueModelObject<OutputSchedules>();

  EXPECT_EQ("Hourly", mo.keyField());

  EXPECT_TRUE(mo.setKeyField("Timestep"));
  EXPECT_EQ("Timestep", mo.keyField());

  EXPECT_FALSE(mo.setKeyField("Invalid"));
  EXPECT_EQ("Timestep", mo.keyField());
}  // End of Getter_Setters test
