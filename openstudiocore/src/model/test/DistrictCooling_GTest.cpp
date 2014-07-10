/**********************************************************************
 *  Copyright (c) 2008-2014, Alliance for Sustainable Energy.
 *  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 **********************************************************************/

#include <gtest/gtest.h>
#include "ModelFixture.hpp"
#include "../DistrictCooling.hpp"
#include "../DistrictCooling_Impl.hpp"
#include "../../utilities/units/Quantity.hpp"
#include "../../utilities/units/Unit.hpp"
#include "../AirLoopHVAC.hpp"
#include "../PlantLoop.hpp"
#include "../Node.hpp"
#include "../Node_Impl.hpp"
#include "../AirLoopHVACZoneSplitter.hpp"

using namespace openstudio;
using namespace openstudio::model;

//test construction of the object
TEST_F(ModelFixture,DistrictCooling_DistrictCooling)
{
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  ASSERT_EXIT (
  {
    Model m; 
    DistrictCooling testObject(m);

    exit(0); 
  } ,
    ::testing::ExitedWithCode(0), "" );
}

//test connecting the object to a loop and get the inlet node and the outlet node
TEST_F(ModelFixture,DistrictCooling_connections)
{
  Model m;

  //make a plant loop
  PlantLoop plantLoop(m);

  //make a DistrictCooling object
  DistrictCooling testObject(m);

  //get the supply outlet node of the plant loop
  Node plantOutletNode = plantLoop.supplyOutletNode();

  //hook the DistrictCooling object to the supply outlet node
  ASSERT_TRUE (testObject.addToNode(plantOutletNode));

  //it should now be on a loop and have inlet and outlet objects
  ASSERT_TRUE (testObject.loop());
  ASSERT_TRUE(testObject.inletModelObject() );
  ASSERT_TRUE(testObject.outletModelObject() );

  //it should be removable from the loop
  ASSERT_TRUE(testObject.isRemovable() );

  //now, disconnect the object
  testObject.disconnect();

  //it should no longer have a loop or inlet/outlet objects
  ASSERT_FALSE(testObject.loop() );
  ASSERT_FALSE(testObject.inletModelObject() );
  ASSERT_FALSE(testObject.outletModelObject() );

  //make an airloop
  AirLoopHVAC airLoop(m);

  //get the supply outlet node of the airloop
  Node airOutletNode = airLoop.supplyOutletNode();

  //it should not connect to an airloop
  ASSERT_FALSE(testObject.addToNode(airOutletNode));
}

//test setting and getting the nominal capacity
TEST_F(ModelFixture,DistrictCooling_NominalCapacity_Quantity) {
  Model m;
  DistrictCooling testObject(m);

  //test setting and getting the field with a double
  double testValue(1);
  testObject.setNominalCapacity(testValue);
  ASSERT_EQ(1,testObject.nominalCapacity());

  //test setting and getting the field with a quantity
  Unit testUnits = testObject.getNominalCapacity(true).units(); // Get IP units.

  Quantity testQuantity(testValue,testUnits);
  EXPECT_TRUE(testObject.setNominalCapacity(testQuantity));

  Quantity testOutQuantity = testObject.getNominalCapacity(true);
  EXPECT_NEAR(testValue,testOutQuantity.value(),1.0E-8);
  EXPECT_EQ(testUnits.standardString(),testOutQuantity.units().standardString());
}

//test cloning the object
TEST_F(ModelFixture,DistrictCooling_Clone){
  Model m;
  //make an object to clone, and edit some property to make sure the clone worked
  DistrictCooling testObject(m);
  
  testObject.setNominalCapacity(1234);

  //clone into the same model
  DistrictCooling testObjectClone = testObject.clone(m).cast<DistrictCooling>();
  ASSERT_EQ(1234,testObjectClone.nominalCapacity());

  //clone into another model
  Model m2;
  DistrictCooling testObjectClone2 = testObject.clone(m2).cast<DistrictCooling>();

  ASSERT_EQ(1234,testObjectClone2.nominalCapacity());
}

TEST_F(ModelFixture,DistrictCooling_addToNode) {
  Model m;
  DistrictCooling testObject(m);

  AirLoopHVAC airLoop(m);

  Node supplyOutletNode = airLoop.supplyOutletNode();

  EXPECT_FALSE(testObject.addToNode(supplyOutletNode));
  EXPECT_EQ( (unsigned)2, airLoop.supplyComponents().size() );

  Node inletNode = airLoop.zoneSplitter().lastOutletModelObject()->cast<Node>();

  EXPECT_FALSE(testObject.addToNode(inletNode));
  EXPECT_EQ((unsigned)5, airLoop.demandComponents().size());

  PlantLoop plantLoop(m);
  supplyOutletNode = plantLoop.supplyOutletNode();
  EXPECT_TRUE(testObject.addToNode(supplyOutletNode));
  EXPECT_EQ( (unsigned)7, plantLoop.supplyComponents().size() );

  Node demandOutletNode = plantLoop.demandOutletNode();
  EXPECT_FALSE(testObject.addToNode(demandOutletNode));
  EXPECT_EQ( (unsigned)5, plantLoop.demandComponents().size() );

  DistrictCooling testObjectClone = testObject.clone(m).cast<DistrictCooling>();
  supplyOutletNode = plantLoop.supplyOutletNode();

  EXPECT_TRUE(testObjectClone.addToNode(supplyOutletNode));
  EXPECT_EQ( (unsigned)9, plantLoop.supplyComponents().size() );
}
