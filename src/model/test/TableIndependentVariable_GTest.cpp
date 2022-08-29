/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2022, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

TEST_F(ModelFixture, TableIndependentVariable) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  ASSERT_EXIT(
    {
      Model m;
      TableIndependentVariable independentVariable(m);

      exit(0);
    },
    ::testing::ExitedWithCode(0), "");

  Model m;
  TableIndependentVariable independentVariable(m);

  EXPECT_EQ("Linear", independentVariable.interpolationMethod());
  EXPECT_EQ("Constant", independentVariable.extrapolationMethod());
  EXPECT_FALSE(independentVariable.minimumValue());
  EXPECT_FALSE(independentVariable.maximumValue());
  EXPECT_FALSE(independentVariable.normalizationReferenceValue());
  EXPECT_EQ("Dimensionless", independentVariable.unitType());
  EXPECT_EQ(0u, independentVariable.tableLookups().size());

  EXPECT_TRUE(independentVariable.setInterpolationMethod("Cubic"));
  EXPECT_TRUE(independentVariable.setExtrapolationMethod("Linear"));
  EXPECT_TRUE(independentVariable.setMinimumValue(10));
  EXPECT_TRUE(independentVariable.setMaximumValue(20));
  EXPECT_TRUE(independentVariable.setNormalizationReferenceValue(30));
  EXPECT_TRUE(independentVariable.setUnitType("Temperature"));
  EXPECT_TRUE(independentVariable.addValue(50));
  EXPECT_TRUE(independentVariable.addValue(100));
  EXPECT_TRUE(independentVariable.addValue(150));
  TableLookup tableLookup1(m);
  EXPECT_TRUE(tableLookup1.addIndependentVariable(independentVariable));

  EXPECT_EQ("Cubic", independentVariable.interpolationMethod());
  EXPECT_EQ("Linear", independentVariable.extrapolationMethod());
  ASSERT_TRUE(independentVariable.minimumValue());
  EXPECT_EQ(10, independentVariable.minimumValue().get());
  ASSERT_TRUE(independentVariable.maximumValue());
  EXPECT_EQ(20, independentVariable.maximumValue().get());
  ASSERT_TRUE(independentVariable.normalizationReferenceValue());
  EXPECT_EQ(30, independentVariable.normalizationReferenceValue().get());
  EXPECT_EQ("Temperature", independentVariable.unitType());
  EXPECT_EQ(3u, independentVariable.values().size()
  EXPECT_EQ(3u, independentVariable.numberofValues());
  EXPECT_EQ(1u, independentVariable.tableLookups().size());

  EXPECT_TRUE(independentVariable.removeValue(1));
  EXPECT_EQ(2u, independentVariable.values().size());
  independentVariable.removeAllValues();
  EXPECT_EQ(0u, independentVariable.values().size());
  EXPECT_EQ(0u, independentVariable.numberofValues());

  TableLookup tableLookup2(m);
  EXPECT_TRUE(tableLookup2.addIndependentVariable(independentVariable));
  TableLookup tableLookup3(m);
  EXPECT_TRUE(tableLookup3.addIndependentVariable(independentVariable));
  EXPECT_EQ(3u, independentVariable.tableLookups().size());
}
