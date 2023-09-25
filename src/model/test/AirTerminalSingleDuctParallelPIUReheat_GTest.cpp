/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>
#include "ModelFixture.hpp"
#include "../AirTerminalSingleDuctParallelPIUReheat.hpp"
#include "../AirTerminalSingleDuctParallelPIUReheat_Impl.hpp"
#include "../ScheduleRuleset.hpp"
#include "../ScheduleRuleset_Impl.hpp"
#include "../FanConstantVolume.hpp"
#include "../CoilHeatingElectric.hpp"
#include "../Schedule.hpp"
#include "../AirLoopHVAC.hpp"
#include "../PlantLoop.hpp"
#include "../Node.hpp"
#include "../Node_Impl.hpp"
#include "../ThermalZone.hpp"
#include "../ThermalZone_Impl.hpp"
#include "../PortList.hpp"
#include "../PortList_Impl.hpp"
#include "../AirLoopHVACZoneSplitter.hpp"

#include "../../utilities/idf/IdfExtensibleGroup.hpp"
#include "../ModelExtensibleGroup.hpp"
#include <utilities/idd/OS_PortList_FieldEnums.hxx>

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, AirTerminalSingleDuctParallelPIUReheat_AirTerminalSingleDuctParallelPIUReheat) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  ASSERT_EXIT(
    {
      Model m;
      Schedule s = m.alwaysOnDiscreteSchedule();
      FanConstantVolume piuFan = FanConstantVolume(m, s);
      CoilHeatingElectric piuReheatCoil = CoilHeatingElectric(m, s);
      AirTerminalSingleDuctParallelPIUReheat testObject(m, s, piuFan, piuReheatCoil);

      exit(0);
    },
    ::testing::ExitedWithCode(0), "");
}

TEST_F(ModelFixture, AirTerminalSingleDuctParallelPIUReheat_addToNode) {
  Model m;
  Schedule s = m.alwaysOnDiscreteSchedule();
  FanConstantVolume piuFan = FanConstantVolume(m, s);
  CoilHeatingElectric piuReheatCoil = CoilHeatingElectric(m, s);
  AirTerminalSingleDuctParallelPIUReheat testObject(m, s, piuFan, piuReheatCoil);

  AirLoopHVAC airLoop(m);

  Node supplyOutletNode = airLoop.supplyOutletNode();

  EXPECT_FALSE(testObject.addToNode(supplyOutletNode));
  EXPECT_EQ((unsigned)2, airLoop.supplyComponents().size());

  Node inletNode = airLoop.zoneSplitter().lastOutletModelObject()->cast<Node>();

  EXPECT_TRUE(testObject.addToNode(inletNode));
  EXPECT_EQ((unsigned)7, airLoop.demandComponents().size());

  PlantLoop plantLoop(m);
  supplyOutletNode = plantLoop.supplyOutletNode();
  EXPECT_FALSE(testObject.addToNode(supplyOutletNode));
  EXPECT_EQ((unsigned)5, plantLoop.supplyComponents().size());

  Node demandOutletNode = plantLoop.demandOutletNode();
  EXPECT_FALSE(testObject.addToNode(demandOutletNode));
  EXPECT_EQ((unsigned)5, plantLoop.demandComponents().size());

  auto testObjectClone = testObject.clone(m).cast<AirTerminalSingleDuctParallelPIUReheat>();
  inletNode = airLoop.zoneSplitter().lastOutletModelObject()->cast<Node>();

  EXPECT_FALSE(testObjectClone.addToNode(inletNode));
  EXPECT_TRUE(airLoop.addBranchForHVACComponent(testObjectClone));
  EXPECT_EQ((unsigned)10, airLoop.demandComponents().size());
}

TEST_F(ModelFixture, AirTerminalSingleDuctParallelPIUReheat) {
  // test that setAvailabilitySchedule also set PIU fan schedule
  {
    Model m;
    Schedule schedule = m.alwaysOnDiscreteSchedule();
    FanConstantVolume fan(m, schedule);
    CoilHeatingElectric coil(m, schedule);
    AirTerminalSingleDuctParallelPIUReheat terminal(m, schedule, fan, coil);

    AirLoopHVAC airLoopHVAC(m);
    airLoopHVAC.addBranchForHVACComponent(terminal);

    ScheduleRuleset hvacSchedule(m);
    airLoopHVAC.setAvailabilitySchedule(hvacSchedule);

    auto fanSchedule = fan.availabilitySchedule();
    ASSERT_EQ(hvacSchedule.handle(), fanSchedule.handle());
  }

  // test that addToNode (by proxy addBranchForZone) sets the fan schedule to match system availabilitySchedule
  {
    Model m;
    Schedule schedule = m.alwaysOnDiscreteSchedule();
    FanConstantVolume fan(m, schedule);
    CoilHeatingElectric coil(m, schedule);
    AirTerminalSingleDuctParallelPIUReheat terminal(m, schedule, fan, coil);

    AirLoopHVAC airLoopHVAC(m);

    ScheduleRuleset hvacSchedule(m);
    airLoopHVAC.setAvailabilitySchedule(hvacSchedule);

    ThermalZone zone(m);

    // KSB: I don't think it is the greatest idea to test these private methods,
    // but this area has resulted in a simulation error so it needs to be tested
    // (Note: JM 2021-03-02 These aren't private anymore)
    EXPECT_EQ(0u, zone.exhaustPortList().numExtensibleGroups());
    EXPECT_EQ(0u, zone.inletPortList().numExtensibleGroups());

    airLoopHVAC.addBranchForZone(zone, terminal);
    auto fanSchedule = fan.availabilitySchedule();
    ASSERT_EQ(hvacSchedule.handle(), fanSchedule.handle());

    ASSERT_EQ(1u, zone.exhaustPortList().numExtensibleGroups());
    EXPECT_TRUE(zone.exhaustPortList().extensibleGroups()[0].cast<ModelExtensibleGroup>().getTarget(OS_PortListExtensibleFields::Port));
    ASSERT_EQ(1u, zone.inletPortList().numExtensibleGroups());
    EXPECT_TRUE(zone.inletPortList().extensibleGroups()[0].cast<ModelExtensibleGroup>().getTarget(OS_PortListExtensibleFields::Port));

    EXPECT_EQ(9u, airLoopHVAC.demandComponents().size());
    EXPECT_EQ(1u, zone.equipment().size());

    auto zoneImpl = zone.getImpl<openstudio::model::detail::ThermalZone_Impl>();
    auto exhaustMo = zoneImpl->exhaustPortList().lastModelObject();
    ASSERT_TRUE(exhaustMo);
    auto exhaustNode = exhaustMo->optionalCast<Node>();
    ASSERT_TRUE(exhaustNode);
    ASSERT_TRUE(exhaustNode->outletModelObject());
    ASSERT_EQ(terminal, exhaustNode->outletModelObject().get());

    terminal.remove();

    EXPECT_EQ(0u, zone.exhaustPortList().numExtensibleGroups());
    ASSERT_EQ(1u, zone.inletPortList().numExtensibleGroups());
    EXPECT_TRUE(zone.inletPortList().extensibleGroups()[0].cast<ModelExtensibleGroup>().getTarget(OS_PortListExtensibleFields::Port));

    EXPECT_EQ(7u, airLoopHVAC.demandComponents().size());
    EXPECT_TRUE(zone.equipment().empty());

    EXPECT_FALSE(zoneImpl->exhaustPortList().lastModelObject());
  }
}

TEST_F(ModelFixture, AirTerminalSingleDuctParallelPIUReheat_connectSecondaryAirInlet_regularCase_2033) {

  // Test for #2033
  // Base case: works fine
  Model m;
  Schedule schedule = m.alwaysOnDiscreteSchedule();
  FanConstantVolume fan(m, schedule);
  CoilHeatingElectric coil(m, schedule);
  AirTerminalSingleDuctParallelPIUReheat atu(m, schedule, fan, coil);

  ThermalZone zone(m);
  AirLoopHVAC airLoopHVAC(m);

  EXPECT_FALSE(atu.secondaryAirInletNode());
  EXPECT_FALSE(zone.exhaustPortList().lastModelObject());

  // Connect simulateanously the branch and atu
  EXPECT_TRUE(airLoopHVAC.addBranchForZone(zone, atu));

  ASSERT_TRUE(atu.secondaryAirInletNode());
  ASSERT_TRUE(zone.exhaustPortList().lastModelObject());
  EXPECT_EQ(atu.secondaryAirInletNode().get(), zone.exhaustPortList().lastModelObject().get());
}

TEST_F(ModelFixture, AirTerminalSingleDuctParallelPIUReheat_connectSecondaryAirInlet_atuFirst_2033) {

  // Test for #2033: When you connect the atu first, then add a zone it should work as well.
  Model m;
  Schedule schedule = m.alwaysOnDiscreteSchedule();
  FanConstantVolume fan(m, schedule);
  CoilHeatingElectric coil(m, schedule);
  AirTerminalSingleDuctParallelPIUReheat atu(m, schedule, fan, coil);

  AirLoopHVAC airLoopHVAC(m);

  // Connect atu only first
  airLoopHVAC.addBranchForHVACComponent(atu);
  EXPECT_FALSE(atu.secondaryAirInletNode());

  // First zone: this is the problematic case
  {
    ThermalZone zone(m);
    EXPECT_FALSE(zone.exhaustPortList().lastModelObject());

    // Now add zone (this was the problematic case)
    EXPECT_TRUE(airLoopHVAC.addBranchForZone(zone));
    ASSERT_TRUE(atu.secondaryAirInletNode());  // <===== Actual test is here
    ASSERT_TRUE(zone.exhaustPortList().lastModelObject());
    EXPECT_EQ(atu.secondaryAirInletNode().get(), zone.exhaustPortList().lastModelObject().get());
  }

  // Should work for any zone added after that too
  {
    ThermalZone zone(m);
    EXPECT_FALSE(zone.exhaustPortList().lastModelObject());

    // Now add zone (this was the problematic case)
    EXPECT_TRUE(airLoopHVAC.addBranchForZone(zone));

    // Get the cloned ATU
    ASSERT_EQ(1u, zone.equipment().size());
    auto _atu = zone.equipment()[0].optionalCast<AirTerminalSingleDuctParallelPIUReheat>();
    ASSERT_TRUE(_atu);

    ASSERT_TRUE(_atu->secondaryAirInletNode());
    ASSERT_TRUE(zone.exhaustPortList().lastModelObject());
    EXPECT_EQ(_atu->secondaryAirInletNode().get(), zone.exhaustPortList().lastModelObject().get());
  }
}
