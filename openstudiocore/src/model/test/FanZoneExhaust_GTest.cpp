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
#include "../FanZoneExhaust.hpp"
#include "../FanZoneExhaust_Impl.hpp"
#include "../Model.hpp"
#include "../Node.hpp"
#include "../Node_Impl.hpp"
#include "../ThermalZone.hpp"
#include "../ThermalZone_Impl.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, FanZoneExhaust_DefaultConstructor)
{
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  ASSERT_EXIT ( 
  {  
    Model model;
    FanZoneExhaust testObject = FanZoneExhaust(model);

    exit(0); 
  } ,
    ::testing::ExitedWithCode(0), "" );
}

TEST_F(ModelFixture, FanZoneExhaust_AddToAndRemoveFromThermalZone)
{
  Model model;
  FanZoneExhaust testObject = FanZoneExhaust(model);
  ThermalZone thermalZone(model);
  
  // Add to thermal zone
  EXPECT_TRUE(testObject.addToThermalZone(thermalZone));
  boost::optional<ThermalZone> testThermalZone = testObject.thermalZone();
  ASSERT_TRUE(testThermalZone);
  EXPECT_EQ(*(testThermalZone),testObject.thermalZone());
  EXPECT_EQ(1u, thermalZone.equipment().size());

  // Check inlet and outlet nodes
  EXPECT_TRUE(testObject.inletNode());
  EXPECT_TRUE(testObject.outletNode());
  
  // Remove from thermal zone
  testObject.removeFromThermalZone();
  EXPECT_FALSE(testObject.thermalZone());
  EXPECT_EQ(0u, thermalZone.equipment().size());
}
