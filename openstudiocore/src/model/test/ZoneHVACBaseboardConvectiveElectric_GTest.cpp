/**********************************************************************
 *  Copyright (c) 2008-2014, Alliance for Sustainable Energy.
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
#include "../ZoneHVACBaseboardConvectiveElectric.hpp"
#include "../ZoneHVACBaseboardConvectiveElectric_Impl.hpp"
#include "../../utilities/units/Quantity.hpp"
#include "../../utilities/units/Unit.hpp"
#include "../ScheduleConstant.hpp"
#include "../ScheduleConstant_Impl.hpp"
#include "../ThermalZone.hpp"
#include "../ThermalZone_Impl.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture,ZoneHVACBaseboardConvectiveElectric_Test) {
  Model model;

   // test constructor
  ZoneHVACBaseboardConvectiveElectric zoneHVACBBConv(model);
  
  // test set and get availibility schedule
  ScheduleConstant sched1(model);
  sched1.setValue(1.0);
  EXPECT_TRUE(zoneHVACBBConv.setAvailabilitySchedule(sched1));
  EXPECT_EQ(zoneHVACBBConv.availabilitySchedule(),sched1);

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

