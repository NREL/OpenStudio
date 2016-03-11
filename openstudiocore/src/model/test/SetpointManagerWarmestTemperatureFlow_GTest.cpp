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

#include "../SetpointManagerWarmestTemperatureFlow.hpp"
#include "../SetpointManagerWarmestTemperatureFlow_Impl.hpp"
#include "../Node.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, SetpointManagerWarmestTemperatureFlow)
{
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  ASSERT_EXIT ( 
  {  
    Model m;
    SetpointManagerWarmestTemperatureFlow testObject(m);

    exit(0); 
  } ,
    ::testing::ExitedWithCode(0), "" );

  {
    Model m;
    SetpointManagerWarmestTemperatureFlow spm(m);

    AirLoopHVAC airLoop(m);
    auto node = airLoop.supplyOutletNode();

    EXPECT_TRUE(spm.addToNode(node));
    EXPECT_EQ(1u,node.setpointManagers().size());

    spm.remove();
    EXPECT_EQ(0u,node.setpointManagers().size());
  }
}

