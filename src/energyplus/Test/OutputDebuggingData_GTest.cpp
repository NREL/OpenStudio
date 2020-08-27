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
  OutputDebuggingData outputDebuggingData = m.getUniqueModelObject<OutputDebuggingData>();

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

  Workspace w(StrictnessLevel::None, IddFileType::EnergyPlus);

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
    OutputDebuggingData outputDebuggingData = m.getUniqueModelObject<OutputDebuggingData>();
    EXPECT_TRUE(outputDebuggingData.reportDebuggingData());
    EXPECT_FALSE(outputDebuggingData.reportDuringWarmup());
  }

  {
    EXPECT_TRUE(_i_outputDebuggingData->setString(Output_DebuggingDataFields::ReportDebuggingData, "No"));
    EXPECT_TRUE(_i_outputDebuggingData->setString(Output_DebuggingDataFields::ReportDuringWarmup, "Yes"));

    Model m = rt.translateWorkspace(w);

    // Get the unique object
    OutputDebuggingData outputDebuggingData = m.getUniqueModelObject<OutputDebuggingData>();
    EXPECT_FALSE(outputDebuggingData.reportDebuggingData());
    EXPECT_TRUE(outputDebuggingData.reportDuringWarmup());
  }
}
