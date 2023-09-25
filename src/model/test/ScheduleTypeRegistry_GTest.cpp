/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>

#include "ModelFixture.hpp"
#include "../ScheduleTypeRegistry.hpp"
#include "../Model.hpp"
#include "../ScheduleConstant.hpp"
#include "../ScheduleTypeLimits.hpp"
#include "../ScheduleTypeLimits_Impl.hpp"
#include "../LightsDefinition.hpp"
#include "../Lights.hpp"

#include "../../utilities/core/Containers.hpp"

#include <sstream>

using namespace openstudio::model;
using namespace openstudio;

TEST_F(ModelFixture, ScheduleTypeRegistry_ClassNames) {
  StringVector classesWithSchedules = ScheduleTypeRegistry::instance().classNames();
  EXPECT_FALSE(classesWithSchedules.empty());
  std::stringstream ss;
  for (const std::string& className : classesWithSchedules) {
    EXPECT_FALSE(ScheduleTypeRegistry::instance().getScheduleTypesByClassName(className).empty());
    ss << "  " << className << '\n';
  }
  LOG(Trace, "The following ModelObject classes reference at least one schedule:" << '\n' << ss.str());
}

TEST_F(ModelFixture, ScheduleTypeRegistry_UseInSetSchedule) {
  Model model;

  // create a schedule
  ScheduleConstant schedule(model);

  // set a minimal schedule type limits
  ScheduleTypeLimits anyNumber(model);
  anyNumber.setName("Any Number");
  bool ok = schedule.setScheduleTypeLimits(anyNumber);
  EXPECT_TRUE(ok);

  // try to set Lights schedule--should fail b/c needs between 0 and 1
  LightsDefinition definition(model);
  definition.setLightingLevel(100.0);
  Lights light(definition);
  ok = light.setSchedule(schedule);
  EXPECT_FALSE(ok);

  // set Availablity schedule type limits
  ScheduleType scheduleType = ScheduleTypeRegistry::instance().getScheduleType("CoilCoolingDXSingleSpeed", "Availability");
  ScheduleTypeLimits availabilityLimits = ScheduleTypeRegistry::instance().getOrCreateScheduleTypeLimits(scheduleType, model);
  ok = schedule.setScheduleTypeLimits(availabilityLimits);
  EXPECT_TRUE(ok);

  // try to set Lights schedule--used to fail b/c lights schedule is Dimensionless, not Availability, now passes
  ok = light.setSchedule(schedule);
  EXPECT_TRUE(ok);

  // clear conflicting information
  availabilityLimits.resetNumericType();
  availabilityLimits.resetUnitType();

  // set Lights schedule--should succeed b/c not in conflict
  ok = light.setSchedule(schedule);
  EXPECT_TRUE(ok);

  // try to clear schedule type limits--should fail b/c in use
  ok = schedule.resetScheduleTypeLimits();
  EXPECT_FALSE(ok);

  // reset Lights schedule
  light.resetSchedule();

  // try to clear schedule type limits--should succeed
  ok = schedule.resetScheduleTypeLimits();
  EXPECT_TRUE(ok);
  EXPECT_FALSE(schedule.scheduleTypeLimits());

  // set Lights schedule--should succeed and assign an appropriate ScheduleTypeLimits
  ok = light.setSchedule(schedule);
  EXPECT_TRUE(ok);
  ASSERT_TRUE(schedule.scheduleTypeLimits());
  ScheduleTypeLimits lightsLimits = schedule.scheduleTypeLimits().get();
  EXPECT_EQ("Fractional", lightsLimits.name().get());
  ASSERT_TRUE(lightsLimits.numericType());
  EXPECT_EQ("Continuous", lightsLimits.numericType().get());
  EXPECT_EQ("Dimensionless", lightsLimits.unitType());
  EXPECT_TRUE(lightsLimits.isUnitTypeDefaulted());
  ASSERT_TRUE(lightsLimits.lowerLimitValue());
  EXPECT_DOUBLE_EQ(0.0, lightsLimits.lowerLimitValue().get());
  ASSERT_TRUE(lightsLimits.upperLimitValue());
  EXPECT_DOUBLE_EQ(1.0, lightsLimits.upperLimitValue().get());
}

TEST_F(ModelFixture, ScheduleTypeRegistry_GetOrCreateScheduleTypeLimits) {
  {
    Model model;

    ScheduleType scheduleType = ScheduleTypeRegistry::instance().getScheduleType("People", "Activity Level");

    ASSERT_TRUE(scheduleType.lowerLimitValue);
    EXPECT_EQ(0.0, scheduleType.lowerLimitValue.get());
    EXPECT_FALSE(scheduleType.upperLimitValue);

    ScheduleTypeLimits limits = ScheduleTypeRegistry::instance().getOrCreateScheduleTypeLimits(scheduleType, model);
    ScheduleTypeLimits limits2 = ScheduleTypeRegistry::instance().getOrCreateScheduleTypeLimits(scheduleType, model);

    EXPECT_EQ(limits.handle(), limits2.handle());

    EXPECT_TRUE(isCompatible(scheduleType, limits));
  }

  {
    Model model;

    ScheduleType scheduleType = ScheduleTypeRegistry::instance().getScheduleType("RefrigerationCase", "Refrigerated Case Restocking");

    ASSERT_TRUE(scheduleType.lowerLimitValue);
    EXPECT_EQ(0.0, scheduleType.lowerLimitValue.get());
    EXPECT_FALSE(scheduleType.upperLimitValue);

    ScheduleTypeLimits limits = ScheduleTypeRegistry::instance().getOrCreateScheduleTypeLimits(scheduleType, model);
    ScheduleTypeLimits limits2 = ScheduleTypeRegistry::instance().getOrCreateScheduleTypeLimits(scheduleType, model);

    EXPECT_EQ(limits.handle(), limits2.handle());

    EXPECT_TRUE(isCompatible(scheduleType, limits));
  }

  {
    Model model;

    ScheduleType scheduleType = ScheduleTypeRegistry::instance().getScheduleType("ThermostatSetpointDualSetpoint", "Heating Setpoint Temperature");

    EXPECT_FALSE(scheduleType.lowerLimitValue);
    EXPECT_FALSE(scheduleType.upperLimitValue);

    ScheduleTypeLimits limits = ScheduleTypeRegistry::instance().getOrCreateScheduleTypeLimits(scheduleType, model);
    ScheduleTypeLimits limits2 = ScheduleTypeRegistry::instance().getOrCreateScheduleTypeLimits(scheduleType, model);

    EXPECT_EQ(limits.handle(), limits2.handle());
    EXPECT_EQ("Temperature", limits.name().get());

    EXPECT_TRUE(isCompatible(scheduleType, limits));

    ASSERT_EQ(1u, model.getConcreteModelObjectsByName<ScheduleTypeLimits>("Temperature").size());
    EXPECT_EQ("Temperature", model.getConcreteModelObjectsByName<ScheduleTypeLimits>("Temperature")[0].name().get());

    ModelObject scheduleType2 = limits.clone();
    EXPECT_EQ("Temperature 1", scheduleType2.name().get());

    EXPECT_EQ(2u, model.getConcreteModelObjectsByName<ScheduleTypeLimits>("Temperature").size());

    limits.remove();

    ASSERT_EQ(1u, model.getConcreteModelObjectsByName<ScheduleTypeLimits>("Temperature").size());
    EXPECT_EQ("Temperature 1", model.getConcreteModelObjectsByName<ScheduleTypeLimits>("Temperature 1")[0].name().get());

    limits = ScheduleTypeRegistry::instance().getOrCreateScheduleTypeLimits(scheduleType, model);
    EXPECT_EQ(limits.handle(), scheduleType2.handle());
    EXPECT_EQ("Temperature 1", limits.name().get());
  }

  {
    Model model;
    for (const auto& className : ScheduleTypeRegistry::instance().classNames()) {
      for (const auto& scheduleType : ScheduleTypeRegistry::instance().getScheduleTypesByClassName(className)) {
        ScheduleTypeLimits limits = ScheduleTypeRegistry::instance().getOrCreateScheduleTypeLimits(scheduleType, model);
        ScheduleTypeLimits limits2 = ScheduleTypeRegistry::instance().getOrCreateScheduleTypeLimits(scheduleType, model);

        EXPECT_EQ(limits.handle(), limits2.handle());

        EXPECT_EQ(ScheduleTypeLimits::units(scheduleType.unitType, false).get(), ScheduleTypeLimits::units(limits.unitType(), false).get());

        EXPECT_TRUE(isCompatible(scheduleType, limits));
      }
    }
  }
}
