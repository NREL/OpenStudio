/**********************************************************************
*  Copyright (c) 2008-2010, Alliance for Sustainable Energy.
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
#include <model/CoolingTowerSingleSpeed.hpp>
#include <model/CoolingTowerSingleSpeed_Impl.hpp>

using namespace openstudio;

//Test construction of the coil

TEST(CoolingTowerSingleSpeed,CoolingTowerSingleSpeed_CoolingTowerSingleSpeed)
{
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  ASSERT_EXIT ( 
  {  
    //create a model to use in testing this code.
    model::Model m; 

    model::CoolingTowerSingleSpeed coolingTowerSingleSpeed(m);

    exit(0); 
  } ,
  ::testing::ExitedWithCode(0), "" );
}


