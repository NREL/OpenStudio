/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>
#include "ModelFixture.hpp"
#include "../AirTerminalSingleDuctSeriesPIUReheat.hpp"
#include "../AirTerminalSingleDuctSeriesPIUReheat_Impl.hpp"
#include "../AirLoopHVAC.hpp"
#include "../AirLoopHVAC_Impl.hpp"
#include "../Schedule.hpp"
#include "../Schedule_Impl.hpp"
#include "../FanConstantVolume.hpp"
#include "../FanConstantVolume_Impl.hpp"
#include "../FanSystemModel.hpp"
#include "../FanSystemModel_Impl.hpp"
#include "../CoilHeatingElectric.hpp"
#include "../CoilHeatingElectric_Impl.hpp"
#include "../ScheduleRuleset.hpp"
#include "../ScheduleRuleset_Impl.hpp"
#include "../ThermalZone.hpp"
#include "../ThermalZone_Impl.hpp"
#include "../PortList.hpp"
#include "../PortList_Impl.hpp"
#include "../Node.hpp"
#include "../Node_Impl.hpp"
#include "../Model.hpp"
#include "../Model_Impl.hpp"

#include "../../utilities/idf/IdfExtensibleGroup.hpp"
#include "../ModelExtensibleGroup.hpp"
#include <utilities/idd/OS_PortList_FieldEnums.hxx>

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, AirTerminalSingleDuctSeriesPIUReheat) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  ASSERT_EXIT(
    {
      Model m;
      Schedule schedule = m.alwaysOnDiscreteSchedule();
      FanConstantVolume fan(m, schedule);
      CoilHeatingElectric coil(m, schedule);
      AirTerminalSingleDuctSeriesPIUReheat terminal(m, fan, coil);

      exit(0);
    },
    ::testing::ExitedWithCode(0), "");

  {
    Model m;
    Schedule schedule = m.alwaysOnDiscreteSchedule();
    FanConstantVolume fan(m, schedule);
    CoilHeatingElectric coil(m, schedule);
    AirTerminalSingleDuctSeriesPIUReheat terminal(m, fan, coil);

    EXPECT_NO_THROW(terminal.reheatCoil());
    EXPECT_NO_THROW(terminal.fan());

    ThermalZone zone(m);
    AirLoopHVAC airSystem(m);

    EXPECT_TRUE(airSystem.addBranchForZone(zone, terminal));
  }

  {
    Model m;
    Schedule schedule = m.alwaysOnDiscreteSchedule();
    FanConstantVolume fan(m, schedule);
    CoilHeatingElectric coil(m, schedule);
    AirTerminalSingleDuctSeriesPIUReheat terminal(m, fan, coil);

    terminal.reheatCoil();
    terminal.fan();

    auto terminalClone = terminal.clone(m).cast<AirTerminalSingleDuctSeriesPIUReheat>();
    ASSERT_NO_THROW(terminalClone.reheatCoil());
    ASSERT_NO_THROW(terminalClone.fan());

    EXPECT_NE(terminalClone.reheatCoil(), terminal.reheatCoil());
    EXPECT_NE(terminalClone.fan(), terminal.fan());
  }

  // test that setAvailabilitySchedule also set PIU fan schedule
  {
    Model m;
    Schedule schedule = m.alwaysOnDiscreteSchedule();
    FanConstantVolume fan(m, schedule);
    CoilHeatingElectric coil(m, schedule);
    AirTerminalSingleDuctSeriesPIUReheat terminal(m, fan, coil);

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
    AirTerminalSingleDuctSeriesPIUReheat terminal(m, fan, coil);

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

    auto zoneImpl = zone.getImpl<model::detail::ThermalZone_Impl>();
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

TEST_F(ModelFixture, AirTerminalSingleDuctSeriesPIUReheat_connectSecondaryAirInlet_regularCase_2033) {

  // Test for #2033
  // Base case: works fine
  Model m;
  Schedule schedule = m.alwaysOnDiscreteSchedule();
  FanConstantVolume fan(m, schedule);
  CoilHeatingElectric coil(m, schedule);
  AirTerminalSingleDuctSeriesPIUReheat atu(m, fan, coil);

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

TEST_F(ModelFixture, AirTerminalSingleDuctSeriesPIUReheat_connectSecondaryAirInlet_atuFirst_2033) {

  // Test for #2033: When you connect the atu first, then add a zone it should work as well.
  Model m;
  Schedule schedule = m.alwaysOnDiscreteSchedule();
  FanConstantVolume fan(m, schedule);
  CoilHeatingElectric coil(m, schedule);
  AirTerminalSingleDuctSeriesPIUReheat atu(m, fan, coil);

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
    auto _atu = zone.equipment()[0].optionalCast<AirTerminalSingleDuctSeriesPIUReheat>();
    ASSERT_TRUE(_atu);

    ASSERT_TRUE(_atu->secondaryAirInletNode());
    ASSERT_TRUE(zone.exhaustPortList().lastModelObject());
    EXPECT_EQ(_atu->secondaryAirInletNode().get(), zone.exhaustPortList().lastModelObject().get());
  }
}

TEST_F(ModelFixture, AirTerminalSingleDuctSeriesPIUReheat_fanControl) {
  Model m;
  Schedule schedule = m.alwaysOnDiscreteSchedule();
  FanConstantVolume fan(m, schedule);
  CoilHeatingElectric coil(m, schedule);
  AirTerminalSingleDuctSeriesPIUReheat atu(m, fan, coil);

  EXPECT_EQ("ConstantSpeed", atu.fanControlType());
  EXPECT_FALSE(atu.setFanControlType("VariableSpeed"));
  EXPECT_EQ("ConstantSpeed", atu.fanControlType());

  FanSystemModel fan2(m);
  EXPECT_TRUE(atu.setFan(fan2));
  EXPECT_EQ("ConstantSpeed", atu.fanControlType());
  EXPECT_TRUE(atu.setFanControlType("VariableSpeed"));
  EXPECT_EQ("VariableSpeed", atu.fanControlType());

  EXPECT_TRUE(atu.setFan(fan));
  EXPECT_EQ("ConstantSpeed", atu.fanControlType());

  EXPECT_EQ(0.3, atu.minimumFanTurnDownRatio());
  EXPECT_EQ("Staged", atu.heatingControlType());
  EXPECT_EQ(32.1, atu.designHeatingDischargeAirTemperature());
  EXPECT_EQ(37.7, atu.highLimitHeatingDischargeAirTemperature());

  EXPECT_TRUE(atu.setMinimumFanTurnDownRatio(0.4));
  EXPECT_TRUE(atu.setHeatingControlType("Modulated"));
  EXPECT_TRUE(atu.setDesignHeatingDischargeAirTemperature(33.0));
  EXPECT_TRUE(atu.setHighLimitHeatingDischargeAirTemperature(38.0));

  EXPECT_EQ(0.4, atu.minimumFanTurnDownRatio());
  EXPECT_EQ("Modulated", atu.heatingControlType());
  EXPECT_EQ(33.0, atu.designHeatingDischargeAirTemperature());
  EXPECT_EQ(38.0, atu.highLimitHeatingDischargeAirTemperature());
}
