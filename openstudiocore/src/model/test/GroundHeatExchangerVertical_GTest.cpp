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

#include <model/test/ModelFixture.hpp>

#include <model/GroundHeatExchangerVertical.hpp>
#include <model/GroundHeatExchangerVertical_Impl.hpp>
#include <model/Node.hpp>
#include <model/Node_Impl.hpp>

#include <utilities/units/Quantity.hpp>
#include <utilities/units/Unit.hpp>

using namespace openstudio;
using namespace openstudio::model;

TEST(GroundHeatExchangerVertical,GroundHeatExchangerVertical_connections)
{
  model::Model m; 
  
  model::GroundHeatExchangerVertical groundHeatExchanger(m); 

  model::Node inletNode(m);

  model::Node outletNode(m);

  m.connect(inletNode,inletNode.outletPort(),groundHeatExchanger,groundHeatExchanger.inletPort());
  m.connect(groundHeatExchanger,groundHeatExchanger.outletPort(),outletNode,outletNode.inletPort());

  ASSERT_TRUE( groundHeatExchanger.inletModelObject() );
  ASSERT_TRUE( groundHeatExchanger.outletModelObject() );
 
  EXPECT_EQ( inletNode.handle(), groundHeatExchanger.inletModelObject()->handle() );
  EXPECT_EQ( outletNode.handle(), groundHeatExchanger.outletModelObject()->handle() );
}

TEST_F(ModelFixture,GroundHeatExchangerVertical_addToNode)
{
  Model model; 
  GroundHeatExchangerVertical groundHeatExchangerVertical(model);

//  AirLoopHVAC airLoop(model);
  PlantLoop plantLoop(model);

//  Node supplyOutletNode = airLoop.supplyOutletNode();
  Node supplyOutletNode = plantLoop.supplyOutletNode();

  EXPECT_TRUE(groundHeatExchangerVertical.addToNode(supplyOutletNode));

  ASSERT_EQ( (unsigned)3, plantLoop.supplyComponents().size() );
				
  // inlet and outlet ports
  EXPECT_TRUE(groundHeatExchangerVertical.inletPort());
  EXPECT_TRUE(groundHeatExchangerVertical.outletPort());

}

//test cloning the object
TEST(GroundHeatExchangerVertical,GroundHeatExchangerVertical_Clone){

  openstudio::model::Model m;

  //make an object to clone, and edit some property to make sure the clone worked

  openstudio::model::GroundHeatExchangerVertical GroundHeatExchangerVertical(m);

  GroundHeatExchangerVertical.setMaximumFlowRate(3.14);

  //clone into the same model
  
  openstudio::model::GroundHeatExchangerVertical GroundHeatExchangerVerticalClone = GroundHeatExchangerVertical.clone(m).cast<openstudio::model::GroundHeatExchangerVertical>();

  ASSERT_EQ(3.14,GroundHeatExchangerVerticalClone.maximumFlowRate());

  //clone into another model

  openstudio::model::Model m2;

  openstudio::model::GroundHeatExchangerVertical GroundHeatExchangerVerticalClone2 = GroundHeatExchangerVertical.clone(m2).cast<openstudio::model::GroundHeatExchangerVertical>();

  ASSERT_EQ(3.14,GroundHeatExchangerVerticalClone2.maximumFlowRate());

}

TEST_F(ModelFixture,GroundHeatExchangerVertical_Test_Setters_and_Getters)
{
  Model model;
  // TODO: Check constructor.
  GroundHeatExchangerVertical groundHeatExchangerVertical(model);

  // Maximum Flow Rate
				
  EXPECT_TRUE(groundHeatExchangerVertical.setMaximumFlowRate(0.0033));
  boost::optional<double> testMaximumFlowRate = groundHeatExchangerVertical.maximumFlowRate();
				 
  EXPECT_EQ((*testMaximumFlowRate),0.0033);
//  EXPECT_FALSE(groundHeatExchangerVertical.isMaximumFlowRateDefaulted());
				 
  groundHeatExchangerVertical.resetMaximumFlowRate();
//  EXPECT_TRUE(groundHeatExchanger.isMaximumFlowRateDefaulted());

}

//TEST_F(ModelFixture,GroundHeatExchangerVertical_MaximumFlowRate_Quantity) {
//  Model model;
//  // TODO: Check constructor.
//  GroundHeatExchangerVertical groundHeatExchangerVertical(model);
//
//  Unit units = groundHeatExchangerVertical.getMaximumFlowRate(true).units(); // Get IP units.
//  // TODO: Check that value is appropriate (within bounds)
//  double value(1.0);
//  Quantity testQ(value,units);
//  EXPECT_TRUE(groundHeatExchangerVertical.setMaximumFlowRate(testQ));
//  OSOptionalQuantity q = groundHeatExchangerVertical.getMaximumFlowRate(true);
//  ASSERT_TRUE(q.isSet());
//  EXPECT_NEAR(value,q.get().value(),1.0E-8);
//  EXPECT_EQ(units.standardString(),q.units().standardString());
//}

//TEST_F(ModelFixture,GroundHeatExchangerVertical_BoreHoleLength_Quantity) {
//  Model model;
//  // TODO: Check constructor.
//  GroundHeatExchangerVertical groundHeatExchangerVertical(model);
//
//  Unit units = groundHeatExchangerVertical.getBoreHoleLength(true).units(); // Get IP units.
//  // TODO: Check that value is appropriate (within bounds)
//  double value(1.0);
//  Quantity testQ(value,units);
//  EXPECT_TRUE(groundHeatExchangerVertical.setBoreHoleLength(testQ));
//  OSOptionalQuantity q = groundHeatExchangerVertical.getBoreHoleLength(true);
//  ASSERT_TRUE(q.isSet());
//  EXPECT_NEAR(value,q.get().value(),1.0E-8);
//  EXPECT_EQ(units.standardString(),q.units().standardString());
//}

//TEST_F(ModelFixture,GroundHeatExchangerVertical_BoreHoleRadius_Quantity) {
//  Model model;
//  // TODO: Check constructor.
//  GroundHeatExchangerVertical groundHeatExchangerVertical(model);
//
//  Unit units = groundHeatExchangerVertical.getBoreHoleRadius(true).units(); // Get IP units.
//  // TODO: Check that value is appropriate (within bounds)
//  double value(1.0);
//  Quantity testQ(value,units);
//  EXPECT_TRUE(groundHeatExchangerVertical.setBoreHoleRadius(testQ));
//  OSOptionalQuantity q = groundHeatExchangerVertical.getBoreHoleRadius(true);
//  ASSERT_TRUE(q.isSet());
//  EXPECT_NEAR(value,q.get().value(),1.0E-8);
//  EXPECT_EQ(units.standardString(),q.units().standardString());
//}

//TEST_F(ModelFixture,GroundHeatExchangerVertical_GroundThermalConductivity_Quantity) {
//  Model model;
//  // TODO: Check constructor.
//  GroundHeatExchangerVertical groundHeatExchangerVertical(model);
//
//  Unit units = groundHeatExchangerVertical.getGroundThermalConductivity(true).units(); // Get IP units.
//  // TODO: Check that value is appropriate (within bounds)
//  double value(1.0);
//  Quantity testQ(value,units);
//  EXPECT_TRUE(groundHeatExchangerVertical.setGroundThermalConductivity(testQ));
//  OSOptionalQuantity q = groundHeatExchangerVertical.getGroundThermalConductivity(true);
//  ASSERT_TRUE(q.isSet());
//  EXPECT_NEAR(value,q.get().value(),1.0E-8);
//  EXPECT_EQ(units.standardString(),q.units().standardString());
//}

//TEST_F(ModelFixture,GroundHeatExchangerVertical_GroundThermalHeatCapacity_Quantity) {
//  Model model;
//  // TODO: Check constructor.
//  GroundHeatExchangerVertical groundHeatExchangerVertical(model);
//
//  Unit units = groundHeatExchangerVertical.getGroundThermalHeatCapacity(true).units(); // Get IP units.
//  // TODO: Check that value is appropriate (within bounds)
//  double value(1.0);
//  Quantity testQ(value,units);
//  EXPECT_TRUE(groundHeatExchangerVertical.setGroundThermalHeatCapacity(testQ));
//  OSOptionalQuantity q = groundHeatExchangerVertical.getGroundThermalHeatCapacity(true);
//  ASSERT_TRUE(q.isSet());
//  EXPECT_NEAR(value,q.get().value(),1.0E-8);
//  EXPECT_EQ(units.standardString(),q.units().standardString());
//}

//TEST_F(ModelFixture,GroundHeatExchangerVertical_GroundTemperature_Quantity) {
//  Model model;
//  // TODO: Check constructor.
//  GroundHeatExchangerVertical groundHeatExchangerVertical(model);
//
//  Unit units = groundHeatExchangerVertical.getGroundTemperature(true).units(); // Get IP units.
//  // TODO: Check that value is appropriate (within bounds)
//  double value(1.0);
//  Quantity testQ(value,units);
//  EXPECT_TRUE(groundHeatExchangerVertical.setGroundTemperature(testQ));
//  OSOptionalQuantity q = groundHeatExchangerVertical.getGroundTemperature(true);
//  ASSERT_TRUE(q.isSet());
//  EXPECT_NEAR(value,q.get().value(),1.0E-8);
//  EXPECT_EQ(units.standardString(),q.units().standardString());
//}

//TEST_F(ModelFixture,GroundHeatExchangerVertical_DesignFlowRate_Quantity) {
//  Model model;
//  // TODO: Check constructor.
//  GroundHeatExchangerVertical groundHeatExchangerVertical(model);
//
//  Unit units = groundHeatExchangerVertical.getDesignFlowRate(true).units(); // Get IP units.
//  // TODO: Check that value is appropriate (within bounds)
//  double value(1.0);
//  Quantity testQ(value,units);
//  EXPECT_TRUE(groundHeatExchangerVertical.setDesignFlowRate(testQ));
//  OSOptionalQuantity q = groundHeatExchangerVertical.getDesignFlowRate(true);
//  ASSERT_TRUE(q.isSet());
//  EXPECT_NEAR(value,q.get().value(),1.0E-8);
//  EXPECT_EQ(units.standardString(),q.units().standardString());
//}

//TEST_F(ModelFixture,GroundHeatExchangerVertical_GroutThermalConductivity_Quantity) {
//  Model model;
//  // TODO: Check constructor.
//  GroundHeatExchangerVertical groundHeatExchangerVertical(model);
//
//  Unit units = groundHeatExchangerVertical.getGroutThermalConductivity(true).units(); // Get IP units.
//  // TODO: Check that value is appropriate (within bounds)
//  double value(1.0);
//  Quantity testQ(value,units);
//  EXPECT_TRUE(groundHeatExchangerVertical.setGroutThermalConductivity(testQ));
//  OSOptionalQuantity q = groundHeatExchangerVertical.getGroutThermalConductivity(true);
//  ASSERT_TRUE(q.isSet());
//  EXPECT_NEAR(value,q.get().value(),1.0E-8);
//  EXPECT_EQ(units.standardString(),q.units().standardString());
//}

//TEST_F(ModelFixture,GroundHeatExchangerVertical_PipeThermalConductivity_Quantity) {
//  Model model;
//  // TODO: Check constructor.
//  GroundHeatExchangerVertical groundHeatExchangerVertical(model);
//
//  Unit units = groundHeatExchangerVertical.getPipeThermalConductivity(true).units(); // Get IP units.
//  // TODO: Check that value is appropriate (within bounds)
//  double value(1.0);
//  Quantity testQ(value,units);
//  EXPECT_TRUE(groundHeatExchangerVertical.setPipeThermalConductivity(testQ));
//  OSOptionalQuantity q = groundHeatExchangerVertical.getPipeThermalConductivity(true);
//  ASSERT_TRUE(q.isSet());
//  EXPECT_NEAR(value,q.get().value(),1.0E-8);
//  EXPECT_EQ(units.standardString(),q.units().standardString());
//}

//TEST_F(ModelFixture,GroundHeatExchangerVertical_PipeOutDiameter_Quantity) {
//  Model model;
//  // TODO: Check constructor.
//  GroundHeatExchangerVertical groundHeatExchangerVertical(model);
//
//  Unit units = groundHeatExchangerVertical.getPipeOutDiameter(true).units(); // Get IP units.
//  // TODO: Check that value is appropriate (within bounds)
//  double value(1.0);
//  Quantity testQ(value,units);
//  EXPECT_TRUE(groundHeatExchangerVertical.setPipeOutDiameter(testQ));
//  OSOptionalQuantity q = groundHeatExchangerVertical.getPipeOutDiameter(true);
//  ASSERT_TRUE(q.isSet());
//  EXPECT_NEAR(value,q.get().value(),1.0E-8);
//  EXPECT_EQ(units.standardString(),q.units().standardString());
//}

//TEST_F(ModelFixture,GroundHeatExchangerVertical_UTubeDistance_Quantity) {
//  Model model;
//  // TODO: Check constructor.
//  GroundHeatExchangerVertical groundHeatExchangerVertical(model);
//
//  Unit units = groundHeatExchangerVertical.getUTubeDistance(true).units(); // Get IP units.
//  // TODO: Check that value is appropriate (within bounds)
//  double value(1.0);
//  Quantity testQ(value,units);
//  EXPECT_TRUE(groundHeatExchangerVertical.setUTubeDistance(testQ));
//  OSOptionalQuantity q = groundHeatExchangerVertical.getUTubeDistance(true);
//  ASSERT_TRUE(q.isSet());
//  EXPECT_NEAR(value,q.get().value(),1.0E-8);
//  EXPECT_EQ(units.standardString(),q.units().standardString());
//}

//TEST_F(ModelFixture,GroundHeatExchangerVertical_PipeThickness_Quantity) {
//  Model model;
//  // TODO: Check constructor.
//  GroundHeatExchangerVertical groundHeatExchangerVertical(model);
//
//  Unit units = groundHeatExchangerVertical.getPipeThickness(true).units(); // Get IP units.
//  // TODO: Check that value is appropriate (within bounds)
//  double value(1.0);
//  Quantity testQ(value,units);
//  EXPECT_TRUE(groundHeatExchangerVertical.setPipeThickness(testQ));
//  OSOptionalQuantity q = groundHeatExchangerVertical.getPipeThickness(true);
//  ASSERT_TRUE(q.isSet());
//  EXPECT_NEAR(value,q.get().value(),1.0E-8);
//  EXPECT_EQ(units.standardString(),q.units().standardString());
//}

//TEST_F(ModelFixture,GroundHeatExchangerVertical_MaximumLengthofSimulation_Quantity) {
//  Model model;
//  // TODO: Check constructor.
//  GroundHeatExchangerVertical groundHeatExchangerVertical(model);
//
//  Unit units = groundHeatExchangerVertical.getMaximumLengthofSimulation(true).units(); // Get IP units.
//  // TODO: Check that value is appropriate (within bounds)
//  double value(1.0);
//  Quantity testQ(value,units);
//  EXPECT_TRUE(groundHeatExchangerVertical.setMaximumLengthofSimulation(testQ));
//  OSOptionalQuantity q = groundHeatExchangerVertical.getMaximumLengthofSimulation(true);
//  ASSERT_TRUE(q.isSet());
//  EXPECT_NEAR(value,q.get().value(),1.0E-8);
//  EXPECT_EQ(units.standardString(),q.units().standardString());
//}

//TEST_F(ModelFixture,GroundHeatExchangerVertical_GFunctionReferenceRatio_Quantity) {
//  Model model;
//  // TODO: Check constructor.
//  GroundHeatExchangerVertical groundHeatExchangerVertical(model);
//
//  Unit units = groundHeatExchangerVertical.getGFunctionReferenceRatio(true).units(); // Get IP units.
//  // TODO: Check that value is appropriate (within bounds)
// double value(1.0);
//  Quantity testQ(value,units);
//  EXPECT_TRUE(groundHeatExchangerVertical.setGFunctionReferenceRatio(testQ));
//  Quantity q = groundHeatExchangerVertical.getGFunctionReferenceRatio(true);
//  EXPECT_NEAR(value,q.value(),1.0E-8);
//  EXPECT_EQ(units.standardString(),q.units().standardString());
//}

