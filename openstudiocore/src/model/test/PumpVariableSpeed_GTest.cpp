/**********************************************************************
*  Copyright (c) 2008-2013, Alliance for Sustainable Energy.
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
#include <model/Model.hpp>
#include <model/Node.hpp>
#include <model/Node_Impl.hpp>
#include <model/PumpVariableSpeed.hpp>
#include <model/PumpVariableSpeed_Impl.hpp>

using namespace openstudio;

TEST(PumpVariableSpeed,PumpVariableSpeed_PumpVariableSpeed)
{
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  ASSERT_EXIT ( 
  {  
     model::Model m; 

     model::PumpVariableSpeed pump(m); 

     exit(0); 
  } ,
    ::testing::ExitedWithCode(0), "" );
}

TEST(PumpVariableSpeed,PumpVariableSpeed_connections)
{
  model::Model m; 
  
  model::PumpVariableSpeed pump(m); 

  model::Node inletNode(m);

  model::Node outletNode(m);

  m.connect(inletNode,inletNode.outletPort(),pump,pump.inletPort());
  m.connect(pump,pump.outletPort(),outletNode,outletNode.inletPort());

  ASSERT_TRUE( pump.inletModelObject() );
  ASSERT_TRUE( pump.outletModelObject() );
 
  EXPECT_EQ( inletNode.handle(), pump.inletModelObject()->handle() );
  EXPECT_EQ( outletNode.handle(), pump.outletModelObject()->handle() );
}

