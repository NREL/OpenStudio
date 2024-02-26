/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>

#include "ModelFixture.hpp"
#include "../OutputControlTableStyle.hpp"
#include "../OutputControlTableStyle_Impl.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, OutputControlTableStyle_GettersSetters) {
  Model model;
  OutputControlTableStyle outputControlTableStyle = model.getUniqueModelObject<OutputControlTableStyle>();

  EXPECT_EQ("HTML", outputControlTableStyle.columnSeparator());
  EXPECT_EQ("None", outputControlTableStyle.unitConversion());

  EXPECT_TRUE(outputControlTableStyle.setColumnSeparator("Tab"));
  EXPECT_TRUE(outputControlTableStyle.setUnitConversion("JtoKWH"));
  EXPECT_EQ("Tab", outputControlTableStyle.columnSeparator());
  EXPECT_EQ("JtoKWH", outputControlTableStyle.unitConversion());

  EXPECT_FALSE(outputControlTableStyle.setColumnSeparator("Invalid"));
  EXPECT_FALSE(outputControlTableStyle.setUnitConversion("AnotherInvalid"));
  EXPECT_EQ("Tab", outputControlTableStyle.columnSeparator());
  EXPECT_EQ("JtoKWH", outputControlTableStyle.unitConversion());

  EXPECT_TRUE(outputControlTableStyle.setUnitConversion("InchPoundExceptElectricity"));
  EXPECT_EQ("InchPoundExceptElectricity", outputControlTableStyle.unitConversion());
}
