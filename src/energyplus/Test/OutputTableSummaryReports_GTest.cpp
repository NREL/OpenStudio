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
