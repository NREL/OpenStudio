/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
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
