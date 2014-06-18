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
#include "../DistrictHeating.hpp"
#include "../DistrictHeating_Impl.hpp"
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
TEST_F(ModelFixture,DistrictHeating_DistrictHeating)
{
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  ASSERT_EXIT (
  {
    Model m;
    DistrictHeating districtHeating(m);

    exit(0); 
  } ,
    ::testing::ExitedWithCode(0), "" );
}

//test connecting the object to a loop and get the inlet node and the outlet node
TEST_F(ModelFixture,DistrictHeating_connections)
{
  Model m;

  //make a plant loop
  PlantLoop plantLoop(m);

  //make a districtheating object
  DistrictHeating districtHeating(m);

  //get the supply outlet node of the plant loop
  Node plantOutletNode = plantLoop.supplyOutletNode();

  //hook the districtheating object to the supply outlet node
  ASSERT_TRUE (districtHeating.addToNode(plantOutletNode));

  //it should now be on a loop and have inlet and outlet objects
  ASSERT_TRUE (districtHeating.loop());
  ASSERT_TRUE(districtHeating.inletModelObject() );
  ASSERT_TRUE(districtHeating.outletModelObject() );

  //it should be removable from the loop
  ASSERT_TRUE(districtHeating.isRemovable() );

  //now, disconnect the object
  districtHeating.disconnect();

  //it should no longer have a loop or inlet/outlet objects
  ASSERT_FALSE(districtHeating.loop() );
  ASSERT_FALSE(districtHeating.inletModelObject() );
  ASSERT_FALSE(districtHeating.outletModelObject() );
 
  //make an airloop
  AirLoopHVAC airLoop(m);

  //get the supply outlet node of the airloop
  Node airOutletNode = airLoop.supplyOutletNode();

  //it should not connect to an airloop
  ASSERT_FALSE(districtHeating.addToNode(airOutletNode));
}

//test setting and getting the nominal capacity
TEST_F(ModelFixture,DistrictHeating_NominalCapacity_Quantity) {

  Model m;
  DistrictHeating districtHeating(m);

  //test setting and getting the field with a double
  double testValue(1);

  districtHeating.setNominalCapacity(testValue);

  ASSERT_EQ(1,districtHeating.nominalCapacity());

  //test setting and getting the field with a quantity

  Unit testUnits = districtHeating.getNominalCapacity(true).units(); // Get IP units.

  Quantity testQuantity(testValue,testUnits);

  EXPECT_TRUE(districtHeating.setNominalCapacity(testQuantity));

  Quantity testOutQuantity = districtHeating.getNominalCapacity(true);

  EXPECT_NEAR(testValue,testOutQuantity.value(),1.0E-8);

  EXPECT_EQ(testUnits.standardString(),testOutQuantity.units().standardString());
}

//test cloning the object
TEST_F(ModelFixture,DistrictHeating_Clone){

  Model m;

  //make an object to clone, and edit some property to make sure the clone worked

  DistrictHeating districtHeating(m);

  districtHeating.setNominalCapacity(1234);

  //clone into the same model

  DistrictHeating districtHeatingClone = districtHeating.clone(m).cast<DistrictHeating>();

  ASSERT_EQ(1234,districtHeatingClone.nominalCapacity());

  //clone into another model

  Model m2;

  DistrictHeating districtHeatingClone2 = districtHeating.clone(m2).cast<DistrictHeating>();

  ASSERT_EQ(1234,districtHeatingClone2.nominalCapacity());
}

TEST_F(ModelFixture,DistrictHeating_addToNode) {
  Model m;
  DistrictHeating testObject(m);

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

  DistrictHeating testObjectClone = testObject.clone(m).cast<DistrictHeating>();
  supplyOutletNode = plantLoop.supplyOutletNode();

  EXPECT_TRUE(testObjectClone.addToNode(supplyOutletNode));
  EXPECT_EQ( (unsigned)9, plantLoop.supplyComponents().size() );
}
