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
#include "../../model/OutputDiagnostics.hpp"
#include "../../model/OutputDiagnostics_Impl.hpp"

#include "../../utilities/idf/IdfFile.hpp"
#include "../../utilities/idf/Workspace.hpp"
#include "../../utilities/idf/IdfObject.hpp"
#include "../../utilities/idf/WorkspaceObject.hpp"
#include "../../utilities/idf/IdfExtensibleGroup.hpp"
#include "../../utilities/idf/WorkspaceExtensibleGroup.hpp"

#include <utilities/idd/Output_Diagnostics_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::energyplus;
using namespace openstudio::model;
using namespace openstudio;

TEST_F(EnergyPlusFixture, ForwardTranslator_OutputDiagnostics) {

  ForwardTranslator ft;

  // Create a model
  Model m;

  // Get the unique object
  OutputDiagnostics outputDiagnostics = m.getUniqueModelObject<OutputDiagnostics>();

  EXPECT_TRUE(outputDiagnostics.keys().empty());
  // No keys: not translated
  {
    Workspace w = ft.translateModel(m);

    WorkspaceObjectVector idfObjs = w.getObjectsByType(IddObjectType::Output_Diagnostics);
    EXPECT_EQ(0u, idfObjs.size());
  }

  {
    std::vector<std::string> keys({"ReportDuringWarmup", "ReportDetailedWarmupConvergence", "ReportDuringHVACSizingSimulation"});
    EXPECT_TRUE(outputDiagnostics.setKeys(keys));
    EXPECT_EQ(3u, outputDiagnostics.keys().size());

    Workspace w = ft.translateModel(m);

    WorkspaceObjectVector idfObjs = w.getObjectsByType(IddObjectType::Output_Diagnostics);
    ASSERT_EQ(1u, idfObjs.size());
    WorkspaceObject idf_diagnostics(idfObjs[0]);

    ASSERT_EQ(3u, idf_diagnostics.extensibleGroups().size());
    for (int i = 0; i < 3; ++i) {
      EXPECT_EQ(keys[i], idf_diagnostics.extensibleGroups()[i].getString(Output_DiagnosticsExtensibleFields::Key).get());
    }
  }
}

TEST_F(EnergyPlusFixture, ReverseTranslator_OutputDiagnostics) {

  ReverseTranslator rt;

  Workspace w(StrictnessLevel::None, IddFileType::EnergyPlus);
  OptionalWorkspaceObject _i_outputDiagnostics = w.addObject(IdfObject(IddObjectType::Output_Diagnostics));
  ASSERT_TRUE(_i_outputDiagnostics);

  // Not there, Model shouldn't have it either
  {
    Model m = rt.translateWorkspace(w);
    EXPECT_FALSE(m.getOptionalUniqueModelObject<OutputDiagnostics>());
  }

  {
    std::vector<std::string> keys({"ReportDuringWarmup", "ReportDetailedWarmupConvergence",
                                   // Oops, this one is twice
                                   "ReportDuringHVACSizingSimulation", "ReportDuringHVACSizingSimulation"});

    for (const auto& key : keys) {
      IdfExtensibleGroup eg = _i_outputDiagnostics->pushExtensibleGroup();
      EXPECT_TRUE(eg.setString(Output_DiagnosticsExtensibleFields::Key, key));
    }

    EXPECT_EQ(4u, _i_outputDiagnostics->extensibleGroups().size());

    Model m = rt.translateWorkspace(w);

    // Get the unique object
    OutputDiagnostics outputDiagnostics = m.getUniqueModelObject<OutputDiagnostics>();

    // Should have three keys, one was duplicate
    ASSERT_EQ(3u, outputDiagnostics.keys().size());
    for (int i = 0; i < 3; ++i) {
      EXPECT_EQ(keys[i], outputDiagnostics.keys()[i]);
    }
  }
}
