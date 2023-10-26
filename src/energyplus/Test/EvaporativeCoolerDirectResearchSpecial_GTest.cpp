/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>
#include "EnergyPlusFixture.hpp"

#include "../ForwardTranslator.hpp"
#include "../ReverseTranslator.hpp"

#include "../../model/Model.hpp"
#include "../../model/Schedule.hpp"

#include "../../model/EvaporativeCoolerDirectResearchSpecial.hpp"

#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/EvaporativeCooler_Direct_ResearchSpecial_FieldEnums.hxx>

#include <resources.hxx>

#include <sstream>

using namespace openstudio::energyplus;
using namespace openstudio::model;
using namespace openstudio;

TEST_F(EnergyPlusFixture, ForwardTranslator_EvaporativeCoolerDirectResearchSpecial_LimitFields) {
  Model m;

  Schedule sch = m.alwaysOnDiscreteSchedule();
  EvaporativeCoolerDirectResearchSpecial e(m, sch);

  EXPECT_TRUE(e.setPrimaryAirDesignFlowRate(1.05));
  EXPECT_TRUE(e.setEvaporativeOperationMinimumDrybulbTemperature(16));
  EXPECT_TRUE(e.setEvaporativeOperationMaximumLimitWetbulbTemperature(24));
  EXPECT_TRUE(e.setEvaporativeOperationMaximumLimitDrybulbTemperature(28));

  ASSERT_TRUE(e.primaryAirDesignFlowRate());
  EXPECT_DOUBLE_EQ(1.05, e.primaryAirDesignFlowRate().get());
  EXPECT_DOUBLE_EQ(16, e.evaporativeOperationMinimumDrybulbTemperature());
  EXPECT_DOUBLE_EQ(24, e.evaporativeOperationMaximumLimitWetbulbTemperature());
  EXPECT_DOUBLE_EQ(28, e.evaporativeOperationMaximumLimitDrybulbTemperature());

  ForwardTranslator ft;
  Workspace w = ft.translateModel(m);

  WorkspaceObjectVector idfObjs = w.getObjectsByType(IddObjectType::EvaporativeCooler_Direct_ResearchSpecial);
  ASSERT_EQ(1u, idfObjs.size());

  WorkspaceObject idfObj(idfObjs[0]);

  EXPECT_DOUBLE_EQ(e.primaryAirDesignFlowRate().get(),
                   idfObj.getDouble(EvaporativeCooler_Direct_ResearchSpecialFields::PrimaryAirDesignFlowRate).get());
  EXPECT_DOUBLE_EQ(e.evaporativeOperationMinimumDrybulbTemperature(),
                   idfObj.getDouble(EvaporativeCooler_Direct_ResearchSpecialFields::EvaporativeOperationMinimumDrybulbTemperature).get());
  EXPECT_DOUBLE_EQ(e.evaporativeOperationMaximumLimitWetbulbTemperature(),
                   idfObj.getDouble(EvaporativeCooler_Direct_ResearchSpecialFields::EvaporativeOperationMaximumLimitWetbulbTemperature).get());
  EXPECT_DOUBLE_EQ(e.evaporativeOperationMaximumLimitDrybulbTemperature(),
                   idfObj.getDouble(EvaporativeCooler_Direct_ResearchSpecialFields::EvaporativeOperationMaximumLimitDrybulbTemperature).get());
}
