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

#include "../ZoneHVACLowTemperatureRadiantElectric.hpp"
#include "../ZoneHVACLowTemperatureRadiantElectric_Impl.hpp"

#include "../ScheduleConstant.hpp"
#include "../ScheduleConstant_Impl.hpp"
#include "../ThermalZone.hpp"
#include "../ThermalZone_Impl.hpp"

#include "../../utilities/units/Unit.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio;
using namespace openstudio::model;



TEST_F(ModelFixture,ZoneHVACLowTemperatureRadiantElectric_Check_Constructor)
{

  // Test constructor

  Model model;
  ScheduleConstant availabilitySchedule(model);
  availabilitySchedule.setValue(1.0); // Always on
  ScheduleConstant temperatureSchedule(model);
  temperatureSchedule.setValue(70.0); // Fixed at 70
  ZoneHVACLowTemperatureRadiantElectric zoneHVACLowTemperatureRadiantElectric(model,
                                                                              availabilitySchedule,
                                                                              temperatureSchedule);
  // Testing .idd object type

  EXPECT_EQ(openstudio::IddObjectType::OS_ZoneHVAC_LowTemperatureRadiant_Electric,zoneHVACLowTemperatureRadiantElectric.iddObjectType().value());

  //test add and remove from thermal zone
  ThermalZone thermalZone(model);
  EXPECT_EQ(0u,thermalZone.equipment().size());

  EXPECT_TRUE(zoneHVACLowTemperatureRadiantElectric.addToThermalZone(thermalZone));

  EXPECT_EQ(1u,thermalZone.equipment().size());

}

TEST_F(ModelFixture,ZoneHVACLowTemperatureRadiantElectric_Check_Schedules)
{

  Model model;
  ScheduleConstant availabilitySchedule(model);
  availabilitySchedule.setValue(1.0); // Always on
  ScheduleConstant temperatureSchedule(model);
  temperatureSchedule.setValue(70.0); // Fixed at 70
  ZoneHVACLowTemperatureRadiantElectric zoneHVACLowTemperatureRadiantElectric(model,
                                                                              availabilitySchedule,
                                                                              temperatureSchedule);
  //Test set and get availability schedule

  ScheduleConstant test_availability_schedule(model);
  test_availability_schedule.setValue(1.0);
  EXPECT_TRUE(zoneHVACLowTemperatureRadiantElectric.setAvailabilitySchedule(test_availability_schedule));
  EXPECT_EQ(zoneHVACLowTemperatureRadiantElectric.availabilitySchedule(),test_availability_schedule);

  //Test set and get temperature schedule

  ScheduleConstant test_temperature_schedule(model);
  test_temperature_schedule.setValue(70.0);
  EXPECT_TRUE(zoneHVACLowTemperatureRadiantElectric.setHeatingSetpointTemperatureSchedule(test_temperature_schedule));
  EXPECT_EQ(zoneHVACLowTemperatureRadiantElectric.heatingSetpointTemperatureSchedule(),test_temperature_schedule);

}
    // Test cloning the coil

TEST_F(ModelFixture,ZoneHVACLowTemperatureRadiantElectric_Check_Clone)
{
   Model model;
  ScheduleConstant availabilitySchedule(model);
  availabilitySchedule.setValue(1.0); // Always on
  ScheduleConstant temperatureSchedule(model);
  temperatureSchedule.setValue(70.0); // Fixed at 70
  ZoneHVACLowTemperatureRadiantElectric zoneHVACLowTemperatureRadiantElectric(model,
                                                                              availabilitySchedule,
                                                                              temperatureSchedule);
  zoneHVACLowTemperatureRadiantElectric.setMaximumElectricalPowertoPanel(100);

  // Clone into the same model
  ZoneHVACLowTemperatureRadiantElectric cloneLTRE = zoneHVACLowTemperatureRadiantElectric.clone(model).cast<model::ZoneHVACLowTemperatureRadiantElectric>();
  ASSERT_EQ(zoneHVACLowTemperatureRadiantElectric.maximumElectricalPowertoPanel(), cloneLTRE.maximumElectricalPowertoPanel());

  // Clone into another model
  Model model2;
  ZoneHVACLowTemperatureRadiantElectric cloneLTRE2 =   cloneLTRE.clone(model2).cast<model::ZoneHVACLowTemperatureRadiantElectric>();
  ASSERT_EQ(cloneLTRE.maximumElectricalPowertoPanel(), cloneLTRE2.maximumElectricalPowertoPanel());
}


TEST_F(ModelFixture,ZoneHVACLowTemperatureRadiantElectric_Check_Setters_Getters)
{

  Model model;
  ScheduleConstant availabilitySchedule(model);
  availabilitySchedule.setValue(1.0); // Always on
  ScheduleConstant temperatureSchedule(model);
  temperatureSchedule.setValue(70.0); // Fixed at 70
  ZoneHVACLowTemperatureRadiantElectric zoneHVACLowTemperatureRadiantElectric(model,
                                                                              availabilitySchedule,
                                                                              temperatureSchedule);
  // Field A4 Radiant Surface Type

  // Check default ("Ceilings")
  boost::optional<std::string> testRadiantSurfaceType = zoneHVACLowTemperatureRadiantElectric.radiantSurfaceType();
  EXPECT_EQ(*testRadiantSurfaceType, "Ceilings");

  zoneHVACLowTemperatureRadiantElectric.setRadiantSurfaceType("Floors");
  boost::optional<std::string> str1 = zoneHVACLowTemperatureRadiantElectric.radiantSurfaceType();
  EXPECT_EQ(*str1,"Floors");

  zoneHVACLowTemperatureRadiantElectric.resetRadiantSurfaceType();
  testRadiantSurfaceType = zoneHVACLowTemperatureRadiantElectric.radiantSurfaceType();
  EXPECT_EQ(*testRadiantSurfaceType,"Ceilings");

  // Field N1 Maximum Electrical Power to Panel. Test set and get Maximum Electrical Power to Panel, and test autosize and reset functions

  EXPECT_TRUE(zoneHVACLowTemperatureRadiantElectric.setMaximumElectricalPowertoPanel(100));
  boost::optional<double> testMaximumElectricalPowertoPanel = zoneHVACLowTemperatureRadiantElectric.maximumElectricalPowertoPanel();

  EXPECT_EQ((*testMaximumElectricalPowertoPanel),100);
  EXPECT_FALSE(zoneHVACLowTemperatureRadiantElectric.isMaximumElectricalPowertoPanelDefaulted());
  EXPECT_FALSE(zoneHVACLowTemperatureRadiantElectric.isMaximumElectricalPowertoPanelAutosized());

  zoneHVACLowTemperatureRadiantElectric.resetMaximumElectricalPowertoPanel();
  EXPECT_FALSE(zoneHVACLowTemperatureRadiantElectric.maximumElectricalPowertoPanel());
  EXPECT_TRUE(zoneHVACLowTemperatureRadiantElectric.isMaximumElectricalPowertoPanelDefaulted());

  zoneHVACLowTemperatureRadiantElectric.autosizeMaximumElectricalPowertoPanel();
  EXPECT_TRUE(zoneHVACLowTemperatureRadiantElectric.isMaximumElectricalPowertoPanelAutosized());

  // Field A5 Temperature Control Type. Test set and get Temperature Control Type, and test default and reset functions

  EXPECT_TRUE(zoneHVACLowTemperatureRadiantElectric.setTemperatureControlType("MeanRadiantTemperature"));
  std::string testTemperatureControlType = zoneHVACLowTemperatureRadiantElectric.temperatureControlType();

  EXPECT_EQ(testTemperatureControlType,"MeanRadiantTemperature");
  EXPECT_FALSE(zoneHVACLowTemperatureRadiantElectric.isTemperatureControlTypeDefaulted());

  zoneHVACLowTemperatureRadiantElectric.resetTemperatureControlType();
  testTemperatureControlType = zoneHVACLowTemperatureRadiantElectric.temperatureControlType();
  EXPECT_EQ(testTemperatureControlType,"MeanAirTemperature");
  EXPECT_TRUE(zoneHVACLowTemperatureRadiantElectric.isTemperatureControlTypeDefaulted());

  // Field N2 Heating Throttling Range. Test set and get Heating Throttling Range, and test default and reset functions

  EXPECT_TRUE(zoneHVACLowTemperatureRadiantElectric.setHeatingThrottlingRange(6));
  double testHeatingThrottlingRange = zoneHVACLowTemperatureRadiantElectric.heatingThrottlingRange();

  EXPECT_EQ(testHeatingThrottlingRange,6);
  EXPECT_FALSE(zoneHVACLowTemperatureRadiantElectric.isHeatingThrottlingRangeDefaulted());

  zoneHVACLowTemperatureRadiantElectric.resetHeatingThrottlingRange();
  testHeatingThrottlingRange = zoneHVACLowTemperatureRadiantElectric.heatingThrottlingRange();
  EXPECT_EQ(testHeatingThrottlingRange,0);
  EXPECT_TRUE(zoneHVACLowTemperatureRadiantElectric.isHeatingThrottlingRangeDefaulted());

}
