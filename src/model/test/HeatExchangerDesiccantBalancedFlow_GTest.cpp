/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2023, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
*  following conditions are met:
*
*  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
*  disclaimer.
*
*  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
*  disclaimer in the documentation and/or other materials provided with the distribution.
*
*  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote products
*  derived from this software without specific prior written permission from the respective party.
*
*  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative works
*  may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without specific prior
*  written permission from Alliance for Sustainable Energy, LLC.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER(S) AND ANY CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
*  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
*  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER(S), ANY CONTRIBUTORS, THE UNITED STATES GOVERNMENT, OR THE UNITED
*  STATES DEPARTMENT OF ENERGY, NOR ANY OF THEIR EMPLOYEES, BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
*  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
*  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
*  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
*  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
***********************************************************************************************************************/

#include <gtest/gtest.h>
#include "ModelFixture.hpp"

#include "../HeatExchangerDesiccantBalancedFlow.hpp"
#include "../HeatExchangerDesiccantBalancedFlow_Impl.hpp"

#include "../HeatExchangerDesiccantBalancedFlowPerformanceDataType1.hpp"
#include "../HeatExchangerDesiccantBalancedFlowPerformanceDataType1_Impl.hpp"
#include "../Schedule.hpp"
#include "../Schedule_Impl.hpp"
#include "../ScheduleConstant.hpp"
#include "../ScheduleConstant_Impl.hpp"
#include "../Node.hpp"
#include "../Node_Impl.hpp"
#include "../AirLoopHVAC.hpp"
#include "../AirLoopHVAC_Impl.hpp"
#include "../AirLoopHVACOutdoorAirSystem.hpp"
#include "../AirLoopHVACOutdoorAirSystem_Impl.hpp"
#include "../ControllerOutdoorAir.hpp"
#include "../ControllerOutdoorAir_Impl.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, HeatExchangerDesiccantBalancedFlow_HeatExchangerDesiccantBalancedFlow) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  ASSERT_EXIT(
    {
      Model m;
      HeatExchangerDesiccantBalancedFlow hx(m);
      HeatExchangerDesiccantBalancedFlowPerformanceDataType1 p(m);
      HeatExchangerDesiccantBalancedFlow hx2(m, p);

      exit(0);
    },
    ::testing::ExitedWithCode(0), "");

  Model m;
  HeatExchangerDesiccantBalancedFlow hx(m);
  HeatExchangerDesiccantBalancedFlowPerformanceDataType1 p(m);
  HeatExchangerDesiccantBalancedFlow hx2(m, p);

  auto alwaysOn = m.alwaysOnDiscreteSchedule();

  EXPECT_EQ(alwaysOn, hx.availabilitySchedule());
  EXPECT_NE(p, hx.heatExchangerPerformance());
  EXPECT_NE(p.handle(), hx.heatExchangerPerformance().handle());
  EXPECT_FALSE(hx.economizerLockout());

  EXPECT_EQ(alwaysOn, hx2.availabilitySchedule());
  EXPECT_EQ(p, hx2.heatExchangerPerformance());
  EXPECT_EQ(p.handle(), hx2.heatExchangerPerformance().handle());
  EXPECT_FALSE(hx2.economizerLockout());
}

TEST_F(ModelFixture, HeatExchangerDesiccantBalancedFlow_GettersSetters) {
  Model m;
  HeatExchangerDesiccantBalancedFlowPerformanceDataType1 p(m);
  HeatExchangerDesiccantBalancedFlow hx(m, p);

  auto alwaysOn = m.alwaysOnDiscreteSchedule();

  EXPECT_EQ(alwaysOn, hx.availabilitySchedule());
  HeatExchangerDesiccantBalancedFlowPerformanceDataType1 performanceObject = hx.heatExchangerPerformance();
  EXPECT_EQ(performanceObject.name().get(), p.name().get());

  ScheduleConstant scheduleConstant2(m);
  hx.setAvailabilitySchedule(scheduleConstant2);
  HeatExchangerDesiccantBalancedFlowPerformanceDataType1 p2(m);
  hx.setHeatExchangerPerformance(p2);

  EXPECT_EQ(scheduleConstant2, hx.availabilitySchedule());
  HeatExchangerDesiccantBalancedFlowPerformanceDataType1 performanceObject2 = hx.heatExchangerPerformance();
  EXPECT_EQ(p2.name().get(), performanceObject2.name().get());
}

TEST_F(ModelFixture, HeatExchangerDesiccantBalancedFlow_addToNode) {
  Model m;

  AirLoopHVAC loop(m);
  Node supplyOutletNode = loop.supplyOutletNode();

  ControllerOutdoorAir controllerOutdoorAir(m);
  AirLoopHVACOutdoorAirSystem oaSystem(m, controllerOutdoorAir);
  oaSystem.addToNode(supplyOutletNode);

  Node oaNode = oaSystem.outboardOANode().get();
  Node reliefNode = oaSystem.outboardReliefNode().get();

  HeatExchangerDesiccantBalancedFlowPerformanceDataType1 p(m);
  HeatExchangerDesiccantBalancedFlow hx1(m, p);
  EXPECT_TRUE(hx1.addToNode(oaNode));

  HeatExchangerDesiccantBalancedFlowPerformanceDataType1 p2(m);
  HeatExchangerDesiccantBalancedFlow hx2(m, p2);
  EXPECT_TRUE(hx2.addToNode(oaNode));

  HeatExchangerDesiccantBalancedFlowPerformanceDataType1 p3(m);
  HeatExchangerDesiccantBalancedFlow hx3(m, p3);
  EXPECT_TRUE(hx3.addToNode(reliefNode));

  HeatExchangerDesiccantBalancedFlowPerformanceDataType1 p4(m);
  HeatExchangerDesiccantBalancedFlow hx4(m, p4);
  EXPECT_TRUE(hx4.addToNode(reliefNode));
}

TEST_F(ModelFixture, HeatExchangerDesiccantBalancedFlow_remove) {
  Model m;

  AirLoopHVAC loop(m);
  Node supplyOutletNode = loop.supplyOutletNode();

  ControllerOutdoorAir controllerOutdoorAir(m);
  AirLoopHVACOutdoorAirSystem oaSystem(m, controllerOutdoorAir);
  oaSystem.addToNode(supplyOutletNode);

  Node oaNode = oaSystem.outboardOANode().get();
  Node reliefNode = oaSystem.outboardReliefNode().get();

  EXPECT_EQ(1u, oaSystem.oaComponents().size());
  EXPECT_EQ(1u, oaSystem.reliefComponents().size());

  HeatExchangerDesiccantBalancedFlowPerformanceDataType1 p(m);
  HeatExchangerDesiccantBalancedFlow hx1(m, p);
  EXPECT_TRUE(hx1.addToNode(oaNode));

  HeatExchangerDesiccantBalancedFlowPerformanceDataType1 p2(m);
  HeatExchangerDesiccantBalancedFlow hx2(m, p2);
  EXPECT_TRUE(hx2.addToNode(oaNode));

  HeatExchangerDesiccantBalancedFlowPerformanceDataType1 p3(m);
  HeatExchangerDesiccantBalancedFlow hx3(m, p3);
  EXPECT_TRUE(hx3.addToNode(reliefNode));

  HeatExchangerDesiccantBalancedFlowPerformanceDataType1 p4(m);
  HeatExchangerDesiccantBalancedFlow hx4(m, p4);
  EXPECT_TRUE(hx4.addToNode(reliefNode));

  EXPECT_EQ(9u, oaSystem.oaComponents().size());
  EXPECT_EQ(9u, oaSystem.reliefComponents().size());

  hx1.remove();
  hx2.remove();
  hx3.remove();
  hx4.remove();

  EXPECT_EQ(1u, oaSystem.oaComponents().size());
  EXPECT_EQ(1u, oaSystem.reliefComponents().size());
}

TEST_F(ModelFixture, HeatExchangerDesiccantBalancedFlow_clone) {
  Model m;
  HeatExchangerDesiccantBalancedFlowPerformanceDataType1 p(m);
  HeatExchangerDesiccantBalancedFlow hx(m, p);

  EXPECT_EQ(p, hx.heatExchangerPerformance());
  EXPECT_EQ(1u, m.getConcreteModelObjects<HeatExchangerDesiccantBalancedFlow>().size());
  EXPECT_EQ(1u, m.getConcreteModelObjects<HeatExchangerDesiccantBalancedFlowPerformanceDataType1>().size());

  auto hxClone = hx.clone(m).cast<HeatExchangerDesiccantBalancedFlow>();
  EXPECT_EQ(2u, m.getConcreteModelObjects<HeatExchangerDesiccantBalancedFlow>().size());
  EXPECT_EQ(1u, m.getConcreteModelObjects<HeatExchangerDesiccantBalancedFlowPerformanceDataType1>().size());
  EXPECT_EQ(p, hx.heatExchangerPerformance());
  EXPECT_EQ(p, hxClone.heatExchangerPerformance());

  hx.remove();
  EXPECT_EQ(1u, m.getConcreteModelObjects<HeatExchangerDesiccantBalancedFlow>().size());
  EXPECT_EQ(1u, m.getConcreteModelObjects<HeatExchangerDesiccantBalancedFlowPerformanceDataType1>().size());
  EXPECT_EQ(p, hxClone.heatExchangerPerformance());

  hxClone.remove();
  EXPECT_EQ(0u, m.getConcreteModelObjects<HeatExchangerDesiccantBalancedFlow>().size());
  EXPECT_EQ(0u, m.getConcreteModelObjects<HeatExchangerDesiccantBalancedFlowPerformanceDataType1>().size());
}
