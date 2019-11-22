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
#include "../HeatExchangerAirToAirSensibleAndLatent.hpp"
#include "../HeatExchangerAirToAirSensibleAndLatent_Impl.hpp"
#include "../AirLoopHVAC.hpp"
#include "../AirLoopHVAC_Impl.hpp"
#include "../Schedule.hpp"
#include "../Schedule_Impl.hpp"
#include "../Node.hpp"
#include "../Node_Impl.hpp"
#include "../AirLoopHVACOutdoorAirSystem.hpp"
#include "../AirLoopHVACOutdoorAirSystem_Impl.hpp"
#include "../EvaporativeCoolerDirectResearchSpecial.hpp"
#include "../EvaporativeCoolerDirectResearchSpecial_Impl.hpp"
#include "../HVACTemplates.hpp"
#include "../../utilities/units/Quantity.hpp"
#include "../../utilities/units/Unit.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture,HeatExchangerAirToAirSensibleAndLatent_addToNode) {
  Model model;
  HeatExchangerAirToAirSensibleAndLatent heatExchangerAirToAirSensibleAndLatent(model);

  AirLoopHVAC loop = addSystemType3(model).cast<AirLoopHVAC>();

  AirLoopHVACOutdoorAirSystem oaSystem = loop.airLoopHVACOutdoorAirSystem().get();

  Node oaNode = oaSystem.outboardOANode().get();
  Node reliefNode = oaSystem.outboardReliefNode().get();

  HeatExchangerAirToAirSensibleAndLatent hx1(model);
  EXPECT_TRUE(hx1.addToNode(oaNode));

  HeatExchangerAirToAirSensibleAndLatent hx2(model);
  EXPECT_TRUE(hx2.addToNode(oaNode));

  HeatExchangerAirToAirSensibleAndLatent hx3(model);
  EXPECT_TRUE(hx3.addToNode(reliefNode));

  oaNode = oaSystem.outdoorAirModelObject()->cast<Node>();
  reliefNode = oaSystem.reliefAirModelObject()->cast<Node>();

  HeatExchangerAirToAirSensibleAndLatent hx4(model);
  EXPECT_TRUE(hx4.addToNode(reliefNode));

  HeatExchangerAirToAirSensibleAndLatent hx5(model);
  EXPECT_TRUE(hx5.addToNode(oaNode));

  Schedule schedule = model.alwaysOnDiscreteSchedule();
  EvaporativeCoolerDirectResearchSpecial evap(model,schedule);
  EXPECT_TRUE(evap.addToNode(oaNode));
  Node inletNode = evap.inletModelObject()->cast<Node>();
  Node outletNode = evap.outletModelObject()->cast<Node>();

  HeatExchangerAirToAirSensibleAndLatent hx6(model);
  EXPECT_TRUE(hx6.addToNode(inletNode));

  HeatExchangerAirToAirSensibleAndLatent hx7(model);
  EXPECT_TRUE(hx7.addToNode(outletNode));

  hx1.remove();
  hx2.remove();
  hx3.remove();
  hx4.remove();
  hx5.remove();
  hx6.remove();
  hx7.remove();
  evap.remove();

  EXPECT_EQ(1u,oaSystem.oaComponents().size());
  EXPECT_EQ(1u,oaSystem.reliefComponents().size());
}

TEST_F(ModelFixture,HeatExchangerAirToAirSensibleAndLatent_remove) {
  Model model;
  HeatExchangerAirToAirSensibleAndLatent heatExchangerAirToAirSensibleAndLatent(model);

  AirLoopHVAC loop = addSystemType3(model).cast<AirLoopHVAC>();

  AirLoopHVACOutdoorAirSystem oaSystem = loop.airLoopHVACOutdoorAirSystem().get();

  Node oaNode = oaSystem.outboardOANode().get();
  Node reliefNode = oaSystem.outboardReliefNode().get();

  HeatExchangerAirToAirSensibleAndLatent hx1(model);
  EXPECT_TRUE(hx1.addToNode(oaNode));

  HeatExchangerAirToAirSensibleAndLatent hx2(model);
  EXPECT_TRUE(hx2.addToNode(oaNode));

  HeatExchangerAirToAirSensibleAndLatent hx3(model);
  EXPECT_TRUE(hx3.addToNode(reliefNode));

  oaNode = oaSystem.outdoorAirModelObject()->cast<Node>();
  reliefNode = oaSystem.reliefAirModelObject()->cast<Node>();

  HeatExchangerAirToAirSensibleAndLatent hx4(model);
  EXPECT_TRUE(hx4.addToNode(reliefNode));

  HeatExchangerAirToAirSensibleAndLatent hx5(model);
  EXPECT_TRUE(hx5.addToNode(oaNode));

  Schedule schedule = model.alwaysOnDiscreteSchedule();
  EvaporativeCoolerDirectResearchSpecial evap(model,schedule);
  EXPECT_TRUE(evap.addToNode(oaNode));
  Node inletNode = evap.inletModelObject()->cast<Node>();
  Node outletNode = evap.outletModelObject()->cast<Node>();

  HeatExchangerAirToAirSensibleAndLatent hx6(model);
  EXPECT_TRUE(hx6.addToNode(inletNode));

  HeatExchangerAirToAirSensibleAndLatent hx7(model);
  EXPECT_TRUE(hx7.addToNode(outletNode));

  EXPECT_TRUE(loop.remove().size() > 0);
}

TEST_F(ModelFixture,HeatExchangerAirToAirSensibleAndLatent_NominalSupplyAirFlowRate_Quantity) {
  Model model;
  HeatExchangerAirToAirSensibleAndLatent heatExchangerAirToAirSensibleAndLatent(model);

  Unit units = heatExchangerAirToAirSensibleAndLatent.getNominalSupplyAirFlowRate(true).units(); // Get IP units.
  double value(1.0);
  Quantity testQ(value,units);
  EXPECT_TRUE(heatExchangerAirToAirSensibleAndLatent.setNominalSupplyAirFlowRate(testQ));
  OSOptionalQuantity q = heatExchangerAirToAirSensibleAndLatent.getNominalSupplyAirFlowRate(true);
  ASSERT_TRUE(q.isSet());
  EXPECT_NEAR(value,q.get().value(),1.0E-8);
  EXPECT_EQ(units.standardString(),q.units().standardString());
}

TEST_F(ModelFixture,HeatExchangerAirToAirSensibleAndLatent_SensibleEffectivenessat100HeatingAirFlow_Quantity) {
  Model model;
  HeatExchangerAirToAirSensibleAndLatent heatExchangerAirToAirSensibleAndLatent(model);

  Unit units = heatExchangerAirToAirSensibleAndLatent.getSensibleEffectivenessat100HeatingAirFlow(true).units(); // Get IP units.
  double value(1.0);
  Quantity testQ(value,units);
  EXPECT_TRUE(heatExchangerAirToAirSensibleAndLatent.setSensibleEffectivenessat100HeatingAirFlow(testQ));
  Quantity q = heatExchangerAirToAirSensibleAndLatent.getSensibleEffectivenessat100HeatingAirFlow(true);
  EXPECT_NEAR(value,q.value(),1.0E-8);
  EXPECT_EQ(units.standardString(),q.units().standardString());
}

TEST_F(ModelFixture,HeatExchangerAirToAirSensibleAndLatent_LatentEffectivenessat100HeatingAirFlow_Quantity) {
  Model model;
  HeatExchangerAirToAirSensibleAndLatent heatExchangerAirToAirSensibleAndLatent(model);

  Unit units = heatExchangerAirToAirSensibleAndLatent.getLatentEffectivenessat100HeatingAirFlow(true).units(); // Get IP units.
  double value(1.0);
  Quantity testQ(value,units);
  EXPECT_TRUE(heatExchangerAirToAirSensibleAndLatent.setLatentEffectivenessat100HeatingAirFlow(testQ));
  Quantity q = heatExchangerAirToAirSensibleAndLatent.getLatentEffectivenessat100HeatingAirFlow(true);
  EXPECT_NEAR(value,q.value(),1.0E-8);
  EXPECT_EQ(units.standardString(),q.units().standardString());
}

TEST_F(ModelFixture,HeatExchangerAirToAirSensibleAndLatent_SensibleEffectivenessat75HeatingAirFlow_Quantity) {
  Model model;
  HeatExchangerAirToAirSensibleAndLatent heatExchangerAirToAirSensibleAndLatent(model);

  Unit units = heatExchangerAirToAirSensibleAndLatent.getSensibleEffectivenessat75HeatingAirFlow(true).units(); // Get IP units.
  double value(1.0);
  Quantity testQ(value,units);
  EXPECT_TRUE(heatExchangerAirToAirSensibleAndLatent.setSensibleEffectivenessat75HeatingAirFlow(testQ));
  Quantity q = heatExchangerAirToAirSensibleAndLatent.getSensibleEffectivenessat75HeatingAirFlow(true);
  EXPECT_NEAR(value,q.value(),1.0E-8);
  EXPECT_EQ(units.standardString(),q.units().standardString());
}

TEST_F(ModelFixture,HeatExchangerAirToAirSensibleAndLatent_LatentEffectivenessat75HeatingAirFlow_Quantity) {
  Model model;
  HeatExchangerAirToAirSensibleAndLatent heatExchangerAirToAirSensibleAndLatent(model);

  Unit units = heatExchangerAirToAirSensibleAndLatent.getLatentEffectivenessat75HeatingAirFlow(true).units(); // Get IP units.
  double value(1.0);
  Quantity testQ(value,units);
  EXPECT_TRUE(heatExchangerAirToAirSensibleAndLatent.setLatentEffectivenessat75HeatingAirFlow(testQ));
  Quantity q = heatExchangerAirToAirSensibleAndLatent.getLatentEffectivenessat75HeatingAirFlow(true);
  EXPECT_NEAR(value,q.value(),1.0E-8);
  EXPECT_EQ(units.standardString(),q.units().standardString());
}

TEST_F(ModelFixture,HeatExchangerAirToAirSensibleAndLatent_SensibleEffectivenessat100CoolingAirFlow_Quantity) {
  Model model;
  HeatExchangerAirToAirSensibleAndLatent heatExchangerAirToAirSensibleAndLatent(model);

  Unit units = heatExchangerAirToAirSensibleAndLatent.getSensibleEffectivenessat100CoolingAirFlow(true).units(); // Get IP units.
  double value(1.0);
  Quantity testQ(value,units);
  EXPECT_TRUE(heatExchangerAirToAirSensibleAndLatent.setSensibleEffectivenessat100CoolingAirFlow(testQ));
  Quantity q = heatExchangerAirToAirSensibleAndLatent.getSensibleEffectivenessat100CoolingAirFlow(true);
  EXPECT_NEAR(value,q.value(),1.0E-8);
  EXPECT_EQ(units.standardString(),q.units().standardString());
}

TEST_F(ModelFixture,HeatExchangerAirToAirSensibleAndLatent_LatentEffectivenessat100CoolingAirFlow_Quantity) {
  Model model;
  HeatExchangerAirToAirSensibleAndLatent heatExchangerAirToAirSensibleAndLatent(model);

  Unit units = heatExchangerAirToAirSensibleAndLatent.getLatentEffectivenessat100CoolingAirFlow(true).units(); // Get IP units.
  double value(1.0);
  Quantity testQ(value,units);
  EXPECT_TRUE(heatExchangerAirToAirSensibleAndLatent.setLatentEffectivenessat100CoolingAirFlow(testQ));
  Quantity q = heatExchangerAirToAirSensibleAndLatent.getLatentEffectivenessat100CoolingAirFlow(true);
  EXPECT_NEAR(value,q.value(),1.0E-8);
  EXPECT_EQ(units.standardString(),q.units().standardString());
}

TEST_F(ModelFixture,HeatExchangerAirToAirSensibleAndLatent_SensibleEffectivenessat75CoolingAirFlow_Quantity) {
  Model model;
  HeatExchangerAirToAirSensibleAndLatent heatExchangerAirToAirSensibleAndLatent(model);

  Unit units = heatExchangerAirToAirSensibleAndLatent.getSensibleEffectivenessat75CoolingAirFlow(true).units(); // Get IP units.
  double value(1.0);
  Quantity testQ(value,units);
  EXPECT_TRUE(heatExchangerAirToAirSensibleAndLatent.setSensibleEffectivenessat75CoolingAirFlow(testQ));
  Quantity q = heatExchangerAirToAirSensibleAndLatent.getSensibleEffectivenessat75CoolingAirFlow(true);
  EXPECT_NEAR(value,q.value(),1.0E-8);
  EXPECT_EQ(units.standardString(),q.units().standardString());
}

TEST_F(ModelFixture,HeatExchangerAirToAirSensibleAndLatent_LatentEffectivenessat75CoolingAirFlow_Quantity) {
  Model model;
  HeatExchangerAirToAirSensibleAndLatent heatExchangerAirToAirSensibleAndLatent(model);

  Unit units = heatExchangerAirToAirSensibleAndLatent.getLatentEffectivenessat75CoolingAirFlow(true).units(); // Get IP units.
  double value(1.0);
  Quantity testQ(value,units);
  EXPECT_TRUE(heatExchangerAirToAirSensibleAndLatent.setLatentEffectivenessat75CoolingAirFlow(testQ));
  Quantity q = heatExchangerAirToAirSensibleAndLatent.getLatentEffectivenessat75CoolingAirFlow(true);
  EXPECT_NEAR(value,q.value(),1.0E-8);
  EXPECT_EQ(units.standardString(),q.units().standardString());
}

TEST_F(ModelFixture,HeatExchangerAirToAirSensibleAndLatent_NominalElectricPower_Quantity) {
  Model model;
  HeatExchangerAirToAirSensibleAndLatent heatExchangerAirToAirSensibleAndLatent(model);

  Unit units = heatExchangerAirToAirSensibleAndLatent.getNominalElectricPower(true).units(); // Get IP units.
  double value(1.0);
  Quantity testQ(value,units);
  EXPECT_TRUE(heatExchangerAirToAirSensibleAndLatent.setNominalElectricPower(testQ));
  Quantity q = heatExchangerAirToAirSensibleAndLatent.getNominalElectricPower(true);
  EXPECT_NEAR(value,q.value(),1.0E-8);
  EXPECT_EQ(units.standardString(),q.units().standardString());
}

TEST_F(ModelFixture,HeatExchangerAirToAirSensibleAndLatent_ThresholdTemperature_Quantity) {
  Model model;
  HeatExchangerAirToAirSensibleAndLatent heatExchangerAirToAirSensibleAndLatent(model);

  Unit units = heatExchangerAirToAirSensibleAndLatent.getThresholdTemperature(true).units(); // Get IP units.
  double value(1.0);
  Quantity testQ(value,units);
  EXPECT_TRUE(heatExchangerAirToAirSensibleAndLatent.setThresholdTemperature(testQ));
  Quantity q = heatExchangerAirToAirSensibleAndLatent.getThresholdTemperature(true);
  EXPECT_NEAR(value,q.value(),1.0E-8);
  EXPECT_EQ(units.standardString(),q.units().standardString());
}

TEST_F(ModelFixture,HeatExchangerAirToAirSensibleAndLatent_InitialDefrostTimeFraction_Quantity) {
  Model model;
  HeatExchangerAirToAirSensibleAndLatent heatExchangerAirToAirSensibleAndLatent(model);

  Unit units = heatExchangerAirToAirSensibleAndLatent.getInitialDefrostTimeFraction(true).units(); // Get IP units.
  double value(1.0);
  Quantity testQ(value,units);
  EXPECT_TRUE(heatExchangerAirToAirSensibleAndLatent.setInitialDefrostTimeFraction(testQ));
  OSOptionalQuantity q = heatExchangerAirToAirSensibleAndLatent.getInitialDefrostTimeFraction(true);
  ASSERT_TRUE(q.isSet());
  EXPECT_NEAR(value,q.get().value(),1.0E-8);
  EXPECT_EQ(units.standardString(),q.units().standardString());
}

TEST_F(ModelFixture,HeatExchangerAirToAirSensibleAndLatent_RateofDefrostTimeFractionIncrease_Quantity) {
  Model model;
  HeatExchangerAirToAirSensibleAndLatent heatExchangerAirToAirSensibleAndLatent(model);

  Unit units = heatExchangerAirToAirSensibleAndLatent.getRateofDefrostTimeFractionIncrease(true).units(); // Get IP units.
  double value(1.0);
  Quantity testQ(value,units);
  EXPECT_TRUE(heatExchangerAirToAirSensibleAndLatent.setRateofDefrostTimeFractionIncrease(testQ));
  OSOptionalQuantity q = heatExchangerAirToAirSensibleAndLatent.getRateofDefrostTimeFractionIncrease(true);
  ASSERT_TRUE(q.isSet());
  EXPECT_NEAR(value,q.get().value(),1.0E-8);
  EXPECT_EQ(units.standardString(),q.units().standardString());
}

