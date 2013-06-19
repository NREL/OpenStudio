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
#include <model/BoilerHotWater.hpp>
#include <model/BoilerHotWater_Impl.hpp>

using namespace openstudio;

TEST(BoilerHotWater,BoilerHotWater_BoilerHotWater)
{
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  ASSERT_EXIT ( 
  {  
     model::Model m; 

     model::BoilerHotWater boiler(m); 

     exit(0); 
  } ,
    ::testing::ExitedWithCode(0), "" );
}

TEST(BoilerHotWater,BoilerHotWater_connections)
{
  model::Model m; 
  
  model::BoilerHotWater boiler(m); 

  model::Node inletNode(m);

  model::Node outletNode(m);

  m.connect(inletNode,inletNode.outletPort(),boiler,boiler.inletPort());
  m.connect(boiler,boiler.outletPort(),outletNode,outletNode.inletPort());

  ASSERT_TRUE( boiler.inletModelObject() );
  ASSERT_TRUE( boiler.outletModelObject() );
 
  EXPECT_EQ( inletNode.handle(), boiler.inletModelObject()->handle() );
  EXPECT_EQ( outletNode.handle(), boiler.outletModelObject()->handle() );
}

