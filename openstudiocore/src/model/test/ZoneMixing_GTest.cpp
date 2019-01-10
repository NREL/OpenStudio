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