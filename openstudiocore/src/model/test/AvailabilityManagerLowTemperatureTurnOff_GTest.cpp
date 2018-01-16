/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2018, Alliance for Sustainable Energy, LLC. All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 *  following conditions are met:
 *
 *  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
 *  disclaimer.
 *
 *  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *  following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 *  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote
 *  products derived from this software without specific prior written permission from the respective party.
 *
 *  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative
 *  works may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without
 *  specific prior written permission from Alliance for Sustainable Energy, LLC.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER, THE UNITED STATES GOVERNMENT, OR ANY CONTRIBUTORS BE LIABLE FOR
 *  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **********************************************************************************************************************/

#include <gtest/gtest.h>

#include <model/test/ModelFixture.hpp>

#include "../AvailabilityManagerLowTemperatureTurnOff.hpp"
#include "../AvailabilityManagerLowTemperatureTurnOff_Impl.hpp"

#include "../Node.hpp"
#include "../AirLoopHVAC.hpp"
#include "../PlantLoop.hpp"
#include "../ScheduleConstant.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, AvailabilityManagerLowTemperatureTurnOff_DefaultConstructor)
{
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  ASSERT_EXIT (
  {
     Model m;
     AvailabilityManagerLowTemperatureTurnOff avm(m);

     exit(0);
  } ,
    ::testing::ExitedWithCode(0), "" );
}

TEST_F(ModelFixture, AvailabilityManagerLowTemperatureTurnOff_Setters_Getters)
{
  Model m;

  AvailabilityManagerLowTemperatureTurnOff avm(m);

  // Test Sensor node fields
  // Random node
  Node n(m);
  ASSERT_FALSE(avm.sensorNode());
  EXPECT_TRUE(avm.setSensorNode(n));
  ASSERT_TRUE(avm.sensorNode());
  ASSERT_EQ(n.handle(), avm.sensorNode()->handle());
  avm.resetSensorNode();
  ASSERT_FALSE(avm.sensorNode());

  // AirLoop Node
  AirLoopHVAC airloop(m);
  Node airLoop_Node = airloop.supplyOutletNode();
  EXPECT_TRUE(avm.setSensorNode(airLoop_Node));
  ASSERT_TRUE(avm.sensorNode());
  ASSERT_EQ(airLoop_Node.handle(), avm.sensorNode()->handle());

  // PlantLoop Node
  PlantLoop plantLoop(m);
  Node plantLoop_Node = plantLoop.supplyOutletNode();
  EXPECT_TRUE(avm.setSensorNode(plantLoop_Node));
  ASSERT_TRUE(avm.sensorNode());
  ASSERT_EQ(plantLoop_Node.handle(), avm.sensorNode()->handle());


  // Test temperature
  EXPECT_TRUE(avm.setTemperature(60.19));
  ASSERT_EQ(60.19, avm.temperature());

  // Test Applicability Schedule
  ASSERT_EQ(m.alwaysOnDiscreteSchedule(), avm.applicabilitySchedule());
  ScheduleConstant tempSch(m);
  tempSch.setValue(50);
  EXPECT_TRUE(avm.setApplicabilitySchedule(tempSch));
  ASSERT_EQ(tempSch.handle(), avm.applicabilitySchedule().handle());
}
