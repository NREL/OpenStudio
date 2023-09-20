/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>
#include "ModelFixture.hpp"
#include "../DistrictHeatingSteam.hpp"
#include "../DistrictHeatingSteam_Impl.hpp"
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
TEST_F(ModelFixture, DistrictHeatingSteam_GettersSetters) {

  Model m;
  DistrictHeatingSteam districtHeatingSteam(m);

  // Default to autosize
  EXPECT_FALSE(districtHeatingSteam.nominalCapacity());
  EXPECT_TRUE(districtHeatingSteam.isNominalCapacityAutosized());
  // Set it
  districtHeatingSteam.setNominalCapacity(1000.0);
  ASSERT_TRUE(districtHeatingSteam.nominalCapacity());
  EXPECT_EQ(1000.0, districtHeatingSteam.nominalCapacity().get());
  EXPECT_FALSE(districtHeatingSteam.isNominalCapacityAutosized());
  // autosize
  districtHeatingSteam.autosizeNominalCapacity();
  EXPECT_TRUE(districtHeatingSteam.isNominalCapacityAutosized());
  EXPECT_FALSE(districtHeatingSteam.nominalCapacity());

  EXPECT_EQ(m.alwaysOnContinuousSchedule(), districtHeatingSteam.capacityFractionSchedule());
  ScheduleConstant scheduleConstant(m);
  EXPECT_TRUE(districtHeatingSteam.setCapacityFractionSchedule(scheduleConstant));
  EXPECT_EQ(scheduleConstant, districtHeatingSteam.capacityFractionSchedule());
}

//test connecting the object to a loop and get the inlet node and the outlet node
TEST_F(ModelFixture, DistrictHeatingSteam_connections) {
  Model m;

  //make a plant loop
  PlantLoop plantLoop(m);

  //make a districtHeatingSteam object
  DistrictHeatingSteam districtHeatingSteam(m);

  //get the supply outlet node of the plant loop
  Node plantOutletNode = plantLoop.supplyOutletNode();

  //hook the districtHeatingSteam object to the supply outlet node
  ASSERT_TRUE(districtHeatingSteam.addToNode(plantOutletNode));

  //it should now be on a loop and have inlet and outlet objects
  ASSERT_TRUE(districtHeatingSteam.loop());
  ASSERT_TRUE(districtHeatingSteam.inletModelObject());
  ASSERT_TRUE(districtHeatingSteam.outletModelObject());

  //it should be removable from the loop
  ASSERT_TRUE(districtHeatingSteam.isRemovable());

  //now, disconnect the object
  districtHeatingSteam.disconnect();

  //it should no longer have a loop or inlet/outlet objects
  ASSERT_FALSE(districtHeatingSteam.loop());
  ASSERT_FALSE(districtHeatingSteam.inletModelObject());
  ASSERT_FALSE(districtHeatingSteam.outletModelObject());

  //make an airloop
  AirLoopHVAC airLoop(m);

  //get the supply outlet node of the airloop
  Node airOutletNode = airLoop.supplyOutletNode();

  //it should not connect to an airloop
  ASSERT_FALSE(districtHeatingSteam.addToNode(airOutletNode));
}

//test cloning the object
TEST_F(ModelFixture, DistrictHeatingSteam_Clone) {

  Model m;

  //make an object to clone, and edit some property to make sure the clone worked

  DistrictHeatingSteam districtHeatingSteam(m);

  districtHeatingSteam.setNominalCapacity(1234);

  //clone into the same model

  auto districtHeatingClone = districtHeatingSteam.clone(m).cast<DistrictHeatingSteam>();

  auto capacity = districtHeatingClone.nominalCapacity();
  ASSERT_TRUE(capacity);
  ASSERT_EQ(1234, capacity.get());

  //clone into another model

  Model m2;

  auto districtHeatingClone2 = districtHeatingSteam.clone(m2).cast<DistrictHeatingSteam>();

  capacity = districtHeatingClone2.nominalCapacity();
  ASSERT_TRUE(capacity);
  ASSERT_EQ(1234, capacity.get());
}

TEST_F(ModelFixture, DistrictHeatingSteam_addToNode) {
  Model m;
  DistrictHeatingSteam testObject(m);

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

  auto testObjectClone = testObject.clone(m).cast<DistrictHeatingSteam>();
  supplyOutletNode = plantLoop.supplyOutletNode();

  EXPECT_TRUE(testObjectClone.addToNode(supplyOutletNode));
  EXPECT_EQ((unsigned)9, plantLoop.supplyComponents().size());
}
