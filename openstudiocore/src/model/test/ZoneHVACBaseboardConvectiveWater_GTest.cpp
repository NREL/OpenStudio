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

#include "../CoilHeatingWaterBaseboard.hpp"
#include "../CoilHeatingWaterBaseboard_Impl.hpp"
#include "../ZoneHVACBaseboardConvectiveWater.hpp"
#include "../ZoneHVACBaseboardConvectiveWater_Impl.hpp"

#include "../ScheduleConstant.hpp"
#include "../ScheduleConstant_Impl.hpp"
#include "../ThermalZone.hpp"
#include "../ThermalZone_Impl.hpp"

#include "../../utilities/units/Quantity.hpp"
#include "../../utilities/units/Unit.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture,ZoneHVACBaseboardConvectiveWater_Test) {
  Model model;

  ScheduleConstant sched(model);
  sched.setValue(1.0); // Always on
  CoilHeatingWaterBaseboard HCBaseboard(model);
   // test constructor
  ZoneHVACBaseboardConvectiveWater zoneHVACBBConv(model,sched,HCBaseboard);

  // test set and get availibility schedule
  ScheduleConstant sched1(model);
  sched1.setValue(1.0);
  EXPECT_TRUE(zoneHVACBBConv.setAvailabilitySchedule(sched1));
  EXPECT_EQ(zoneHVACBBConv.availabilitySchedule(),sched1);

  // test if ZoneHVACBaseboardConvectiveWater has a hot water coil
  CoilHeatingWaterBaseboard HCBaseboard1(model);
  zoneHVACBBConv.setHeatingCoil(HCBaseboard1);
  EXPECT_EQ(zoneHVACBBConv.heatingCoil(),HCBaseboard1);

  // test add thermal zone
  // check if it has a thermal zone, should return no thermal zone
  EXPECT_FALSE(zoneHVACBBConv.thermalZone());
  // add to a thermal zone, check to see that it should have a thermal zone
  ThermalZone thermalZone1(model);
  zoneHVACBBConv.addToThermalZone(thermalZone1);
  boost::optional<ThermalZone> test_ThermalZone = zoneHVACBBConv.thermalZone();
  EXPECT_EQ(*test_ThermalZone,thermalZone1);

  //test remove a thermal zone
  //check to see it should have a thermal zone
  EXPECT_TRUE(zoneHVACBBConv.thermalZone());
  // remove the thermal zone
  zoneHVACBBConv.removeFromThermalZone();
  // check to see if it doesn't have a thermal zone
  EXPECT_FALSE(zoneHVACBBConv.thermalZone());
}
