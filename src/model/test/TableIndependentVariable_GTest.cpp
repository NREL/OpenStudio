/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>
#include "ModelFixture.hpp"
#include "../TableIndependentVariable.hpp"
#include "../TableIndependentVariable_Impl.hpp"
#include "../TableLookup.hpp"
#include "../TableLookup_Impl.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, TableIndependentVariable_GettersSetters) {

  Model m;
  TableIndependentVariable tableIndependentVariable(m);

  // Defaults
  EXPECT_EQ("Linear", tableIndependentVariable.interpolationMethod());
  EXPECT_EQ("Constant", tableIndependentVariable.extrapolationMethod());
  EXPECT_FALSE(tableIndependentVariable.minimumValue());
  EXPECT_FALSE(tableIndependentVariable.maximumValue());
  EXPECT_FALSE(tableIndependentVariable.normalizationReferenceValue());
  EXPECT_EQ("Dimensionless", tableIndependentVariable.unitType());
  EXPECT_EQ(0u, tableIndependentVariable.tableLookups().size());

  // Interpolation Method: Required String
  EXPECT_TRUE(tableIndependentVariable.setInterpolationMethod("Cubic"));
  EXPECT_EQ("Cubic", tableIndependentVariable.interpolationMethod());
  // Bad Value
  EXPECT_FALSE(tableIndependentVariable.setInterpolationMethod("BADENUM"));
  EXPECT_EQ("Cubic", tableIndependentVariable.interpolationMethod());

  // Extrapolation Method: Required String
  EXPECT_TRUE(tableIndependentVariable.setExtrapolationMethod("Linear"));
  EXPECT_EQ("Linear", tableIndependentVariable.extrapolationMethod());
  // Bad Value
  EXPECT_FALSE(tableIndependentVariable.setExtrapolationMethod("BADENUM"));
  EXPECT_EQ("Linear", tableIndependentVariable.extrapolationMethod());

  // Minimum Value: Optional Double
  EXPECT_TRUE(tableIndependentVariable.setMinimumValue(0.5));
  ASSERT_TRUE(tableIndependentVariable.minimumValue());
  EXPECT_EQ(0.5, tableIndependentVariable.minimumValue().get());
  tableIndependentVariable.resetMinimumValue();
  EXPECT_FALSE(tableIndependentVariable.minimumValue());

  // Maximum Value: Optional Double
  EXPECT_TRUE(tableIndependentVariable.setMaximumValue(0.6));
  ASSERT_TRUE(tableIndependentVariable.maximumValue());
  EXPECT_EQ(0.6, tableIndependentVariable.maximumValue().get());
  tableIndependentVariable.resetMaximumValue();
  EXPECT_FALSE(tableIndependentVariable.maximumValue());

  // Normalization Reference Value: Optional Double
  EXPECT_TRUE(tableIndependentVariable.setNormalizationReferenceValue(0.7));
  ASSERT_TRUE(tableIndependentVariable.normalizationReferenceValue());
  EXPECT_EQ(0.7, tableIndependentVariable.normalizationReferenceValue().get());
  tableIndependentVariable.resetNormalizationReferenceValue();
  EXPECT_FALSE(tableIndependentVariable.normalizationReferenceValue());

  // Unit Type: Required String
  EXPECT_TRUE(tableIndependentVariable.setUnitType("Temperature"));
  EXPECT_EQ("Temperature", tableIndependentVariable.unitType());
  // Bad Value
  EXPECT_FALSE(tableIndependentVariable.setUnitType("BADENUM"));
  EXPECT_EQ("Temperature", tableIndependentVariable.unitType());

  // Values
  EXPECT_TRUE(tableIndependentVariable.addValue(1.0));
  EXPECT_EQ(1u, tableIndependentVariable.values().size());
  EXPECT_EQ(1u, tableIndependentVariable.numberofValues());
  EXPECT_TRUE(tableIndependentVariable.addValue(2.0));
  EXPECT_TRUE(tableIndependentVariable.addValue(3.0));
  EXPECT_EQ(3u, tableIndependentVariable.numberofValues());
  {
    std::vector<double> values = tableIndependentVariable.values();
    ASSERT_EQ(3u, values.size());
    EXPECT_EQ(1.0, values[0]);
    EXPECT_EQ(2.0, values[1]);
    EXPECT_EQ(3.0, values[2]);
  }

  EXPECT_TRUE(tableIndependentVariable.removeValue(1));
  {
    std::vector<double> values = tableIndependentVariable.values();
    ASSERT_EQ(2, values.size());
    EXPECT_EQ(1.0, values[0]);
    EXPECT_EQ(3.0, values[1]);
  }

  tableIndependentVariable.removeAllValues();
  EXPECT_EQ(0u, tableIndependentVariable.values().size());
  EXPECT_EQ(0u, tableIndependentVariable.numberofValues());

  EXPECT_TRUE(tableIndependentVariable.addValue(10.0));
  EXPECT_EQ(1u, tableIndependentVariable.values().size());

  // Clears any existing values first
  EXPECT_TRUE(tableIndependentVariable.setValues({1.0, 2.0, 3.0, 4.0}));
  EXPECT_EQ(4u, tableIndependentVariable.numberofValues());
  {
    std::vector<double> values = tableIndependentVariable.values();
    ASSERT_EQ(4u, values.size());
    EXPECT_EQ(1.0, values[0]);
    EXPECT_EQ(2.0, values[1]);
    EXPECT_EQ(3.0, values[2]);
    EXPECT_EQ(4.0, values[3]);
  }

  // Convenience method to get parent TableLookup(s)
  TableLookup tableLookup1(m);
  EXPECT_TRUE(tableLookup1.addIndependentVariable(tableIndependentVariable));
  EXPECT_EQ(1u, tableIndependentVariable.tableLookups().size());

  TableLookup tableLookup2(m);
  EXPECT_TRUE(tableLookup2.addIndependentVariable(tableIndependentVariable));
  TableLookup tableLookup3(m);
  EXPECT_TRUE(tableLookup3.addIndependentVariable(tableIndependentVariable));
  EXPECT_EQ(3u, tableIndependentVariable.tableLookups().size());
}
