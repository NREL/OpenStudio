/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2023, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
*  following conditions are met:
*
*  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
*  disclaimer.
*
*  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
*  disclaimer in the documentation and/or other materials provided with the distribution.
*
*  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote products
*  derived from this software without specific prior written permission from the respective party.
*
*  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative works
*  may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without specific prior
*  written permission from Alliance for Sustainable Energy, LLC.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER(S) AND ANY CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
*  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
*  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER(S), ANY CONTRIBUTORS, THE UNITED STATES GOVERNMENT, OR THE UNITED
*  STATES DEPARTMENT OF ENERGY, NOR ANY OF THEIR EMPLOYEES, BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
*  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
*  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
*  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
*  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
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
