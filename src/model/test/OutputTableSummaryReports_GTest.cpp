/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
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
  auto outputTableSummaryReportsClone = outputTableSummaryReports.clone(model).cast<OutputTableSummaryReports>();
  // UniqueModelObject: should be the same as the original
  EXPECT_EQ(outputTableSummaryReports, outputTableSummaryReportsClone);
  ASSERT_EQ(1, outputTableSummaryReportsClone.numberofSummaryReports());
  EXPECT_EQ("ClimaticDataSummary", outputTableSummaryReportsClone.summaryReports()[0]);

  // clone it into a different model
  Model model2;
  auto outputTableSummaryReportsClone2 = outputTableSummaryReports.clone(model2).cast<OutputTableSummaryReports>();
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

  EXPECT_TRUE(!outputTableSummaryReports.summaryReportValues().empty());
  EXPECT_TRUE(!outputTableSummaryReports.validSummaryReportValues().empty());
}
