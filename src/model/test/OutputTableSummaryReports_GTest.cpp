/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2019, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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
#include <string>

#include "ModelFixture.hpp"

#include "../Model.hpp"
#include "../Model_Impl.hpp"

#include "../OutputTableSummaryReports.hpp"
#include "../OutputTableSummaryReports_Impl.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, OutputTableSummaryReports_OutputTableSummaryReports) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  ASSERT_EXIT(
    {
      // create a model to use
      Model model;

      // create an output table summary reports object to use
      OutputTableSummaryReports outputTableSummaryReports = model.getUniqueModelObject<OutputTableSummaryReports>();

      exit(0);
    },
    ::testing::ExitedWithCode(0), "");

  // create a model to use
  Model model;

  // create an output table summary reports object to use
  OutputTableSummaryReports outputTableSummaryReports = model.getUniqueModelObject<OutputTableSummaryReports>();

  EXPECT_EQ(0, outputTableSummaryReports.numberofSummaryReports());
}

// check summary reports
TEST_F(ModelFixture, OutputTableSummaryReports_SummaryReports) {
  Model model;
  OutputTableSummaryReports outputTableSummaryReports = model.getUniqueModelObject<OutputTableSummaryReports>();

  outputTableSummaryReports.removeAllSummaryReports();

  EXPECT_EQ(0, outputTableSummaryReports.numberofSummaryReports());
  EXPECT_TRUE(outputTableSummaryReports.addSummaryReport("AnnualBuildingUtilityPerformanceSummary"));
  EXPECT_EQ(1, outputTableSummaryReports.numberofSummaryReports());
  EXPECT_TRUE(outputTableSummaryReports.addSummaryReport("InputVerificationandResultsSummary"));
  EXPECT_EQ(2, outputTableSummaryReports.numberofSummaryReports());
  EXPECT_TRUE(outputTableSummaryReports.addSummaryReport("SourceEnergyEndUseComponentsSummary"));
  EXPECT_EQ(3, outputTableSummaryReports.numberofSummaryReports());
  // Shouldn't work, already added once
  EXPECT_FALSE(outputTableSummaryReports.addSummaryReport("SourceEnergyEndUseComponentsSummary"));
  EXPECT_EQ(3, outputTableSummaryReports.numberofSummaryReports());
  // Shouldn't work, not a valid choice
  EXPECT_FALSE(outputTableSummaryReports.addSummaryReport("MadeUpSummaryReport"));
  EXPECT_EQ(3, outputTableSummaryReports.numberofSummaryReports());
  EXPECT_THROW(outputTableSummaryReports.getSummaryReport(3), openstudio::Exception);
  EXPECT_TRUE(outputTableSummaryReports.getSummaryReport(2));
  EXPECT_EQ("SourceEnergyEndUseComponentsSummary", outputTableSummaryReports.getSummaryReport(2).get());

  EXPECT_FALSE(outputTableSummaryReports.summaryReportIndex("MadeUpSummaryReport"));
  ASSERT_TRUE(outputTableSummaryReports.summaryReportIndex("SourceEnergyEndUseComponentsSummary"));
  EXPECT_EQ(2, outputTableSummaryReports.summaryReportIndex("SourceEnergyEndUseComponentsSummary").get());

  outputTableSummaryReports.removeSummaryReport(1);
  EXPECT_EQ(2, outputTableSummaryReports.numberofSummaryReports());

  // check that remaining reports moved correctly
  std::vector<std::string> summaryReports = outputTableSummaryReports.summaryReports();
  ASSERT_EQ(2, summaryReports.size());
  EXPECT_EQ("AnnualBuildingUtilityPerformanceSummary", summaryReports[0]);
  EXPECT_EQ("SourceEnergyEndUseComponentsSummary", summaryReports[1]);

  // more remove checking
  outputTableSummaryReports.removeAllSummaryReports();
  EXPECT_EQ(0, outputTableSummaryReports.numberofSummaryReports());
  outputTableSummaryReports.removeSummaryReport(0);
  EXPECT_EQ(0, outputTableSummaryReports.numberofSummaryReports());

  // check bulk-adding summary reports
  std::vector<std::string> summaryReportsToAdd;
  std::string summaryReport1 = "SurfaceShadowingSummary";
  summaryReportsToAdd.push_back(summaryReport1);
  std::string summaryReport2 = "ShadingSummary";
  summaryReportsToAdd.push_back(summaryReport2);
  EXPECT_TRUE(outputTableSummaryReports.addSummaryReports(summaryReportsToAdd));
  EXPECT_EQ(2, outputTableSummaryReports.numberofSummaryReports());
  summaryReports = outputTableSummaryReports.summaryReports();
  ASSERT_EQ(2, summaryReports.size());
  EXPECT_EQ("SurfaceShadowingSummary", summaryReports[0]);
  EXPECT_EQ("ShadingSummary", summaryReports[1]);
}

// test cloning it
TEST_F(ModelFixture, OutputTableSummaryReports_Clone) {
  // create a model to use
  Model model;

  // create an output table summary reports object to use
  OutputTableSummaryReports outputTableSummaryReports = model.getUniqueModelObject<OutputTableSummaryReports>();

  // change some of the fields
  outputTableSummaryReports.addSummaryReport("ClimaticDataSummary");

  // clone it into the same model
  OutputTableSummaryReports outputTableSummaryReportsClone = outputTableSummaryReports.clone(model).cast<OutputTableSummaryReports>();
  // UniqueModelObject: should be the same as the original
  EXPECT_EQ(outputTableSummaryReports, outputTableSummaryReportsClone);
  ASSERT_EQ(1, outputTableSummaryReportsClone.numberofSummaryReports());
  EXPECT_EQ("ClimaticDataSummary", outputTableSummaryReportsClone.summaryReports()[0]);

  // clone it into a different model
  Model model2;
  OutputTableSummaryReports outputTableSummaryReportsClone2 = outputTableSummaryReports.clone(model2).cast<OutputTableSummaryReports>();
  ASSERT_EQ(1, outputTableSummaryReportsClone2.numberofSummaryReports());
  EXPECT_EQ("ClimaticDataSummary", outputTableSummaryReportsClone2.summaryReports()[0]);
}

// check that remove works
TEST_F(ModelFixture, OutputTableSummaryReports_Remove) {
  Model model;
  auto size = model.modelObjects().size();
  OutputTableSummaryReports outputTableSummaryReports = model.getUniqueModelObject<OutputTableSummaryReports>();
  EXPECT_FALSE(outputTableSummaryReports.remove().empty());
  EXPECT_EQ(size, model.modelObjects().size());
}

TEST_F(ModelFixture, OutputTableSummaryReports_OtherMethods) {
  Model model;
  OutputTableSummaryReports outputTableSummaryReports = model.getUniqueModelObject<OutputTableSummaryReports>();

  EXPECT_TRUE(outputTableSummaryReports.enableAllSummaryReport());
  ASSERT_EQ(1, outputTableSummaryReports.numberofSummaryReports());
  EXPECT_EQ("AllSummary", outputTableSummaryReports.summaryReports()[0]);

  EXPECT_TRUE(outputTableSummaryReports.summaryReportValues().size() > 0);
  EXPECT_TRUE(outputTableSummaryReports.validSummaryReportValues().size() > 0);
}
