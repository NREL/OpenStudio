/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>
#include "EnergyPlusFixture.hpp"

#include "../ForwardTranslator.hpp"
#include "../ReverseTranslator.hpp"

#include "../../model/Model.hpp"
#include "../../model/SimulationControl.hpp"
#include "../../model/SimulationControl_Impl.hpp"

#include "../../model/PlantLoop.hpp"
#include "../../model/SizingPlant.hpp"
#include "../../model/DesignDay.hpp"

#include "../../utilities/idf/IdfFile.hpp"
#include "../../utilities/idf/Workspace.hpp"
#include "../../utilities/idf/IdfObject.hpp"
#include "../../utilities/idf/WorkspaceObject.hpp"
#include "../../utilities/idf/IdfExtensibleGroup.hpp"
#include "../../utilities/idf/WorkspaceExtensibleGroup.hpp"

#include <utilities/idd/SimulationControl_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::energyplus;
using namespace openstudio::model;
using namespace openstudio;

TEST_F(EnergyPlusFixture, ForwardTranslator_SimulationControl) {

  ForwardTranslator ft;

  // Create a model
  Model m;

  // Get the unique object
  auto simCon = m.getUniqueModelObject<SimulationControl>();

  // Check all cases where a single output request is True so we know we assigned the fields correctly
  auto boolToString = [](bool b) { return b ? "Yes" : "No"; };

  for (int i = 0; i < 6; ++i) {
    bool status[] = {false, false, false, false, false, false};
    status[i] = true;
    EXPECT_TRUE(simCon.setDoZoneSizingCalculation(status[0]));
    EXPECT_TRUE(simCon.setDoSystemSizingCalculation(status[1]));
    EXPECT_TRUE(simCon.setDoPlantSizingCalculation(status[2]));
    EXPECT_TRUE(simCon.setRunSimulationforSizingPeriods(status[3]));
    EXPECT_TRUE(simCon.setRunSimulationforWeatherFileRunPeriods(status[4]));
    EXPECT_TRUE(simCon.setDoHVACSizingSimulationforSizingPeriods(status[5]));

    Workspace w = ft.translateModel(m);

    WorkspaceObjectVector idfObjs = w.getObjectsByType(IddObjectType::SimulationControl);
    ASSERT_EQ(1u, idfObjs.size());

    WorkspaceObject idf_simCon(idfObjs[0]);

    EXPECT_EQ(boolToString(status[0]), idf_simCon.getString(SimulationControlFields::DoZoneSizingCalculation, false).get());
    EXPECT_EQ(boolToString(status[1]), idf_simCon.getString(SimulationControlFields::DoSystemSizingCalculation, false).get());
    EXPECT_EQ(boolToString(status[2]), idf_simCon.getString(SimulationControlFields::DoPlantSizingCalculation, false).get());
    EXPECT_EQ(boolToString(status[3]), idf_simCon.getString(SimulationControlFields::RunSimulationforSizingPeriods, false).get());
    EXPECT_EQ(boolToString(status[4]), idf_simCon.getString(SimulationControlFields::RunSimulationforWeatherFileRunPeriods, false).get());
    EXPECT_EQ(boolToString(status[5]), idf_simCon.getString(SimulationControlFields::DoHVACSizingSimulationforSizingPeriods, false).get());
  }
}

TEST_F(EnergyPlusFixture, ForwardTranslator_SimulationControl_Logic) {

  ForwardTranslator ft;

  // Create a model
  Model m;

  // Get the unique object
  auto simCon = m.getUniqueModelObject<SimulationControl>();

  {
    Workspace w = ft.translateModel(m);

    WorkspaceObjectVector idfObjs = w.getObjectsByType(IddObjectType::SimulationControl);
    ASSERT_EQ(1u, idfObjs.size());
    WorkspaceObject idf_simCon(idfObjs[0]);

    EXPECT_EQ("No", idf_simCon.getString(SimulationControlFields::DoZoneSizingCalculation, false).get());
    EXPECT_EQ("No", idf_simCon.getString(SimulationControlFields::DoSystemSizingCalculation, false).get());
    EXPECT_EQ("No", idf_simCon.getString(SimulationControlFields::DoPlantSizingCalculation, false).get());
    EXPECT_EQ("Yes", idf_simCon.getString(SimulationControlFields::RunSimulationforSizingPeriods, false).get());
    EXPECT_EQ("Yes", idf_simCon.getString(SimulationControlFields::RunSimulationforWeatherFileRunPeriods, false).get());

    EXPECT_FALSE(idf_simCon.getString(SimulationControlFields::DoHVACSizingSimulationforSizingPeriods, false, true));
    EXPECT_FALSE(idf_simCon.getString(SimulationControlFields::MaximumNumberofHVACSizingSimulationPasses, false, true));
  }

  // Add a plantLoop, and a SizingPeriod, so it forces DoPlantSizingCalculation
  PlantLoop p(m);
  SizingPlant sizingPlant = p.sizingPlant();
  EXPECT_TRUE(sizingPlant.setSizingOption("NonCoincident"));
  DesignDay dd(m);

  {
    Workspace w = ft.translateModel(m);

    WorkspaceObjectVector idfObjs = w.getObjectsByType(IddObjectType::SimulationControl);
    ASSERT_EQ(1u, idfObjs.size());
    WorkspaceObject idf_simCon(idfObjs[0]);

    EXPECT_EQ("No", idf_simCon.getString(SimulationControlFields::DoZoneSizingCalculation, false).get());
    EXPECT_EQ("No", idf_simCon.getString(SimulationControlFields::DoSystemSizingCalculation, false).get());
    // Now it's 'Yes' automagically
    EXPECT_EQ("Yes", idf_simCon.getString(SimulationControlFields::DoPlantSizingCalculation, false).get());
    EXPECT_EQ("Yes", idf_simCon.getString(SimulationControlFields::RunSimulationforSizingPeriods, false).get());
    EXPECT_EQ("Yes", idf_simCon.getString(SimulationControlFields::RunSimulationforWeatherFileRunPeriods, false).get());
    EXPECT_FALSE(idf_simCon.getString(SimulationControlFields::DoHVACSizingSimulationforSizingPeriods, false, true));
    EXPECT_FALSE(idf_simCon.getString(SimulationControlFields::MaximumNumberofHVACSizingSimulationPasses, false, true));
  }

  // Set the SizingPlant to "Coincident" => should force the "DoHVACSizingSimulationforSizingPeriods"
  EXPECT_TRUE(sizingPlant.setSizingOption("Coincident"));
  {
    Workspace w = ft.translateModel(m);

    WorkspaceObjectVector idfObjs = w.getObjectsByType(IddObjectType::SimulationControl);
    ASSERT_EQ(1u, idfObjs.size());
    WorkspaceObject idf_simCon(idfObjs[0]);

    EXPECT_EQ("No", idf_simCon.getString(SimulationControlFields::DoZoneSizingCalculation, false).get());
    EXPECT_EQ("No", idf_simCon.getString(SimulationControlFields::DoSystemSizingCalculation, false).get());
    EXPECT_EQ("Yes", idf_simCon.getString(SimulationControlFields::DoPlantSizingCalculation, false).get());
    EXPECT_EQ("Yes", idf_simCon.getString(SimulationControlFields::RunSimulationforSizingPeriods, false).get());
    EXPECT_EQ("Yes", idf_simCon.getString(SimulationControlFields::RunSimulationforWeatherFileRunPeriods, false).get());
    // Now it's 'Yes' automagically
    EXPECT_EQ("Yes", idf_simCon.getString(SimulationControlFields::DoHVACSizingSimulationforSizingPeriods, false).get());
    EXPECT_FALSE(idf_simCon.getString(SimulationControlFields::MaximumNumberofHVACSizingSimulationPasses, false, true));
  }

  // Check if the new Integer is translated too
  EXPECT_TRUE(simCon.setMaximumNumberofHVACSizingSimulationPasses(2));
  {
    Workspace w = ft.translateModel(m);

    WorkspaceObjectVector idfObjs = w.getObjectsByType(IddObjectType::SimulationControl);
    ASSERT_EQ(1u, idfObjs.size());
    WorkspaceObject idf_simCon(idfObjs[0]);

    EXPECT_EQ("No", idf_simCon.getString(SimulationControlFields::DoZoneSizingCalculation, false).get());
    EXPECT_EQ("No", idf_simCon.getString(SimulationControlFields::DoSystemSizingCalculation, false).get());
    EXPECT_EQ("Yes", idf_simCon.getString(SimulationControlFields::DoPlantSizingCalculation, false).get());
    EXPECT_EQ("Yes", idf_simCon.getString(SimulationControlFields::RunSimulationforSizingPeriods, false).get());
    EXPECT_EQ("Yes", idf_simCon.getString(SimulationControlFields::RunSimulationforWeatherFileRunPeriods, false).get());
    // Now it's 'Yes' automagically
    EXPECT_EQ("Yes", idf_simCon.getString(SimulationControlFields::DoHVACSizingSimulationforSizingPeriods, false).get());
    EXPECT_EQ(2, idf_simCon.getInt(SimulationControlFields::MaximumNumberofHVACSizingSimulationPasses, false).get());
  }
}

TEST_F(EnergyPlusFixture, ReverseTranslator_SimulationControl) {

  ReverseTranslator rt;

  Workspace w(StrictnessLevel::Minimal, IddFileType::EnergyPlus);

  // Not there, Model shouldn't have it either
  Model m = rt.translateWorkspace(w);
  EXPECT_FALSE(m.getOptionalUniqueModelObject<SimulationControl>());

  OptionalWorkspaceObject _i_simCon = w.addObject(IdfObject(IddObjectType::SimulationControl));
  ASSERT_TRUE(_i_simCon);

  EXPECT_TRUE(_i_simCon->setInt(SimulationControlFields::MaximumNumberofHVACSizingSimulationPasses, 2));

  auto boolToString = [](bool b) { return b ? "Yes" : "No"; };

  for (int i = 0; i < 6; ++i) {
    bool status[] = {false, false, false, false, false, false};
    status[i] = true;

    EXPECT_TRUE(_i_simCon->setString(SimulationControlFields::DoZoneSizingCalculation, boolToString(status[0])));
    EXPECT_TRUE(_i_simCon->setString(SimulationControlFields::DoSystemSizingCalculation, boolToString(status[1])));
    EXPECT_TRUE(_i_simCon->setString(SimulationControlFields::DoPlantSizingCalculation, boolToString(status[2])));
    EXPECT_TRUE(_i_simCon->setString(SimulationControlFields::RunSimulationforSizingPeriods, boolToString(status[3])));
    EXPECT_TRUE(_i_simCon->setString(SimulationControlFields::RunSimulationforWeatherFileRunPeriods, boolToString(status[4])));
    EXPECT_TRUE(_i_simCon->setString(SimulationControlFields::DoHVACSizingSimulationforSizingPeriods, boolToString(status[5])));

    Model m = rt.translateWorkspace(w);
    // Get the unique object
    auto simCon = m.getUniqueModelObject<SimulationControl>();

    EXPECT_EQ(status[0], simCon.doZoneSizingCalculation());
    EXPECT_EQ(status[1], simCon.doSystemSizingCalculation());
    EXPECT_EQ(status[2], simCon.doPlantSizingCalculation());
    EXPECT_EQ(status[3], simCon.runSimulationforSizingPeriods());
    EXPECT_EQ(status[4], simCon.runSimulationforWeatherFileRunPeriods());
    EXPECT_EQ(status[5], simCon.doHVACSizingSimulationforSizingPeriods());

    EXPECT_EQ(2, simCon.maximumNumberofHVACSizingSimulationPasses());
  }
}
