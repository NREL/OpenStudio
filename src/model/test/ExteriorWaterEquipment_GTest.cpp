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
#include "../ExteriorWaterEquipment.hpp"
#include "../ExteriorWaterEquipment_Impl.hpp"
#include "../ExteriorWaterEquipmentDefinition.hpp"
#include "../ExteriorWaterEquipmentDefinition_Impl.hpp"
#include "../Schedule.hpp"
#include "../ScheduleConstant.hpp"
#include "../LifeCycleCost.hpp"
#include "../Facility.hpp"
#include "../Facility_Impl.hpp"

#include <utilities/idd/IddEnums.hxx>

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, ExteriorWaterEquipment_ExteriorWaterEquipment) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  ASSERT_EXIT(
    {
      Model model;
      // make a new waterEqs object
      ExteriorWaterEquipmentDefinition definition(model);
      ExteriorWaterEquipment waterEq(definition);
      EXPECT_EQ(2u, model.numObjects());

      EXPECT_EQ(definition.handle(), waterEq.exteriorWaterEquipmentDefinition().handle());

      exit(0);
    },
    ::testing::ExitedWithCode(0), "");
}

// Test the various setters and getters
TEST_F(ModelFixture, ExteriorWaterEquipment_SettersGetters) {

  Model model;

  // make a new waterEqs object
  ExteriorWaterEquipmentDefinition definition(model);
  ExteriorWaterEquipment waterEq(definition);
  // This adds the alwaysOnDiscreteSchedule and its scheduletypelimit
  EXPECT_EQ(4u, model.numObjects());

  // Test definition
  EXPECT_DOUBLE_EQ(0.0, definition.designLevel());
  EXPECT_TRUE(definition.setDesignLevel(2303.3));
  EXPECT_DOUBLE_EQ(2303.3, definition.designLevel());

  ASSERT_EQ(1u, definition.instances().size());
  EXPECT_EQ(waterEq.handle(), definition.instances()[0].handle());
  EXPECT_EQ(definition.handle(), waterEq.definition().handle());

  // Test Equipment

  // Design level
  EXPECT_DOUBLE_EQ(1.0, waterEq.multiplier());
  EXPECT_TRUE(waterEq.isMultiplierDefaulted());
  EXPECT_TRUE(waterEq.setMultiplier(5.1));
  EXPECT_DOUBLE_EQ(5.1, waterEq.multiplier());
  EXPECT_FALSE(waterEq.isMultiplierDefaulted());

  // Schedule
  Schedule alwaysOn = model.alwaysOnDiscreteSchedule();
  EXPECT_EQ(alwaysOn.handle(), waterEq.schedule().handle());
  ScheduleConstant sch(model);
  EXPECT_TRUE(waterEq.setSchedule(sch));
  EXPECT_EQ(sch.handle(), waterEq.schedule().handle());
  waterEq.resetSchedule();
  EXPECT_EQ(alwaysOn.handle(), waterEq.schedule().handle());

  // Facility
  auto facility = model.getUniqueModelObject<Facility>();
  EXPECT_EQ(facility.handle(), waterEq.facility().handle());
}

/* Checks that if the Equipment is removed, the Definition stays */
TEST_F(ModelFixture, ExteriorWaterEquipment_RemoveEquipment) {
  {
    Model model;

    ExteriorWaterEquipmentDefinition definition(model);
    ExteriorWaterEquipment waterEq(definition);

    // This adds the alwaysOnDiscreteSchedule and its scheduletypelimit
    EXPECT_EQ(4u, model.numObjects());
    EXPECT_EQ(1u, model.getConcreteModelObjects<ExteriorWaterEquipment>().size());
    EXPECT_EQ(1u, model.getConcreteModelObjects<ExteriorWaterEquipmentDefinition>().size());

    std::vector<IdfObject> removed = waterEq.remove();
    ASSERT_EQ(1u, removed.size());
    EXPECT_EQ(IddObjectType::OS_Exterior_WaterEquipment, removed[0].iddObject().type().value());
    EXPECT_EQ(0u, model.getConcreteModelObjects<ExteriorWaterEquipment>().size());
    EXPECT_EQ(1u, model.getConcreteModelObjects<ExteriorWaterEquipmentDefinition>().size());
    EXPECT_EQ(3u, model.numObjects());
  }

  {
    Model model;

    ExteriorWaterEquipmentDefinition definition(model);
    ExteriorWaterEquipment waterEq1(definition);
    ExteriorWaterEquipment waterEq2(definition);

    // This adds the alwaysOnDiscreteSchedule and its scheduletypelimit
    EXPECT_EQ(5u, model.numObjects());
    EXPECT_EQ(2u, model.getConcreteModelObjects<ExteriorWaterEquipment>().size());
    EXPECT_EQ(1u, model.getConcreteModelObjects<ExteriorWaterEquipmentDefinition>().size());

    std::vector<IdfObject> removed = waterEq1.remove();
    ASSERT_EQ(1u, removed.size());
    EXPECT_EQ(IddObjectType::OS_Exterior_WaterEquipment, removed[0].iddObject().type().value());
    EXPECT_EQ(1u, model.getConcreteModelObjects<ExteriorWaterEquipment>().size());
    EXPECT_EQ(1u, model.getConcreteModelObjects<ExteriorWaterEquipmentDefinition>().size());
    EXPECT_EQ(4u, model.numObjects());
  }
}

/* Checks that if the Definition is removed, all equipment is removed too */
TEST_F(ModelFixture, ExteriorWaterEquipment_RemoveDefinition) {
  {
    Model model;

    ExteriorWaterEquipmentDefinition definition(model);
    ExteriorWaterEquipment waterEq(definition);

    // This adds the alwaysOnDiscreteSchedule and its scheduletypelimit
    EXPECT_EQ(4u, model.numObjects());
    EXPECT_EQ(1u, model.getConcreteModelObjects<ExteriorWaterEquipment>().size());
    EXPECT_EQ(1u, model.getConcreteModelObjects<ExteriorWaterEquipmentDefinition>().size());

    std::vector<IdfObject> removed = definition.remove();
    ASSERT_EQ(2u, removed.size());
    EXPECT_EQ(IddObjectType::OS_Exterior_WaterEquipment_Definition, removed[0].iddObject().type().value());
    EXPECT_EQ(IddObjectType::OS_Exterior_WaterEquipment, removed[1].iddObject().type().value());
    EXPECT_EQ(0u, model.getConcreteModelObjects<ExteriorWaterEquipment>().size());
    EXPECT_EQ(0u, model.getConcreteModelObjects<ExteriorWaterEquipmentDefinition>().size());
    EXPECT_EQ(2u, model.numObjects());
  }

  {
    Model model;

    ExteriorWaterEquipmentDefinition definition(model);
    ExteriorWaterEquipment waterEq1(definition);
    ExteriorWaterEquipment waterEq2(definition);

    // This adds the alwaysOnDiscreteSchedule and its scheduletypelimit
    EXPECT_EQ(5u, model.numObjects());
    EXPECT_EQ(2u, model.getConcreteModelObjects<ExteriorWaterEquipment>().size());
    EXPECT_EQ(1u, model.getConcreteModelObjects<ExteriorWaterEquipmentDefinition>().size());
    std::vector<IdfObject> removed = definition.remove();
    ASSERT_EQ(3u, removed.size());
    EXPECT_EQ(IddObjectType::OS_Exterior_WaterEquipment_Definition, removed[0].iddObject().type().value());
    EXPECT_EQ(IddObjectType::OS_Exterior_WaterEquipment, removed[1].iddObject().type().value());
    EXPECT_EQ(IddObjectType::OS_Exterior_WaterEquipment, removed[2].iddObject().type().value());
    EXPECT_EQ(0u, model.getConcreteModelObjects<ExteriorWaterEquipment>().size());
    EXPECT_EQ(0u, model.getConcreteModelObjects<ExteriorWaterEquipmentDefinition>().size());
    EXPECT_EQ(2u, model.numObjects());
  }
}

/* If you clone the equipment, the definition comes with, and vice versa */
TEST_F(ModelFixture, ExteriorWaterEquipment_Clone) {
  Model model;

  // make a new waterEqs object
  ExteriorWaterEquipmentDefinition definition(model);
  ExteriorWaterEquipment waterEq(definition);

  // This adds the alwaysOnDiscreteSchedule and its scheduletypelimit
  EXPECT_EQ(4u, model.numObjects());

  // clone definition into new model
  {
    Model model2;
    definition.clone(model2);
    EXPECT_EQ(1u, model2.numObjects());
  }

  // clone waterEq into new model
  {
    Model model2;
    waterEq.clone(model2);
    EXPECT_EQ(4u, model2.numObjects());
  }
}

TEST_F(ModelFixture, ExteriorWaterEquipment_Costs) {
  Model model;

  // make a new waterEqs object
  ExteriorWaterEquipmentDefinition definition(model);
  ExteriorWaterEquipment waterEq(definition);

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
 *  // clone waterEq into new model
 *  {
 *    Model model2;
 *    waterEq.clone(model2);
 *    EXPECT_EQ(3u, model2.numObjects());
 *  }
 *
 *  // clone this into same model
 *  ModelObject waterEq2 = waterEq.clone(model);
 *  EXPECT_EQ(4u, model.numObjects());
 *  EXPECT_EQ(0u, waterEq.lifeCycleCosts().size());
 *  EXPECT_EQ(0u, waterEq2.lifeCycleCosts().size());
 *  EXPECT_EQ(1u, definition.lifeCycleCosts().size());
 *
 *  // remove one waterEq
 *  std::vector<IdfObject> removed = waterEq2.remove();
 *  EXPECT_EQ(1u, removed.size());
 *  EXPECT_EQ(3u, model.numObjects());
 *
 *  // remove definition
 *  removed = definition.remove();
 *  EXPECT_EQ(3u, removed.size());
 *  EXPECT_EQ(0u, model.numObjects());
 */
}
