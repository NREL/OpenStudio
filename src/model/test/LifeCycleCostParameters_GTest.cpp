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

#include "../LifeCycleCostParameters.hpp"
#include "../LifeCycleCostParameters_Impl.hpp"
#include "../LifeCycleCost.hpp"
#include "../LifeCycleCost_Impl.hpp"
#include "../Building.hpp"
#include "../Building_Impl.hpp"

#include <utilities/idd/OS_LifeCycleCost_FieldEnums.hxx>

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

TEST_F(ModelFixture, LifeCycleCost_BadNames) {
  Model model1;
  Model model2;

  Building building = model1.getUniqueModelObject<Building>();

  {
    boost::optional<LifeCycleCost> cost = LifeCycleCost::createLifeCycleCost("Good Name", building, 1000.0, "CostPerEach", "Construction");
    ASSERT_TRUE(cost);
    EXPECT_EQ("Good Name", cost->nameString());
    ASSERT_TRUE(cost->getString(OS_LifeCycleCostFields::Name));
    EXPECT_EQ("Good Name", cost->getString(OS_LifeCycleCostFields::Name).get());
    IdfObject idfObject = cost->idfObject();
    ASSERT_TRUE(idfObject.getString(OS_LifeCycleCostFields::Name));
    EXPECT_EQ("Good Name", idfObject.getString(OS_LifeCycleCostFields::Name).get());

    std::stringstream ss;
    ss << *cost;
    boost::optional<IdfObject> idf2 = IdfObject::load(ss.str());
    ASSERT_TRUE(idf2);
    ASSERT_TRUE(idf2->getString(OS_LifeCycleCostFields::Name));
    EXPECT_EQ("Good Name", idf2->getString(OS_LifeCycleCostFields::Name).get());

    boost::optional<WorkspaceObject> modelObject = model2.addObject(*idf2);
    ASSERT_TRUE(modelObject);
    EXPECT_EQ("Good Name", modelObject->nameString());
  }

  {
    // &#44;
    boost::optional<LifeCycleCost> cost = LifeCycleCost::createLifeCycleCost("Comma, In, Name", building, 1000.0, "CostPerEach", "Construction");
    ASSERT_TRUE(cost);
    EXPECT_EQ("Comma, In, Name", cost->nameString());
    ASSERT_TRUE(cost->getString(OS_LifeCycleCostFields::Name));
    EXPECT_EQ("Comma, In, Name", cost->getString(OS_LifeCycleCostFields::Name).get());
    IdfObject idfObject = cost->idfObject();
    ASSERT_TRUE(idfObject.getString(OS_LifeCycleCostFields::Name));
    EXPECT_EQ("Comma, In, Name", idfObject.getString(OS_LifeCycleCostFields::Name).get());

    std::stringstream ss;
    ss << *cost;
    boost::optional<IdfObject> idf2 = IdfObject::load(ss.str());
    ASSERT_TRUE(idf2);
    ASSERT_TRUE(idf2->getString(OS_LifeCycleCostFields::Name));
    EXPECT_EQ("Comma, In, Name", idf2->getString(OS_LifeCycleCostFields::Name).get());

    boost::optional<WorkspaceObject> modelObject = model2.addObject(*idf2);
    ASSERT_TRUE(modelObject);
    EXPECT_EQ("Comma, In, Name", modelObject->nameString());
  }

  {
    // &#59;
    boost::optional<LifeCycleCost> cost = LifeCycleCost::createLifeCycleCost("Semicolon; In; Name", building, 1000.0, "CostPerEach", "Construction");
    ASSERT_TRUE(cost);
    EXPECT_EQ("Semicolon; In; Name", cost->nameString());
    ASSERT_TRUE(cost->getString(OS_LifeCycleCostFields::Name));
    EXPECT_EQ("Semicolon; In; Name", cost->getString(OS_LifeCycleCostFields::Name).get());
    IdfObject idfObject = cost->idfObject();
    ASSERT_TRUE(idfObject.getString(OS_LifeCycleCostFields::Name));
    EXPECT_EQ("Semicolon; In; Name", idfObject.getString(OS_LifeCycleCostFields::Name).get());

    std::stringstream ss;
    ss << *cost;
    boost::optional<IdfObject> idf2 = IdfObject::load(ss.str());
    ASSERT_TRUE(idf2);
    ASSERT_TRUE(idf2->getString(OS_LifeCycleCostFields::Name));
    EXPECT_EQ("Semicolon; In; Name", idf2->getString(OS_LifeCycleCostFields::Name).get());

    boost::optional<WorkspaceObject> modelObject = model2.addObject(*idf2);
    ASSERT_TRUE(modelObject);
    EXPECT_EQ("Semicolon; In; Name", modelObject->nameString());
  }

  {
    // &#33;
    boost::optional<LifeCycleCost> cost =
      LifeCycleCost::createLifeCycleCost("Exclamation! In! Name", building, 1000.0, "CostPerEach", "Construction");
    ASSERT_TRUE(cost);
    EXPECT_EQ("Exclamation! In! Name", cost->nameString());
    ASSERT_TRUE(cost->getString(OS_LifeCycleCostFields::Name));
    EXPECT_EQ("Exclamation! In! Name", cost->getString(OS_LifeCycleCostFields::Name).get());
    IdfObject idfObject = cost->idfObject();
    ASSERT_TRUE(idfObject.getString(OS_LifeCycleCostFields::Name));
    EXPECT_EQ("Exclamation! In! Name", idfObject.getString(OS_LifeCycleCostFields::Name).get());

    std::stringstream ss;
    ss << *cost;
    boost::optional<IdfObject> idf2 = IdfObject::load(ss.str());
    ASSERT_TRUE(idf2);
    ASSERT_TRUE(idf2->getString(OS_LifeCycleCostFields::Name));
    EXPECT_EQ("Exclamation! In! Name", idf2->getString(OS_LifeCycleCostFields::Name).get());

    boost::optional<WorkspaceObject> modelObject = model2.addObject(*idf2);
    ASSERT_TRUE(modelObject);
    EXPECT_EQ("Exclamation! In! Name", modelObject->nameString());
  }

  {
    boost::optional<LifeCycleCost> cost =
      LifeCycleCost::createLifeCycleCost("Crazy Name !@#$%^&*(),.?;'{}|-_+=<>:\"[]~`", building, 1000.0, "CostPerEach", "Construction");
    ASSERT_TRUE(cost);
    EXPECT_EQ("Crazy Name !@#$%^&*(),.?;'{}|-_+=<>:\"[]~`", cost->nameString());
    ASSERT_TRUE(cost->getString(OS_LifeCycleCostFields::Name));
    EXPECT_EQ("Crazy Name !@#$%^&*(),.?;'{}|-_+=<>:\"[]~`", cost->getString(OS_LifeCycleCostFields::Name).get());
    IdfObject idfObject = cost->idfObject();
    ASSERT_TRUE(idfObject.getString(OS_LifeCycleCostFields::Name));
    EXPECT_EQ("Crazy Name !@#$%^&*(),.?;'{}|-_+=<>:\"[]~`", idfObject.getString(OS_LifeCycleCostFields::Name).get());

    std::stringstream ss;
    ss << *cost;
    boost::optional<IdfObject> idf2 = IdfObject::load(ss.str());
    ASSERT_TRUE(idf2);
    ASSERT_TRUE(idf2->getString(OS_LifeCycleCostFields::Name));
    EXPECT_EQ("Crazy Name !@#$%^&*(),.?;'{}|-_+=<>:\"[]~`", idf2->getString(OS_LifeCycleCostFields::Name).get());

    boost::optional<WorkspaceObject> modelObject = model2.addObject(*idf2);
    ASSERT_TRUE(modelObject);
    EXPECT_EQ("Crazy Name !@#$%^&*(),.?;'{}|-_+=<>:\"[]~`", modelObject->nameString());
  }
}