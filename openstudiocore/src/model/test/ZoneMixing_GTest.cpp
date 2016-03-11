/**********************************************************************
 *  Copyright (c) 2008-2016, Alliance for Sustainable Energy.
 *  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 **********************************************************************/

#include <gtest/gtest.h>

#include "ModelFixture.hpp"

#include "../ZoneMixing.hpp"
#include "../ZoneMixing_Impl.hpp"
#include "../ThermalZone.hpp"
#include "../ScheduleRuleset.hpp"
#include "../ScheduleTypeLimits.hpp"
#include "../Model.hpp"
#include "../Model_Impl.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, ZoneMixing)
{
  Model model;
  EXPECT_NE(model.alwaysOnContinuousSchedule().handle(), model.alwaysOnDiscreteSchedule().handle());

  ThermalZone zone1(model);
  ThermalZone zone2(model);
  ZoneMixing mixing(zone1);
  EXPECT_EQ(zone1.handle(), mixing.zone().handle());
  EXPECT_EQ(model.alwaysOnContinuousSchedule().handle(), mixing.schedule().handle());
  EXPECT_FALSE(mixing.sourceZone());

  EXPECT_EQ(1u, zone1.zoneMixing().size());
  EXPECT_EQ(1u, zone1.supplyZoneMixing().size());
  EXPECT_EQ(0, zone1.exhaustZoneMixing().size());
  EXPECT_EQ(0, zone2.zoneMixing().size());
  EXPECT_EQ(0, zone2.supplyZoneMixing().size());
  EXPECT_EQ(0, zone2.exhaustZoneMixing().size());

  EXPECT_EQ("Flow/Zone", mixing.designFlowRateCalculationMethod());
  ASSERT_TRUE(mixing.designFlowRate());
  EXPECT_EQ(0.0, mixing.designFlowRate().get());
  EXPECT_FALSE(mixing.flowRateperZoneFloorArea());
  EXPECT_FALSE(mixing.flowRateperPerson());
  EXPECT_FALSE(mixing.airChangesperHour());

  EXPECT_TRUE(mixing.setDesignFlowRate(100.0));
  EXPECT_EQ("Flow/Zone", mixing.designFlowRateCalculationMethod());
  ASSERT_TRUE(mixing.designFlowRate());
  EXPECT_EQ(100.0, mixing.designFlowRate().get());
  EXPECT_FALSE(mixing.flowRateperZoneFloorArea());
  EXPECT_FALSE(mixing.flowRateperPerson());
  EXPECT_FALSE(mixing.airChangesperHour());

  EXPECT_TRUE(mixing.setFlowRateperZoneFloorArea(100.0));
  EXPECT_EQ("Flow/Area", mixing.designFlowRateCalculationMethod());
  EXPECT_FALSE(mixing.designFlowRate());
  ASSERT_TRUE(mixing.flowRateperZoneFloorArea());
  EXPECT_EQ(100.0, mixing.flowRateperZoneFloorArea().get());
  EXPECT_FALSE(mixing.flowRateperPerson());
  EXPECT_FALSE(mixing.airChangesperHour());

  EXPECT_TRUE(mixing.setFlowRateperPerson(100.0));
  EXPECT_EQ("Flow/Person", mixing.designFlowRateCalculationMethod());
  EXPECT_FALSE(mixing.designFlowRate());
  EXPECT_FALSE(mixing.flowRateperZoneFloorArea());
  ASSERT_TRUE(mixing.flowRateperPerson());
  EXPECT_EQ(100.0, mixing.flowRateperPerson().get());
  EXPECT_FALSE(mixing.airChangesperHour());

  EXPECT_TRUE(mixing.setAirChangesperHour(100.0));
  EXPECT_EQ("AirChanges/Hour", mixing.designFlowRateCalculationMethod());
  EXPECT_FALSE(mixing.designFlowRate());
  EXPECT_FALSE(mixing.flowRateperZoneFloorArea());
  EXPECT_FALSE(mixing.flowRateperPerson());
  ASSERT_TRUE(mixing.airChangesperHour());
  EXPECT_EQ(100.0, mixing.airChangesperHour().get());

  mixing.setSourceZone(zone2);
  ASSERT_TRUE(mixing.sourceZone());
  EXPECT_EQ(zone2.handle(), mixing.sourceZone().get().handle());

  EXPECT_EQ(1u, zone1.zoneMixing().size());
  EXPECT_EQ(1u, zone1.supplyZoneMixing().size());
  EXPECT_EQ(0, zone1.exhaustZoneMixing().size());
  EXPECT_EQ(1u, zone2.zoneMixing().size());
  EXPECT_EQ(0, zone2.supplyZoneMixing().size());
  EXPECT_EQ(1u, zone2.exhaustZoneMixing().size());

  EXPECT_FALSE(mixing.deltaTemperature());
  EXPECT_FALSE(mixing.deltaTemperatureSchedule());
  EXPECT_FALSE(mixing.minimumZoneTemperatureSchedule());
  EXPECT_FALSE(mixing.maximumZoneTemperatureSchedule());
  EXPECT_FALSE(mixing.minimumSourceZoneTemperatureSchedule());
  EXPECT_FALSE(mixing.maximumSourceZoneTemperatureSchedule());
  EXPECT_FALSE(mixing.minimumOutdoorTemperatureSchedule());
  EXPECT_FALSE(mixing.maximumOutdoorTemperatureSchedule());

  ScheduleRuleset fractionalSchedule(model);
  ScheduleTypeLimits fractionalLimits(model);
  EXPECT_TRUE(fractionalLimits.setUnitType("Dimensionless"));
  fractionalLimits.setLowerLimitValue(0.0); // needs to match schedule type registry
  fractionalLimits.setUpperLimitValue(1.0); // needs to match schedule type registry
  EXPECT_TRUE(fractionalSchedule.setScheduleTypeLimits(fractionalLimits));

  ScheduleRuleset temperatureSchedule(model);
  ScheduleTypeLimits temperatureLimits(model);
  EXPECT_TRUE(temperatureLimits.setUnitType("Temperature"));
  EXPECT_TRUE(temperatureSchedule.setScheduleTypeLimits(temperatureLimits));

  ScheduleRuleset deltaTemperatureSchedule(model);
  ScheduleTypeLimits deltaTemperatureLimits(model);
  EXPECT_TRUE(deltaTemperatureLimits.setUnitType("DeltaTemperature"));
  EXPECT_TRUE(deltaTemperatureSchedule.setScheduleTypeLimits(deltaTemperatureLimits));

  EXPECT_FALSE(mixing.setSchedule(temperatureSchedule));
  EXPECT_FALSE(mixing.setSchedule(deltaTemperatureSchedule));
  EXPECT_TRUE(mixing.setSchedule(fractionalSchedule));
  EXPECT_EQ(fractionalSchedule.handle(), mixing.schedule().handle());

  mixing.setDeltaTemperature(10.0);
  ASSERT_TRUE(mixing.deltaTemperature());
  EXPECT_EQ(10.0, mixing.deltaTemperature().get());
  EXPECT_FALSE(mixing.deltaTemperatureSchedule());

  EXPECT_FALSE(mixing.setDeltaTemperatureSchedule(fractionalSchedule));
  EXPECT_FALSE(mixing.setDeltaTemperatureSchedule(temperatureSchedule));
  EXPECT_TRUE(mixing.setDeltaTemperatureSchedule(deltaTemperatureSchedule));
  ASSERT_TRUE(mixing.deltaTemperatureSchedule());
  EXPECT_EQ(deltaTemperatureSchedule.handle(), mixing.deltaTemperatureSchedule().get().handle());

  mixing.setDeltaTemperature(-10.0);
  ASSERT_TRUE(mixing.deltaTemperature());
  EXPECT_EQ(-10.0, mixing.deltaTemperature().get());
  EXPECT_FALSE(mixing.deltaTemperatureSchedule());

  EXPECT_FALSE(mixing.maximumZoneTemperatureSchedule());
  EXPECT_FALSE(mixing.setMaximumZoneTemperatureSchedule(fractionalSchedule));
  EXPECT_FALSE(mixing.setMaximumZoneTemperatureSchedule(deltaTemperatureSchedule));
  EXPECT_TRUE(mixing.setMaximumZoneTemperatureSchedule(temperatureSchedule));
  ASSERT_TRUE(mixing.maximumZoneTemperatureSchedule());
  EXPECT_EQ(temperatureSchedule.handle(), mixing.maximumZoneTemperatureSchedule().get().handle());
}

TEST_F(ModelFixture, ZoneMixing_SameZone)
{
  {
    Model model;
    ThermalZone zone1(model);
    ThermalZone zone2(model);
    ZoneMixing mixing(zone1);
    EXPECT_TRUE(mixing.setSourceZone(zone2));
  }
  {
    Model model;
    ThermalZone zone1(model);
    ZoneMixing mixing(zone1);
    EXPECT_FALSE(mixing.setSourceZone(zone1));
  }
}

TEST_F(ModelFixture, ZoneMixing_ZoneRemove)
{
  {
    Model model;
    ThermalZone zone1(model);
    ThermalZone zone2(model);
    ZoneMixing mixing(zone1);
    mixing.setSourceZone(zone2);

    EXPECT_EQ(1u, model.getModelObjects<ZoneMixing>().size());
    zone1.remove();
    EXPECT_EQ(0u, model.getModelObjects<ZoneMixing>().size());
  }
  {
    Model model;
    ThermalZone zone1(model);
    ThermalZone zone2(model);
    ZoneMixing mixing(zone1);
    mixing.setSourceZone(zone2);

    EXPECT_EQ(1u, model.getModelObjects<ZoneMixing>().size());
    zone2.remove();
    EXPECT_EQ(0u, model.getModelObjects<ZoneMixing>().size());
  }
}