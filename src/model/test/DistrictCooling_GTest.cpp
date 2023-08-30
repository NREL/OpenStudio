/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>
#include "ModelFixture.hpp"
#include "../DistrictCooling.hpp"
#include "../DistrictCooling_Impl.hpp"
#include "../AirLoopHVAC.hpp"
#include "../PlantLoop.hpp"
#include "../Node.hpp"
#include "../Node_Impl.hpp"
#include "../AirLoopHVACZoneSplitter.hpp"

using namespace openstudio;
using namespace openstudio::model;

//test construction of the object
TEST_F(ModelFixture, DistrictCooling_DistrictCooling) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  ASSERT_EXIT(
    {
      Model m;
      DistrictCooling testObject(m);

      exit(0);
    },
    ::testing::ExitedWithCode(0), "");

  Model m;
  DistrictCooling districtCooling(m);

  EXPECT_FALSE(districtCooling.capacityFractionSchedule());

  ScheduleConstant scheduleConstant(model);
  scheduleConstant.setValue(0.5);
  EXPECT_TRUE(districtCooling.setCapacityFractionSchedule(scheduleConstant));

  ASSERT_TRUE(districtCooling.capacityFractionSchedule());
  EXPECT_EQ(scheduleConstant, districtCooling.capacityFractionSchedule().get());
  
  districtCooling.resetCapacityFractionSchedule();
  EXPECT_FALSE(districtCooling.capacityFractionSchedule());
}

//test connecting the object to a loop and get the inlet node and the outlet node
TEST_F(ModelFixture, DistrictCooling_connections) {
  Model m;

  //make a plant loop
  PlantLoop plantLoop(m);

  //make a DistrictCooling object
  DistrictCooling testObject(m);

  //get the supply outlet node of the plant loop
  Node plantOutletNode = plantLoop.supplyOutletNode();

  //hook the DistrictCooling object to the supply outlet node
  ASSERT_TRUE(testObject.addToNode(plantOutletNode));

  //it should now be on a loop and have inlet and outlet objects
  ASSERT_TRUE(testObject.loop());
  ASSERT_TRUE(testObject.inletModelObject());
  ASSERT_TRUE(testObject.outletModelObject());

  //it should be removable from the loop
  ASSERT_TRUE(testObject.isRemovable());

  //now, disconnect the object
  testObject.disconnect();

  //it should no longer have a loop or inlet/outlet objects
  ASSERT_FALSE(testObject.loop());
  ASSERT_FALSE(testObject.inletModelObject());
  ASSERT_FALSE(testObject.outletModelObject());

  //make an airloop
  AirLoopHVAC airLoop(m);

  //get the supply outlet node of the airloop
  Node airOutletNode = airLoop.supplyOutletNode();

  //it should not connect to an airloop
  ASSERT_FALSE(testObject.addToNode(airOutletNode));
}

//test setting and getting the nominal capacity
TEST_F(ModelFixture, DistrictCooling_NominalCapacity) {
  Model m;
  DistrictCooling testObject(m);

  //test setting and getting the field with a double
  double testValue(1);
  testObject.setNominalCapacity(testValue);
  auto capacity = testObject.nominalCapacity();
  ASSERT_TRUE(capacity);
  ASSERT_EQ(1, capacity.get());
}

//test cloning the object
TEST_F(ModelFixture, DistrictCooling_Clone) {
  Model m;
  //make an object to clone, and edit some property to make sure the clone worked
  DistrictCooling testObject(m);

  testObject.setNominalCapacity(1234);

  //clone into the same model
  auto testObjectClone = testObject.clone(m).cast<DistrictCooling>();
  auto capacity = testObjectClone.nominalCapacity();
  ASSERT_TRUE(capacity);
  ASSERT_EQ(1234, capacity.get());

  //clone into another model
  Model m2;
  auto testObjectClone2 = testObject.clone(m2).cast<DistrictCooling>();

  capacity = testObjectClone2.nominalCapacity();
  ASSERT_TRUE(capacity);
  ASSERT_EQ(1234, capacity.get());
}

TEST_F(ModelFixture, DistrictCooling_addToNode) {
  Model m;
  DistrictCooling testObject(m);

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

  auto testObjectClone = testObject.clone(m).cast<DistrictCooling>();
  supplyOutletNode = plantLoop.supplyOutletNode();

  EXPECT_TRUE(testObjectClone.addToNode(supplyOutletNode));
  EXPECT_EQ((unsigned)9, plantLoop.supplyComponents().size());
}
