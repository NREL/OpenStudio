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
#include "../ScheduleConstant.hpp"
#include "../ScheduleConstant_Impl.hpp"
#include "../AirLoopHVAC.hpp"
#include "../PlantLoop.hpp"
#include "../Node.hpp"
#include "../Node_Impl.hpp"
#include "../AirLoopHVACZoneSplitter.hpp"

#include "../CoilHeatingLowTempRadiantConstFlow.hpp"
#include "../CoilHeatingLowTempRadiantConstFlow_Impl.hpp"

using namespace openstudio::model;

TEST_F(ModelFixture,CoilHeatingLowTempRadiantConstFlow_SetGetFields) {
  Model model;
  ScheduleConstant heatingHighWaterTempSched(model);
  ScheduleConstant heatingLowWaterTempSched(model);
  ScheduleConstant heatingHighControlTempSched(model);
  ScheduleConstant heatingLowControlTempSched(model);

  CoilHeatingLowTempRadiantConstFlow testHC(model,
                                            heatingHighWaterTempSched,
                                            heatingLowWaterTempSched,
                                            heatingHighControlTempSched,
                                            heatingLowControlTempSched);
  heatingHighWaterTempSched.setValue(50.0);
  EXPECT_TRUE(testHC.setHeatingHighWaterTemperatureSchedule(heatingHighWaterTempSched));
  boost::optional<Schedule> sch1 = testHC.heatingHighWaterTemperatureSchedule();
  EXPECT_EQ(*sch1,heatingHighWaterTempSched);

  heatingLowWaterTempSched.setValue(30.0);
  EXPECT_TRUE(testHC.setHeatingLowWaterTemperatureSchedule(heatingLowWaterTempSched));
  boost::optional<Schedule> sch2 = testHC.heatingLowWaterTemperatureSchedule();
  EXPECT_EQ(*sch2,heatingLowWaterTempSched);

  heatingHighControlTempSched.setValue(21.0);
  EXPECT_TRUE(testHC.setHeatingHighControlTemperatureSchedule(heatingHighControlTempSched));
  boost::optional<Schedule> sch3 = testHC.heatingHighControlTemperatureSchedule();
  EXPECT_EQ(*sch3,heatingHighControlTempSched);
  
  heatingLowControlTempSched.setValue(15.0);
  EXPECT_TRUE(testHC.setHeatingLowControlTemperatureSchedule(heatingLowControlTempSched));
  boost::optional<Schedule> sch4 = testHC.heatingLowControlTemperatureSchedule();
  EXPECT_EQ(*sch4,heatingLowControlTempSched);

}

TEST_F(ModelFixture,CoilHeatingLowTempRadiantConstFlow_addToNode) {
  Model m;
  ScheduleConstant heatingHighWaterTempSched(m);
  ScheduleConstant heatingLowWaterTempSched(m);
  ScheduleConstant heatingHighControlTempSched(m);
  ScheduleConstant heatingLowControlTempSched(m);

  CoilHeatingLowTempRadiantConstFlow testObject(m,
                                                heatingHighWaterTempSched,
                                                heatingLowWaterTempSched,
                                                heatingHighControlTempSched,
                                                heatingLowControlTempSched);

  AirLoopHVAC airLoop(m);

  Node supplyOutletNode = airLoop.supplyOutletNode();

  EXPECT_FALSE(testObject.addToNode(supplyOutletNode));
  EXPECT_EQ( (unsigned)2, airLoop.supplyComponents().size() );

  Node inletNode = airLoop.zoneSplitter().lastOutletModelObject()->cast<Node>();

  EXPECT_FALSE(testObject.addToNode(inletNode));
  EXPECT_EQ((unsigned)5, airLoop.demandComponents().size());

  PlantLoop plantLoop(m);
  supplyOutletNode = plantLoop.supplyOutletNode();
  EXPECT_FALSE(testObject.addToNode(supplyOutletNode));
  EXPECT_EQ( (unsigned)5, plantLoop.supplyComponents().size() );

  Node demandOutletNode = plantLoop.demandOutletNode();
  EXPECT_TRUE(testObject.addToNode(demandOutletNode));
  EXPECT_EQ( (unsigned)7, plantLoop.demandComponents().size() );

  CoilHeatingLowTempRadiantConstFlow testObjectClone = testObject.clone(m).cast<CoilHeatingLowTempRadiantConstFlow>();
  demandOutletNode = plantLoop.demandOutletNode();

  EXPECT_TRUE(testObjectClone.addToNode(demandOutletNode));
  EXPECT_EQ( (unsigned)9, plantLoop.demandComponents().size() );
}
