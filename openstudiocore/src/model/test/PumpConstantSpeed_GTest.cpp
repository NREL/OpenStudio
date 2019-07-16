/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2019, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
*  following conditions are met:
*
*  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
*  disclaimer.
*
*  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
*  disclaimer in the documentation and/or other materials provided with the distribution.
*
*  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote products
*  derived from this software without specific prior written permission from the respective party.
*
*  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative works
*  may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without specific prior
*  written permission from Alliance for Sustainable Energy, LLC.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER(S) AND ANY CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
*  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
*  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER(S), ANY CONTRIBUTORS, THE UNITED STATES GOVERNMENT, OR THE UNITED
*  STATES DEPARTMENT OF ENERGY, NOR ANY OF THEIR EMPLOYEES, BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
*  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
*  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
*  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
*  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
***********************************************************************************************************************/

#include <gtest/gtest.h>
#include "ModelFixture.hpp"
#include "../PumpConstantSpeed.hpp"
#include "../PumpConstantSpeed_Impl.hpp"
#include "../AirLoopHVAC.hpp"
#include "../PlantLoop.hpp"
#include "../Node.hpp"
#include "../Node_Impl.hpp"
#include "../Schedule.hpp"
#include "../Schedule_Impl.hpp"
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

TEST_F(ModelFixture,PumpConstantSpeed_flowRateSchedule) {
  Model m;
  PumpConstantSpeed pump(m);
  auto alwaysOnSchedule = m.alwaysOnDiscreteSchedule();
  EXPECT_TRUE(pump.setPumpFlowRateSchedule(alwaysOnSchedule));
  auto s = pump.pumpFlowRateSchedule();
  EXPECT_TRUE(s);
  EXPECT_EQ(alwaysOnSchedule,s.get());
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
  EXPECT_TRUE(testObject.addToNode(demandOutletNode));
  EXPECT_EQ( (unsigned)7, plantLoop.demandComponents().size() );

  PumpConstantSpeed testObject2(m);

  EXPECT_TRUE(testObject2.addToNode(demandOutletNode));
  EXPECT_EQ( (unsigned)9, plantLoop.demandComponents().size() );

  PlantLoop plantLoop2(m);
  demandOutletNode = plantLoop2.demandOutletNode();
  EXPECT_TRUE(testObject.addToNode(demandOutletNode));
  EXPECT_EQ( (unsigned)7, plantLoop2.demandComponents().size() );

  PumpConstantSpeed testObjectClone = testObject.clone(m).cast<PumpConstantSpeed>();
  supplyOutletNode = plantLoop.supplyOutletNode();

  EXPECT_TRUE(testObjectClone.addToNode(supplyOutletNode));
  EXPECT_EQ( (unsigned)7, plantLoop.supplyComponents().size() );
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

