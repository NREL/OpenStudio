/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>
#include "EnergyPlusFixture.hpp"

#include "../ForwardTranslator.hpp"
#include "../ReverseTranslator.hpp"

#include "../../model/Model.hpp"
#include "../../model/OutputControlReportingTolerances.hpp"
#include "../../model/OutputControlReportingTolerances_Impl.hpp"

#include "../../utilities/idf/IdfFile.hpp"
#include "../../utilities/idf/Workspace.hpp"
#include "../../utilities/idf/IdfObject.hpp"
#include "../../utilities/idf/WorkspaceObject.hpp"

#include <utilities/idd/OutputControl_ReportingTolerances_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::energyplus;
using namespace openstudio::model;
using namespace openstudio;

TEST_F(EnergyPlusFixture, ForwardTranslator_OutputControlReportingTolerances) {

  // Not there, and workspace should not have it
  {
    Model m;
    ForwardTranslator ft;
    Workspace w = ft.translateModel(m);

    WorkspaceObjectVector idfObjs = w.getObjectsByType(IddObjectType::OutputControl_ReportingTolerances);
    ASSERT_EQ(0u, idfObjs.size());
  }

  // It's there
  {
    Model m;
    ForwardTranslator ft;

    OutputControlReportingTolerances outputControlReportingTolerances = m.getUniqueModelObject<OutputControlReportingTolerances>();

    EXPECT_TRUE(outputControlReportingTolerances.setToleranceforTimeHeatingSetpointNotMet(1.16));
    EXPECT_TRUE(outputControlReportingTolerances.setToleranceforTimeCoolingSetpointNotMet(0.75));

    Workspace w = ft.translateModel(m);

    WorkspaceObjectVector idfObjs = w.getObjectsByType(IddObjectType::OutputControl_ReportingTolerances);
    ASSERT_EQ(1u, idfObjs.size());

    WorkspaceObject idf_reportingtolerances(idfObjs[0]);

    EXPECT_EQ(1.16, idf_reportingtolerances.getDouble(OutputControl_ReportingTolerancesFields::ToleranceforTimeHeatingSetpointNotMet, false).get());
    EXPECT_EQ(0.75, idf_reportingtolerances.getDouble(OutputControl_ReportingTolerancesFields::ToleranceforTimeCoolingSetpointNotMet, false).get());
  }
}

TEST_F(EnergyPlusFixture, ReverseTranslator_OutputControlReportingTolerances) {

  ReverseTranslator rt;

  Workspace w(StrictnessLevel::Minimal, IddFileType::EnergyPlus);

  // Not there, Model shouldn't have it either
  Model m;
  m = rt.translateWorkspace(w);
  EXPECT_FALSE(m.getOptionalUniqueModelObject<OutputControlReportingTolerances>());

  OptionalWorkspaceObject _i_outputControlReportingTolerances = w.addObject(IdfObject(IddObjectType::OutputControl_ReportingTolerances));
  ASSERT_TRUE(_i_outputControlReportingTolerances);

  EXPECT_TRUE(_i_outputControlReportingTolerances->setDouble(OutputControl_ReportingTolerancesFields::ToleranceforTimeHeatingSetpointNotMet, 1.75));
  EXPECT_TRUE(_i_outputControlReportingTolerances->setDouble(OutputControl_ReportingTolerancesFields::ToleranceforTimeCoolingSetpointNotMet, 0.25));

  m = rt.translateWorkspace(w);

  // Get the unique object
  OutputControlReportingTolerances outputControlReportingTolerances = m.getUniqueModelObject<OutputControlReportingTolerances>();
  EXPECT_EQ(1.75, outputControlReportingTolerances.toleranceforTimeHeatingSetpointNotMet());
  EXPECT_EQ(0.25, outputControlReportingTolerances.toleranceforTimeCoolingSetpointNotMet());
}
