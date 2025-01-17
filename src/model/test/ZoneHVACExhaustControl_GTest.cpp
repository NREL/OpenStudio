/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2023, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

#include "ModelFixture.hpp"

#include "../ZoneHVACExhaustControl.hpp"
#include "../ZoneHVACExhaustControl_Impl.hpp"
#include "../Schedule.hpp"
#include "../Schedule_Impl.hpp"
#include "../ScheduleConstant.hpp"
#include "../ScheduleConstant_Impl.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, ZoneHVACExhaustControl_GettersSetters) {
  Model m;

  ZoneHVACExhaustControl zoneHVACExhaustControl(m);

  zoneHVACExhaustControl.setName("My ZoneHVACExhaustControl");

  // Availability Schedule Name: Required Object
  Schedule availabilitySchedule = m.alwaysOnDiscreteSchedule();
  EXPECT_TRUE(zoneHVACExhaustControl.setAvailabilitySchedule(availabilitySchedule));
  EXPECT_EQ(availabilitySchedule, zoneHVACExhaustControl.availabilitySchedule());

  // Design Exhaust Flow Rate: Required Double
  // Autosize
  zoneHVACExhaustControl.autosizeDesignExhaustFlowRate();
  EXPECT_TRUE(zoneHVACExhaustControl.isDesignExhaustFlowRateAutosized());
  // Set
  EXPECT_TRUE(zoneHVACExhaustControl.setDesignExhaustFlowRate(0.7));
  ASSERT_TRUE(zoneHVACExhaustControl.designExhaustFlowRate());
  EXPECT_EQ(0.7, zoneHVACExhaustControl.designExhaustFlowRate().get());
  // Bad Value
  EXPECT_FALSE(zoneHVACExhaustControl.setDesignExhaustFlowRate(-10.0));
  ASSERT_TRUE(zoneHVACExhaustControl.designExhaustFlowRate());
  EXPECT_EQ(0.7, zoneHVACExhaustControl.designExhaustFlowRate().get());
  EXPECT_FALSE(zoneHVACExhaustControl.isDesignExhaustFlowRateAutosized());

  // Flow Control Type: Required String
  EXPECT_TRUE(zoneHVACExhaustControl.setFlowControlType("Scheduled"));
  EXPECT_EQ("Scheduled", zoneHVACExhaustControl.flowControlType());
  // Bad Value
  EXPECT_FALSE(zoneHVACExhaustControl.setFlowControlType("BADENUM"));
  EXPECT_EQ("Scheduled", zoneHVACExhaustControl.flowControlType());

  // Exhaust Flow Fraction Schedule Name: Optional Object
  ScheduleConstant exhaustFlowFractionSchedule(m);
  EXPECT_TRUE(zoneHVACExhaustControl.setExhaustFlowFractionSchedule(exhaustFlowFractionSchedule));
  ASSERT_TRUE(zoneHVACExhaustControl.exhaustFlowFractionSchedule());
  EXPECT_EQ(exhaustFlowFractionSchedule, zoneHVACExhaustControl.exhaustFlowFractionSchedule().get());

  // Minimum Zone Temperature Limit Schedule Name: Optional Object
  ScheduleConstant minimumZoneTemperatureLimitSchedule(m);
  EXPECT_TRUE(zoneHVACExhaustControl.setMinimumZoneTemperatureLimitSchedule(minimumZoneTemperatureLimitSchedule));
  ASSERT_TRUE(zoneHVACExhaustControl.minimumZoneTemperatureLimitSchedule());
  EXPECT_EQ(minimumZoneTemperatureLimitSchedule, zoneHVACExhaustControl.minimumZoneTemperatureLimitSchedule().get());

  // Minimum Exhaust Flow Fraction Schedule Name: Optional Object
  ScheduleConstant minimumExhaustFlowFractionSchedule(m);
  EXPECT_TRUE(zoneHVACExhaustControl.setMinimumExhaustFlowFractionSchedule(minimumExhaustFlowFractionSchedule));
  ASSERT_TRUE(zoneHVACExhaustControl.minimumExhaustFlowFractionSchedule());
  EXPECT_EQ(minimumExhaustFlowFractionSchedule, zoneHVACExhaustControl.minimumExhaustFlowFractionSchedule().get());

  // Balanced Exhaust Fraction Schedule Name: Optional Object
  ScheduleConstant balancedExhaustFractionSchedule(m);
  EXPECT_TRUE(zoneHVACExhaustControl.setBalancedExhaustFractionSchedule(balancedExhaustFractionSchedule));
  ASSERT_TRUE(zoneHVACExhaustControl.balancedExhaustFractionSchedule());
  EXPECT_EQ(balancedExhaustFractionSchedule, zoneHVACExhaustControl.balancedExhaustFractionSchedule().get());
}
TEST_F(ModelFixture, ZoneHVACExhaustControl_HeatCoolFuelTypes) {
  Model m;

  ZoneHVACExhaustControl zoneHVACExhaustControl(m);

  EXPECT_EQ(ComponentType(ComponentType::Both), zoneHVACExhaustControl.componentType());
  testFuelTypeEquality({FuelType::Electricity}, zoneHVACExhaustControl.coolingFuelTypes());
  testFuelTypeEquality({FuelType::Electricity, FuelType::Propane}, zoneHVACExhaustControl.heatingFuelTypes());
  testAppGFuelTypeEquality({AppGFuelType::Fuel, AppGFuelType::HeatPump}, zoneHVACExhaustControl.appGHeatingFuelTypes());
}
