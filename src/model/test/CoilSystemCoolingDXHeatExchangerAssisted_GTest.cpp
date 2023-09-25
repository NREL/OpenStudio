/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>
#include "ModelFixture.hpp"
#include "../CoilSystemCoolingDXHeatExchangerAssisted.hpp"
#include "../CoilSystemCoolingDXHeatExchangerAssisted_Impl.hpp"

#include "../CoilCoolingDXSingleSpeed.hpp"
#include "../CoilCoolingDXSingleSpeed_Impl.hpp"
#include "../CoilCoolingDXTwoSpeed.hpp"
#include "../CoilCoolingDXVariableSpeed.hpp"
#include "../CoilHeatingGasMultiStage.hpp"
#include "../CoilCoolingDXMultiSpeed.hpp"

#include "../HeatExchangerAirToAirSensibleAndLatent.hpp"
#include "../HeatExchangerAirToAirSensibleAndLatent_Impl.hpp"
#include "../HeatExchangerDesiccantBalancedFlow.hpp"
#include "../HeatExchangerDesiccantBalancedFlow_Impl.hpp"
#include "../AirLoopHVAC.hpp"
#include "../PlantLoop.hpp"
#include "../Node.hpp"
#include "../AirLoopHVACUnitarySystem.hpp"
#include "../AirLoopHVACUnitaryHeatPumpAirToAir.hpp"
#include "../AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass.hpp"
#include "../AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed.hpp"
#include "../ZoneHVACPackagedTerminalHeatPump.hpp"
#include "../CoilHeatingElectric.hpp"
#include "../CoilHeatingDXSingleSpeed.hpp"
#include "../FanConstantVolume.hpp"
#include "../Schedule.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, CoilSystemCoolingDXHeatExchangerAssisted) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  ASSERT_EXIT(
    {
      Model m;
      CoilSystemCoolingDXHeatExchangerAssisted valve(m);

      exit(0);
    },
    ::testing::ExitedWithCode(0), "");
}

// This test ensures that only the parent CoilSystem can call addToNode, the individual CoilCoolingDXSingleSpeed and HX cannot
TEST_F(ModelFixture, CoilSystemCoolingDXHeatExchangerAssisted_addToNode) {

  Model m;
  CoilSystemCoolingDXHeatExchangerAssisted coilSystem(m);

  AirLoopHVAC a(m);
  Node n = a.supplyOutletNode();

  auto cc = coilSystem.coolingCoil().cast<CoilCoolingDXSingleSpeed>();
  auto hx = coilSystem.heatExchanger().cast<HeatExchangerAirToAirSensibleAndLatent>();

  EXPECT_EQ(2, a.supplyComponents().size());

  EXPECT_FALSE(cc.addToNode(n));
  EXPECT_EQ(2, a.supplyComponents().size());

  EXPECT_FALSE(hx.addToNode(n));
  EXPECT_EQ(2, a.supplyComponents().size());

  /**
   * Note JM 2019-03-13: At this point in time
   * CoilSystemCoolingDXHeatExchangerAssisted is **NOT** allowed on a Branch directly and should be placed inside one of the Unitary systems
   * cf https://github.com/NREL/EnergyPlus/issues/7222
   * This method returns false and does nothing as a result
   */
  EXPECT_FALSE(coilSystem.addToNode(n));
  EXPECT_EQ(2, a.supplyComponents().size());

  {
    auto containingHVACComponent = cc.containingHVACComponent();
    ASSERT_TRUE(containingHVACComponent);
    EXPECT_EQ(containingHVACComponent->handle(), coilSystem.handle());
  }

  {
    auto containingHVACComponent = hx.containingHVACComponent();
    ASSERT_TRUE(containingHVACComponent);
    EXPECT_EQ(containingHVACComponent->handle(), coilSystem.handle());
  }
}

// This test ensures that only the parent CoilSystem can call addToNode, the individual CoilCoolingDXSingleSpeed and HX cannot
TEST_F(ModelFixture, CoilSystemCoolingDXHeatExchangerAssisted_addToNode2) {

  Model m;
  HeatExchangerDesiccantBalancedFlow hx(m);
  CoilSystemCoolingDXHeatExchangerAssisted coilSystem(m, hx);

  AirLoopHVAC a(m);
  Node n = a.supplyOutletNode();

  auto cc = coilSystem.coolingCoil().cast<CoilCoolingDXSingleSpeed>();

  EXPECT_EQ(2, a.supplyComponents().size());

  EXPECT_FALSE(cc.addToNode(n));
  EXPECT_EQ(2, a.supplyComponents().size());

  EXPECT_FALSE(hx.addToNode(n));
  EXPECT_EQ(2, a.supplyComponents().size());

  /**
   * Note JM 2019-03-13: At this point in time
   * CoilSystemCoolingDXHeatExchangerAssisted is **NOT** allowed on a Branch directly and should be placed inside one of the Unitary systems
   * cf https://github.com/NREL/EnergyPlus/issues/7222
   * This method returns false and does nothing as a result
   */
  EXPECT_FALSE(coilSystem.addToNode(n));
  EXPECT_EQ(2, a.supplyComponents().size());

  {
    auto containingHVACComponent = cc.containingHVACComponent();
    ASSERT_TRUE(containingHVACComponent);
    EXPECT_EQ(containingHVACComponent->handle(), coilSystem.handle());
  }

  {
    auto containingHVACComponent = hx.containingHVACComponent();
    ASSERT_TRUE(containingHVACComponent);
    EXPECT_EQ(containingHVACComponent->handle(), coilSystem.handle());
  }
}

TEST_F(ModelFixture, CoilSystemCoolingDXHeatExchangerAssisted_GettersSetters) {

  Model m;

  // Create a CoilSystem, connected to an AirLoopHVAC
  CoilSystemCoolingDXHeatExchangerAssisted coilSystem(m);
  auto cc = coilSystem.coolingCoil().cast<CoilCoolingDXSingleSpeed>();
  auto hx = coilSystem.heatExchanger().cast<HeatExchangerAirToAirSensibleAndLatent>();

  // Check the cooling coils
  CoilCoolingDXTwoSpeed cc_bad(m);
  CoilCoolingDXVariableSpeed cc_ok(m);

  EXPECT_FALSE(coilSystem.setCoolingCoil(cc_bad));
  EXPECT_EQ(cc, coilSystem.coolingCoil());

  EXPECT_TRUE(coilSystem.setCoolingCoil(cc_ok));
  EXPECT_EQ(cc_ok, coilSystem.coolingCoil());

  HeatExchangerAirToAirSensibleAndLatent hx2(m);
  EXPECT_TRUE(coilSystem.setHeatExchanger(hx2));
  EXPECT_EQ(hx2, coilSystem.heatExchanger());
}

TEST_F(ModelFixture, CoilSystemCoolingDXHeatExchangerAssisted_clone) {

  Model m;

  // Create a CoilSystem, connected to an AirLoopHVAC
  CoilSystemCoolingDXHeatExchangerAssisted coilSystem(m);
  auto cc = coilSystem.coolingCoil().cast<CoilCoolingDXSingleSpeed>();
  auto hx = coilSystem.heatExchanger().cast<HeatExchangerAirToAirSensibleAndLatent>();

  AirLoopHVAC a(m);
  Node n = a.supplyOutletNode();

  /**
   * Note JM 2019-03-13: At this point in time
   * CoilSystemCoolingDXHeatExchangerAssisted is **NOT** allowed on a Branch directly and should be placed inside one of the Unitary systems
   * cf https://github.com/NREL/EnergyPlus/issues/7222
   * This method returns false and does nothing as a result
   */
  EXPECT_FALSE(coilSystem.addToNode(n));
  // ASSERT_TRUE(coilSystem.airLoopHVAC());
  // EXPECT_EQ(coilSystem.airLoopHVAC()->handle(), a.handle());

  EXPECT_EQ(1u, m.getConcreteModelObjects<CoilSystemCoolingDXHeatExchangerAssisted>().size());
  EXPECT_EQ(1u, m.getConcreteModelObjects<CoilCoolingDXSingleSpeed>().size());
  EXPECT_EQ(1u, m.getConcreteModelObjects<HeatExchangerAirToAirSensibleAndLatent>().size());

  // TODO: should these work?
  // EXPECT_TRUE(hx.airLoopHVAC());

  auto coilSystem2 = coilSystem.clone(m).cast<CoilSystemCoolingDXHeatExchangerAssisted>();

  EXPECT_EQ(2u, m.getConcreteModelObjects<CoilSystemCoolingDXHeatExchangerAssisted>().size());
  EXPECT_EQ(2u, m.getConcreteModelObjects<CoilCoolingDXSingleSpeed>().size());
  EXPECT_EQ(2u, m.getConcreteModelObjects<HeatExchangerAirToAirSensibleAndLatent>().size());

  // Cannot be directly on airLoopHVAC right now
  // EXPECT_TRUE(coilSystem.airLoopHVAC());
  // EXPECT_TRUE(coilSystem.inletModelObject());
  // EXPECT_TRUE(coilSystem.outletModelObject());

  EXPECT_FALSE(coilSystem2.airLoopHVAC());
  EXPECT_FALSE(coilSystem2.inletModelObject());
  EXPECT_FALSE(coilSystem2.outletModelObject());

  auto cc2 = coilSystem2.coolingCoil().cast<CoilCoolingDXSingleSpeed>();
  auto hx2 = coilSystem2.heatExchanger().cast<HeatExchangerAirToAirSensibleAndLatent>();

  EXPECT_NE(cc2.handle(), cc.handle());
  EXPECT_NE(hx2.handle(), hx.handle());

  EXPECT_FALSE(hx2.airLoopHVAC());
}

TEST_F(ModelFixture, CoilSystemCoolingDXHeatExchangerAssisted_containingComponent) {

  Model m;

  {
    CoilSystemCoolingDXHeatExchangerAssisted coilSystem(m);

    AirLoopHVACUnitarySystem unitary(m);
    EXPECT_TRUE(unitary.setCoolingCoil(coilSystem));
    boost::optional<HVACComponent> _c = coilSystem.containingHVACComponent();
    ASSERT_TRUE(_c);
    EXPECT_EQ(_c->handle(), unitary.handle());
  }

  {
    Schedule s = m.alwaysOnDiscreteSchedule();
    FanConstantVolume supplyFan(m);
    CoilHeatingDXSingleSpeed coilHeatingDX(m);
    CoilCoolingDXSingleSpeed coilCoolingDX(m);
    CoilHeatingElectric supplementalHeatingCoil(m);

    AirLoopHVACUnitaryHeatPumpAirToAir unitary(m, s, supplyFan, coilHeatingDX, coilCoolingDX, supplementalHeatingCoil);

    CoilSystemCoolingDXHeatExchangerAssisted coilSystem(m);
    EXPECT_TRUE(unitary.setCoolingCoil(coilSystem));
    boost::optional<HVACComponent> _c = coilSystem.containingHVACComponent();
    ASSERT_TRUE(_c);
    EXPECT_EQ(_c->handle(), unitary.handle());
  }

  {
    FanConstantVolume supplyFan(m);
    CoilCoolingDXSingleSpeed coilCoolingDX(m);
    CoilHeatingElectric coilHeatingElectric(m);

    AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass unitary(m, supplyFan, coilCoolingDX, coilHeatingElectric);

    CoilSystemCoolingDXHeatExchangerAssisted coilSystem(m);
    EXPECT_TRUE(unitary.setCoolingCoil(coilSystem));
    boost::optional<HVACComponent> _c = coilSystem.containingHVACComponent();
    ASSERT_TRUE(_c);
    EXPECT_EQ(_c->handle(), unitary.handle());
  }

  // AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed: shouldn't accept it
  {
    FanConstantVolume supplyFan(m);
    // CC and HC have to be multispeed ones
    CoilHeatingGasMultiStage coilHeatingGasMulti(m);
    CoilCoolingDXMultiSpeed coilCoolingDXMulti(m);
    CoilHeatingElectric supplementalHeatingCoil(m);

    AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed unitary(m, supplyFan, coilHeatingGasMulti, coilCoolingDXMulti, supplementalHeatingCoil);

    CoilSystemCoolingDXHeatExchangerAssisted coilSystem(m);
    EXPECT_FALSE(unitary.setCoolingCoil(coilSystem));
  }

  {
    Schedule s = m.alwaysOnDiscreteSchedule();
    FanConstantVolume supplyFan(m);
    CoilHeatingElectric coilHeatingElectric(m);
    CoilCoolingDXSingleSpeed coilCoolingDX(m);
    CoilHeatingElectric supplementalHeatingCoil(m);

    ZoneHVACPackagedTerminalHeatPump pthp(m, s, supplyFan, coilHeatingElectric, coilCoolingDX, supplementalHeatingCoil);

    CoilSystemCoolingDXHeatExchangerAssisted coilSystem(m);
    EXPECT_TRUE(pthp.setCoolingCoil(coilSystem));
    boost::optional<ZoneHVACComponent> _c = coilSystem.containingZoneHVACComponent();
    ASSERT_TRUE(_c);
    EXPECT_EQ(_c->handle(), pthp.handle());
  }
}
