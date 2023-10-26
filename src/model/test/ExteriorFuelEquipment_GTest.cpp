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
#include "../ExteriorFuelEquipment.hpp"
#include "../ExteriorFuelEquipment_Impl.hpp"
#include "../ExteriorFuelEquipmentDefinition.hpp"
#include "../ExteriorFuelEquipmentDefinition_Impl.hpp"
#include "../Schedule.hpp"
#include "../ScheduleConstant.hpp"
#include "../LifeCycleCost.hpp"
#include "../Facility.hpp"
#include "../Facility_Impl.hpp"

#include <utilities/idd/IddEnums.hxx>

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, ExteriorFuelEquipment_ExteriorFuelEquipment) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  ASSERT_EXIT(
    {
      Model model;
      // make a new fuelEqs object
      ExteriorFuelEquipmentDefinition definition(model);
      ExteriorFuelEquipment fuelEq(definition);
      EXPECT_EQ(2u, model.numObjects());

      EXPECT_EQ(definition.handle(), fuelEq.exteriorFuelEquipmentDefinition().handle());

      exit(0);
    },
    ::testing::ExitedWithCode(0), "");
}

// Test the various setters and getters
TEST_F(ModelFixture, ExteriorFuelEquipment_SettersGetters) {

  Model model;

  // make a new fuelEqs object
  ExteriorFuelEquipmentDefinition definition(model);
  ExteriorFuelEquipment fuelEq(definition);
  // This adds the alwaysOnDiscreteSchedule and its scheduletypelimit
  EXPECT_EQ(4u, model.numObjects());

  // Test definition
  EXPECT_DOUBLE_EQ(0.0, definition.designLevel());
  EXPECT_TRUE(definition.setDesignLevel(2303.3));
  EXPECT_DOUBLE_EQ(2303.3, definition.designLevel());

  ASSERT_EQ(1u, definition.instances().size());
  EXPECT_EQ(fuelEq.handle(), definition.instances()[0].handle());
  EXPECT_EQ(definition.handle(), fuelEq.definition().handle());

  // Test Equipment

  // Design level
  EXPECT_DOUBLE_EQ(1.0, fuelEq.multiplier());
  EXPECT_TRUE(fuelEq.isMultiplierDefaulted());
  EXPECT_TRUE(fuelEq.setMultiplier(5.1));
  EXPECT_DOUBLE_EQ(5.1, fuelEq.multiplier());
  EXPECT_FALSE(fuelEq.isMultiplierDefaulted());

  // Schedule
  Schedule alwaysOn = model.alwaysOnDiscreteSchedule();
  EXPECT_EQ(alwaysOn.handle(), fuelEq.schedule().handle());
  ScheduleConstant sch(model);
  EXPECT_TRUE(fuelEq.setSchedule(sch));
  EXPECT_EQ(sch.handle(), fuelEq.schedule().handle());
  fuelEq.resetSchedule();
  EXPECT_EQ(alwaysOn.handle(), fuelEq.schedule().handle());

  // Fuel Use Type
  EXPECT_EQ("Electricity", fuelEq.fuelType());
  EXPECT_TRUE(fuelEq.setFuelType("NaturalGas"));
  EXPECT_EQ("NaturalGas", fuelEq.fuelType());
  EXPECT_FALSE(fuelEq.setFuelType("Nonsense fuel type"));
  EXPECT_EQ("NaturalGas", fuelEq.fuelType());

  // Facility
  auto facility = model.getUniqueModelObject<Facility>();
  EXPECT_EQ(facility.handle(), fuelEq.facility().handle());
}

/* Checks that if the Equipment is removed, the Definition stays */
TEST_F(ModelFixture, ExteriorFuelEquipment_RemoveEquipment) {
  {
    Model model;

    ExteriorFuelEquipmentDefinition definition(model);
    ExteriorFuelEquipment fuelEq(definition);

    // This adds the alwaysOnDiscreteSchedule and its scheduletypelimit
    EXPECT_EQ(4u, model.numObjects());
    EXPECT_EQ(1u, model.getConcreteModelObjects<ExteriorFuelEquipment>().size());
    EXPECT_EQ(1u, model.getConcreteModelObjects<ExteriorFuelEquipmentDefinition>().size());

    std::vector<IdfObject> removed = fuelEq.remove();
    ASSERT_EQ(1u, removed.size());
    EXPECT_EQ(IddObjectType::OS_Exterior_FuelEquipment, removed[0].iddObject().type().value());
    EXPECT_EQ(0u, model.getConcreteModelObjects<ExteriorFuelEquipment>().size());
    EXPECT_EQ(1u, model.getConcreteModelObjects<ExteriorFuelEquipmentDefinition>().size());
    EXPECT_EQ(3u, model.numObjects());
  }

  {
    Model model;

    ExteriorFuelEquipmentDefinition definition(model);
    ExteriorFuelEquipment fuelEq1(definition);
    ExteriorFuelEquipment fuelEq2(definition);

    // This adds the alwaysOnDiscreteSchedule and its scheduletypelimit
    EXPECT_EQ(5u, model.numObjects());
    EXPECT_EQ(2u, model.getConcreteModelObjects<ExteriorFuelEquipment>().size());
    EXPECT_EQ(1u, model.getConcreteModelObjects<ExteriorFuelEquipmentDefinition>().size());

    std::vector<IdfObject> removed = fuelEq1.remove();
    ASSERT_EQ(1u, removed.size());
    EXPECT_EQ(IddObjectType::OS_Exterior_FuelEquipment, removed[0].iddObject().type().value());
    EXPECT_EQ(1u, model.getConcreteModelObjects<ExteriorFuelEquipment>().size());
    EXPECT_EQ(1u, model.getConcreteModelObjects<ExteriorFuelEquipmentDefinition>().size());
    EXPECT_EQ(4u, model.numObjects());
  }
}

/* Checks that if the Definition is removed, all equipment is removed too */
TEST_F(ModelFixture, ExteriorFuelEquipment_RemoveDefinition) {
  {
    Model model;

    ExteriorFuelEquipmentDefinition definition(model);
    ExteriorFuelEquipment fuelEq(definition);

    // This adds the alwaysOnDiscreteSchedule and its scheduletypelimit
    EXPECT_EQ(4u, model.numObjects());
    EXPECT_EQ(1u, model.getConcreteModelObjects<ExteriorFuelEquipment>().size());
    EXPECT_EQ(1u, model.getConcreteModelObjects<ExteriorFuelEquipmentDefinition>().size());

    std::vector<IdfObject> removed = definition.remove();
    ASSERT_EQ(2u, removed.size());
    EXPECT_EQ(IddObjectType::OS_Exterior_FuelEquipment_Definition, removed[0].iddObject().type().value());
    EXPECT_EQ(IddObjectType::OS_Exterior_FuelEquipment, removed[1].iddObject().type().value());
    EXPECT_EQ(0u, model.getConcreteModelObjects<ExteriorFuelEquipment>().size());
    EXPECT_EQ(0u, model.getConcreteModelObjects<ExteriorFuelEquipmentDefinition>().size());
    EXPECT_EQ(2u, model.numObjects());
  }

  {
    Model model;

    ExteriorFuelEquipmentDefinition definition(model);
    ExteriorFuelEquipment fuelEq1(definition);
    ExteriorFuelEquipment fuelEq2(definition);

    // This adds the alwaysOnDiscreteSchedule and its scheduletypelimit
    EXPECT_EQ(5u, model.numObjects());
    EXPECT_EQ(2u, model.getConcreteModelObjects<ExteriorFuelEquipment>().size());
    EXPECT_EQ(1u, model.getConcreteModelObjects<ExteriorFuelEquipmentDefinition>().size());
    std::vector<IdfObject> removed = definition.remove();
    ASSERT_EQ(3u, removed.size());
    EXPECT_EQ(IddObjectType::OS_Exterior_FuelEquipment_Definition, removed[0].iddObject().type().value());
    EXPECT_EQ(IddObjectType::OS_Exterior_FuelEquipment, removed[1].iddObject().type().value());
    EXPECT_EQ(IddObjectType::OS_Exterior_FuelEquipment, removed[2].iddObject().type().value());
    EXPECT_EQ(0u, model.getConcreteModelObjects<ExteriorFuelEquipment>().size());
    EXPECT_EQ(0u, model.getConcreteModelObjects<ExteriorFuelEquipmentDefinition>().size());
    EXPECT_EQ(2u, model.numObjects());
  }
}

/* If you clone the equipment, the definition comes with, and vice versa */
TEST_F(ModelFixture, ExteriorFuelEquipment_Clone) {
  Model model;

  // make a new fuelEqs object
  ExteriorFuelEquipmentDefinition definition(model);
  ExteriorFuelEquipment fuelEq(definition);

  // This adds the alwaysOnDiscreteSchedule and its scheduletypelimit
  EXPECT_EQ(4u, model.numObjects());

  // clone definition into new model
  {
    Model model2;
    definition.clone(model2);
    EXPECT_EQ(1u, model2.numObjects());
  }

  // clone fuelEq into new model
  {
    Model model2;
    fuelEq.clone(model2);
    EXPECT_EQ(4u, model2.numObjects());
  }
}

TEST_F(ModelFixture, ExteriorFuelEquipment_Costs) {
  Model model;

  // make a new fuelEqs object
  ExteriorFuelEquipmentDefinition definition(model);
  ExteriorFuelEquipment fuelEq(definition);

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
 *  // clone fuelEq into new model
 *  {
 *    Model model2;
 *    fuelEq.clone(model2);
 *    EXPECT_EQ(3u, model2.numObjects());
 *  }
 *
 *  // clone this into same model
 *  ModelObject fuelEq2 = fuelEq.clone(model);
 *  EXPECT_EQ(4u, model.numObjects());
 *  EXPECT_EQ(0u, fuelEq.lifeCycleCosts().size());
 *  EXPECT_EQ(0u, fuelEq2.lifeCycleCosts().size());
 *  EXPECT_EQ(1u, definition.lifeCycleCosts().size());
 *
 *  // remove one fuelEq
 *  std::vector<IdfObject> removed = fuelEq2.remove();
 *  EXPECT_EQ(1u, removed.size());
 *  EXPECT_EQ(3u, model.numObjects());
 *
 *  // remove definition
 *  removed = definition.remove();
 *  EXPECT_EQ(3u, removed.size());
 *  EXPECT_EQ(0u, model.numObjects());
 */
}
