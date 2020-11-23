/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2020, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
*  following conditions are met:
*
*  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
*  disclaimer.
*
*  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
*  disclaimer in the documentation and/or other materials provided with the distribution.
*
*  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote products
*  derived from this software without specific prior written permission from the respective party.
*
*  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative works
*  may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without specific prior
*  written permission from Alliance for Sustainable Energy, LLC.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER(S) AND ANY CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
*  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
*  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER(S), ANY CONTRIBUTORS, THE UNITED STATES GOVERNMENT, OR THE UNITED
*  STATES DEPARTMENT OF ENERGY, NOR ANY OF THEIR EMPLOYEES, BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
*  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
*  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
*  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
*  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
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
  Facility facility = model.getUniqueModelObject<Facility>();
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
    EXPECT_EQ(1u, model.getModelObjects<ExteriorFuelEquipment>().size());
    EXPECT_EQ(1u, model.getModelObjects<ExteriorFuelEquipmentDefinition>().size());

    std::vector<IdfObject> removed = fuelEq.remove();
    ASSERT_EQ(1u, removed.size());
    EXPECT_EQ(IddObjectType::OS_Exterior_FuelEquipment, removed[0].iddObject().type().value());
    EXPECT_EQ(0u, model.getModelObjects<ExteriorFuelEquipment>().size());
    EXPECT_EQ(1u, model.getModelObjects<ExteriorFuelEquipmentDefinition>().size());
    EXPECT_EQ(3u, model.numObjects());
  }

  {
    Model model;

    ExteriorFuelEquipmentDefinition definition(model);
    ExteriorFuelEquipment fuelEq1(definition);
    ExteriorFuelEquipment fuelEq2(definition);

    // This adds the alwaysOnDiscreteSchedule and its scheduletypelimit
    EXPECT_EQ(5u, model.numObjects());
    EXPECT_EQ(2u, model.getModelObjects<ExteriorFuelEquipment>().size());
    EXPECT_EQ(1u, model.getModelObjects<ExteriorFuelEquipmentDefinition>().size());

    std::vector<IdfObject> removed = fuelEq1.remove();
    ASSERT_EQ(1u, removed.size());
    EXPECT_EQ(IddObjectType::OS_Exterior_FuelEquipment, removed[0].iddObject().type().value());
    EXPECT_EQ(1u, model.getModelObjects<ExteriorFuelEquipment>().size());
    EXPECT_EQ(1u, model.getModelObjects<ExteriorFuelEquipmentDefinition>().size());
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
    EXPECT_EQ(1u, model.getModelObjects<ExteriorFuelEquipment>().size());
    EXPECT_EQ(1u, model.getModelObjects<ExteriorFuelEquipmentDefinition>().size());

    std::vector<IdfObject> removed = definition.remove();
    ASSERT_EQ(2u, removed.size());
    EXPECT_EQ(IddObjectType::OS_Exterior_FuelEquipment_Definition, removed[0].iddObject().type().value());
    EXPECT_EQ(IddObjectType::OS_Exterior_FuelEquipment, removed[1].iddObject().type().value());
    EXPECT_EQ(0u, model.getModelObjects<ExteriorFuelEquipment>().size());
    EXPECT_EQ(0u, model.getModelObjects<ExteriorFuelEquipmentDefinition>().size());
    EXPECT_EQ(2u, model.numObjects());
  }

  {
    Model model;

    ExteriorFuelEquipmentDefinition definition(model);
    ExteriorFuelEquipment fuelEq1(definition);
    ExteriorFuelEquipment fuelEq2(definition);

    // This adds the alwaysOnDiscreteSchedule and its scheduletypelimit
    EXPECT_EQ(5u, model.numObjects());
    EXPECT_EQ(2u, model.getModelObjects<ExteriorFuelEquipment>().size());
    EXPECT_EQ(1u, model.getModelObjects<ExteriorFuelEquipmentDefinition>().size());
    std::vector<IdfObject> removed = definition.remove();
    ASSERT_EQ(3u, removed.size());
    EXPECT_EQ(IddObjectType::OS_Exterior_FuelEquipment_Definition, removed[0].iddObject().type().value());
    EXPECT_EQ(IddObjectType::OS_Exterior_FuelEquipment, removed[1].iddObject().type().value());
    EXPECT_EQ(IddObjectType::OS_Exterior_FuelEquipment, removed[2].iddObject().type().value());
    EXPECT_EQ(0u, model.getModelObjects<ExteriorFuelEquipment>().size());
    EXPECT_EQ(0u, model.getModelObjects<ExteriorFuelEquipmentDefinition>().size());
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
