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

