/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2019, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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


TEST_F(ModelFixture, ExteriorLights_ExteriorLights)
{
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  ASSERT_EXIT (
  {
    Model model;
    // make a new lightss object
    ExteriorLightsDefinition definition(model);
    ExteriorLights lights(definition);
    EXPECT_EQ(2u, model.numObjects());

    EXPECT_EQ(definition.handle(), lights.exteriorLightsDefinition().handle());

    exit(0);
  } ,
    ::testing::ExitedWithCode(0), "" );
}

// Test the various setters and getters
TEST_F(ModelFixture, ExteriorLights_SettersGetters)
{

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
  Facility facility = model.getUniqueModelObject<Facility>();
  EXPECT_EQ(facility.handle(), lights.facility().handle());
}


/* Checks that if the Equipment is removed, the Definition stays */
TEST_F(ModelFixture, ExteriorLights_RemoveEquipment)
{
  {
    Model model;

    ExteriorLightsDefinition definition(model);
    ExteriorLights lights(definition);

    // This adds the alwaysOnDiscreteSchedule and its scheduletypelimit
    EXPECT_EQ(4u, model.numObjects());
    EXPECT_EQ(1u, model.getModelObjects<ExteriorLights>().size());
    EXPECT_EQ(1u, model.getModelObjects<ExteriorLightsDefinition>().size());

    std::vector<IdfObject> removed = lights.remove();
    ASSERT_EQ(1u, removed.size());
    EXPECT_EQ(IddObjectType::OS_Exterior_Lights, removed[0].iddObject().type().value());
    EXPECT_EQ(0u, model.getModelObjects<ExteriorLights>().size());
    EXPECT_EQ(1u, model.getModelObjects<ExteriorLightsDefinition>().size());
    EXPECT_EQ(3u, model.numObjects());

  }

  {
    Model model;

    ExteriorLightsDefinition definition(model);
    ExteriorLights lights1(definition);
    ExteriorLights lights2(definition);

    // This adds the alwaysOnDiscreteSchedule and its scheduletypelimit
    EXPECT_EQ(5u, model.numObjects());
    EXPECT_EQ(2u, model.getModelObjects<ExteriorLights>().size());
    EXPECT_EQ(1u, model.getModelObjects<ExteriorLightsDefinition>().size());

    std::vector<IdfObject> removed = lights1.remove();
    ASSERT_EQ(1u, removed.size());
    EXPECT_EQ(IddObjectType::OS_Exterior_Lights, removed[0].iddObject().type().value());
    EXPECT_EQ(1u, model.getModelObjects<ExteriorLights>().size());
    EXPECT_EQ(1u, model.getModelObjects<ExteriorLightsDefinition>().size());
    EXPECT_EQ(4u, model.numObjects());
  }
}

/* Checks that if the Definition is removed, all equipment is removed too */
TEST_F(ModelFixture, ExteriorLights_RemoveDefinition)
{
  {
    Model model;

    ExteriorLightsDefinition definition(model);
    ExteriorLights lights(definition);

    // This adds the alwaysOnDiscreteSchedule and its scheduletypelimit
    EXPECT_EQ(4u, model.numObjects());
    EXPECT_EQ(1u, model.getModelObjects<ExteriorLights>().size());
    EXPECT_EQ(1u, model.getModelObjects<ExteriorLightsDefinition>().size());

    std::vector<IdfObject> removed = definition.remove();
    ASSERT_EQ(2u, removed.size());
    EXPECT_EQ(IddObjectType::OS_Exterior_Lights_Definition, removed[0].iddObject().type().value());
    EXPECT_EQ(IddObjectType::OS_Exterior_Lights, removed[1].iddObject().type().value());
    EXPECT_EQ(0u, model.getModelObjects<ExteriorLights>().size());
    EXPECT_EQ(0u, model.getModelObjects<ExteriorLightsDefinition>().size());
    EXPECT_EQ(2u, model.numObjects());
  }

  {
    Model model;

    ExteriorLightsDefinition definition(model);
    ExteriorLights lights1(definition);
    ExteriorLights lights2(definition);

    // This adds the alwaysOnDiscreteSchedule and its scheduletypelimit
    EXPECT_EQ(5u, model.numObjects());
    EXPECT_EQ(2u, model.getModelObjects<ExteriorLights>().size());
    EXPECT_EQ(1u, model.getModelObjects<ExteriorLightsDefinition>().size());
    std::vector<IdfObject> removed = definition.remove();
    ASSERT_EQ(3u, removed.size());
    EXPECT_EQ(IddObjectType::OS_Exterior_Lights_Definition, removed[0].iddObject().type().value());
    EXPECT_EQ(IddObjectType::OS_Exterior_Lights, removed[1].iddObject().type().value());
    EXPECT_EQ(IddObjectType::OS_Exterior_Lights, removed[2].iddObject().type().value());
    EXPECT_EQ(0u, model.getModelObjects<ExteriorLights>().size());
    EXPECT_EQ(0u, model.getModelObjects<ExteriorLightsDefinition>().size());
    EXPECT_EQ(2u, model.numObjects());
  }
}

/* If you clone the equipment, the definition comes with, and vice versa */
TEST_F(ModelFixture, ExteriorLights_Clone)
{
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

TEST_F(ModelFixture, ExteriorLights_Costs)
{
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
