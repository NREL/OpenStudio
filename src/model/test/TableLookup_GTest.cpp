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

#include "../TableLookup.hpp"
#include "../TableLookup_Impl.hpp"
#include "../TableIndependentVariable.hpp"
#include "../TableIndependentVariable_Impl.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, TableLookup) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  ASSERT_EXIT(
    {
      Model m;
      TableLookup tableLookup(m);

      exit(0);
    },
    ::testing::ExitedWithCode(0), "");

  Model m;
  TableLookup tableLookup(m);

  EXPECT_EQ("None", tableLookup.normalizationMethod());
  EXPECT_EQ(1.0, tableLookup.normalizationDivisor());
  EXPECT_EQ("Dimensionless", tableLookup.outputUnitType());
  EXPECT_EQ(0u, tableLookup.independentVariables().size());

  EXPECT_TRUE(tableLookup.setNormalizationMethod("DivisorOnly"));
  EXPECT_TRUE(tableLookup.setNormalizationDivisor(0.9));
  EXPECT_TRUE(tableLookup.setOutputUnitType("Capacity"));
  TableIndependentVariable independentVariable1(m);
  EXPECT_TRUE(tableLookup.addIndependentVariable(independentVariable1));

  EXPECT_EQ("DivisorOnly", tableLookup.normalizationMethod());
  EXPECT_EQ(0.9, tableLookup.normalizationDivisor());
  EXPECT_EQ("Capacity", tableLookup.outputUnitType());
  EXPECT_EQ(1u, tableLookup.independentVariables().size());

  TableIndependentVariable independentVariable2(m);
  EXPECT_TRUE(tableLookup.addIndependentVariable(independentVariable2));
  TableIndependentVariable independentVariable3(m);
  EXPECT_TRUE(tableLookup.addIndependentVariable(independentVariable3));
  EXPECT_EQ(3u, tableLookup.independentVariables().size());

  tableLookup.removeIndependentVariable(independentVariable2);
  EXPECT_EQ(2u, tableLookup.independentVariables().size());
  tableLookup.removeAllIndependentVariables();
  EXPECT_EQ(0u, tableLookup.independentVariables().size());
}
