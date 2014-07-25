/**********************************************************************
*  Copyright (c) 2008-2014, Alliance for Sustainable Energy.
*  All rights reserved.
*
*  This library is free software; you can redistribute it and/or
*  modify it under the terms of the GNU Lesser General Public
*  License as published by the Free Software Foundation; either
*  version 2.1 of the License, or (at your option) any later version.
*
*  This library is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
*  Lesser General Public License for more details.
*
*  You should have received a copy of the GNU Lesser General Public
*  License along with this library; if not, write to the Free Software
*  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
**********************************************************************/

#include <gtest/gtest.h>

#include "ModelFixture.hpp"

#include "../LifeCycleCostParameters.hpp"
#include "../LifeCycleCostParameters_Impl.hpp"

#include "../../utilities/time/Date.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, LifeCycleCostParameters) {

  Model model;

  EXPECT_FALSE(model.lifeCycleCostParameters());

  LifeCycleCostParameters lifeCycleCostParameters = model.getUniqueModelObject<LifeCycleCostParameters>();

  EXPECT_TRUE(model.lifeCycleCostParameters());

  // check defaults
  EXPECT_TRUE(lifeCycleCostParameters.isFEMPAnalysis());
  EXPECT_EQ("EndOfYear", lifeCycleCostParameters.discountingConvention());
  EXPECT_TRUE(lifeCycleCostParameters.isDiscountingConventionDefaulted());
  EXPECT_EQ("ConstantDollar", lifeCycleCostParameters.inflationApproach());
  EXPECT_TRUE(lifeCycleCostParameters.isInflationApproachDefaulted());
  EXPECT_TRUE(lifeCycleCostParameters.isConstantDollarAnalysis());
  ASSERT_TRUE(lifeCycleCostParameters.realDiscountRate());
  EXPECT_EQ(0.03, lifeCycleCostParameters.realDiscountRate().get());
  EXPECT_FALSE(lifeCycleCostParameters.nominalDiscountRate());
  EXPECT_FALSE(lifeCycleCostParameters.inflation());
  EXPECT_EQ(MonthOfYear::Jan, lifeCycleCostParameters.baseDateMonth().value());
  EXPECT_TRUE(lifeCycleCostParameters.isBaseDateMonthDefaulted());
  EXPECT_EQ(2011, lifeCycleCostParameters.baseDateYear());
  EXPECT_TRUE(lifeCycleCostParameters.isBaseDateYearDefaulted());
  EXPECT_EQ(MonthOfYear::Jan, lifeCycleCostParameters.serviceDateMonth().value());
  EXPECT_TRUE(lifeCycleCostParameters.isServiceDateMonthDefaulted());
  EXPECT_EQ(2011, lifeCycleCostParameters.serviceDateYear());
  EXPECT_TRUE(lifeCycleCostParameters.isServiceDateYearDefaulted());
  EXPECT_EQ(25, lifeCycleCostParameters.lengthOfStudyPeriodInYears());
  EXPECT_TRUE(lifeCycleCostParameters.isLengthOfStudyPeriodInYearsDefaulted());
  EXPECT_FALSE(lifeCycleCostParameters.taxRate());
  EXPECT_EQ("None", lifeCycleCostParameters.depreciationMethod());
  EXPECT_TRUE(lifeCycleCostParameters.isDepreciationMethodDefaulted());
  EXPECT_TRUE(lifeCycleCostParameters.useNISTFuelEscalationRates());
  EXPECT_TRUE(lifeCycleCostParameters.isUseNISTFuelEscalationRatesDefaulted());
  ASSERT_TRUE(lifeCycleCostParameters.nistRegion());
  EXPECT_EQ("U.S. Avg", lifeCycleCostParameters.nistRegion().get());
  EXPECT_TRUE(lifeCycleCostParameters.isNISTRegionDefaulted());
  ASSERT_TRUE(lifeCycleCostParameters.nistSector());
  EXPECT_EQ("Commercial", lifeCycleCostParameters.nistSector().get());
  EXPECT_TRUE(lifeCycleCostParameters.isNISTSectorDefaulted());
  EXPECT_FALSE(lifeCycleCostParameters.electricityInflation());
  EXPECT_FALSE(lifeCycleCostParameters.naturalGasInflation());
  EXPECT_FALSE(lifeCycleCostParameters.steamInflation());
  EXPECT_FALSE(lifeCycleCostParameters.gasolineInflation());
  EXPECT_FALSE(lifeCycleCostParameters.dieselInflation());
  EXPECT_FALSE(lifeCycleCostParameters.coalInflation());
  EXPECT_FALSE(lifeCycleCostParameters.fuelOil1Inflation());
  EXPECT_FALSE(lifeCycleCostParameters.fuelOil2Inflation());
  EXPECT_FALSE(lifeCycleCostParameters.propaneInflation());
  EXPECT_FALSE(lifeCycleCostParameters.waterInflation());

  // change to current dollar
  EXPECT_TRUE(lifeCycleCostParameters.setInflationApproach("CurrentDollar"));
  EXPECT_FALSE(lifeCycleCostParameters.isInflationApproachDefaulted());
  EXPECT_FALSE(lifeCycleCostParameters.isConstantDollarAnalysis());
  EXPECT_FALSE(lifeCycleCostParameters.realDiscountRate());
  ASSERT_TRUE(lifeCycleCostParameters.nominalDiscountRate());
  EXPECT_EQ(0.035, lifeCycleCostParameters.nominalDiscountRate().get());
  ASSERT_TRUE(lifeCycleCostParameters.inflation());
  EXPECT_EQ(0.005, lifeCycleCostParameters.inflation().get());

  EXPECT_FALSE(lifeCycleCostParameters.setRealDiscountRate(0.04));
  EXPECT_FALSE(lifeCycleCostParameters.setNominalDiscountRate(0.05));
  EXPECT_FALSE(lifeCycleCostParameters.setInflation(0.01));
  
  EXPECT_FALSE(lifeCycleCostParameters.realDiscountRate());
  ASSERT_TRUE(lifeCycleCostParameters.nominalDiscountRate());
  EXPECT_EQ(0.035, lifeCycleCostParameters.nominalDiscountRate().get());
  ASSERT_TRUE(lifeCycleCostParameters.inflation());
  EXPECT_EQ(0.005, lifeCycleCostParameters.inflation().get());

  EXPECT_TRUE(lifeCycleCostParameters.setInflationApproach("ConstantDollar"));
  EXPECT_FALSE(lifeCycleCostParameters.isInflationApproachDefaulted());
  EXPECT_TRUE(lifeCycleCostParameters.isConstantDollarAnalysis());
  ASSERT_TRUE(lifeCycleCostParameters.realDiscountRate());
  EXPECT_EQ(0.03, lifeCycleCostParameters.realDiscountRate().get());
  EXPECT_FALSE(lifeCycleCostParameters.nominalDiscountRate());
  EXPECT_FALSE(lifeCycleCostParameters.inflation());

  EXPECT_FALSE(lifeCycleCostParameters.setRealDiscountRate(0.04));
  EXPECT_FALSE(lifeCycleCostParameters.setNominalDiscountRate(0.05));
  EXPECT_FALSE(lifeCycleCostParameters.setInflation(0.01));
  
  ASSERT_TRUE(lifeCycleCostParameters.realDiscountRate());
  EXPECT_EQ(0.03, lifeCycleCostParameters.realDiscountRate().get());
  EXPECT_FALSE(lifeCycleCostParameters.nominalDiscountRate());
  EXPECT_FALSE(lifeCycleCostParameters.inflation());


  // cant be more than 25 years under FEMP
  EXPECT_FALSE(lifeCycleCostParameters.setLengthOfStudyPeriodInYears(30));
  EXPECT_EQ(25, lifeCycleCostParameters.lengthOfStudyPeriodInYears());
  EXPECT_TRUE(lifeCycleCostParameters.isLengthOfStudyPeriodInYearsDefaulted());
  EXPECT_TRUE(lifeCycleCostParameters.setLengthOfStudyPeriodInYears(20));
  EXPECT_EQ(20, lifeCycleCostParameters.lengthOfStudyPeriodInYears());
  EXPECT_FALSE(lifeCycleCostParameters.isLengthOfStudyPeriodInYearsDefaulted());

  // cant set fuel escalation rates when using nist escalation
  EXPECT_FALSE(lifeCycleCostParameters.setElectricityInflation(0.04));
  EXPECT_FALSE(lifeCycleCostParameters.electricityInflation());

  EXPECT_TRUE(lifeCycleCostParameters.setUseNISTFuelEscalationRates(false));
  EXPECT_FALSE(lifeCycleCostParameters.electricityInflation());
  EXPECT_TRUE(lifeCycleCostParameters.setElectricityInflation(0.04));
  ASSERT_TRUE(lifeCycleCostParameters.electricityInflation());
  EXPECT_EQ(0.04, lifeCycleCostParameters.electricityInflation().get());

  // change to custom analysis
  EXPECT_TRUE(lifeCycleCostParameters.setAnalysisType("Custom"));

  EXPECT_TRUE(lifeCycleCostParameters.setInflationApproach("ConstantDollar"));
  EXPECT_TRUE(lifeCycleCostParameters.setRealDiscountRate(0.04));
  EXPECT_FALSE(lifeCycleCostParameters.setNominalDiscountRate(0.05));
  EXPECT_FALSE(lifeCycleCostParameters.setInflation(0.01));
  ASSERT_TRUE(lifeCycleCostParameters.realDiscountRate());
  EXPECT_EQ(0.04, lifeCycleCostParameters.realDiscountRate().get());
  EXPECT_FALSE(lifeCycleCostParameters.nominalDiscountRate());
  EXPECT_FALSE(lifeCycleCostParameters.inflation());

  EXPECT_TRUE(lifeCycleCostParameters.setInflationApproach("CurrentDollar"));
  EXPECT_FALSE(lifeCycleCostParameters.setRealDiscountRate(0.04));
  EXPECT_TRUE(lifeCycleCostParameters.setNominalDiscountRate(0.05));
  EXPECT_TRUE(lifeCycleCostParameters.setInflation(0.01));
  EXPECT_FALSE(lifeCycleCostParameters.realDiscountRate());
  ASSERT_TRUE(lifeCycleCostParameters.nominalDiscountRate());
  EXPECT_EQ(0.05, lifeCycleCostParameters.nominalDiscountRate().get());
  ASSERT_TRUE(lifeCycleCostParameters.inflation());
  EXPECT_EQ(0.01, lifeCycleCostParameters.inflation().get());

  // can be more than 25 years under FEMP
  EXPECT_TRUE(lifeCycleCostParameters.setLengthOfStudyPeriodInYears(30));
  EXPECT_EQ(30, lifeCycleCostParameters.lengthOfStudyPeriodInYears());
  EXPECT_FALSE(lifeCycleCostParameters.isLengthOfStudyPeriodInYearsDefaulted());

  // setting back to FEMP changes length
  EXPECT_TRUE(lifeCycleCostParameters.setAnalysisType("FEMP"));
  EXPECT_EQ(25, lifeCycleCostParameters.lengthOfStudyPeriodInYears());
  EXPECT_FALSE(lifeCycleCostParameters.isLengthOfStudyPeriodInYearsDefaulted());

  EXPECT_TRUE(lifeCycleCostParameters.setAnalysisType("Custom"));
  EXPECT_TRUE(lifeCycleCostParameters.setLengthOfStudyPeriodInYears(5));
  EXPECT_EQ(5, lifeCycleCostParameters.lengthOfStudyPeriodInYears());
  EXPECT_FALSE(lifeCycleCostParameters.isLengthOfStudyPeriodInYearsDefaulted());

  EXPECT_TRUE(lifeCycleCostParameters.setAnalysisType("FEMP"));
  EXPECT_EQ(5, lifeCycleCostParameters.lengthOfStudyPeriodInYears());
  EXPECT_FALSE(lifeCycleCostParameters.isLengthOfStudyPeriodInYearsDefaulted());

  // limit of 30 years if using NIST fuel escalation rates
  EXPECT_TRUE(lifeCycleCostParameters.setAnalysisType("Custom"));
  EXPECT_TRUE(lifeCycleCostParameters.setLengthOfStudyPeriodInYears(50));
  EXPECT_EQ(50, lifeCycleCostParameters.lengthOfStudyPeriodInYears());
  EXPECT_FALSE(lifeCycleCostParameters.isLengthOfStudyPeriodInYearsDefaulted());

  EXPECT_TRUE(lifeCycleCostParameters.setUseNISTFuelEscalationRates(true));
  EXPECT_EQ(30, lifeCycleCostParameters.lengthOfStudyPeriodInYears());
  EXPECT_FALSE(lifeCycleCostParameters.isLengthOfStudyPeriodInYearsDefaulted());

  EXPECT_FALSE(lifeCycleCostParameters.setLengthOfStudyPeriodInYears(50));
  EXPECT_EQ(30, lifeCycleCostParameters.lengthOfStudyPeriodInYears());
  EXPECT_FALSE(lifeCycleCostParameters.isLengthOfStudyPeriodInYearsDefaulted());

  EXPECT_TRUE(lifeCycleCostParameters.setLengthOfStudyPeriodInYears(5));
  EXPECT_EQ(5, lifeCycleCostParameters.lengthOfStudyPeriodInYears());
  EXPECT_FALSE(lifeCycleCostParameters.isLengthOfStudyPeriodInYearsDefaulted());
}
