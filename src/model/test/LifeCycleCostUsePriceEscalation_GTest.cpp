/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2020, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

#include "../LifeCycleCostUsePriceEscalation.hpp"
#include "../LifeCycleCostUsePriceEscalation_Impl.hpp"

#include "../../utilities/time/Date.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, LifeCycleCostUsePriceEscalation) {

  Model model;

  LifeCycleCostUsePriceEscalation lifeCycleCostUsePriceEscalation(model);

  // check defaults
  ASSERT_TRUE(lifeCycleCostUsePriceEscalation.name());
  EXPECT_EQ("Life Cycle Cost Use Price Escalation 1", lifeCycleCostUsePriceEscalation.name().get());
  ASSERT_TRUE(lifeCycleCostUsePriceEscalation.resource());
  EXPECT_EQ(lifeCycleCostUsePriceEscalation.resource().get(), "");
  EXPECT_FALSE(lifeCycleCostUsePriceEscalation.escalationStartMonth());
  EXPECT_FALSE(lifeCycleCostUsePriceEscalation.escalationStartYear());
  EXPECT_EQ(lifeCycleCostUsePriceEscalation.numYears(), 0);

  // check setters
  EXPECT_TRUE(lifeCycleCostUsePriceEscalation.setResource("Electricity"));
  ASSERT_TRUE(lifeCycleCostUsePriceEscalation.resource());
  EXPECT_EQ(lifeCycleCostUsePriceEscalation.resource().get(), "Electricity");
  EXPECT_TRUE(lifeCycleCostUsePriceEscalation.setResource("NaturalGas"));
  ASSERT_TRUE(lifeCycleCostUsePriceEscalation.resource());
  EXPECT_EQ(lifeCycleCostUsePriceEscalation.resource().get(), "NaturalGas");
  EXPECT_FALSE(lifeCycleCostUsePriceEscalation.setResource("Natural Gas"));
  ASSERT_TRUE(lifeCycleCostUsePriceEscalation.resource());
  EXPECT_EQ(lifeCycleCostUsePriceEscalation.resource().get(), "NaturalGas");

  EXPECT_TRUE(lifeCycleCostUsePriceEscalation.setEscalationStartMonth("January"));
  ASSERT_TRUE(lifeCycleCostUsePriceEscalation.escalationStartMonth());
  EXPECT_EQ(lifeCycleCostUsePriceEscalation.escalationStartMonth().get(), "January");
  EXPECT_TRUE(lifeCycleCostUsePriceEscalation.setEscalationStartMonth("February"));
  ASSERT_TRUE(lifeCycleCostUsePriceEscalation.escalationStartMonth());
  EXPECT_EQ(lifeCycleCostUsePriceEscalation.escalationStartMonth().get(), "February");
  EXPECT_FALSE(lifeCycleCostUsePriceEscalation.setEscalationStartMonth("Febuary"));
  ASSERT_TRUE(lifeCycleCostUsePriceEscalation.escalationStartMonth());
  EXPECT_EQ(lifeCycleCostUsePriceEscalation.escalationStartMonth().get(), "February");

  EXPECT_TRUE(lifeCycleCostUsePriceEscalation.setEscalationStartYear(2020));
  ASSERT_TRUE(lifeCycleCostUsePriceEscalation.escalationStartYear());
  EXPECT_EQ(lifeCycleCostUsePriceEscalation.escalationStartYear().get(), 2020);
  EXPECT_TRUE(lifeCycleCostUsePriceEscalation.setEscalationStartYear(2021));
  ASSERT_TRUE(lifeCycleCostUsePriceEscalation.escalationStartYear());
  EXPECT_EQ(lifeCycleCostUsePriceEscalation.escalationStartYear().get(), 2021);
  EXPECT_FALSE(lifeCycleCostUsePriceEscalation.setEscalationStartYear(1899));
  ASSERT_TRUE(lifeCycleCostUsePriceEscalation.escalationStartYear());
  EXPECT_EQ(lifeCycleCostUsePriceEscalation.escalationStartYear().get(), 2021);

  EXPECT_TRUE(lifeCycleCostUsePriceEscalation.setYearEscalation(0, 100.0));
  ASSERT_TRUE(lifeCycleCostUsePriceEscalation.yearEscalation(0));
  EXPECT_EQ(lifeCycleCostUsePriceEscalation.yearEscalation(0).get(), 100.0);
  EXPECT_EQ(lifeCycleCostUsePriceEscalation.numYears(), 1);
  EXPECT_TRUE(lifeCycleCostUsePriceEscalation.setYearEscalation(0, 200.0));
  ASSERT_TRUE(lifeCycleCostUsePriceEscalation.yearEscalation(0));
  EXPECT_EQ(lifeCycleCostUsePriceEscalation.yearEscalation(0).get(), 200.0);
  EXPECT_EQ(lifeCycleCostUsePriceEscalation.numYears(), 1);
  EXPECT_TRUE(lifeCycleCostUsePriceEscalation.setYearEscalation(1, 300.0));
  ASSERT_TRUE(lifeCycleCostUsePriceEscalation.yearEscalation(1));
  EXPECT_EQ(lifeCycleCostUsePriceEscalation.yearEscalation(1).get(), 300.0);
  EXPECT_EQ(lifeCycleCostUsePriceEscalation.numYears(), 2);
}