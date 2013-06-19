/**********************************************************************
 *  Copyright (c) 2008-2013, Alliance for Sustainable Energy.
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
#include <model/Model.hpp>
#include <model/test/ModelFixture.hpp>
#include <model/Node.hpp>
#include <model/Node_Impl.hpp>
#include <model/DistrictCooling.hpp>
#include <model/DistrictCooling_Impl.hpp>
#include <utilities/units/Quantity.hpp>
#include <utilities/units/Unit.hpp>
#include <model/PlantLoop.hpp>
#include <model/PlantLoop_Impl.hpp>


//test construction of the object
TEST(DistrictCooling,DistrictCooling_DistrictCooling)
{
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  ASSERT_EXIT (
  {
    openstudio::model::Model m; 

    openstudio::model::DistrictCooling DistrictCooling(m);

    exit(0); 
  } ,
    ::testing::ExitedWithCode(0), "" );
}

//test connecting the object to a loop and get the inlet node and the outlet node
TEST(DistrictCooling,DistrictCooling_connections)
{
  openstudio::model::Model m;

  //make a plant loop
  openstudio::model::PlantLoop plantLoop(m);

  //make a DistrictCooling object
  openstudio::model::DistrictCooling DistrictCooling(m);

  //get the supply outlet node of the plant loop
  openstudio::model::Node plantOutletNode = plantLoop.supplyOutletNode();

  //hook the DistrictCooling object to the supply outlet node
  ASSERT_TRUE (DistrictCooling.addToNode(plantOutletNode));

  //it should now be on a loop and have inlet and outlet objects
  ASSERT_TRUE (DistrictCooling.loop());
  ASSERT_TRUE(DistrictCooling.inletModelObject() );
  ASSERT_TRUE(DistrictCooling.outletModelObject() );

  //it should be removable from the loop
  ASSERT_TRUE(DistrictCooling.isRemovable() );

  //now, disconnect the object
  DistrictCooling.disconnect();

  //it should no longer have a loop or inlet/outlet objects
  ASSERT_FALSE(DistrictCooling.loop() );
  ASSERT_FALSE(DistrictCooling.inletModelObject() );
  ASSERT_FALSE(DistrictCooling.outletModelObject() );

  //make an airloop
  openstudio::model::AirLoopHVAC airLoop(m);

  //get the supply outlet node of the airloop
  openstudio::model::Node airOutletNode = airLoop.supplyOutletNode();

  //it should not connect to an airloop
  ASSERT_FALSE(DistrictCooling.addToNode(airOutletNode));

}

//test setting and getting the nominal capacity
TEST_F(ModelFixture,DistrictCooling_NominalCapacity_Quantity) {

  openstudio::model::Model m;

  openstudio::model::DistrictCooling DistrictCooling(m);

  //test setting and getting the field with a double
  double testValue(1);

  DistrictCooling.setNominalCapacity(testValue);

  ASSERT_EQ(1,DistrictCooling.nominalCapacity());

  //test setting and getting the field with a quantity

  openstudio::Unit testUnits = DistrictCooling.getNominalCapacity(true).units(); // Get IP units.

  openstudio::Quantity testQuantity(testValue,testUnits);

  EXPECT_TRUE(DistrictCooling.setNominalCapacity(testQuantity));

  openstudio::Quantity testOutQuantity = DistrictCooling.getNominalCapacity(true);

  EXPECT_NEAR(testValue,testOutQuantity.value(),1.0E-8);

  EXPECT_EQ(testUnits.standardString(),testOutQuantity.units().standardString());

}

//test cloning the object
TEST(DistrictCooling,DistrictCooling_Clone){

  openstudio::model::Model m;

  //make an object to clone, and edit some property to make sure the clone worked

  openstudio::model::DistrictCooling DistrictCooling(m);

  DistrictCooling.setNominalCapacity(1234);

  //clone into the same model
  
  openstudio::model::DistrictCooling DistrictCoolingClone = DistrictCooling.clone(m).cast<openstudio::model::DistrictCooling>();

  ASSERT_EQ(1234,DistrictCoolingClone.nominalCapacity());

  //clone into another model

  openstudio::model::Model m2;

  openstudio::model::DistrictCooling DistrictCoolingClone2 = DistrictCooling.clone(m2).cast<openstudio::model::DistrictCooling>();

  ASSERT_EQ(1234,DistrictCoolingClone2.nominalCapacity());


}