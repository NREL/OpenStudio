/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>

#include "ModelFixture.hpp"
#include "../OutputConstructions.hpp"
#include "../OutputConstructions_Impl.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, OutputConstructions_GettersSetters) {
  Model m;
  OutputConstructions mo = m.getUniqueModelObject<OutputConstructions>();

  EXPECT_TRUE(mo.reportConstructions());
  EXPECT_FALSE(mo.reportMaterials());

  EXPECT_TRUE(mo.setReportConstructions(false));
  EXPECT_TRUE(mo.setReportMaterials(true));

  EXPECT_FALSE(mo.reportConstructions());
  EXPECT_TRUE(mo.reportMaterials());
}  // End of Getter_Setters test
