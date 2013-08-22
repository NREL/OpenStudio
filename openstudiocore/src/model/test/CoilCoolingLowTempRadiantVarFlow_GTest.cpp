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

#include <model/CoilCoolingLowTempRadiantVarFlow.hpp>
#include <model/CoilCoolingLowTempRadiantVarFlow_Impl.hpp>


using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture,CoilCoolingLowTempRadiantVarFlow_Check_Constructor) 
{  
  Model model;
  ScheduleConstant coolingControlTemperatureSchedule(model);
  coolingControlTemperatureSchedule.setValue(15.0);

  CoilCoolingLowTempRadiantVarFlow testCoil(model,
                                            coolingControlTemperatureSchedule);
 
  // Test set and get temperature schedule
  EXPECT_TRUE(testCoil.setCoolingControlTemperatureSchedule(coolingControlTemperatureSchedule));
  boost::optional<Schedule> sch1 = testCoil.coolingControlTemperatureSchedule();
  EXPECT_EQ(*sch1,coolingControlTemperatureSchedule);
} 

TEST_F(ModelFixture,CoilCoolingLowTempRadiantVarFlow_Getters_Setters) 
{  
		Model model;
  ScheduleConstant coolingControlTemperatureSchedule(model);
  coolingControlTemperatureSchedule.setValue(15.0);
  
  CoilCoolingLowTempRadiantVarFlow testCoil(model,coolingControlTemperatureSchedule);

  // Field N1 Maximum Hot Water Flow
  
  EXPECT_TRUE(testCoil.setMaximumColdWaterFlow(100));
  EXPECT_EQ(*testCoil.maximumColdWaterFlow(),100);
  EXPECT_FALSE(testCoil.isMaximumColdWaterFlowDefaulted());
  EXPECT_FALSE(testCoil.isMaximumColdWaterFlowAutosized());
  
  testCoil.resetMaximumColdWaterFlow();
		EXPECT_TRUE(testCoil.isMaximumColdWaterFlowDefaulted());
		
		testCoil.autosizeMaximumColdWaterFlow();
		EXPECT_TRUE(testCoil.isMaximumColdWaterFlowAutosized());
		
		// Field N2 Cooling Control Throttling Range
		
		EXPECT_TRUE(testCoil.setCoolingControlThrottlingRange(1.0));
		boost::optional<double> value = testCoil.coolingControlThrottlingRange();
  EXPECT_EQ(*value,1.0);
  EXPECT_FALSE(testCoil.isCoolingControlThrottlingRangeDefaulted());
  
  testCoil.resetCoolingControlThrottlingRange();
  value = testCoil.coolingControlThrottlingRange();
		EXPECT_TRUE(testCoil.isCoolingControlThrottlingRangeDefaulted());
		EXPECT_EQ(*value,0.5);

		// Field A6 Condensation Control Type
		
		testCoil.setCondensationControlType("VariableOff");
		boost::optional<std::string> str =  testCoil.condensationControlType();
		EXPECT_EQ(*str,"VariableOff");
		EXPECT_FALSE(testCoil.isCondensationControlTypeDefaulted());
		
		testCoil.resetCondensationControlType();
		str = testCoil.condensationControlType();
		EXPECT_EQ(*str,"SimpleOff");
		EXPECT_TRUE(testCoil.isCondensationControlTypeDefaulted());
		
		// Field N3 Condensation Control Dewpoint Offset
		
		testCoil.setCondensationControlDewpointOffset(0.5);
		value = testCoil.condensationControlDewpointOffset();
		EXPECT_EQ(*value,0.5);
		EXPECT_FALSE(testCoil.isCondensationControlDewpointOffsetDefaulted());
		
		testCoil.resetCondensationControlDewpointOffset();
		value = testCoil.condensationControlDewpointOffset();
		EXPECT_EQ(*value,1.0);
		EXPECT_TRUE(testCoil.isCondensationControlDewpointOffsetDefaulted());
		
}

