/**********************************************************************
 *  Copyright (c) 2008-2015, Alliance for Sustainable Energy.
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
#include <model/test/ModelFixture.hpp>
#include <model/AirTerminalDualDuctVAV.hpp>
#include <model/AirTerminalDualDuctVAV_Impl.hpp>
#include <model/AirLoopHVAC.hpp>
#include <model/AirLoopHVAC_Impl.hpp>

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture,AirTerminalDualDuctVAV) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  ASSERT_EXIT ( 
  {  
     Model m; 
     AirTerminalDualDuctVAV terminal(m); 

     exit(0); 
  } ,
    ::testing::ExitedWithCode(0), "" );

  {
    Model m; 
    AirTerminalDualDuctVAV terminal(m); 
    
    AirLoopHVAC airLoopHVAC(m);
    EXPECT_TRUE(airLoopHVAC.addBranchForHVACComponent(terminal));

    EXPECT_EQ(7u,airLoopHVAC.demandComponents().size());
  }
}
