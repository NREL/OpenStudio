/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
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
