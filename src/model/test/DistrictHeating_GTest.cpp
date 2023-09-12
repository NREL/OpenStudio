/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>
#include "ModelFixture.hpp"
#include "../DistrictHeating.hpp"
#include "../DistrictHeating_Impl.hpp"
#include "../AirLoopHVAC.hpp"
#include "../PlantLoop.hpp"
#include "../Node.hpp"
#include "../Node_Impl.hpp"
#include "../AirLoopHVACZoneSplitter.hpp"
#include "../Schedule.hpp"
#include "../Schedule_Impl.hpp"
#include "../ScheduleConstant.hpp"
#include "../ScheduleConstant_Impl.hpp"

using namespace openstudio;
using namespace openstudio::model;

//test construction of the object
TEST_F(ModelFixture, DistrictHeating_DistrictHeating) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  ASSERT_EXIT(
    {
      Model m;
      DistrictHeating districtHeating(m);

      exit(0);
    },
    ::testing::ExitedWithCode(0), "");

  Model m;
  DistrictHeating districtHeating(m);

  EXPECT_FALSE(districtHeating.nominalCapacity());
  EXPECT_TRUE(districtHeating.isNominalCapacityAutosized());
  EXPECT_FALSE(districtHeating.capacityFractionSchedule());

  ScheduleConstant scheduleConstant(m);
  scheduleConstant.setValue(0.5);
  EXPECT_TRUE(districtHeating.setCapacityFractionSchedule(scheduleConstant));

  ASSERT_TRUE(districtHeating.capacityFractionSchedule());
  EXPECT_EQ(scheduleConstant, districtHeating.capacityFractionSchedule().get());

  districtHeating.resetCapacityFractionSchedule();
  EXPECT_FALSE(districtHeating.capacityFractionSchedule());
}

//test connecting the object to a loop and get the inlet node and the outlet node
TEST_F(ModelFixture, DistrictHeating_connections) {
  Model m;

  //make a plant loop
  PlantLoop plantLoop(m);

  //make a districtheating object
  DistrictHeating districtHeating(m);

  //get the supply outlet node of the plant loop
  Node plantOutletNode = plantLoop.supplyOutletNode();

  //hook the districtheating object to the supply outlet node
  ASSERT_TRUE(districtHeating.addToNode(plantOutletNode));

  //it should now be on a loop and have inlet and outlet objects
  ASSERT_TRUE(districtHeating.loop());
  ASSERT_TRUE(districtHeating.inletModelObject());
  ASSERT_TRUE(districtHeating.outletModelObject());

  //it should be removable from the loop
  ASSERT_TRUE(districtHeating.isRemovable());

  //now, disconnect the object
  districtHeating.disconnect();

  //it should no longer have a loop or inlet/outlet objects
  ASSERT_FALSE(districtHeating.loop());
  ASSERT_FALSE(districtHeating.inletModelObject());
  ASSERT_FALSE(districtHeating.outletModelObject());

  //make an airloop
  AirLoopHVAC airLoop(m);

  //get the supply outlet node of the airloop
  Node airOutletNode = airLoop.supplyOutletNode();

  //it should not connect to an airloop
  ASSERT_FALSE(districtHeating.addToNode(airOutletNode));
}

//test setting and getting the nominal capacity
TEST_F(ModelFixture, DistrictHeating_NominalCapacity) {

  Model m;
  DistrictHeating districtHeating(m);

  //test setting and getting the field with a double
  double testValue(1);

  districtHeating.setNominalCapacity(testValue);

  auto capacity = districtHeating.nominalCapacity();
  ASSERT_TRUE(capacity);
  ASSERT_EQ(1, capacity.get());

  EXPECT_FALSE(districtHeating.isNominalCapacityAutosized());
  districtHeating.autosizeNominalCapacity();
  EXPECT_TRUE(districtHeating.isNominalCapacityAutosized());

  //test cloning the object
  TEST_F(ModelFixture, DistrictHeating_Clone) {

    Model m;

    //make an object to clone, and edit some property to make sure the clone worked

    DistrictHeating districtHeating(m);

    districtHeating.setNominalCapacity(1234);

    //clone into the same model

    auto districtHeatingClone = districtHeating.clone(m).cast<DistrictHeating>();

    auto capacity = districtHeatingClone.nominalCapacity();
    ASSERT_TRUE(capacity);
    ASSERT_EQ(1234, capacity.get());

    //clone into another model

    Model m2;

    auto districtHeatingClone2 = districtHeating.clone(m2).cast<DistrictHeating>();

    capacity = districtHeatingClone2.nominalCapacity();
    ASSERT_TRUE(capacity);
    ASSERT_EQ(1234, capacity.get());
  }

  TEST_F(ModelFixture, DistrictHeating_addToNode) {
    Model m;
    DistrictHeating testObject(m);

    AirLoopHVAC airLoop(m);

    Node supplyOutletNode = airLoop.supplyOutletNode();

    EXPECT_FALSE(testObject.addToNode(supplyOutletNode));
    EXPECT_EQ((unsigned)2, airLoop.supplyComponents().size());

    Node inletNode = airLoop.zoneSplitter().lastOutletModelObject()->cast<Node>();

    EXPECT_FALSE(testObject.addToNode(inletNode));
    EXPECT_EQ((unsigned)5, airLoop.demandComponents().size());

    PlantLoop plantLoop(m);
    supplyOutletNode = plantLoop.supplyOutletNode();
    EXPECT_TRUE(testObject.addToNode(supplyOutletNode));
    EXPECT_EQ((unsigned)7, plantLoop.supplyComponents().size());

    Node demandOutletNode = plantLoop.demandOutletNode();
    EXPECT_FALSE(testObject.addToNode(demandOutletNode));
    EXPECT_EQ((unsigned)5, plantLoop.demandComponents().size());

    auto testObjectClone = testObject.clone(m).cast<DistrictHeating>();
    supplyOutletNode = plantLoop.supplyOutletNode();

    EXPECT_TRUE(testObjectClone.addToNode(supplyOutletNode));
    EXPECT_EQ((unsigned)9, plantLoop.supplyComponents().size());
  }
