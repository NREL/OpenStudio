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
#include "../PumpConstantSpeed.hpp"
#include "../PumpConstantSpeed_Impl.hpp"
#include "../AirLoopHVAC.hpp"
#include "../PlantLoop.hpp"
#include "../Node.hpp"
#include "../Node_Impl.hpp"
#include "../AirLoopHVACZoneSplitter.hpp"
#include "../../utilities/units/Quantity.hpp"
#include "../../utilities/units/Unit.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture,PumpConstantSpeed_PumpConstantSpeed)
{
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  ASSERT_EXIT ( 
  {  
     Model m; 
     PumpConstantSpeed pump(m); 

     exit(0); 
  } ,
    ::testing::ExitedWithCode(0), "" );
}

TEST_F(ModelFixture,PumpConstantSpeed_addToNode) {
  Model m;
  PumpConstantSpeed testObject(m);

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

  PumpConstantSpeed testObject2(m);

  EXPECT_TRUE(testObject2.addToNode(demandOutletNode));
  EXPECT_EQ( (unsigned)7, plantLoop.demandComponents().size() );

  PlantLoop plantLoop2(m);
  demandOutletNode = plantLoop2.demandOutletNode();
  EXPECT_FALSE(testObject.addToNode(demandOutletNode));
  EXPECT_EQ( (unsigned)5, plantLoop2.demandComponents().size() );

  PumpConstantSpeed testObjectClone = testObject.clone(m).cast<PumpConstantSpeed>();
  supplyOutletNode = plantLoop.supplyOutletNode();

  EXPECT_TRUE(testObjectClone.addToNode(supplyOutletNode));
  EXPECT_EQ( (unsigned)9, plantLoop.supplyComponents().size() );
}

TEST_F(ModelFixture,PumpConstantSpeed_RatedFlowRate_Quantity) {
  Model model;
  // TODO: Check constructor.
  PumpConstantSpeed pumpConstantSpeed(model);

  Unit units = pumpConstantSpeed.getRatedFlowRate(true).units(); // Get IP units.
  // TODO: Check that value is appropriate (within bounds)
  double value(1.0);
  Quantity testQ(value,units);
  EXPECT_TRUE(pumpConstantSpeed.setRatedFlowRate(testQ));
  OSOptionalQuantity q = pumpConstantSpeed.getRatedFlowRate(true);
  ASSERT_TRUE(q.isSet());
  EXPECT_NEAR(value,q.get().value(),1.0E-8);
  EXPECT_EQ(units.standardString(),q.units().standardString());
}

TEST_F(ModelFixture,PumpConstantSpeed_RatedPumpHead_Quantity) {
  Model model;
  // TODO: Check constructor.
  PumpConstantSpeed pumpConstantSpeed(model);

  Unit units = pumpConstantSpeed.getRatedPumpHead(true).units(); // Get IP units.
  // TODO: Check that value is appropriate (within bounds)
  double value(1.0);
  Quantity testQ(value,units);
  EXPECT_TRUE(pumpConstantSpeed.setRatedPumpHead(testQ));
  Quantity q = pumpConstantSpeed.getRatedPumpHead(true);
  EXPECT_NEAR(value,q.value(),1.0E-8);
  EXPECT_EQ(units.standardString(),q.units().standardString());
}

TEST_F(ModelFixture,PumpConstantSpeed_RatedPowerConsumption_Quantity) {
  Model model;
  // TODO: Check constructor.
  PumpConstantSpeed pumpConstantSpeed(model);

  Unit units = pumpConstantSpeed.getRatedPowerConsumption(true).units(); // Get IP units.
  // TODO: Check that value is appropriate (within bounds)
  double value(1.0);
  Quantity testQ(value,units);
  EXPECT_TRUE(pumpConstantSpeed.setRatedPowerConsumption(testQ));
  OSOptionalQuantity q = pumpConstantSpeed.getRatedPowerConsumption(true);
  ASSERT_TRUE(q.isSet());
  EXPECT_NEAR(value,q.get().value(),1.0E-8);
  EXPECT_EQ(units.standardString(),q.units().standardString());
}

TEST_F(ModelFixture,PumpConstantSpeed_MotorEfficiency_Quantity) {
  Model model;
  // TODO: Check constructor.
  PumpConstantSpeed pumpConstantSpeed(model);

  Unit units = pumpConstantSpeed.getMotorEfficiency(true).units(); // Get IP units.
  // TODO: Check that value is appropriate (within bounds)
  double value(1.0);
  Quantity testQ(value,units);
  EXPECT_TRUE(pumpConstantSpeed.setMotorEfficiency(testQ));
  Quantity q = pumpConstantSpeed.getMotorEfficiency(true);
  EXPECT_NEAR(value,q.value(),1.0E-8);
  EXPECT_EQ(units.standardString(),q.units().standardString());
}

TEST_F(ModelFixture,PumpConstantSpeed_FractionofMotorInefficienciestoFluidStream_Quantity) {
  Model model;
  // TODO: Check constructor.
  PumpConstantSpeed pumpConstantSpeed(model);

  Unit units = pumpConstantSpeed.getFractionofMotorInefficienciestoFluidStream(true).units(); // Get IP units.
  // TODO: Check that value is appropriate (within bounds)
  double value(1.0);
  Quantity testQ(value,units);
  EXPECT_TRUE(pumpConstantSpeed.setFractionofMotorInefficienciestoFluidStream(testQ));
  Quantity q = pumpConstantSpeed.getFractionofMotorInefficienciestoFluidStream(true);
  EXPECT_NEAR(value,q.value(),1.0E-8);
  EXPECT_EQ(units.standardString(),q.units().standardString());
}

TEST_F(ModelFixture,PumpConstantSpeed_ImpellerDiameter_Quantity) {
  Model model;
  // TODO: Check constructor.
  PumpConstantSpeed pumpConstantSpeed(model);

  Unit units = pumpConstantSpeed.getImpellerDiameter(true).units(); // Get IP units.
  // TODO: Check that value is appropriate (within bounds)
  double value(1.0);
  Quantity testQ(value,units);
  EXPECT_TRUE(pumpConstantSpeed.setImpellerDiameter(testQ));
  OSOptionalQuantity q = pumpConstantSpeed.getImpellerDiameter(true);
  ASSERT_TRUE(q.isSet());
  EXPECT_NEAR(value,q.get().value(),1.0E-8);
  EXPECT_EQ(units.standardString(),q.units().standardString());
}

TEST_F(ModelFixture,PumpConstantSpeed_RotationalSpeed_Quantity) {
  Model model;
  // TODO: Check constructor.
  PumpConstantSpeed pumpConstantSpeed(model);

  Unit units = pumpConstantSpeed.getRotationalSpeed(true).units(); // Get IP units.
  // TODO: Check that value is appropriate (within bounds)
  double value(1.0);
  Quantity testQ(value,units);
  EXPECT_TRUE(pumpConstantSpeed.setRotationalSpeed(testQ));
  OSOptionalQuantity q = pumpConstantSpeed.getRotationalSpeed(true);
  ASSERT_TRUE(q.isSet());
  EXPECT_NEAR(value,q.get().value(),1.0E-8);
  EXPECT_EQ(units.standardString(),q.units().standardString());
}

TEST_F(ModelFixture,PumpConstantSpeed_SkinLossRadiativeFraction_Quantity) {
  Model model;
  // TODO: Check constructor.
  PumpConstantSpeed pumpConstantSpeed(model);

  Unit units = pumpConstantSpeed.getSkinLossRadiativeFraction(true).units(); // Get IP units.
  // TODO: Check that value is appropriate (within bounds)
  double value(1.0);
  Quantity testQ(value,units);
  EXPECT_TRUE(pumpConstantSpeed.setSkinLossRadiativeFraction(testQ));
  OSOptionalQuantity q = pumpConstantSpeed.getSkinLossRadiativeFraction(true);
  ASSERT_TRUE(q.isSet());
  EXPECT_NEAR(value,q.get().value(),1.0E-8);
  EXPECT_EQ(units.standardString(),q.units().standardString());
}

