/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
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
  auto outputDiagnostics = m.getUniqueModelObject<OutputDiagnostics>();

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

  Workspace w(StrictnessLevel::Minimal, IddFileType::EnergyPlus);
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
    auto outputDiagnostics = m.getUniqueModelObject<OutputDiagnostics>();

    // Should have three keys, one was duplicate
    ASSERT_EQ(3u, outputDiagnostics.keys().size());
    for (int i = 0; i < 3; ++i) {
      EXPECT_EQ(keys[i], outputDiagnostics.keys()[i]);
    }
  }
}
