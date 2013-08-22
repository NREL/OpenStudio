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

#include <model/test/ModelFixture.hpp>
#include <model/ScheduleConstant.hpp>
#include <model/ScheduleConstant_Impl.hpp>

#include <model/CoilHeatingLowTempRadiantVarFlow.hpp>
#include <model/CoilHeatingLowTempRadiantVarFlow_Impl.hpp>


using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture,CoilHeatingLowTempRadiantVarFlow_Check_Constructor) 
{  
  Model model;
  ScheduleConstant heatingControlTemperatureSchedule(model);
  heatingControlTemperatureSchedule.setValue(15.0);

  CoilHeatingLowTempRadiantVarFlow testCoil(model,
                                            heatingControlTemperatureSchedule);
 
  // Test set and get temperature schedule
  EXPECT_TRUE(testCoil.setHeatingControlTemperatureSchedule(heatingControlTemperatureSchedule));
  boost::optional<Schedule> sch1 = testCoil.heatingControlTemperatureSchedule();
  EXPECT_EQ(*sch1,heatingControlTemperatureSchedule);
} 

TEST_F(ModelFixture,CoilHeatingLowTempRadiantVarFlow_Getters_Setters) 
{  
		Model model;
  ScheduleConstant heatingControlTemperatureSchedule(model);
  heatingControlTemperatureSchedule.setValue(15.0);
  
  CoilHeatingLowTempRadiantVarFlow testCoil(model,
																																														heatingControlTemperatureSchedule);

  // Field N1 Maximum Hot Water Flow
  
  EXPECT_TRUE(testCoil.setMaximumHotWaterFlow(100));
  EXPECT_EQ(*testCoil.maximumHotWaterFlow(),100);
  EXPECT_FALSE(testCoil.isMaximumHotWaterFlowDefaulted());
  EXPECT_FALSE(testCoil.isMaximumHotWaterFlowAutosized());
  
  testCoil.resetMaximumHotWaterFlow();
		EXPECT_TRUE(testCoil.isMaximumHotWaterFlowDefaulted());
		
		testCoil.autosizeMaximumHotWaterFlow();
		EXPECT_TRUE(testCoil.isMaximumHotWaterFlowAutosized());
		
		// Field N2 Heating Control Throttling Range
		
		EXPECT_TRUE(testCoil.setHeatingControlThrottlingRange(1.0));
		boost::optional<double> value = testCoil.heatingControlThrottlingRange();
  EXPECT_EQ(*value,1.0);
  EXPECT_FALSE(testCoil.isHeatingControlThrottlingRangeDefaulted());
  
  testCoil.resetHeatingControlThrottlingRange();
  value = testCoil.heatingControlThrottlingRange();
		EXPECT_TRUE(testCoil.isHeatingControlThrottlingRangeDefaulted());
		EXPECT_EQ(*value,0.5); 
}

