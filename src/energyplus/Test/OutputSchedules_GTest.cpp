/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>
#include "EnergyPlusFixture.hpp"

#include "../ForwardTranslator.hpp"
#include "../ReverseTranslator.hpp"

#include "../../model/Model.hpp"
#include "../../model/OutputSchedules.hpp"
#include "../../model/OutputSchedules_Impl.hpp"

#include "../../utilities/idf/IdfFile.hpp"
#include "../../utilities/idf/Workspace.hpp"
#include "../../utilities/idf/IdfObject.hpp"
#include "../../utilities/idf/WorkspaceObject.hpp"

#include <utilities/idd/Output_Schedules_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::energyplus;
using namespace openstudio::model;
using namespace openstudio;

TEST_F(EnergyPlusFixture, ForwardTranslator_OutputSchedules) {

  ForwardTranslator ft;

  // Create a model
  Model m;

  // Get the unique object
  OutputSchedules outputSchedules = m.getUniqueModelObject<OutputSchedules>();
  EXPECT_EQ("Hourly", outputSchedules.keyField());
  {
    Workspace w = ft.translateModel(m);

    WorkspaceObjectVector idfObjs = w.getObjectsByType(IddObjectType::Output_Schedules);
    EXPECT_EQ(1u, idfObjs.size());
    WorkspaceObject idf_schedules(idfObjs[0]);

    EXPECT_EQ("Hourly", idf_schedules.getString(Output_SchedulesFields::KeyField).get());
  }
  {
    EXPECT_TRUE(outputSchedules.setKeyField("Timestep"));
    Workspace w = ft.translateModel(m);

    WorkspaceObjectVector idfObjs = w.getObjectsByType(IddObjectType::Output_Schedules);
    EXPECT_EQ(1u, idfObjs.size());
    WorkspaceObject idf_schedules(idfObjs[0]);

    EXPECT_EQ("Timestep", idf_schedules.getString(Output_SchedulesFields::KeyField).get());
  }
}

TEST_F(EnergyPlusFixture, ReverseTranslator_OutputSchedules) {

  ReverseTranslator rt;

  Workspace w(StrictnessLevel::Minimal, IddFileType::EnergyPlus);

  // Not there, Model shouldn't have it either
  {
    Model m = rt.translateWorkspace(w);
    EXPECT_FALSE(m.getOptionalUniqueModelObject<OutputSchedules>());
  }

  OptionalWorkspaceObject _i_outputSchedules = w.addObject(IdfObject(IddObjectType::Output_Schedules));
  ASSERT_TRUE(_i_outputSchedules);

  // There but no keys, Model shouldn't have it either
  {
    Model m = rt.translateWorkspace(w);
    EXPECT_FALSE(m.getOptionalUniqueModelObject<OutputSchedules>());
  }

  _i_outputSchedules->setString(Output_SchedulesFields::KeyField, "Hourly");

  {
    Model m = rt.translateWorkspace(w);
    ASSERT_TRUE(m.getOptionalUniqueModelObject<OutputSchedules>());
    auto outputSchedules = m.getUniqueModelObject<OutputSchedules>();
    EXPECT_EQ("Hourly", outputSchedules.keyField());
  }

  _i_outputSchedules->setString(Output_SchedulesFields::KeyField, "Timestep");
  {
    Model m = rt.translateWorkspace(w);
    ASSERT_TRUE(m.getOptionalUniqueModelObject<OutputSchedules>());
    auto outputSchedules = m.getUniqueModelObject<OutputSchedules>();
    EXPECT_EQ("Timestep", outputSchedules.keyField());
  }
}
