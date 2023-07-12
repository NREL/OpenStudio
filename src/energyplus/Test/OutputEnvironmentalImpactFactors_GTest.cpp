/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>
#include "EnergyPlusFixture.hpp"

#include "../ForwardTranslator.hpp"
#include "../ReverseTranslator.hpp"

#include "../../model/Model.hpp"
#include "../../model/OutputEnvironmentalImpactFactors.hpp"
#include "../../model/OutputEnvironmentalImpactFactors_Impl.hpp"

#include "../../model/FuelFactors.hpp"

#include "../../utilities/idf/IdfFile.hpp"
#include "../../utilities/idf/Workspace.hpp"
#include "../../utilities/idf/IdfObject.hpp"
#include "../../utilities/idf/WorkspaceObject.hpp"

#include <utilities/idd/Output_EnvironmentalImpactFactors_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::energyplus;
using namespace openstudio::model;
using namespace openstudio;

TEST_F(EnergyPlusFixture, ForwardTranslator_OutputEnvironmentalImpactFactors) {

  ForwardTranslator ft;

  Model m;

  OutputEnvironmentalImpactFactors o(m);
  EXPECT_TRUE(o.setReportingFrequency("Hourly"));

  // I also made it required to actually have at least one FuelFactor
  {
    Workspace w = ft.translateModel(m);
    WorkspaceObjectVector idfObjs = w.getObjectsByType(IddObjectType::Output_EnvironmentalImpactFactors);
    EXPECT_EQ(0u, idfObjs.size());
  }

  FuelFactors fuelFactors(m);
  {
    Workspace w = ft.translateModel(m);
    WorkspaceObjectVector idfObjs = w.getObjectsByType(IddObjectType::Output_EnvironmentalImpactFactors);
    ASSERT_EQ(1u, idfObjs.size());

    WorkspaceObject idf_o(idfObjs[0]);
    EXPECT_EQ("Hourly", idf_o.getString(Output_EnvironmentalImpactFactorsFields::ReportingFrequency).get());
  }
}

TEST_F(EnergyPlusFixture, ReverseTranslator_OutputEnvironmentalImpactFactors) {

  ReverseTranslator rt;

  Workspace w(StrictnessLevel::Minimal, IddFileType::EnergyPlus);

  // Not there, Model shouldn't have it either
  {
    Model m = rt.translateWorkspace(w);
    auto outputEnvironmentalImpactFactors = m.getConcreteModelObjects<openstudio::model::OutputEnvironmentalImpactFactors>();
    EXPECT_EQ(0, outputEnvironmentalImpactFactors.size());
  }

  OptionalWorkspaceObject _i_outputEnvironmentalImpactFactors = w.addObject(IdfObject(IddObjectType::Output_EnvironmentalImpactFactors));
  ASSERT_TRUE(_i_outputEnvironmentalImpactFactors);
  EXPECT_TRUE(_i_outputEnvironmentalImpactFactors->setString(Output_EnvironmentalImpactFactorsFields::ReportingFrequency, "Hourly"));

  {
    Model m = rt.translateWorkspace(w);
    auto outputEnvironmentalImpactFactors = m.getConcreteModelObjects<openstudio::model::OutputEnvironmentalImpactFactors>();
    ASSERT_EQ(1, outputEnvironmentalImpactFactors.size());
    auto outputEnvironmentalImpactFactor = outputEnvironmentalImpactFactors[0];
    EXPECT_EQ("Hourly", outputEnvironmentalImpactFactor.reportingFrequency());
  }
}
