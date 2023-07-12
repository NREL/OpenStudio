/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>

#include "ModelFixture.hpp"
#include "../Model_Impl.hpp"
#include "../Space.hpp"
#include "../SpaceType.hpp"
#include "../ThermalZone.hpp"
#include "../ExteriorLights.hpp"
#include "../ExteriorLights_Impl.hpp"
#include "../ExteriorLightsDefinition.hpp"
#include "../ExteriorLightsDefinition_Impl.hpp"
#include "../Schedule.hpp"
#include "../ScheduleConstant.hpp"
#include "../LifeCycleCost.hpp"
#include "../Facility.hpp"
#include "../Facility_Impl.hpp"

#include <utilities/idd/IddEnums.hxx>

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, ExteriorLights_ExteriorLights) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  ASSERT_EXIT(
    {
      Model model;
      // make a new lightss object
      ExteriorLightsDefinition definition(model);
      ExteriorLights lights(definition);
      EXPECT_EQ(2u, model.numObjects());

      EXPECT_EQ(definition.handle(), lights.exteriorLightsDefinition().handle());

      exit(0);
    },
    ::testing::ExitedWithCode(0), "");
}

// Test the various setters and getters
TEST_F(ModelFixture, ExteriorLights_SettersGetters) {

  Model model;

  // make a new lightss object
  ExteriorLightsDefinition definition(model);
  ExteriorLights lights(definition);
  // This adds the alwaysOnDiscreteSchedule and its scheduletypelimit
  EXPECT_EQ(4u, model.numObjects());

  // Test definition
  EXPECT_DOUBLE_EQ(0.0, definition.designLevel());
  EXPECT_TRUE(definition.setDesignLevel(2303.3));
  EXPECT_DOUBLE_EQ(2303.3, definition.designLevel());

  ASSERT_EQ(1u, definition.instances().size());
  EXPECT_EQ(lights.handle(), definition.instances()[0].handle());
  EXPECT_EQ(definition.handle(), lights.definition().handle());

  // Test Equipment

  // Design level
  EXPECT_DOUBLE_EQ(1.0, lights.multiplier());
  EXPECT_TRUE(lights.isMultiplierDefaulted());
  EXPECT_TRUE(lights.setMultiplier(5.1));
  EXPECT_DOUBLE_EQ(5.1, lights.multiplier());
  EXPECT_FALSE(lights.isMultiplierDefaulted());

  // Schedule (exterior lights was implemented before, and historically uses an optional schedule...)
  ScheduleConstant sch(model);
  EXPECT_TRUE(lights.setSchedule(sch));
  ASSERT_TRUE(lights.schedule());
  EXPECT_EQ(sch.handle(), lights.schedule().get().handle());
  lights.resetSchedule();
  EXPECT_FALSE(lights.schedule());

  // Control Option
  EXPECT_EQ("ScheduleNameOnly", lights.controlOption());
  EXPECT_TRUE(lights.setControlOption("AstronomicalClock"));
  EXPECT_EQ("AstronomicalClock", lights.controlOption());
  EXPECT_FALSE(lights.setControlOption("Nonsense control option"));
  EXPECT_EQ("AstronomicalClock", lights.controlOption());

  // Facility
  auto facility = model.getUniqueModelObject<Facility>();
  EXPECT_EQ(facility.handle(), lights.facility().handle());
}

/* Checks that if the Equipment is removed, the Definition stays */
TEST_F(ModelFixture, ExteriorLights_RemoveEquipment) {
  {
    Model model;

    ExteriorLightsDefinition definition(model);
    ExteriorLights lights(definition);

    // This adds the alwaysOnDiscreteSchedule and its scheduletypelimit
    EXPECT_EQ(4u, model.numObjects());
    EXPECT_EQ(1u, model.getConcreteModelObjects<ExteriorLights>().size());
    EXPECT_EQ(1u, model.getConcreteModelObjects<ExteriorLightsDefinition>().size());

    std::vector<IdfObject> removed = lights.remove();
    ASSERT_EQ(1u, removed.size());
    EXPECT_EQ(IddObjectType::OS_Exterior_Lights, removed[0].iddObject().type().value());
    EXPECT_EQ(0u, model.getConcreteModelObjects<ExteriorLights>().size());
    EXPECT_EQ(1u, model.getConcreteModelObjects<ExteriorLightsDefinition>().size());
    EXPECT_EQ(3u, model.numObjects());
  }

  {
    Model model;

    ExteriorLightsDefinition definition(model);
    ExteriorLights lights1(definition);
    ExteriorLights lights2(definition);

    // This adds the alwaysOnDiscreteSchedule and its scheduletypelimit
    EXPECT_EQ(5u, model.numObjects());
    EXPECT_EQ(2u, model.getConcreteModelObjects<ExteriorLights>().size());
    EXPECT_EQ(1u, model.getConcreteModelObjects<ExteriorLightsDefinition>().size());

    std::vector<IdfObject> removed = lights1.remove();
    ASSERT_EQ(1u, removed.size());
    EXPECT_EQ(IddObjectType::OS_Exterior_Lights, removed[0].iddObject().type().value());
    EXPECT_EQ(1u, model.getConcreteModelObjects<ExteriorLights>().size());
    EXPECT_EQ(1u, model.getConcreteModelObjects<ExteriorLightsDefinition>().size());
    EXPECT_EQ(4u, model.numObjects());
  }
}

/* Checks that if the Definition is removed, all equipment is removed too */
TEST_F(ModelFixture, ExteriorLights_RemoveDefinition) {
  {
    Model model;

    ExteriorLightsDefinition definition(model);
    ExteriorLights lights(definition);

    // This adds the alwaysOnDiscreteSchedule and its scheduletypelimit
    EXPECT_EQ(4u, model.numObjects());
    EXPECT_EQ(1u, model.getConcreteModelObjects<ExteriorLights>().size());
    EXPECT_EQ(1u, model.getConcreteModelObjects<ExteriorLightsDefinition>().size());

    std::vector<IdfObject> removed = definition.remove();
    ASSERT_EQ(2u, removed.size());
    EXPECT_EQ(IddObjectType::OS_Exterior_Lights_Definition, removed[0].iddObject().type().value());
    EXPECT_EQ(IddObjectType::OS_Exterior_Lights, removed[1].iddObject().type().value());
    EXPECT_EQ(0u, model.getConcreteModelObjects<ExteriorLights>().size());
    EXPECT_EQ(0u, model.getConcreteModelObjects<ExteriorLightsDefinition>().size());
    EXPECT_EQ(2u, model.numObjects());
  }

  {
    Model model;

    ExteriorLightsDefinition definition(model);
    ExteriorLights lights1(definition);
    ExteriorLights lights2(definition);

    // This adds the alwaysOnDiscreteSchedule and its scheduletypelimit
    EXPECT_EQ(5u, model.numObjects());
    EXPECT_EQ(2u, model.getConcreteModelObjects<ExteriorLights>().size());
    EXPECT_EQ(1u, model.getConcreteModelObjects<ExteriorLightsDefinition>().size());
    std::vector<IdfObject> removed = definition.remove();
    ASSERT_EQ(3u, removed.size());
    EXPECT_EQ(IddObjectType::OS_Exterior_Lights_Definition, removed[0].iddObject().type().value());
    EXPECT_EQ(IddObjectType::OS_Exterior_Lights, removed[1].iddObject().type().value());
    EXPECT_EQ(IddObjectType::OS_Exterior_Lights, removed[2].iddObject().type().value());
    EXPECT_EQ(0u, model.getConcreteModelObjects<ExteriorLights>().size());
    EXPECT_EQ(0u, model.getConcreteModelObjects<ExteriorLightsDefinition>().size());
    EXPECT_EQ(2u, model.numObjects());
  }
}

/* If you clone the equipment, the definition comes with, and vice versa */
TEST_F(ModelFixture, ExteriorLights_Clone) {
  Model model;

  // make a new lightss object
  ExteriorLightsDefinition definition(model);
  ExteriorLights lights(definition);

  // This adds the alwaysOnDiscreteSchedule and its scheduletypelimit
  EXPECT_EQ(4u, model.numObjects());

  // clone definition into new model
  {
    Model model2;
    definition.clone(model2);
    EXPECT_EQ(1u, model2.numObjects());
  }

  // clone lights into new model
  {
    Model model2;
    lights.clone(model2);
    EXPECT_EQ(4u, model2.numObjects());
  }
}

TEST_F(ModelFixture, ExteriorLights_Costs) {
  Model model;

  // make a new lightss object
  ExteriorLightsDefinition definition(model);
  ExteriorLights lights(definition);

  // This adds the alwaysOnDiscreteSchedule and its scheduletypelimit
  EXPECT_EQ(4u, model.numObjects());

  // add a cost to the definition
  // This isn't implemented, so it shouldn't work.
  EXPECT_ANY_THROW(LifeCycleCost definitionCost(definition));
  EXPECT_EQ(4u, model.numObjects());

  // This will be useful if we implement the lifecyclecost for this
  /*
 *  // clone definition into new model
 *  {
 *    Model model2;
 *    definition.clone(model2);
 *    EXPECT_EQ(2u, model2.numObjects());
 *  }
 *
 *  // clone lights into new model
 *  {
 *    Model model2;
 *    lights.clone(model2);
 *    EXPECT_EQ(3u, model2.numObjects());
 *  }
 *
 *  // clone this into same model
 *  ModelObject lights2 = lights.clone(model);
 *  EXPECT_EQ(4u, model.numObjects());
 *  EXPECT_EQ(0u, lights.lifeCycleCosts().size());
 *  EXPECT_EQ(0u, lights2.lifeCycleCosts().size());
 *  EXPECT_EQ(1u, definition.lifeCycleCosts().size());
 *
 *  // remove one lights
 *  std::vector<IdfObject> removed = lights2.remove();
 *  EXPECT_EQ(1u, removed.size());
 *  EXPECT_EQ(3u, model.numObjects());
 *
 *  // remove definition
 *  removed = definition.remove();
 *  EXPECT_EQ(3u, removed.size());
 *  EXPECT_EQ(0u, model.numObjects());
 */
}
