/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>
#include "EnergyPlusFixture.hpp"

#include "../ErrorFile.hpp"
#include "../ForwardTranslator.hpp"
#include "../ReverseTranslator.hpp"

#include "../../model/Model.hpp"
#include "../../model/OutputTableSummaryReports.hpp"

#include "../../utilities/idf/IdfObject.hpp"
#include "../../utilities/idf/IdfExtensibleGroup.hpp"

#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/Output_Table_SummaryReports_FieldEnums.hxx>

#include <vector>

using namespace openstudio::energyplus;
using namespace openstudio::model;
using namespace openstudio;

TEST_F(EnergyPlusFixture, ForwardTranslator_OutputTableSummaryReports_NoOutputTableSummaryReports) {
  Model model;

  ForwardTranslator ft;
  Workspace workspace = ft.translateModel(model);

  // If the model doesn't have an OutputTableSummaryReports, we expect the historical behavior of adding the "AllSummary" report
  std::vector<WorkspaceObject> idf_outputTableSummaryReports = workspace.getObjectsByType(IddObjectType::Output_Table_SummaryReports);
  ASSERT_EQ(1u, idf_outputTableSummaryReports.size());
  WorkspaceObject idf_outputTableSummaryReport(idf_outputTableSummaryReports[0]);

  ASSERT_EQ(1u, idf_outputTableSummaryReport.numExtensibleGroups());
  IdfExtensibleGroup eg = idf_outputTableSummaryReport.extensibleGroups()[0];
  EXPECT_EQ("AllSummary", eg.getString(Output_Table_SummaryReportsExtensibleFields::ReportName).get());
}

TEST_F(EnergyPlusFixture, ForwardTranslator_OutputTableSummaryReports_NoAddedSummaryReports) {
  Model model;

  OutputTableSummaryReports outputTableSummaryReports = model.getUniqueModelObject<OutputTableSummaryReports>();
  EXPECT_EQ(0u, outputTableSummaryReports.numberofSummaryReports());

  ForwardTranslator ft;
  Workspace workspace = ft.translateModel(model);

  // We have an explicitly-instantiated object, with zero report => shouldn't be translated at all
  std::vector<WorkspaceObject> idf_outputTableSummaryReports = workspace.getObjectsByType(IddObjectType::Output_Table_SummaryReports);
  EXPECT_EQ(0u, idf_outputTableSummaryReports.size());
}

TEST_F(EnergyPlusFixture, ForwardTranslator_OutputTableSummaryReports_UserSpecified) {
  Model model;

  OutputTableSummaryReports outputTableSummaryReports = model.getUniqueModelObject<OutputTableSummaryReports>();
  EXPECT_TRUE(outputTableSummaryReports.addSummaryReport("ComponentSizingSummary"));
  EXPECT_TRUE(outputTableSummaryReports.addSummaryReport("OutdoorAirSummary"));
  EXPECT_TRUE(outputTableSummaryReports.addSummaryReport("SystemSummary"));
  EXPECT_TRUE(outputTableSummaryReports.addSummaryReport("AdaptiveComfortSummary"));

  EXPECT_EQ(4u, outputTableSummaryReports.summaryReports().size());

  ForwardTranslator ft;
  Workspace workspace = ft.translateModel(model);

  std::vector<WorkspaceObject> idf_outputTableSummaryReports = workspace.getObjectsByType(IddObjectType::Output_Table_SummaryReports);
  ASSERT_EQ(1u, idf_outputTableSummaryReports.size());
  WorkspaceObject idf_outputTableSummaryReport(idf_outputTableSummaryReports[0]);

  ASSERT_EQ(4u, idf_outputTableSummaryReport.numExtensibleGroups());
  IdfExtensibleGroup eg1 = idf_outputTableSummaryReport.extensibleGroups()[0];
  std::string s1 = eg1.getString(Output_Table_SummaryReportsExtensibleFields::ReportName).get();
  EXPECT_EQ(s1, "ComponentSizingSummary");
  IdfExtensibleGroup eg2 = idf_outputTableSummaryReport.extensibleGroups()[1];
  std::string s2 = eg2.getString(Output_Table_SummaryReportsExtensibleFields::ReportName).get();
  EXPECT_EQ(s2, "OutdoorAirSummary");
  IdfExtensibleGroup eg3 = idf_outputTableSummaryReport.extensibleGroups()[2];
  std::string s3 = eg3.getString(Output_Table_SummaryReportsExtensibleFields::ReportName).get();
  EXPECT_EQ(s3, "SystemSummary");
  IdfExtensibleGroup eg4 = idf_outputTableSummaryReport.extensibleGroups()[3];
  std::string s4 = eg4.getString(Output_Table_SummaryReportsExtensibleFields::ReportName).get();
  EXPECT_EQ(s4, "AdaptiveComfortSummary");
}
