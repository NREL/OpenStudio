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
  Facility facility = model.getUniqueModelObject<Facility>();
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
    EXPECT_EQ(1u, model.getModelObjects<ExteriorWaterEquipment>().size());
    EXPECT_EQ(1u, model.getModelObjects<ExteriorWaterEquipmentDefinition>().size());

    std::vector<IdfObject> removed = waterEq.remove();
    ASSERT_EQ(1u, removed.size());
    EXPECT_EQ(IddObjectType::OS_Exterior_WaterEquipment, removed[0].iddObject().type().value());
    EXPECT_EQ(0u, model.getModelObjects<ExteriorWaterEquipment>().size());
    EXPECT_EQ(1u, model.getModelObjects<ExteriorWaterEquipmentDefinition>().size());
    EXPECT_EQ(3u, model.numObjects());
  }

  {
    Model model;

    ExteriorWaterEquipmentDefinition definition(model);
    ExteriorWaterEquipment waterEq1(definition);
    ExteriorWaterEquipment waterEq2(definition);

    // This adds the alwaysOnDiscreteSchedule and its scheduletypelimit
    EXPECT_EQ(5u, model.numObjects());
    EXPECT_EQ(2u, model.getModelObjects<ExteriorWaterEquipment>().size());
    EXPECT_EQ(1u, model.getModelObjects<ExteriorWaterEquipmentDefinition>().size());

    std::vector<IdfObject> removed = waterEq1.remove();
    ASSERT_EQ(1u, removed.size());
    EXPECT_EQ(IddObjectType::OS_Exterior_WaterEquipment, removed[0].iddObject().type().value());
    EXPECT_EQ(1u, model.getModelObjects<ExteriorWaterEquipment>().size());
    EXPECT_EQ(1u, model.getModelObjects<ExteriorWaterEquipmentDefinition>().size());
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
    EXPECT_EQ(1u, model.getModelObjects<ExteriorWaterEquipment>().size());
    EXPECT_EQ(1u, model.getModelObjects<ExteriorWaterEquipmentDefinition>().size());

    std::vector<IdfObject> removed = definition.remove();
    ASSERT_EQ(2u, removed.size());
    EXPECT_EQ(IddObjectType::OS_Exterior_WaterEquipment_Definition, removed[0].iddObject().type().value());
    EXPECT_EQ(IddObjectType::OS_Exterior_WaterEquipment, removed[1].iddObject().type().value());
    EXPECT_EQ(0u, model.getModelObjects<ExteriorWaterEquipment>().size());
    EXPECT_EQ(0u, model.getModelObjects<ExteriorWaterEquipmentDefinition>().size());
    EXPECT_EQ(2u, model.numObjects());
  }

  {
    Model model;

    ExteriorWaterEquipmentDefinition definition(model);
    ExteriorWaterEquipment waterEq1(definition);
    ExteriorWaterEquipment waterEq2(definition);

    // This adds the alwaysOnDiscreteSchedule and its scheduletypelimit
    EXPECT_EQ(5u, model.numObjects());
    EXPECT_EQ(2u, model.getModelObjects<ExteriorWaterEquipment>().size());
    EXPECT_EQ(1u, model.getModelObjects<ExteriorWaterEquipmentDefinition>().size());
    std::vector<IdfObject> removed = definition.remove();
    ASSERT_EQ(3u, removed.size());
    EXPECT_EQ(IddObjectType::OS_Exterior_WaterEquipment_Definition, removed[0].iddObject().type().value());
    EXPECT_EQ(IddObjectType::OS_Exterior_WaterEquipment, removed[1].iddObject().type().value());
    EXPECT_EQ(IddObjectType::OS_Exterior_WaterEquipment, removed[2].iddObject().type().value());
    EXPECT_EQ(0u, model.getModelObjects<ExteriorWaterEquipment>().size());
    EXPECT_EQ(0u, model.getModelObjects<ExteriorWaterEquipmentDefinition>().size());
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
