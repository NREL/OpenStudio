/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>
#include "EnergyPlusFixture.hpp"

#include "../ForwardTranslator.hpp"
#include "../ReverseTranslator.hpp"

#include "../../model/Model.hpp"
#include "../../model/OutputDebuggingData.hpp"
#include "../../model/OutputDebuggingData_Impl.hpp"

#include "../../utilities/idf/IdfFile.hpp"
#include "../../utilities/idf/Workspace.hpp"
#include "../../utilities/idf/IdfObject.hpp"
#include "../../utilities/idf/WorkspaceObject.hpp"

#include <utilities/idd/Output_DebuggingData_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::energyplus;
using namespace openstudio::model;
using namespace openstudio;

TEST_F(EnergyPlusFixture, ForwardTranslator_OutputDebuggingData) {

  ForwardTranslator ft;

  // Create a model
  Model m;

  // Get the unique object
  auto outputDebuggingData = m.getUniqueModelObject<OutputDebuggingData>();

  {
    EXPECT_TRUE(outputDebuggingData.setReportDebuggingData(true));
    EXPECT_TRUE(outputDebuggingData.setReportDuringWarmup(false));

    Workspace w = ft.translateModel(m);

    WorkspaceObjectVector idfObjs = w.getObjectsByType(IddObjectType::Output_DebuggingData);
    ASSERT_EQ(1u, idfObjs.size());

    WorkspaceObject idf_debugging(idfObjs[0]);

    // E+ uses a numeric field to store this.. I'm using getInt to avoid float comparisons
    EXPECT_EQ("Yes", idf_debugging.getString(Output_DebuggingDataFields::ReportDebuggingData).get());
    EXPECT_EQ("No", idf_debugging.getString(Output_DebuggingDataFields::ReportDuringWarmup).get());
  }

  {
    EXPECT_TRUE(outputDebuggingData.setReportDebuggingData(false));
    EXPECT_TRUE(outputDebuggingData.setReportDuringWarmup(true));

    Workspace w = ft.translateModel(m);

    WorkspaceObjectVector idfObjs = w.getObjectsByType(IddObjectType::Output_DebuggingData);
    ASSERT_EQ(1u, idfObjs.size());

    WorkspaceObject idf_debugging(idfObjs[0]);

    // E+ uses a numeric field to store this..
    EXPECT_EQ("No", idf_debugging.getString(Output_DebuggingDataFields::ReportDebuggingData).get());
    EXPECT_EQ("Yes", idf_debugging.getString(Output_DebuggingDataFields::ReportDuringWarmup).get());
  }
}

TEST_F(EnergyPlusFixture, ReverseTranslator_OutputDebuggingData) {

  ReverseTranslator rt;

  Workspace w(StrictnessLevel::Minimal, IddFileType::EnergyPlus);

  // Not there, Model shouldn't have it either
  {
    Model m = rt.translateWorkspace(w);
    EXPECT_FALSE(m.getOptionalUniqueModelObject<OutputDebuggingData>());
  }

  OptionalWorkspaceObject _i_outputDebuggingData = w.addObject(IdfObject(IddObjectType::Output_DebuggingData));
  ASSERT_TRUE(_i_outputDebuggingData);

  {
    EXPECT_TRUE(_i_outputDebuggingData->setString(Output_DebuggingDataFields::ReportDebuggingData, "Yes"));
    EXPECT_TRUE(_i_outputDebuggingData->setString(Output_DebuggingDataFields::ReportDuringWarmup, "No"));

    Model m = rt.translateWorkspace(w);

    // Get the unique object
    auto outputDebuggingData = m.getUniqueModelObject<OutputDebuggingData>();
    EXPECT_TRUE(outputDebuggingData.reportDebuggingData());
    EXPECT_FALSE(outputDebuggingData.reportDuringWarmup());
  }

  {
    EXPECT_TRUE(_i_outputDebuggingData->setString(Output_DebuggingDataFields::ReportDebuggingData, "No"));
    EXPECT_TRUE(_i_outputDebuggingData->setString(Output_DebuggingDataFields::ReportDuringWarmup, "Yes"));

    Model m = rt.translateWorkspace(w);

    // Get the unique object
    auto outputDebuggingData = m.getUniqueModelObject<OutputDebuggingData>();
    EXPECT_FALSE(outputDebuggingData.reportDebuggingData());
    EXPECT_TRUE(outputDebuggingData.reportDuringWarmup());
  }
}
