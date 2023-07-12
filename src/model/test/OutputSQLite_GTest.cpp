/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>

#include "ModelFixture.hpp"
#include "../OutputSQLite.hpp"
#include "../OutputSQLite_Impl.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, OutputSQLite_GettersSetters) {

  Model model;
  OutputSQLite outputSQLite = model.getUniqueModelObject<OutputSQLite>();

  EXPECT_EQ("SimpleAndTabular", outputSQLite.optionType());
  EXPECT_EQ("UseOutputControlTableStyle", outputSQLite.unitConversionforTabularData());

  EXPECT_TRUE(outputSQLite.setOptionType("Simple"));
  EXPECT_TRUE(outputSQLite.setUnitConversionforTabularData("None"));
  EXPECT_EQ("Simple", outputSQLite.optionType());
  EXPECT_EQ("None", outputSQLite.unitConversionforTabularData());

  EXPECT_FALSE(outputSQLite.setOptionType("Invalid"));
  EXPECT_FALSE(outputSQLite.setUnitConversionforTabularData("AnotherInvalid"));
  EXPECT_EQ("Simple", outputSQLite.optionType());
  EXPECT_EQ("None", outputSQLite.unitConversionforTabularData());
}  // End of Getter_Setters test
