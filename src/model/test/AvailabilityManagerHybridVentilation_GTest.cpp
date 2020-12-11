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

#include "../AvailabilityManagerHybridVentilation.hpp"
#include "../AvailabilityManagerHybridVentilation_Impl.hpp"
#include "../AirLoopHVAC.hpp"
#include "../AirTerminalSingleDuctConstantVolumeNoReheat.hpp"
#include "../ThermalZone.hpp"
#include "../ScheduleConstant.hpp"
#include "../Schedule.hpp"
#include "../CurveLinear.hpp"
#include "../Curve.hpp"
#include "../ZoneVentilationDesignFlowRate.hpp"
#include "../ZoneVentilationWindandStackOpenArea.hpp"
#include "../ZoneMixing.hpp"
#include "../Model.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, AvailabilityManagerHybridVentilation) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  ASSERT_EXIT(
    {
      Model m;
      AvailabilityManagerHybridVentilation avm(m);

      exit(0);
    },
    ::testing::ExitedWithCode(0), "");
}

TEST_F(ModelFixture, AvailabilityManagerHybridVentilation_GettersSetters) {
  Model m;

  AvailabilityManagerHybridVentilation avm(m);
  avm.setName("My AvailabilityManagerHybridVentilation");
  EXPECT_FALSE(avm.loop());

  AirLoopHVAC a(m);
  a.addAvailabilityManager(avm);
  ASSERT_TRUE(avm.loop());
  EXPECT_EQ(a, avm.loop().get());

  // Controlled Zone: Optional Object
  ThermalZone z(m);
  Schedule alwaysOn = m.alwaysOnDiscreteSchedule();

  AirTerminalSingleDuctConstantVolumeNoReheat atu(m, alwaysOn);
  EXPECT_TRUE(a.addBranchForZone(z, atu));

  EXPECT_FALSE(avm.controlledZone());
  EXPECT_TRUE(avm.setControlledZone(z));
  ASSERT_TRUE(avm.controlledZone());
  EXPECT_EQ(z, avm.controlledZone().get());
  avm.resetControlledZone();
  EXPECT_FALSE(avm.controlledZone());

  // Ventilation Control Mode Schedule: Required Object
  ScheduleConstant ventilation_control_sch(m);
  EXPECT_TRUE(avm.setVentilationControlModeSchedule(ventilation_control_sch));
  EXPECT_EQ(ventilation_control_sch, avm.ventilationControlModeSchedule());

  // Use Weather File Rain Indicators: Required Boolean
  EXPECT_TRUE(avm.setUseWeatherFileRainIndicators(true));
  EXPECT_TRUE(avm.useWeatherFileRainIndicators());
  EXPECT_TRUE(avm.setUseWeatherFileRainIndicators(false));
  EXPECT_FALSE(avm.useWeatherFileRainIndicators());

  // Maximum Wind Speed: Required Double
  EXPECT_TRUE(avm.setMaximumWindSpeed(20.0));
  EXPECT_EQ(20.0, avm.maximumWindSpeed());
  // Bad Value
  EXPECT_FALSE(avm.setMaximumWindSpeed(-10.0));
  EXPECT_EQ(20.0, avm.maximumWindSpeed());

  // Minimum Outdoor Temperature: Required Double
  EXPECT_TRUE(avm.setMinimumOutdoorTemperature(0.0));
  EXPECT_EQ(0.0, avm.minimumOutdoorTemperature());
  // Bad Value
  EXPECT_FALSE(avm.setMinimumOutdoorTemperature(-110.0));
  EXPECT_EQ(0.0, avm.minimumOutdoorTemperature());

  // Maximum Outdoor Temperature: Required Double
  EXPECT_TRUE(avm.setMaximumOutdoorTemperature(0.0));
  EXPECT_EQ(0.0, avm.maximumOutdoorTemperature());
  // Bad Value
  EXPECT_FALSE(avm.setMaximumOutdoorTemperature(-110.0));
  EXPECT_EQ(0.0, avm.maximumOutdoorTemperature());

  // Minimum Outdoor Enthalpy: Required Double
  EXPECT_TRUE(avm.setMinimumOutdoorEnthalpy(150000.0));
  EXPECT_EQ(150000.0, avm.minimumOutdoorEnthalpy());
  // Bad Value
  EXPECT_FALSE(avm.setMinimumOutdoorEnthalpy(-10.0));
  EXPECT_EQ(150000.0, avm.minimumOutdoorEnthalpy());

  // Maximum Outdoor Enthalpy: Required Double
  EXPECT_TRUE(avm.setMaximumOutdoorEnthalpy(150000.0));
  EXPECT_EQ(150000.0, avm.maximumOutdoorEnthalpy());
  // Bad Value
  EXPECT_FALSE(avm.setMaximumOutdoorEnthalpy(-10.0));
  EXPECT_EQ(150000.0, avm.maximumOutdoorEnthalpy());

  // Minimum Outdoor Dewpoint: Required Double
  EXPECT_TRUE(avm.setMinimumOutdoorDewpoint(0.0));
  EXPECT_EQ(0.0, avm.minimumOutdoorDewpoint());
  // Bad Value
  EXPECT_FALSE(avm.setMinimumOutdoorDewpoint(-110.0));
  EXPECT_EQ(0.0, avm.minimumOutdoorDewpoint());

  // Maximum Outdoor Dewpoint: Required Double
  EXPECT_TRUE(avm.setMaximumOutdoorDewpoint(0.0));
  EXPECT_EQ(0.0, avm.maximumOutdoorDewpoint());
  // Bad Value
  EXPECT_FALSE(avm.setMaximumOutdoorDewpoint(-110.0));
  EXPECT_EQ(0.0, avm.maximumOutdoorDewpoint());

  // Minimum Outdoor Ventilation Air Schedule: Optional Object but set in Ctor and non optional
  ScheduleConstant min_oa_sch(m);
  EXPECT_TRUE(avm.setMinimumOutdoorVentilationAirSchedule(min_oa_sch));
  EXPECT_EQ(min_oa_sch, avm.minimumOutdoorVentilationAirSchedule());

  // Opening Factor Function of Wind Speed Curve: Optional Object
  CurveLinear opening_factor_curve(m);
  EXPECT_FALSE(avm.openingFactorFunctionofWindSpeedCurve());
  EXPECT_TRUE(avm.setOpeningFactorFunctionofWindSpeedCurve(opening_factor_curve));
  ASSERT_TRUE(avm.openingFactorFunctionofWindSpeedCurve());
  EXPECT_EQ(opening_factor_curve, avm.openingFactorFunctionofWindSpeedCurve().get());
  avm.resetOpeningFactorFunctionofWindSpeedCurve();
  EXPECT_FALSE(avm.openingFactorFunctionofWindSpeedCurve());

  // AirflowNetwork Control Type Schedule: Optional Object
  ScheduleConstant afn_control_sch(m);
  EXPECT_FALSE(avm.airflowNetworkControlTypeSchedule());
  EXPECT_TRUE(avm.setAirflowNetworkControlTypeSchedule(afn_control_sch));
  ASSERT_TRUE(avm.airflowNetworkControlTypeSchedule());
  EXPECT_EQ(afn_control_sch, avm.airflowNetworkControlTypeSchedule().get());
  avm.resetAirflowNetworkControlTypeSchedule();
  EXPECT_FALSE(avm.airflowNetworkControlTypeSchedule());

  // Simple Airflow Control Type Schedule: Optional Object
  ScheduleConstant simple_control_sch(m);
  EXPECT_FALSE(avm.simpleAirflowControlTypeSchedule());
  EXPECT_TRUE(avm.setSimpleAirflowControlTypeSchedule(simple_control_sch));
  ASSERT_TRUE(avm.simpleAirflowControlTypeSchedule());
  EXPECT_EQ(simple_control_sch, avm.simpleAirflowControlTypeSchedule().get());
  avm.resetSimpleAirflowControlTypeSchedule();
  EXPECT_FALSE(avm.simpleAirflowControlTypeSchedule());

  // ZoneVentilation Object: Optional Object
  EXPECT_FALSE(avm.zoneVentilationObject());
  ZoneVentilationDesignFlowRate zvd(m);
  EXPECT_TRUE(avm.setZoneVentilationObject(zvd));
  ASSERT_TRUE(avm.zoneVentilationObject());
  EXPECT_EQ(zvd, avm.zoneVentilationObject().get());
  ZoneVentilationWindandStackOpenArea zvw(m);
  EXPECT_TRUE(avm.setZoneVentilationObject(zvw));
  ASSERT_TRUE(avm.zoneVentilationObject());
  EXPECT_EQ(zvw, avm.zoneVentilationObject().get());
  // Bad type
  ZoneMixing zm(z);
  EXPECT_FALSE(avm.setZoneVentilationObject(zm));
  ASSERT_TRUE(avm.zoneVentilationObject());
  EXPECT_EQ(zvw, avm.zoneVentilationObject().get());
  avm.resetZoneVentilationObject();
  EXPECT_FALSE(avm.zoneVentilationObject());

  // Minimum HVAC Operation Time: Required Double
  EXPECT_TRUE(avm.setMinimumHVACOperationTime(0.1));
  EXPECT_EQ(0.1, avm.minimumHVACOperationTime());
  // Bad Value
  EXPECT_FALSE(avm.setMinimumHVACOperationTime(-10.0));
  EXPECT_EQ(0.1, avm.minimumHVACOperationTime());

  // Minimum Ventilation Time: Required Double
  EXPECT_TRUE(avm.setMinimumVentilationTime(0.2));
  EXPECT_EQ(0.2, avm.minimumVentilationTime());
  // Bad Value
  EXPECT_FALSE(avm.setMinimumVentilationTime(-10.0));
  EXPECT_EQ(0.2, avm.minimumVentilationTime());
}

TEST_F(ModelFixture, AvailabilityManagerHybridVentilation_Clone) {

  Model m;

  AvailabilityManagerHybridVentilation avm(m);
  avm.setName("My AvailabilityManagerHybridVentilation");

  AirLoopHVAC a(m);
  a.addAvailabilityManager(avm);
  ASSERT_TRUE(avm.loop());
  EXPECT_EQ(a, avm.loop().get());

  // Controlled Zone: Optional Object
  ThermalZone z(m);
  z.setName("Zone 1");

  Schedule alwaysOn = m.alwaysOnDiscreteSchedule();
  AirTerminalSingleDuctConstantVolumeNoReheat atu(m, alwaysOn);
  EXPECT_TRUE(a.addBranchForZone(z, atu));

  EXPECT_TRUE(avm.setControlledZone(z));
  ASSERT_TRUE(avm.controlledZone());
  EXPECT_EQ(z, avm.controlledZone().get());

  // Ventilation Control Mode Schedule: Required Object
  ScheduleConstant ventilation_control_sch(m);
  ventilation_control_sch.setName("Ventilation Control Mode Schedule");
  EXPECT_TRUE(avm.setVentilationControlModeSchedule(ventilation_control_sch));
  EXPECT_EQ(ventilation_control_sch, avm.ventilationControlModeSchedule());

  // Use Weather File Rain Indicators: Required Boolean
  EXPECT_TRUE(avm.setUseWeatherFileRainIndicators(true));
  EXPECT_TRUE(avm.useWeatherFileRainIndicators());

  // Maximum Wind Speed: Required Double
  EXPECT_TRUE(avm.setMaximumWindSpeed(40.0));
  EXPECT_EQ(40.0, avm.maximumWindSpeed());

  // Minimum Outdoor Temperature: Required Double
  EXPECT_TRUE(avm.setMinimumOutdoorTemperature(15));
  EXPECT_EQ(15, avm.minimumOutdoorTemperature());

  // Maximum Outdoor Temperature: Required Double
  EXPECT_TRUE(avm.setMaximumOutdoorTemperature(35.0));
  EXPECT_EQ(35.0, avm.maximumOutdoorTemperature());

  // Minimum Outdoor Enthalpy: Required Double
  EXPECT_TRUE(avm.setMinimumOutdoorEnthalpy(20000.0));
  EXPECT_EQ(20000.0, avm.minimumOutdoorEnthalpy());

  // Maximum Outdoor Enthalpy: Required Double
  EXPECT_TRUE(avm.setMaximumOutdoorEnthalpy(30000.0));
  EXPECT_EQ(30000.0, avm.maximumOutdoorEnthalpy());

  // Minimum Outdoor Dewpoint: Required Double
  EXPECT_TRUE(avm.setMinimumOutdoorDewpoint(17.0));
  EXPECT_EQ(17.0, avm.minimumOutdoorDewpoint());

  // Maximum Outdoor Dewpoint: Required Double
  EXPECT_TRUE(avm.setMaximumOutdoorDewpoint(30.0));
  EXPECT_EQ(30.0, avm.maximumOutdoorDewpoint());

  // Minimum Outdoor Ventilation Air Schedule: Optional Object but set in Ctor and non optional
  ScheduleConstant min_oa_sch(m);
  min_oa_sch.setName("Min OA Schedule");
  EXPECT_TRUE(avm.setMinimumOutdoorVentilationAirSchedule(min_oa_sch));
  EXPECT_EQ(min_oa_sch, avm.minimumOutdoorVentilationAirSchedule());

  // Opening Factor Function of Wind Speed Curve: Optional Object
  CurveLinear opening_factor_curve(m);
  opening_factor_curve.setName("Opening Factor Function of Wind Speed Curve");
  EXPECT_TRUE(avm.setOpeningFactorFunctionofWindSpeedCurve(opening_factor_curve));
  ASSERT_TRUE(avm.openingFactorFunctionofWindSpeedCurve());
  EXPECT_EQ(opening_factor_curve, avm.openingFactorFunctionofWindSpeedCurve().get());

  // AirflowNetwork Control Type Schedule: Optional Object
  ScheduleConstant afn_control_sch(m);
  afn_control_sch.setName("AirflowNetwork Control Type Schedule");
  EXPECT_TRUE(avm.setAirflowNetworkControlTypeSchedule(afn_control_sch));
  ASSERT_TRUE(avm.airflowNetworkControlTypeSchedule());
  EXPECT_EQ(afn_control_sch, avm.airflowNetworkControlTypeSchedule().get());

  // Simple Airflow Control Type Schedule: Optional Object
  ScheduleConstant simple_control_sch(m);
  simple_control_sch.setName("Simple Airflow Control Type Schedule");
  EXPECT_TRUE(avm.setSimpleAirflowControlTypeSchedule(simple_control_sch));
  ASSERT_TRUE(avm.simpleAirflowControlTypeSchedule());
  EXPECT_EQ(simple_control_sch, avm.simpleAirflowControlTypeSchedule().get());

  // ZoneVentilation Object: Optional Object
  ZoneVentilationDesignFlowRate zv(m);
  EXPECT_TRUE(avm.setZoneVentilationObject(zv));
  ASSERT_TRUE(avm.zoneVentilationObject());
  EXPECT_EQ(zv, avm.zoneVentilationObject().get());

  // Minimum HVAC Operation Time: Required Double
  EXPECT_TRUE(avm.setMinimumHVACOperationTime(0.1));
  EXPECT_EQ(0.1, avm.minimumHVACOperationTime());

  // Minimum Ventilation Time: Required Double
  EXPECT_TRUE(avm.setMinimumVentilationTime(0.2));
  EXPECT_EQ(0.2, avm.minimumVentilationTime());

  AvailabilityManagerHybridVentilation avmClone = avm.clone(m).cast<AvailabilityManagerHybridVentilation>();
  EXPECT_FALSE(avmClone.controlledZone());
  EXPECT_FALSE(avmClone.loop());
  EXPECT_FALSE(avmClone.zoneVentilationObject());
  EXPECT_EQ(ventilation_control_sch, avmClone.ventilationControlModeSchedule());
  EXPECT_TRUE(avmClone.useWeatherFileRainIndicators());
  EXPECT_EQ(40.0, avmClone.maximumWindSpeed());
  EXPECT_EQ(15, avmClone.minimumOutdoorTemperature());
  EXPECT_EQ(35.0, avmClone.maximumOutdoorTemperature());
  EXPECT_EQ(20000.0, avmClone.minimumOutdoorEnthalpy());
  EXPECT_EQ(30000.0, avmClone.maximumOutdoorEnthalpy());
  EXPECT_EQ(17.0, avmClone.minimumOutdoorDewpoint());
  EXPECT_EQ(30.0, avmClone.maximumOutdoorDewpoint());
  EXPECT_EQ(min_oa_sch, avmClone.minimumOutdoorVentilationAirSchedule());
  EXPECT_EQ(opening_factor_curve, avmClone.openingFactorFunctionofWindSpeedCurve().get());
  EXPECT_EQ(afn_control_sch, avmClone.airflowNetworkControlTypeSchedule().get());
  EXPECT_EQ(simple_control_sch, avmClone.simpleAirflowControlTypeSchedule().get());
  EXPECT_EQ(0.1, avmClone.minimumHVACOperationTime());
  EXPECT_EQ(0.2, avmClone.minimumVentilationTime());
}
