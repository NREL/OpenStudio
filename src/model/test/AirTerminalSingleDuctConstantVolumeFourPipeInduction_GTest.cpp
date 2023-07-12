/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>
#include "ModelFixture.hpp"
#include "../AirTerminalSingleDuctConstantVolumeFourPipeInduction.hpp"
#include "../AirTerminalSingleDuctConstantVolumeFourPipeInduction_Impl.hpp"
#include "../CoilHeatingWater.hpp"
#include "../CoilCoolingWater.hpp"
#include "../Schedule.hpp"
#include "../AirLoopHVAC.hpp"
#include "../PlantLoop.hpp"
#include "../ThermalZone.hpp"
#include "../ThermalZone_Impl.hpp"
#include "../PortList.hpp"
#include "../PortList_Impl.hpp"
#include "../Node.hpp"
#include "../Node_Impl.hpp"
#include "../AirLoopHVACZoneSplitter.hpp"

#include "../../utilities/idf/IdfExtensibleGroup.hpp"
#include "../ModelExtensibleGroup.hpp"
#include <utilities/idd/OS_PortList_FieldEnums.hxx>

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, AirTerminalSingleDuctConstantVolumeFourPipeInduction_AirTerminalSingleDuctConstantVolumeFourPipeInduction) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  ASSERT_EXIT(
    {
      Model m;
      Schedule s = m.alwaysOnDiscreteSchedule();
      CoilHeatingWater heatingCoil(m, s);
      AirTerminalSingleDuctConstantVolumeFourPipeInduction testObject(m, heatingCoil);

      exit(0);
    },
    ::testing::ExitedWithCode(0), "");
}

TEST_F(ModelFixture, AirTerminalSingleDuctConstantVolumeFourPipeInduction_addToNode) {
  Model m;
  Schedule s = m.alwaysOnDiscreteSchedule();
  CoilHeatingWater heatingCoil(m, s);
  AirTerminalSingleDuctConstantVolumeFourPipeInduction testObject(m, heatingCoil);

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

  auto testObjectClone = testObject.clone(m).cast<AirTerminalSingleDuctConstantVolumeFourPipeInduction>();
  inletNode = airLoop.zoneSplitter().lastOutletModelObject()->cast<Node>();

  EXPECT_FALSE(testObjectClone.addToNode(inletNode));
  EXPECT_TRUE(airLoop.addBranchForHVACComponent(testObjectClone));
  EXPECT_EQ((unsigned)10, airLoop.demandComponents().size());
}

TEST_F(ModelFixture, AirTerminalSingleDuctConstantVolumeFourPipeInduction_remove) {
  Model m;
  Schedule s = m.alwaysOnDiscreteSchedule();
  CoilHeatingWater heatingCoil(m, s);
  CoilCoolingWater coolingCoil(m, s);
  AirTerminalSingleDuctConstantVolumeFourPipeInduction testObject(m, heatingCoil);

  testObject.setCoolingCoil(coolingCoil);

  AirLoopHVAC airLoop(m);
  ThermalZone thermalZone(m);
  PlantLoop plantLoop(m);

  // KSB: I don't think it is the greatest idea to test these private methods,
  // but this area has resulted in a simulation error so it needs to be tested
  // (Note: JM 2021-03-02 These aren't private anymore)
  EXPECT_EQ(0u, thermalZone.exhaustPortList().numExtensibleGroups());
  EXPECT_EQ(0u, thermalZone.inletPortList().numExtensibleGroups());

  airLoop.addBranchForZone(thermalZone, testObject);
  plantLoop.addDemandBranchForComponent(heatingCoil);
  plantLoop.addDemandBranchForComponent(coolingCoil);

  ASSERT_EQ(1u, thermalZone.exhaustPortList().numExtensibleGroups());
  EXPECT_TRUE(thermalZone.exhaustPortList().extensibleGroups()[0].cast<ModelExtensibleGroup>().getTarget(OS_PortListExtensibleFields::Port));
  ASSERT_EQ(1u, thermalZone.inletPortList().numExtensibleGroups());
  EXPECT_TRUE(thermalZone.inletPortList().extensibleGroups()[0].cast<ModelExtensibleGroup>().getTarget(OS_PortListExtensibleFields::Port));

  EXPECT_EQ((unsigned)10, plantLoop.demandComponents().size());
  EXPECT_EQ((unsigned)9, airLoop.demandComponents().size());

  testObject.remove();

  EXPECT_EQ(0u, thermalZone.exhaustPortList().numExtensibleGroups());
  ASSERT_EQ(1u, thermalZone.inletPortList().numExtensibleGroups());
  EXPECT_TRUE(thermalZone.inletPortList().extensibleGroups()[0].cast<ModelExtensibleGroup>().getTarget(OS_PortListExtensibleFields::Port));

  EXPECT_EQ((unsigned)5, plantLoop.demandComponents().size());
  EXPECT_EQ((unsigned)7, airLoop.demandComponents().size());
}

TEST_F(ModelFixture, AirTerminalSingleDuctConstantVolumeFourPipeInduction_clone) {
  Model m;
  Schedule s = m.alwaysOnDiscreteSchedule();
  CoilHeatingWater heatingCoil(m, s);
  CoilCoolingWater coolingCoil(m, s);
  AirTerminalSingleDuctConstantVolumeFourPipeInduction testObject(m, heatingCoil);

  testObject.setCoolingCoil(coolingCoil);

  AirLoopHVAC airLoop(m);
  ThermalZone thermalZone(m);
  PlantLoop plantLoop(m);

  airLoop.addBranchForZone(thermalZone, testObject);
  plantLoop.addDemandBranchForComponent(heatingCoil);
  plantLoop.addDemandBranchForComponent(coolingCoil);

  auto testObjectClone = testObject.clone(m).cast<AirTerminalSingleDuctConstantVolumeFourPipeInduction>();
  ASSERT_NO_THROW(testObjectClone.heatingCoil());
  ASSERT_TRUE(testObjectClone.coolingCoil());
  EXPECT_FALSE(testObjectClone.airLoopHVAC());

  EXPECT_NE(testObjectClone.heatingCoil(), testObject.heatingCoil());
  EXPECT_NE(testObjectClone.coolingCoil().get(), testObject.coolingCoil().get());

  Model m2;

  auto testObjectClone2 = testObject.clone(m2).cast<AirTerminalSingleDuctConstantVolumeFourPipeInduction>();
  ASSERT_NO_THROW(testObjectClone2.heatingCoil());
  ASSERT_TRUE(testObjectClone2.coolingCoil());
  EXPECT_FALSE(testObjectClone.airLoopHVAC());

  EXPECT_NE(testObjectClone2.heatingCoil(), testObject.heatingCoil());
  EXPECT_NE(testObjectClone2.coolingCoil().get(), testObject.coolingCoil().get());
}

TEST_F(ModelFixture, AirTerminalSingleDuctConstantVolumeFourPipeInduction_connectSecondaryAirInlet_regularCase_2033) {

  // Test for #2033
  // Base case: works fine
  Model m;
  Schedule s = m.alwaysOnDiscreteSchedule();
  CoilHeatingWater heatingCoil(m, s);
  AirTerminalSingleDuctConstantVolumeFourPipeInduction atu(m, heatingCoil);

  ThermalZone zone(m);
  AirLoopHVAC airLoopHVAC(m);

  EXPECT_FALSE(atu.inducedAirInletNode());
  EXPECT_FALSE(zone.exhaustPortList().lastModelObject());

  // Connect simulateanously the branch and atu
  EXPECT_TRUE(airLoopHVAC.addBranchForZone(zone, atu));

  ASSERT_TRUE(atu.inducedAirInletNode());
  ASSERT_TRUE(zone.exhaustPortList().lastModelObject());
  EXPECT_EQ(atu.inducedAirInletNode().get(), zone.exhaustPortList().lastModelObject().get());
}

TEST_F(ModelFixture, AirTerminalSingleDuctConstantVolumeFourPipeInduction_connectSecondaryAirInlet_atuFirst_2033) {

  // Test for #2033: When you connect the atu first, then add a zone it should work as well.
  Model m;
  Schedule s = m.alwaysOnDiscreteSchedule();
  CoilHeatingWater heatingCoil(m, s);
  AirTerminalSingleDuctConstantVolumeFourPipeInduction atu(m, heatingCoil);
  AirLoopHVAC airLoopHVAC(m);

  // Connect atu only first
  airLoopHVAC.addBranchForHVACComponent(atu);
  EXPECT_FALSE(atu.inducedAirInletNode());

  // First zone: this is the problematic case
  {
    ThermalZone zone(m);
    EXPECT_FALSE(zone.exhaustPortList().lastModelObject());

    // Now add zone (this was the problematic case)
    EXPECT_TRUE(airLoopHVAC.addBranchForZone(zone));
    ASSERT_TRUE(atu.inducedAirInletNode());  // <===== Actual test is here
    ASSERT_TRUE(zone.exhaustPortList().lastModelObject());
    EXPECT_EQ(atu.inducedAirInletNode().get(), zone.exhaustPortList().lastModelObject().get());
  }

  // Should work for any zone added after that too
  {
    ThermalZone zone(m);
    EXPECT_FALSE(zone.exhaustPortList().lastModelObject());

    // Now add zone (this was the problematic case)
    EXPECT_TRUE(airLoopHVAC.addBranchForZone(zone));

    // Get the cloned ATU
    ASSERT_EQ(1u, zone.equipment().size());
    auto _atu = zone.equipment()[0].optionalCast<AirTerminalSingleDuctConstantVolumeFourPipeInduction>();
    ASSERT_TRUE(_atu);

    ASSERT_TRUE(_atu->inducedAirInletNode());
    ASSERT_TRUE(zone.exhaustPortList().lastModelObject());
    EXPECT_EQ(_atu->inducedAirInletNode().get(), zone.exhaustPortList().lastModelObject().get());
  }
}
