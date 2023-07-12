/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>
#include "EnergyPlusFixture.hpp"

#include "../ForwardTranslator.hpp"

#include "../../model/AvailabilityManagerScheduledOff.hpp"
#include "../../model/Model.hpp"
#include "../../model/PlantLoop.hpp"
#include "../../model/ScheduleConstant.hpp"

#include <utilities/idd/AvailabilityManager_ScheduledOff_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::energyplus;
using namespace openstudio::model;
using namespace openstudio;

TEST_F(EnergyPlusFixture, ForwardTranslator_AvailabilityManagerScheduledOff) {

  Model m;

  AvailabilityManagerScheduledOff avm(m);

  // Test Schedule
  ScheduleConstant sch(m);
  sch.setValue(50);
  EXPECT_TRUE(avm.setSchedule(sch));

  // Assign it to a plant loop
  PlantLoop p(m);
  p.addAvailabilityManager(avm);

  // ForwardTranslate
  ForwardTranslator forwardTranslator;
  Workspace workspace = forwardTranslator.translateModel(m);

  WorkspaceObjectVector idfObjs(workspace.getObjectsByType(IddObjectType::AvailabilityManager_ScheduledOff));
  EXPECT_EQ(1u, idfObjs.size());
  WorkspaceObject idf_avm(idfObjs[0]);

  // Schedule Name
  ASSERT_EQ(sch.name().get(), idf_avm.getString(AvailabilityManager_ScheduledOffFields::ScheduleName).get());
}
